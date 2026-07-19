// 启用ComCtl32 v6 视觉样式(Windows XP+外观)
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <commctrl.h>
#include "MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/, int nCmdShow)
{
	// 初始化通用控件库
	INITCOMMONCONTROLSEX icc = {};
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES | ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES
		| ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&icc);

	MainWindow app(hInstance);
	if (!app.Init(nCmdShow))
		return 1;

	// 消息循环
	MSG msg;
	while (GetMessageA(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return static_cast<int>(msg.wParam);
}
