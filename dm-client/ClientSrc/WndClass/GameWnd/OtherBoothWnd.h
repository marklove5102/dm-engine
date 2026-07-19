#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "BoothWnd.h"

//其他人的摊位
class COtherBoothWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(COtherBoothWnd)

public:
	COtherBoothWnd(void);
	~COtherBoothWnd(void);
protected:
	int		m_iCellW,m_iCellH;				//单元格的宽度和高度
	int		m_iWCells,m_iHCells;			//水平和垂直方向的格数
	int		m_iCellDisW,m_iCellDisH;		//单元格水平和垂直的间隔
	DWORD   m_dwLastBuyGoodID;//最后一次正在购买的物品，在没有回应之前不能再买这个物品
	DWORD   m_dwLastClickBuyTime;//最后一次点购买时间

	POINT	m_ptGoodPoint;		    	    //物品栏左上角点坐标
	bool	m_bDClicked;					//是否是双击左键
	CParserTip * m_pTip;                    //Tip
	CCtrlButton*  m_pBuyButton;				//购买按钮
	CCtrlButton*  m_pMoreOtherbooth;		//查看身边其它摊位

	CCtrlEdit* m_pChatEdit;                 //聊天输入框

	//搜索界面部分
	//CCtrlEdit* m_pSearchGoodName;

	//CCtrlButton* m_pSearchButton;
	//CCtrlButton* m_pGoButton;

	CTalkViewer*   m_pTalkViewer;
	int     m_nChannel;
	//int     m_iBoothType;

	void	GetGoodGrid(int* iX, int* iY, int ix, int iy);	//取得物品所在单元格
	bool	IsInGoodGrid(int ix,int iy);    //是否在金钱交易物品栏内
public:

	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
};

///////////////////////////////////////////////////////////////////////////////////
//确认是否购买的对话框
class CBoothBuyConfirmWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBoothBuyConfirmWnd)

public:
	enum BUY_TYPE{ BUY_MONEY = 0, BUY_YUANBAO};
	//构造函数
	CBoothBuyConfirmWnd();
	~CBoothBuyConfirmWnd();
protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pCancelButton;
	CCtrlButton* m_pGoButton;
	BUY_TYPE            m_iBuyType;
	std::string         m_strMsg;
	int                 m_iPos; //元宝两个字的位置

	DWORD				m_dwFlag;//sm_dwWindowType: 0-默认:玩家摊位,1-租赁摊位

public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
};
