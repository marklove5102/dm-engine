#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"

class CNpcSaleWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcSaleWnd)

public:
	CNpcSaleWnd(void);
public:
	~CNpcSaleWnd(void);
public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
protected:
	CCtrlButton * m_pOkButton;
	CCtrlButton * m_pCancelButton;
	int m_iSale;			//1:卖，2:修理	3表示保管物品窗口4表示锻造物品窗口7表示鉴定物品窗口5祝福6表示弹出托盘8不关闭的弹出托盘
	bool m_bBackToPackage;
	DWORD m_dwLastClickOkBtn;
	BYTE m_UseBind;

};
