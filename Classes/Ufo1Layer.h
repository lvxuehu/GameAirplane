#ifndef _UFO1LAYER_H_
#define _UFO1LAYER_H_
#include "cocos2d.h"

USING_NS_CC;
const int UFO=1000;
const int FUO1_OUT=5000;//每到5000分的整数倍出现ufo1；

class Ufo1Layer:public CCLayer
{
public:
	Ufo1Layer(void);
	~Ufo1Layer(void);

	virtual bool init();
	CREATE_FUNC(Ufo1Layer);

public:
	void ufo1Ai(int score);
	void addUfo1();
	void ufoMoveFinished(CCNode* pSender);
    void ufoRemove(CCSprite* pSender);
	CCRect getBoundingBox();

private:
	CCSprite* p_ufo;
};


#endif


