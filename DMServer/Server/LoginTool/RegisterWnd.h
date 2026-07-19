// RegisterWnd.h - 注册窗口（替代 CRegisterDlg）
#pragma once
#include "stdafx.h"

namespace app {

// 显示注册窗口（模态）；返回 true 表示注册成功
bool ShowRegisterWindow(HINSTANCE hInstance, HWND hParent,
                        const std::string& serverAddr, int serverPort,
                        const std::string& loginKey);

} // namespace app
