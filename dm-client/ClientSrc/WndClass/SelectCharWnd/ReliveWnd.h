#pragma once

#include "Global/Global.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"

class CReliveWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CReliveWnd)
public:
	CReliveWnd(void);
	~CReliveWnd(void);

	virtual void OnCreate(void);
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);		// KEYDOWN 

	void ReliveUseYanBao(bool bUse);
protected:

	CCtrlButton * m_pRelive;  //原地复活按钮
	CCtrlButton * m_pClose;  //关闭按钮
	bool m_bClickedReliveBtn;//是否按下复活按钮
	int  m_iLastSecond;

	virtual void OnReliveImm();
};
