#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include "Definitions.h"

class AbstractScene
{
public:
	AbstractScene();
	~AbstractScene(void);
	virtual void createScene(void) = 0;

	Ogre::SceneManager* getSceneManager(void);

protected:
	void createSceneCommon(void);

	Ogre::Root* mRoot;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::RenderWindow* mWindow;
};