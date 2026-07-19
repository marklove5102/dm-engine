#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "BaseClass/Control/CtrlButton.h"

class CChangePasswordWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CChangePasswordWnd)

public:
	CChangePasswordWnd();
	~CChangePasswordWnd();

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
protected:
	CCtrlEdit * m_pUserID;
	CCtrlEdit * m_pOldPassword;
	CCtrlEdit * m_pNewPassword1;
	CCtrlEdit * m_pNewPassword2;

	CCtrlButton * m_pSubmit;
	CCtrlButton * m_pCancel;
	CCtrlButton * m_pClose;

	DWORD		m_dwState;

	bool OnSubmit();
	bool OnCancel();
	bool HasSpecChar(const char *pText);
};