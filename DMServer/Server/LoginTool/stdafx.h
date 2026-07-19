// stdafx.h - 预编译头文件（纯 Win32 SDK + C++17）
#pragma once

// 抑制 Windows 头文件中的无关警告
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

// Winsock2 必须在 windows.h 之前包含
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <windowsx.h>           // GET_X_LPARAM, Button_Enable 等宏
#include <commctrl.h>           // 通用控件（InitCommonControls）
#pragma comment(lib, "comctl32.lib")

// GDI+ 依赖 OLE 类型，需在 gdiplus.h 之前包含
#include <ole2.h>
#include <gdiplus.h>            // GDI+ 图片加载（替代 CImage）
#pragma comment(lib, "gdiplus.lib")

#include <wininet.h>            // HTTP 下载
#pragma comment(lib, "wininet.lib")

#include <shlobj.h>             // SHBrowseForFolder
#include <shlwapi.h>            // PathFileExists 等
#include <shellapi.h>           // ShellExecuteA
#pragma comment(lib, "shlwapi.lib")

// C++17 标准库
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <thread>
#include <functional>
#include <cstring>
#include <cstdio>

// 配置标记
#define CONFIG_MARKER_BEGIN "###CONFIG_BEGIN###"
#define CONFIG_MARKER_END   "###CONFIG_END###"
#define CONFIG_MAX_SIZE     4096

// 版本信息
#define VERSION_STR			"版本：20260713"
#define TECH_GUIDE			"客户端：2.9.10.31"

// WIN32_LEAN_AND_MEAN 会排除 IDC_STATIC，手动补充
#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif
