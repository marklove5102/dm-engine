#pragma once

#include "Global/Global.h"
#include "GameData/SimpleGood.h"
#include "GameData/Good.h"

typedef list<DWORD> ListItem;

class CAIAutoPickMgr
{
public:
	CAIAutoPickMgr(void);
	~CAIAutoPickMgr(void);

	void LoadBestItems();
	void PetPickupClear();
	void PetPackageCapacity(int iSize);
	void ItemAppear(CSimpleGood* pItem);
	void ItemDisappear(DWORD id);
	void ItemPickup(int x,int y);
	void PetItemPickupFail(DWORD id,WORD wErr);
	void Clear();

	void BatFlyFinish(int x,int y); //化身蝙蝠抢成功
	bool FilterRecv(const char* str);   //判断消息框内容是否是指定字符串
	void AddObject(CGood* pGood);
	void RemoveObject(DWORD id);
	void MoveFailed(int x,int y);   //移动失败
	void MoveSuccess();             //移动成功
	void AutoMoveFinish();          //自动移动完成
	void AutoMoveFaild();          //自动移动失败
	void DoLoop();                  //每次刷新周期内不停的更新
	void EnableAutoGoPick(bool b = true);
	//不然在从一步以内直接发送消息到自动走路的时候
	//没有计算前面一个外挂发送的包，所以就会导致不断的失败
	void SetLimitRange(int iLimit = 3) {   m_iLimitRange = iLimit;}
	void SetPetGood(DWORD id,int iItemX,int iItemY);
	void GotoEat(CSimpleGood* pItem);
	bool NeedPickUpGood();

	ListItem& GetListItem(){ return m_listItem; }
private:
	CSimpleGood* GetBestItem();     //获得最近的极品
	CSimpleGood* GetNearestItem();  //得到要提醒显示的物品
	bool IsNeedFly(int iSelfX,int iSelfY,int iX,int iY);
	void AutoPickup();              //自动捡物的逻辑判断
	void GoAndPickup();             //走到某个地方去捡
	void SEND_Object_Pickup(int x,int y);
	void DisplayItem(CSimpleGood* pItem);
	bool IsInOneStep(int iSelfX,int iSelfY,int iItemX,int iItemY); //是否在一步以内
	bool IsHoldByOther(int iItemX,int iItemY); //此点是否被其他人占了
	bool EatForBestItem(int iItemX = -1,int iItemY = -1);

	CSimpleGood* GetPetItem();      //得到宠物现在该捡的物品
	void PetPickup();               //让宠物捡东西
	int   m_iLimitRange;

	CSimpleGood* m_pPickItem;
	bool  m_bAutoStep;       //直接一步走
	bool  m_bAutoFly;        //是否蝙蝠飞
	bool  m_bOverWeight;     //超重
	DWORD m_dwNotMyGoodID;   //物品不属于我的物品ID
	DWORD m_dwLastNotifyTime; //最近一次通知的时间

	char  m_cDir;            //目的方向
	bool  m_bRun;            //是否跑步？

	ListItem m_listItem; //要捡的物品

	//宠物捡物
	DWORD m_dwPetGoodID;     //宠物正捡的物品
	int   m_iPetPickX,m_iPetPickY;//宠物捡物坐标
	ListItem m_listPet;  //宠物需要捡的物品
	int m_iAutoX,m_iAutoY;
	DWORD m_dwLastMoveTime;
	DWORD m_dwLastPickItem;
	DWORD m_dwLastWalkPickItemID;
	DWORD m_dwLastWalkPickTime;
};

extern CAIAutoPickMgr g_AutoPickMgr;
