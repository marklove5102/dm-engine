#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

class CRegisterInfoWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CRegisterInfoWnd)
public:
	CRegisterInfoWnd(void);
	~CRegisterInfoWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate(void);
	virtual void OnClickCloseButton();

private:

	CCtrlEdit*          m_pRealName;
	CCtrlEdit*          m_pIdentityCard;

	CCtrlButton* m_pSubmitButton;
	CCtrlButton* m_pCancelButton;

	string         m_strMessage;

};
