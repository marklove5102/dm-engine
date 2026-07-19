#include "VoiceAdapter.h"
#include "GameData/GameData.h"
#include "GameData/LoginData.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/StreamInterface.h"
#include "GameData/TalkMgr.h"
#include <algorithm>

#ifdef _CHAT

typedef IGroupSpeaker* (__stdcall * LPCreateGroupSpeaker)();
CVoiceAdapter g_VoiceAdapter;

//通过dll导出的接口
IGroupSpeaker *   g_pGroupSpeaker = NULL;	//接口主对象（通过dll唯一暴露的接口获得）
ISearchServer *   g_pSearchSever  = NULL;   //搜索引擎
IRoomManager*     g_pRoomManager  = NULL;	//语音服务主应用服务器
ISpeak*		      g_pSpeak        = NULL;		//语音引擎（实现语音的采集和播放）
IRoomEvent*       g_pRoomEvent;
ISearchEvent*     g_pSearchEvent;

bool SGroupGreater ( IRoom* elem1, IRoom *elem2 )
{
	if(elem1->GetRoomType() > elem2->GetRoomType())
		return true;
	else if ((elem1->GetRoomType() == elem2->GetRoomType()) && (elem1->HasPassword()))
		return true;
	return false;
}

CVoiceAdapter::CVoiceAdapter(void)
{
	m_bConnected = false;
	m_bIsWaiting = false;
	m_bIsWaitSever = false;
	m_bIsInMyGame = true;
	m_bCreatTeamRoomAfterLeave = false;
	m_bEnterTeamRoomAfterLeave = false;
	m_bLoginOutAfterLeaveRoom = false;
	m_pCurRoom  = NULL;
	m_nPort     = 0;
	m_dwGroupId = 0;
	m_strIP.clear();
	m_VGuildRoom.clear();

	m_iMyGameType = 0;
	m_iMyGameArea = 0;
	m_iMyGameGroup = 0;
	m_iGameType = 0;
	m_iGameArea = 0;
	m_iGameGroup = 0;
	m_iPageIndex = 0;
	m_dwWaitLoginSearchStartTime = 0;
	m_dwWaitLoginVoiceSeverStartTime = 0;
	m_dwUserType = -1;

	m_iLocalPort = 9712;
	m_hDll = NULL;

	m_strSearchIP.clear();
	m_nSearchPort = 0;
	m_strSearchKey.clear();
	m_dwLastSearchServer = 0;
}

CVoiceAdapter::~CVoiceAdapter(void)
{
	ExitAdapter();

	if(m_hDll)
	{
		TRY_BEGIN
			FreeLibrary(m_hDll);
		TRY_END
	}

	SAFE_DELETE(g_pRoomEvent);
	SAFE_DELETE(g_pSearchEvent);
	m_dwGroupId = 0;
	m_dwTeamRoomID = 0;
	m_VGuildRoom.clear();
}

void CVoiceAdapter::SetIP(const char* strIP,int port)
{
	m_strIP.assign(strIP);
	m_nPort = port;
}

void CVoiceAdapter::SetMyGameInfo(int type,int area,int group)
{
	m_iMyGameType = type;
	m_iMyGameArea = area;
	m_iMyGameGroup = group;
	m_iGameType = m_iMyGameType;
}

void  CVoiceAdapter::ChangeUserStatus(BOOL bSpeaking, IUser * pUser)
{
	UserVector::iterator it = m_VUser.begin();
	for (;it != m_VUser.end();it ++)
	{
		if(pUser == (*it).pUser)
		{
			if (bSpeaking)
				(*it).bSpeaking = true;
			else
				(*it).bSpeaking = false;

			return;
		}
	}
}

void CVoiceAdapter::DeleteUser(IUser* pUser)
{
	UserVector::iterator itr;
	for(itr = m_VUser.begin();itr != m_VUser.end();itr++)
	{
		if((*itr).pUser == pUser)
		{
			m_VUser.erase(itr);
			break;
		}
	}
}

BOOL CVoiceAdapter::Init()
{
	if(m_strIP.empty() ||  m_nPort == 0)
		return FALSE;

	if(g_pGroupSpeaker)
		return TRUE;

	if(m_hDll == NULL)
		m_hDll = ::LoadLibrary("speakcore.dll");

    LoadSearchConfig();
	m_bLoginOutAfterLeaveRoom = false;

    LPCreateGroupSpeaker lpCreateSpeaker = NULL;
	lpCreateSpeaker = (LPCreateGroupSpeaker)GetProcAddress(m_hDll,"CreateGroupSpeaker");
	if(lpCreateSpeaker)
	{
		g_pGroupSpeaker = (*lpCreateSpeaker)();
	}

	if(!g_pGroupSpeaker)
		return FALSE;

	// 初始化语音引擎（实现声音采集和播放等功能）
	g_pSpeak = g_pGroupSpeaker->CreateSpeaker();
	if(!g_pSpeak)
		return FALSE;

	//先随便指定一个，如果占用，内部会自动调整
	g_pSpeak->Init(m_strIP.c_str(),m_nPort,++m_iLocalPort);	//指定一个本地的udp端口 实际使用时可能要先检测是否已经被占用

	TRY_BEGIN
		if(g_pRoomEvent == NULL)
			g_pRoomEvent = new CRoomEvent();

		string strUser = g_Login.GetLoginID();

		g_pRoomManager = g_pSpeak->LoginEx(m_iMyGameType,m_iMyGameArea,m_iMyGameGroup,m_dwGroupId,strUser.c_str()
			,m_strLoginKey.c_str(),SELF.GetName(),g_pRoomEvent);

		m_iSpeakWay = g_pStreamMgr->GetConfigInt("ActivateMode",2);
		m_dwVoiceActiLevel = g_pStreamMgr->GetConfigInt("ActivateLevel",17);

	TRY_END_RETURN(FALSE)
	return TRUE;
}

void CVoiceAdapter::ExitAdapter()
{
	if(m_bConnected)
	{
		g_pSpeak->LogOut();//关闭语音连接
		g_pSpeak->Release();
		g_pGroupSpeaker->Release();
	}

	LogoutSearchSever();
	m_nPort = 0;
	m_strIP.clear();
	g_pGroupSpeaker = NULL;	
	g_pRoomManager = NULL;	
	g_pSpeak = NULL;		
	m_bConnected = false;
	m_VRoom.clear();
	m_VGuildRoom.clear();
	m_VUser.clear();
	m_bLoginOutAfterLeaveRoom = false;
	m_dwTeamRoomID = 0;
}

bool CVoiceAdapter::SearchServer()
{
	if(IsInRoom())
		return false;

	if(GetTickCount() - m_dwLastSearchServer < 2000) //距离上次搜索的时间2秒以内
		return true;

	m_dwLastSearchServer = GetTickCount();

	if(m_nSearchPort == 0 || m_strSearchIP.empty()) //没有搜索服务器的地址
	{
		if(m_bConnected)
			g_pRoomManager->GetSearchServer(); //获得搜索服务器的地址和端口
		return false;
	}

	return true;
}

bool CVoiceAdapter::EnterRoom(int i,const char* strPassword,int iType)
{
	if(i < 0 )
		return false;
	if(iType == 0 && i >= m_VRoom.size())
		return false;
	if(iType == 1 && i >= m_VGuildRoom.size())
		return false;

	if(!m_bConnected) //还没有进入，直接结束
		return false;

	IRoom* pRoom = NULL;
	if(iType == 0)
		pRoom = m_VRoom.at(i);
	else if(iType == 1)
		pRoom = m_VGuildRoom.at(i);

	if(pRoom == NULL)
		return false;

	if(m_pCurRoom)
	{
		if(m_pCurRoom == pRoom)
		{
			g_pControl->PopupWindow(MSG_CTRL_VOICE_WND,OPER_CLOSE);
			g_pControl->PopupWindow(MSG_CTRL_VOICE_CHAT_WND,OPER_CREATE);
		}
		else
		{
			g_MsgBoxMgr.PopSimpleAlert("在进入其他频道之前，请先退出当前语音频道!");
		}
	}
	else
	{
		if(pRoom->HasPassword() && strPassword == NULL)
		{
			g_pControl->PopupWindow(MSG_CTRL_VOICE_PASSWORD_WND,OPER_CREATE,i);
			g_pControl->MsgToWnd(MSG_CTRL_VOICE_PASSWORD_WND,MSG_CTRL_VOICE_PASSWORD_WND,iType);
		}
		else
		{
			pRoom->Enter(strPassword); //进入频道
			m_bIsWaiting = true;
		}
	}
	return true;
}

void CVoiceAdapter::LeaveRoom(bool bRequestRoom)
{
	if (m_pCurRoom == NULL) 
		return;

	m_VUser.clear();
	m_pCurRoom->Leave();
	m_pCurRoom = NULL;
	if(bRequestRoom)
		this->RequestRoom();
}

void CVoiceAdapter::RequestRoom()
{
	if(g_pRoomManager == NULL)
		return;

	TRY_BEGIN
		g_VoiceAdapter.GetRoomArray().clear();
		g_VoiceAdapter.GetGuildRoomArray().clear();
		g_pRoomManager->GetRoomList(m_iPageIndex);
	TRY_END
}

void CVoiceAdapter::RequestUserList()
{
	if(m_pCurRoom == NULL || !m_bConnected)
		return;

	m_pCurRoom->GetUserList();
}

void CVoiceAdapter::DisConnect()
{
	SetIP("",0);
	m_bConnected = false;

	g_VoiceAdapter.LeaveRoom();
	//g_VoiceAdapter.SetRoomEnterStatus(false);
	g_pControl->PopupWindow(MSG_CTRL_VOICE_CHAT_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_VOICE_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_VOICE_SEARCH_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_VOICE_PASSWORD_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_VOICE_ADD_WND,OPER_CLOSE);
	ExitAdapter();
}

int CVoiceAdapter::GetSpeakVol()
{
	if(g_pSpeak == NULL || !m_bConnected)
		return 0;

	return (g_pSpeak->GetSpeakVol() / VOL_SIZE);
}

int CVoiceAdapter::GetMicVol()
{
	if(g_pSpeak == NULL || !m_bConnected)
		return 0;

	return (g_pSpeak->GetMicVol() / VOL_SIZE);
}

void CVoiceAdapter::SetSpeakVol(int iVol)
{
	if(g_pSpeak == NULL )
		return;
	if(iVol < 0)
		iVol = 0;
	else if(iVol > 12)
		iVol = 12;

	return (g_pSpeak->SetSpeakVol(iVol * VOL_SIZE));
}

void CVoiceAdapter::SetMicVol(int iVol)
{
	if(g_pSpeak == NULL)
		return;
	if(iVol < 0)
		iVol = 0;
	else if(iVol > 12)
		iVol = 12;

	return (g_pSpeak->SetMicVol(iVol * VOL_SIZE));
}

void CVoiceAdapter::IncSpeakVol(int i)
{
	if(g_pSpeak == NULL || !m_bConnected || i == 0)
		return;

	int iVol = i + (g_pSpeak->GetSpeakVol() / VOL_SIZE);
	if(iVol > VOL_MAX)
		iVol = VOL_MAX;
	else if(iVol < 0)
		iVol = 0;

	g_pSpeak->SetSpeakVol(iVol*VOL_SIZE);
}

void CVoiceAdapter::IncMicVol(int i)
{
	if(g_pSpeak == NULL || !m_bConnected || i == 0)
		return;

	int iVol = i + (g_pSpeak->GetMicVol() / VOL_SIZE);
	if(iVol > VOL_MAX)
		iVol = VOL_MAX;
	else if(iVol < 0)
		iVol = 0;

	g_pSpeak->SetMicVol(iVol*VOL_SIZE);
}

int CVoiceAdapter::GetSpeakLevel()
{
	if(g_pSpeak == NULL)
		return 0;
	return g_pSpeak->GetSpeakLevel();
}

int CVoiceAdapter::GetMicLevel()
{
	if(g_pSpeak == NULL)
		return 0;

	return g_pSpeak->GetMicLevel();
}

void CVoiceAdapter::InitializeSearchSever(const char *SearchSeverIp,int iPort,__int64 LoginKey)
{
	if(!g_pGroupSpeaker || !SearchSeverIp)
		return;

	char tmp[32] = {0};
	sprintf(tmp,"%I64d",LoginKey);
	m_strLoginSearchKey.assign(tmp);
	m_nSearchPort = iPort;

	if(g_pSearchEvent == NULL)
		g_pSearchEvent = new CSearchEvent();

	g_pSearchSever = g_pGroupSpeaker->CreateSearchServer(SearchSeverIp,iPort,g_pSearchEvent);
	m_dwWaitLoginSearchStartTime = GetTickCount();
}

void CVoiceAdapter::SetSpeakWay(int iSpeakWay,DWORD dwData)
{
	if (m_bConnected && IsInRoom())
	{
		m_iSpeakWay = iSpeakWay;
		m_dwVoiceActiLevel = dwData;
		if(g_pSpeak)
			g_pSpeak->SetSpeakWay(iSpeakWay,dwData);
	}
}

void CVoiceAdapter::LoadSearchConfig()
{
	char pBuff[100] = {0};
	char strTemp[64] = {0};
	int iGameId;
	DWORD temp;
	char szPath[1024] = {0};
	sprintf(szPath,"%s\\gameconfig.ini",GetGameDataDir());
	int iGameCount = (int)GetPrivateProfileInt("GameConfig","GameCount",0,szPath);
	if (iGameCount < 0)
		return;

	for(int i=0;i< iGameCount;i++)//游戏总数
	{
		sprintf(pBuff,"Game_%d",i);
		temp = GetPrivateProfileString(pBuff,"GameName","",strTemp,64,szPath);
		iGameId = (int)GetPrivateProfileInt(pBuff,"GameId",0,szPath);
		m_MGameInfo[iGameId] = strTemp;
	}
}

void CVoiceAdapter::ChangeIsInMyGameStatus()
{
	if (m_iMyGameType == m_iGameType 
		&& strcmp(m_strGameAreaName.c_str(),g_Login.GetAreaName()) == 0
		&& strcmp(m_strGameGroupName.c_str(),g_Login.GetGroupName()) == 0)
	{
		m_bIsInMyGame = true;
	}
	else
		m_bIsInMyGame= false;
}

void CVoiceAdapter::SearchSeverLogin()
{
	if (m_nSearchPort == 0 || g_pSearchSever == NULL)
		return;

	g_pSearchSever->Login(g_Login.GetLoginID(),m_iMyGameType,m_iMyGameArea,m_iMyGameGroup,m_strLoginSearchKey.c_str());

}

void CVoiceAdapter::LogoutSearchSever()
{
	if(g_pSearchSever)
	{
		g_pSearchSever->Release();	//主动登出
		g_pSearchSever = NULL;
	}	
	//m_bIsLoginSearchServer = false;
	m_dwWaitLoginSearchStartTime = 0;
	m_VGameInfo.clear();
}

void CVoiceAdapter::SelectGameType(int iSelCol)
{
	if (iSelCol < 0 || iSelCol > m_VGameInfo.size())
		return;

	if (m_VGameInfo[iSelCol].gameArea.size() == 0)
	{
		g_pSearchSever->SelectGame(m_VGameInfo[iSelCol].dwGameId);
		return;
	}
	//调用ChangeAreaButtonStatus函数
	g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_AREA_BUTTON_STATUS,0);
}

void CVoiceAdapter::SelectGameGroup(int iTypeSelCol,int iAreaSelCol,int iGroupSelCol,int nPageIndex)
{
	if (iTypeSelCol < 0 || iTypeSelCol > m_VGameInfo.size())
	{
		return;
	}

	if (m_VGameInfo[iTypeSelCol].gameArea[iAreaSelCol].vGameGroup[iGroupSelCol].vGroupTip.size() == 0)
	{
		g_pSearchSever->SelectGameGroup(m_VGameInfo[iTypeSelCol].dwGameId,
			m_VGameInfo[iTypeSelCol].gameArea[iAreaSelCol].strGameArea.c_str(),
			m_VGameInfo[iTypeSelCol].gameArea[iAreaSelCol].vGameGroup[iGroupSelCol].strGameGroup.c_str(),
			nPageIndex);
		return;
	}
	g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_GROUP_BUTTON_STATUS,2);
}

void CVoiceAdapter::SelectGameArea(int iTypeSecCol,int iAreaSelCol, const char * pszGameAreaName)
{
	if(iTypeSecCol < 0 || iTypeSecCol > m_VGameInfo.size())
	{
		return;
	}

	if (m_VGameInfo[iTypeSecCol].gameArea[iAreaSelCol].vGameGroup.size() == 0)
	{
		g_pSearchSever->SelectGameArea(m_VGameInfo[iTypeSecCol].dwGameId,pszGameAreaName);
		return;
	}
	//调用ChangeGroupButtonStatus函数
	g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_GROUP_BUTTON_STATUS,0);
}

void CVoiceAdapter::EnterGameRoomGroup(DWORD iSelCol,DWORD dwGameAreaNum,DWORD dwGameGroupNum)
{
	m_iGameType = m_VGameInfo[iSelCol].dwGameId;
	m_iGameArea = dwGameAreaNum;
	m_iGameGroup = dwGameGroupNum;
	SetGameAreaName(m_VGameInfo[iSelCol].gameArea[dwGameAreaNum].strGameArea.c_str());
	SetGameGroupName(m_VGameInfo[iSelCol].gameArea[dwGameAreaNum].vGameGroup[dwGameGroupNum].strGameGroup.c_str());
	DWORD tempGameGroupId = m_VGameInfo[iSelCol].gameArea[dwGameAreaNum].vGameGroup[dwGameGroupNum].vGroupTip[0].dwGameRoomId;
	
	g_pSearchSever->EnterGameRoomGroup(m_iGameType,m_strGameAreaName.c_str(),m_strGameGroupName.c_str(),tempGameGroupId);
}

void CVoiceAdapter::FillGameTypeVec(DWORD * dwGameTypeArray,int GameNum)
{
	if (m_MGameInfo.empty())
	{
		g_TalkMgr.PushSysTalk("读取配置文件失败！");
		return;
	}

	GameInfo gmeInfo;
	DWORD id;
	map<int,string>::iterator iter;
	for (int i = 0;i < GameNum;i++)
	{
		id = *(dwGameTypeArray);
		iter = m_MGameInfo.find(id);
		if (iter != m_MGameInfo.end())
		{
			gmeInfo.dwGameId = id;
			gmeInfo.strGameName = iter->second;
			m_VGameInfo.push_back(gmeInfo);
		}
		dwGameTypeArray++;
	}
}


void CVoiceAdapter::FillGameAreaVec(DWORD dwGameType,DWORD dwGameAreaCount,const char ** ppszAreaName)
{
	if (m_VGameInfo.size() == 0)
	{
		return;
	}
	GameAreaInfo vGameArea;
	for (int i = 0;i < m_VGameInfo.size();i++)
	{
		if (dwGameType == m_VGameInfo[i].dwGameId)
		{
			for (int j=0;j<dwGameAreaCount;j++)
			{
				vGameArea.strGameArea.assign(*ppszAreaName);
				m_VGameInfo[i].gameArea.push_back(vGameArea);
				ppszAreaName++;
			}
			//调用ChangeAreaButtonStatus函数
			g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_AREA_BUTTON_STATUS,0);
			return;
		}
	}
}

void CVoiceAdapter::FillGameGroupVec(DWORD dwGameType,const char * pszAreaName,const char ** pszGameGroupName,int nGameGroupCount)
{
	if (m_VGameInfo.size() == 0)
		return;

	GameGroup tempGroup;
	for (int i = 0;i < m_VGameInfo.size();i++)
	{
		if (dwGameType == m_VGameInfo[i].dwGameId)
		{
			int iAreaNum = m_VGameInfo[i].gameArea.size();
			for (int j = 0;j < iAreaNum;j++)
			{
				if(strcmp(m_VGameInfo[i].gameArea[j].strGameArea.c_str(),pszAreaName) == 0)
				{
					for (int ii = 0;ii < nGameGroupCount;ii++)
					{
						tempGroup.strGameGroup.assign(*pszGameGroupName);
						m_VGameInfo[i].gameArea[j].vGameGroup.push_back(tempGroup);
						pszGameGroupName++;
					}
					//调用ChangeGroupButtonStatus函数
					g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_GROUP_BUTTON_STATUS,0);
					return;
				}

			}
		}
	}
}

void CVoiceAdapter::FillGameGroupTipVec(DWORD dwGameType,const char * pszAreaName,const char * pszGameGroupName,IGameRoomGroup * pGameRoomGroup,int nGameRoomcount)
{
	if (m_VGameInfo.size() == 0)
		return;

	Group tempGroupTip;
	for (int i=0;i < m_VGameInfo.size();i++)
	{
		if (dwGameType == m_VGameInfo[i].dwGameId)
		{
			int iAreaNum = m_VGameInfo[i].gameArea.size();
			for (int j = 0;j < iAreaNum;j++)
			{
				if(strcmp(m_VGameInfo[i].gameArea[j].strGameArea.c_str(),pszAreaName) == 0)
				{
					int iGroupNum = m_VGameInfo[i].gameArea[j].vGameGroup.size();
					for (int ii = 0;ii < iGroupNum;ii++)
					{
						if (strcmp(m_VGameInfo[i].gameArea[j].vGameGroup[ii].strGameGroup.c_str(),pszGameGroupName) == 0)
						{
							for (int iii=0;iii<nGameRoomcount;iii++)
							{
								tempGroupTip.strGameGroupName.assign(pGameRoomGroup->GetName());
								tempGroupTip.dwGameRoomId = pGameRoomGroup->GetRoomGroupId();
								m_VGameInfo[i].gameArea[j].vGameGroup[ii].vGroupTip.push_back(tempGroupTip);
								pGameRoomGroup = pGameRoomGroup->GetNextRoomGroup();
							}
							//调用ChangeGroupTipButtonStatus函数
							g_pControl->MsgToWnd("VoiceSearchWnd",MSG_CTRL_CHANGE_GROUP_BUTTON_STATUS,2);
							return;
						}
					}
				}
			}
		}
	}
}

void CVoiceAdapter::ReLogin()
{
	if(IsInRoom())
	{
		if(g_pSearchSever)
		{
			g_TalkMgr.PushSysTalk("你已经在一个语音频道中，要想登陆其他服务器，请先退出语音频道!");
		}
		return ;
	}
	m_VRoom.clear();
	m_VGuildRoom.clear();
	if(g_pSpeak != NULL)
	{
		if (g_pSpeak->IsConnected())
		{
			g_pSpeak->LogOut();
		}
		g_pSpeak->Release();
		g_pSpeak = NULL;
		m_bConnected = false;
		m_dwWaitLoginVoiceSeverStartTime = GetTickCount();
	}
}

void CVoiceAdapter::ReLoginKeepOn()
{
	if(IsInRoom())
	{
		return ;
	}
	if(!g_pGroupSpeaker)
		return;

	TRY_BEGIN
		g_pSpeak = g_pGroupSpeaker->CreateSpeaker();
		g_pSpeak->Init(m_strIP.c_str(),m_nPort,++m_iLocalPort);

		g_pRoomManager = g_pSpeak->LoginEx(m_iMyGameType,m_iMyGameArea,m_iMyGameGroup,m_dwGroupId,g_Login.GetLoginID(),m_strLoginKey.c_str(),SELF.GetName(),g_pRoomEvent);
		m_iSpeakWay = g_pStreamMgr->GetConfigInt("ActivateMode",3);
		m_dwVoiceActiLevel = g_pStreamMgr->GetConfigInt("ActivateLevel",2);
	TRY_END
}

void CVoiceAdapter::CreateTeamVoiceRoom()
{
	if(g_pRoomManager == NULL || !m_bConnected)
		return;

	char szPassWord[64]={0};
	sprintf(szPassWord,"%d",rand());
	g_pRoomManager->CreateTeamRoom(1,szPassWord);
	m_strTeamRoomPassword = szPassWord;
}

void CVoiceAdapter::EnterTeamRoom()
{
	if (m_dwTeamRoomID == 0)
	{
		g_TalkMgr.PushSysTalk("未组队或队长没有开启组队语聊,要进入其它语聊房间请点击面板上的语聊按钮");
		return;
	}

	if(m_bConnected && g_pRoomManager)
		g_pRoomManager->EnterTeamRoom(m_dwTeamRoomID,m_strTeamRoomPassword.c_str());
	else
	{
		g_TalkMgr.PushSysTalk("与语音服务器断开,正在重连,请稍候再试");
		if(g_pGroupSpeaker)
			ReLogin();
		else
			Init();
	}
}

void CVoiceAdapter::LeaveTeamRoom()
{
	if(IsInTeamRoom())
	{
		LeaveRoom();
		m_strTeamRoomPassword.clear();
	}
	m_dwTeamRoomID = 0;
}

void CVoiceAdapter::SendTextMessage(const char * pszText,int iLen,IUser * pToUser)
{
	if (!g_pRoomManager || !m_bConnected)
		return;	

	g_pRoomManager->SendChatText(pszText,iLen,pToUser);
}

void CVoiceAdapter::SetSuperMic(bool bMicEnhance)
{
	if (g_pSpeak && g_pSpeak->GetSuperMic() != 0)
	{
		g_pSpeak->SetSuperMic(bMicEnhance);
	}
}

void CVoiceAdapter::SortRoom()
{
	sort(m_VRoom.begin(),m_VRoom.end(),SGroupGreater);
	sort(m_VGuildRoom.begin(),m_VGuildRoom.end(),SGroupGreater);
}

DWORD CVoiceAdapter::GetUserType()
{
	return m_dwUserType;
}

void CVoiceAdapter::SetUserType(DWORD dwUserType)
{
	if (g_pRoomManager == NULL || !m_bConnected)
		return;

	if(dwUserType == 1)
	{
		if(g_pRoomManager->GetMyUser())
			m_dwUserType = (g_pRoomManager->GetMyUser())->GetUserType();
	}
	else if (dwUserType == 0)
	{
		m_dwUserType = -1;
	}
}


#endif
