#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/GameDefine.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/ItemCfgMgr.h"

class CTodayActivityLocalWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTodayActivityLocalWnd)
public:
	CTodayActivityLocalWnd(void);
	~CTodayActivityLocalWnd(void);
public:
	virtual void OnCreate();
	virtual void Draw(void);
	virtual void OnSetFocus();							// 系统将获得焦点时此函数被调用
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);	

protected:
	_ActivityInfo** m_pAct;
	CMarkViewer*	m_pMarkTheme;
	CTagText		m_Tag;
};
