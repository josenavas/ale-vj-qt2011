#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>

class InputManager : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	InputManager(Ogre::RenderWindow* window, Ogre::SceneManager* sceneManager);
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

	bool collisionControl(void);

	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;

	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::Vector3 mDirection;

	Ogre::SceneNode *mPlayerNode;
	Ogre::AnimationState* mAnimationState;

	bool mShutDown;
	Ogre::Real mRotateEx;

	Ogre::RaySceneQuery *mRaySceneQuery;
};