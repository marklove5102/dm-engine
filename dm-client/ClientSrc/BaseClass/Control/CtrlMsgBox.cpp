#include "CtrlMsgBox.h"
#include "CtrlButton.h"
#include "CtrlEdit.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "Global/Interface/AudioInterface.h"
#include <Shellapi.h>
#include "GameData/LoginData.h"
#include "GameClient/SDOAInterface.h"


tagMsgBoxInfo::tagMsgBoxInfo()
{
	ZeroMemory(&m_stcOkMsg, sizeof(m_stcOkMsg));
	ZeroMemory(&m_stcCancelMsg, sizeof(m_stcCancelMsg));
	m_dwProperty	= 0;
	m_dwShowTick	= 0;
	m_dwStartTick	= 0;
	m_dwMsgLength	= 0;
	m_bModel        = true;
	m_bForbidDirtyWord = false;
	m_strMainString.clear();
	m_strCheckString.clear();
	m_strServerString.clear();
}

void tagMsgBoxInfo::operator =(const MsgBoxInfo& rf)
{
	memcpy(&m_stcOkMsg, &rf.m_stcOkMsg, sizeof(m_stcOkMsg));
	memcpy(&m_stcCancelMsg, &rf.m_stcCancelMsg, sizeof(m_stcCancelMsg));
	m_dwProperty	= rf.m_dwProperty;
	m_dwShowTick	= rf.m_dwShowTick;
	m_dwStartTick	= rf.m_dwStartTick;
	m_dwMsgLength	= rf.m_dwMsgLength;
	m_strMainString	= rf.m_strMainString;
	m_strCheckString	= rf.m_strCheckString;
	m_strEditString = rf.m_strEditString;
	m_strServerString	= rf.m_strServerString;
	m_bModel = rf.m_bModel;
	m_bForbidDirtyWord = rf.m_bForbidDirtyWord;
}

INIT_WND_POS(CCtrlMsgBox,POS_AUTO,POS_AUTO)

CCtrlMsgBox::CCtrlMsgBox(void)
:m_pOKButton(NULL),m_pCancelBt(NULL),m_pEdit(NULL),m_pCheck(NULL)
{
	m_iControlMode = CTRL_MODE_MESSAGEBOX;
	g_MsgBoxMgr.AddMsgBoxTrack(this);
	ZeroMemory(&m_rcTextPos, sizeof(m_rcTextPos));
	g_MsgBoxMgr.PopMsg(m_Info);

	m_bModel = m_Info.m_bModel;
	//使用不同的底板
	if((m_Info.m_dwProperty & MSGBOX_NEED_CANCLE_BT) ||
		(m_Info.m_dwProperty & MSGBOX_NEED_EDIT))
		 m_iIndex = 38;
	else m_iIndex = 40;	

    if(m_Info.m_dwProperty & MSGBOX_NEED_TAG)
	{
		m_npcText.Parse(m_Info.m_strMainString,0,"\\");
	}
}

CCtrlMsgBox::~CCtrlMsgBox(void)
{
	if(g_MsgBoxMgr.GetInstance() == this)
	{
		g_MsgBoxMgr.SetInstance(NULL);
	}
	g_MsgBoxMgr.RemoveMsgBoxTrack(this);
 	g_pControl->RemoveMsg(MSG_POPUP_MSG_BOX, OPER_CLOSE, this);
}

void CCtrlMsgBox::OnClickCloseButton()
{
	g_pAudio->Play(EAT_OTHER,1,g_pAudio->GetRand()++);
	g_pControl->Msg(MSG_POPUP_MSG_BOX, OPER_CLOSE, this);
}

void CCtrlMsgBox::OnCreate()
{
	int iY = 129;
	int	iXInt	= (m_Info.m_dwProperty & MSGBOX_NEED_CANCLE_BT)?54:0;
	RECT	rcEditPos = {65, 80, 320, 22};

	m_rcTextPos.left = 8;
	m_rcTextPos.top = 20;

	if((m_Info.m_dwProperty & MSGBOX_NEED_CANCLE_BT) ||
		(m_Info.m_dwProperty & MSGBOX_NEED_EDIT))
	{
		m_rcTextPos.left = 30;
		m_rcTextPos.top = 30;
	}

	m_rcTextPos.right = 380;
	m_rcTextPos.bottom = 96;

	if(m_Info.m_dwProperty & MSGBOX_NEED_TAG)
	{
		m_pMarkViewer = new CMarkViewer(16,15);
		AddControl(m_pMarkViewer);
		m_pMarkViewer->Create(this,30,16,330,130);
		m_pMarkViewer->SetTagText(&m_npcText);
	}


	int iXOK,iYOK;
	if((m_Info.m_dwProperty & MSGBOX_NEED_CANCLE_BT) ||
		(m_Info.m_dwProperty & MSGBOX_NEED_EDIT))
	{
		m_pCancelBt = new CCtrlButton();
		AddControl(m_pCancelBt);
		m_pCancelBt->CreateEx(this,232,139,1658,1659,1660);
		m_pCancelBt->SetText("取消",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);

		iXOK = 115;
		iYOK = 139;
	}
	else
	{
		iXOK = 153;
		iYOK = 139;
	}

	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,iXOK,iYOK,1658,1659,1660);
	m_pOKButton->SetText("确定",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);

	if(m_Info.m_dwProperty & MSGBOX_NEED_EDIT)
	{
		m_pEdit = new CCtrlEdit();
		AddControl(m_pEdit);
		//m_pEdit->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,rcEditPos.left, rcEditPos.top, rcEditPos.right, rcEditPos.bottom,CTRL_STYLE_BACKMODE_NODRAW);
		m_pEdit->CreateEx(this,rcEditPos.left, rcEditPos.top, rcEditPos.right, rcEditPos.bottom,0,39,39,39,39);
		m_pEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI,8);

		if(m_Info.m_dwProperty & MSGBOX_NEED_LIMITED_EDIT)
			m_pEdit->SetMaxLength(m_Info.m_dwMsgLength);

		if(m_Info.m_dwProperty & MSGBOX_NEED_DIGITAL_EDIT)
			m_pEdit->SetDigital(true);

		if(!(m_Info.m_strEditString.empty()))
			m_pEdit->SetText(m_Info.m_strEditString.c_str());

		m_rcTextPos.bottom -= 28;
	}

	if(m_Info.m_dwProperty & MSGBOX_NEED_CHECK)
	{
		m_pCheck = new CCtrlRadio();
		AddControl(m_pCheck);
		m_pCheck->Create(this, 0, 0,11137,137);
		m_pCheck->SetText(m_Info.m_strCheckString.c_str());
		m_pCheck->Move(35, iY-m_pCheck->GetHeight()-5);
		m_rcTextPos.bottom -= m_pCheck->GetHeight()-2;
	}

	int iCurSize = g_MsgBoxMgr.GetCurMsgBoxListSize()-1;
	int iDeltaSize = 20;

	int iWX = GetX(), iWY = GetY();
	if(CCtrlMsgBox* pLast = g_MsgBoxMgr.GetLastMsgBox())
	{
		iWX = pLast->GetX();
		iWY = pLast->GetY();
		iWX += iDeltaSize;
		iWY += iDeltaSize;

		if(m_pMainWnd)
		{
			iWX %= m_pMainWnd->GetWidth();
			iWY %= m_pMainWnd->GetHeight();
		}
	}
	Move(iWX, iWY);
}

bool CCtrlMsgBox::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOKButton)
			{
				OnClickOkButton();
				return true;
			}
			else if(pControl == m_pCancelBt)
			{
				OnClickCancelBt();
				return true;
			}
		}
		break;
	case MSG_CTRL_EDIT_ENTRY:
		{
			OnClickOkButton();
			return true;
		}
		break;
	case MSG_CTRL_EDIT_ESCAPE:
		{
			if(m_pCancelBt)
			{
				OnClickCancelBt();
			}
			else
			{
				OnClickOkButton();
			}
			return true;
		}
		break;
	}
	return Super::Msg(dwMsg, dwData, pControl);
}

bool CCtrlMsgBox::OnKeyDown(WORD wState, UCHAR cKey)
{
	switch(cKey)
	{
	case VK_RETURN:
		{
			OnClickOkButton();
			return true;
		}
		break;
	case VK_ESCAPE:
		{
			if(m_pCancelBt)
			{
				OnClickCancelBt();
			}
			else
			{
				OnClickOkButton();
			}
			return true;
		}
		break;
	}
	return Super::OnKeyDown(wState, cKey);
}

void CCtrlMsgBox::OnDraw()
{
	if( (m_Info.m_dwProperty & MSGBOX_NEED_TAG) == 0)
	{
		RECT	rcTextPos = m_rcTextPos;
		rcTextPos.left += m_iScreenX;
		rcTextPos.top += m_iScreenY;
		g_pFont->DrawTextScroll(rcTextPos.left, rcTextPos.top, m_Info.m_strMainString.c_str(),(m_Info.m_dwProperty & MSGBOX_NEED_CANCLE_BT)?COLOR_ALERT_TEXT:COLOR_POPUP_TEXT, rcTextPos.right, rcTextPos.bottom,0,16,0,FONT_YAHEI,FONTSIZE_SMALL);
	}
}

bool CCtrlMsgBox::IsChecked() const
{
	//if(m_pCheck)
	//{
	//	return m_pCheck->GetState()!=0;
	//}
	return false;
}

int CCtrlMsgBox::GetNum() const
{
	if(m_pEdit)
	{
		return m_pEdit->GetNum();
	}
	return 0;
}

const char* CCtrlMsgBox::GetInputString() const
{
	if(m_pEdit)
	{
		return m_pEdit->GetText();
	}
	return "";
}

char pSep[2] = {0x0C, 0};

void CCtrlMsgBox::OnClickOkButton()
{
	g_pAudio->Play(EAT_OTHER,3,g_pAudio->GetRand()++);

	g_pControl->PushMsg(MSG_POPUP_MSG_BOX, OPER_CLOSE, this);

	//如果是自动登录服务器,因为中间弹出任何对话框都是自动登录失败,除了要求输入的框,因为有可能要输入验证码
	if(g_Login.GetAutoLoginInType() > 0 && g_Login.GetAutoLoginInType() != 5 && (m_Info.m_dwProperty & MSGBOX_NEED_EDIT) == 0)
	{
		g_Login.SetAutoLoginInType(0);
		g_pControl->Msg(MSG_CTRL_SELECTCHARWND,OPER_CLOSE); //关闭角色窗口,从角色窗口可以返回登录界面
		g_pControl->Msg(MSG_CTRL_USERLOGINWND,OPER_CREATE);
		g_pNet->Close(SERVER_GAME);
		Sleep(500);
		g_pGameControl->SEND_Login_LoginInServer();
	}

	g_MsgBoxMgr.SetInstance(this);
	MSG_STRUCT* pMsg = NULL;
	pMsg = &m_Info.m_stcOkMsg;
	if( pMsg && pMsg->dwMsg)
	{
		if(pMsg->dwMsg == MSG_CTRL_MSGBOX_SERVER || pMsg->dwMsg ==MSG_CTRL_MSGBOX_SERVER_NOTNPC)
		{
			char temp[1024] = {0};
			if(m_Info.m_dwProperty & MSGBOX_NEED_EDIT)
			{
				if(m_pEdit && m_pEdit->GetText())
				{
					strcpy(temp, m_pEdit->GetText());
				}
			}

			if(!m_Info.m_strServerString.empty())
			{
				if(temp[0]) strcat(temp, pSep);
				strcat(temp, m_Info.m_strServerString.c_str());
			}

			if(strlen(temp) > 0)
			{
				if(pMsg->dwMsg == MSG_CTRL_MSGBOX_SERVER_NOTNPC)
					g_pGameControl->SEND_Messagebox_Reply(0, temp,m_Info.m_bForbidDirtyWord);
				else
					g_pGameControl->SEND_Messagebox_Reply(g_NPC.GetID(), temp,m_Info.m_bForbidDirtyWord);
			}
		}
		else if(pMsg->dwMsg != MSG_POPUP_MSG_BOX)
		{
			g_pControl->Msg(pMsg->dwMsg, pMsg->dwData, pMsg->pControl);
		}
	}
	//OnClickCloseButton();	//发送的消息可能造成本控件删除
}

void CCtrlMsgBox::OnClickCancelBt()
{
	g_pAudio->Play(EAT_OTHER,4,g_pAudio->GetRand()++);

	g_pControl->PushMsg(MSG_POPUP_MSG_BOX, OPER_CLOSE, this);

	//如果是自动登录服务器,因为中间弹出任何对话框都是自动登录失败,除了要求输入的框,因为有可能要输入验证码
	if(g_Login.GetAutoLoginInType() > 0 && g_Login.GetAutoLoginInType() != 5 && (m_Info.m_dwProperty & MSGBOX_NEED_EDIT) == 0)
	{
		g_Login.SetAutoLoginInType(0);
		g_pControl->Msg(MSG_CTRL_SELECTCHARWND,OPER_CLOSE); //关闭角色窗口,从角色窗口可以返回登录界面
		g_pControl->Msg(MSG_CTRL_USERLOGINWND,OPER_CREATE);
		g_pNet->Close(SERVER_GAME);
		Sleep(500);
		g_pGameControl->SEND_Login_LoginInServer();
	}

	g_MsgBoxMgr.SetInstance(this);
	MSG_STRUCT* pMsg = NULL;
	pMsg = &m_Info.m_stcCancelMsg;
	if(pMsg && pMsg->dwMsg)
	{
		if(pMsg->dwMsg == MSG_CTRL_MSGBOX_SERVER || pMsg->dwMsg ==MSG_CTRL_MSGBOX_SERVER_NOTNPC)
		{
			char temp[1024] = {0};
			if(m_Info.m_dwProperty & MSGBOX_NEED_EDIT)
			{
				if(m_pEdit && m_pEdit->GetText())
				{
					strcpy(temp, m_pEdit->GetText());
				}
			}

			if(!m_Info.m_strServerString.empty())
			{
				if(temp[0]) strcat(temp, pSep);
				strcat(temp, m_Info.m_strServerString.c_str());
			}

			if(pMsg->dwMsg == MSG_CTRL_MSGBOX_SERVER_NOTNPC)
				g_pGameControl->SEND_Messagebox_Reply(0, temp,m_Info.m_bForbidDirtyWord);
			else
				g_pGameControl->SEND_Messagebox_Reply(g_NPC.GetID(), temp,m_Info.m_bForbidDirtyWord);
		}
		else if(pMsg->dwMsg != MSG_POPUP_MSG_BOX)
		{
			g_pControl->Msg(pMsg->dwMsg, pMsg->dwData, pMsg->pControl);
		}
	}
}

DWORD CCtrlMsgBox::GetProperty() const
{
	return m_Info.m_dwProperty;
}

bool CCtrlMsgBox::OnLeftButtonUp(int iX, int iY)
{
	if(m_Info.m_dwProperty & MSGBOX_NEED_TAG)
	{
		string strCommand = m_pMarkViewer->GetCommand();

		if(!strCommand.empty())
		{
			//up 音乐
			g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);

			if(strCommand.substr(0,9) == "@@OpenUrl")//打开网页
			{
				string strUrl = strCommand.substr(10,strCommand.length() - 10);
				ShellExecute(NULL,"open","iexplore.exe",strUrl.c_str(),NULL,SW_SHOW);
			}
			else if(strCommand.substr(0,5) == "@@url")//打开网页
			{
				string strUrl = strCommand.substr(5,strCommand.length() - 5);
				ShellExecute(NULL,"open","iexplore.exe",strUrl.c_str(),NULL,SW_SHOW);
			}
			else if(strCommand == "@@NoYuanBao")//元宝不足
			{
				if (!g_strChannelName.empty())
				{
					char szUrl[512] = {0};
					sprintf(szUrl, "%s?channelname=%s&userid=%s&userfrom=%s&area=%d"
						, g_pStreamMgr->GetWebsite("ChannelPayURL","http://act.ws.sdo.com/pay/pay.asp")
						, g_strChannelName.c_str(), g_Login.GetThirdUserID().c_str(), g_Login.GetThirdChannel().c_str(), g_Login.GetInnerAreaNo());
					ShellExecute(NULL,"open","iexplore.exe",szUrl,NULL,SW_SHOW);
				}
				else if(g_pSDOAInterface)
				{
					if(SDOA_FALSE == g_pSDOAInterface->OpenWidget("sdoNewPay"))
					{
						char szDefalutUrl[128] = {0};
						sprintf(szDefalutUrl,"%s%u","https://pay.sdo.com/Deposit/HomePage.aspx?sel_game=",WS_GAME_ID);

						string url = g_pStreamMgr->GetWebsite("PayURL",szDefalutUrl);
						ShellExecute(NULL,"open","iexplore.exe",url.c_str(),NULL,SW_SHOW);
					}
				}
				else
				{			
					char szDefalutUrl[128] = {0};
					sprintf(szDefalutUrl,"%s%u","https://pay.sdo.com/Deposit/HomePage.aspx?sel_game=",WS_GAME_ID);

					string url = g_pStreamMgr->GetWebsite("PayURL",szDefalutUrl);
					ShellExecute(NULL,"open","iexplore.exe",url.c_str(),NULL,SW_SHOW);
				}
			}

			return true;
		}
	}

	return CCtrlWindowS::OnLeftButtonUp(iX,iY);
}

bool CCtrlMsgBox::SetFocus()
{
	if((m_Info.m_dwProperty & MSGBOX_NEED_EDIT) && m_pEdit)
	{
		//if(g_pControl->GetFocusCtrl() != m_pEdit)
		{
			return (m_pEdit->SetFocus());
		}
	}

	return Super::SetFocus();
}
