
#ifndef __GAME_MANAGE_H__
#define __GAME_MANAGE_H__

#include "cocos2d.h"
#include "CCGeometry.h"
#include "SimpleAudioEngine.h"
#include <wrl\client.h>
#include <memory>
#include <vector>
const int ITEMSROW=5,ITEMSCOL=8;

struct GameBoard{
	CCfloat x,y,width,height;
	CCfloat rowOneItemPixel,colOneItemPixel;
};
//GBoard是结构体变量

class CurItemClass{
public:
	CCint rowNo,colNo;
	CCint aftVId,befVId,aftHId,befHId;
	CCint sameCount;
};

class GameManage 
{
public:   
	//CCMutableArray * gameItems;
	CCint  gameItems[ITEMSROW][ITEMSCOL];
	static CurItemClass *curItem;
	//static GameBoard Gboard;

public:
	GameManage();
	~GameManage();
	bool hasSame();
	bool hasVerSame();
	bool hasHorSame();
	bool hasCrossSame();
	bool popSame();
	// implement the "static create()" method manually
	// CREATE_FUNC(GameManage);

protected:

private:

};

#endif 