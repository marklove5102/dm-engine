#include "personpanelwnd.h"
#include "GameData\Gamedata.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Control/CtrlMenuWnd.h"
#include "GameAI/AIBossCfgMgr.h"
#include "GameData/GuildData.h"
#include "GameData/OtherData.h"




INIT_WND_POSX(CPersonPanelWnd,POS_VARIABLE,POS_VARIABLE)

CPersonPanelWnd::CPersonPanelWnd(void)
{
	m_iControlMode = CTRL_MODE_WINDOW;

	m_dwPersonID = sm_dwWindowType;

	m_iIndex = 13478;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 400;
	m_iOffY = 30;

}

CPersonPanelWnd::~CPersonPanelWnd(void)
{
}

void CPersonPanelWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
	{
		CloseWindow();
		return;
	}

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,13471+pChar->GetSex()*3+pChar->GetCareer(),EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 2,m_iScreenY + 8,pTex);
	}

	//显示生命值
	if(pChar->GetHP() > 0 && pChar->GetHPMax() > 0)
	{
		float fLife = (float)pChar->GetHP() / pChar->GetHPMax();
		if(fLife > 1.0f) 
			fLife = 1.0f;

		LPTexture pLife = g_pTexMgr->GetTex(PACKAGE_INTERFACE,13479,EP_UI);

		if(pLife)
			g_pGfx->DrawArcTexture(m_iScreenX + 70,m_iScreenY + 54,pLife,0,fLife*360.0f);
	}
}

void CPersonPanelWnd::OnCreate()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(pChar) 
		m_strPersonName = pChar->GetName();

	SetMask(13478);
}	

bool CPersonPanelWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_POP_MENU:
		{
			switch(dwData)
			{
			case POP_MENU_ITEM_TEAM:
				OnTeam();
				break;
			case POP_MENU_ITEM_TRADE:
				OnTrade();
				break;
			case POP_MENU_ITEM_WATCH_EQUIP:
				OnWatchEquip();
				break;
			case POP_MENU_ITEM_CHATSECRET:
				OnChatSecret();
				break;
			case POP_MENU_ITEM_FRIEND:
				OnFriend();
				break;
			case POP_MENU_ITEM_GUILD:
				OnTakeInGuild();
				break;
			case POP_MENU_ITEM_PHYLE:
				OnTakeInPhyle();
				break;
			case POP_MENU_ITEM_TAKEIN_PRENTICE:
				OnTakeinPrentice();
				break;
			case POP_MENU_ITEM_WATCH_BOOTH:
				OnWatchBooth();
				break;
			case POP_MENU_ITEM_TAKEIN_MASTER:
				OnTakeInMaster();
				break;
			case POP_MENU_ITEM_ADDBLACK:
				OnAddBlack();
				break;
			case POP_MENU_ITEM_ADDJIEYI:
				OnAddJieyi();
				break;
// 			case POP_MENU_ITEM_INVITE_TNEUP_FREE:
// 			case POP_MENU_ITEM_INVITE_TNEUP_AREA:
// 			case POP_MENU_ITEM_APPLY_TNEUP_FREE:
// 			case POP_MENU_ITEM_APPLY_TNEUP_AREA:
// 				OnTneup(dwData);
// 				break;
#ifdef _CHAT
			case POP_MENU_ITEM_INVITE_VOICE:
				OnInviteVoice();
				break;
#endif	
			default:
				break;
			}

			return true;
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CPersonPanelWnd::OnMouseMove(int iX,int iY)
{
	CCtrlWindowX::OnMouseMove(iX,iY);

	CParserTip* pTip = g_pControl->GetTipWnd();
	pTip->SetShow(false);
	pTip->Clear();

	if(g_pControl->GetMenuWnd()->GetCaller() != this)
	{
		pTip->SetShow(true);
		pTip->AddText(m_strPersonName.c_str());
		pTip->AddText("右键点击可查看人物详细资料");
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
	}

	return true;
}

bool CPersonPanelWnd::OnLeftButtonUp(int iX, int iY)
{

	return CCtrlWindowX::OnLeftButtonUp(iX, iY);
}

bool CPersonPanelWnd::OnRightButtonUp(int iX,int iY)
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
		return true;


	CCtrlMenuWnd* pMenuWnd = g_pControl->GetMenuWnd();
	pMenuWnd->Clear();
	S_POP_MENU_DATA & PopMenuData = pMenuWnd->GetData();
	PopMenuData.fItemHeight = 16.0f;
	PopMenuData.iW = 128;

	pMenuWnd->AddMenuItem(POP_MENU_ITEM_NONE,m_strPersonName);
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_WATCH_EQUIP,"观察");

	pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHATSECRET,"密聊");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_TRADE,"交易");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_TEAM,"组队");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_FRIEND,"加为好友");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ADDJIEYI,"好友结义");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_TAKEIN_MASTER,"请求拜师");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_TAKEIN_PRENTICE,"招收徒弟");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_GUILD,"邀请加入行会");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_PHYLE,"邀请加入宗族");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_WATCH_BOOTH,"查看摊位");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ADDBLACK,"加入黑名单");
	

// 	int nNum = g_pGameData->GetTroopMembers();
// 	if (nNum > 0 && g_TrusteeshipData.IsTeamTrusteeship()) //组队中并且队长佩戴了阵谱
// 	{
// 		if (strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) == 0)//自己是队长
// 		{
// 			if(g_TrusteeshipData.FindMemberByID(pChar->GetID()) == NULL)//对方未托管
// 			{
// 				pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_FREE, "邀请加入自由群英模式");
// 				pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_AREA, "邀请加入安全群英模式");
// 			}
// 		}
// 		else//不是队长
// 		{
// 			if ((strcmp(g_pGameData->GetMemberName(0), pChar->GetName()) == 0) && (!g_TrusteeshipData.IsTrusteeship()))//对方是队长并且自己未托管
// 			{
// 				pMenuWnd->AddMenuItem(POP_MENU_ITEM_APPLY_TNEUP_FREE, "申请加入自由群英模式");
// 				pMenuWnd->AddMenuItem(POP_MENU_ITEM_APPLY_TNEUP_AREA, "申请加入安全群英模式");
// 			}
// 		}
// 	}
// 	else if (SELF.GetEquipGood(ITEM_TNEUPCHART).GetID()!=0)//放宽条件，只要自己佩戴了阵谱，未组队也能邀请托管
// 	{
// 		pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_FREE, "邀请加入自由群英模式");
// 		pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_AREA, "邀请加入安全群英模式");
// 	}

#ifdef _CHAT

	if (g_VoiceAdapter.GetRoomEnterStatus() && 
		g_VoiceAdapter.GetRoomManager()->GetMyRoom()->GetRoomType() != 3 &&
		g_VoiceAdapter.GetRoomManager()->GetMyRoom()->GetRoomType() != 4)
	{
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_VOICE,"邀请加入语聊","");
	}
	else
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_VOICE,"邀请加入语聊","",false);
#endif


	m_bClick = false;
	m_bRBClick = false;//否则MenuWnd获得不到焦点
	pMenuWnd->SetCaller(this);
	pMenuWnd->ShowMenu();
	g_pControl->GetTipWnd()->SetShow(false);

	return true;
}

void CPersonPanelWnd::OnAddBlack()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(pChar)
	{
		g_BossCfgMgr.AddBlackName((char *)pChar->GetName());
	}
}

void CPersonPanelWnd::OnAddJieyi()
{
	if (SELF.GetLevel() < 30)
	{
		g_MsgBoxMgr.PopSimpleAlert("请到达30级以后再发起结义。");
		return;
	}

	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
		return;

	if (pChar->IsJieYi())
	{
		g_MsgBoxMgr.PopSimpleAlert("您已经和该玩家结义。");
		return;
	}

	//if (pChar->GetLevel() >= 20)
	//{
	//	g_MsgBoxMgr.PopSimpleAlert("对方等级现在已超过20级，结义后奖励低于和20级以下玩家结义。");
	//	return;
	//}

	g_pGameControl->SEND_Add_Swear((char *)pChar->GetName());
}

void CPersonPanelWnd::OnTeam()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
		return;

	if( g_pGameData->GetTroopMembers() > 0 && 
		(strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) != 0))//组队了但不是队长，立即反回
	{
		g_MsgBoxMgr.PopSimpleAlert("你已经在队伍中，不是队长不可以邀请其它玩家组队。");
		return;
	}
	else
	{
		if( g_pGameData->GetTroopMembers() <= 0 ) //未组队
		{
			g_pGameControl->SEND_Group_Create( pChar->GetName() ); 
		}
		else//组队了是队长
		{
			g_pGameControl->SEND_Group_Add( pChar->GetName() );
		}
	}
}

void CPersonPanelWnd::OnTrade()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里，无法交易。");
		return;
	}

	g_pGameControl->SEND_Player_Trade(0,pChar->GetID());
}

void CPersonPanelWnd::OnWatchEquip()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
		return;

	int x,y;
	pChar->GetRealXY(x,y);
	g_pGameControl->SEND_Look_Player_Info(pChar->GetID(),x,y,pChar->GetDir()); 
}

void CPersonPanelWnd::OnChatSecret()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
		return;

	string strTemp = pChar->GetName();
	strTemp += " ";

	g_pControl->Msg(MSG_CTRL_INSERT_TEXT,1,(CControl*)strTemp.c_str());
}

void CPersonPanelWnd::OnFriend()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(pChar)
		g_pGameControl->SEND_Add_Friend(pChar->GetName());
}

void CPersonPanelWnd::OnTakeInPhyle()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
		return;

	if(!g_OtherData.IsPhyleMaster())
	{
		g_MsgBoxMgr.PopSimpleAlert("只有宗族族长才能邀请其它玩家加入宗族。");
	}
	else
	{
		g_pGameControl->SEND_Phyle_Operate(pChar->GetName(),0);		
	}
}


void CPersonPanelWnd::OnTakeInGuild()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(NULL == pChar)
		return;

	BOOL bHasRight = FALSE;
	if (g_GuildData.GetGuildTowerLevel() > 0)
	{
		DWORD dwFunc = g_GuildData.GetGuildFunc();
		bHasRight = ((dwFunc & eAdmitMember) != 0);
	}
	else
	{
		bHasRight = g_GuildData.IsHeader();
	}

	if(strlen(SELF.GetTitle()) <= 0 || (!bHasRight && (g_GuildData.GetMember().size() > 0 || g_GuildData.GetGuildMemberOfficer().size() > 0)) )
	{
		g_MsgBoxMgr.PopSimpleAlert("您没有权限邀请其它玩家加入行会。");
	}
	else
	{
		g_pGameControl->SEND_Guild_Add_Member(pChar->GetName());
	}

}

void CPersonPanelWnd::OnTakeinPrentice()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(!pChar)
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里,无法进行收徒操作。");
		return;
	}

	g_pGameControl->SEND_Master_Prentice_Request(pChar->GetID(),1);
}

void CPersonPanelWnd::OnTakeInMaster()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
	if(!pChar)
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里,无法进行拜师操作。");
		return;
	}

	g_pGameControl->SEND_Master_Prentice_Request(pChar->GetID(),2);

}

void CPersonPanelWnd::OnWatchBooth()
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);

	if(pChar && pChar->GetBoothState())
	{
		g_pGameControl->SEND_Exchange_Goin(pChar->GetID());
	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家没有摆摊。");
	}

}


// void CPersonPanelWnd::OnTneup(DWORD dwData)
// {
// 	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwPersonID);
// 	if(!pChar)
// 	{
// 		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里,无法进行拜师操作。");
// 		return;
// 	}
// 
// 	switch (dwData)
// 	{
// 	case POP_MENU_ITEM_INVITE_TNEUP_FREE:
// 		g_pGameControl->Send_Trusteeship(pChar->GetName(), 1);
// 		break;
// 	case POP_MENU_ITEM_INVITE_TNEUP_AREA:
// 		g_pGameControl->Send_Trusteeship(pChar->GetName(), 0);
// 		break;
// 	case POP_MENU_ITEM_APPLY_TNEUP_FREE:
// 		g_pGameControl->Send_Trusteeship(NULL,1);
// 		break;
// 	case POP_MENU_ITEM_APPLY_TNEUP_AREA:
// 		g_pGameControl->Send_Trusteeship(NULL, 0);
// 		break;
// 	}
// }

#ifdef _CHAT

void CPersonPanelWnd::OnInviteVoice()
{
	DWORD dwID = g_pGameData->GetOtherSimpleData().GetClickedPersonID();
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(dwID);

	if (pChar && g_VoiceAdapter.GetRoomEnterStatus())
	{
		char Temp[256] = {0};
		sprintf(Temp,"/%s 邀请你加入%s一起语聊！",pChar->GetName(),g_VoiceAdapter.GetRoomManager()->GetMyRoom()->GetRoomName());
		g_pGameControl->SEND_Message_Private(Temp,strlen(Temp),false);
		g_pGameData->AddDataMessage("已发出邀请！",12, 0xFF04,TALKTYPE_NORMAL);

	}
}

#endif