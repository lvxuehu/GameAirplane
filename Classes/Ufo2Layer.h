#ifndef _UFO2LAYER_H_
#define _UFO2LAYER_H_

#include "cocos2d.h"

USING_NS_CC;
const int UFO2=1001;
const int UFO2_OUT=9000;//每到9000分的整数倍出现ufo2；

class Ufo2Layer:public CCLayer
{
public:
	Ufo2Layer(void);
	~Ufo2Layer(void);

	virtual bool init();

	CREATE_FUNC(Ufo2Layer);

	void ufo2Ai(int score);
	void ufoRemove(CCNode* pSender);

	
private:
	void addUfo2();
	void ufoMoveFinished(CCNode* pSender);

};

#endif

