#include "AbstractScene.h"

class ExampleScene : AbstractScene
{
public:
	ExampleScene(Ogre::Root* root, Ogre::RenderWindow* window);
	~ExampleScene(void);
	virtual void createScene(void);
};