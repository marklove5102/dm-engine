#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"

#include <string>


class CLeaveMessageWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CLeaveMessageWnd)

public:
	CLeaveMessageWnd(void);
	~CLeaveMessageWnd(void);

protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pCancelButton;
	CCtrlEdit*          m_pUserName;
	CCtrlEdit*          m_pAddress;
	CCtrlMultiEdit*      m_pLeavWords;
	bool   IsInvalidCharacter(std::string str);

public:
	//÷ÿ‘ÿ∏∏¿‡∫Ø ˝
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
};
