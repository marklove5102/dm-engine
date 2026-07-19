/*
	插件使用的环境变量和消息定义
*/

#ifndef _PLUGIN_DEF_H_
#define _PLUGIN_DEF_H_

//环境变量定义
#define  _EVE_PLUGIN_HWND_W		L"_cgp_wndhandle"
#define  _EVE_PLUGIN_HWND_A		"_cgp_wndhandle"
/* 
名称:   插件窗口句柄变量
含义: 	 插件传给游戏作为游戏窗口的父窗口句柄，格式为十六进制数字 "0xXXXXXXXX"
*/

#define  _EVE_DOWNLOAD_STATE_W		L"_cgp_downcompleted"
#define  _EVE_DOWNLOAD_STATE_A		"_cgp_downcompleted"
/* 
名称:   游戏下载状态变量

含义:   插件传给游戏描述本地客户是否下载完成，取值 0 未完成，1 完成
*/

#define  _EVE_CGP_PORT_W		L"_cgp_tcp_port"
#define  _EVE_CGP_PORT_A		"_cgp_tcp_port"
/* 
名称:   CGP通讯端口

含义:   用于插件和游戏与CGP服务连接通讯
*/

#define  _EVE_GAME_NAME_W		L"_cgp_game_name"
#define  _EVE_GAME_NAME_A		"_cgp_game_name"
/* 
名称:   游戏应用名称

含义:   
*/


#define  _EVE_GAME_PATH_W		L"_cgp_game_path"
#define  _EVE_GAME_PATH_A		"_cgp_game_path"
/* 
名称:   游戏应用路径

含义:   
*/

#define  _EVE_BROWSER_MODE_W		L"_cgp_browser_mode"
#define  _EVE_BROWSER_MODE_A		"_cgp_browser_mode"
/* 
名称:   游戏模式

含义:   
*/

#define  _EVE_GAMEMUTEX_NAME_W		L"_cgp_gamemutex_name"
#define  _EVE_GAMEMUTEX_NAME_A		"_cgp_gamemutex_name"

/* 
名称:   游戏模式

含义:   
*/

//消息定义
#define WM_CGP_APPEXIT (WM_USER+1001)	
/*
退出通知
WPARAM:	未使用，默认等于0
LPARAM:	未使用，默认等于0
用途:	插件通知游戏关闭退出
发送:  	插件发送
接收:  	游戏接收
*/

#define WM_CGP_FULLSCREEN (WM_USER+1002)
/*
消息定义：全屏通知
WPARAM: 显示标志，1 全屏显示，0 正常显示
LPARAM: 未使用，默认等于0
用途:	插件通知游戏进行显示切换操作	
发送:  	插件发送
接收:  	游戏接收
*/


#define WM_CGP_GAMESTART  (WM_USER+1003)	
/*
消息定义：游戏启动完成通知
WPARAM: 游戏主窗口句柄 WPARAM = (HWND)g_MainWnd
LPARAM: 未使用，默认等于0
用途:	游戏启动完毕后通知插件
发送:  	游戏发送
接收:  	插件接收
*/

#define WM_CGP_SETDISPLAY  (WM_USER+1004)	
/*
消息定义：游戏分辨率设置
WPARAM: 分辨率尺寸(低16位为宽度，高16位为高度)
LPARAM: 未使用，默认等于0
用途:	插件通知游戏设置修改分辨率
发送:  	插件发送
接收:  	游戏接收
*/

#define WM_CGP_GETVARIANT  (WM_USER+1005)	
/*
消息定义：获取指定的变量的内容
WPARAM:    
LPARAM: 
用途:	用来获得一些
发送:  	游戏发送
接收:  	插件接收 
*/
#define WM_CGP_APPHEART  (WM_USER+1006)	

/*
消息定义：游戏心跳消息
WPARAM:    
LPARAM: 
用途:	用来确认游戏进程运行状态
发送:  	游戏发送
接收:  	插件接收 
*/

#define WM_CGP_GAMELOAD  (WM_USER+1007)	
/*
消息定义：游戏加载消息
WPARAM:    游戏进程ID
LPARAM: 
用途:	用来通知游戏已经加载
发送:  	游戏发送
接收:  	插件接收 
*/
#endif //_PLUGIN_DEF_H_