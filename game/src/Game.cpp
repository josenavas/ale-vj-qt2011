#include "Game.h"

#include <OgreConfigFile.h>
#include <OgreException.h>

Game::Game(void)
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK)
{
}

Game::~Game(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, mInputMgr);
	mInputMgr->windowClosed(mWindow);
	delete mScene;
	delete mInputMgr;
	delete mRoot;
}

bool Game::go(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	//Construct Ogre::Root
	mRoot = new Ogre::Root(mPluginsCfg);

	loadResources();
	bool ret = createRenderSystem();
	if(!ret) return false;

	initialisingResources();

	mScene = (AbstractScene*) new ExampleScene(mRoot, mWindow);
	mScene->createScene();

	mInputMgr = new InputManager(mWindow, mScene->getSceneManager());
	mInputMgr->windowResized(mWindow);

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, mInputMgr);

	mRoot->addFrameListener(mInputMgr);
	mRoot->startRendering();

	return true;
}

void Game::loadResources(void)
{
	//set up resources
	//Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	//Go through all sections & settins in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for( i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}

bool Game::createRenderSystem(void)
{
	// configure
	// Show the configuration dialog and initialise the system
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog())) return false;

	mWindow = mRoot->initialise(true, "Game");
	return true;
}

void Game::initialisingResources(void)
{
	//Set default mipmap level
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	//initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern"C" {
#endif

#if OGRE__PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main (int argc, char *argv[])
#endif
	{
		Game app;
		try {
			app.go();
		} catch(Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}
#ifdef __cplusplus
}
#endif