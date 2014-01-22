#include "Ufo2Layer.h"


Ufo2Layer::Ufo2Layer(void)
{
}


Ufo2Layer::~Ufo2Layer(void)
{
}


bool Ufo2Layer::init(){
	bool pRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		pRet=true;
	} while (0);
	return pRet;
}

//���ݻ��ּ����Ƿ����ufo1
void Ufo2Layer::ufo2Ai(int score){

	if (score>0&&score%UFO2_OUT==0)
	{
		addUfo2();
	}

}


void Ufo2Layer::addUfo2(){
	CCSprite* p_ufo=CCSprite::createWithSpriteFrameName("ufo2.png");
	//����ufo���ֵ���ʼλ��
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	int minStartX=p_ufo->getContentSize().width/2;
	int maxStartX=winSize.width-p_ufo->getContentSize().width/2;
	int rangex=maxStartX-minStartX;

	//���������ʼxֵ
	int startX=(rand()%rangex)+minStartX;
	p_ufo->setPosition(ccp(startX,winSize.height+p_ufo->getContentSize().height/2));
	this->addChild(p_ufo,1,UFO2);

	//������ʧ��λ��
	int endX=startX;
	int endY=0-p_ufo->getContentSize().height/2;

	//���е�ʱ��(�ٶ�)
	float durationTime=2.0;

	CCFiniteTimeAction* moveTo=CCMoveTo::create(durationTime,ccp(endX,endY));
	CCFiniteTimeAction* moveEnd=CCCallFuncN::create(this,callfuncN_selector(Ufo2Layer::ufoMoveFinished));
	CCSequence* sequence=CCSequence::create(moveTo,moveEnd,NULL);

	p_ufo->runAction(sequence);

}

void Ufo2Layer::ufoMoveFinished(CCNode* pSender){
	this->removeChild(pSender);
}

void Ufo2Layer::ufoRemove(CCNode* pSender){
	this->removeChild(pSender);
}