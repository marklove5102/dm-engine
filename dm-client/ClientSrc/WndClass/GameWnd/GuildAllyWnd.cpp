#include "guildallywnd.h"
#include "GameControl/GameControl.h"


INIT_WND_POSX(CGuildAllyWnd,POS_AUTO,POS_AUTO)

CGuildAllyWnd::CGuildAllyWnd(void)
{
	m_iIndex = 40;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
}

CGuildAllyWnd::~CGuildAllyWnd(void)
{	
}

void CGuildAllyWnd::OnCreate()
{
	//确定按钮
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,90,129,90,91,92);
	m_pOKButton->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//取消按钮
	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,225,129,90,91,92);
	m_pCancelButton->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pPermit = new CCtrlRadio;
	m_pPermit->Create(this,150,96,125,126,127,128,20);
	AddControl(m_pPermit);
	m_pPermit->SetText("允许联盟",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	//m_pPermit->SetChecked(true);
}

void CGuildAllyWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX+ 70 , m_iScreenY+40 ,"和对方行会结盟应该在“允许联盟状态下！”",0xFFFFFF00,FONT_YAHEI);
	g_pFont->DrawText(m_iScreenX+ 70 , m_iScreenY+70 ,"而且你应该面对对方行会首领。你想结盟吗？",0xFFFFFF00,FONT_YAHEI);
}

bool CGuildAllyWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOKButton && m_pPermit->IsChecked())
			{	
				//发送联盟信息
				g_pGameControl->SEND_Guild_Ally("@联盟");
				CloseWindow();
				return true;
			}else if(pControl == m_pPermit)
			{	
				char str[]="@允许联盟";		
				g_pGameControl->SEND_Message_Send(str,strlen(str));				
			}
			else if (pControl == m_pCancelButton)
			{
				CloseWindow();
				return true;
			}
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CGuildAllyWnd::OnLeftButtonDown(int iX, int iY)
{
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CGuildAllyWnd::OnLeftButtonUp(int iX, int iY)
{
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}