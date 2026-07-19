#pragma once
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/Good.h"
#include "BaseClass/Control/CtrlMenuButton.h"

class CLeaseBoothWnd :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(CLeaseBoothWnd)
public:
	CLeaseBoothWnd(void);
	~CLeaseBoothWnd(void);
public:
	//重载父类函数
	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	//virtual bool OnLeftButtonDClick(int iX,int iY);
protected:
	int		m_iCellW,m_iCellH;				//单元格的宽度和高度
	int		m_iWCells,m_iHCells;			//水平和垂直方向的格数
	int		m_iCellDisW,m_iCellDisH;		//单元格水平和垂直的间隔

	POINT	m_ptGoodPoint;		    	    //物品栏左上角点坐标
	bool	m_bDClicked;					//是否是双击左键
	bool	m_bFistTimeUse;					//购买摊位后第一次打开
	DWORD   m_dwtype;						//0-为租赁,2-自己已租赁

	CCtrlButton*  m_pStartButton;    //确认租赁
	CCtrlButton*  m_pEndButton;      //取消租赁
	CCtrlMenuButton*    m_pMType;

	////
	DWORD   m_dwLastBuyGoodID;//最后一次正在购买的物品，在没有回应之前不能再买这个物品
	DWORD   m_dwLastClickBuyTime;//最后一次点购买时间

	void	GetGoodGrid(int* iX, int* iY, int ix, int iy);	//取得物品所在单元格
	bool	IsInGoodGrid(int ix,int iy);    //是否在金钱交易物品栏内
};
