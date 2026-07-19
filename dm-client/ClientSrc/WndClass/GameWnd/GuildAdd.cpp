#include "GuildAdd.h"
#include "Global\GlobalMsg.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"


INIT_WND_POSX(CGuildAdd,POS_VARIABLE,POS_VARIABLE)

CGuildAdd::CGuildAdd()
{
	m_pOk = NULL;
	m_pCancel = NULL;
	m_begin_time = g_Timer._GetTickCount();
	SetLifeTime(m_begin_time + 30000);
	m_bFriend = false;

	//	m_bFriend = false;
	//	if(m_bFriend)
	//	{
	//		m_txt+="申请将你加入好友名单，";
	//		m_txt2="请验证。";
	//	}
	//	else
	//		m_txt+="邀请您加入行会";
	//}
	//else
	//	m_txt += "邀请您加入行会";


	m_iIndex = 30;
	m_iPages = 1;
	m_uStyle = CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX;
	m_iAlignType = XAL_BOTTOMLEFT;
	m_iOffX = 10;
	m_iOffY = -184-121;//g_pCG->GetHeight() - m_pGamePanelWnd->GetHeight() - m_pGuildAddWnd->GetHeight();

}


void CGuildAdd::Draw(void)
{
	CCtrlWindow::Draw();

	g_Font.DrawText(m_iScreenX+ 30,m_iScreenY+23,m_txt.c_str(),0xFFFFFFFF,FONT_SMALL);

	if(!m_txt2.empty())
		g_Font.DrawText(m_iScreenX+ 30,m_iScreenY + 39,m_txt2.c_str(),0xFFFFFFFF,FONT_SMALL);

	DWORD m_end_time = g_Timer._GetTickCount();
	int iLeftTime = (30 - (m_end_time - m_begin_time)/1000);
	if(iLeftTime < 0)
		iLeftTime = 0;

	char temp[32] = {0};
	sprintf(temp,"%d",iLeftTime);
	g_Font.DrawText(m_iScreenX+112,m_iScreenY+ 52,temp,0xFFFFFFFF,FONT_SMALL);
}

bool CGuildAdd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if( dwMsg == MSG_CTRL_BUTTON_CLICK && pControl== (CControl*)m_pOk)
	{
		OnOk();
		return true;
	}
	else if( dwMsg == MSG_CTRL_BUTTON_CLICK && pControl== (CControl*)m_pCancel )
	{
		OnCancel();
		return true;
	}
	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

void CGuildAdd::OnCreate()
{
	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,45,77,31,32,33);

	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,150,77,34,35,36);
}

void CGuildAdd::OnOk()
{
	if(m_bFriend)
	{
		//const char* name = OtherData.GetFriendRequestVector().front().c_str();
		//g_pGameControl->SEND_Answer_Friend_Request(name,true);
		//OtherData.GetFriendRequestVector().erase(OtherData.GetFriendRequestVector().begin());
	}
	else
	{
		g_pGameControl->SEND_Guild_Reply_Invited(true);
	}
	g_pControl->Msg(MSG_GUILDADD_WND,0);
}

void CGuildAdd::OnCancel()
{
	OnClickCloseButton();
}

bool CGuildAdd::OnLeftButtonDown(int iX,int iY)
{
	return true;
}
void CGuildAdd::OnClickCloseButton()
{
	if(m_bFriend)
	{
		//const char* name = OtherData.GetFriendRequestVector().front().c_str();
		//g_pGameControl->SEND_Answer_Friend_Request(name,false);
		//OtherData.GetFriendRequestVector().erase(OtherData.GetFriendRequestVector().begin());
	}
	else
	{
		g_pGameControl->SEND_Guild_Reply_Invited(false);
	}
	CloseWindow();
}