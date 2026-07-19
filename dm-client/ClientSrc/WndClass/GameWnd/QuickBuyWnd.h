#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"

class CQuickBuyWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CQuickBuyWnd)

public:
	CQuickBuyWnd(void);
	~CQuickBuyWnd(void);

	virtual void Draw(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);

protected:
	CCtrlButton * m_pOK;
	CCtrlButton * m_pCancel;
	DWORD m_dwStartTime;
	DWORD m_dwDuraTime;

	int GetLeftGrid();
};
