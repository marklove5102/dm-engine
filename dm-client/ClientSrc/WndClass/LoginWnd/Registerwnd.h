#pragma once
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlWindowX.h"


class CRegisterWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CRegisterWnd)

public:
	CRegisterWnd(void);
	~CRegisterWnd(void);

	void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	virtual void OnClickCloseButton();
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual void OnMove();

protected:
	DWORD m_dwRealStorLoginTime;
	bool m_bLoaded;

	void GetVersion(char* szVersion);
};
