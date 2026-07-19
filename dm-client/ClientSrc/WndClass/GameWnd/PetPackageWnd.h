#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/GoodGrid.h"


class CPetPackageWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CPetPackageWnd)

public:
	CPetPackageWnd();
	~CPetPackageWnd(void);

	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);

	inline  int    GetType()       { return m_iType;  }
protected:
	int        m_iType;   //类型，1：5个格子，2：10个格子
	int m_iWCells,m_iHCells;//包裹大小
	int m_iCellWidth,m_iCellHeight,m_iCellDisW,m_iCellDisH;//包裹格子大小及间隔
	POINT	m_ptGoodPoint;		    	    //物品栏左上角点坐标
	bool	m_bDClicked;					//是否是双击左键

	CCtrlButton* m_pPaiMaiBtn;
	CGoodGrid *m_pGoodGrid;//包裹物品

};
