#pragma once
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"



class CBoothNameWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBoothNameWnd)

public:
	CBoothNameWnd();
	~CBoothNameWnd();
protected:
	CCtrlButton*  m_pOKButton;
	CCtrlButton*  m_pCancelButton;
	CCtrlEdit*  m_pNameEdit;
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool SetFocus();
};

//设置摊位价格窗口
class CBoothPriceWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBoothPriceWnd)

public:
	CBoothPriceWnd();
	~CBoothPriceWnd();
protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pCancelButton;
	CCtrlEdit*          m_pCountEdit;
	void SwitchPage(int i);
	int m_iCurPage;
	DWORD       m_dwMsg;
public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();
	virtual bool SetFocus();

};