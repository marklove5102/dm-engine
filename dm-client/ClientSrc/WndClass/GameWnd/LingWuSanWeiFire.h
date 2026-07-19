#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

class CLingWuSanWeiFire:
	public CCtrlWindowX
{
	DECLARE_WND_POSX(CLingWuSanWeiFire)
public:
	CLingWuSanWeiFire(void);
	~CLingWuSanWeiFire(void);


	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

	CCtrlButton*  m_pLingWu;				//Á¶»¯
};
