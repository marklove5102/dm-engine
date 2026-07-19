#pragma once

#include <windows.h>
#include <commctrl.h>
#include "ActionCfgData.h"
#include "resource.h"
#include <string>

/// 主窗口——管理全部UI与编辑逻辑
class MainWindow
{
public:
	MainWindow(HINSTANCE hInst);
	~MainWindow() = default;

	/// 注册窗口类 + 创建窗口
	bool Init(int nCmdShow);

	/// 主窗口过程
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	/// ListView子类化——处理NM_DBLCLK弹出编辑框
	static LRESULT CALLBACK LvSubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
		UINT_PTR subclassId, DWORD_PTR refData);

	/// 编辑框子类化——处理回车/ESC/失焦提交
	static LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
		UINT_PTR subclassId, DWORD_PTR refData);

private:
	// ---- 创建 ----
	void CreateMenuBar();
	void CreateTabControl();
	void CreateStatusBar();
	void CreateTabPages();
	void CreateDrawOrderPage();
	void CreateMonsterPage();
	void CreateWingPage();
	void CreateWingEffPage();

	// ---- 布局 ----
	void OnSize(int width, int height);

	// ---- Tab切换 ----
	void OnTabSelChange();
	void ShowTabPage(int index);

	// ---- 文件操作 ----
	void OnOpen();
	void OnSave();
	void OnSaveAs();
	void RefreshAll();

	// ---- DrawOrder ----
	void RefreshDrawOrder();
	void OnDoSelectionChanged();
	void RefreshDrawOrderDetail(SDrawOrderEntry& entry);

	// ---- Monster ----
	void RefreshMonsterList();
	void OnMonsterSelChange();
	void RefreshMonsterDetail(MonsterEntry& monster);
	void OnAddMonster();
	void OnDelMonster();
	void OnAddMonsterAction();
	void OnDelMonsterAction();

	// ---- Wing ----
	void RefreshWingList();
	void OnWingSelChange();
	void RefreshWingDetail(WingEntry& wing);
	void OnAddWing();
	void OnDelWing();
	void OnAddWingAction();
	void OnDelWingAction();

	// ---- WingEffect ----
	void RefreshWingEffList();
	void OnWingEffSelChange();
	void RefreshWingEffDetail(WingEntry& wing);
	void OnAddWingEff();
	void OnDelWingEff();
	void OnAddWingEffAction();
	void OnDelWingEffAction();

	// ---- 数据同步 ----
	void ApplyAllChanges();

	// ---- 辅助 ----
	static const char* GetActionName(int action);
	static int ParseActionName(const char* name);
	static const char* GetDrawPartName(int idx);
	
	void InitListViewColumns(HWND hLv, const char* const* headers, const int* widths, int count, bool firstEditable);
	void SetListViewItem(HWND hLv, int row, int col, const char* text);
	std::string GetListViewItem(HWND hLv, int row, int col);
	
	void BeginLvEdit(HWND hLv, int item, int subitem);
	void EndLvEdit(bool accept);
	
	int GetComboActionIndex(HWND hCombo);
	void SetComboAction(HWND hCombo, int action);

	// ---- 数据 ----
	HINSTANCE m_hInst;
	HWND m_hwnd = nullptr;
	HWND m_hTab = nullptr;
	HWND m_hStatus = nullptr;

	// Tab页容器(共4页)
	HWND m_rgTabPages[4] = {};

	// ==== DrawOrder控件 ====
	HWND m_hDoList = nullptr;       // 上: 条目列表ListView
	HWND m_hDoDetail = nullptr;     // 下: 详细数据ListView

	// ==== Monster控件 ====
	HWND m_hMonListBox = nullptr;
	HWND m_hMonListView = nullptr;
	HWND m_hMonBtnAdd = nullptr, m_hMonBtnDel = nullptr;
	HWND m_hMonBtnAddAct = nullptr, m_hMonBtnDelAct = nullptr;
	int m_nSelectedMonster = -1;

	// ==== Wing控件 ====
	HWND m_hWingListBox = nullptr;
	HWND m_hWingListView = nullptr;
	HWND m_hWingBtnAdd = nullptr, m_hWingBtnDel = nullptr;
	HWND m_hWingBtnAddAct = nullptr, m_hWingBtnDelAct = nullptr;
	int m_nSelectedWing = -1;

	// ==== WingEffect控件 ====
	HWND m_hWEffListBox = nullptr;
	HWND m_hWEffListView = nullptr;
	HWND m_hWEffBtnAdd = nullptr, m_hWEffBtnDel = nullptr;
	HWND m_hWEffBtnAddAct = nullptr, m_hWEffBtnDelAct = nullptr;
	int m_nSelectedWEff = -1;

	// ==== ListView编辑覆盖 ====
	HWND m_hLvEdit = nullptr;
	int m_nLvEditItem = -1, m_nLvEditSubItem = -1;
	bool m_bLvEditAccept = false;

	// ==== 核心数据 ====
	ActionCfgData m_data;
	std::string m_strCurrentFile;

	// ==== 动作名称列表(静态) ====
	static const char* sm_ActionNames[];
	static const char* sm_DrawPartNames[];
};
