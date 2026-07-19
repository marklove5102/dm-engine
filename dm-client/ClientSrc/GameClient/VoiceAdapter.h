#pragma once

#ifndef _CHAT
#define _CHAT
#endif

#include "VoiceProxy.h"
#include "Global/StdHeaders.h"

#ifdef _CHAT

#define VOL_SIZE  5461
#define VOL_MAX   12
#define VOL_LEVEL_MAX  256


typedef vector<IRoom*> RoomVector;

struct UserInfo
{
	UserInfo()
	{
		pUser = NULL;
		bSpeaking = FALSE;
	}

	IUser*  pUser;
	BOOL    bSpeaking;//是否语聊中
};

typedef vector<UserInfo> UserVector;

//搜索服务器信息
struct Group
{
	string strGameGroupName; 
	DWORD dwGameRoomId;
};
typedef vector<Group> GroupTip;

struct GameGroup 
{
	string strGameGroup;
	GroupTip vGroupTip;
};

typedef vector<GameGroup> GroupInfo;

struct  GameAreaInfo
{
	string strGameArea;
	GroupInfo vGameGroup;
};
typedef vector<GameAreaInfo> VVGameAreaInfo;

struct GameInfo 
{
	DWORD dwGameId;
	string strGameName;
	VVGameAreaInfo gameArea;
};
typedef vector<GameInfo> VVGameInfo;


class CVoiceAdapter
{
	friend class CRoomEvent;
	friend class CSearchEvent;
public:
	CVoiceAdapter(void);
	~CVoiceAdapter(void);

	void        SetIP(const char* strIP,int port);
	int         GetPort(){return m_nPort;}
	void        SetMyGameInfo(int type,int area,int group);
	void        SetLoginKey(const char* strLoginKey){	m_strLoginKey.assign(strLoginKey);}
	BOOL        Init();
	void        ExitAdapter();
	bool        IsWaitServer(){return m_bIsWaitSever;}
	void        SetWaitServer(bool bWaitServer){m_bIsWaitSever = bWaitServer;}

	RoomVector& GetRoomArray(){ return m_VRoom; }
	RoomVector& GetGuildRoomArray(){ return m_VGuildRoom; }
	UserVector& GetUserArray(){ return m_VUser; }
	void        ChangeUserStatus(BOOL bSpeaking, IUser * pUser);
	void        DeleteUser(IUser* pUser);
	const char* GetCurRoomName(){ if(m_pCurRoom) return m_pCurRoom->GetRoomName(); else return "";}
	VVGameInfo& GetGameInfoVec(){return m_VGameInfo;}

	bool        IsWaiting(){  return m_bIsWaiting; }
	bool        IsConnected() { return m_bConnected; }
	bool        IsInRoom()    { return (m_bConnected && m_pCurRoom != NULL); }
	void        SetTeamRoomID(DWORD id){ m_dwTeamRoomID = id; }
	DWORD       GetTeamRoomID(){ return m_dwTeamRoomID; }

	bool        SearchServer(); //搜索

	bool        EnterRoom(int i,const char* strPassword = NULL,int iType = 0); //进入房间
	void        LeaveRoom(bool bRequestRoom = true); //离开当前频道,bRequestRoom是否重新请求房间列表
	void        RequestRoom();
	void        RequestUserList();

	void        DisConnect();

	int         GetSpeakVol();
	int         GetMicVol();
	void        SetSpeakVol(int iVol);
	void        SetMicVol(int iVol);
	void        IncSpeakVol(int i = 0);
	void        IncMicVol(int i = 0);
	int         GetSpeakLevel();
	int         GetMicLevel();
	void        SetSpeakWay(int iSpeakWay,DWORD dwData);
	int         GetSpeakWay(){return m_iSpeakWay;}
	void        SetGameGroupId(DWORD dwGroupId){m_dwGroupId = dwGroupId;}
	DWORD       GetSpeakVoiceActiLevel(){return m_dwVoiceActiLevel;}
	void        SetWaitLoginSearchStartTime(DWORD dwTime){m_dwWaitLoginSearchStartTime = dwTime;}
	DWORD       GetWaitLoginSearchStartTime(){return m_dwWaitLoginSearchStartTime;}
	void        SetWaitLoginVoiceSeverStartTime(DWORD dwTime){m_dwWaitLoginVoiceSeverStartTime = dwTime;}
	DWORD       GetWaitLoginVoiceSeverStartTime(){return m_dwWaitLoginVoiceSeverStartTime;}
	void        SearchSeverLogin();
	void        LogoutSearchSever();
	void        SelectGameType(int iSelCol);
	void        SelectGameGroup(int iTypeSelCol,int iAreaSelCol,int iGroupSelCol,int nPageIndex = 0);
	void        SelectGameArea(int iTypeSecCol,int iAreaSelCol, const char * pszGameAreaName);
	void        EnterGameRoomGroup(DWORD iSelCol,DWORD dwGameAreaNum,DWORD dwGameGroupNum);
	void        SetGameAreaName(const char * tempAreaName){m_strGameAreaName.assign(tempAreaName);}
	void        SetGameGroupName(const char * tempGroupName){m_strGameGroupName.assign(tempGroupName);}
	const char* GetGameAreaName(){return m_strGameAreaName.c_str();}
	const char* GetGameGroupName(){return m_strGameGroupName.c_str();}
	void        ChangeIsInMyGameStatus();
	bool        IsInMyGame(){return m_bIsInMyGame;}
	void        ReLogin();
	void        ReLoginKeepOn();
	bool        IsInTeamRoom(){return (m_bConnected && m_pCurRoom != NULL && m_pCurRoom->GetRoomType() == 3);}
	void        SetTeamRoomPassword(const char* str){ m_strTeamRoomPassword.assign(str); }
	const char* GetTeamRoomPassword(){ return m_strTeamRoomPassword.c_str(); }
	void        CreateTeamVoiceRoom();
	void        EnterTeamRoom();
	void        LeaveTeamRoom();
	bool        IsCreateTeamRoomAfterLeave(){return m_bCreatTeamRoomAfterLeave;}
	void        SetCreateTeamRoomAfterLeave(bool b){m_bCreatTeamRoomAfterLeave = b;}
	bool        IsEnterTeamRoomAfterLeave(){return m_bEnterTeamRoomAfterLeave;}
	void        SetEnterTeamRoomAfterLeave(bool b){m_bEnterTeamRoomAfterLeave = b;}
	bool        IsLoginOutAfterLeaveRoom(){return m_bLoginOutAfterLeaveRoom;}
	void        SetLoginOutAfterLeaveRoom(bool b){m_bLoginOutAfterLeaveRoom = b;}
	void        SendTextMessage(const char * pszText,int iLen,IUser * pToUser);
	void        SetSuperMic(bool bMicEnhance);
	void        SetGameVoice(bool b){m_bGameVoice;}
	bool        GetGameVoice(){return m_bGameVoice;}
	DWORD       GetUserType();
	void        SetUserType(DWORD dwUserType);

	void        InitializeSearchSever(const char *SearchSeverIp,int iPort,__int64 LoginKey);
	void        FillGameTypeVec(DWORD * dwGameTypeArray,int GameNum);
	void        FillGameAreaVec(DWORD dwGameType,DWORD dwGameAreaCount,const char ** ppszAreaName);
	void        FillGameGroupVec(DWORD dwGameType,const char * pszAreaName,const char ** pszGameGroupName,int nGameGroupCount);
	void        FillGameGroupTipVec(DWORD dwGameType,const char * pszAreaName,const char * pszGameGroupName,IGameRoomGroup * pGameRoomGroup,int nGameRoomcount);

	void        SortRoom();

private:
	bool		m_bConnected;
	bool        m_bIsWaitSever;
	bool		m_bIsWaiting; //等待回调
	bool        m_bIsInMyGame;
	bool        m_bCreatTeamRoomAfterLeave;
	bool        m_bEnterTeamRoomAfterLeave;
	bool        m_bLoginOutAfterLeaveRoom;
	RoomVector  m_VRoom,m_VGuildRoom;
	UserVector	m_VUser;
	IRoom*		m_pCurRoom; //当前房间
	int         m_iPageIndex; //
	int         m_iSpeakWay;
	DWORD       m_dwVoiceActiLevel;
	DWORD       m_dwWaitLoginSearchStartTime;
	DWORD       m_dwWaitLoginVoiceSeverStartTime;
	map<int,string> m_MGameInfo;  
	VVGameInfo  m_VGameInfo; 

	string      m_strSearchIP;
	int         m_nSearchPort;
	string      m_strSearchKey;  //搜索的登录Key
	string      m_strGameAreaName;
	string      m_strGameGroupName;
	string      m_strTeamRoomPassword;
	DWORD		m_dwLastSearchServer; //上次搜索服务器的时间

	string		m_strIP;
	int			m_nPort;
	DWORD		m_dwTeamRoomID;//组队语聊房间id
	int         m_iMyGameType,m_iMyGameArea,m_iMyGameGroup;//自身游戏的信息
	DWORD       m_dwGroupId;
	int         m_iGameType,m_iGameArea,m_iGameGroup;//跨服语聊的游戏信息
	string      m_strLoginKey;		// 登陆语音服务器钥匙
	string      m_strLoginSearchKey;//登陆搜索服务器key
	int         m_iLocalPort;
	HMODULE     m_hDll;
	bool        m_bGameVoice; //记录进入房间前游戏声音开关状态
	DWORD       m_dwUserType;

	void LoadSearchConfig();
};

extern CVoiceAdapter g_VoiceAdapter;

extern IGroupSpeaker *   g_pGroupSpeaker;	//接口主对象（通过dll唯一暴露的接口获得）
extern ISearchServer *   g_pSearchSever;   //搜索引擎
extern IRoomManager*     g_pRoomManager;	//语音服务主应用服务器
extern ISpeak*		     g_pSpeak;		//语音引擎（实现语音的采集和播放）
extern IRoomEvent*       g_pRoomEvent;
extern ISearchEvent*     g_pSearchEvent;
#endif
