#include "GameScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0, -1000));

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Point center = Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	auto backgroundSprite = Sprite::create("background.png");
	//backgroundSprite->setAnchorPoint(Vec2(0.5, 0.5));
	backgroundSprite->setPosition(Point((origin.x + visibleSize.width) / 2, (origin.y + visibleSize.height) / 2));
	this->addChild(backgroundSprite, 0);

	auto map = TMXTiledMap::create("TestStage.tmx");
	map->setAnchorPoint(Vec2(0.5, 0.5));
	map->setPosition(Point((origin.x + visibleSize.width) / 2, (origin.y + visibleSize.height) / 2));
	this->addChild(map);

	auto platformLayer = map->layerNamed("platform");
	auto groundLayer = map->layerNamed("ground");

	for (int x = 0; x < 60; x++) {
		for (int y = 0; y < 34; y++) {
			auto platformTile = platformLayer->getTileAt(Vec2(x, y));
			if (platformTile != NULL) {
				PhysicsBody* tilePhysics = PhysicsBody::createBox(Size(32.0f, 14.0f), PHYSICSBODY_MATERIAL_DEFAULT);
				tilePhysics->setPositionOffset(Vec2(0, 7));
				tilePhysics->setDynamic(false);
				tilePhysics->setGroup(PLATFORM_GROUP);
				tilePhysics->setCategoryBitmask(PLATFORM_BITMASK);
				tilePhysics->setContactTestBitmask(SHURIKEN_BITMASK);
				//tilePhysics->setCollisionBitmask(SHURIKEN_BITMASK);
				platformTile->setPhysicsBody(tilePhysics);
			}
			auto groundTile = groundLayer->getTileAt(Vec2(x, y));
			if (groundTile != NULL) {
				PhysicsBody* tilePhysics = PhysicsBody::createBox(Size(32.0f, 32.0f), PHYSICSBODY_MATERIAL_DEFAULT);
				tilePhysics->setDynamic(false);
				tilePhysics->setGroup(GROUND_GROUP);
				tilePhysics->setCategoryBitmask(PLATFORM_BITMASK);
				tilePhysics->setContactTestBitmask(SHURIKEN_BITMASK);
				//tilePhysics->setCollisionBitmask(SHURIKEN_BITMASK);
				groundTile->setPhysicsBody(tilePhysics);
			}
		}
	}

	//map->layerNamed("platforms");

	float playfield_width = backgroundSprite->getContentSize().width; // make the x-boundry 2 times the screen width
	float playfield_height = backgroundSprite->getContentSize().height; // make the y-boundry 2 times the screen height

	auto borderBody = PhysicsBody::createEdgeBox(backgroundSprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto borderNode = Node::create();
	borderNode->setPosition(Point((origin.x + visibleSize.width) / 2, (origin.y + visibleSize.height) / 2));
	borderNode->setPhysicsBody(borderBody);
	this->addChild(borderNode);

	auto playerNode = Node::create();
	playerNode->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(playerNode);

	this->runAction(Follow::create(playerNode, Rect(center.x - playfield_width / 2, center.y - playfield_height / 2, playfield_width, playfield_height)));

	ninja = Ninja::create(playerNode);
	//ninja->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	//ninja->setContentSize(Size(80, 80));
	//auto ninjaBody = PhysicsBody::createBox(ninja->getContentSize(), PhysicsMaterial(0,0,0));
	//playerNode->setPhysicsBody(ninjaBody);
	playerNode->addChild(ninja);

	this->scheduleUpdate();


	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
			case EventKeyboard::KeyCode::KEY_A:
				ninja->run(0); //0 == left
				//playerNode->setPositionX(playerNode->getPositionX() - 5);
				break;
			case EventKeyboard::KeyCode::KEY_D:
				ninja->run(1); //1 == right
				break;
			case EventKeyboard::KeyCode::KEY_S:
				ninja->down(); //1 == right
				break;
			case EventKeyboard::KeyCode::KEY_SPACE:
				ninja->jump(); //1 == right
				break;
			case EventKeyboard::KeyCode::KEY_J:
				ninja->attack(); //1 == right
				break;
		}
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_A:
			ninja->idle();
			//ninjaBody->setVelocity(Vec2(0, 0));
			break;
		case EventKeyboard::KeyCode::KEY_D:
			ninja->idle();
			//ninjaBody->setVelocity(Vec2(0, 0));
			break;
		case EventKeyboard::KeyCode::KEY_S:
			ninja->undown();
			//ninjaBody->setVelocity(Vec2(0, 0));
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, ninja);

	//auto shurikenContactListener = EventListenerPhysicsContactWithGroup::create(SHURIKEN_GROUP);
	//auto contactListener = EventListenerPhysicsContactWithGroup::create(SHURIKEN_GROUP);
	//shurikenContactListener->onContactBegin == CC_CALLBACK_0();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

bool GameScene::onContactBegin(PhysicsContact &contact)
{
	auto shapeABody = contact.getShapeA()->getBody();
	auto shapeBBody = contact.getShapeB()->getBody();
	if (shapeABody->getGroup() == SHURIKEN_GROUP) {
		shapeABody->removeFromWorld();
		shapeABody->getOwner()->pause();
	}
	if (shapeBBody->getGroup() == SHURIKEN_GROUP) {
		shapeBBody->removeFromWorld();
		shapeBBody->getOwner()->pause();
	}
	//PhysicsBody *a = contact.getShapeA
	return true;
}

void GameScene::update(float dt)
{
	ninja->update();
}










