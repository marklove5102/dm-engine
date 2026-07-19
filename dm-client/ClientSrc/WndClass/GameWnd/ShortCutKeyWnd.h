#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"

class CShortCutKeyWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CShortCutKeyWnd)
public:
	CShortCutKeyWnd(void);
	~CShortCutKeyWnd(void);
	void OnCreate();
	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	void Draw();
	bool OnMouseMove(int iX,int iY);
	bool OnLeftButtonDown(int iX, int iY);
private:
	int GetGrid(int iX,int iY);

	CCtrlButton * m_pSwitchBtn;
	bool m_bShowEx;
};
