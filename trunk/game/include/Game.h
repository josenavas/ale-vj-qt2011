#include <OgreRoot.h>
#include "ExampleScene.h"
#include "InputManager.h"

class Game
{
public:
	Game(void);
	virtual ~Game(void);
	bool go(void);

private:
	void loadResources(void);
	bool createRenderSystem(void);
	void initialisingResources(void);

	Ogre::Root* mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow* mWindow;

	AbstractScene* mScene;
	InputManager* mInputMgr;
};