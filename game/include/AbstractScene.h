#ifndef __ABSTRACT_SCENE_H
#define __ABSTRACT_SCENE_H

#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreOverlay.h>
#include "Definitions.h"

class AbstractScene
{
public:
	AbstractScene();
	~AbstractScene(void);
	virtual void createScene(void) = 0;

	Ogre::SceneManager* getSceneManager(void);

	virtual int objectInteraction(Ogre::String) = 0;

	void getItem(Ogre::String);

	void setPointedObject(Ogre::String);

protected:
	void createSceneCommon(void);

	Ogre::Root* mRoot;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::RenderWindow* mWindow;
	Ogre::Overlay* mOverlayItems;
	Ogre::Overlay* mOverlayObjName;

	bool mHasElements;
};

#endif