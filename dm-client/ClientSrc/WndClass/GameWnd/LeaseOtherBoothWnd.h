#pragma once
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/TalkViewer.h"
#include "GameData/Good.h"

class CLeaseOtherBoothWnd :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(CLeaseOtherBoothWnd)
public:
	CLeaseOtherBoothWnd(void);
	~CLeaseOtherBoothWnd(void);
public:
	//重载父类函数
	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnLeftButtonDClick(int iX,int iY);
protected:
	int		m_iCellW,m_iCellH;				//单元格的宽度和高度
	int		m_iWCells,m_iHCells;			//水平和垂直方向的格数
	int		m_iCellDisW,m_iCellDisH;		//单元格水平和垂直的间隔

	POINT	m_ptGoodPoint;		    	    //物品栏左上角点坐标
	bool	m_bDClicked;					//是否是双击左键
	
	CParserTip * m_pTip;                    //Tip
	CCtrlButton*  m_pBuyButton;				//购买按钮
	CCtrlEdit* m_pChatEdit;                 //聊天输入框
	CTalkViewer*   m_pTalkViewer;

	////
	DWORD   m_dwLastBuyGoodID;//最后一次正在购买的物品，在没有回应之前不能再买这个物品
	DWORD   m_dwLastClickBuyTime;//最后一次点购买时间
	DWORD   m_dwLastSayTime;	//上一次留言时间,每10秒一次

	void	GetGoodGrid(int* iX, int* iY, int ix, int iy);	//取得物品所在单元格
	bool	IsInGoodGrid(int ix,int iy);    //是否在金钱交易物品栏内
};
