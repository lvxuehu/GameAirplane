#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "BulletLayer.h"
#include "PlaneLayer.h"
#include "EnemyLayer.h"
#include "ControlLayer.h"
#include "Bullet2Layer.h"
#include "Ufo1Layer.h"

USING_NS_CC;

const int ENEMY1_SCORE=1000;
const int ENEMY2_SCORE=6000;
const int ENEMY3_SCORE=30000;

const int MAX_BIGBOOM_COUNT=100000;
const int TAG_BIGBOOM_MENUITEM=1;
const int TAG_BIGBOOMCOUNT_LABEL=2;

class GameLayer:public CCLayer{
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void update(float delta);
	static Level getCurLevel();

private:
	void backgroundMove(float dt);

private:
	CCSprite *background1;
	CCSprite *background2;
	PlaneLayer *planeLayer;
	BulletLayer *bulletLayer;
	Bullet2Layer* bullet2Layer;
	EnemyLayer *enemyLayer;
	ControlLayer* controlLayer;
	Ufo1Layer* ufo1Layer;
	

	int score;
	int bigBoomCount;
	static Level level;
};





#endif //_GAMELAYER_H_