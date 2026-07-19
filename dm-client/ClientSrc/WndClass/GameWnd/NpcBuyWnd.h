#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"

class CNpcBuyWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcBuyWnd)

public:
	CNpcBuyWnd();
	~CNpcBuyWnd(void);

	void SetWndType(int type);
	int GetWndType(){ return m_iBuy; }

public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnWheel(int iWheel);
	virtual void OnClickCloseButton();

protected:
	//CCtrlButton * m_pUpButton;
	//CCtrlButton * m_pDownButton;
	CCtrlButton * m_pOkButton;
	CCtrlButton * m_pCancelButton;


	CCtrlScroll*     m_pScroll;
	int       m_iDisLine;//显示的第一行的行号（下标）
	int       m_iCount;
	int       m_iClickRow;

	int       m_iBuy;    //1买，2，仓库取回物品窗口 3毒药窗口	4/二级购买窗口

	std::string  m_strGoodName;
	DWORD m_dwNpcID;

};