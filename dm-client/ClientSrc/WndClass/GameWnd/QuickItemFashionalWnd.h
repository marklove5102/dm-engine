#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMenuButton.h"


class CQuickItemFashionalWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CQuickItemFashionalWnd)

public:
	CQuickItemFashionalWnd(void);
	~CQuickItemFashionalWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();	
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual bool OnMouseMove(int iX, int iY);

protected:
	bool IsInStoreGrid(int ix,int iy,int &iIndex);

	static const POINT m_ptPos[];

};
