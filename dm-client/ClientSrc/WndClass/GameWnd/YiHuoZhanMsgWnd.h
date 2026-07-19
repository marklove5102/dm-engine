#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/TagText.h"
#include "BaseClass/Control/MarkViewer.h"


class CYiHuoZhanMsgWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CYiHuoZhanMsgWnd)

public:
	CYiHuoZhanMsgWnd(void);
	~CYiHuoZhanMsgWnd(void);

public:
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw();

protected:
	CCtrlButton* m_pSubmit;
	CCtrlButton* m_pClose;
	CMarkViewer* m_pMarkViewer;	
};
