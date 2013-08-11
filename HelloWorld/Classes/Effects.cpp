#include "pch.h"
#include "CCCommon.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

#include "Effects.h"
#include "GameManage.h"
using namespace cocos2d;
using namespace CocosDenshion;

static Effects* hEffects = NULL;
Effects*  Effects::GetInstance()//获得当前的GamaMange实例
{
	if (hEffects == NULL)
		hEffects = new Effects();

	return hEffects;
}

Effects::Effects()
{
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	GameManage * gm=GameManage::GetInstance();
	AniPool=CCAnimationCache::sharedAnimationCache();

	//消除的粒子效果
	popEffect = CCParticleFlower::create();
	//popEffect->retain();
	popEffect->setTexture(CCTextureCache::sharedTextureCache()->addImage("Img/stars.png"));
	popEffect->setDuration(.05);
	popEffect->setEmissionRate(100);
	popEffect->setPosVar(ccp(5,5));

	//移动的动画效果
	//CCActionInterval* = CCMoveBy::create(.1f,ccp(gm->Gboard.colOneItemPixel,0));
	CCActionInterval* MoveH= CCEaseOut::create((CCActionInterval*)(CCMoveBy::create(.1f,ccp(gm->Gboard.colOneItemPixel,0))->copy()->autorelease()), 2.0f);
	AniPool->addAnimation((CCAnimation*)MoveH,"Ani_Item_MoveH");
	//CCDelayTime *waiting=CCDelayTime::create(0.2f);  
	CCActionInterval* MoveV= CCMoveBy::create(.1f,ccp(0,gm->Gboard.rowOneItemPixel));
	//CCFiniteTimeAction* action= CCSequence::create(MoveM,MoveL,NULL);  //动画序列

}


Effects::~Effects()
{
	delete hEffects;
}