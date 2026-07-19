#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"


class CCommonSelectWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CCommonSelectWnd);
public:
	CCommonSelectWnd(void);
	~CCommonSelectWnd(void);

public:
	virtual void Draw(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl * pControl);

protected:
	DWORD m_dwType;//用来标识类型1是圣灵精华2是圣灵宝石
	DWORD m_dwTemp;//传过来的参数数据

	//各类窗口
	//圣灵精华升级装备
	CCtrlButton* m_pArmUpdate;
	CCtrlButton* m_pSheildUpdate;
	std::string  m_strWndInfo;
};