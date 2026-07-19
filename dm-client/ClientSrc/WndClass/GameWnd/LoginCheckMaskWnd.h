#pragma once
#include "BaseClass/Control/CtrlEdit.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"

class CLoginCheckMaskWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CLoginCheckMaskWnd)

public:
	CLoginCheckMaskWnd(void);
	~CLoginCheckMaskWnd(void);

public:
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	//virtual void OnClickCloseButton();
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual void OnMove();

protected:
	CCtrlButton* m_pSubmit;
	CCtrlButton* m_pNextMask;
	CCtrlEdit*	 m_pCheckMask;
};
