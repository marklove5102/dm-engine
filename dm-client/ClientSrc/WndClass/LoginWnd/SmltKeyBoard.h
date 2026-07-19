#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"

class CSmltKeyBoard : public CCtrlWindowX
{
	DECLARE_WND_POSX(CSmltKeyBoard)

public:
	CSmltKeyBoard(void);
	~CSmltKeyBoard(void);

	void OnCreate();
	virtual void OnDraw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

protected:
	bool m_bCapsLock;
	bool m_bShift;
	char m_cCurChar[2];

	CCtrlButton * m_pEnter;
	CCtrlButton * m_pBack;
	CCtrlButton * m_pCapsLock;
	CCtrlButton * m_pShift;

	CCtrlButton * m_pUnderLine;
	CCtrlButton * m_pDot;

	CCtrlButton * m_pNumber[10];
	CCtrlButton * m_pLetter[26];

	void ChangeCaps();
};
