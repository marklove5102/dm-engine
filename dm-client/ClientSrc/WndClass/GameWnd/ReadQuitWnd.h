///////////////////////////////////////////////////////////////////////
//文件名：   .h
//版权：上海盛大网络有限公司版权所有
//作者：
//创建日期：
//版本：
//文件说明：
//    用户信息收集窗口
//
//
//
//
//
///////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseClass/Control/CtrlWindowX.h"


class CReadQuitWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CReadQuitWnd)

public:
	CReadQuitWnd();
	~CReadQuitWnd(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl* pControl);

protected:
	DWORD   m_dwStartTime; 
};
