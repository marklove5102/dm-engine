#pragma once

#include "BaseClass\Control\CtrlWindowX.h"
#include "BaseClass\Control\CtrlButton.h"

class CWoolStorCleanWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CWoolStorCleanWnd)
public:
	virtual void OnDraw(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

public:
	CWoolStorCleanWnd();
	~CWoolStorCleanWnd(void);

private:

	std::string          m_strInform;//提示消息内容
	std::string			 m_strItemName;//接收物品的名称
	CCtrlButton * m_pOK;
 
};



