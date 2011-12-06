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