
#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlMenuButton.h"

#define  VIPSTOREWND_MAXITEM_PER_PAGE_SELL 2
#define  VIPSTOREWND_MAXITEM_PER_PAGE_GIFT 1

class CVipStoreWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVipStoreWnd)

public:
	CVipStoreWnd(void);
public:
	~CVipStoreWnd(void);

public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnWheel(int iWheel);	
	virtual void OnDraw();

protected:
	CCtrlMenuButton* m_pGetGiftMBtn;
	CCtrlButton*    m_pBuyBtn[VIPSTOREWND_MAXITEM_PER_PAGE_SELL],*m_pGetGiftBtn;
	CMarkViewer* m_pMarkViewer;
	//CCtrlButton * m_pUpButton;//向上按钮
	//CCtrlButton * m_pDownButton;//向下按钮
	CCtrlScroll * m_pScroll; //滚动条


	string m_strHeader;
	CTagText m_ServiceTagText;
	POINT m_ptBuy[VIPSTOREWND_MAXITEM_PER_PAGE_SELL];//各出售物品绘制位置
	DWORD m_dwScrollTipTime;
	VString m_vScrollText;
	BYTE m_byFirstVipType;//开启窗口时的vip类型,开启窗口后再升级为vip信用会有误,所以从非vip变成vip时要请求数据
	bool m_bHaveGetGift;//是否领取物品

	void InitData();
	void ReflashBtnState();

};
