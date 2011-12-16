#ifndef __START_SCENE_H
#define __START_SCENE_H

#include "AbstractScene.h"

class StartScene : AbstractScene
{
public:
	StartScene(Ogre::Root* root, Ogre::RenderWindow* window);
	~StartScene(void);

	virtual void createScene(void);
	virtual int objectInteraction(Ogre::String);
	virtual bool isLevelScene(void){return false;};
	virtual AbstractScene* getNextScene(void);

protected:
	virtual void initObjectNames(void);

private:
	Ogre::Overlay* mOverlay;
};

#endif