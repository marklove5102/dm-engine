#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/Good.h"


class CSplitStoneWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CSplitStoneWnd)

public:
	CSplitStoneWnd(void);
	~CSplitStoneWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate(void);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual void OnClickCloseButton();
	virtual bool OnMouseMove(int iX,int iY);
	
	void SetSplitType(int i){m_iTypeForSplit = i;}

protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pCancel;
	CCtrlEdit*			 m_pEdit;
	CGood				 m_FuShiGood;
	int					 m_iTypeForSplit;//如果为1则为拆分符石，2为拆分圣灵精华
};
