#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/MarkViewer.h"

class CActivityLogWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CActivityLogWnd)
public:
	CActivityLogWnd(void);
	~CActivityLogWnd(void);
public:
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual bool OnWheel(int iWheel);
protected:
	int GetWeekDay();
protected:
	CMarkViewer *m_pMVContent,*m_pMVPrize,*m_pMVTheme,*m_pMVNPC;
	CCtrlGrid *   m_pGrid;//ап╠М©ь╪Ч
	CTagText	  m_TagContent,m_TagPrize,m_TagTheme,m_TagNPC;
	WORD		  m_wActivityID;
	CCtrlRadio *  m_pAutoShow;
	CCtrlButton * m_pGetPrize1,*m_pGetPrize2;
	CCtrlButton * m_pPrize1,*m_pPrize2,*m_pPrize3,*m_pPrize4;
};
