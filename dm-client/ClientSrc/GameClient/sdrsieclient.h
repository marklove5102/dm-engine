#ifndef __SDRSIECLIENT_H__
#define __SDRSIECLIENT_H__

#pragma once

#ifndef SDAPI
	#define SDAPI _stdcall
#endif

/*
!typedef
精灵回调游戏的函数定义
*/
typedef int( *PFN_RSGameCB)( int nMsgType, const char* szMsg);

/*!
!macro SDRSIEClIENT_
窗口类型
!inc SDRSIEClient.h
*/
#define SDRSIECLIENT_INGAME			1		// 模拟成游戏内部窗口
#define SDRSIECLIENT_POPUP			2		// 表现为漂浮在游戏外部的窗口

/*!
!macro SDRSIECLIENT_ERROR_
SDRSIEClient组件错误代码
!inc SDRSIEClient.h
*/
#define SDRSIECLIENT_ERROR_OK				0		// >=0 表示函数执行成功
#define SDRSIECLIENT_ERROR_LOADCONFIG		-1		// -1 表示加载配置文件失败 检查配置文件是否存在 格式是否正确
#define SDRSIECLIENT_ERROR_NET				-2		// -2 初始化服务器连接时失败 检查DLL是否存在 服务器地址设置是否正确
#define SDRSIECLIENT_ERROR_WIN32			-3		// -3 创建Win32窗口时失败了 检查图片资源是否存在


class ISDRSIEClient
{
public:
	/*******************************************************************************************************/
	//!group 创建函数
	/*!func
	此函数用来创建彩虹精灵窗口
	!param [in] 游戏主窗口句柄,请注意游戏主窗口的风格应该包含WS_CLIPCHILDREN ,防止在某些DX游戏下,精灵窗口不可见
	!param [in] 精灵窗口的模式,目前有两种参见SDRSIECLIENT_
	!param [in] 窗口布局配置文件
	!return 错误代码 参见 SDRSIECLIENT_ERROR_
	*/
	virtual int SDAPI CreateRSWindow( HWND hMainWnd, int nMode= SDRSIECLIENT_INGAME, const char* strConfigFile= NULL ) = 0;
	
	/*!func
	此函数用来初始化精灵需要的游戏相关数据,此函数必须调用成功,否则无法正常工作
	!param [in] 游戏种类(参见游戏的数字编码:例如: 传世 1 梦幻 2)
	!param [in] 玩家所在区数字编码
	!param [in] 帐号种类 数字帐号:0 盛大通行证: 1
	!param [in] 帐号
	!param [in] 角色名
	!return 错误代码 参见 SDRIECLIENT_ERROR_
	*/
	virtual int SDAPI SetGameInfo( int nGameType, int nAreaCode, int nIDType, const char* szUserID, const char* szRoleID, PFN_RSGameCB pfnRSGame) = 0;
	
	/*****************************************************************************************************/
	//!group 精灵窗口控制函数
	/*func
	显示或隐藏精灵窗口
	!param [in] 精灵类别 1: 普通精灵 2:问吧 3: 客服论坛 4:NCC  默认为1
	!param [in] 游戏服务区组
	!param [in] 显示 true 隐藏 false
	!return  目前不会失败
	*/
	virtual bool SDAPI Show( int nAppCode, int nSrvCode, bool bShow = true) = 0;
	
	/*func
	查询精灵的窗口的显示状况
	!return true 显示 false 隐藏
	*/
	virtual bool SDAPI IsShow() = 0;
	
	/*!func
	设置窗口移动效果
	!param [in] 是否允许窗口移动
	!param [in[ 窗口拖动效果是否为线框效果,即拖动时不显示窗口内容
	!return 目前总是成功
	*/
	virtual bool SDAPI SetMovable( bool bCanMove = true, bool bLineEffect = false) = 0;

	/*!func
	查询精灵窗口是否可移动
	!return 可移动 true 不可以 false
	*/
	virtual bool SDAPI IsMovable() = 0;
	
	/*!func
	设置窗口的坐标(注意在POPUP模式下,坐标为桌面坐标)
	!param [in] x
	!param [in] y
	!return 成功 true 失败 false
	*/
	virtual bool SDAPI SetPos( int iX, int iY) = 0;

	/*!func
	查询窗口的位置
	!param [out] x
	!param [out] y
	!return 成功 true, 失败false
	*/
	virtual bool SDAPI GetPos( int& iX, int& iY) = 0;

	/*!func
	设置窗口的大小
	!param [in] 窗口宽度
	!param [in] 窗口高度
	!return 目前的窗口size需要匹配图片大小 因此改变size的操作总是失败
	*/
	virtual bool SDAPI SetSize( int iWidth, int iHeight) = 0;

	/*!func
	查询窗口的大小
	!param [out] 窗口宽度
	!param [out] 窗口高度
	!return 成功 true, 失败false
	*/
	virtual bool SDAPI GetSize( int& iWidth, int& iHeight) = 0;


	/*
	获取窗口模式
	!return 参SDRSIECLIENT_
	*/
	virtual int SDAPI GetMode() = 0;


	/*************************************************************************************************************/
	//!group 销毁函数
	/*func!
	释放内存
	*/
	virtual void SDAPI Release() = 0;
};

ISDRSIEClient* SDAPI SDRSIEClientGetModule();

typedef ISDRSIEClient* (SDAPI *PFN_SDRSClientGetModule)();

#endif //__SDRSIECLIENT_H__