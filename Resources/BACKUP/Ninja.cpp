#include "Ninja.h"

USING_NS_CC;

Ninja::Ninja(Layer *layer)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto spritecache = SpriteFrameCache::getInstance();

	spritecache->addSpriteFramesWithFile("NinjaMovement.plist");

	ninja = Sprite::create();

	ninja->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	layer->addChild(ninja);
}

void Ninja::Idle()
{

}

void Ninja::Run()
{

}