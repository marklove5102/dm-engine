#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"

#include <string>

class CInputYelloFlagWnd :	public CCtrlWindowX
{
	DECLARE_WND_POSX(CInputYelloFlagWnd)

public:
	CInputYelloFlagWnd(void);
	~CInputYelloFlagWnd(void);

public:
	//÷ÿ‘ÿ∏∏¿‡∫Ø ˝
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCancelButton();
	virtual void OnClickOKButton();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pCancelButton;
	CCtrlEdit*          m_pTitle;
	CCtrlMultiEdit*     m_pInfo;

	std::string m_strTitle;
	std::string m_strInfo;
	int m_iFlagX,m_iFlagY;
	DWORD m_id;
};
