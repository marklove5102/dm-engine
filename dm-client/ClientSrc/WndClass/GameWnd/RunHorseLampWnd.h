#pragma once
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
class CRunHorseLampWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CRunHorseLampWnd)
public:
	CRunHorseLampWnd(void);
	~CRunHorseLampWnd(void);
public:
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);
protected:
	int m_iHorseDrawX;
};
