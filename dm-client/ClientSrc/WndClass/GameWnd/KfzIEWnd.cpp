#include "KfzIEWnd.h"
#include "GameClient/WidgetManager.h"

INIT_WND_POSX(CKfzIEWnd,POS_AUTO,POS_AUTO)

CKfzIEWnd::CKfzIEWnd(void)
{
	m_iType = sm_dwWindowType;

	m_iIndex = 16026;
	m_iPages = 1;
	m_bNeedSavePos = false;
}

CKfzIEWnd::~CKfzIEWnd(void)
{
	g_WidgetMgr.SetShow(EWT_KFZ_IE,false);
	g_WidgetMgr.SetFocus(EWT_KFZ_IE,false);
}

void CKfzIEWnd::OnCreate()
{
	if(m_iType == 1)
	{
		g_WidgetMgr.LoginKfzRange();
	}
	else if(m_iType == 2)
	{
		g_WidgetMgr.LoginKfzVSMap();
	}
	else if(m_iType == 3)
	{
		g_WidgetMgr.LoginXLRange();
	}

	g_WidgetMgr.MoveWindow(EWT_KFZ_IE,m_iScreenX + 35,m_iScreenY + 56,678,378);
	SetCloseButton(716,42);
	SetMask(16026);
	m_dwStartLoadPageTime = GetTickCount();
}	

void  CKfzIEWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_KFZ_IE,true);
}

void CKfzIEWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_KFZ_IE,false);
}

void CKfzIEWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_KFZ_IE,m_iScreenX + 35,m_iScreenY + 54);
}

void CKfzIEWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iOffx = 0,iOffy = 0;
	if(g_WidgetMgr.IsShow(EWT_KFZ_IE))
	{
		g_WidgetMgr.RenderWidget(EWT_KFZ_IE);
	}
	else
	{
		DWORD dwPastTime = GetTickCount() - m_dwStartLoadPageTime;
		if(dwPastTime > 60000)
		{
			iOffx = m_iScreenX + 330;
			iOffy = m_iScreenY + 240;

			g_pFont->DrawText(iOffx, iOffy,"连接超时！",0xFFFFFFFF);
		}
		else
		{
			iOffx = m_iScreenX + 300;
			iOffy = m_iScreenY + 240;

			if(dwPastTime % 1000 < 250)
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候",0xFFFFFFFF);
			else if(dwPastTime % 1000 < 500)
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候。",0xFFFFFFFF);
			else if(dwPastTime % 1000 < 750)
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候。。",0xFFFFFFFF);
			else
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候。。。",0xFFFFFFFF);
		}
	}
}