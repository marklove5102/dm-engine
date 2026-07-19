#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

//快捷消费提醒框
class CBaoJianWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBaoJianWnd)

public:
	CBaoJianWnd();
	~CBaoJianWnd(void);

	virtual void Draw(void);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();

	bool DrawShuffling();

private:
	int m_iFirstPos;
	int m_iSecondPos;
	int m_iThirdPos;
	int m_iAdditionPos;

	POINT m_ptGrid[12];

	DWORD m_dwStartTime;

	DWORD m_dwShowAllPrizeStart;	

	int m_PrizeIndex[12];	//洗牌后相应位置的牌在洗牌前的位置
	bool m_bShowPrize[12];	//是否显示格子里的奖品
	bool m_bRealPrize[12];	//格子里的奖品有没有被获得

	DWORD m_dwShuffleRand;

	int m_iOpenedGrid;

	DWORD m_dwLastOpenGridTime;
};