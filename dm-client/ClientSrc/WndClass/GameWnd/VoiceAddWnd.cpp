#include "VoiceWnd.h"
#include "GameData/MsgBoxMgr.h"
#include "GameClient/VoiceAdapter.h"
#include "GameData/TalkMgr.h"

#ifdef _CHAT

INIT_WND_POSX(CVoiceAddWnd,POS_AUTO,POS_AUTO)

CVoiceAddWnd::CVoiceAddWnd(void)
{
	m_iType = sm_dwWindowType;
	m_pConfirmButton = NULL;
	m_pCancelButton = NULL;
	m_pGetDownButton = NULL;
	m_pEditName = NULL;                 
	m_pEditPassword = NULL;
	m_pEditConfirmPassword = NULL;                  

	m_iIndex = 14001;
}

CVoiceAddWnd::~CVoiceAddWnd(void)
{

}

void CVoiceAddWnd::OnCreate()
{
	SetCloseButton(190,2,80);

	m_pGetDownButton = new CCtrlMenuButton();
	AddControl(m_pGetDownButton);
	m_pGetDownButton->Create(this,80,155,115,120,0,39,39,39,39,1030,1031,1032,1033,1,22,false,0,2);
	m_pGetDownButton->SetMenuTextColor(0xFF00FF00);
	m_pGetDownButton->ResetContent();
	m_pGetDownButton->SetFont(FONT_YAHEI);

	m_pGetDownButton->AddString("15");
	m_pGetDownButton->AddString("20");
	m_pGetDownButton->AddString("35");
	m_pGetDownButton->AddString("50");
	m_pGetDownButton->AddString("60");
	m_pGetDownButton->AddString("200");
	m_pGetDownButton->SetCurSel(5);

	m_pConfirmButton = new CCtrlButton();
	AddControl(m_pConfirmButton);
	m_pConfirmButton->CreateEx(this, 30,200, 95, 96, 97, 98);
	m_pConfirmButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this, 115,200, 95, 96, 97, 98);
	m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	//频道名称
	m_pEditName = new CCtrlEdit;
	AddControl(m_pEditName);
	m_pEditName->CreateEx(this,80,52,114,22,0,39,39,39,39);
	m_pEditName->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pEditName->SetMaxLength(16);

	//密码
	m_pEditPassword = new CCtrlEdit;
	AddControl(m_pEditPassword);
	m_pEditPassword->CreateEx(this,80,86,114,22,0,39,39,39,39);
	m_pEditPassword->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pEditPassword->SetPwd(true);
	m_pEditPassword->SetMaxLength(15);

	//确认密码
	m_pEditConfirmPassword = new CCtrlEdit;
	AddControl(m_pEditConfirmPassword);
	m_pEditConfirmPassword->CreateEx(this,80,121,114,22,0,39,39,39,39);
	m_pEditConfirmPassword->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pEditConfirmPassword->SetPwd(true);
	m_pEditConfirmPassword->SetMaxLength(15);

}

void CVoiceAddWnd::OnClickCloseButton()
{
	CloseWindow();
}

bool CVoiceAddWnd::OnLeftButtonDown(int iX,int iY)
{
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

void CVoiceAddWnd::Draw()
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 109,m_iScreenY + 5,"创建语聊频道",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
	g_pFont->DrawText(m_iScreenX + 21,m_iScreenY + 55,"频道名称:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 21,m_iScreenY + 89,"设置密码:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 21,m_iScreenY + 124,"确认密码:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 21,m_iScreenY + 158,"频道人数:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
}

bool CVoiceAddWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if (pControl == m_pGetDownButton)
		{
			return true;
		}
		else if (pControl == m_pConfirmButton)
		{
			char User[32] = {0},Pwd[32] = {0},ConfirmPwd[32] = {0},Num[8] = {0};
			BYTE RoomNum = 0;
			if (m_iType == 0)
			{
				strcpy(User,m_pEditName->GetText());
				if (strncmp(User,"[行会]",6) == 0)
				{
					g_TalkMgr.PushSysTalk("公共大厅不能创立带[行会]标志的房间！");
					return true;
				}
			}
			else
				sprintf(User,"%s%s","[行会]",m_pEditName->GetText());



			strcpy(Pwd ,m_pEditPassword->GetText());
			strcpy(ConfirmPwd,m_pEditConfirmPassword->GetText());
			strcpy(Num,m_pGetDownButton->GetText());
			RoomNum = (BYTE)(atoi(Num));
			//发送创建语聊消息
			BYTE bHasPassword = 0;

			if(strcmp(User,"")&& RoomNum!=0 && (strcmp(ConfirmPwd,Pwd) == 0))
			{
				if (strcmp(Pwd,""))
					bHasPassword = 1;

				if(g_pRoomManager)
					g_pRoomManager->CreateRoom( User
					,""					//房间说明
					,RoomNum			//限制人数
					,bHasPassword	
					,Pwd);

				CloseWindow();
				return true;
			}
			else
			{
				g_MsgBoxMgr.PopSimpleAlert("请填写完整创建信息或两次密码不一致");
			}
			return true;
		}
		else if (pControl == m_pCancelButton)
		{
			CloseWindow();
			return true;
		}
		break;
	default:
		break;
	}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
#endif
