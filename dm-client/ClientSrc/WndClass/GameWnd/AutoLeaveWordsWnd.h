#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"


class CAutoLeaveWordsWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CAutoLeaveWordsWnd)

public:
	CAutoLeaveWordsWnd(void);
	~CAutoLeaveWordsWnd(void);

	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);	
	void OnCreate(void);

protected:
	CCtrlButton * m_pOk;
	CCtrlButton * m_pCancel;
	//CCtrlRadio * m_pAutoLeaveWords;
};
