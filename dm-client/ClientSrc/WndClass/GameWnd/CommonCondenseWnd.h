#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/NpcData.h"



class CCommonCondenseWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CCommonCondenseWnd)

public:
	CCommonCondenseWnd();
	~CCommonCondenseWnd(void);
public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	void 		SetTipShow(bool b);
	int  		GetType() {return m_iType;}
	int         GetCommonGoodIndex(int iX,int iY);
	void        SetEnableUse(bool b);
	bool        GetEnableUse();

	void        BackToPackage();
private:	
	RECT    m_rcCommonGood[MAX_COMMON_GOOD_COUNT];
	CCtrlButton*  m_pStartButton;   //开始合成按钮
	int        m_iType;         //类型
	bool       m_bIsCondensing; //正在合成
	int        m_iMouseOnGood;   //鼠标在此上面的索引
	int        m_iMouseType;    //鼠标在此上面的类型
	bool       m_bEnableUse;   //窗口是否可以放置宝石
	bool       m_bIsStart;
};
