#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"


#define MAX_TRADE_ELEMENT			10

class CTradeWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTradeWnd)

public:
	CTradeWnd(void);
	~CTradeWnd(void);
public:
	bool	IsInMyMoneyGrid(int ix,int iy);
	bool	IsInMyYuanBaoGrid(int ix,int iy);
	void	GetMyGrid(int* iX, int* iY, int ix, int iy);	//取得点所在的表格
	void	GetPeerGrid(int* iX, int* iY, int ix, int iy);	//取得点所在的表格
	bool	IsInMyGrid(int ix, int iy);						//判断点是否在表格范围内
	bool    IsInPeerGrid(int ix, int iy);

	//重载父类函数
public:

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);

protected:
	int m_iCellW,	 m_iCellH;
	int m_iCellsW,   m_iCellsH;
	int m_iCellDisW, m_iCellDisH;
	string m_strRealtion;//交易者和我的关系

	CCtrlButton * m_pCancelButton;
	CCtrlButton * m_pTradeButton;
	POINT m_ptMyHeadPoint, m_ptPeerHeadPoint;
	bool m_bTrade;

};
