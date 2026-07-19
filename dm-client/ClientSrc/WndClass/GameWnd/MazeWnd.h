#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
class CMazeWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CMazeWnd)
public:
	CMazeWnd(void);
	~CMazeWnd(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
private:
	CCtrlButton * m_pExitButton;
};
