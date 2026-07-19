#include "AuthenWnd.h"
#include "Global/GlobalMsg.h"
#include "Global/Interface/StreamInterface.h"
#include "GameData/GameData.h"
#include "GameClient/WidgetManager.h"
#include "GameData/ConfigData.h"
#include <Shellapi.h>
#include "GameControl/GameControl.h"

INIT_WND_POSX(CAuthenWnd,POS_VARIABLE,POS_VARIABLE)

CAuthenWnd::CAuthenWnd(void)
{
	m_dwRealTime = GetTickCount();
	m_bLoaded = false;
	m_bDisableEscape = true;

	m_iIndex = 21690;
	m_iPages = 1;
	m_iAlignType = XAL_CENTER;
	m_iOffX = 0;
	m_iOffY = 0;
	m_uStyle &= ~CTRL_STYLE_TRANS;

	//m_pSubmit = NULL;
	//m_pCancel = NULL;

}

CAuthenWnd::~CAuthenWnd(void)
{
	g_WidgetMgr.SetShow(EWT_AUTHEN,false);
	g_WidgetMgr.SetFocus(EWT_AUTHEN,false);
}

void CAuthenWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(!g_WidgetMgr.IsShow(EWT_AUTHEN))
	{
		if(!m_bLoaded && GetTickCount() - m_dwRealTime> 30000)
		{
			const string& url = g_WidgetMgr.GetAuthenURL();			
			ShellExecute(NULL,"open","iexplore.exe",url.c_str(),NULL,SW_SHOW);

			m_bLoaded = true;
			m_dwLifeTime = GetTickCount() + 50;

			return;
		}
		// 		else if(!m_bLoaded && GetTickCount() - m_dwRealTime> 25000)
		// 		{
		// 			char strTemp[128];
		// 			sprintf(strTemp,"加载页面超时,%d秒后转到网站",5-(GetTickCount() - m_dwRealTime-25000)/1000);
		// 			g_pFont->DrawText(m_iScreenX + 200, m_iScreenY + 295, strTemp ,0xFFFFFF00,FONT_DEFAULT, FONTSIZE_BIG);
		// 		}
		else
		{
			int i = (GetTickCount() - m_dwRealTime)/500%3;
			if(i == 0)
				g_pFont->DrawText(m_iScreenX + 160, m_iScreenY + 140, "正在加载页面.",0xFF000000, FONT_DEFAULT,FONTSIZE_BIG);
			else if(i==1)
				g_pFont->DrawText(m_iScreenX + 160, m_iScreenY + 140, "正在加载页面..",0xFF000000, FONT_DEFAULT,FONTSIZE_BIG);
			else
				g_pFont->DrawText(m_iScreenX + 160, m_iScreenY + 140, "正在加载页面...",0xFF000000, FONT_DEFAULT,FONTSIZE_BIG);
		}
	}
	else if(!m_bLoaded)
	{
		m_bLoaded = true;
	}

	g_WidgetMgr.RenderWidget(EWT_AUTHEN);
}

void CAuthenWnd::OnClickCloseButton()
{
	g_WidgetMgr.SetShow(EWT_AUTHEN,false);
	g_WidgetMgr.SetFocus(EWT_AUTHEN,false);
	g_pControl->Msg(MSG_CTRL_USERLOGINWND,OPER_CREATE);//重新生成userloginwnd
	CloseWindow();
}

void CAuthenWnd::OnCreate()
{
	//SetCloseButton(752,5,66);

	g_WidgetMgr.LoginAuthen();
	g_WidgetMgr.MoveWindow(EWT_AUTHEN,m_iScreenX+14,m_iScreenY+14,430,270);
	m_dwRealTime = GetTickCount();

	//m_pSubmit = new CCtrlButton;				// 提交按钮	
	//AddControl(m_pSubmit);
	//m_pSubmit->SetText("确定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);
	//m_pSubmit->CreateEx(this,200,500,1658,1659,1660);

	//m_pCancel = new CCtrlButton; // 取消按钮	
	//AddControl(m_pCancel);
	//m_pCancel->SetText("取消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);
	//m_pCancel->CreateEx(this,400,500,1658,1659,1660);
}

bool CAuthenWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			//if (pControl == m_pSubmit)
			//{
			//	OnClickCloseButton();
			//	return true;
			//}
			//else if (pControl == m_pCancel)
			//{
			//	OnClickCloseButton();
			//	return true;
			//}
		}
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void  CAuthenWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_AUTHEN,true);
}

void CAuthenWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_AUTHEN,false);
}

void CAuthenWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_AUTHEN,m_iScreenX + 14,m_iScreenY + 14);
}
