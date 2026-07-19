#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/GoodGrid.h"

class CPackageWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CPackageWnd)

public:
	CPackageWnd(void);
	~CPackageWnd(void);
    virtual void OnCreate();
    virtual void OnDraw(void); 

	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);

protected: 
	int m_iPackageType;//包裹类型，０为小包裹，１为大包裹
	int m_iWCells,m_iHCells;//包裹大小
    int m_iCellWidth,m_iCellHeight,m_iCellDisW,m_iCellDisH;//包裹格子大小及间隔
	int	m_iMouseOn;
	POINT m_ptGoodPoint;		    	    //物品栏左上角点坐标
	CGoodGrid *m_pGoodGrid;//包裹物品
	CCtrlButton * m_pLingXiBtn;//至尊宝盒按钮
	CCtrlButton * m_pCreditBtn;

	CCtrlButton * m_pEnlargeMoneybag;
	CCtrlButton * m_pMoreButton;
	CCtrlButton * m_pVIPButton;
	CCtrlButton * m_pGoldButton;
	CCtrlButton * m_pYuanBaoButton;
	CCtrlButton * m_pVIPTradeWndBtn;//vip交易行

	CCtrlButton * m_pWenPeiButton;

	CCtrlButton * m_pZhengLiButton;
// 	CGoodGrid *m_pTrusteeShipPackage1;		//托管物品栏左数第一个
// 	CGoodGrid *m_pTrusteeShipPackage2;		//托管物品栏左数第二个
// 	CGoodGrid *m_pTrusteeShipPackage3;		//托管物品栏左数第三个
// 	CGoodGrid *m_pTrusteeShipPackage4;		//托管物品栏左数第四个
// 	CCtrlRadio* m_pSendToMermberPackage1;   //放物品到队员包裹1
// 	CCtrlRadio* m_pSendToMermberPackage2;   //放物品到队员包裹2
// 	CCtrlRadio* m_pSendToMermberPackage3;   //放物品到队员包裹3
// 	CCtrlRadio* m_pSendToMermberPackage4;   //放物品到队员包裹4


	int	 GetIndex(int iX,int iY);
	bool IsInMoneyGrid(int ix,int iy);
	bool IsInYuanBaoGrid(int ix,int iy);
	void InCorporateMedal(int iType,DWORD dwData,DWORD drFromGoodID,int iClickPos);
	BOOL PopupArrowTip_Good(ePaoPaoPromptStatus eState,const char *szName);//物品冒泡
	BOOL PopupArrowTip_BaoJian();//群英宝鉴冒泡
};
