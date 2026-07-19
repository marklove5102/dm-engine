#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CEnterShanWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CEnterShanWnd)

public:
	CEnterShanWnd(void);
	~CEnterShanWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);

protected:
	CCtrlButton* m_pEnter;
};