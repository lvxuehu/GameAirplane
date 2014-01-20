#include "GameOverLayer.h"


int GameOverLayer::highestHistoryScore=0;
GameOverLayer::GameOverLayer(void)
{
	score=0;
	highestScore=NULL;
}


GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::create(int passScore){
	GameOverLayer* pRet=new GameOverLayer();
	pRet->score=passScore;
	if (pRet&&pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet=NULL;
		return NULL;
	}
}


bool GameOverLayer::init(){
	bool pRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());


		//停止播放背景音乐
		if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		}

	    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/game_over.wav");


		
		//新的背景图片
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();	
		CCSprite* background=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gameover.png"));
		background->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(background);	

		//回到游戏按钮
		CCSprite* normalBackToGame=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_finish.png"));
		CCSprite* pressedBackToGame=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("btn_finish.png"));
		CCMenuItemImage* pBackItem=CCMenuItemImage::create();
		pBackItem->initWithNormalSprite(normalBackToGame,pressedBackToGame,NULL,this,menu_selector(GameOverLayer::menuBackCallback));
		pBackItem->setPosition(ccp(winSize.width-normalBackToGame->getContentSize().width/2-10,normalBackToGame->getContentSize().height/2+10));
		CCMenu *menuBack=CCMenu::create(pBackItem,NULL);
		menuBack->setPosition(CCPointZero);
		this->addChild(menuBack);

		//当前分数
		CCString* strScore=CCString::createWithFormat("%d",score);
		CCLabelBMFont* finalScore=CCLabelBMFont::create(strScore->m_sString.c_str(),"font/font.fnt");
		finalScore->setColor(ccc3(143,146,147));
		finalScore->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(finalScore);

		//先显示分数，然后播放放大效果动画
		CCDelayTime* delay=CCDelayTime::create(1.0f);
		CCScaleTo* scalebig=CCScaleTo::create(1.0f,3.0f);
		CCScaleTo* scalelittle=CCScaleTo::create(0.3f,2.0f);
		CCCallFunc* showAD=CCCallFunc::create(this,callfunc_selector(GameOverLayer::showAD));
		CCFiniteTimeAction* sequence=CCSequence::create(delay,scalebig,scalelittle,showAD,NULL);
		finalScore->runAction(sequence);


		

		//最高分数
		CCString* strHighestScore=CCString::createWithFormat("%d",highestHistoryScore);
		highestScore=CCLabelBMFont::create(strHighestScore->m_sString.c_str(),"font/font.fnt");
		highestScore->setColor(ccc3(143,146,147));
		highestScore->setAnchorPoint(ccp(0,0.5));
		highestScore->setPosition(ccp(140,winSize.height-30));
		this->addChild(highestScore);

		//更新存在本地的最高分数
		if (score>highestHistoryScore)
		{
			CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",score);
			highestHistoryScore=score;
			CCDelayTime* delayChange=CCDelayTime::create(1.3f);
			CCMoveBy* moveOut=CCMoveBy::create(0.1f,ccp(0,100));
			CCCallFuncN* beginChange=CCCallFuncN::create(this,callfuncN_selector(GameOverLayer::beginChangeHighestScore));
			CCMoveBy* moveIn=CCMoveBy::create(0.1f,ccp(0,-100));
			CCFiniteTimeAction* sequence=CCSequence::create(delayChange,moveOut,beginChange,moveIn,NULL);
			highestScore->runAction(sequence);
		}

		this->setKeypadEnabled(true);

		pRet=true;
	} while (0);
	return pRet;

}

//回到游戏，重新来一局
void GameOverLayer::menuBackCallback(CCObject* pSender)
{
	CCScene* pScene=GameScene::create();
	CCTransitionSlideInL* animateScene=CCTransitionSlideInL::create(1.0f,pScene);
	CCDirector::sharedDirector()->replaceScene(animateScene);
}

void GameOverLayer::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}

void GameOverLayer::beginChangeHighestScore(CCNode* pNode)
{
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/achievement.mp3");
	CCString* changeScore=CCString::createWithFormat("%d",score);
	highestScore->setString(changeScore->m_sString.c_str());
}

void GameOverLayer::showAD()
{

}