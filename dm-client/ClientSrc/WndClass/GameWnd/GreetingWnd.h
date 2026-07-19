#pragma once

#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlWindowX.h"


class CGreetingWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CGreetingWnd)

public:
	CGreetingWnd(void);
	~CGreetingWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);	
	virtual bool OnWheel(int iWheel);					// 滚轮消息
protected:
	CMarkViewer* m_pMarkViewer;
	CCtrlButton*  m_pOk; //确定按钮
	CCtrlRadio*	  m_pNewHandPrompt;
};
