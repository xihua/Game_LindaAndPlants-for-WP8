/*
* cocos2d-x   http://www.cocos2d-x.org
*
* Copyright (c) 2010-2011 - cocos2d-x community
* 
* Portions Copyright (c) Microsoft Open Technologies, Inc.
* All Rights Reserved
* 
* Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at 
* 
* http://www.apache.org/licenses/LICENSE-2.0 
* 
* Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an 
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and limitations under the License.
*/

#include "pch.h"

#include "MainMenu.h"
#include "GameScene.h"
#include "SelectMenu.h"

#include "CCCommon.h"
#include "BasicLoader.h"
#include <CCParticleExamples.h>
#include <vector>
#include <time.h>
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace Windows::Foundation;
using namespace Windows::Devices::Sensors;
using namespace CocosDenshion;


HelloWorld::HelloWorld()
{

}
HelloWorld::~HelloWorld()
{

}

CCScene* HelloWorld::scene()
{
	CCScene * scene = NULL;
	do 
	{		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloWorld *layer = HelloWorld::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	bool bRet = false;

	do 
	{
		if ( !CCLayer::init() )
		{
			break;
		}

		CCSize size = CCDirector::sharedDirector()->getWinSize();

		//添加背景音乐
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Assets\\Audio\\background.wav", true);//绝对路径

		//载入按钮
		CCDictionary* plistDic = CCDictionary::createWithContentsOfFile("Plist/MainMenu.plist");
		CCDictionary* Dic = (CCDictionary*)plistDic->objectForKey("button");
		CCArray* picture_array = (CCArray*)Dic->objectForKey("button_img");
		CCArray* point_array = (CCArray*)Dic->objectForKey("button_pos");
		CCArray* fun_array = (CCArray*)Dic->objectForKey("button_fun");

		for (int i=0;i<picture_array->count();i++)
		{
			CCString* pi_string = (CCString*)picture_array->objectAtIndex(i);//获取图片地址
			CCSprite* btn_sprite = CCSprite::create(pi_string->getCString());
			CCSprite* btn_sprite1 = CCSprite::create(pi_string->getCString());

			CCString* fun_string = (CCString*)fun_array->objectAtIndex(i);
			int fun_id = fun_string->intValue();//获取功能id
			CCMenuItemSprite* mplay=new CCMenuItemSprite();
            switch(fun_id){
				case 1:	
					mplay=CCMenuItemSprite::create(btn_sprite,btn_sprite1,NULL,this,menu_selector(HelloWorld::startGame));
						break;
				case 2:	
					mplay=CCMenuItemSprite::create(btn_sprite,btn_sprite1,NULL,this,menu_selector(HelloWorld::gotoGarden));
						break;
				case 3:	
					mplay=CCMenuItemSprite::create(btn_sprite,btn_sprite1,NULL,this,menu_selector(HelloWorld::gotoSetting));
						break;
				case 4:	
					mplay=CCMenuItemSprite::create(btn_sprite,btn_sprite1,NULL,this,menu_selector(HelloWorld::gotoHelp));
						break;
			}
			CCMenu* menu=CCMenu::createWithItem(mplay);

			CCString* point_string = (CCString*)point_array->objectAtIndex(i);//获取图片坐标
			CCPoint point = CCPointFromString(point_string->getCString());
			menu->setPosition(ccp(size.width*point.x,size.height*point.y));
			menu->setTouchEnabled(true);
			this->addChild(menu,10);
		}	

		//图片的载入

		picture_array = (CCArray*)plistDic->objectForKey("picture");
		point_array = (CCArray*)plistDic->objectForKey("point");
		//CCArray* scale_array = (CCArray*)plistDic->objectForKey("scale");
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

		//CCSprite* sprite = CCSprite::create("Img/Main_bg.png");
		//sprite->setPosition(ccp(size.width*0.5,size.height*0.5));
		//this->addChild(sprite,1);
	
		setTouchEnabled(false);//禁止触摸

		bRet = true;
	} while (0);

	return bRet;
}


//void HelloWorld::registerWithTouchDispatcher()
//{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
	//CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0);
//}

void HelloWorld::startGame(CCObject* sender)
{
	//切换到下一个场景
	CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::create(1,SelectMenu::scene()));

}

void HelloWorld::gotoGarden(CCObject* sender)
{
	//切换到下一个场景
	//CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::create(1,GameScene::scene()));

}

void HelloWorld::gotoSetting(CCObject* sender)
{
	//切换到下一个场景
	//CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::create(1,GameScene::scene()));

}

void HelloWorld::gotoHelp(CCObject* sender)
{
	//切换到下一个场景
	//CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::create(1,GameScene::scene()));

}