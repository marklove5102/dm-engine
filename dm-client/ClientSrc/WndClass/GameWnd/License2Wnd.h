#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlLabel.h"

class CLicense2Wnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CLicense2Wnd)

public:
	CLicense2Wnd(void);
	~CLicense2Wnd(void);

	virtual void OnCreate();
	virtual void AfterDraw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	virtual bool OnLeftButtonUp(int iX, int iY);

protected:
	CCtrlButton*  m_pOk;
	CCtrlRadio* m_pBtn,*m_pClassic,*m_pFashional;
	bool m_bHaveSetUiType;
	CMarkViewer* m_pMarkViewer;
	CTagText m_TagText;

};