#pragma once

#include "BaseClass/Control/CtrlWindowX.h"


class CGuildAllyWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CGuildAllyWnd)

public:
	CGuildAllyWnd(void);
	~CGuildAllyWnd(void);
public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual void Draw();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
protected:
	
	CCtrlButton*	m_pOKButton;
	CCtrlButton*	m_pCancelButton;
	CCtrlRadio*		m_pPermit;
};
