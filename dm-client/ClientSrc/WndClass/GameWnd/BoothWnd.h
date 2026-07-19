#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/TalkViewer.h"
#include "GameData/Good.h"


//摊位窗口
class CBoothWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CBoothWnd)

public:
	CBoothWnd();
	~CBoothWnd(void);

public:
	//重载父类函数
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
	CCtrlButton*  m_pNameButton;     //设置摊位名称
	CCtrlButton*  m_pStartButton;    //开始摆摊
	CCtrlButton*  m_pEndButton;      //结束摆摊
	CCtrlButton*  m_pLepoardButton;     //豹子守摊
	CCtrlButton*  m_pGetSaleButton;    //取回卖物品所得
	CCtrlButton*  m_pGetGoodButton;      //取回物品

	CCtrlButton* m_pUpButton;
	CCtrlButton* m_pDownButton;
	CCtrlEdit* m_pChatEdit;                 //聊天输入框

	//聊天框
	CTalkViewer*   m_pTalkViewer;
	int          m_nChannel;

	void	GetGoodGrid(int* iX, int* iY, int ix, int iy);	//取得物品所在单元格
	bool	IsInGoodGrid(int ix,int iy);    //是否在金钱交易物品栏内
	bool    IsInChatArea(int x,int y);

};
