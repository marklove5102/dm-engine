#pragma once

#include "BaseClass\Control\CtrlWindowX.h"
#include "BaseClass\Control\CtrlButton.h"

class CWoolStorSendOKWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CWoolStorSendOKWnd)
public:
	CWoolStorSendOKWnd(void);
	~CWoolStorSendOKWnd(void);

	virtual void OnDraw(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

protected:

	std::string			 m_strSend;//发送消息包
	std::string          m_strPrice;       //物品价格
	std::string          m_strTerminalUser;//接收人
	CCtrlButton * m_pOK;
	CCtrlButton * m_pCancel;
};
