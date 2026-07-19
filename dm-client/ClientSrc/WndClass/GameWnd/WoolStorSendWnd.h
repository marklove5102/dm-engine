#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/CtrlButton.h"

class CWoolStorSendWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CWoolStorSendWnd)
public:
	CWoolStorSendWnd(void);
	~CWoolStorSendWnd(void);

	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw();

protected:
	CCtrlButton * m_pOK;
	CCtrlButton * m_pCancel;
	CCtrlEdit  *         m_pEditRecvUser; 
	CCtrlMultiEdit*      m_pSendText;  
};
