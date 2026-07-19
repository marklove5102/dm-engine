// RegisterWnd.cpp - 注册窗口实现（替代 CRegisterDlg）
#include "stdafx.h"
#include "RegisterWnd.h"
#include "WinString.h"
#include "GdiRaii.h"
#include "Protocol.h"
#include "resource.h"

namespace app {

// ========== 注册窗口状态 ==========
struct RegisterState
{
	HFONT        hFont;
	std::string  serverAddr;
	int          serverPort;
	std::string  loginKey;
	bool         success;       // 注册是否成功
};

// ========== 注册账号结构体（与服务器一致） ==========
#pragma pack(push, 1)
struct REGACC
{
	BYTE btAccount;     char szAccount[10];
	BYTE btPassword;    char szPassword[10];
	BYTE btName;        char szName[20];
	BYTE btIdCard;      char szIdCard[19];
	BYTE btPhoneNumber; char szPhoneNumber[14];
	BYTE btQ1;          char szQ1[20];
	BYTE btA1;          char szA1[20];
	BYTE btEmail;       char szEmail[40];
	BYTE btQ2;          char szQ2[20];
	BYTE btA2;          char szA2[20];
	BYTE btBirthday;    char szBirthday[10];
	BYTE btMobileNumber;char szMobileNumber[10];
};
#pragma pack(pop)

// ========== 辅助函数：字符串安全填充 ==========
static void SafeFill(BYTE& lenField, char* dst, int maxLen, const std::string& src)
{
	int n = (int)src.size();
	if (n > maxLen) n = maxLen;
	lenField = (BYTE)n;
	memcpy(dst, src.c_str(), n);
}

// ========== 前向声明 ==========
static LRESULT CALLBACK RegisterWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void OnRegisterOk(HWND hWnd);

// ========== 窗口类 ==========
static const char* REGISTER_CLASS = "LoginTool_RegisterWnd";

static void RegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXA wc = {};
	wc.cbSize        = sizeof(WNDCLASSEXA);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = RegisterWndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursorA(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = REGISTER_CLASS;
	RegisterClassExA(&wc);
}

// ========== 控件创建辅助 ==========
static HWND RegLabel(HWND hParent, int id, const char* text, int x, int y, int w, int h)
{
	return CreateWindowExA(0, "STATIC", text,
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x, y, w, h, hParent, (HMENU)(INT_PTR)id, nullptr, nullptr);
}

static HWND RegEdit(HWND hParent, int id, const char* text, int x, int y, int w, int h, DWORD style = 0)
{
	return CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", text,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | style,
		x, y, w, h, hParent, (HMENU)(INT_PTR)id, nullptr, nullptr);
}

static HWND RegButton(HWND hParent, int id, const char* text, int x, int y, int w, int h)
{
	return CreateWindowExA(0, "BUTTON", text,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
		x, y, w, h, hParent, (HMENU)(INT_PTR)id, nullptr, nullptr);
}

// ========== 窗口过程 ==========
static LRESULT CALLBACK RegisterWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		auto* state = new RegisterState();
		state->hFont    = gdi::CreateFontEx(14, 0, 0, 0, FW_NORMAL, false, false, false, "Microsoft YaHei");
		state->success  = false;
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)state);

		// 创建控件（简化布局，每行 28px 间距）
		int y = 8;
		const int LH = 16;  // 标签高度
		const int EH = 20;  // 编辑框高度
		const int LX = 10;  // 标签 X
		const int LW = 70;  // 标签宽度
		const int EX = 85;  // 编辑框 X
		const int EW = 125; // 编辑框宽度

		RegLabel(hWnd, IDC_STATIC,            "账号：",     LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_ACCOUNT,  "",           EX, y,    EW, EH);  y += 28;

		RegLabel(hWnd, IDC_STATIC,            "密码：",     LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_PASSWORD, "",           EX, y,    EW, EH, ES_PASSWORD); y += 28;

		RegLabel(hWnd, IDC_STATIC,            "确认密码：", LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_CONFIRM,  "",           EX, y,    EW, EH, ES_PASSWORD); y += 28;

		RegLabel(hWnd, IDC_STATIC,            "真实姓名：", LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_NAME,     "",           EX, y,    EW, EH);  y += 28;

		RegLabel(hWnd, IDC_STATIC,            "出生日期：", LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_BIRTHDAY, "1990/01/01", EX, y,    EW, EH);  y += 28;

		RegLabel(hWnd, IDC_STATIC,            "问题1：",    LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_Q1,       "",           EX, y,    EW, EH);  y += 28;

		RegLabel(hWnd, IDC_STATIC,            "答案1：",    LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_A1,       "",           EX, y,    EW, EH);  y += 28;

		RegLabel(hWnd, IDC_STATIC,            "问题2：",    LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_Q2,       "",           EX, y,    EW, EH);  y += 28;

		RegLabel(hWnd, IDC_STATIC,            "答案2：",    LX, y+3,  LW, LH);
		RegEdit (hWnd, IDC_EDIT_REG_A2,       "",           EX, y,    EW, EH);  y += 32;

		// 按钮
		RegButton(hWnd, IDC_BTN_REG_OK,     "确定注册",    55,  y, 60, 24);
		RegButton(hWnd, IDC_BTN_REG_CANCEL, "取消",       125, y, 50, 24);

		// 设置字体
		HWND child = GetWindow(hWnd, GW_CHILD);
		while (child)
		{
			SendMessageA(child, WM_SETFONT, (WPARAM)state->hFont, TRUE);
			child = GetWindow(child, GW_HWNDNEXT);
		}
		return 0;
	}

	case WM_COMMAND:
	{
		WORD id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if (id == IDC_BTN_REG_OK && code == BN_CLICKED)
		{
			OnRegisterOk(hWnd);
			return 0;
		}

		if (id == IDC_BTN_REG_CANCEL && code == BN_CLICKED)
		{
			DestroyWindow(hWnd);
			return 0;
		}
		return 0;
	}

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
	{
		auto* state = (RegisterState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		int exitCode = 0;
		if (state)
		{
			exitCode = state->success ? 1 : 0;
			if (state->hFont) DeleteObject(state->hFont);
			delete state;
		}
		PostQuitMessage(exitCode);
		return 0;
	}

	default:
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
}

// ========== 注册逻辑（从 CRegisterDlg::OnBnClickedOk 移植） ==========
static void OnRegisterOk(HWND hWnd)
{
	auto* state = (RegisterState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	if (!state) return;

	// 收集输入
	char account[256]  = {0}, password[256] = {0}, confirm[256] = {0};
	char name[256]     = {0}, birthday[256] = {0};
	char q1[256] = {0}, a1[256] = {0}, q2[256] = {0}, a2[256] = {0};

	GetDlgItemTextA(hWnd, IDC_EDIT_REG_ACCOUNT,  account,  sizeof(account)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_PASSWORD, password, sizeof(password)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_CONFIRM,  confirm,  sizeof(confirm)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_NAME,     name,     sizeof(name)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_BIRTHDAY, birthday, sizeof(birthday)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_Q1,       q1,       sizeof(q1)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_A1,       a1,       sizeof(a1)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_Q2,       q2,       sizeof(q2)-1);
	GetDlgItemTextA(hWnd, IDC_EDIT_REG_A2,       a2,       sizeof(a2)-1);

	// 基本验证
	if (account[0] == '\0' || password[0] == '\0')
	{
		MessageBoxA(hWnd, "请填写账号和密码！", "提示", MB_ICONWARNING);
		return;
	}
	if (strcmp(password, confirm) != 0)
	{
		MessageBoxA(hWnd, "两次输入的密码不一致！", "提示", MB_ICONWARNING);
		return;
	}

	// 校验出生日期
	int year = 0, month = 0, day = 0;
	if (strlen(birthday) >= 8)
	{
		// 标准化分隔符
		std::string bd = birthday;
		for (auto& c : bd) { if (c == '-' || c == '.') c = '/'; }
		if (sscanf(bd.c_str(), "%d/%d/%d", &year, &month, &day) == 3)
		{
			if (year < 1900 || year > 2020 || month < 1 || month > 12 || day < 1 || day > 31)
			{
				MessageBoxA(hWnd, "出生日期格式错误！", "提示", MB_ICONWARNING);
				return;
			}
			snprintf(birthday, sizeof(birthday), "%04d/%02d/%02d", year, month, day);
		}
	}

	if (state->serverAddr.empty() || state->serverPort == 0)
	{
		MessageBoxA(hWnd, "服务器信息未配置！", "提示", MB_ICONWARNING);
		return;
	}

	// 锁定 UI
	SetDlgItemTextA(hWnd, IDC_BTN_REG_OK, "注册中...");
	EnableWindow(GetDlgItem(hWnd, IDC_BTN_REG_OK), FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_BTN_REG_CANCEL), FALSE);

	bool regSuccess = false;
	std::string errorMsg;

	// ========== TCP 注册流程 ==========
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
	{
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock != INVALID_SOCKET)
		{
			DWORD timeout = 8000;
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

			struct sockaddr_in addr = {};
			addr.sin_family = AF_INET;
			addr.sin_port   = htons(state->serverPort);
			addr.sin_addr.s_addr = inet_addr(state->serverAddr.c_str());

			if (addr.sin_addr.s_addr != INADDR_NONE)
			{
				// 非阻塞连接
				u_long nb = 1;
				ioctlsocket(sock, FIONBIO, &nb);
				int cr = connect(sock, (struct sockaddr*)&addr, sizeof(addr));

				if (cr != 0 && WSAGetLastError() == WSAEWOULDBLOCK)
				{
					fd_set ws;
					FD_ZERO(&ws);
					FD_SET(sock, &ws);
					timeval tv = {5, 0};
					if (select(0, NULL, &ws, NULL, &tv) > 0)
					{
						nb = 0;
						ioctlsocket(sock, FIONBIO, &nb);

						// 等待服务器初始消息
						Sleep(300);
						char initBuf[1024] = {0};
						int initRecv = 0;
						for (int i = 0; i < 5; i++)
						{
							fd_set fds;
							FD_ZERO(&fds);
							FD_SET(sock, &fds);
							timeval tv2 = {1, 0};
							if (select(0, &fds, NULL, NULL, &tv2) <= 0) break;
							int n = recv(sock, initBuf + initRecv, (int)(sizeof(initBuf) - 1 - initRecv), 0);
							if (n <= 0) break;
							initRecv += n;
							bool hasEnd = false;
							for (int j = 0; j < initRecv; j++) { if (initBuf[j] == '!') { hasEnd = true; break; } }
							if (hasEnd) break;
						}

						// 构造 REGACC
						REGACC reg = {};
						SafeFill(reg.btAccount,  reg.szAccount,  10, account);
						SafeFill(reg.btPassword, reg.szPassword, 10, password);
						SafeFill(reg.btName,     reg.szName,     20, name);
						SafeFill(reg.btIdCard,   reg.szIdCard,   19, "");
						SafeFill(reg.btPhoneNumber, reg.szPhoneNumber, 14, "");
						SafeFill(reg.btQ1,       reg.szQ1,       20, q1);
						SafeFill(reg.btA1,       reg.szA1,       20, a1);
						SafeFill(reg.btEmail,    reg.szEmail,    40, "");
						SafeFill(reg.btQ2,       reg.szQ2,       20, q2);
						SafeFill(reg.btA2,       reg.szA2,       20, a2);
						SafeFill(reg.btBirthday, reg.szBirthday, 10, birthday);
						SafeFill(reg.btMobileNumber, reg.szMobileNumber, 10, "");

						// 编码消息头和注册数据
						char headerBuf[12] = {};
						WORD wCmd = protocol::CM_ADDNEWUSER;
						memcpy(headerBuf + 4, &wCmd, 2);
						char encHeader[64];
						int encHeaderLen = protocol::CodeGameCode((BYTE*)headerBuf, 12, (BYTE*)encHeader);

						char encData[1024];
						int encDataLen = protocol::CodeGameCode((BYTE*)&reg, 226, (BYTE*)encData);

						std::string pwdStr = "Password@" + state->loginKey;
						char pwdEnc[256];
						int pwdEncLen = protocol::CodeGameCode((BYTE*)pwdStr.c_str(), (int)pwdStr.size(), (BYTE*)pwdEnc);

						// 组装数据包
						char packet[2048];
						int packetLen = 0;
						packet[packetLen++] = '#';
						packet[packetLen++] = '1';
						memcpy(packet + packetLen, encHeader, encHeaderLen); packetLen += encHeaderLen;
						memcpy(packet + packetLen, encData, encDataLen);   packetLen += encDataLen;
						packet[packetLen++] = '!';
						memcpy(packet + packetLen, pwdEnc, pwdEncLen);     packetLen += pwdEncLen;

						send(sock, packet, packetLen, 0);
						Sleep(1000); // 等待服务器处理

						// 接收响应（最多 15 秒）
						char buf[4096] = {0};
						int totalRecv = 0;
						int starCount = 0;

						for (int attempt = 0; attempt < 15; attempt++)
						{
							fd_set fds;
							FD_ZERO(&fds);
							FD_SET(sock, &fds);
							timeval tv = {1, 0};
							int sr = select(0, &fds, NULL, NULL, &tv);
							if (sr == 0) continue;
							if (sr < 0) { errorMsg = "select错误"; break; }

							int n = recv(sock, buf + totalRecv, (int)(sizeof(buf) - 1 - totalRecv), 0);
							if (n < 0) { errorMsg = winstr::Format("recv错误: %d", WSAGetLastError()); break; }
							if (n == 0) { errorMsg = "服务器关闭连接"; break; }
							totalRecv += n;

							// 处理服务器 ping (*)
							bool hasStar = false;
							for (int i = 0; i < totalRecv; i++) { if ((BYTE)buf[i] == 0x2A) hasStar = true; }
							if (hasStar)
							{
								starCount++;
								send(sock, "*", 1, 0);
								int wp = 0;
								for (int i = 0; i < totalRecv; i++) { if ((BYTE)buf[i] != 0x2A) buf[wp++] = buf[i]; }
								totalRecv = wp;
								if (starCount >= 5) { errorMsg = winstr::Format("服务器连续%d次ping，无注册响应", starCount); break; }
								continue;
							}
							break;
						}

						if (totalRecv > 0 && errorMsg.empty())
						{
							bool ok = false;
							std::string failReason;

							// 解析 #...! 响应
							int searchPos = 0;
							while (searchPos < totalRecv - 1)
							{
								int hashPos = -1;
								for (int i = searchPos; i < totalRecv - 1; i++) { if (buf[i] == '#') { hashPos = i; break; } }
								if (hashPos < 0) break;

								int bangPos = -1;
								for (int j = hashPos + 1; j < totalRecv; j++) { if (buf[j] == '!') { bangPos = j; break; } }
								if (bangPos < 0) break;

								char* encStart = buf + hashPos + 1;
								if (*encStart >= '0' && *encStart <= '9') encStart++;
								buf[bangPos] = 0;
								BYTE decoded[1024];
								int decLen = protocol::UnGameCode(encStart, decoded);
								buf[bangPos] = '!';

								if (decLen > 4)
								{
									WORD rc = *(WORD*)(decoded + 4);
									if (rc == protocol::SM_REGISTERACCOUNTOK) ok = true;
									else if (rc == protocol::SM_REGISTERACCOUNTFAIL && failReason.empty())
										failReason = "注册失败";
									else if (rc == 0x0afa && decLen > 12)
									{
										std::string txt((char*)(decoded + 12));
										if (!txt.empty()) failReason = txt;
									}
								}
								searchPos = bangPos + 1;
							}

							// 再等 2 秒看是否有后续消息
							Sleep(2000);
							char buf2[4096] = {0};
							fd_set fds2;
							FD_ZERO(&fds2);
							FD_SET(sock, &fds2);
							timeval tv2 = {3, 0};
							if (select(0, &fds2, NULL, NULL, &tv2) > 0)
							{
								int n2 = recv(sock, buf2, sizeof(buf2) - 1, 0);
								if (n2 > 0 && totalRecv + n2 < (int)sizeof(buf))
								{
									memcpy(buf + totalRecv, buf2, n2);
									totalRecv += n2;

									ok = false;
									failReason.clear();
									searchPos = 0;
									while (searchPos < totalRecv - 1)
									{
										int hashPos = -1;
										for (int i = searchPos; i < totalRecv - 1; i++) { if (buf[i] == '#') { hashPos = i; break; } }
										if (hashPos < 0) break;
										int bangPos = -1;
										for (int j = hashPos + 1; j < totalRecv; j++) { if (buf[j] == '!') { bangPos = j; break; } }
										if (bangPos < 0) break;
										char* encStart = buf + hashPos + 1;
										if (*encStart >= '0' && *encStart <= '9') encStart++;
										buf[bangPos] = 0;
										BYTE decoded[1024];
										int decLen = protocol::UnGameCode(encStart, decoded);
										buf[bangPos] = '!';
										if (decLen > 4)
										{
											WORD rc = *(WORD*)(decoded + 4);
											if (rc == protocol::SM_REGISTERACCOUNTOK) ok = true;
											else if (rc == protocol::SM_REGISTERACCOUNTFAIL && failReason.empty())
												failReason = "注册失败";
											else if (rc == 0x0afa && decLen > 12)
											{
												std::string txt((char*)(decoded + 12));
												if (!txt.empty()) failReason = txt;
											}
										}
										searchPos = bangPos + 1;
									}
								}
							}

							if (ok) regSuccess = true;
							else if (!failReason.empty() && failReason != "注册失败")
								errorMsg = failReason;
							else if (errorMsg.empty())
								errorMsg = "注册失败，该账号可能已存在";
						}
						else if (errorMsg.empty())
							errorMsg = "服务器15秒内无响应";
					}
					else { errorMsg = winstr::Format("连接超时 %s:%d", state->serverAddr.c_str(), state->serverPort); }
				}
				else
				{
					errorMsg = winstr::Format("无法连接 %s:%d", state->serverAddr.c_str(), state->serverPort);
				}
			}
			else { errorMsg = "服务器地址无效"; }
			closesocket(sock);
		}
		else { errorMsg = "创建连接失败"; }
		WSACleanup();
	}
	else { errorMsg = "网络初始化失败"; }

	// 恢复 UI
	SetDlgItemTextA(hWnd, IDC_BTN_REG_OK, "确定注册");
	EnableWindow(GetDlgItem(hWnd, IDC_BTN_REG_OK), TRUE);
	EnableWindow(GetDlgItem(hWnd, IDC_BTN_REG_CANCEL), TRUE);

	if (regSuccess)
	{
		state->success = true;
		std::string msg = "注册成功！\n账号：" + std::string(account);
		MessageBoxA(hWnd, msg.c_str(), "注册结果", MB_ICONINFORMATION);
		DestroyWindow(hWnd);
	}
	else
	{
		if (errorMsg.empty()) errorMsg = "注册失败，请稍后重试";
		MessageBoxA(hWnd, errorMsg.c_str(), "注册失败", MB_ICONERROR);
	}
}

// ========== 显示注册窗口（模态循环） ==========
bool ShowRegisterWindow(HINSTANCE hInstance, HWND hParent,
                        const std::string& serverAddr, int serverPort,
                        const std::string& loginKey)
{
	RegisterClass(hInstance);

	HWND hWnd = CreateWindowExA(
		0, REGISTER_CLASS, "注册账号",
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		230, 340,
		hParent, NULL, hInstance, NULL);

	if (!hWnd) return false;

	// 设置状态（在 WM_CREATE 之后）
	auto* state = (RegisterState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
	if (state)
	{
		state->serverAddr = serverAddr;
		state->serverPort = serverPort;
		state->loginKey   = loginKey;
	}

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

	// msg.wParam 来自 PostQuitMessage 的退出码（WM_DESTROY 根据 state->success 设置）
	return (msg.wParam == 1);
}

} // namespace app

