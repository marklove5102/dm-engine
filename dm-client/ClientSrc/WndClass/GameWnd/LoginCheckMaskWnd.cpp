#include "LoginCheckMaskWnd.h"
#include "Global/Global.h"
#include "GameClient/WidgetManager.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "Global/GlobalMsg.h"

INIT_WND_POSX(CLoginCheckMaskWnd,POS_AUTO,POS_AUTO)

CLoginCheckMaskWnd::CLoginCheckMaskWnd(void)
{
	m_iIndex = 38;
}

CLoginCheckMaskWnd::~CLoginCheckMaskWnd(void)
{
	g_WidgetMgr.SetShow(EWT_LOGIN_CHECK,false);
	::SetFocus(g_hWnd);
}

void CLoginCheckMaskWnd::OnCreate()
{
	SetCloseButton(392,14,80);

	m_pSubmit = new CCtrlButton();
	m_pSubmit->CreateEx(this,285,144,90,91,92,93);
	m_pSubmit->SetText("确定");
	AddControl(m_pSubmit);
	

	m_pNextMask = new CCtrlButton();
	m_pNextMask->CreateEx(this,205,144,90,91,92,93);
	m_pNextMask->SetText("换一张");
	AddControl(m_pNextMask);
	

	m_pCheckMask = new CCtrlEdit();
	m_pCheckMask->CreateEx(this,200, 70, 200, 30,0,39,39,39,39);
	m_pCheckMask->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI,8);
	m_pCheckMask->SetMaxLength(7);
	AddControl(m_pCheckMask);
	m_pCheckMask->SetFocus();
	m_pCheckMask->SetPrompt("在此输入验证码");

	string str = g_pGameData->GetLoginCheckMask();
	g_WidgetMgr.LoginCheckMask(str.c_str());
	g_WidgetMgr.MoveWindow(EWT_LOGIN_CHECK,m_iScreenX + 50,m_iScreenY + 46,140,80);
}

void CLoginCheckMaskWnd::Draw()
{
	CCtrlWindowX::Draw();

	g_pFont->DrawText(m_iScreenX+ 205,m_iScreenY +25,"登陆验证码",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
	
	//g_pFont->DrawText(m_iScreenX+ 205,m_iScreenY + 47,"连续输错5次将被封停5分钟",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX+ 205,m_iScreenY + 107,"请输入左方对应字符",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	
	if(g_WidgetMgr.IsShow(EWT_LOGIN_CHECK))
	{
		m_pNextMask->SetEnable(true);
		g_WidgetMgr.RenderWidget(EWT_LOGIN_CHECK);
	}
	else
	{
		m_pNextMask->SetEnable(false);
		g_pGfx->DrawFillRect(m_iScreenX + 50, m_iScreenY + 46,140,80,0xff000000);
		char* tips[] = {"验证码下载中","验证码下载中.","验证码下载中..","验证码下载中..."};
		
		g_pFont->DrawText(m_iScreenX+ 60,m_iScreenY + 76,tips[g_pGfx->GetFrameCount()%80/20],0xffffffff,FONT_SONGTI,FONTSIZE_MIDDLE);
	}
}

void CLoginCheckMaskWnd::OnMove()
{
	CCtrlWindowX::OnMove();

	g_WidgetMgr.MoveWindow(EWT_LOGIN_CHECK,m_iScreenX + 50,m_iScreenY + 46);
}

void CLoginCheckMaskWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_LOGIN_CHECK,true);
}

void CLoginCheckMaskWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_LOGIN_CHECK,false);
}

bool CLoginCheckMaskWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pSubmit)
			{
				const char* pszMask = m_pCheckMask->GetText();
				g_pGameControl->Send_Login_Check_Mask(pszMask);

				return true;
			}
			else if (pControl == m_pNextMask)
			{
				string strUrl = g_pGameData->GetLoginCheckMask();
				if (strUrl.size() > 0)
					g_WidgetMgr.LoginCheckMask(strUrl.c_str());

				return true;
			}
		}
		break;
	case MSG_CTRL_LOGIN_CHECK_MASK_UPDATE:
		{
			string str = g_pGameData->GetLoginCheckMask();
			g_WidgetMgr.LoginCheckMask(str.c_str());
			break;
		}
	case MSG_CTRL_EDIT_ENTRY:
		{
			const char* pszMask = m_pCheckMask->GetText();
			
			g_pGameControl->Send_Login_Check_Mask(pszMask);

			return true;
		}
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}