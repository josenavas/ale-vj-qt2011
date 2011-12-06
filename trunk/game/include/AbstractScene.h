#include <OgreRoot.h>

class AbstractScene
{
public:
	AbstractScene();
	~AbstractScene(void);
	virtual void createScene(void) = 0;

	Ogre::SceneManager* getSceneManager(void);

protected:
	Ogre::Root* mRoot;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::RenderWindow* mWindow;
};