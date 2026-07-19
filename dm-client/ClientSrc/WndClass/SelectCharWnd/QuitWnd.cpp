#include "QuitWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/LoginData.h"
#include "GameData/OtherData.h"
#include "GameAI/AIAutoMgr.h"
#include "GameData/TaskData.h"
#include "GameData/MsgBoxMgr.h"

INIT_WND_POS(CQuitWnd,POS_AUTO,POS_AUTO)

CQuitWnd::CQuitWnd()
{
	m_pQuitGame = NULL;
	m_pQuitToSafe = NULL;
	m_pCancel = NULL;
	m_pQuitToSafe = NULL;
	m_iIndex = /*16*/21610;
	m_sName  = "QuitWnd";
	m_bNeedSavePos = false;
	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CQuitWnd::~CQuitWnd()
{

}

bool CQuitWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pQuitToSafe)
		{
			//g_pControl->Msg(MSG_CTRL_UPDATE_TRACK_INFO,MAKELONG(0,9));
			OnQuitToSafe();
			return true;
		}
		else if(pControl == m_pQuitGame )
		{
			OnQuitGame();
			return true;
		}
		else if(pControl== m_pQuitToBegin )
		{
			//g_pControl->Msg(MSG_CTRL_UPDATE_TRACK_INFO,MAKELONG(0,9));
			OnQuitToBegin();
			return true;
		}
		else if(pControl== m_pCancel )
		{
			OnCancel();
			return true;
		}
	}
	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

bool CQuitWnd::OnKeyDown(WORD wState, UCHAR cKey)
{
	switch(cKey)
	{
	case VK_ESCAPE:
		{
			OnCancel();
			return true;
		}
		break;
	}
	return true;
}

void CQuitWnd::OnClickOKButton(void)
{
}

void CQuitWnd::OnClickCancelButton(void)
{

}

void CQuitWnd::OnCreate(void)
{
	

	if(SELF.GetID() > 0 && SELF.IsDead())
	{
		//退到安全区
		m_pQuitToSafe = new CCtrlButton();
		AddControl(m_pQuitToSafe);
		m_pQuitToSafe->CreateEx(this,54,47,21,22,23,24);
		m_pQuitToSafe->SetText("立 即 复 活", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		//退到登录界面
		m_pQuitToBegin = new CCtrlButton();
		AddControl(m_pQuitToBegin);
		m_pQuitToBegin->CreateEx(this,54,85,21,22,23,24);
		m_pQuitToBegin->SetText("退到登录界面", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		//退出游戏
		m_pQuitGame = new CCtrlButton();
		AddControl(m_pQuitGame);
		m_pQuitGame->CreateEx(this,54,123,21,22,23,24);
		m_pQuitGame->SetText("退 出 游 戏", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	}
	else
	{
		//退出游戏
		m_pQuitGame = new CCtrlButton();
		AddControl(m_pQuitGame);
		m_pQuitGame->CreateEx(this,54,47,21,22,23,24);
		m_pQuitGame->SetText("退 出 游 戏", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		//退到登录界面
		m_pQuitToBegin = new CCtrlButton();
		AddControl(m_pQuitToBegin);
		m_pQuitToBegin->CreateEx(this,54,85,21,22,23,24);
		m_pQuitToBegin->SetText("退到登录界面", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		//返回游戏
		m_pCancel = new CCtrlButton();
		AddControl(m_pCancel);
		m_pCancel->CreateEx(this,54,123,21,22,23,24);
		m_pCancel->SetText("返 回 游 戏", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	}
	SetCloseButton(200,12, 80);

	// 如果是免注册和登录提前，不能退到登录界面
	if (g_Login.GetLoginInExpType() > 0)
	{
		m_pQuitToBegin->SetEnable(false);
		m_pQuitToBegin->SetShow(false);
	}
}

void CQuitWnd::OnClickCloseButton()
{
	g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);
}

void CQuitWnd::OnCancel()
{
	g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);
}

void CQuitWnd::OnQuitGame()
{
	if((g_dwServerSwitch & SS_QUIT_DELAY) && SELF.GetID() != 0 && !SELF.IsDead())//读秒退出
	{
		g_pControl->PopupWindow(MSG_CTRL_READ_QUIT_WND,OPER_CREATE,1);
		g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);
		return;
	}
	else
	{
		if (g_Login.GetLoginInExpType() == 1)
		{
			g_pControl->PopupWindow(MSG_CTRL_BINDPTMSGWND_WND,OPER_CREATE);
		}
		else
		{
			if(g_pControl->GetGameState() == MSG_CTRL_GAMEWND)
			{
				g_pGameControl->SEND_Objects_Position();
			}
			PostMessage(g_hWnd,WM_DESTROY,0,0);
		}
	}
}

void CQuitWnd::OnQuitToBegin()
{
	if((g_dwServerSwitch & SS_QUIT_DELAY) && SELF.GetID() != 0 && !SELF.IsDead())//读秒退出
	{
		g_pControl->PopupWindow(MSG_CTRL_READ_QUIT_WND,OPER_CREATE,2);
		g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);
		return;
	}
	else
		g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CUSTOM);
}

void CQuitWnd::OnQuitToSafe()
{
	//防止复活的时候地图跳到0,0的位置,这里清除SELF数据后还原x,y
	int iX,iY;
	SELF.GetXY(iX,iY);
	SELF.clear();
	SELF.SetXY(iX,iY);

	g_OtherData.SetAutoEnter(true);
	//清除
	g_pGameControl->SEND_Out_Game(false);
	Sleep(1000);

	//if(g_Login.GetLoginGateType() != 1)
	//	g_pNet->Reset();

	//g_pGameControl->SEND_Selchar_CharServer();
	g_pGameControl->SEND_SelRole_Req(g_Login.GetLoginID(),g_OtherData.GetCharName());

	g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);

	g_AIAutoMgr.LaunchWaigua(); //进入开启外挂

}

void CQuitWnd::Draw()
{
	CCtrlWindowS::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	/*if(!(SELF.GetID() > 0 && SELF.IsDead()))*/
	{
		g_pFont->DrawText(28+m_iScreenX, 158+m_iScreenY, "新角色在进入游戏后的第5、15、", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, /*FONTSIZE_MIDDLE*/12);
		g_pFont->DrawText(28+m_iScreenX, 176+m_iScreenY, "30分钟时，将会获得来自中州国王", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, /*FONTSIZE_MIDDLE*/12);
		g_pFont->DrawText(28+m_iScreenX, 194+m_iScreenY, "的馈赠，使新手之旅更加顺利。之", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, /*FONTSIZE_MIDDLE*/12);
		g_pFont->DrawText(28+m_iScreenX, 212+m_iScreenY, "后连续7天进入游戏时每天将获得", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, /*FONTSIZE_MIDDLE*/12);
		g_pFont->DrawText(28+m_iScreenX, 230+m_iScreenY, "更多来自中州国王的赠品。敬请期", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, /*FONTSIZE_MIDDLE*/12);
		g_pFont->DrawText(28+m_iScreenX, 248+m_iScreenY, "待！", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, /*FONTSIZE_MIDDLE*/12);
	}
	
}
