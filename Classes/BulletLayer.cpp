#include "BulletLayer.h"
#include "PlaneLayer.h"

BulletLayer::BulletLayer(void)
{
	//bulletSpriteFrame=NULL;
	bulletBatchNode=NULL;

	m_pAllBullet=CCArray::create();
	m_pAllBullet->retain();
}

BulletLayer::~BulletLayer(void)
{
	m_pAllBullet->release();
	m_pAllBullet=NULL;
}

bool BulletLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		//bulletSpriteFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet1.png");

		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey("ui/shoot.png");
		bulletBatchNode = CCSpriteBatchNode::createWithTexture(texture);
		this->addChild(bulletBatchNode);

		bRet=true;
	} while (0);
	return bRet;
}

void BulletLayer::StartShoot(float delay)
{
	this->schedule(schedule_selector(BulletLayer::AddBullet),0.20f,kCCRepeatForever,delay);
}

void BulletLayer::StopShoot()
{
	this->unschedule(schedule_selector(BulletLayer::AddBullet));
}

void BulletLayer::AddBullet(float dt)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/bullet.wav");
	//CCLog("playeffect");

	CCSprite* bullet=CCSprite::createWithSpriteFrameName("bullet1.png");
	bulletBatchNode->addChild(bullet);
	
	this->m_pAllBullet->addObject(bullet);

	CCNode *node=PlaneLayer::sharePlane->getChildByTag(AIRPLANE);
	CCPoint planePosition=node->getPosition();//得到飞机的位置
	CCPoint bulletPosition=ccp(planePosition.x,planePosition.y+node->getContentSize().height/2);//子弹的位置，为飞机机头位置
	bullet->setPosition(bulletPosition);

	float length=CCDirector::sharedDirector()->getWinSize().height+bullet->getContentSize().height/2-bulletPosition.y;//子弹消失的位置
	float velocity=320/1;//320pixel/sec
	float realMoveDuration=length/velocity;

	CCFiniteTimeAction* actionMove=CCMoveTo::create(realMoveDuration,ccp(bulletPosition.x,CCDirector::sharedDirector()->getWinSize().height+bullet->getContentSize().height/2));
	CCFiniteTimeAction* actionDone=CCCallFuncN::create(this,callfuncN_selector(BulletLayer::bulletMoveFinished));

	CCSequence* sequence=CCSequence::create(actionMove,actionDone,NULL);
	bullet->runAction(sequence);
}

//子弹超出屏幕后清除
void BulletLayer::bulletMoveFinished(CCNode* pSender)
{
	CCSprite* bullet=(CCSprite*)pSender;
	m_pAllBullet->removeObject(bullet);
	this->removeChild(bullet,true);
}


//子弹碰撞后清除
void BulletLayer::RemoveBullet(CCSprite* bullet)
{
	if (bullet!=NULL)
	{
		this->m_pAllBullet->removeObject(bullet);
		this->removeChild(bullet,true);
	}
}
