#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlScroll.h"

class CPrayTreeWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CPrayTreeWnd)
public:
	CPrayTreeWnd(void);
	~CPrayTreeWnd(void);
public:
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual void Draw();
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool SetFocus();

	void DrawForceSlot();
	void PlayEffect();
	bool HasPrayCharm();

protected:
	CCtrlButton * m_pThrowButton;
	CCtrlButton * m_pUpButton;
	CCtrlButton * m_pDownButton;
	CCtrlScroll * m_pScroll;

	int m_iCurrentLevel;	
	int m_iFrameCount;
	bool m_bThrowButtonClick;
	static bool sm_bSendForPrize;
	int m_iStartPos;
	bool m_bPlayEffect;
	int m_iEffectFrameCount;
	int m_iRandomNum;
	bool m_bStop;
	std::string m_strPrizeInfo;
};
