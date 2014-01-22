#include "Bullet2Layer.h"
#include "PlaneLayer.h"


Bullet2Layer::Bullet2Layer(void)
{
    m_pSpriteBatchNode=NULL;
    m_pAllButtlets=CCArray::create();
    m_pAllButtlets->retain();
}


Bullet2Layer::~Bullet2Layer(void)
{
    m_pAllButtlets->release();
    m_pAllButtlets=NULL;
}

bool Bullet2Layer::init(){
    bool pRet=false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());

        CCTexture2D* p_Texture2d=CCTextureCache::sharedTextureCache()->textureForKey("ui/shoot.png");
        m_pSpriteBatchNode=CCSpriteBatchNode::createWithTexture(p_Texture2d);
        this->addChild(m_pSpriteBatchNode);
        pRet=true;
    } while (0);
    return pRet;
}



void Bullet2Layer::startShoot(float dt){
    this->schedule(schedule_selector(Bullet2Layer::addBullet),0.15f,kCCRepeatForever,dt);
}

void Bullet2Layer::stopShoot(){
    this->unschedule(schedule_selector(Bullet2Layer::addBullet));
}

void Bullet2Layer::addBullet(float dt){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/bullet.wav");

    CCSprite* p_Lbullet=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet2.png"));
    CCSprite* p_Rbullet=CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet2.png"));
    m_pAllButtlets->addObject(p_Lbullet);
    m_pAllButtlets->addObject(p_Rbullet);
    m_pSpriteBatchNode->addChild(p_Lbullet);
    m_pSpriteBatchNode->addChild(p_Rbullet);

    //得到飞机的位置，得出子弹发射的坐标
    CCNode* p_Airplane=PlaneLayer::sharePlane->getChildByTag(AIRPLANE);
    CCPoint  airplanePoint=p_Airplane->getPosition();
    CCPoint l_BulletShootPoint=ccp(airplanePoint.x-4,airplanePoint.y+p_Airplane->getContentSize().height/2);//左边子弹的发射位置
    CCPoint r_BulletShootPoint=ccp(airplanePoint.x+4,airplanePoint.y+p_Airplane->getContentSize().height/2);//右边子弹的发射位置
    p_Lbullet->setPosition(l_BulletShootPoint);
    p_Rbullet->setPosition(r_BulletShootPoint);


    //子弹飞行的动画，到了屏幕顶端后停止动画
    //子弹终点坐标
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    float length=winSize.height+p_Lbullet->getContentSize().height/2-p_Airplane->getContentSize().height/2;//子弹消失的位置
    float velocity=420/1;//320pixel/sec
    float bulletDurationTime=length/velocity;

    //CCPoint l_BulletEndPoint=ccp(p_Lbullet->getPositionX(),winSize.height+p_Lbullet->getContentSize.height/2);//左侧子弹的消失的位置
    //CCPoint r_BulletEndPoint=ccp(p_Rbullet->getPositionX(),winSize.height+p_Rbullet->getContentSize.height/2);//右侧子弹的消失的位置
    
    CCFiniteTimeAction*  l_BulletMoveTo=CCMoveTo::create(bulletDurationTime,ccp(l_BulletShootPoint.x,winSize.height+p_Lbullet->getContentSize().height/2));
    CCFiniteTimeAction*  r_BulletMoveTo=CCMoveTo::create(bulletDurationTime,ccp(r_BulletShootPoint.x,winSize.height+p_Rbullet->getContentSize().height/2));
    CCFiniteTimeAction*  l_BulletMoveEnd=CCCallFuncN::create(this,callfuncN_selector(Bullet2Layer::buttletMoveFinined));
    CCFiniteTimeAction*  r_BulletMoveEnd=CCCallFuncN::create(this,callfuncN_selector(Bullet2Layer::buttletMoveFinined));
    


    CCSequence* l_Sequence=CCSequence::create(l_BulletMoveTo,l_BulletMoveEnd,NULL);
    CCSequence* r_Sequence=CCSequence::create(r_BulletMoveTo,r_BulletMoveEnd,NULL);
    p_Lbullet->runAction(l_Sequence);
    p_Rbullet->runAction(r_Sequence);
}


void Bullet2Layer::buttletMoveFinined(CCNode* buttlet){
    CCSprite* sprite=(CCSprite*)buttlet;
    m_pAllButtlets->removeObject(sprite);
    this->removeChild(sprite,true);
}

void Bullet2Layer::removeButtet(CCSprite* buttlet){
    m_pAllButtlets->removeObject(buttlet);
    this->removeChild(buttlet);
}