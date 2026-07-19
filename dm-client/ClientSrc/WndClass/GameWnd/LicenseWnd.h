#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/MarkViewer.h"

class CLicenseWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CLicenseWnd)

public:
	CLicenseWnd(void);
	~CLicenseWnd(void);

	virtual void OnCreate();
	virtual void OnDraw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

protected:
	CCtrlButton* m_pOk;
	CCtrlButton*  m_pCancel;
	CMarkViewer* m_pMarkViewer;
	//CMarkViewer* m_pMVInfo;
	CCtrlMultiEdit* m_pInfo;

	CTagText m_TagText;
	CTagText m_TagContent;
};
