#include "greetingwnd.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include <Shellapi.h>
#include "GameData/NpcData.h"
#include "GameMap/GameMap.h"
#include "GameAI/AIConfigMgr.h"
#include "GameData/ConfigData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/LoginData.h"

INIT_WND_POSX(CGreetingWnd,POS_AUTO,POS_AUTO)

CGreetingWnd::CGreetingWnd(void)
{
	m_bModel		= true;
	m_pMarkViewer = NULL;

	m_iIndex = 16350;
	m_iPages = 2;

	m_iAlignType = XAL_TOPLEFT;

	m_iVersion = 2;

	AddTabPage(0,0,MAKELONG(16350,PACKAGE_INTERFACE),37,201,120,121,122,123,"内容介绍");
	AddTabPage(0,0,MAKELONG(16350,PACKAGE_INTERFACE),112,201,120,121,122,123,"版本下载");


	g_AICfgMgr.LoadConfigBeforeEnter();
}

CGreetingWnd::~CGreetingWnd(void)
{
}

void CGreetingWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	m_iCurPage = m_TabPage.iCurPage;

	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,224,380,59,60,61);
	m_pOk->SetText("确 定", 0xFFF0AF64 ,0xFFFAC896, 0xFF5AA0A0, 0xFF646464, FONTSIZE_MIDDLE,0,FONT_YAHEI);

	m_pMarkViewer = new CMarkViewer(17,7,2,3);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,47,236,451,360);
	//m_pMarkViewer->AddUpButton(389,1,16357,16358,16359);
	//m_pMarkViewer->AddDownButton(389,105,16361,16362,16363);
	m_pMarkViewer->AddScrollEx(415,-16,17, 153);

	if(m_iCurPage == 0)
		m_pMarkViewer->SetTagText(&g_TagTextMgr.GetGreetingData());
	else if(m_iCurPage == 1)
		m_pMarkViewer->SetTagText(&g_TagTextMgr.GetGreetingDownloadData());

	m_pMarkViewer->SetFont(FONT_YAHEI, FONTSIZE_DEFAULT);
//	m_pMarkViewer->SetColor(COLOR_TEXT_NORMAL);
	m_pMarkViewer->SetDefaultColor(0xFFFAC897);

	m_pNewHandPrompt = new CCtrlRadio;
	AddControl(m_pNewHandPrompt);
	m_pNewHandPrompt->Create(this,250,201,125,126,127,128,110,20);
	m_pNewHandPrompt->SetText("我已经熟悉游戏，无需新手提示",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	m_pNewHandPrompt->SetChecked(!g_AICfgMgr.IsShowNewHandPrompt());

	SetCloseButton(462,15,80);
}

void CGreetingWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(212+m_iScreenX, 17+m_iScreenY, "热 点 活 动", 0xFFFAC896, FONT_YAHEI, FONTSIZE_BIG);

	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE, 23200,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 39,m_iScreenY + 55,pTex);
	}
}

bool CGreetingWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOk)
			{
				OnClickCloseButton();
				return true;
			}
			else if(pControl == m_pNewHandPrompt)
			{
				g_AICfgMgr.SetShowNewHandPrompt(!g_AICfgMgr.IsShowNewHandPrompt());
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CGreetingWnd::OnClickCloseButton()
{			
	//进入游戏
TRY_BEGIN
	g_pGameControl->SEND_Game_EnterGame();

	//打开小地图
	g_pGameControl->SEND_Open_MapView();
	
	g_pControl->Msg(MSG_CTRL_AI_START,1);
TRY_END

	CloseWindow();

	if(g_AICfgMgr.IsAutoShowActLog())
	{
		g_pControl->PopupWindow(MSG_CTRL_ACTIVITY_LOG_WND);
	}

	if (g_Login.GetLoginInExpType() == 1)
	{	
		string str = "                        警  告\\";
		str += "您的角色还没有绑定盛大通行证，<color=red 如果退出或异常掉线将会导致>\\";
		str += "<color=red 角色信息丢失！>请尽快与您的盛大通行证进行绑定操作以防造成\\";
		str += "损失！\\";
		str += "<color=ggreen 注：未绑定盛大通行证的角色不可进行交易、摆摊、使用元宝等>\\";
		str += "<color=ggreen 操作。>";
		g_MsgBoxMgr.PopTagAlert(str.c_str());
	}
}

bool CGreetingWnd::OnLeftButtonDown(int iX,int iY)
{
	m_bClick = true;
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CGreetingWnd::OnLeftButtonUp(int iX, int iY)
{
	m_bClick = false;
	std::string strCommand = m_pMarkViewer->GetCommand();
	if(!strCommand.empty())
	{
		//up 音乐
		g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);

		if(strCommand.substr(0,2) == "@@")
		{
			if(strCommand.substr(2,3)=="url")
			{
				if( ((g_dwServerSwitch & SS_REGISTER_FCM) != 0) && (stricmp(strCommand.c_str()+5,"http://pwd.sdo.com/ptinfo/ptinfo/filladultinfo.aspx?from=1") == 0) )
				{
					OnClickCloseButton();
					g_pControl->Msg(MSG_CTRL_REGISTER_INFO_WND,2);
					g_pControl->Msg(MSG_CTRL_REGISTER_INFO_WND_FOCUS,1);
					return true;
				}
				else
				{
					ShellExecute(NULL,"open","iexplore.exe",strCommand.substr(5).c_str(),NULL,SW_SHOW);
				}
			}

			return true;
		}
		else
		{
			g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
			return true;
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CGreetingWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	char name[40]={0};
	switch(cKey)
	{
	case VK_RETURN:
		{
			OnClickCloseButton();
			return true;
		}
	} 

	return CCtrlWindowX::OnKeyDown(wState,cKey);
}

bool CGreetingWnd::OnWheel(int iWheel)
{
	if(m_pMarkViewer)
		m_pMarkViewer->OnWheel(iWheel);

	return true;
}
