#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/Good.h"

#define MAXNUMBERSTORAGE (40+40+40)	//三页

class CNpcQuickWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcQuickWnd)

public:
	CNpcQuickWnd(void);
	~CNpcQuickWnd(void);

public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnWheel(int iWheel);	

	int GetIndex(int iX,int iY/*,int *pMouseOnIdx = NULL*/);//根据坐标获得index
	void BuyGoods();//购买选中物品
	void UpdateScrollPos();//刷新滚动条的位置
	void OnClickBodyRepair();//点了全身修理按钮
	void ClearDropGood();//清除鼠标上的物品
	void CarryBackGoods();//取回仓库里的物品

	int GetType(){ return m_iType; }
	static DWORD* GetGoodPos(){ return s_vGoodPosition;}

protected:
	CCtrlButton * m_pOkButton;//购买
	CCtrlButton * m_pCancelButton;//取消
	CCtrlButton * m_pCommonRepair;//普通修理
	CCtrlButton * m_pSpecialRepair;//特殊修理
	CCtrlButton * m_pBodyRepair;//用冰泉圣水全身修理
	CCtrlButton * m_pUpButton; //上一页
	CCtrlButton * m_pDownButton; //下一页
	CCtrlScroll * m_pScroll; //滚动条


	int m_iType;//类型：1表示快捷购买，2表示快捷仓库
	int m_iStartX,m_iStartY;//物品格子的起始位置
	int m_iCellW,m_iCellH;//格子的宽度和高度
	int m_iIntervalW,m_iIntervalH;//格子的Ｘ，Ｙ间隔
	int m_iRows,m_iCols;//X,Y方向的格子数
	int m_iSelectIndex;//选中的是那个一编号的物品

	int IsMouseOnTabPage(int x,int y);
	static DWORD s_vGoodPosition[MAXNUMBERSTORAGE];

	bool FindGoodById(DWORD goodid, CGood*& result);
};
