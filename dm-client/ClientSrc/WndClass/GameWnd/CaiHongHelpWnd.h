#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

// ²Êºç°ïÖú
class CCaiHongHelpWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CCaiHongHelpWnd)
public:
	CCaiHongHelpWnd(void);
	~CCaiHongHelpWnd(void);
public:
	virtual void OnCreate();
	virtual void Draw(void);
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual void OnMove();
	virtual void OnClickCloseButton();

protected:
	DWORD m_dwStartLoadPageTime;
};
