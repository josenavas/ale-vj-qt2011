#include "ExampleScene.h"
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>

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

	// Poisition it at 80 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::Entity* ogreHead = mSceneMgr->createEntity("Woman", "woman.mesh");

	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerNode");
	headNode->attachObject(ogreHead);

	//Set ambient light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

}