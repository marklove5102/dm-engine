#include "VoiceWnd.h"
#include "GameData/GameDefine.h"
#include "GameClient/VoiceAdapter.h"
#include "GameData/TalkMgr.h"
#include "Global/Interface/StreamInterface.h"
#include "BaseClass/Control/CtrlMenuWnd.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameClient/WidgetManager.h"
#include "GameClient/SDOAInterface.h"
#include "GameData/MsgBoxMgr.h"

#ifdef _CHAT

//--------------------------------
INIT_WND_POSX(CVoiceChatWnd,POS_VARIABLE,POS_VARIABLE)

CVoiceChatWnd::CVoiceChatWnd(void)
{
	m_pLeaveButton = NULL;//离开按钮
	//m_pForbidButton = NULL;//禁言按钮
	m_pConfigButton = NULL;//设置拉按钮
	m_pSendButton = NULL;//发送按钮

	m_pAddVoiceButton = NULL;//2套声音控制按钮
	m_pDecVoiceButton = NULL;
	m_pAddMicButton = NULL;
	m_pDecMicButton = NULL;
	m_pSendEdit = NULL; //编辑信息 

	m_iIndex = 13990;

	if(g_pControl->GetWindowPos(m_iOffX,m_iOffY,"VoiceWnd"))
	{
		m_iAlignType = XAL_TOPLEFT;
	}
	else
	{
		m_iOffX = m_iOffY = 0;
		m_iAlignType = XAL_RIGHT;
	}
	m_bNeedSavePos = false;

	g_VoiceAdapter.RequestUserList();

	PostMessage(g_hWnd,MSG_IGW_WIDGET,APPID_VOICE_CHAT,HOSTAPP_OPEN);


	m_iVersion = 2;
	AddTabPage(0,0,MAKELONG(13990,PACKAGE_INTERFACE),35,36,120,121,122,123,"公共大厅",NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(13990,PACKAGE_INTERFACE),107,36,120,121,122,123,"行会大厅",NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(13990,PACKAGE_INTERFACE),180,36,145,146,147,148,g_VoiceAdapter.GetCurRoomName(),NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	m_TabPage.iCurPage = 2;
}

CVoiceChatWnd::~CVoiceChatWnd(void)
{
	g_TalkMgr.Clear(TALKTYPE_VOICE);
	g_pStreamMgr->SetConfigInt("SpeakVol",g_VoiceAdapter.GetSpeakVol());
	g_pStreamMgr->SetConfigInt("ListenVol",g_VoiceAdapter.GetMicVol());
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"VoiceWnd");
}


void CVoiceChatWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();
	SetCloseButton(325,4,80);

	g_VoiceAdapter.SetSpeakWay(g_VoiceAdapter.GetSpeakWay(),g_VoiceAdapter.GetSpeakVoiceActiLevel());	//设置语音采集模式,1为持续发言,2为按键激活,第二个参数是键位,3为语音激活(当声音大于某个强度时,传输声音) 
	if(g_pSpeak)
		g_pSpeak->StartTalk(0);

	int iSpeakVolume = g_pStreamMgr->GetConfigInt("SpeakVol",g_VoiceAdapter.GetSpeakVol());
	int iListenVolume = g_pStreamMgr->GetConfigInt("ListenVol",g_VoiceAdapter.GetMicVol());
	if(iSpeakVolume <= 0)
		iSpeakVolume = 12;
	if(iListenVolume <= 0)
		iListenVolume = 12;

	if(g_pSpeak)
		g_VoiceAdapter.SetMicVol(iSpeakVolume);
	if(g_pSpeak)
		g_VoiceAdapter.SetSpeakVol(iSpeakVolume);

	m_pLeaveButton = new CCtrlButton();
	AddControl(m_pLeaveButton);
	m_pLeaveButton->CreateEx(this, 31,354, 90, 91, 92, 93);
	m_pLeaveButton->SetText("离开频道", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	//m_pForbidButton = new CCtrlButton();
	//AddControl(m_pForbidButton);
	//m_pForbidButton->CreateEx(this,113,328,14033,14034,14035,14036);
	//m_pForbidButton->SetEnable(false);

	m_pSendButton = new CCtrlButton();
	AddControl(m_pSendButton);
	m_pSendButton->CreateEx(this, 276,302, 95, 96, 97, 98);
	m_pSendButton->SetText("发送", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pConfigButton = new CCtrlButton();
	AddControl(m_pConfigButton);
	m_pConfigButton->CreateEx(this, 242,354, 90, 91, 92, 93);
	m_pConfigButton->SetText("语音设置", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pAddVoiceButton = new CCtrlButton();
	AddControl(m_pAddVoiceButton);
	m_pAddVoiceButton->CreateEx(this,140,178,200,201,202,203);

	m_pAddMicButton = new CCtrlButton();
	AddControl(m_pAddMicButton);
	m_pAddMicButton->CreateEx(this,296,178,200,201,202,203);

	m_pDecVoiceButton = new CCtrlButton();
	AddControl(m_pDecVoiceButton);
	m_pDecVoiceButton->CreateEx(this,59,178,205,206,207,208);

	m_pDecMicButton = new CCtrlButton();
	AddControl(m_pDecMicButton);
	m_pDecMicButton->CreateEx(this,214,178,205,206,207,208);

	m_pSendEdit = new CCtrlEdit;
	AddControl(m_pSendEdit);
	m_pSendEdit->CreateEx(this,30,300,240,22,0,39,39,39,39);
	m_pSendEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pSendEdit->SetMaxLength(100);
	m_pSendEdit->SetPrompt("请输入文字聊天信息：");

	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
	m_pGrid->Create(this,28,71,329,171,5,20.0f);
	m_pGrid->AddScrollEx(286,0,16,100);

	m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);
	m_pGrid->SetTextColor(0xFF00FFFF,0xFF009696);
	m_pGrid->SetSelTextColor(0xFF666600);
	m_pGrid->SetDrawOffXY(0,0);

	m_pGrid->ClearColumn();
	m_pGrid->PushColumn(8);
	m_pGrid->PushColumn(10);
	m_pGrid->PushColumn(12);
	m_pGrid->PushColumn(100);
	m_pGrid->PushColumn(30);
	m_pGrid->PushColumn(35);

	m_pTalkViewer = new CTalkViewer();
	AddControl(m_pTalkViewer);
	m_pTalkViewer->Create(this,28,210,329,81);
	m_pTalkViewer->AddScrollEx(286,0,16,81);
	m_pTalkViewer->SetTalkType(&g_TalkMgr.GetVoiceTalk());
}


void CVoiceChatWnd::OnClickCloseButton()
{
	g_pStreamMgr->SetConfigInt("SpeakVol",g_VoiceAdapter.GetSpeakVol());
	g_pStreamMgr->SetConfigInt("ListenVol",g_VoiceAdapter.GetMicVol());

	PostMessage(g_hWnd,MSG_IGW_WIDGET,APPID_VOICE_CHAT,HOSTAPP_CLOSE);

	CloseWindow();
}

void CVoiceChatWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 181,m_iScreenY + 11,"语音聊天",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	//绘制音量设置表
	int iSpeakVol = g_VoiceAdapter.GetSpeakVol();
	int iMicVol = g_VoiceAdapter.GetMicVol();

	m_pAddVoiceButton->SetEnable(iSpeakVol != VOL_MAX);
	m_pDecVoiceButton->SetEnable(iSpeakVol != 0);
	m_pAddMicButton->SetEnable(iMicVol != VOL_MAX);
	m_pDecMicButton->SetEnable(iMicVol != 0);

	LPTexture pVolume = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,14053,EP_UI);
	if(pVolume)
	{
		for(int i = 0; i < iSpeakVol; i++)
			g_pGfx->DrawTextureNL(m_iScreenX+80+i*5,m_iScreenY+180,pVolume);

		for(int i = 0; i < iMicVol; i++)
			g_pGfx->DrawTextureNL(m_iScreenX+232+i*5,m_iScreenY+180,pVolume);
	}

	g_pGfx->DrawTextureNL(m_iScreenX+39,m_iScreenY+178,g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,14019,EP_UI));
	g_pGfx->DrawTextureNL(m_iScreenX+199,m_iScreenY+180,g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,14020,EP_UI));
	//绘制用户列表
	if(m_pGrid)
	{
		UserVector& VUser = g_VoiceAdapter.GetUserArray();
		m_pGrid->SetTotalCount(VUser.size());
		for(int i = 0;i < m_pGrid->GetLinesPerPage();i++)
		{
			int iIdx = m_pGrid->GetDisLine() + i;
			if(iIdx >= VUser.size())
				continue;

			UserInfo &user = VUser.at(iIdx);
			if(user.pUser == NULL)
				continue;

			m_pGrid->DrawGrid(i,0,user.pUser->IsGameUser()?" ":"*");


			m_pGrid->DrawGrid(i,1,PACKAGE_INTERFACE,user.pUser->GetUserSex() == 0 ? 14065:14066);			

			if(user.bSpeaking)
				m_pGrid->DrawGrid(i,2,PACKAGE_INTERFACE,14064);			

			m_pGrid->DrawGrid(i,3,user.pUser->GetUserName());			

			if(user.pUser->IsMySpeaker())
				m_pGrid->DrawGrid(i,4,"接收");
			else
				m_pGrid->DrawGrid(i,4,"拒听");

			if(user.pUser->GetUserType() == 1 || user.pUser->GetUserType() == 2)
				m_pGrid->DrawGrid(i,5,"管理员");
			else
				m_pGrid->DrawGrid(i,5," ");

		}
	}


	//画聊天状态指示条
	int iSpeakLevel = g_VoiceAdapter.GetSpeakLevel();
	int iMicLevel = g_VoiceAdapter.GetMicLevel();

	g_pGfx->DrawFillRect(m_iScreenX + 61,m_iScreenY+198,iSpeakLevel*90/VOL_LEVEL_MAX,2,0xFFE0C000);
	if(iMicLevel > 15)
	{
		g_pGfx->DrawFillRect(m_iScreenX + 220,m_iScreenY+198,iMicLevel*90/VOL_LEVEL_MAX,2,0xFFE0C000);
	}
}

bool CVoiceChatWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_TABPAGE_TABCHANGE)
	{
		if(dwData == 0)
		{
			g_VoiceAdapter.RequestRoom();
			CloseWindow();
			g_pControl->PopupWindow(MSG_CTRL_VOICE_WND,OPER_CREATE); //公共房间列表
			return true;
		}
		else if (dwData == 1)
		{
			//g_VoiceAdapter.RequestRoom(); //重新请求房间列表();
			RoomVector& RoomList = g_VoiceAdapter.GetGuildRoomArray();
			if (RoomList.size() == 0)
			{
				g_TalkMgr.PushSysTalk("你没有行会房间！");
				return true;
			}
			else
			{
				g_pControl->PopupWindow(MSG_CTRL_VOICE_CHAT_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_VOICE_WND,OPER_CREATE,1);
				return true;
			}
		}

		return true;
	}
	else if(dwMsg == MSG_CTRL_TALKVIEW_LCLICK)
	{
		string strLine;
		if(m_pTalkViewer->getLineHeader(strLine))
		{
			m_pSendEdit->SetText(strLine.c_str() + 1);
			m_pSendEdit->SetFocus();
		}
		return true;
	}
	else if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pAddVoiceButton)
		{
			g_VoiceAdapter.IncSpeakVol(1);
			return true;
		} 
		else if (pControl == m_pConfigButton)
		{
			g_pControl->PopupWindow(MSG_CTRL_VOICE_CONFIG_WND,OPER_CREATE);
			return true;
		}
		else if(pControl == m_pAddMicButton)
		{
			g_VoiceAdapter.IncMicVol(1);
			return true;
		} 
		else if(pControl == m_pDecVoiceButton)
		{
			g_VoiceAdapter.IncSpeakVol(-1);
			return true;
		} 
		else if(pControl == m_pDecMicButton)
		{
			g_VoiceAdapter.IncMicVol(-1);

			return true;
		} 
		else if (pControl == m_pLeaveButton)
		{
			g_VoiceAdapter.LeaveRoom();

			g_pStreamMgr->SetConfigInt("SpeakVol",g_VoiceAdapter.GetSpeakVol());
			g_pStreamMgr->SetConfigInt("ListenVol",g_VoiceAdapter.GetMicVol());

			return true;
		}
		//else if (pControl == m_pForbidButton)
		//{
		//	//房主特权
		//	int iSelUser = m_pGrid->GetSelLine();
		//	if(g_pRoomManager && iSelUser >= 0 && iSelUser < m_pGrid->GetTotalCount())
		//	{
		//		UserVector& VUser = g_VoiceAdapter.GetUserArray();
		//		IUser* pUser = VUser.at(iSelUser).pUser;

		//		if (pUser && pUser->IsForbided())
		//		{
		//			g_pRoomManager->ForbidSpeak(pUser,true,0,"NoReason");
		//			m_pForbidButton->ReloadTex(14033,14034,14035);
		//		}
		//		else
		//		{
		//			g_pRoomManager->ForbidSpeak(pUser,false,0,"NoReason");
		//			m_pForbidButton->ReloadTex(14006,14007,14008);
		//		}
		//	}
		//	return true;
		//}
		else if(pControl == m_pSendButton)
		{
			const char* szChat = m_pSendEdit->GetText();
			if(strlen(szChat) != 0)
			{
				//g_TalkMgr.PushTalk(TALKTYPE_VOICE,szChat,TALKCOLOR_BLUE); //加入语聊的话
				g_VoiceAdapter.SendTextMessage(szChat,strlen(szChat) + 1,NULL);
				m_pSendEdit->Clear();
			}
			m_pSendEdit->SetFocus();
			return true;
		}
	}
	else if(dwMsg == MSG_CTRL_EDIT_ENTRY)
	{
		this->Msg(MSG_CTRL_BUTTON_CLICK,0,m_pSendButton);
		m_pSendEdit->SetFocus();
		return true;
	}
	else if(dwMsg == MSG_CTRL_GRID_RCLICK_LINE_COL)
	{
		OnRightBtnClick(LOWORD(dwData));
		return true;
	}
	else if(dwMsg == MSG_CTRL_POP_MENU)
	{
		switch(dwData)
		{
		case POP_MENU_ITEM_CHANGE_PASWORD:
			OnChangePasword();
			break;
		case POP_MENU_ITEM_KICK_OUT:
			OnKickOut();
			break;
		case POP_MENU_ITEM_FORBID_ONE_SPEAK:
			OnForbidOne();
			break;
		case POP_MENU_ITEM_ALLOW_ONE_SPEAK:
			OnAllowOne();
			break;
		case POP_MENU_ITEM_RECEIVE_ONE_VOICE:
			OnReceiveOne();
			break;
		case POP_MENU_ITEM_REJECT_ONE_VOICE:
			OnRejectOne();
			break;
		case POP_MENU_ITEM_ALLOW_ONE_HEAR_ME:
			OnAllowOneHearMe();
			break;
		case POP_MENU_ITEM_REJECT_ONE_HEAR_ME:
			OnRejectOneHearMe();
			break;
		case POP_MENU_ITEM_RECEIVE_ALL_VOICE:
			OnReceiveAll();
			break;
		case POP_MENU_ITEM_REJECT_ALL_VOICE:
			OnRejectAll();
			break;
		case POP_MENU_ITEM_FORBID_ALL:
			OnForbidAll();
			break;
		case POP_MENU_ITEM_ALLOW_ALL:
			OnAllowAll();
			break;
		default:
			break;
		}

		return true;
	}




	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CVoiceChatWnd::OnRightBtnClick(int iRow)
{
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iRow < 0 || iRow >= VUser.size())
		return;
	IUser*  pUser = VUser[iRow].pUser;
	if(!pUser)
		return;

	CCtrlMenuWnd* pMenuWnd = g_pControl->GetMenuWnd();
	pMenuWnd->Clear();
	S_POP_MENU_DATA & PopMenuData = pMenuWnd->GetData();
	PopMenuData.fItemHeight = 16.0f;
	PopMenuData.iW = 125;


	if (g_VoiceAdapter.GetUserType() != 1 && g_VoiceAdapter.GetUserType() != 2)
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANGE_PASWORD,"修改密码","",false);
	else
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANGE_PASWORD,"修改密码");

	if (g_VoiceAdapter.GetUserType() != 1 && g_VoiceAdapter.GetUserType() != 2)
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_KICK_OUT,"踢出房间","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_KICK_OUT,"踢出房间");

	if (g_VoiceAdapter.GetUserType() != 1 && g_VoiceAdapter.GetUserType() != 2)
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_FORBID_ONE_SPEAK,"禁言该用户","",false);
	else if (pUser->IsForbided())
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_FORBID_ONE_SPEAK,"禁言该用户","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_FORBID_ONE_SPEAK,"禁言该用户");


	if (g_VoiceAdapter.GetUserType() != 1 && g_VoiceAdapter.GetUserType() != 2)
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_ALLOW_ONE_SPEAK,"解禁该用户","",false);
	else if (!pUser->IsForbided())
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_ALLOW_ONE_SPEAK,"解禁该用户","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_ALLOW_ONE_SPEAK,"解禁该用户");

	if (pUser->IsMySpeaker())
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_RECEIVE_ONE_VOICE,"接收该用户语音","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_RECEIVE_ONE_VOICE,"接收该用户语音");


	if (!pUser->IsMySpeaker())
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_REJECT_ONE_VOICE,"拒收该用户语音","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_REJECT_ONE_VOICE,"拒收该用户语音");


	if (pUser->IsMyReader())
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_ALLOW_ONE_HEAR_ME,"允许该用户收听我","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_ALLOW_ONE_HEAR_ME,"允许该用户收听我");


	if (!pUser->IsMyReader())
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_REJECT_ONE_HEAR_ME,"拒绝该用户收听我","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_REJECT_ONE_HEAR_ME,"拒绝该用户收听我");


	pMenuWnd->AddMenuItem(POP_MENU_ITEM_RECEIVE_ALL_VOICE,"接收所有语音");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_REJECT_ALL_VOICE,"拒收所有语音");


	if (g_VoiceAdapter.GetUserType() != 1 && g_VoiceAdapter.GetUserType() != 2)
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_FORBID_ALL,"禁言所有用户","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_FORBID_ALL,"禁言所有用户");

	if (g_VoiceAdapter.GetUserType() != 1 && g_VoiceAdapter.GetUserType() != 2)
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_ALLOW_ALL,"解禁所有用户","",false);
	else 
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_ALLOW_ALL,"解禁所有用户");



	m_bClick = false;
	m_bRBClick = false;//否则MenuWnd获得不到焦点
	pMenuWnd->SetCaller(this);
	pMenuWnd->ShowMenu();
	g_pControl->GetTipWnd()->SetShow(false);

}

void CVoiceChatWnd::OnChangePasword()
{
	//g_pControl->Msg(MSG_CTRL_EDITMESSAGEBOX,MS_OK_CANCEL_EDIT,(CControl*)MSG_CTRL_INPUT_NEW_PASWORD);
	g_MsgBoxMgr.PopEditBox("请输入新密码",MSG_CTRL_INPUT_NEW_PASWORD,0);
}

void CVoiceChatWnd::OnKickOut()
{
	int iSelUser = m_pGrid->GetSelLine();
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iSelUser < 0 || iSelUser >= VUser.size())
		return;

	if (VUser[iSelUser].pUser == g_pRoomManager->GetMyUser())
	{
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"请不要对自己做此项操作！",TALKCOLOR_BLUE);
		return;
	}

	g_pRoomManager->KickUser(VUser[iSelUser].pUser,"Kick");
}

void CVoiceChatWnd::OnForbidOne()
{
	int iSelUser = m_pGrid->GetSelLine();
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iSelUser < 0 || iSelUser >= VUser.size())
		return;


	if (VUser[iSelUser].pUser == g_pRoomManager->GetMyUser())
	{
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"请不要对自己做此项操作！",TALKCOLOR_BLUE);
		return;
	}

	g_pRoomManager->ForbidSpeak(VUser[iSelUser].pUser,false,0,"Forbid");
}

void CVoiceChatWnd::OnAllowOne()
{
	int iSelUser = m_pGrid->GetSelLine();
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iSelUser < 0 || iSelUser >= VUser.size())
		return;

	if (VUser[iSelUser].pUser == g_pRoomManager->GetMyUser())
	{
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"请不要对自己做此项操作！",TALKCOLOR_BLUE);
		return;
	}
	g_pRoomManager->ForbidSpeak(VUser[iSelUser].pUser,true,0,"Allow");
}

void CVoiceChatWnd::OnReceiveOne()
{
	int iSelUser = m_pGrid->GetSelLine();
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iSelUser < 0 || iSelUser >= VUser.size())
		return;


	if (VUser[iSelUser].pUser == g_pRoomManager->GetMyUser())
	{
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"请不要对自己做此项操作！",TALKCOLOR_BLUE);
		return;
	}

	g_pRoomManager->SpeakON(VUser[iSelUser].pUser);

}

void CVoiceChatWnd::OnRejectOne()
{
	int iSelUser = m_pGrid->GetSelLine();
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iSelUser < 0 || iSelUser >= VUser.size())
		return;


	if (VUser[iSelUser].pUser == g_pRoomManager->GetMyUser())
	{
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"请不要对自己做此项操作！",TALKCOLOR_BLUE);
		return;
	}
	g_pRoomManager->SpeakOFF(VUser[iSelUser].pUser);
}

void CVoiceChatWnd::OnReceiveAll()
{
	g_pRoomManager->SpeakON(NULL);
}

void CVoiceChatWnd::OnRejectAll()
{
	g_pRoomManager->SpeakOFF(NULL);
}

void CVoiceChatWnd::OnForbidAll()
{
	g_pRoomManager->ForbidSpeak(NULL,false,0,"ForbidAll");
}

void CVoiceChatWnd::OnAllowAll()
{
	g_pRoomManager->ForbidSpeak(NULL,true,0,"AllowAll");
}

void CVoiceChatWnd::OnAllowOneHearMe()
{
	int iSelUser = m_pGrid->GetSelLine();
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iSelUser < 0 || iSelUser >= VUser.size())
		return;

	if (VUser[iSelUser].pUser == g_pRoomManager->GetMyUser())
	{
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"请不要对自己做此项操作！",TALKCOLOR_BLUE);
		return;
	}

	g_pRoomManager->AddReader(VUser[iSelUser].pUser);
}

void CVoiceChatWnd::OnRejectOneHearMe()
{
	int iSelUser = m_pGrid->GetSelLine();
	UserVector &VUser = g_VoiceAdapter.GetUserArray();
	if(iSelUser < 0 || iSelUser >= VUser.size())
		return;


	if (VUser[iSelUser].pUser == g_pRoomManager->GetMyUser())
	{
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"请不要对自己做此项操作！",TALKCOLOR_BLUE);
		return;
	}
	g_pRoomManager->RemoveReader(VUser[iSelUser].pUser);

}




#endif