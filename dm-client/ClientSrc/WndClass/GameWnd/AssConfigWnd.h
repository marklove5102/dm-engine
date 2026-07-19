#pragma once

#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/TalkViewer.h"
#include "GameAI/AIMedicaDefine.h"


#define PAGESX  66
#define PAGESY  69
#define PAGEEX  114
#define PAGEEY  90

#define WNDCLIENTSX   24
#define WNDCLIENTEX   513
#define WNDCLIENTSY   37
#define WNDCLIENTEY   381


class CAssConfigWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CAssConfigWnd)

public:
	enum
	{
		PAGE_BASE = 0,
		PAGE_PROTECT,
		PAGE_FIGHT,
		PAGE_ITEM,
		PAGE_MAGIC,
		PAGE_MONSTER,
		PAGE_CHAT,
		PAGE_RECORD,
		PAGE_README
	};
public:
	CAssConfigWnd(void);
	~CAssConfigWnd(void);

	virtual void OnCreate(void);
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnWheel(int iWheel);
	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	bool    IsInKeyCol();
	int     IsInFaceBtn(int iX,int iY);
private:
	int    m_iCurPage;     //当前属性页
	static int m_iLastPage;//上一个页面
	void   UpdateListPos(int iLine ,int iTotalCount);
	void   SetEatConfig();
	int    m_iFaceIndex;

	//公用的控件
	CCtrlButton* m_pDealutEatCfg;
	CCtrlGrid *   m_pGrid;//列表控件

	//基本页签////////////////////////////////////////////////////////

	//基本信息显示
	CCtrlRadio*         m_pRDShowObjectName;//显示物品名字
	CCtrlRadio*         m_pRDShowPlayerName;//显示玩家名字
	CCtrlRadio*         m_pRDShowArrow;    //突出显示自己
	CCtrlRadio*         m_pRDShowSysInfo;  //显示系统信息
	CCtrlRadio*         m_pRDShowQuickBuy; //快捷消费提醒
	CCtrlRadio*         m_pRDShowNpcPriority; //NPC突显
	CCtrlRadio*			m_pRDShowNewHandPrompt;//显示新手提示
	CCtrlRadio*			m_pRDShowHpChange;//显示伤血及加血冒泡提示
	CCtrlRadio*			m_pRDAcceptFrientTrade;//是否接受加好友和交易请求


	//战斗设置
	CCtrlRadio*         m_pRDQuickOutGame;  //快速小退
	CCtrlRadio*         m_pRDCloseTeam;     //关闭组队
	CCtrlRadio*         m_pRDDurationPrompt;//持久提醒
	CCtrlRadio*         m_pRDTeamColor;    //队友变色
	//CCtrlRadio*         m_pRDDismount;     //自动下马
	CCtrlRadio*         m_pRDAutoKill;    //自动打怪

	//其他辅助功能
	CCtrlRadio*         m_pRDAutoFindWay;  //智能寻路
	CCtrlRadio*         m_pRDGoPickup;     //拾取极品
	CCtrlRadio*         m_pRDAutoPickup;   //自动捡物
	CCtrlRadio*         m_pRDShowMagicTime;//法力提示
	CCtrlRadio*         m_pRDBury;         //掩埋尸体
	CCtrlRadio*         m_pRDMountain;     //忽略受伤动作
	CCtrlRadio*         m_pRDStarObject;   //闪烁物品
	CCtrlRadio*         m_pRDAutoMogong;     //自动点魔环
	CCtrlRadio*         m_pRDAllowFly;     //瞬移抢物
	CCtrlRadio*         m_pRDRunNotStop;   //跑步不停

	//战斗页签///////////////////////////////////////////////////////////

	CCtrlRadio*         m_pRDAutoProtectSkin ;   //自动护身真气
	CCtrlRadio*         m_pRDAutoSteelProtect;   //自动金刚
	CCtrlRadio*         m_pRDAutoFire;           //自动烈火
	CCtrlRadio*         m_pRDDestroyShield;      //持续破盾
	CCtrlRadio*         m_pRDAutoWildCollide;    //自动突斩
	CCtrlRadio*         m_pRDDirWild;            //定向突斩
	CCtrlRadio*         m_pRDDoubleFire;         //自动双烈火
	CCtrlRadio*         m_pRDDestroyShell;       //持续破击
	CCtrlRadio*         m_pRDAutoSuperAttack;    //自动强化攻杀


	CCtrlRadio*         m_pRDAutoMagicProtect;   //持续魔法盾
	CCtrlRadio*         m_pRDAutoDodgeSkill;     //持续风影盾
	CCtrlRadio*         m_pRDAutoDispute;        //自动抗拒

	CCtrlRadio*         m_pRDAutoProtectGhost;   //持续幽灵盾
	CCtrlRadio*         m_pRDAutoProtectArmor;   //持续神圣战甲术
	CCtrlRadio*         m_pRDAutoLionCall;       //自动狮子吼
	CCtrlRadio*         m_pRDSmartLionCall;      //定向狮子吼

	//战斗公共辅助部分
	CCtrlRadio*         m_pRDAntiAttack;         //自动反击
	CCtrlRadio*         m_pRDAutoSkill;          //自动练功
	//CCtrlEdit*          m_pEdAutoSkillKey;       //自动练功的键
	CCtrlMenuButton*    m_pMBAutoSkillKey;       //选择自动练功的键

	CCtrlEdit*          m_pEdAutoSkillTime;      //自动练功的时间间隔

	//保护页签///////////////////////////////////////////////////////////


	CCtrlRadio*         m_pRDUseHPMedia[MAX_MEDICA_NUMBER];
	CCtrlEdit*          m_pEdHPMediaLimit[MAX_MEDICA_NUMBER];
	CCtrlEdit*          m_pEdHPMediaDelay[MAX_MEDICA_NUMBER];

	CCtrlRadio*         m_pRDUseMPMedia[MAX_MEDICA_NUMBER];
	CCtrlEdit*          m_pEdMPMediaLimit[MAX_MEDICA_NUMBER];
	CCtrlEdit*          m_pEdMPMediaDelay[MAX_MEDICA_NUMBER];

	CCtrlRadio*         m_pRDUsePTSuperHP;
	CCtrlRadio*         m_pRDUsePTSuperMP;

	CCtrlMenuButton*    m_pMBAutoAction;
	CCtrlEdit*          m_pEdAutoActionLimit;

	CCtrlEdit*          m_pEdAutoQuitLimit;
	CCtrlRadio*         m_pRDAutoQuit;
	//CCtrlRadio*         m_pRDAutoFeed;
	//物品页签///////////////////////////////////////////////////////////

	CCtrlEdit*          m_pEdItemName;
	CCtrlButton* m_pAddItemButton;
	CCtrlButton* m_pDeleteItemButton;

	CCtrlMenuButton*    m_pMBBestItem;
	CCtrlMenuButton*    m_pMBExpensiveItem;
	CCtrlMenuButton*    m_pMBPickupItem;
	CCtrlMenuButton*    m_pMBNormItem;

	//魔法页签///////////////////////////////////////////////////////////

	//怪物页签///////////////////////////////////////////////////////////

	CCtrlEdit*          m_pEdMonsterName;
	CCtrlButton* m_pAddMonsterButton;
	CCtrlButton* m_pDeleteMonsterButton;

	CCtrlRadio*         m_pRDBossPrompt;    //BOSS提醒
	CCtrlRadio*         m_pRDObjectPrompt;  //极品提醒
	CCtrlRadio*         m_pRDBlackFilter;   //黑名过滤
	CCtrlRadio*         m_pRDPkAllAttack;   //PK变全攻击
	CCtrlRadio*         m_pRDPkAlarm;       //PK加仇敌


	//聊天页签///////////////////////////////////////////////////////////

	CCtrlRadio*         m_pRDAutoSay;       //自动发言
	CCtrlRadio*         m_pRDAutoReply;     //自动回复
	CCtrlEdit*          m_pEdAutoReplyMsg;
	CCtrlEdit*          m_pEdAutoSayTimer;  //发言时间间隔
	CCtrlMultiEdit*     m_pAutoSayMsg;      //自动发言内容
	CCtrlRadio*         m_pRDMessageAwoke;  //来言发声
	CCtrlRadio*			m_pKeepOutInfo;		//屏蔽陌生人喊话与私聊信息

	//记录///////////////////////////////////////////////////////////
	CCtrlRadio*         m_pRDRecordMsg;     //是否记录私聊
	CCtrlButton* m_pClearMsgButton;  //清除私聊记录
	CTalkViewer*   m_pTalkViewer; //聊天信息内容

	//使用///////////////////////////////////////////////////////////
};

