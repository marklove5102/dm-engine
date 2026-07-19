#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CMeritoriousWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CMeritoriousWnd)
public:
	CMeritoriousWnd(void);
	~CMeritoriousWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();

private:
	CCtrlButton * m_pMaxBtn;				//功勋值界面的三个按钮:最大,领取,关闭
	CCtrlButton * m_pTakeBtn;
	CCtrlButton * m_pCloseBtn;
	CCtrlEdit   * m_pExpEdit;					//功勋值界面领取经验输入框
	
};
