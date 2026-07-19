// LoginWnd.h - 登录器主窗口（替代 LoginerDlg）
#pragma once
#include "stdafx.h"
#include <string>
#include <vector>

namespace app {

// ========== 服务器信息结构 ==========
struct ServerInfo
{
	std::string name;       // 服务器名称
	std::string address;    // 服务器地址
	int         port;       // 服务器端口
	int         status;     // 状态：0=维护，1=开放，2=爆满，3=推荐
	int         color;      // 颜色代码
	std::string loginKey;   // LoginKey（注册认证）
};

// ========== 游戏配置结构 ==========
struct GameConfig
{
	std::string noticeUrl;      // 游戏公告
	std::string webSite;        // 官方网站
	std::string buyUrl;         // 游戏充值
	std::string forgotUrl;      // 找回密码
	std::vector<ServerInfo> servers; // 服务器列表
};

// ========== Ping 参数（后台线程） ==========
struct PingParam
{
	HWND        hWnd;
	std::string address;
	int         port;
	int         timeoutMs;
};

// 显示登录器窗口，返回退出码
int ShowLoginWindow(HINSTANCE hInstance);

} // namespace app
