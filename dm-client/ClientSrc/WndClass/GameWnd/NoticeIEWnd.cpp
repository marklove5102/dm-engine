#include "NoticeIEWnd.h"
#include "GameClient/WidgetManager.h"
#include "GameData/GameData.h"

INIT_WND_POSX(CNoticeIEWnd,POS_AUTO,POS_AUTO)

CNoticeIEWnd::CNoticeIEWnd(void)
{
	m_iIndex = 22602;
}

CNoticeIEWnd::~CNoticeIEWnd(void)
{
	g_WidgetMgr.SetShow(EWT_NOTICE,false);
	g_WidgetMgr.SetFocus(EWT_NOTICE,false);
}

void CNoticeIEWnd::OnCreate()
{
	SetCloseButton(619,0,66);

	g_WidgetMgr.LoginNoticeIE(g_pGameData->GetNoticeIEURL());
	g_WidgetMgr.MoveWindow(EWT_NOTICE,m_iScreenX + 14,m_iScreenY + 14,627 - 14,447 - 14);	

	m_dwStartLoadPageTime = GetTickCount();
}

void CNoticeIEWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iOffx = 0,iOffy = 0;
	if(g_WidgetMgr.IsShow(EWT_NOTICE))
	{
		g_WidgetMgr.RenderWidget(EWT_NOTICE);
	}
	else
	{
		DWORD dwPastTime = GetTickCount() - m_dwStartLoadPageTime;
		if(dwPastTime > 60000)
		{
			iOffx = m_iScreenX + 284;
			iOffy = m_iScreenY + 205;

			g_pFont->DrawText(iOffx, iOffy,"连接超时！",-1);
		}
		else
		{
			iOffx = m_iScreenX + 248;
			iOffy = m_iScreenY + 205;

			if(dwPastTime % 1000 < 250)
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候",-1);
			else if(dwPastTime % 1000 < 500)
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候。",-1);
			else if(dwPastTime % 1000 < 750)
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候。。",-1);
			else
				g_pFont->DrawText(iOffx, iOffy,"正在加载数据，请稍候。。。",-1);
		}
	}

}

void CNoticeIEWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_NOTICE,m_iScreenX + 14,m_iScreenY + 14);
}

void  CNoticeIEWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_NOTICE,true);

	if (m_pParent)
	{
		m_pParent->Msg(MSG_CTRL_ON_SET_FOCUS,0,this);
	}
}

void CNoticeIEWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_NOTICE,false);
}

bool CNoticeIEWnd::OnKeyDown(WORD wState,UCHAR cKey)
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