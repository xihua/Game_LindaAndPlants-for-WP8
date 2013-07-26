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

#include "TouchLayer.h"
#include "GameScene.h"
#include "GameManage.h"


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

		bRet = true;
	} while (0);

	return bRet;
}



bool TouchLayer::ccTouchBegan(CCTouch* touch, CCEvent *event)
{
		return true;
}

void TouchLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{

}

void TouchLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint curPixelPos=touch->getLocationInView();
	curPixelPos=CCDirector::sharedDirector()->convertToGL(curPixelPos);
	this->getDelegate()->singleTouchEnd();
}

void TouchLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);

}


