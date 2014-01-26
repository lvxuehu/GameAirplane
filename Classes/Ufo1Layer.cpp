#include "Ufo1Layer.h"
#include "cocos2d.h"

USING_NS_CC;


Ufo1Layer::Ufo1Layer(void)
{
	//p_ufo=NULL;
}


Ufo1Layer::~Ufo1Layer(void)
{
}


bool Ufo1Layer::init(){
	bool pRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		pRet=true;
	} while (0);
	return pRet;
}

//根据积分计算是否出现ufo1
void Ufo1Layer::ufo1Ai(int score){

	if (score>0&&score%UFO1_OUT==0)
	{
		addUfo1();
	}
	 
}



void Ufo1Layer::addUfo1(){
	CCSprite* p_ufo=CCSprite::createWithSpriteFrameName("ufo1.png");
	//计算ufo出现的起始位置
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	int minStartX=p_ufo->getContentSize().width/2;
	int maxStartX=winSize.width-p_ufo->getContentSize().width/2;
	int rangex=maxStartX-minStartX;
	
	//随机生成起始x值
	int startX=(rand()%rangex)+minStartX;
	p_ufo->setPosition(ccp(startX,winSize.height+p_ufo->getContentSize().height/2));
	this->addChild(p_ufo,1,UFO);

	//计算消失的位置
	int endX=startX;
	int endY=0-p_ufo->getContentSize().height/2;

	//飞行的时间(速度)
	float durationTime=2.0;

	//降落伞效果
	CCMoveBy* move1=CCMoveBy::create(0.2f,ccp(0,-150));
	CCMoveBy* move2=CCMoveBy::create(0.7f,ccp(0,100));
	CCSequence* upAndDownAction=CCSequence::create(move1,move2,NULL);

	//摇摆效果
	p_ufo->setRotation(20);
	CCActionInterval* rotate2=CCRotateBy::create(0.5f,-40);
	CCSequence* rotateSequence=CCSequence::create(rotate2,rotate2->reverse(),NULL);
	p_ufo->runAction(CCRepeatForever::create(rotateSequence));


	CCFiniteTimeAction* moveTo=CCMoveTo::create(durationTime,ccp(endX,endY));
	CCFiniteTimeAction* moveEnd=CCCallFuncN::create(this,callfuncN_selector(Ufo1Layer::ufoMoveFinished));
	CCSequence* sequence=CCSequence::create(upAndDownAction,moveTo,moveEnd,NULL);	

	p_ufo->runAction(sequence);

}

void Ufo1Layer::ufoMoveFinished(CCNode* pSender){
	this->removeChild(pSender);
}

void Ufo1Layer::ufoRemove(CCNode* pSender){
	this->removeChild(pSender);
}


//CCRect Ufo1Layer::getBoundingBox(){
//	CCRect rect=p_ufo->boundingBox();
//	CCPoint pos=convertToWorldSpace(rect.origin);
//	CCRect enemyRect(pos.x,pos.y,rect.size.width,rect.size.height);
//	return enemyRect;
//}



