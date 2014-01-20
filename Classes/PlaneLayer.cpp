#include "PlaneLayer.h"
#include "GameOverScene.h"



PlaneLayer* PlaneLayer::sharePlane=NULL;


PlaneLayer::PlaneLayer(){
	isAlive=true;
	score=0;
}

PlaneLayer::~PlaneLayer(){

}


PlaneLayer* PlaneLayer::create(){
	PlaneLayer *pRet = new PlaneLayer();  
	if (pRet && pRet->init())  
	{  
		pRet->autorelease();  
		sharePlane=pRet;//获得静态指针sharedPlane的值  
		return pRet;  
	}  
	else  
	{  
		CC_SAFE_DELETE(pRet);  
		return NULL;  
	}
}

bool PlaneLayer::init(){

	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		CCSprite *plane=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
		plane->setPosition(ccp(winSize.width/2,plane->getContentSize().height/2));
		this->addChild(plane,0,AIRPLANE);

		CCBlink *blink=CCBlink::create(1,3);

		CCAnimation *animation=CCAnimation::create();
		animation->setDelayPerUnit(0.1f);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero2.png"));
		CCAnimate *animate=CCAnimate::create(animation);

		plane->runAction(blink);
		plane->runAction(CCRepeatForever::create(animate));

		bRet=true;
	} while (0);

	return bRet;
}


//主角死亡，播放动画
void PlaneLayer::Blowup(int passScore){
	if (isAlive)
	{
		isAlive=false;
		score=passScore;

		CCAnimation *animation=CCAnimation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n1.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n2.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n3.png"));
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n4.png"));


		CCAnimate *animate=CCAnimate::create(animation);
		CCCallFunc *removePlane=CCCallFunc::create(this,callfunc_selector(PlaneLayer::RemovePlane));
		CCSequence *sequence=CCSequence::create(animate,removePlane,NULL);
		this->getChildByTag(AIRPLANE)->stopAllActions();
		this->getChildByTag(AIRPLANE)->runAction(sequence);
	}
}

void PlaneLayer::MoveTo(CCPoint location){
	if (isAlive&&!CCDirector::sharedDirector()->isPaused())
	{
		CCPoint actualPoint;
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();
		CCNode *plane=getChildByTag(AIRPLANE);
		CCSize planeSize=plane->getContentSize();
		if (location.x<planeSize.width/2)
		{
			location.x=planeSize.width/2;
		}

		float maxX=winSize.width-planeSize.width/2;
		if (location.x>maxX)
		{
			location.x=maxX;
		}

		if (location.y<planeSize.height/2)
		{
			location.y=planeSize.height/2;
		}

		float maxY=winSize.height-planeSize.height/2;
		if (location.y>maxY)
		{
			location.y=maxY;
		}

		plane->setPosition(location);
	}
}


//hero爆炸后，移除hero，用gameover场景替换当前game场景
void PlaneLayer::RemovePlane(){
	this->removeChildByTag(AIRPLANE,true);
	GameOverScene* pScene=GameOverScene::create(score);
	CCTransitionMoveInT* animateScene=CCTransitionMoveInT::create(0.8f,pScene);
	CCDirector::sharedDirector()->replaceScene(animateScene);
}
