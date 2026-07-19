#include "BindPTWnd.h"
#include "GameControl/GameControl.h"

#include <Shellapi.h>


INIT_WND_POSX(CBindPTWnd,POS_AUTO,POS_AUTO)

CBindPTWnd::CBindPTWnd(void)
{
	m_iIndex = 23300;
	m_pOKButton = NULL;
	m_pZhuButton = NULL;
	m_pEditUserID = NULL;
	m_pEditPassword = NULL;
}

CBindPTWnd::~CBindPTWnd(void)
{

}

void CBindPTWnd::OnCreate()
{
	//SetCloseButton(231,2,80);

	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,30,132,90, 91, 92, 93);
	m_pOKButton->SetText("保 存",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,FONTSIZE_MIDDLE,DTF_BlackFrame,FONT_YAHEI);

	m_pZhuButton = new CCtrlButton();
	AddControl(m_pZhuButton);
	m_pZhuButton->CreateEx(this,150,132,90, 91, 92, 93);
	m_pZhuButton->SetText("注 册",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,FONTSIZE_MIDDLE,DTF_BlackFrame,FONT_YAHEI);


	m_pEditUserID = new CCtrlEdit;
	AddControl(m_pEditUserID);
	m_pEditUserID->Create(this,FONTSIZE_DEFAULT,0xFFA06E32,93,54,130,18);
	m_pEditUserID->SetMaxLength(50);
	m_pEditUserID->SetFocus();

	m_pEditPassword = new CCtrlEdit;
	AddControl(m_pEditPassword);
	m_pEditPassword->Create(this,FONTSIZE_DEFAULT,0xFFA06E32,93,90,130,18);
	m_pEditPassword->SetPwd(true);
	m_pEditPassword->SetMaxLength(40);
}

void CBindPTWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
		return;


	g_pFont->DrawText(m_iScreenX + 126,m_iScreenY + 6,"保存角色",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	g_pFont->DrawText(m_iScreenX + 14,m_iScreenY + 56,"盛大通行证：",0xffb48c5a,FONT_YAHEI);
	g_pFont->DrawText(m_iScreenX + 50,m_iScreenY + 92,"密码：",0xffb48c5a,FONT_YAHEI);
}

bool CBindPTWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BINDPTWND_WND:
		{
			if (dwData == 10)
			{
				m_pOKButton->SetEnable(true);
				return true;
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pOKButton)
			{
				char user[64]={0},pwd[64]={0};
				if(m_pEditUserID && m_pEditPassword)
				{
					strcpy(user,m_pEditUserID->GetText());
					strcpy(pwd ,m_pEditPassword->GetText());
					if( strlen(user)<3 || strlen(pwd)<3 ) 
						return true;

					m_pOKButton->SetEnable(false);
					g_pGameControl->SEND_MergePT_Req(user, pwd);
				}

				return true;
			}
			else if (pControl == m_pZhuButton)
			{
				string url = g_pStreamMgr->GetWebsite("NewUser");
				ShellExecute(NULL,"open","iexplore.exe",url.c_str(),NULL,SW_SHOW);
				return true;
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}