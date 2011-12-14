#include "InputManager.h"
#include <OgreQuaternion.h>
#include "Definitions.h"

InputManager::InputManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneManager)
{
	mWindow = window;
	mSceneMgr = sceneManager;

	mPlayerNode = sceneManager->getSceneNode(PLAYER_NODE_NAME);
	mPlayerEntity = (Ogre::Entity*)(mPlayerNode->getAttachedObject(PLAYER_MESH_NAME));
	mAnimationState = mPlayerEntity->getAnimationState("anim_idle");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);

	mCamPitchNode = sceneManager->getSceneNode(CAMERA_PITCH_NODE_NAME);
	mCamYawNode = sceneManager->getSceneNode(CAMERA_YAW_NODE_NAME);
	mCamNode = sceneManager->getSceneNode(CAMERA_NODE_NAME);

	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

	mRotate = Ogre::Real(0.13);
	mMove = 150;

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	mDirection = Ogre::Vector3::ZERO;

	mShutDown = false;
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());

	mState = eIDLE;
	mLeftShiftDown = false;
}

InputManager::~InputManager(void)
{
	mSceneMgr->destroyQuery(mRaySceneQuery);
}

void InputManager::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow* rw)
{
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

bool InputManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed()) return false;
	if(mShutDown) return false;

	mKeyboard->capture();
	mMouse->capture();

	switch(mState)
	{
	case eWALKING:
		orientatePlayer();
		if(!collisionControlZNeg())
		{
			mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			mCamYawNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		break;
	case eLEFT:
		orientatePlayer();
		if(!collisionControlXNeg())
		{
			mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			mCamYawNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		break;
	case eRIGHT:
		orientatePlayer();
		if(!collisionControlXPos())
		{
			mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			mCamYawNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		break;
	case ePICKUP:
	case eACTIVATE:
		if(mAnimationState->hasEnded())
		{
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_idle");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mState = eIDLE;
		}
		break;
	case eIDLE:
		break;
	default:
		break;
	}
	mAnimationState->addTime(evt.timeSinceLastFrame);
	changePointedObject();

	return true;
}

bool InputManager::keyPressed(const OIS::KeyEvent& evt)
{
	switch (evt.key)
	{
	case OIS::KC_ESCAPE:
		mShutDown = true;
		break;
	case OIS::KC_W:
		// El movimiento hacia delante siempre es respecto al adelante de la camara!!!
		if(mState == eIDLE)
		{
			mAnimationState->setEnabled(false);
			if(mLeftShiftDown)
			{
				mAnimationState = mPlayerEntity->getAnimationState("anim_run");
				mDirection.z = -mMove * 2;
			}
			else
			{
				mAnimationState = mPlayerEntity->getAnimationState("anim_walk");
				mDirection.z = -mMove;
			}
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mState = eWALKING;
		}
		break;
	case OIS::KC_A:
		if(mState == eIDLE)
		{
			mDirection.x = -mMove;
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_walkleft");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mState = eLEFT;
		}
		break;
	case OIS::KC_D:
		if(mState == eIDLE)
		{
			mDirection.x = mMove;
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_walkright");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mState = eRIGHT;
		}
		break;
	case OIS::KC_LSHIFT:
		mLeftShiftDown = true;
		if(eWALKING == mState)
		{
			mDirection.z = -mMove * 2;
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_run");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
		}
		break;
	default:
		break;
	}
	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent& evt)
{
	switch (evt.key)
	{
	case OIS::KC_W:
		if(eWALKING == mState)
		{
			mDirection.z = 0;
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_idle");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mState = eIDLE;
		}
		break;
	case OIS::KC_A:
		if(eLEFT == mState)
		{
			mDirection.x = 0;
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_idle");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mState = eIDLE;
		}
		break;
	case OIS::KC_D:
		if(eRIGHT == mState)
		{
			mDirection.x = 0;
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_idle");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mState = eIDLE;
		}
		break;
	case OIS::KC_LSHIFT:
		mLeftShiftDown = false;
		if(mState == eWALKING)
		{
			mDirection.z = -mMove;
			mAnimationState->setEnabled(false);
			mAnimationState = mPlayerEntity->getAnimationState("anim_walk");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
		}
		break;
	default:
		break;
	}
	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent& evt)
{
	mCamYawNode->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), Ogre::Node::TS_WORLD);
	if(evt.state.Y.rel < 0)
	{
		if(mCamNode->_getDerivedPosition().y > Ogre::Real(30)) mCamPitchNode->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);
	}
	else
	{
		if(mCamNode->_getDerivedPosition().y < Ogre::Real(260)) mCamPitchNode->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);
	}
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if(mState == eIDLE)
	{
		if( id == OIS::MB_Left)
		{
			switch(executeActive())
			{
			case OBJECT_ANIM_ACTIV:
				mAnimationState->setEnabled(false);
				mAnimationState = mPlayerEntity->getAnimationState("anim_activate");
				mAnimationState->setTimePosition(Ogre::Real(0));
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(false);
				mState = eACTIVATE;
				break;
			case OBJECT_ANIM_PICKUP:
				mAnimationState->setEnabled(false);
				mAnimationState = mPlayerEntity->getAnimationState("anim_pickup");
				mAnimationState->setTimePosition(Ogre::Real(0));
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(false);
				mState = ePICKUP;
				break;
			case OBJECT_ANIM_NONE:
			default:
				break;
			}
		}
	}
	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}

bool InputManager::collisionControlZNeg(void)
{
	Ogre::Vector3 orientation = mPlayerNode->getOrientation() * (-Ogre::Vector3::UNIT_Z);
	Ogre::Vector3 playerPos = mPlayerNode->getPosition();

	Ogre::Ray playerRay(playerPos, orientation);

	mRaySceneQuery->setRay(playerRay);

	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	Ogre::Real threshold = Ogre::Real(20);
	if(mLeftShiftDown) threshold = Ogre::Real(30);

	for(itr; itr != result.end(); itr++)
	{
		if (itr->movable->getName() != PLAYER_MESH_NAME)
		{
			if (itr->distance < threshold) return true;
			break;
		}
	}
	return false;
}

bool InputManager::collisionControlXPos(void)
{
	Ogre::Vector3 orientation = mPlayerNode->getOrientation() * (Ogre::Vector3::UNIT_X);

	Ogre::Vector3 playerPos = mPlayerNode->getPosition();
	Ogre::Ray playerRay(playerPos, orientation);

	mRaySceneQuery->setRay(playerRay);

	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	Ogre::Real threshold = Ogre::Real(20);

	for(itr; itr != result.end(); itr++)
	{
		if (itr->movable->getName() != PLAYER_MESH_NAME)
		{
			if (itr->distance < threshold) return true;
			break;
		}
	}
	return false;
}

bool InputManager::collisionControlXNeg(void)
{
	Ogre::Vector3 orientation = mPlayerNode->getOrientation() * (-Ogre::Vector3::UNIT_X);

	Ogre::Vector3 playerPos = mPlayerNode->getPosition();
	Ogre::Ray playerRay(playerPos, orientation);

	mRaySceneQuery->setRay(playerRay);

	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	Ogre::Real threshold = Ogre::Real(20);

	for(itr; itr != result.end(); itr++)
	{
		if (itr->movable->getName() != PLAYER_MESH_NAME)
		{
			if (itr->distance < threshold) return true;
			break;
		}
	}
	return false;
}

void InputManager::orientatePlayer(void)
{
	Ogre::Vector3 playerOrientation = mPlayerNode->getOrientation() * (-Ogre::Vector3::UNIT_Z);
	Ogre::Vector3 cameraOrientation = mCamYawNode->getOrientation() * (-Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion q = Ogre::Vector3(playerOrientation.x, 1, playerOrientation.z).getRotationTo(Ogre::Vector3(cameraOrientation.x, 1, cameraOrientation.z));
	mPlayerNode->yaw( q.getYaw());
}

int InputManager::executeActive(void)
{
	Ogre::MovableObject* obj = executeCamRay();
	if(obj == NULL) return OBJECT_ANIM_NONE;
	return mScene->objectInteraction(obj->getName());
}

void InputManager::changePointedObject(void)
{
	Ogre::MovableObject* obj = executeCamRay();
	if(NULL == obj)
	{
		mScene->setPointedObject(Ogre::String("Nothing"));
	}
	else
	{
		mScene->setPointedObject(obj->getName());
	}
}

Ogre::MovableObject* InputManager::executeCamRay(void)
{
	Ogre::Vector3 orientation = mCamNode->_getDerivedOrientation() * (-Ogre::Vector3::UNIT_Z);
	Ogre::Vector3 camPos = mCamNode->_getDerivedPosition();

	Ogre::Ray camRay(camPos, orientation);

	mRaySceneQuery->setRay(camRay);

	mRaySceneQuery->setSortByDistance(true);

	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	Ogre::Real threshold = Ogre::Real(400);

	for(itr; itr != result.end(); itr++)
	{
		if(itr->distance > Ogre::Real(150))
		{
			if (itr->movable->getName() != PLAYER_MESH_NAME)
			{
				if(itr->movable->getName().find("Wall") > itr->movable->getName().length())
				{
					if(itr->distance < threshold) return itr->movable;
					else
					{
						Ogre::MovableObject* obj = executePlayerRay();
						if(NULL == obj) return NULL;
						else if (!obj->getName().compare(itr->movable->getName()))
							if(itr->movable->getName().find("Wall") > itr->movable->getName().length()) return itr->movable;
					}
				}
				break;
			}
			else
			{
				threshold = itr->distance + Ogre::Real(65);
			}
		}
	}
	return NULL;
}

Ogre::MovableObject* InputManager::executePlayerRay(void)
{
	Ogre::Vector3 orientation = mPlayerNode->getOrientation() * (-Ogre::Vector3::UNIT_Z);
	Ogre::Vector3 playerPos = mPlayerNode->getPosition();

	Ogre::Ray playerRay(playerPos, orientation);

	mRaySceneQuery->setRay(playerRay);

	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	Ogre::Real threshold = Ogre::Real(65);

	for(itr; itr != result.end(); itr++)
	{
		if (itr->movable->getName() != PLAYER_MESH_NAME)
		{
			if (itr->distance < threshold) return itr->movable;
			break;
		}
	}
	return NULL;
}