#include "AbstractScene.h"

AbstractScene::AbstractScene()
{
}

AbstractScene::~AbstractScene(void)
{
}

void AbstractScene::createScene(void)
{
}

Ogre::SceneManager* AbstractScene::getSceneManager(void)
{
	return mSceneMgr;
}

void AbstractScene::createSceneCommon(void)
{
	// create the camera
	mCamera = mSceneMgr->createCamera(PLAYER_CAM_NAME);
	mCamera->setNearClipDistance(1);

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::Entity* playerEntity = mSceneMgr->createEntity(PLAYER_MESH_NAME, "man.mesh");
	playerEntity->setCastShadows(true);

	Ogre::SceneNode* playerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(PLAYER_NODE_NAME);
	playerNode->attachObject(playerEntity);

	Ogre::SceneNode* camYawNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(CAMERA_YAW_NODE_NAME, Ogre::Vector3(0, 0, 0));

	Ogre::SceneNode* camPitchNode = camYawNode->createChildSceneNode(CAMERA_PITCH_NODE_NAME, Ogre::Vector3(0, 0, 0));

	Ogre::SceneNode* camNode = camPitchNode->createChildSceneNode(CAMERA_NODE_NAME, Ogre::Vector3(0, 200, 200));
	camNode->pitch(Ogre::Degree(-27));
	camNode->attachObject(mCamera);

}