#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/Good.h"

class CFirmDetailInfoWnd :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(CFirmDetailInfoWnd)
public:
	CFirmDetailInfoWnd(void);
	~CFirmDetailInfoWnd(void);
	
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnLeftButtonUp(int iX,int iY);

private:
	bool	IsInSellGoodsArea(int iX,int iY);
	void	warnMessage();
	
	CCtrlButton*  m_pConfirmSell;				//确认出售
	CCtrlButton*  m_pCancelSell;				//取消出售
	DWORD		  m_iRow;
	CGood		  m_pGoods;						//要出售的物品
	char		  m_warnMessage[512];		
	bool          m_bRightGood;//是否放入正确的物品
};
