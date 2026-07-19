#include "TodayActivityIEWnd.h"
#include "GameClient/WidgetManager.h"

INIT_WND_POSX(CTodayActivityIEWnd,POS_VARIABLE,POS_VARIABLE)

CTodayActivityIEWnd::CTodayActivityIEWnd(void)
{
	m_bInheritScaleFromParent = false;
	m_iIndex = 21592;
	//m_bNoMove = true;

	m_iAlignType = XAL_RIGHT;
	
	m_iOffX = 0;
	m_iOffY = -40;
}

CTodayActivityIEWnd::~CTodayActivityIEWnd(void)
{
	g_WidgetMgr.SetShow(EWT_TODAYACTIVITY,false);
	g_WidgetMgr.SetFocus(EWT_TODAYACTIVITY,false);
}

void CTodayActivityIEWnd::OnCreate()
{
	//SetCloseButton(332,1,66);
	g_WidgetMgr.LoginTodayActivity();
	g_WidgetMgr.MoveWindow(EWT_TODAYACTIVITY,m_iScreenX + 27,m_iScreenY + 22,190,306);	 	

	m_dwStartLoadPageTime = GetTickCount();
}

void CTodayActivityIEWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iOffx = 0,iOffy = 0;
	if(g_WidgetMgr.IsShow(EWT_TODAYACTIVITY))
	{
		g_WidgetMgr.RenderWidget(EWT_TODAYACTIVITY);
	}
	else
	{
		DWORD dwPastTime = GetTickCount() - m_dwStartLoadPageTime;
		if(dwPastTime > 60000)
		{
			iOffx = m_iScreenX + 90;
			iOffy = m_iScreenY + 170;

			g_pFont->DrawText(iOffx, iOffy,"连接超时！",0xFFFFFFFF);
		}
		else
		{
			iOffx = m_iScreenX + 50;
			iOffy = m_iScreenY + 170;

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

void CTodayActivityIEWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_TODAYACTIVITY,m_iScreenX + 27,m_iScreenY + 22);
}

void  CTodayActivityIEWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_TODAYACTIVITY,true);

	if (m_pParent)
	{
		m_pParent->Msg(MSG_CTRL_ON_SET_FOCUS,0,this);
	}
}

void CTodayActivityIEWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_TODAYACTIVITY,false);
}

bool CTodayActivityIEWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	switch(cKey)
	{
	case VK_RETURN:
		{
			if (m_pParent)
			{
				return m_pParent->OnKeyDown(wState,cKey);
			}
		}
	} 

	return CCtrlWindowX::OnKeyDown(wState,cKey);
}