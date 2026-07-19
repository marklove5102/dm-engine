#pragma once

#include "Global/Global.h"
#include "BaseClass/Control/Control.h"

class CGameWnd;
class CLoginWnd;
class CSelectCharWnd;
class CQuitWnd;
class CTalkViewWnd;


//窗口管理器
class CMainWnd : public CCtrlMainWin
{
DTI_DECLARE(CMainWnd, CCtrlMainWin)

public:
	CMainWnd(void);
	~CMainWnd(void);

public:
	virtual void	OnCreate();
	virtual void	Draw(void);
	virtual void	OnSetFocus();
	virtual bool	Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool    IsPanelFakeFocus();
	virtual bool	OnLeftButtonUp(int iX,int iY);
	virtual bool	OnLeftButtonDown(int iX,int iY);
	virtual bool    PopupArrowTip(DWORD dwWndMsg,int iTipID,int iX,int iY,int iAlignType = XAL_TOPLEFT,bool bShowNow = false,int iTipDir = XAL_TOPRIGHT,int iTipType = 0,DWORD dwData = 0,DWORD dwLastTime = 0,bool bNeedMsg = true);
	virtual bool    PopupArrowTip(const char* szWndName,int iTipID,int iX,int iY,int iAlignType = XAL_TOPLEFT,bool bShowNow = false,int iTipDir = XAL_TOPRIGHT,int iTipType = 0,DWORD dwData = 0,DWORD dwLastTime = 0,bool bNeedMsg = true);
	virtual bool    MsgToWnd(DWORD dwWndMsg,DWORD dwMsg,DWORD dwData = 0,CControl * pControl = NULL);
	virtual bool    MsgToWnd(const char *szName,DWORD dwMsg,DWORD dwData = 0,CControl * pControl = NULL);
	
	virtual int GetWidth();
	virtual int GetHeight();
	virtual void ReSetAllWndPos();
	virtual bool ChangeUi(E_UITYPE eUi = EUT_NONE,bool bCheckWindowWidth = false);//改变Ui,传EUT_NONE表示界面轮流切换,否则切换成指定的


	//快捷键
	bool DoAccelKey(WORD wKey);

private:
	CGameWnd*           m_pGameWnd;
	CLoginWnd*          m_pLoginWnd;
	CSelectCharWnd*     m_pSelectCharWnd;
	CQuitWnd*           m_pQuitWnd;
    CTalkViewWnd*       m_pTalkViewWnd;

private:
	// 处理各种输入消息
	bool Msg_MainSystem (DWORD dwMsg, DWORD dwData, CControl * pControl);
	// 管理子窗口
	bool Msg_ManageChild(DWORD dwMsg, DWORD dwData, CControl * pControl);

	// MessageBox消息
	bool Msg_MessageBox (DWORD dwMsg, DWORD dwData, CControl * pControl);

	// 传递给各种GameWnds内控件的消息
	bool Msg_GameWnds	(DWORD dwMsg, DWORD dwData, CControl * pControl);

	// 其他CMainWnd自己处理的消息
	bool Msg_MainOther  (DWORD dwMsg,DWORD dwData, CControl * pControl);

	//画鼠标及鼠标上的东西
	void DrawMouse();

	//改变UI
	void ReSetUI(E_UITYPE eNewType);
};
