#include "Ninja.h"

USING_NS_CC;

static cocos2d::Node *player;

Ninja * Ninja::create(Node *playerNode)
{
	Ninja * ninja = new Ninja();
	if (ninja && ninja->initWithFile("Ninja_idle1.png"))
	{
		player = playerNode;

		ninja->autorelease();
		ninja->initNinja(playerNode);
		
		return ninja;
	}

	CC_SAFE_DELETE(ninja);
	return NULL;
}

void Ninja::initNinja(Node *player)
{
	audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("jump.wav");
	audio->preloadEffect("throw.wav");
	audio->preloadEffect("jumpAttack.wav");
	audio->preloadEffect("subJumpAttack.wav");



	auto spritecache_movement = SpriteFrameCache::getInstance();
	spritecache_movement->addSpriteFramesWithFile("NinjaMovement.plist");

	auto spritecache_attacks = SpriteFrameCache::getInstance();
	spritecache_attacks->addSpriteFramesWithFile("NinjaAttacks.plist");

	auto spritecache_shuriken = SpriteFrameCache::getInstance();
	spritecache_shuriken->addSpriteFramesWithFile("NinjaShuriken.plist");

	char str[100] = { 0 };

	idleBody = PhysicsBody::createBox(spritecache_movement->getSpriteFrameByName("Ninja_idle1.png")->getOriginalSize(), PhysicsMaterial(0, 0, 0.8));
	//idleBody->setMass(20);
	idleBody->setRotationEnable(false);
	idleBody->setGroup(PLAYER_GROUP);
	
	idleBody->setContactTestBitmask(PLATFORM_BITMASK);
	
	player->setPhysicsBody(idleBody);

	//runBody = PhysicsBody::createBox(spritecache_movement->getSpriteFrameByName("Ninja_run1.png")->getOriginalSize(), PhysicsMaterial(0, 0, 0));


	Vector<SpriteFrame*> idleAnimFrames(6);
	for (int i = 1; i <= 4; i++) //Iterate for the number of images you have
	{
		sprintf(str, "Ninja_idle%01d.png", i);
		idleAnimFrames.pushBack(spritecache_movement->getSpriteFrameByName(str));
	}

	auto idleAnimation = Animation::createWithSpriteFrames(idleAnimFrames, 0.2f);
	idleAnimate = Animate::create(idleAnimation);
	idleAnimate->retain(); //Retain to use it later
	this->runAction(RepeatForever::create(idleAnimate)); //This will be the starting animation

	Vector<SpriteFrame*> moveAnimFrames(6);
	for (int i = 1; i <= 3; i++)
	{
		sprintf(str, "Ninja_run%01d.png", i);
		moveAnimFrames.pushBack(spritecache_movement->getSpriteFrameByName(str));
	}

	auto moveAnimation = Animation::createWithSpriteFrames(moveAnimFrames, 0.15f);
	moveAnimate = Animate::create(moveAnimation);
	moveAnimate->retain();

	Vector<SpriteFrame*> jumpAnimFrames(6);
	for (int i = 1; i <= 3; i++)
	{
		sprintf(str, "Ninja_jump%01d.png", i);
		jumpAnimFrames.pushBack(spritecache_movement->getSpriteFrameByName(str));
	}

	auto jumpAnimation = Animation::createWithSpriteFrames(jumpAnimFrames, 0.08f);
	jumpAnimate = Animate::create(jumpAnimation);
	jumpAnimate->retain();

	Vector<SpriteFrame*> landAnimFrames(6);

	landAnimFrames.pushBack(spritecache_movement->getSpriteFrameByName("Ninja_land1.png"));
	landAnimFrames.pushBack(spritecache_movement->getSpriteFrameByName("Ninja_idle1.png"));

	auto landAnimation = Animation::createWithSpriteFrames(landAnimFrames, 0.5f);
	landAnimate = Animate::create(landAnimation);
	landAnimate->retain();

	Vector<SpriteFrame*> attackAnimFrames(6);

	for (int i = 1; i <= 4; i++)
	{
		sprintf(str, "Ninja_throw%01d.png", i);
		attackAnimFrames.pushBack(spritecache_attacks->getSpriteFrameByName(str));
	}

	auto attackAnimation = Animation::createWithSpriteFrames(attackAnimFrames, 0.05f);
	attackAnimate = Animate::create(attackAnimation);
	attackAnimate->retain();

	Vector<SpriteFrame*> jumpattackAnimFrames(6);

	for (int i = 1; i <= 3; i++)
	{
		sprintf(str, "Ninja_jumpatk%01d.png", i);
		jumpattackAnimFrames.pushBack(spritecache_attacks->getSpriteFrameByName(str));
	}

	auto jumpattackAnimation = Animation::createWithSpriteFrames(jumpattackAnimFrames, 0.10f);
	jumpattackAnimate = Animate::create(jumpattackAnimation);
	jumpattackAnimate->retain();

	Vector<SpriteFrame*> subjumpattackAnimFrames(6);

	for (int i = 4; i <= 7; i++)
	{
		sprintf(str, "Ninja_jumpatk%01d.png", i);
		subjumpattackAnimFrames.pushBack(spritecache_attacks->getSpriteFrameByName(str));
	}

	auto subjumpattackAnimation = Animation::createWithSpriteFrames(subjumpattackAnimFrames, 0.10f);
	subjumpattackAnimate = Animate::create(subjumpattackAnimation);
	subjumpattackAnimate->retain();

	//Vector<SpriteFrame*> shurikenAnimFrames(6);

	//for (int i = 1; i <= 4; i++)
	//{
	//	sprintf(str, "Ninja_star%01d.png", i);
	//	shurikenAnimFrames.pushBack(spritecache_shuriken->getSpriteFrameByName(str));
	//}

	//auto shurikenAnimation = Animation::createWithSpriteFrames(shurikenAnimFrames, 0.2f);
	//shurikenAnimate = Animate::create(shurikenAnimation);
	//shurikenAnimate->retain();

}

Ninja::~Ninja()
{
	//if retained, must release
	CC_SAFE_RELEASE(idleAnimate);
	CC_SAFE_RELEASE(moveAnimate);
	CC_SAFE_RELEASE(jumpAnimate);
	CC_SAFE_RELEASE(landAnimate);
	CC_SAFE_RELEASE(attackAnimate);
	//CC_SAFE_RELEASE(shurikenAnimate);
}

void Ninja::idle()
{
	moving = false;
	if (attackFinish) {
		isIdle = true;
		this->stopAllActions();
		this->runAction(RepeatForever::create(idleAnimate));
		//player->removeComponent(player->getPhysicsBody());
		//player->setPhysicsBody(idleBody);
		//idleBody->setVelocity(Vec2(0, 0));
	}
}

void Ninja::run(int dirParam)
{
	if (attackFinish) {
		isIdle = false;
		this->stopAllActions();
		auto runAct = RepeatForever::create(moveAnimate);
		runAct->setTag(1);
		this->runAction(runAct);

		//player->removeComponent(player->getPhysicsBody());
		//player->setPhysicsBody(runBody);

		dir = dirParam;
		moving = true;
	}
}

void Ninja::setGrounded(bool grounded)
{
	isGrounded = grounded;
}

void Ninja::stop()
{
	isIdle = true;

	//player->removeComponent(player->getPhysicsBody());
	//player->setPhysicsBody(runBody);
	moving = false;
}

void Ninja::jump()
{
	if (attackFinish) {
		if (jumpCount < 2) {
			isIdle == false;
			isGrounded = false;
			jumpCount++;
			this->stopAllActions();
			this->runAction(Repeat::create(jumpAnimate, 1));
			idleBody->setVelocity(Vec2(idleBody->getVelocity().x, 0));
			idleBody->applyImpulse(Vec2(0, 500));
			audio->playEffect("jump.wav");
		}
	}
}

void Ninja::down()
{
	downHold = true;
	idleBody->setGroup(PLATFORM_GROUP);
}

void Ninja::undown()
{
	downHold = false;
	idleBody->setGroup(PLAYER_GROUP);
}

void Ninja::land()
{
	isIdle == false;
	this->stopAllActions();
	this->runAction(Repeat::create(landAnimate, 1));
}

void Ninja::attack()
{	
	//log("attackPress");
	if (attackFinish) {
		if (isGrounded) {
			attackFinish = false;
			isIdle == false;
			auto attack = Repeat::create(attackAnimate, 1);

			auto attackDelay = DelayTime::create(0.2);
			auto spawnShuriken = CallFunc::create([=]() {
				//auto shuriken = Node::create();
				auto shuriken = Sprite::create("Ninja_shuriken.png");
				//shuriken->setPosition(this->convertToWorldSpace(Vec2(this->getPositionX() + 40, this->getPositionY())));
				//auto shurikenSprite = Sprite::create();
				shuriken->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
				//shuriken->addChild(shurikenSprite);
				auto shurikenBody = PhysicsBody::createCircle(12, PhysicsMaterial(0.1, 0, 1));
				shurikenBody->setGroup(SHURIKEN_GROUP);
				shurikenBody->setCategoryBitmask(SHURIKEN_BITMASK);
				//shurikenBody->setCollisionBitmask(PLATFORM_BITMASK);
				shurikenBody->setContactTestBitmask(PLATFORM_BITMASK);
				shuriken->setPhysicsBody(shurikenBody);
				this->getParent()->getParent()->addChild(shuriken);

				auto playerPos = this->getParent()->getParent()->convertToWorldSpace(this->getParent()->getPosition());
				auto shurikenPos = this->getParent()->getParent()->convertToNodeSpace(playerPos);
				shuriken->setPosition(shurikenPos);
				audio->playEffect("throw.wav");

				//shuriken->setPosition(this->getParent()->getParent()->convertToWorldSpace(this->getPosition()));
				if (dir == 0) {
					shurikenBody->applyImpulse(Vec2(-100000, 0));
				}
				else {
					shurikenBody->applyImpulse(Vec2(100000, 0));
				}
				//shuriken->runAction(MoveBy::create(1, Vec3(500, 0, 0)));

			});
			auto callbackAttack = CallFunc::create([=]() {
				attackFinish = true;
				if (moving) { this->runAction(RepeatForever::create(moveAnimate)); }
				else { this->runAction(RepeatForever::create(idleAnimate)); }
			});
			auto attackSeq = Sequence::create(attack, spawnShuriken, attackDelay, callbackAttack, nullptr);
			this->stopAllActions();
			this->runAction(attackSeq);
		}
		else {
			attackFinish = false;
			isIdle == false;
			jumpAttacking = true;
			//log("set True");
			auto jumpattack = Repeat::create(jumpattackAnimate, 1);
			auto jumpattackDelay = DelayTime::create(0.2);
			auto callbackJumpAttack = CallFunc::create([=]() {
				//log("callback");
				if (subJumpAttack) { 
					auto subjumpattack = Repeat::create(subjumpattackAnimate, 1);
					//auto subjumpattackDelay = DelayTime::create(0.3);
					auto callbackSubJumpAttack = CallFunc::create([=]() {
						attackFinish = true;
						jumpAttacking = false;
						subJumpAttack = false;
						if (moving) { this->runAction(RepeatForever::create(moveAnimate)); }
						else { this->runAction(RepeatForever::create(idleAnimate)); }
					});
					auto subjumpattackSeq = Sequence::create(subjumpattack, callbackSubJumpAttack, nullptr);
					this->stopAllActions();
					this->runAction(subjumpattackSeq);
					audio->playEffect("subJumpAttack.wav");
				}else if (moving) { 
					this->runAction(RepeatForever::create(moveAnimate));
					jumpAttacking = false;
					attackFinish = true;
				}else { 
					this->runAction(RepeatForever::create(idleAnimate));
					jumpAttacking = false;
					attackFinish = true;
				}
			});
			auto jumpattackSeq = Sequence::create(jumpattack, jumpattackDelay, callbackJumpAttack, nullptr);
			this->stopAllActions();
			this->runAction(jumpattackSeq);
			audio->playEffect("jumpAttack.wav");
		}
	}
	else if (jumpAttacking) {
		subJumpAttack = true;
		//log("subattack");
	}
}


void Ninja::update()
{
	if (moving) //check if moving
	{
		if (dir == 0) //check if going left
		{
			this->setScaleX(-1); //flip
			idleBody->applyImpulse(Vec2(-3000, 0));
			//this->setPositionX(this->getPositionX() - 5);
		}
		else
		{
			this->setScaleX(1); //flip
			idleBody->applyImpulse(Vec2(3000, 0));
			//idleBody->setVelocity(Vec2(200, 0));
			//this->setPositionX(this->getPositionX() + 5);
		}
	}
	if (idleBody->getVelocity().x > 200) {
		idleBody->setVelocity(Vec2(300, idleBody->getVelocity().y));
	}
	else if (idleBody->getVelocity().x < -200) {
		idleBody->setVelocity(Vec2(-300, idleBody->getVelocity().y));
	}
	if (isGrounded) {
		jumpCount = 0;
	}
	if (idleBody->getVelocity().y > 0) {
		idleBody->setGroup(PLATFORM_GROUP);
	}
	else if (!downHold) {
		idleBody->setGroup(PLAYER_GROUP);
	}

}