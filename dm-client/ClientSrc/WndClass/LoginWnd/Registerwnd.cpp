#include "registerwnd.h"
#include "Global/GlobalMsg.h"
#include "Global/Interface/StreamInterface.h"
#include "GameData/GameData.h"
#include "GameClient/WidgetManager.h"
#include "GameData/ConfigData.h"
#include <Shellapi.h>

INIT_WND_POSX(CRegisterWnd,POS_VARIABLE,POS_VARIABLE)

CRegisterWnd::CRegisterWnd(void)
{
	m_dwRealStorLoginTime = GetTickCount();
	m_bNoMove = true;
	m_bLoaded = false;

	m_iIndex = 14225;
	m_iPages = 1;
	m_iAlignType = XAL_CENTER;
	m_iOffX = 0;
	m_iOffY = 0;
	m_uStyle &= ~CTRL_STYLE_TRANS;

}

CRegisterWnd::~CRegisterWnd(void)
{
	g_WidgetMgr.SetShow(EWT_INNER_REGISTER,false);
	g_WidgetMgr.SetFocus(EWT_INNER_REGISTER,false);
}

void CRegisterWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

 	if(!g_WidgetMgr.IsShow(EWT_INNER_REGISTER))
 	{
 		if(!m_bLoaded && GetTickCount() - m_dwRealStorLoginTime> 30000)
 		{
 			string url = g_pStreamMgr->GetWebsite("NewUser");
 			//string zone = g_pStreamMgr->GetGameStr("Area");
 
 			//char newuser_url[512] = {0};
 			//char szVersion[256] = {0};
 			//GetVersion(szVersion);
 			//sprintf(newuser_url,"%s&zone=%s&version=%s",url.c_str(),zone.c_str(),szVersion);
 
 			//ShellExecute(NULL,"open","iexplore.exe",newuser_url,NULL,SW_SHOW);
 
			ShellExecute(NULL,"open","iexplore.exe",url.c_str(),NULL,SW_SHOW);
 			m_bLoaded = true;
 			m_dwLifeTime = GetTickCount() + 50;
 
 			return;
 		}
 		else if(!m_bLoaded && GetTickCount() - m_dwRealStorLoginTime> 25000)
 		{
 			char strTemp[128];
 			sprintf(strTemp,"加载注册页面超时,%d秒后转到网站注册",5-(GetTickCount() - m_dwRealStorLoginTime-25000)/1000);
 			g_pFont->DrawText(m_iScreenX + 200, m_iScreenY + 295, strTemp ,0xFFFFFF00,FONT_DEFAULT, FONTSIZE_BIG);
 		}
 		else
 		{
 			int i = (GetTickCount() - m_dwRealStorLoginTime)/500%3;
 			if(i == 0)
 				g_pFont->DrawText(m_iScreenX + 250, m_iScreenY + 295, "正在加载注册页面.",0xFFFFFF00, FONT_DEFAULT,FONTSIZE_BIG);
 			else if(i==1)
 				g_pFont->DrawText(m_iScreenX + 250, m_iScreenY + 295, "正在加载注册页面..",0xFFFFFF00, FONT_DEFAULT,FONTSIZE_BIG);
 			else
 				g_pFont->DrawText(m_iScreenX + 250, m_iScreenY + 295, "正在加载注册页面...",0xFFFFFF00, FONT_DEFAULT,FONTSIZE_BIG);
 		}
 	}
 	else if(!m_bLoaded)
 	{
 		m_bLoaded = true;
 	}

	g_WidgetMgr.RenderWidget(EWT_INNER_REGISTER);
}

void CRegisterWnd::OnClickCloseButton()
{
	g_WidgetMgr.SetShow(EWT_INNER_REGISTER,false);
	g_WidgetMgr.SetFocus(EWT_INNER_REGISTER,false);
	g_pControl->Msg(MSG_CTRL_USERLOGINWND,OPER_CREATE);//重新生成userloginwnd
	CloseWindow();
}

void CRegisterWnd::OnCreate()
{
	SetCloseButton(752,5,66);
	g_WidgetMgr.LoginRegister();
	g_WidgetMgr.MoveWindow(EWT_INNER_REGISTER,m_iScreenX+15,m_iScreenY+10,781-42,602-24);	 	
	m_dwRealStorLoginTime = GetTickCount();
}

bool CRegisterWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CRegisterWnd::GetVersion(char* szVersion)
{
	int iLen = strlen(g_strVersion);
	int j = 0;
	for(int i=0;i<iLen;i++)
	{
		char ch = g_strVersion[i];
		if((ch >= '0' && ch <= '9' ) || ch == '.')
			szVersion[j++] = ch;
	}
	szVersion[j] = '\0';
}

void  CRegisterWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_INNER_REGISTER,true);
}

void CRegisterWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_INNER_REGISTER,false);
}

void CRegisterWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_INNER_REGISTER,m_iScreenX + 15,m_iScreenY + 10);
}
