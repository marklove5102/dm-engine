/*
#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/TalkViewer.h"


class CUploadWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CUploadWnd)

public:
	CUploadWnd(void);
	~CUploadWnd(void);
public:

	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
private:
	int		        m_iCellW,m_iCellH;		//单元格的宽度和高度
	int		        m_iWCells,m_iHCells;	//水平和垂直方向的格数
	int		        m_iCellDisW,m_iCellDisH;//单元格水平和垂直的间隔
	POINT	        m_ptGoodPoint;		    //物品栏左上角点坐标
	POINT           m_ptHeadPoint;          //映像的包裹栏窗口
	int             m_iWHeads,m_iHHeads;    //映像的包裹宽度和高度格子数目
	bool	        m_bDClicked;			//是否是双击左键

	CCtrlButton*    m_pUploadButton;
	CCtrlButton*    m_pGoButton;
	CCtrlButton*    m_pLeaveStore;		    // 离开商城按钮
	CCtrlButton*    m_pClose;				// 关闭按钮
	CCtrlButton*    m_pSNDAJiFen;			// 盛大积分按钮
	CCtrlButton*    m_pAddYuanBao;			// 我要冲值按钮
	CTalkViewer*    m_pTalkViewer;      

	void	GetGoodGrid(int* iX, int* iY, int ix, int iy);	//取得物品所在单元格
	bool	IsInGoodGrid(int ix,int iy);                    //是否在金钱交易物品栏内
	void    GetPackageGrid(int* iX, int* iY, int ix, int iy);
	void    DrawSysInfo();                                  //绘制盛大积分，元宝等信息
};
*/