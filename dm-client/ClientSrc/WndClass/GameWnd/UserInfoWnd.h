#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlRadio.h"
#include <string>


class CUserInfoWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CUserInfoWnd)

public:
	CUserInfoWnd(void);
	~CUserInfoWnd(void);

protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pCancelButton;
	CCtrlEdit*   m_pUserName;
	CCtrlEdit*   m_pTelephone;
	CCtrlEdit*   m_pPostCode;
	CCtrlEdit*   m_pAddress;
	CCtrlRadio*  m_pMale;
	CCtrlRadio*  m_pFemale;
	bool         m_bMale;

	bool   IsInvalidCharacter(std::string str);

public:
	//÷ÿ‘ÿ∏∏¿‡∫Ø ˝
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
};
