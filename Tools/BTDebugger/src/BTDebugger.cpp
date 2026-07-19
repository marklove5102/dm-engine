#include "BTDebugger.h"
#include <fstream>
#include <sstream>
#include <codecvt>
#include <map>
#include <algorithm>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ============================================================================
// 自制对话框窗口过程
// ============================================================================
static LRESULT CALLBACK DlgFrameWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        // 将 WM_COMMAND 转发为自定义消息，让模态循环能捕获
        PostMessageW(hWnd, WM_USER + 100, wParam, lParam);
        return 0;
    case WM_CLOSE:
        PostMessageW(hWnd, WM_USER + 101, 0, 0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

CBTDebugger* CBTDebugger::s_pInstance = nullptr;

// 增强功能控件句柄（头文件未声明这些成员，使用文件作用域 static）
static HWND g_hBtnDiagnose  = nullptr;
static HWND g_hBtnLogFilter = nullptr;
static HWND g_hBtnSearch    = nullptr;
static HWND g_hEditSearch   = nullptr;

// 日志过滤模式显示名
static const wchar_t* GetLogFilterName(int filter)
{
    switch (filter)
    {
    case LOG_FILTER_ALL:     return L"全部";
    case LOG_FILTER_FAIL:    return L"失败";
    case LOG_FILTER_SUCCESS: return L"成功";
    case LOG_FILTER_RUNNING: return L"执行";
    }
    return L"全部";
}

// 递归在 TreeView 中查找指定节点名对应的 hItem
// 注: 树项显示文本可能带 "[结果]" 后缀, 故通过 lParam 取真实节点名比对
static HTREEITEM FindTreeItemByName(HWND hTree, HTREEITEM hItem, const std::wstring& name)
{
    if (!hItem) return nullptr;
    TVITEMW item = {};
    item.hItem = hItem;
    item.mask = TVIF_PARAM;
    if (TreeView_GetItem(hTree, &item) && item.lParam)
    {
        BTNode* pNode = (BTNode*)item.lParam;
        if (pNode->name == name) return hItem;
    }
    HTREEITEM hChild = TreeView_GetChild(hTree, hItem);
    while (hChild)
    {
        HTREEITEM found = FindTreeItemByName(hTree, hChild, name);
        if (found) return found;
        hChild = TreeView_GetNextSibling(hTree, hChild);
    }
    return nullptr;
}

// 内置示例数据 - 对齐 GameServer 机器人配置备份目录下的真实行为树
// 来源: MirWorldServer_xLib/资料文件/开发资料/机器人配置备份/
//   BotBehavior_Warrior.xml  - 战士(近战肉搏型)
//   BotBehavior_Mage.xml     - 法师(远程魔法型)
//   BotBehavior_Hunter.xml   - 猎人(简化型)
//   BotBehavior_HumanLike.xml- 真人模拟(覆盖23种节点)
// 注: 为控制字符串量已删除原 XML 中的大段中文注释, 保留全部节点与属性
static const std::map<std::wstring, std::string> g_builtInSamples = {
    { L"战士行为树(Warrior)", R"BT(<?xml version="1.0" encoding="GBK"?>
<BehaviorTree name="战士行为树">
    <Selector name="战士主决策">
        <MemSequence name="死亡-复活">
            <ConditionIsDead name="检查死亡"/>
            <ActionSay name="死亡喊话" message="哎呀，我挂了..."/>
            <ActionRevive name="延迟复活回城" hpPercent="100" teleportHome="1" delay="8000"/>
        </MemSequence>
        <Sequence name="生存-紧急逃跑">
            <ConditionLowHP name="HP低于15%" percent="15"/>
            <ActionFlee name="立即逃跑"/>
        </Sequence>
        <Sequence name="生存-紧急吃药">
            <ConditionLowHP name="HP低于35%" percent="35"/>
            <ActionUsePotion name="使用HP药水" hpType="1"/>
        </Sequence>
        <Sequence name="生存-补蓝">
            <ConditionLowMP name="MP低于20%" percent="20"/>
            <ActionUsePotion name="使用MP药水" hpType="0"/>
        </Sequence>
        <Sequence name="战斗-攻击目标">
            <ConditionHasTarget name="有攻击目标"/>
            <Random name="战斗随机行为">
                <Sequence name="正常攻击">
                    <ActionChangeAttackMode name="切换善恶模式" attackMode="0"/>
                    <Selector name="战士技能选择">
                        <Sequence name="烈火连招">
                            <ActionUseSkill name="烈火剑法" magicId="26"/>
                            <ActionAttack name="接普攻"/>
                        </Sequence>
                        <Sequence name="半月连招">
                            <ActionUseSkill name="半月弯刀" magicId="25"/>
                            <ActionAttack name="接普攻"/>
                        </Sequence>
                        <ActionAttack name="普通攻击"/>
                    </Selector>
                    <Random name="战后拾取">
                        <ActionPickupItem name="拾取物品"/>
                        <ActionPickupItem name="拾取物品"/>
                        <ActionAttack name="继续攻击"/>
                    </Random>
                </Sequence>
                <Sequence name="接近目标">
                    <ActionMoveToTarget name="跑向目标"/>
                </Sequence>
                <Sequence name="战斗中吃药">
                    <ActionUsePotion name="补一口血" hpType="1"/>
                    <ActionAttack name="继续攻击"/>
                </Sequence>
                <ActionRest name="战斗中停顿" duration="800"/>
            </Random>
        </Sequence>
        <Sequence name="巡逻-搜索目标">
            <Inverter name="不在安全区">
                <ConditionInSafeArea name="检查安全区"/>
            </Inverter>
            <Random name="巡逻行为">
                <Sequence name="巡逻找怪">
                    <ActionPatrol name="巡逻移动"/>
                    <ActionPatrol name="继续巡逻"/>
                </Sequence>
                <Sequence name="边走边聊">
                    <ActionPatrol name="巡逻移动"/>
                    <ActionChat name="聊天"/>
                </Sequence>
                <ActionRest name="发呆" duration="3000"/>
                <ActionPickupItem name="捡东西"/>
            </Random>
        </Sequence>
        <Sequence name="安全区-休闲">
            <ConditionInSafeArea name="在安全区"/>
            <Random name="安全区行为">
                <ActionChat name="聊天"/>
                <ActionRest name="发呆" duration="5000"/>
                <ActionPatrol name="城内走动"/>
                <Random name="使用物品">
                    <ActionUseItem name="使用回城卷" itemName="回城卷"/>
                    <ActionUsePotion name="补药" hpType="1"/>
                </Random>
            </Random>
        </Sequence>
        <Sequence name="背包满-回城">
            <ConditionBagFull name="背包已满"/>
            <ActionUseItem name="使用回城卷" itemName="回城卷"/>
            <ActionRest name="回城后休息" duration="8000"/>
        </Sequence>
    </Selector>
</BehaviorTree>)BT" },

    { L"法师行为树(Mage)", R"BT(<?xml version="1.0" encoding="GBK"?>
<BehaviorTree name="法师行为树">
    <Selector name="法师主决策">
        <MemSequence name="死亡-复活">
            <ConditionIsDead name="检查死亡"/>
            <ActionSay name="死亡喊话" message="哎呀，我挂了..."/>
            <ActionRevive name="延迟复活回城" hpPercent="100" teleportHome="1" delay="8000"/>
        </MemSequence>
        <Sequence name="生存-紧急逃跑">
            <ConditionLowHP name="HP低于20%" percent="20"/>
            <Selector name="法师逃跑策略">
                <Sequence name="随机传送逃跑">
                    <ActionUseItem name="使用随机传送卷" itemName="随机传送卷"/>
                    <ActionRest name="传送后停顿" duration="1500"/>
                </Sequence>
                <ActionFlee name="跑步逃跑"/>
            </Selector>
        </Sequence>
        <Sequence name="生存-紧急吃药">
            <ConditionLowHP name="HP低于45%" percent="45"/>
            <Selector name="吃药策略">
                <Sequence name="吃HP药">
                    <ActionUsePotion name="使用HP药水" hpType="1"/>
                    <ActionFlee name="后退一步"/>
                </Sequence>
                <ActionFlee name="没有药水逃跑"/>
            </Selector>
        </Sequence>
        <Sequence name="生存-补蓝">
            <ConditionLowMP name="MP低于30%" percent="30"/>
            <ActionUsePotion name="使用MP药水" hpType="0"/>
        </Sequence>
        <Sequence name="战斗-魔法攻击">
            <ConditionHasTarget name="有攻击目标"/>
            <ActionChangeAttackMode name="切换善恶模式" attackMode="0"/>
            <Random name="法师战斗行为">
                <Sequence name="标准魔法攻击">
                    <Selector name="法师技能选择">
                        <Sequence name="雷电术连击">
                            <ActionUseSkill name="雷电术" magicId="11"/>
                            <ActionRest name="施法间隔" duration="600"/>
                            <ActionUseSkill name="雷电术" magicId="11"/>
                        </Sequence>
                        <Sequence name="地狱雷光AOE">
                            <ActionUseSkill name="地狱雷光" magicId="24"/>
                            <ActionRest name="施法间隔" duration="500"/>
                        </Sequence>
                        <Sequence name="爆裂火焰">
                            <ActionUseSkill name="爆裂火焰" magicId="23"/>
                            <ActionRest name="施法间隔" duration="500"/>
                        </Sequence>
                        <Sequence name="火墙控场">
                            <ActionUseSkill name="火墙" magicId="22"/>
                            <ActionRest name="等怪走进火墙" duration="1000"/>
                        </Sequence>
                        <ActionUseSkill name="火球术" magicId="1"/>
                    </Selector>
                </Sequence>
                <Sequence name="走位放风筝">
                    <ActionFlee name="后退走位"/>
                    <Selector name="走位后技能">
                        <ActionUseSkill name="雷电术" magicId="11"/>
                        <ActionUseSkill name="爆裂火焰" magicId="23"/>
                    </Selector>
                </Sequence>
                <Sequence name="接近目标">
                    <ActionMoveToTarget name="跑向目标"/>
                </Sequence>
                <Sequence name="战斗中补药">
                    <ActionUsePotion name="补蓝" hpType="0"/>
                    <ActionUseSkill name="雷电术" magicId="11"/>
                </Sequence>
                <ActionRest name="战斗中停顿" duration="1000"/>
            </Random>
        </Sequence>
        <Sequence name="巡逻-搜索目标">
            <Inverter name="不在安全区">
                <ConditionInSafeArea name="检查安全区"/>
            </Inverter>
            <Random name="法师巡逻行为">
                <Sequence name="巡逻找怪">
                    <ActionPatrol name="巡逻移动"/>
                    <ActionPatrol name="继续巡逻"/>
                </Sequence>
                <Sequence name="边走边聊">
                    <ActionPatrol name="巡逻移动"/>
                    <ActionChat name="聊天"/>
                </Sequence>
                <ActionRest name="观察四周" duration="4000"/>
                <ActionPickupItem name="捡东西"/>
                <Sequence name="巡逻中补药">
                    <ActionUsePotion name="补蓝" hpType="0"/>
                    <ActionPatrol name="继续巡逻"/>
                </Sequence>
            </Random>
        </Sequence>
        <Sequence name="安全区-休闲">
            <ConditionInSafeArea name="在安全区"/>
            <Random name="安全区行为">
                <ActionChat name="聊天"/>
                <ActionRest name="发呆" duration="6000"/>
                <ActionPatrol name="城内走动"/>
                <Sequence name="整理补给">
                    <ActionUsePotion name="补HP" hpType="1"/>
                    <ActionUsePotion name="补MP" hpType="0"/>
                </Sequence>
                <ActionUseItem name="使用回城卷" itemName="回城卷"/>
            </Random>
        </Sequence>
        <Sequence name="背包满-回城">
            <ConditionBagFull name="背包已满"/>
            <ActionUseItem name="使用回城卷" itemName="回城卷"/>
            <ActionRest name="回城后休息" duration="10000"/>
        </Sequence>
    </Selector>
</BehaviorTree>)BT" },

    { L"猎人行为树(Hunter)", R"BT(<?xml version="1.0" encoding="GBK"?>
<BehaviorTree name="Hunter">
    <Selector name="Root">
        <Sequence name="Survival">
            <ConditionLowHP percent="30"/>
            <ActionFlee/>
        </Sequence>
        <Sequence name="Heal">
            <ConditionLowHP percent="50"/>
            <ActionUsePotion hpType="1"/>
        </Sequence>
        <Sequence name="Combat">
            <ConditionHasTarget/>
            <Selector name="CombatAction">
                <ActionAttack/>
                <ActionMoveToTarget/>
            </Selector>
        </Sequence>
        <Sequence name="Loot">
            <Inverter>
                <ConditionBagFull/>
            </Inverter>
            <ActionPickupItem/>
        </Sequence>
        <Sequence name="Patrol">
            <ActionPatrol/>
        </Sequence>
        <ActionChat/>
    </Selector>
</BehaviorTree>)BT" },

    { L"真人模拟行为树(HumanLike)", R"BT(<?xml version="1.0" encoding="GBK"?>
<BehaviorTree name="HumanLike">
    <Selector>
        <Sequence>
            <ConditionLowHP percent="15"/>
            <ActionFlee/>
        </Sequence>
        <Sequence>
            <ConditionLowHP percent="35"/>
            <Random>
                <ActionUsePotion hpType="1"/>
                <ActionUsePotion hpType="1"/>
                <ActionUsePotion hpType="0"/>
            </Random>
        </Sequence>
        <Sequence>
            <ConditionLowHP percent="50"/>
            <ConditionLowMP percent="20"/>
            <Random>
                <ActionRest duration="8000"/>
                <ActionRest duration="8000"/>
                <Sequence>
                    <ActionRest duration="4000"/>
                    <ActionChat/>
                </Sequence>
            </Random>
        </Sequence>
        <Sequence>
            <ConditionLowMP percent="25"/>
            <Random>
                <ActionRest duration="5000"/>
                <ActionUsePotion hpType="0"/>
            </Random>
        </Sequence>
        <Sequence>
            <ConditionHasTarget/>
            <ActionChangeAttackMode attackMode="1"/>
            <Random>
                <Sequence>
                    <ActionUseSkill magicId="0"/>
                    <ActionMoveToTarget/>
                    <ActionAttack/>
                </Sequence>
                <Sequence>
                    <ActionAttack/>
                    <ActionMoveToTarget/>
                </Sequence>
                <Sequence>
                    <ActionUseSkill magicId="0"/>
                    <DecoratorRepeat count="2">
                        <ActionAttack/>
                    </DecoratorRepeat>
                </Sequence>
            </Random>
        </Sequence>
        <Sequence>
            <ConditionBagFull/>
            <ActionChangeAttackMode attackMode="2"/>
        </Sequence>
        <Sequence>
            <ConditionInSafeArea/>
            <Random>
                <ActionRest duration="5000"/>
                <ActionChat/>
                <Sequence>
                    <DecoratorRepeat count="2">
                        <ActionPatrol/>
                    </DecoratorRepeat>
                </Sequence>
            </Random>
        </Sequence>
        <Sequence>
            <Inverter>
                <ConditionHasTarget/>
            </Inverter>
            <ActionChat/>
        </Sequence>
        <Random>
            <Parallel>
                <Sequence>
                    <DecoratorRepeat count="3">
                        <ActionPatrol/>
                    </DecoratorRepeat>
                    <ActionRest duration="2000"/>
                </Sequence>
                <ActionPickupItem/>
            </Parallel>
            <Sequence>
                <DecoratorRepeat count="5">
                    <ActionPatrol/>
                </DecoratorRepeat>
                <ActionRest duration="1500"/>
            </Sequence>
            <ActionPatrol/>
            <ActionPickupItem/>
            <Sequence>
                <ActionRest duration="3000"/>
                <ActionChat/>
            </Sequence>
        </Random>
    </Selector>
</BehaviorTree>)BT" }
};

CBTDebugger::CBTDebugger(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(nullptr)
    , m_hTreeView(nullptr), m_hPropList(nullptr), m_hLogEdit(nullptr)
    , m_hStatusBar(nullptr), m_hBtnLoad(nullptr), m_hBtnStep(nullptr)
    , m_hBtnAuto(nullptr), m_hBtnReset(nullptr), m_hBtnSave(nullptr)
    , m_hSpeedSlider(nullptr), m_hSpeedLabel(nullptr)
    , m_hCtxPanel(nullptr), m_hCtxHP(nullptr), m_hCtxMP(nullptr)
    , m_hCtxIsDead(nullptr), m_hCtxSafeArea(nullptr), m_hCtxHasTarget(nullptr)
    , m_hCtxTargetDist(nullptr), m_hCtxBagFull(nullptr)
    , m_hCtxMonsterCount(nullptr), m_hCtxApply(nullptr), m_ctxPanelW(0)
    , m_splitterPos(480), m_dragMode(0), m_rightSplitterPos(300), m_logFilter(0)
    , m_pRoot(nullptr), m_isAutoRunning(false), m_autoSpeed(500)
    , m_width(1200), m_height(750)
{
    s_pInstance = this;
}

CBTDebugger::~CBTDebugger()
{
    s_pInstance = nullptr;
}

bool CBTDebugger::Init(int nCmdShow)
{
    // 注册窗口类
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = m_hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(10, 10, 20));
    wcex.lpszClassName = L"BTDebuggerWindow";
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClassExW(&wcex);

    // 注册自制对话框窗口类
    WNDCLASSEXW dlgClass = {};
    dlgClass.cbSize = sizeof(WNDCLASSEXW);
    dlgClass.lpfnWndProc = DlgFrameWndProc;
    dlgClass.hInstance = m_hInstance;
    dlgClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    dlgClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    dlgClass.lpszClassName = L"BTDlgFrame";
    RegisterClassExW(&dlgClass);

    // 创建窗口
    m_hWnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        L"BTDebuggerWindow",
        L"机器人行为树调试器 - 达摩引擎",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        m_width, m_height,
        nullptr, nullptr, m_hInstance, nullptr
    );

    if (!m_hWnd) return false;

    CreateControls();
    LayoutControls();
    SyncContextToUI();  // 初始化游戏状态面板显示

    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);

    return true;
}

void CBTDebugger::CreateControls()
{
    // 工具栏
    m_hBtnLoad = CreateWindowW(L"BUTTON", L"加载XML文件",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        8, 8, 120, 28, m_hWnd, (HMENU)ID_BTN_LOAD, m_hInstance, nullptr);

    m_hBtnSave = CreateWindowW(L"BUTTON", L"保存XML",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        136, 8, 100, 28, m_hWnd, (HMENU)ID_BTN_SAVE, m_hInstance, nullptr);

    m_hBtnStep = CreateWindowW(L"BUTTON", L"单步执行",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        244, 8, 100, 28, m_hWnd, (HMENU)ID_BTN_STEP, m_hInstance, nullptr);

    m_hBtnAuto = CreateWindowW(L"BUTTON", L"自动播放",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        352, 8, 100, 28, m_hWnd, (HMENU)ID_BTN_AUTO, m_hInstance, nullptr);

    m_hBtnReset = CreateWindowW(L"BUTTON", L"重置",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        460, 8, 80, 28, m_hWnd, (HMENU)ID_BTN_RESET, m_hInstance, nullptr);

    m_hSpeedLabel = CreateWindowW(L"STATIC", L"速度: 500ms",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        548, 12, 100, 20, m_hWnd, (HMENU)ID_SPEED_LABEL, m_hInstance, nullptr);

    m_hSpeedSlider = CreateWindowW(TRACKBAR_CLASSW, L"",
        WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
        648, 8, 130, 28, m_hWnd, (HMENU)ID_SPEED_SLIDER, m_hInstance, nullptr);
    SendMessage(m_hSpeedSlider, TBM_SETRANGE, TRUE, MAKELONG(100, 2000));
    SendMessage(m_hSpeedSlider, TBM_SETPOS, TRUE, 500);
    SendMessage(m_hSpeedSlider, TBM_SETTICFREQ, 200, 0);

    // 增强功能: 诊断 / 日志过滤 / 节点查找
    g_hBtnDiagnose = CreateWindowW(L"BUTTON", L"诊断",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        790, 8, 70, 28, m_hWnd, (HMENU)ID_BTN_DIAGNOSE, m_hInstance, nullptr);

    g_hBtnLogFilter = CreateWindowW(L"BUTTON", L"过滤:全部",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        866, 8, 90, 28, m_hWnd, (HMENU)ID_BTN_LOGFILTER, m_hInstance, nullptr);

    CreateWindowW(L"STATIC", L"查找:", WS_CHILD | WS_VISIBLE | SS_LEFT,
        962, 12, 36, 20, m_hWnd, nullptr, m_hInstance, nullptr);
    g_hEditSearch = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        1000, 8, 130, 22, m_hWnd, (HMENU)ID_EDIT_SEARCH, m_hInstance, nullptr);
    g_hBtnSearch = CreateWindowW(L"BUTTON", L"查找",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        1136, 8, 56, 28, m_hWnd, (HMENU)ID_BTN_SEARCH, m_hInstance, nullptr);

    // 树形视图
    m_hTreeView = CreateWindowExW(WS_EX_CLIENTEDGE,
        WC_TREEVIEWW, L"",
        WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_EDITLABELS,
        0, 44, m_splitterPos, 500,
        m_hWnd, (HMENU)ID_TREEVIEW, m_hInstance, nullptr);

    // 属性列表
    m_hPropList = CreateWindowExW(WS_EX_CLIENTEDGE,
        WC_LISTVIEWW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER,
        m_splitterPos + 4, 44, 300, 300,
        m_hWnd, (HMENU)ID_PROPLIST, m_hInstance, nullptr);

    // 日志面板（ListView 报表，替代原 EDIT 多行文本框；保留变量名 m_hLogEdit 避免大改）
    m_hLogEdit = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS,
        m_splitterPos + 4, 348, 300, 200, m_hWnd, (HMENU)ID_LOGEDIT, m_hInstance, nullptr);
    ListView_SetExtendedListViewStyle(m_hLogEdit, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    // 状态栏
    m_hStatusBar = CreateWindowW(STATUSCLASSNAMEW, L"",
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0, m_hWnd, (HMENU)ID_STATUS_BAR, m_hInstance, nullptr);

    // 设置字体
    HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
    SendMessage(m_hTreeView, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(m_hPropList, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(m_hLogEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 初始化 ListView 列
    LVCOLUMNW lvc = {};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = 120;
    lvc.pszText = (LPWSTR)L"属性";
    ListView_InsertColumn(m_hPropList, 0, &lvc);
    lvc.cx = 200;
    lvc.pszText = (LPWSTR)L"值";
    ListView_InsertColumn(m_hPropList, 1, &lvc);
    ListView_SetExtendedListViewStyle(m_hPropList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // ========================================================================
    // 游戏状态面板（ExecutionContext 编辑）
    // 让用户直接修改机器人状态，驱动条件节点判定结果
    // 布局: 横向排列 HP/MP/死亡/安全区/有目标/距离/包满/怪数 + 应用按钮
    // 坐标此处先用相对值, 由 LayoutControls 统一定位到窗口底部
    // ========================================================================
    m_ctxPanelH = 56;
    auto addCtxItem = [&](const wchar_t* label, int editId, int editW, HWND& outEdit) {
        HWND hLbl = CreateWindowW(L"STATIC", label, WS_CHILD | WS_VISIBLE | SS_RIGHT,
            0, 0, 40, 16, m_hWnd, nullptr, m_hInstance, nullptr);
        SendMessage(hLbl, WM_SETFONT, (WPARAM)hFont, TRUE);
        m_ctxStatics.push_back(hLbl);
        outEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            0, 0, editW, 22, m_hWnd, (HMENU)(LONG_PTR)editId, m_hInstance, nullptr);
        SendMessage(outEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    };
    addCtxItem(L"HP%",   ID_CTX_HP,        40, m_hCtxHP);
    addCtxItem(L"MP%",   ID_CTX_MP,        40, m_hCtxMP);
    addCtxItem(L"死亡",  ID_CTX_ISDEAD,    30, m_hCtxIsDead);
    addCtxItem(L"安全区",ID_CTX_SAFEAREA,  30, m_hCtxSafeArea);
    addCtxItem(L"有目标",ID_CTX_HASTARGET, 30, m_hCtxHasTarget);
    addCtxItem(L"距离",  ID_CTX_TARGETDIST,40, m_hCtxTargetDist);
    addCtxItem(L"包满",  ID_CTX_BAGFULL,   30, m_hCtxBagFull);
    addCtxItem(L"怪数",  ID_CTX_MONSTERCOUNT, 40, m_hCtxMonsterCount);
    m_hCtxApply = CreateWindowW(L"BUTTON", L"应用状态",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 80, 22, m_hWnd, (HMENU)(LONG_PTR)ID_CTX_APPLY, m_hInstance, nullptr);
    SendMessage(m_hCtxApply, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 设置新控件字体
    SendMessage(g_hBtnDiagnose, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hBtnLogFilter, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hBtnSearch, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(g_hEditSearch, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 初始化日志 ListView 列
    InitLogListColumns();
}

void CBTDebugger::LayoutControls()
{
    // 工具栏高度 / 状态栏高度 / 游戏状态面板高度
    int toolbarH = 44;
    int statusH = 22;
    int ctxH = m_ctxPanelH;
    int mainH = m_height - toolbarH - statusH - ctxH;  // 主区域高度

    // 左侧 TreeView
    SetWindowPos(m_hTreeView, nullptr, 0, toolbarH,
        m_splitterPos, mainH, SWP_NOZORDER);

    // 右侧 PropList + LogEdit（由水平分隔条 m_rightSplitterPos 划分高度）
    int rightX = m_splitterPos + 4;
    int rightW = m_width - rightX;
    int propH = m_rightSplitterPos;
    if (propH < 80) propH = 80;
    if (propH > mainH - 80) propH = mainH - 80;
    SetWindowPos(m_hPropList, nullptr, rightX, toolbarH,
        rightW, propH, SWP_NOZORDER);
    SetWindowPos(m_hLogEdit, nullptr, rightX, toolbarH + propH + 4,
        rightW, mainH - propH - 4, SWP_NOZORDER);

    // 游戏状态面板（底部横向排列 Label+Edit）
    // 控件创建顺序与 m_ctxStatics 入栈顺序一致
    int ctxY = m_height - statusH - ctxH + 10;
    int x = 8;
    struct CtxLayout { HWND hLbl; HWND hEdit; int editW; };
    CtxLayout items[] = {
        { m_ctxStatics[0], m_hCtxHP,           40 },
        { m_ctxStatics[1], m_hCtxMP,           40 },
        { m_ctxStatics[2], m_hCtxIsDead,       30 },
        { m_ctxStatics[3], m_hCtxSafeArea,     30 },
        { m_ctxStatics[4], m_hCtxHasTarget,    30 },
        { m_ctxStatics[5], m_hCtxTargetDist,   40 },
        { m_ctxStatics[6], m_hCtxBagFull,      30 },
        { m_ctxStatics[7], m_hCtxMonsterCount, 40 },
    };
    for (auto& it : items)
    {
        SetWindowPos(it.hLbl, nullptr, x, ctxY + 4, 44, 16, SWP_NOZORDER);
        x += 48;
        SetWindowPos(it.hEdit, nullptr, x, ctxY, it.editW, 22, SWP_NOZORDER);
        x += it.editW + 8;
    }
    SetWindowPos(m_hCtxApply, nullptr, x, ctxY, 80, 22, SWP_NOZORDER);

    // 状态栏
    SetWindowPos(m_hStatusBar, nullptr, 0, 0, 0, 0, SWP_NOZORDER);
    SendMessage(m_hStatusBar, WM_SIZE, 0, 0);
}

void CBTDebugger::OnSize(int width, int height)
{
    m_width = width;
    m_height = height;
    LayoutControls();
}

void CBTDebugger::OnCommand(WORD id)
{
    switch (id)
    {
    case ID_BTN_LOAD:
        LoadXMLFile();
        break;
    case ID_BTN_SAVE:
        SaveXMLFile();
        break;
    case ID_BTN_STEP:
        StepExecute();
        break;
    case ID_BTN_AUTO:
        if (m_isAutoRunning) StopAuto();
        else StartAuto();
        break;
    case ID_BTN_RESET:
        ResetExecution();
        break;
    case ID_BTN_DIAGNOSE:
        DiagnoseTree();
        break;
    case ID_BTN_LOGFILTER:
    {
        // 切换过滤模式 ALL→FAIL→SUCCESS→RUNNING→ALL
        m_logFilter++;
        if (m_logFilter > LOG_FILTER_RUNNING) m_logFilter = LOG_FILTER_ALL;
        std::wstring txt = L"过滤:" + std::wstring(GetLogFilterName(m_logFilter));
        SetWindowTextW(g_hBtnLogFilter, txt.c_str());
        UpdateLogPanel();
        break;
    }
    case ID_BTN_SEARCH:
        OnSearch();
        break;
    case ID_CTX_APPLY:
        OnContextApply();
        break;
    }
}

LRESULT CBTDebugger::OnNotify(NMHDR* pnmh)
{
    if (pnmh->idFrom == ID_TREEVIEW)
    {
        if (pnmh->code == TVN_SELCHANGEDW)
        {
            NMTREEVIEWW* pnmtv = (NMTREEVIEWW*)pnmh;
            TVITEMW item = pnmtv->itemNew;
            if (item.lParam)
            {
                BTNode* pNode = (BTNode*)item.lParam;
                m_selectedNodeId = pNode->id;
                UpdatePropertyPanel();
            }
        }
        else if (pnmh->code == TVN_ENDLABELEDITW)
        {
            NMTVDISPINFOW* pnmtv = (NMTVDISPINFOW*)pnmh;
            if (pnmtv->item.pszText)
            {
                BTNode* pNode = (BTNode*)pnmtv->item.lParam;
                if (pNode)
                {
                    pNode->name = pnmtv->item.pszText;
                    UpdatePropertyPanel();
                    AppendLogText(L"[编辑] 节点重命名为 '" + pNode->name + L"'\r\n");
                    return TRUE; // 接受重命名
                }
            }
            return FALSE; // 拒绝重命名
        }
        else if (pnmh->code == NM_RCLICK)
        {
            POINT pt;
            GetCursorPos(&pt);
            OnTreeContextMenu(pt);
        }
        else if (pnmh->code == NM_CUSTOMDRAW)
        {
            // 树节点自定义绘制：按执行结果着色 + 当前执行节点加粗
            LPNMTVCUSTOMDRAW pcd = (LPNMTVCUSTOMDRAW)pnmh;
            if (pcd->nmcd.dwDrawStage == CDDS_PREPAINT)
                return CDRF_NOTIFYITEMDRAW;
            else if (pcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
            {
                BTNode* pNode = (BTNode*)pcd->nmcd.lItemlParam;
                if (pNode)
                {
                    // 根据上次执行结果设置文字颜色
                    switch (pNode->lastResult)
                    {
                    case BTResult::SUCCESS: pcd->clrText = RGB(0, 200, 0);   break; // 绿
                    case BTResult::FAILURE: pcd->clrText = RGB(255, 80, 80);  break; // 红
                    case BTResult::RUNNING: pcd->clrText = RGB(255, 200, 0);  break; // 黄
                    default: break; // IDLE 保持默认色
                    }
                    // 当前执行节点加粗显示
                    if (pNode->isActive)
                    {
                        static HFONT hBoldFont = nullptr;
                        if (!hBoldFont)
                        {
                            hBoldFont = CreateFontW(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
                        }
                        SelectObject(pcd->nmcd.hdc, hBoldFont);
                        return CDRF_NEWFONT;
                    }
                }
                return CDRF_DODEFAULT;
            }
            return CDRF_DODEFAULT;
        }
    }
    else if (pnmh->idFrom == ID_PROPLIST && pnmh->code == NM_DBLCLK)
    {
        OnPropListDoubleClick();
    }
    else if (pnmh->idFrom == ID_LOGEDIT && pnmh->code == NM_DBLCLK)
    {
        OnLogListDoubleClick();
    }
    return 0;
}

void CBTDebugger::OnTimer(UINT_PTR id)
{
    if (id == ID_TIMER_AUTO && m_isAutoRunning)
    {
        if (!m_pRoot) return;
        // 自动播放：每 tick 推进 m_autoSpeed 毫秒模拟时钟
        // 使用 ExecuteStep 而非 ExecuteFull,保留跨 tick 时间状态:
        //   - m_decoratorState: ActionRest(duration)/Revive(delay)/Cooldown(ms) 计时
        //   - m_ctx.frameTime: AdvanceFrame 累加,不被 Reset 清零
        //   - m_memPositions: MemSequence/MemSelector 记忆位置
        // 这样时间相关节点能产生可见的 RUNNING→SUCCESS 演变:
        //   ActionRest duration=3000 → 连续 RUNNING 3 秒后 SUCCESS
        //   DecoratorCooldown ms=3000 → 冷却期内连续 FAILURE 3 秒
        m_engine.AdvanceFrame((DWORD)m_autoSpeed);
        m_engine.ExecuteStep(m_pRoot);

        UpdateNodeStates();
        UpdateLogPanel();
        UpdateStatusBar();
        InvalidateRect(m_hTreeView, nullptr, TRUE);
    }
}

void CBTDebugger::OnPaint(HDC hdc)
{
    // 绘制垂直分隔条
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    RECT splitter = { m_splitterPos, 44, m_splitterPos + 4, rc.bottom - 22 };
    FillRect(hdc, &splitter, (HBRUSH)GetStockObject(DKGRAY_BRUSH));

    // 绘制水平分隔条（PropList 与 LogList 之间）
    RECT hSplitter = { m_splitterPos + 4, 44 + m_rightSplitterPos,
                       m_width, 44 + m_rightSplitterPos + 4 };
    FillRect(hdc, &hSplitter, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
}

void CBTDebugger::OnLButtonDown(int x, int y)
{
    // 垂直分隔条
    if (x >= m_splitterPos && x <= m_splitterPos + 4 && y > 44)
    {
        m_dragMode = 1;
        SetCapture(m_hWnd);
        return;
    }
    // 水平分隔条
    int hSplitY = 44 + m_rightSplitterPos;
    if (y >= hSplitY && y <= hSplitY + 4 && x > m_splitterPos + 4)
    {
        m_dragMode = 2;
        SetCapture(m_hWnd);
    }
}

void CBTDebugger::OnMouseMove(int x, int y)
{
    if (m_dragMode == 1)
    {
        // 调整垂直分隔条
        if (x > 200 && x < m_width - 200)
        {
            m_splitterPos = x;
            LayoutControls();
            InvalidateRect(m_hWnd, nullptr, FALSE);
        }
    }
    else if (m_dragMode == 2)
    {
        // 调整水平分隔条
        int statusH = 22;
        int ctxH = m_ctxPanelH;
        int mainH = m_height - 44 - statusH - ctxH;
        int newPos = y - 44;
        if (newPos < 80) newPos = 80;
        if (mainH - 80 >= 80 && newPos > mainH - 80) newPos = mainH - 80;
        m_rightSplitterPos = newPos;
        LayoutControls();
        InvalidateRect(m_hWnd, nullptr, FALSE);
    }
}

void CBTDebugger::OnLButtonUp()
{
    if (m_dragMode != 0)
    {
        m_dragMode = 0;
        ReleaseCapture();
    }
}

void CBTDebugger::LoadXMLFile()
{
    OPENFILENAMEW ofn = {};
    wchar_t szFile[260] = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 260;
    ofn.lpstrFilter = L"XML 行为树文件\0*.xml\0所有文件\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        m_currentFile = szFile;
        auto root = XMLParser::ParseFile(m_currentFile);
        if (root)
        {
            m_pRoot = root;
            m_engine.Reset();
            PopulateTreeView();
            UpdateStatusBar();
            SyncContextToUI();  // 同步默认 ExecutionContext 到面板
        }
        else
        {
            MessageBoxW(m_hWnd, L"XML 文件解析失败！请检查文件格式。", L"错误", MB_ICONERROR);
        }
    }
}

void CBTDebugger::LoadXMLFromString(const std::string& xml)
{
    auto root = XMLParser::ParseString(xml);
    if (root)
    {
        m_pRoot = root;
        m_engine.Reset();
        m_currentFile = L"(内置示例)";
        PopulateTreeView();
        UpdateStatusBar();
        SyncContextToUI();  // 同步默认 ExecutionContext 到面板
    }
    else
    {
        MessageBoxW(m_hWnd, L"XML 解析失败！", L"错误", MB_ICONERROR);
    }
}

void CBTDebugger::PopulateTreeView()
{
    TreeView_DeleteAllItems(m_hTreeView);
    if (!m_pRoot) return;
    PopulateTreeViewRecursive(TVI_ROOT, m_pRoot);
}

void CBTDebugger::PopulateTreeViewRecursive(HTREEITEM hParent, std::shared_ptr<BTNode> node)
{
    TVINSERTSTRUCTW tvis = {};
    tvis.hParent = hParent;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item.mask = TVIF_TEXT | TVIF_PARAM;
    tvis.item.pszText = (LPWSTR)node->name.c_str();
    tvis.item.lParam = (LPARAM)node.get();

    HTREEITEM hItem = TreeView_InsertItem(m_hTreeView, &tvis);

    for (auto& child : node->children)
        PopulateTreeViewRecursive(hItem, child);
}

void CBTDebugger::StepExecute()
{
    if (!m_pRoot) return;

    // 单步执行:完整重置(含时间状态),独立运行一次
    // ExecuteFull 内部调 ClearTimeState+Reset,清空所有跨tick状态
    m_engine.ExecuteFull(m_pRoot);

    UpdateNodeStates();
    UpdateLogPanel();
    UpdateStatusBar();

    // 重新绘制树视图
    InvalidateRect(m_hTreeView, nullptr, TRUE);
}

void CBTDebugger::UpdateNodeStates()
{
    if (!m_pRoot) return;
    HTREEITEM hRoot = TreeView_GetRoot(m_hTreeView);
    if (hRoot)
        UpdateNodeStateRecursive(hRoot);
}

void CBTDebugger::UpdateNodeStateRecursive(HTREEITEM hItem)
{
    if (!hItem) return;

    TVITEMW item = {};
    item.hItem = hItem;
    item.mask = TVIF_PARAM;
    TreeView_GetItem(m_hTreeView, &item);
    BTNode* pNode = (BTNode*)item.lParam;

    if (pNode)
    {
        std::wstring text = pNode->name;
        if (pNode->lastResult != BTResult::IDLE)
        {
            text += L" [";
            text += GetResultName(pNode->lastResult);
            // 时间相关节点在 RUNNING 时显示剩余时间,直观体现时间属性效果
            // ActionRest(duration)/Revive(delay)/Cooldown(ms)/Timeout(ms)/Periodic(ms)
            if (pNode->lastResult == BTResult::RUNNING)
            {
                std::wstring remain = CBTDebugger::s_pInstance ? CBTDebugger::s_pInstance->GetNodeRemainingTimeImpl(pNode) : L"";
                if (!remain.empty())
                {
                    text += L" ";
                    text += remain;
                }
            }
            text += L"]";
        }
        item.mask = TVIF_TEXT;
        item.pszText = (LPWSTR)text.c_str();
        TreeView_SetItem(m_hTreeView, &item);
    }

    // 递归处理所有子节点
    HTREEITEM hChild = TreeView_GetChild(m_hTreeView, hItem);
    while (hChild)
    {
        UpdateNodeStateRecursive(hChild);
        hChild = TreeView_GetNextSibling(m_hTreeView, hChild);
    }
}

void CBTDebugger::StartAuto()
{
    if (!m_pRoot) return;
    m_isAutoRunning = true;
    SetWindowTextW(m_hBtnAuto, L"暂停");
    SetTimer(m_hWnd, ID_TIMER_AUTO, m_autoSpeed, nullptr);
    StepExecute();
}

void CBTDebugger::StopAuto()
{
    m_isAutoRunning = false;
    KillTimer(m_hWnd, ID_TIMER_AUTO);
    SetWindowTextW(m_hBtnAuto, L"自动播放");
}

void CBTDebugger::ResetExecution()
{
    StopAuto();
    m_engine.ClearTimeState();  // 显式清空时间状态(m_decoratorState + frameTime)
    m_engine.Reset();
    if (m_pRoot) m_engine.ResetNodeStates(m_pRoot);
    PopulateTreeView();
    ListView_DeleteAllItems(m_hPropList);
    SetLogText(L"");
    UpdateStatusBar();
}

// ============================================================================
// 保存 XML
// ============================================================================
void CBTDebugger::SaveXMLFile()
{
    if (!m_pRoot) return;

    std::wstring savePath;
    if (m_currentFile.empty() || m_currentFile.find(L"(内置") == 0)
    {
        OPENFILENAMEW ofn = {};
        wchar_t szFile[260] = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = m_hWnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = 260;
        ofn.lpstrFilter = L"XML 行为树文件\0*.xml\0所有文件\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
        ofn.lpstrDefExt = L"xml";

        if (!GetSaveFileNameW(&ofn)) return;
        savePath = szFile;
        m_currentFile = savePath;
    }
    else
    {
        savePath = m_currentFile;
    }

    std::wstring treeName = m_pRoot->name;
    std::string xml = XMLParser::SerializeTree(m_pRoot, treeName);
    if (xml.empty())
    {
        MessageBoxW(m_hWnd, L"序列化失败！", L"错误", MB_ICONERROR);
        return;
    }

    std::ofstream file(savePath, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        MessageBoxW(m_hWnd, L"无法写入文件！", L"错误", MB_ICONERROR);
        return;
    }
    file.write(xml.c_str(), xml.size());
    file.close();

    wchar_t buf[300];
    wsprintfW(buf, L"已保存到: %s", savePath.c_str());
    SetLogText(buf);
    UpdateStatusBar();
}

// ============================================================================
// 节点编辑功能
// ============================================================================
std::shared_ptr<BTNode> CBTDebugger::GetSelectedNode()
{
    if (m_selectedNodeId.empty() || !m_pRoot) return nullptr;
    return FindNodeById(m_pRoot, m_selectedNodeId);
}

void CBTDebugger::OnTreeContextMenu(POINT pt)
{
    if (!m_pRoot) return;

    // 获取右键点击位置的树节点
    TVHITTESTINFO ht = {};
    ht.pt = pt;
    ScreenToClient(m_hTreeView, &ht.pt);
    TreeView_HitTest(m_hTreeView, &ht);

    if (ht.flags & TVHT_ONITEM)
    {
        // 先选中该节点
        TreeView_SelectItem(m_hTreeView, ht.hItem);

        // 更新选中状态
        TVITEMW item = {};
        item.hItem = ht.hItem;
        item.mask = TVIF_PARAM;
        TreeView_GetItem(m_hTreeView, &item);
        if (item.lParam)
        {
            m_selectedNodeId = ((BTNode*)item.lParam)->id;
            UpdatePropertyPanel();
        }
    }

    // 创建右键菜单
    HMENU hMenu = CreatePopupMenu();
    AppendMenuW(hMenu, MF_STRING, IDM_RENAME_NODE, L"重命名");
    AppendMenuW(hMenu, MF_STRING, IDM_ADD_CHILD,   L"添加子节点");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenu, MF_STRING, IDM_DELETE_NODE, L"删除节点");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenu, MF_STRING, IDM_MOVE_UP,     L"上移");
    AppendMenuW(hMenu, MF_STRING, IDM_MOVE_DOWN,   L"下移");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hMenu, MF_STRING, IDM_EDIT_PROPS,  L"编辑属性...");
    AppendMenuW(hMenu, MF_STRING, IDM_CHANGE_TYPE, L"更改类型...");

    // 如果没有选中节点，禁用编辑菜单
    if (m_selectedNodeId.empty())
    {
        EnableMenuItem(hMenu, IDM_RENAME_NODE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_ADD_CHILD,   MF_GRAYED);
        EnableMenuItem(hMenu, IDM_DELETE_NODE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_MOVE_UP,     MF_GRAYED);
        EnableMenuItem(hMenu, IDM_MOVE_DOWN,   MF_GRAYED);
    }

    TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, nullptr);
    DestroyMenu(hMenu);
}

void CBTDebugger::RenameSelectedNode()
{
    auto node = GetSelectedNode();
    if (!node) return;

    // 使用 TreeView 内置的编辑标签功能
    HTREEITEM hItem = TreeView_GetSelection(m_hTreeView);
    if (hItem)
        TreeView_EditLabel(m_hTreeView, hItem);
}

void CBTDebugger::AddChildToSelectedNode()
{
    auto parent = GetSelectedNode();
    if (!parent) return;

    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"BTDlgFrame", L"添加子节点",
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        0, 0, 380, 180, m_hWnd, nullptr, m_hInstance, nullptr);
    if (!hDlg) return;

    int xM = 12, y = 10, w = 356, rowH = 26;
    CreateWindowW(L"STATIC", L"节点类型:", WS_CHILD | WS_VISIBLE,
        xM, y, 75, 20, hDlg, nullptr, nullptr, nullptr);
    HWND hType = CreateWindowW(L"COMBOBOX", L"",
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
        xM + 80, y, w - xM - 80, 200, hDlg, (HMENU)100, nullptr, nullptr);

    // 节点类型列表来自 GetNodeCatalog()，覆盖 GameServer 全部 54 种节点
    // 保证调试器可选类型与服务端 CreateNodeByTypeName 工厂表完全一致
    const auto& catalog = GetNodeCatalog();
    for (const auto& e : catalog)
        SendMessageW(hType, CB_ADDSTRING, 0, (LPARAM)e.displayName);
    SendMessageW(hType, CB_SETCURSEL, 0, 0);

    y += rowH + 4;
    CreateWindowW(L"STATIC", L"节点名称:", WS_CHILD | WS_VISIBLE,
        xM, y, 75, 20, hDlg, nullptr, nullptr, nullptr);
    HWND hName = CreateWindowW(L"EDIT", L"新节点",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        xM + 80, y, w - xM - 80, 22, hDlg, nullptr, nullptr, nullptr);

    y += rowH + 12;
    CreateWindowW(L"BUTTON", L"确定", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        200, y, 80, 26, hDlg, (HMENU)IDOK, nullptr, nullptr);
    CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE,
        285, y, 80, 26, hDlg, (HMENU)IDCANCEL, nullptr, nullptr);

    RECT wr;
    GetWindowRect(m_hWnd, &wr);
    SetWindowPos(hDlg, nullptr, wr.left + (wr.right - wr.left - 380) / 2,
        wr.top + (wr.bottom - wr.top - 180) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    EnableWindow(m_hWnd, FALSE);
    MSG msg2;
    while (GetMessage(&msg2, nullptr, 0, 0))
    {
        if (msg2.message == WM_USER + 100)
        {
            WORD id = LOWORD(msg2.wParam);
            if (id == IDOK)
            {
                int sel = (int)SendMessageW(hType, CB_GETCURSEL, 0, 0);
                if (sel >= 0 && sel < (int)catalog.size())
                {
                    wchar_t nameBuf[128] = {};
                    GetWindowTextW(hName, nameBuf, 128);

                    auto child = std::make_shared<BTNode>();
                    child->name = nameBuf;
                    child->parent = parent.get();
                    child->depth = parent->depth + 1;

                    // 直接从目录取节点三元组，无需手动维护索引映射
                    const auto& e = catalog[sel];
                    child->type = e.type;
                    child->conditionType = e.ct;
                    child->actionType = e.at;
                    child->id = std::to_wstring(child->depth) + L"_" + GetTagName(child->type, child->conditionType, child->actionType) + L"_" + std::to_wstring((size_t)child.get());
                    child->params = GetDefaultParams(child->type, child->conditionType, child->actionType);

                    parent->children.push_back(child);
                    PopulateTreeView();
                    AppendLogText(L"[编辑] 已在 '" + parent->name + L"' 下添加子节点 '" + child->name + L"'\r\n");
                }
                DestroyWindow(hDlg);
                EnableWindow(m_hWnd, TRUE);
                SetFocus(m_hWnd);
                break;
            }
            else if (id == IDCANCEL)
            {
                DestroyWindow(hDlg);
                EnableWindow(m_hWnd, TRUE);
                SetFocus(m_hWnd);
                break;
            }
        }
        else if (msg2.message == WM_USER + 101)
        {
            DestroyWindow(hDlg);
            EnableWindow(m_hWnd, TRUE);
            SetFocus(m_hWnd);
            break;
        }
        if (!IsWindow(hDlg)) break;
        if (!IsDialogMessageW(hDlg, &msg2))
        {
            TranslateMessage(&msg2);
            DispatchMessage(&msg2);
        }
    }
}

void CBTDebugger::EditNodeProperties()
{
    auto node = GetSelectedNode();
    if (!node) return;

    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"BTDlgFrame", L"",
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        0, 0, 400, 270, m_hWnd, nullptr, m_hInstance, nullptr);
    if (!hDlg) return;

    wchar_t title[256];
    wsprintfW(title, L"编辑属性 - %s", node->name.c_str());
    SetWindowTextW(hDlg, title);

    int xM = 12, y = 10, w = 376;
    CreateWindowW(L"STATIC", L"参数 (每行一个, 格式: key=value):",
        WS_CHILD | WS_VISIBLE, xM, y, w - xM, 16, hDlg, nullptr, nullptr, nullptr);

    y += 20;
    std::wstring params;
    for (auto& p : node->params)
        params += p.first + L"=" + p.second + L"\r\n";

    HWND hParams = CreateWindowW(L"EDIT", params.c_str(),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        xM, y, w - xM, 150, hDlg, nullptr, nullptr, nullptr);

    y += 158;
    CreateWindowW(L"BUTTON", L"确定", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        w - 180, y, 80, 26, hDlg, (HMENU)IDOK, nullptr, nullptr);
    CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE,
        w - 90, y, 80, 26, hDlg, (HMENU)IDCANCEL, nullptr, nullptr);

    // 居中
    RECT wr;
    GetWindowRect(m_hWnd, &wr);
    SetWindowPos(hDlg, nullptr, wr.left + (wr.right - wr.left - 400) / 2,
        wr.top + (wr.bottom - wr.top - 270) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    EnableWindow(m_hWnd, FALSE);
    MSG msg2;
    while (GetMessage(&msg2, nullptr, 0, 0))
    {
        if (msg2.message == WM_USER + 100)
        {
            WORD id = LOWORD(msg2.wParam);
            if (id == IDOK)
            {
                int len = GetWindowTextLengthW(hParams);
                std::wstring text(len + 1, L'\0');
                GetWindowTextW(hParams, &text[0], len + 1);
                text.resize(len);

                node->params.clear();
                size_t pos = 0;
                while (pos < text.length())
                {
                    size_t eol = text.find(L'\n', pos);
                    if (eol == std::wstring::npos) eol = text.length();
                    std::wstring line = text.substr(pos, eol - pos);
                    while (!line.empty() && line.back() == L'\r') line.pop_back();
                    pos = eol + 1;
                    if (line.empty()) continue;
                    size_t eq = line.find(L'=');
                    if (eq != std::wstring::npos)
                        node->params[line.substr(0, eq)] = line.substr(eq + 1);
                }

                UpdatePropertyPanel();
                AppendLogText(L"[编辑] 已更新节点 '" + node->name + L"' 的属性\r\n");
                DestroyWindow(hDlg);
                EnableWindow(m_hWnd, TRUE);
                SetFocus(m_hWnd);
                break;
            }
            else if (id == IDCANCEL)
            {
                DestroyWindow(hDlg);
                EnableWindow(m_hWnd, TRUE);
                SetFocus(m_hWnd);
                break;
            }
        }
        else if (msg2.message == WM_USER + 101)
        {
            DestroyWindow(hDlg);
            EnableWindow(m_hWnd, TRUE);
            SetFocus(m_hWnd);
            break;
        }
        if (!IsWindow(hDlg)) break;
        if (!IsDialogMessageW(hDlg, &msg2))
        {
            TranslateMessage(&msg2);
            DispatchMessage(&msg2);
        }
    }
}

void CBTDebugger::OnPropListDoubleClick()
{
    EditNodeProperties();
}

void CBTDebugger::ChangeNodeType()
{
    auto node = GetSelectedNode();
    if (!node) return;

    HWND hDlg = CreateWindowExW(WS_EX_DLGMODALFRAME, L"BTDlgFrame", L"更改节点类型",
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        0, 0, 380, 130, m_hWnd, nullptr, m_hInstance, nullptr);
    if (!hDlg) return;

    int xM = 12, y = 10, w = 356;
    CreateWindowW(L"STATIC", L"选择新类型:",
        WS_CHILD | WS_VISIBLE, xM, y, 85, 20, hDlg, nullptr, nullptr, nullptr);
    HWND hType = CreateWindowW(L"COMBOBOX", L"",
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
        xM + 90, y, w - xM - 90, 200, hDlg, (HMENU)100, nullptr, nullptr);

    // 节点类型列表来自 GetNodeCatalog()，覆盖 GameServer 全部 54 种节点
    const auto& catalog = GetNodeCatalog();
    for (const auto& e : catalog)
        SendMessageW(hType, CB_ADDSTRING, 0, (LPARAM)e.displayName);

    // 根据当前节点类型预选（用 FindCatalogIndex 查找目录索引）
    int preSel = FindCatalogIndex(node->type, node->conditionType, node->actionType);
    if (preSel < 0) preSel = 0;
    SendMessageW(hType, CB_SETCURSEL, preSel, 0);

    y = 50;
    CreateWindowW(L"BUTTON", L"确定", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        w - 180, y, 80, 26, hDlg, (HMENU)IDOK, nullptr, nullptr);
    CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE,
        w - 90, y, 80, 26, hDlg, (HMENU)IDCANCEL, nullptr, nullptr);

    RECT wr;
    GetWindowRect(m_hWnd, &wr);
    SetWindowPos(hDlg, nullptr, wr.left + (wr.right - wr.left - 380) / 2,
        wr.top + (wr.bottom - wr.top - 130) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    EnableWindow(m_hWnd, FALSE);
    MSG msg2;
    while (GetMessage(&msg2, nullptr, 0, 0))
    {
        if (msg2.message == WM_USER + 100)
        {
            WORD id = LOWORD(msg2.wParam);
            if (id == IDOK)
            {
                int sel = (int)SendMessageW(hType, CB_GETCURSEL, 0, 0);
                if (sel >= 0 && sel < (int)catalog.size())
                {
                    // 直接从目录取节点三元组，无需手动维护索引映射
                    const auto& e = catalog[sel];
                    node->type = e.type;
                    node->conditionType = e.ct;
                    node->actionType = e.at;
                    node->id = std::to_wstring(node->depth) + L"_" + GetTagName(node->type, node->conditionType, node->actionType) + L"_" + std::to_wstring((size_t)node.get());
                    // 合并默认参数（保留用户已设置的值）
                    auto defParams = GetDefaultParams(node->type, node->conditionType, node->actionType);
                    for (auto& dp : defParams)
                        if (node->params.find(dp.first) == node->params.end())
                            node->params[dp.first] = dp.second;
                    m_selectedNodeId = node->id;

                    PopulateTreeView();
                    // 恢复树视图选中状态
                    HTREEITEM hItem = TreeView_GetRoot(m_hTreeView);
                    while (hItem)
                    {
                        TVITEMW tv = {}; tv.hItem = hItem; tv.mask = TVIF_PARAM;
                        TreeView_GetItem(m_hTreeView, &tv);
                        if ((BTNode*)tv.lParam == node.get()) { TreeView_SelectItem(m_hTreeView, hItem); break; }
                        hItem = TreeView_GetNextItem(m_hTreeView, hItem, TVGN_NEXTVISIBLE);
                    }
                    UpdatePropertyPanel();
                    AppendLogText(L"[编辑] 节点 '" + node->name + L"' 类型已更改\r\n");
                }
                DestroyWindow(hDlg);
                EnableWindow(m_hWnd, TRUE);
                SetFocus(m_hWnd);
                break;
            }
            else if (id == IDCANCEL)
            {
                DestroyWindow(hDlg);
                EnableWindow(m_hWnd, TRUE);
                SetFocus(m_hWnd);
                break;
            }
        }
        else if (msg2.message == WM_USER + 101)
        {
            DestroyWindow(hDlg);
            EnableWindow(m_hWnd, TRUE);
            SetFocus(m_hWnd);
            break;
        }
        if (!IsWindow(hDlg)) break;
        if (!IsDialogMessageW(hDlg, &msg2))
        {
            TranslateMessage(&msg2);
            DispatchMessage(&msg2);
        }
    }
}

void CBTDebugger::DeleteSelectedNode()
{
    auto node = GetSelectedNode();
    if (!node || !node->parent) return; // 不能删除根节点

    int result = MessageBoxW(m_hWnd,
        (L"确定要删除节点 '" + node->name + L"' 及其所有子节点吗？\n此操作不可撤销！").c_str(),
        L"删除节点", MB_YESNO | MB_ICONWARNING);

    if (result != IDYES) return;

    auto parent = node->parent;
    auto& siblings = parent->children;
    siblings.erase(std::remove_if(siblings.begin(), siblings.end(),
        [&](std::shared_ptr<BTNode>& c) { return c == node; }), siblings.end());

    m_selectedNodeId.clear();
    PopulateTreeView();
    UpdatePropertyPanel();
    UpdateStatusBar();
    AppendLogText(L"[编辑] 已删除节点 '" + node->name + L"'\r\n");
}

void CBTDebugger::MoveSelectedNodeUp()
{
    auto node = GetSelectedNode();
    if (!node || !node->parent) return;

    auto& siblings = node->parent->children;
    for (size_t i = 1; i < siblings.size(); i++)
    {
        if (siblings[i] == node)
        {
            std::swap(siblings[i], siblings[i - 1]);
            PopulateTreeView();
            break;
        }
    }
}

void CBTDebugger::MoveSelectedNodeDown()
{
    auto node = GetSelectedNode();
    if (!node || !node->parent) return;

    auto& siblings = node->parent->children;
    for (size_t i = 0; i + 1 < siblings.size(); i++)
    {
        if (siblings[i] == node)
        {
            std::swap(siblings[i], siblings[i + 1]);
            PopulateTreeView();
            break;
        }
    }
}

void CBTDebugger::UpdatePropertyPanel()
{
    ListView_DeleteAllItems(m_hPropList);

    if (m_selectedNodeId.empty() || !m_pRoot) return;

    auto node = FindNodeById(m_pRoot, m_selectedNodeId);
    if (!node) return;

    auto addRow = [&](const wchar_t* key, const std::wstring& val) {
        LVITEMW lvi = {};
        lvi.mask = LVIF_TEXT;
        lvi.pszText = (LPWSTR)key;
        int idx = ListView_InsertItem(m_hPropList, &lvi);
        ListView_SetItemText(m_hPropList, idx, 1, (LPWSTR)val.c_str());
    };

    addRow(L"节点名称", node->name);
    addRow(L"节点类型", GetNodeTypeDetail(node->type, node->conditionType, node->actionType));
    addRow(L"节点分类", GetNodeCategory(node->type) == BTNodeCategory::COMPOSITE ? L"复合节点" :
        GetNodeCategory(node->type) == BTNodeCategory::DECORATOR ? L"装饰节点" :
        GetNodeCategory(node->type) == BTNodeCategory::CONDITION ? L"条件节点" : L"动作节点");
    addRow(L"深度", std::to_wstring(node->depth));
    addRow(L"子节点数", std::to_wstring(node->children.size()));
    addRow(L"结果", GetResultName(node->lastResult));
    addRow(L"执行顺序", node->execOrder >= 0 ? std::to_wstring(node->execOrder) : L"未执行");

    // 参数
    for (auto& param : node->params)
    {
        addRow(param.first.c_str(), param.second);
    }
}

void CBTDebugger::UpdateLogPanel()
{
    ListView_DeleteAllItems(m_hLogEdit);
    auto& logs = m_engine.GetLogs();

    int row = 0;
    for (size_t i = 0; i < logs.size(); i++)
    {
        auto& log = logs[i];

        // 按 m_logFilter 过滤
        if (m_logFilter == LOG_FILTER_FAIL    && log.result != BTResult::FAILURE) continue;
        if (m_logFilter == LOG_FILTER_SUCCESS && log.result != BTResult::SUCCESS) continue;
        if (m_logFilter == LOG_FILTER_RUNNING && log.result != BTResult::RUNNING) continue;

        wchar_t idxBuf[16];
        wsprintfW(idxBuf, L"%d", (int)(i + 1));

        LVITEMW lvi = {};
        lvi.mask = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem = row;
        lvi.iSubItem = 0;
        lvi.pszText = idxBuf;
        lvi.lParam = (LPARAM)i;   // 保存日志索引,供双击跳转
        int ins = ListView_InsertItem(m_hLogEdit, &lvi);

        ListView_SetItemText(m_hLogEdit, ins, 1, (LPWSTR)GetResultName(log.result));
        ListView_SetItemText(m_hLogEdit, ins, 2, (LPWSTR)log.nodeName.c_str());
        ListView_SetItemText(m_hLogEdit, ins, 3, (LPWSTR)GetNodeTypeName(log.type));
        row++;
    }
}

void CBTDebugger::UpdateStatusBar()
{
    if (!m_pRoot)
    {
        SendMessageW(m_hStatusBar, SB_SETTEXTW, 0, (LPARAM)L"就绪 - 请加载行为树文件");
        return;
    }

    wchar_t buf[256];
    int totalNodes = CountNodes(m_pRoot);
    int logCount = (int)m_engine.GetLogs().size();
    wsprintfW(buf, L"节点总数: %d | 日志条数: %d | 速度: %dms %s",
        totalNodes, logCount, m_autoSpeed,
        m_isAutoRunning ? L"(自动运行中)" : L"");
    SendMessageW(m_hStatusBar, SB_SETTEXTW, 0, (LPARAM)buf);
}

// ============================================================================
// 游戏状态面板：ExecutionContext 同步
// SyncContextToUI: 从引擎上下文同步到 Edit 控件（加载行为树后调用）
// SyncContextFromUI: 从 Edit 控件回写到引擎上下文（用户点击"应用状态"后调用）
// ============================================================================
void CBTDebugger::SyncContextToUI()
{
	auto& ctx = m_engine.GetContext();
	wchar_t buf[32];
	wsprintfW(buf, L"%d", ctx.hpPercent);    SetWindowTextW(m_hCtxHP, buf);
	wsprintfW(buf, L"%d", ctx.mpPercent);    SetWindowTextW(m_hCtxMP, buf);
	wsprintfW(buf, L"%d", ctx.isDead ? 1 : 0);      SetWindowTextW(m_hCtxIsDead, buf);
	wsprintfW(buf, L"%d", ctx.inSafeArea ? 1 : 0);  SetWindowTextW(m_hCtxSafeArea, buf);
	wsprintfW(buf, L"%d", ctx.hasTarget ? 1 : 0);   SetWindowTextW(m_hCtxHasTarget, buf);
	wsprintfW(buf, L"%d", ctx.targetDistance);      SetWindowTextW(m_hCtxTargetDist, buf);
	wsprintfW(buf, L"%d", ctx.bagFull ? 1 : 0);     SetWindowTextW(m_hCtxBagFull, buf);
	wsprintfW(buf, L"%d", ctx.monsterCount);        SetWindowTextW(m_hCtxMonsterCount, buf);
}

void CBTDebugger::SyncContextFromUI()
{
	auto& ctx = m_engine.GetContext();
	auto getInt = [](HWND h) -> int {
		wchar_t b[32] = {};
		GetWindowTextW(h, b, 32);
		return _wtoi(b);
	};
	auto getBool = [](HWND h) -> bool {
		wchar_t b[32] = {};
		GetWindowTextW(h, b, 32);
		return _wtoi(b) != 0;
	};
	ctx.hpPercent     = getInt(m_hCtxHP);
	ctx.mpPercent     = getInt(m_hCtxMP);
	ctx.isDead        = getBool(m_hCtxIsDead);
	ctx.inSafeArea    = getBool(m_hCtxSafeArea);
	ctx.hasTarget     = getBool(m_hCtxHasTarget);
	ctx.targetDistance= getInt(m_hCtxTargetDist);
	ctx.bagFull       = getBool(m_hCtxBagFull);
	ctx.monsterCount  = getInt(m_hCtxMonsterCount);
	// 边界保护（防止用户输入非法值导致条件判定异常）
	if (ctx.hpPercent < 0) ctx.hpPercent = 0;
	if (ctx.hpPercent > 100) ctx.hpPercent = 100;
	if (ctx.mpPercent < 0) ctx.mpPercent = 0;
	if (ctx.mpPercent > 100) ctx.mpPercent = 100;
	if (ctx.targetDistance < 0) ctx.targetDistance = 0;
	if (ctx.monsterCount < 0) ctx.monsterCount = 0;
}

void CBTDebugger::OnContextApply()
{
	SyncContextFromUI();
	auto& ctx = m_engine.GetContext();
	AppendLogText(L"[上下文] 游戏状态已更新: HP=" + std::to_wstring(ctx.hpPercent) +
		L"% MP=" + std::to_wstring(ctx.mpPercent) +
		L"% 死亡=" + std::to_wstring(ctx.isDead ? 1 : 0) +
		L" 安全区=" + std::to_wstring(ctx.inSafeArea ? 1 : 0) +
		L" 有目标=" + std::to_wstring(ctx.hasTarget ? 1 : 0) +
		L" 距离=" + std::to_wstring(ctx.targetDistance) +
		L" 包满=" + std::to_wstring(ctx.bagFull ? 1 : 0) +
		L" 怪数=" + std::to_wstring(ctx.monsterCount) + L"\r\n");
}

void CBTDebugger::SetLogText(const std::wstring& text)
{
    // 日志面板已改为 ListView: 空串表示清空, 非空串作为一条消息追加
    if (text.empty())
        ListView_DeleteAllItems(m_hLogEdit);
    else
        AppendLogText(text);
}

void CBTDebugger::AppendLogText(const std::wstring& text)
{
    // 去除尾部换行符
    std::wstring msg = text;
    while (!msg.empty() && (msg.back() == L'\r' || msg.back() == L'\n')) msg.pop_back();
    if (msg.empty()) return;

    int row = ListView_GetItemCount(m_hLogEdit);
    wchar_t idxBuf[16];
    wsprintfW(idxBuf, L"%d", row + 1);

    LVITEMW lvi = {};
    lvi.mask = LVIF_TEXT | LVIF_PARAM;
    lvi.iItem = row;
    lvi.iSubItem = 0;
    lvi.pszText = idxBuf;
    lvi.lParam = -1;   // 提示类消息,不可双击跳转
    int ins = ListView_InsertItem(m_hLogEdit, &lvi);
    ListView_SetItemText(m_hLogEdit, ins, 1, (LPWSTR)L"消息");
    ListView_SetItemText(m_hLogEdit, ins, 2, (LPWSTR)msg.c_str());
    ListView_SetItemText(m_hLogEdit, ins, 3, (LPWSTR)L"提示");
    ListView_EnsureVisible(m_hLogEdit, ins, FALSE);
}

std::string CBTDebugger::WStringToUTF8(const std::wstring& wstr)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(len - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], len, nullptr, nullptr);
    return result;
}

std::wstring CBTDebugger::UTF8ToWString(const std::string& str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring result(len - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], len);
    return result;
}

// ============================================================================
// 增强功能实现
// ============================================================================

// 初始化日志 ListView 的列: 序号 / 结果 / 节点名 / 类型
void CBTDebugger::InitLogListColumns()
{
    LVCOLUMNW lvc = {};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.cx = 40;  lvc.pszText = (LPWSTR)L"序号";   ListView_InsertColumn(m_hLogEdit, 0, &lvc);
    lvc.cx = 60;  lvc.pszText = (LPWSTR)L"结果";   ListView_InsertColumn(m_hLogEdit, 1, &lvc);
    lvc.cx = 180; lvc.pszText = (LPWSTR)L"节点名"; ListView_InsertColumn(m_hLogEdit, 2, &lvc);
    lvc.cx = 120; lvc.pszText = (LPWSTR)L"类型";   ListView_InsertColumn(m_hLogEdit, 3, &lvc);
}

// WM_SETCURSOR: 在分隔条上方显示对应的调整光标
LRESULT CBTDebugger::OnSetCursor(HWND hWnd, WORD hitCode)
{
    if (hitCode != HTCLIENT) return FALSE;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);

    // 垂直分隔条 → 左右调整光标
    if (pt.x >= m_splitterPos && pt.x <= m_splitterPos + 4 && pt.y > 44)
    {
        SetCursor(LoadCursor(nullptr, IDC_SIZEWE));
        return TRUE;
    }
    // 水平分隔条 → 上下调整光标
    int hSplitY = 44 + m_rightSplitterPos;
    if (pt.y >= hSplitY && pt.y <= hSplitY + 4 && pt.x > m_splitterPos + 4)
    {
        SetCursor(LoadCursor(nullptr, IDC_SIZENS));
        return TRUE;
    }
    return FALSE;
}

// 日志 ListView 双击: 跳转到对应树节点
void CBTDebugger::OnLogListDoubleClick()
{
    int sel = ListView_GetNextItem(m_hLogEdit, -1, LVNI_SELECTED);
    if (sel < 0) return;

    LVITEMW lvi = {};
    lvi.iItem = sel;
    lvi.iSubItem = 0;
    lvi.mask = LVIF_PARAM;
    if (!ListView_GetItem(m_hLogEdit, &lvi)) return;

    // lParam 存的是日志索引(诊断/提示条目为 -1,不可跳转)
    LPARAM lp = lvi.lParam;
    if (lp < 0) return;
    size_t logIdx = (size_t)lp;
    auto& logs = m_engine.GetLogs();
    if (logIdx >= logs.size()) return;

    JumpToNode(logs[logIdx].nodeName);
}

// 跳转到指定节点名: 递归查找并选中/滚动可见
bool CBTDebugger::JumpToNode(const std::wstring& nodeName)
{
    if (nodeName.empty()) return false;
    HTREEITEM hRoot = TreeView_GetRoot(m_hTreeView);
    HTREEITEM hFound = FindTreeItemByName(m_hTreeView, hRoot, nodeName);
    if (hFound)
    {
        TreeView_SelectItem(m_hTreeView, hFound);
        TreeView_EnsureVisible(m_hTreeView, hFound);
        // 同步选中节点的属性面板
        TVITEMW item = {};
        item.hItem = hFound;
        item.mask = TVIF_PARAM;
        if (TreeView_GetItem(m_hTreeView, &item) && item.lParam)
        {
            m_selectedNodeId = ((BTNode*)item.lParam)->id;
            UpdatePropertyPanel();
        }
        return true;
    }
    return false;
}

// 行为树诊断: 检查结构错误并输出到日志 ListView
void CBTDebugger::DiagnoseTree()
{
    if (!m_pRoot)
    {
        MessageBoxW(m_hWnd, L"请先加载行为树！", L"诊断", MB_ICONINFORMATION);
        return;
    }

    // 清空日志 ListView,设置过滤为 ALL 以显示全部诊断结果
    ListView_DeleteAllItems(m_hLogEdit);
    m_logFilter = LOG_FILTER_ALL;
    std::wstring ftxt = L"过滤:" + std::wstring(GetLogFilterName(m_logFilter));
    SetWindowTextW(g_hBtnLogFilter, ftxt.c_str());

    std::vector<std::wstring> issues;
    DiagnoseRecursive(m_pRoot, issues);

    // 根节点应为复合或装饰节点
    BTNodeCategory rootCat = GetNodeCategory(m_pRoot->type);
    if (rootCat != BTNodeCategory::COMPOSITE && rootCat != BTNodeCategory::DECORATOR)
    {
        issues.insert(issues.begin(),
            L"根节点应为复合或装饰节点（当前: " + std::wstring(GetNodeTypeName(m_pRoot->type)) + L"）");
    }

    // 输出到日志 ListView: 结果列="诊断", 类型列="问题"
    if (issues.empty())
    {
        LVITEMW lvi = {};
        lvi.mask = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem = 0; lvi.iSubItem = 0;
        lvi.pszText = (LPWSTR)L"OK";
        lvi.lParam = -1;
        int ins = ListView_InsertItem(m_hLogEdit, &lvi);
        ListView_SetItemText(m_hLogEdit, ins, 1, (LPWSTR)L"诊断");
        ListView_SetItemText(m_hLogEdit, ins, 2, (LPWSTR)L"行为树结构正常,未发现问题");
        ListView_SetItemText(m_hLogEdit, ins, 3, (LPWSTR)L"通过");
    }
    else
    {
        for (size_t i = 0; i < issues.size(); i++)
        {
            wchar_t idxBuf[16];
            wsprintfW(idxBuf, L"%d", (int)(i + 1));
            LVITEMW lvi = {};
            lvi.mask = LVIF_TEXT | LVIF_PARAM;
            lvi.iItem = (int)i; lvi.iSubItem = 0;
            lvi.pszText = idxBuf;
            lvi.lParam = -1;
            int ins = ListView_InsertItem(m_hLogEdit, &lvi);
            ListView_SetItemText(m_hLogEdit, ins, 1, (LPWSTR)L"诊断");
            ListView_SetItemText(m_hLogEdit, ins, 2, (LPWSTR)issues[i].c_str());
            ListView_SetItemText(m_hLogEdit, ins, 3, (LPWSTR)L"问题");
        }
    }

    wchar_t buf[128];
    wsprintfW(buf, L"诊断完成: 发现 %d 个问题", (int)issues.size());
    SendMessageW(m_hStatusBar, SB_SETTEXTW, 0, (LPARAM)buf);
}

// 递归诊断单个节点:
//  1. 复合节点无子节点
//  2. 装饰节点子节点数≠1
//  3. 叶子节点(条件/动作)有子节点
//  4. 参数缺失(对照 GetDefaultParams)
void CBTDebugger::DiagnoseRecursive(std::shared_ptr<BTNode> node, std::vector<std::wstring>& issues)
{
    if (!node) return;

    BTNodeCategory cat = GetNodeCategory(node->type);
    size_t childCount = node->children.size();

    if (cat == BTNodeCategory::COMPOSITE)
    {
        if (childCount == 0)
            issues.push_back(L"[" + node->name + L"] 复合节点无子节点,无法执行");
    }
    else if (cat == BTNodeCategory::DECORATOR)
    {
        if (childCount != 1)
            issues.push_back(L"[" + node->name + L"] 装饰节点必须恰好1个子节点(当前:" + std::to_wstring((int)childCount) + L")");
    }
    else if (cat == BTNodeCategory::CONDITION || cat == BTNodeCategory::ACTION)
    {
        if (childCount > 0)
            issues.push_back(L"[" + node->name + L"] 叶子节点不应有子节点(当前:" + std::to_wstring((int)childCount) + L")");
    }

    // 参数缺失检查: 对照默认参数集
    auto defaultParams = GetDefaultParams(node->type, node->conditionType, node->actionType);
    for (const auto& dp : defaultParams)
    {
        if (node->params.find(dp.first) == node->params.end())
            issues.push_back(L"[" + node->name + L"] 缺少参数:" + dp.first);
    }

    // 递归检查所有子节点
    for (auto& child : node->children)
        DiagnoseRecursive(child, issues);
}

// 获取时间相关节点的剩余时间描述(供树节点显示)
std::wstring CBTDebugger::GetNodeRemainingTimeImpl(BTNode* pNode)
{
    if (!pNode) return L"";
    auto& ctx = m_engine.GetContext();
    DWORD dwNow = ctx.frameTime;
    auto& dstate = m_engine.GetDecoratorState();
    auto getRemain = [&](const std::wstring& keyPrefix, int total) -> std::wstring {
        std::wstring key = pNode->id + keyPrefix;
        auto it = dstate.find(key);
        if (it == dstate.end()) return L"";
        DWORD elapsed = (dwNow >= it->second) ? (dwNow - it->second) : 0;
        int remain = total - (int)elapsed;
        if (remain < 0) remain = 0;
        wchar_t buf[32];
        wsprintfW(buf, L"剩余:%dms", remain);
        return buf;
        };
    auto getParam = [&](const std::wstring& k, int def) -> int {
        auto it = pNode->params.find(k);
        return it != pNode->params.end() ? _wtoi(it->second.c_str()) : def;
        };
    switch (pNode->type)
    {
    case BTNodeType::ACTION:
        switch (pNode->actionType)
        {
        case ActionType::REST:   return getRemain(L"_REST_START", getParam(L"duration", 5000));
        case ActionType::REVIVE: return getRemain(L"_REVIVE_START", getParam(L"delay", 0));
        case ActionType::MINE:   return getRemain(L"_MINE_START", getParam(L"duration", 10000));
        case ActionType::RECALL: return getRemain(L"_RECALL_START", 1000);
        case ActionType::DELAY:
        {
            std::wstring key = pNode->id + L"_DELAY_START";
            auto it = dstate.find(key);
            if (it == dstate.end()) return L"";
            auto tit = dstate.find(key + L"_T");
            DWORD target = (tit != dstate.end()) ? tit->second : 1000;
            DWORD elapsed = (dwNow >= it->second) ? (dwNow - it->second) : 0;
            int remain = (int)target - (int)elapsed;
            if (remain < 0) remain = 0;
            wchar_t buf[32];
            wsprintfW(buf, L"剩余:%dms", remain);
            return buf;
        }
        default: return L"";
        }
    case BTNodeType::DECORATOR_COOLDOWN: return getRemain(L"_CD_LAST", getParam(L"ms", 3000));
    case BTNodeType::DECORATOR_TIMEOUT:  return getRemain(L"_TO_START", getParam(L"ms", 5000));
    default: return L"";
    }
}

// 搜索: 取搜索框文本并跳转
void CBTDebugger::OnSearch()
{
    wchar_t buf[256] = {};
    GetWindowTextW(g_hEditSearch, buf, 256);
    std::wstring keyword(buf);
    if (keyword.empty()) return;
    if (!JumpToNode(keyword))
    {
        std::wstring msg = L"未找到节点: " + keyword;
        MessageBoxW(m_hWnd, msg.c_str(), L"查找", MB_ICONINFORMATION);
    }
}

LRESULT CALLBACK CBTDebugger::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto* pApp = s_pInstance;

    switch (msg)
    {
    case WM_CREATE:
    {
        // 创建右键菜单 - 对齐 GameServer 机器人配置备份目录下的真实行为树
        HMENU hMenu = CreatePopupMenu();
        AppendMenuW(hMenu, MF_STRING, 10001, L"加载战士行为树(Warrior)");
        AppendMenuW(hMenu, MF_STRING, 10002, L"加载法师行为树(Mage)");
        AppendMenuW(hMenu, MF_STRING, 10003, L"加载猎人行为树(Hunter)");
        AppendMenuW(hMenu, MF_STRING, 10004, L"加载真人模拟行为树(HumanLike)");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(hMenu, MF_STRING, 10005, L"打开XML文件...");
        SetPropW(hWnd, L"ContextMenu", hMenu);
        break;
    }

    case WM_CONTEXTMENU:
    {
        // 判断右键来源：TreeView 有自己的 NM_RCLICK 处理，这里只处理窗口空白区域的右键
        if ((HWND)wParam != pApp->m_hTreeView)
        {
            HMENU hMenu = (HMENU)GetPropW(hWnd, L"ContextMenu");
            if (hMenu)
            {
                POINT pt = { LOWORD(lParam), HIWORD(lParam) };
                TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, nullptr);
            }
        }
        break;
    }

    case WM_COMMAND:
    {
        WORD id = LOWORD(wParam);
        switch (id)
        {
        case 10001: pApp->LoadXMLFromString(g_builtInSamples.at(L"战士行为树(Warrior)")); break;
        case 10002: pApp->LoadXMLFromString(g_builtInSamples.at(L"法师行为树(Mage)")); break;
        case 10003: pApp->LoadXMLFromString(g_builtInSamples.at(L"猎人行为树(Hunter)")); break;
        case 10004: pApp->LoadXMLFromString(g_builtInSamples.at(L"真人模拟行为树(HumanLike)")); break;
        case 10005: pApp->LoadXMLFile(); break;
        // 节点编辑右键菜单
        case IDM_RENAME_NODE: pApp->RenameSelectedNode(); break;
        case IDM_ADD_CHILD:   pApp->AddChildToSelectedNode(); break;
        case IDM_DELETE_NODE: pApp->DeleteSelectedNode(); break;
        case IDM_MOVE_UP:     pApp->MoveSelectedNodeUp(); break;
        case IDM_MOVE_DOWN:   pApp->MoveSelectedNodeDown(); break;
        case IDM_EDIT_PROPS:  pApp->EditNodeProperties(); break;
        case IDM_CHANGE_TYPE: pApp->ChangeNodeType(); break;
        default: pApp->OnCommand(id); break;
        }
        break;
    }

    case WM_NOTIFY:
        // 直接返回 OnNotify 结果（NM_CUSTOMDRAW 需要原始 CDRF 值,不能折叠为 TRUE/FALSE）
        return pApp->OnNotify((NMHDR*)lParam);

    case WM_HSCROLL:
    {
        if ((HWND)lParam == pApp->m_hSpeedSlider)
        {
            pApp->m_autoSpeed = (int)SendMessage(pApp->m_hSpeedSlider, TBM_GETPOS, 0, 0);
            wchar_t buf[32];
            wsprintfW(buf, L"速度: %dms", pApp->m_autoSpeed);
            SetWindowTextW(pApp->m_hSpeedLabel, buf);
            // 如果正在自动运行，重新设置定时器速率
            if (pApp->m_isAutoRunning)
            {
                KillTimer(hWnd, ID_TIMER_AUTO);
                SetTimer(hWnd, ID_TIMER_AUTO, pApp->m_autoSpeed, nullptr);
            }
        }
        break;
    }

    case WM_TIMER:
        pApp->OnTimer(wParam);
        break;

    case WM_SIZE:
        pApp->OnSize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        pApp->OnPaint(hdc);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN:
        pApp->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    case WM_MOUSEMOVE:
        pApp->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    case WM_LBUTTONUP:
        pApp->OnLButtonUp();
        break;

    case WM_SETCURSOR:
        return (LRESULT)pApp->OnSetCursor((HWND)wParam, LOWORD(lParam));

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    {
        HDC hdc = (HDC)wParam;
        SetBkColor(hdc, RGB(15, 15, 30));
        SetTextColor(hdc, RGB(200, 200, 220));
        static HBRUSH hBrush = CreateSolidBrush(RGB(15, 15, 30));
        return (LRESULT)hBrush;
    }

    case WM_DESTROY:
    {
        HMENU hMenu = (HMENU)GetPropW(hWnd, L"ContextMenu");
        if (hMenu) DestroyMenu(hMenu);
        RemovePropW(hWnd, L"ContextMenu");
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int CBTDebugger::Run()
{
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// ============================================================================
// 程序入口
// ============================================================================
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    // 初始化公共控件
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);

    CBTDebugger app(hInstance);
    if (!app.Init(nCmdShow))
    {
        MessageBoxW(nullptr, L"窗口初始化失败！", L"错误", MB_ICONERROR);
        return 1;
    }

    return app.Run();
}