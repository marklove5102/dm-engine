#pragma once

#include "Global/Global.h"
#include "SpeakInterface.h"

#ifdef _CHAT

//-----------------------------------------------------------------------------------
//CRoomManager的返回代理类
class CRoomEvent : public IRoomEvent
{
public:
	CRoomEvent();

	// 登陆事件回调
	virtual void     __stdcall OnLogin( BOOL bSucc , int nErrorCode);
	// 取房间列表信息返回 nPageIndex 客户端发送请求时的页面索引/nPage服务器上一共的页面个数/ ppRoom房间对象指针列表/ iCount返回房间个数
	virtual void     __stdcall OnGetRoomList( BOOL bSucc, IRoom** ppRoom , int iCount, int nPageIndex, int nPageCount );
	virtual void     __stdcall OnCreateRoom( DWORD dwResult, IRoom * pRoom, IUser *pOwnerUser );
	virtual void	 __stdcall OnCreateTeamRoom( DWORD dwResult, IRoom * pRoom, IUser * pOwnerUser );
	virtual void     __stdcall OnEnterRoom( DWORD dwResult, IRoom * pRoom );
	virtual void     __stdcall OnLeaveRoom( BOOL bSucc );
	virtual void     __stdcall OnGetUserList( BOOL bSucc, IUser** ppUser,int iCount );
	virtual void     __stdcall OnUserEnter( IUser * pUser);
	virtual void     __stdcall OnUserLeave( IUser * pUser );
	virtual void     __stdcall OnKickUser( BOOL bSucc, IUser *pUser );
	virtual void     __stdcall OnForbidUser( BOOL bSucc, IUser *pUser );
	virtual void     __stdcall OnSpeakOFF( BOOL bSucc, IUser *pUser );
	virtual void     __stdcall OnSpeakON( BOOL bSucc, IUser *pUser );
	virtual void     __stdcall OnAddReader( BOOL bSucc, IUser * pUser);
	virtual void     __stdcall OnRemoveReader(BOOL bSucc , IUser * pUser );
	/// 收到某用户发送过来的语音数据
	virtual void     __stdcall OnRecvVoiceData( IUser * pUser );
	/// 收到某用户发送过来的文本消息
	virtual void     __stdcall OnRecvChatMsg(BOOL bSucc, IUser * pFromUser, IUser * pToUser, LPCSTR pszText, WORD wLength, int nMsgType  );// RE_MSG_BCAST RE_MSG_PRIVATE 
	/// 收到服务器发送来的通用文本消息
	virtual void     __stdcall OnRecvCommonMsg( WORD wMessageType, LPCTSTR pszText, WORD wLength );
	/// 连接断开
	virtual void     __stdcall OnNetDisConnect() ;
	/// 修改房间密码
	virtual void     __stdcall OnChangeRoomPassWord( WORD wResult );
	/// 修改群密码
	virtual void     __stdcall OnChangeGroupPassWord( WORD wResult );
	/// 私聊语音
	virtual void     __stdcall OnPrivateSpeak(WORD wResult, IUser * pUser );
	/// 某一用户停止语音
	virtual void     __stdcall OnStopVoiceData(IUser * pUser);
	///  语音状态发生变化
	virtual void	 __stdcall OnSpeakStatusChange( IUser * pUser, BOOL bCanSpeak );
	/// 取搜索服务器信息返回
	virtual void	 __stdcall OnGetSearchServer( WORD wResult, const char * pszAddress, int nPort, __int64 nSearchKey );


	// 获取当前能语音的人数列表
	virtual void	__stdcall OnGetSpeakerList( WORD wResult, IUser ** pUser, DWORD dwUserCount );
	// 得到房间修改名字结果
	virtual void	__stdcall OnChangeRoomName( WORD wResult, IRoom * pRoom );
	// 修改房间说明 管理员功能
	virtual void	__stdcall OnChangeRoomText( WORD wResult, IRoom * pRoom );
	// 删除房间
	virtual void	 __stdcall OnDeleteRoom( WORD wResult , IRoom * pRoom );
	// 修改模式
	virtual void	__stdcall OnChangeTalkMode( WORD wResult , IRoom * pRoom, int nMode);
	// 设置管理员
	virtual void    __stdcall OnChangeManager( WORD wResult , IRoom * pRoom, IUser * pUser);
	// 设置最大人数
	virtual void    __stdcall OnChangeMaxUser( WORD wResult , IRoom * pRoom, int nMaxUser);	
	// 设置房间类型
	virtual void    __stdcall OnChangeRoomType( WORD wResult , IRoom * pRoom, int nRoomType);
	// 房间内用户权限发生变化
	virtual void	__stdcall OnUserTypeChange( WORD wResult, IUser * pUser, int nOldUserType );
	// 获取用户的帐号
	virtual void	__stdcall OnGetUserPtId(WORD wResult, IUser* pUser, const char* pUserPtId);
	// 申请（取消）麦序
	virtual void	__stdcall OnRequestMicOrder(WORD wResult, BOOL bEnable);
	// 设置（增加/取消）麦序（管理员）
	virtual void	__stdcall OnSetMicOrder(WORD wResult, IUser* pUser, BOOL bEnable);
	// 修改麦序（管理员）
	virtual void	__stdcall OnChangeMicOrder(WORD wResult, IUser* pUser, DWORD dwOldOrder, DWORD dwNewOrder);
	// 获取麦序列表
	virtual void	__stdcall OnGetMicOrder(WORD wResult, IRoom* pRoom, IUser** ppUser, DWORD dwCount);
	// 正在使用麦序的用户列表
	virtual void	__stdcall OnGetMicOrderUsed(WORD wResult, IRoom* pRoom, IUser** ppUser, DWORD dwCount);
	// 设置多人同时使用麦克风（麦序）
	virtual void	__stdcall OnSetMicOrderNumber(WORD wResult, IRoom* pRoom, DWORD dwMicNum);
	// 设置麦克风使用时间（麦序）
	virtual void	__stdcall OnSetMicOrderTime(WORD wResult, IRoom* pRoom, DWORD dwMicTime);
	// 送鲜花
	virtual void	__stdcall OnVoteUser(WORD wResult, IUser* pUser);
	// 开启/关闭送鲜花活动
	virtual void	__stdcall OnSetVoteUser(WORD wResult, IRoom* pRoom, BOOL bEnable);
	// 获取鲜花用户列表
	virtual void	__stdcall OnGetListVoteUser(WORD wResult, IRoom* pRoom, IVoteUser** ppVoteUser, DWORD dwCount, LPCTSTR pStartTime, LPCTSTR pStopTime);
	// 禁止/允许文字聊天
	virtual void	__stdcall OnUserForbiddenChat(WORD wResult, IRoom* pRoom, IUser* pUser, BOOL bFlag); 



}; 

//-----------------------------------------------------------------------------------
//ISearchServer搜索的返回代理类
class CSearchEvent : public ISearchEvent
{
public:
	// 登陆返回结果
	virtual void __stdcall OnLogin(BOOL bSucc,const char * pszErrorMessage,DWORD * pGameTypeArray,DWORD dwGameTypeCount,int nLevel);
	// 得到游戏区信息
	virtual void __stdcall OnGetGameArea(BOOL bSucc,DWORD dwGameType,DWORD dwGameAreaCount,const char ** ppszAreaName);
	// 得到游戏区组 信息
	virtual void __stdcall OnGetGameGroup(BOOL bSucc,DWORD dwGameType,const char * pszAreaName,const char ** pszGameGroupName,int nGameGroupCount);
	//得到房间
	virtual void __stdcall OnGetGameRoomGroup(BOOL bSucc,DWORD dwGameType,const char * pszAreaName,const char * pszGameGroupName,IGameRoomGroup * pGameRoomGroup,int nGameRoomcount);
	// 得到选定房间的IP，端口和key
	virtual void __stdcall OnEnterGameGroup(BOOL bSucc,DWORD dwGameType,const char * pszAreaName,const char * pszGroupName,const char * pszAddress,int nPort,DWORD dwRoomID,__int64 nLoginKey);
	// 得到推荐房间
	virtual void __stdcall OnGetRecommendRoomList(BOOL bSucc,IGameRoomGroupInfo* pGroupInfo);
	// 设置推荐房间
	virtual void __stdcall OnSetRecommendRoom(BOOL bSucc,DWORD dwGameType,const char* pszAreaName,const char* pszGroupName,DWORD dwRoomId);
	// 删除推荐房间
	virtual void __stdcall OnClearRecommendRoom(BOOL bSucc,DWORD dwGameType,const char* pszAreaName,const char* pszGroupName,DWORD dwRoomId);
	// 搜索指定房间
	virtual void __stdcall OnSearchRoomById(BOOL bSucc,IGameRoomGroupInfo* pGroupInfo);


};
#endif