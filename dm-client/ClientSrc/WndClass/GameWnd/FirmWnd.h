#pragma once
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "GameData/Good.h"
#include "GameData/NpcData.h"

#define REMOVEITEMNUMBER 9 //取消页可显示几条

class CFirmWnd :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(CFirmWnd)
public:
	CFirmWnd(void);
	~CFirmWnd(void);

	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual void SwitchToPage(int iPage,S_TabPage * pParentTabPage = NULL,bool bReCreate = false);//bReCreate为true时就算要切换的页就是当前页也重新创建

	virtual bool OnWheel(int iWheel);	

	int GetIndex(int iX,int iY,int &iRow,int &iCol);//根据坐标获得index

private:
	//收购页
	CCtrlGrid *   m_pItemGrid;					//列表控件
	CCtrlMenuButton*	m_pMBKindOfItem;		//收购种类的下拉菜单
	CCtrlButton*  m_pGoLastPage;				//向前翻页
	CCtrlButton*  m_pGoNextPage;				//向后翻页
	//详细收购信息页
	CCtrlButton*  m_pConfirmSell;				//确认出售
	CCtrlButton*  m_pCancelSell;				//取消出售
	CCtrlButton*  m_pReturnButton;				//返回按钮
	CGood		  m_pGoods;						//要出售的物品
	POINT		  m_GoodsPos;					//物品的显示位置

	//发布收购信息
	CCtrlEdit*	  m_pENumPerUnit;				//最小收购数量
	CCtrlEdit*	  m_pEUnitPrice;				//最小收购数量的价格
	CCtrlMenuButton*	m_pMBTradeType;			//价格单位,金币/元宝
	CCtrlEdit*	  m_pEUnitNum;					//收购份数
	CCtrlButton*  m_pConfirmSend;				//确认发布按钮	

	//取消收购信息
	CCtrlButton*  m_pRemoveItem[REMOVEITEMNUMBER];		//取消收购按钮,若干
	CCtrlButton*  m_pDetailItem[REMOVEITEMNUMBER];		//详细按钮
	//翻页,复用上面的翻页按钮指针
	
	//商铺,寄卖/所得/
	CCtrlButton*  m_pTakeSellGoods;		//领取寄卖物品
	CCtrlButton*  m_pTakeSellMoney;		//领取寄卖所得
	CCtrlButton*  m_pTakeBuyGoods;		//领取收购到的物品
	CCtrlButton*  m_pTakeBuyMoney;		//领取收购预付金额

	CCtrlButton*  m_pTradeBuyBtn;		//交易行购买按钮
	CCtrlScroll * m_pScroll; //滚动条
	CCtrlRadio*   m_pOnlyShowMySelf;//只显示自己的


	string m_message;



	int m_iStartX,m_iStartY;//物品格子的起始位置
	int m_iCellW,m_iCellH;//格子的宽度和高度
	int m_iIntervalW,m_iIntervalH;//格子的Ｘ，Ｙ间隔
	int m_iRows,m_iCols;//X,Y方向的格子数
	int m_iSelectiRow,m_iSelectCol;//选中的是那个一位置的物品

	DWORD m_dwLastRequireTime;

	SVipTradGood *m_pVTradeGood;
	SVipTradGood mSelfVTradeGood;

	void UpdateScrollPos();
	int  GetVipLevelByExp(DWORD dwExp);

};
