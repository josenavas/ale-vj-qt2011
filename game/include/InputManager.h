#ifndef __INPUT_MANAGER_H
#define __INPUT_MANAGER_H

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <OgreEntity.h>
#include "AbstractScene.h"

class InputManager : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	InputManager(Ogre::RenderWindow* window, Ogre::Root* root);
	~InputManager(void);

	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// OIS::KeyListener
	virtual bool keyPressed(const OIS::KeyEvent& evt);
	virtual bool keyReleased(const OIS::KeyEvent& evt);

	// OIS::MouseListener
	virtual bool mouseMoved(const OIS::MouseEvent& evt);
	virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

private:
	void initScene(AbstractScene*);

	bool keyPressedStyleLevel(const OIS::KeyEvent& evt);
	bool keyReleasedStyleLevel(const OIS::KeyEvent& evt);
	bool mouseMovedStyleLevel(const OIS::MouseEvent& evt);
	bool mousePressedStyleLevel(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool mouseReleasedStyleLevel(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	bool keyPressedStyleMenu(const OIS::KeyEvent& evt);
	bool keyReleasedStyleMenu(const OIS::KeyEvent& evt);
	bool mouseMovedStyleMenu(const OIS::MouseEvent& evt);
	bool mousePressedStyleMenu(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool mouseReleasedStyleMenu(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	typedef enum {eIDLE, eWALKING, eLEFT, eRIGHT, ePICKUP, eACTIVATE} ePlayerState;

	bool collisionControlZNeg(void);
	bool collisionControlXPos(void);
	bool collisionControlXNeg(void);

	void orientatePlayer(void);

	void changePointedObject(void);

	Ogre::MovableObject* executeCamRay(void);
	Ogre::MovableObject* executePlayerRay(void);
	Ogre::MovableObject* executePlayerHandRay(void);

	int executeActive(void);

	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;

	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::Vector3 mDirection;

	Ogre::SceneNode *mPlayerNode;
	Ogre::Entity *mPlayerEntity;
	Ogre::AnimationState* mAnimationState;

	Ogre::SceneNode *mCamYawNode;
	Ogre::SceneNode *mCamPitchNode;
	Ogre::SceneNode *mCamNode;

	bool mShutDown;
	bool mLeftShiftDown;

	ePlayerState mState;

	Ogre::RaySceneQuery *mRaySceneQuery;

	AbstractScene *mScene;

	bool mSceneIsLevel;

	Ogre::Degree mAcumYaw;
};

#endif