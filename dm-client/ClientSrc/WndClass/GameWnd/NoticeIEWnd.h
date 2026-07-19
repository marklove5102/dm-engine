#pragma once
#include "BaseClass/Control/CtrlWindowX.h"


class CNoticeIEWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNoticeIEWnd)
public:
	CNoticeIEWnd(void);
	~CNoticeIEWnd(void);
public:
	virtual void OnCreate();
	virtual void Draw(void);
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual void OnMove();
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);	


protected:
	DWORD m_dwStartLoadPageTime;
};
