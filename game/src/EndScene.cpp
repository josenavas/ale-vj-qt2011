#include "EndScene.h"
#include <OgreOverlayManager.h>

EndScene::EndScene(Ogre::Root* root, Ogre::RenderWindow* window)
{
	mRoot = root;
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	mWindow = window;
	mExit = false;
}

EndScene::~EndScene(void)
{
}

void EndScene::createScene(void)
{
	createSceneCommon();
	initObjectNames();

	mOverlayItems->hide();
	mOverlayObjName->hide();

	mOverlay = Ogre::OverlayManager::getSingleton().getByName("Overlays/EndGame");
	mOverlay->show();
}

void EndScene::initObjectNames(void)
{
	for(int i = 0; i < 10; i++) mObjectNames[i] = new Ogre::String("");
}

int EndScene::objectInteraction(Ogre::String name)
{
	mTime = 0;
	mExit = true;
	mOverlay->hide();
	return OBJECT_ANIM_NONE;
}

AbstractScene* EndScene::getNextScene(void)
{
	return NULL;
}