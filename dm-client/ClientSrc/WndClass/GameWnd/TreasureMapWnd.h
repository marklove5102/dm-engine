#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"

class CTreasureMapWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CTreasureMapWnd)	

public:
	CTreasureMapWnd(void);
	~CTreasureMapWnd(void);
	void OnCreate();
	void Draw();
	bool OnMouseMove(int iX,int iY);
	bool OnLeftButtonUp(int iX, int iY);
	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */);
private:
	CCtrlButton * m_pOKButton;

	int m_iFlashPart;
	DWORD m_dwGoodID;
	int m_iType;	//1“¯‘¬2÷À»’
};