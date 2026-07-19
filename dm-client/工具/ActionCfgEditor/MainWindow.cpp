#include "MainWindow.h"
#include <commdlg.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <windowsx.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")

// ===================================================================
// 静态数据 —— 动作名称 & 部件名称映射表
// ===================================================================

const char* MainWindow::sm_ActionNames[] = {
	"ACTION_STAND", "ACTION_WALK", "ACTION_RUN", "ACTION_ATTACK1",
	"ACTION_ATTACKED", "ACTION_MAGIC", "ACTION_HSTAND", "ACTION_HWALK",
	"ACTION_HRUN", "ACTION_HDEATH", "ACTION_DEATH", "ACTION_CUT",
	"ACTION_APPEAR", "ACTION_HATTACKED", "ACTION_SALUTE", "ACTION_LSALUTE",
	"ACTION_LSTAND", "ACTION_LWALK", "ACTION_LRUN", "ACTION_LDEATH",
	"ACTION_LATTACKED", "ACTION_LFSTAND", "ACTION_LFWALK", "ACTION_LFRUN",
	"ACTION_LFDEATH", "ACTION_LFATTACKED", "ACTION_LFATTACK1", "ACTION_LFMAGIC",
	"ACTION_LFMAGICFGH", "ACTION_LFMAGICRAB", "ACTION_LFMAGICTAO",
	"ACTION_SWT", "ACTION_LSJ", "ACTION_JYL",
	"ACTION_LFSWT", "ACTION_LFLSJ", "ACTION_LFJYL"
};

const char* MainWindow::sm_DrawPartNames[] = {
	"WingShadow","WingShadowEff","WingLeft","WingLeftEff",
	"WingRight","WingRightEff","Weapon","WeaponEff","Shield","ShieldEff",
	"MonsterBody","MonsterArm","MonsterHead","MonsterHeadArm","MonsterLeft","MonsterRight",
	"Cloth","Pattern","Hair","ClothEff","FaBao","FaBaoEFF","MonsterLeftArm","MonsterRightArm"
};

// ---- 动作名称 <-> 编号 ----

const char* MainWindow::GetActionName(int action)
{
	if (action >= 0 && action < _countof(sm_ActionNames))
		return sm_ActionNames[action];

	if (action >= 40 && action < 60) // 40~59
	{
		static const char* extra[] = {
			"ACTION_STAND2", "ACTION_STAND3", "ACTION_ATTACK2", "ACTION_ATTACK3",
			"ACTION_ATTACK4", "ACTION_ATTACK5", "ACTION_ATTACK6", "ACTION_ATTACKED2", "ACTION_ATTACKED3",
			"ACTION_MAGIC2", "ACTION_MAGIC3", "ACTION_APPEAR2", "ACTION_APPEAR3", "ACTION_DEATH2",
			"ACTION_DEATH3", "ACTION_ADDACTION1", "ACTION_ADDACTION2", "ACTION_ADDACTION3",
			"ACTION_ADDACTION4", "ACTION_ADDACTION5"
		};
		int idx = action - 40;
		if (idx < _countof(extra))
			return extra[idx];
	}

	if (action >= 60 && action < 70)
	{
		static char buf[32];
		sprintf_s(buf, "ACTION_ADDACTION%d", action - 59);
		return buf; // 注意: 返回静态缓冲区, 调用者需立即使用
	}

	static char buf[32];
	sprintf_s(buf, "ACTION_%d", action);
	return buf;
}

int MainWindow::ParseActionName(const char* name)
{
	if (!name || !*name) return 0;

	// 先尝试匹配预定义名称
	for (int i = 0; i < _countof(sm_ActionNames); i++)
	{
		if (_stricmp(name, sm_ActionNames[i]) == 0)
			return i;
	}

	// 尝试解析纯数字
	int val = 0;
	if (sscanf_s(name, "%d", &val) == 1)
		return val;

	// 解析 ACTION_ 前缀数字
	if (strncmp(name, "ACTION_", 7) == 0)
	{
		if (sscanf_s(name + 7, "%d", &val) == 1)
			return val;
	}

	return 0;
}

const char* MainWindow::GetDrawPartName(int idx)
{
	if (idx >= 0 && idx < _countof(sm_DrawPartNames))
		return sm_DrawPartNames[idx];
	static char buf[16];
	sprintf_s(buf, "%d", idx);
	return buf;
}

// ===================================================================
// 构造 / 初始化
// ===================================================================

MainWindow::MainWindow(HINSTANCE hInst)
	: m_hInst(hInst)
{
}

bool MainWindow::Init(int nCmdShow)
{
	// 注册主窗口类
	WNDCLASSEXA wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInst;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = "ActionCfgEditorMain";
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassExA(&wc))
		return false;

	m_hwnd = CreateWindowExA(0, "ActionCfgEditorMain", "ActionCfg.dat Editor",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700,
		nullptr, nullptr, m_hInst, this);

	if (!m_hwnd)
		return false;

	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);
	return true;
}

// ===================================================================
// 主窗口过程
// ===================================================================

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	MainWindow* pThis = nullptr;

	if (msg == WM_NCCREATE)
	{
		auto* pCs = reinterpret_cast<CREATESTRUCT*>(lp);
		pThis = static_cast<MainWindow*>(pCs->lpCreateParams);
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		pThis->m_hwnd = hwnd;
	}
	else
	{
		pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
	}

	if (!pThis)
		return DefWindowProcA(hwnd, msg, wp, lp);

	switch (msg)
	{
	case WM_CREATE:
	{
		pThis->CreateMenuBar();
		pThis->CreateTabControl();
		pThis->CreateTabPages();
		pThis->CreateStatusBar();
		return 0;
	}

	case WM_SIZE:
		pThis->OnSize(LOWORD(lp), HIWORD(lp));
		return 0;

	case WM_NOTIFY:
	{
		auto* pNmh = reinterpret_cast<NMHDR*>(lp);

		// Tab切换
		if (pNmh->idFrom == IDC_TAB && pNmh->code == TCN_SELCHANGE)
		{
			pThis->OnTabSelChange();
			return 0;
		}

		// DrawOrder条目选择变化
		if (pNmh->idFrom == IDC_DO_LISTVIEW && pNmh->code == LVN_ITEMCHANGED)
		{
			auto* pNmlv = reinterpret_cast<NMLISTVIEW*>(lp);
			if ((pNmlv->uNewState & LVIS_SELECTED) && !(pNmlv->uOldState & LVIS_SELECTED))
				pThis->OnDoSelectionChanged();
			return 0;
		}
		break;
	}

	case WM_COMMAND:
	{
		WORD id = LOWORD(wp);
		WORD code = HIWORD(wp);

		switch (id)
		{
		case IDM_OPEN:   pThis->OnOpen();   return 0;
		case IDM_SAVE:   pThis->OnSave();   return 0;
		case IDM_SAVEAS: pThis->OnSaveAs(); return 0;
		case IDM_EXIT:   DestroyWindow(hwnd); return 0;

		// Monster按钮
		case IDC_MON_BTNADD:     pThis->OnAddMonster();       return 0;
		case IDC_MON_BTNDEL:     pThis->OnDelMonster();       return 0;
		case IDC_MON_BTNADDACT:  pThis->OnAddMonsterAction(); return 0;
		case IDC_MON_BTNDELACT:  pThis->OnDelMonsterAction(); return 0;

		// Wing按钮
		case IDC_WING_BTNADD:    pThis->OnAddWing();          return 0;
		case IDC_WING_BTNDEL:    pThis->OnDelWing();          return 0;
		case IDC_WING_BTNADDACT: pThis->OnAddWingAction();    return 0;
		case IDC_WING_BTNDELACT: pThis->OnDelWingAction();    return 0;

		// WingEffect按钮
		case IDC_WEFF_BTNADD:    pThis->OnAddWingEff();       return 0;
		case IDC_WEFF_BTNDEL:    pThis->OnDelWingEff();       return 0;
		case IDC_WEFF_BTNADDACT: pThis->OnAddWingEffAction(); return 0;
		case IDC_WEFF_BTNDELACT: pThis->OnDelWingEffAction(); return 0;

		// ListBox选择变化(Monster)
		case IDC_MON_LISTBOX:
			if (code == LBN_SELCHANGE) { pThis->OnMonsterSelChange(); return 0; }
			break;

		// ListBox选择变化(Wing)
		case IDC_WING_LISTBOX:
			if (code == LBN_SELCHANGE) { pThis->OnWingSelChange(); return 0; }
			break;

		// ListBox选择变化(WingEffect)
		case IDC_WEFF_LISTBOX:
			if (code == LBN_SELCHANGE) { pThis->OnWingEffSelChange(); return 0; }
			break;
		}
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcA(hwnd, msg, wp, lp);
}

// ===================================================================
// 菜单栏 / Tab / 状态栏 创建
// ===================================================================

void MainWindow::CreateMenuBar()
{
	HMENU hMenu = LoadMenuA(m_hInst, MAKEINTRESOURCEA(IDR_MAINMENU));
	SetMenu(m_hwnd, hMenu);
}

void MainWindow::CreateTabControl()
{
	m_hTab = CreateWindowA(WC_TABCONTROLA, nullptr,
		WS_CHILD | WS_VISIBLE | TCS_FIXEDWIDTH,
		0, 0, 100, 100, m_hwnd, (HMENU)IDC_TAB, m_hInst, nullptr);

	TCITEMA tci = {};
	tci.mask = TCIF_TEXT;

	const char* tabs[] = { "Draw Order", "Monsters", "Wings", "Wing Effects" };
	for (int i = 0; i < 4; i++)
	{
		tci.pszText = const_cast<char*>(tabs[i]);
		TabCtrl_InsertItem(m_hTab, i, &tci);
	}
}

void MainWindow::CreateStatusBar()
{
	m_hStatus = CreateWindowA(STATUSCLASSNAMEA, "Ready",
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		0, 0, 0, 0, m_hwnd, (HMENU)IDC_STATUS, m_hInst, nullptr);
}

void MainWindow::CreateTabPages()
{
	CreateDrawOrderPage();
	CreateMonsterPage();
	CreateWingPage();
	CreateWingEffPage();

	// 默认显示第一页
	if (m_rgTabPages[0])
	{
		ShowWindow(m_rgTabPages[0], SW_SHOW);
		ShowTabPage(0);
	}
	for (int i = 1; i < 4; i++)
	{
		if (m_rgTabPages[i])
			ShowWindow(m_rgTabPages[i], SW_HIDE);
	}
}

// ===================================================================
// DrawOrder 页
// ===================================================================

void MainWindow::CreateDrawOrderPage()
{
	// 容器窗口
	m_rgTabPages[0] = CreateWindowExA(0, "STATIC", nullptr,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 100, 100, m_hwnd, nullptr, m_hInst, nullptr);

	// 提示标签
	HWND hLabel = CreateWindowExA(0, "STATIC",
		"Draw Order: select entry above, edit per-direction frame order below. Each cell = DrawPart layer index (0~49).",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		0, 0, 100, 24, m_rgTabPages[0], nullptr, m_hInst, nullptr);

	// 上: 条目列表
	m_hDoList = CreateWindowExA(0, WC_LISTVIEWA, nullptr,
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
		0, 0, 100, 100, m_rgTabPages[0], (HMENU)IDC_DO_LISTVIEW, m_hInst, nullptr);
	ListView_SetExtendedListViewStyle(m_hDoList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 下: 详细数据列表(8方向×N帧×50列)
	m_hDoDetail = CreateWindowExA(0, WC_LISTVIEWA, nullptr,
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
		0, 0, 100, 100, m_rgTabPages[0], (HMENU)IDC_DO_DETAIL_LV, m_hInst, nullptr);
	ListView_SetExtendedListViewStyle(m_hDoDetail, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为两个ListView添加双击编辑子类化
	SetWindowSubclass(m_hDoList, LvSubclassProc, 10, reinterpret_cast<DWORD_PTR>(this));
	SetWindowSubclass(m_hDoDetail, LvSubclassProc, 11, reinterpret_cast<DWORD_PTR>(this));
}

// ===================================================================
// Monster 页
// ===================================================================

void MainWindow::CreateMonsterPage()
{
	m_rgTabPages[1] = CreateWindowExA(0, "STATIC", nullptr,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 100, 100, m_hwnd, nullptr, m_hInst, nullptr);

	m_hMonBtnAdd = CreateWindowExA(0, "BUTTON", "Add Monster",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[1], (HMENU)IDC_MON_BTNADD, m_hInst, nullptr);

	m_hMonBtnDel = CreateWindowExA(0, "BUTTON", "Del Monster",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[1], (HMENU)IDC_MON_BTNDEL, m_hInst, nullptr);

	m_hMonListBox = CreateWindowExA(0, "LISTBOX", nullptr,
		WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER,
		0, 0, 100, 100, m_rgTabPages[1], (HMENU)IDC_MON_LISTBOX, m_hInst, nullptr);

	m_hMonBtnAddAct = CreateWindowExA(0, "BUTTON", "Add Action",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[1], (HMENU)IDC_MON_BTNADDACT, m_hInst, nullptr);

	m_hMonBtnDelAct = CreateWindowExA(0, "BUTTON", "Del Action",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[1], (HMENU)IDC_MON_BTNDELACT, m_hInst, nullptr);

	m_hMonListView = CreateWindowExA(0, WC_LISTVIEWA, nullptr,
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
		0, 0, 100, 100, m_rgTabPages[1], (HMENU)IDC_MON_LISTVIEW, m_hInst, nullptr);
	ListView_SetExtendedListViewStyle(m_hMonListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SetWindowSubclass(m_hMonListView, LvSubclassProc, 20, reinterpret_cast<DWORD_PTR>(this));

	// 设置Monster ListView列
	const char* monHeaders[] = { "#", "Action", "StartIndex", "FrameMax", "FrameReal", "Speed" };
	const int monWidths[] = { 40, 160, 80, 80, 80, 80 };
	InitListViewColumns(m_hMonListView, monHeaders, monWidths, 6, true);
}

// ===================================================================
// Wing 页
// ===================================================================

void MainWindow::CreateWingPage()
{
	m_rgTabPages[2] = CreateWindowExA(0, "STATIC", nullptr,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 100, 100, m_hwnd, nullptr, m_hInst, nullptr);

	m_hWingBtnAdd = CreateWindowExA(0, "BUTTON", "Add Wing",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[2], (HMENU)IDC_WING_BTNADD, m_hInst, nullptr);

	m_hWingBtnDel = CreateWindowExA(0, "BUTTON", "Del Wing",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[2], (HMENU)IDC_WING_BTNDEL, m_hInst, nullptr);

	m_hWingListBox = CreateWindowExA(0, "LISTBOX", nullptr,
		WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER,
		0, 0, 100, 100, m_rgTabPages[2], (HMENU)IDC_WING_LISTBOX, m_hInst, nullptr);

	m_hWingBtnAddAct = CreateWindowExA(0, "BUTTON", "Add Action",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[2], (HMENU)IDC_WING_BTNADDACT, m_hInst, nullptr);

	m_hWingBtnDelAct = CreateWindowExA(0, "BUTTON", "Del Action",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[2], (HMENU)IDC_WING_BTNDELACT, m_hInst, nullptr);

	m_hWingListView = CreateWindowExA(0, WC_LISTVIEWA, nullptr,
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
		0, 0, 100, 100, m_rgTabPages[2], (HMENU)IDC_WING_LISTVIEW, m_hInst, nullptr);
	ListView_SetExtendedListViewStyle(m_hWingListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SetWindowSubclass(m_hWingListView, LvSubclassProc, 30, reinterpret_cast<DWORD_PTR>(this));

	const char* wingHeaders[] = { "#", "Action", "Package", "StartIndex", "FrameLeft", "FrameRight", "FrameShadow" };
	const int wingWidths[] = { 40, 160, 70, 80, 80, 80, 80 };
	InitListViewColumns(m_hWingListView, wingHeaders, wingWidths, 7, true);
}

// ===================================================================
// WingEffect 页
// ===================================================================

void MainWindow::CreateWingEffPage()
{
	m_rgTabPages[3] = CreateWindowExA(0, "STATIC", nullptr,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, 100, 100, m_hwnd, nullptr, m_hInst, nullptr);

	m_hWEffBtnAdd = CreateWindowExA(0, "BUTTON", "Add WingEff",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[3], (HMENU)IDC_WEFF_BTNADD, m_hInst, nullptr);

	m_hWEffBtnDel = CreateWindowExA(0, "BUTTON", "Del WingEff",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[3], (HMENU)IDC_WEFF_BTNDEL, m_hInst, nullptr);

	m_hWEffListBox = CreateWindowExA(0, "LISTBOX", nullptr,
		WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL | WS_BORDER,
		0, 0, 100, 100, m_rgTabPages[3], (HMENU)IDC_WEFF_LISTBOX, m_hInst, nullptr);

	m_hWEffBtnAddAct = CreateWindowExA(0, "BUTTON", "Add Action",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[3], (HMENU)IDC_WEFF_BTNADDACT, m_hInst, nullptr);

	m_hWEffBtnDelAct = CreateWindowExA(0, "BUTTON", "Del Action",
		WS_CHILD | WS_VISIBLE, 0, 0, 120, 28,
		m_rgTabPages[3], (HMENU)IDC_WEFF_BTNDELACT, m_hInst, nullptr);

	m_hWEffListView = CreateWindowExA(0, WC_LISTVIEWA, nullptr,
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
		0, 0, 100, 100, m_rgTabPages[3], (HMENU)IDC_WEFF_LISTVIEW, m_hInst, nullptr);
	ListView_SetExtendedListViewStyle(m_hWEffListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SetWindowSubclass(m_hWEffListView, LvSubclassProc, 40, reinterpret_cast<DWORD_PTR>(this));

	const char* weffHeaders[] = { "#", "Action", "Package", "StartIndex", "FrameLeft", "FrameRight", "FrameShadow" };
	const int weffWidths[] = { 40, 160, 70, 80, 80, 80, 80 };
	InitListViewColumns(m_hWEffListView, weffHeaders, weffWidths, 7, true);
}

// ===================================================================
// 布局
// ===================================================================

void MainWindow::OnSize(int width, int height)
{
	// 状态栏
	if (m_hStatus)
	{
		SendMessageA(m_hStatus, WM_SIZE, 0, 0);
		RECT rcStatus;
		GetWindowRect(m_hStatus, &rcStatus);
		height -= (rcStatus.bottom - rcStatus.top);
	}

	// Tab控件 — 占据顶部26px
	RECT rcTab = { 0, 0, width, 26 };
	if (m_hTab)
	{
		SetWindowPos(m_hTab, nullptr, 0, 0, width, 26, SWP_NOZORDER);
	}

	// Tab页面区域 = 从Tab下方到底部
	const int tabAreaTop = 26;
	const int tabAreaHeight = height - tabAreaTop;

	for (int i = 0; i < 4; i++)
	{
		if (m_rgTabPages[i])
			SetWindowPos(m_rgTabPages[i], nullptr, 0, tabAreaTop, width, tabAreaHeight, SWP_NOZORDER);
	}

	// ---- DrawOrder页布局 ----
	if (m_rgTabPages[0] && IsWindowVisible(m_rgTabPages[0]))
	{
		// 用EnumChildWindows遍历子控件布局更简单，这里手动布局
		RECT rc;
		GetClientRect(m_rgTabPages[0], &rc);
		int cw = rc.right;
		int ch = rc.bottom;

		// 标签在顶部24px
		HWND hChild = GetWindow(m_rgTabPages[0], GW_CHILD);
		while (hChild)
		{
			char cls[32];
			GetClassNameA(hChild, cls, 32);
			int cid = GetDlgCtrlID(hChild);
			if (strcmp(cls, "Static") == 0 && cid == 0)
			{
				// 顶部提示标签
				SetWindowPos(hChild, nullptr, 4, 2, cw - 8, 24, SWP_NOZORDER);
			}
			else if (cid == IDC_DO_LISTVIEW)
			{
				// 上半部分
				int upperH = (ch - 30) * 45 / 100;
				SetWindowPos(hChild, nullptr, 4, 30, cw - 8, upperH, SWP_NOZORDER);
			}
			else if (cid == IDC_DO_DETAIL_LV)
			{
				// 下半部分
				int upperH = (ch - 30) * 45 / 100;
				SetWindowPos(hChild, nullptr, 4, 30 + upperH + 2, cw - 8, ch - 32 - upperH, SWP_NOZORDER);
			}
			hChild = GetWindow(hChild, GW_HWNDNEXT);
		}
	}

	// ---- Monster页布局 ----
	if (m_rgTabPages[1] && IsWindowVisible(m_rgTabPages[1]))
	{
		RECT rc;
		GetClientRect(m_rgTabPages[1], &rc);
		// 左侧面板: 220px宽
		SetWindowPos(m_hMonBtnAdd, nullptr, 2, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hMonBtnDel, nullptr, 104, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hMonListBox, nullptr, 2, 32, 210, rc.bottom - 34, SWP_NOZORDER);

		// 右侧面板
		int rx = 216;
		int rw = rc.right - rx - 2;
		SetWindowPos(m_hMonBtnAddAct, nullptr, rx, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hMonBtnDelAct, nullptr, rx + 104, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hMonListView, nullptr, rx, 32, rw, rc.bottom - 34, SWP_NOZORDER);
	}

	// ---- Wing页布局 ----
	if (m_rgTabPages[2] && IsWindowVisible(m_rgTabPages[2]))
	{
		RECT rc;
		GetClientRect(m_rgTabPages[2], &rc);
		SetWindowPos(m_hWingBtnAdd, nullptr, 2, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWingBtnDel, nullptr, 104, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWingListBox, nullptr, 2, 32, 210, rc.bottom - 34, SWP_NOZORDER);

		int rx = 216;
		int rw = rc.right - rx - 2;
		SetWindowPos(m_hWingBtnAddAct, nullptr, rx, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWingBtnDelAct, nullptr, rx + 104, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWingListView, nullptr, rx, 32, rw, rc.bottom - 34, SWP_NOZORDER);
	}

	// ---- WingEffect页布局 ----
	if (m_rgTabPages[3] && IsWindowVisible(m_rgTabPages[3]))
	{
		RECT rc;
		GetClientRect(m_rgTabPages[3], &rc);
		SetWindowPos(m_hWEffBtnAdd, nullptr, 2, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWEffBtnDel, nullptr, 104, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWEffListBox, nullptr, 2, 32, 210, rc.bottom - 34, SWP_NOZORDER);

		int rx = 216;
		int rw = rc.right - rx - 2;
		SetWindowPos(m_hWEffBtnAddAct, nullptr, rx, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWEffBtnDelAct, nullptr, rx + 104, 2, 100, 26, SWP_NOZORDER);
		SetWindowPos(m_hWEffListView, nullptr, rx, 32, rw, rc.bottom - 34, SWP_NOZORDER);
	}
}

// ===================================================================
// Tab切换
// ===================================================================

void MainWindow::OnTabSelChange()
{
	int idx = TabCtrl_GetCurSel(m_hTab);
	ShowTabPage(idx);
}

void MainWindow::ShowTabPage(int index)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_rgTabPages[i])
			ShowWindow(m_rgTabPages[i], (i == index) ? SW_SHOW : SW_HIDE);
	}

	// 触发一次OnSize重新布局
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	OnSize(rc.right, rc.bottom);
}

// ===================================================================
// 文件操作
// ===================================================================

void MainWindow::OnOpen()
{
	char szFile[MAX_PATH] = "ActionCfg.dat";

	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hwnd;
	ofn.lpstrFilter = "ActionCfg.dat\0ActionCfg.dat\0DAT Files\0*.dat\0All Files\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if (!GetOpenFileNameA(&ofn))
		return;

	try
	{
		m_data.Load(szFile);
		m_strCurrentFile = szFile;

		char title[512];
		sprintf_s(title, "ActionCfg.dat Editor - %s", szFile);
		SetWindowTextA(m_hwnd, title);

		RefreshAll();

		char status[512];
		sprintf_s(status, "Loaded: %zu DrawOrder, %zu Monsters, %zu Wings, %zu WingEffects",
			m_data.DrawOrders.size(), m_data.Monsters.size(),
			m_data.Wings.size(), m_data.WingEffects.size());
		SetWindowTextA(m_hStatus, status);
	}
	catch (const std::exception& ex)
	{
		MessageBoxA(m_hwnd, ex.what(), "Error", MB_OK | MB_ICONERROR);
	}
}

void MainWindow::OnSave()
{
	if (m_strCurrentFile.empty())
	{
		MessageBoxA(m_hwnd, "No file loaded.", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}

	try
	{
		ApplyAllChanges();
		m_data.Save(m_strCurrentFile);

		char status[512];
		sprintf_s(status, "Saved: %s", m_strCurrentFile.c_str());
		SetWindowTextA(m_hStatus, status);
		MessageBoxA(m_hwnd, "File saved successfully!", "Success", MB_OK | MB_ICONINFORMATION);
	}
	catch (const std::exception& ex)
	{
		MessageBoxA(m_hwnd, ex.what(), "Error", MB_OK | MB_ICONERROR);
	}
}

void MainWindow::OnSaveAs()
{
	char szFile[MAX_PATH] = "ActionCfg.dat";

	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hwnd;
	ofn.lpstrFilter = "DAT Files\0*.dat\0All Files\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	if (!GetSaveFileNameA(&ofn))
		return;

	try
	{
		ApplyAllChanges();
		m_data.Save(szFile);
		m_strCurrentFile = szFile;

		char title[512];
		sprintf_s(title, "ActionCfg.dat Editor - %s", szFile);
		SetWindowTextA(m_hwnd, title);

		char status[512];
		sprintf_s(status, "Saved: %s", szFile);
		SetWindowTextA(m_hStatus, status);
		MessageBoxA(m_hwnd, "File saved successfully!", "Success", MB_OK | MB_ICONINFORMATION);
	}
	catch (const std::exception& ex)
	{
		MessageBoxA(m_hwnd, ex.what(), "Error", MB_OK | MB_ICONERROR);
	}
}

void MainWindow::RefreshAll()
{
	RefreshDrawOrder();
	RefreshMonsterList();
	if (m_data.Monsters.empty())
		ListBox_ResetContent(m_hMonListBox);
	else if (m_nSelectedMonster < 0)
	{
		m_nSelectedMonster = 0;
		ListBox_SetCurSel(m_hMonListBox, 0);
		RefreshMonsterDetail(m_data.Monsters[0]);
	}

	RefreshWingList();
	if (m_data.Wings.empty())
		ListBox_ResetContent(m_hWingListBox);
	else if (m_nSelectedWing < 0)
	{
		m_nSelectedWing = 0;
		ListBox_SetCurSel(m_hWingListBox, 0);
		RefreshWingDetail(m_data.Wings[0]);
	}

	RefreshWingEffList();
	if (m_data.WingEffects.empty())
		ListBox_ResetContent(m_hWEffListBox);
	else if (m_nSelectedWEff < 0)
	{
		m_nSelectedWEff = 0;
		ListBox_SetCurSel(m_hWEffListBox, 0);
		RefreshWingEffDetail(m_data.WingEffects[0]);
	}
}

// ===================================================================
// DrawOrder 刷新
// ===================================================================

void MainWindow::RefreshDrawOrder()
{
	// 先结束编辑
	EndLvEdit(true);

	ListView_DeleteAllItems(m_hDoList);

	// 删除旧列
	while (ListView_DeleteColumn(m_hDoList, 0)) {}

	// 添加列
	LVCOLUMNA lvc = {};
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvc.fmt = LVCFMT_LEFT;

	lvc.pszText = const_cast<char*>("#");
	lvc.cx = 40;
	ListView_InsertColumn(m_hDoList, 0, &lvc);

	lvc.pszText = const_cast<char*>("Action");
	lvc.cx = 180;
	ListView_InsertColumn(m_hDoList, 1, &lvc);

	lvc.pszText = const_cast<char*>("Frames");
	lvc.cx = 60;
	ListView_InsertColumn(m_hDoList, 2, &lvc);

	// 填充数据
	for (size_t i = 0; i < m_data.DrawOrders.size(); i++)
	{
		auto& entry = m_data.DrawOrders[i];
		LVITEMA lvi = {};
		lvi.mask = LVIF_TEXT;
		lvi.iItem = static_cast<int>(i);

		char buf[32];
		sprintf_s(buf, "%zu", i);
		lvi.pszText = buf;
		int idx = ListView_InsertItem(m_hDoList, &lvi);

		SetListViewItem(m_hDoList, idx, 1, GetActionName(entry.Action));

		sprintf_s(buf, "%d", entry.Frame);
		SetListViewItem(m_hDoList, idx, 2, buf);
	}

	// 触发选择变化
	if (ListView_GetItemCount(m_hDoList) > 0)
	{
		ListView_SetItemState(m_hDoList, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		OnDoSelectionChanged();
	}
}

void MainWindow::OnDoSelectionChanged()
{
	int sel = ListView_GetNextItem(m_hDoList, -1, LVNI_SELECTED);
	if (sel < 0 || sel >= static_cast<int>(m_data.DrawOrders.size()))
	{
		ListView_DeleteAllItems(m_hDoDetail);
		return;
	}

	RefreshDrawOrderDetail(m_data.DrawOrders[sel]);
}

void MainWindow::RefreshDrawOrderDetail(SDrawOrderEntry& entry)
{
	EndLvEdit(true);
	ListView_DeleteAllItems(m_hDoDetail);

	// 删除旧列
	while (ListView_DeleteColumn(m_hDoDetail, 0)) {}

	// 第0列: Dir*Frame
	LVCOLUMNA lvc = {};
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvc.fmt = LVCFMT_CENTER;
	lvc.pszText = const_cast<char*>("Dir*Frame");
	lvc.cx = 80;
	ListView_InsertColumn(m_hDoDetail, 0, &lvc);

	// 50列部件
	for (int p = 0; p < DP_DrawPartMaxNum; p++)
	{
		lvc.pszText = const_cast<char*>(GetDrawPartName(p));
		lvc.cx = 30;
		ListView_InsertColumn(m_hDoDetail, p + 1, &lvc);
	}

	if (entry.Frame <= 0 || entry.OrderData.empty())
		return;

	const char* dirNames[] = { "右下", "右", "右上", "上", "左上", "左", "左下", "下" };

	for (int d = 0; d < 8; d++)
	{
		for (int f = 0; f < entry.Frame; f++)
		{
			LVITEMA lvi = {};
			lvi.mask = LVIF_TEXT;
			lvi.iItem = d * entry.Frame + f;

			char buf[64];
			sprintf_s(buf, "%s[%d]", dirNames[d], f);
			lvi.pszText = buf;
			ListView_InsertItem(m_hDoDetail, &lvi);

			for (int p = 0; p < DP_DrawPartMaxNum; p++)
			{
				sprintf_s(buf, "%u", static_cast<unsigned>(entry.Get(d, f, p)));
				SetListViewItem(m_hDoDetail, lvi.iItem, p + 1, buf);
			}
		}
	}
}

// ===================================================================
// Monster
// ===================================================================

void MainWindow::RefreshMonsterList()
{
	ListBox_ResetContent(m_hMonListBox);
	for (size_t i = 0; i < m_data.Monsters.size(); i++)
	{
		char buf[128];
		sprintf_s(buf, "Monster %zu (%zu actions)", i, m_data.Monsters[i].Actions.size());
		ListBox_AddString(m_hMonListBox, buf);
	}
	if (ListBox_GetCount(m_hMonListBox) > 0)
		ListBox_SetCurSel(m_hMonListBox, 0);
}

void MainWindow::OnMonsterSelChange()
{
	int sel = ListBox_GetCurSel(m_hMonListBox);
	if (sel < 0 || sel >= static_cast<int>(m_data.Monsters.size()))
		return;
	m_nSelectedMonster = sel;
	RefreshMonsterDetail(m_data.Monsters[sel]);
}

void MainWindow::RefreshMonsterDetail(MonsterEntry& monster)
{
	EndLvEdit(true);
	ListView_DeleteAllItems(m_hMonListView);

	// 确保列存在(已在CreateMonsterPage中设置)
	for (size_t i = 0; i < monster.Actions.size(); i++)
	{
		auto& act = monster.Actions[i];
		char buf[32];
		sprintf_s(buf, "%zu", i);
		LVITEMA lvi = {};
		lvi.mask = LVIF_TEXT;
		lvi.iItem = static_cast<int>(i);
		lvi.pszText = buf;
		ListView_InsertItem(m_hMonListView, &lvi);

		SetListViewItem(m_hMonListView, lvi.iItem, 1, GetActionName(act.Action));
		sprintf_s(buf, "%d", act.StartIndex);  SetListViewItem(m_hMonListView, lvi.iItem, 2, buf);
		sprintf_s(buf, "%d", act.Mmx);         SetListViewItem(m_hMonListView, lvi.iItem, 3, buf);
		sprintf_s(buf, "%d", act.Realx);       SetListViewItem(m_hMonListView, lvi.iItem, 4, buf);
		sprintf_s(buf, "%d", act.Speedx);      SetListViewItem(m_hMonListView, lvi.iItem, 5, buf);
	}
}

void MainWindow::OnAddMonster()
{
	m_data.Monsters.emplace_back();
	RefreshMonsterList();
	ListBox_SetCurSel(m_hMonListBox, ListBox_GetCount(m_hMonListBox) - 1);
	OnMonsterSelChange();
}

void MainWindow::OnDelMonster()
{
	int sel = ListBox_GetCurSel(m_hMonListBox);
	if (sel < 0 || sel >= static_cast<int>(m_data.Monsters.size()))
		return;
	m_data.Monsters.erase(m_data.Monsters.begin() + sel);
	RefreshMonsterList();
	if (sel > 0)
		ListBox_SetCurSel(m_hMonListBox, sel - 1);
	OnMonsterSelChange();
}

void MainWindow::OnAddMonsterAction()
{
	if (m_nSelectedMonster < 0 || m_nSelectedMonster >= static_cast<int>(m_data.Monsters.size()))
		return;
	m_data.Monsters[m_nSelectedMonster].Actions.emplace_back();
	RefreshMonsterDetail(m_data.Monsters[m_nSelectedMonster]);

	// 更新ListBox文本
	char buf[128];
	sprintf_s(buf, "Monster %d (%zu actions)", m_nSelectedMonster,
		m_data.Monsters[m_nSelectedMonster].Actions.size());
	ListBox_DeleteString(m_hMonListBox, m_nSelectedMonster);
	ListBox_InsertString(m_hMonListBox, m_nSelectedMonster, buf);
	ListBox_SetCurSel(m_hMonListBox, m_nSelectedMonster);
}

void MainWindow::OnDelMonsterAction()
{
	if (m_nSelectedMonster < 0 || m_nSelectedMonster >= static_cast<int>(m_data.Monsters.size()))
		return;
	int sel = ListView_GetNextItem(m_hMonListView, -1, LVNI_SELECTED);
	if (sel < 0 || sel >= static_cast<int>(m_data.Monsters[m_nSelectedMonster].Actions.size()))
		return;
	m_data.Monsters[m_nSelectedMonster].Actions.erase(
		m_data.Monsters[m_nSelectedMonster].Actions.begin() + sel);
	RefreshMonsterDetail(m_data.Monsters[m_nSelectedMonster]);

	char buf[128];
	sprintf_s(buf, "Monster %d (%zu actions)", m_nSelectedMonster,
		m_data.Monsters[m_nSelectedMonster].Actions.size());
	ListBox_DeleteString(m_hMonListBox, m_nSelectedMonster);
	ListBox_InsertString(m_hMonListBox, m_nSelectedMonster, buf);
	ListBox_SetCurSel(m_hMonListBox, m_nSelectedMonster);
}

// ===================================================================
// Wing
// ===================================================================

void MainWindow::RefreshWingList()
{
	ListBox_ResetContent(m_hWingListBox);
	for (size_t i = 0; i < m_data.Wings.size(); i++)
	{
		char buf[128];
		sprintf_s(buf, "Wing %zu (%zu actions)", i, m_data.Wings[i].Actions.size());
		ListBox_AddString(m_hWingListBox, buf);
	}
	if (ListBox_GetCount(m_hWingListBox) > 0)
		ListBox_SetCurSel(m_hWingListBox, 0);
}

void MainWindow::OnWingSelChange()
{
	int sel = ListBox_GetCurSel(m_hWingListBox);
	if (sel < 0 || sel >= static_cast<int>(m_data.Wings.size()))
		return;
	m_nSelectedWing = sel;
	RefreshWingDetail(m_data.Wings[sel]);
}

void MainWindow::RefreshWingDetail(WingEntry& wing)
{
	EndLvEdit(true);
	ListView_DeleteAllItems(m_hWingListView);

	for (size_t i = 0; i < wing.Actions.size(); i++)
	{
		auto& act = wing.Actions[i];
		char buf[32];
		sprintf_s(buf, "%zu", i);
		LVITEMA lvi = {};
		lvi.mask = LVIF_TEXT;
		lvi.iItem = static_cast<int>(i);
		lvi.pszText = buf;
		ListView_InsertItem(m_hWingListView, &lvi);

		SetListViewItem(m_hWingListView, lvi.iItem, 1, GetActionName(act.Action));
		sprintf_s(buf, "%d", act.Package);    SetListViewItem(m_hWingListView, lvi.iItem, 2, buf);
		sprintf_s(buf, "%d", act.StartIndex); SetListViewItem(m_hWingListView, lvi.iItem, 3, buf);
		sprintf_s(buf, "%d", act.FrameLeft);  SetListViewItem(m_hWingListView, lvi.iItem, 4, buf);
		sprintf_s(buf, "%d", act.FrameRight); SetListViewItem(m_hWingListView, lvi.iItem, 5, buf);
		sprintf_s(buf, "%d", act.FrameShadow);SetListViewItem(m_hWingListView, lvi.iItem, 6, buf);
	}
}

void MainWindow::OnAddWing()
{
	m_data.Wings.emplace_back();
	RefreshWingList();
	ListBox_SetCurSel(m_hWingListBox, ListBox_GetCount(m_hWingListBox) - 1);
	OnWingSelChange();
}

void MainWindow::OnDelWing()
{
	int sel = ListBox_GetCurSel(m_hWingListBox);
	if (sel < 0 || sel >= static_cast<int>(m_data.Wings.size()))
		return;
	m_data.Wings.erase(m_data.Wings.begin() + sel);
	RefreshWingList();
	if (sel > 0)
		ListBox_SetCurSel(m_hWingListBox, sel - 1);
	OnWingSelChange();
}

void MainWindow::OnAddWingAction()
{
	if (m_nSelectedWing < 0 || m_nSelectedWing >= static_cast<int>(m_data.Wings.size()))
		return;
	m_data.Wings[m_nSelectedWing].Actions.emplace_back();
	RefreshWingDetail(m_data.Wings[m_nSelectedWing]);

	char buf[128];
	sprintf_s(buf, "Wing %d (%zu actions)", m_nSelectedWing,
		m_data.Wings[m_nSelectedWing].Actions.size());
	ListBox_DeleteString(m_hWingListBox, m_nSelectedWing);
	ListBox_InsertString(m_hWingListBox, m_nSelectedWing, buf);
	ListBox_SetCurSel(m_hWingListBox, m_nSelectedWing);
}

void MainWindow::OnDelWingAction()
{
	if (m_nSelectedWing < 0 || m_nSelectedWing >= static_cast<int>(m_data.Wings.size()))
		return;
	int sel = ListView_GetNextItem(m_hWingListView, -1, LVNI_SELECTED);
	if (sel < 0 || sel >= static_cast<int>(m_data.Wings[m_nSelectedWing].Actions.size()))
		return;
	m_data.Wings[m_nSelectedWing].Actions.erase(
		m_data.Wings[m_nSelectedWing].Actions.begin() + sel);
	RefreshWingDetail(m_data.Wings[m_nSelectedWing]);

	char buf[128];
	sprintf_s(buf, "Wing %d (%zu actions)", m_nSelectedWing,
		m_data.Wings[m_nSelectedWing].Actions.size());
	ListBox_DeleteString(m_hWingListBox, m_nSelectedWing);
	ListBox_InsertString(m_hWingListBox, m_nSelectedWing, buf);
	ListBox_SetCurSel(m_hWingListBox, m_nSelectedWing);
}

// ===================================================================
// WingEffect
// ===================================================================

void MainWindow::RefreshWingEffList()
{
	ListBox_ResetContent(m_hWEffListBox);
	for (size_t i = 0; i < m_data.WingEffects.size(); i++)
	{
		char buf[128];
		sprintf_s(buf, "WingEffect %zu (%zu actions)", i,
			m_data.WingEffects[i].Actions.size());
		ListBox_AddString(m_hWEffListBox, buf);
	}
	if (ListBox_GetCount(m_hWEffListBox) > 0)
		ListBox_SetCurSel(m_hWEffListBox, 0);
}

void MainWindow::OnWingEffSelChange()
{
	int sel = ListBox_GetCurSel(m_hWEffListBox);
	if (sel < 0 || sel >= static_cast<int>(m_data.WingEffects.size()))
		return;
	m_nSelectedWEff = sel;
	RefreshWingEffDetail(m_data.WingEffects[sel]);
}

void MainWindow::RefreshWingEffDetail(WingEntry& wing)
{
	EndLvEdit(true);
	ListView_DeleteAllItems(m_hWEffListView);

	for (size_t i = 0; i < wing.Actions.size(); i++)
	{
		auto& act = wing.Actions[i];
		char buf[32];
		sprintf_s(buf, "%zu", i);
		LVITEMA lvi = {};
		lvi.mask = LVIF_TEXT;
		lvi.iItem = static_cast<int>(i);
		lvi.pszText = buf;
		ListView_InsertItem(m_hWEffListView, &lvi);

		SetListViewItem(m_hWEffListView, lvi.iItem, 1, GetActionName(act.Action));
		sprintf_s(buf, "%d", act.Package);    SetListViewItem(m_hWEffListView, lvi.iItem, 2, buf);
		sprintf_s(buf, "%d", act.StartIndex); SetListViewItem(m_hWEffListView, lvi.iItem, 3, buf);
		sprintf_s(buf, "%d", act.FrameLeft);  SetListViewItem(m_hWEffListView, lvi.iItem, 4, buf);
		sprintf_s(buf, "%d", act.FrameRight); SetListViewItem(m_hWEffListView, lvi.iItem, 5, buf);
		sprintf_s(buf, "%d", act.FrameShadow);SetListViewItem(m_hWEffListView, lvi.iItem, 6, buf);
	}
}

void MainWindow::OnAddWingEff()
{
	m_data.WingEffects.emplace_back();
	RefreshWingEffList();
	ListBox_SetCurSel(m_hWEffListBox, ListBox_GetCount(m_hWEffListBox) - 1);
	OnWingEffSelChange();
}

void MainWindow::OnDelWingEff()
{
	int sel = ListBox_GetCurSel(m_hWEffListBox);
	if (sel < 0 || sel >= static_cast<int>(m_data.WingEffects.size()))
		return;
	m_data.WingEffects.erase(m_data.WingEffects.begin() + sel);
	RefreshWingEffList();
	if (sel > 0)
		ListBox_SetCurSel(m_hWEffListBox, sel - 1);
	OnWingEffSelChange();
}

void MainWindow::OnAddWingEffAction()
{
	if (m_nSelectedWEff < 0 || m_nSelectedWEff >= static_cast<int>(m_data.WingEffects.size()))
		return;
	m_data.WingEffects[m_nSelectedWEff].Actions.emplace_back();
	RefreshWingEffDetail(m_data.WingEffects[m_nSelectedWEff]);

	char buf[128];
	sprintf_s(buf, "WingEffect %d (%zu actions)", m_nSelectedWEff,
		m_data.WingEffects[m_nSelectedWEff].Actions.size());
	ListBox_DeleteString(m_hWEffListBox, m_nSelectedWEff);
	ListBox_InsertString(m_hWEffListBox, m_nSelectedWEff, buf);
	ListBox_SetCurSel(m_hWEffListBox, m_nSelectedWEff);
}

void MainWindow::OnDelWingEffAction()
{
	if (m_nSelectedWEff < 0 || m_nSelectedWEff >= static_cast<int>(m_data.WingEffects.size()))
		return;
	int sel = ListView_GetNextItem(m_hWEffListView, -1, LVNI_SELECTED);
	if (sel < 0 || sel >= static_cast<int>(m_data.WingEffects[m_nSelectedWEff].Actions.size()))
		return;
	m_data.WingEffects[m_nSelectedWEff].Actions.erase(
		m_data.WingEffects[m_nSelectedWEff].Actions.begin() + sel);
	RefreshWingEffDetail(m_data.WingEffects[m_nSelectedWEff]);

	char buf[128];
	sprintf_s(buf, "WingEffect %d (%zu actions)", m_nSelectedWEff,
		m_data.WingEffects[m_nSelectedWEff].Actions.size());
	ListBox_DeleteString(m_hWEffListBox, m_nSelectedWEff);
	ListBox_InsertString(m_hWEffListBox, m_nSelectedWEff, buf);
	ListBox_SetCurSel(m_hWEffListBox, m_nSelectedWEff);
}

// ===================================================================
// 数据提交 —— 从ListView控件回写到数据模型
// ===================================================================

void MainWindow::ApplyAllChanges()
{
	// 任何正在编辑的单元格先提交
	EndLvEdit(true);

	// ---- DrawOrder 条目 ----
	int count = ListView_GetItemCount(m_hDoList);
	if (count > 0)
	{
		// 警告: 条目数量与数据模型不匹配时不安全同步
		// 仅同步列数据(允许用户在Grid中修改Action/Frames)
		// DrawOrder 条目的添加/删除通过直接操作数据模型完成
		for (int i = 0; i < count && i < static_cast<int>(m_data.DrawOrders.size()); i++)
		{
			auto& entry = m_data.DrawOrders[i];
			std::string s = GetListViewItem(m_hDoList, i, 1);
			entry.Action = ParseActionName(s.c_str());

			s = GetListViewItem(m_hDoList, i, 2);
			int newFrame = atoi(s.c_str());
			if (newFrame != entry.Frame)
			{
				entry.Frame = newFrame;
				entry.ResizeData();
			}
		}

		// ---- DrawOrder 详细数据 ----
		// 找到当前选中的条目
		int sel = ListView_GetNextItem(m_hDoList, -1, LVNI_SELECTED);
		if (sel >= 0 && sel < static_cast<int>(m_data.DrawOrders.size()))
		{
			auto& entry = m_data.DrawOrders[sel];
			int detailCount = ListView_GetItemCount(m_hDoDetail);
			int expectedRows = 8 * entry.Frame;
			for (int r = 0; r < detailCount && r < expectedRows; r++)
			{
				int d = r / entry.Frame;
				int f = r % entry.Frame;
				for (int p = 0; p < DP_DrawPartMaxNum; p++)
				{
					std::string s = GetListViewItem(m_hDoDetail, r, p + 1);
					uint8_t val = static_cast<uint8_t>(atoi(s.c_str()));
					entry.Set(d, f, p, val);
				}
			}
		}
	}

	// ---- Monster ----
	if (m_nSelectedMonster >= 0 && m_nSelectedMonster < static_cast<int>(m_data.Monsters.size()))
	{
		auto& monster = m_data.Monsters[m_nSelectedMonster];
		int actCount = ListView_GetItemCount(m_hMonListView);
		for (int i = 0; i < actCount && i < static_cast<int>(monster.Actions.size()); i++)
		{
			auto& act = monster.Actions[i];
			std::string s = GetListViewItem(m_hMonListView, i, 1);
			act.Action = ParseActionName(s.c_str());
			act.StartIndex = atoi(GetListViewItem(m_hMonListView, i, 2).c_str());
			act.Mmx = atoi(GetListViewItem(m_hMonListView, i, 3).c_str());
			act.Realx = atoi(GetListViewItem(m_hMonListView, i, 4).c_str());
			act.Speedx = atoi(GetListViewItem(m_hMonListView, i, 5).c_str());
		}
	}

	// ---- Wing ----
	if (m_nSelectedWing >= 0 && m_nSelectedWing < static_cast<int>(m_data.Wings.size()))
	{
		auto& wing = m_data.Wings[m_nSelectedWing];
		int actCount = ListView_GetItemCount(m_hWingListView);
		for (int i = 0; i < actCount && i < static_cast<int>(wing.Actions.size()); i++)
		{
			auto& act = wing.Actions[i];
			std::string s = GetListViewItem(m_hWingListView, i, 1);
			act.Action = ParseActionName(s.c_str());
			act.Package = atoi(GetListViewItem(m_hWingListView, i, 2).c_str());
			act.StartIndex = atoi(GetListViewItem(m_hWingListView, i, 3).c_str());
			act.FrameLeft = atoi(GetListViewItem(m_hWingListView, i, 4).c_str());
			act.FrameRight = atoi(GetListViewItem(m_hWingListView, i, 5).c_str());
			act.FrameShadow = atoi(GetListViewItem(m_hWingListView, i, 6).c_str());
		}
	}

	// ---- WingEffect ----
	if (m_nSelectedWEff >= 0 && m_nSelectedWEff < static_cast<int>(m_data.WingEffects.size()))
	{
		auto& wing = m_data.WingEffects[m_nSelectedWEff];
		int actCount = ListView_GetItemCount(m_hWEffListView);
		for (int i = 0; i < actCount && i < static_cast<int>(wing.Actions.size()); i++)
		{
			auto& act = wing.Actions[i];
			std::string s = GetListViewItem(m_hWEffListView, i, 1);
			act.Action = ParseActionName(s.c_str());
			act.Package = atoi(GetListViewItem(m_hWEffListView, i, 2).c_str());
			act.StartIndex = atoi(GetListViewItem(m_hWEffListView, i, 3).c_str());
			act.FrameLeft = atoi(GetListViewItem(m_hWEffListView, i, 4).c_str());
			act.FrameRight = atoi(GetListViewItem(m_hWEffListView, i, 5).c_str());
			act.FrameShadow = atoi(GetListViewItem(m_hWEffListView, i, 6).c_str());
		}
	}
}

// ===================================================================
// ListView 辅助函数
// ===================================================================

void MainWindow::InitListViewColumns(HWND hLv, const char* const* headers,
	const int* widths, int count, bool /*firstEditable*/)
{
	LVCOLUMNA lvc = {};
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvc.fmt = LVCFMT_LEFT;
	for (int i = 0; i < count; i++)
	{
		lvc.pszText = const_cast<char*>(headers[i]);
		lvc.cx = widths[i];
		ListView_InsertColumn(hLv, i, &lvc);
	}
}

void MainWindow::SetListViewItem(HWND hLv, int row, int col, const char* text)
{
	LVITEMA lvi = {};
	lvi.iItem = row;
	lvi.iSubItem = col;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = const_cast<char*>(text);
	if (col == 0)
		ListView_SetItem(hLv, &lvi);
	else
		ListView_SetItem(hLv, &lvi);
}

std::string MainWindow::GetListViewItem(HWND hLv, int row, int col)
{
	char buf[256] = {};
	LVITEMA lvi = {};
	lvi.iItem = row;
	lvi.iSubItem = col;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = buf;
	lvi.cchTextMax = sizeof(buf);
	ListView_GetItem(hLv, &lvi);
	return std::string(buf);
}

// ===================================================================
// ListView 双击编辑 —— 子类化
// ===================================================================

LRESULT CALLBACK MainWindow::LvSubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
	UINT_PTR /*subclassId*/, DWORD_PTR refData)
{
	auto* pThis = reinterpret_cast<MainWindow*>(refData);

	switch (msg)
	{
	case WM_NCDESTROY:
		RemoveWindowSubclass(hwnd, LvSubclassProc, 0);
		break;

	case WM_NOTIFY:
	{
		auto* pNmh = reinterpret_cast<NMHDR*>(lp);
		if (pNmh->code == NM_DBLCLK)
		{
			auto* pNmItem = reinterpret_cast<NMITEMACTIVATE*>(lp);
			if (pNmItem->iItem >= 0 && pNmItem->iSubItem >= 1)
			{
				pThis->BeginLvEdit(hwnd, pNmItem->iItem, pNmItem->iSubItem);
			}
			return 0;
		}
		break;
	}
	}

	return DefSubclassProc(hwnd, msg, wp, lp);
}

LRESULT CALLBACK MainWindow::EditSubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
	UINT_PTR /*subclassId*/, DWORD_PTR refData)
{
	auto* pThis = reinterpret_cast<MainWindow*>(refData);

	switch (msg)
	{
	case WM_KEYDOWN:
		if (wp == VK_RETURN)
		{
			pThis->EndLvEdit(true);
			return 0;
		}
		if (wp == VK_ESCAPE)
		{
			pThis->EndLvEdit(false);
			return 0;
		}
		break;

	case WM_KILLFOCUS:
		pThis->EndLvEdit(true);
		return 0;

	case WM_NCDESTROY:
		RemoveWindowSubclass(hwnd, EditSubclassProc, 0);
		break;
	}

	return DefSubclassProc(hwnd, msg, wp, lp);
}

void MainWindow::BeginLvEdit(HWND hLv, int item, int subitem)
{
	// 如果已有编辑框，先结束
	EndLvEdit(true);

	m_nLvEditItem = item;
	m_nLvEditSubItem = subitem;
	m_bLvEditAccept = false;

	// 获取子项矩形
	RECT rc = {};
	ListView_GetSubItemRect(hLv, item, subitem, LVIR_LABEL, &rc);

	// 获取当前文本
	std::string curText = GetListViewItem(hLv, item, subitem);

	// 创建编辑框
	m_hLvEdit = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", curText.c_str(),
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		hLv, (HMENU)IDC_LVEDIT, m_hInst, nullptr);

	// 子类化编辑框
	SetWindowSubclass(m_hLvEdit, EditSubclassProc, 0, reinterpret_cast<DWORD_PTR>(this));

	// 全选文本
	SendMessageA(m_hLvEdit, EM_SETSEL, 0, -1);
	SetFocus(m_hLvEdit);
}

void MainWindow::EndLvEdit(bool accept)
{
	if (!m_hLvEdit)
		return;

	if (accept)
	{
		char buf[256] = {};
		GetWindowTextA(m_hLvEdit, buf, sizeof(buf));

		// 找到编辑框的父窗口(ListView)
		HWND hLv = GetParent(m_hLvEdit);
		SetListViewItem(hLv, m_nLvEditItem, m_nLvEditSubItem, buf);
	}

	DestroyWindow(m_hLvEdit);
	m_hLvEdit = nullptr;
	m_nLvEditItem = -1;
	m_nLvEditSubItem = -1;
}
