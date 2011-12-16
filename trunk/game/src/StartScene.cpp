#include "StartScene.h"
#include "ExampleScene.h"
#include <OgreOverlayManager.h>

StartScene::StartScene(Ogre::Root* root, Ogre::RenderWindow* window)
{
	mRoot = root;
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	mWindow = window;
	mExit = false;
}

StartScene::~StartScene(void)
{
}

void StartScene::createScene(void)
{
	createSceneCommon();
	initObjectNames();

	//mOverlayItems->hide();
	//mOverlayObjName->hide();

	//mOverlay = Ogre::OverlayManager::getSingleton().getByName("Overlays/StartGame");
	//mOverlay->show();
}

void StartScene::initObjectNames(void)
{
	for(int i = 0; i < 10; i++) mObjectNames[i] = new Ogre::String("");
}

int StartScene::objectInteraction(Ogre::String name)
{
	mTime = 0;
	mExit = true;
	return OBJECT_ANIM_NONE;
}

AbstractScene * StartScene::getNextScene(void)
{
	return (AbstractScene*)new ExampleScene(mRoot, mWindow);
}