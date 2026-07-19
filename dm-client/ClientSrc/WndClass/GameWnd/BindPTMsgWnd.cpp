#include "BindPTMsgWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/LoginData.h"


INIT_WND_POSX(CBindPTMsgWnd,POS_AUTO,POS_AUTO)

CBindPTMsgWnd::CBindPTMsgWnd(void)
{
	m_iIndex = 40;	
	m_pQutiButton = NULL;
	m_pSaveButton = NULL;
	m_pCancelButton = NULL;
	m_pMarkViewer = NULL;


	string str = "  \\";
	str += "                        警  告\\";
	str += "您的角色还没有绑定盛大通行证，<color=red 如果退出或异常掉线将会导致>\\";
	str += "<color=red 角色信息丢失！>请尽快与您的盛大通行证进行绑定操作以防造成\\";
	str += "损失！\\";
	str += "<color=red 您确定要退出游戏吗？>";
	m_tagText.Parse(str,0,"\\");
}

CBindPTMsgWnd::~CBindPTMsgWnd(void)
{

}

void CBindPTMsgWnd::OnCreate()
{
	m_pQutiButton = new CCtrlButton();
	AddControl(m_pQutiButton);
	m_pQutiButton->CreateEx(this,40,132,90, 91, 92, 93);
	m_pQutiButton->SetText("退 出",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,FONTSIZE_MIDDLE,DTF_BlackFrame,FONT_YAHEI);

	m_pSaveButton = new CCtrlButton();
	AddControl(m_pSaveButton);
	m_pSaveButton->CreateEx(this,150,132,90, 91, 92, 93);
	m_pSaveButton->SetText("保 存",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,FONTSIZE_MIDDLE,DTF_BlackFrame,FONT_YAHEI);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,260,132,90, 91, 92, 93);
	m_pCancelButton->SetText("取 消",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,FONTSIZE_MIDDLE,DTF_BlackFrame,FONT_YAHEI);

	m_pMarkViewer = new CMarkViewer(16,15);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,30,16,330,130);
	m_pMarkViewer->SetTagText(&m_tagText);
}


bool CBindPTMsgWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{	
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pQutiButton)
			{
				if(g_pControl->GetGameState() == MSG_CTRL_GAMEWND)
				{
					g_pGameControl->SEND_Objects_Position();
				}
				PostMessage(g_hWnd,WM_DESTROY,0,0);

				return true;
			}
			else if (pControl == m_pSaveButton)
			{
				CloseWindow();
				g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);

				if (g_Login.GetLoginInExpType() == 1)
					g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_CREATE);
				return true;
			}
			else if (pControl == m_pCancelButton)
			{
				CloseWindow();
				g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);
				return true;
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}