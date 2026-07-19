#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/GoodGrid.h"

class CQiShouWnd:
	public CCtrlWindowX
{
	DECLARE_WND_POSX(CQiShouWnd)
public:
	CQiShouWnd(void);
	~CQiShouWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnMouseMove(int iX,int iY);
	//virtual bool OnLeftButtonUp(int iX, int iY);
private:

	CGoodGrid *m_pGoodGrid;//°ü¹üÎïÆ·
	bool m_bLastState;
	bool m_bNeedFind;
	CGood m_FocusGood;
};
