#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"

class CNpcBoardWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcBoardWnd)

public:
	CNpcBoardWnd(void);
	~CNpcBoardWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnWheel(int iWheel);

private:
	CMarkViewer *m_pMarkViewerLeft;
	CMarkViewer *m_pMarkViewerRight;
	CCtrlScroll*       m_pScroll;
	static const int MAX_ROWS;

};
