#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"


class CBindPTMsgWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBindPTMsgWnd)

public:
	CBindPTMsgWnd(void);
	~CBindPTMsgWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

protected:
	CCtrlButton* m_pQutiButton;
	CCtrlButton* m_pSaveButton;
	CCtrlButton* m_pCancelButton;

	CMarkViewer*    m_pMarkViewer;
	CTagText        m_tagText;
};

