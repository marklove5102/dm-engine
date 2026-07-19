// BuilderWnd.cpp - 配置器窗口实现（替代 BuilderDlg）
#include "stdafx.h"
#include "BuilderWnd.h"
#include "WinString.h"
#include "GdiRaii.h"
#include "resource.h"

namespace app {

// ========== 配置器窗口状态 ==========
struct BuilderState
{
	HFONT  hFont;       // 控件字体
};

// ========== 前向声明 ==========
static LRESULT CALLBACK BuilderWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ========== 窗口类注册 ==========
static const char* BUILDER_CLASS = "LoginTool_BuilderWnd";

static void RegisterBuilderClass(HINSTANCE hInstance)
{
	WNDCLASSEXA wc = {};
	wc.cbSize        = sizeof(WNDCLASSEXA);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = BuilderWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIconA(hInstance, MAKEINTRESOURCEA(IDR_MAINFRAME));
	wc.hCursor       = LoadCursorA(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = BUILDER_CLASS;
	RegisterClassExA(&wc);
}

// ========== 创建控件宏 ==========
static HWND CreateLabel(HWND hParent, int id, const char* text, int x, int y, int w, int h)
{
	HWND ctrl = CreateWindowExA(0, "STATIC", text,
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		x, y, w, h, hParent, (HMENU)(INT_PTR)id, nullptr, nullptr);
	return ctrl;
}

static HWND CreateEdit(HWND hParent, int id, const char* text, int x, int y, int w, int h)
{
	HWND ctrl = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", text,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
		x, y, w, h, hParent, (HMENU)(INT_PTR)id, nullptr, nullptr);
	return ctrl;
}

static HWND CreateButton(HWND hParent, int id, const char* text, int x, int y, int w, int h)
{
	HWND ctrl = CreateWindowExA(0, "BUTTON", text,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
		x, y, w, h, hParent, (HMENU)(INT_PTR)id, nullptr, nullptr);
	return ctrl;
}

// ========== 窗口过程 ==========
static LRESULT CALLBACK BuilderWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		// 创建字体
		auto* state = new BuilderState();
		state->hFont = gdi::CreateFontEx(14, 0, 0, 0, FW_NORMAL, false, false, false, "Microsoft YaHei");
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)state);

		// 动态创建控件（像素坐标）
		CreateLabel(hWnd, IDC_STATIC,         "登录器名称:", 10, 10, 65, 16);
		CreateEdit (hWnd, IDC_EDIT_NAME,      "传奇世界",   80,  8, 160, 20);
		CreateLabel(hWnd, IDC_STATIC,         "列表 URL:",  10, 38, 65, 16);
		CreateEdit (hWnd, IDC_EDIT_URL,       "http://127.0.0.1/list.txt", 80, 36, 160, 20);
		CreateButton(hWnd, IDC_BTN_GENERATE,  "生成登录器", 45, 70, 160, 36);

		// 设置控件字体
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

		if (id == IDC_BTN_GENERATE && code == BN_CLICKED)
		{
			char nameBuf[256] = {0};
			char urlBuf[1024] = {0};
			GetDlgItemTextA(hWnd, IDC_EDIT_NAME, nameBuf, sizeof(nameBuf) - 1);
			GetDlgItemTextA(hWnd, IDC_EDIT_URL,  urlBuf,  sizeof(urlBuf) - 1);

			if (nameBuf[0] == '\0' || urlBuf[0] == '\0')
			{
				MessageBoxA(hWnd, "请填写名称和 URL！", "提示", MB_ICONWARNING);
				return 0;
			}

			// 获取 EXE 所在目录
			char exePath[MAX_PATH];
			GetModuleFileNameA(NULL, exePath, MAX_PATH);
			std::string exeDir(exePath);
			size_t slashPos = exeDir.find_last_of("\\/");
			if (slashPos != std::string::npos) exeDir = exeDir.substr(0, slashPos);

			// 创建 output 子目录
			std::string outputDir = exeDir + "\\output";
			int dirResult = SHCreateDirectoryExA(NULL, outputDir.c_str(), NULL);
			if (dirResult != ERROR_SUCCESS && dirResult != ERROR_ALREADY_EXISTS)
			{
				std::string errMsg = "创建 output 目录失败！\n错误码：" + std::to_string(dirResult);
				errMsg += "\n路径: " + outputDir;
				MessageBoxA(hWnd, errMsg.c_str(), "错误", MB_ICONERROR);
				return 0;
			}

			// 构建配置数据
			std::string configData = CONFIG_MARKER_BEGIN;
			configData += nameBuf;
			configData += "|";
			configData += urlBuf;
			configData += CONFIG_MARKER_END;

			// 过滤文件名非法字符
			{
				static const char* ILLEGAL_CHARS = "\\/:*?\"<>|";
				for (char* p = nameBuf; *p; ++p)
					if (strchr(ILLEGAL_CHARS, *p)) *p = '_';
				// 去除首尾空格
				size_t len = strlen(nameBuf);
				while (len > 0 && nameBuf[len - 1] == ' ') nameBuf[--len] = '\0';
				if (nameBuf[0] == '\0')
				{
					MessageBoxA(hWnd, "名称过滤后为空，请使用有效字符！", "提示", MB_ICONWARNING);
					return 0;
				}
			}

			// 目标路径
			char dest[MAX_PATH];
			snprintf(dest, sizeof(dest), "%s\\%s.exe", outputDir.c_str(), nameBuf);

			// 清理残留
			SetFileAttributesA(dest, FILE_ATTRIBUTE_NORMAL);
			DeleteFileA(dest);

			// === 一次性完成：读取源EXE到内存 → 写入目标EXE → 紧接着追加配置 ===
			bool bTailOk = false;
			std::string errDetail;

			// 1) 将源 EXE 读入内存
			HANDLE hSrc = CreateFileA(exePath, GENERIC_READ, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hSrc != INVALID_HANDLE_VALUE)
			{
				DWORD srcSize = GetFileSize(hSrc, NULL);
				std::vector<BYTE> buffer(srcSize);
				DWORD rd = 0;
				if (ReadFile(hSrc, buffer.data(), srcSize, &rd, NULL) && rd == srcSize)
				{
					// 2) 创建目标文件，写入 EXE 内容 + 配置尾部
					HANDLE hDst = CreateFileA(dest, GENERIC_WRITE, FILE_SHARE_READ,
						NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hDst != INVALID_HANDLE_VALUE)
					{
						DWORD wr = 0;
						WriteFile(hDst, buffer.data(), srcSize, &wr, NULL);

						// 紧接写入配置
						DWORD wr2 = 0;
						WriteFile(hDst, configData.c_str(), (DWORD)configData.size(), &wr2, NULL);
						// 强制刷盘，消除 CloseHandle 后立即读取的竞态窗口
						FlushFileBuffers(hDst);
						CloseHandle(hDst);

						if (wr == srcSize && wr2 == (DWORD)configData.size())
						{
							// 3) 验证：读取文件末尾，与 configData 逐字节比对
							//    使用 memcmp 而非 strstr，因为 PE 文件尾部有 0x00 零填充，
							//    strstr 遇到 0x00 会立即截断，无法跨越零字节找到配置标记
							HANDLE hVerify = CreateFileA(dest, GENERIC_READ, FILE_SHARE_READ,
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
							if (hVerify != INVALID_HANDLE_VALUE)
							{
								DWORD totalSize = GetFileSize(hVerify, NULL);
								DWORD configLen = (DWORD)configData.size();
								if (totalSize >= configLen)
								{
									SetFilePointer(hVerify, totalSize - configLen, NULL, FILE_BEGIN);
									std::vector<char> endBuf(configLen);
									DWORD endRd = 0;
									if (ReadFile(hVerify, endBuf.data(), configLen, &endRd, NULL)
										&& endRd == configLen
										&& memcmp(endBuf.data(), configData.c_str(), configLen) == 0)
									{
										bTailOk = true;
									}
									else
									{
										errDetail = "EXE尾部验证失败：末尾"
										          + std::to_string(configLen) + "字节比对不匹配\n"
										          "文件大小=" + std::to_string(totalSize)
										          + " 预期=" + std::to_string(srcSize + configData.size())
										          + " 读取=" + std::to_string(endRd);
									}
								}
								else
								{
									errDetail = "EXE尾部验证失败：文件过小\n"
									          "文件大小=" + std::to_string(totalSize)
									          + " 预期至少=" + std::to_string(configLen);
								}
								CloseHandle(hVerify);
							}
							else
								errDetail = "验证打开失败，错误码=" + std::to_string(GetLastError());
						}
						else
							errDetail = "写入不完整 src=" + std::to_string(wr) + "/" + std::to_string(srcSize)
							          + " cfg=" + std::to_string(wr2) + "/" + std::to_string(configData.size());
					}
					else
						errDetail = "创建目标文件失败，错误码=" + std::to_string(GetLastError());
				}
				else
					errDetail = "读取源EXE失败，错误码=" + std::to_string(GetLastError());
				CloseHandle(hSrc);
			}
			else
				errDetail = "打开源EXE失败，错误码=" + std::to_string(GetLastError());

			// 如果嵌入失败，回退到 CopyFileA
			if (!bTailOk)
			{
				DeleteFileA(dest);
				CopyFileA(exePath, dest, FALSE);
			}

			// 生成成功时同步释放 list.txt
			if (bTailOk)
			{
				std::string srcList = exeDir + "\\list.txt";
				std::string dstList = outputDir + "\\list.txt";
				if (GetFileAttributesA(srcList.c_str()) != INVALID_FILE_ATTRIBUTES)
				{
					DeleteFileA(dstList.c_str());
					CopyFileA(srcList.c_str(), dstList.c_str(), FALSE);
				}
			}

			// 结果提示
			std::string msg = "output\\" + std::string(nameBuf) + ".exe";
			if (!bTailOk)
			{
				msg = errDetail + "\n\n" + msg;
				MessageBoxA(hWnd, msg.c_str(),"失败", MB_ICONINFORMATION);
			}
			else
			{
				MessageBoxA(hWnd, "登录器生成成功", "成功", MB_ICONINFORMATION);
			}
			return 0;
		}
		return 0;
	}

	case WM_DESTROY:
	{
		auto* state = (BuilderState*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		if (state)
		{
			if (state->hFont) DeleteObject(state->hFont);
			delete state;
		}
		PostQuitMessage(0);
		return 0;
	}

	default:
		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}
}

// ========== 显示配置器窗口（模态循环） ==========
int ShowBuilderWindow(HINSTANCE hInstance)
{
	RegisterBuilderClass(hInstance);

	HWND hWnd = CreateWindowExA(
		0,                          // 扩展样式
		BUILDER_CLASS,              // 窗口类名
		"传奇世界 - 登录器配置器",    // 标题
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,  // 样式
		CW_USEDEFAULT, CW_USEDEFAULT,  // 位置（系统决定）
		260, 160,                   // 窗口大小（像素）x,y
		NULL,                       // 父窗口
		NULL,                       // 菜单
		hInstance,
		NULL);

	if (!hWnd)
		return -1;

	// 居中显示
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
