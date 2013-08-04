#include "pch.h"
#include "CCCommon.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

#include "Effects.h"
#include "GameManage.h"
using namespace cocos2d;
using namespace CocosDenshion;

static Effects* hEffects = NULL;
Effects*  Effects::GetInstance()//��õ�ǰ��GamaMangeʵ��
{
	if (hEffects == NULL)
		hEffects = new Effects();

	return hEffects;
}

Effects::Effects()
{
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	GameManage * gm=GameManage::GetInstance();

	//����������Ч��
	popEffect = CCParticleFlower::create();
	//popEffect->retain();
	popEffect->setTexture(CCTextureCache::sharedTextureCache()->addImage("Img/stars.png"));
	popEffect->setDuration(.05);
	popEffect->setEmissionRate(100);
	popEffect->setPosVar(ccp(5,5));

	//�ƶ��Ķ���Ч��
	CCMoveBy* MoveH= CCMoveBy::create(.1f,ccp(gm->Gboard.colOneItemPixel,0));
	//CCDelayTime *waiting=CCDelayTime::create(0.2f);  
	CCMoveBy* MoveV= CCMoveBy::create(.1f,ccp(0,gm->Gboard.rowOneItemPixel));
	//CCFiniteTimeAction* action= CCSequence::create(MoveM,MoveL,NULL);  //��������
	moveVerAni=(CCAction *)MoveV;  
	moveHorAni=(CCAction *)MoveH;  
}


Effects::~Effects()
{
	delete hEffects;
}