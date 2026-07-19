#include "assconfigwnd.h"
#include "GameAI/AIConfigMgr.h"
#include "GameAI/AIAutoEatMgr.h"
#include "GameAI/AIPickCfgMgr.h"
#include "GameAI/AIBossCfgMgr.h"
#include "GameAI/AutoKillMonsterMgr.h"
#include "GameAI/AIMgr.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIMedicaDefine.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TalkMgr.h"
#include "GameData/ConfigData.h"
#include "GameAI/AIAutoPickMgr.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameData/MagicDefine.h"
#include "Global/Interface/StreamInterface.h"

#define BASECOL1   55
#define BASECOL2   210
#define BASECOL3   368
#define BASEROW1   100
#define BASE_LINE_GAP 21

#define FIGHTCOL1  59
#define FIGHTCOL2  226
#define FIGHTCOL3  377
#define FIGHTROW1  92
#define FIGHTROW2  233
#define FIGHTROW3  311

#define MONSTCOL1  368
#define MONSTERROW1 91

#define PAGEINTERNAL 49
#define PAGECOUNT 9

#define FACESX          86
#define FACESY          226
#define FACECOLCOUNT    20
#define FACETOTALCOUNT  54

INIT_WND_POSX(CAssConfigWnd,POS_VARIABLE,POS_VARIABLE)

static const char* RIGHT_SYM = "√";
static const char* BOSS_SYM = "BOSS";
static const char* ENEMY_SYM = "仇敌";
static const char* BLACK_SYM = "黑名";

int CAssConfigWnd::m_iLastPage = 0;

CAssConfigWnd::CAssConfigWnd(void)
{
	g_pControl->PopupWindow(MSG_CTRL_SYSCONFIG_WND,OPER_CLOSE);

	m_iCurPage = m_iLastPage;
	m_pGrid = NULL;
	m_iFaceIndex = -1; 
	m_pTalkViewer = NULL;

 //	if((g_dwServerSwitch & SS_QUIT_DELAY) && m_iCurPage == PAGE_PROTECT)
	//	m_iIndex = 10155;
	//else
	//	m_iIndex = 10156 + m_iCurPage;

	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	if(!g_pControl->GetWindowPos(m_iOffX,m_iOffY,"SysConfigWnd"))
	{
		m_iOffX = (g_pGfx->GetWidth() - 545)/2;
		m_iOffY = (g_pGfx->GetHeight() - 442)/2;
	}

	m_bNeedSavePos = false;


	//以下为新版页签相关
	m_iVersion = 2;

	S_TabPage* pGameConfigPage = AddTabPage(0,0,MAKELONG(10110,PACKAGE_INTERFACE),0,37,85,86,87,88,"游戏设置",NULL,true,0,0,1);
	AddTabPage(0,0,MAKELONG(10110,PACKAGE_INTERFACE),47,36,115,116,117,118,"基 本",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10111,PACKAGE_INTERFACE),99,36,115,116,117,118,"保 护",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10112,PACKAGE_INTERFACE),148,36,115,116,117,118,"战 斗",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10113,PACKAGE_INTERFACE),199,36,115,116,117,118,"物 品",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10114,PACKAGE_INTERFACE),250,36,115,116,117,118,"魔 法",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10115,PACKAGE_INTERFACE),303,36,115,116,117,118,"怪 物",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10116,PACKAGE_INTERFACE),352,36,115,116,117,118,"聊 天",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10117,PACKAGE_INTERFACE),403,36,115,116,117,118,"记 录",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(10118,PACKAGE_INTERFACE),454,36,115,116,117,118,"帮 助",pGameConfigPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);

	AddTabPage(0,0,MAKELONG(10110,PACKAGE_INTERFACE),0,122,85,86,87,88,"系统设置",NULL,true,0,0,1);


	if (m_iLastPage < m_TabPage.vSubTabPage.size())
	{
		m_TabPage.iCurPage = m_iLastPage;
	}
}

CAssConfigWnd::~CAssConfigWnd(void)
{
	m_iLastPage = m_TabPage.iCurPage;
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"SysConfigWnd");
}

void CAssConfigWnd::OnCreate(void)
{
	SetCloseButton(496,4,80);
	SetMask(10110);
	SetPageRect(0,0,0,0,0);

	m_pGrid = NULL;
	m_pDealutEatCfg =NULL;

	m_iCurPage = m_TabPage.vSubTabPage[0].iCurPage;

	//大页签//////////////////////////////////基本////////////////////////////////////////
	if(m_iCurPage == PAGE_BASE)
	{
		m_pDealutEatCfg = new CCtrlButton();
		AddControl(m_pDealutEatCfg);
		m_pDealutEatCfg->CreateEx(this, 237,62, 90, 91, 92, 93);
		m_pDealutEatCfg->SetText("默认配置", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pRDShowObjectName = new CCtrlRadio();
		AddControl(m_pRDShowObjectName);
		m_pRDShowObjectName->Create(this,BASECOL1,BASEROW1,125,126,127,128,110,20);
		m_pRDShowObjectName->SetText("显示物品名称",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDShowPlayerName = new CCtrlRadio();
		AddControl(m_pRDShowPlayerName);
		m_pRDShowPlayerName->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP,125,126,127,128,110,20);
		m_pRDShowPlayerName->SetText("显示玩家名字",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDShowArrow = new CCtrlRadio();
		AddControl(m_pRDShowArrow);
		m_pRDShowArrow->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP * 2,125,126,127,128,110,20);
		m_pRDShowArrow->SetText("突出玩家自己",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDShowSysInfo = new CCtrlRadio();
		AddControl(m_pRDShowSysInfo);
		m_pRDShowSysInfo->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP * 3,125,126,127,128,110,20);
		m_pRDShowSysInfo->SetText("显示系统信息",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDShowQuickBuy = new CCtrlRadio();
		AddControl(m_pRDShowQuickBuy);
		m_pRDShowQuickBuy->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP * 4,125,126,127,128,110,20);
		m_pRDShowQuickBuy->SetText("快捷消费提醒",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDShowNpcPriority = new CCtrlRadio();
		AddControl(m_pRDShowNpcPriority);
		m_pRDShowNpcPriority->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP * 5,125,126,127,128,110,20);
		m_pRDShowNpcPriority->SetText("突出显示NPC",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDShowNewHandPrompt = new CCtrlRadio();
		AddControl(m_pRDShowNewHandPrompt);
		m_pRDShowNewHandPrompt->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP * 6,125,126,127,128,110,20);
		m_pRDShowNewHandPrompt->SetText("开启新手提示",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		
		m_pRDShowHpChange = new CCtrlRadio();
		AddControl(m_pRDShowHpChange);
		m_pRDShowHpChange->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP * 7,125,126,127,128,110,20);
		m_pRDShowHpChange->SetText("开启伤害/恢复提示",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAcceptFrientTrade = new CCtrlRadio();
		AddControl(m_pRDAcceptFrientTrade);
		m_pRDAcceptFrientTrade->Create(this,BASECOL1,BASEROW1 + BASE_LINE_GAP * 8,125,126,127,128,110,20);
		m_pRDAcceptFrientTrade->SetText("接受加好友/交易请求",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		//战斗辅助
		m_pRDQuickOutGame = new CCtrlRadio();
		AddControl(m_pRDQuickOutGame);
		m_pRDQuickOutGame->Create(this,BASECOL2,BASEROW1,125,126,127,128,110,20);
		m_pRDQuickOutGame->SetText("快速小退",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDCloseTeam= new CCtrlRadio();
		AddControl(m_pRDCloseTeam);
		m_pRDCloseTeam->Create(this,BASECOL2,BASEROW1 + BASE_LINE_GAP,125,126,127,128,110,20);
		m_pRDCloseTeam->SetText("关闭编组",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDDurationPrompt = new CCtrlRadio();
		AddControl(m_pRDDurationPrompt);
		m_pRDDurationPrompt->Create(this,BASECOL2,BASEROW1 + BASE_LINE_GAP * 2,125,126,127,128,110,20);
		m_pRDDurationPrompt->SetText("持久警告",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDTeamColor = new CCtrlRadio();
		AddControl(m_pRDTeamColor);
		m_pRDTeamColor->Create(this,BASECOL2,BASEROW1 + BASE_LINE_GAP * 3,125,126,127,128,110,20);
		m_pRDTeamColor->SetText("队友变色",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//m_pRDDismount = new CCtrlRadio();
		//AddControl(m_pRDDismount);
		//m_pRDDismount->Create(this,BASECOL2,BASEROW1 + BASE_LINE_GAP * 4,125,126,127,128,110,20);
		//m_pRDDismount->SetText("自动下马",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		if (g_pStreamMgr->GetConfigInt("EnableAutoKillMonster",1) == 1)
		{
			m_pRDAutoKill = new CCtrlRadio();
			AddControl(m_pRDAutoKill);
			m_pRDAutoKill->Create(this,BASECOL2,BASEROW1 + BASE_LINE_GAP * 4,125,126,127,128,110,20);
			m_pRDAutoKill->SetText("自动打怪",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		}
		else
		{
			m_pRDAutoKill = NULL;
		}

		//其他辅助
		m_pRDAutoFindWay = new CCtrlRadio();
		AddControl(m_pRDAutoFindWay);
		m_pRDAutoFindWay->Create(this,BASECOL3,BASEROW1,125,126,127,128,110,20);
		m_pRDAutoFindWay->SetText("智能寻路",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDGoPickup= new CCtrlRadio();
		AddControl(m_pRDGoPickup);
		m_pRDGoPickup->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP,125,126,127,128,110,20);
		m_pRDGoPickup->SetText("拾取极品",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAutoPickup = new CCtrlRadio();
		AddControl(m_pRDAutoPickup);
		m_pRDAutoPickup->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 2,125,126,127,128,110,20);
		m_pRDAutoPickup->SetText("自动捡物",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDShowMagicTime = new CCtrlRadio();
		AddControl(m_pRDShowMagicTime);
		m_pRDShowMagicTime->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 3,125,126,127,128,110,20);
		m_pRDShowMagicTime->SetText("法力提示",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDBury = new CCtrlRadio();
		AddControl(m_pRDBury);
		m_pRDBury->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 4,125,126,127,128,110,20);
		m_pRDBury->SetText("掩埋尸体",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDMountain = new CCtrlRadio();
		AddControl(m_pRDMountain);
		m_pRDMountain->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 5,125,126,127,128,110,20);
		m_pRDMountain->SetText("忽略受伤动作",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDStarObject = new CCtrlRadio();
		AddControl(m_pRDStarObject);
		m_pRDStarObject->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 6,125,126,127,128,110,20);
		m_pRDStarObject->SetText("闪烁物品",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAutoMogong = new CCtrlRadio();
		AddControl(m_pRDAutoMogong);
		m_pRDAutoMogong->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 7,125,126,127,128,110,20);
		m_pRDAutoMogong->SetText("自动点击魔环",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAllowFly = new CCtrlRadio();
		AddControl(m_pRDAllowFly);
		m_pRDAllowFly->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 8,125,126,127,128,110,20);
		m_pRDAllowFly->SetText("瞬移抢物",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDRunNotStop = new CCtrlRadio();
		AddControl(m_pRDRunNotStop);
		m_pRDRunNotStop->Create(this,BASECOL3,BASEROW1 + BASE_LINE_GAP * 9,125,126,127,128,110,20);
		m_pRDRunNotStop->SetText("跑步不停",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//大页签//////////////////////////////////////////////////////////////////////////////
	}
	else if(m_iCurPage == PAGE_PROTECT)
	{
		char temp_str[64] = {0};
		for(int i = 0; i < MAX_MEDICA_NUMBER;i++)
		{
			WOOOLMEDICAEAT * pMedicaInfo = g_AutoEatMgr.GetHPMPMedicaInfo(i);
			if (!pMedicaInfo)
			{
				break;
			}

			m_pRDUseHPMedia[i] = new CCtrlRadio();
			AddControl(m_pRDUseHPMedia[i]);
			m_pRDUseHPMedia[i]->Create(this,56,106+i*21,125,126,127,128,110,20);
			m_pRDUseHPMedia[i]->SetText(pMedicaInfo->sHPName.c_str(),COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

			m_pEdHPMediaLimit[i] = new CCtrlEdit();
			AddControl(m_pEdHPMediaLimit[i]);
			m_pEdHPMediaLimit[i]->CreateEx(this,165,108 + i*21,42,18);
			m_pEdHPMediaLimit[i]->SetDigital(true);
			m_pEdHPMediaLimit[i]->SetMaxLength(4);

			sprintf(temp_str,"%d",pMedicaInfo->hp_limit);
			m_pEdHPMediaLimit[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

			m_pEdHPMediaDelay[i] = new CCtrlEdit();
			AddControl(m_pEdHPMediaDelay[i]);
			m_pEdHPMediaDelay[i]->CreateEx(this,215,108 + i*21,42,18);
			m_pEdHPMediaDelay[i]->SetDigital(true);
			m_pEdHPMediaDelay[i]->SetMaxLength(4);

			sprintf(temp_str,"%d",pMedicaInfo->hp_delay);
			m_pEdHPMediaDelay[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);


			m_pRDUseMPMedia[i] = new CCtrlRadio();
			AddControl(m_pRDUseMPMedia[i]);
			m_pRDUseMPMedia[i]->Create(this,289,106+i*21,125,126,127,128,110,20);
			m_pRDUseMPMedia[i]->SetText(pMedicaInfo->sMPName.c_str(),COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

			m_pEdMPMediaLimit[i] = new CCtrlEdit();
			AddControl(m_pEdMPMediaLimit[i]);
			m_pEdMPMediaLimit[i]->CreateEx(this,399,108 + i*21,42,18);
			m_pEdMPMediaLimit[i]->SetDigital(true);
			m_pEdMPMediaLimit[i]->SetMaxLength(4);

			sprintf(temp_str,"%d",pMedicaInfo->mp_limit);
			m_pEdMPMediaLimit[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

			m_pEdMPMediaDelay[i] = new CCtrlEdit();
			AddControl(m_pEdMPMediaDelay[i]);
			m_pEdMPMediaDelay[i]->CreateEx(this,449,108 + i*21,42,18);
			m_pEdMPMediaDelay[i]->SetDigital(true);
			m_pEdMPMediaDelay[i]->SetMaxLength(4);

			sprintf(temp_str,"%d",pMedicaInfo->mp_delay);
			m_pEdMPMediaDelay[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

		}

		m_pRDUsePTSuperHP = new CCtrlRadio();
		AddControl(m_pRDUsePTSuperHP);
		m_pRDUsePTSuperHP->Create(this,56,298,125,126,127,128,110,20);
		m_pRDUsePTSuperHP->SetText("持续使用天山雪莲/超级天山雪莲",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDUsePTSuperMP = new CCtrlRadio();
		AddControl(m_pRDUsePTSuperMP);
		m_pRDUsePTSuperMP->Create(this,289,298,125,126,127,128,110,20);
		m_pRDUsePTSuperMP->SetText("持续使用深海灵礁/超级深海灵礁",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		////其他保护方式
		//m_pEdAutoAction = new CCtrlEdit();
		//AddControl(m_pEdAutoAction);
		//m_pEdAutoAction->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,89,346,65,16);
		//m_pEdAutoAction->SetEnable(false);

		//菜单
		m_pMBAutoAction = new CCtrlMenuButton();
		AddControl(m_pMBAutoAction);
		m_pMBAutoAction->Create(this,50,336,100,120);
		m_pMBAutoAction->SetMenuTextColor(0xFFFFFFFF);
		m_pMBAutoAction->SetFont(FONT_YAHEI);
		m_pMBAutoAction->AddString("无");
		m_pMBAutoAction->AddString("自动随机");
		m_pMBAutoAction->AddString("自动地牢");
		m_pMBAutoAction->AddString("自动回城");
		m_pMBAutoAction->AddString("自动回城石");
		//m_pMBAutoAction->AddString("随机神石");
		m_pMBAutoAction->AddString("回城神石");
		m_pMBAutoAction->SetCurSel(g_AICfgMgr.GetAutoAction());

		//m_pEdAutoAction->SetText(m_pMBAutoAction->GetSelText(),COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

		//自动保护HP下限制
		m_pEdAutoActionLimit = new CCtrlEdit();
		AddControl(m_pEdAutoActionLimit);
		m_pEdAutoActionLimit->CreateEx(this,216,334,42,18);
		m_pEdAutoActionLimit->SetDigital(true);
		m_pEdAutoActionLimit->SetMaxLength(4);
		m_pEdAutoActionLimit->SetFocus();

		sprintf(temp_str,"%d",g_AICfgMgr.GetAutoActionLimit());
		m_pEdAutoActionLimit->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

		if((g_dwServerSwitch & SS_QUIT_DELAY) == 0)
		{
			//自动小退
			m_pRDAutoQuit = new CCtrlRadio();
			AddControl(m_pRDAutoQuit);
			m_pRDAutoQuit->Create(this,290,334,125,126,127,128,110,20);
			m_pRDAutoQuit->SetText("自动退出",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

			//自动小退下限
			m_pEdAutoQuitLimit = new CCtrlEdit();
			AddControl(m_pEdAutoQuitLimit);
			m_pEdAutoQuitLimit->CreateEx(this,450,334,42,18);
			m_pEdAutoQuitLimit->SetDigital(true);
			m_pEdAutoQuitLimit->SetMaxLength(4);

			sprintf(temp_str,"%d",g_AICfgMgr.GetAutoQuitLimit());
			m_pEdAutoQuitLimit->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		}

		////灵兽微俄时自动喂食兽粮
		//m_pRDAutoFeed = new CCtrlRadio();
		//AddControl(m_pRDAutoFeed);
		//m_pRDAutoFeed->Create(this,56,336,125,126,127,128,110,20);
		//m_pRDAutoFeed->SetText("灵兽被放出微饿时自动喂食兽粮",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	
		m_pDealutEatCfg = new CCtrlButton();
		AddControl(m_pDealutEatCfg);
		m_pDealutEatCfg->CreateEx(this, 237,58, 90, 91, 92, 93);
		m_pDealutEatCfg->SetText("推荐配置", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);


    }
	else if(m_iCurPage == PAGE_FIGHT)
	{
		//大页签//////////////////////////////////战斗////////////////////////////////////////

		//战士辅助
		m_pRDAutoProtectSkin = new CCtrlRadio();
		AddControl(m_pRDAutoProtectSkin);
		m_pRDAutoProtectSkin->Create(this,FIGHTCOL1,FIGHTROW1,125,126,127,128,110,20);
		m_pRDAutoProtectSkin->SetText("持续护身真气",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAutoSteelProtect = new CCtrlRadio();
		AddControl(m_pRDAutoSteelProtect);
		m_pRDAutoSteelProtect->Create(this,FIGHTCOL1,FIGHTROW1+21,125,126,127,128,110,20);
		m_pRDAutoSteelProtect->SetText("持续金刚护体",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		
		m_pRDAutoFire = new CCtrlRadio();
		AddControl(m_pRDAutoFire);
		m_pRDAutoFire->Create(this,FIGHTCOL1,FIGHTROW1+42,125,126,127,128,110,20);
		m_pRDAutoFire->SetText("自动烈火",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDDestroyShield = new CCtrlRadio();
		AddControl(m_pRDDestroyShield);
		m_pRDDestroyShield->Create(this,FIGHTCOL1,FIGHTROW1+63,125,126,127,128,110,20);
		m_pRDDestroyShield->SetText("持续破盾",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAutoSuperAttack = new CCtrlRadio();
		AddControl(m_pRDAutoSuperAttack);
		m_pRDAutoSuperAttack->Create(this,FIGHTCOL1,FIGHTROW1+84,125,126,127,128,110,20);
		m_pRDAutoSuperAttack->SetText("自动强化攻杀",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//
		m_pRDAutoWildCollide = new CCtrlRadio();
		AddControl(m_pRDAutoWildCollide);
		m_pRDAutoWildCollide->Create(this,FIGHTCOL2,FIGHTROW1,125,126,127,128,110,20);
		m_pRDAutoWildCollide->SetText("自动突斩",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDDirWild = new CCtrlRadio();
		AddControl(m_pRDDirWild);
		m_pRDDirWild->Create(this,FIGHTCOL2,FIGHTROW1+21,125,126,127,128,110,20);
		m_pRDDirWild->SetText("定向突斩",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDDoubleFire = new CCtrlRadio();
		AddControl(m_pRDDoubleFire);
		m_pRDDoubleFire->Create(this,FIGHTCOL2,FIGHTROW1+42,125,126,127,128,110,20);
		m_pRDDoubleFire->SetText("自动双烈火",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDDestroyShell = new CCtrlRadio();
		AddControl(m_pRDDestroyShell);
		m_pRDDestroyShell->Create(this,FIGHTCOL2,FIGHTROW1+63,125,126,127,128,110,20);
		m_pRDDestroyShell->SetText("持续破击",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//法师辅助
		m_pRDAutoMagicProtect = new CCtrlRadio();
		AddControl(m_pRDAutoMagicProtect);
		m_pRDAutoMagicProtect->Create(this,FIGHTCOL1,FIGHTROW2,125,126,127,128,110,20);
		m_pRDAutoMagicProtect->SetText("持续魔法盾",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAutoDodgeSkill = new CCtrlRadio();
		AddControl(m_pRDAutoDodgeSkill);
		m_pRDAutoDodgeSkill->Create(this,FIGHTCOL2,FIGHTROW2,125,126,127,128,110,20);
		m_pRDAutoDodgeSkill->SetText("持续风影盾",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAutoDispute = new CCtrlRadio();
		AddControl(m_pRDAutoDispute);
		m_pRDAutoDispute->Create(this,FIGHTCOL1,FIGHTROW2+21,125,126,127,128,110,20);
		m_pRDAutoDispute->SetText("自动抗拒",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//道士辅助
		m_pRDAutoProtectGhost = new CCtrlRadio();
		AddControl(m_pRDAutoProtectGhost);
		m_pRDAutoProtectGhost->Create(this,FIGHTCOL1,FIGHTROW3,125,126,127,128,110,20);
		m_pRDAutoProtectGhost->SetText("持续幽灵盾",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDSmartLionCall = new CCtrlRadio();
		AddControl(m_pRDSmartLionCall);
		m_pRDSmartLionCall->Create(this,FIGHTCOL2,FIGHTROW3,125,126,127,128,110,20);
		m_pRDSmartLionCall->SetText("定向狮子吼",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		
		m_pRDAutoProtectArmor = new CCtrlRadio();
		AddControl(m_pRDAutoProtectArmor);
		m_pRDAutoProtectArmor->Create(this,FIGHTCOL1,FIGHTROW3+21,125,126,127,128,110,20);
		m_pRDAutoProtectArmor->SetText("持续神圣战甲术",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);


		m_pRDAutoLionCall = new CCtrlRadio();
		AddControl(m_pRDAutoLionCall);
		m_pRDAutoLionCall->Create(this,FIGHTCOL2,FIGHTROW3+21,125,126,127,128,110,20);
		m_pRDAutoLionCall->SetText("自动狮子吼",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//公共辅助
		m_pRDAntiAttack = new CCtrlRadio();
		AddControl(m_pRDAntiAttack);
		m_pRDAntiAttack->Create(this,FIGHTCOL3,FIGHTROW1,125,126,127,128,110,20);
		m_pRDAntiAttack->SetText("自动反击",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDAutoSkill = new CCtrlRadio();
		AddControl(m_pRDAutoSkill);
		m_pRDAutoSkill->Create(this,FIGHTCOL3,FIGHTROW1 + 21,125,126,127,128,110,20);
		m_pRDAutoSkill->SetText("自动练功",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		////自动练功的键
		//m_pEdAutoSkillKey = new CCtrlEdit();
		//AddControl(m_pEdAutoSkillKey);
		//m_pEdAutoSkillKey->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,380,187,50,16);
		//m_pEdAutoSkillKey->SetEnable(false);

		char temp_str[64] = {0};

		//if(g_AICfgMgr.GetAutoSkillKey() > 0x30)
		//	sprintf(temp_str,"F%d",g_AICfgMgr.GetAutoSkillKey() - 0x30);
		//else
		//	strcpy(temp_str,"");
		//m_pEdAutoSkillKey->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);


		m_pMBAutoSkillKey = new CCtrlMenuButton();//
		AddControl(m_pMBAutoSkillKey);
		m_pMBAutoSkillKey->Create(this,372,137,98,140);
		m_pMBAutoSkillKey->SetFont(FONT_YAHEI);
		m_pMBAutoSkillKey->SetMenuTextColor(0xFFFFFFFF);

		for(int i = 0; i < 8; i++)
		{
			sprintf(temp_str,"F%d",i + 1);
			m_pMBAutoSkillKey->AddString(temp_str);

			if (g_AICfgMgr.GetAutoSkillKey() - 0x30 == i + 1)
			{
				m_pMBAutoSkillKey->SetCurSel(i);
			}
		}

		//间隔时间
		m_pEdAutoSkillTime = new CCtrlEdit();
		AddControl(m_pEdAutoSkillTime);
		m_pEdAutoSkillTime->CreateEx(this,373,217,116,18);
		sprintf(temp_str,"%d",g_AICfgMgr.GetAutoSkillTime());
		m_pEdAutoSkillTime->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pEdAutoSkillTime->SetDigital(true);
		m_pEdAutoSkillTime->SetMaxLength(9);
	}
	else if(m_iCurPage == PAGE_ITEM)
	{
		m_pEdItemName = new CCtrlEdit();
		AddControl(m_pEdItemName);
		m_pEdItemName->CreateEx(this,45,343,115,18);
		m_pEdItemName->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pEdItemName->SetMaxLength(30);

		m_pAddItemButton = new CCtrlButton();
		AddControl(m_pAddItemButton);
		m_pAddItemButton->CreateEx(this, 165,343, 95, 96, 97, 98);
		m_pAddItemButton->SetText("新增", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pDeleteItemButton = new CCtrlButton();
		AddControl(m_pDeleteItemButton);
		m_pDeleteItemButton->CreateEx(this, 224,343, 95, 96, 97, 98);
		m_pDeleteItemButton->SetText("删除", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pMBBestItem = new CCtrlMenuButton();
		AddControl(m_pMBBestItem);
		m_pMBBestItem->SetVAlignTop(true);
		m_pMBBestItem->Create(this,306,342,36,140,0,0,0,0,0,1030,1031,1032,1033,1,18,true);
		m_pMBBestItem->SetFont(FONT_YAHEI);

		m_pMBExpensiveItem = new CCtrlMenuButton();
		AddControl(m_pMBExpensiveItem);
		m_pMBExpensiveItem->SetVAlignTop(true);
		m_pMBExpensiveItem->Create(this,360,342,36,140,0,0,0,0,0,1030,1031,1032,1033,1,18,true);
		m_pMBExpensiveItem->SetFont(FONT_YAHEI);

		m_pMBPickupItem = new CCtrlMenuButton();
		AddControl(m_pMBPickupItem);
		m_pMBPickupItem->SetFont(FONT_YAHEI);
		m_pMBPickupItem->Create(this,415,342,36,140,0,0,0,0,0,1030,1031,1032,1033,1,18,true);
		m_pMBPickupItem->SetVAlignTop(true);

		m_pMBNormItem = new CCtrlMenuButton();
		AddControl(m_pMBNormItem);
		m_pMBNormItem->SetVAlignTop(true);
		m_pMBNormItem->Create(this,470,342,36,140,0,0,0,0,0,1030,1031,1032,1033,1,18,true);
		m_pMBNormItem->SetFont(FONT_YAHEI);

		//列表控件
		m_pGrid = new CCtrlGrid();
		AddControl(m_pGrid);
		m_pGrid->Create(this,46,90,506,334,12,20.0f);
		m_pGrid->AddScrollEx(444,0,16,244);
		m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);
		m_pGrid->SetTextColor(COLOR_TEXT_NORMAL,0xFFFF0000);


		m_pGrid->SetSelAllCols(false);
		m_pGrid->PushColumn(120);
		m_pGrid->PushColumn(80);
		m_pGrid->PushColumn(80);
		m_pGrid->PushColumn(80);
		m_pGrid->PushColumn(80);
		m_pGrid->SetTotalCount(g_PickCfgMgr.GetItemClassCount());
		m_pGrid->SetDrawOffXY(0,6);
	}
	else if(m_iCurPage == PAGE_MAGIC)
	{
		//魔法列表控件
		m_pGrid = new CCtrlGrid();
		AddControl(m_pGrid);
		m_pGrid->Create(this,46,90,504,316,11,20.0f);
		m_pGrid->AddScrollEx(444,0,16,226);
		m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);
		m_pGrid->SetTextColor(COLOR_TEXT_NORMAL,0xFFFF0000);

		m_pGrid->SetSelAllCols(false);
		m_pGrid->PushColumn(120);
		m_pGrid->PushColumn(80);
		m_pGrid->PushColumn(80);
		m_pGrid->PushColumn(80);
		m_pGrid->PushColumn(80);
		m_pGrid->SetDrawOffXY(0,6);

		//计算主角的魔法总数
		m_pGrid->SetTotalCount(SELF.MagicArray().GetMagicNumber());
	}
	else if(m_iCurPage == PAGE_MONSTER)
	{
 		m_pEdMonsterName = new CCtrlEdit();
		AddControl(m_pEdMonsterName);
		m_pEdMonsterName->CreateEx(this,45,344,115,18);
		m_pEdMonsterName->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pEdMonsterName->SetMaxLength(30);

		m_pAddMonsterButton = new CCtrlButton();
		AddControl(m_pAddMonsterButton);
		m_pAddMonsterButton->CreateEx(this, 165,343, 95, 96, 97, 98);
		m_pAddMonsterButton->SetText("新增", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pDeleteMonsterButton = new CCtrlButton();
		AddControl(m_pDeleteMonsterButton);
		m_pDeleteMonsterButton->CreateEx(this, 224,343, 95, 96, 97, 98);
		m_pDeleteMonsterButton->SetText("删除", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		int iGap = 32;
		m_pRDBossPrompt = new CCtrlRadio();
		AddControl(m_pRDBossPrompt);
		m_pRDBossPrompt->Create(this,MONSTCOL1,MONSTERROW1,125,126,127,128,110,20);
		m_pRDBossPrompt->SetText("BOSS提醒",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDObjectPrompt = new CCtrlRadio();
		AddControl(m_pRDObjectPrompt);
		m_pRDObjectPrompt->Create(this,MONSTCOL1,MONSTERROW1 + iGap,125,126,127,128,110,20);
		m_pRDObjectPrompt->SetText("极品提醒",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDPkAllAttack = new CCtrlRadio();
		AddControl(m_pRDPkAllAttack);
		m_pRDPkAllAttack->Create(this,MONSTCOL1,MONSTERROW1 + iGap * 2,125,126,127,128,110,20);
		m_pRDPkAllAttack->SetText("PK变全攻",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDBlackFilter = new CCtrlRadio();
		AddControl(m_pRDBlackFilter);
		m_pRDBlackFilter->Create(this,MONSTCOL1,MONSTERROW1 + iGap * 3,125,126,127,128,110,20);
		m_pRDBlackFilter->SetText("黑名过滤",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		m_pRDPkAlarm = new CCtrlRadio();
		AddControl(m_pRDPkAlarm);
		m_pRDPkAlarm->Create(this,MONSTCOL1,MONSTERROW1 + iGap * 4,125,126,127,128,110,20);
		m_pRDPkAlarm->SetText("PK加仇敌",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//怪物列表控件
		m_pGrid = new CCtrlGrid();
		AddControl(m_pGrid);
		m_pGrid->Create(this,46,90,345,336,12,20.0f);
		m_pGrid->AddScrollEx(286,0,16,246);
		m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);
		m_pGrid->SetTextColor(COLOR_TEXT_NORMAL,0xFFFF0000);
		m_pGrid->PushColumn(157);
		m_pGrid->PushColumn(62);
		m_pGrid->PushColumn(61);
		m_pGrid->SetTotalCount(g_BossCfgMgr.GetBossClassCount());
		m_pGrid->SetSelAllCols(false);
		m_pGrid->SetDrawOffXY(0,6);
	}
	else if(m_iCurPage == PAGE_CHAT)
	{
		//自动发言
		m_pRDAutoSay = new CCtrlRadio();
		AddControl(m_pRDAutoSay);
		m_pRDAutoSay->Create(this,58,69,125,126,127,128,110,20);
		m_pRDAutoSay->SetText("自动发言",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//自动发言时间间隔
		m_pEdAutoSayTimer = new CCtrlEdit();
		AddControl(m_pEdAutoSayTimer);
		m_pEdAutoSayTimer->CreateEx(this,280,69,35,18);
		m_pEdAutoSayTimer->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pEdAutoSayTimer->SetDigital(true);
		m_pEdAutoSayTimer->SetMaxLength(5);

		char temp_str[64] = {0};
		sprintf(temp_str,"%d",g_AICfgMgr.GetAutoSayTimer());
		m_pEdAutoSayTimer->SetText(temp_str);

		//自动发言内容
		m_pAutoSayMsg = new CCtrlMultiEdit(100,63,4);
		AddControl(m_pAutoSayMsg);
		m_pAutoSayMsg->Create(this,FONTSIZE_SMALL,80,126,465 - 80,217 - 126);
		m_pAutoSayMsg->SetLineHeight(16);
		m_pAutoSayMsg->SetText(g_AICfgMgr.GetAutoSay().c_str(),FONT_YAHEI,FONTSIZE_SMALL);

		//自动回复?
		m_pRDAutoReply = new CCtrlRadio();
		AddControl(m_pRDAutoReply);
		m_pRDAutoReply->Create(this,58,335,125,126,127,128,110,20);
		m_pRDAutoReply->SetText("自动回复",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//自动回复内容
		m_pEdAutoReplyMsg = new CCtrlEdit();
		AddControl(m_pEdAutoReplyMsg);
		m_pEdAutoReplyMsg->CreateEx(this,147,338,490 - 147,18);
		m_pEdAutoReplyMsg->SetText(g_AICfgMgr.GetReplyMsg().c_str(),COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pEdAutoReplyMsg->SetMaxLength(200);


		//来言发声
		m_pRDMessageAwoke = new CCtrlRadio();
		AddControl(m_pRDMessageAwoke);
		m_pRDMessageAwoke->Create(this,58,308,125,126,127,128,110,20);
		m_pRDMessageAwoke->SetText("来言发声",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//屏蔽陌生人喊话与私聊信息
		m_pKeepOutInfo = new CCtrlRadio();
		AddControl(m_pKeepOutInfo);
		m_pKeepOutInfo->Create(this,200,308,125,126,127,128,110,20);
		m_pKeepOutInfo->SetText("不显示陌生人私聊内容",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	}
	else if(m_iCurPage == PAGE_RECORD)
	{
		m_pRDRecordMsg = new CCtrlRadio();
		AddControl(m_pRDRecordMsg);
		m_pRDRecordMsg->Create(this,60,340,125,126,127,128,110,20);
		m_pRDRecordMsg->SetText("记录私聊",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

		//记录的信息列表控件
		m_pTalkViewer = new CTalkViewer();
		AddControl(m_pTalkViewer);
		m_pTalkViewer->Create(this,50,70,458,259);
		m_pTalkViewer->SetTalkType(&g_TalkMgr.GetAssPrivateTalk());
		m_pTalkViewer->AddScrollEx(447,0,16,259);

		m_pClearMsgButton = new CCtrlButton();
		AddControl(m_pClearMsgButton);
		m_pClearMsgButton->CreateEx(this, 424,340, 132, 133, 134, 135);
		m_pClearMsgButton->SetText("清空记录", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	}
	else if(m_iCurPage == PAGE_README)
	{
		//帮助信息列表控件
		m_pGrid = new CCtrlGrid();
		AddControl(m_pGrid);
		m_pGrid->Create(this,49,69,508,357,13,20.3f);
		m_pGrid->AddScrollEx(444,0,16,287);
		m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

		m_pGrid->PushColumn(324);
		m_pGrid->SetSelAllCols(false);
		m_pGrid->SetTotalCount(g_AICfgMgr.GetReadmeCount());
	}


	CCtrlWindowX::OnCreate();
}

void CAssConfigWnd::SetEatConfig()
{
	char temp_str[64] = {0};
	for(int i = 0; i < MAX_MEDICA_NUMBER;i++)
	{
		WOOOLMEDICAEAT * pMedicaInfo = g_AutoEatMgr.GetHPMPMedicaInfo(i);
		if (!pMedicaInfo)
		{
			break;
		}

		sprintf(temp_str,"%d",pMedicaInfo->hp_limit);
		m_pEdHPMediaLimit[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

		sprintf(temp_str,"%d",pMedicaInfo->hp_delay);
		m_pEdHPMediaDelay[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

		sprintf(temp_str,"%d",pMedicaInfo->mp_limit);
		m_pEdMPMediaLimit[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

		sprintf(temp_str,"%d",pMedicaInfo->mp_delay);
		m_pEdMPMediaDelay[i]->SetText(temp_str,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	}
}


bool CAssConfigWnd::Msg(DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if (pControl)
			{
				S_TabPage * pParentTabpage = (S_TabPage * )pControl;
				if (pParentTabpage == &m_TabPage && dwData == 1)//点了游戏设置页签
				{
					CloseWindow();
					g_pControl->PopupWindow(MSG_CTRL_SYSCONFIG_WND,OPER_CREATE);
					return true;
				}
			}
		}
		break;
	case MSG_CTRL_TALKVIEW_LCLICK:
		{
			string strLine;
			if(m_iCurPage == PAGE_RECORD && m_pTalkViewer->getLineHeader(strLine))
				g_pControl->Msg(MSG_CTRL_INSERT_TEXT,2,(CControl *)strLine.c_str());
			break;
		}
	case MSG_CTRL_EDIT_ENTRY:
		{
			if(m_iCurPage == PAGE_ITEM)
			{
				if(pControl == m_pEdItemName)
				{
					this->Msg(MSG_CTRL_BUTTON_CLICK,0,m_pAddItemButton);
					return true;
				}
			}
			else if(m_iCurPage == PAGE_MONSTER)
			{
				if(pControl == m_pEdMonsterName)
				{
					this->Msg(MSG_CTRL_BUTTON_CLICK,0,m_pAddMonsterButton);
					return true;
				}
			}
		}
		break;
	case MSG_CTRL_EDIT_ONCHAR:

		if(m_iCurPage == PAGE_FIGHT)
		{
			if(pControl == m_pEdAutoSkillTime)
			{
				const char* szTitle = m_pEdAutoSkillTime->GetText();
				if(szTitle)
					g_AICfgMgr.SetAutoSkillTime(atoi(szTitle));

				return true;
			}
		}
		else if(m_iCurPage == PAGE_PROTECT)
		{
			if(pControl == m_pEdAutoActionLimit)
			{
				const char* szTitle = m_pEdAutoActionLimit->GetText();
				if(szTitle)
					g_AICfgMgr.SetAutoActionLimit(atoi(szTitle));
				return true;
			}
			else if(pControl == m_pEdAutoQuitLimit)
			{
				const char* szTitle = m_pEdAutoQuitLimit->GetText();
				if(szTitle)
					g_AICfgMgr.SetAutoQuitLimit(atoi(szTitle));
				return true;
			}
			else
			{
				for(int i = 0;i < MAX_MEDICA_NUMBER;i++)
				{
					if(pControl == m_pEdHPMediaLimit[i])
					{
						const char* szTitle = m_pEdHPMediaLimit[i]->GetText();
						if(szTitle)
							g_AutoEatMgr.SetHPMediaLimit(i,atoi(szTitle));
						return true;

					}
					else if(pControl == m_pEdHPMediaDelay[i])
					{
						const char* szTitle = m_pEdHPMediaDelay[i]->GetText();
						if(szTitle)
							g_AutoEatMgr.SetHPMediaDelay(i,atoi(szTitle));
						return true;

					}
					else if(pControl == m_pEdMPMediaLimit[i])
					{
						const char* szTitle = m_pEdMPMediaLimit[i]->GetText();
						if(szTitle)
							g_AutoEatMgr.SetMPMediaLimit(i,atoi(szTitle));
						return true;

					}
					else if(pControl == m_pEdMPMediaDelay[i])
					{
						const char* szTitle = m_pEdMPMediaDelay[i]->GetText();
						if(szTitle)
							g_AutoEatMgr.SetMPMediaDelay(i,atoi(szTitle));
						return true;
					}
				}
			}
		}
		else if(m_iCurPage == PAGE_CHAT)
		{
			if(pControl == m_pEdAutoSayTimer)
			{
				const char* szTitle = m_pEdAutoSayTimer->GetText();
				if(szTitle)
					g_AICfgMgr.SetAutoSayTimer(atoi(szTitle));
				return true;
			}
			else if(pControl == m_pEdAutoReplyMsg)
			{
				const char* szTitle = m_pEdAutoReplyMsg->GetText();
				if(szTitle)
					g_AICfgMgr.SetAutoReplyMsg(szTitle);
			}
		}

		break;
	case MSG_CTRL_MENU_SELCHANGED:

		if(m_iCurPage == PAGE_ITEM)
		{
			if(pControl == m_pMBBestItem)
			{
				DWORD dwSelColor = m_pMBBestItem->GetSelColor();
				if(dwSelColor != 0)
				{
					g_AICfgMgr.SetBestItemColor(dwSelColor);
				}
				return true;
			}
			else if(pControl == m_pMBExpensiveItem)
			{
				DWORD dwSelColor = m_pMBExpensiveItem->GetSelColor();
				if(dwSelColor != 0)
				{
					g_AICfgMgr.SetExpensiveItemColor(dwSelColor);
				}
				return true;
			}
			else if(pControl == m_pMBPickupItem)
			{
				DWORD dwSelColor = m_pMBPickupItem->GetSelColor();
				if(dwSelColor != 0)
				{
					g_AICfgMgr.SetPickItemColor(dwSelColor);
				}
				return true;
			}
			else if(pControl == m_pMBNormItem)
			{
				DWORD dwSelColor = m_pMBNormItem->GetSelColor();
				if(dwSelColor != 0)
				{
					g_AICfgMgr.SetShowItemColor(dwSelColor);
				}
				return true;
			}
		}
		else if(m_iCurPage == PAGE_FIGHT)
		{
			if(pControl == m_pMBAutoSkillKey)
			{
				int iCurSel = m_pMBAutoSkillKey->GetCurSel();
				if(iCurSel >= 0)
					g_AICfgMgr.SetAutoSkillKey(0x31 + iCurSel);

				//m_pEdAutoSkillKey->SetText(m_pMBAutoSkillKey->GetSelText());

				return true;
			}
		}
		else if(m_iCurPage == PAGE_PROTECT)
		{
			if(pControl == m_pMBAutoAction)
			{
				int iCurSel = m_pMBAutoAction->GetCurSel();
				if(iCurSel >= 0)
					g_AICfgMgr.SetAutoAction(iCurSel);

				//m_pEdAutoAction->SetText(m_pMBAutoAction->GetSelText());
			}
		}

		break;
	case MSG_CTRL_RECORD_ADDPRIVATEMSG:
		{
			if(m_iCurPage == PAGE_RECORD && m_pTalkViewer)
			{
				m_pTalkViewer->WheelToEnd();
			}
		}
		return true;
	case MSG_CTRL_BUTTON_CLICK:

		if(pControl == m_pCloseButton)
		{
			return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
		}
		if(m_iCurPage == PAGE_BASE)
		{
			if(pControl == m_pRDShowObjectName)
			{
				g_AICfgMgr.SetShowObjectName(!g_AICfgMgr.IsShowObjectName());
				return true;
			}
			else if(pControl == m_pRDShowPlayerName)
			{
				g_AICfgMgr.SetShowPlayerName(!g_AICfgMgr.IsShowPlayerName());
				return true;
			}
			else if(pControl == m_pRDShowArrow)
			{
				g_AICfgMgr.SetShowArrow(!g_AICfgMgr.IsShowArrow());
				return true;
			}
			else if(pControl == m_pRDShowSysInfo)
			{
				g_AICfgMgr.SetShowSysInfo(!g_AICfgMgr.IsShowSysInfo());
				return true;
			}
			else if(pControl == m_pRDShowQuickBuy)
			{
				g_Config.SetCheckQuick(!g_Config.GetCheckQuick());
				return true;
			}
			else if(pControl == m_pRDShowNpcPriority)
			{
				g_AICfgMgr.SetShowNpcPriority(!g_AICfgMgr.IsShowNpcPriority());
				return true;
			}
			else if (pControl == m_pRDShowNewHandPrompt)
			{
				g_AICfgMgr.SetShowNewHandPrompt(!g_AICfgMgr.IsShowNewHandPrompt());
				return true;
			}
			else if (pControl == m_pRDShowHpChange)
			{
				g_AICfgMgr.SetShowHpChangeEffect(!g_AICfgMgr.IsShowHpChangeEffect());
				return true;
			}
			else if (pControl == m_pRDAcceptFrientTrade)
			{
				g_AICfgMgr.SetAcceptFrientAndTrade(!g_AICfgMgr.IsAcceptFrientAndTrade());
				return true;
			}
			else if(pControl == m_pRDQuickOutGame)
			{
				g_AICfgMgr.SetQuickOutGame(!g_AICfgMgr.IsQuickOutGame());
				return true;
			}
			else if(pControl == m_pRDCloseTeam)
			{
				g_AICfgMgr.SetCloseTeam(!g_AICfgMgr.IsCloseTeam());
				return true;
			}
			else if(pControl == m_pRDDurationPrompt)
			{
				g_AICfgMgr.SetDurationPrompt(!g_AICfgMgr.IsDurationPrompt());
				return true;
			}
			else if(pControl == m_pRDTeamColor)
			{
				g_AICfgMgr.SetTeamColor(!g_AICfgMgr.IsTeamColor());
				return true;
			}
			else if (pControl == m_pRDAutoKill)
			{
				g_AICfgMgr.SetAutoKillShow(!g_AICfgMgr.IsAutoKillShow());				
				return true;
			}
			else if(pControl == m_pRDAutoFindWay)
			{
				g_AICfgMgr.SetAutoFindWay(!g_AICfgMgr.IsAutoFindWay());
				return true;
			}
			else if(pControl == m_pRDGoPickup)
			{
				g_AutoPickMgr.EnableAutoGoPick(!g_AICfgMgr.IsGoPickup());
				return true;
			}
			else if(pControl == m_pRDAutoPickup)
			{
				g_AICfgMgr.SetAutoPickup(!g_AICfgMgr.IsAutoPickup());
				return true;
			}
			else if(pControl == m_pRDShowMagicTime)
			{
				g_AICfgMgr.SetShowMagicTime(!g_AICfgMgr.IsShowMagicTime());
				return true;
			}
			else if(pControl == m_pRDBury)
			{
				g_AICfgMgr.SetBury(!g_AICfgMgr.IsBury());
				return true;
			}
			else if(pControl == m_pRDMountain)
			{
				g_AICfgMgr.SetMountain(!g_AICfgMgr.IsMountain());
				return true;
			}
			else if(pControl == m_pRDStarObject)
			{
				g_AICfgMgr.SetStarObject(!g_AICfgMgr.IsStarObject());
				return true;
			}
			else if(pControl == m_pRDAutoMogong)
			{
				g_AICfgMgr.SetAutoMogong(!g_AICfgMgr.IsAutoMogong());
				return true;
			}
			else if(pControl == m_pRDAllowFly)
			{
				g_AICfgMgr.SetAllowFly(!g_AICfgMgr.IsAllowFly());
				return true;
			}
			//else if(pControl == m_pRDDismount)
			//{
			//	g_AICfgMgr.SetDismount(!g_AICfgMgr.IsDismount());
			//	return true;
			//}
			else if(pControl == m_pRDRunNotStop)
			{
				g_AICfgMgr.SetRunNotStop(!g_AICfgMgr.IsRunNotStop());

				char temp[1024] = {0};
				if(g_AICfgMgr.IsRunNotStop())
					sprintf(temp,"[跑不停开启]");
				else
					sprintf(temp,"[跑不停关闭]");
				g_TalkMgr.PushSysTalk(temp);

				return true;
			}
			else if(pControl == m_pDealutEatCfg)
			{
				g_AICfgMgr.SetShowObjectName(true);
				g_AICfgMgr.SetShowPlayerName(true);
				g_AICfgMgr.SetShowArrow(false);
				g_AICfgMgr.SetShowSysInfo(true);
				g_Config.SetCheckQuick(true);
				g_AICfgMgr.SetShowNpcPriority(true);
				g_AICfgMgr.SetShowNewHandPrompt(true);
				g_AICfgMgr.SetShowHpChangeEffect(true);
				g_AICfgMgr.SetAcceptFrientAndTrade(true);
				g_AICfgMgr.SetQuickOutGame(true);
				g_AICfgMgr.SetCloseTeam(true);
				g_AICfgMgr.SetDurationPrompt(true);
				g_AICfgMgr.SetTeamColor(false);
				g_AICfgMgr.SetAutoKillShow(true);
				g_AICfgMgr.SetAutoFindWay(true);
				g_AutoPickMgr.EnableAutoGoPick(true);
				g_AICfgMgr.SetAutoPickup(false);
				g_AICfgMgr.SetShowMagicTime(true);
				g_AICfgMgr.SetBury(false);
				g_AICfgMgr.SetMountain(true);
				g_AICfgMgr.SetStarObject(false);
				g_AICfgMgr.SetAutoMogong(true);
				g_AICfgMgr.SetAllowFly(true);

				if (g_AICfgMgr.IsRunNotStop())
				{
					g_AICfgMgr.SetRunNotStop(false);
					g_TalkMgr.PushSysTalk("[跑不停关闭]");
				}
			}
		}
		else if(m_iCurPage == PAGE_PROTECT)
		{
			if(pControl == m_pRDUsePTSuperHP)
			{
				g_AICfgMgr.SetUsePTSuperHP(!g_AICfgMgr.IsUsePTSuperHP());
				return true;
			}
			else if(pControl == m_pRDUsePTSuperMP)
			{
				g_AICfgMgr.SetUsePTSuperMP(!g_AICfgMgr.IsUsePTSuperMP());
				return true;
			}
			else if(pControl == m_pRDAutoQuit)
			{
				g_AICfgMgr.SetAutoQuit(!g_AICfgMgr.IsAutoQuit());
				return true;
			}
			//else if(pControl == m_pRDAutoFeed)
			//{
			//    g_AICfgMgr.SetAutoFeed(!g_AICfgMgr.IsAutoFeed());
			//	return true;
			//}
			else if(pControl == m_pDealutEatCfg)
			{				
				float fRateHP[3][MAX_MEDICA_NUMBER] = {
					{0.90f,0.80f,0.70f,0.60f,0.50f,0.40f,0.35f,0.30f,0.20f},//战士
					{0.95f,0.75f,0.55f,0.45f,0.35f,0.35f,0.30f,0.25f,0.20f},//法师
					{0.95f,0.80f,0.65f,0.50f,0.35f,0.35f,0.30f,0.25f,0.20f},//道士
				};
				float fRateMP[3][MAX_MEDICA_NUMBER] = {
					{0.80f,0.60f,0.40f,0.20f,0.10f,0.0f,0.0f,0.0f,0.0f},//战士
					{0.95f,0.80f,0.60f,0.40f,0.20f,0.0f,0.0f,0.0f,0.0f},//法师
					{0.95f,0.80f,0.60f,0.40f,0.20f,0.0f,0.0f,0.0f,0.0f},//道士
				};
				int iCareer = SELF.GetCareer();
				int iHpMax = SELF.GetHPMax();
				int iMpMax = SELF.GetMPMax();
				g_AICfgMgr.SetUsePTSuperHP(false);
				g_AICfgMgr.SetUsePTSuperMP(false);

				for(int i = 0;i < MAX_MEDICA_NUMBER;i++)
				{
					WOOOLMEDICAEAT* pEatCfg = g_AutoEatMgr.GetHPMPMedicaInfo(i); 
					if (pEatCfg)
					{
						pEatCfg->Clear();
						pEatCfg->hp_bused = 1;

						if(i < 5)
						{
							pEatCfg->mp_bused = 1;
							pEatCfg->hp_delay = 3000;
							pEatCfg->mp_delay = 5000;
						}
						else
						{
							pEatCfg->hp_delay = 1000;
							pEatCfg->mp_delay = 0;
						}

						pEatCfg->hp_limit = (int)(fRateHP[iCareer][i] * (float)iHpMax);
						pEatCfg->mp_limit = (int)(fRateMP[iCareer][i] * (float)iMpMax);	 	   

						g_AutoEatMgr.SetUseHPMedia(i,pEatCfg->hp_bused != 0);
						g_AutoEatMgr.SetUseMPMedia(i,pEatCfg->mp_bused != 0);

						g_AutoEatMgr.SetHPMediaLimit(i,pEatCfg->hp_limit);
						g_AutoEatMgr.SetHPMediaDelay(i,pEatCfg->hp_delay);
						g_AutoEatMgr.SetMPMediaLimit(i,pEatCfg->mp_limit);
						g_AutoEatMgr.SetMPMediaDelay(i,pEatCfg->mp_delay);
					}
				}

				SetEatConfig();				
				return true;
			}
			else
			{
				for(int i = 0; i < MAX_MEDICA_NUMBER;i++)
				{
					if(pControl == m_pRDUseHPMedia[i])
					{
						g_AutoEatMgr.SetUseHPMedia(i,!g_AutoEatMgr.IsUseHPMedica(i));
						return true;
					}
					else if(pControl == m_pRDUseMPMedia[i])
					{
						g_AutoEatMgr.SetUseMPMedia(i,!g_AutoEatMgr.IsUseMPMedica(i));
						return true;
					}
				}
			}
		}
		else if(m_iCurPage == PAGE_FIGHT)
		{
			if(pControl == m_pRDAutoProtectSkin)
			{
				if(g_AICfgMgr.IsAutoProtectSkin())
				{
					g_AICfgMgr.SetAutoProtectSkin(false);
				}
				else
				{
					g_AICfgMgr.SetAutoProtectSkin(true);
					g_AICfgMgr.SetAutoSteelProtect(false);
				}
				return true;
			}
			else if(pControl == m_pRDAutoSteelProtect)
			{
				if(g_AICfgMgr.IsAutoSteelProtect())
				{
					g_AICfgMgr.SetAutoSteelProtect(false);
				}
				else
				{
					g_AICfgMgr.SetAutoSteelProtect(true);
					g_AICfgMgr.SetAutoProtectSkin(false);
				}
				return true;
			}
			else if(pControl == m_pRDAutoFire)
			{
				g_AICfgMgr.SetAutoFire(!g_AICfgMgr.IsAutoFire());
				return true;
			}
			else if(pControl == m_pRDAutoSuperAttack)
			{
				g_AICfgMgr.SetAutoSuperAttack(!g_AICfgMgr.IsAutoSuperAttack());
				return true;
			}
			else if(pControl == m_pRDDestroyShield)
			{
				g_AICfgMgr.SetDestroyShield(!g_AICfgMgr.IsDestroyShield());
				if (!g_AICfgMgr.IsDestroyShield())
					g_pGameMgr->AttackUseMagic(MAGICID_DESTROY_SHIELD);
				return true;
			}
			else if(pControl == m_pRDAutoWildCollide)
			{
				g_AICfgMgr.SetAutoWildCollide(!g_AICfgMgr.IsAutoWildCollide());
				return true;
			}
			else if(pControl == m_pRDDirWild)
			{
				g_AICfgMgr.SetDirWild(!g_AICfgMgr.IsDirWild());
				return true;
			}
			else if(pControl == m_pRDDoubleFire)
			{
				g_AICfgMgr.SetDoubleFire(!g_AICfgMgr.IsDoubleFire());
				return true;
			}
			else if(pControl == m_pRDDestroyShell)
			{
				g_AICfgMgr.SetDestroyShell(!g_AICfgMgr.IsDestroyShell());
				if (!g_AICfgMgr.IsDestroyShell())
					g_pGameMgr->AttackUseMagic(MAGICID_DESTROY_SHELL);
				return true;
			}
			else if(pControl == m_pRDAutoMagicProtect)
			{
				g_AICfgMgr.SetAutoMagicProtect(!g_AICfgMgr.IsAutoMagicProtect());
				return true;
			}
			else if(pControl == m_pRDAutoDodgeSkill)
			{
				g_AICfgMgr.SetAutoDodgeSkill(!g_AICfgMgr.IsAutoDodgeSkill());
				return true;
			}
			else if(pControl == m_pRDAutoDispute)
			{
				g_AICfgMgr.SetAutoDispute(!g_AICfgMgr.IsAutoDispute());
				return true;
			}
			else if(pControl == m_pRDAutoProtectGhost)
			{
				g_AICfgMgr.SetAutoProtectGhost(!g_AICfgMgr.IsAutoProtectGhost());
				return true;
			}
			else if(pControl == m_pRDAutoProtectArmor)
			{
				g_AICfgMgr.SetAutoProtectArmor(!g_AICfgMgr.IsAutoProtectArmor());
				return true;
			}
			else if(pControl == m_pRDAutoLionCall)
			{
				g_AICfgMgr.SetAutoLionCall(!g_AICfgMgr.IsAutoLionCall());
				return true;
			}
			else if(pControl == m_pRDSmartLionCall)
			{
				g_AICfgMgr.SetSmartLionCall(!g_AICfgMgr.IsSmartLionCall());
				return true;
			}
			else if(pControl == m_pRDAntiAttack)
			{
				g_AICfgMgr.SetAntiAttack(!g_AICfgMgr.IsAntiAttack());
				return true;
			}
			else if(pControl == m_pRDAutoSkill)
			{
				g_AICfgMgr.SetAutoSkill(!g_AICfgMgr.IsAutoSkill());
				return true;
			}
		}
		else if(m_iCurPage == PAGE_ITEM)
		{
			if(pControl == m_pAddItemButton)
			{
				const char* szItemName = m_pEdItemName->GetText();
				if(szItemName)
				{
					int iLine = g_PickCfgMgr.InsertItemClass(szItemName,-1);
					UpdateListPos(iLine,g_PickCfgMgr.GetItemClassCount());
				}
				return true;
			}
			else if(pControl == m_pDeleteItemButton)
			{
				int iLine = g_PickCfgMgr.RemoveItemClass(m_pGrid->GetSelLine());
				UpdateListPos(iLine,g_PickCfgMgr.GetItemClassCount());
				return true;
			}
		}
		else if(m_iCurPage == PAGE_MONSTER)
		{
			if(pControl == m_pRDBossPrompt)
			{
				g_AICfgMgr.SetBossPrompt(!g_AICfgMgr.IsBossPrompt());
				return true;
			}
			else if(pControl == m_pRDObjectPrompt)
			{
				g_AICfgMgr.SetObjectPrompt(!g_AICfgMgr.IsObjectPrompt());
				return true;
			}
			else if(pControl == m_pRDPkAllAttack)
			{
				g_AICfgMgr.SetPkAllAttack(!g_AICfgMgr.IsPkAllAttack());
				return true;
			}
			else if(pControl == m_pRDBlackFilter)
			{
				g_AICfgMgr.SetBlackFilter(!g_AICfgMgr.IsBlackFilter());
				return true;
			}
			else if(pControl == m_pRDPkAlarm)
			{
				g_AICfgMgr.SetPKAlarm(!g_AICfgMgr.IsPKAlarm());
				return true;
			}
			else if(pControl == m_pAddMonsterButton)
			{
				const char* szMonsterName = m_pEdMonsterName->GetText();
				if(szMonsterName)
				{
					int iLine = g_BossCfgMgr.InsertMonster(szMonsterName,-1);
					UpdateListPos(iLine,g_BossCfgMgr.GetBossClassCount());//更新范围
				}
				return true;
			}
			else if(pControl == m_pDeleteMonsterButton)
			{
				int iLine = g_BossCfgMgr.RemoveMonster(m_pGrid->GetSelLine());
				UpdateListPos(iLine,g_BossCfgMgr.GetBossClassCount());//更新范围

				return true;
			}
		}
		else if(m_iCurPage == PAGE_CHAT)
		{
			if(pControl == m_pRDAutoSay)
			{
				g_AICfgMgr.SetAutoSay(!g_AICfgMgr.IsAutoSay());

				if(g_AICfgMgr.IsAutoSay())
				{
					g_AICfgMgr.SetAutoSayMsg(m_pAutoSayMsg->GetText());
				}
				return true;
			}
			else if(pControl == m_pRDAutoReply)
			{
				g_AICfgMgr.SetAutoReply(!g_AICfgMgr.IsAutoReply());
				return true;
			}
			else if(pControl == m_pRDMessageAwoke)
			{
				g_AICfgMgr.SetMessageAwoke(!g_AICfgMgr.IsMessageAwoke());
				return true;
			}
			else if(pControl == m_pKeepOutInfo)
			{
				g_AICfgMgr.SetKeepOutInfo(!g_AICfgMgr.IsKeepOutInfo());
				return true;
			}
		}
		else if(m_iCurPage == PAGE_RECORD)
		{
			if(pControl == m_pRDRecordMsg)
			{
				g_AICfgMgr.SetRecordMsg(!g_AICfgMgr.IsRecordMsg());
				return true;
			}
			else if(pControl == m_pClearMsgButton)
			{
				g_AICfgMgr.ClearPrivateMsg();
				return true;
			}
		}

		break;
	case MSG_CTRL_GRID_SELECT_LINE_COL:			
		if(m_iCurPage == PAGE_ITEM)
		{
			switch(HIWORD(dwData))
			{
			case 1:
			    g_PickCfgMgr.FlipBest(LOWORD(dwData));
			    break;
			case 2:
			    g_PickCfgMgr.FlipExpensive(LOWORD(dwData));
			    break;
			case 3:
			    g_PickCfgMgr.FlipPick(LOWORD(dwData));
			    break;
			case 4:
			    g_PickCfgMgr.FlipShow(LOWORD(dwData));
			    break;
			}
		}
		else if(m_iCurPage == PAGE_MAGIC)
		{
			if(LOWORD(dwData) >= 0 && LOWORD(dwData) < MAX_MAGIC_SKILL && HIWORD(dwData) > 0 && HIWORD(dwData) <= 2)
			{
				CMagicData& tmp = SELF.GetMagic(LOWORD(dwData));
				switch(HIWORD(dwData))
				{
				case 1:
					tmp.UpdateAttr(MATTR_LOCK);
					break;
				case 2:
					tmp.UpdateAttr(MATTR_REPEAT);
					break;
				}
				g_AICfgMgr.SaveShortCutConfig();
			}
		}
		else if(m_iCurPage == PAGE_MONSTER)
		{
			switch(HIWORD(dwData))
			{
			case 1:
				g_BossCfgMgr.FlipAlarm(LOWORD(dwData));
				break;
			case 2:
				g_BossCfgMgr.FlipBossType(LOWORD(dwData));
				break;
			}
		}

		return true;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CAssConfigWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 276,m_iScreenY + 10,"游戏设置",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	if(m_iCurPage == PAGE_BASE)
	{

		m_pRDShowObjectName->SetChecked(g_AICfgMgr.IsShowObjectName());
		m_pRDShowPlayerName->SetChecked(g_AICfgMgr.IsShowPlayerName());
		m_pRDShowArrow->SetChecked(g_AICfgMgr.IsShowArrow());
		m_pRDShowSysInfo->SetChecked(g_AICfgMgr.IsShowSysInfo());
		m_pRDShowQuickBuy->SetChecked(g_Config.GetCheckQuick());
		m_pRDShowNpcPriority->SetChecked(g_AICfgMgr.IsShowNpcPriority());
		m_pRDShowNewHandPrompt->SetChecked(g_AICfgMgr.IsShowNewHandPrompt());
		m_pRDShowHpChange->SetChecked(g_AICfgMgr.IsShowHpChangeEffect());
		m_pRDAcceptFrientTrade->SetChecked(g_AICfgMgr.IsAcceptFrientAndTrade());

		m_pRDQuickOutGame->SetChecked(g_AICfgMgr.IsQuickOutGame());
		m_pRDCloseTeam->SetChecked(g_AICfgMgr.IsCloseTeam());
		m_pRDDurationPrompt->SetChecked(g_AICfgMgr.IsDurationPrompt());
		m_pRDTeamColor->SetChecked(g_AICfgMgr.IsTeamColor());
		//m_pRDDismount->SetChecked(g_AICfgMgr.IsDismount());
		if (m_pRDAutoKill)
		{
			m_pRDAutoKill->SetChecked(g_AICfgMgr.IsAutoKillShow());
		}

		m_pRDAutoFindWay->SetChecked(g_AICfgMgr.IsAutoFindWay());
		m_pRDGoPickup->SetChecked(g_AICfgMgr.IsGoPickup());
		m_pRDAutoPickup->SetChecked(g_AICfgMgr.IsAutoPickup());
		m_pRDShowMagicTime->SetChecked(g_AICfgMgr.IsShowMagicTime());
		m_pRDBury->SetChecked(g_AICfgMgr.IsBury());
		m_pRDMountain->SetChecked(g_AICfgMgr.IsMountain());
		m_pRDStarObject->SetChecked(g_AICfgMgr.IsStarObject());
		m_pRDAutoMogong->SetChecked(g_AICfgMgr.IsAutoMogong());
		m_pRDAllowFly->SetChecked(g_AICfgMgr.IsAllowFly());
		m_pRDRunNotStop->SetChecked(g_AICfgMgr.IsRunNotStop());

	}
	else if(m_iCurPage == PAGE_PROTECT)
	{
		g_pFont->DrawText(m_iScreenX + 116,m_iScreenY + 63,"补充红药(HP)",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 350,m_iScreenY + 63,"补充蓝药(MP)",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 169,m_iScreenY + 339,"HP下限",0xffbd9a73,FONT_YAHEI,FONTSIZE_SMALL);
		g_pFont->DrawText(m_iScreenX + 401,m_iScreenY + 339,"HP下限",0xffbd9a73,FONT_YAHEI,FONTSIZE_SMALL);

		g_pFont->DrawText(m_iScreenX + 176,m_iScreenY + 89,"下限",0xFFB46428,FONT_YAHEI,FONTSIZE_SMALL);
		g_pFont->DrawText(m_iScreenX + 226,m_iScreenY + 89,"延迟",0xFFB46428,FONT_YAHEI,FONTSIZE_SMALL);
		g_pFont->DrawText(m_iScreenX + 410,m_iScreenY + 89,"下限",0xFFB46428,FONT_YAHEI,FONTSIZE_SMALL);
		g_pFont->DrawText(m_iScreenX + 460,m_iScreenY + 89,"延迟",0xFFB46428,FONT_YAHEI,FONTSIZE_SMALL);

		for(int i = 0;i < MAX_MEDICA_NUMBER;i++)
		{
			m_pRDUseHPMedia[i]->SetChecked(g_AutoEatMgr.IsUseHPMedica(i));
			m_pRDUseMPMedia[i]->SetChecked(g_AutoEatMgr.IsUseMPMedica(i));
		}

		if((g_dwServerSwitch & SS_QUIT_DELAY) == 0)
		{
			m_pRDAutoQuit->SetChecked(g_AICfgMgr.IsAutoQuit());
		}
		m_pRDUsePTSuperHP->SetChecked(g_AICfgMgr.IsUsePTSuperHP());
		m_pRDUsePTSuperMP->SetChecked(g_AICfgMgr.IsUsePTSuperMP());
        //m_pRDAutoFeed->SetChecked(g_AICfgMgr.IsAutoFeed());

	}
	else if(m_iCurPage == PAGE_FIGHT)
	{
		g_pFont->DrawText(m_iScreenX + 170,m_iScreenY + 64,"战士辅助",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 170,m_iScreenY + 207,"法师辅助",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 170,m_iScreenY + 285,"道士辅助",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 404,m_iScreenY + 64,"公共辅助",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 373,m_iScreenY + 199,"练功间隔",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);

		if (m_pMBAutoSkillKey->IsMenuDown())
		{
			m_pMBAutoSkillKey->Draw();
		}

		m_pRDAutoProtectSkin->SetChecked(g_AICfgMgr.IsAutoProtectSkin());
		m_pRDAutoSteelProtect->SetChecked(g_AICfgMgr.IsAutoSteelProtect());
		m_pRDAutoFire->SetChecked(g_AICfgMgr.IsAutoFire());
		m_pRDDestroyShield->SetChecked(g_AICfgMgr.IsDestroyShield());
		m_pRDAutoWildCollide->SetChecked(g_AICfgMgr.IsAutoWildCollide());
		m_pRDDirWild->SetChecked(g_AICfgMgr.IsDirWild());
		m_pRDDoubleFire->SetChecked(g_AICfgMgr.IsDoubleFire());
		m_pRDDestroyShell->SetChecked(g_AICfgMgr.IsDestroyShell());
		m_pRDAutoSuperAttack->SetChecked(g_AICfgMgr.IsAutoSuperAttack());

		m_pRDAutoMagicProtect->SetChecked(g_AICfgMgr.IsAutoMagicProtect());
		m_pRDAutoDodgeSkill->SetChecked(g_AICfgMgr.IsAutoDodgeSkill());
		m_pRDAutoDispute->SetChecked(g_AICfgMgr.IsAutoDispute());

		m_pRDAutoProtectGhost->SetChecked(g_AICfgMgr.IsAutoProtectGhost());
		m_pRDAutoProtectArmor->SetChecked(g_AICfgMgr.IsAutoProtectArmor());
		m_pRDAutoLionCall->SetChecked(g_AICfgMgr.IsAutoLionCall());
		m_pRDSmartLionCall->SetChecked(g_AICfgMgr.IsSmartLionCall());

		m_pRDAntiAttack->SetChecked(g_AICfgMgr.IsAntiAttack());
		m_pRDAutoSkill->SetChecked(g_AICfgMgr.IsAutoSkill());

	}
	else if(m_iCurPage == PAGE_ITEM)
	{
		g_pFont->DrawText(m_iScreenX + 53,m_iScreenY + 71,"物品名称",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 192,m_iScreenY + 71,"极品",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 274,m_iScreenY + 71,"贵重",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 357,m_iScreenY + 71,"拾取",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 440,m_iScreenY + 71,"显示",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);

		g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 345,"极",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 345,m_iScreenY + 345,"贵",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 400,m_iScreenY + 345,"拾",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 454,m_iScreenY + 345,"普",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);

		//绘制物品列表相关的内容
		if(m_pGrid)
		{
			for(int i = 0;i < m_pGrid->GetLinesPerPage(); i++)
			{
				int iIndex = m_pGrid->GetDisLine() + i;
				CItemClass* pClass = g_PickCfgMgr.GetItemClass(iIndex);
				if(pClass == NULL)
					continue;

				m_pGrid->DrawGrid(i,0,pClass->strName.c_str());

				if(pClass->bBest)
					m_pGrid->DrawGrid(i,1,RIGHT_SYM,true);

				if(pClass->bExpensive)
					m_pGrid->DrawGrid(i,2,RIGHT_SYM,true);

				if(pClass->bPick)
					m_pGrid->DrawGrid(i,3,RIGHT_SYM,true);

				if(pClass->bShow)
					m_pGrid->DrawGrid(i,4,RIGHT_SYM,true);
			}
		}

		//绘制物品颜色设置的小块颜色
		int iItemColorX = m_iScreenX + 308;
		int iItemColorY = m_iScreenY + 345;
		g_pGfx->DrawFillRect(iItemColorX,iItemColorY,15,15,g_AICfgMgr.GetBestItemColor());
		iItemColorX += 55;
		g_pGfx->DrawFillRect(iItemColorX,iItemColorY,15,15,g_AICfgMgr.GetExpensiveItemColor());
		iItemColorX += 55;
		g_pGfx->DrawFillRect(iItemColorX,iItemColorY,15,15,g_AICfgMgr.GetPickItemColor());
		iItemColorX += 54;
		g_pGfx->DrawFillRect(iItemColorX,iItemColorY,15,15,g_AICfgMgr.GetShowItemColor());
	}
	else if(m_iCurPage == PAGE_MAGIC)
	{
		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 71,"魔法名称",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 177,m_iScreenY + 71,"是否锁定",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 258,m_iScreenY + 71,"是否持续",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 330,m_iScreenY + 71,"基本快捷键",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 411,m_iScreenY + 71,"扩展快捷键",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		
		g_pFont->DrawText(m_iScreenX + 49,m_iScreenY + 319,"提示：清除快捷键请按“Delete”键",0xFFB46428,FONT_YAHEI,FONTSIZE_SMALL);
		g_pFont->DrawText(m_iScreenX + 49,m_iScreenY + 336,"      基本快捷键可用“F1～F8”",0xFFB46428,FONT_YAHEI,FONTSIZE_SMALL);
		g_pFont->DrawText(m_iScreenX + 49,m_iScreenY + 353,"      扩展快捷键可用“A ～ Z”或者“Esc+F1 ~ Esc+F8”",0xFFB46428,FONT_YAHEI,FONTSIZE_SMALL);

		m_pGrid->SetTotalCount(SELF.MagicArray().GetMagicNumber());

		//魔法找到指定位置
		char temp_str[64] = {0};
		//开始绘制魔法
		for(int i = 0;i < m_pGrid->GetLinesPerPage();i++)
		{
			int _iMS = i + m_pGrid->GetDisLine();

			if(_iMS >= MAX_MAGIC_SKILL)
				break;

			CMagicData& tmp = SELF.GetMagic(_iMS++);
			if(tmp.GetMagicID() == 0) break;

			m_pGrid->DrawGrid(i,0,tmp.GetMagicName());
			if(tmp.HasAttr(MATTR_LOCK))
				m_pGrid->DrawGrid(i,1,RIGHT_SYM,true);
			if(tmp.HasAttr(MATTR_REPEAT))
				m_pGrid->DrawGrid(i,2,RIGHT_SYM,true);

			if(tmp.GetShortCutKey() > 0)
			{
				sprintf(temp_str,"F%d",tmp.GetShortCutKey() - 0x30);
				m_pGrid->DrawGrid(i,3,temp_str,true);
			}

			string strKeyEx = g_AICfgMgr.GetShortCutKeyExStr(tmp.GetShortCutKeyEx());
			if(!strKeyEx.empty())
				m_pGrid->DrawGrid(i,4,strKeyEx.c_str(),true);
		}
	}
	else if(m_iCurPage == PAGE_MONSTER)
	{
		g_pFont->DrawText(m_iScreenX + 53,m_iScreenY + 71,"BOSS名称",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 193,m_iScreenY + 71,"提醒",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 274,m_iScreenY + 71,"类别",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);

		//绘制怪物相关的内容
		if(m_pGrid)
		{
			for(int i = 0;i < m_pGrid->GetLinesPerPage(); i++)
			{
				int iIndex = m_pGrid->GetDisLine() + i;
				CBossClass* pClass = g_BossCfgMgr.GetBossClass(iIndex);
				if(pClass == NULL)
					continue;

				m_pGrid->DrawGrid(i,0,pClass->strName.c_str());

				if(pClass->bAlarm)
					m_pGrid->DrawGrid(i,1,RIGHT_SYM,true);

				string strBossType;
				if(pClass->iType == 0)
					strBossType = BOSS_SYM;
				else if(pClass->iType == 1)
					strBossType = ENEMY_SYM;
				else
					strBossType = BLACK_SYM;

				m_pGrid->DrawGrid(i,2,strBossType.c_str(),true);
			}
		}

		m_pRDBossPrompt->SetChecked(g_AICfgMgr.IsBossPrompt());
		m_pRDObjectPrompt->SetChecked(g_AICfgMgr.IsObjectPrompt());
		m_pRDPkAllAttack->SetChecked(g_AICfgMgr.IsPkAllAttack());
		m_pRDBlackFilter->SetChecked(g_AICfgMgr.IsBlackFilter());
		m_pRDPkAlarm->SetChecked(g_AICfgMgr.IsPKAlarm());

	}
	else if(m_iCurPage == PAGE_CHAT)
	{
		g_pFont->DrawText(m_iScreenX + 180,m_iScreenY + 71,"发言时间间隔(秒)",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
		g_pFont->DrawText(m_iScreenX + 248,m_iScreenY + 98,"内容预览",0xFFB46428,FONT_YAHEI,FONTSIZE_MIDDLE);

		m_pRDAutoSay->SetChecked(g_AICfgMgr.IsAutoSay());
		m_pRDAutoReply->SetChecked(g_AICfgMgr.IsAutoReply());
		m_pRDMessageAwoke->SetChecked(g_AICfgMgr.IsMessageAwoke());
		m_pKeepOutInfo->SetChecked(g_AICfgMgr.IsKeepOutInfo());

		if(m_iFaceIndex >= 0)
		{
			int _xx = m_iScreenX + FACESX + (m_iFaceIndex % FACECOLCOUNT) * 19;
			int _yy = m_iScreenY + FACESY + (m_iFaceIndex / FACECOLCOUNT) * 19;

			g_pGfx->DrawFillRect(_xx+1,_yy+1,17,17,0x400000FF);

			BYTE chFace[3] = {0};
			chFace[0] = 0xF8;
			chFace[1] = 0xA1;
			chFace[1] += m_iFaceIndex;
			g_pFont->DrawText(_xx + 3,_yy+3,(char*)chFace,-1);
		}

	}
	else if(m_iCurPage == PAGE_RECORD)
	{
		m_pRDRecordMsg->SetChecked(g_AICfgMgr.IsRecordMsg());
	}
	else if(m_iCurPage == PAGE_README)
	{
		if(m_pGrid)
		{
			for(int i = 0; i < m_pGrid->GetLinesPerPage();i++)
			{
				int iIndex =  m_pGrid->GetDisLine() + i;

				m_pGrid->DrawGrid(i,0,g_AICfgMgr.GetReadme(iIndex).c_str());
			}
		}
	}
}

bool CAssConfigWnd::OnLeftButtonUp(int iX, int iY)
{
	if(m_iCurPage == PAGE_CHAT)
	{
		m_iFaceIndex = IsInFaceBtn(iX,iY);
		if(m_iFaceIndex >= 0)
		{
			BYTE chFace[3] = {0};
			chFace[0] = 0xF8;
			chFace[1] = 0xA1;
			chFace[1] += m_iFaceIndex;
			int iPos = m_pAutoSayMsg->GetCursorPos();
			m_pAutoSayMsg->InsertStr(iPos,(char*)chFace);
		}

	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CAssConfigWnd::OnMouseMove(int iX,int iY)
{
	if(m_iCurPage == PAGE_CHAT)
	{
		m_iFaceIndex = IsInFaceBtn(iX,iY);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CAssConfigWnd::OnWheel(int iWheel)
{
	if(m_iCurPage == PAGE_RECORD)
	{
		if(m_pTalkViewer)
			m_pTalkViewer->OnWheel(iWheel);
	}
	else
	{
		if(m_pGrid)
			m_pGrid->OnWheel(iWheel);
	}
	return true;
}

void CAssConfigWnd::UpdateListPos(int iLine ,int iTotalCount)
{
	if(m_pGrid)
	{
		//更新范围
		m_pGrid->SetTotalCount(iTotalCount);
		m_pGrid->MoveAsNeed(iLine);
	}
}

bool CAssConfigWnd::OnKeyDown(WORD wState, UCHAR cKey)
{
	if(m_iCurPage == PAGE_MAGIC)
	{
		WORD wHighKey = g_pInput->IsEscape()?1:0;

		if(m_pGrid)
		{
			int iLine = m_pGrid->GetSelLine();
			int iCol = m_pGrid->GetSelCol();
			if(iLine < 0 || iLine >= MAX_MAGIC_SKILL)
				return true;

			CMagicData& tmp = SELF.GetMagic(iLine);
			if(iCol == 3)
			{
				if(cKey >= VK_F1 && cKey <= VK_F8)
				{
					tmp.SetShortCutKey(cKey+0x31-VK_F1);

					g_pGameControl->SEND_ShortCut_Key_Change(tmp.GetMagicID(),tmp.GetShortCutKey());

					for(int i = 0;i < MAX_MAGIC_SKILL;i++)
					{
						if(i == iLine) continue;

						WORD tMagicID = SELF.GetMagic(i).GetMagicID();
						if(tMagicID == 0)
							continue;

						if(SELF.GetMagic(i).GetShortCutKey() == tmp.GetShortCutKey())
						{
							SELF.GetMagic(i).SetShortCutKey(0);
							g_pGameControl->SEND_ShortCut_Key_Change(tMagicID,0);
						}
					}

					ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByKey(MAKELONG(cKey,0));
					if (pConSkillData2)
					{
						pConSkillData2->dwKey = 0;
					}

					//法决
					/*if (SELF.GetYiHuoShortCutKey() == tmp.GetShortCutKey())
					{
						g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
						SELF.SetYiHuoShortCutKey(0);
					}*/

					g_AICfgMgr.SaveShortCutConfig();

					return true;
				}
				else if(cKey == VK_DELETE) //清除
				{
					tmp.SetShortCutKey(0);
					g_pGameControl->SEND_ShortCut_Key_Change(tmp.GetMagicID(),0);
					g_AICfgMgr.SaveShortCutConfig();
					return true;
				}
			}
			else if(iCol == 4)
			{
				if((wHighKey == 1 && cKey >= VK_F1 && cKey <= VK_F8) 
				   ||(!g_pInput->IsCtrl()  && !g_pInput->IsAlt() && cKey >= 'A' && cKey <='Z'))
				{
					tmp.SetShortCutKeyEx(MAKELONG(cKey,wHighKey));

					for(int i = 0;i < MAX_MAGIC_SKILL;i++)
					{
						if(i == iLine) continue;

						WORD tMagicID = SELF.GetMagic(i).GetMagicID();
						if(tMagicID == 0)
							continue;

						if(SELF.GetMagic(i).GetShortCutKeyEx() == tmp.GetShortCutKeyEx())
						{
							SELF.GetMagic(i).SetShortCutKeyEx(0);
						}
					}

					ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByKey(MAKELONG(cKey,1));
					if (pConSkillData2)
					{
						pConSkillData2->dwKey = 0;
					}

					//法决
					/*if (SELF.GetYiHuoShortCutKeyEx() == tmp.GetShortCutKeyEx())
					{						
						SELF.SetYiHuoShortCutKeyEx(0);
					}*/

					g_AICfgMgr.SaveShortCutConfig();
					return true;
				}
				else if(cKey == VK_DELETE)
				{
					tmp.SetShortCutKeyEx(0);
					g_AICfgMgr.SaveShortCutConfig();
					return true;
				}
			}
		}
		return true;
	}
	else if(m_iCurPage == PAGE_RECORD)
	{

	}
	return CCtrlWindowX::OnKeyDown(wState,cKey);
}

bool CAssConfigWnd::IsInKeyCol()
{
	if(m_iCurPage == PAGE_MAGIC)
	{
		if(m_pGrid)
		{
			int iLine = m_pGrid->GetSelLine();
			int iCol  = m_pGrid->GetSelCol();
			if(iLine >= 0 && iLine < MAX_MAGIC_SKILL && iCol >= 3 && iCol <= 4)
				return true;
		}
	}
	return false;
}

int CAssConfigWnd::IsInFaceBtn(int iX,int iY)
{
	if(m_iCurPage != PAGE_CHAT)
		return -1;

	if(iX >= FACESX && iX < FACESX + FACECOLCOUNT * 19 && iY >= FACESY && iY < FACESY + 57)
	{
		int iIndex = ((iY - FACESY) / 19 ) * FACECOLCOUNT + (iX - FACESX) / 19;

		if(iIndex >= FACETOTALCOUNT)
			return - 1;

		return iIndex;
	}
	return -1;
}



