#include "ExampleScene.h"
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>

ExampleScene::ExampleScene(Ogre::Root* root, Ogre::RenderWindow* window)
{
	mRoot = root;
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	mWindow = window;
}

ExampleScene::~ExampleScene(void)
{
}

void ExampleScene::createScene(void)
{
	// create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setNearClipDistance(5);

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::Entity* ogreHead = mSceneMgr->createEntity("Woman", "woman.mesh");

	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	headNode->attachObject(ogreHead);

	Ogre::SceneNode* camNode = headNode->createChildSceneNode("CamNode", Ogre::Vector3(0, 200, 200));
	//mCamera->lookAt(headNode->getPosition());
	//camNode->yaw(Ogre::Degree(-45));
	camNode->pitch(Ogre::Degree(-30));
	camNode->attachObject(mCamera);

	Ogre::Plane ground(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ground, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Textures/MRAMOR");
	entGround->setCastShadows(false);

	Ogre::Plane leftWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("leftWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, leftWall, 1500, 500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entLeftWall = mSceneMgr->createEntity("leftWallEntity", "leftWall");
	Ogre::SceneNode* node =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entLeftWall);
	node->translate(Ogre::Vector3(-750, 0, 0));
	entLeftWall->setMaterialName("Textures/Wood");
	entLeftWall->setCastShadows(false);

	Ogre::Plane rightWall(-Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rightWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rightWall, 1500, 500, 20, 20, true, 1, 5, 5, -Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRightWall = mSceneMgr->createEntity("rightWallEntity", "rightWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRightWall);
	node->translate(Ogre::Vector3(750, 0, 0));
	entRightWall->setMaterialName("Textures/Wood");
	entRightWall->setCastShadows(false);

	Ogre::Plane frontWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("frontWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, frontWall, 1500, 500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entFrontWall = mSceneMgr->createEntity("frontWallEntity", "frontWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entFrontWall);
	node->yaw(Ogre::Degree(-90));
	node->translate(Ogre::Vector3(0, 0, -750));
	entFrontWall->setMaterialName("Textures/Wood");
	entFrontWall->setCastShadows(false);

	Ogre::Plane rearWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rearWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rearWall, 1500, 500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRearWall = mSceneMgr->createEntity("rearWallEntity", "rearWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRearWall);
	node->yaw(Ogre::Degree(90));
	node->translate(Ogre::Vector3(0, 0, 750));
	entRearWall->setMaterialName("Textures/Wood");
	entRearWall->setCastShadows(false);


	//Set ambient light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

}