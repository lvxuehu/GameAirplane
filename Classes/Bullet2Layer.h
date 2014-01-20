#ifndef _BULLET2LAYER_H_
#define _BULLET2LAYER_H_
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class Bullet2Layer:public CCLayer
{
public:
	Bullet2Layer(void);
	~Bullet2Layer(void);

	CREATE_FUNC(Bullet2Layer);

	virtual bool init();

	void startShoot(float dt);
	void stopShoot();
	void addBullet(float dt);
	void removeButtet(CCSprite* buttlet);
	void buttletMoveFinined(CCNode* buttlet);
	
public:
    CCArray* m_pAllButtlets;
	CCSpriteBatchNode* m_pSpriteBatchNode;
};

#endif



