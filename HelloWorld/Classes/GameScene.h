#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"
#include "CCGeometry.h"
#include "SimpleAudioEngine.h"
#include <wrl\client.h>
#include <memory>
#include <vector>

#include "TouchLayer.h"
//#include "GameManage.h"
#define AR_FM_PL(key) (CCArray*)plistDic->objectForKey(key)

class GameScene : public cocos2d::CCLayerColor,public TouchLayerDelegate
{
public:
	GameScene();
	~GameScene();


	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	// a selector callback
	//virtual void menuCloseCallback(CCObject* pSender);

	// implement the "static create()" method manually
	 CREATE_FUNC(GameScene);

	//void registerWithTouchDispatcher();
	void singleTouchEnd(); 
	void singleTouchMove(); 
	void pauseGame(CCObject *sender);
	void backGame(CCObject *sender);
	void initItem();
	bool updateItem(int tag,int pic);//Ë¢ÐÂÌî³äÍ¼Æ¬

private: 
	int SoundId;
};

#endif  // __GameScene_SCENE_H__