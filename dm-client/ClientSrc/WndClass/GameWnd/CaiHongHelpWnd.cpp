#include "CaiHongHelpWnd.h"
#include "GameClient/WidgetManager.h"
#include "GameData/GameData.h"
#include "GameData/LoginData.h"


INIT_WND_POSX(CCaiHongHelpWnd,POS_AUTO,POS_AUTO)

CCaiHongHelpWnd::CCaiHongHelpWnd(void)
{
	m_iIndex = 23212;
}

CCaiHongHelpWnd::~CCaiHongHelpWnd(void)
{
	g_WidgetMgr.SetShow(EWT_CAIHONGHELP,false);
	g_WidgetMgr.SetFocus(EWT_CAIHONGHELP,false);
}

void CCaiHongHelpWnd::OnCreate()
{
	SetCloseButton(440,10,80);

	//g_WidgetMgr.LoginCaiHong(102, 0, "21");
	g_WidgetMgr.LoginCaiHong(g_Login.GetInnerAreaNo(), g_Login.GetInnerGroupNo(), g_Login.GetThirdChannel().c_str());
	g_WidgetMgr.MoveWindow(EWT_CAIHONGHELP,m_iScreenX + 8,m_iScreenY + 40,455,455);	

	m_dwStartLoadPageTime = GetTickCount();
}

void CCaiHongHelpWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pGfx->DrawFillRect(m_iScreenX+8,m_iScreenY+40,455,455,0xFF000000);

	int iOffx = 0,iOffy = 0;
	if(g_WidgetMgr.IsShow(EWT_CAIHONGHELP))
	{
		g_WidgetMgr.RenderWidget(EWT_CAIHONGHELP);
	}
	else
	{
		DWORD dwPastTime = GetTickCount() - m_dwStartLoadPageTime;
		if(dwPastTime > 60000)
		{
			iOffx = m_iScreenX + 160;
			iOffy = m_iScreenY + 250;

			g_pFont->DrawText(iOffx, iOffy,"连接超时！",-1);
		}
		else
		{
			iOffx = m_iScreenX + 160;
			iOffy = m_iScreenY + 250;

			if(dwPastTime % 1000 < 250)
				g_pFont->DrawText(iOffx, iOffy,"正在加载，请稍候",-1);
			else if(dwPastTime % 1000 < 500)
				g_pFont->DrawText(iOffx, iOffy,"正在加载，请稍候。",-1);
			else if(dwPastTime % 1000 < 750)
				g_pFont->DrawText(iOffx, iOffy,"正在加载，请稍候。。",-1);
			else
				g_pFont->DrawText(iOffx, iOffy,"正在加载，请稍候。。。",-1);
		}
	}

}

void CCaiHongHelpWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_CAIHONGHELP,m_iScreenX + 8,m_iScreenY + 40);
}

void  CCaiHongHelpWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_CAIHONGHELP,true);
}

void CCaiHongHelpWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_CAIHONGHELP,false);
}

void CCaiHongHelpWnd::OnClickCloseButton()
{
	if(g_WidgetMgr.IsShow(EWT_CAIHONGHELP))
	{
		g_WidgetMgr.ExecScript(EWT_CAIHONGHELP, "if(typeof(System)==\"object\"&&System!=null&&typeof(System.CloseQuery)==\"function\"&&!System.CloseQuery()){}else{window.close();}");
		g_WidgetMgr.SetFocus(EWT_CAIHONGHELP,true);
	}
	else
	{
		CloseWindow();
	}
}

