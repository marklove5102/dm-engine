#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/NpcData.h"


class CGoodSubmitWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CGoodSubmitWnd)

public:
	CGoodSubmitWnd(void);
	~CGoodSubmitWnd(void);
public:
	//÷ÿ‘ÿ∏∏¿‡∫Ø ˝
	virtual void OnDraw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	bool    InSubmitPos(int iX, int iY);
	void    BackToPackage();

private:
	CCtrlButton * m_pOK;
	CCtrlButton * m_pCancel;
	DWORD  m_GoodID;
};
