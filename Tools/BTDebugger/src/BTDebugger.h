#pragma once
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
#include <string>
#include <vector>
#include <memory>
#include "BTNode.h"
#include "BTEngine.h"

// 控件ID
#define ID_TREEVIEW      1001
#define ID_PROPLIST      1002
#define ID_LOGEDIT       1003
#define ID_BTN_LOAD      2001
#define ID_BTN_STEP      2002
#define ID_BTN_AUTO      2003
#define ID_BTN_RESET     2004
#define ID_BTN_SAVE      2005
#define ID_SPEED_SLIDER  2006
#define ID_SPEED_LABEL   2007
#define ID_STATUS_BAR    3001
#define ID_TIMER_AUTO    4001

// 日志过滤模式
#define LOG_FILTER_ALL      0
#define LOG_FILTER_FAIL     1
#define LOG_FILTER_SUCCESS  2
#define LOG_FILTER_RUNNING  3

// 新增按钮ID
#define ID_BTN_DIAGNOSE  2008
#define ID_BTN_LOGFILTER 2009
#define ID_BTN_SEARCH    2010
#define ID_EDIT_SEARCH   2011

// 游戏状态面板控件ID（7000段）
// 该面板让用户直接修改 ExecutionContext，驱动条件节点判定结果
#define ID_CTX_PANEL     7000
#define ID_CTX_HP        7001
#define ID_CTX_MP        7002
#define ID_CTX_ISDEAD    7003
#define ID_CTX_SAFEAREA  7004
#define ID_CTX_HASTARGET 7005
#define ID_CTX_TARGETDIST 7006
#define ID_CTX_BAGFULL   7007
#define ID_CTX_MONSTERCOUNT 7008
#define ID_CTX_APPLY     7009

// 树节点右键菜单 ID
#define IDM_RENAME_NODE   5001
#define IDM_ADD_CHILD     5002
#define IDM_DELETE_NODE   5003
#define IDM_MOVE_UP       5004
#define IDM_MOVE_DOWN     5005
#define IDM_EDIT_PROPS    5006
#define IDM_CHANGE_TYPE   5007

// 窗口类
class CBTDebugger
{
public:
    CBTDebugger(HINSTANCE hInstance);
    ~CBTDebugger();

    bool Init(int nCmdShow);
    int Run();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HWND m_hTreeView;
    HWND m_hPropList;
    HWND m_hLogEdit;
    HWND m_hStatusBar;
    HWND m_hBtnLoad;
    HWND m_hBtnStep;
    HWND m_hBtnAuto;
    HWND m_hBtnReset;
    HWND m_hBtnSave;
    HWND m_hSpeedSlider;
    HWND m_hSpeedLabel;

    // 游戏状态面板控件（让用户编辑 ExecutionContext）
    // 用于驱动 ConditionLowHP/HasTarget/InSafeArea 等条件节点的判定结果
    HWND m_hCtxPanel;
    HWND m_hCtxHP;
    HWND m_hCtxMP;
    HWND m_hCtxIsDead;
    HWND m_hCtxSafeArea;
    HWND m_hCtxHasTarget;
    HWND m_hCtxTargetDist;
    HWND m_hCtxBagFull;
    HWND m_hCtxMonsterCount;
    HWND m_hCtxApply;
    int  m_ctxPanelW;   // 面板宽度（布局用）
    int  m_ctxPanelH;   // 面板高度（布局用）
    std::vector<HWND> m_ctxStatics; // 面板内 STATIC 标签句柄（布局时需重定位）

    // 分隔条
    int m_splitterPos;          // 垂直分隔条x(TreeView右边界)
    int m_rightSplitterPos;     // 水平分隔条y(PropList底/LogList顶,相对工具栏下方)
    int m_dragMode;             // 拖拽模式: 0=无 1=垂直 2=水平
    int m_logFilter;            // 日志过滤模式(LOG_FILTER_*)

    // 行为树数据
    std::shared_ptr<BTNode> m_pRoot;
    BTEngine m_engine;
    std::wstring m_currentFile;

    // 自动执行
    bool m_isAutoRunning;
    int m_autoSpeed; // ms

    // 搜索状态
    std::wstring m_selectedNodeId;

    // 窗口尺寸
    int m_width, m_height;

    // 初始化
    void CreateControls();
    void LayoutControls();
    void InitTreeViewImages();

    // 消息处理
    void OnSize(int width, int height);
    void OnCommand(WORD id);
    LRESULT OnNotify(NMHDR* pnmh);
    void OnTimer(UINT_PTR id);
    void OnPaint(HDC hdc);

    // 行为树操作
    void LoadXMLFile();
    void LoadXMLFromString(const std::string& xml);
    bool LoadBuiltInSample(const std::wstring& name);
    void SaveXMLFile();
    void PopulateTreeView();
    void PopulateTreeViewRecursive(HTREEITEM hParent, std::shared_ptr<BTNode> node);
    void UpdateNodeStates();
    void UpdateNodeStateRecursive(HTREEITEM hItem);
    HTREEITEM FindTreeItem(std::shared_ptr<BTNode> node);
    void StepExecute();
    void StartAuto();
    void StopAuto();
    void ResetExecution();
    void UpdatePropertyPanel();
    void UpdateLogPanel();
    void UpdateStatusBar();

    // 节点编辑
    void OnTreeContextMenu(POINT pt);
    void RenameSelectedNode();
    void AddChildToSelectedNode();
    void DeleteSelectedNode();
    void MoveSelectedNodeUp();
    void MoveSelectedNodeDown();
    void EditNodeProperties();
    void ChangeNodeType();
    void OnPropListDoubleClick();
    std::shared_ptr<BTNode> GetSelectedNode();

    // 游戏状态面板（ExecutionContext 编辑）
    // SyncContextToUI: 从 m_engine.GetContext() 同步到 Edit 控件
    // SyncContextFromUI: 从 Edit 控件回写到 m_engine.GetContext()
    // OnContextApply: 用户点击"应用"按钮时调用
    void SyncContextToUI();
    void SyncContextFromUI();
    void OnContextApply();

    // 分隔条
    void OnLButtonDown(int x, int y);
    void OnMouseMove(int x, int y);
    void OnLButtonUp();

    // 工具函数
    std::string WStringToUTF8(const std::wstring& wstr);
    std::wstring UTF8ToWString(const std::string& str);
    void SetLogText(const std::wstring& text);
    void AppendLogText(const std::wstring& text);

    // ===== 增强功能 =====
    // 行为树诊断:检查结构错误(复合节点无子、装饰节点子数错、叶子节点有子、参数缺失等)
    // 结果输出到日志 ListView
    void DiagnoseTree();
    void DiagnoseRecursive(std::shared_ptr<BTNode> node, std::vector<std::wstring>& issues);
    // 日志 ListView 双击:跳转到对应树节点并选中
    void OnLogListDoubleClick();
    // 跳转到指定节点名(展开父节点+选中+滚动可见)
    bool JumpToNode(const std::wstring& nodeName);
    // 初始化日志 ListView 列
    void InitLogListColumns();
    // 设置光标(分隔条上方显示分割光标)
    LRESULT OnSetCursor(HWND hWnd, WORD hitCode);
    // 搜索:在树中定位节点名包含关键字的第一个节点
    void OnSearch();
    // 获取时间相关节点的剩余时间描述(供 UpdateNodeStateRecursive 显示)
    // 返回如 "剩余:2000ms" 或空串
    std::wstring GetNodeRemainingTimeImpl(BTNode* pNode);
    // 暴露引擎给辅助函数(获取 frameTime/m_decoratorState 计算剩余时间)
    BTEngine& GetEngine() { return m_engine; }

    // 静态实例指针
    static CBTDebugger* s_pInstance;
};