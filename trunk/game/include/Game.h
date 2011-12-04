#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>

class Game : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
	Game(void);
	virtual ~Game(void);
	bool go(void);

protected:
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
	Ogre::Root* mRoot;
	Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	//OIS input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
};