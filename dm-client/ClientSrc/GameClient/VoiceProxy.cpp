#include "VoiceProxy.h"
#include "VoiceAdapter.h"

#ifdef _CHAT
#include "GameData/TalkMgr.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/Control.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/ConfigData.h"
#include "Global/Interface/AudioInterface.h"


CRoomEvent::CRoomEvent()
{
}

// 登陆事件回调
void CRoomEvent::OnLogin( BOOL bSucc , int nErrorCode )
{
	if (bSucc == TRUE)
	{
		//显示进入成功
		g_TalkMgr.PushSysTalk("语音服务器已连接！");
		//初始化搜索服务器
		//判断是否在自己游戏
		g_VoiceAdapter.ChangeIsInMyGameStatus();
		// 成功后 服务器会主动发送一次房间列表，以后如果要获得房间列表可调用IRoomManager::GetRoomList获得
		g_VoiceAdapter.m_bConnected = TRUE;
	}
	else
	{
		//显示进入失败，显示错误码
		//output_debug("登陆失败 Err = %d\n",nErrorCode);
	}
}

// 取房间列表信息返回
// nPageIndex 客户端发送请求时的页面索引
// nPage服务器上一共的页面个数
// ppRoom房间对象指针列表
// iCount返回房间个数
void CRoomEvent::OnGetRoomList( BOOL bSucc, IRoom** ppRoom , int iCount, int nPageIndex, int nPageCount )
{
	if (bSucc == TRUE)
	{
		g_VoiceAdapter.m_iPageIndex = nPageCount;

		//显示获取房间列表成功;
		g_VoiceAdapter.GetRoomArray().clear();
		g_VoiceAdapter.GetGuildRoomArray().clear();
		for(int ii = 0;ii < iCount;ii++)
		{
			if(ppRoom[ii]->GetRoomType() == 4)
				g_VoiceAdapter.GetGuildRoomArray().push_back(ppRoom[ii]);
			else
				g_VoiceAdapter.GetRoomArray().push_back(ppRoom[ii]);
		}
	}
	else
	{
		//显示获取房间列表失败
		//output_debug("获取房间列表失败\n");
	}


}

void CRoomEvent::OnCreateRoom( DWORD dwResult, IRoom * pRoom, IUser *pOwnerUser )
{	
	if (dwResult == TRUE)
	{
		g_pControl->PopupWindow(MSG_CTRL_CHECK_OPEN_WND,OPER_CREATE,3);
		g_VoiceAdapter.m_pCurRoom = pRoom;
		//显示创建房间成功，获得房间号
		if (pRoom->GetRoomType() == 3)
		{	
			g_pGameControl->SEND_Voice_Request_Info(pRoom->GetRoomId(),g_VoiceAdapter.GetTeamRoomPassword());				
		}
		else 
		{
			g_pControl->Msg(MSG_CTRL_VOICE_ADD_WND,OPER_CLOSE);//关闭创建窗
			g_pControl->Msg(MSG_CTRL_VOICE_CHAT_WND ,OPER_CREATE);//打开聊天窗口
		}
		g_VoiceAdapter.SetUserType(1);
		g_VoiceAdapter.SetSuperMic(false);
	}
	else
	{
		//添加房主信息
		//显示创建房间失败
		string str;
		if(dwResult == FAIL_ROOM_IS_FULL)
			str = "语聊频道数量过多，请稍后再创建频道";
		else if(dwResult == FAIL_ROOM_IS_TWO)
			str = "只允许一个用户创建一个频道";	
		else if (dwResult == FAIL_ROOM_IN_ROOM)
			str = "房间内不允许创建房间";	
		else if (dwResult == FAIL_ROOM_IS_EXIST)
			str = "频道已经存在";	
		else
			str = "创建频道失败";

		g_TalkMgr.PushSysTalk(str.c_str());
	}
	g_VoiceAdapter.RequestRoom(); //重新请求房间列表
}

void  CRoomEvent::OnCreateTeamRoom( DWORD dwResult, IRoom * pRoom, IUser * pOwnerUser )
{
}

void  CRoomEvent::OnEnterRoom( DWORD dwResult, IRoom * pRoom )
{
	g_VoiceAdapter.m_bIsWaiting = false;

	if (!pRoom)
		return;

	if (dwResult == TRUE)
	{
		//显示进入房间成功
		//通知界面进入房间
		//进入voicechatwnd
		//g_VoiceAdapter.SetTeamWndButtonStatus(1);
		//g_VoiceAdapter.SetUserType(1);
		g_VoiceAdapter.m_pCurRoom = pRoom;

		g_pControl->PopupWindow(MSG_CTRL_VOICE_WND,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_CHECK_OPEN_WND,OPER_CREATE,3);
		g_pControl->PopupWindow(MSG_CTRL_VOICE_CHAT_WND,OPER_CREATE);//打开聊天窗口

		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"Win98系统用户在全屏模式下语音质量较差,建议使用按键激活方式！！",TALKCOLOR_RED);
		g_VoiceAdapter.SetUserType(1);
		g_VoiceAdapter.SetSuperMic(false);
	}
	//显示进入房间失败
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("无法进入频道，可能你输入的密码不正确，或者系统正忙。请稍后再试。");
	}
}
void  CRoomEvent::OnLeaveRoom( BOOL bSucc )
{
	if (bSucc == TRUE)
	{
		if(g_VoiceAdapter.GetGameVoice())
		{
			g_Config.SetSndOn(true);
			g_pAudio->EnableSound(true);
		}

		//成功离开房间
		if(g_pControl && g_pControl->FindWindowByName("VoiceChatWnd") != NULL)
		{
			g_pControl->PopupWindow(MSG_CTRL_VOICE_CHAT_WND,OPER_CLOSE);
			g_pControl->PopupWindow(MSG_CTRL_VOICE_WND,OPER_CREATE); //公共房间列表
		}

		g_VoiceAdapter.SetUserType(0);

		if(g_VoiceAdapter.IsCreateTeamRoomAfterLeave())
			g_VoiceAdapter.CreateTeamVoiceRoom();
		else if(g_VoiceAdapter.IsEnterTeamRoomAfterLeave())
			g_VoiceAdapter.EnterTeamRoom();
	} 
	else
	{
		g_TalkMgr.PushSysTalk("你还没有离开频道"); //显示离开房间失败
	}

	if(g_VoiceAdapter.IsLoginOutAfterLeaveRoom())
		g_VoiceAdapter.ExitAdapter();

	g_VoiceAdapter.SetCreateTeamRoomAfterLeave(false);
	g_VoiceAdapter.SetEnterTeamRoomAfterLeave(false);
}

void   CRoomEvent::OnGetUserList( BOOL bSucc, IUser** ppUser,int iCount )
{
	if (bSucc == TRUE)
	{
		UserVector& VUser = g_VoiceAdapter.GetUserArray();
		VUser.clear();
		for(int i = 0;i < iCount;i++)
		{
			UserInfo user;
			user.pUser = ppUser[i];
			VUser.push_back(user);
		}
	}
	else
	{
		g_TalkMgr.PushSysTalk("获取用户列表失败");		
	}
}

void   CRoomEvent::OnUserEnter( IUser * pUser)
{
	//显示用户进入
	if(!pUser)
		return;

	UserInfo user;
	user.pUser = pUser;
	g_VoiceAdapter.GetUserArray().push_back(user);

	char temp[64] = {0};
	sprintf(temp,"%s进入房间",pUser->GetUserName());
	g_TalkMgr.PushTalk(TALKTYPE_VOICE,temp,TALKCOLOR_BLUE);
}

void   CRoomEvent::OnUserLeave( IUser * pUser )
{
	//显示用户离开
	if(!pUser)
		return;

	g_VoiceAdapter.DeleteUser(pUser);
	char temp[64] = {0};
	sprintf(temp,"%s离开房间",pUser->GetUserName());
	g_TalkMgr.PushTalk(TALKTYPE_VOICE,temp,TALKCOLOR_BLUE);
}

void   CRoomEvent::OnKickUser( BOOL bSucc, IUser *pUser )
{

}
void   CRoomEvent::OnForbidUser( BOOL bSucc, IUser *pUser )
{
	if(bSucc)
	{
		if (pUser)
		{	
			if (pUser == g_pRoomManager->GetMyUser())
				return;
			char strTemp[128];
			if (pUser->IsForbided())
				sprintf(strTemp,"%s已被你禁言",pUser->GetUserName());
			else
				sprintf(strTemp,"%s已被解除禁言",pUser->GetUserName());

			g_TalkMgr.PushTalk(TALKTYPE_VOICE,strTemp,TALKCOLOR_RED);
		}
		else
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,"所有用户语音状态已改变！",TALKCOLOR_RED);
	}

}

void   CRoomEvent::OnSpeakOFF( BOOL bSucc, IUser *pUser )
{
	if (bSucc)
	{
		if (pUser == NULL)
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,"你已经拒听所有用户语音！",TALKCOLOR_BLUE);
		else
		{
			char strTemp[128];
			sprintf(strTemp,"你已拒听%s的语音！",pUser->GetUserName());
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,strTemp,TALKCOLOR_BLUE);
		}
	}
	else
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"拒听语音未生效！",TALKCOLOR_BLUE);

}
void   CRoomEvent::OnSpeakON( BOOL bSucc, IUser *pUser )
{
	if (bSucc)
	{
		if (pUser == NULL)
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,"你已经设置接收所有用户语音！",TALKCOLOR_BLUE);
		else
		{
			char strTemp[128];
			sprintf(strTemp,"你已设置接听%s的语音！",pUser->GetUserName());
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,strTemp,TALKCOLOR_BLUE);
		}
	}
	else
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"接听语音设置未生效！",TALKCOLOR_BLUE);

}

void   CRoomEvent::OnAddReader( BOOL bSucc, IUser * pUser)
{
	if (bSucc)
	{
		if (pUser == NULL)
			return;
		else
		{
			char strTemp[128];
			sprintf(strTemp,"你已增加%s为你的听众！",pUser->GetUserName());
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,strTemp,TALKCOLOR_BLUE);
		}
	}
	else
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"增加听众设置未生效！",TALKCOLOR_BLUE);

}
void   CRoomEvent::OnRemoveReader(BOOL bSucc , IUser * pUser )
{
	if (bSucc)
	{
		if (pUser == NULL)
			return;
		else
		{
			char strTemp[128];
			sprintf(strTemp,"你已拒绝%s为你的听众！",pUser->GetUserName());
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,strTemp,TALKCOLOR_BLUE);
		}
	}
	else
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"移除听众设置未生效！",TALKCOLOR_BLUE);

}

/// 收到某用户发送过来的语音数据
void  CRoomEvent::OnRecvVoiceData( IUser * pUser )
{
	if(!pUser)
		return;

	g_VoiceAdapter.ChangeUserStatus(true,pUser);
}

/// 收到某用户发送过来的文本消息// RE_MSG_BCAST RE_MSG_PRIVATE 
void   CRoomEvent::OnRecvChatMsg(BOOL bSucc, IUser * pFromUser, IUser * pToUser, LPCSTR pszText, WORD wLength, int nMsgType)
{
	if(wLength > 1000 || pFromUser == NULL || wLength == 0 || pszText == "")
		return;

	//计算颜色
	char str[1024] = {0};
	if(nMsgType == RE_MSG_PRIVATE)
	{
		if(pToUser)
		{
			sprintf(str,"%s对%s说:%s",pFromUser->GetUserName(),pToUser->GetUserName(),pszText);
			g_TalkMgr.PushTalk(TALKTYPE_VOICE,str,TALKCOLOR_BLUE);

			sprintf(str,"[语聊频道]：%s对%s说:%s",pFromUser->GetUserName(),pToUser->GetUserName(),pszText);
			g_TalkMgr.PushTalk(TALKTYPE_NORMAL,str,TALKCOLOR_BLUE);
		}
	}
	else
	{
		sprintf(str,"%s说:%s",pFromUser->GetUserName(),pszText);
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,str,TALKCOLOR_BLUE);

		sprintf(str,"[语聊频道]：%s说:%s",pFromUser->GetUserName(),pszText);
		g_TalkMgr.PushTalk(TALKTYPE_NORMAL,str,TALKCOLOR_BLUE);
	}
}

/// 收到服务器发送来的通用文本消息
void   CRoomEvent::OnRecvCommonMsg( WORD wMessageType, LPCTSTR pszText, WORD wLength )
{
	if(wLength > 1000 || pszText == "")
		return;

	char str[1024] = {0};
	strcpy(str,pszText);
	g_TalkMgr.PushTalk(TALKTYPE_VOICE,str,TALKCOLOR_RED);
}

/// 连接断开
void  CRoomEvent::OnNetDisConnect() 
{
	//显示与服务器断开连接
	g_TalkMgr.PushSysTalk("与语音服务器断开，请点击语音按钮重新进入！");
	g_VoiceAdapter.DisConnect();
}

/// 修改房间密码
void  CRoomEvent::OnChangeRoomPassWord( WORD wResult )
{
	if (wResult)
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"修改密码成功！",TALKCOLOR_BLUE);
	else
		g_TalkMgr.PushTalk(TALKTYPE_VOICE,"修改密码失败，请重新修改！",TALKCOLOR_BLUE);
}

/// 修改群密码
void  CRoomEvent::OnChangeGroupPassWord( WORD wResult )
{

}

/// 私聊语音
void   CRoomEvent::OnPrivateSpeak(WORD wResult, IUser * pUser )
{

}

/// 某一用户停止语音
void  CRoomEvent::OnStopVoiceData(IUser * pUser)
{
	if(!pUser)
		return;
	g_VoiceAdapter.ChangeUserStatus(false,pUser);
}

///  语音状态发生变化
void	CRoomEvent::OnSpeakStatusChange( IUser * pUser, BOOL bCanSpeak )
{
	if(!pUser)
		return;

	string str;
	if (!bCanSpeak)
		str = "你已被管理员禁言";
	else
		str = "你被允许发言";

	g_TalkMgr.PushTalk(TALKTYPE_VOICE,str.c_str(),TALKCOLOR_RED);
}

/// 取搜索服务器信息返回,在g_pRoomManager->GetSearchServer()执行后此函数会被回调
void CRoomEvent::OnGetSearchServer(WORD wResult, const char * pszAddress, int nPort, __int64 nSearchKey )
{
	if (wResult)
	{
		g_VoiceAdapter.InitializeSearchSever(pszAddress,nPort,nSearchKey);
	}
	//else
	//	g_VoiceAdapter.SetSearchSeverLoginStatus(false);
}

// 获取当前能语音的人数列表
void CRoomEvent::OnGetSpeakerList( WORD wResult, IUser ** pUser, DWORD dwUserCount )
{

}
// 得到房间修改名字结果
void CRoomEvent::OnChangeRoomName( WORD wResult, IRoom * pRoom )
{

}
// 修改房间说明 管理员功能
void CRoomEvent::OnChangeRoomText( WORD wResult, IRoom * pRoom )
{

}
// 删除房间
void CRoomEvent::OnDeleteRoom( WORD wResult , IRoom * pRoom )
{

}
// 修改模式
void CRoomEvent::OnChangeTalkMode( WORD wResult , IRoom * pRoom, int nMode)
{

}
// 设置管理员
void CRoomEvent::OnChangeManager( WORD wResult , IRoom * pRoom, IUser * pUser)
{

}
// 设置最大人数
void CRoomEvent::OnChangeMaxUser( WORD wResult , IRoom * pRoom, int nMaxUser)
{

}
// 设置房间类型
void CRoomEvent::OnChangeRoomType( WORD wResult , IRoom * pRoom, int nRoomType)
{

}
// 房间内用户权限发生变化
void CRoomEvent::OnUserTypeChange( WORD wResult, IUser * pUser, int nOldUserType )
{

}
// 获取用户的帐号
void CRoomEvent::OnGetUserPtId(WORD wResult, IUser* pUser, const char* pUserPtId)
{

}
// 申请（取消）麦序
void CRoomEvent::OnRequestMicOrder(WORD wResult, BOOL bEnable)
{

}
// 设置（增加/取消）麦序（管理员）
void CRoomEvent::OnSetMicOrder(WORD wResult, IUser* pUser, BOOL bEnable)
{

}
// 修改麦序（管理员）
void CRoomEvent::OnChangeMicOrder(WORD wResult, IUser* pUser, DWORD dwOldOrder, DWORD dwNewOrder)
{

}
// 获取麦序列表
void CRoomEvent::OnGetMicOrder(WORD wResult, IRoom* pRoom, IUser** ppUser, DWORD dwCount)
{

}
// 正在使用麦序的用户列表
void CRoomEvent::OnGetMicOrderUsed(WORD wResult, IRoom* pRoom, IUser** ppUser, DWORD dwCount)
{

}
// 设置多人同时使用麦克风（麦序）
void CRoomEvent::OnSetMicOrderNumber(WORD wResult, IRoom* pRoom, DWORD dwMicNum)
{

}
// 设置麦克风使用时间（麦序）
void CRoomEvent::OnSetMicOrderTime(WORD wResult, IRoom* pRoom, DWORD dwMicTime)
{

}
// 送鲜花
void CRoomEvent::OnVoteUser(WORD wResult, IUser* pUser)
{

}
// 开启/关闭送鲜花活动
void CRoomEvent::OnSetVoteUser(WORD wResult, IRoom* pRoom, BOOL bEnable)
{

}
// 获取鲜花用户列表
void CRoomEvent::OnGetListVoteUser(WORD wResult, IRoom* pRoom, IVoteUser** ppVoteUser, DWORD dwCount, LPCTSTR pStartTime, LPCTSTR pStopTime)
{

}
// 禁止/允许文字聊天
void CRoomEvent::OnUserForbiddenChat(WORD wResult, IRoom* pRoom, IUser* pUser, BOOL bFlag)
{

}

//在g_pGroupSpeaker->LogoutSearchSever执行后此函数会被回调
void CSearchEvent::OnLogin(BOOL bSucc, const char * pszErrorMessage,DWORD * pGameTypeArray,DWORD dwGameTypeCount,int nLevel)
{
	if (bSucc)
	{
		//处理获得的游戏列表
		g_VoiceAdapter.FillGameTypeVec(pGameTypeArray,dwGameTypeCount);
		//g_VoiceAdapter.SetSearchSeverLoginStatus(true);
		////弹出搜索框
		g_pControl->Msg(MSG_CTRL_VOICE_SEARCH_WND,OPER_CREATE);
	}
	else
	{
		//处理pszErrorMessage错误类型
		//g_VoiceAdapter.SetSearchSeverLoginStatus(false);
		g_TalkMgr.PushSysTalk("登陆搜索服务器失败！");
	}

	g_pControl->MsgToWnd("VoiceWnd",MSG_CTRL_VOICE_WND,1);
}

// 得到游戏区信息
void CSearchEvent::OnGetGameArea(BOOL bSucc,DWORD dwGameType,DWORD dwGameAreaCount,const char ** ppszAreaName)
{
	if (bSucc)
	{
		if (ppszAreaName == NULL || dwGameAreaCount == 0)
		{
			g_TalkMgr.PushSysTalk("获得游戏区信息失败！");
			//变灰搜索按钮
			return;
		}
		//保存游戏Type，处理获得的游戏区信息
		g_VoiceAdapter.FillGameAreaVec(dwGameType,dwGameAreaCount,ppszAreaName);
	}
	else
	{
		g_TalkMgr.PushSysTalk("获得游戏区信息失败！");
		g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_AREA_BUTTON_STATUS,1);
	}
}

// 得到游戏区组 信息
void CSearchEvent::OnGetGameGroup(BOOL bSucc,DWORD dwGameType,const char * pszAreaName,const char ** pszGameGroupName,int nGameGroupCount)
{
	if (bSucc)
	{
		//保存GameType，AreaName，处理Group信息
		g_VoiceAdapter.FillGameGroupVec(dwGameType,pszAreaName,pszGameGroupName,nGameGroupCount);
	}
	else
	{
		g_TalkMgr.PushSysTalk("获取游戏组信息失败！");
		g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_GROUP_BUTTON_STATUS,1);
	}
}

void CSearchEvent::OnGetGameRoomGroup(BOOL bSucc,DWORD dwGameType,const char * pszAreaName,const char * pszGameGroupName,IGameRoomGroup * pGameRoomGroup,int nGameRoomcount)
{
	if (bSucc)
	{
		if (pGameRoomGroup == NULL)
		{
			g_TalkMgr.PushSysTalk("获得频道列表失败!");
			return;
		}
		//若需要，保存游戏信息dwGametype,AreaName等信息
		g_VoiceAdapter.FillGameGroupTipVec(dwGameType,pszAreaName,pszGameGroupName,pGameRoomGroup,nGameRoomcount);
	}
	else 
	{
		g_TalkMgr.PushSysTalk("进入指定游戏组失败!");
	}
}

// 得到选定房间的IP，端口和key
void CSearchEvent::OnEnterGameGroup(BOOL bSucc,DWORD dwGameType,const char * pszAreaName,const char * pszGroupName,const char * pszAddress,int nPort, DWORD dwRoomID,__int64 nLoginKey)	 
{
	if (bSucc)
	{
		char tempKey[64] = {0};
		sprintf(tempKey,"%I64d",nLoginKey);

		g_VoiceAdapter.SetLoginKey(tempKey);
		g_VoiceAdapter.SetIP(pszAddress,nPort);
		g_VoiceAdapter.SetGameGroupId(dwRoomID);
		////重联login
		g_VoiceAdapter.ReLogin();
		g_VoiceAdapter.LogoutSearchSever();		
	}
	g_pControl->PopupWindow(MSG_CTRL_VOICE_SEARCH_WND,OPER_CLOSE);
}

// 得到推荐房间
void CSearchEvent::OnGetRecommendRoomList(BOOL bSucc,IGameRoomGroupInfo* pGroupInfo)
{

}
// 设置推荐房间
void CSearchEvent::OnSetRecommendRoom(BOOL bSucc,DWORD dwGameType,const char* pszAreaName,const char* pszGroupName,DWORD dwRoomId)
{

}
// 删除推荐房间
void CSearchEvent::OnClearRecommendRoom(BOOL bSucc,DWORD dwGameType,const char* pszAreaName,const char* pszGroupName,DWORD dwRoomId)
{

}
// 搜索指定房间
void CSearchEvent::OnSearchRoomById(BOOL bSucc,IGameRoomGroupInfo* pGroupInfo)
{

}





#endif