#pragma once

#include "SpeakInterface.h"

//////////////////////////////////////////////////////////////////////////
//
// 管理员接口
//
//////////////////////////////////////////////////////////////////////////

//
// 在线
//
#define OFF_LINE_TEAM_STATUS	0  

//
// 不在线
//
#define ON_LINE_TEAM_STATUS		1  


//
// 最多16个分组
//
#define MAX_TEAM_GROUP_COUNT	16

//
// 工会成员接口
//
class ITeamMember:public ISpeakClass
{
public:
	//virtual void	__stdcall SetSex(DWORD dwSex) = 0;				// 设置性别
	//virtual void	__stdcall SetAge(DWORD dwAge) = 0;				// 设置年龄
	//virtual void	__stdcall SetNickName(char* pNickName) = 0;		// 设置昵称
	//virtual void	__stdcall SetCityName(char* pCityName) = 0;		// 设置城市

	virtual int		__stdcall GetId() = 0 ;			// ID
	//virtual int		__stdcall GetSex() = 0;			// 性别
	//virtual int		__stdcall GetAge() = 0;			// 年龄
	virtual char*	__stdcall GetNickName() = 0;	// 昵称
	//virtual char*	__stdcall GetCityName() = 0;	// 城市
	virtual char *  __stdcall GetTeamName() = 0 ;	// 行会呢称
	virtual int		__stdcall GetType() = 0 ;		// 类型
	//virtual char *	__stdcall GetJoinTime() = 0 ;	// 加入时间
	virtual char *  __stdcall GetLastTime() = 0 ;	// 最后一次登陆时间
	virtual int		__stdcall GetStatus() = 0 ;				// 当前状态
	virtual int		__stdcall GetOnlineTime() = 0;
	virtual void	__stdcall SetTeamName(const char * pszTeamName) = 0 ; // 修改行会呢称
	virtual ITeamMember* __stdcall GetPrev() = 0;		// 上一个成员
	virtual ITeamMember * __stdcall GetNext() = 0 ;		// 下一个成员
};

#define DEFAULT_TEAM_ROOM_GROUP_TYPE	1	// 默认分组 不允许用户修改名称和删除该分组
#define USER_TEAM_ROOM_GROUP_TYPE		0	// 用户自定义分组

class ITeamRoomGroup:public ISpeakClass
{
public:
	//
	// 得到ID
	//
	virtual int __stdcall GetId() = 0 ;

	//
	// 得到房间个数
	//
	virtual int __stdcall GetRoomCount() = 0 ;

	//
	// 得到类型 默认分组不允许修改名称，删除，等操作
	//
	virtual int	__stdcall GetType() = 0 ;

	//
	// 得到分组名称
	//
	virtual char * __stdcall GetName() =  0 ;

	//
	// 得到房间
	//
	virtual IRoom ** __stdcall GetRoomArray() = 0 ;

	//
	// 加入新房间到该分组
	//
	virtual void __stdcall AddRoom(IRoom * pRoom) = 0 ;

	//
	// 删除房间，删除的房间放到默认房间里去了。
	//
	virtual void __stdcall RemoveRoom(IRoom * pRoom) = 0 ;

	//
	// 分组重新命名
	//
	virtual void __stdcall Rename(const char * pNewName) = 0 ;

	//
	// 删除分组
	//
	virtual void __stdcall Delete() = 0 ;

	//
	// 得到上一个分组
	//
	virtual ITeamRoomGroup* __stdcall GetPrev() = 0;

	//
	// 得到下一个分组
	//
	virtual ITeamRoomGroup * __stdcall GetNext() = 0 ;

	//
	// 移动频道位置, iFlag 为1时表示上移一位，而-1时表示下移一位
	//
	virtual void __stdcall ChangeRoomOrder(IRoom *pRoom, int iFlag) = 0;

	//
	// 在分组内创建频道
	//
//	virtual void __stdcall CreateRoom(LPCTSTR pszRoomName, LPCSTR pszText, BYTE byMaxUser, BYTE byHasPass, LPCSTR pszPassWord) = 0;
};

//
// 不建议暴露出去，系统开销太大
//
class ITeamEvent
{
public:

	//
	// 获得全部的分组列表
	//
	virtual void __stdcall OnTeamRoomGroupList(ITeamRoomGroup * pTeamRoomGroup) =  0 ;

	//
	// 全部在线的工会成员
	//
	virtual void __stdcall OnOnlineMemberList(ITeamMember * pTeamMember) = 0;

	//
	// 全部的VIP成员
	//
	virtual void __stdcall OnAllVIPMemberList(ITeamMember * pTeamMember) = 0;

	//
	// 全部的工会成员列表
	//
	virtual void __stdcall OnAllTeamMemberList(ITeamMember * pTeamMember) =  0;

	//
	// 有成员登陆
	//
	virtual void __stdcall OnTeamMemberEnter(ITeamMember * pTeamMember, IRoom *pRoom) = 0 ;

	//
	// 有成员离开
	//
	virtual void __stdcall OnTeamMemberLeave(ITeamMember * pTeamMember, IRoom * pRoom) = 0 ;

	//
	// 创建TEAM ROOM 分组
	//
	virtual void __stdcall OnTeamCreateTeamGroup(int nErrorCode, ITeamRoomGroup * pTeamRoomGroup) =  0 ;

	//
	// 分组被删除
	//
	virtual void __stdcall OnTeamRemoveTeamGroup(int nErrorCode, ITeamRoomGroup * pTeamRoomGroup );

	//
	// 分组重命名
	//
	virtual void __stdcall OnTeamRenameTeamGroup(int nErrorCode, ITeamRoomGroup* pTeamRoomGroup);

	//
	// 删除TEAM ROOM 分组中的房间
	//
	virtual void __stdcall OnTeamRemoveRoomFromTeamGroup(int nErrorCode, ITeamRoomGroup * pTeamRoomGroup, IRoom * pRoom ) = 0 ;

	//
	// 移动分组中的房间
	//
	virtual void __stdcall OnTeamMoveRoomInGroup(int nErrorCode, ITeamRoomGroup* pTeamRoomGroup, IRoom* pRoom) = 0;

	//
	// 刷新工会信息
	//
	virtual void __stdcall OnTeamGroupInfo(int nErrorCode, ITeamManager* pTeamManager) = 0;

	//
	// 添加工会成员
	//
	virtual void __stdcall OnTeamMemberAdd(int nErrorCode, IUser* pUser) = 0;

	// 
	// 删除工会成员
	//
	virtual void __stdcall OnTeamMemberRemove(int nErrorCode, ITeamMember* pTeamMember) = 0;

	//
	// 离线修改工会管理员
	//
	virtual void __stdcall OnTeamChangeManager(int nErrorCode, ITeamMember* pTeamMember) = 0;

	//
	// 设置用户信息
	//
	virtual void __stdcall OnSetUserInfo(int nErrorCode) = 0;

	//
	// 获取用户信息
	//
	virtual void __stdcall OnGetUserInfo(int nErrorCode, int nSex, int nAge, char* pNickName, char* pCityName) = 0;

	//
	// 用户信息为空，需要设置用户信息 
	//
	virtual void __stdcall OnCallSetUserInfo() = 0;

	//
	// 用户信息已接收信息
	//
	virtual void __stdcall OnUserInfoReady() = 0;

	//
	// 移动分组中房间的次序
	//
	virtual void __stdcall OnMoveRoomInGroup(int nErrorCode, ITeamRoomGroup* pTeamRoomGroup, IRoom* pRoom, int iSite) = 0;

	//
	// 分组内创建频道
	//
//	virtual void __stdcall OnTeamCreateRoomInGroup(int nErrorCode, ITeamRoomGroup* pTeamRoomGroup, IRoom* pRoom) = 0;

	//
	// 退出工会
	//
//	virtual void __stdcall OnTeamMemberQuit(int nErrorCode);

};

class ITeamManager
{
public:
	//
	// 设置工会事件
	//
	virtual int __stdcall SetTeamEvent(ITeamEvent * pTeamEvent) = 0 ;

	//
	// 得到工会名称
	//
	virtual char * __stdcall GetTeamName() = 0 ;

	//
	// 得到公告牌
	//
	virtual char * __stdcall GetBoardMessage() = 0 ;

	//
	// 得到成员数目
	//
	virtual int __stdcall GetMemberCount() = 0 ;

	//
	// 得到VIP成员数目
	//
	virtual int __stdcall GetVIPMemberCount() = 0 ; 

	//
	// 得到在线数目
	//
	virtual int __stdcall GetOnlineCount() = 0;

	//
	// 得到最高在线数目
	//
	virtual int __stdcall GetMaxOnlineCount() = 0 ;

	//
	// 得到行会级别
	//
	virtual int __stdcall GetTeamLevel() = 0 ;

	//
	// 得到行会积分
	//
	virtual int __stdcall GetTeamMoney() = 0 ;

	//
	// 得到当前用户权限
	//
	virtual int  __stdcall GetPower() = 0 ;

	//
	// 显示全部的队列成员
	//
	virtual void __stdcall ListTeamMember() = 0 ;

	// 
	// 显示在线的工会成员
	//
	virtual void __stdcall ListOnlineMember() = 0;

	//
	// 显示全部VIP成员
	//
	virtual void __stdcall ListVIPMember() = 0 ; 

	//
	// 显示全部的房间和分组信息
	//
	virtual void __stdcall ListTeamGroupList() = 0 ;
	
	//
	// 强制刷新工会信息
	//
	virtual void __stdcall GetTeamGroupInfo() = 0;

	//
	// 修改公告牌消息
	//
	virtual int  __stdcall ChangeTeamBoardMessage(const char * pszMessage) = 0 ;

	//
	// 修改工会管理员
	// nManagerLevel为0时表示无管理员权限
	//
	virtual void __stdcall ChangeTeamManager(ITeamMember* pTeamMember, int nManagerLevel) = 0 ;

	//
	// 增加工会成员
	//
	virtual void __stdcall AddMember(IUser* pUser) = 0;

	//
	// 删除工会成员
	//
	virtual void __stdcall RemoveMember(ITeamMember * pTeamMember) = 0;

	//
	// 新建分组
	//
	virtual BOOL __stdcall AddRoomGroup(const char  *pszRoomGroupName) = 0;

	//
	// 获取分组链表头结点
	//
	virtual ITeamRoomGroup* __stdcall GetGroupList() = 0;

	//
	// 获取工会成员链表头结点
	//
	virtual ITeamMember* __stdcall GetMemberList() = 0;

	//
	// 修改用户信息
	//
	virtual void __stdcall SetUserInfo(int nSex, int nAge, char* pNickName, char* pCityName) = 0;

	//
	// 获取用户信息
	//
	virtual void __stdcall GetUserInfo() = 0;

	//
	// 退出工会
	//
//	virtual void __stdcall TeamMemberQuit() = 0;
};







