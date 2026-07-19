#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CDefend12CtrlWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CDefend12CtrlWnd)
public:
	CDefend12CtrlWnd();
	~CDefend12CtrlWnd();

public:
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual bool OnLeftButtonDClick(int iX,int iY);	
	virtual bool OnMouseMove(int iX,int iY);
	virtual void OnClickCloseButton();

protected:
	CCtrlButton* m_pGenSuiButton;
	CCtrlButton* m_pGenSuiAttackButton;
	CCtrlButton* m_pZiYouButton;
	CCtrlButton* m_pDaiJiButton;
};