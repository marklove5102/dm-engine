#include "VoiceWnd.h"
#include "GameData/MsgBoxMgr.h"
#include "GameClient/VoiceAdapter.h"

#ifdef _CHAT
INIT_WND_POSX(CVoicePasswordWnd,POS_AUTO,POS_AUTO)

CVoicePasswordWnd::CVoicePasswordWnd(void)
{
	m_pConfirmButton = NULL;
	m_pCancelButton = NULL;
	m_pPasswordEdit = NULL;
	m_nRoomNum = (int)sm_dwWindowType;
	m_iIndex = 14055;
	m_iType = 0;
}

CVoicePasswordWnd::~CVoicePasswordWnd(void)
{
}

void CVoicePasswordWnd::OnCreate()
{
	SetCloseButton(236,1,80);

	m_pConfirmButton = new CCtrlButton();
	AddControl(m_pConfirmButton);
	m_pConfirmButton->CreateEx(this, 40,135, 95, 96, 97, 98);
	m_pConfirmButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this, 150,135, 95, 96, 97, 98);
	m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pPasswordEdit = new CCtrlEdit;
	AddControl(m_pPasswordEdit);
	m_pPasswordEdit->CreateEx(this,93,95,127,22);//,0,39,39,39,39);
	m_pPasswordEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pPasswordEdit->SetMaxLength(16);
	m_pPasswordEdit->SetFocus();
}

void CVoicePasswordWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 130,m_iScreenY + 6,"进入语音频道",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
	g_pFont->DrawText(m_iScreenX + 27,m_iScreenY + 47,"语音频道被创建者设置了密码限制,需",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 27,m_iScreenY + 69,"要输入正确的密码才能进入",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 34,m_iScreenY + 100,"频道密码:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
}


bool CVoicePasswordWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pConfirmButton && m_pPasswordEdit->GetText() != NULL)
		{
			string strPasword = m_pPasswordEdit->GetText();
			if(strPasword.empty())
			{
				g_MsgBoxMgr.PopSimpleAlert("无法进入所选择的频道，你输入的密码不正确。");
				return true;
			}
			else
			{
				g_VoiceAdapter.EnterRoom(m_nRoomNum,strPasword.c_str(),m_iType);
			}
			CloseWindow(TRUE);
			return true;
		}
		else if(pControl == m_pCancelButton) 
		{
			CloseWindow();
			return true;
		}
	}
	else if(dwMsg == MSG_CTRL_VOICE_PASSWORD_WND)
	{
		m_iType = (int)dwData;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
#endif