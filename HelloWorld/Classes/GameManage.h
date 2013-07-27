#ifndef __Game_Manage_H__
#define __Game_Manage_H__

#include "cocos2d.h"
#include "CCGeometry.h"
#include "SimpleAudioEngine.h"
#include <wrl\client.h>
#include <memory>
#include <vector>


struct GameBoard{
	float x,y,width,height;
	float rowOneItemPixel,colOneItemPixel;
};
//GBoard�ǽṹ�����

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
	bool playInit;//�Ƿ��һ����˹�
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
	int scanAll();//����������ͬ
	bool isMovable();
	bool updateSame();//����0���������
};

#endif 