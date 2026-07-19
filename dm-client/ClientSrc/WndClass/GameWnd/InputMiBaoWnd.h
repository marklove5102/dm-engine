#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

class CInputMiBaoWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CInputMiBaoWnd)

public:
	CInputMiBaoWnd(void);
	~CInputMiBaoWnd(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl* pControl);
protected:
	CCtrlButton*	m_pOKButton,*m_pChangePic;
	CCtrlEdit * m_pEdit;

	DWORD   m_dwStartTime;
	bool    m_bMinBao;
	string  m_strText;
	DWORD   m_dwLastClickFrashBtnTime;//上一次点刷新按钮的时间

	int m_iType;//0:普通密宝,1:usb密宝,2:准备输入验证码倒计时,3:开始输入验证码倒计时
	int m_byUsbEKeyType;
	int m_iMaxLen;//最多输入几位
	LPTexture m_pValidate;

	void OnClickOkButton();

};
