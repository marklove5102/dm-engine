#pragma once
#include "Control.h"

#define  ARROW_LAST_TIME 5000  //泡泡持续时间

class CArrowTip : public CCtrlWindow
{
public:
	CArrowTip(void);
	~CArrowTip(void);
	virtual bool Create(CControl * pParent,int iPointX,int iPointY,int iAlignType = XAL_TOPLEFT,int iArrowDir = XAL_TOPRIGHT,int iTipType = 0,DWORD dwData = 0,DWORD dwLastTime = 0,bool bNeedMsg = true);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	virtual void OnMove();
	virtual bool IsInControl(int iX,int iY); //是否在控件内
	virtual int  GetTipID() { return m_iTipID; }
	virtual void SetTipID(int val);

private:
	POINT m_ptPoint;//指向的父窗口的点
	bool  m_bLBClickOnCloseBtn;//左键是否点下关闭按钮
	int   m_iTipDir;//tip的方向
	int   m_iTipType;//tip类型,0:普通,1:指可以挖的怪物,要跟着怪物的位置而变化,消失而消失,挖好了也消失,2:指向物品,3:指向tab键
	DWORD m_dwData;//数据,类型为1时表示怪物id,为2时表示物品id
	DWORD m_dwLastTime;//持续时间
	bool  m_bNeedMsg;//是否要处理消息,true的话下面控件的消息就会被挡住,否则不阻挡下面控件的消息
	int   m_iTipID;


	void OnClickCloseBtn();
};
