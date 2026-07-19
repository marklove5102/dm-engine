#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"


class CAutoSayWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CAutoSayWnd)

public:
	CAutoSayWnd(void);
	~CAutoSayWnd(void);

public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl* pControl);
protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pStopButton;
	CCtrlMultiEdit*     m_pEdit;
};
