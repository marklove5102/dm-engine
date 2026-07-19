#include "TeamWnd.h"
#include "GameMap/GameMap.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/GameData.h"
#include "GameClient/VoiceAdapter.h"
#include "GameData/OtherData.h"
#include "GameAI/AIConfigMgr.h"
#include "GameData/TalkMgr.h"
#include "GameData/TaskData.h"

INIT_WND_POSX(CTeamWnd,POS_AUTO,POS_AUTO)

CTeamWnd::CTeamWnd(void)
{
	m_pEnableTeamButton = NULL;
	m_pAddButton		= NULL;
	m_pDeleteButton		= NULL;	
	//m_pVolumeTex = NULL;
	m_pVoiceControlButton = NULL;

	m_nWhichPerson = -1;
	m_nPage = 0;
	m_nNumPersons = 0;
	m_dwLastLeaveOrEnterRoomTime = 0;

#ifdef _CHAT
	m_bInTeamRoom = g_VoiceAdapter.IsInTeamRoom();
#else
	m_bInTeamRoom = false;
#endif

	CSimpleCharacterNode * pChar = NULL;
	int iSelfX,iSelfY;
	int i,j;
	int nBx,nBy,nEx,nEy;
	int nMapW = g_pGameMap->GetWidth();
	int nMapH = g_pGameMap->GetHeight();

	SELF.GetXY(iSelfX, iSelfY); 
	nBx = max(0, iSelfX-5);
	nEx = min(iSelfX+5, nMapW);
	nBy = max(0, iSelfY-5);
	nEy = min(iSelfY+5, nMapH);

	for( i=nBx; i<nEx; i++ )
	{
		for( j=nBy; j<nEy; j++ )
		{
			pChar = MapArray.GetSimpleCharacterHead( i, j );
			while( pChar )
			{
				if( pChar->IsHuman())
				{
					m_vecNearPersons.push_back(pChar->GetName());
					m_nNumPersons++;
				}
				pChar = pChar->m_MapNext;
			}
		}
	}

	m_iIndex = 16060;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

	//for (int i = 0;i<10;i++)
	//{
	//	m_vecNearPersons.push_back("ahahaahah");
	//	g_pGameData->AddTroopMember("HahaaH");
	//}
}

CTeamWnd::~CTeamWnd(void)
{	
	m_vecNearPersons.clear();	
	//g_pTexMgr->ReleaseTex(m_pVolumeTex);
}

void CTeamWnd::OnCreate()
{
	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_FLASH_BTN,1004);//停止按钮闪烁
	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_REMOVE_ARROWTIP_CONTROL,EP_First_Team_Step1);


#ifdef _CHAT
	if(!g_VoiceAdapter.IsConnected())
		g_VoiceAdapter.Init();
#endif
	
	SetCloseButton(373,5,80);

	m_pNeedGroupGrid = new CCtrlGrid;
	m_pNeedGroupGrid->CreateEx(this,27,51,170,238,8,21,17665,0,0.74f);
	AddControl(m_pNeedGroupGrid);
	m_pNeedGroupGrid->PushColumn(125);
	m_pNeedGroupGrid->SetDrawOffXY(0,26);
	m_pNeedGroupGrid->SetFont(FONT_YAHEI);

	m_pGroupedGrid = new CCtrlGrid;
	m_pGroupedGrid->CreateEx(this,238,51,380,238,9,21,17665,0,0.74f);
	AddControl(m_pGroupedGrid);
	m_pGroupedGrid->SetDrawOffXY(0,5);
	m_pGroupedGrid->PushColumn(125);
	m_pGroupedGrid->SetFont(FONT_YAHEI);

	m_pAddButton = new CCtrlButton();
	AddControl(m_pAddButton);
	m_pAddButton->CreateEx(this,174,112,95,96,97,98);
	m_pAddButton->SetFont(FONT_YAHEI,14);
	m_pAddButton->SetText("添 加",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_DISABLE);

	m_pDeleteButton = new CCtrlButton();
	AddControl(m_pDeleteButton);
	m_pDeleteButton->CreateEx(this,174,155,95,96,97,98);
	m_pDeleteButton->SetText("删 除",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_DISABLE);

	m_pEnableTeamButton = new CCtrlRadio();
	AddControl(m_pEnableTeamButton); 
	m_pEnableTeamButton->CreateEx(this,22,251,110,20); 
	m_pEnableTeamButton->SetText("允许组队",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
	m_pEnableTeamButton->SetChecked(g_pGameData->IsAllowTroop());


	m_pAutoTeamVoice = new CCtrlRadio();
	AddControl(m_pAutoTeamVoice);
	m_pAutoTeamVoice->CreateEx(this,22,278,110,20);
	m_pAutoTeamVoice->SetText("自动组队语聊",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
	m_pAutoTeamVoice->SetChecked(g_AICfgMgr.IsAutoTeamVoice());
	m_pAutoTeamVoice->SetTips("组队后自动进入组队语聊频道");
	m_pAutoTeamVoice->SetFont(FONT_YAHEI,12);
	
	//m_pEnableFreeQunying = new CCtrlRadio();
	//AddControl(m_pEnableFreeQunying);
	//m_pEnableFreeQunying->CreateEx(this,22,260,110,20);
	//m_pEnableFreeQunying->SetText("允许自由群英模式",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
	//m_pEnableFreeQunying->SetChecked(SELF.IsFreeTrusteeship());

	//m_pEnableRegionQunying = new CCtrlRadio();
	//AddControl(m_pEnableRegionQunying);
	//m_pEnableRegionQunying->CreateEx(this,22,279,110,20);
	//m_pEnableRegionQunying->SetText("允许安全群英模式",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
	//m_pEnableRegionQunying->SetChecked(SELF.IsAreaTrusteeship());

	//m_pTrustepshipOnlyFriend = new CCtrlRadio();
	//AddControl(m_pTrustepshipOnlyFriend);
	//m_pTrustepshipOnlyFriend->CreateEx(this,22,298,110,20);
	//m_pTrustepshipOnlyFriend->SetText("仅允许好友群英模式",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
	//m_pTrustepshipOnlyFriend->SetChecked(SELF.IsTrustepshipOnlyFriend());
	//m_pTrustepshipOnlyFriend->SetTips("勾选后，仅接受您的好友邀请您进入群英模式（包括安全、自由）");


	m_pAddListenVoiceButton = new CCtrlButton();
	AddControl(m_pAddListenVoiceButton);
	m_pAddListenVoiceButton->CreateEx(this,349,260,200,201,202,203);

	m_pAddSpeakVoiceButton = new CCtrlButton();
	AddControl(m_pAddSpeakVoiceButton);
	m_pAddSpeakVoiceButton->CreateEx(this,349,285,200,201,202,203);

	m_pDecListenVoiceButton = new CCtrlButton();
	AddControl(m_pDecListenVoiceButton);
	m_pDecListenVoiceButton->CreateEx(this,269,260,205,206,207,208);
	m_pDecListenVoiceButton->SetEnable(false);

	m_pDecSpeakVoiceButton = new CCtrlButton();
	AddControl(m_pDecSpeakVoiceButton);
	m_pDecSpeakVoiceButton->CreateEx(this,269,285,205,206,207,208);
	m_pDecSpeakVoiceButton->SetEnable(false);

	m_pCloseVoiceButton = new CCtrlButton();
	m_pCloseVoiceButton->CreateEx(this,165,253,90,91,92);
	AddControl(m_pCloseVoiceButton);
	m_pCloseVoiceButton->SetFont(FONT_YAHEI,14);
	if(m_bInTeamRoom)	m_pCloseVoiceButton->SetText("关闭语音",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_DISABLE);
	else				m_pCloseVoiceButton->SetText("开启语音",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_DISABLE);

	m_pVoiceControlButton = new CCtrlButton();
	AddControl(m_pVoiceControlButton);
	m_pVoiceControlButton->CreateEx(this,165,283,90,91,92,93);
	m_pVoiceControlButton->SetText("语音设置",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_DISABLE);

#ifdef _CHAT
	m_pVoiceControlButton->SetEnable(g_VoiceAdapter.IsInRoom());
#endif

	//m_pVolumeTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,14053);


	if (g_pGameData->HasPaoPaoStatus(EP_First_Team_Step1) && !g_pGameData->HasPaoPaoStatus(EP_First_Team_TeamWnd))
	{
		AddArrowTip(EP_First_Team_TeamWnd,37,267,XAL_TOPLEFT,false,XAL_TOPLEFT);
		g_pGameControl->Send_Player_Prompt_Status(EP_First_Team);//完成任务			
	}
}

void CTeamWnd::Draw()
{
	if(m_pEnableTeamButton)
	{
		m_pEnableTeamButton->SetChecked(g_pGameData->IsAllowTroop());
	}

	//if(m_pTrustepshipOnlyFriend)
	//{
	//	m_pTrustepshipOnlyFriend->SetChecked(SELF.IsTrustepshipOnlyFriend());
	//}

	if(g_pGameData->GetTroopMembers() <= 0) //没组队 
	{
		if (m_vecNearPersons.size() > 0)
			 m_pAddButton->SetEnable(true);
		else m_pAddButton->SetEnable(false);

		m_pDeleteButton->SetEnable(false);
	}
	else 	//组队了
	{
		if(strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) == 0)//是队长
		{
			m_pAddButton->SetEnable(true);
			m_pDeleteButton->SetEnable(true);
		}
		else//不是队长
		{
			m_pAddButton->SetEnable(false);
			m_pDeleteButton->SetEnable(false);
		}
	}

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX+193,m_iScreenY+13,"组 队",COLOR_TABPAGE_MOUSEON,FONT_YAHEI,16);

	g_pFont->DrawText(m_iScreenX+32,m_iScreenY+54,"附近玩家",COLOR_TEXT_NORMAL,FONT_YAHEI,12);

	int nNumTeamMembers = g_pGameData->GetTroopMembers();
	if (nNumTeamMembers != m_pGroupedGrid->GetTotalCount())
		m_pGroupedGrid->SetTotalCount(nNumTeamMembers);
	int iPos = m_pGroupedGrid->GetScrollPos();
	for(int i = iPos;i<nNumTeamMembers && i < 9 + iPos;i++)
	{
		m_pGroupedGrid->DrawGridEx(i - iPos,0,g_pGameData->GetMemberName(i),false,0,0,COLOR_TEXT_NORMAL);
	} 

	int iNearNum = m_vecNearPersons.size();
	if (iNearNum != m_pNeedGroupGrid->GetTotalCount())
		m_pNeedGroupGrid->SetTotalCount(iNearNum);

	iPos = m_pNeedGroupGrid->GetScrollPos();
	for(int i = iPos; i < iNearNum && i < iPos + 9;i++)
	{
		m_pNeedGroupGrid->DrawGridEx(i - iPos,0,m_vecNearPersons.at(i).c_str(),false,0,0,COLOR_TEXT_NORMAL);
	}

	//画音量控制条
	int iSpeakVol = g_VoiceAdapter.GetSpeakVol();
	int iMicVol = g_VoiceAdapter.GetMicVol();

	m_pAddListenVoiceButton->SetEnable(iSpeakVol != VOL_MAX);
	m_pDecListenVoiceButton->SetEnable(iSpeakVol != 0);
	m_pAddSpeakVoiceButton->SetEnable(iMicVol != VOL_MAX);
	m_pDecSpeakVoiceButton->SetEnable(iMicVol != 0);


	for(int i = 0;i < iSpeakVol;i++)
	{
		g_pGfx->DrawTextureNL(m_iScreenX+288+i*5,m_iScreenY+258,g_pTexMgr->GetTex(PACKAGE_INTERFACE,14053,EP_UI)); 
	}

	for(int j = 0;j<iMicVol;j++)
	{
		g_pGfx->DrawTextureNL(m_iScreenX+288+j*5,m_iScreenY+284,g_pTexMgr->GetTex(PACKAGE_INTERFACE,14053,EP_UI));
	}

#ifdef _CHAT
	//画聊天状态指示条
	if (g_VoiceAdapter.GetMicLevel() > 15)
	{
	    g_pGfx->DrawFillRect(m_iScreenX + 256,m_iScreenY + 296,g_VoiceAdapter.GetMicLevel()*90/256,2,0xffe0c000);
	}
	g_pGfx->DrawFillRect(m_iScreenX + 256,m_iScreenY + 263,g_VoiceAdapter.GetSpeakLevel()*90/256,2,0xffe0c000);

	if(m_bInTeamRoom != g_VoiceAdapter.IsInTeamRoom())
	{
	    SwitchButton(g_VoiceAdapter.IsInTeamRoom());
	}
#endif
	
}

bool CTeamWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	//case MSG_CTRL_NOTIFY_TRUSTEESHIP_MODE:
	//	{
	//		m_pEnableFreeQunying->SetChecked(SELF.IsFreeTrusteeship());
	//		m_pEnableRegionQunying->SetChecked(SELF.IsAreaTrusteeship());
	//	}
	//	break;
	case MSG_CTRL_DELETE_TEAMMEMBER:
		{
			int iSelPos = m_pGroupedGrid->GetSelLine();
			if(iSelPos >= 0)
			{
				g_OtherData.SetDeleteMember(g_pGameData->GetMemberName(iSelPos));
			    g_pGameControl->SEND_Group_Delete(g_pGameData->GetMemberName(iSelPos));
			}
			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		//音乐
		if(pControl == m_pAddButton ||pControl == m_pDeleteButton)
		{
			g_pAudio->Play(EAT_OTHER, 22, g_pAudio->GetRand()++);
		}

		if(pControl == m_pAddButton)
		{
			if( g_pGameData->GetTroopMembers() > 0 && (strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) != 0))//不是队长，立即反回 
			{
				return true;
			}
			else
			{
				int iSelLine = m_pNeedGroupGrid->GetSelLine();
				if (iSelLine >= 0 && iSelLine < m_vecNearPersons.size())
				{
					string& str =  m_vecNearPersons.at(iSelLine);

					if( g_pGameData->GetTroopMembers() <= 0 ) //未组队
						 g_pGameControl->SEND_Group_Create(str.c_str()); 
					else g_pGameControl->SEND_Group_Add(str.c_str());					
				}
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("请先选中被组人员！");
				}
			}
		}
		else if(pControl == m_pDeleteButton)
		{
			if( g_pGameData->GetTroopMembers() <= 0 ||
				(strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) != 0))//未组队或不许组队成员都<=0，或则不是队长，立即反回
			{
				return true;
			}
			else
			{
				int iSelPos = m_pGroupedGrid->GetSelLine();
				if (iSelPos >= 0 && iSelPos < g_pGameData->GetTroopMembers())
				{
					char buffer[256];				
					strcpy(buffer,"你真的想从编组中删除");
					strcat(buffer,g_pGameData->GetMemberName(iSelPos));
					strcat(buffer,"这个成员吗？");
					g_MsgBoxMgr.PopSimpleComfirm(buffer,MSG_CTRL_DELETE_TEAMMEMBER,0);
				}

				return true;
			}			
		}
		else if(pControl == m_pEnableTeamButton)
		{	
			if(g_pGameData->IsAllowTroop())
			{					
				//音乐
				g_pAudio->Play(EAT_OTHER, 18, g_pAudio->GetRand()++);
				g_pGameControl->SEND_Group_Open_Team(false);			
			}
			else
			{
				//音乐
				g_pAudio->Play(EAT_OTHER, 19, g_pAudio->GetRand()++);
				g_pGameControl->SEND_Group_Open_Team(true);
			}

			this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_Team_Step1,NULL);

		}
		else if(pControl == m_pAutoTeamVoice)
		{
#ifdef _CHAT
			g_AICfgMgr.SetAutoTeamVoice(m_pAutoTeamVoice->IsChecked());
#endif
		}
		else if (pControl == m_pVoiceControlButton)
		{
	        g_pControl->Msg(MSG_CTRL_VOICE_CONFIG_WND,OPER_CREATE); //语音相关，暂时注销，待修改
		    return true;
		}

		else if(pControl == m_pCloseVoiceButton)
		{  
#ifdef _CHAT
			if(GetTickCount() - m_dwLastLeaveOrEnterRoomTime < 2000)
				return true;

			m_dwLastLeaveOrEnterRoomTime = GetTickCount();


			//关闭语音
			if(m_bInTeamRoom)
			{
			    g_VoiceAdapter.LeaveRoom();
			}
			else
			{
			    if(g_VoiceAdapter.IsInMyGame())
				{
					if(g_VoiceAdapter.IsInRoom())
					{
						g_TalkMgr.PushSysTalk("请先退出你目前所在的频道");
					}
					else
					{
						g_VoiceAdapter.EnterTeamRoom();
					}
				}
				else
				{
				    g_MsgBoxMgr.PopSimpleAlert("你不在自己的语聊服务器中，无法进入\n组队频道,请返回自己的语聊服务器！");
				}
			}
#endif
			return true;
		}
		else if (pControl == m_pAddListenVoiceButton)
		{
#ifdef _CHAT
			g_VoiceAdapter.IncSpeakVol(1);
#endif
			return true;
		}
		else if (pControl == m_pAddSpeakVoiceButton)
		{
#ifdef _CHAT
			g_VoiceAdapter.IncMicVol(1);
#endif
			return true;
		}
		else if (pControl == m_pDecListenVoiceButton)
		{
#ifdef _CHAT
			g_VoiceAdapter.IncSpeakVol(-1);
#endif
			return true;
		}
		else if (pControl == m_pDecSpeakVoiceButton)
		{
#ifdef _CHAT
			g_VoiceAdapter.IncMicVol(-1);
#endif
			return true;
		}
		//else if (pControl == m_pEnableFreeQunying)
		//{
		//	if(SELF.IsFreeTrusteeship())
		//	{					
		//		//音乐
		//		g_pAudio->Play(EAT_OTHER, 18, g_pAudio->GetRand()++);
		//		g_pGameControl->Send_TrusteeshipSwitch(1,false);			
		//	}
		//	else
		//	{
		//		//音乐
		//		g_pAudio->Play(EAT_OTHER, 19, g_pAudio->GetRand()++);
		//		g_pControl->PopupWindow(MSG_CTRL_QUNYING_EXP_WND,OPER_RECREATE,1);
		//	}
		//	m_pEnableFreeQunying->SetChecked(false);
		//}
		//else if (pControl == m_pEnableRegionQunying)
		//{
		//	if(SELF.IsAreaTrusteeship())
		//	{					
		//		//音乐
		//		g_pAudio->Play(EAT_OTHER, 18, g_pAudio->GetRand()++);
		//		g_pGameControl->Send_TrusteeshipSwitch(0,false);
		//	}
		//	else
		//	{
		//		//音乐
		//		g_pAudio->Play(EAT_OTHER, 19, g_pAudio->GetRand()++);
		//		g_pControl->PopupWindow(MSG_CTRL_QUNYING_EXP_WND,OPER_RECREATE,2);
		//	}
		//	m_pEnableRegionQunying->SetChecked(false);
		//}
		//else if (pControl == m_pTrustepshipOnlyFriend)
		//{
		//	//音乐
		//	g_pAudio->Play(EAT_OTHER, 18, g_pAudio->GetRand()++);
		//	g_pGameControl->Send_Trustepship_Only_Friend(m_pTrustepshipOnlyFriend->IsChecked()?1:0);
		//}

		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CTeamWnd::OnLeftButtonDown(int iX,int iY)
{
	return CCtrlWindowX::OnLeftButtonDown(iX,iY); 
}

void CTeamWnd::SwitchButton(bool bInTeamRoom)
{
	m_bInTeamRoom = bInTeamRoom;
	if(m_bInTeamRoom == 1)
		 m_pCloseVoiceButton->SetText("关闭语音");
	else m_pCloseVoiceButton->SetText("开启语音");
}