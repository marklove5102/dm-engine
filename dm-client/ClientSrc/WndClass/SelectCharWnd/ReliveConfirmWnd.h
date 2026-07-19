#pragma once

#include "Global/Global.h"
#include "BaseClass/Control/CtrlWindowS.h"
#include "BaseClass/Control/CtrlButton.h"


class CReliveConfirmWnd: public CCtrlWindowS
{
	DECLARE_WND_POS(CReliveConfirmWnd)
public:
	CReliveConfirmWnd(void);
	~CReliveConfirmWnd(void);

	virtual void OnCreate(void);
	virtual void OnDraw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

private:
	CCtrlButton * m_pOKButton;
	CCtrlButton * m_pCancelButton;

	void OnClickOKButton(void);
	void OnClickCancelButton(void);

};
