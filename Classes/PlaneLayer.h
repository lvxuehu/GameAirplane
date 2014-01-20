#ifndef _PLANELAYER_H_
#define _PLANELAYER_H_


#include "cocos2d.h"

USING_NS_CC;

const int AIRPLANE=747;

class PlaneLayer:public CCLayer{
public:
	PlaneLayer(void);
	~PlaneLayer(void);

	static PlaneLayer* create();

	virtual bool init();

	void MoveTo(CCPoint location);

	void Blowup(int passScore);

	void RemovePlane();

public:
	bool isAlive;
	int score;
	static PlaneLayer* sharePlane;
};



#endif //_PLANELAYER_H_
