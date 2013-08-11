#include "pch.h"
    
#include "GameScene.h"
#include "SelectMenu.h"
#include "GameManage.h"
#include "Effects.h"
#include "GameControl.h"

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

#define TagScore 1001
#define TagStr 1002
//const char gameStr[3][50]={"好眼力！","这也能看到?!","洗牌"};

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
			this->addChild(menu,30);
		}	

		//图片的载入

		picture_array =AR_FM_PL("picture");
		point_array = AR_FM_PL("point");
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

		//得分和文字的载入
		CCString *str=CCString::createWithFormat("%d",gm->Gdata.gameScore);
		CCLabelTTF* pLabel = CCLabelTTF::create(str->getCString(), "msyh", 30);
		pLabel->setPosition( ccp(size.width * 0.15, size.height * 0.85) );
		pLabel->setColor(ccc3(60, 180, 150));
		this->addChild(pLabel, 10,TagScore);
		//updateStr(1);


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
	SimpleAudioEngine::sharedEngine()->playEffect("Assets\\Audio\\change.wav",false);



	//粒子效果有问题
	//Effects *ge=Effects ::GetInstance();
	//ge->popEffect->setAutoRemoveOnFinish(true);	
	//this->addChild(ge->popEffect,10);
	//ge->popEffect->setPosition(ccp(gm->Gboard.x+COL_1*(gm->NItem.colNo+.5),gm->Gboard.y+ROW_1*(gm->NItem.rowNo+.5)));
	//ge->popEffect->retain();

	CCParticleSystem* popEffect = CCParticleFlower::create();
	//popEffect->retain();
	popEffect->setAutoRemoveOnFinish(true);	
	popEffect->setTexture(CCTextureCache::sharedTextureCache()->addImage("Img/stars.png"));
	popEffect->setDuration(.05);
	popEffect->setEmissionRate(100);
	popEffect->setPosVar(ccp(5,5));
	this->addChild(popEffect,10);
	popEffect->setPosition(ccp(gm->Gboard.x+COL_1*(gm->NItem.colNo+.5),gm->Gboard.y+ROW_1*(gm->NItem.rowNo+.5)));
	
	//得分更新
	CCString *str=CCString::createWithFormat("%d",gm->Gdata.gameScore);
	CCLabelTTF* Sp = (CCLabelTTF*)this->getChildByTag(1001);
	Sp->setString(str->getCString());

	for(int i=0;i<ITEMSROW;i++)
		for(int j=0;j<ITEMSCOL;j++){updateItem(i*ITEMSCOL+j+1,gm->Items[i][j]);}


}

void GameScene::singleTouchMove(int Move_dir,int Move_rbk){
	GameManage * gm=GameManage::GetInstance();	
	CCActionInterval *Back=NULL,*NBack=NULL,*Move=NULL;
	switch(Move_dir) 
	{
	case 1:Move=CCMoveBy::create(.2f,ccp(gm->Gboard.colOneItemPixel,0));break;//向右
	case 2:Move=CCMoveBy::create(.2f,ccp(-gm->Gboard.colOneItemPixel,0));break;
	case 3:Move=CCMoveBy::create(.2f,ccp(0,gm->Gboard.colOneItemPixel));break;//向下
	case 4:Move=CCMoveBy::create(.2f,ccp(0,-gm->Gboard.colOneItemPixel));break;
	}
	
	CCActionInterval* ca= CCEaseOut::create((CCActionInterval*)(Move->copy()->autorelease()), 1.0f);	
	if(Move_rbk) Back=((CCActionInterval*)ca->copy())->reverse();
	else {Back=((CCActionInterval*)Move->copy())->reverse();Back->setDuration(0);}

	int i=gm->CItem.rowNo,j=gm->CItem.colNo;
	CCSprite* Sp = (CCSprite*)this->getChildByTag(i*ITEMSCOL+j+1);
	CCFiniteTimeAction* action= CCSequence::create(ca,Back,NULL); 
	Sp->runAction((CCActionInterval*)action->copy()->autorelease());

	i=gm->NItem.rowNo;j=gm->NItem.colNo;
	Sp = (CCSprite*)this->getChildByTag(i*ITEMSCOL+j+1);
	//Sp->runAction(ca->reverse());
	ca= CCEaseOut::create((CCActionInterval*)(Move->reverse()->copy()->autorelease()), 1.0f);
	//CCDelayTime* delayTime = CCDelayTime::create(0.5);
    //CCFiniteTimeAction* callFunc = CCCallFunc::create(this, callfunc_selector(GameScene::singleTouchEnd));//序列加入此执行崩溃
	if(Move_rbk) {NBack=((CCActionInterval*)ca->copy())->reverse();/*callFunc=NULL;*/}
	else {NBack=(CCActionInterval*)Move->copy();NBack->setDuration(0);}
	action= CCSequence::create(ca,NBack/*,callFunc*/,NULL); 
	Sp->runAction(action);

}

void GameScene::pauseGame(CCObject *sender){
	//CCDirector::sharedDirector()->pause();
	PauseLayer *pauLayer=PauseLayer::create();
	this->addChild(pauLayer,15,1002);
}

void GameScene::backGame(CCObject *sender){
	CCDirector::sharedDirector()->replaceScene(CCTransitionShrinkGrow::create(1,SelectMenu::scene()));	
}

void GameScene::initItem(){
	GameManage * gm=GameManage::GetInstance();
	if(gm->Gdata.playInit) gm->genItems();//是否第一次玩此关
	///while(gm->scanAll());//生成没有三个相同的棋盘
	//while(!gm->isMovable()) gm->genItems();//是否洗牌
	//gm->pScreen();//test
	for(int i=0;i<ITEMSROW;i++)
		for(int j=0;j<ITEMSCOL;j++){
			CCString *str=CCString::createWithFormat("gr_%d.png", gm->Items[i][j]);
			//CCLog("%d,%dItem:%d",i,j,gm->Items[i][j]);
			//CCMotionStreak *sprite=CCMotionStreak::create(2, 3, 32, ccGREEN,"");
			CCSprite* sprite=CCSprite::createWithSpriteFrameName(str->getCString());
			//CCSprite* sprite = CCSprite::create(str->getCString());
			sprite->setAnchorPoint(ccp(0,0));
			sprite->setPosition(ccp(gm->Gboard.x+COL_1*j,gm->Gboard.y+ROW_1*i));
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
void GameScene::updateStr(int type){
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCString *str=CCString::createWithFormat("Img/GameStr_%d.png",type);
	CCSprite* sprite=CCSprite::create(str->getCString());	
	sprite->setPosition(ccp(size.width*0.8, size.height*0.05));
	sprite->setScale(0.1);
	this->addChild(sprite,10,TagStr);
		CCActionInterval* MoveToM= CCMoveTo::create(.2f,ccp(size.width*0.7, size.height*.2));
		CCActionInterval* ScB=CCScaleBy::create(.2f,4);		
		CCDelayTime *waiting=CCDelayTime::create(.5);  
		CCActionInterval* fX=CCFadeOut::create(0.5); 
		CCActionInterval* move_ease_out = CCEaseOut::create((CCActionInterval*)(MoveToM->copy()->autorelease()), 2.0f);
		CCActionInterval* Spa=CCSpawn::createWithTwoActions(move_ease_out,ScB);
		
		CCActionInterval* MoveToM2=CCMoveTo::create(.2f,ccp(size.width*0.7, size.height*.1));		
		CCActionInterval* move_ease_out2 = CCEaseOut::create((CCActionInterval*)(MoveToM2->copy()->autorelease()), 2.0f);
		CCActionInterval* Spa2=CCSpawn::createWithTwoActions(move_ease_out2,ScB);

		CCFiniteTimeAction* action;
		if(type==3) action= CCSequence::create(waiting,Spa2,fX,NULL);  //动画序列
		else action= CCSequence::create(Spa,waiting,fX,NULL);
		sprite->runAction(action);
		//this->removeChild(sprite,false);
}