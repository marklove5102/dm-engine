#ifndef SDOA4CLIENTPATCH_H
#define SDOA4CLIENTPATCH_H

#pragma pack(push,1)

//-------------------------------------------------------------------------------------------
// 补丁接口增加让外部能设置 圈圈 操作界面逻辑屏幕大小

//-------------------------------------------------------------------------------------------
// IGW配置接口相关
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetSelfCursor)(const bool value);
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetSelfCursor)();

// 注：IGW屏幕包括接入后IGW所有显示的可视元素
// 获取IGW当前屏幕状态，值为上面定义的igwScreenNone、igwScreenNormal、igwScreenMini
typedef DECLSPEC_IMPORT int (WINAPI* PFNGetScreenStatus)();
// 设置屏幕状态参数value值为igwScreenNormal、igwScreenMini
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetScreenStatus)(const int value);
// 获取当前屏幕可用状态,false为不可用，true为可用
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetScreenEnabled)();
// 设置当前屏幕可用状态,false为不可用，true为可用
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetScreenEnabled)(const bool value);
// 获取当前TaskBar的位置 p不能为NULL
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetTaskBarPosition)(PPOINT p);
// 设置当前TaskBar的位置
typedef DECLSPEC_IMPORT bool (WINAPI* PFNSetTaskBarPosition)(const POINT p);
typedef DECLSPEC_IMPORT bool (WINAPI* PFNGetFocus)();
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetFocus)(const bool value);
// 判断当前坐标下是否有IGW的界面
typedef DECLSPEC_IMPORT bool (WINAPI* PFNHasUI)(const POINT p);
// 设置Bar展开模式
typedef DECLSPEC_IMPORT int (WINAPI* PFNGetTaskBarMode)();
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetTaskBarMode)(const int iBarMode);
// 设置逻辑屏幕大小
typedef DECLSPEC_IMPORT void (WINAPI* PFNSetLogicScreen)(const int sw, const int sh);
typedef DECLSPEC_IMPORT void (WINAPI* PFNGetLogicScreen)( int* sw, int* sh );

//-------------------------------------------------------------------------------------------
// IGW Configure
typedef struct _IGWConfigureInterface{
	PFNGetSelfCursor		GetSelfCursor;      // 得到当前IGW的鼠标指针绘制状态 true为自绘 false为不处理
	PFNSetSelfCursor		SetSelfCursor;      // 设置IGW鼠标绘制状态，参数同上
	PFNGetScreenStatus		GetScreenStatus;    // 获取IGW屏幕状态，值为上面定义的igwScreenNone、igwScreenNormal、igwScreenMini
	PFNSetScreenStatus		SetScreenStatus;    // 设置屏幕状态参数value值为igwScreenNormal、igwScreenMini
	PFNGetScreenEnabled		GetScreenEnabled;   // 获取屏幕可用状态
	PFNSetScreenEnabled		SetScreenEnabled;   // 设置屏幕可用状态
	PFNGetTaskBarPosition	GetTaskBarPosition; // 获取任务控制条的位置
	PFNSetTaskBarPosition	SetTaskBarPosition; // 设置任务控制条的位置
	PFNGetFocus				GetFocus;			// 获取当前IGW是否处于焦点状态true为焦点状态，false没有焦点状态
	PFNSetFocus				SetFocus;			// 设置IGW当前是来有焦点true为有焦点，false则无焦点
	PFNHasUI				HasUI;				// 判断指定坐标下是否有IGW界面
	PFNGetTaskBarMode		GetTaskBarMode;
	PFNSetTaskBarMode		SetTaskBarMode;
	PFNGetLogicScreen		GetLogicScreen;
	PFNSetLogicScreen		SetLogicScreen;
} IGWConfigureInterface, *PIGWConfigureInterface, *LPIGWConfigureInterface;

// IGW配置接口 (用于动态配置IGW的一些特性
#define igwConfigureInterface       0x0200

typedef DECLSPEC_IMPORT int (WINAPI* PFNigwSupportExW)(int nCode, void* FuncGroup, int FuncGroupSize);

#pragma pack(pop)
#endif /* SDOA4CLIENTPATCH_H */