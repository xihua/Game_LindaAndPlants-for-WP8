#ifndef __Game_Manage_H__
#define __Game_Manage_H__

#include "cocos2d.h"
#include "CCGeometry.h"
#include "SimpleAudioEngine.h"
#include <wrl\client.h>
#include <memory>
#include <vector>

#define GB_X gm->Gboard.x
#define GB_Y gm->Gboard.y
#define GB_W gm->Gboard.width
#define GB_H gm->Gboard.height
#define ROW_1 gm->Gboard.rowOneItemPixel
#define COL_1 gm->Gboard.colOneItemPixel


struct GameBoard{
	float x,y,width,height;
	float rowOneItemPixel,colOneItemPixel;
};
//GBoard是结构体变量

 struct Cell{
	int rowNo,colNo;
};
//struct NextItem{
//	int rowNo,colNo;
//};

struct CurItem{
public:
	int rowNo,colNo;
	int aftVId,befVId,aftHId,befHId;
	int same;
};

const int ITEMSROW=5,ITEMSCOL=8;

struct GameData{
	bool playInit;//是否第一次玩此关
	int gameScore,gameLevel;
	//int savedItems[ITEMSROW][ITEMSCOL];
};


class GameManage 
{
public: 
	//CCMutableArray * gameItems;
	int  Items[ITEMSROW][ITEMSCOL];
	CurItem CItem;
	CurItem NItem;
	GameBoard Gboard;
	GameData Gdata;

public:
	GameManage();
	~GameManage();
	static GameManage* GetInstance();
	void genItems();
	int ifHorSame(CurItem itm,int id);
	int ifVerSame(CurItem itm,int id);
	bool ifCrossSame(CurItem itm,bool setZero=true);
	bool isSwapItem(CurItem tmpCItem,CurItem tmpNItem,bool setZero=true);
	int scanAll();//遍历三个相同
	bool isMovable();
	bool updateSame();//遍历0消除并填充
	//void pScreen();
};

#endif 