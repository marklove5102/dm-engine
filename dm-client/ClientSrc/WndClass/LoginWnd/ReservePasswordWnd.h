#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

class CReservePasswordWnd :	public CCtrlWindowX
{
	DECLARE_WND_POSX(CReservePasswordWnd)

public:
	CReservePasswordWnd(void);
	~CReservePasswordWnd(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl * pControl);
	virtual void OnClickCloseButton(void);

private:

	int					m_iType;
	CCtrlEdit *			m_pReservePassword;
	CCtrlButton * m_pOK;				// 确定
	CCtrlButton * m_pCancel;			// 取消
};