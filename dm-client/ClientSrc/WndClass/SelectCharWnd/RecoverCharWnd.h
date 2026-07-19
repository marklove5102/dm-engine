#pragma once

#include "Global/Global.h"
#include "BaseClass/Control/CtrlWindowS.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "BaseClass/Control/CtrlScroll.h"


class CRecoverCharWnd : public CCtrlWindowS
{
	DECLARE_WND_POS(CRecoverCharWnd)
public:
	CRecoverCharWnd(void);
	~CRecoverCharWnd(void);

	void OnDraw();
	bool OnLeftButtonDown(int iX,int iY);
	void OnCreate();
	void OnClickCloseButton();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnWheel(int iWheel);	

protected:
	CCtrlButton * m_pRecover;
	CCtrlButton * m_pCancel;
	CCtrlScroll * m_pScroll; //滚动条


	int m_nRecoverChar;

	int InWhichArea(int iX,int iY);//决定鼠标在那个区
	bool OnRecover();
};
