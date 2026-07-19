#include "VoiceWnd.h"
#include "GameClient/VoiceAdapter.h"

#ifdef _CHAT

INIT_WND_POSX(CVoiceSearchWnd,POS_AUTO,POS_AUTO)

CVoiceSearchWnd::CVoiceSearchWnd(void)
{
	m_pSearchButton = NULL;
	m_pNameDownButton = NULL;
	m_pAreaDownButton = NULL;
	m_pGroupDownButton = NULL;

	m_iIndex = 14056;
}

CVoiceSearchWnd::~CVoiceSearchWnd(void)
{
}

void CVoiceSearchWnd::OnCreate()
{
	SetCloseButton(302,1,80);
	//游戏名

	//游戏名
	m_pNameDownButton = new CCtrlMenuButton();
	AddControl(m_pNameDownButton);
	m_pNameDownButton->Create(this,93,50,180,140,0,39,39,39,39,1030,1031,1032,1033,1,24,false,0,4);
	m_pNameDownButton->SetMenuTextColor(0xFF00FF00);
	m_pNameDownButton->ResetContent();
	m_pNameDownButton->SetText("请选择游戏名称");
	m_pNameDownButton->SetFont(FONT_YAHEI);
	VVGameInfo vGameInfo = g_VoiceAdapter.GetGameInfoVec();
	for(int i = 0;i < vGameInfo.size();i++)
	{
		m_pNameDownButton->AddString(vGameInfo[i].strGameName.c_str());
	}

	//游戏区名
	m_pAreaDownButton = new CCtrlMenuButton();
	AddControl(m_pAreaDownButton);
	m_pAreaDownButton->Create(this,93,85,180,140,0,39,39,39,39,1030,1031,1032,1033,1,24,false,0,4);
	m_pAreaDownButton->SetMenuTextColor(0xFF00FF00);
	m_pAreaDownButton->ResetContent();
	m_pAreaDownButton->SetEnable(false);
	m_pAreaDownButton->SetText("请选择对应游戏的区");
	m_pAreaDownButton->SetFont(FONT_YAHEI);

	//游戏组名
	m_pGroupDownButton = new CCtrlMenuButton();
	AddControl(m_pGroupDownButton);
	m_pGroupDownButton->Create(this,93,122,180,140,0,39,39,39,39,1030,1031,1032,1033,1,24,false,0,4);
	m_pGroupDownButton->SetMenuTextColor(0xFF00FF00);
	m_pGroupDownButton->ResetContent();
	m_pGroupDownButton->SetEnable(false);
	m_pGroupDownButton->SetText("请选择游戏名称");
	m_pGroupDownButton->SetFont(FONT_YAHEI);

	m_pSearchButton = new CCtrlButton();
	AddControl(m_pSearchButton);
	m_pSearchButton->CreateEx(this, 131,163, 95, 96, 97, 98);
	m_pSearchButton->SetText("搜索", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	m_pSearchButton->SetEnable(false);
}

void CVoiceSearchWnd::OnClickCloseButton()
{
	g_VoiceAdapter.LogoutSearchSever();
	CloseWindow();
}

void CVoiceSearchWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 163,m_iScreenY + 6,"语聊搜索",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	g_pFont->DrawText(m_iScreenX + 17,m_iScreenY + 53,"游戏名称:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 17,m_iScreenY + 89,"游戏大区:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 17,m_iScreenY + 123,"游戏组名:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
}

bool CVoiceSearchWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if (pControl == m_pSearchButton)
		{
			//搜索更新房间列表
			m_pSearchButton->SetEnable(false);
			g_VoiceAdapter.EnterGameRoomGroup(m_pNameDownButton->GetCurSel(),m_pAreaDownButton->GetCurSel(),m_pGroupDownButton->GetCurSel());
			//关闭窗口，返回用户需要的界面
			return true;
		}
		break;
	case MSG_CTRL_MENU_SELCHANGED:
		if(pControl == m_pNameDownButton)
		{
			int iSel = m_pNameDownButton->GetCurSel();
			if(iSel < 0)
				return true;

			m_pNameDownButton->SetEnable(false);
			m_pAreaDownButton->ResetContent();
			m_pAreaDownButton->SetEnable(false);
			m_pGroupDownButton->ResetContent();
			m_pGroupDownButton->SetEnable(false);
			m_pAreaDownButton->SetText("请选择对应游戏的区");
			m_pGroupDownButton->SetText("请选择对应游戏区域的组");

			m_pSearchButton->SetEnable(false);
			g_VoiceAdapter.SelectGameType(iSel);
			return true;
		}
		else if(pControl == m_pAreaDownButton)
		{
			int iSel = m_pAreaDownButton->GetCurSel();
			if(iSel < 0)
				return true;

			m_pAreaDownButton->SetEnable(false);
			m_pGroupDownButton->ResetContent();
			m_pGroupDownButton->SetEnable(false);
			m_pGroupDownButton->SetText("请选择对应游戏区域的组");
			m_pSearchButton->SetEnable(false);
			g_VoiceAdapter.SelectGameArea(m_pNameDownButton->GetCurSel(),iSel,m_pAreaDownButton->GetSelText());
			return true;
		}
		else if (pControl == m_pGroupDownButton)
		{
			int iSel = m_pGroupDownButton->GetCurSel();
			if(iSel < 0)
				return true;

			m_pGroupDownButton->SetEnable(false);
			m_pSearchButton->SetEnable(false);
			g_VoiceAdapter.SelectGameGroup(m_pNameDownButton->GetCurSel(),m_pAreaDownButton->GetCurSel(),m_pGroupDownButton->GetCurSel());
			return true;
		}
		break;

	case MSG_CTRL_CHANGE_AREA_BUTTON_STATUS:
		ChangeAreaButtonStatus(dwData);
		break;
	case MSG_CTRL_CHANGE_GROUP_BUTTON_STATUS:
		ChangeGroupButtonStatus(dwData);
		break;

	default:
		break;
	}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CVoiceSearchWnd::ChangeAreaButtonStatus(int flag)
{
	if(flag == 0)
	{
		VVGameInfo vGameInfo = g_VoiceAdapter.GetGameInfoVec();
		for (int i=0;i<vGameInfo[m_pNameDownButton->GetCurSel()].gameArea.size();i++)
		{
			m_pAreaDownButton->AddString(vGameInfo[m_pNameDownButton->GetCurSel()].gameArea[i].strGameArea.c_str());
		}

		m_pNameDownButton->SetEnable(true);
		m_pAreaDownButton->SetEnable(true);
	}
	else if (flag == 1)//获取area信息失败
	{
		m_pNameDownButton->SetEnable(true);
	}
}

void CVoiceSearchWnd::ChangeGroupButtonStatus(int flag)
{
	if(flag == 0)
	{
		VVGameInfo vGameInfo = g_VoiceAdapter.GetGameInfoVec();
		for (int i=0;i<vGameInfo[m_pNameDownButton->GetCurSel()].gameArea[m_pAreaDownButton->GetCurSel()].vGameGroup.size();i++)
		{
			m_pGroupDownButton->AddString(vGameInfo[m_pNameDownButton->GetCurSel()].gameArea[m_pAreaDownButton->GetCurSel()].vGameGroup[i].strGameGroup.c_str());
		}
		m_pAreaDownButton->SetEnable(true);
		m_pGroupDownButton->SetEnable(true);
	}
	else if (flag == 1)//获取group信息失败
	{
		m_pAreaDownButton->SetEnable(true);
	}
	else if (flag == 2)
	{
		m_pGroupDownButton->SetEnable(true);
		m_pSearchButton->SetEnable(true);
	}
}

#endif
