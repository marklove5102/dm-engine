#pragma once

#include "BaseClass/Control/CtrlWindowS.h"
#include "BaseClass/Control/CtrlButton.h"

class CQuitWnd : public CCtrlWindowS
{
	DECLARE_WND_POS(CQuitWnd)
public:
	CQuitWnd(void);
	~CQuitWnd(void);

	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickOKButton(void);
	virtual void OnClickCancelButton(void);
	virtual void OnCreate(void);
	virtual void OnClickCloseButton();
	virtual void Draw();//  [2/3/2010 dujun]

private:
	void OnCancel();
	void OnQuitGame();
	void OnQuitToBegin();
	void OnQuitToSafe();
private:

	CCtrlButton*  m_pQuitToSafe;
	CCtrlButton*  m_pQuitGame;
	CCtrlButton*  m_pQuitToBegin;
	CCtrlButton*  m_pCancel;
};
