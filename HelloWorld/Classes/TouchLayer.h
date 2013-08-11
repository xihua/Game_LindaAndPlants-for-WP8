
#ifndef __TOUCH_LAYER_H__
#define __TOUCH_LAYER_H__

#include "cocos2d.h"
#include "CCGeometry.h"
#include "SimpleAudioEngine.h"
#include <wrl\client.h>
#include <memory>
#include <vector>


class TouchLayerDelegate{//触摸代理类
public:
	virtual void singleTouchEnd()=0; 
	virtual void singleTouchMove(int Move_dir,int Move_rbk)=0; 
	virtual void updateStr(int type)=0; 
};


class TouchLayer : public cocos2d::CCLayerColor//,public GameManage
{
public:
	TouchLayer();
	~TouchLayer();

	virtual bool init();  
	// a selector callback
	//virtual void menuCloseCallback(CCObject* pSender);

public:
	
	//触摸代理类的实现 很重要！！
	CC_SYNTHESIZE(TouchLayerDelegate*,m_pDelegate,Delegate);
	// implement the "static create()" method manually
	CREATE_FUNC(TouchLayer);//!!!自动生成工厂方法create
	void registerWithTouchDispatcher();
private:
	void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

};

#endif  // __TOUCH_LAYER_H__