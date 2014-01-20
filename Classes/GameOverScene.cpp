#include "GameOverScene.h"


GameOverScene::GameOverScene(void)
{
	gameOverlayer=NULL;
	score=0;
}


GameOverScene::~GameOverScene(void)
{
}


GameOverScene* GameOverScene::create(int passScore){
	GameOverScene *pRet=new GameOverScene();
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


bool GameOverScene::init(){
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCScene::init());
		gameOverlayer=GameOverLayer::create(score);
		this->addChild(gameOverlayer);
		bRet=true;
	} while (0);
	return bRet;
}
