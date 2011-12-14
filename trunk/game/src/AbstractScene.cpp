#include "AbstractScene.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>

#define NO_ITEM_MATERIAL "Textures/NoItem"

AbstractScene::AbstractScene()
{
}

AbstractScene::~AbstractScene(void)
{
}

void AbstractScene::createScene(void)
{
}

Ogre::SceneManager* AbstractScene::getSceneManager(void)
{
	return mSceneMgr;
}

void AbstractScene::createSceneCommon(void)
{
	// create the camera
	mCamera = mSceneMgr->createCamera(PLAYER_CAM_NAME);
	mCamera->setNearClipDistance(1);

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::Entity* playerEntity = mSceneMgr->createEntity(PLAYER_MESH_NAME, "man.mesh");
	playerEntity->setCastShadows(true);

	Ogre::SceneNode* playerNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(PLAYER_NODE_NAME);
	playerNode->attachObject(playerEntity);

	Ogre::SceneNode* camYawNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(CAMERA_YAW_NODE_NAME, Ogre::Vector3(0, playerEntity->getBoundingBox().getHalfSize().y, 0));

	Ogre::SceneNode* camPitchNode = camYawNode->createChildSceneNode(CAMERA_PITCH_NODE_NAME, Ogre::Vector3(0, 0, 0));

	Ogre::SceneNode* camNode = camPitchNode->createChildSceneNode(CAMERA_NODE_NAME, Ogre::Vector3(0, 200-playerEntity->getBoundingBox().getHalfSize().y, 200));
	camNode->pitch(Ogre::Degree(-27));
	camNode->attachObject(mCamera);

	mOverlayItems = Ogre::OverlayManager::getSingleton().getByName("Overlays/MenuObjects");
	mOverlayItems->show();

	mOverlayObjName = Ogre::OverlayManager::getSingleton().getByName("Overlays/ObjectName");
	mOverlayObjName->show();

	for(unsigned int i = 0; i < 10; i++) mHasElements[i] = false;
}

void AbstractScene::addItemToInventary(Ogre::String name)
{
	unsigned int i;
	for(i = 0; i < 10; i++)
	{
		if(! mObjectNames[i]->compare(name) ) break;
	}
	Ogre::String nameItem("Elements/Item"+i);
	Ogre::String nameItemInside(nameItem+"Inside");
	mHasElements[i] = true;
	mOverlayItems->getChild(nameItem)->getChild(nameItemInside)->setMaterialName("OverlayTextures/"+name);
}

void AbstractScene::RemoveItemFromInventary(Ogre::String name)
{
	unsigned int i;
	for(i = 0; i < 10; i++)
	{
		if(! mObjectNames[i]->compare(name) ) break;
	}
	Ogre::String nameItem("Elements/Item"+i);
	Ogre::String nameItemInside(nameItem+"Inside");
	mHasElements[i] = false;
	mOverlayItems->getChild(nameItem)->getChild(nameItemInside)->setMaterialName("OverlayTextures/NoItem");
}

void AbstractScene::setPointedObject(Ogre::String name)
{
	mOverlayObjName->getChild("Elements/PanelText")->getChild("Elements/NameText")->setCaption(Ogre::DisplayString(name));
}