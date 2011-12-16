#ifndef __END_SCENE_H
#define __END_SCENE_H

#include "AbstractScene.h"

class EndScene : AbstractScene
{
public:
	EndScene(Ogre::Root* root, Ogre::RenderWindow* window);
	~EndScene(void);

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