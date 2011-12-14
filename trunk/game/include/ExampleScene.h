#ifndef __EXAMPLE_SCENE_H
#define __EXAMPLE_SCENE_H

#define xLen 500
#define yLen 200
#define zLen 500

#include "AbstractScene.h"

class ExampleScene : AbstractScene
{
public:
	ExampleScene(Ogre::Root* root, Ogre::RenderWindow* window);
	~ExampleScene(void);
	virtual void createScene(void);
	virtual int objectInteraction(Ogre::String);

private:
	void putWalls(void);
	void putLights(void);
	void putObjects(void);
};

#endif