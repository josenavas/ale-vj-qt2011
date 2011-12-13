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

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.05f,0.05f,0.05f));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Light* l1 = mSceneMgr->createLight("light1");
	l1->setType(Ogre::Light::LT_POINT);
	l1->setPosition(700, 450, 700);
	l1->setDiffuseColour(1.0, 1.0, 1.0);
	l1->setSpecularColour(1.0, 1.0, 1.0);
	l1->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));

	Ogre::Light* l2 = mSceneMgr->createLight("light2");
	l2->setType(Ogre::Light::LT_POINT);
	l2->setPosition(-700, 450, -700);
	l2->setDiffuseColour(1.0, 1.0, 1.0);
	l2->setSpecularColour(1.0, 1.0, 1.0);
	l2->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
	
	Ogre::Light* l3 = mSceneMgr->createLight("light3");
	l3->setType(Ogre::Light::LT_POINT);
	l3->setPosition(700, 450, -700);
	l3->setDiffuseColour(1.0, 1.0, 1.0);
	l3->setSpecularColour(1.0, 1.0, 1.0);
	l3->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));

	Ogre::Light* l4 = mSceneMgr->createLight("light4");
	l4->setType(Ogre::Light::LT_POINT);
	l4->setPosition(-700, 450, 700);
	l4->setDiffuseColour(1.0, 1.0, 1.0);
	l4->setSpecularColour(1.0, 1.0, 1.0);
	l4->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));

	Ogre::Plane ground(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ground, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Textures/MRAMOR");
	entGround->setCastShadows(false);

	Ogre::Plane roof(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("roof", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, roof, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entRoof = mSceneMgr->createEntity("RoofEntity", "roof");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRoof);
	node->translate(0, 500, 0);
	entRoof->setMaterialName("Examples/Rockwall");
	entRoof->setCastShadows(false);

	Ogre::Plane leftWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("leftWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, leftWall, 1500, 500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entLeftWall = mSceneMgr->createEntity("leftWallEntity", "leftWall");
	node =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entLeftWall);
	node->translate(Ogre::Vector3(-750, 250, 0));
	entLeftWall->setMaterialName("Textures/Wall");
	entLeftWall->setCastShadows(false);

	Ogre::Plane rightWall(-Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rightWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rightWall, 1500, 500, 200, 200, true, 1, 5, 5, -Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRightWall = mSceneMgr->createEntity("rightWallEntity", "rightWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRightWall);
	node->translate(Ogre::Vector3(750, 250, 0));
	entRightWall->setMaterialName("Textures/Wall");
	entRightWall->setCastShadows(false);

	Ogre::Plane frontWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("frontWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, frontWall, 1500, 500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entFrontWall = mSceneMgr->createEntity("frontWallEntity", "frontWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entFrontWall);
	node->yaw(Ogre::Degree(-90));
	node->translate(Ogre::Vector3(0, 250, -750));
	entFrontWall->setMaterialName("Textures/Wall");
	entFrontWall->setCastShadows(false);

	Ogre::Plane rearWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rearWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rearWall, 1500, 500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRearWall = mSceneMgr->createEntity("rearWallEntity", "rearWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRearWall);
	node->yaw(Ogre::Degree(90));
	node->translate(Ogre::Vector3(0, 250, 750));
	entRearWall->setMaterialName("Textures/Wall");
	entRearWall->setCastShadows(false);
}