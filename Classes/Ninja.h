#ifndef __NINJA_H__
#define __NINJA_H__

#include "cocos2d.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h" 

class Ninja : public cocos2d::Sprite
{
public:
	static Ninja * create(cocos2d::Node *player);

	void idle();
	void run(int dirParam);
	void stop();
	void jump();
	void land();

	void down();
	void undown();

	void attack();

	void setGrounded(bool grounded);

	void update();


private:
	void initNinja(cocos2d::Node *player);

	~Ninja();

	int dir = 1;
	int jumpCount = 0;

	bool moving = false;

	bool isGrounded = true;
	bool landing = false;
	bool isIdle = true;
	bool downHold = false;
	bool attackFinish = true;
	bool jumpAttacking = false;
	bool subJumpAttack = false;

	cocos2d::PhysicsBody * idleBody;
	cocos2d::PhysicsBody * runBody;

	cocos2d::Animate * idleAnimate;
	cocos2d::Animate * moveAnimate;
	cocos2d::Animate * jumpAnimate;
	cocos2d::Animate * landAnimate;

	cocos2d::Animate * attackAnimate;
	cocos2d::Animate * jumpattackAnimate;
	cocos2d::Animate * subjumpattackAnimate;
	//cocos2d::Animate * shurikenAnimate;

	//bool isBusy;

	CocosDenshion::SimpleAudioEngine * audio;

};

#endif // __NINJA_H__