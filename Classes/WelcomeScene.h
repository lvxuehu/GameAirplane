#ifndef _WELCOMESCENE_H_
#define _WELCOMESCENE_H_
#include "cocos2d.h"
#include "WelcomeLayer.h"

USING_NS_CC;

class WelcomeScene:public CCScene{
public:
	WelcomeScene(void);
	~WelcomeScene(void);

	CREATE_FUNC(WelcomeScene);

	virtual bool init();

	void preloadMusic();
	

public:
	WelcomeLayer* welcomeLayer;
};



#endif


