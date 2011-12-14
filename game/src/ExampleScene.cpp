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

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.4f,0.4f,0.4f));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	putWalls();
	putLights();

	Ogre::Entity* entBed = mSceneMgr->createEntity("Bed", "bed.mesh");
	Ogre::SceneNode* nodeBed = mSceneMgr->getRootSceneNode()->createChildSceneNode("Bed",Ogre::Vector3(0,entBed->getBoundingBox().getHalfSize().y-5,0));
	nodeBed->attachObject(entBed);
	nodeBed->yaw(Ogre::Degree(-90));
	nodeBed->translate(Ogre::Vector3(xLen/2-entBed->getBoundingBox().getHalfSize().x-10,0,zLen/2-entBed->getBoundingBox().getHalfSize().z-50));

	Ogre::Entity* entBedTable = mSceneMgr->createEntity("BedTable", "endtable.mesh");
	Ogre::SceneNode* nodeBedTable = nodeBed->createChildSceneNode("BedTable",Ogre::Vector3(0,-entBedTable->getBoundingBox().getHalfSize().y,0));
	nodeBedTable->attachObject(entBedTable);
	nodeBedTable->translate(Ogre::Vector3(-entBed->getBoundingBox().getHalfSize().x-30,0,-entBed->getBoundingBox().getHalfSize().z+entBedTable->getBoundingBox().getHalfSize().z));
	
	Ogre::Entity* entExitDoor = mSceneMgr->createEntity("ExitDoor", "door.mesh");
	Ogre::SceneNode* nodeExitDoor = mSceneMgr->getRootSceneNode()->createChildSceneNode("ExitDoor", Ogre::Vector3(0,entExitDoor->getBoundingBox().getHalfSize().y-6,0));
	nodeExitDoor->attachObject(entExitDoor);
	nodeExitDoor->translate(Ogre::Vector3(0,0,-zLen/2-5));

	Ogre::Entity* entShelves = mSceneMgr->createEntity("Shelves", "shelves.mesh");
	Ogre::SceneNode* nodeShelves = mSceneMgr->getRootSceneNode()->createChildSceneNode("Shelves");
	nodeShelves->attachObject(entShelves);
	nodeShelves->yaw(Ogre::Degree(90));
	nodeShelves->translate(Ogre::Vector3(-xLen/2,0,zLen/4));

	Ogre::Entity* entDesk = mSceneMgr->createEntity("Desk", "desk.mesh");
	Ogre::SceneNode* nodeDesk = mSceneMgr->getRootSceneNode()->createChildSceneNode("Desk");
	nodeDesk->attachObject(entDesk);
	nodeDesk->yaw(Ogre::Degree(90));
	nodeDesk->translate(Ogre::Vector3(-xLen/2+entDesk->getBoundingBox().getHalfSize().x/2,0,-zLen/4));

	Ogre::Entity* entDeskLamp = mSceneMgr->createEntity("DeskLamp", "lamp.mesh");
	Ogre::SceneNode* nodeDeskLamp = nodeDesk->createChildSceneNode("DeskLamp");
	nodeDeskLamp->attachObject(entDeskLamp);
	nodeDeskLamp->yaw(Ogre::Degree(-135));
	nodeDeskLamp->translate(Ogre::Vector3(-40,entDesk->getBoundingBox().getSize().y,-10));

	//Ogre::Light* ldesk = mSceneMgr->createLight("DeskLampLight");
	//ldesk->setType(Ogre::Light::LT_SPOTLIGHT);
	//ldesk->setDiffuseColour(1.0, 1.0, 0);
	//ldesk->setSpecularColour(1.0, 1.0, 0);
	//ldesk->setDirection(0,-1,-1);
	//ldesk->setSpotlightRange(Ogre::Degree(30),Ogre::Degree(40),2);
	//Ogre::SceneNode* nodeLightDesk = nodeDeskLamp->createChildSceneNode("DeskLampLight");
	//nodeLightDesk->attachObject(ldesk);
	//nodeLightDesk->translate(Ogre::Vector3(0,entDeskLamp->getBoundingBox().getSize().y-10,0));

	Ogre::Entity* entDeskChair = mSceneMgr->createEntity("DeskChair", "chair.mesh");
	Ogre::SceneNode* nodeDeskChair = nodeDesk->createChildSceneNode("DeskChair",Ogre::Vector3(0,entDeskChair->getBoundingBox().getHalfSize().y-5,0));
	nodeDeskChair->attachObject(entDeskChair);
	nodeDeskChair->translate(Ogre::Vector3(-20,0,30));

	Ogre::Entity* entFrame = mSceneMgr->createEntity("Frame", "frame.mesh");
	Ogre::SceneNode* nodeFrame = mSceneMgr->getRootSceneNode()->createChildSceneNode("Frame");
	nodeFrame->attachObject(entFrame);
	nodeFrame->translate(Ogre::Vector3(xLen/3,yLen/2,-zLen/2-1));

	Ogre::Plane interruptor(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("interruptorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, interruptor, 10, 10, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entInterruptorEntity = mSceneMgr->createEntity("Interruptor", "interruptorPlane");
	Ogre::SceneNode* nodeInterruptor = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	nodeInterruptor->attachObject(entInterruptorEntity);
	nodeInterruptor->yaw(Ogre::Degree(-90));
	nodeInterruptor->translate(Ogre::Vector3(60, mSceneMgr->getEntity(PLAYER_MESH_NAME)->getBoundingBox().getSize().y * Ogre::Real(0.65), -zLen/2+1));
	entInterruptorEntity->setMaterialName("Textures/Interruptor");
	entInterruptorEntity->setCastShadows(false);
}

int ExampleScene::objectInteraction(Ogre::String name)
{
	if(!name.compare("Interruptor"))
	{
		Ogre::Light* l = mSceneMgr->getLight("Light1");
		l->setVisible(!l->getVisible());
		Ogre::Entity* e = mSceneMgr->getEntity("rightWallTextEntity");
		e->setVisible(!e->getVisible());
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
	entGround->setMaterialName("Textures/floor");
	entGround->setCastShadows(false);

	Ogre::Plane roof(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("roof", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, roof, xLen, zLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entRoof = mSceneMgr->createEntity("RoofEntity", "roof");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRoof);
	node->translate(0, yLen, 0);
	entRoof->setMaterialName("Textures/roof");
	entRoof->setCastShadows(false);

	Ogre::Plane leftWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("leftWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, leftWall, zLen, yLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entLeftWall = mSceneMgr->createEntity("leftWallEntity", "leftWall");
	node =  mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entLeftWall);
	node->yaw(Ogre::Degree(-0.1f));
	node->translate(Ogre::Vector3(-xLen/2+1, yLen/2, 0));
	entLeftWall->setMaterialName("Textures/wall");
	entLeftWall->setCastShadows(false);

	Ogre::Plane rightWall(-Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rightWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rightWall, zLen, yLen, 200, 200, true, 1, 5, 5, -Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRightWall = mSceneMgr->createEntity("rightWallEntity", "rightWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRightWall);
	node->yaw(Ogre::Degree(0.1f));
	node->translate(Ogre::Vector3(xLen/2-1, yLen/2, 0));
	entRightWall->setMaterialName("Textures/wall");
	entRightWall->setCastShadows(false);

	Ogre::Plane rightWallText(-Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rightWallText", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rightWall, 100, 70, 200, 200, true, 1, 1, 1, -Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRightWallText = mSceneMgr->createEntity("rightWallTextEntity", "rightWallText");
	node = node->createChildSceneNode();
	node->attachObject(entRightWallText);
	node->pitch(Ogre::Degree(180));
	node->translate(Ogre::Vector3(-0.1f,20,-zLen/4));
	entRightWallText->setMaterialName("Textures/writing");
	entRightWallText->setCastShadows(false);
	entRightWallText->setVisible(false);

	Ogre::Plane frontWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("frontWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, frontWall, xLen, yLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entFrontWall = mSceneMgr->createEntity("frontWallEntity", "frontWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entFrontWall);
	node->yaw(Ogre::Degree(-90));
	node->translate(Ogre::Vector3(0, yLen/2, -zLen/2));
	entFrontWall->setMaterialName("Textures/wall");
	entFrontWall->setCastShadows(false);

	Ogre::Plane rearWall(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("rearWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, rearWall, xLen, yLen, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRearWall = mSceneMgr->createEntity("rearWallEntity", "rearWall");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entRearWall);
	node->yaw(Ogre::Degree(90));
	node->translate(Ogre::Vector3(0, yLen/2, zLen/2));
	entRearWall->setMaterialName("Textures/windows");
	entRearWall->setCastShadows(false);
}

void ExampleScene::putLights(void)
{
	Ogre::Light* lsun = mSceneMgr->createLight("SunLight");
	lsun->setType(Ogre::Light::LT_DIRECTIONAL);
	lsun->setDiffuseColour(0.7, 0.7, 0.7);
	lsun->setSpecularColour(0.7, 0.7, 0.7);
	lsun->setDirection(0, -1, -5);
	lsun->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));

	//Ogre::Light* lmain = mSceneMgr->createLight("MainLight");
	//lmain->setType(Ogre::Light::LT_POINT);
	//lmain->setDiffuseColour(1.0, 1.0, 1.0);
	//lmain->setSpecularColour(1.0, 1.0, 1.0);
	//lmain->setPosition(0, yLen, 0);
	//lmain->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
	
	Ogre::Light* l1 = mSceneMgr->createLight("Light1");
	l1->setType(Ogre::Light::LT_SPOTLIGHT);
	l1->setPosition(xLen/4, yLen, -zLen/4);
	l1->setDirection(2, -1, 0);
	l1->setDiffuseColour(1.0, 0.0, 0.0);
	l1->setSpecularColour(1.0, 0.0, 0.0);	
    l1->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
	l1->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
	l1->setVisible(false);
}