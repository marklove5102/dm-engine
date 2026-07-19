// main.cpp - Win32 应用程序入口（替代 Loginer.cpp）
#include "stdafx.h"
#include "WinString.h"

// 前向声明 — 各窗口创建函数（实现在各自的 .cpp 中）
namespace app {

// 显示配置器窗口（无嵌入配置时显示）
int ShowBuilderWindow(HINSTANCE hInstance);

// 显示登录器窗口（有嵌入配置时显示）
int ShowLoginWindow(HINSTANCE hInstance);

} // namespace app

// ========== 检查自身 EXE 是否包含嵌入配置 ==========
static bool HasEmbeddedConfig()
{
	// 检查 EXE 尾部嵌入配置
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	HANDLE hFile = CreateFileA(exePath, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize <= CONFIG_MAX_SIZE) { CloseHandle(hFile); return false; }

	SetFilePointer(hFile, fileSize - CONFIG_MAX_SIZE, NULL, FILE_BEGIN);
	char buffer[CONFIG_MAX_SIZE + 1] = {0};
	DWORD bytesRead = 0;
	ReadFile(hFile, buffer, CONFIG_MAX_SIZE, &bytesRead, NULL);
	CloseHandle(hFile);

	// 用带长度的 std::string 构造，不依赖 null 终止——PE 零填充会导致
	// strstr 在第一个 0x00 处截断，永远找不到尾部配置标记
	std::string tail(buffer, bytesRead);
	return (tail.find(CONFIG_MARKER_BEGIN) != std::string::npos &&
	        tail.find(CONFIG_MARKER_END)   != std::string::npos);
}

// ========== WinMain 入口 ==========
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                   _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;

	// 初始化通用控件（ListBox 等需要）
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_STANDARD_CLASSES };
	InitCommonControlsEx(&icc);

	// 初始化 GDI+（用于加载皮肤图片）
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken = 0;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	int result = 0;
	if (HasEmbeddedConfig())
		result = app::ShowLoginWindow(hInstance);
	else
		result = app::ShowBuilderWindow(hInstance);

	// 清理 GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return result;
}
