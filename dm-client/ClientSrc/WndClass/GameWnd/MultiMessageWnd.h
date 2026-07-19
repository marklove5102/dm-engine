#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"


class CMultiMessageWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CMultiMessageWnd)

public:
	CMultiMessageWnd();
	~CMultiMessageWnd(void);

public:
	void SetMultiEditText(char * sText);

	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl * pControl);
	virtual void OnClickCancelButton(void);
	virtual void OnClickOKButton(void);
	virtual void OnCreate(void);
	virtual void Draw(void);

private:
	DWORD			m_dwMsg;
	DWORD			m_dwData;
	int				m_iMultiEditSize;

	CCtrlMultiEdit	*m_pEdit;
	CCtrlButton * m_pOKButton;
	CCtrlButton * m_pCancelButton;
};
