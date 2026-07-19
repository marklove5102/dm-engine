#pragma once

#include "BaseClass\Control\CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "BaseClass/Control/CtrlRadio.h"

class CWoolStorBuyCount	:public CCtrlWindowX
{
	DECLARE_WND_POSX(CWoolStorBuyCount)
public:
	CWoolStorBuyCount(void);
	~CWoolStorBuyCount(void);
	virtual void OnDraw(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	int GetLeftGrid();

protected:
	std::string          m_strConfirm;
	std::string          m_strMoneyCount;
	CCtrlButton *	     m_pOK;
	CCtrlButton *        m_pCancel;
	CCtrlMenuButton *    m_pCountSelect;
	//CCtrlRadio*          m_pRDBuyUseBindYuanBoa;
	bool				 m_bBind;
};
