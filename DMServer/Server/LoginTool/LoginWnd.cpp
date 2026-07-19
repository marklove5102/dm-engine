// LoginWnd.cpp - 登录器主窗口实现（替代 LoginerDlg）
#include "stdafx.h"
#include "LoginWnd.h"
#include "RegisterWnd.h"
#include "WinString.h"
#include "GdiRaii.h"
#include "Protocol.h"
#include "resource.h"

namespace app {

// ========== 颜色定义 ==========
#define COLOR_BG          RGB(30, 35, 50)
#define COLOR_PANEL       RGB(20, 25, 40)
#define COLOR_GOLD        RGB(255, 215, 0)
#define COLOR_TITLE_GOLD  RGB(255, 200, 50)
#define COLOR_RED_BTN     RGB(200, 30, 30)
#define COLOR_GREEN_OPEN  RGB(0, 220, 0)
#define COLOR_RED_FULL    RGB(255, 50, 50)
#define COLOR_YELLOW_REC  RGB(255, 255, 0)
#define COLOR_GRAY_MAINT  RGB(150, 150, 150)
#define COLOR_WHITE       RGB(255, 255, 255)
#define COLOR_LIGHT_TEXT  RGB(200, 200, 220)

// Ping 结果消息
#define WM_PING_RESULT  (WM_USER + 100)

// 游戏启动器路径
#define GAME_LAUNCHER   "Data\\woool.dat.update"

// 常见游戏目录候选
static const char* GAME_DIR_CANDIDATES[] = {
	"C:\\传奇世界", "C:\\Program Files\\传奇世界",
	"C:\\Program Files (x86)\\传奇世界", "D:\\传奇世界",
	"D:\\Games\\传奇世界", "C:\\传世", "C:\\Program Files\\传世", NULL
};

// 窗口类名
static const char* LOGIN_CLASS = "LoginTool_MainWnd";

// ========== 窗口状态（替代 CLoginerDlg 成员变量） ==========
struct LoginState
{
	// GDI 资源
	HFONT  hTitleFont;
	HFONT  hLabelFont;
	HFONT  hBtnFont;
	HBRUSH hBgBrush;
	HBRUSH hBtnBrush;
	HBRUSH hPanelBrush;
	HBRUSH hEditBrush;
	HBRUSH hComboBrush;

	// GDI+ 皮肤图片
	Gdiplus::Bitmap* pSkinBg;
	Gdiplus::Bitmap* pSkinTitle;
	Gdiplus::Bitmap* pSkinLoginBtn;
	bool   bSkinLoaded;

	// 数据
	GameConfig  config;
	std::string loginerName;   // 登录器名称（ANSI/GBK）
	bool        bWindowMode;
	int         selectedServer;
	std::string statusText;
	std::string gamePath;
	bool        bServerOnline;
};

// ========== 前向声明 ==========
static LRESULT CALLBACK LoginWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam);
static DWORD WINAPI PingThreadProc(LPVOID pParam);
static void UpdateServerList(HWND hWnd, LoginState* s);
static void OnSelectServer(HWND hWnd, LoginState* s);

// ========== 注册窗口类 ==========
static void RegisterLoginClass(HINSTANCE hInstance)
{
	WNDCLASSEXA wc = {};
	wc.cbSize        = sizeof(WNDCLASSEXA);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = LoginWndProc;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIconA(hInstance, MAKEINTRESOURCEA(IDR_MAINFRAME));
	wc.hCursor       = LoadCursorA(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = LOGIN_CLASS;
	RegisterClassExA(&wc);
}

// ========== 控件创建辅助（像素坐标，按 400x320 基准布局） ==========
static HWND LLabel(HWND hP, int id, const char* t, int x, int y, int w, int h, DWORD extra = 0)
{ return CreateWindowExA(0, "STATIC", t, WS_CHILD|WS_VISIBLE|SS_LEFT|extra, x,y,w,h, hP, (HMENU)(INT_PTR)id, NULL, NULL); }

static HWND LEdit(HWND hP, int id, const char* t, int x, int y, int w, int h, DWORD extra = 0)
{ return CreateWindowExA(0, "EDIT", t, WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|extra, x,y,w,h, hP, (HMENU)(INT_PTR)id, NULL, NULL); }

static HWND LButton(HWND hP, int id, const char* t, int x, int y, int w, int h, DWORD extra = 0)
{ return CreateWindowExA(0, "BUTTON", t, WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|extra, x,y,w,h, hP, (HMENU)(INT_PTR)id, NULL, NULL); }

static HWND LCombo(HWND hP, int id, int x, int y, int w, int h)
{ return CreateWindowExA(0, "COMBOBOX", "", WS_CHILD|WS_VISIBLE|WS_TABSTOP|CBS_DROPDOWN|CBS_AUTOHSCROLL, x,y,w,h, hP, (HMENU)(INT_PTR)id, NULL, NULL); }

static HWND LListBox(HWND hP, int id, int x, int y, int w, int h)
{ return CreateWindowExA(0, "LISTBOX", "",
	WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|WS_VSCROLL|LBS_NOTIFY|LBS_HASSTRINGS|LBS_OWNERDRAWFIXED,
	x,y,w,h, hP, (HMENU)(INT_PTR)id, NULL, NULL); }

// ========== 状态颜色/文字 ==========
static COLORREF GetStatusColor(int status)
{
	switch (status)
	{
	case 0: return COLOR_GRAY_MAINT;
	case 1: return COLOR_GREEN_OPEN;
	case 2: return COLOR_RED_FULL;
	case 3: return COLOR_YELLOW_REC;
	default: return COLOR_WHITE;
	}
}

static const char* GetStatusText(int status)
{
	switch (status)
	{
	case 0: return "[维护]";
	case 1: return "[开放]";
	case 2: return "[爆满]";
	case 3: return "[推荐]";
	default: return "[未知]";
	}
}

// ========== 验证游戏目录 ==========
static bool IsValidGameDirectory(const std::string& dir)
{
	std::string launcher = dir + "\\" + GAME_LAUNCHER;
	return GetFileAttributesA(launcher.c_str()) != INVALID_FILE_ATTRIBUTES;
}

// ========== 搜索游戏目录 ==========
static std::string FindGameDirectory()
{
	// 优先检查 EXE 自身所在目录（登录器放在客户端根目录的场景）
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	std::string exeDir(exePath);
	size_t pos = exeDir.find_last_of("\\/");
	if (pos != std::string::npos) exeDir = exeDir.substr(0, pos);
	if (IsValidGameDirectory(exeDir)) return exeDir;

	for (int i = 0; GAME_DIR_CANDIDATES[i] != NULL; i++)
	{
		if (GetFileAttributesA(GAME_DIR_CANDIDATES[i]) != INVALID_FILE_ATTRIBUTES
		    && IsValidGameDirectory(GAME_DIR_CANDIDATES[i]))
			return GAME_DIR_CANDIDATES[i];
	}

	// 检查注册表
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\盛大网络\\传奇世界", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		char path[MAX_PATH] = {0};
		DWORD size = sizeof(path);
		if (RegQueryValueExA(hKey, "InstallDir", NULL, NULL, (LPBYTE)path, &size) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			if (path[0] && IsValidGameDirectory(path)) return path;
		}
		else RegCloseKey(hKey);
	}

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\盛大网络\\传奇世界", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		char path[MAX_PATH] = {0};
		DWORD size = sizeof(path);
		if (RegQueryValueExA(hKey, "InstallDir", NULL, NULL, (LPBYTE)path, &size) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			if (path[0] && IsValidGameDirectory(path)) return path;
		}
		else RegCloseKey(hKey);
	}

	return "";
}

// ========== 目录选择对话框 ==========
static bool ShowDirSelectDialog(HWND hWnd, std::string& outPath)
{
	BROWSEINFOA bi = {};
	bi.hwndOwner = hWnd;
	bi.lpszTitle = "请选择传奇世界安装目录：";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  // 兼容 ANSI
	if (pidl != NULL)
	{
		char path[MAX_PATH];
		if (SHGetPathFromIDListA(pidl, path))
		{
			outPath = path;
			CoTaskMemFree(pidl);
			return true;
		}
		CoTaskMemFree(pidl);
	}
	return false;
}

// ========== TCP Ping 服务器 ==========
static bool PingServer(const std::string& address, int port, int timeoutMs)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) { WSACleanup(); return false; }

	DWORD timeout = timeoutMs;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(address.c_str());

	if (addr.sin_addr.s_addr == INADDR_NONE) { closesocket(sock); WSACleanup(); return false; }

	bool result = (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0);
	closesocket(sock);
	WSACleanup();
	return result;
}

// ========== 从 EXE 读取嵌入配置 ==========
static bool ReadConfigFromExe(std::string& outName, std::string& outUrl)
{
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	// 从 EXE 尾部读取嵌入配置
	HANDLE hFile = CreateFileA(exePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize <= CONFIG_MAX_SIZE) { CloseHandle(hFile); return false; }

	SetFilePointer(hFile, fileSize - CONFIG_MAX_SIZE, NULL, FILE_BEGIN);
	char buffer[CONFIG_MAX_SIZE + 1] = {0};
	DWORD bytesRead;
	ReadFile(hFile, buffer, CONFIG_MAX_SIZE, &bytesRead, NULL);
	CloseHandle(hFile);

	std::string content(buffer, bytesRead);
	size_t begin = content.find(CONFIG_MARKER_BEGIN);
	size_t end   = content.find(CONFIG_MARKER_END);
	if (begin != std::string::npos && end != std::string::npos)
	{
		begin += strlen(CONFIG_MARKER_BEGIN);
		std::string configData = content.substr(begin, end - begin);
		size_t pipePos = configData.find('|');
		if (pipePos != std::string::npos)
		{
			outName = configData.substr(0, pipePos);
			outUrl  = configData.substr(pipePos + 1);
			return true;
		}
		else
		{
			outName = "传奇世界";
			outUrl  = configData;
			return true;
		}
	}
	return false;
}

// ========== HTTP 下载列表 ==========
static bool DownloadList(const std::string& url, std::string& content)
{
	HINTERNET hInternet = InternetOpenA("Loginer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternet) return false;

	HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hUrl) { InternetCloseHandle(hInternet); return false; }

	char buffer[4096];
	DWORD bytesRead;
	while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
		content.append(buffer, bytesRead);

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);
	return !content.empty();
}

// ========== 从 EXE 同目录读取 list.txt ==========
static bool LoadListFromFile(std::string& content)
{
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	std::string dir(exePath);
	size_t pos = dir.find_last_of("\\/");
	if (pos != std::string::npos) dir = dir.substr(0, pos + 1);
	std::string listPath = dir + "list.txt";

	HANDLE hFile = CreateFileA(listPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	DWORD size = GetFileSize(hFile, NULL);
	if (size == 0 || size > 65536) { CloseHandle(hFile); return false; }

	std::vector<char> buf(size + 1);
	DWORD rd = 0;
	bool ok = ReadFile(hFile, buf.data(), size, &rd, NULL) && rd == size;
	CloseHandle(hFile);

	if (ok) content.assign(buf.data(), size);
	return ok;
}

// ========== 从嵌入资源读取 list.txt（RCDATA） ==========
static bool LoadListFromResource(std::string& content)
{
	HRSRC hRes = FindResourceA(NULL, MAKEINTRESOURCEA(IDR_LIST_DATA), RT_RCDATA);
	if (!hRes) return false;

	HGLOBAL hGlobal = LoadResource(NULL, hRes);
	if (!hGlobal) return false;

	DWORD size = SizeofResource(NULL, hRes);
	if (size == 0 || size > 65536) return false;

	const char* pData = (const char*)LockResource(hGlobal);
	if (!pData) return false;

	content.assign(pData, size);
	return true;
}

// ========== 解析服务器列表 ==========
static bool ParseList(const std::string& content, GameConfig& config)
{
	std::istringstream stream(content);
	std::string line, section;
	config.servers.clear();

	while (std::getline(stream, line))
	{
		while (!line.empty() && (line[0] == ' ' || line[0] == '\t')) line.erase(0, 1);
		while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r' || line.back() == '\n'))
			line.pop_back();
		if (line.empty() || line[0] == ';') continue;

		if (line[0] == '[')
		{
			size_t end = line.find(']');
			if (end != std::string::npos) section = line.substr(1, end - 1);
			continue;
		}

		size_t eq = line.find('=');
		if (eq == std::string::npos) continue;
		std::string key = line.substr(0, eq);
		std::string value = line.substr(eq + 1);

		if (section == "Config")
		{
			if (key == "游戏公告") config.noticeUrl = value;
			else if (key == "官方网站") config.webSite = value;
			else if (key == "游戏充值") config.buyUrl = value;
		}
		else if (section == "Server")
		{
			std::vector<std::string> parts;
			std::istringstream vs(value);
			std::string part;
			while (std::getline(vs, part, '|')) parts.push_back(part);
			if (parts.size() >= 3)
			{
				ServerInfo srv;
				srv.name = key;
				srv.address = parts[0];
				srv.port = atoi(parts[1].c_str());
				srv.status = parts.size() > 2 ? atoi(parts[2].c_str()) : 1;
				srv.color  = parts.size() > 3 ? atoi(parts[3].c_str()) : 65280;
				srv.loginKey = parts.size() > 5 ? parts[5] : "";
				config.servers.push_back(srv);
			}
		}
	}
	return true;
}

// ========== 更新服务器列表控件 ==========
static void UpdateServerList(HWND hWnd, LoginState* s)
{
	HWND hList = GetDlgItem(hWnd, IDC_LIST_SERVERS);
	SendMessageA(hList, LB_RESETCONTENT, 0, 0);

	for (size_t i = 0; i < s->config.servers.size(); i++)
	{
		std::string name = s->config.servers[i].name;
		SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)name.c_str());
	}

	SendMessageA(hList, LB_SETCURSEL, (WPARAM)-1, 0);
	s->selectedServer = -1;
	s->bServerOnline = false;

	EnableWindow(GetDlgItem(hWnd, IDC_BTN_REGISTER),  FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_BTN_MODIFYPWD), FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_BTN_FORGOT),    FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_BTN_LOGIN),     FALSE);
	SetDlgItemTextA(hWnd, IDC_STATIC_CUR_SERVER, "当前服务器：请选择");
}

// ========== 服务器选择变化处理 ==========
static void OnSelectServer(HWND hWnd, LoginState* s)
{
	HWND hList = GetDlgItem(hWnd, IDC_LIST_SERVERS);
	int sel = (int)SendMessageA(hList, LB_GETCURSEL, 0, 0);

	if (sel >= 0 && sel < (int)s->config.servers.size())
	{
		s->selectedServer = sel;
		ServerInfo& srv = s->config.servers[sel];

		InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_CUR_SERVER), NULL, TRUE);
		SetDlgItemTextA(hWnd, IDC_STATIC_CUR_SERVER, "当前服务器：检测中...");

		EnableWindow(GetDlgItem(hWnd, IDC_BTN_REGISTER),  FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_MODIFYPWD), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_FORGOT),    FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_LOGIN),     FALSE);

		if (srv.status == 0)  // 仅维护状态跳过Ping
		{
			s->bServerOnline = false;
			SetDlgItemTextA(hWnd, IDC_STATIC_CUR_SERVER, "当前服务器：维护中");
		}
		else
		{
			// 后台 Ping
			PingParam* p = new PingParam;
			p->hWnd = hWnd;
			p->address = srv.address;
			p->port = srv.port;
			p->timeoutMs = 200;

			HANDLE hThread = CreateThread(NULL, 0, PingThreadProc, p, 0, NULL);
			if (hThread) CloseHandle(hThread);
		}
	}
	else
	{
		s->selectedServer = -1;
		s->bServerOnline = false;
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_REGISTER),  FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_MODIFYPWD), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_FORGOT),    FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_LOGIN),     FALSE);
		SetDlgItemTextA(hWnd, IDC_STATIC_CUR_SERVER, "当前服务器：未选择");
	}
}

// ========== 后台 Ping 线程 ==========
static DWORD WINAPI PingThreadProc(LPVOID pParam)
{
	PingParam* p = (PingParam*)pParam;
	bool online = PingServer(p->address, p->port, p->timeoutMs);
	PostMessageA(p->hWnd, WM_PING_RESULT, online ? 1 : 0, 0);
	delete p;
	return 0;
}

// ========== 窗口过程 ==========
static LRESULT CALLBACK LoginWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		auto* s = new LoginState();
		memset(s, 0, sizeof(LoginState));
		s->selectedServer = -1;
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)s);

		// 创建 GDI 资源
		s->hTitleFont = gdi::CreateFontEx(18, 0, 0, 0, FW_BOLD, false, false, false, "Microsoft YaHei");
		s->hLabelFont = gdi::CreateFontEx(13, 0, 0, 0, FW_BOLD, false, false, false, "Microsoft YaHei");
		s->hBtnFont   = gdi::CreateFontEx(26, 0, 0, 0, FW_BOLD, false, false, false, "Microsoft YaHei");
		s->hBgBrush    = CreateSolidBrush(COLOR_BG);
		s->hBtnBrush   = CreateSolidBrush(COLOR_RED_BTN);
		s->hPanelBrush = CreateSolidBrush(COLOR_PANEL);
		s->hEditBrush  = CreateSolidBrush(COLOR_WHITE);
		s->hComboBrush = CreateSolidBrush(RGB(240, 240, 240));

		// 动态创建控件（400x320 像素窗口基准）
		// 左上角文字
		LLabel(hWnd, IDC_STATIC_TECH_GUIDE, TECH_GUIDE, 6, 4, 120, 16, SS_NOTIFY|SS_CENTERIMAGE);
		// 版本号
		LLabel(hWnd, IDC_STATIC_VERSION, VERSION_STR, 300, 5, 95, 14);

		// 面板标签
		LLabel(hWnd, IDC_STATIC_LABEL_SERVER, "游戏列表", 12, 28, 50, 14, SS_CENTERIMAGE);
		LListBox(hWnd, IDC_LIST_SERVERS, 10, 42, 122, 136);

		LLabel(hWnd, IDC_STATIC_LABEL_ANN, "游戏公告", 142, 28, 50, 14, SS_CENTERIMAGE);
		LEdit(hWnd, IDC_EDIT_ANNOUNCE, "", 142, 42, 240, 136, ES_MULTILINE|ES_READONLY|ES_NOHIDESEL);

		// 当前服务器
		LLabel(hWnd, IDC_STATIC_CUR_SERVER, "当前服务器：未选择", 10, 188, 122, 14);

		// 功能按钮第一行
		LButton(hWnd, IDC_BTN_REGISTER,  "注册账号", 10, 208, 50, 22);
		LButton(hWnd, IDC_BTN_MODIFYPWD, "修改密码", 62, 208, 50, 22);
		LButton(hWnd, IDC_BTN_FORGOT,    "找回密码", 114, 208, 50, 22);

		// 功能按钮第二行
		LButton(hWnd, IDC_BTN_WEBSITE,  "官方网站", 10, 236, 50, 22);
		LButton(hWnd, IDC_BTN_RECHARGE, "游戏充值", 62, 236, 50, 22);
		LButton(hWnd, IDC_BTN_SETTINGS, "游戏设置", 114, 236, 50, 22);

		// 进入游戏大按钮（自绘）
		LButton(hWnd, IDC_BTN_LOGIN, "进入游戏", 220, 208, 130, 50, BS_OWNERDRAW);

		// 为所有子控件统一应用字体
		{
			HWND child = GetWindow(hWnd, GW_CHILD);
			while (child)
			{
				UINT cid = GetDlgCtrlID(child);
				if (cid == IDC_BTN_LOGIN)
					SendMessageA(child, WM_SETFONT, (WPARAM)s->hBtnFont, TRUE);
				else
					SendMessageA(child, WM_SETFONT, (WPARAM)s->hLabelFont, TRUE);
				child = GetWindow(child, GW_HWNDNEXT);
			}
		}

		// 异步初始化：下载配置和服务器列表
		PostMessageA(hWnd, WM_APP, 0, 0);
		return 0;
	}

	case WM_APP: // 异步初始化
	{
		auto* s = (LoginState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

		// 读取配置
		std::string url;
		std::string name;
		if (ReadConfigFromExe(name, url))
		{
			s->loginerName = name;
			std::string title = name + " - 登录器";
			SetWindowTextA(hWnd, title.c_str());
		}
		else
		{
			s->loginerName = "传奇世界";
		}

		// 加载服务器列表：优先本地文件 > 嵌入资源 > 网络下载
		std::string content;
		bool bListLoaded = false;

		if (LoadListFromFile(content))
		{
			bListLoaded = true;
		}
		else if (LoadListFromResource(content))
		{
			bListLoaded = true;
		}
		else if (!url.empty() && DownloadList(url, content))
		{
			bListLoaded = true;
		}

		if (bListLoaded && ParseList(content, s->config))
		{
			UpdateServerList(hWnd, s);
			// 显示公告
			if (!s->config.noticeUrl.empty())
			{
				std::string announce = "游戏公告\r\n\r\n点击打开公告网址：\r\n"
					+ s->config.noticeUrl + "\r\n\r\n或在浏览器中访问以上网址查看最新公告。";
				SetDlgItemTextA(hWnd, IDC_EDIT_ANNOUNCE, announce.c_str());
			}
			else
				SetDlgItemTextA(hWnd, IDC_EDIT_ANNOUNCE, "暂无公告");
		}
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		GetClientRect(hWnd, &rect);

		auto* s = (LoginState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

		// 绘制背景（皮肤或默认颜色）
		if (s && s->bSkinLoaded && s->pSkinBg)
		{
			Gdiplus::Graphics g(hdc);
			g.DrawImage(s->pSkinBg, (INT)rect.left, (INT)rect.top, (INT)(rect.right - rect.left), (INT)(rect.bottom - rect.top));
		}
		else
		{
			HBRUSH hOld = (HBRUSH)SelectObject(hdc, (s ? s->hBgBrush : GetStockObject(BLACK_BRUSH)));
			FillRect(hdc, &rect, s ? s->hBgBrush : (HBRUSH)GetStockObject(BLACK_BRUSH));
			SelectObject(hdc, hOld);
		}

		// 绘制面板边框
		HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(80, 90, 120));
		HPEN oldPen = (HPEN)SelectObject(hdc, borderPen);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

		Rectangle(hdc, 8, 26, 134, 180);    // 游戏列表面板
		Rectangle(hdc, 140, 26, 392, 180);   // 游戏公告面板

		// 标签与内容分隔线（与面板边框同色重叠）
		MoveToEx(hdc, 10, 40, NULL); LineTo(hdc, 132, 40);
		MoveToEx(hdc, 142, 40, NULL); LineTo(hdc, 390, 40);

		// 当前服务器状态边框
		Rectangle(hdc, 8, 186, 134, 204);

		SelectObject(hdc, oldPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(borderPen);

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_DRAWITEM:
	{
		OnDrawItem(hWnd, wParam, lParam);
		return 0;
	}

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		HWND hCtrl = (HWND)lParam;
		UINT id = GetDlgCtrlID(hCtrl);
		auto* s = (LoginState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		if (!s) return DefWindowProcA(hWnd, msg, wParam, lParam);

		if (id == IDC_STATIC_VERSION)
		{
			SetTextColor(hdc, COLOR_LIGHT_TEXT);
			SetBkMode(hdc, TRANSPARENT);
			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
		if (id == IDC_LIST_SERVERS)
		{
			SetBkColor(hdc, COLOR_PANEL);
			SetTextColor(hdc, COLOR_WHITE);
			return (LRESULT)s->hPanelBrush;
		}
		if (id == IDC_EDIT_ANNOUNCE)
		{
			SetBkColor(hdc, COLOR_WHITE);
			SetTextColor(hdc, RGB(50, 50, 50));
			return (LRESULT)s->hEditBrush;
		}
		if (id == IDC_STATIC_CUR_SERVER)
		{
			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, s->bServerOnline ? COLOR_GREEN_OPEN : COLOR_RED_FULL);
			return (LRESULT)GetStockObject(BLACK_BRUSH);
		}
		if (id == IDC_STATIC_TECH_GUIDE || id == IDC_STATIC_LABEL_SERVER || id == IDC_STATIC_LABEL_ANN)
		{
			SetTextColor(hdc, COLOR_GOLD);
			SetBkMode(hdc, TRANSPARENT);
			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	case WM_COMMAND:
	{
		WORD id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		auto* s = (LoginState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		if (!s) return 0;

		if (code == BN_CLICKED)
		{
			switch (id)
			{
			case IDC_BTN_LOGIN:
			{
				if (s->selectedServer < 0 || s->selectedServer >= (int)s->config.servers.size())
				{
					MessageBoxA(hWnd, "请选择服务器！", "提示", MB_ICONWARNING);
					return 0;
				}
				ServerInfo& srv = s->config.servers[s->selectedServer];
				std::string srvName = srv.name;

				if (!PingServer(srv.address, srv.port, 1500))
				{
					std::string msg = "服务器 [" + srvName + "] 无法连接！\n请确认服务器是否正常运行。";
					MessageBoxA(hWnd, msg.c_str(), "服务器离线", MB_ICONWARNING);
					return 0;
				}

				// 查找游戏目录
				std::string gameDir = FindGameDirectory();

				if (gameDir.empty())
				{
					if (!ShowDirSelectDialog(hWnd, gameDir)) return 0;
				}

				if (!IsValidGameDirectory(gameDir))
				{
					MessageBoxA(hWnd, "该目录不是有效的传奇世界安装目录！\n未找到 Data\\woool.dat.update 文件。",
						"错误", MB_ICONERROR);
					return 0;
				}

				// 写 Game.ini
				std::string iniPath = gameDir + "\\Data\\Game.ini";
				WritePrivateProfileStringA("Config", "Area", "100", iniPath.c_str());
				WritePrivateProfileStringA("Config", "AreaName", srvName.c_str(), iniPath.c_str());
				WritePrivateProfileStringA("Config", "ServerIP", srv.address.c_str(), iniPath.c_str());
				char portStr[16]; snprintf(portStr, sizeof(portStr), "%d", srv.port);
				WritePrivateProfileStringA("Config", "ServerPort", portStr, iniPath.c_str());
				WritePrivateProfileStringA("Config", "GroupNum", "1", iniPath.c_str());
				WritePrivateProfileStringA("Config", "Group0", srvName.c_str(), iniPath.c_str());
				WritePrivateProfileStringA("Config", "GroupNick0", srvName.c_str(), iniPath.c_str());

				// 启动游戏
				std::string exePath = gameDir + "\\Data\\woool.dat.update";
				std::string workDir = gameDir + "\\Data\\";

				STARTUPINFOA si = { sizeof(si) };
				PROCESS_INFORMATION pi = {};
				if (CreateProcessA(exePath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, workDir.c_str(), &si, &pi))
				{
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
					ShowWindow(hWnd, SW_MINIMIZE);
				}
				else
				{
					std::string errMsg = winstr::Format("启动游戏失败！错误代码：%d", GetLastError());
					MessageBoxA(hWnd, errMsg.c_str(), "错误", MB_ICONERROR);
				}
				return 0;
			}

			case IDC_BTN_REGISTER:
			{
				if (s->selectedServer >= 0 && s->selectedServer < (int)s->config.servers.size())
				{
					ServerInfo& srv = s->config.servers[s->selectedServer];
					ShowRegisterWindow((HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
						hWnd, srv.address, srv.port, srv.loginKey);
				}
				return 0;
			}

			case IDC_BTN_MODIFYPWD:
				MessageBoxA(hWnd, "请联系客服修改密码", "修改密码", MB_ICONINFORMATION);
				return 0;

			case IDC_BTN_FORGOT:
				if (!s->config.forgotUrl.empty())
					ShellExecuteA(hWnd, "open", s->config.forgotUrl.c_str(), NULL, NULL, SW_SHOW);
				else
					MessageBoxA(hWnd, "请联系客服找回密码", "找回密码", MB_ICONINFORMATION);
				return 0;

			case IDC_BTN_WEBSITE:
				if (!s->config.webSite.empty())
					ShellExecuteA(hWnd, "open", s->config.webSite.c_str(), NULL, NULL, SW_SHOW);
				return 0;

			case IDC_BTN_RECHARGE:
				if (!s->config.buyUrl.empty())
					ShellExecuteA(hWnd, "open", s->config.buyUrl.c_str(), NULL, NULL, SW_SHOW);
				return 0;

			case IDC_BTN_SETTINGS:
			{
				char exePath[MAX_PATH];
				GetModuleFileNameA(NULL, exePath, MAX_PATH);
				std::string dir(exePath);
				size_t pos = dir.find_last_of("\\/");
				if (pos != std::string::npos) dir = dir.substr(0, pos + 1);
				std::string cfgPath = dir + "wooolcfg.exe";
				ShellExecuteA(hWnd, "open", cfgPath.c_str(), NULL, NULL, SW_SHOW);
				return 0;
			}
			}
		}

		if (id == IDC_LIST_SERVERS && code == LBN_SELCHANGE)
		{
			OnSelectServer(hWnd, s);
			return 0;
		}

		return 0;
	}

	case WM_PING_RESULT:
	{
		auto* s = (LoginState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		if (!s) return 0;

		bool online = (wParam != 0);
		s->bServerOnline = online;

		if (s->selectedServer >= 0 && s->selectedServer < (int)s->config.servers.size())
		{
			ServerInfo& srv = s->config.servers[s->selectedServer];
			bool canLogin = online && (srv.status != 0);  // 只要不维护即可登录
			EnableWindow(GetDlgItem(hWnd, IDC_BTN_LOGIN),     canLogin);
			EnableWindow(GetDlgItem(hWnd, IDC_BTN_REGISTER),  online);
			EnableWindow(GetDlgItem(hWnd, IDC_BTN_MODIFYPWD), online);
			EnableWindow(GetDlgItem(hWnd, IDC_BTN_FORGOT),    online);

			const char* text = online ? "  当前服务器：正常" : "  当前服务器：关闭";
			InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_CUR_SERVER), NULL, TRUE);
			SetDlgItemTextA(hWnd, IDC_STATIC_CUR_SERVER, text);
		}
		return 0;
	}

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
	{
		auto* s = (LoginState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		if (s)
		{
			if (s->hTitleFont)  DeleteObject(s->hTitleFont);
			if (s->hLabelFont)  DeleteObject(s->hLabelFont);
			if (s->hBtnFont)    DeleteObject(s->hBtnFont);
			if (s->hBgBrush)    DeleteObject(s->hBgBrush);
			if (s->hBtnBrush)   DeleteObject(s->hBtnBrush);
			if (s->hPanelBrush) DeleteObject(s->hPanelBrush);
			if (s->hEditBrush)  DeleteObject(s->hEditBrush);
			if (s->hComboBrush) DeleteObject(s->hComboBrush);
			delete s->pSkinBg;
			delete s->pSkinTitle;
			delete s->pSkinLoginBtn;
			delete s;
		}
		PostQuitMessage(0);
		return 0;
	}

	default:
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
}

// ========== 自绘控件（服务器列表 + 进入游戏按钮） ==========
static void OnDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lp = (LPDRAWITEMSTRUCT)lParam;
	auto* s = (LoginState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	if (!s) return;

	UINT id = (UINT)wParam;

	if (id == IDC_LIST_SERVERS)
	{
		HDC hdc = lp->hDC;
		int nIndex = lp->itemID;
		RECT rect = lp->rcItem;

		if (nIndex >= 0 && nIndex < (int)s->config.servers.size())
		{
			ServerInfo& srv = s->config.servers[nIndex];

			if (lp->itemState & ODS_SELECTED)
			{
				HBRUSH hBr = CreateSolidBrush(RGB(60, 80, 120));
				FillRect(hdc, &rect, hBr);
				DeleteObject(hBr);
			}
			else
			{
				FillRect(hdc, &rect, s->hPanelBrush);
			}

			SetBkMode(hdc, TRANSPARENT);

			const char* statusText = GetStatusText(srv.status);
			SetTextColor(hdc, GetStatusColor(srv.status));
			TextOutA(hdc, rect.left + 2, rect.top + 1, statusText, (int)strlen(statusText));

			// 获取状态文字宽度用于偏移
			SIZE sz;
			GetTextExtentPoint32A(hdc, statusText, (int)strlen(statusText), &sz);

			std::string srvName = srv.name;
			SetTextColor(hdc, COLOR_WHITE);
			TextOutA(hdc, rect.left + sz.cx + 6, rect.top + 1, srvName.c_str(), (int)srvName.size());
		}
		return;
	}

	if (id == IDC_BTN_LOGIN)
	{
		HDC hdc = lp->hDC;
		RECT rect = lp->rcItem;

		// 皮肤按钮或默认绘制
		if (s->pSkinLoginBtn && s->pSkinLoginBtn->GetLastStatus() == Gdiplus::Ok)
		{
			Gdiplus::Graphics g(hdc);
			g.DrawImage(s->pSkinLoginBtn, (INT)rect.left, (INT)rect.top, (INT)(rect.right - rect.left), (INT)(rect.bottom - rect.top));
		}
		else
		{
			FillRect(hdc, &rect, s->hBtnBrush);
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 100, 100));
			HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
			HBRUSH oldBr = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBr);
			DeleteObject(hPen);
		}

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, COLOR_GOLD);
		HFONT oldFont = (HFONT)SelectObject(hdc, s->hBtnFont);
		DrawTextA(hdc, "进入游戏", -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		SelectObject(hdc, oldFont);
		return;
	}
}

// ========== 显示登录器窗口 ==========
int ShowLoginWindow(HINSTANCE hInstance)
{
	RegisterLoginClass(hInstance);

	// 先创建一个临时窗口获取加载后的尺寸
	HWND hWnd = CreateWindowExA(
		0, LOGIN_CLASS, "传奇世界 - 登录器",
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 306,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return -1;

	// 居中
	RECT rc;
	GetWindowRect(hWnd, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
	SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// 模态消息循环
	MSG msg;
	while (GetMessageA(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return (int)msg.wParam;
}

} // namespace app
