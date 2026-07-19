#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"


class CNpcBookWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcBookWnd)

public:
	CNpcBookWnd();
	~CNpcBookWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnWheel(int iWheel);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

	void    UpdateScroll();
	void    SwitchToPage(int iPage);
protected:
	CMarkViewer* m_pMarkViewer;
	CCtrlScroll* m_pScroll;

	int     m_iCurPage;
	bool	m_bNewAnswer;
};
