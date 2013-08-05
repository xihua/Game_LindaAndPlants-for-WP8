#include "pch.h"

#include "CCCommon.h"
#include "BasicLoader.h"
#include "SimpleAudioEngine.h"

#include "GameControl.h"

using namespace cocos2d;
using namespace CocosDenshion;


PauseLayer::PauseLayer()
{
}
PauseLayer::~PauseLayer()
{

}

bool PauseLayer::init()
{

	bool bRet = false;

	do 
	{
		if ( !CCLayerColor::init() )
		{
			break;
		}
		
		CCSize size=CCDirector::sharedDirector()->getWinSize();
		CCLayerColor *pauseLayer=CCLayerColor::create();
		CCDictionary* plistDic = CCDictionary::createWithContentsOfFile("Plist/GameScene.plist");

		CCArray* picture_array = (CCArray*)plistDic->objectForKey("pause");
		CCArray* point_array = (CCArray*)plistDic->objectForKey("pause_pos");
		//CCArray* scale_array = (CCArray*)plistDic->objectForKey("pause_scale");
		for (int i=0;i<picture_array->count();i++)
		{
			CCString* pi_string = (CCString*)picture_array->objectAtIndex(i);
			CCSprite* sprite = CCSprite::create(pi_string->getCString());

			//CCString* scale_string = (CCString*)scale_array->objectAtIndex(i);
			//int scale = scale_string->intValue();
			//sprite->setScale(scale);

			CCString* point_string = (CCString*)point_array->objectAtIndex(i);
			CCPoint point = CCPointFromString(point_string->getCString());
			sprite->setPosition(ccp(size.width*point.x,size.height*point.y));

			this->addChild(sprite,1);
		}

		this->setTouchEnabled(false);
		bRet = true;
	} while (0);

	return bRet;
}

		



