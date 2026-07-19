#pragma once
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlWindowX.h"


class CAuthenWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CAuthenWnd)

public:
	CAuthenWnd(void);
	~CAuthenWnd(void);

	void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	virtual void OnClickCloseButton();
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual void OnMove();

protected:
	DWORD m_dwRealTime;
	bool m_bLoaded;

	//CCtrlButton * m_pSubmit;				// 提交按钮	
	//CCtrlButton * m_pCancel;				// 取消按钮	
};
