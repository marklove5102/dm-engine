#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CLuckQyzWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CLuckQyzWnd)

public:
	CLuckQyzWnd(void);
	~CLuckQyzWnd(void);

	void TurnCard(DWORD nCard, DWORD nExp);//翻转卡片

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();
	virtual bool OnMouseMove(int iX, int iY);
	virtual void Draw();

protected:
	CCtrlButton* m_pOK;
	RECT m_rcCardArea;
	bool m_bOpened[30];
	bool m_bClicked[30];
	DWORD m_dwCompleteTime;
	int m_nExps[30];
	int m_iLastCards;
	int m_iTurnedCards;
	bool m_bLuckQYZDoubleChance;//今天是否有两次机会

	DWORD m_dwOpenTimes[30];
	DWORD m_dwLastOpenTime;
	
	bool m_bEffectBox[30];
	DWORD m_dwEffectMoveTime[30];

	int m_nCardWidth;
	int m_nCardHeight;

	DWORD m_dwPushSysTalkTime[3];
};
