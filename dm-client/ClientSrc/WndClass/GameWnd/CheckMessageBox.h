#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

//快捷消费提醒框
class CCheckMessageBox : public CCtrlWindowX
{
	DECLARE_WND_POSX(CCheckMessageBox)

public:
	CCheckMessageBox();
	~CCheckMessageBox(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();

protected:
	int  m_iGoodIndex;
	CCtrlRadio*         m_pCheckOn;

	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pCancelButton;
};
