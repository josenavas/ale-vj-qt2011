#ifndef __EXAMPLE_SCENE_H
#define __EXAMPLE_SCENE_H

#include "AbstractScene.h"

class ExampleScene : AbstractScene
{
public:
	ExampleScene(Ogre::Root* root, Ogre::RenderWindow* window);
	~ExampleScene(void);
	virtual void createScene(void);
	virtual int objectInteraction(Ogre::String);

private:
	static const int xLen = 500;
	static const int yLen = 200;
	static const int zLen = 500;

	void putWalls(void);
	void putLights(void);
};

#endif