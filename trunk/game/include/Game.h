#include <OgreRoot.h>

class Game
{
public:
	Game(void);
	virtual ~Game(void);
	bool go(void);
private:
	Ogre::Root* mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
};