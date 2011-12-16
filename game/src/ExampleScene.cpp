#include "ExampleScene.h"
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreMaterialManager.h>

#define INTERRUPTOR_ENTITY "Interruptor"
#define BED_TABLE_ENTITY "BedTable"
#define DESK_ENTITY "Desk"
#define DESK_LAMP_ENTITY "DeskLamp"
#define SHELVES_ENTITY "Shelves"
#define EXIT_DOOR_ENTITY "ExitDoor"

#define HOLE_BLUE_ENTITY "BlueHole"
#define HOLE_GREEN_ENTITY "GreenHole"
#define HOLE_RED_ENTITY "RedHole"

#define HOLE_BLUE_NODE "BlueHoleNode"
#define HOLE_GREEN_NODE "GreenHoleNode"
#define HOLE_RED_NODE "RedHoleNode"

#define SHELF_KEY_ENTITY "ShelfKey"
#define SHELF_KEY_RED_ENTITY "ShelfKeyRed"
#define SHELF_KEY_BLUE_ENTITY "ShelfKeyBlue"
#define SHELF_KEY_GREEN_ENTITY "ShelfKeyGreen"

#define SHELF_KEY_NODE "ShelfKeyNode"
#define SHELF_KEY_RED_NODE "ShelfKeyRedNode"
#define SHELF_KEY_BLUE_NODE "ShelfKeyBlueNode"
#define SHELF_KEY_GREEN_NODE "ShelfKeyGreenNode"


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
	putObjects();

	initObjectNames();
}

void ExampleScene::initObjectNames(void)
{
	mObjectNames[0] = new Ogre::String(SHELF_KEY_RED_ENTITY);
	mObjectNames[1] = new Ogre::String(SHELF_KEY_BLUE_ENTITY);
	mObjectNames[2] = new Ogre::String(SHELF_KEY_GREEN_ENTITY);
	mObjectNames[3] = new Ogre::String(SHELF_KEY_ENTITY);
	for(int i = 4; i < 10; i++)
	{
		mObjectNames[i] = new Ogre::String("");
	}
}

int ExampleScene::objectInteraction(Ogre::String name)
{
	if(!name.compare(INTERRUPTOR_ENTITY))
	{
		Ogre::Entity* e = mSceneMgr->getEntity("rightWallTextEntity");
		e->setVisible(!e->getVisible());
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(DESK_ENTITY) && mSceneMgr->getSceneNode(SHELF_KEY_BLUE_NODE)->numAttachedObjects() != 0)
	{
		mSceneMgr->getSceneNode(SHELF_KEY_BLUE_NODE)->detachAllObjects();
		addItemToInventary(SHELF_KEY_BLUE_ENTITY);
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(BED_TABLE_ENTITY) && mSceneMgr->getSceneNode(SHELF_KEY_GREEN_NODE)->numAttachedObjects() != 0)
	{
		mSceneMgr->getSceneNode(SHELF_KEY_GREEN_NODE)->detachAllObjects();
		addItemToInventary(SHELF_KEY_GREEN_ENTITY);
		return OBJECT_ANIM_PICKUP;
	}

	if(!name.compare(DESK_LAMP_ENTITY))
	{
		mSceneMgr->getEntity(HOLE_BLUE_ENTITY)->setVisible(!mSceneMgr->getEntity(HOLE_BLUE_ENTITY)->isVisible());
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(SHELVES_ENTITY) && mSceneMgr->getSceneNode(SHELF_KEY_RED_NODE)->numAttachedObjects() != 0)
	{
		mSceneMgr->getSceneNode(SHELF_KEY_RED_NODE)->detachAllObjects();
		addItemToInventary(SHELF_KEY_RED_ENTITY);
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(HOLE_GREEN_ENTITY) && isItemInInventary(SHELF_KEY_RED_ENTITY))
	{
		RemoveItemFromInventary(SHELF_KEY_RED_ENTITY);
		mSceneMgr->getSceneNode(SHELF_KEY_NODE)->attachObject(mSceneMgr->getEntity(SHELF_KEY_ENTITY));
		mSceneMgr->getSceneNode(CAMERA_NODE_NAME)->roll(Ogre::Degree(180));
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(HOLE_RED_ENTITY) && isItemInInventary(SHELF_KEY_BLUE_ENTITY))
	{
		RemoveItemFromInventary(SHELF_KEY_BLUE_ENTITY);
		mSceneMgr->getEntity(HOLE_GREEN_ENTITY)->setVisible(true);
		mSceneMgr->getSceneNode(CAMERA_NODE_NAME)->roll(Ogre::Degree(180));
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(HOLE_BLUE_ENTITY) && isItemInInventary(SHELF_KEY_GREEN_ENTITY))
	{
		RemoveItemFromInventary(SHELF_KEY_GREEN_ENTITY);
		mSceneMgr->getEntity(HOLE_RED_ENTITY)->setVisible(true);
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(DESK_ENTITY) && mSceneMgr->getSceneNode(SHELF_KEY_NODE)->numAttachedObjects() != 0)
	{
		mSceneMgr->getSceneNode(SHELF_KEY_NODE)->detachAllObjects();
		addItemToInventary(SHELF_KEY_ENTITY);
		return OBJECT_ANIM_ACTIV;
	}

	if(!name.compare(EXIT_DOOR_ENTITY) && isItemInInventary(SHELF_KEY_ENTITY) )
	{
		// TODO: Hemos ganado!!
		RemoveItemFromInventary(SHELF_KEY_ENTITY);
		mExit = true;
		return OBJECT_ANIM_ACTIV;
	}

	return OBJECT_ANIM_NONE;
}
	
void ExampleScene::putObjects(void)
{
	Ogre::Entity* entBed = mSceneMgr->createEntity("Bed", "bed.mesh");
	entBed->setCastShadows(true);
	Ogre::SceneNode* nodeBed = mSceneMgr->getRootSceneNode()->createChildSceneNode("BedNode",Ogre::Vector3(0,entBed->getBoundingBox().getHalfSize().y,0));
	nodeBed->attachObject(entBed);
	nodeBed->yaw(Ogre::Degree(-90));
	nodeBed->translate(Ogre::Vector3(xLen/2-entBed->getBoundingBox().getHalfSize().x-20,-2,zLen/2-entBed->getBoundingBox().getHalfSize().z-50));

	Ogre::Entity* entBedTable = mSceneMgr->createEntity(BED_TABLE_ENTITY, "endtable.mesh");
	entBedTable->setCastShadows(true);
	Ogre::SceneNode* nodeBedTable = nodeBed->createChildSceneNode("BedTableNode",Ogre::Vector3(0,-entBed->getBoundingBox().getHalfSize().y,0));
	nodeBedTable->attachObject(entBedTable);
	nodeBedTable->translate(Ogre::Vector3(-entBed->getBoundingBox().getHalfSize().x-30,1,-entBed->getBoundingBox().getHalfSize().z+entBedTable->getBoundingBox().getHalfSize().z));

	Ogre::Entity* entShelfKeyGreen = mSceneMgr->createEntity(SHELF_KEY_GREEN_ENTITY, "key.mesh");
	entShelfKeyGreen->setCastShadows(true);
	entShelfKeyGreen->setMaterialName("Material/KeyGreen");
	Ogre::SceneNode* nodeShelfKeyGreen = nodeBedTable->createChildSceneNode(SHELF_KEY_GREEN_NODE, Ogre::Vector3::ZERO);
	nodeShelfKeyGreen->attachObject(entShelfKeyGreen);
	nodeShelfKeyGreen->translate(Ogre::Vector3(0,entBedTable->getBoundingBox().getSize().y-15,0));
	
	Ogre::Entity* entShelves = mSceneMgr->createEntity(SHELVES_ENTITY, "shelves.mesh");
	entShelves->setCastShadows(true);
	Ogre::SceneNode* nodeShelves = mSceneMgr->getRootSceneNode()->createChildSceneNode("ShelvesNode");
	nodeShelves->attachObject(entShelves);
	nodeShelves->yaw(Ogre::Degree(90));
	nodeShelves->translate(Ogre::Vector3(-xLen/2+entShelves->getBoundingBox().getHalfSize().x-35,0,zLen/4));

	Ogre::Entity* entShelfKeyRed = mSceneMgr->createEntity(SHELF_KEY_RED_ENTITY, "key.mesh");
	entShelfKeyRed->setCastShadows(true);
	entShelfKeyRed->setMaterialName("Material/KeyRed");
	Ogre::SceneNode* nodeShelfKeyRed = nodeShelves->createChildSceneNode(SHELF_KEY_RED_NODE,Ogre::Vector3(0,-entShelves->getBoundingBox().getHalfSize().y,0));
	nodeShelfKeyRed->attachObject(entShelfKeyRed);
	nodeShelfKeyRed->yaw(Ogre::Degree(35));
	nodeShelfKeyRed->translate(Ogre::Vector3(15,yLen/2+10,15));
	
	Ogre::Entity* entDesk = mSceneMgr->createEntity(DESK_ENTITY, "desk.mesh");
	entDesk->setCastShadows(true);
	Ogre::SceneNode* nodeDesk = mSceneMgr->getRootSceneNode()->createChildSceneNode("DeskNode");
	nodeDesk->attachObject(entDesk);
	nodeDesk->yaw(Ogre::Degree(90));
	nodeDesk->translate(Ogre::Vector3(-xLen/2+entDesk->getBoundingBox().getHalfSize().x-20,0,-zLen/4));

	Ogre::Entity* entShelfKeyBlue = mSceneMgr->createEntity(SHELF_KEY_BLUE_ENTITY, "key.mesh");
	entShelfKeyBlue->setCastShadows(true);
	entShelfKeyBlue->setMaterialName("Material/KeyBlue");
	Ogre::SceneNode* nodeShelfKeyBlue = nodeDesk->createChildSceneNode(SHELF_KEY_BLUE_NODE, Ogre::Vector3(0,0,0));
	nodeShelfKeyBlue->attachObject(entShelfKeyBlue);
	nodeShelfKeyBlue->yaw(Ogre::Degree(45));
	nodeShelfKeyBlue->translate(0, entDesk->getBoundingBox().getSize().y-1,0);

	Ogre::Entity* entShelfKey = mSceneMgr->createEntity(SHELF_KEY_ENTITY, "key.mesh");
	entShelfKey->setCastShadows(true);
	Ogre::SceneNode* nodeShelfKey = nodeDesk->createChildSceneNode(SHELF_KEY_NODE, Ogre::Vector3(0,0,0));
	nodeShelfKey->yaw(Ogre::Degree(45));
	nodeShelfKey->translate(0, entDesk->getBoundingBox().getSize().y-1,0);

	Ogre::Entity* entDeskLamp = mSceneMgr->createEntity(DESK_LAMP_ENTITY, "lamp.mesh");
	entDeskLamp->setCastShadows(true);
	Ogre::SceneNode* nodeDeskLamp = nodeDesk->createChildSceneNode("DeskLampNode");
	nodeDeskLamp->attachObject(entDeskLamp);
	nodeDeskLamp->yaw(Ogre::Degree(-135));
	nodeDeskLamp->translate(Ogre::Vector3(-40,entDesk->getBoundingBox().getSize().y-1,-10));

	Ogre::Entity* entDeskChair = mSceneMgr->createEntity("DeskChair", "chair.mesh");
	entDeskChair->setCastShadows(true);
	Ogre::SceneNode* nodeDeskChair = nodeDesk->createChildSceneNode("DeskChairNode",Ogre::Vector3(0,entDeskChair->getBoundingBox().getHalfSize().y-5,0));
	nodeDeskChair->attachObject(entDeskChair);
	nodeDeskChair->translate(Ogre::Vector3(-20,0,30));

	Ogre::Entity* entFrame = mSceneMgr->createEntity("Frame", "frame.mesh");
	Ogre::SceneNode* nodeFrame = mSceneMgr->getRootSceneNode()->createChildSceneNode("FrameNode");
	nodeFrame->attachObject(entFrame);
	nodeFrame->yaw(Ogre::Degree(90));
	nodeFrame->translate(Ogre::Vector3(xLen/3,yLen/2,-zLen/2+1));

	Ogre::Entity* entExitDoor = mSceneMgr->createEntity(EXIT_DOOR_ENTITY, "door.mesh");
	Ogre::SceneNode* nodeExitDoor = mSceneMgr->getRootSceneNode()->createChildSceneNode("ExitDoorNode", Ogre::Vector3(0,entExitDoor->getBoundingBox().getHalfSize().y-6,0));
	nodeExitDoor->attachObject(entExitDoor);
	nodeExitDoor->translate(Ogre::Vector3(0,0,-zLen/2-5));

	Ogre::Plane interruptor(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("interruptorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, interruptor, 10, 10, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entInterruptorEntity = mSceneMgr->createEntity(INTERRUPTOR_ENTITY, "interruptorPlane");
	Ogre::SceneNode* nodeInterruptor = mSceneMgr->getRootSceneNode()->createChildSceneNode("InterruptorNode");
	nodeInterruptor->attachObject(entInterruptorEntity);
	nodeInterruptor->yaw(Ogre::Degree(-90));
	nodeInterruptor->translate(Ogre::Vector3(60, mSceneMgr->getEntity(PLAYER_MESH_NAME)->getBoundingBox().getSize().y * Ogre::Real(0.65), -zLen/2+1));
	entInterruptorEntity->setMaterialName("Textures/Interruptor");
	entInterruptorEntity->setCastShadows(false);
}

void ExampleScene::putLights(void)
{
	Ogre::Light* lsun = mSceneMgr->createLight("SunLight");
	lsun->setType(Ogre::Light::LT_DIRECTIONAL);
	lsun->setDiffuseColour(0.7f, 0.7f, 0.7f);
	lsun->setSpecularColour(0.7f, 0.7f, 0.7f);
	lsun->setDirection(0, -1, -5);
	lsun->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
	
	//Ogre::Light* lRed = mSceneMgr->createLight("RedLight");
	//lRed->setType(Ogre::Light::LT_SPOTLIGHT);
	//lRed->setPosition(xLen/4, yLen, -zLen/4);
	//lRed->setDirection(2, -1, 0);
	//lRed->setDiffuseColour(1.0, 0.0, 0.0);
	//lRed->setSpecularColour(1.0, 0.0, 0.0);	
 //   lRed->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
	//lRed->setAttenuation(Ogre::Real(3250), Ogre::Real(1.0), Ogre::Real(0.0014), Ogre::Real(0.000007));
	//lRed->setVisible(false);
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

	Ogre::Plane redHolePlane(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("redHole", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, redHolePlane, 10, 10, 200, 200, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entRedHole = mSceneMgr->createEntity(HOLE_RED_ENTITY, "redHole");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode(HOLE_RED_NODE);
	node->attachObject(entRedHole);
	node->yaw(Ogre::Degree(-0.1f));
	node->translate(Ogre::Vector3(-xLen/2+2, mSceneMgr->getEntity(PLAYER_MESH_NAME)->getBoundingBox().getSize().y * Ogre::Real(0.65), 50));
	entRedHole->setMaterialName("Textures/RedHole");
	entRedHole->setCastShadows(false);
	entRedHole->setVisible(false);

	Ogre::Plane blueHolePlane(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("blueHole", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, blueHolePlane, 10, 10, 200, 200, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entBlueHole = mSceneMgr->createEntity(HOLE_BLUE_ENTITY, "blueHole");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode(HOLE_BLUE_NODE);
	node->attachObject(entBlueHole);
	node->yaw(Ogre::Degree(-90));
	node->translate(Ogre::Vector3(xLen/2-20, mSceneMgr->getEntity(PLAYER_MESH_NAME)->getBoundingBox().getSize().y*Ogre::Real(0.65), -zLen/2+1));
	entBlueHole->setMaterialName("Textures/BlueHole");
	entBlueHole->setCastShadows(false);
	entBlueHole->setVisible(false);

	Ogre::Plane greenHolePlane(Ogre::Vector3::UNIT_X, 0);
	Ogre::MeshManager::getSingleton().createPlane("greenHole", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, greenHolePlane, 10, 10, 200, 200, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* entGreenHole = mSceneMgr->createEntity(HOLE_GREEN_ENTITY, "greenHole");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode(HOLE_GREEN_NODE);
	node->attachObject(entGreenHole);
	node->yaw(Ogre::Degree(180));
	node->translate(Ogre::Vector3(xLen/2-2, mSceneMgr->getEntity(PLAYER_MESH_NAME)->getBoundingBox().getSize().y * Ogre::Real(0.65), zLen/2-20));
	entGreenHole->setMaterialName("Textures/GreenHole");
	entGreenHole->setCastShadows(false);
	entGreenHole->setVisible(false);
}