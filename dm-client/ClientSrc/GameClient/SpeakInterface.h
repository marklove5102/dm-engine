#pragma once

#ifndef _CHAT
#define _CHAT
#endif

#ifdef _CHAT

class ISpeakClass
{
public:
	virtual void     __stdcall SetData( DWORD dwData ) = 0 ;
	virtual DWORD    __stdcall GetData() = 0 ;	
};

#define USER_DEFAULT         0  // 普通用户
#define USER_ROOM_MANAGER    1  // 房间管理员
#define USER_GROUP_MANAGER   2  // 群管理员 or GM


#define USER_FEMALE	1						// 女
#define USER_MALE	0						// 男

class ITeamManager;

class IVoteUser	: public ISpeakClass
{
public:
	//
	// 得到用户PT帐号
	//
	virtual LPCTSTR	__stdcall GetPtName() = 0;

	//
	// 得到用户昵称
	//
	virtual LPCTSTR	__stdcall GetNickName() = 0;

	//
	// 得到用户鲜花数目
	//
	virtual DWORD	__stdcall GetBallot() = 0;
};

//#define VOICE_TEST_RECORD_READY		0x1
//#define VOICE_TEST_RECORD_PENDING	0x1<<1
//#define VOICE_TEST_PLAY_READY		0x1<<2
//#define VOICE_TEST_PLAY_PENDING		0x1<<3
//
//class IVoiceTestEvent
//{
//public:
//	//
//	// VoiceTest状态回调
//	//
//	virtual void __stdcall OnStatusChange(int iFlag) = 0;
//
//	//
//	// 剩余时间回调
//	//
//	virtual void __stdcall OnTimeElapse(int iTime) = 0;
//};
//
//class IVoiceTest
//{
//public:
//	//
//	// 开始录音
//	//
//	virtual BOOL __stdcall	StartRecord() = 0;
//
//	//
//	// 停止录音
//	//
//	virtual BOOL __stdcall	StopRecord() = 0;
//
//	//
//	// 开始播放
//	//
//	virtual BOOL __stdcall StartPlay() = 0;
//
//	//
//	// 停止播放
//	//
//	virtual BOOL __stdcall StopPlay() = 0;
//
//	//
//	// 获取当前状态
//	//
//	virtual int	__stdcall GetStatus() = 0;	
//};

class  IUser : public ISpeakClass
{
public:
	//
	// 得到该玩家角色名
	//
	virtual LPCTSTR  __stdcall GetUserName( ) = 0 ;
	
	//
	// 是否是本游戏的玩家
	//
	virtual BOOL    __stdcall IsGameUser() = 0 ;

	//
	// 得到该用户的Id
	//
	virtual DWORD    __stdcall GetUserId( ) = 0 ;
	
	//
	// 获取用户类型(管理员,普通用户) 
	//
	virtual DWORD    __stdcall GetUserType( ) = 0 ;
	
	//
	// 是否被临时禁言
	//
	virtual BOOL     __stdcall IsForbided( ) = 0 ;
	
	//
	// 是否被禁言
	//
	virtual BOOL     __stdcall IsSpeakOff( ) = 0 ;
	
	//
	// 是否是我的听众
	//
	virtual BOOL     __stdcall IsMyReader( ) = 0 ;
	
	//
	// 是否允许听他说话
	//
	virtual BOOL     __stdcall IsMySpeaker() = 0 ;

	//
	// 得到该玩家性别
	//
	virtual DWORD    __stdcall GetUserSex() = 0 ; 

	//
	// 得到该玩家的游戏类型
	//
	virtual DWORD	__stdcall GetGameType() = 0 ;

	//
	// 得到该玩家的游戏区
	//
	virtual DWORD	__stdcall GetGameArea() = 0 ;

	//
	// 得到该玩家的游戏区
	//
	virtual DWORD	__stdcall GetGameGroup() = 0 ;

	//
	// 是否允许文字聊天
	//
	virtual BOOL __stdcall CanBeChat() = 0;
};

#define DEFAULT_ROOM_TYPE		0   // 普通房间
#define VIP_ROOM_TYPE			1   // VIP房间
#define COMMAND_ROOM_TYPE		2	// 大型房间 200人
#define TEAM_ROOM_TYPE			3	// 组队房间 30
#define TEAM_US_ROOM_TYPE		4	// 工会房间


#define FREE_TALK_MODE			0	// 自由模式
#define LIMITED_TALK_MODE		1	// 限制模式，管理员设置
#define MIC_CONCTR_MODE			2	// 排MIC模式

class  IRoom : public ISpeakClass
{
public:
	virtual BOOL     __stdcall Enter( LPCSTR pszPwd ) = 0 ;
    
	virtual BOOL     __stdcall Leave( ) = 0 ;

    virtual BOOL     __stdcall GetUserList() = 0 ;

	virtual LPCTSTR  __stdcall GetRoomName( ) = 0 ;
	virtual DWORD    __stdcall GetRoomId( ) = 0 ;
	/// 当前用户数
	virtual BYTE     __stdcall GetUserCount( ) = 0 ;	
	/// 获取房间最大用户数
	virtual BYTE     __stdcall GetMaxUser( ) = 0 ;
	/// 获取房间类型
	virtual BYTE     __stdcall GetRoomType( ) = 0 ;
	/// 获取房间状态
	virtual BYTE     __stdcall GetRoomStatus( ) = 0 ; 
	/// 获取房间附加信息(描述等)
	virtual LPCTSTR  __stdcall GetRoomText( ) = 0 ;

	//
	// 房间是否已加密
	//
	virtual BOOL     __stdcall HasPassword( ) = 0 ;	

	//
	// 修改房间密码 管理员功能
	//
    virtual void     __stdcall ChangePwd(LPCSTR pszPwd ) = 0 ;

	//
	// 修改房间名称 管理员功能
	//
	virtual void	 __stdcall ChangeName(LPCSTR pszName) = 0 ;

	//
	// 修改房间说明 管理员功能
	//
	virtual void	__stdcall ChangeText(LPCSTR pszRoomText) = 0 ;

	//
	// 删除房间
	//
	virtual void	 __stdcall Delete() = 0 ;

	// 
	// 得到语音模式
	//
	virtual int __stdcall GetTalkMode()  = 0  ;

	//
	// 修改模式
	//
	virtual void	__stdcall ChangeTalkMode(int nMode) = 0 ;

	//
	// 设置管理员
	//
	virtual void    __stdcall ChangeManager( IUser * pUser, BOOL bEnable) = 0 ;

	//
	// 设置最大人数
	//
	virtual void    __stdcall ChangeMaxUser( int nMaxUser) = 0 ;	

	//
	// 设置房间类型
	//
	virtual void    __stdcall ChangeRoomType( int nRoomType) = 0 ;

	//
	// 送鲜花
	//
	virtual void	__stdcall VoteUser(IUser* pUser, BOOL bFlag) = 0;

	//
	// 请求（取消）麦序
	//
	virtual void	__stdcall RequestMicOrder(BOOL bFlag) = 0;

	//
	// 设置用户（增加/删除）麦序（管理员操作）
	//
	virtual void	__stdcall SetMicOrder(IUser* pUser, BOOL bFlag) = 0;

	//
	// 改变麦序（管理员）
	//
	virtual void	__stdcall ChangeMicOrder(IUser* pUser, int iOrder) = 0;

	////
	//// 获取麦序
	////
	//virtual DWORD	__stdcall GetMicOrder();

	//
	// 设置多人同时使用麦克风（麦序）
	//
	virtual void	__stdcall SetMicOrderNumber(DWORD dwNumber) = 0;

	//
	// 设置麦克风使用时间（麦序）
	//
	virtual void	__stdcall SetMicOrderTime(DWORD dwTime) = 0;

	//
	// 同时使用麦序人数
	// 
	virtual DWORD	__stdcall GetMicOrderNumber() = 0;

	//
	// 麦序使用时间
	//
	virtual DWORD	__stdcall GetMicOrderTime() = 0;

	//
	// 开启/关闭送鲜花活动
	// bEnable为TRUE表示开启，bEnable为FALSE表示关闭
	//
	virtual void	__stdcall SetVoteUser(BOOL bEnable) = 0;

	//
	// 获取鲜花用户列表
	//
	virtual void	__stdcall GetListVoteUser() = 0;
};



#define RE_MSG_BCAST    0  // 广播消息
#define RE_MSG_PRIVATE  1  // 私人消息

class ILoginEvent
{
public:
    virtual void __stdcall OnLogin( BOOL bSucc, const char * pszSearchAddress, int nPort, const char * pszLoginKey, const char * pszErrorMessage) = 0 ;

	//
	// CAS登陆回调
	//
	virtual void __stdcall OnCASLogin(BOOL bSucc, const char* pszSearchAddress, int nPort, const char* pszLoginKey, const char* pszErrorMessage) = 0;
};


class ILoginManager
{
public:
    virtual void __stdcall Release() = 0 ;
	//
	// dwTag是登陆帐号类型
	// 默认0为PT登陆，dwTag为1表示浩方登陆，dwTag为2表示边锋登陆

    virtual BOOL __stdcall Login(const char * pszLoginId, const char * pszPwd, DWORD dwTag) = 0 ;
    virtual BOOL __stdcall IsConnected() = 0 ; 

	//
	// dwTag是登陆帐号类型，3表示IGW登陆
	// pszToken为CAS登陆所需的令牌，不超过512个字节
	// pszEncrypt为CAS登陆所需的加密串,不超过128个字节
	//
	virtual BOOL __stdcall CASLogin(const char * pszLoginId, const char* pszToken, const char* pszEncrypt, DWORD dwTag) = 0;
};

//
// 群属性
//
class IGameRoomGroup
{
public:
    virtual const char *  __stdcall  GetName() = 0 ;			// 群名
	virtual int	__stdcall GetRoomGroupType() = 0 ;				// 得到群类型
    virtual int __stdcall GetRoomGroupId() = 0 ;                // 群Id
    virtual int __stdcall GetRoomCount() = 0 ;					// 房间个数
	virtual int __stdcall GetMaxRoomCount() = 0 ;				// 得到最大房间个数
    virtual int __stdcall GetMaxUser() = 0 ;					// 最大用户数目
    virtual int __stdcall GetLocalUser() = 0 ;					// 当前用户数目
    virtual int __stdcall GetHasLock() = 0 ;					// 是否加锁
    virtual IGameRoomGroup * __stdcall GetNextRoomGroup()  = 0 ; // 得到下一个 

	// 增加TAG
	virtual int __stdcall GetGameType() = 0;
	virtual char* __stdcall GetAreaName() = 0;
	virtual char* __stdcall GetGroupName() = 0;
};

//class IGameRoomGroupEx
//{
//public:
//	virtual void __stdcall GetGameType() = 0;
//	virtual char* __stdcall GetAreaName() = 0;
//	virtual char* __stdcall GetGroupName() = 0;
//	virtual IGameRoomGroup* __stdcall GetData() = 0;
//
//	virtual IGameRoomGroupEx* __stdcall GetNext() = 0;
//};

//class IGameRoomGroupEntity
//{
//public:
//	virtual int __stdcall GetGameType() = 0;
////	virtual IGameRoomGroupEx* __stdcall GetGameRoomGroupEx() = 0;
//	virtual IGameRoomGroup* __stdcall GetGameRoomGroup() = 0;
//	virtual int __stdcall GetCount() = 0;
//
//	virtual IGameRoomGroupEntity* __stdcall GetNext() = 0;
//};

class IGameRoomGroupInfo
{
public:
	virtual IGameRoomGroup* __stdcall GetGameRoomGroup() = 0; 
	virtual int __stdcall GetCount() = 0;
};

class ISearchEvent
{
public:
	
	//
    // 登陆返回结果
    //
	virtual void __stdcall OnLogin(
		BOOL bSucc,						// 操作结果
		const char * pszErrorMessage,	// 错误信息
		DWORD * pGameTypeArray,			// 游戏类型数组
		DWORD dwGameTypeCount,			// 游戏类型数组个数
		int nLevel						// 操作权限
		) = 0 ;

	//
    // 得到游戏区信息
    // 
	virtual void __stdcall OnGetGameArea( 
		BOOL bSucc,  
		DWORD dwGameType,
		DWORD dwGameAreaCount,
		const char ** ppszAreaName	
		) = 0 ;

	//
    // 得到游戏区组 信息
	//
    virtual void __stdcall OnGetGameGroup(
		BOOL bSucc,
		DWORD dwGameType, 
		const char * pszAreaName, 
		const char ** pszGameGroupName,
		int nGameGroupCount ) = 0 ;

	virtual void __stdcall OnGetGameRoomGroup(
        BOOL bSucc,
		DWORD dwGameType, 
		const char * pszAreaName, 
		const char * pszGameGroupName,
		IGameRoomGroup * pGameRoomGroup,
		int nGameRoomcount
		) = 0 ;


	//
	// 得到选定房间的IP，端口和key
    //
	virtual void __stdcall OnEnterGameGroup( 
		BOOL bSucc, 
		DWORD dwGameType, 
		const char * pszAreaName, 
		const char * pszGroupName, 
		const char * pszAddress, 
		int nPort, 
		DWORD dwRoomID,
		__int64 nLoginKey ) = 0 ;

	//
	// 得到推荐房间
	//
	virtual void __stdcall OnGetRecommendRoomList(
		BOOL bSucc,
		IGameRoomGroupInfo* pGroupInfo
		) = 0;

	//
	// 设置推荐房间
	//
	virtual void __stdcall OnSetRecommendRoom(
		BOOL bSucc,
		DWORD dwGameType,
		const char* pszAreaName,
		const char* pszGroupName,
		DWORD dwRoomId
		) = 0;

	//
	// 删除推荐房间
	//
	virtual void __stdcall OnClearRecommendRoom(
		BOOL bSucc,
		DWORD dwGameType,
		const char* pszAreaName,
		const char* pszGroupName,
		DWORD dwRoomId
		) = 0;

	//
	// 搜索指定房间
	//
	virtual void __stdcall OnSearchRoomById(
		BOOL bSucc,
		IGameRoomGroupInfo* pGroupInfo
		) = 0;

	////
	//// 获取指定游戏的房间推荐列表
	////
	//virtual void __stdcall OnGetGameRoomList(
	//	BOOL bSucc,
	//	IGameRoomGroupInfo* pGroupInfo
	//	) = 0;
};


class ISearchServer
{
public:
    virtual void __stdcall Release() = 0 ;

    virtual void __stdcall Login(
		const char * pszLoginId,		// 游戏帐号
		DWORD dwGameType,				// 游戏类型
		DWORD dwGameArea,				// 游戏区
		DWORD dwGameGroup,				// 游戏组
		const char * pszLoginKey		// 登陆钥匙
		) = 0 ;


	//
	// 获取推荐房间列表
	//
	virtual void __stdcall GetRecommendRoomList() = 0;

	//
	// 设置推荐房间
	//
	virtual void __stdcall SetRecommendRoom(DWORD dwGameType, const char* pszAreaName, const char* pszGroupName, DWORD dwRoomId) = 0;

	//
	// 清除推荐房间
	//
	virtual void __stdcall ClearRecommendRoom(DWORD dwGameType, const char* pszAreaName, const char* pszGroupName, DWORD dwRoomId) = 0;

	//
	// dwGameType 游戏类型
	//
	virtual void __stdcall SelectGame( DWORD dwGameType ) = 0 ;

	//
	// pszGameAreaName 游戏区名
	//
	virtual void __stdcall SelectGameArea( DWORD dwGameType, const char * pszGameAreaName ) = 0 ;

	//
	// dwGameType 游戏类型
	// pszGameAreaName 游戏区名
	// pszGameGroup 游戏组名
	// nPageIndex 页索引 0
	//
	virtual void __stdcall SelectGameGroup(
		DWORD dwGameType, 
		const char * pszGameAreaName, 
		const char * pszGameGroup,
		int nPageIndex ) = 0 ;// 注意这里是按照"个"为单位的

	//
	// dwGameType 游戏类型
	// pszGameAreaName 游戏区名
	// pszGameGroup 游戏组名
	// nGroupId 房间ID 
	//
	virtual void __stdcall EnterGameRoomGroup(
		DWORD dwGameType, 
		const char * pszGameAreaName, 
		const char * pszGameGroup,
		int nRoomGroupId ) = 0 ;

    //
    // 任何一个节点传进来都会删除全部的结果
    //
    virtual void __stdcall FreeSearchGroup( IGameRoomGroup * pGameRoomGroup ) = 0 ;


    virtual BOOL __stdcall IsConnected() = 0;

	virtual void __stdcall SearchRoomById(DWORD dwRoomId) = 0;

	//
	// 获取指定游戏类型的推荐房间列表
	//
	virtual void __stdcall GetGameRoomList(DWORD dwGameType) = 0;
};

//
// OnLogin nErrorCode 的可能值
//
#define PROCESS_MAX_USER    0x2  // 超过人数限制
#define PROCESS_PWD_ERR     0x3  // 密码错误
#define PROCESS_BUSY        0x4  // 服务器忙

//
// OnCreateRoom 错误
//
#define FAIL_ROOM_IS_FULL   0x2     // 房间内的频道满了
#define FAIL_ROOM_IS_TWO    0x3     // 只允许创建一个房间一个用户
#define FAIL_ROOM_IN_ROOM   0x4     // 房间内不允许创建房间
#define FAIL_ROOM_IS_EXIST  0x5     // 频道已存在



class IRoomEvent
{
public:
    // 登陆事件回调
	virtual void     __stdcall OnLogin( BOOL bSucc , int nErrorCode ) = 0 ;

	//
	// 取房间列表信息返回
    // nPageIndex 客户端发送请求时的页面索引
    // nPage 服务器上一共的页面个数
    // ppRoom 房间对象指针列表
    // iCount 返回房间个数
	//
    virtual void     __stdcall OnGetRoomList( BOOL bSucc, IRoom** ppRoom , int iCount, int nPageIndex, int nPageCount ) = 0 ;
	
	virtual void     __stdcall OnCreateRoom( DWORD dwResult, IRoom * pRoom, IUser * pOwnerUser ) = 0;
	virtual void     __stdcall OnEnterRoom( DWORD dwResult, IRoom * pRoom ) = 0;
	virtual void	 __stdcall OnCreateTeamRoom( DWORD dwResult, IRoom * pRoom, IUser * pOwnerUser ) = 0 ;
	virtual void     __stdcall OnLeaveRoom( BOOL bSucc ) = 0;

	virtual void     __stdcall OnGetUserList( BOOL bSucc, IUser** ppUser,int iCount ) = 0 ;
	virtual void     __stdcall OnUserEnter( IUser * pUser) = 0 ;
	virtual void     __stdcall OnUserLeave( IUser * pUser ) = 0 ;

	virtual void     __stdcall OnKickUser( BOOL bSucc, IUser *pUser ) = 0 ;

	//
	// 禁止语音
	//
	virtual void     __stdcall OnForbidUser( BOOL bSucc, IUser *pUser ) = 0 ;

	//
	// 不允许语音
	//
	virtual void     __stdcall OnSpeakOFF( BOOL bSucc, IUser *pUser ) = 0 ;

	//
	// 允许语音
	//
	virtual void     __stdcall OnSpeakON( BOOL bSucc, IUser *pUser ) = 0 ;
    
	//
	// 增加一个听众
	// 
    virtual void     __stdcall OnAddReader( BOOL bSucc, IUser * pUser) = 0;

	//
	// 删除一个听众
	//
    virtual void     __stdcall OnRemoveReader(BOOL bSucc , IUser * pUser ) = 0;
	
	//
	// 收到某用户发送过来的语音数据
	//
	virtual void     __stdcall OnRecvVoiceData( IUser * pUser ) = 0 ;
	
	//
	// 收到某用户发送过来的文本消息
	//
    virtual void     __stdcall OnRecvChatMsg(BOOL bSucc, IUser * pFromUser, IUser * pToUser, LPCSTR pszText, WORD wLength, int nMsgType /* RE_MSG_BCAST RE_MSG_PRIVATE*/ ) = 0 ;
	
	//
	// 收到服务器发送来的通用文本消息
	//
	virtual void     __stdcall OnRecvCommonMsg( WORD wMessageType, LPCTSTR pszText, WORD wLength ) = 0 ; 

	//
    // 连接断开
	//
    virtual void     __stdcall OnNetDisConnect() = 0 ;

	//
    // 修改房间密码
	//
    virtual void     __stdcall OnChangeRoomPassWord( WORD wResult ) = 0 ;

	//
    // 修改群密码
	//
    virtual void     __stdcall OnChangeGroupPassWord( WORD wResult ) = 0 ;

    /// 私聊语音
    virtual void     __stdcall OnPrivateSpeak(WORD wResult, IUser * pUser ) =  0 ;

	//
	// 某一用户停止语音
	//
	virtual void     __stdcall OnStopVoiceData(IUser * pUser) = 0 ;

	//
	//  语音状态发生变化
	//
	virtual void	 __stdcall OnSpeakStatusChange( IUser * pUser, BOOL bCanSpeak ) = 0 ;

	//
	// 取搜索服务器信息返回
	//
	virtual void	 __stdcall OnGetSearchServer( WORD wResult, const char * pszAddress, int nPort, __int64 nSearchKey ) = 0 ;

	//
	// 获取当前能语音的人数列表
	//
	virtual void	__stdcall OnGetSpeakerList( WORD wResult, IUser ** pUser, DWORD dwUserCount ) = 0 ;


	//
	// 得到房间修改名字结果
	//
	virtual void	__stdcall OnChangeRoomName( WORD wResult, IRoom * pRoom )= 0;

	//
	// 修改房间说明 管理员功能
	//
	virtual void	__stdcall OnChangeRoomText( WORD wResult, IRoom * pRoom ) = 0 ;

	//
	// 删除房间
	//
	virtual void	 __stdcall OnDeleteRoom( WORD wResult , IRoom * pRoom ) = 0 ;

	//
	// 修改模式
	//
	virtual void	__stdcall OnChangeTalkMode( WORD wResult , IRoom * pRoom, int nMode) = 0 ;

	//
	// 设置管理员
	//
	virtual void    __stdcall OnChangeManager( WORD wResult , IRoom * pRoom, IUser * pUser) = 0 ;

	//
	// 设置最大人数
	//
	virtual void    __stdcall OnChangeMaxUser( WORD wResult , IRoom * pRoom, int nMaxUser) = 0 ;	

	//
	// 设置房间类型
	//
	virtual void    __stdcall OnChangeRoomType( WORD wResult , IRoom * pRoom, int nRoomType) = 0 ;

	//
	// 房间内用户权限发生变化
	//
	virtual void	__stdcall OnUserTypeChange( WORD wResult, IUser * pUser, int nOldUserType ) = 0;

	//
	// 获取用户的帐号
	//
	virtual void	__stdcall OnGetUserPtId(WORD wResult, IUser* pUser, const char* pUserPtId) = 0;

	//
	// 申请（取消）麦序
	//
	virtual void	__stdcall OnRequestMicOrder(WORD wResult, BOOL bEnable) = 0;

	//
	// 设置（增加/取消）麦序（管理员）
	//
	virtual void	__stdcall OnSetMicOrder(WORD wResult, IUser* pUser, BOOL bEnable) = 0;

	//
	// 修改麦序（管理员）
	//
	virtual void	__stdcall OnChangeMicOrder(WORD wResult, IUser* pUser, DWORD dwOldOrder, DWORD dwNewOrder) = 0;

	//
	// 获取麦序列表
	//
	virtual void	__stdcall OnGetMicOrder(WORD wResult, IRoom* pRoom, IUser** ppUser, DWORD dwCount) = 0;

	// 
	// 正在使用麦序的用户列表
	//
	virtual void	__stdcall OnGetMicOrderUsed(WORD wResult, IRoom* pRoom, IUser** ppUser, DWORD dwCount) = 0;

	//
	// 设置多人同时使用麦克风（麦序）
	//
	virtual void	__stdcall OnSetMicOrderNumber(WORD wResult, IRoom* pRoom, DWORD dwMicNum) = 0;

	//
	// 设置麦克风使用时间（麦序）
	//
	virtual void	__stdcall OnSetMicOrderTime(WORD wResult, IRoom* pRoom, DWORD dwMicTime) = 0;

	//
	// 送鲜花
	//
	virtual void	__stdcall OnVoteUser(WORD wResult, IUser* pUser) = 0;

	//
	// 开启/关闭送鲜花活动
	//
	virtual void	__stdcall OnSetVoteUser(WORD wResult, IRoom* pRoom, BOOL bEnable) = 0;

	//
	// 获取鲜花用户列表
	//
	virtual void	__stdcall OnGetListVoteUser(WORD wResult, IRoom* pRoom, IVoteUser** ppVoteUser, DWORD dwCount, LPCTSTR pStartTime, LPCTSTR pStopTime) = 0;

	//
	// 禁止/允许文字聊天
	//
	virtual void	__stdcall OnUserForbiddenChat(WORD wResult, IRoom* pRoom, IUser* pUser, BOOL bFlag) = 0; 
};


class  IRoomManager
{
public:
	virtual void     __stdcall GetRoomList( int nPageIndex ) = 0 ;

	virtual BOOL     __stdcall CreateRoom( LPCTSTR pszRoomName, LPCSTR pszText, BYTE byMaxUser, BYTE byHasPass, LPCSTR pszPassWord ) = 0;
	
	//
	// 不接收该用户的发言 
	//
	virtual void	 __stdcall SpeakOFF( IUser * pUser ) = 0 ;
	
	//
	// 接收该用户的发言
	//
	virtual void	 __stdcall SpeakON( IUser * pUser ) = 0 ;
	
	//
	// 把某用户踢出房间，并带上原因
	//
	virtual void     __stdcall KickUser( IUser * pUser,LPCSTR pszReason ) = 0 ;

	//
	// 禁止房间内某用户说话,并带上原因
	//
	virtual void	 __stdcall ForbidSpeak( IUser * pUser, BOOL bEnable, int iTimeOut,LPCSTR pszReason ) = 0 ;	
    
	//
	// 增加一个当前听众
	//
    virtual void     __stdcall AddReader( IUser * pUser ) = 0 ;
    
	//
	// 删除一个听众
	//
    virtual void     __stdcall RemoveReader( IUser * pUser ) = 0 ;
    
	//
	// 用户当前的房间
	//
    virtual IRoom *  __stdcall GetMyRoom() = 0 ;
    
	//
	// 用户自己
	//
    virtual IUser *  __stdcall GetMyUser() = 0 ;
    
	//
	// 发送文字
	//
    virtual void     __stdcall SendChatText(const char * pszText, int iLength, IUser * pUser) = 0 ;

    virtual void     __stdcall ChangePwd(LPCSTR pszPwd ) = 0 ;
    
    //
    // 私语聊
    //
    virtual void     __stdcall PrivateSpeak( IUser * pUser ) = 0 ;

	//
	// 创建组队房间
	//
	virtual BOOL	 __stdcall CreateTeamRoom( 	BYTE byHasPass, LPCSTR pszPassWord) = 0 ;

	//
	// 进入组队房间
	//
	virtual BOOL     __stdcall EnterTeamRoom( DWORD dwRoomId, const char *pszRoomPwd ) = 0 ;

	//
	// 取搜索服务器信息
	//
	virtual BOOL	__stdcall GetSearchServer() = 0 ;

	//
	// 得到当前房间可以语音的用户列表，非管理员
	//
	virtual void	__stdcall GetSpeakerList() = 0 ;

	//
	// 获得工会管理器对象
	//
	virtual ITeamManager* __stdcall	GetTeamManager() = 0;

	//
	// 得到该玩家的用户帐号
	//
	virtual void	__stdcall GetUserPtId(IUser* pUser) = 0;

	//
	// 禁止(接触禁止)文字聊天，bFlag为FALSE时表示禁言，TRUE时表示允许
	// pUser等于NULL时表示整个房间禁止/允许文字聊天
	//
	virtual void	__stdcall UserForbiddenChat(IRoom* pRoom, IUser* pUser, BOOL bFlag) = 0; 
};

class ISpeak
{
public:
	virtual	void     __stdcall Init( LPCTSTR  pszIP,int iTCPPort,int iLocalPort ) = 0;

	//
	// dwServerIP 服务器IP iTCPPort服务器端端口 iLocalPort 本地监听的UDP端口 设置个
	// 较大值 9000~12000之间的值，如果打开的端口已经被占用，内核为循环30次找空闲端口
	//
	virtual	void     __stdcall InitEx( DWORD dwServerIP,int iTCPPort,int iLocalPort ) = 0;

	//
	// dwGameType 游戏类型
	// dwGameArea 游戏区   在游戏中使用该 3个参数是从 GameServer 获得，不论切换了几次房间，该值不修改
	// dwGameGroup 游戏组
	// 
    virtual IRoomManager*   __stdcall Login( 
		DWORD		dwGameType,
		DWORD		dwGameArea,
		DWORD		dwGameGroup,
		const char* szUserName,
		const char* szLoginEKey,
		const char* szNickName,       
		IRoomEvent* pRoomEvent ) = 0 ;


    virtual void     __stdcall  LogOut() = 0 ;        // 离开
    virtual BOOL     __stdcall  IsConnected() = 0 ;   // 是否登陆成功

	/***********************************************************************

	nQuality 建议从 4～8 

	4 13k bit/s 
	6 14k bit/s  
	8 15k bit/s 

	***********************************************************************/
    
	virtual BOOL     __stdcall  StartTalk( int nQuality ) = 0 ;
    virtual void     __stdcall  SetQuality(int nQuality) = 0 ;



	/***********************************************************************

	iSpeakWay 可能的值 
	持续语音 swAlways 1		 dwParam 无意义
	按键激活 swPressKey 2	 dwParam 是key的键盘码
	语音激活 swVoiceActive 3 dwParam 是级别 1~4 
	1是灵敏度最高，2是灵敏度中，3是灵敏度较低，4是灵敏度最低

	*************************************************************************/

	virtual void	 __stdcall  SetSpeakWay( int iSpeakWay, DWORD dwParam ) = 0 ;


	//
	// 获取本机说话的音量级别 0~127 
	//
	virtual int      __stdcall  GetMicLevel( ) = 0 ;	

	//
	// 获取网络发送过来的语音音量级别 0~127
	//
	virtual int      __stdcall  GetSpeakLevel( ) = 0 ;	

	/************************************************************************/
	/*                                                                      */
	/*   本地音量设置和读取                                                 */
	/*                                                                      */
	/************************************************************************/

	//
	// 获取麦克风音量设置 0~maxword (0~65535)
	//
	virtual int		 __stdcall  GetMicVol() = 0 ;		 
	
	//
	// 获取系统WAV设备音量设置就是speaker的音量0~maxword(0~65535)
	//
	virtual int  	 __stdcall  GetSpeakVol() = 0 ;		 
	
	//
	// 下面是设置函数
	//
	virtual void     __stdcall  SetMicVol( INT iValue ) = 0 ;	// 0~max word(0~65535)
	virtual void	 __stdcall  SetSpeakVol( INT iValue ) = 0 ; // 0~max word(0~65535)

	virtual void	 __stdcall  Release( ) = 0 ;

	//
	// dwGameType 游戏类型
	// dwGameArea 游戏区   在游戏中使用该 3个参数是从 GameServer 获得，不论切换了几次房间，该值不修改
	// dwGameGroup 游戏组
	// dwRoomGroupId  是从SearchServer拉取。
	//
    virtual IRoomManager*   __stdcall LoginEx(   
		DWORD		dwGameType ,
		DWORD		dwGameArea ,
		DWORD		dwGameGroup ,
		DWORD		dwRoomGroupId ,
        const char* szUserName ,
        const char* szLoginEKey ,
        const char* szNickName ,       
        IRoomEvent* pRoomEvent ) = 0 ;
   
    //
    // 得到MIC增强
    //  @0 无 super mic
    //  @1 没有选中 super mic
    //  @2 选中了 super mic
    //
    virtual int     __stdcall GetSuperMic() = 0 ;

    //
    // 设置MIC增强
    //
    virtual BOOL    __stdcall SetSuperMic(BOOL bEnable) = 0 ; 


	//
	// 是否回音消除
	//
	virtual void	__stdcall CancelEcho(BOOL bCancel) = 0 ;
    
	//
	// 设置混音
	// @1 取消（恢复麦克风输入）
	// @2 放歌+麦克风（混音）
	// @3 放歌
	//
	virtual BOOL	__stdcall SetSoundMixer(DWORD dwFlag) = 0;

	//
	// 获取混音设备
	// @0 无混音设备
	// @1 没有选中混音
	// @2 放歌+麦克风（混音）
	// @3 放歌
	//
	virtual DWORD	__stdcall GetSoundMixer() = 0;

	//
	// 设置拒绝收听任何声音（仅配合混音放歌等转播功能存在）
	//
	virtual void	__stdcall SetMixerListen(BOOL bFlag) = 0;

	//
	//
	//
	virtual BOOL	__stdcall GetMixerListen() = 0; 
};

class IGroupSpeaker
{
public:
    virtual ILoginManager * __stdcall CreateLoginManager(
        const char * pszLoginIP , 
        int nPort , 
        ILoginEvent * pLoginEvent ) = 0 ; 

    virtual ISearchServer * __stdcall CreateSearchServer(
        const char * pszAddress ,
        int nPort ,
        ISearchEvent * pSearchEvent ) = 0 ; 

    virtual ISpeak * __stdcall CreateSpeaker() = 0 ; 
    virtual void __stdcall Release() = 0 ; 
};

IGroupSpeaker* __stdcall CreateGroupSpeaker();

#endif