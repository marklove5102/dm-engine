// BuilderWnd.h - 配置器窗口（替代 BuilderDlg）
#pragma once
#include "stdafx.h"
#include "resource.h"

namespace app {

// 显示配置器窗口（模态循环），返回退出码
int ShowBuilderWindow(HINSTANCE hInstance);

} // namespace app
