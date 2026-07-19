#include "GameControl.h"
#include "GameData/MagicDefine.h"
#include "BaseClass/Control/Control.h"
#include "GameData/TalkMgr.h"
#include "GameData/GameData.h"
#include "GameData/OtherData.h"
#include "GameData/LoginData.h"
#include "GameData/DirtyWords.h"
#include "GameData/MsgBoxMgr.h"
#include "GameClient/VoiceAdapter.h"
#include "GameAI/AIConfigMgr.h"
#include "GameAI/PathFinder.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameClient/SDSamplerMgr.h"
#include "GameData/MagicCtrlMgr.h"

#include <memory>

void CGameControl::MSG_Group_Member_Delete_Name(const char * msg,int iLen)
{
	char temp[256] = {0};
	memcpy(temp,msg + 12, iLen -12);
	temp[iLen - 12] = 0; 
	g_pGameData->DelTroopMember(temp);
}

void CGameControl::MSG_Group_Company_Failed(const char * msg,int iLen)
{
	string str;

	switch(*((DWORD *)msg))
	{
	case 0xFFFFFFFE:
		str = "被组队人不存在！";
		break;
	case 0xFFFFFFFC:
		str = "被组队人不允许组队！";
		break;
	case 0xFFFFFFFD:
		str = "被组队人已经加入其他队伍！";
		break;
	case 0xFFFFFFFB:
		str = "被组队人不在本地图！";
		break;
	default:
		str = "组队失败！";
		break;
	}
	
	g_TalkMgr.PushSysTalk(str.c_str());
}


void CGameControl::MSG_Group_Member_Add_Failed(const char * msg,int iLen)
{
	string str;

	switch(*((DWORD *)msg))
	{
	case 0xFFFFFFFF:
		str = "你不是队长，不能添加成员！";
		break;
	case 0xFFFFFFFE:
		str = "被加入人不存在！";
		break;
	case 0xFFFFFFFC:
		str = "被加入人不允许组队！";
		break;
	case 0xFFFFFFFD:
		str = "重复加入或被加入人已经加入别的队伍！";
		break;
	default:
		str = "添加成员失败！";
		break;
	}
	g_TalkMgr.PushSysTalk(str.c_str());
}


void CGameControl::MSG_Group_Member_Delete_Failed(const char * msg,int iLen)
{
	if( *((DWORD *)msg) == 0xFFFFFFFE)
	{
		g_TalkMgr.PushSysTalk("被删除的人不存在！");
		return;
	}
	if( *((DWORD *)msg) == 0xFFFFFFFF)
	{
		g_TalkMgr.PushSysTalk("你不是队长，不能删除队员！");
		return;
	}
}

void CGameControl::MSG_Group_Enable(const char * msg,int iLen)
{

	bool b;
	b = (msg[6]==0)? false:true;
	g_pGameData->EnableTroop(b);

	if(!b) 
	{
#ifdef _CHAT
		if(g_VoiceAdapter.GetTeamRoomID() != 0)//g_VoiceAdapter.LeaveTeamRoom();//解散队伍也不退出语聊
			g_VoiceAdapter.SetTeamRoomID(0);			
#endif
		g_pGameData->DelAllTroopMember();
		g_pGameData->GetGroupPos().clear(); 

		MiniChar& _chars = MapArray.GetMiniChar();
		MiniChar::iterator it;
		for(it=_chars.begin();it!=_chars.end();it++)
		{
			CSimpleCharacter* pChar = it->second;
			if(pChar)
			{
				pChar->RemoveRelationType(RT_GROUP);
			}
		}
	}

}

void CGameControl::MSG_Group_Member_Delete_Ok(const char * msg,int iLen)
{

//解散队伍也不退出语聊
#ifdef _CHAT
//	if(g_pGameData->GetTroopMembers() <= 0)
//	{
//		g_VoiceAdapter.LeaveTeamRoom();
//	}
//	else if (strcmp(SELF.GetName(),g_pGameData->GetMemberName(0)) != 0)
//	{
//		g_VoiceAdapter.LeaveTeamRoom();
//	}
//	else if(strcmp(g_OtherData.GetDeleteMember(),SELF.GetName())==0 || g_pGameData->GetTroopMembers() <= 2)
//	{
//		g_VoiceAdapter.LeaveTeamRoom();
//	}

	if(g_pGameData->GetTroopMembers() <= 0)
	{
		g_VoiceAdapter.SetTeamRoomID(0);
	}
	else if (strcmp(SELF.GetName(),g_pGameData->GetMemberName(0)) != 0)
	{
		g_VoiceAdapter.SetTeamRoomID(0);
	}
	else if(strcmp(g_OtherData.GetDeleteMember(),SELF.GetName())==0 || g_pGameData->GetTroopMembers() <= 2)
	{
		g_VoiceAdapter.SetTeamRoomID(0);
	}
#endif

	g_pGameData->DelAllTroopMember();
	g_pGameData->GetGroupPos().clear();

	//是否组队发生了改变,发送收集信息
	g_SdSamplerMgr.SendSocietyInfoSample();


	MiniChar& _chars = MapArray.GetMiniChar();
	MiniChar::iterator it;
	for(it=_chars.begin();it!=_chars.end();it++)
	{
		CSimpleCharacter* pChar = it->second;
		if(pChar)   
		{
			pChar->RemoveRelationType(RT_GROUP);
		}
	}
}

void CGameControl::MSG_Group_Member_Info(const char * msg,int iLen)
{

	int i = 12,j = 12;
	char temp[255];	
	int iOldMembers = g_pGameData->GetTroopMembers(); //组队成员数目

	g_pGameData->DelAllTroopMember();
	g_pGameData->GetGroupPos().clear();

	char  group_member[256] = {0};
	int m = 0;

	while( i < iLen)
	{
		if( msg[i] == '/' )
		{
			memcpy(temp,msg + j, i- j);
			temp[i- j] = 0;
			j = i + 1;

			g_pGameData->AddTroopMember(temp);

			CSimpleCharacter* pChar = NULL;
			pChar = g_pGameData->FindSimpleCharacter(temp);
			if(pChar)
			{
				pChar->AddRelationType(RT_GROUP);
			}
		}
		i++;
	}

	//g_pGameData->EnableTroop(true);

	//是否组队发生了改变,发送收集信息
	if((iOldMembers == 0 && g_pGameData->GetTroopMembers() > 0) || (iOldMembers != 0 && g_pGameData->GetTroopMembers() == 0))
	{
		g_SdSamplerMgr.SendSocietyInfoSample();
	}



#ifdef _CHAT
	//语音聊天
	if(iOldMembers == 0 && g_pGameData->GetTroopMembers() > 0)
	{
		if(g_pGameData->GetTroopMembers() > 2 && g_VoiceAdapter.IsConnected())  //新队员
		{
			SEND_Voice_Request_Info(0,"");
		}
		else if(stricmp(g_pGameData->GetMemberName(0),SELF.GetName()) == 0) //队长
		{
			if(g_AICfgMgr.IsAutoTeamVoice())
			{
				if(g_VoiceAdapter.IsInRoom())
				{
					g_VoiceAdapter.SetCreateTeamRoomAfterLeave(true);
					g_VoiceAdapter.LeaveRoom();
				}
				else
					g_VoiceAdapter.CreateTeamVoiceRoom(); //创建组队房间
			}
			else
			{
				if(g_VoiceAdapter.IsConnected() && g_VoiceAdapter.GetTeamRoomID() == 0)
				{
					if (!g_VoiceAdapter.IsInMyGame())
						g_MsgBoxMgr.PopSimpleAlert("你不在自己的语聊服务器中，将无法创建自己的组队频道！");
					else
						g_MsgBoxMgr.PopSimpleComfirm("系统将为你创建组队语音频道，创建前将退出公共频道 确定要创建吗？",MSG_CTRL_CREATE_TEAM_VOICE_ROOM,0);
				}
			}
		}
	}
	//else if(iOldMembers > 0 && g_pGameData->GetTroopMembers() <= 0)//解散队伍也不退出语聊
	//{
	//	g_VoiceAdapter.LeaveTeamRoom();
	//}
	else if(iOldMembers > 0 && g_pGameData->GetTroopMembers() <= 0)
	{
		g_VoiceAdapter.SetTeamRoomID(0);
	}
#endif

}


void CGameControl::MSG_Group_Member_Pos(const char * msg,int iLen)
{
	g_pGameData->GetGroupPos().clear();

	char *p = (char*)(msg + 12);
	DWORD stMemberPos=0;

	for(int i =0; i < (iLen-12)/4 ;++i)
	{
		stMemberPos =((*((WORD *)p))<<16)  | *((WORD *)(p+2));
		p += 4;
		g_pGameData->GetGroupPos().push_back(stMemberPos);
	}
}


// 要发送的消息处理
void CGameControl::SEND_Group_Create(const char * name)
{
	string _name = name;
	StringUtil::trim(_name);
	if(_name.empty())
		return;

	SET_COMMAND(CS_GROUP_CREATE,256);
	ADD_STR(_name.c_str());
	SEND_GAME_SERVER();

	g_pGameControl->Send_Player_Prompt_Status(44,1);
}

void CGameControl::SEND_Group_Add(const char * name)
{
	string _name = name;
	StringUtil::trim(_name);
	if(_name.empty())
		return;

	SET_COMMAND(CS_GROUP_ADD,256);
	ADD_STR(_name.c_str());
	SEND_GAME_SERVER();
}

void CGameControl::SEND_Group_Delete(const char * name)
{
	string _name = name;
	StringUtil::trim(_name);
	if(_name.empty())
		return;

	SET_COMMAND(CS_GROUP_DELETE,256);
	ADD_STR(_name.c_str());
	SEND_GAME_SERVER();
}

void CGameControl::SEND_Group_Open_Team(bool bOpen)
{
	SET_COMMAND(CS_GROUP_OPEN_TEAM,12);
	temp[6] = bOpen?0x01:0x00;

	SEND_GAME_SERVER();
}

void CGameControl::SEND_Group_Open_VoiceChat(bool bOpen)
{
	SET_COMMAND(CS_GROUP_OPEN_VOICECHAT,12);
	ASSIGN_BYTE(6, bOpen?0x01:0x00);
	SEND_GAME_SERVER();
}

void CGameControl::SEND_Group_Query()
{
// 	if (g_TrusteeshipData.IsTrusteeship() && !g_TrusteeshipData.IsCaptain())
// 	{
// 		return;
// 	}

	SET_COMMAND(CS_GROUP_QUERY,12);
	SEND_GAME_SERVER();
}

void CGameControl::MSG_Other_Messages(const char * msg,int nLen)
{
	WORD color = *(WORD*)(msg + 6);

	_OtherMsg MsgTemp;
	if(msg[11] == 1)
	{
		auto_ptr<char> p(new char[nLen]);
		auto_ptr<char> str(new char[nLen]);
		memset(str.get(),0,nLen);
		memset(p.get(),0,nLen);
		memcpy(str.get(),msg + 12,nLen - 12);

		string strPlayerName = SELF.GetName();

		if(g_DirtyWords.ClearWords1(msg + 12))
			return;

		g_DirtyWords.ClearWords(str.get(),p.get());

		MsgTemp.strMsg.assign(p.get(),strlen(p.get()));
		MsgTemp.wColor = color;
		g_OtherData.GetAnotherMsg().push_back(MsgTemp);
	}
	else if(msg[11] == 2 || msg[11] == 4)
	{
		MsgTemp.strMsg.assign(msg + 12,nLen - 12);
		MsgTemp.wColor = color;
		g_OtherData.GetOtherMsg2().push_back(MsgTemp);
	}
	else
	{
		MsgTemp.strMsg.assign(msg + 12,nLen - 12);
		MsgTemp.wColor = color;
		g_OtherData.GetOtherMsg().clear();
		g_OtherData.GetOtherMsg().push_back(MsgTemp);
		g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_LEFT_FLOW_INFO,0,NULL);
	}
}
void CGameControl::MSG_Custom_Piaohong(const char * msg,int nLen)
{
	VCustomPiaohongMsg& vecMsgTemp = g_OtherData.GetCustomPiaohong();
	
	CustomPiaohongMsg MsgTemp;
	MsgTemp.bNeedbk = Conv_BYTE(msg);
	MsgTemp.byFontType = Conv_BYTE(msg + 6);
	MsgTemp.byFontSize = Conv_BYTE(msg + 7);
	MsgTemp.byFontColor = Conv_BYTE(msg + 8);
	MsgTemp.byBkColor = Conv_BYTE(msg + 9);
	MsgTemp.Message.assign(msg + 12,nLen - 12);

	if (Conv_BYTE(msg + 10) == 1)
	{
		g_pMagicCtrl->CreateMagic(MAGICID_REGALEMENT_WITHOUT_CAKE,EMP_MAGIC_MYSELF,SELF.GetID());
	}

	vecMsgTemp.clear();
	
	vecMsgTemp.push_back(MsgTemp);
	
	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_CUSTOM_PIAOHONG,0,NULL);
}
void CGameControl::SEND_Guild_Reply_Invited(bool bJoin)
{
	SET_COMMAND(CS_GUILD_REPLY_INVITE,CMD_SIZE);
	temp[6] = bJoin;
	SEND_GAME_SERVER();
}

void CGameControl::MSG_Guild_Invited(const char * msg,int iLen)
{
	S_CommonConfirm CommonConfirm;
	string strName;
	strName.assign(msg + 12,iLen - 12);

	CommonConfirm.iType = 5;
	CommonConfirm.dwLastTime = 30000;
	//CommonConfirm.dwRequestPersonID = p->GetID();
	CommonConfirm.strMsg = strName + "邀请您加入行会。";
	g_OtherData.GetCommonConfirmVector().push_back(CommonConfirm);

	g_pControl->PopupWindow(MSG_CTRL_COMMON_CONFIRM_WND,OPER_CREATE);
}

void CGameControl::SEND_Voice_Request_Info(int id,const char* strPasWd,int Type)
{
	static DWORD dwLastReqInfo = 0;
	if(GetTickCount() - dwLastReqInfo < 2000)
		return;

	dwLastReqInfo = GetTickCount();

	SET_COMMAND(CS_VOICE_REQUST_INFO,256);
	ASSIGN_DWORD(6,id);	

	if(Type == 1)
	{
		if(g_pGameData->GetTroopMembers() > 0 && (strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) == 0)) //队长
			temp[10] = 1;
		else
			temp[10] = 2;
	}
	else if (Type == 2)
	{
		temp[10] = 3;
	}
	ADD_STR(strPasWd);
	SEND_GAME_SERVER();
}

void CGameControl::SEND_Voice_Request_Server_Addr()
{
	SET_COMMAND(CS_VOICE_REQUEST_SERVER_ADDR,12);
	SEND_GAME_SERVER();
}

void CGameControl::MSG_VoiceChat_Enable(const char* msg, int iLen)
{
}

void CGameControl::MSG_Voice_Info(const char* msg,int iLen)
{
	DWORD dwRoomID = Conv_DWORD(msg+6);
	WORD  wResult = Conv_WORD(msg+10);

	if(dwRoomID == 0)
	{
		g_TalkMgr.PushSysTalk("没有指定的组队频道");
		return;
	}
	else
	{
#ifdef _CHAT
		g_VoiceAdapter.SetTeamRoomID(dwRoomID);

		if (wResult == 2)
		{
			//g_pGameData->AddSysMessage("通知/查询服务器失败");
		}
		else if (wResult == 0)
		{
			std::string strToken;
			strToken.assign(msg+12,iLen-12);
			g_VoiceAdapter.SetTeamRoomPassword(strToken.c_str());
			if(g_AICfgMgr.IsAutoTeamVoice())
			{
				if(g_VoiceAdapter.IsInRoom())
				{
					g_VoiceAdapter.LeaveRoom();
					g_VoiceAdapter.SetEnterTeamRoomAfterLeave(true);//延时登录语聊房间
				}
				else
					g_VoiceAdapter.EnterTeamRoom();

			}
			else
			{
				if(g_VoiceAdapter.IsInRoom())
				{
					g_TalkMgr.PushSysTalk("组队聊天频道已可以进入，请先退出当前频道后点击组队窗口中的开启语音按键进入！");
				}
				else
				{
					g_TalkMgr.PushSysTalk("组队聊天频道已可以进入，请点击组队窗口中的开启语音按键进入！");
				}
			}
		}
		else if (wResult == 1)
		{
			g_TalkMgr.PushSysTalk("频道已为你创建");
			g_pControl->PopupWindow(MSG_CTRL_VOICE_CHAT_WND,OPER_CREATE);//打开聊天窗口	
		}
#endif
	}
}

void CGameControl::MSG_Voice_Server_Addr(const char* msg,int iLen)
{
	if (iLen < 24)
		return;

#ifdef _CHAT
	int iPort = Conv_WORD(msg+23);
	if (iPort == 0)
	{
		g_VoiceAdapter.SetWaitServer(false);
		return;
	}

	std::string strIP;
	strIP.assign(msg+25,iLen-17);
	g_VoiceAdapter.SetIP(strIP.c_str(),iPort);

	//组号为msg[13]的低四位,wArea为msg[13]的高四位接msg[12]的8位
	BYTE byHArea = (BYTE)msg[14] & 0xF0;
	WORD wArea = (((WORD)byHArea) << 4) | ((WORD)((BYTE)msg[13]));
	BYTE byGroup = (BYTE)msg[14] & 0x0F;
	g_VoiceAdapter.SetMyGameInfo(msg[12],wArea,byGroup);

	__int64 Key = *((__int64*)(msg+15));
	char tmp[64] = {0};
	sprintf(tmp,"%I64d",Key);
	g_VoiceAdapter.SetLoginKey(tmp);

	g_VoiceAdapter.SetGameAreaName(g_Login.GetAreaName());
	g_VoiceAdapter.SetGameGroupName(g_Login.GetGroupName());
	g_VoiceAdapter.SetGameGroupId(0);

	if (g_pGameData->IsAllowTroop() && !g_VoiceAdapter.IsConnected())
	{
		g_VoiceAdapter.Init();
	}
	
	//如果是返回自己房间，重新登陆，不用初始化
	if(g_VoiceAdapter.IsConnected())
	{
		g_VoiceAdapter.ReLogin();
	}

	if(g_VoiceAdapter.IsWaitServer())
	{
		g_VoiceAdapter.SetWaitServer(false);
		g_pControl->Msg(MSG_CTRL_VOICE_WND,OPER_CREATE);
	}
#endif
}


//wType: 1:请求dwPlayerID喝药设置数据,，收到后弹设置窗口 2:发送自身的喝药设置数据到dwPlayerID,3:队长给dwPlayerID设置喝药数据,4:队长给dwPlayerID设置战斗配置
//5:请求dwPlayerID喝药设置数据,收到后不弹设置窗口,6:同2, 
//7.请求队员战斗设置,8:发送自己战斗设置给队长
// void CGameControl::Send_Trusteeship_DataExchange(DWORD dwPlayerID,WORD wType)
// {
// 	SET_COMMAND(CS_TRUSTEESHIP_DATA_EXCHANGE,512);
// 
// 	ASSIGN_ID(dwPlayerID);
// 	ASSIGN_DWORD(6,SELF.GetID());
// 	ASSIGN_WORD(10,wType);
// 
// 
// 	if (wType == 2 || wType == 6)
// 	{
// 		TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 		if (!pSelf)
// 		{
// 			return;
// 		}
// 
// 		ADD_ARRAY(&(pSelf->eatConfig),sizeof(STrusteeshipEatConfig));
// 	}
// 	else if (wType == 3)
// 	{
// 		TneupMember* pMember = g_TrusteeshipData.FindMemberByID(dwPlayerID);
// 		if (!pMember)
// 		{
// 			return;
// 		}
// 
// 		ADD_ARRAY(&(pMember->eatConfig),sizeof(STrusteeshipEatConfig));
// 	}
// 	else if (wType ==4)
// 	{
// 		TneupMember* pMember = g_TrusteeshipData.FindMemberByID(dwPlayerID);
// 		if (!pMember)
// 		{
// 			return;
// 		}
// 
// 		ADD_ARRAY(&(pMember->fightConfig),sizeof(STrusteeshipFightConfig));
// 	}
// 	else if (wType ==8)
// 	{
// 		TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 		if (!pSelf)
// 		{
// 			return;
// 		}
// 
// 		ADD_ARRAY(&(pSelf->fightConfig),sizeof(STrusteeshipFightConfig));
// 	}
// 
// 	SEND_GAME_SERVER();
// }

//wType: 1:对方请求我的喝药设置数据,收到回应后弹设置窗口,2:对方回应他的喝药数据给我,收到后要求打开其设置窗口,3:队长给我设置喝药数据,4:队长给我设置战斗配置
//5:同1，但收到回应后不弹设置窗口,6,同2,收到后不弹设置窗口,7.请求我战斗设置,8收到队员回应的战斗设置
// void CGameControl::MSG_Trusteeship_DataExchange(const char* msg,int iLen)
// {
// 	WORD wType = Conv_WORD(msg + 10);
// 	DWORD dwID = Conv_DWORD(msg + 6);//对方id
// 
// 	if (wType == 1 || wType == 5 || wType == 7)
// 	{
// 		Send_Trusteeship_DataExchange(dwID,wType + 1);
// 	}
// 	else if (wType == 2 || wType == 6)
// 	{
// 		int iPos = g_TrusteeshipData.FindMemberPosByID(dwID);
// 		if (iPos >= 0/* && iLen - 12 == sizeof(STrusteeshipEatConfig)*/)//新客户端在后面加了新配置项,为了兼容老客户端
// 		{
// 			TneupMember &member = g_TrusteeshipData.GetTneupMember(iPos);
// 			memcpy(&(member.eatConfig),msg + 12,min(iLen - 12,sizeof(STrusteeshipEatConfig)));
// 			member.bHaveRequestData = true;
// 			if (wType == 2)
// 			{
// 				g_pControl->PopupWindow(MSG_CTRL_TRUSTEESHIPCONFIG_WND, OPER_CREATE,iPos);
// 			}
// 		}
// 	}
// 	else if (wType == 8/* && iLen - 12 == sizeof(STrusteeshipFightConfig)*/)//新客户端在后面加了新配置项,为了兼容老客户端
// 	{
// 		int iPos = g_TrusteeshipData.FindMemberPosByID(dwID);
// 		if (iPos >= 0)
// 		{
// 			TneupMember &member = g_TrusteeshipData.GetTneupMember(iPos);
// 			memcpy(&(member.fightConfig),msg + 12,min(iLen - 12,sizeof(STrusteeshipFightConfig)));
// 		}
// 	}
// 	else if (wType == 3/* && iLen - 12 == sizeof(STrusteeshipEatConfig)*/)//新客户端在后面加了新配置项,为了兼容老客户端
// 	{
// 		int iCfgLen = min(iLen - 12,sizeof(STrusteeshipEatConfig));
// 		TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 		if (pSelf)
// 		{
// 			memcpy(&(pSelf->eatConfig),msg + 12,iCfgLen);
// 			string strPath = g_AICfgMgr.GetConfigDir();
// 			strPath += "tgEatCfg.dat";
// 			FILE *fp = fopen(strPath.c_str(),"wb");
// 			if (fp)
// 			{
// 				fwrite(msg + 12,iCfgLen,1,fp);
// 				fclose(fp);
// 			}
// 		}
// 	}
// 	else if (wType == 4/* && iLen - 12 == sizeof(STrusteeshipFightConfig)*/)//新客户端在后面加了新配置项,为了兼容老客户端
// 	{
// 		int iCfgLen = min(iLen - 12,sizeof(STrusteeshipFightConfig));
// 		TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 		if (pSelf)
// 		{
// 			memcpy(&(pSelf->fightConfig),msg + 12,iCfgLen);
// 			string strPath = g_AICfgMgr.GetConfigDir();
// 			strPath += "tgFightCfg.dat";
// 			FILE *fp = fopen(strPath.c_str(),"wb");
// 			if (fp)
// 			{
// 				fwrite(msg + 12,iCfgLen,1,fp);
// 				fclose(fp);
// 			}
// 		}
// 	}
//}
//一个物品从自己的包裹到托管包裹
//type,1代表从自己包裹到托管包裹，0代表从托管包裹到自己包裹,2:从自己包裹直接放到队员包裹里,
// bool CGameControl::Send_Trusteeship_GoodExchange(CGood& Good,DWORD dwMemberID,BYTE type)
// {
// 	//太长时间没有回应,清除GetTrusteeTemp最上面一个,防止出现一直不能移动物品的情况
// 	DWORD dwCount = GetTickCount();
// 	if(!SELF.GetTrusteeTemp().Add(Good))
// 	{
// 		if(dwCount - SELF.GetReserveData(plyLastExchangeTrusteeshipObjectTime) < 3000)
// 		{
// 			return false;
// 		}
// 
// 		SELF.GetTrusteeTemp().Get(0).SetID(0);
// 		if(!SELF.GetTrusteeTemp().Add(Good))
// 		{
// 			return false;
// 		}
// 	}
// 
// 	SELF.SetReserveData(plyLastExchangeTrusteeshipObjectTime,dwCount);
// 
// 
// 	DWORD id = Good.GetID();
// 	SET_COMMAND(CS_TRUSTEESHIP_GOOD_EXCHANGE,12);
// 	memcpy(temp,&id,4);
// 	ASSIGN_DWORD(6,dwMemberID);
// 	ASSIGN_BYTE(10,type);
// 	SEND_GAME_SERVER();
// 
// 	Good.SetID(0);
// 
// 	return true;
// }
// 
// void CGameControl::MSG_Trusteeship_GoodExchange(const char* msg, int iLen)
// {
// 	//没有存物品
// 	CGood& tmp = SELF.GetTrusteeTemp().Get(0);
// 	if(tmp.GetID() == 0) //没有存物品
// 		return;
// 	//包裹所属队员ID
// 	DWORD dwTempID = *(DWORD*)msg;
// 	BYTE	type = *(BYTE*)(msg + 6);
// 	BYTE	result = *(BYTE*)(msg + 7);
// 	
// 	TneupMember* pMemper = g_TrusteeshipData.FindMemberByID(dwTempID);
// 	
// 	if ( pMemper == NULL)
// 	{
// 		return;
// 	}
// 
// 	if (result > 0)//操作成功
// 	{
// 		//type==2表示放到队员本身的包裹,而不是群英包裹
// 		if (type == 1)//从自己包裹到队员包裹，将物品放入队员包裹
// 		{
// 			pMemper->package.Add(tmp);
// 		} 
// 		else if (type == 0)//从队员包裹到自己包裹
// 		{
// 			SELF.PackageGood().Add(tmp);
// 		}
// 	} 
// 	else//操作失败
// 	{
// 		//type==2表示放到队员本身的包裹失败,而不是群英包裹
// 		if (type > 0)//物品放回自己的包裹
// 		{
// 			SELF.PackageGood().Add(tmp);
// 		} 
// 		else//放回队员的包裹
// 		{
// 			pMemper->package.Add(tmp);
// 		}	
// 	}
// 
// 	SELF.GetTrusteeTemp().Clear();
// }
// 
// //选择操控对象,dwid:被操控者指针,取消微操模式，传0
// //byType:0表示微操,1:表示附身.
// void CGameControl::SEND_MicroControl_Select(DWORD dwID,BYTE byType)
// {
// 	SET_COMMAND(CS_MICROCONTROL_SELECT,512);
// 	ASSIGN_ID(dwID);
// 	ASSIGN_BYTE(6,byType);
// 	SEND_GAME_SERVER();
// }
// 
// //选择操控对象成功,0-3被操控者指针,取消微操模式id为0
// void CGameControl::MSG_MicroControl_Select(const char* msg,int iLen)
// {
// 	DWORD dwID = Conv_DWORD(msg);//对方id
// 	BYTE byType = BYTE(msg[6]);//0:微操,1:附身
// 	int iSelPlayPos = -1;
// 
// 	if (dwID != 0)
// 	{
// 		for (int i = 0 ; i < MAX_TRUSTEESHIP_NUM; i++)
// 		{
// 			if (g_TrusteeshipData.GetTneupMember(i).dwID == dwID)
// 			{
// 				iSelPlayPos = i;
// 				break;					 
// 			}			
// 		}
// 	}
// 
// 
// 
// 	SELF.GetMapPathFinder().SetOnRoute(false);
// 
// 	if (g_TrusteeshipData.IsCaptain())
// 	{
// 		//切换角色时如果鼠标上有物品,先返回原来的包裹
// 		CCharacter *pOldSelf = g_pSelf;
// 		if (g_TrusteeshipData.GetMicroControlPos() >= 0 || g_TrusteeshipData.GetCaptionSubstitutePos() >= 0)
// 		{
// 			g_pSelf = g_TrusteeshipData.GetMicroControledCharacter();
// 		}
// 		else
// 		{
// 			g_pSelf = &(g_pGameData->Self());
// 		}
// 		CGoodGrid::ReleaseDrop();
// 		g_pSelf = pOldSelf;
// 
// 
// 
// 		if (byType == 0)
// 		{
// 			g_TrusteeshipData.SetMicroControlPos(iSelPlayPos);
// 		}
// 		else if (byType == 1)
// 		{
// 			g_TrusteeshipData.SetCaptionSubstitutePos(iSelPlayPos);
// 			g_TrusteeshipData.SetMicroControlPos(-1);
// 		}
// 
// 		if (g_pControl->FindWindowByName("PackageWnd"))
// 		{
// 			g_pControl->PopupWindow("PackageWnd",OPER_RECREATE);
// 		}
// 
// 		if (iSelPlayPos >= 0 && iSelPlayPos < MAX_TRUSTEESHIP_NUM && byType >= 0 && byType <= 1)
// 		{
// 
// 			//微操时原来的队长驻扎在原地
// 			if (byType == 0)
// 			{
// 				TneupMember *pCaptain = g_TrusteeshipData.GetCaptain();
// 				if (pCaptain)
// 				{
// 					pCaptain->byAttackMode = eTrusteeshipModeGarrsion;
// 					pCaptain->dwGarrsionXY = MAKELONG(pCaptain->wX,pCaptain->wY);
// 					Send_TrusteeshipMode(pCaptain->szName,pCaptain->byAttackMode,pCaptain->dwGarrsionXY);
// 				}
// 			}
// 
// 
// 			//防止刚切换过去的时候协议还没有来人物会跳到0,0
// 			int iX,iY;
// 			SELF.GetXY(iX,iY);
// 			CCharacter *pCharacter = g_TrusteeshipData.GetMicroControledCharacter();
// 			if (pCharacter)
// 			{
// 				pCharacter->clear();
// 				pCharacter->SetXY(iX,iY);
// 				pCharacter->SetSelfTrusteeshipPos(iSelPlayPos);
// 				if(byType == 0)
// 					pCharacter->SetMicroControled(true);
// 			}
// 
// 			TneupMember* pMember = byType == 0?g_TrusteeshipData.GetMicroControlPlayer():g_TrusteeshipData.GetCaptainSubstitutePlayer();
// 			if (pMember && !pMember->bHaveRequestData && !pMember->byOffLineMode)
// 			{
// 				g_pGameControl->Send_Trusteeship_DataExchange(pMember->dwID,7);
// 				g_pGameControl->Send_Trusteeship_DataExchange(pMember->dwID,5);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		g_TrusteeshipData.SetMicroControlPos(-1);
// 		g_TrusteeshipData.SetCaptionSubstitutePos(-1);
// 
// 		if (iSelPlayPos >= 0 && iSelPlayPos == SELF.GetSelfTrusteeshipPos())
// 		{
// 			SELF.SetMicroControled(true);
// 		}
// 		else
// 		{
// 			SELF.SetMicroControled(false);
// 		}
// 	}
// }
// 
// //被微操控制角色的相关协议，协议体为封装原始协议,dwid:被操控者指针
// void CGameControl::SEND_MicroControl_SendProtocol(DWORD dwID)
// {
// 	SET_COMMAND(CS_MICROCONTROL_SENDPROTOCOL,512);
// 	ASSIGN_ID(dwID);
// 
// 
// 	SEND_GAME_SERVER();
// }
// 
// //被微操控制角色的相关协议，协议体为封装原始协议,0-3被操控者指针
// void CGameControl::MSG_MicroControl_ReceiveProtocol(const char* msg,int iLen)
// {
// 	DWORD dwID = Conv_DWORD(msg);//对方id
// 	WORD wMsgID = Conv_WORD(msg + 12 + 4);
// 
// 	//if (!g_TrusteeshipData.GetMicroControlPlayer() && !g_TrusteeshipData.GetCaptainSubstitutePlayer())
// 	//{
// 	//	return;
// 	//}
// 
// 	//处理消息
// 	map<WORD,lpFNMSGProc>::iterator itr = m_callback.find(wMsgID);
// 	if(itr != m_callback.end())
// 	{
// 		TRY_BEGIN;
// 		(this->*itr->second)(msg + 12,iLen - 12);
// 		TRY_END_DO(
// 			strAddMsg = "W协议内容:[";
// 		strAddMsg +=  HexDump((const BYTE *)msg + 12,iLen - 12,"") + "]";
// 		);
// 	}
// 	else
// 	{
// #ifdef _DEBUG
// 		output_debug("%s(%d): T_没处理的网络协议(0x%04x)\n",__FILE__,__LINE__,wMsgID);
// #endif
// 	}
// }

//
void CGameControl::MSG_Group_Request(const char* msg,int iLen)
{
	char temp[256]={0};
	memcpy(temp,msg+12,iLen-12);
	temp[iLen-12] =0;

	CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(temp);
	if(!p)
		return;

	if (!g_pGameData->IsAllowTroop())
	{
		g_pGameControl->SEND_Answer_Group_Request(temp,false);
		return;
	}

	S_CommonConfirm CommonConfirm;
	CommonConfirm.iType = 8;
	CommonConfirm.dwLastTime = 30000;
	CommonConfirm.dwRequestPersonID = p->GetID();
	CommonConfirm.strMsg = CommonConfirm.strMsg + temp + "邀请您加入队伍，是否确认加入?";
	g_OtherData.GetCommonConfirmVector().push_back(CommonConfirm);
	g_pControl->PopupWindow(MSG_CTRL_COMMON_CONFIRM_WND,OPER_CREATE);
}

void CGameControl::SEND_Answer_Group_Request(const char * name, bool bApprove)
{
	SET_COMMAND(CS_ANSWER_GROUP_REQUEST,256);
	ASSIGN_BYTE(0,bApprove ? 0x01 : 0x02);//approve 
	ADD_STR(name);
	SEND_GAME_SERVER();
}

// void CGameControl::MSG_Trusteeship_Member_Is_Offline(const char* msg,int iLen)
// {
// 	DWORD dwID = Conv_DWORD(msg);
// 	BYTE byOfflineMode = msg[6];//0表示在线,1表示离线托管
// 	
// 	TneupMember* pMemper = g_TrusteeshipData.FindMemberByID(dwID);
// 
// 	if (pMemper)
// 	{
// 		pMemper->byOffLineMode = byOfflineMode;
// 		if (byOfflineMode && !pMemper->pCharacter)
// 		{
// 			pMemper->pCharacter = new CCharacter();
// 			pMemper->pCharacter->SetSelfTrusteeshipPos(pMemper->byPos);
// 			g_TrusteeshipData.SetDeaultEatConfig(pMemper->eatConfig,pMemper->byJob,pMemper->wMaxHP,pMemper->wMaxMP);
// 		}
// 		else if (!byOfflineMode && pMemper->pCharacter)
// 		{
// 			//队长微操或附身的离线托管队员上线,不要delete该对象,//微操 或 附身且没有微操 时,此角色AI在队长客户端处理;附身且微操时,微操对象AI在队长客户端处理,附身对像AI在原对象所在客户端处理
// 			if (pMemper->pCharacter != g_TrusteeshipData.GetMicroControledCharacter())
// 			{
// 				SAFE_DELETE(pMemper->pCharacter);
// 			}
// 		}
// 	}
// }
// //1：只允许好友群英
// void CGameControl::Send_Trustepship_Only_Friend(BYTE byType)
// {
// 	SET_COMMAND(CS_TRUSTEPSHIP_ONLY_FRIEND,256);
// 	ASSIGN_BYTE(6,byType);
// 	SEND_GAME_SERVER();	
// }
// 
// void CGameControl::MSG_Trustepship_Only_Friend(const char* msg, int iLen)
// {
// 	SELF.SetTrustepshipOnlyFriend(msg[6] != 0);
// }
