#include "InputManager.h"
#include <OgreEntity.h>

InputManager::InputManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneManager)
{
	mWindow = window;
	mSceneMgr = sceneManager;

	mPlayerNode = sceneManager->getSceneNode("PlayerNode");
	mAnimationState = ((Ogre::Entity*)(mPlayerNode->getAttachedObject("Woman")))->getAnimationState("walk");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(false);

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

	mRotate = Ogre::Real(0.13);//0.13;
	mMove = 250;

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	mDirection = Ogre::Vector3::ZERO;

	mShutDown = false;
}

InputManager::~InputManager(void)
{
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

	mPlayerNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	mAnimationState->addTime(evt.timeSinceLastFrame);

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
		mDirection.z = -mMove;
		mAnimationState->setEnabled(true);
		break;
	case OIS::KC_S:
		mDirection.z = mMove;
		break;
	case OIS::KC_A:
		mDirection.x = -mMove;
		break;
	case OIS::KC_D:
		mDirection.x = mMove;
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
		mDirection.z = 0;
		mAnimationState->setEnabled(false);
		break;
	case OIS::KC_S:
		mDirection.z = 0;
		break;
	case OIS::KC_A:
		mDirection.x = 0;
		break;
	case OIS::KC_D:
		mDirection.x = 0;
		break;
	default:
		break;
	}
	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent& evt)
{
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}