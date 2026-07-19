#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/GameDefine.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/ItemCfgMgr.h"

class CTodayActivityIEWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTodayActivityIEWnd)
public:
	CTodayActivityIEWnd(void);
	~CTodayActivityIEWnd(void);
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
