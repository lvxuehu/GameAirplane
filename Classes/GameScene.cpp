#include "GameScene.h"
#include "cocos2d.h"


GameScene::GameScene(){
	pGameLayer=NULL;
}

GameScene::~GameScene(){

}

bool GameScene::init(){
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCScene::init());
		pGameLayer=GameLayer::create();
		CC_BREAK_IF(!pGameLayer);
		this->addChild(pGameLayer);
		bRet=true;
	} while (0);

	return bRet;
}