
#pragma once
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>
#endif

#include "GameClient/clientinfo.h"				//游戏客户端质量监控接口

////////////////////////////////////////////////////////////////////////////////////////
// SimpleCharacter.h需要用到的一些宏定义

#define	GAME_FIXED_FPS			25			// 游戏基本FPS

#define PLAYER_FRAMES			1000		// 玩家每件衣服的最大帧数
#define HAIR_FRAMES             1000        // 头发的最大帧数目
#define WEAPON_FRAMES           1000        // 武器的最大帧数目

#define PET_FRAMES				700			// 宠物的最大图片帧数
#define MONSTER_FRAMES			500			// 怪物的最大图片帧数
#define NPC_FRAMES				200			// NPC的最大图片帧数

// 动作相关的枚举

// 方向
enum E_DIRECTION
{
	DIR_UP = 0,						// 上
	DIR_RIGHTUP,					// 右上
	DIR_RIGHT,						// 右
	DIR_RIGHTDOWN,					// 右下
	DIR_DOWN,						// 下
	DIR_LEFTDOWN,					// 左下
	DIR_LEFT,						// 左
	DIR_LEFTUP,						// 左上
	DIR_NUMS,                       // 总方向数8
	DIR_NO,							// 无
};

// 所有的动作
enum E_ACTION
{
	ACTION_STAND = 0,	
	ACTION_WALK,		
	ACTION_RUN,		
	ACTION_ATTACK1,	
	ACTION_ATTACKED,	
	ACTION_MAGIC,	
	ACTION_HSTAND,	
	ACTION_HWALK,	
	ACTION_HRUN,		
	ACTION_HDEATH,	
	ACTION_DEATH,	
	ACTION_CUT,		
	ACTION_APPEAR,	
	ACTION_HATTACKED,
	ACTION_SALUTE,
	ACTION_LSALUTE,
	ACTION_LSTAND,
	ACTION_LWALK,
	ACTION_LRUN,
	ACTION_LDEATH,
	ACTION_LATTACKED,
	ACTION_LFSTAND,
	ACTION_LFWALK,
	ACTION_LFRUN,
	ACTION_LFDEATH,
	ACTION_LFATTACKED,
	ACTION_LFATTACK1,
	ACTION_LFMAGIC,
	ACTION_LFMAGICFGH,//战士骑战技能
	ACTION_LFMAGICRAB,//法师骑战技能
	ACTION_LFMAGICTAO,//道士骑战技能
	ACTION_SWT,       //圣武天套装技能动作,战士
	ACTION_LSJ,       //雷神绝套装技能动作,法师
	ACTION_JYL,       //九幽灵套装技能动作,道士
	ACTION_LFSWT,     //骑战 圣武天套装技能动作,战士
	ACTION_LFLSJ,     //骑战 雷神绝套装技能动作,法师
	ACTION_LFJYL,     //骑战 九幽灵套装技能动作,道士

	ACTION_STAND2 = 40,
	ACTION_STAND3,
	ACTION_ATTACK2,
	ACTION_ATTACK3,
	ACTION_ATTACK4,
	ACTION_ATTACK5,
	ACTION_ATTACK6,
	ACTION_ATTACKED2,
	ACTION_ATTACKED3,
	ACTION_MAGIC2,
	ACTION_MAGIC3,
	ACTION_APPEAR2,
	ACTION_APPEAR3,
	ACTION_DEATH2,
	ACTION_DEATH3,
	ACTION_ADDACTION1,
	ACTION_ADDACTION2,
	ACTION_ADDACTION3,
	ACTION_ADDACTION4,
	ACTION_ADDACTION5,
	ACTION_ADDACTION6,
	ACTION_ADDACTION7,
	ACTION_ADDACTION8,
	ACTION_ADDACTION9,
	ACTION_ADDACTION10,

	ACTION_TEMP, //临时动作
	ACTION_TURN, //
	ACTION_NUMS,
	ACTION_NULL,
};
//tips颜色
enum E_TIPCOLOR
{
	TC_BLUE	   = 0x00EB,
	TC_YELLOW  = 0x00FB,
	TC_RED	   = 0x0001,
	TC_GREEN   = 0x00DF,
	TC_WHITE   = 0x00FF,
	TC_CHAT	   = 0xFFEF,
	TC_IM	   = 0xFF5F,
	TC_PINK	   = 0x001F
};
//鼠标类型
enum E_MouseType
{
	MOUSE_STANDARD =              0,   //标准
	MOUSE_REPAIR,                      //修理
	MOUSE_SPECIALREPAIR,               //特殊修理
	MOUSE_SELFORCEATTOBJ,              //托管模式队长选择强攻目标
	MOUSE_SELGARRSIONXY,               //托管模式队长选择驻扎坐标
	MOUSE_HULU,						   //炼化葫芦
	MOUSE_BUBING,					   //12宫布兵
};
//鼠标上的钱或元宝从哪来的
enum E_DropMoneyYuanBaoFrom
{
	EDMYF_NONE =                0,   //鼠标上没有金钱或元宝
	EDMYF_PACKAG                 ,   //本体包裹
	EDMYF_TRADEWND               ,   //交易窗口
};

////////////////////////////////////////////////////////////////////////////////////////

//职业
enum E_JOB
{
	JOB_ZHANSHI			= 0,
	JOB_FASHI			= 1,
	JOB_DAO             = 2,
	JOB_GENERAL			= 9	//所有职业都能学习该类型的技能
};


//物品在那个窗口
enum E_ITEM_WINDOW
{
	ITEM_WIN_PKG				= 1,		//在人物包裹中
	ITEM_WIN_BODY				= 2,		//在人物身上
	ITEM_WIN_ACCEL				= 3,		//快捷栏
	ITEM_WIN_RESOURCE			= 4,		//原料类物品
	ITEM_WIN_SHOP				= 5,		//商店
	ITEM_WIN_STORAGE			= 6,		//仓库

	ITEM_WIN_EXCHANGE,						//玩家交易，己方
	ITEM_WIN_EXPEER,						//玩家交易，对方
	ITEM_WIN_TRADE,							//NPC交易
	ITEM_WIN_SUBSTITUTION,					//装备交换
	ITEM_WIN_NPCTRADE,						//NPC买卖

	ITEM_WIN_VENDOR,						//到摆摊界面中
	ITEM_WIN_COMPOSE,						//到合成窗口
	
	ITEM_WIN_LOTTERY,						//提交奖券时用于放置奖券
	ITEM_WIN_MARKET,						//寄售窗口
	ITEM_WIN_QUERYRESOURCE,					//原料用途查询窗口
	ITEM_WIN_SENDITEM,						//赠送物品的物品放置
	ITEM_WIN_MALL_PREVIEW,					//商城中的预览位置
	ITEM_WIN_MALL_HOT,						//商城热销排行位置
	
	ITEM_WIN_MAX,							//总数
	ITEM_USE					= 0xFD,		//物品使用
	ITEM_GROUND					= 0xFE,		//地上
	ITEM_FAIL					= 0xFF,		//失败

	ITEM_ONMOUSE				= 0,		//在鼠标上的物品
	ITEM_ONSUBMIT				= -1,		//当前正在提交的物品
};


enum MOUSE_ON_TYPE
{
	MOUSE_ON_CONTROL	= 0xFFFFFFFF,	//指向控件
	MOUSE_ON_INVALID	= 0xFFFFFFFE,	//无效
	MOUSE_ON_GOOD		= 0xFFFFFFFD,	//物品
	MOUSE_ON_SKILL		= 0xFFFFFFFC,	//技能图标
	MOUSE_ON_PETSKILL	= 0xFFFFFFFB,	//宠物技能图标
	MOUSE_ON_NAME		= 0xFFFFFFF8,	//游戏角色
	MOUSE_ON_VALUE		= 0xFFFFFFF7,	//数值
	MOUSE_ON_CTRLACTION	= 0xFFFFFFF6,	//表情说明
};

//鼠标上物品的类型
enum DROP_GOOD_TYPE
{
	DROPGOOD_NONE	= 0,
	DROPGOOD_ITEM,
	DROPGOOD_CASH,
	DROPGOOD_DEPS,
	DROPGOOD_SPLT,
};

//
enum DATA_LENGTH
{
	DATA_LEN_APPR						= 8,	//外观
	DATA_LEN_NAME						= 16,	//名字
	DATA_LEN_TITLE						= 140,	//头衔
	DATA_LEN_GUILDTITLE					= 16,	//率团组名
	DATA_LEN_MAGIC_NAME					= 16,	//魔法名字
	DATA_LEN_ITEM_SET_EXT				= 16,	//套装附加属性
	DATA_LEN_ITEM_EXT					= 20,	//物品特殊附加属性
};

enum E_JOB_TITLE
{
	PALYER_NAME				= 0	 ,		//玩家名字
	PALYER_JOB				= 16 ,		//职业
	PALYER_JOB_TITLE		= 30 ,		//职业称号
};

// 
enum NAMETYPE
{
	NT_PALYER_NAME			= 0,	// 玩家名字
	NT_PALYER_JOB			,		// 职业
	NT_PALYER_JOB_TITLE		,		// 职业称号

	NAME_TYPE_MAX			,
};

//Creature状态
enum E_CREATE_STATE
{
	CREATURE_ELEHIT			= (1<< 0),	//电击
	CREATURE_GREENPOIS		= (1<< 1),	//中毒

	// 以下的宠物和玩家的重用
};

// 玩家自己下一步动作的状态
enum E_NEXT_STATE
{
	ENS_NO_ACTION	= 0	,	// 没有下一步动作
	ENS_RAND_ACTION		,	// 随机动作，比如：stand1动作
	ENS_PLAYER_ACTION	,	// 玩家主动的动作，如：走路，攻击等
	ENS_DURANT_ACTION	,	// 玩家持续动作，如：露营，拥抱，接吻等
	ENS_SERVER_ACTION	,	// 服务器要求玩家执行的动作，如：被冲撞等
};
////////////////////////////////////////////////////////////////////////////////////////
// 物品相关的一些宏的定义CGood类

//身上的装备，顺序：衣服、武器、勋章、项链、头盔、右手镯、左手镯、右戒指、左戒指、靴子、腰带、宝石、杂物
enum E_ITEM_POS
{
	ITEM_POS_CLOTH			= 0,
	ITEM_POS_WEAPON,
	ITEM_POS_MEDAL,
	ITEM_POS_NECKLACE,
	ITEM_POS_HELMET,
	ITEM_POS_RIGHT_BANGLE,
	ITEM_POS_LEFT_BANGLE,
	ITEM_POS_RIGHT_RING,
	ITEM_POS_LEFT_RING,
	ITEM_POS_CLOTH_BOOT,
	ITEM_POS_BELT,
	ITEM_POS_GEM,
	ITEM_POS_OTHERS,
	ITEM_POS_SHIELD,
	ITEM_POS_BALL,				//魂珠
	ITEM_TNEUPCHART,            //阵谱
	ITEM_POS_WENPEI,            //纹佩
	
	
	
	


	ITEM_POS_WING,              //翅膀相关数据,和gs一并把它当成一个装备,实际不并不是一个装备不可以装备或脱下
	ITEM_POS_FABAO,             //法宝相关数据,和gs一并把它当成一个装备
	//ITEM_POS_FABAOLEFT,			//左法宝
	//ITEM_POS_FABAORIGHT,		//右法宝
	MAX_EQUIPMENT,				//装备最多个数


	GOODGRID_TYPE_PACKAGE,      //表示goodgrid的类型为包裹
	GOODGRID_TYPE_BELT,         //表示goodgrid的类型为腰带
	GOODGRID_TYPE_PETPACKAGE,   //表示goodgrid的类型为宠物包裹
// 	GOODGRID_TYPE_TRUSTEESHIP_PACKAGE_1,//表示goodgrid的类型为托管包裹,左数第一个
// 	GOODGRID_TYPE_TRUSTEESHIP_PACKAGE_2,//表示goodgrid的类型为托管包裹,左数第二个
// 	GOODGRID_TYPE_TRUSTEESHIP_PACKAGE_3,//表示goodgrid的类型为托管包裹,左数第三个
// 	GOODGRID_TYPE_TRUSTEESHIP_PACKAGE_4,//表示goodgrid的类型为托管包裹,左数第四个
};

//升级类型，圣灵精华
enum E_EQUIPMENT_UPDATE
{
	EQUIPMENT_ARM		= 1,
	EQUIPMENT_SHIELD	= 2,
};

extern UINT	MAX_PACKAGE_ELEMENT;
extern UINT MAX_PACKAGE_NUM;
extern UINT MAX_PET_PACKAGE;

#define MAX_MER_PACKAGE 40

#define MAX_TRADE_OBJECTS 20        // 与玩家交易最大物品个数
#define MAX_EXCHANGE	9			// 与玩家交易最大个数
#define MAX_STORAGE		39			// 仓库存放的物品容量

// 物品的m_dwData标示的属性
enum E_ITEM_STATE
{
	PUT_SALE    =   0x00000001,		// 正在卖出窗口
	PUT_COMPOSE	=	0x00000002,		// 正在合成窗口
	PUT_REPAIR	=	0x00000004,		// 正在一般修理窗口
	PUT_REPAIR1	=	0x00000008,		// 正在特殊修理窗口
	PUT_STORAGE	=	0x00000010,		// 正在保管窗口
	PUT_FORGE	=	0x00000020,	    // 正在特修窗口
	PUT_TURN_IN	=	0x00000040,		// 正在上缴窗口

	WAIT_PUT_CONFIRM =		0x40000000,		// 正在等待服务器的放入确认
	WAIT_SUBMIT_CONFIRM	=	0x80000000,		// 正在等待服务器的提交确认
};

// 交易窗口类型
enum GOOD_TRADE_TYPE
{
	TRADE_SALE	= 1,	// 卖出
	TRADE_COMPOSE,		// 合成
	TRADE_REPAIR,		// 一般修理
	TRADE_REPAIR1,		// 宠物治疗
	TRADE_FORGE,		// 特修

	TRADE_TYPE_NUM,		// 交易种类数
	TRADE_STORAGE,		// 保管
	TRADE_PETCARE,		// 宠物托管
	TRADE_BUY,			// 买入
	TRADE_BUY_ONE,		// 每次只能买一个的类型
	OPERATE_IDENTIFY,	// //极品鉴定
	TRADE_SUBMIT,		// 提交
	TRADE_RECYCLE,		// 中转系统，卖出
	TRADE_MAX_TYPE,		// 目前支持最大类型
};
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// 魔法数据结构相关的宏定义CMagicData类
#define MAX_ACCEL_NUMS		14				//最大的快捷键设置,其中PVE(1-7),PVP(8-14)
#define	MAX_MAGIC_SKILL		60
#define MAX_PRODUCEMAGIC_SKILL 10           //生产技能最大个数

#define MAX_MAGIC_SHOW		128
#define MAX_RESERVEDATA_SIZE  32
#define MAX_LEVEL                   22
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// 服务器组信息宏
#define MAX_GROUP_NUM				4
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// 选人相关
#define MAX_SEL_CHAR_NUM			2	// 可选角色数
#define MAX_DEL_CHAR_NUM			4	// 可恢复角色数
#define MAX_HAIR_COLOR              16  // 头发颜色数目
#define MAX_BODY_COLOR              16
#define NUM_HAIR_STYLE	            8
#define NUM_HALF_HAIR               3

////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define QUIT_TIME_DELAY  2000
#else
#define QUIT_TIME_DELAY  5000
#endif

////////////////////////////////////////////////////////////////////////////////////////
// 地图信息相关宏定义CMapArray类
#define MAPARR_WIDTH				30	// 屏幕有效区域宽度高度

#define VIEW_WIDTH	12
#define VIEW_HEIGHT 12

#define TILE_WIDTH		64      //格子的宽度
#define TILE_HEIGHT     32      //格子的高度

////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// OtherSimpleData类
#define MAXMAGICBEGIN	100
#define CONTINUE_MAGIC_SPEED 3

#define SHORTCUT_KEY	6

////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// TalkContent类
#define TALKCONTENT_BUFFER	500
#define PANEL_SIZE			50
#define LEFTTOP_SIZE		16
#define PETSTATE_TALK_SIZE	1

#define MAXHEADWIDTH		24	// 玩家头顶说话每行的字符数
#define MAXTALKSIZE			42	// 说话最大容纳字符数
#define VALIDTALKSIZE		40	// 说话有效的字符个数

enum MESSAGE_TALK_TYPE
{
	TALKTYPE_SERVER		= 0x00000001,		// 系统广播，显示在顶部
	TALKTYPE_PRIVATE	= 0x00000002,		// 私聊
	TALKTYPE_TROOP		= 0x00000004,		// 组队
	TALKTYPE_GUILD		= 0x00000008,		// 行会
	TALKTYPE_PUBLIC		= 0x00000010,		// 公聊
	TALKTYPE_NORMAL		= 0x00000020,		// 提示
	TALKTYPE_MTALK		= 0x00000040,		// 怪物说话
	TALKTYPE_SYSTEM     = 0x00000080,       // 提示频道
	TALKTYPE_BOOTH      = 0x00000100,       // 自己摊位
	TALKTYPE_BUY        = 0x00000200,       // 别人摊位留言
	TALKTYPE_PAIMAI     = 0x00000400,       // 拍卖行 
	TALKTYPE_VOICE      = 0x00000800,		// 语言聊天
	TALKTYPE_ASSPRIVATE = 0x00001000,		// 辅助工具中间的私聊
	TALKTYPE_MGUILD     = 0x00002000,       // 行会界面上的行会聊天
	TALKTYPE_PHYLE		= 0x00004000,		// 宗派聊天
	TALKTYPE_ALL        = 0x0000FFFF,       // 全部频道
};
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// TalkName类
#define MAX_NAME_COUNT 10

////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// TradeData类

////////////////////////////////////////////////////////////////////////////////////////
#define GROUP_SIZE					10	//组队人数上限


//与NPC交互的弹出类型
enum NPC_POPINPUT_TYPE
{
	POPINPUT_NONE	= 0,
	POPINPUT_DIGITAL
};

////////////////////////////////////////////////////////////////////////////////////////
//
enum MONEYTYPE
{
	TYPE_DEFAULT	= 0,	//0 (普通或默多克)
	TYPE_GOLD		= 1,	//1 梦幻币
	TYPE_YUANBAO	= 2,	//2 只接受点券购买
	TYPE_REPUTE		= 4,	//4 只是声望
	TYPE_GOLD_NUM,
};

enum E_COLOR
{
	COLOR_DEFAULT = 0xFFFFFFFF,
	COLOR_CLIP = 0xFFFF00FF,
	COLOR_GRAY = 0xFF888888,
	COLOR_RED  = 0xFFFF0000,
	COLOR_GREEN = 0xFF00FF00,
	COLOR_BLUE = 0xFF0000FF,
	COLOR_PURPLE = 0xFFFF00FF,
	COLOR_FROZEN = 0xFFAEF5FF,
	COLOR_BLACK  = 0xFF000000,
	COLOR_ORANGE = 0xFFFFF100,
	COLOR_YELLOW = 0xFFFFFF00,
	COLOR_HIDE = 0x88FFFFFF,

};

enum E_TALKCOLOR
{
	TALKCOLOR_RED	=	0x38FF,		// 0xFFF9 系统提示颜色1
	TALKCOLOR_GREEN =   0x00FA,
	TALKCOLOR_BLUE  =   0xFCFF,      
	TALKCOLOR_PINK  =   0xFDFF,
	TALKCOLOR_WHITE =   0xFFFC,
	TALKCOLOR_GUILD =   0x05FF,
};

enum E_G_SERVER_SWITCH
{
	//SS_BOOTH_LEOPARD    = 0x00000001,         //豹子摆摊功能
	SS_QUIT_DELAY       = 0x00000002,         //延迟下线   
	SS_RAINBOW          = 0x00000004,         //是否开启彩虹帮助
	SS_C2C              = 0x00000008,         //是否开启C2C
	SS_QUICK_CONSUME    = 0x00000010,         //是否开启(腰带上的)快捷消费使用功能
	SS_ADD_LINGLI       = 0x00000020,         //是否开启天关幻镜冲灵符灵力功能
	SS_REGISTER_FCM     = 0x00000040,         //是否开启防沉迷信息补填
	SS_DELETE_MAIN_TASK = 0x00000100,		  //是否放开主线任务删除按钮
	SS_HEART_NACK		= 0x00000200,		  //灵力系统开发
	SS_IGW      		= 0x00000400,		  //是否开启IGW
	SS_CREDIT           = 0x00000800,         //是否开启信用系统
	SS_SEARCH_BET_MAP	= 0x00001000,		  //是否关闭自动寻路跨地图
	SS_CLIENT_INFO_COL	= 0x00002000,		  //是否开启客户端信息收集
};

enum E_RACE_TYPE
{
	CHARACTER_HUMAN		        =		0,		// 人
	CHARACTER_NPC		        =		1,		// NPC
	CHARACTER_MONSTER	        =		2,		// 怪物
	CHARACTER_DEMON             =       3,      // 人形怪物,假心魔
};

enum eMAGICSTATE
{
	MS_ATTACKKILL    = 0x00000001,
	MS_ATTACKSTICK   = 0x00000002,
	MS_ATTACKMOON    = 0x00000004,
	MS_ATTACKFIRE    = 0x00000008,
	MS_VALIDDIG      = 0x00000010,
	MS_WEAPONBREAK   = 0x00000040,
	MS_ANIMAPPEAR    = 0x00000080,
	MS_MAGICPROTECT  = 0x00000100,
	MS_REMAINSHADE   = 0x00000200,
	MS_BLOODSHADE    = 0x00000400,
	MS_WHOOLEMOON    = 0x00000800,
	MS_THUNDERFIRE   = 0x00001000,
	MS_DESTROYWEAPON = 0x00004000,
	MS_DESTROYSHELL  = 0x00008000,
	MS_DESTROYSHIELD = 0x00010000,
	MS_SATTACKKILL   = 0x00020000,
	MS_NPCWAITSERVER = 0x10000000,
};

enum eCREATESTATUS
{
	CS_ROTATEFIRE    = 0x0001, //风火轮
	CS_PROTECTSKIN   = 0x0002, //护身真气
	CS_SOULWALL      = 0x0004, //灵魂墙
	CS_MAGICPROTECT  = 0x0010, //魔法盾
	CS_HIDE          = 0x0080, //隐身
	CS_STAMP         = 0x0008, //特殊道具
	CS_LULL			 = 0x0400,
	CS_RED			 = 0x4000,
	CS_GREEN		 = 0x8000,
	CS_PURPLE		 = 0x0200,
	CS_BLUE			 = 0x0800,
	CS_BLACK		 = 0x2000,
};

enum eBIGTEACHER
{
	CS_SHEN_SHIELD	 = 0x0001,//大师级魔法盾神
	CS_MO_SHIELD	 = 0x0002,//大师级魔法盾魔
};

enum eEXTRASTATUS
{
	ES_STEELPROTECT  = 0x01,
	ES_DODGEPROTECT  = 0x02,
	ES_SHENYOU       = 0x04,
	ES_FROST         = 0x20,
	ES_PAOPAO        = 0x80,
	ES_ZHENYUAN		 = 0x100,//真元攻击
	ES_GOLDENOXSUIT  = 0x200,//金牛套装
	ES_FIRE          = 0x400,//火烧状态,红毒一样,gs端处理不一样
	ES_BRAST         = 0x800,//自爆状态,心魔必杀的一样
	ES_FROST2        = 0x1000,//冰冻,和ES_FROST一样,gs端处理不一样
	ES_KUANGNUWIND   = 0x2000,//设置狂怒旋风状态
	ES_COOLICESTORM  = 0x4000,//设置寒冰风暴状态
	ES_DIANLINGFUTI  = 0x8000,//设置电灵附体状态
};
//指明玩家某技能是不是大师级,bodycolor3
enum eGREATEMAGIC
{
	//EG_PROTECTSKIN			= 0x01,//大师级护身真气
	//EG_STEELPROTECT			= 0x02,//大师级金刚护体
	//EG_MAGICID_ROTATE_FIRE	= 0x20,//风火轮(大师)
	//EG_MAGICID_SHIELD		= 0x40,//魔法盾(大师)
	EG_KULONGQIULONG		= 0x0001,//骷髅囚笼
	EG_STEELPROTECT			= 0x0002,//大师级金刚护体
	EG_PROTECTSKIN			= 0x0004,//大师级护身真气
	EG_POISON   			= 0x0008,//大师级施毒
	EG_CURSE      			= 0x0010,//大师级诅咒
	EG_ROTATE_FIRE      	= 0x0020,//风火轮(大师)
	EG_SILENCE          	= 0x0040,//风火轮(大师)
	EG_FANZHEN_BURNNING     = 0x0080,//法诀技能燃烧效果

	EG_MAGICID_SOULWALL     = 0x1000,//大师灵魂墙
	EG_MAGICID_MOFADUN		= 0x2000,//魔法盾(大师)

};


enum eRELATIONTYPE
{
	RT_GROUP            = 0x00000001, //是否队友
	RT_ATTACKED         = 0x00000002, //是否为自己的攻击对象
	RT_HUSBAND          = 0x00000004, //是否为自己丈夫
	RT_WIFE             = 0x00000008, //是否为自己老婆
	RT_MASTER           = 0x00000010, //是否为自己师傅
	RT_PRENTICE         = 0x00000020, //是否为自己徒弟
	RT_GUILD            = 0x00000040, //是否和自己同行会
	RT_FRIEND           = 0x00000080, //是否为自己好友
	RT_PHYLE			= 0x00000100, //是否为自己同一宗派成员
	RT_JIEYI			= 0x00000200, //是否为自己结义
};
//0 隐身 1 反射 2 生命 3重力 4诅咒5巫毒6巫术7魔力8仇恨9箭塔10陷阱 
enum E_TuTeng_State
{
	ETTS_HIDE       = 0x00000001,//隐身图腾
	ETTS_REFLECT    = 0x00000002,//反射图腾
	ETTS_LIFE       = 0x00000004,//生命图腾  
	ETTS_GRAVITY    = 0x00000008,//重力图腾  
	ETTS_CURSE      = 0x00000010,//诅咒图腾  
	ETTS_ZOMBIE     = 0x00000020,//巫毒图腾  
	ETTS_SORCERY    = 0x00000040,//巫术图腾  
	ETTS_MAGIC      = 0x00000080,//魔力图腾  

};

enum ePaoPaoPromptStatus
{
	//
	EP_FirstInGmae            = 0,      //第一次进入游戏
	EP_FirstInGmae_Step2,               //第一次进入游戏,第2步
	EP_FirstInGmae_Step3,               //第一次进入游戏,第3步
	EP_FirstInGmae_Step4,               //第一次进入游戏,第4步
	EP_First_TalkWithNPC,               //第一次与任务NPC对话
	EP_First_AutoFindPath,              //第一次自动寻路
	EP_First_ShortcutKeyWnd_PaoPao,     //第1次点打开设置技能快捷方式界面时
	EP_First_SetShortcutKey_Base,       //第1次装备初级剑法到技能快捷栏中
	EP_First_SetShortcutKey_Fire,       //第1次装备小火球到技能快捷栏中
	EP_First_SetShortcutKey_Poison,     //第1次装备施毒术到技能快捷栏中
	EP_First_SetShortcutKey_Symbol,     //第1次装备灵魂道符到技能快捷栏中
	EP_First_Sell,                      //第一次出售
	EP_First_Repair,                    //第一次修理
	EP_First_Level_30,                  //到达30级以后第一次打开包裹
	EP_First_Open_WeiPeiWnd,            //第一次打开纹配包裹


















	EP_First_GetTaoMuJian,              //第一次获得桃木剑
	EP_First_GetWaKuangBook,            //第一次获得挖矿技能书
	EP_First_GetFaMuBook,               //第一次获得伐木技能书
	EP_First_GetFaMuFu,                 //第一次获得伐木斧
	EP_First_GetHuanCaiJieZhi,          //第一次获得幻彩戒指
	EP_First_GetSkillBook,              //第一次获得小火球/治疗术/初级剑法
	EP_First_GetQunYingBaoJian,         //第一次获得群英宝鉴
	EP_First_GetLianYuTiYuanKa,         //第一次获得炼狱体验卡
	EP_First_GetXinShouLingPai,         //第一次获得新手装备令牌
	EP_FirstGetNewTask_PaoPao,          //第一次新接到任务
	EP_FirstOpenTaskWnd_PaoPao,         //第一次打开任务界面
	EP_FirstWalkToZhongZhou_PaoPao,     //第一次到中州
	EP_FirstOpenMapWnd_PaoPao,          //第一次在中州打开地图界面时
	EP_FirstSelectMap_PaoPao,           //第一次在中州对地图位置进行了选择
	EP_FirstLevelUp_PaoPao,             //第一次升级时
	EP_First_GetTaoMuJian_PaoPao,       //第一次获得桃木剑
	EP_First_GetTaoMuJian_PaoPao1,      //第一次获得桃木剑打开包裹
	EP_First_GetFaMuBook_PaoPao,        //第一次获得伐木技能书
	EP_First_GetQunYingBaoJian_PaoPao,  //第一次获得群英宝鉴
	EP_First_GetLianYuTiYuanKa_PaoPao,  //第一次获得炼狱体验卡
	EP_First_WeigthFull_PaoPao,         //第一次人物负重量满时
	EP_First_WanLiFull_PaoPao,          //第一次腕力负重满时
	EP_First_OpenSkillWnd_PaoPao,       //第一次打开技能界面时
	EP_First_GetAttackSkillBook_PaoPao, //第一次获得小火球/治疗术/初级剑法
	EP_First_StudySkill_PaoPao,         //第一次学会技能
	EP_First_Skill_SkillWnd_PaoPao,     //学会技能后,第1次打开技能界面
	EP_First_CutZhangShu_PaoPao,        //第1次砍倒樟树后
	EP_First_UseSkill_PaoPao,           //第1次使用小火球、灵魂道符后
	EP_First_PackageFull_Panel_PaoPao,  //包裹负重第1次满时
	EP_First_WatchGood_Panel_PaoPao,    //第1次看到地面掉落物品时
	EP_First_OpenWuSeTaskWnd_PaoPao,    //第1次打开五色任务界面时
	EP_First_GetHuanCaiShouZhuo,        //第一次获得幻彩手镯
	EP_First_Team_TeamWnd,              //第一次组队
	EP_First_FaYan_Step1,               //第一次发言/屏蔽发言,第1步
	EP_First_FaYan_Step2,               //第一次发言/屏蔽发言,第2步
	EP_First_FaYan_Step3,               //第一次发言/屏蔽发言,第3步
	EP_First_Team_Step1,                //第一次组队,第1步
	EP_First_FirstHurt_PaoPao,			//第一次受伤


	//下面的gs任务任务系统也要用到,编号指定
	EP_First_BaiShiShouTu = 59,         //第一次拜师收徒,gs用到,客户端不用占用这位
	EP_First_FaYan = 60,                //第一次发言/屏蔽发言
	EP_First_Team = 61,                 //第一次组队
	//EP_First_Trusteeship = 62,          //第一次体验群英模式
	//EP_First_XinYunQunYingZhuan = 63,   //第一次幸运珍宝阁

};	

enum eNewHanderPrompt
{
	ES_CHARACTER_MOVE = 0,
	ES_NEWHANDER_TEACHER,
	ES_FIND_TASK_TO_DO,
	ES_LOOKFOR_JOB_GUIDER,
	ES_CHARACTER_DEAD,
	ES_TALK_TO_PLAYER,
	ES_HOW_TO_GROUP,
	ES_NO_CONV_FULLSC,
	ES_SEND_PRIVATE_TALK,
	ES_HOW_TO_LOOKFOR_HELP,
	ES_ABOUT_GAME_SET,
	ES_ABOUT_INTERFACE,
	ES_HOW_TRADE,
	ES_MAGIC_SHORT_CUT,
	ES_LEARNABOUT_MAGIC,
	ES_PANEL_HIDE_PIC,
	ES_START_OFF_AUDIO,
	ES_SOLUTE,
	ES_ABOUT_POSION,
	ES_SWITCH_ATTACK_MODE,
	ES_ABOUT_HORSE,
	ES_ABOUT_PAIMAI,
	ES_AIM,
	ES_LIFE,
	ES_MAGIC,
	ES_FIGHT,
	ES_ENEMY_MONSTER,
	ES_PICKUP_FIRST,
	ES_DIG_GOODS,
	ES_SHORT_USE_GOOD,
	ES_RAINBOW,
	ES_SHADER_WEATHER,
	ES_ADJUST_AUDIO,
	ES_SHOW_CUR_LIFEBAR,
	ES_ABOUT_WORLD_VIEW,
	ES_ABOUT_MAP,
	ES_ABOUT_ACTION_LOG,
	ES_LOOK_AROUNT_NPC,
	ES_FILTER_PLAYER,
	ES_JOIN_GUILD,
	ES_ADD_FRIEND,
	ES_HOW_TO_BOOTH,
	ES_ABOUT_PK,
	ES_ABOUT_FASHION,
	ES_GROUP_CONFINE,
};

#define  MAX_EQUIP_SOUL_LEVEL 4


//聊天频道
enum eSendTalkChannel
{
	ETC_COMMON = 0,   //普通
	ETC_TEAM,         //组队
	ETC_SHOUT,        //喊话
	ETC_PHYLE,        //宗族
	ETC_GUILD,        //行会
	ETC_VOICE,        //语聊
	ETC_NUM           //总数
};


#define SAMPLE_GAMEVERSION     1001
#define SAMPLE_GAMEINFO        1002
#define SAMPLE_SOCIETY         1003
#define SAMPLE_ACTION          1004
#define SAMPLE_ACTIVITYMAP     1005
#define SAMPLE_CLIENTINFO      1006
#define SAMPLE_GAMEEFFECT      1007
#define SAMPLE_GAMEQUALITY     1008





//跟游戏逻辑相关的一些全局变量
extern DWORD g_dwDeathTime;  //死亡的时间

