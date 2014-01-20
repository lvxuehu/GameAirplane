#ifndef _WELCOMELAYER_H_
#define _WELCOMELAYER_H_
#include "cocos2d.h"

USING_NS_CC;


class WelcomeLayer:public CCLayer
{
public:
	WelcomeLayer(void);
	~WelcomeLayer(void);

	CREATE_FUNC(WelcomeLayer);
	virtual bool init();

	void loadingDone(CCNode *pNode);
	virtual void keyBackClicked();
	bool isHaveSaveFile();

	void getHighestHistorySorce();
};



#endif


