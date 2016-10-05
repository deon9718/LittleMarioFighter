#ifndef __NINJA_H__
#define __NINJA_H__

#include "cocos2d.h"

class Ninja
{
public:
	Ninja(cocos2d::Layer *layer);

	void Idle();
	void Run();

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	cocos2d::Sprite *ninja;

	//cocos2d::SpriteFrameCache spritecache = SpriteFrameCache::getInstance();

	bool isBusy;

};

#endif // __NINJA_H__