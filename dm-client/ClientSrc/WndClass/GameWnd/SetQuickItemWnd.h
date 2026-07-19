#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMenuButton.h"


class CSetQuickItemWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CSetQuickItemWnd)

public:
	CSetQuickItemWnd(void);
	~CSetQuickItemWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
protected:
	CCtrlButton* m_pConfirm;
	CCtrlButton* m_pCancel;
	CCtrlMenuButton *    m_pCountSelect;

private:
	void   OnOK();
	void   OnCancel();
};
