#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_
#include "GameOverLayer.h"
#include "cocos2d.h"


USING_NS_CC;

class GameOverScene:public CCScene
{
public:
	GameOverScene(void);
	~GameOverScene(void);

	virtual bool init();

	static GameOverScene* create(int passScore);

public:
	GameOverLayer *gameOverlayer;
	int score;
};



#endif


