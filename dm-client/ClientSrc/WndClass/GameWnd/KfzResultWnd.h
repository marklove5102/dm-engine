#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/TagText.h"
#include "BaseClass/Control/MarkViewer.h"


class CKfzResultWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CKfzResultWnd)

public:
	CKfzResultWnd(void);
	~CKfzResultWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

protected:
	CTagText m_TagText;
	CMarkViewer *m_pMarkViewer;
	CCtrlButton * m_pCancel;

	int m_iResult;
	DWORD m_dwDrawTimes;
};