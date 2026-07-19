#pragma once

#include "CtrlWindowX.h"
#include <vector>
#include <string>

using namespace std;

//右键弹出菜单ID定义
enum POP_MENU_ITEM_ID
{
	POP_MENU_CLOSE		     = -1     , //关闭菜单
	POP_MENU_ITEM_NONE       = 0      , //空
	POP_MENU_ITEM_CHATSECRET = 1      , //密聊
	POP_MENU_ITEM_FRIEND              , //加为好友
	POP_MENU_ITEM_TEAM                , //邀请组队
	POP_MENU_ITEM_GUILD               , //邀请加入行会
	POP_MENU_ITEM_PHYLE				  , //邀请加入宗派
	POP_MENU_ITEM_TAKEIN_PRENTICE     , //招收徒弟
	POP_MENU_ITEM_TRADE               , //交易
	POP_MENU_ITEM_WATCH_BOOTH         , //查看摊位
	POP_MENU_ITEM_WATCH_EQUIP         , //查看装备
	POP_MENU_ITEM_COPYNAME            , //复制人名
	POP_MENU_ITEM_TAKEIN_MASTER       , //拜师
	POP_MENU_ITEM_ADDBLACK            , //加入黑名单
	POP_MENU_ITEM_ADDJIEYI            , //好友结义
	POP_MENU_ITEM_ATTACK_TYPE_SE      , //更改攻击模式为善恶攻击
	POP_MENU_ITEM_ATTACK_TYPE_ALL     , //更改攻击模式为全体攻击
	POP_MENU_ITEM_ATTACK_TYPE_PEACE   , //更改攻击模式为和平攻击
	POP_MENU_ITEM_ATTACK_TYPE_GROUP   , //更改攻击模式为编组攻击
	POP_MENU_ITEM_ATTACK_TYPE_GUILD   , //更改攻击模式为行会攻击
	POP_MENU_ITEM_ATTACK_TYPE_ST      , //更改攻击模式为师徒攻击
	POP_MENU_ITEM_ATTACK_TYPE_WH      , //更改攻击模式为夫妻攻击
	POP_MENU_ITEM_STORE_BUY           , //购买商城物品
	POP_MENU_ITEM_STORE_PRESENT       , //赠送商城物品
	POP_MENU_ITEM_STORE_SET_QUICK_ITEM, //设置商城快捷键
	POP_MENU_ITEM_NPC_QUICK_BUY       , //NPC图形化购物时购买物品
	POP_MENU_ITEM_NPC_QUICK_CARRYBACK , //NPC图形化仓库取回物品
	POP_MENU_ITEM_INVITE_VOICE,			//邀请语聊	
// 	POP_MENU_ITEM_INVITE_TNEUP_FREE   , //邀请自由群英模式
// 	POP_MENU_ITEM_INVITE_TNEUP_AREA   , //邀请区域群英模式
// 	POP_MENU_ITEM_APPLY_TNEUP_FREE    , //申请自由群英模式
// 	POP_MENU_ITEM_APPLY_TNEUP_AREA    , //申请区域群英模式

	//语聊弹出窗口
	POP_MENU_ITEM_CHANGE_PASWORD,       //修改密码，管理员特权
	POP_MENU_ITEM_KICK_OUT,				//踢人，管理员特权
	POP_MENU_ITEM_FORBID_ONE_SPEAK,		//禁言某个用户，管理员特权
	POP_MENU_ITEM_ALLOW_ONE_SPEAK,		//解除某个用户禁言，管理员特权
	POP_MENU_ITEM_RECEIVE_ONE_VOICE,	//接收某用户语音
	POP_MENU_ITEM_REJECT_ONE_VOICE,		//拒收某用户语音
	POP_MENU_ITEM_RECEIVE_ALL_VOICE,	//接收所有语音
	POP_MENU_ITEM_REJECT_ALL_VOICE,		//拒收所有语音
	POP_MENU_ITEM_ALLOW_ONE_HEAR_ME,	//允许某人收听我
	POP_MENU_ITEM_REJECT_ONE_HEAR_ME,	//拒绝某人收听我
	POP_MENU_ITEM_FORBID_ALL,			//禁言所有人，管理员特权
	POP_MENU_ITEM_ALLOW_ALL,			//解禁所有用户，管理员特权
	
	//宠物界面功能
	POP_MENU_ITEM_RIDE,					//本体骑乘
	POP_MENU_ITEM_MERRIDE,				//元神骑乘
	POP_MENU_ITEM_EXPLORE,				//探宝	
	//聊天频道选择
	POP_MENU_ITEM_CHANNEL_COMMON,   	//普通聊天频道	
	POP_MENU_ITEM_CHANNEL_TEAM,   	    //组队频道	
	POP_MENU_ITEM_CHANNEL_SHOUT,   	    //喊话频道	
	POP_MENU_ITEM_CHANNEL_PHYLE,   	    //家族频道	
	POP_MENU_ITEM_CHANNEL_GUILD,   	    //行会频道	
	POP_MENU_ITEM_CHANNEL_VOICE,   	    //语聊频道	


	//群英阵谱队长控制菜单
	POP_MENU_MICRO_CONTROL,             //微操
	POP_MENU_ITEM_CONFIG,               //托管设置
	POP_MENU_ITEM_KICK,                 //开除托管成员

	POP_MENU_ITEM_QIHUNFENJIE,          //器魂分解
	POP_MENU_ITEM_QIHUNTOCHAT,          //贴图到聊天框
	POP_MENU_ITEM_CHAIFEN,              //拆分

	POP_MENU_ITEM_FABAOCHAIFEN,          //法宝拆分
};

struct POP_MENU_ITEM
{
	POP_MENU_ITEM()
	{
		dwItemId = 0;
		strText.clear();
		strTips.clear();
		bEnable = true;
	}

	DWORD dwItemId;//菜单项ＩＤ
	string strText;//菜单文本
	string strTips;//tips文字
	bool bEnable;//是否可用
};

struct POP_MENUBUTTON_ITEM
{
	POP_MENUBUTTON_ITEM()
	{
		dwItemId = 0;	
		pbutton = NULL;
	}

	DWORD dwItemId;//菜单项ID
	CCtrlButton * pbutton;//按钮
};
typedef vector<POP_MENUBUTTON_ITEM> VPopMenuButtonItem;

//菜单属性数据
struct S_POP_MENU_DATA
{
	S_POP_MENU_DATA()
	{
		clear();
	}

	void clear();

	WORD wBackTex;//背景纹理,菜单的大小也由这张纹理大小决定,默认放在interface包里,这里的值是指在inteface包里的序号
	DWORD dwBackColor;//背景颜色,在背景纹理为0时有效
	DWORD dwItemTextColor;//文字绘制颜色
	DWORD dwItemDisableTextColor;//不可用项文字绘制颜色
	DWORD dwSelectColor;//鼠标移上去后的菜单项背景颜色
	DWORD dwHoverTextColor;//鼠标移上去后的字体颜色
	float fItemHeight;//菜单项行高
	int iX,iY;//弹出位置，如果没有设置(<=-100)则用菜单系统根据鼠标位置自动计算
	int iW,iH;//菜单宽度和高度，如果为0的话则宽度根据iLeftOffX,iRightOffX及菜单项的最大字符数确定，iH根据iTopOffY，iBottomOffY及fItemHeight与行数决定
	int iLeftOffX,iTopOffY,iRightOffX,iBottomOffY;//绘制时的偏移及右、下的空白大小
	int iSelBackColorLeftOffX,iSelBackColorRigthOffX;//选中项菜单背景颜色的左右边距
	int iFontType;//字体类型
	int iFontSize;//字体大小
};

typedef vector<POP_MENU_ITEM> VPopMenuItem;


class CCtrlMenuWnd : public CCtrlWindow
{
public:
	CCtrlMenuWnd();
	virtual ~CCtrlMenuWnd(void);

	virtual bool Create(CControl* pParent);
	virtual void Draw();
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual void OnKillFocus();

protected:
	bool    m_bClosed;//控制窗口自我关闭时避免重复调用
	int     m_iMouseOnItemIdx;//鼠标停在那个菜单项
	int     m_iItemWidht;//菜单项宽度
	float   m_fItemHeight;//菜单项高度



//下面是菜单及菜单项数据,本来放在gamedata，现移到这里
public:
	void Clear();
	void SetCaller(CControl *pCaller){ m_pCaller = pCaller;}
	CControl * GetCaller(){ return m_pCaller;}
	VPopMenuItem & GetMenuItemVector(){return m_VMenuItem;}
	bool AddMenuItem(const POP_MENU_ITEM &sMenuItem);//增加一个菜单项
	bool AddMenuItem(DWORD dwItemId,string strText,string strTips = "",bool bEnable = true);//增加一个菜单项
	bool DelMenuItem(DWORD dwItemID);//根据ID删除一个菜单项
	bool SetMenuItemEnable(DWORD dwItemID,bool bEnable);//设置enable
	bool SetMenuItem(DWORD dwItemId,string strText,string strTips = "",bool bEnable = true);
	S_POP_MENU_DATA & GetData(){return m_SPopMenuData;}


	void ShowMenu();//设置完数据后显示菜单
	void CloseMenu();//隐藏菜单

protected:
	CControl * m_pCaller;//请求弹出本菜单的窗口
	VPopMenuItem m_VMenuItem;//菜单项vector
	S_POP_MENU_DATA m_SPopMenuData;//菜单属性数据
	
};

class CCtrlMenuButtonWnd : public CCtrlWindow
{
public:
	CCtrlMenuButtonWnd();
	~CCtrlMenuButtonWnd();

	virtual bool Create(CControl* pParent,int iX,int iY);	
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

	void AddMenuButtonItem(DWORD dwItemId,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex = 0);//增加一个菜单项
	bool SetMenuButtonItem(DWORD dwItemId,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex = 0,bool bEnable = true);
	bool SetMenuButtonItem(DWORD dwItemId,bool bEnable);

	void Clear();
protected:	
	VPopMenuButtonItem m_VMenuButtonItem;//菜单按钮项vector	
};
