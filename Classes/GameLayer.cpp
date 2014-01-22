#include "GameLayer.h"
#include "cocos2d.h"
#include "PlaneLayer.h"
#include "BulletLayer.h"
#include "EnemyLayer.h"


Level GameLayer::level=EASY;

GameLayer::GameLayer(){
	background2=NULL;
	background1=NULL;
	planeLayer=NULL;
	bulletLayer=NULL;
	bullet2Layer=NULL;
	enemyLayer=NULL;
	ufo1Layer=NULL;
	ufo2Layer=NULL;
	
	score=0;
	bigBoomCount=0;

	labelBigBombCountItem=NULL;
	menuBigBoom=NULL;
}

GameLayer::~GameLayer(){

}


bool GameLayer::init(){
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		if (!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_music.wav",true);
		}

		background1=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background1->setAnchorPoint(ccp(0,0));
		background1->setPosition(ccp(0,0));
		this->addChild(background1);

		background2=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("background.png"));
		background2->setAnchorPoint(ccp(0,0));
		background2->setPosition(ccp(0,background1->getContentSize().height-2));

		
		//添加背景
		this->addChild(background2);
		this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

		//添加飞机
		this->planeLayer=PlaneLayer::create();
		this->addChild(planeLayer);


		//添加敌机
		this->enemyLayer=EnemyLayer::create();
		this->addChild(enemyLayer);


		//添加子弹1
		this->bulletLayer=BulletLayer::create();
		this->addChild(bulletLayer);
		bulletLayer->StartShoot(0.01f);

		//添加子弹2
		this->bullet2Layer=Bullet2Layer::create();
		this->addChild(bullet2Layer);
		//bullet2Layer->startShoot(0.01f);


		//添加触摸事件
		this->setTouchEnabled(true);
		//触发update方法
		this->scheduleUpdate();


		//添加暂停按钮
		this->controlLayer=ControlLayer::create();
		this->addChild(controlLayer);

		//加入ufo1
		this->ufo1Layer=Ufo1Layer::create();
		this->addChild(ufo1Layer);
		//ufo1Layer->addUfo1();


		//加入ufo2
		this->ufo2Layer=Ufo2Layer::create();
		this->addChild(ufo2Layer);
		//ufo2Layer->ufo2Ai(100000);


		bRet=true;

	} while (0);

	return bRet;
}


/************************************************************************/
// 游戏每一帧的控制
//子弹和敌机的碰撞
//hero和敌机的碰撞
//UFO和hero的碰撞更新
/************************************************************************/
void GameLayer::update(float delta){
	if (level==EASY&&score>=1000000)
	{
		level=MIDDLE;
	}else if (level==MIDDLE&&score>=2000000)
	{
		level=HARD;
	}else if (score<1000000)
	{
		level=EASY;
	}


	/************************************************************************/
	/* 子弹和敌机的碰撞                                                     */
	/************************************************************************/
	CCArray *bulletsToDelete=CCArray::create();
	bulletsToDelete->retain();
	CCObject *bt,*et;



	//循环子弹
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt){
		CCSprite *bullet=(CCSprite*)bt;

		CCArray *enemy1sToDelete=CCArray::create();
		enemy1sToDelete->retain();
		//int a=this->enemyLayer->m_pAllEnemy1->capacity();//集合的长度
		
		//循环屏幕中的敌机1
		CCARRAY_FOREACH(enemyLayer->m_pAllEnemy1,et){
			Enemy *enemy1=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife()==1)
				{
					enemy1->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy1sToDelete->addObject(enemy1);
					score+=ENEMY1_SCORE;
					this->ufo1Layer->ufo1Ai(score);//出现ufo1
					this->ufo2Layer->ufo2Ai(score);//出现ufo2
					this->controlLayer->updateScore(score);
				}else;
			}
		}

		//清理碰撞的敌机
		CCARRAY_FOREACH(enemy1sToDelete,et){
			Enemy *enemy1=(Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}

	//清理碰撞后的子弹
	CCARRAY_FOREACH(bulletsToDelete,bt){
		CCSprite *bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();



	//循环二号敌机
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy2sToDelete=CCArray::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et)
		{
			Enemy* enemy2=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife()>1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if(enemy2->getLife()==1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy2sToDelete->addObject(enemy2);
					score+=ENEMY2_SCORE;
					this->ufo1Layer->ufo1Ai(score);//出现ufo1
					this->ufo2Layer->ufo2Ai(score);//出现ufo2
					this->controlLayer->updateScore(score);
				}
				else ;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete,et)
		{
			Enemy* enemy2=(Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();


	//循环3号敌机
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;

		CCArray* enemy3sToDelete=CCArray::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et)
		{
			Enemy* enemy3=(Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				//如果life>1,移除bullet
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				//如果life==1,移除enemy3
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy3sToDelete->addObject(enemy3);
					score+=ENEMY3_SCORE;
					this->ufo1Layer->ufo1Ai(score);//出现ufo1
					this->ufo2Layer->ufo2Ai(score);//出现ufo2
					this->controlLayer->updateScore(score);
				}
				//此时处在animate阶段,不做处理
				else ;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete,et)
		{
			Enemy* enemy3=(Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete,bt)
	{
		CCSprite* bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();
	bulletsToDelete->release();//1号子弹循环完成，清除敌机和碰撞后的子弹，释放存放清除子弹的集合。





	/************************************************************************/
	/* 2号子弹和飞机的碰撞检测                                              */
	/************************************************************************/
	CCArray*  p_Bullet2sToDelete=CCArray::create();
	p_Bullet2sToDelete->retain();
	CCObject *mbt,*et2;

	CCARRAY_FOREACH(this->bullet2Layer->m_pAllButtlets,mbt){
		CCSprite* p_Bullet2=(CCSprite*)mbt;

		CCArray* p_Enemy1sToDelete=CCArray::create();
		p_Enemy1sToDelete->retain();

		//循环1号敌机
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et2){
			Enemy* p_Enemy1=(Enemy*)et2;
			if (p_Enemy1->getBoundingBox().intersectsRect(p_Bullet2->boundingBox()))
			{
				if (p_Enemy1->getLife()==1)
				{
					p_Enemy1->loseLife();
					p_Bullet2sToDelete->addObject(p_Bullet2);
					p_Enemy1sToDelete->addObject(p_Enemy1);
					score+=ENEMY1_SCORE;
					this->ufo1Layer->ufo1Ai(score);//出现ufo1
					this->ufo2Layer->ufo2Ai(score);//出现ufo2
					this->controlLayer->updateScore(score);
				}else;
			}
		}

		CCARRAY_FOREACH(p_Enemy1sToDelete,et2){
			Enemy* p_EnemyToDelete=(Enemy*)et2;
			this->enemyLayer->enemy1Blowup(p_EnemyToDelete);
		}
		p_Enemy1sToDelete->release();
	}



	CCARRAY_FOREACH(p_Bullet2sToDelete,bt){
		CCSprite* p_BulletToDelete=(CCSprite*)bt;
		this->bullet2Layer->removeButtet(p_BulletToDelete);
	}		
	p_Bullet2sToDelete->removeAllObjects();

	//循环2号敌机
	CCARRAY_FOREACH(this->bullet2Layer->m_pAllButtlets,mbt){
		CCSprite* p_Bullet2=(CCSprite*)mbt;

		CCArray* p_Enemy2sToDelete=CCArray::create();
		p_Enemy2sToDelete->retain();

		
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et2){
			Enemy* p_Enemy2=(Enemy*)et2;
			if (p_Enemy2->getBoundingBox().intersectsRect(p_Bullet2->boundingBox()))
			{
				if (p_Enemy2->getLife()>1)
				{
					p_Enemy2->loseLife();
				}
				if (p_Enemy2->getLife()==1)
				{					
					p_Bullet2sToDelete->addObject(p_Bullet2);
					p_Enemy2sToDelete->addObject(p_Enemy2);	
					p_Enemy2->loseLife();		
					score+=ENEMY2_SCORE;
					this->ufo1Layer->ufo1Ai(score);//出现ufo1
					this->ufo2Layer->ufo2Ai(score);//出现ufo2
					this->controlLayer->updateScore(score);
				}
				
			}
		}

		CCARRAY_FOREACH(p_Enemy2sToDelete,et2){
			Enemy* p_EnemyToDelete=(Enemy*)et2;
			this->enemyLayer->enemy2Blowup(p_EnemyToDelete);
		}
		p_Enemy2sToDelete->release();
	}


	CCARRAY_FOREACH(p_Bullet2sToDelete,bt){
		CCSprite* p_BulletToDelete=(CCSprite*)bt;
		this->bullet2Layer->removeButtet(p_BulletToDelete);
	}
	p_Bullet2sToDelete->removeAllObjects();


	//循环3号敌机
	CCARRAY_FOREACH(this->bullet2Layer->m_pAllButtlets,mbt){
		CCSprite* p_Bullet2=(CCSprite*)mbt;

		CCArray* p_Enemy3sToDelete=CCArray::create();
		p_Enemy3sToDelete->retain();


		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et2){
			Enemy* p_Enemy3=(Enemy*)et2;
			if (p_Enemy3->getBoundingBox().intersectsRect(p_Bullet2->boundingBox()))
			{
				if (p_Enemy3->getLife()>1)
				{
					p_Enemy3->loseLife();
				}else if (p_Enemy3->getLife()==1)
				{					
					p_Bullet2sToDelete->addObject(p_Bullet2);
					p_Enemy3sToDelete->addObject(p_Enemy3);	
					p_Enemy3->loseLife();		
					score+=ENEMY3_SCORE;
					this->ufo1Layer->ufo1Ai(score);//出现ufo1
					this->ufo2Layer->ufo2Ai(score);//出现ufo2
					this->controlLayer->updateScore(score);
				}				
			}
		}

		CCARRAY_FOREACH(p_Enemy3sToDelete,et2){
			Enemy* p_EnemyToDelete=(Enemy*)et2;
			this->enemyLayer->enemy3Blowup(p_EnemyToDelete);
		}
		p_Enemy3sToDelete->release();
	}


	CCARRAY_FOREACH(p_Bullet2sToDelete,bt){
		CCSprite* p_BulletToDelete=(CCSprite*)bt;
		this->bullet2Layer->removeButtet(p_BulletToDelete);
	}
	p_Bullet2sToDelete->removeAllObjects();
	p_Bullet2sToDelete->release();






	/************************************************************************/ 
	//敌机和hero相撞
	/************************************************************************/
	CCNode *hero=this->planeLayer->getChildByTag(AIRPLANE);
	CCRect airPlaneRect=hero->boundingBox();
	airPlaneRect.origin.x+=30;
	airPlaneRect.size.width-=60;

	//1号敌机
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et){
		Enemy *enemy1=(Enemy*)et;
		
		if (enemy1->getLife()>0)
		{
			if (enemy1->getBoundingBox().intersectsRect(hero->boundingBox()))
			//if (airPlaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();//停止所有的定时器
				this->bulletLayer->StopShoot();//停止发射
				this->bullet2Layer->stopShoot();
				this->enemyLayer->enemy1Blowup(enemy1);//敌机爆炸
				this->planeLayer->Blowup(score);//hero爆炸，游戏结束
				return;
			}
		}
	}

	//2号敌机
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et){
		Enemy *enemy2=(Enemy*)et;

		if (enemy2->getLife()>0)
		{
			if (enemy2->getBoundingBox().intersectsRect(hero->boundingBox()))
				//if (airPlaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();//停止所有的定时器
				this->bulletLayer->StopShoot();//停止发射
				this->bullet2Layer->stopShoot();
				this->enemyLayer->enemy2Blowup(enemy2);//敌机爆炸
				this->planeLayer->Blowup(score);//hero爆炸，游戏结束
				return;
			}
		}
	}


	//3号敌机
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et){
		Enemy *enemy3=(Enemy*)et;

		if (enemy3->getLife()>0)
		{
			if (enemy3->getBoundingBox().intersectsRect(hero->boundingBox()))
				//if (airPlaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();//停止所有的定时器
				this->bulletLayer->StopShoot();//停止发射
				this->bullet2Layer->stopShoot();
				this->enemyLayer->enemy3Blowup(enemy3);//敌机爆炸
				this->planeLayer->Blowup(score);//hero爆炸，游戏结束
				return;
			}
		}
	}


	/************************************************************************/
	/* hero和ufo1碰撞                                                                                             */
	/************************************************************************/
	CCNode* ufo1=this->ufo1Layer->getChildByTag(UFO);

	/*CCRect rect=ufo1->boundingBox();
	CCPoint pos=convertToWorldSpace(rect.origin);
	CCRect enemyRect(pos.x,pos.y,rect.size.width,rect.size.height);*/
	

	if (ufo1!=NULL)
	{
		if (ufo1->boundingBox().intersectsRect(hero->boundingBox()))
		{
			this->ufo1Layer->ufoRemove(ufo1);
			this->bulletLayer->StopShoot();
			this->bullet2Layer->startShoot(0.01f);
			this->scheduleOnce(schedule_selector(GameLayer::bullet2ShootEnd),5);
		}
	}


	/************************************************************************/
	/* hero和ufo2碰撞                                                                                             */
	/************************************************************************/
	CCNode* ufo2=this->ufo2Layer->getChildByTag(UFO2);

	/*CCRect rect=ufo1->boundingBox();
	CCPoint pos=convertToWorldSpace(rect.origin);
	CCRect enemyRect(pos.x,pos.y,rect.size.width,rect.size.height);*/
	

	if (ufo2!=NULL)
	{
		if (ufo2->boundingBox().intersectsRect(hero->boundingBox()))
		{
			this->ufo2Layer->ufoRemove(ufo2);
			this->bigBoomCount++;
			updateBigBombButton(bigBoomCount);
		}
	}

}


//hero碰撞红色炸弹后，生成左下角炸弹
void GameLayer::updateBigBombButton(int bombCount){
	CCSprite* bigBombNormal=CCSprite::createWithSpriteFrameName("bomb.png");
	CCSprite* bigBombPressed=CCSprite::createWithSpriteFrameName("bomb.png");
	if (bombCount<0)
	{
		return;
	}else if (bombCount==0)
	{
		if (this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			this->removeChildByTag(TAG_BIGBOOM_MENUITEM,true);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
	}else if (bombCount==1)
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			cocos2d::CCMenuItemImage* bigBombMenuItem=CCMenuItemImage::create();
			bigBombMenuItem->initWithNormalSprite(bigBombNormal,bigBombPressed,NULL,this,menu_selector(GameLayer::bigBombMenuCallBack));
			bigBombMenuItem->setPosition(bigBombNormal->getContentSize().width/2+10,bigBombNormal->getContentSize().height/2+10);
			menuBigBoom=CCMenu::create(bigBombMenuItem,NULL);
			menuBigBoom->setPosition(cocos2d::CCPointZero);
			this->addChild(menuBigBoom,0,TAG_BIGBOOM_MENUITEM);

		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
	}else
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			CCMenuItemImage* bombMenuItem=CCMenuItemImage::create();
			bombMenuItem->initWithNormalSprite(bigBombNormal,bigBombPressed,NULL,this,menu_selector(GameLayer::bigBombMenuCallBack));
			bombMenuItem->setPosition(bigBombNormal->getContentSize().width/2+10,bigBombNormal->getContentSize().height/2+10);
			menuBigBoom=CCMenu::create(bombMenuItem,NULL);
			menuBigBoom->setPosition(CCPointZero);
			this->addChild(menuBigBoom,TAG_BIGBOOM_MENUITEM);
		}

		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL,true);
		}
		if (bombCount>0&&bigBoomCount<MAX_BIGBOOM_COUNT)
		{
			CCString* strCount=CCString::createWithFormat("X%d",bigBoomCount);
			labelBigBombCountItem=CCLabelBMFont::create(strCount->m_sString.c_str(),"font/font.fnt");
			labelBigBombCountItem->setColor(ccc3(143,146,147));
			labelBigBombCountItem->setAnchorPoint(ccp(0,0.5));
			labelBigBombCountItem->setPosition(ccp(bigBombNormal->getContentSize().width+15,bigBombNormal->getContentSize().height/2+5));
			this->addChild(labelBigBombCountItem,0,TAG_BIGBOOMCOUNT_LABEL);
		}


	}
	
}


//点击使用炸弹
void GameLayer::bigBombMenuCallBack(CCObject* pEvent){
	if (bigBoomCount>0 &&!CCDirector::sharedDirector()->isPaused())
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/use_bomb.mp3");
		bigBoomCount--;		
		score+=this->enemyLayer->m_pAllEnemy1->count()*ENEMY1_SCORE;
		score+=this->enemyLayer->m_pAllEnemy2->count()*ENEMY2_SCORE;
		score+=this->enemyLayer->m_pAllEnemy3->count()*ENEMY3_SCORE;
		this->enemyLayer->removeAllEnemy();
		updateBigBombButton(bigBoomCount);
		this->controlLayer->updateScore(score);
	}
}




//子弹二发射倒计时结束时触发
void GameLayer::bullet2ShootEnd(float dt){
	this->bulletLayer->StartShoot(0.01f);
	this->bullet2Layer->stopShoot();
}



void GameLayer::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;
}

void GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if (planeLayer->isAlive)
	{
		CCDirector *cCDirector=CCDirector::sharedDirector();
		CCNode *plane=planeLayer->getChildByTag(AIRPLANE);

		CCPoint beginPoint=pTouch->getLocationInView();
		beginPoint=cCDirector->convertToGL(beginPoint);

		CCRect planeRect=plane->boundingBox();
		planeRect.origin.x-=15;
		planeRect.origin.y-=15;
		planeRect.size.height+=30;
		planeRect.size.width+=30;

		if (planeRect.containsPoint(getParent()->convertTouchToNodeSpace(pTouch)))
		{
			CCPoint endPoint=pTouch->getPreviousLocationInView();
			endPoint=cCDirector->convertToGL(endPoint);
			CCPoint offset=ccpSub(beginPoint,endPoint);
			CCPoint toPoint=ccpAdd(plane->getPosition(),offset);
			planeLayer->MoveTo(toPoint);
		}

	}
}




void GameLayer::backgroundMove(float dt){
	background1->setPositionY(background1->getPositionY()-2);
	background2->setPositionY(background1->getPositionY()+background1->getContentSize().height-2);

	if (background2->getPositionY()==0)
	{
		background1->setPositionY(0);
	}
}


Level GameLayer::getCurLevel(){
	return level;
}
