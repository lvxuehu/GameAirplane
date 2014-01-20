#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"


WelcomeScene::WelcomeScene(void)
{
	welcomeLayer=NULL;
}


WelcomeScene::~WelcomeScene(void)
{
}

bool WelcomeScene::init(){
	bool pRet=false;
	do 
	{
		CC_BREAK_IF(!CCScene::init());
		welcomeLayer=WelcomeLayer::create();
		CC_BREAK_IF(!welcomeLayer);
		this->addChild(welcomeLayer);
		preloadMusic();
		pRet=true;
	} while (0);
	return pRet;
}


void WelcomeScene::preloadMusic(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/game_music.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/bullet.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/enemy1_down.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/enemy2_down.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/enemy3_down.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/game_over.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/get_bomb.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/get_double_laser.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/use_bomb.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/big_spaceship_flying.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/achievement.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/out_porp.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/button.wav");

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_music.wav",true);
}


