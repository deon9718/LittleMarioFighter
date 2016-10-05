#include "MenuScene.h"
#include "GameScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto label = Label::createWithTTF("Little Ninja World", "fonts/Marker Felt.ttf", 54);

	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	this->addChild(label, 1);


	Vector<MenuItem*> MenuItems;

	auto playLabel = Label::createWithTTF("Play", "fonts/Marker Felt.ttf", 32);
	auto playMenuItem = MenuItemLabel::create(playLabel, CC_CALLBACK_1(MenuScene::GoToGameScene, this));
	playMenuItem->setPosition(0, 50);

	auto settingsLabel = Label::createWithTTF("Settings", "fonts/Marker Felt.ttf", 32);
	auto settingsMenuItem = MenuItemLabel::create(settingsLabel);
	settingsMenuItem->setPosition(0, 0);

	auto quitLabel = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
	auto quitMenuItem = MenuItemLabel::create(quitLabel);
	quitMenuItem->setPosition(0, -50);

	MenuItems.pushBack(playMenuItem);
	MenuItems.pushBack(settingsMenuItem);
	MenuItems.pushBack(quitMenuItem);

	auto menu = Menu::createWithArray(MenuItems);
	this->addChild(menu, 1);

	auto ninja = Sprite::create("Ninja.png");
	ninja->setPosition(Vec2(60, visibleSize.height / 2 + origin.y));
	this->addChild(ninja, 0);

	auto bandit = Sprite::create("Bandit.png");
	bandit->setPosition(Vec2(visibleSize.width - 60, visibleSize.height / 2 + origin.y));
	this->addChild(bandit, 0);

	//auto backgroundSprite = Sprite::create("Background.png");
	//backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	//this->addChild(backgroundSprite);

	//auto titleSprite = Sprite::create("Title.png");
	//titleSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height - titleSprite->getContentSize().height));

	//this->addChild(titleSprite);

	//auto playItem = MenuItemImage::create("Play Button.png", "Play Button Clicked.png", CC_CALLBACK_1(MenuScene::GoToGameScene, this));
	//playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	//auto menu = Menu::create(playItem, NULL);
	//menu->setPosition(Point::ZERO);

	//this->addChild(menu);

	return true;
}

void MenuScene::GoToGameScene(cocos2d::Ref *sender)
{
	auto scene = GameScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}



