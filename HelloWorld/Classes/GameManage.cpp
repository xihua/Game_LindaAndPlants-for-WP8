#include "pch.h"
#include "CCCommon.h"
#include <vector>
#include <queue>
#include <time.h>
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

#include "GameManage.h"
using namespace cocos2d;
using std::queue;
using namespace CocosDenshion;

static GameManage* hGameManage = NULL;
GameManage*  GameManage::GetInstance()//获得当前的GamaMange实例
{
	if (hGameManage == NULL)
		hGameManage = new GameManage();

	return hGameManage;
}

GameManage::GameManage()
{
	CCSize size=CCDirector::sharedDirector()->getWinSize();

	//初始化items数组
	//Items[3][3]={{1,2,1},{2,1,2},{1,2,1}};
	for(int i=0;i<ITEMSROW;i++) 
		for(int j=0;j<ITEMSCOL;j++) Items[i][j]=1;
	//Items[0][2]=2;

	//初始化
	NItem.rowNo=NItem.colNo=0;
	NItem.befVId=NItem.befHId=-1;			
	NItem.aftVId=NItem.aftHId=1;
	
	//CItem.aftVId=CItem.befVId=CItem.aftHId=CItem.befHId=0;
	CItem.rowNo=CItem.colNo=0;
	CItem.befVId=CItem.befHId=-1;			
	CItem.aftVId=CItem.aftHId=1;
	CItem.same=0;
	//初始化gameBoard
	Gboard.x=size.width*.255;Gboard.y=size.height*.3;
	Gboard.colOneItemPixel=Gboard.rowOneItemPixel=60.0f;
	Gboard.width= Gboard.colOneItemPixel*ITEMSCOL;
	Gboard.height= Gboard.rowOneItemPixel*ITEMSROW;
	//初始化GameData
	Gdata.gameLevel=1;
	Gdata.gameScore=0;
	Gdata.playInit=1;

}
void GameManage::genItems(){//随机生成数组
	do{
		for(int i=0;i<ITEMSROW;i++) 
			for(int j=0;j<ITEMSCOL;j++) 
			{Items[i][j]=(int)(9*CCRANDOM_0_1())+1;/*CCLog("%d,%d:%d",i,j,Items[i][j]);*/}
			while(scanAll());	
	}while(!isMovable());
	//while(gm->scanAll());//生成没有三个相同的棋盘
	//if(isMovable()) CCLog("movable");
	//else CCLog("regen");
}

 bool GameManage::ifCrossSame(CurItem itm,bool setZero){	 
	 if(itm.befHId==-1  ){itm.befHId=ifHorSame(itm,itm.befHId);}
	 if(itm.aftHId==1)	{itm.aftHId=ifHorSame(itm,itm.aftHId);}
	 if(itm.befVId==-1 ){itm.befVId=ifVerSame(itm,itm.befVId);}
	 if(itm.aftVId==1)	{itm.aftVId=ifVerSame(itm,itm.aftVId);}
	 //cout<<jud; 		
		 if(((itm.befHId+itm.aftHId)>=2 )|| ((itm.befVId+itm.aftVId)>=2)){//相邻大于三个相同则为零
			 if(setZero==true){
			 for(;itm.aftHId>0;itm.aftHId--) Items[itm.rowNo][itm.colNo+itm.aftHId]=0;
			 for(;itm.befHId>0;itm.befHId--) Items[itm.rowNo][itm.colNo-itm.befHId]=0;
			 for(;itm.aftVId>0;itm.aftVId--) Items[itm.rowNo+itm.aftVId][itm.colNo]=0;
			 for(;itm.befVId>0;itm.befVId--) Items[itm.rowNo-itm.befVId][itm.colNo]=0;			 
			 Items[itm.rowNo][itm.colNo]=0;}
		return 1;}
	 else return 0;
 }

bool GameManage::updateSame(){

	queue<int> sq;
	for(int i=0;i<ITEMSROW;i++)
		for(int j=0;j<ITEMSCOL;j++){//遍历0
				if(Items[i][j]==0) {
					/*cout<<endl<<"i,j"<<i<<" "<<j;*/
					int sameC=0;//偏移量
					
					for(int r=1;r<ITEMSROW;r++){
					if(Items[i+r][j]==0){//列向遍历0
						sameC++;}
					else break;
					}
					sq.empty();
					//sq.count=-1;
					for(int r=1;i+sameC+r<ITEMSROW;r++){
						sq.push(Items[i+sameC+r][j]);
						//++sq.count;//暂存入队列
						//sq.row[sq.count]=Items[i+sameC+r][j];
						}
					for(int r=0;r<sameC+1;r++){
						sq.push(rand()%9+1);
						//++sq.count;//补充sameC+1个随机数入队列
						//sq.row[sq.count]=rand()%9+1;
						}
					//cout<<endl;
					// for(int r=0;r<=sq.count;r++){
					//	 cout<<sq.row[sq.count]<<" ";
					// }
					//取出队列 并下移
					for(int r=0;i+r<ITEMSROW;r++){
						Items[i+r][j]=sq.front();sq.pop();}
						//Items[i+r][j]=sq.row[r];
				}
		}
	 return 1;
}
int GameManage::ifHorSame(CurItem itm,int id){	
	 int i=1,sameC=0;//sameC为偏移量
	 do{		 
		 if(itm.same==Items[itm.rowNo][itm.colNo+id*i])
		 {sameC++;
		 //if(sameC>=2){Items[itm.rowNo][itm.colNo+id*i]=0;//相邻大于三个相同则为零
		 //Items[itm.rowNo][itm.colNo+1]=Items[itm.rowNo][itm.colNo+2]=0;}
		 i++;}
		 else break;
	}while(0<=itm.colNo+id*i && itm.colNo+id*i<ITEMSCOL);
	 return sameC;
}

int GameManage::ifVerSame(CurItem itm,int id){	
	 int i=1,sameC=0;
	 do{		 
		 if(itm.same==Items[itm.rowNo+id*i][itm.colNo])
		 {sameC++;
/*		 if(sameC>=2){Items[itm.rowNo+id*i][itm.colNo]=0;
		 Items[itm.rowNo+1][itm.colNo]=Items[itm.rowNo+2][itm.colNo]=0;;}	*/	 
		 i++;}
		 else break;
	}while(0<=itm.rowNo+id*i && itm.rowNo+id*i<ITEMSROW);
	 return sameC;
}

bool swap(int &ct,int&nt){
	int tmp;tmp=ct;ct=nt;nt=tmp;
	return 1;}

bool GameManage::isSwapItem(CurItem tmpCItem,CurItem tmpNItem,bool setZero){
	int cTmp,nTmp;
	cTmp=Items[tmpCItem.rowNo][tmpCItem.colNo];
	nTmp=Items[tmpCItem.rowNo][tmpCItem.colNo]=Items[tmpNItem.rowNo][tmpNItem.colNo];
	Items[tmpNItem.rowNo][tmpNItem.colNo]=cTmp;

	tmpCItem.same=Items[tmpCItem.rowNo][tmpCItem.colNo];
	tmpNItem.same=Items[tmpNItem.rowNo][tmpNItem.colNo];
	if(ifCrossSame(tmpCItem,setZero)||ifCrossSame(tmpNItem,setZero)){ 
		return 1;}
	else {
		Items[tmpCItem.rowNo][tmpCItem.colNo]=cTmp;//交换回去
		Items[tmpNItem.rowNo][tmpNItem.colNo]=nTmp;
		return 0;
	}
	//int cTmp,nTmp;
	//cTmp=Items[tmpCItem.rowNo][tmpCItem.colNo];
	//nTmp=Items[tmpCItem.rowNo][tmpCItem.colNo];
	//Items[tmpCItem.rowNo][tmpCItem.colNo]=Items[tmpNItem.rowNo][tmpNItem.colNo];
	//Items[tmpNItem.rowNo][tmpNItem.colNo]=cTmp;
	//
	//tmpCItem.same=Items[tmpCItem.rowNo][tmpCItem.colNo];
	//tmpNItem.same=Items[tmpNItem.rowNo][tmpNItem.colNo];
	////CCLog("swap%d,%d:%d",tmpCItem.rowNo,tmpCItem.colNo,tmpCItem.same);
	////CCLog("and%d,%d:%d",tmpNItem.rowNo,tmpNItem.colNo,tmpNItem.same);
	//if(ifCrossSame(tmpCItem,setZero)||ifCrossSame(tmpNItem,setZero)){ 
	//	return 1;}
	//else {
	//	Items[tmpCItem.rowNo][tmpCItem.colNo]=cTmp;//交换回去
	//	Items[tmpNItem.rowNo][tmpNItem.colNo]=nTmp;
	//	return 0;
	//}
}
int GameManage::scanAll(){
	CurItem tmpItem;
	 queue<Cell>sq;			
	tmpItem.aftVId=tmpItem.aftHId=1;
	//遍历整个GameBoard
	 for(int i=0;i<ITEMSROW;i++)
		 for(int j=0;j<ITEMSCOL;j++){			 
			tmpItem.rowNo=i;	tmpItem.colNo=j;
			tmpItem.same=Items[tmpItem.rowNo][tmpItem.colNo];
			int HSame=ifHorSame(tmpItem,tmpItem.aftHId);
			int VSame=ifVerSame(tmpItem,tmpItem.aftVId);			
			if(VSame>=2){					
				for(int r=1;r<=VSame;r++) {//纵向大于三个相同则入队
					Cell tmpC;tmpC.rowNo=i+r;tmpC.colNo=j;sq.push(tmpC);}
				}
			if(HSame>=2){	
				for(int c=0;c<=HSame;c++){//横向大于三个相同则入队
					Cell tmpC;tmpC.rowNo=i;tmpC.colNo=j+c;sq.push(tmpC);}
				}
		}
	 if(sq.empty()) return 0;
	 else {
		 int sqCount=0;
	//弹出队列，并置为零
	 while(!sq.empty()){
		Cell tmpC=sq.front();
		//cout<<"tmpC:"<<tmpC.rowNo<<"-"<<tmpC.colNo;
		Items[tmpC.rowNo][tmpC.colNo]=0;
		sq.pop();sqCount++;}
		updateSame();
		return sqCount;
	 }
}

bool GameManage::isMovable(){
	CurItem tmpCItem,tmpNItem;		
	tmpCItem.befVId=tmpCItem.befHId=tmpNItem.befVId=tmpNItem.befHId=-1;
	tmpCItem.aftVId=tmpCItem.aftHId=tmpNItem.aftVId=tmpNItem.aftHId=1;

	//遍历整个GameBoard
	 for(int i=0;i<ITEMSROW;i++)
		 for(int j=0;j<ITEMSCOL;j++){
			 //CCLog("%d,%d",i,j);
			 tmpCItem.rowNo=i;tmpCItem.colNo=j;
			 tmpCItem.same=Items[tmpCItem.rowNo][tmpCItem.colNo];
 			
			 tmpNItem.rowNo=i;tmpNItem.colNo=j+1;//横向交换
			 tmpNItem.same=Items[tmpNItem.rowNo][tmpNItem.colNo];
			 
			 //printf("HorN%d,%d:%d\n",tmpNItem.rowNo,tmpNItem.colNo, tmpNItem.same);
			 if(isSwapItem(tmpCItem,tmpNItem,false) && tmpNItem.colNo<ITEMSCOL)//检测交换是否成功
					{swap(Items[tmpCItem.rowNo][tmpCItem.colNo],Items[tmpNItem.rowNo][tmpNItem.colNo]);//交换回去
					CCLog("%d,%d,Hor success\n",tmpCItem.rowNo,tmpCItem.colNo);
					// printf("%d,%d,Hor success\n",tmpCItem.rowNo,tmpCItem.colNo);
					 return 1;}	

			tmpNItem.rowNo=i+1;tmpNItem.colNo=j;//纵向交换
			 tmpNItem.same=Items[tmpNItem.rowNo][tmpNItem.colNo];
			 //printf("VerN%d,%d:%d\n",tmpNItem.rowNo,tmpNItem.colNo, tmpNItem.same);
			 if(isSwapItem(tmpCItem,tmpNItem,false) && tmpNItem.rowNo<ITEMSROW)//检测交换是否成功
					{swap(Items[tmpCItem.rowNo][tmpCItem.colNo],Items[tmpNItem.rowNo][tmpNItem.colNo]);//交换回去
					CCLog("%d,%d,Vor success\n",tmpCItem.rowNo,tmpCItem.colNo);
					//printf("%d,%d,Vor success\n",tmpCItem.rowNo,tmpCItem.colNo);
				return 1;}	
		 }
	return 0;	
}

GameManage::~GameManage()
{
	delete hGameManage;
}