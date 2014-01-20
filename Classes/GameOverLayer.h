#ifndef _GAMEOVERLAYER_H_
#define _GAMEOVERLAYER_H_
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;


class GameOverLayer:public CCLayer
{
public:
	GameOverLayer(void);
	~GameOverLayer(void);
	virtual bool init();

	static GameOverLayer* create(int passScore);

	void menuBackCallback(CCObject* pSender);
	virtual void keyBackClicked();
	void beginChangeHighestScore(CCNode* pNode);
	void showAD();

public:
	int score;
	static int highestHistoryScore;
	CCLabelBMFont* highestScore;

};

#endif


