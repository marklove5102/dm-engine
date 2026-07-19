//ControlDefine.h	控件各种定义
//

#pragma once

// 控件风格，使用SetStyle设置，风格是一个32bit无符号数
enum eCtrlStyle
{
	CTRL_STYLE_BACKMODE_NODRAW	=	0x00000001,			// 背景模式：不绘制
	CTRL_STYLE_BACKMODE_COLOR	=	0x00000002,			// 背景模式：颜色填充
	CTRL_STYLE_BACKMODE_TEX		=	0x00000004,			// 背景模式：显示图片
	CTRL_STYLE_ALIGN_LEFT		=	0x00000008,			// 对齐模式：左对齐
	CTRL_STYLE_ALIGN_RIGHT		=	0x00000010,			// 对齐模式：右对齐
	CTRL_STYLE_ALIGN_TOP		=	0x00000020, 		// 对齐模式：顶对齐
	CTRL_STYLE_ALIGN_BOTTOM		=	0x00000040,			// 对齐模式：底对齐
	CTRL_STYLE_ALIGN_CENTER		=	0x00000080,			// 对齐模式：中央对齐
	CTRL_STYLE_AUTOSIZE			=	0x00000100,			// 自动调整尺寸
	CTRL_STYLE_HORIZONTAL 		=	0x00000200,			// 横向
	CTRL_STYLE_VERTICAL			=	0x00000400,			// 纵向
	CTRL_STYLE_SELFDELETE_TEX	=	0x00000800,			// 控件被摧毁时，自动摧毁用到的纹理
	CTRL_STYLE_SELFDELETE_BACKTEX=	0x00001000,			// 控件被摧毁时，自动摧毁用到的背景纹理
	CTRL_STYLE_CLOSE_BUTTON		=	0x00002000,			// 控件窗口具有关闭按钮，只有窗口类和它的派生类可以使用这个风格
	CTRL_STYLE_TRANS			=	0x00004000,			// 窗口背景可以透明
	CTRL_STYLE_DEFAULT          =   0x00000801,         // 默认设置NODRAW+SELFDELETE_TEX
};

// Tips的时延
#define	SHOWTIPS_DELAY					12

// 界面图库偏移
#define INTERFACE_PIC(a)				a

#define SICON_BEGIN						0xFE				// 静态表情字体开头字节
#define DICON_BEGIN						0xFF				// 动态表情字体开头字节

// 控件的颜色
enum eCtrlMode
{
	CTRL_MODE_ALL,              // 所有不区分控件

	// 需要纹理换色的控件
	CTRL_MODE_BUTTON,			// 换色按钮
	CTRL_MODE_LABEL,			// LABEL

	// 需要纹理换色的控件个数

	// 需要自填充换色
	CTRL_MODE_TIP,				// 提示框
	CTRL_MODE_EDIT,				// 单行输入框
	CTRL_MODE_MULIEDIT,			// 多行输入框

	// 需要换色的控件总个数

	// 不需要换色的控件
	CTRL_MODE_BASE,				// 基础底层
	CTRL_MODE_CONTAINER,		// 容器
	CTRL_MODE_WINDOW,			// 一般窗口
	CTRL_MODE_MAINWIN,			// 主窗口
	CTRL_MODE_RADIO,			// 单选
	CTRL_MODE_MESSAGEBOX,		// 消息框
	CTRL_MODE_SCROLL,			// 滚动条
	CTRL_MODE_MENU_BUTTON,		// MenuButton
	CTRL_MODE_POPMENU,		    // 弹出菜单

	CTRL_MODE_GAMEWND,			// 游戏主窗口
	CTRL_MODE_PANELWND,			// 游戏主界面面板
	CTRL_MODE_HANDYMAPWND,      // 小地图窗口
};

// 调节透明
#define ADJUST_ALPHA(color)		            (color & Config.GetTransColor())

enum eMouseType
{
	//
	MOUSE_NORMAL	= 1,

	// 
	MOUSE_RUN,
	MOUSE_WALK,
	MOUSE_DIG,
	
	//
	MOUSE_ATTACK,
	MOUSE_MAGIC,
	MOUSE_HAND,
	MOUSE_TALK,

	// 
	MOUSE_ZOOM,

	MOUSE_NUMS
};
