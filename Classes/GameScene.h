#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

class GameScene : public CCScene
{
public:
	GameScene();
	~GameScene();

	CREATE_FUNC(GameScene);

	virtual bool init();  

private:
	GameLayer* pGameLayer;
	
};

#endif // __GAME_SCENE_H__
