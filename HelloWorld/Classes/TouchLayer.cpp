﻿#include "pch.h"

#include "CCCommon.h"
#include "BasicLoader.h"
#include "SimpleAudioEngine.h"

#include "TouchLayer.h"
#include "GameManage.h"

using namespace cocos2d;
using namespace Windows::Foundation;
using namespace Windows::Devices::Sensors;
using namespace CocosDenshion;


TouchLayer::TouchLayer()
{
}
TouchLayer::~TouchLayer()
{

}

bool TouchLayer::init()
{

	bool bRet = false;

	do 
	{
		if ( !CCLayerColor::init() )
		{
			break;
		}
		this->setTouchEnabled(true);
		this->setDelegate(NULL);
		CCSize size=CCDirector::sharedDirector()->getWinSize();

	//			//初始化

	//NItem.rowNo=NItem.colNo=0;

	//CItem.rowNo=CItem.colNo=0;
	//CItem.aftVId=CItem.befVId=CItem.aftHId=CItem.befHId=0;
	//CItem.sameCount=0;

	//Gboard.x=size.width*.5;Gboard.y=size.height*.5;
	//Gboard.colOneItemPixel=Gboard.rowOneItemPixel=60.0f;
	//Gboard.width= Gboard.colOneItemPixel*3;
	//Gboard.height= Gboard.rowOneItemPixel*3;

		bRet = true;
	} while (0);

	return bRet;
}



bool TouchLayer::ccTouchBegan(CCTouch* touch, CCEvent *event)
{
	   		//CCTouch *touch=dynamic_cast<CCTouch*>(touch->anyObject());
		CCPoint curPixelPos=touch->getLocationInView();
		curPixelPos=CCDirector::sharedDirector()->convertToGL(curPixelPos);		
		GameManage * gm=GameManage::GetInstance();
		//判断在GameBoard范围
		float difWd =curPixelPos.x-  gm->Gboard.x;
		float difHt =curPixelPos.y-   gm->Gboard.y;
		//CCLog("CItem.xy:%f,%f",curPixelPos.x,curPixelPos.y);
		//CCLog("dif.xy:%f,%f",difWd,difHt);
		if( difWd>=0 && difWd<=   gm->Gboard.width && difHt>=0 && difHt<=  gm->Gboard.height){
			   gm->CItem.rowNo =difHt/   gm->Gboard.rowOneItemPixel;
			// CCLog("CItem.rowNo:%d",  gm->CItem.rowNo);
			   gm->CItem.colNo =difWd/   gm->Gboard.colOneItemPixel;
			// CCLog("CItem.colNo:%d",  gm->CItem.colNo);
		
		return true;
		}
		return false;
}

void TouchLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void TouchLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint curPixelPos=touch->getLocationInView();
	curPixelPos=CCDirector::sharedDirector()->convertToGL(curPixelPos);
	GameManage * gm=GameManage::GetInstance();
			//判断在GameBoard范围
		float difWd =curPixelPos.x-  gm->Gboard.x;
		float difHt =curPixelPos.y-  gm->Gboard.y;
		if( difWd>=0 && difWd<=  gm->Gboard.width && difHt>=0 && difHt<=  gm->Gboard.height){
			  gm->NItem.rowNo =difHt/gm->Gboard.rowOneItemPixel;
			 //CCLog("NItem.rowNo:%d", gm->NItem.rowNo);
			  gm->NItem.colNo =difWd/gm->Gboard.colOneItemPixel;
			 //CCLog("NItem.colNo:%d", gm->NItem.colNo);
			 int difCol= gm->NItem.colNo- gm->CItem.colNo ;
			 int difRow= gm->NItem.rowNo- gm->CItem.rowNo;
			 if((difCol==0||difRow==0)&&(difCol!=difRow)&&(abs(difCol)==1||abs(difRow)==1)) {//是否相邻
				// SimpleAudioEngine::sharedEngine()->playEffect("Assets\\Audio\\move.wav", false);
				// gm->pScreen();//test
				 if(gm->isSwapItem(gm->CItem,gm->NItem))//检测交换是否成功
					{
					//gm->pScreen();//test
					gm->updateSame();gm->Gdata.gameScore+=10;//CCLog("swap success");
					//gm->pScreen();//test
					while(gm->scanAll()) {gm->Gdata.gameScore+=15;CCLog("Great!");}//连击加分	
					//gm->pScreen();//test
					if(!(gm->isMovable())) {gm->genItems();CCLog("reset board!");}//洗牌
					//gm->pScreen();//test
					this->getDelegate()->singleTouchEnd();
					
				 }
				 else CCLog("swap fail");}
		}
}

void TouchLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);

}


