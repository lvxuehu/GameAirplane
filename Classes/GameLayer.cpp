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

		
		//��ӱ���
		this->addChild(background2);
		this->schedule(schedule_selector(GameLayer::backgroundMove),0.01f);

		//��ӷɻ�
		this->planeLayer=PlaneLayer::create();
		this->addChild(planeLayer);


		//��ӵл�
		this->enemyLayer=EnemyLayer::create();
		this->addChild(enemyLayer);


		//����ӵ�1
		this->bulletLayer=BulletLayer::create();
		this->addChild(bulletLayer);
		bulletLayer->StartShoot(0.01f);

		//����ӵ�2
		this->bullet2Layer=Bullet2Layer::create();
		this->addChild(bullet2Layer);
		//bullet2Layer->startShoot(0.01f);


		//��Ӵ����¼�
		this->setTouchEnabled(true);
		//����update����
		this->scheduleUpdate();


		//�����ͣ��ť
		this->controlLayer=ControlLayer::create();
		this->addChild(controlLayer);

		//����ufo1
		this->ufo1Layer=Ufo1Layer::create();
		this->addChild(ufo1Layer);
		//ufo1Layer->addUfo1();


		//����ufo2
		this->ufo2Layer=Ufo2Layer::create();
		this->addChild(ufo2Layer);
		//ufo2Layer->ufo2Ai(100000);


		bRet=true;

	} while (0);

	return bRet;
}


/************************************************************************/
// ��Ϸÿһ֡�Ŀ���
//�ӵ��͵л�����ײ
//hero�͵л�����ײ
//UFO��hero����ײ����
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
	/* �ӵ��͵л�����ײ                                                     */
	/************************************************************************/
	CCArray *bulletsToDelete=CCArray::create();
	bulletsToDelete->retain();
	CCObject *bt,*et;



	//ѭ���ӵ�
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet,bt){
		CCSprite *bullet=(CCSprite*)bt;

		CCArray *enemy1sToDelete=CCArray::create();
		enemy1sToDelete->retain();
		//int a=this->enemyLayer->m_pAllEnemy1->capacity();//���ϵĳ���
		
		//ѭ����Ļ�еĵл�1
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
					this->ufo1Layer->ufo1Ai(score);//����ufo1
					this->ufo2Layer->ufo2Ai(score);//����ufo2
					this->controlLayer->updateScore(score);
				}else;
			}
		}

		//������ײ�ĵл�
		CCARRAY_FOREACH(enemy1sToDelete,et){
			Enemy *enemy1=(Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}

	//������ײ����ӵ�
	CCARRAY_FOREACH(bulletsToDelete,bt){
		CCSprite *bullet=(CCSprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();



	//ѭ�����ŵл�
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
					this->ufo1Layer->ufo1Ai(score);//����ufo1
					this->ufo2Layer->ufo2Ai(score);//����ufo2
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


	//ѭ��3�ŵл�
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
				//���life>1,�Ƴ�bullet
				if (enemy3->getLife()>1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				//���life==1,�Ƴ�enemy3
				else if(enemy3->getLife()==1)
				{
					enemy3->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy3sToDelete->addObject(enemy3);
					score+=ENEMY3_SCORE;
					this->ufo1Layer->ufo1Ai(score);//����ufo1
					this->ufo2Layer->ufo2Ai(score);//����ufo2
					this->controlLayer->updateScore(score);
				}
				//��ʱ����animate�׶�,��������
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
	bulletsToDelete->release();//1���ӵ�ѭ����ɣ�����л�����ײ����ӵ����ͷŴ������ӵ��ļ��ϡ�





	/************************************************************************/
	/* 2���ӵ��ͷɻ�����ײ���                                              */
	/************************************************************************/
	CCArray*  p_Bullet2sToDelete=CCArray::create();
	p_Bullet2sToDelete->retain();
	CCObject *mbt,*et2;

	CCARRAY_FOREACH(this->bullet2Layer->m_pAllButtlets,mbt){
		CCSprite* p_Bullet2=(CCSprite*)mbt;

		CCArray* p_Enemy1sToDelete=CCArray::create();
		p_Enemy1sToDelete->retain();

		//ѭ��1�ŵл�
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
					this->ufo1Layer->ufo1Ai(score);//����ufo1
					this->ufo2Layer->ufo2Ai(score);//����ufo2
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

	//ѭ��2�ŵл�
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
					this->ufo1Layer->ufo1Ai(score);//����ufo1
					this->ufo2Layer->ufo2Ai(score);//����ufo2
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


	//ѭ��3�ŵл�
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
					this->ufo1Layer->ufo1Ai(score);//����ufo1
					this->ufo2Layer->ufo2Ai(score);//����ufo2
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
	//�л���hero��ײ
	/************************************************************************/
	CCNode *hero=this->planeLayer->getChildByTag(AIRPLANE);
	CCRect airPlaneRect=hero->boundingBox();
	airPlaneRect.origin.x+=30;
	airPlaneRect.size.width-=60;

	//1�ŵл�
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1,et){
		Enemy *enemy1=(Enemy*)et;
		
		if (enemy1->getLife()>0)
		{
			if (enemy1->getBoundingBox().intersectsRect(hero->boundingBox()))
			//if (airPlaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();//ֹͣ���еĶ�ʱ��
				this->bulletLayer->StopShoot();//ֹͣ����
				this->bullet2Layer->stopShoot();
				this->enemyLayer->enemy1Blowup(enemy1);//�л���ը
				this->planeLayer->Blowup(score);//hero��ը����Ϸ����
				return;
			}
		}
	}

	//2�ŵл�
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2,et){
		Enemy *enemy2=(Enemy*)et;

		if (enemy2->getLife()>0)
		{
			if (enemy2->getBoundingBox().intersectsRect(hero->boundingBox()))
				//if (airPlaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();//ֹͣ���еĶ�ʱ��
				this->bulletLayer->StopShoot();//ֹͣ����
				this->bullet2Layer->stopShoot();
				this->enemyLayer->enemy2Blowup(enemy2);//�л���ը
				this->planeLayer->Blowup(score);//hero��ը����Ϸ����
				return;
			}
		}
	}


	//3�ŵл�
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3,et){
		Enemy *enemy3=(Enemy*)et;

		if (enemy3->getLife()>0)
		{
			if (enemy3->getBoundingBox().intersectsRect(hero->boundingBox()))
				//if (airPlaneRect.intersectsRect(enemy1->getBoundingBox()))
			{
				this->unscheduleAllSelectors();//ֹͣ���еĶ�ʱ��
				this->bulletLayer->StopShoot();//ֹͣ����
				this->bullet2Layer->stopShoot();
				this->enemyLayer->enemy3Blowup(enemy3);//�л���ը
				this->planeLayer->Blowup(score);//hero��ը����Ϸ����
				return;
			}
		}
	}


	/************************************************************************/
	/* hero��ufo1��ײ                                                                                             */
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
	/* hero��ufo2��ײ                                                                                             */
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


//hero��ײ��ɫը�����������½�ը��
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


//���ʹ��ը��
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




//�ӵ������䵹��ʱ����ʱ����
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
