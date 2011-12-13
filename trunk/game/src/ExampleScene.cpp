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

	putWalls();
	putLights();

	Ogre::Entity* entBed = mSceneMgr->createEntity("Bed", "bed.mesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entBed);
	node->translate(Ogre::Vector3(xLen/2-entBed->getBoundingBox().getHalfSize().x,entBed->getBoundingBox().getHalfSize().y,zLen/2-entBed->getBoundingBox().getHalfSize().z));
	
	Ogre::Entity* entExitDoor = mSceneMgr->createEntity("ExitDoor", "door.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entExitDoor);
	node->translate(Ogre::Vector3(0,entExitDoor->getBoundingBox().getHalfSize().y,-zLen/2+entExitDoor->getBoundingBox().getHalfSize().z));

	Ogre::Entity* entDesk = mSceneMgr->createEntity("Desk", "desk.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entDesk);
	node->yaw(Ogre::Degree(90));
	node->translate(Ogre::Vector3(-xLen/2+entDesk->getBoundingBox().getHalfSize().x/2,0,0));

	Ogre::Entity* entDeskLamp = mSceneMgr->createEntity("DeskLamp", "lamp.mesh");
	node = node->createChildSceneNode();
	node->attachObject(entDeskLamp);
	node->yaw(Ogre::Degree(200));
	node->translate(Ogre::Vector3(-50,entDesk->getBoundingBox().getHalfSize().y+entDeskLamp->getBoundingBox().getHalfSize().y+5,0));

	Ogre::Light* ldesk = mSceneMgr->createLight("lightDesk");
	ldesk->setType(Ogre::Light::LT_SPOTLIGHT);
	ldesk->setDiffuseColour(0.9, 1.0, 1.0);
	ldesk->setSpecularColour(0.9, 1.0, 1.0);
	ldesk->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
	node->attachObject(ldesk);

	Ogre::Plane interruptor(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("interruptor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, interruptor, 10, 10, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entInterruptorEntity = mSceneMgr->createEntity("interruptorEntity", "interruptor");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entInterruptorEntity);
	node->yaw(Ogre::Degree(-90));
	node->translate(Ogre::Vector3(60, mSceneMgr->getEntity(PLAYER_MESH_NAME)->getBoundingBox().getSize().y * Ogre::Real(0.65), -zLen/2+1));
	entInterruptorEntity->setMaterialName("Textures/Interruptor");
	entInterruptorEntity->setCastShadows(false);
}

int ExampleScene::objectInteraction(Ogre::String name)
{
	if(!name.compare("interruptorEntity"))
	{
		Ogre::Light* l = mSceneMgr->getLight("light1");
		l->setVisible(!l->getVisible());
		return OBJECT_ANIM_ACTIV;
	}
	return OBJECT_ANIM_NONE;
}

void ExampleScene::putWalls(void)
{
	Ogre::Plane ground(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ground, xLen, zLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Textures/MRAMOR");
	entGround->setCastShadows(false);

	Ogre::Plane roof(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("roof", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, roof, xLen, zLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entRoof = mSceneMgr->createEntity("RoofEntity", "roof");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRoof);
	node->translate(0, yLen, 0);
	entRoof->setMaterialName("Examples/Rockwall");
	entRoof->setCastShadows(false);

	Ogre::Plane leftWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("leftWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, leftWall, zLen, yLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entLeftWall = mSceneMgr->createEntity("leftWallEntity", "leftWall");
	node =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entLeftWall);
	node->translate(Ogre::Vector3(-xLen/2, yLen/2, 0));
	entLeftWall->setMaterialName("Textures/Wall");
	entLeftWall->setCastShadows(false);

	Ogre::Plane rightWall(-Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rightWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rightWall, zLen, yLen, 200, 200, true, 1, 5, 5, -Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRightWall = mSceneMgr->createEntity("rightWallEntity", "rightWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRightWall);
	node->translate(Ogre::Vector3(xLen/2, yLen/2, 0));
	entRightWall->setMaterialName("Textures/Wall");
	entRightWall->setCastShadows(false);

	Ogre::Plane frontWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("frontWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, frontWall, xLen, yLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entFrontWall = mSceneMgr->createEntity("frontWallEntity", "frontWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entFrontWall);
	node->yaw(Ogre::Degree(-90));
	node->translate(Ogre::Vector3(0, yLen/2, -zLen/2));
	entFrontWall->setMaterialName("Textures/Wall");
	entFrontWall->setCastShadows(false);

	Ogre::Plane rearWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rearWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rearWall, xLen, yLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRearWall = mSceneMgr->createEntity("rearWallEntity", "rearWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRearWall);
	node->yaw(Ogre::Degree(90));
	node->translate(Ogre::Vector3(0, yLen/2, zLen/2));
	entRearWall->setMaterialName("Textures/Wall");
	entRearWall->setCastShadows(false);
}

void ExampleScene::putLights(void)
{
	Ogre::Light* l1 = mSceneMgr->createLight("light1");
	l1->setType(Ogre::Light::LT_POINT);
	l1->setPosition(0, yLen, 0);
	l1->setDiffuseColour(1.0, 1.0, 1.0);
	l1->setSpecularColour(1.0, 1.0, 1.0);
	l1->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
	
	//Ogre::Light* l3 = mSceneMgr->createLight("light3");
	//l3->setType(Ogre::Light::LT_POINT);
	//l3->setPosition(700, 450, -700);
	//l3->setDiffuseColour(1.0, 1.0, 1.0);
	//l3->setSpecularColour(1.0, 1.0, 1.0);
	//l3->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));

	//Ogre::Light* l4 = mSceneMgr->createLight("light4");
	//l4->setType(Ogre::Light::LT_POINT);
	//l4->setPosition(-700, 450, 700);
	//l4->setDiffuseColour(1.0, 1.0, 1.0);
	//l4->setSpecularColour(1.0, 1.0, 1.0);
	//l4->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
}