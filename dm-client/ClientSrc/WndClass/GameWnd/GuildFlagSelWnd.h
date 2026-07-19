#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"

class CGuildFlagSelWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CGuildFlagSelWnd)
public:
	CGuildFlagSelWnd(void);
	~CGuildFlagSelWnd(void);
	void OnCreate();
	void Draw();
	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
private:
	CCtrlButton* m_pOKBtn;
	CCtrlButton* m_pCancelBtn;		

	CCtrlRadio * m_pFlag1;
	CCtrlRadio * m_pFlag2;
	CCtrlRadio * m_pFlag3;
	CCtrlRadio * m_pFlag4;	

	CCtrlButton * m_pGoLeftButton;
	CCtrlButton * m_pGoRightButton;

	DWORD m_dwPos;
};
