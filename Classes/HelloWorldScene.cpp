#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
	Vector<MenuItem*> MenuItems;

	int index = 2;

	auto playLabel = Label::createWithTTF("Play", "fonts/Marker Felt.ttf", 32);
	auto playMenuItem = MenuItemLabel::create(playLabel);
	playMenuItem->setPosition(0,50);

	auto settingsLabel = Label::createWithTTF("Settings", "fonts/Marker Felt.ttf", 32);
	auto settingsMenuItem = MenuItemLabel::create(settingsLabel);
	settingsMenuItem->setPosition(0,0);

	auto quitLabel = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
	auto quitMenuItem = MenuItemLabel::create(quitLabel);
	quitMenuItem->setPosition(0,-50);

	MenuItems.pushBack(playMenuItem);
	MenuItems.pushBack(settingsMenuItem);
	MenuItems.pushBack(quitMenuItem);

    // create menu, it's an autorelease object
	auto menu = Menu::createWithArray(MenuItems);//(closeItem, NULL);
    //menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Little Ninja World", "fonts/Marker Felt.ttf", 54);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto ninja = Sprite::create("Ninja.png");

    // position the sprite on the center of the screen
    ninja->setPosition(Vec2(60, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(ninja, 0);

	auto bandit = Sprite::create("Bandit.png");
	bandit->setPosition(Vec2(visibleSize.width-60, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(bandit, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
