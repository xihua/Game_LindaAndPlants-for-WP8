﻿/*
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
    
#include "GameScene.h"
#include "SelectMenu.h"
#include "GameManage.h"
#include "Effects.h"

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


GameScene::~GameScene()
{

}

GameScene::GameScene()
{

}

CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do 
	{		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	bool bRet = false;

	do 
	{
		if ( !CCLayer::init() )
		{
			break;
		}
		//SimpleAudioEngine::sharedEngine()->stopAllEffects();

		CCString * pString,*SoundString;
		CCSize size=CCDirector::sharedDirector()->getWinSize();
		CCSpriteFrameCache *cache;
		cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("Img/Plants/plants.plist","Img/Plants/plants.png");
		GameManage * gm=GameManage::GetInstance();

		//载入按钮
		CCDictionary* plistDic = CCDictionary::createWithContentsOfFile("Plist/GameScene.plist");
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
					mplay=CCMenuItemSprite::create(btn_sprite,btn_sprite1,NULL,this,menu_selector(GameScene::pauseGame));
						break;
				case 0:	
					mplay=CCMenuItemSprite::create(btn_sprite,btn_sprite1,NULL,this,menu_selector(GameScene::backGame));
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

		//	//CCString* scale_string = (CCString*)scale_array->objectAtIndex(i);
		//	//int scale = scale_string->intValue();
		//	//sprite->setScale(scale);

			CCString* point_string = (CCString*)point_array->objectAtIndex(i);
			CCPoint point = CCPointFromString(point_string->getCString());
			sprite->setPosition(ccp(size.width*point.x,size.height*point.y));

			this->addChild(sprite,1);
		}	

		//游戏的载入
		//plistDic = CCDictionary::createWithContentsOfFile("Plist/Level/l_0.plist");
		//picture_array = (CCArray*)plistDic->objectForKey("picture");
		//point_array = (CCArray*)plistDic->objectForKey("point");
		////CCArray* scale_array = (CCArray*)plistDic->objectForKey("scale");
		//for (int i=0;i<picture_array->count();i++)
		//{
		//	CCString* pi_string = (CCString*)picture_array->objectAtIndex(i);
		//	CCSprite* sprite = CCSprite::create(pi_string->getCString());

		////	//CCString* scale_string = (CCString*)scale_array->objectAtIndex(i);
		////	//int scale = scale_string->intValue();
		////	//sprite->setScale(scale);

		//	CCString* point_string = (CCString*)point_array->objectAtIndex(i);
		//	CCPoint point = CCPointFromString(point_string->getCString());
		//	sprite->setPosition(ccp(size.width*point.x,size.height*point.y));

		//	this->addChild(sprite,5);
		//}

		//GameBoard图片加入
		CCSprite* sprite = CCSprite::create("Img/Game_board.png");
		sprite->setAnchorPoint(ccp(0,0));
		sprite->setPosition(ccp(size.width*.22,size.height*.19));
		this->addChild(sprite,4);
		//游戏Items载入
		initItem();



		//加入触摸层
		TouchLayer *tLayer=TouchLayer::create();
		tLayer->setIsRelativeAnchorPoint(true);
		tLayer->setPosition( ccp(size.width/2, size.height/2) );
		//tLayer->setAnchorPoint(ccp(0,0));
		 CCSize newSize = CCSizeMake( size.width/2,  size.height/2);
		//tLayer->setContentSizeInPixels(newSize);//60*8*,*60*
		tLayer->setDelegate(this);
		tLayer->setColor(ccColor3B());
		this->addChild(tLayer,20);
	
		//setTouchEnabled(false);


		bRet = true;
	} while (0);

	return bRet;
}

void GameScene::singleTouchEnd(){
	//CCDirector* pDirector = CCDirector::sharedDirector();
	GameManage * gm=GameManage::GetInstance();
	// CCSprite* CurSp = (CCSprite*)this->getChildByTag(gm->CItem.colNo*3+gm->CItem.colNo+1);

	//粒子效果有问题
	//Effects *ge=Effects ::GetInstance();
	//ge->popEffect->setAutoRemoveOnFinish(true);	
	//this->addChild(ge->popEffect,10);
	//ge->popEffect->setPosition(ccp(gm->Gboard.x+gm->Gboard.colOneItemPixel*(gm->NItem.colNo+.5),gm->Gboard.y+gm->Gboard.rowOneItemPixel*(gm->NItem.rowNo+.5)));
	//ge->popEffect->retain();

	CCParticleSystem* popEffect = CCParticleFlower::create();
	//popEffect->retain();
	popEffect->setAutoRemoveOnFinish(true);	
	popEffect->setTexture(CCTextureCache::sharedTextureCache()->addImage("Img/stars.png"));
	popEffect->setDuration(.05);
	popEffect->setEmissionRate(100);
	popEffect->setPosVar(ccp(5,5));
	this->addChild(popEffect,10);
	popEffect->setPosition(ccp(gm->Gboard.x+gm->Gboard.colOneItemPixel*(gm->NItem.colNo+.5),gm->Gboard.y+gm->Gboard.rowOneItemPixel*(gm->NItem.rowNo+.5)));
	

	for(int i=0;i<ITEMSROW;i++)
		for(int j=0;j<ITEMSCOL;j++){updateItem(i*ITEMSCOL+j+1,gm->Items[i][j]);}
	//updateItem(gm->CItem.rowNo*ITEMSROW+gm->CItem.colNo+1,gm->Items[gm->CItem.rowNo][gm->CItem.colNo]);
	//updateItem(gm->NItem.rowNo*ITEMSROW+gm->NItem.colNo+1, gm->Items[gm->NItem.rowNo][gm->NItem.colNo]);//更新CItem,NItem

}

void GameScene::singleTouchMove(){

}

void GameScene::pauseGame(CCObject *sender){
	CCDirector::sharedDirector()->pause();
}

void GameScene::backGame(CCObject *sender){
}

void GameScene::initItem(){
	GameManage * gm=GameManage::GetInstance();
	if(gm->Gdata.playInit) gm->genItems();//是否第一次玩此关
	///while(gm->scanAll());//生成没有三个相同的棋盘
	//while(!gm->isMovable()) gm->genItems();//是否洗牌
	for(int i=0;i<ITEMSROW;i++)
		for(int j=0;j<ITEMSCOL;j++){
			CCString *str=CCString::createWithFormat("gr_%d.png", gm->Items[i][j]);
			//CCLog("%d,%dItem:%d",i,j,gm->Items[i][j]);
			CCSprite* sprite=CCSprite::createWithSpriteFrameName(str->getCString());
			//CCSprite* sprite = CCSprite::create(str->getCString());
			sprite->setAnchorPoint(ccp(0,0));
			sprite->setPosition(ccp(gm->Gboard.x+gm->Gboard.colOneItemPixel*j,gm->Gboard.y+gm->Gboard.rowOneItemPixel*i));
			sprite->setScale(0.6);
			this->addChild(sprite,5,i*ITEMSCOL+j+1);
			//CCLog("%d,%dItem:%d",i,j,i*ITEMSROW+j+1);
		}

}
bool GameScene::updateItem(int tag,int pic){
	GameManage * gm=GameManage::GetInstance();
	//CCLog("tag%d",tag);
	CCString *str=CCString::createWithFormat("gr_%d.png", pic);
	CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString());  
	//CCLog("curpic%d",pic);
	CCSprite* Sp = (CCSprite*)this->getChildByTag(tag);
	Sp->setDisplayFrame(frame);

	//CCString *str=CCString::createWithFormat("gr_%d.png", gm->Items[gm->CItem.rowNo][gm->CItem.colNo]);
	//CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString());  
	//CCLog("curpic%d",gm->Items[gm->CItem.rowNo][gm->CItem.colNo]);
	//CCSprite* Sp = (CCSprite*)this->getChildByTag(bTag);
	//Sp->setDisplayFrame(frame);

	//str=CCString::createWithFormat("gr_%d.png", gm->Items[gm->NItem.rowNo][gm->NItem.colNo]);
	//frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString());
	//CCLog("curpic%d",gm->Items[gm->NItem.rowNo][gm->NItem.colNo]);
	//Sp = (CCSprite*)this->getChildByTag(ntag);
	//Sp->setDisplayFrame(frame);


	return true;
}
