#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>

class InputManager : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
	InputManager(Ogre::RenderWindow* window);
	~InputManager(void);

	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	Ogre::RenderWindow* mWindow;
};