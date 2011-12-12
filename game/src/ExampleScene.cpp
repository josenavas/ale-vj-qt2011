#include "ExampleScene.h"
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>

ExampleScene::ExampleScene(Ogre::Root* root, Ogre::RenderWindow* window)
{
	mRoot = root;
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	mWindow = window;
}

ExampleScene::~ExampleScene(void)
{
}

void ExampleScene::createScene(void)
{

	createSceneCommon();

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0,0,0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);

	Ogre::Light* l = mSceneMgr->createLight("pointLight");
	l->setType(Ogre::Light::LT_POINT);
	mSceneMgr->getSceneNode(PLAYER_NODE_NAME)->createChildSceneNode("LIGHT_NODE", Ogre::Vector3(0, 200, 0))->attachObject(l);
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setSpecularColour(1.0, 1.0, 1.0);
	l->setAttenuation(Ogre::Real(1000), Ogre::Real(1.0), Ogre::Real(0.007), Ogre::Real(0.00007));
	
	Ogre::Plane ground(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ground, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Textures/MRAMOR");
	//entGround->setMaterialName("Examples/Rockwall");
	entGround->setCastShadows(false);

	Ogre::Plane leftWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("leftWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, leftWall, 1500, 500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entLeftWall = mSceneMgr->createEntity("leftWallEntity", "leftWall");
	Ogre::SceneNode* node =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entLeftWall);
	node->translate(Ogre::Vector3(-750, 250, 0));
	entLeftWall->setMaterialName("Textures/Wood");
	entLeftWall->setCastShadows(false);

	Ogre::Plane rightWall(-Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rightWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rightWall, 1500, 500, 200, 200, true, 1, 5, 5, -Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRightWall = mSceneMgr->createEntity("rightWallEntity", "rightWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRightWall);
	node->translate(Ogre::Vector3(750, 250, 0));
	entRightWall->setMaterialName("Textures/Wood");
	entRightWall->setCastShadows(false);

	Ogre::Plane frontWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("frontWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, frontWall, 1500, 500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entFrontWall = mSceneMgr->createEntity("frontWallEntity", "frontWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entFrontWall);
	node->yaw(Ogre::Degree(-90));
	node->translate(Ogre::Vector3(0, 250, -750));
	entFrontWall->setMaterialName("Textures/Wood");
	entFrontWall->setCastShadows(false);

	Ogre::Plane rearWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rearWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rearWall, 1500, 500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRearWall = mSceneMgr->createEntity("rearWallEntity", "rearWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRearWall);
	node->yaw(Ogre::Degree(90));
	node->translate(Ogre::Vector3(0, 250, 750));
	entRearWall->setMaterialName("Textures/Wood");
	entRearWall->setCastShadows(false);
}