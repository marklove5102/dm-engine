#include "relationwnd.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIBossCfgMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/GameGlobal.h"
#include "GameData/TalkMgr.h"
#include "GameData/OtherData.h"
//#include "GameData/GuildData.h"
#include "GameData/TaskData.h"
#include "GameData/NpcData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/LoginData.h"
#include "GameData/PhyleData.h"
#include "GameClient/WidgetManager.h"
#include "Global/Global.h"
#include "GameData/WooolStoreData.h"

#define MAX_ROWS 6

#define RL_BTN_NORMAL	0xfff0af64
#define RL_BTN_MOUSEON	0xfffac896
#define RL_BTN_PRESS	0xff5aa0a0
#define RL_BTN_DISABLE	0xff646464

#define RL_PAGE_NORMAL	0xff9e6d34
#define RL_PAGE_MOUSEON 0xfffac896 
#define RL_PAGE_PRESS   0xfff0af64
#define RL_PAGE_DISABLE 0xff646464


#define  SUBPAGE_INDEX_FRIEND                 0 //好友子页签序号
#define  SUBPAGE_INDEX_HUSBAND_WIFE           1 //夫妻子页签序号
#define  SUBPAGE_INDEX_MASTER_PRENTICE        2 //师徒子页签序号
#define  SUBPAGE_INDEX_PHYLE                  3 //宗族子页签序号
#define  SUBPAGE_INDEX_BLACK                  4 //黑名单子页签序号
#define  SUBPAGE_INDEX_JIEYI                  5 //结义子页签序号




INIT_WND_POSX(CRelationWnd,POS_AUTO,POS_AUTO)
std::string CRelationWnd::m_sStrAppointName = "";
CRelationWnd::CRelationWnd(void)
{
	m_iTowerLevelUpFrame = 1000;
	m_iSkullFrame = 1000;
	m_iFireFrame = 1000;
	m_iShineFrame = 100;
	//m_iRandomNum = 0;
	m_bSendButtonClick = false;
	m_iFrame = 0;
	//iSubPage = 0;					//当前属性页
	m_bNoCreate    = true;
// 	m_iShowWnd = 0;

	m_iIndex = 17690;
	m_iCurShowRow = 0;	
	m_bShowOffLineMember = true;
	m_iPublicChat = 0;
	//m_pTextureChat = NULL;
// 	m_bTemp = false;
	m_bRequestedGuildMsg = false;

	m_bJieYi = false;

	m_iClickX = m_iClickY = - 1000;

	m_dwCurLive = 0;
	m_dwYesterLive = 0;

	if(strlen(SELF.GetTitle()) > 0)
	{
		g_pGameControl->SEND_Guild_Look_Exp_Val();//请求行会等级及行会经验值
		RequestGuildMember();
		g_pGameControl->SEND_Guild_Contribute();
	}

	RemoveBackDeal();//清除控件	


	//以下为新版页签相关
	m_iVersion = 2;

	S_TabPage* pRelationPage = AddTabPage(0,0,MAKELONG(17690,PACKAGE_INTERFACE),0,36,85,86,87,88,"人 脉",NULL,true,0,0,1);
	AddTabPage(0,0,MAKELONG(17690,PACKAGE_INTERFACE),51,34,17616,17617,17618,17618,"好 友",pRelationPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17690,PACKAGE_INTERFACE),51 + 50 * 1,34,17616,17617,17618,17618,"夫 妻",pRelationPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17690,PACKAGE_INTERFACE),51 + 50 * 2,34,17616,17617,17618,17618,"师 徒",pRelationPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17691,PACKAGE_INTERFACE),51 + 50 * 3,34,17616,17617,17618,17618,"宗 族",pRelationPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17690,PACKAGE_INTERFACE),51 + 50 * 4,34,17616,17617,17618,17618,"黑名单",pRelationPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22290,PACKAGE_INTERFACE),51 + 50 * 5,34,17616,17617,17618,17618,"结 义",pRelationPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);

	S_TabPage* pGuildPage = AddTabPage(0,0,MAKELONG(17692,PACKAGE_INTERFACE),0,36 + 90,85,86,87,88,"行 会",NULL,true,0,0,1);
	AddTabPage(0,0,MAKELONG(17692,PACKAGE_INTERFACE),51,59,115,116,117,118,"会 员",pGuildPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17693,PACKAGE_INTERFACE),51 + 50 * 1,59,115,116,117,118,"外 交",pGuildPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(16600,PACKAGE_INTERFACE),51 + 50 * 2,59,115,116,117,118,"人 事",pGuildPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17694,PACKAGE_INTERFACE),51 + 50 * 3,59,115,116,117,118,"所辖宗族",pGuildPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17695,PACKAGE_INTERFACE),51 + 50 * 4,59,115,116,117,118,"大事件",pGuildPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	
	
	S_TabPage* pTowerPage = AddTabPage(0,0,MAKELONG(16602,PACKAGE_INTERFACE),0,36 + 180,85,86,87,88,"行会宝塔",NULL,true,0,0,1);
	AddTabPage(0,0,0,0,0,0,0,0,0,"",pTowerPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//用来显示返回信息的页签
	S_TabPage* pMainTowerPage = AddTabPage(0,0,MAKELONG(16602,PACKAGE_INTERFACE),51,59,115,116,117,118,"主宝塔",pTowerPage,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL,true);
	AddTabPage(0,0,MAKELONG(16602,PACKAGE_INTERFACE),0,0,0,0,0,0,"",pMainTowerPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//主灵塔
	AddTabPage(0,0,MAKELONG(16603,PACKAGE_INTERFACE),0,0,0,0,0,0,"",pMainTowerPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//捐献资源
	AddTabPage(0,0,MAKELONG(16604,PACKAGE_INTERFACE),0,0,0,0,0,0,"",pMainTowerPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//所捐献的资源
	AddTabPage(0,0,MAKELONG(16602,PACKAGE_INTERFACE),0,0,0,0,0,0,"",pMainTowerPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//升级建筑介绍
	
	S_TabPage* pZhenBaoGePage = AddTabPage(0,0,MAKELONG(16606,PACKAGE_INTERFACE),51 + 50 * 1,59,115,116,117,118,"珍宝阁",pTowerPage,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL,true);
	AddTabPage(0,0,MAKELONG(16606,PACKAGE_INTERFACE),0,0,0,0,0,0,"",pZhenBaoGePage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//珍宝阁介绍
	AddTabPage(0,0,MAKELONG(16607,PACKAGE_INTERFACE),0,0,0,0,0,0,"",pZhenBaoGePage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//珍宝阁
	AddTabPage(0,0,MAKELONG(16608,PACKAGE_INTERFACE),0,0,0,0,0,0,"",pZhenBaoGePage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);//极品阁

	AddTabPage(0,0,MAKELONG(16612,PACKAGE_INTERFACE),51 + 50 * 2,59,115,116,117,118,"神工坊",pTowerPage,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL,true);

	AddTabPage(0,0,MAKELONG(16610,PACKAGE_INTERFACE),51 + 50 * 3,59,115,116,117,118,"议事厅",pTowerPage,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL,true);
	
	
	int iCurPage = 0;
	int iSubPage = 0;
	int iGrandsonPage = 0;

	if (sm_dwWindowType <= 1)
	{
		iCurPage = sm_dwWindowType;
	}
	else
	{
		iCurPage = LOWORD(sm_dwWindowType);
		if (iCurPage > 2)
			iCurPage = 2;

		iSubPage = HIWORD(sm_dwWindowType);

		if (iCurPage == 2)
		{
			if (iSubPage == 11)//升级建筑介绍
			{
				iSubPage = 1;
				iGrandsonPage = 3;
			}
			//else if(iSubPage == 6 || iSubPage == 20 || iSubPage == 21 || iSubPage == 24)// || iSubPage == 22 || iSubPage == 23)//获得盾牌,获得道具,道具炼制,获取金莲,获取黑莲
			//	m_iIndex = 16254;
			else if (iSubPage == 12 && g_GuildData.GetGuildTowerLevel() <= 0)
			{
				iSubPage = 0;
			}
			else if(iSubPage == 8)//议事厅
			{
				iSubPage = 4;
			}
			else if(iSubPage == 9)//捐献资源
			{
				iSubPage = 1;
				iGrandsonPage = 1;
			}
			else if(iSubPage == 9)//所捐献的资源
			{
				iSubPage = 1;
				iGrandsonPage = 2;
			}			
			else if(iSubPage == 3)//珍宝阁
			{
				iSubPage = 2;
				iGrandsonPage = 1;
			}	
			else if(iSubPage == 4)//极品阁
			{
				iSubPage = 2;
				iGrandsonPage = 2;
			}	
			else if(iSubPage == 5)//神工坊
			{
				iSubPage = 3;
			}
			else if(iSubPage == 1)//主灵塔
			{
				iSubPage = 1;
				iGrandsonPage = 0;
			}	
		}



		//else if (iIdx == 13 || iIdx == 14 || iIdx == 17 || iIdx == 19)
		//	m_iIndex = 16600;
		//else if (iIdx == 15)
		//	m_iIndex = 16601;
		//else if (iIdx == 16)
		//	m_iIndex = 16602;
		//else if (iIdx == 18)
		//	m_iIndex = 16446;
		//else if(iIdx == 25 || iIdx == 26 || iIdx == 27 || iIdx == 28 || iIdx == 29)
		//	m_iIndex = 16260;//行会炼制
		//else if(iIdx == 30)
		//	m_iIndex = 18560;

		if (iSubPage >= m_TabPage.vSubTabPage[iCurPage].vSubTabPage.size())
		{
			iSubPage = 0;
		}
	}

	m_TabPage.iCurPage = iCurPage;
	m_TabPage.vSubTabPage[iCurPage].iCurPage = iSubPage;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage = iGrandsonPage;
	}
}

CRelationWnd::~CRelationWnd(void)
{
	//if(m_pTextureChat)
	//{
	//	g_pTexMgr->ReleaseTex(m_pTextureChat);
	//	m_pTextureChat = NULL;
	//}

	//释放物品
	CGoodArray& goodArray = SELF.GetDevGuildGood();
	for (int i = 0;i<3;i++)
	{
		if (goodArray.Get(i).GetID() != 0)
		{
			SELF.PackageGood().BackToArray(goodArray.Get(i));
			goodArray.Get(i).SetID(0);
		}
	}	

// 	int iCurPage = m_TabPage.iCurPage;
// 	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
// 
// 	if(iSubPage == 7 || iSubPage == 18)
// 	{
// 		if(g_NPC.GetShenGongFangGoods().bSend == false)
// 		{
// 			if(g_NPC.GetShenGongFangGoods().m_Shield.GetID() != 0)
// 			{
// 				SELF.PackageGood().BackToArray(g_NPC.GetShenGongFangGoods().m_Shield);
// 				g_NPC.GetShenGongFangGoods().m_Shield.SetID(0);
// 			}
// 
// 			if(g_NPC.GetShenGongFangGoods().m_Wood.GetID() != 0)
// 			{
// 				SELF.PackageGood().BackToArray(g_NPC.GetShenGongFangGoods().m_Wood);
// 				g_NPC.GetShenGongFangGoods().m_Wood.SetID(0);
// 			}
// 
// 			if(g_NPC.GetShenGongFangGoods().m_Luck.GetID() != 0)
// 			{
// 				SELF.PackageGood().BackToArray(g_NPC.GetShenGongFangGoods().m_Luck);
// 				g_NPC.GetShenGongFangGoods().m_Luck.SetID(0);
// 			}
// 		}		
// 	}
// 
// 	if((iSubPage == 7 || iSubPage == 18) && m_iFireFrame != 103 && m_bSendButtonClick)
// 	{
// 		NoteResult();
// 		g_NPC.GetShenGongFangGoods().iResult = -1;
// 	}
	g_WidgetMgr.SetShow(EWT_GREAT_EVENT,false);
	g_WidgetMgr.SetFocus(EWT_GREAT_EVENT,false);

	g_OtherData.SetFlashRelationTabType(0);

}

void CRelationWnd::OnCreate()
{
	RemoveBackDeal();

	int iCurPage = m_TabPage.iCurPage;	
	
	g_WidgetMgr.SetShow(EWT_GREAT_EVENT,false);
	g_WidgetMgr.SetFocus(EWT_GREAT_EVENT,false);

	//注位置需要调整
	if(m_pCloseButton == NULL)	
		SetCloseButton(586,3,80);

	//string str[2] = {"人 脉","行 会"};
	//for (int i = 0;i < 2;i++)
	//{
	//	m_paTabBgBtn[i] = new CCtrlButton;
	//	if(m_iShowWnd != i)
	//		m_paTabBgBtn[i]->CreateEx(this,1,36 + i * 90,85,86,87,88);
	//	else m_paTabBgBtn[i]->CreateEx(this,1,36 + i * 90,86,85,87,88);

	//	m_paTabBgBtn[i]->SetText(str[i].c_str(),RL_PAGE_NORMAL,RL_PAGE_MOUSEON,RL_PAGE_PRESS,RL_PAGE_DISABLE,14,DTF_BlackFrame,FONT_YAHEI,true);
	//	AddControl(m_paTabBgBtn[i]);
	//}


	if (iCurPage == 0)
		CreateRelationWnd();
	else if(iCurPage == 1)	
		CreateGuildWnd();
	else if ( iCurPage ==2 )
		CreateGuildTowerWnd();

	//先CCtrlWindowX::OnCreate(),再SetTabBtnFlashTexID
	CCtrlWindowX::OnCreate();


	DWORD dwType = g_OtherData.GetFlashRelationTabType();
	if (dwType > 0)
	{
		SetTabBtnFlashTexID(int(dwType));
	}

}

void CRelationWnd::SetTabBtnFlashTexID(int iType)
{
	int iPageTabIdx = -1;
	int iCurPage = m_TabPage.iCurPage;
	if (iCurPage != 0 || iCurPage >= m_TabPage.vSubTabPage.size())//不是好友页签
	{
		return;
	}

	if (iType == 0)//好友
	{
		iPageTabIdx = 0;
	}
	else if (iType == 1 || iType == 2)//师徒
	{
		iPageTabIdx = 2;
	}
	else if (iType == 3)//夫妻
	{
		iPageTabIdx = 1;
	}
	else if (iType == 4)//宗族
	{
		iPageTabIdx = 3;
	}
	else if (iType == 5)//黑名单
	{
		iPageTabIdx = 4;
	}
	else if (iType == 6)//结义
	{
		iPageTabIdx = 5;
	}	

	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	if (iPageTabIdx >= 0 && iPageTabIdx != iSubPage && iPageTabIdx < m_TabPage.vSubTabPage[iCurPage].vSubTabPage.size())
	{
		m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iPageTabIdx].pTabBtn->SetFlashTexID(MAKELONG(21588,PACKAGE_INTERFACE));
	}
	else if (iSubPage == iPageTabIdx)
	{
		g_OtherData.SetFlashRelationTabType(0);
	}

}
void CRelationWnd::RemoveBackDeal()
{
	m_pGrid = NULL;//社会关系
	m_pGrid1 = NULL;//联盟行会
	m_pGrid2 = NULL;//敌对行会
	m_pGuildTree = NULL;//行会树
	m_pRadioBtn = NULL;

	m_pAddFriend = NULL;
	m_pDeleteFriend = NULL;
	m_pAddToBlkList = NULL;
	m_pTeamInvite = NULL;
	m_pLeaveNote = NULL;
	//m_pToMasPreBrand = NULL;
	//m_pBackMasPreBrand = NULL;
	m_pRemove = NULL;

	//行会信息量
	m_pGuildScroll01 = NULL;
	m_pGuildScroll02 = NULL;
	m_pAddMemberBtn = NULL;
	m_pDelMemberBtn = NULL;
	m_pEditPublicBtn = NULL;
	m_pEditTitleBtn = NULL;
	m_pSetUnionBtn = NULL;
	m_pCancelUnionBtn = NULL;
	m_pGuildNoChatBtn = NULL;
	m_pPersonNoChatBtn = NULL;
	m_pQuitGuildBtn = NULL;

	//行会人事信息
	m_pScrollHR = NULL;//人事滚动条
	//m_pScrollHRDes = NULL;
	m_pHRUpBtn = NULL;//滚动条处上按钮
	m_pHRDownBtn = NULL;//滚动条向下按钮
	//m_pAssignTael = NULL;

	//行会塔
	m_pMarkViewer = NULL;
	m_pMarkViewer2 = NULL;
	m_pContributeBtn = NULL;	//捐献资源
	m_pUpdateCons = NULL;		//升级建筑	
	m_pEnterTrea = NULL;		//进入珍宝阁
	m_pGetShield = NULL;
	m_pUpdateShield = NULL;
	m_pShengWangUpdate = NULL;
	m_pRefineShield = NULL;
 	m_pScrollTask = NULL;//任务列表
// 	m_pUpTask = NULL;
// 	m_pDownTask = NULL;
// 	m_pScrollDes = NULL;//任务描述
// 	m_pUpDes = NULL;
// 	m_pDownDes = NULL;
	m_pAcceptTask = NULL;//接取任务
	m_pFinishTask = NULL;//完成任务	
	m_pGetPrice = NULL;
	m_pOpenPrice = NULL;
	for (int i = 0;i<4;i++)
	{
		m_pArrayFlag[i] = NULL;
	}

// 	for (int i = 0;i<5;i++)
// 	{
// 		m_paTabRelBtn[i] = NULL;
// 	}

	//m_paTabBgBtn[0] = NULL;
	//m_paTabBgBtn[1] = NULL;

	// 结义
	m_pJieYiTrans = NULL;
	m_pDeleteJieYi = NULL;
	m_pJieYiTeamInvite = NULL;

}

void CRelationWnd::ReshGuildControl()
{
	if (g_GuildData.GetGuildTowerLevel() > 0)
	{
		DWORD dwFunc = g_GuildData.GetGuildFunc();
		if(m_pAddMemberBtn)		m_pAddMemberBtn->SetEnable((dwFunc & eAdmitMember) != 0);
		if(m_pDelMemberBtn)		m_pDelMemberBtn->SetEnable((dwFunc & eKickOutMember) != 0);	
		if(m_pEditPublicBtn)	m_pEditPublicBtn->SetEnable((dwFunc & eEditBulletin) != 0);
		if(m_pEditTitleBtn)		m_pEditTitleBtn->SetEnable((dwFunc & eEditTile) != 0);
		if(m_pSetUnionBtn)		m_pSetUnionBtn->SetEnable((dwFunc & eGuildAlly) != 0);
		if(m_pCancelUnionBtn)	m_pCancelUnionBtn->SetEnable((dwFunc & eDismissAlly) != 0);
		if(m_pGuildNoChatBtn)	m_pGuildNoChatBtn->SetEnable((dwFunc & eGuildShutUp) != 0);
		if(m_pPersonNoChatBtn)	m_pPersonNoChatBtn->SetEnable((dwFunc & ePersonShutUp) != 0);
		if(m_pQuitGuildBtn)     m_pQuitGuildBtn->SetEnable(!g_GuildData.IsHeader());
	}
	else
	{
		// 没有行会塔只有会长
		if (g_GuildData.IsHeader(false))
		{
			if(m_pAddMemberBtn)		m_pAddMemberBtn->SetEnable(true);
			if(m_pDelMemberBtn)		m_pDelMemberBtn->SetEnable(true);
			if(m_pEditPublicBtn)	m_pEditPublicBtn->SetEnable(true);
			if(m_pEditTitleBtn)		m_pEditTitleBtn->SetEnable(true);
			if(m_pSetUnionBtn)		m_pSetUnionBtn->SetEnable(true);
			if(m_pCancelUnionBtn)	m_pCancelUnionBtn->SetEnable(true);
			if(m_pGuildNoChatBtn)	m_pGuildNoChatBtn->SetEnable(true);
			if(m_pPersonNoChatBtn)	m_pPersonNoChatBtn->SetEnable(true);
			if(m_pQuitGuildBtn)     m_pQuitGuildBtn->SetEnable(false);
		}
		else
		{
			if(m_pAddMemberBtn)		m_pAddMemberBtn->SetEnable(false);
			if(m_pDelMemberBtn)		m_pDelMemberBtn->SetEnable(false);
			if(m_pEditPublicBtn)	m_pEditPublicBtn->SetEnable(false);
			if(m_pEditTitleBtn)		m_pEditTitleBtn->SetEnable(false);
			if(m_pSetUnionBtn)		m_pSetUnionBtn->SetEnable(false);
			if(m_pCancelUnionBtn)	m_pCancelUnionBtn->SetEnable(false);
			if(m_pGuildNoChatBtn)	m_pGuildNoChatBtn->SetEnable(false);
			if(m_pPersonNoChatBtn)	m_pPersonNoChatBtn->SetEnable(false);
			if(m_pQuitGuildBtn && strlen(SELF.GetTitle()) > 0)
			{
				m_pQuitGuildBtn->SetEnable(true);
			}
			else
			{
				m_pQuitGuildBtn->SetEnable(false);
			}
		}		
	}	
}

void CRelationWnd::CreateGuildWnd()
{
// 	//创建行会
// 	string str[4] = {"会 员","外 交","所辖宗族","大事件"};
// 
// 	for (int i = 0;i < 4;i++)
// 	{
// 		m_paTabRelBtn[i] = new CCtrlButton;
// 		if(iSubPage != i)
// 			m_paTabRelBtn[i]->CreateEx(this,51 + i * 50,59,17616,17617,17618);
// 		else m_paTabRelBtn[i]->CreateEx(this,51 + i * 50,59,17618,17617,17616);
// 
// 		m_paTabRelBtn[i]->SetText(str[i].c_str(),RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,12,DTF_BlackFrame);
// 		AddControl(m_paTabRelBtn[i]);
// 	}
// 
// 	m_paTabRelBtn[4] = NULL;

	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(!m_bRequestedGuildMsg)
	{
		g_pGameControl->SEND_Guild_Look_Message();//请求行会公告等信息
		m_bRequestedGuildMsg = true;
	}


	LPTexture pTex = NULL;
	//m_vHR.clear();

	DWORD dwFunc = g_GuildData.GetGuildFunc();
	if (iSubPage == 0 || iSubPage == 1)//先建公共的部分
	{
		//添加成员
		m_pAddMemberBtn = new CCtrlButton();
		AddControl(m_pAddMemberBtn);
		m_pAddMemberBtn->CreateEx(this,407,254,90,91,92,93);
		m_pAddMemberBtn->SetText("添加成员",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//删除成员
		m_pDelMemberBtn = new CCtrlButton();
		AddControl(m_pDelMemberBtn);
		m_pDelMemberBtn->CreateEx(this,495,254,90,91,92,93);
		m_pDelMemberBtn->SetText("删除成员",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//公告设置
		m_pEditPublicBtn = new CCtrlButton();
		AddControl(m_pEditPublicBtn);
		m_pEditPublicBtn->CreateEx(this,407,284,90,91,92,93);
		m_pEditPublicBtn->SetText("公告设置",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//编辑封号
		m_pEditTitleBtn = new CCtrlButton();
		AddControl(m_pEditTitleBtn);
		m_pEditTitleBtn->CreateEx(this,495,284,90,91,92,93);
		m_pEditTitleBtn->SetText("编辑封号",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//联盟
		m_pSetUnionBtn = new CCtrlButton();
		AddControl(m_pSetUnionBtn);
		m_pSetUnionBtn->CreateEx(this,407,315,90,91,92,93);
		m_pSetUnionBtn->SetText("联 盟",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//取消联盟
		m_pCancelUnionBtn = new CCtrlButton();
		AddControl(m_pCancelUnionBtn);
		m_pCancelUnionBtn->CreateEx(this,495,315,90,91,92,93);
		m_pCancelUnionBtn->SetText("取消联盟",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//行会禁言
		m_pGuildNoChatBtn = new CCtrlButton();
		AddControl(m_pGuildNoChatBtn);
		m_pGuildNoChatBtn->CreateEx(this,407,346,90,91,92,93);
		m_pGuildNoChatBtn->SetText("行会禁言",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		//////////////////////////////////////////////////////////////////////////
		//行会禁言的功能待定

		//个人禁言
		m_pPersonNoChatBtn = new CCtrlButton();
		AddControl(m_pPersonNoChatBtn);
		m_pPersonNoChatBtn->CreateEx(this,495,346,90,91,92,93);
		m_pPersonNoChatBtn->SetText("个人禁言",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		//////////////////////////////////////////////////////////////////////////
		//个人禁言的功能待定

		m_pQuitGuildBtn = new CCtrlButton();
		AddControl(m_pQuitGuildBtn);
		m_pQuitGuildBtn->CreateEx(this,407,376,90,91,92,93);
		m_pQuitGuildBtn->SetText("退出行会",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//公告和聊天滚动条
		m_pGuildScroll01 = new CCtrlScroll();
		m_pGuildScroll01->CreateEx(this, 361, 255, 14,148);
		m_pGuildScroll01->SetPos(0);
		m_pGuildScroll01->SetRange(100);
		AddControl(m_pGuildScroll01);
		ReshGuildControl();
	}

	if(iSubPage == 0)
	{
		m_pGuildTree = new CCtrlTree();
		AddControl(m_pGuildTree);
		m_pGuildTree->Create(this,54,95,592,229,7);

		//构建一个树
		ConstructTree();
		m_pGuildTree->SetShow(true);
		m_pGuildTree->SetEnable(true);
	}
	else if(iSubPage == 1)
	{
		//联盟行会
		m_pGrid1 = new CCtrlGrid();
		AddControl(m_pGrid1);
		m_pGrid1->CreateEx(this,56,108,312,229,7,17,17665,-5,1.4f);
		m_pGrid1->SetTotalCount(g_GuildData.GetAlly().size());
		m_pGrid1->PushColumn(227);
		m_pGrid1->SetDrawOffXY(0,0);
		m_pGrid1->SetFont(FONT_YAHEI);

		//敌对行会
		m_pGrid2 = new CCtrlGrid();
		AddControl(m_pGrid2);
		m_pGrid2->CreateEx(this,333,108,594,229,7,17,17665,-5,1.4f);
		m_pGrid2->SetTotalCount(g_GuildData.GetKillAlly().size());//共计15行,6列
		m_pGrid2->PushColumn(230);
		m_pGrid2->SetDrawOffXY(0,0);
		m_pGrid2->SetFont(FONT_YAHEI);
	}
	else if (iSubPage == 2)//当前页为2//为人事页面
	{
		m_vHR.clear();
		//发送申请行会银两协议
		g_pGameControl->SEND_Guild_Online_Tael();
		m_iMouseInJob = 0;//默认为总描述
		bool bMaster = false;
		std::vector<CGuildData::sTileNode>& vOff = g_GuildData.GetGuildMemberOfficer();
		if (vOff.size() <= 0 || g_GuildData.GetGuildTowerLevel() <= 0)
			return;

		std::string strMaster;
		std::vector<CGuildData::_NewMember>& vMaster = vOff[0].vMember;//g_GuildData.GetChairMan();
		if (vMaster.size() > 0)
		{
			strMaster = vMaster[0].strName;
			if(strMaster.compare(SELF.GetName()) == 0)
				bMaster = true;		
		}

		std::map<int ,CGuildData::sOfficer>& mapOff = g_GuildData.GetGuildOfficerMap();
		if (mapOff.size() <= 0)
			return;

		std::map<int ,CGuildData::sOfficer>::iterator it = mapOff.begin()++;

		for (;it!= mapOff.end();it++)
		{
			int iNum = -1;
			int iLeft = 0;
			int iDisLeft = 0;
			string str = it->second.strTile;
			std::string::size_type iPos = str.find("亲信");

			if (it->second.wTotalNum <= 0)
			{
				continue;
			}

			if (iPos != std::string::npos)
				continue;

			for (int j = 0;j<vOff.size();j++)
			{
				if (vOff[j].wNum == it->first)
				{
					iNum = j;
					break;
				}
			}

			if (iNum == -1)
			{
				iLeft = it->second.wNum;
				iDisLeft = it->second.wTotalNum - iLeft;
			}
			else
			{
				iLeft = it->second.wNum -  vOff[iNum].vMember.size();
				iDisLeft = it->second.wTotalNum - (vOff[iNum].vMember.size() > it->second.wNum?vOff[iNum].vMember.size():it->second.wNum);
			}
			iLeft = iLeft<0?0:iLeft;			
			iDisLeft = (iDisLeft < 0) ? 0:iDisLeft;
			sHR sNode;
			sNode.bStile = true;
			sNode.strName = it->second.strTile;
			sNode.wJob = it->first;
			m_vHR.push_back(sNode);

			char ctemp[128] = {0};
			sprintf(ctemp,"被任命者必须在附近/胜任此职位需要:%d银两",it->second.dwTael);

			if (it->first == 1 && iNum >= 0 && iNum < vOff.size())//会长也要出现其中
			{
				sHR sNode;
				if (vOff[iNum].vMember.size() <= 0)
					return;
				CGuildData::_NewMember& newMem = vOff[iNum].vMember[0];
				sNode.strName = newMem.strName;
				sNode.bOnline = newMem.bOnline;
				sNode.wJob = 1;
				if (bMaster)
				{
					sNode.pBtnAppoint = new CCtrlButton();
					sNode.pBtnAppoint->CreateEx(this,0,0,95,96,97,98);
					sNode.pBtnAppoint->SetText("任 命",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);

					AddControl(sNode.pBtnAppoint);
					sNode.pBtnAppoint->SetTips(ctemp);
				}				

				if (!sNode.bOnline)
					sNode.dwColorName = 0xFF7F7F7F;

				m_vHR.push_back(sNode);
				memset(&sNode,0,sizeof(sHR));
				sNode.wJob = -1;
				m_vHR.push_back(sNode);
				continue;
			}

			if(iNum >= 0)
			{//有数据
				for (int j = 0;j < vOff[iNum].vMember.size();j++)
				{
					sHR sNode;
					CGuildData::_NewMember& newMem = vOff[iNum].vMember[j];
					sNode.bOnline = newMem.bOnline;
					sNode.strName = newMem.strName;
					if (!sNode.bOnline)
						sNode.dwColorName = 0xFF7F7F7F;

					if (newMem.bAssist)
					{//有亲信
						if (newMem.strAssist.length() > 0)
						{
							sNode.strFavName = newMem.strAssist;
							if (!newMem.bAssOnline)
								sNode.dwColorFavName = 0xFF7F7F7F;
						}
						else 
						{
							sNode.strFavName = "未设立";
							sNode.dwColorFavName = 0xFFFF0000;
						}						

						if ((dwFunc & eAppointFavor) != 0)
						{
							bool bRightFav = false;
							string strTmp = SELF.GetName();							
							if(sNode.strName.compare(SELF.GetName()) == 0)
								bRightFav = true;

							if (bRightFav)
							{
								sNode.pBtnFavorite = new CCtrlButton();
								sNode.pBtnFavorite->CreateEx(this,0,0,95,96,97,98);
								sNode.pBtnFavorite->SetText("任 命",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
								sNode.bAssOnline = newMem.bAssOnline;
								AddControl(sNode.pBtnFavorite);
							}
						}						
					}

					if (bMaster)//只有会长才能任命
					{
						sNode.pBtnAppoint = new CCtrlButton();
						sNode.pBtnAppoint->CreateEx(this,0,0,95,96,97,98);
						AddControl(sNode.pBtnAppoint);
						sNode.pBtnAppoint->SetText("任 命",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
						sNode.pBtnAppoint->SetTips(ctemp);
					}

					sNode.wJob = it->first;
					m_vHR.push_back(sNode);	
				}
			}

			for (int i = 0;i<iLeft + iDisLeft;i++)
			{
				sHR sNode;				
				if (i < iLeft)//罢免官职
				{
					sNode.strName = "未任命";
					sNode.dwColorName = 0xFF00FF00;
				}
				else
				{
					sNode.strName = "未设立";
					sNode.dwColorName = 0xFFFF0000;
				}

				if (bMaster)
				{
					sNode.pBtnAppoint = new CCtrlButton();
					sNode.pBtnAppoint->CreateEx(this,0,0,95,96,97,98);
					sNode.pBtnAppoint->SetText("任 命",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
					AddControl(sNode.pBtnAppoint);
					sNode.pBtnAppoint->SetTips(ctemp);
					if (i>= iLeft) sNode.pBtnAppoint->SetEnable(false);					
				}

				if (it->second.bFav)
				{//会长没有亲信					
					sNode.strFavName = "未设立";
					sNode.dwColorFavName = 0xFFFF0000;
				}
				sNode.wJob = it->first;
				m_vHR.push_back(sNode);
			}
		}

		m_pScrollHR = new CCtrlScroll();
		m_pScrollHR->CreateEx(this,583,85,14,207);
		m_pScrollHR->SetPos(0);
		m_pScrollHR->SetRange(m_vHR.size());
		AddControl(m_pScrollHR);
		m_pScrollHR->SetStep(3);

// 		m_pAssignTael = new CCtrlButton();
// 		m_pAssignTael->CreateEx(this,356,390,132,133,134,135);
// 		m_pAssignTael->SetText("分配公共银两",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
// 		AddControl(m_pAssignTael);
// 		m_pAssignTael->SetEnable((dwFunc & eAssignTael) != 0);
// 		m_pAssignTael->SetTips("行会宝塔每日维护需要的部分资源将转化\n为行会公共银两，供会长自由分配。");		

		std::string str = "    行会除了会长之外共有3个职位，分别是副会长、长老、堂主。宝塔层级越多，可胜任的\\职位就越多。职位越高，获得的攻击、防御等属性加成就越多。每个职位都可任命一个亲信\\。每个职位都需要一定的行会银两才可胜任。";
		m_vStrJobDes.push_back(str);
		str = "    作为行会的灵魂人物，副会长拥有仅次于会长的福利和属性加成，并承担着招贤纳能、\\笼络人才的使命。同时副会长还可以向其他行会发出挑战或结成同盟！当行会宝塔达到10级\\，会长就可以任命一位副会长，20级时可任命两位。同时每个副会长都可以任命一个亲信，\\该亲信享有行会长老的福利。";
		m_vStrJobDes.push_back(str);
		str = "    作为行会的主要决策者，长老们始终肩负着带领行会走向繁荣的重任，同时每位长老也\\将享受仅次于正副会长的福利和属性加成。当行会宝塔达到8级时，会长就可以任命一位长老\\，18级时可任命两位长老。同时每个长老都可以任命一个亲信，该亲信享有行会堂主的福利。";
		m_vStrJobDes.push_back(str);
		str = "    世代以来行会的护法们都守护着自己行会的荣耀，并为行会的壮大做出各种卓越的贡献\\，所以护法们也将享受到行会中较高的福利和属性加成。当行会宝塔达到6级时，会长就可以\\任命一位护法，16级时可任命两位护法。同时每个护法都可以任命一个亲信，该亲信享有行\\会堂主的福利。";
		m_vStrJobDes.push_back(str);
		str = "    作为行会的中流砥柱，各堂堂主一直站在行会事务的第一线，用自己的热血和智慧，燃\\起了行会复兴的熊熊烈火！所以行会为了表彰他们的功绩，也让他们享受到一定的行会福利\\和属性加成。当行会宝塔达到4级时，会长就可以任命一位堂主，14级时可任命两位堂主。同\\时每个堂主都可以任命一个亲信，该亲信享有高于行会成员的福利。";
		m_vStrJobDes.push_back(str);
		str = "    只有在平时的行会事务中作出较大贡献的会员才能被任命为香主，所以每个香主都是行\\会会员中的精英，理所当然的享受着比普通会员更高的福利和属性加成。当行会宝塔建造后\\，会长就可以任命一位香主，2级时可任命两位香主。";
		m_vStrJobDes.push_back(str);

		m_pMarkViewer = new CMarkViewer(16,5);
		AddControl(m_pMarkViewer);
		m_pMarkViewer->Create(this,45,305,550,80);

		m_TagText.Clear();
		m_TagText.Parse(m_vStrJobDes[0],0,"\\",-1,COLOR_TEXT_NORMAL);
		m_pMarkViewer->SetTagText(&m_TagText);
		m_pMarkViewer->SetFont(FONT_YAHEI);

		ReshHRScroll();
	}
	else if (iSubPage == 3)//行会宗派
	{
		if (strlen(SELF.GetTitle()) > 0 && g_PhyleData.GetPhyle().size() <= 0)
		{
			g_pGameControl->SEND_General_Protocol(CS_GUILD_PHYLE_LIST);
		}

		m_pGuildTree = new CCtrlTree();
		AddControl(m_pGuildTree);
		m_pGuildTree->Create(this,52,93,598,403,15,3);

		//构建一个树
		ConstructPhyleTree();
		m_pGuildTree->SetShow(true);
		m_pGuildTree->SetEnable(true);
	}
	else if (iSubPage == 4)//行会大事件
	{
		if(strlen(SELF.GetGuildTitle()) <= 0) return;
		g_WidgetMgr.LoginGuildGreatEvent(g_Login.GetInnerAreaNo(),g_Login.GetInnerGroupNo(),g_GuildData.GetGuildName());
		g_WidgetMgr.MoveWindow(EWT_GREAT_EVENT,m_iScreenX + 50,m_iScreenY + 92,535,306);
		g_WidgetMgr.SetFocus(EWT_GREAT_EVENT,true);
	}
}
void CRelationWnd::ConstructPhyleTree()
{
	if (!m_pGuildTree) return;
	std::map<std::string,CPhyleData::sPhyleMem>& phyleEle = g_PhyleData.GetPhyle();
	std::map<std::string,CPhyleData::sPhyleMem>::iterator it = phyleEle.begin();
	m_pGuildTree->m_bShowTips = false;
	if (it == phyleEle.end())
		return;

	//行会官员数据
	CTreeNode* pRootNode = new CTreeNode(m_pGuildTree,NULL,0,"",false,false);
	m_pGuildTree->SetRootNode(pRootNode);
	CTreeNode* pNode = NULL;
	string strOfficer = "";
	while(it != phyleEle.end())
	{
		string strPhyleName = it->first;
		CPhyleData::sPhyleMem& phyleEle = it->second;
		if (phyleEle.vPhyleMem.size() > 0)
		{
			char ctemp[128] = {0};
			string strPhyleMaster = phyleEle.vPhyleMem[0].strName;
			sprintf(ctemp,"   宗族:%-20s  宗主:%s",strPhyleName.c_str(),strPhyleMaster.c_str());
			CTreeNode* pTreeNode = new CTreeNode(m_pGuildTree,pRootNode,0,ctemp);

			DWORD dwID = phyleEle.vPhyleMem[0].byPhyleType[0];
			//pTreeNode->SetID((dwID == 1 || dwID == 2)?16820 + dwID - 1:0);
			pRootNode->InsertChildNode(pTreeNode);

			for (int i = 1;i<phyleEle.vPhyleMem.size();i++)
			{
				memset(ctemp,0,128);
				strOfficer = g_GuildData.GetOfficerByName(phyleEle.vPhyleMem[i].strName);

				if (strOfficer.size() > 0)
					sprintf(ctemp,"%s(%s)",phyleEle.vPhyleMem[i].strName.c_str(),strOfficer.substr(4).c_str());
				else sprintf(ctemp,"%s",phyleEle.vPhyleMem[i].strName.c_str());

				CTreeNode* pNodeTemp = new CTreeNode(m_pGuildTree,pTreeNode,0,ctemp,true);
				pNodeTemp->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
				dwID = phyleEle.vPhyleMem[i].byPhyleType[0];
				//pNodeTemp->SetID((dwID == 1 || dwID == 2)?16820 - 1 + dwID:0);					
				pTreeNode->InsertChildNode(pNodeTemp);
			}
		}
		it++;
	}
	m_pGuildTree->RefreshTreeRows();

	CTreeNode::SetShowTips(false);
}

void CRelationWnd::ConstructTree()
{
	//构建行会成员信息树
	if (!m_pGuildTree) return;

	//行会官员数据
	CTreeNode* pRootNode = new CTreeNode(m_pGuildTree,NULL,0,"",false,false);
	m_pGuildTree->SetRootNode(pRootNode);
	CTreeNode* pNode = NULL;
	DWORD	dwColor = 0;
	std::vector<CGuildData::sTileNode>& guildOff = g_GuildData.GetGuildMemberOfficer();
	for (int i = 0;i<guildOff.size();i++)
	{
		//开始建树
		CGuildData::sTileNode& treenode = guildOff[i];
		pNode = new CTreeNode(m_pGuildTree,pRootNode,treenode.wNum,treenode.sTile.c_str());
		pNode->SetID(0);
		pRootNode->InsertChildNode(pNode);
		std::vector<CGuildData::_NewMember>& vMember =  treenode.vMember;
		char cOnline = 0;
		for (int j = 0;j<vMember.size();j++)
		{
			CGuildData::_NewMember& newMem = vMember[j];
			if (newMem.bAssist)
			{//有助手							
				CTreeNode* pElement = new CTreeNode(m_pGuildTree,pNode,0,newMem.strName.c_str(),true,true,false,true);
				if (g_PhyleData.IsPhyleMaster(newMem.strName))
				{
					//宗主	
					if (newMem.bOnline)
						pElement->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
					else pElement->SetColor(COLOR_TEXT_NORMAL,0xFFFFCF63);
				}
				else
				{
					if (newMem.bOnline)
						pElement->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
					else pElement->SetColor(COLOR_TEXT_NORMAL,0xFF7F7F7F);
				}

				//pElement->SetID((newMem.byPhyle == 1 || newMem.byPhyle == 2)?16820 + newMem.byPhyle - 1:0);

				pNode->InsertChildNode(pElement);
				if (newMem.strAssist.length() > 0)
				{
					pElement = new CTreeNode(m_pGuildTree,pNode,0,newMem.strAssist.c_str(),true,true,true);

					if (g_PhyleData.IsPhyleMaster(newMem.strAssist))
					{
						if (newMem.bAssOnline)
							pElement->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
						else pElement->SetColor(COLOR_TEXT_NORMAL,0xFFFFCF63);
					}
					else
					{
						if (newMem.bAssOnline)
							pElement->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
						else pElement->SetColor(COLOR_TEXT_NORMAL,0xFF7F7F7F);
					}

					//pElement->SetID((newMem.byAssPhyle == 1 || newMem.byAssPhyle == 2)?16820 + newMem.byAssPhyle - 1:0);
					pNode->InsertChildNode(pElement);					
				}
			}
			else
			{//没有助手
				bool bOfficer = (g_GuildData.GetGuildTowerLevel() > 0);
				CTreeNode* pElement = new CTreeNode(m_pGuildTree,pNode,0,newMem.strName.c_str(),true,true,false,bOfficer);
				if (g_PhyleData.IsPhyleMaster(newMem.strName))
				{
					//////////////////////////////////////////////////////////////////////////
					if (newMem.bOnline)
						pElement->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
					else
						pElement->SetColor(COLOR_TEXT_NORMAL,0xFFFFCF63);
				}
				else
				{
					if (newMem.bOnline)
						pElement->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
					else pElement->SetColor(COLOR_TEXT_NORMAL,0xFF7F7F7F);
				}

				//pElement->SetID((newMem.byPhyle == 1 || newMem.byPhyle == 2)?16820 + newMem.byPhyle - 1:0);
				pNode->InsertChildNode(pElement);
			}			
		}
	}

	//构建成员树
	std::vector<CGuildData::sTileNode>& guildMember = g_GuildData.GetGuildMemberNormal();
	if (guildMember.size() > 0)
	{
		if (g_GuildData.GetGuildTowerLevel() > 0)
		{
			//////////////////////////////////
			pNode = new CTreeNode(m_pGuildTree,pRootNode,0,"成员");
			pRootNode->InsertChildNode(pNode);
		}
		else
		{
			pNode = pRootNode;
		}

		for (int i = 0;i<guildMember.size();i++)
		{
			//开始构建行会成员部分
			CGuildData::sTileNode& treenode = guildMember[i];
			CTreeNode* pNodeTemp = new CTreeNode(m_pGuildTree,pNode,treenode.wNum,treenode.sTile.c_str());
			pNodeTemp->SetID(0);
			pNode->InsertChildNode(pNodeTemp);
			std::vector<CGuildData::_NewMember>& vMember =  treenode.vMember;
			for (int j = 0;j<vMember.size();j++)
			{
				CGuildData::_NewMember& newMem = vMember[j];
				CTreeNode* pElement = new CTreeNode(m_pGuildTree,pNode,0,newMem.strName.c_str(),true);
				if (g_PhyleData.IsPhyleMaster(newMem.strName))
				{
					if (newMem.bOnline)
						pElement->SetColor(COLOR_TEXT_NORMAL,0xFFFFFF00);
					else pElement->SetColor(COLOR_TEXT_NORMAL,0xFFFFCF63);
				}
				else
				{
					if (newMem.bOnline)
						pElement->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
					else pElement->SetColor(COLOR_TEXT_NORMAL,0xFF7F7F7F);
				}

				//pElement->SetID((newMem.byPhyle == 1 || newMem.byPhyle == 2)?16820 + newMem.byPhyle - 1:0);
				pNodeTemp->InsertChildNode(pElement);
			}
		}
	}

	m_pGuildTree->RefreshTreeRows();
	CTreeNode::SetShowTips(true);
}

void CRelationWnd::ReshHRScroll()
{//刷新人事资源滚动条
	int iPageNum = 9;//每页有9行
	int iStartPos = m_pScrollHR->GetPos();
	int iEndPos = m_vHR.size() - 1;
	iStartPos = (iStartPos/3)*3;

	if (iStartPos + iPageNum <= iEndPos)
		iEndPos = iStartPos + iPageNum - 1;

	if (iEndPos < 0) iEndPos = 0;

	int iDevY = 201/iPageNum;
	for (int i = 0;i<m_vHR.size();i++)
	{
		sHR& sNode = m_vHR[i];
		if (i>= iStartPos && i<= iEndPos)//在显示范围内
		{
			if (!sNode.bStile)
			{
				if (sNode.pBtnAppoint)
				{
					sNode.pBtnAppoint->SetShow(true);
					sNode.pBtnAppoint->Move(190,97 + (i - iStartPos)*iDevY);
				}

				if (sNode.pBtnFavorite)
				{
					sNode.pBtnFavorite->SetShow(true);
					sNode.pBtnFavorite->Move(440,97 + (i- iStartPos)* iDevY);
				}
			}
		}
		else//不在显示范围内
		{
			if(sNode.pBtnAppoint)
			{
				sNode.pBtnAppoint->SetShow(false);
			}

			if (sNode.pBtnFavorite)
			{
				sNode.pBtnFavorite->SetShow(false);
			}
		}
	}
}
void CRelationWnd::CreateRelationWnd()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	m_vRelation.clear();
	//公共部分

	//是否显示离线人员
	m_pRadioBtn = new CCtrlRadio();
	m_pRadioBtn->CreateEx(this,501,35,110,20);
	m_pRadioBtn->SetFont(FONT_YAHEI,12);
	//////////////////
	m_pRadioBtn->SetText("显示离线人员",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE);///
	AddControl(m_pRadioBtn);
	m_pRadioBtn->SetChecked(m_bShowOffLineMember);

	//滚动条
	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);	
	int iY = (iSubPage == 3)?-27:0;
	if (iSubPage == SUBPAGE_INDEX_JIEYI)
	{
		m_pGrid->CreateEx(this,54,80 - iY,599,210,6,21,17637,33);
		m_pGrid->PushColumn(110);
		m_pGrid->PushColumn(100);
		m_pGrid->PushColumn(90);
		m_pGrid->PushColumn(90);
		m_pGrid->PushColumn(90);
	}
	else
	{
		m_pGrid->CreateEx(this,54,80 - iY,599,375,(iSubPage != 3)?14:13,21,17637,33);
		m_pGrid->PushColumn(103);
		m_pGrid->PushColumn(153);
		m_pGrid->PushColumn(44);
		m_pGrid->PushColumn(213);
	}
	m_pGrid->SetFont(FONT_YAHEI,14);
	m_pGrid->SetDrawOffXY(0,5);

	

	//string str[5] = {"好 友","夫 妻","师 徒","宗 族","黑名单"};

	//for (int i = 0;i < 5;i++)
	//{
	//	m_paTabRelBtn[i] = new CCtrlButton;
	//	if(iSubPage != i)
	//		m_paTabRelBtn[i]->CreateEx(this,51 + i * 50,34,17616,17617,17618);
	//	else m_paTabRelBtn[i]->CreateEx(this,51 + i * 50,34,17618,17617,17616);

	//	m_paTabRelBtn[i]->SetText(str[i].c_str(),RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,14,DTF_BlackFrame);
	//	AddControl(m_paTabRelBtn[i]);
	//}

	////填充相关数据
	if (iSubPage>= SUBPAGE_INDEX_FRIEND && iSubPage<= SUBPAGE_INDEX_JIEYI)
		FillRelationData(iSubPage);

	if(iSubPage == SUBPAGE_INDEX_FRIEND)
	{
		m_pAddFriend = new CCtrlButton;
		m_pAddFriend->CreateEx(this,112,386,90,91,92,93);
		m_pAddFriend->SetText("加为好友",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pAddFriend);

		m_pDeleteFriend = new CCtrlButton;
		m_pDeleteFriend->CreateEx(this,222,386,90,91,92,93);
		m_pDeleteFriend->SetText("删除好友",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pDeleteFriend);

		m_pTeamInvite = new CCtrlButton;
		m_pTeamInvite->CreateEx(this,332,386,90,91,92,93);
		m_pTeamInvite->SetText("组队邀请",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pTeamInvite);

		m_pAddJieYi = new CCtrlButton;
		m_pAddJieYi->CreateEx(this,442,386,90,91,92,93);
		m_pAddJieYi->SetText("好友结义",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pAddJieYi);
		m_pAddJieYi->SetEnable(SELF.GetLevel() >= 30);
		

		//m_pLeaveNote = new CCtrlButton;
		//m_pLeaveNote->CreateEx(this,484,386,90,91,92,93);
		//m_pLeaveNote->SetText("留    言",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		//AddControl(m_pLeaveNote);
	}else if(iSubPage == SUBPAGE_INDEX_HUSBAND_WIFE)
	{
		m_pTeamInvite = new CCtrlButton;
		m_pTeamInvite->CreateEx(this,276,386,90,91,92,93);
		m_pTeamInvite->SetText("组队邀请",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pTeamInvite);

		//m_pLeaveNote = new CCtrlButton;
		//m_pLeaveNote->CreateEx(this,350,386,90,91,92,93);
		//m_pLeaveNote->SetText("留    言",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		//AddControl(m_pLeaveNote);
	}
	else if(iSubPage == SUBPAGE_INDEX_MASTER_PRENTICE)
	{//师徒
		m_pTeamInvite = new CCtrlButton;
		m_pTeamInvite->CreateEx(this,276,386,90,91,92,93);
		m_pTeamInvite->SetText("组队邀请",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pTeamInvite);

		//m_pLeaveNote = new CCtrlButton;
		//m_pLeaveNote->CreateEx(this,350,386,90,91,92,93);
		//m_pLeaveNote->SetText("留    言",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		//AddControl(m_pLeaveNote);
	}
	else if (iSubPage == SUBPAGE_INDEX_PHYLE)//宗派
	{
		if (m_vRelation.size() <= 0) return;

		m_pRecruitMem = new CCtrlButton;
		m_pRecruitMem->CreateEx(this,154,386,90,91,92,93);
		m_pRecruitMem->SetText("招收弟子",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pRecruitMem);
		if (!g_OtherData.IsPhyleMaster())
			m_pRecruitMem->Disable();

		m_pDismissMem = new CCtrlButton;
		m_pDismissMem->CreateEx(this,270,386,90,91,92,93);
		m_pDismissMem->SetText("开除弟子",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pDismissMem);
		if (!g_OtherData.IsPhyleMaster())
			m_pDismissMem->Disable();

		m_pPhyleOperate = new CCtrlButton;//宗派操作，离开宗派或解散宗派
		m_pPhyleOperate->CreateEx(this,390,386,90,91,92,93);
		if (g_OtherData.IsPhyleMaster())
			m_pPhyleOperate->SetText("解散宗族",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		else m_pPhyleOperate->SetText("离开宗族",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pPhyleOperate);
	}
	else if(iSubPage == SUBPAGE_INDEX_BLACK)
	{//黑名单
		m_pRemove = new CCtrlButton;
		m_pRemove->CreateEx(this,198,386,90,91,92,93);
		m_pRemove->SetText("移    除",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pRemove);

		m_pAddToBlkList = new CCtrlButton;
		m_pAddToBlkList->CreateEx(this,352,386,90,91,92,93);
		m_pAddToBlkList->SetText("加入黑名单",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pAddToBlkList);
	}
	else if (iSubPage == SUBPAGE_INDEX_JIEYI)
	{
		m_pJieYiTrans = new CCtrlButton;
		m_pJieYiTrans->CreateEx(this,132,386,90,91,92,93);
		m_pJieYiTrans->SetText("结义传送",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pJieYiTrans);

		m_pDeleteJieYi = new CCtrlButton;
		m_pDeleteJieYi->CreateEx(this,276,386,90,91,92,93);

		if (!m_vRelation.empty() && strcmp(SELF.GetName(), g_OtherData.GetJieYiLeader().c_str()) == 0)
			m_pDeleteJieYi->SetText("删除结义",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		else
			m_pDeleteJieYi->SetText("退出结义",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		
		AddControl(m_pDeleteJieYi);

		m_pJieYiTeamInvite = new CCtrlButton;
		m_pJieYiTeamInvite->CreateEx(this,420,386,90,91,92,93);
		m_pJieYiTeamInvite->SetText("组队邀请",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pJieYiTeamInvite);

		m_pMarkViewerJieYi = new CMarkViewer(16,8);
		AddControl(m_pMarkViewerJieYi);
		m_pMarkViewerJieYi->Create(this,85,225,550,80);

		m_pMarkViewerJieYi->SetTagText(&m_TagText);
		m_pMarkViewerJieYi->SetEnable(true);
		m_pMarkViewerJieYi->SetShow(true);

		string str = "<color=FFFFFF 达到><color=92D050 30级以后><color=FFFFFF 就能与好友进行结义><color=92D050 （等级越高，可结义好友越多）>\\";
		str += "<color=92D050 结义关系中的好友每点亮一颗珍宝珠就能增加1点当日活跃度。>\\";
		str += "<color=FFFFFF 每当结义关系中增加1个><color=92D050 当日活跃度≥4><color=FFFFFF 的结义好友，结义关系中所有人次日><color=92D050 通过打怪获>\\";
		str += "<color=92D050 得经验值><color=FFFFFF 时会额外增加：>\\";
		str += "<color=FFFFFF 1. 如果结义好友加入结义时等级><color=92D050 < 20级><color=FFFFFF ，最多><color=E36C0A +4%><color=FFFFFF ；>\\";
		str += "<color=FFFFFF 2. 如果结义好友加入结义时等级><color=92D050 ≥20级><color=FFFFFF ，最多><color=E36C0A +2%><color=FFFFFF 。>\\";
		m_TagText.Parse(str,0,"\\",str.length());
	}
}

void CRelationWnd::CreateGuildTowerWnd()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}

	bool bNeedMarkView = false;
	if ( iSubPage == 0 ||//用来显示返回信息的页签
		 iSubPage == 1 ||//主灵塔
        (iSubPage == 2 && iGrandsonPage == 0) ||//珍宝阁介绍
		iSubPage == 3 ||//神工坊
		 iSubPage == 4//议事厅
		)
	{
		bNeedMarkView = true;
	}

	if (bNeedMarkView)
	{
		if (iSubPage == 3)//神工坊 
		{
			m_pMarkViewer = new CMarkViewer(16,10,0,0,true);
			AddControl(m_pMarkViewer);
			m_pMarkViewer->Create(this,226,97,585 - 226,243 - 97);
			m_pMarkViewer->SetDrawOffXY(0,6);
			m_pMarkViewer->SetTagText(&(g_TagTextMgr.GetNpcText()));	
			m_pMarkViewer->SetScrollStep(3);


			m_pMarkViewer2 = new CMarkViewer(16,8,0,0,true);
			AddControl(m_pMarkViewer2);
			m_pMarkViewer2->Create(this,226,252,585 - 226,390 - 253);
			m_pMarkViewer2->SetDrawOffXY(0,8);
			m_pMarkViewer2->SetTagText(&(g_TagTextMgr.GetNpcText2()));	
		}
		else if (iSubPage == 4)//议事厅
		{
			m_pMarkViewer = new CMarkViewer(16,5,0,0,true);
			AddControl(m_pMarkViewer);
			m_pMarkViewer->Create(this,230,221,601 - 247,301 - 221);
			m_pMarkViewer->SetDrawOffXY(0,5);
			if (g_TaskData.GetGuildTaskList().size() > 0)
			{
				m_pMarkViewer->SetTagText(&(g_TaskData.GetGuildTaskList()[0]->strDesc));	
			}		
		}
		else
		{
			m_pMarkViewer = new CMarkViewer(16,30);
			AddControl(m_pMarkViewer);
			if (iSubPage == 1 && iGrandsonPage == 2)//所捐献的资源
			{
				m_pMarkViewer->Create(this,234,107,0,0);
			}
			else
			{
				m_pMarkViewer->Create(this,234,104,0,0);
			}
			m_pMarkViewer->SetTagText(&g_TagTextMgr.GetNpcText());
		}
	}

	//创建按钮
	if (iSubPage == 0)//所有的返回结果信息窗口
	{
	}
	else if (iSubPage == 1)//行会灵塔
	{
		if (iGrandsonPage == 0)
		{
			//捐献资源
			bool bHasTower = (g_GuildData.GetGuildTowerLevel() > 0);
			m_pContributeBtn = new CCtrlButton();
			if (m_pContributeBtn)
			{
				AddControl(m_pContributeBtn);
				//m_pContributeBtn->CreateEx(this,235,350,16108,16109,16110);
				m_pContributeBtn->CreateEx(this,235,350,165,166,167,168);
				m_pContributeBtn->SetText("捐献资源",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);

				m_pContributeBtn->SetEnable(bHasTower);
			}
			//升级建筑
			m_pUpdateCons = new CCtrlButton();
			if (m_pUpdateCons)
			{
				AddControl(m_pUpdateCons);	    
				//m_pUpdateCons->CreateEx(this,365,350,16123,16124,16125,16126);
				m_pUpdateCons->CreateEx(this,365,350,165,166,167,168);
				m_pUpdateCons->SetText("升级建筑",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
				m_pUpdateCons->SetEnable(bHasTower);
			}	
		}
		else if (iGrandsonPage == 1)//捐献资源
		{
			//捐献和查看
			m_pContributeBtn = new CCtrlButton();
			if (m_pContributeBtn)
			{
				AddControl(m_pContributeBtn);
				//m_pContributeBtn->CreateEx(this,370,348,16127,16128,16129);
				m_pContributeBtn->CreateEx(this,370,343,165,166,167,168);
				m_pContributeBtn->SetText("捐  献",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
			}

			m_pViewRes = new CCtrlButton();
			if (m_pViewRes)
			{
				AddControl(m_pViewRes);
				//m_pViewRes->CreateEx(this,370,378,16274,16275,16276);
				m_pViewRes->CreateEx(this,370,373,165,166,167,168);
				m_pViewRes->SetText("查看资源",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
			}
		}
		else if (iGrandsonPage == 2)//所捐献的资源
		{
			//捐献
			m_pContributeBtn = new CCtrlButton();
			if (m_pContributeBtn)
			{
				AddControl(m_pContributeBtn);
				//m_pContributeBtn->CreateEx(this,370,350,16127,16128,16129);
				m_pContributeBtn->CreateEx(this,370,343,165,166,167,168);
				m_pContributeBtn->SetText("捐  献",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
			}

			m_pUpdateCons = new CCtrlButton();
			if (m_pUpdateCons)
			{
				AddControl(m_pUpdateCons);
				//m_pUpdateCons->CreateEx(this,370,380,16123,16124,16125,16126);
				m_pUpdateCons->CreateEx(this,370,373,165,166,167,168);
				m_pUpdateCons->SetText("升级建筑",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
			}
		}
		else if (iGrandsonPage == 3)//升级建筑介绍
		{
			m_pUpdateCons = new CCtrlButton();
			if (m_pUpdateCons)
			{
				AddControl(m_pUpdateCons);
				//m_pUpdateCons->CreateEx(this,246,359,16123,16124,16125,16126);
				m_pUpdateCons->CreateEx(this,246,359,165,166,167,168);
				m_pUpdateCons->SetText("升级建筑",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
			}

			m_pContributeBtn = new CCtrlButton();
			if (m_pContributeBtn)
			{
				AddControl(m_pContributeBtn);
				//m_pContributeBtn->CreateEx(this,370,359,16127,16128,16129);
				m_pContributeBtn->CreateEx(this,370,359,165,166,167,168);
				m_pContributeBtn->SetText("捐  献",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
			}
		}
	}
	else if (iSubPage == 2)//珍宝阁介绍
	{
		if(iGrandsonPage == 0)
		{
			m_pEnterTrea = new CCtrlButton();//增加珍宝阁按钮
			AddControl(m_pEnterTrea);
			//m_pEnterTrea->CreateEx(this,280,350,16270,16271,16272);
			m_pEnterTrea->CreateEx(this,320,340,165,166,167,168);
			m_pEnterTrea->SetText("开启珍宝阁",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
		}
		else if(iGrandsonPage == 1)//珍宝阁
		{
			g_NPC.GetZhenBaoState().bInViewMode = false;
		}
		else if (iGrandsonPage == 2)//极品阁
		{
		}
	}
	else if (iSubPage == 4)//议事厅
	{
		m_pAcceptTask = new CCtrlButton();
		AddControl(m_pAcceptTask);	    
		//m_pAcceptTask->CreateEx(this,240,350,16114,16115,16116);
		m_pAcceptTask->CreateEx(this,240,320,165,166,167,168);
		m_pAcceptTask->SetText("接取任务",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);

		m_pFinishTask = new CCtrlButton();
		AddControl(m_pFinishTask);
		//m_pFinishTask->CreateEx(this,370,350,16190,16191,16192);
		m_pFinishTask->CreateEx(this,370,320,165,166,167,168);
		m_pFinishTask->SetText("完成任务",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
	
		m_pOpenPrice = new CCtrlButton();
		AddControl(m_pOpenPrice);
		m_pOpenPrice->CreateEx(this,240,360,165,166,167,168);
		m_pOpenPrice->SetText("开启每日福利",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
		if (g_GuildData.IsHeader())
		{
			m_pOpenPrice->SetEnable(true);
		}
		else
		{
			m_pOpenPrice->SetEnable(false);
		}	

		m_pGetPrice = new CCtrlButton();
		AddControl(m_pGetPrice);
		m_pGetPrice->CreateEx(this,370,360,165,166,167,168);
		m_pGetPrice->SetText("领取每日福利",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,0);
		//
		//m_pUpTask = new CCtrlButton();
		//AddControl(m_pUpTask);
		//m_pUpTask->CreateEx(this,556,115,16225,16226,16227);

		//m_pDownTask = new CCtrlButton();
		//AddControl(m_pDownTask);
		//m_pDownTask->CreateEx(this,556,210,16228,16229,16230);

		m_pScrollTask = new CCtrlScroll();
		AddControl(m_pScrollTask);
		m_pScrollTask->CreateEx(this,598 - 17,112,17,211 - 112);

		CTaskData::VOldTaskList& vTask = g_TaskData.GetGuildTaskList();
		m_pScrollTask->SetRange(vTask.size());
		m_pScrollTask->SetPos(0);

		//m_pUpDes = new CCtrlButton();
		//AddControl(m_pUpDes);
		//m_pUpDes->CreateEx(this,556,231,16225,16226,16227);

		//m_pDownDes = new CCtrlButton();
		//AddControl(m_pDownDes);
		//m_pDownDes->CreateEx(this,556,297,16228,16229,16230);

		//m_pScrollDes = new CCtrlScroll();
		//AddControl(m_pScrollDes);
		//m_pScrollDes->Create(this,557,249,17,48,16231);

		//[2009-9-17]
		int iRange = 0;
		if (vTask.size() > 0)
		{
			iRange = vTask[0]->strDesc.GetRow() - 1;
			m_wSelTask = vTask[0]->wTaskID;
			if (iRange < 0) iRange = 0;			
		}
// 		m_pScrollDes->SetRange(iRange);
 		m_pScrollTask->SetRange(vTask.size());
	}
// 	else if (iSubPage == 5)//神工坊简介
// 	{
// 		m_pGetProperty = new CCtrlButton();
// 		AddControl(m_pGetProperty);	    
// 		m_pGetProperty->CreateEx(this,230,346,16315,16316,16317);
// 
// 		m_pPropertyRefine = new CCtrlButton();
// 		AddControl(m_pPropertyRefine);	    
// 		m_pPropertyRefine->CreateEx(this,370,346,16320,16321,16322);		
// 	}
// 	else if (iSubPage == 6)//获得盾牌
// 	{
// 		m_pGetShield = new CCtrlButton();
// 		AddControl(m_pGetShield);	    
// 		m_pGetShield->CreateEx(this,310,365,16180,16181,16182);
// 	}
// 	else if (iSubPage == 7 || iSubPage == 18)//提升品质
// 	{
// 		g_NPC.GetShenGongFangGoods().iResult = -1;
// 
// 		m_pRefineShield = new CCtrlButton();
// 		AddControl(m_pRefineShield);	    
// 		m_pRefineShield->CreateEx(this,270,365,16170,16171,16172,16173); 
// 		m_pRefineShield->SetTips("品质提升失败，物品可能破碎！");
// 	}
// 	else if (iSubPage == 13)//仙灵碑窗口 13 14 15 16 17
// 	{
// 		//挑战仙灵大陆
// 		m_pSteleLand = new CCtrlButton();
// 		if (m_pSteleLand)
// 		{
// 			AddControl(m_pSteleLand);
// 			m_pSteleLand->CreateEx(this,270,340,16610,16611,16612,16612);
// 		}
// 	}
// 	else if (iSubPage == 14)
// 	{
// 		//挑战精英战将
// 		m_pSteleHero = new CCtrlButton();
// 		if (m_pSteleHero)
// 		{
// 			AddControl(m_pSteleHero);
// 			m_pSteleHero->CreateEx(this,270,340,16615,16616,16617,16618);
// 		}
// 	}
// 	else if (iSubPage == 15)
// 	{
// 		m_iSelectGuild = 0;
// 		m_iSelectHero = 0;
// 		//发出动员令
// 		m_pSteleBani = new CCtrlButton();
// 		if (m_pSteleBani)
// 		{
// 			AddControl(m_pSteleBani);
// 			m_pSteleBani->CreateEx(this,270,350,16620,16621,16622,16623);
// 		}
// 
// 		//增加
// 		m_pAddHero = new CCtrlButton();
// 		if (m_pAddHero)
// 		{
// 			AddControl(m_pAddHero);
// 			m_pAddHero->CreateEx(this,359,240,16630,16631,16632,16633);
// 		}
// 		//删除按钮
// 		m_pRemoveHero = new CCtrlButton();
// 		if (m_pRemoveHero)
// 		{
// 			AddControl(m_pRemoveHero);
// 			m_pRemoveHero->CreateEx(this,359,170,16625,16626,16627,16628);
// 		}
// 
// 		m_vHeroList.clear();
// 		m_vGuildMember.clear();	
// 		m_vHeroList.push_back(SELF.GetName());
// 
// 		//行会官员数据
// 		std::vector<CGuildData::sTileNode>& guildOff = g_GuildData.GetGuildMemberOfficer();
// 		if (guildOff.size() > 1)
// 		{
// 			for (int i = 1;i<guildOff.size();i++)
// 			{			
// 				CGuildData::sTileNode& treenode = guildOff[i];
// 				std::vector<CGuildData::_NewMember>& vMember =  treenode.vMember;
// 				for (int j = 0;j<vMember.size();j++)
// 				{
// 					CGuildData::_NewMember& newMem = vMember[j];
// 					if (newMem.bAssist)
// 					{//有助手
// 						m_vGuildMember.push_back(newMem.strName);					
// 						if (newMem.strAssist.length() > 0)
// 						{
// 							m_vGuildMember.push_back(newMem.strAssist);
// 						}
// 					}
// 					else
// 					{//没有助手
// 						m_vGuildMember.push_back(newMem.strName);					
// 					}			
// 				}
// 			}
// 		}
// 
// 		//成员
// 		std::vector<CGuildData::sTileNode>& guildMember = g_GuildData.GetGuildMemberNormal();
// 		if (guildMember.size() > 0)
// 		{
// 			for (int i = 0;i<guildMember.size();i++)
// 			{
// 				//开始构建行会成员部分
// 				CGuildData::sTileNode& treenode = guildMember[i];				
// 				std::vector<CGuildData::_NewMember>& vMember =  treenode.vMember;
// 				for (int j = 0;j<vMember.size();j++)
// 				{
// 					m_vGuildMember.push_back(vMember[j].strName);					
// 				}
// 			}
// 		}
// 
// 		m_pScrollStelHero = new CCtrlScroll();
// 		AddControl(m_pScrollStelHero);
// 		m_pScrollStelHero->Create(this,557,164,17,133,16231);
// 
// 		m_pScrollStelHero->SetRange(0);
// 		m_pScrollStelHero->SetPos(0);
// 
// 		m_pStelHeroUpBtn = new CCtrlButton();
// 		AddControl(m_pStelHeroUpBtn);
// 		m_pStelHeroUpBtn->CreateEx(this,556,146,16225,16226,16227);
// 
// 		m_pStelHeroDwnBtn = new CCtrlButton();
// 		AddControl(m_pStelHeroDwnBtn);
// 		m_pStelHeroDwnBtn->CreateEx(this,556,297,16228,16229,16230);
// 
// 
// 
// 		m_pScrollGuildStel = new CCtrlScroll();
// 		AddControl(m_pScrollGuildStel);
// 		m_pScrollGuildStel->Create(this,343,164,17,133,16231);
// 
// 		m_pStelGuildUpBtn = new CCtrlButton();
// 		AddControl(m_pStelGuildUpBtn);
// 		m_pStelGuildUpBtn->CreateEx(this,342,146,16225,16226,16227);
// 
// 		m_pStelGuildDwnBtn = new CCtrlButton();
// 		AddControl(m_pStelGuildDwnBtn);
// 		m_pStelGuildDwnBtn->CreateEx(this,342,297,16228,16229,16230);
// 
// 		m_pScrollGuildStel->SetRange(m_vGuildMember.size());
// 		m_pScrollGuildStel->SetPos(0);
// 	}
// 	else if (iSubPage == 16)
// 	{
// 		m_pSteleAssess = new CCtrlButton();
// 		if (m_pSteleAssess)
// 		{
// 			AddControl(m_pSteleAssess);
// 			m_pSteleAssess->CreateEx(this,280,359,16635,16636,16637,16638);
// 		}
// 
// 		m_pFghValueScroll = new CCtrlScroll();
// 		AddControl(m_pFghValueScroll);
// 		m_pFghValueScroll->Create(this,557,159,17,131,16231);
// 
// 		m_pFghValueUpBtn = new CCtrlButton();
// 		AddControl(m_pFghValueUpBtn);
// 		m_pFghValueUpBtn->CreateEx(this,556,139,16225,16226,16227);
// 
// 
// 		m_pFghValueDwnBtn = new CCtrlButton();
// 		AddControl(m_pFghValueDwnBtn);
// 		m_pFghValueDwnBtn->CreateEx(this,556,291,16228,16229,16230);
// 
// 		m_pFghValueScroll->SetRange(g_GuildData.GetGuildSteleMember().size());
// 		m_pFghValueScroll->SetPos(0);
// 
// 		for (int i = 0;i<8;i++)
// 		{
// 			m_pBtnArray[i] = new CCtrlButton();
// 			m_pBtnArray[i]->CreateEx(this,454,140 + 24 * i,16640,16641,16642,16643);
// 			AddControl(m_pBtnArray[i]);
// 			m_pBtnArray[i]->SetShow(false);
// 		}
// 		//发送评定战斗值
// 	}
// 	else if (iSubPage == 17)
// 	{
// 		m_pSteleMercenary = new CCtrlButton();
// 		if (m_pSteleMercenary)
// 		{
// 			AddControl(m_pSteleMercenary);
// 			m_pSteleMercenary->CreateEx(this,274,340,16605,16606,16607,16608);
// 		}
// 	}
// 	else if (iSubPage == 19)
// 	{
// 		m_pEnterMagicAera = new CCtrlButton();
// 		if (m_pEnterMagicAera)
// 		{
// 			AddControl(m_pEnterMagicAera);
// 			m_pEnterMagicAera->CreateEx(this,274,340,16650,16651,16652,16653);
// 		}
// 	}
// 	else if(iSubPage == 20)//获得道具
// 	{
// 		m_pGetShield = new CCtrlButton();
// 		AddControl(m_pGetShield);	    
// 		m_pGetShield->CreateEx(this,310,335,16180,16181,16182);
// 	}
// 	else if(iSubPage == 21)//道具炼制
// 	{
// 		m_pUpdateShield = new CCtrlButton();
// 		AddControl(m_pUpdateShield);
// 		m_pUpdateShield->CreateEx(this,234,355,16184,16185,16186);
// 
// 		m_pShengWangUpdate = new CCtrlButton();
// 		AddControl(m_pShengWangUpdate);
// 		m_pShengWangUpdate->CreateEx(this,376,355,16447,16448,16449);
// 	}

}

bool CRelationWnd::OnWheel(int iWheel)
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}
	
	int iMouseX, iMouseY;
	g_pControl->GetMouseXY(iMouseX, iMouseY);


	if (iCurPage == 2)
	{
		if(iSubPage == 4)//议事厅
		{
			if (iMouseX >= m_pMarkViewer->GetScreenX() && iMouseX <= m_pMarkViewer->GetScreenX() + m_pMarkViewer->GetWidth() &&
				iMouseY >= m_pMarkViewer->GetScreenY() && iMouseY <= m_pMarkViewer->GetScreenY() + m_pMarkViewer->GetHeight())
			{
				return m_pMarkViewer->OnWheel(iWheel);
			}
			else if(m_pScrollTask)
			{
				return m_pScrollTask->OnWheel(iWheel);
			}
		}	
	}
	else if (iCurPage == 1)
	{
		if(iSubPage == 2)
		{
			//if (iMouseX >= m_pScrollHRDes->GetScreenX() && iMouseX <= m_pScrollHRDes->GetScreenX() + m_pScrollHRDes->GetWidth() &&
			//	iMouseY >= m_pScrollHRDes->GetScreenY() && iMouseY <= m_pScrollHRDes->GetScreenY() + m_pScrollHRDes->GetHeight())
			//{
			//	if(m_pScrollHRDes)
			//		return m_pScrollHRDes->OnWheel(iWheel);
			//}
			//else
			{
				if (m_pScrollHR)
					return m_pScrollHR->OnWheel(iWheel);
			}
		}
		else if (iSubPage == 0)
		{
			if (m_pGuildTree)
				return m_pGuildTree->OnWheel(iWheel);
		}
	}
	return true;
}

void CRelationWnd::FillRelationData(int iType)
{
	int iCurPage = m_TabPage.iCurPage;
	if(iCurPage != 0) 
		return;

	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	int itemp = iType;
	if(iType == - 1) //-1时为，点中是否显示离线人员
		itemp = iSubPage;
	if(itemp != iSubPage) 
		return;

	DWORD iRelation = 0;
	m_vRelation.clear();
	if(itemp == SUBPAGE_INDEX_FRIEND)		iRelation |= RT_FRIEND;
	else if(itemp == SUBPAGE_INDEX_HUSBAND_WIFE)	iRelation |= RT_HUSBAND | RT_WIFE;
	else if(itemp == SUBPAGE_INDEX_MASTER_PRENTICE)	iRelation |= RT_MASTER | RT_PRENTICE;
	else if (itemp == SUBPAGE_INDEX_PHYLE) iRelation |= RT_PHYLE;
	else if(itemp == SUBPAGE_INDEX_BLACK)
	{
		FillBlackList();
		return;
	}
	else if (itemp == SUBPAGE_INDEX_JIEYI)
	{
		iRelation |= RT_JIEYI;
	}

	m_bJieYi = false;

	int iSize = g_OtherData.GetRelationVector().size();
	for(int i = 0;i< iSize;i++)
	{
		_RelationStruct& rl = g_OtherData.GetRelationVector()[i];		

		if (strcmp(SELF.GetName(), rl.strName.c_str()) == 0)
		{
			if (iSubPage == SUBPAGE_INDEX_JIEYI)
			{
				m_dwCurLive = rl.iCurLive;
				m_dwYesterLive = rl.iYesterLive;
				continue;
			}
		}

		if((rl.iRelType&iRelation)!=0)
		{
			m_bJieYi = true;

			if(!m_pRadioBtn->IsChecked())
			{
				if(rl.iOnline == 1)
				{
					_SRelation srl;
					srl.iMuoQi = rl.iMuoQi;
					srl.iJob = rl.iJob;
					srl.iLevel = rl.iLevel;
					srl.iOnline = rl.iOnline;
					srl.sName = rl.strName;
					srl.sGuild = rl.strGuild;
					srl.iPhyle = rl.byPhyleType;
					srl.iJinJie = rl.byJinJie;
					srl.iCurLive = rl.iCurLive;
					srl.iYesterLive = rl.iYesterLive;
					m_vRelation.push_back(srl);
				}
			}
			else
			{
				_SRelation srl;
				srl.iMuoQi = rl.iMuoQi;
				srl.iJob = rl.iJob;
				srl.iLevel = rl.iLevel;
				srl.iOnline = rl.iOnline;
				srl.sName = rl.strName;
				srl.sGuild = rl.strGuild;
				srl.iPhyle = rl.byPhyleType;
				srl.iJinJie = rl.byJinJie;
				srl.iCurLive = rl.iCurLive;
				srl.iYesterLive = rl.iYesterLive;

				if(rl.iOnline == 1)
					m_vRelation.insert(m_vRelation.begin(),srl);
				else m_vRelation.push_back(srl);
			}
		}
	}


/*
#ifdef _DEBUG
	for (int i = 0;i<30;i++)
	{
		_SRelation srl;
		srl.iMuoQi = 100;
		srl.iJob = i%3;
		srl.iLevel = i + 90;
		srl.iOnline = 1;
		char ctemp[128] = {0};
		sprintf(ctemp,"test%d",i);
		srl.sName = ctemp;
		srl.sGuild = "万人敌";
		srl.iPhyle = 0;
		srl.iJinJie = 0;

		m_vRelation.push_back(srl);
	}
#endif // _DEBUG
*/
	

	m_pGrid->SetTotalCount(m_vRelation.size());

	if (iSubPage == SUBPAGE_INDEX_JIEYI && m_pDeleteJieYi)
	{		
		if (!m_vRelation.empty() && strcmp(SELF.GetName(), g_OtherData.GetJieYiLeader().c_str()) == 0)
		{
			m_pDeleteJieYi->SetText("删除结义",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}
		else
		{
			m_pDeleteJieYi->SetText("退出结义",RL_BTN_NORMAL,RL_BTN_MOUSEON,RL_BTN_PRESS,RL_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}
	}
}

void  CRelationWnd::FillBlackList()
{
	//加入黑名单数据
	m_vRelation.clear();
	const CBossClass* pBoss = NULL;
	int iSize = g_BossCfgMgr.GetBossClassCount();
	for(int i = 0;i< iSize;i++)
	{
		pBoss = g_BossCfgMgr.GetBossClass(i);

		if(pBoss->iType == 2)
		{
			_SRelation sr;
			sr.iMuoQi = 0;
			sr.iJob = -1;
			sr.iLevel = -1;
			sr.iOnline = -1;
			sr.sGuild = "";
			sr.sName = pBoss->strName;
			m_vRelation.push_back(sr);
		}
	}
	m_pGrid->SetTotalCount(m_vRelation.size());
}

bool CRelationWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();
	pTip->SetShow(false);
	int x = m_iScreenX + iX + 10;
	int y = m_iScreenY + iY + 10;

	m_iMX = iX;
	m_iMY = iY;

	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}

	if (iCurPage == 0 && iSubPage == SUBPAGE_INDEX_JIEYI)
	{		
		if(iX > 370 && iY > 58 && iX < 370 + 70 && iY < 58 + 20)
		{
			pTip->AddText("每点亮一颗珍宝珠就能增加1点当日活跃度");
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}		
	}
	else if (iCurPage == 2)
	{
		if(g_GuildData.GetGuildTowerLevel() > 0 && iSubPage != 3)//神工坊不要
		{
			if(iX > 513 && iY > 312 && iX < 513 + 71 && iY < 312 + 83)
			{
 				if(iSubPage == 1 && g_GuildData.IsHeader(false))
 				{
 					pTip->AddText("双击更改行会旗帜");				
 				}

				pTip->AddText(g_GuildData.GetGuildName(),0xFFFFFF00);

				char str[128] = "";
				sprintf(str,"行会宝塔等级: %d",g_GuildData.GetGuildTowerLevel());
				pTip->AddText(str,0xFFFF0000);

				const char * title = SELF.GetGuildOfficer();
				if(title != NULL)
				{
					sprintf(str,"%s属性加成：",title);
				}
				else
				{
					strcpy(str,"行会成员属性加成：");
				}

				pTip->AddText(str,0xFF00FF00,-1);

				if(!g_GuildData.IsGuildBufferEff())
				{
					pTip->AddText("行会宝塔资源不足，附加属性被封闭");
				}
				else
				{
					DWORD dwTextColor = -1;

					if(!g_GuildData.IsSelfGuildBufferEff())
					{
						pTip->AddText("必须为行会作出贡献才能享受以下属性加成",0xFFFF0000,-1);
						dwTextColor = 0xFF888888;
					}

					strcpy(str,"");

					CGuildData::_GuildBuffer &buffer = g_GuildData.GetGuildBuffer();
					if(buffer.m_wHP != 0)
					{
						sprintf(str,"生命值上限 %d         ",buffer.m_wHP);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_wMP != 0)
					{
						sprintf(str,"魔法值上限 %d         ",buffer.m_wMP);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byAttackFgh[0] + buffer.m_byAttackFgh[1] != 0 && SELF.GetCareer() == 0)
					{
						sprintf(str,"攻击 %d-%d         ",buffer.m_byAttackFgh[0],buffer.m_byAttackFgh[1]);				
						pTip->AddText(str,dwTextColor,-1);
					}
					else if(buffer.m_byAttackRab[0] + buffer.m_byAttackRab[1] != 0 && SELF.GetCareer() == 1)
					{
						sprintf(str,"魔法攻击 %d-%d     ",buffer.m_byAttackRab[0],buffer.m_byAttackRab[1]);				
						pTip->AddText(str,dwTextColor,-1);
					}
					else if(buffer.m_byAttackDao[0] + buffer.m_byAttackDao[1] != 0 && SELF.GetCareer() == 2)
					{
						sprintf(str,"道术攻击 %d-%d     ",buffer.m_byAttackDao[0],buffer.m_byAttackDao[1]);				
						pTip->AddText(str,dwTextColor,-1);
					}


					if(buffer.m_byPhyDef[0] + buffer.m_byPhyDef[1] != 0)
					{
						sprintf(str,"防御 %d-%d         ",buffer.m_byPhyDef[0],buffer.m_byPhyDef[1]);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byMagicDef[0] + buffer.m_byMagicDef[1] != 0)
					{
						sprintf(str,"魔防 %d-%d         ",buffer.m_byMagicDef[0],buffer.m_byMagicDef[1]);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byMagicHit != 0)
					{
						sprintf(str,"魔法命中 +%d      ",buffer.m_byMagicHit);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byPhyHit != 0)
					{
						sprintf(str,"物理命中 +%d      ",buffer.m_byPhyHit);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byMagicAvoid != 0)
					{
						sprintf(str,"魔法躲避 +%d      ",buffer.m_byMagicAvoid);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byPhyAvoid != 0)
					{
						sprintf(str,"本体物理躲避 +%d      ",buffer.m_byPhyAvoid);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byJinDefend != 0)
					{
						sprintf(str,"金防 %d%%         ",buffer.m_byJinDefend);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byMuDefend != 0)
					{
						sprintf(str,"木防 %d%%      ",buffer.m_byMuDefend);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byTuDefend != 0)
					{
						sprintf(str,"土防 %d%%      ",buffer.m_byTuDefend);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byShuiDefend != 0)
					{
						sprintf(str,"水防 %d%%      ",buffer.m_byShuiDefend);
						pTip->AddText(str,dwTextColor,-1);
					}

					if(buffer.m_byHuoDefend != 0)
					{
						sprintf(str,"火防 %d%%      ",buffer.m_byHuoDefend);
						pTip->AddText(str,dwTextColor,-1);
					}


// 					//神魔buf加成
// 					BYTE byGuildPhyle = g_GuildData.GetGuildPhyleState();
// 					if(byGuildPhyle != 0)
// 					{
// 						if(!g_GuildData.IsSelfGuildBufferEff())
// 						{
// 							pTip->AddText("必须为行会作出贡献才能享受以下供奉属性加成",0xFFFF0000,-1);
// 						}
// 						else
// 						{
// 							if(byGuildPhyle == 1)
// 							{
// 								pTip->AddText("供奉光明能享受以下属性加成",0xFF00FF00,-1);
// 							}
// 							else
// 							{
// 								pTip->AddText("供奉黑暗能享受以下属性加成",0xFF00FF00,-1);
// 							}
// 						}
// 
// 						strcpy(str,"");
// 
// 						CGuildData::_GuildBuffer &buffer = g_GuildData.GetGuildBuffer();
// 						if(buffer.m_byAttackFgh_Phyle[0] + buffer.m_byAttackFgh_Phyle[1] != 0 && SELF.GetCareer() == 0)
// 						{
// 							sprintf(str,"本体攻击 %d-%d         ",buffer.m_byAttackFgh_Phyle[0],buffer.m_byAttackFgh_Phyle[1]);				
// 						}
// 						else if(buffer.m_byAttackRab_Phyle[0] + buffer.m_byAttackRab_Phyle[1] != 0 && SELF.GetCareer() == 1)
// 						{
// 							sprintf(str,"本体魔法攻击 %d-%d     ",buffer.m_byAttackRab_Phyle[0],buffer.m_byAttackRab_Phyle[1]);				
// 						}
// 						else if(buffer.m_byAttackDao_Phyle[0] + buffer.m_byAttackDao_Phyle[1] != 0 && SELF.GetCareer() == 2)
// 						{
// 							sprintf(str,"本体道术攻击 %d-%d     ",buffer.m_byAttackDao_Phyle[0],buffer.m_byAttackDao_Phyle[1]);				
// 						}
// 
// 						if(strlen(str) != 0)
// 						{
// 							pTip->AddText(str,dwTextColor,-1);
// 						}
// 
// 						if(buffer.m_byPhyDef_Phyle[0] + buffer.m_byPhyDef_Phyle[1] != 0)
// 						{
// 							sprintf(str,"本体防御 %d-%d         ",buffer.m_byPhyDef_Phyle[0],buffer.m_byPhyDef_Phyle[1]);
// 							pTip->AddText(str,dwTextColor,-1);
// 						}
// 
// 
// 						if(buffer.m_byMagicDef_Phyle[0] + buffer.m_byMagicDef_Phyle[1] != 0)
// 						{
// 							sprintf(str,"本体魔防 %d-%d         ",buffer.m_byMagicDef_Phyle[0],buffer.m_byMagicDef_Phyle[1]);
// 							pTip->AddText(str,dwTextColor,-1);
// 						}
// 
// 						if(buffer.m_byMagicHit_Phyle != 0)
// 						{
// 							sprintf(str,"本体魔法命中 +%d      ",buffer.m_byMagicHit_Phyle);
// 							pTip->AddText(str,dwTextColor,-1);
// 						}
// 
// 						if(buffer.m_byPhyHit_Phyle != 0)
// 						{
// 							sprintf(str,"本体物理命中 +%d      ",buffer.m_byPhyHit_Phyle);
// 							pTip->AddText(str,dwTextColor,-1);
// 						}
// 
// 						if(buffer.m_byMagicAvoid_Phyle != 0)
// 						{
// 							sprintf(str,"本体魔法躲避 +%d      ",buffer.m_byMagicAvoid_Phyle);/////////////////////////////
// 							pTip->AddText(str,dwTextColor,-1);
// 						}
// 
// 						if(buffer.m_byPhyAvoid_Phyle != 0)
// 						{
// 							sprintf(str,"本体物理躲避 +%d      ",buffer.m_byPhyAvoid_Phyle);
// 							pTip->AddText(str,dwTextColor,-1);
// 						}					
// 					}


				}

				pTip->AdjustXY(x,y);
				pTip->Move(x,y);//////////////////////////////////////////////////////////////////////////
				pTip->SetShow(true);
				return true;
			}
		}

// 		if(iSubPage == 7 || iSubPage == 18)
// 		{
// 			int iInGrid = -1;
// 			CGood * pGood = NULL;
// 
// 			if(iX > 449 && iX < (449 + 40) && iY > 185 && iY < (185 + 35))
// 			{
// 				if(!IsStop())
// 				{
// 					pGood = &m_tmpShield;
// 				}
// 				else
// 				{
// 					pGood = &(g_NPC.GetShenGongFangGoods().m_Shield);
// 				}
// 				iInGrid = 1;
// 			}
// 			else if(iX > 421 && iX < (421 + 40) && iY > 259 && iY < (259 + 35))
// 			{
// 				pGood = &(g_NPC.GetShenGongFangGoods().m_Wood);	
// 				iInGrid = 2;
// 			}
// 			else if(iX > 483 && iX < (483 + 40) && iY > 259 && iY < (259 + 35))
// 			{
// 				pGood = &(g_NPC.GetShenGongFangGoods().m_Luck);	
// 				iInGrid = 3;
// 			}
// 
// 			if(pGood != NULL && pGood->GetID() != 0)
// 			{
// 				pTip->Parse(*pGood);
// 				pTip->AdjustXY(x,y);
// 				pTip->Move(x,y);
// 				pTip->SetShow(true);
// 				return true;
// 			}
// 			else if(iInGrid != -1)
// 			{
// 				if(iSubPage == 7)
// 				{	
// 					if(iInGrid == 1)
// 						pTip->AddText("请放入想要提升的物品");
// 					else if(iInGrid == 2)
// 						pTip->AddText("请放入高纯度木材");
// 				}
// 				else if(iSubPage == 18)
// 				{
// 					if(iInGrid == 1)
// 						pTip->AddText("请放入想要融合的首饰");
// 					else if(iInGrid == 2)
// 						pTip->AddText("请放入圣王精魄");
// 				}
// 
// 				if(iInGrid == 3)
// 				{
// 					pTip->AddText("放入护宝符可确保物品不碎",-1);
// 					//////////////////////////////////////////////////////////////////////////
// 					pTip->AddText("放入幸运符可提高成功概率",-1);
// 				}
// 
// 				pTip->AdjustXY(x,y);
// 				pTip->Move(x,y);
// 				pTip->SetShow(true);
// 				return true;
// 			}
// 
// 		}
		if(iSubPage == 2 && iGrandsonPage == 1)//珍宝阁
		{
			int iRealMouseOnGrid = -1;
			int iGrid = GetGrid(iX,iY,&iRealMouseOnGrid);//鼠标所在的格子
			int iMaxGrids = GetZhenBaoGeGrids();	
			char temp[128] = "";

			if (iRealMouseOnGrid >= 0 && iRealMouseOnGrid >= iMaxGrids)
			{
				sprintf(temp,"行会宝塔%d级时开启此列",10 + (iRealMouseOnGrid - 42) / 7 * 20);
				pTip->AddText(temp,-1,-1);
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
				return true;
			}
			else if(iGrid >= 0 && iGrid < 77)
			{
				_ZhenBaoState& state = g_NPC.GetZhenBaoState();

				if(state.grid[iGrid].iState == 2 || state.grid[iGrid].iState == 1)	//已经打开
				{
					if(GetTickCount() - state.grid[iGrid].dwOpenTime < 1600)
					{
						return CCtrlWindowX::OnMouseMove(iX,iY);
					}

					if(state.grid[iGrid].iType == 1 || state.grid[iGrid].iType == 2)
					{					
						sprintf(temp,"由玩家%s取走了\n%s",state.grid[iGrid].strOwnerName.c_str(),state.grid[iGrid].strPrizeName.c_str());					
					}
					else if(state.grid[iGrid].iType == 0)
					{
						sprintf(temp,"由玩家%s取走了\n%d经验值",state.grid[iGrid].strOwnerName.c_str(),state.grid[iGrid].iCount);
					}
					else if(state.grid[iGrid].iType == 3)
					{
						sprintf(temp,"玩家%s发现了极品阁",state.grid[iGrid].strOwnerName.c_str());
					}
					else if(state.grid[iGrid].iType == 4)
					{
						sprintf(temp,"玩家%s打开了极品阁",state.grid[iGrid].strOwnerName.c_str());
					}

					pTip->AddText(temp,-1);
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
					return true;
				}

				return true;
			}
		}
		else if(iSubPage == 2 && iGrandsonPage == 2)//极品阁
		{
			int iGrid = GetGrid(iX,iY);		//鼠标所在的格子


			if(iGrid >= 0 && iGrid < 36)
			{
				_ZhenBaoState& state = g_NPC.GetZhenBaoState();
				if(state.jipingrid[iGrid].iState == 2)//已经打开
				{
					if(state.jipingrid[iGrid].iType == 1)	//骷髅
					{

					}
					else
					{
						if(GetTickCount() - state.jipingrid[iGrid].dwOpenTime < 1600)
						{
							return CCtrlWindowX::OnMouseMove(iX,iY);
						}

						pTip->Parse(state.jipingrid[iGrid].prize);
						char temp[128] = "";
						sprintf(temp,"由玩家%s获得",state.jipingrid[iGrid].strOwnerName.c_str());
						pTip->AddText(temp,-1);
						pTip->AdjustXY(x,y);
						pTip->Move(x,y);
						pTip->SetShow(true);
						return true;
					}
				}


				return true;
			}
		}
		else if (iSubPage == 1 && (iGrandsonPage == 1 || iGrandsonPage == 2))//捐献资源,所捐献的资源
		{
			if (iX >= 230 && iX <= 330 + 32 && iY >= 357 && iY <= 357 + 32)
			{
				pTip->Clear();

				int iSel = 0;
				if (iX >= 230 && iX <= 230 + 32)
					iSel = 1;
				else if (iX >= 280 && iX <= 280 + 32)
					iSel = 2;
				else if (iX >= 330 && iX <= 330 + 32)
					iSel = 3;

				if (iSel != 0)
				{	
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
					CGoodArray& goodArray = SELF.GetDevGuildGood();
					if (goodArray.Get(iSel - 1).GetID() != 0)
					{
						pTip->Parse(goodArray.Get(iSel - 1));
					}
					else
					{
						pTip->AddText("请放入所要捐献的资源");
					}
					return true;			
				}
			}
		}
	}	

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

// void CRelationWnd::SwitchToPage(int iShowWnd,int iPage)
// {
// 	RemoveBodyChildControl();
// 	RemoveBackDeal();
// // 	m_bTemp = false;
// 	if(iShowWnd == 0)
// 	{
// 		SetPageTex(0,PACKAGE_INTERFACE,(iPage!=3)?17690:17691);
// 	}
// 	else if (iShowWnd == 1)
// 	{
// 		if(!m_bRequestedGuildMsg)
// 		{
// 			g_pGameControl->SEND_Guild_Look_Message();//请求行会公告等信息
// 			m_bRequestedGuildMsg = true;
// 		}
// 
// 		SetPageTex(0,PACKAGE_INTERFACE,17692+iPage);
// 	}
// 	else if (iShowWnd == 2)
// 	{
// 		m_iIndex = iPage + 16250 - 1;//底板
// 		if (iPage == 11)
// 			m_iIndex = 16250;
// 		if(iPage == 6)//获得盾牌
// 			m_iIndex = 16254;
// 		else if (iPage == 13 || iPage == 14 || iPage == 17 || iPage == 19)
// 			m_iIndex = 16600;
// 		else if (iPage == 15)
// 			m_iIndex = 16601;
// 		else if (iPage == 16)
// 			m_iIndex = 16602;
// 		else if (iPage == 18)
// 			m_iIndex = 16446;
// 
// 		SetPageTex(0,PACKAGE_INTERFACE,m_iIndex);
// 	}
// 	//iSubPage = iPage;
// 	//m_iShowWnd = iShowWnd;
// 	OnCreate();
// }

bool CRelationWnd::Msg(DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	bool bRet = false;
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
    int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}

	switch(dwMsg)
	{
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if (!pControl)
				return true;

			S_TabPage * pParentTabpage = (S_TabPage * )pControl;
			if (pParentTabpage == &(m_TabPage.vSubTabPage[2]))//行会塔界面
			{
				//if(iCurPage == 2 && iSubPage == 2 && iGrandsonPage == 2 && !g_NPC.GetZhenBaoState().bInViewMode)
				//{
				//	g_MsgBoxMgr.PopSimpleComfirm("你确定要离开极品阁吗？",MSG_CTRL_LEAVE_JIPINGE,100 + dwData);
				//	return true;
				//}


				if (dwData == 1)//主灵塔
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@main");
					return true;
				}
				else if (dwData == 2)
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@OpenZBGrid");
					return true;
				}
				else if (dwData == 3)
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@shengongfang");
					return true;
				}
				else if (dwData == 4)
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@RecTask");
					return true;
				}
				//else if (dwData == 5)
				//{
				//	//仙灵碑
				//	g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@XianLingBei");
				//	return true;
				//}
			}
			else if (pParentTabpage == &m_TabPage)
			{
				//if(iCurPage == 2 && iSubPage == 2 && iGrandsonPage == 2 && !g_NPC.GetZhenBaoState().bInViewMode)
				//{
				//	g_MsgBoxMgr.PopSimpleComfirm("你确定要离开极品阁吗？",MSG_CTRL_LEAVE_JIPINGE,104);
				//	return true;
				//}
				//else 
				if (dwData == 2)//行会塔主界面
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@main");
					return true;		
				}
			}
		}
		break;
	case MSG_CTRL_RELATION_WND:
		{
			switch(dwData)
			{
			case 10:
				FillRelationData(SUBPAGE_INDEX_FRIEND);				
				break;
			case 12:
				FillRelationData(SUBPAGE_INDEX_BLACK);
				break;
			case 13:
				FillRelationData(SUBPAGE_INDEX_MASTER_PRENTICE);
				break;
			case 14:
				FillRelationData(SUBPAGE_INDEX_HUSBAND_WIFE);
				break;
			case 15:
				if (iCurPage == 1 && iSubPage == 0)//重新构造数据
					SwitchToPage(iSubPage,&(m_TabPage.vSubTabPage[iCurPage]),true);
				break;
			case 18:
				DelAllySuccess((const char *)pControl);
				break;
			case 19:
				AddAllySuccess((const char *)pControl);
				break;
			case 22:
				SwitchToPage(3,&(m_TabPage.vSubTabPage[0]),true);
				break;
			case 21:
				if (iCurPage == 1 && (iSubPage == 2 || iSubPage == 1))
					SwitchToPage(iSubPage,&(m_TabPage.vSubTabPage[iCurPage]),true);
				break;
			case 20:
				{
					if (iCurPage == 0 && iSubPage == SUBPAGE_INDEX_PHYLE)
						FillRelationData(SUBPAGE_INDEX_PHYLE);
				}
				break;

			case 30:
				FillRelationData(SUBPAGE_INDEX_JIEYI);
				break;
			case OPER_CUSTOM:
				{
					if(iCurPage == 2)
					{
						CloseWindow();
						return true;
					}
				}			
				break;
			case OPER_CUSTOM + 1:
				{
					if(iCurPage == 2 && iSubPage != 3)
					{
						m_iTowerLevelUpFrame = 0;
						return true;
					}
				}			
				break;
			case OPER_CUSTOM + 2:
				{
					if(iCurPage == 2 && iSubPage == 4)
					{
						m_iSkullFrame = 0;
						return true;
					}
				}
				break;
			case OPER_CUSTOM + 3:
				{
					if(iCurPage == 2 && iSubPage == 3)
					{
						m_iShineFrame = 0;
						return true;
					}
				}
				break;
			case OPER_CUSTOM + 104:
				{
					SwitchToPage(0,&(m_TabPage.vSubTabPage[0]),true);
				}
				break;
			case OPER_CUSTOM + 105:
				{
					SwitchToPage(0,&(m_TabPage.vSubTabPage[1]),true);
				}
				break;
			default:
				return false;
			}
		}
		return true;
		break;
	case MSG_CTRL_FLASH_BTN_WND:
		{
			if (iCurPage == 0)
			{
				SetTabBtnFlashTexID(int(dwData));
				return true;
			}
			return false;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
// 			for (int i = 0;i<2;i++)
// 			{
// 				if (pControl == m_paTabBgBtn[i] && m_iShowWnd != i)
// 				{
// 					m_paTabBgBtn[m_iShowWnd]->ReloadTex(85,86,87,88);
// 					m_paTabBgBtn[i]->ReloadTex(86,85,87,88);
// 
// 					m_iShowWnd = i;
// 					SwitchToPage(m_iShowWnd,0);
// 					return true;
// 				}
// 			}

			if (pControl == m_pCloseButton)
			{
				CloseWindow(true);
				return true;
			}
		}
		break;
	default:
		break;
	}


	if(iCurPage == 0)
	{
		bRet = DealRelationMsg(dwMsg,dwData,pControl);
	}
	else if(iCurPage == 1)
	{
		bRet = DealGuildMsg(dwMsg,dwData,pControl);
	}
	else if (iCurPage == 2)
	{
		bRet = DealGuildTowerMsg(dwMsg,dwData,pControl);
	}

	if (bRet) 
	{
		return true;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void  CRelationWnd::OnDeletePhyle()
{
	int iSelLine = m_pGrid->GetSelLine();
	if(iSelLine < 0 || iSelLine >= m_vRelation.size())
	{
		g_MsgBoxMgr.PopSimpleAlert("请先选择你要删除的宗族弟子!");
		return;
	}

	g_pGameControl->SEND_Phyle_Operate(m_vRelation[iSelLine].sName.c_str(),1);
}

void CRelationWnd::OnDeleteFriend()
{
	int iSelLine = m_pGrid->GetSelLine();
	if(iSelLine < 0 || iSelLine >= m_vRelation.size())
	{
		g_MsgBoxMgr.PopSimpleAlert("请选择要删除的好友的名字!");
		return ;
	}

	g_pGameControl->SEND_Del_Friend(m_vRelation[iSelLine].sName.c_str());
}

bool CRelationWnd::DealRelationMsg(DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	static DWORD dwEslapseTime = 0;
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pRadioBtn)
			{
				m_bShowOffLineMember = m_pRadioBtn->IsChecked();
				//修改数据
				FillRelationData(-1);
			}

			if(iSubPage == SUBPAGE_INDEX_FRIEND)
			{
				if(pControl == m_pAddFriend)
				{
					g_pControl->PopupWindow(MSG_GUILDADDMEMBER_WND,OPER_UPDATE,1);
					return true;
				}
				else if(pControl == m_pDeleteFriend)
				{
					OnDeleteFriend();
					return true;
				}
				else if(pControl == m_pAddToBlkList)
				{
				}
				else if(pControl == m_pTeamInvite)
				{
					OnTeamInvite();
					return true;
				}else if(pControl == m_pLeaveNote)
				{
					OnLeaveNote();
					return true;
				}
				else if (pControl == m_pAddJieYi)
				{
					OnAddJieYi();
					return true;
				}
			}
			else if(iSubPage == SUBPAGE_INDEX_HUSBAND_WIFE)
			{
				if(pControl == m_pTeamInvite)
				{
					OnTeamInvite();
					return true;
				}
				else if(pControl == m_pLeaveNote)
				{
					OnLeaveNote();
					return true;
				}
			}
			else if(iSubPage == SUBPAGE_INDEX_MASTER_PRENTICE)
			{
				if(pControl == m_pAddToBlkList)
				{
				}
				else if(pControl == m_pTeamInvite)
				{
					OnTeamInvite();
					return true;
				}
				else if(pControl == m_pLeaveNote)
				{
					OnLeaveNote();
					return true;
				}/*else if(pControl == m_pToMasPreBrand)
				{
					OnToMaster(dwEslapseTime);
					return true;
				}
				else if(pControl == m_pBackMasPreBrand)
				{
					OnBackMaster(dwEslapseTime);
					return true;
				}*/
			}
			else if (iSubPage == SUBPAGE_INDEX_PHYLE)
			{
				if(m_pRecruitMem == pControl)
				{//招收弟子					
					g_pControl->PopupWindow(MSG_GUILDADDMEMBER_WND,OPER_UPDATE,3);//招收宗派弟子
					return true;
				}
				else if (m_pDismissMem == pControl)
				{//解散弟子					
					OnDeletePhyle();
					return true;
				}
				else if (m_pPhyleOperate == pControl)
				{
					//离开宗派或解散宗派
					if (g_OtherData.IsPhyleMaster())
					{//解散宗派						
						g_MsgBoxMgr.PopOkCancelBox("你确定真的要解散你的宗族吗？",MSG_CTRL_DISMISS_PHYLE,0);
						return true;
					}
					else
					{//离开宗派
						char ctemp[256]={0};
						if (strlen(g_GuildData.GetGuildName()) > 0)
						{
							sprintf(ctemp,"你现在是%s宗族的弟子，你离开宗族后，将会同时离开%s行会，你确定退出吗？",g_OtherData.GetPhyleName().c_str(),g_GuildData.GetGuildName());
						}
						else
						{
							sprintf(ctemp,"你确定真的要离开你的宗族吗？");
						}						

						g_MsgBoxMgr.PopOkCancelBox(ctemp,MSG_CTRL_LEAVE_PHYLE,0);						
						return true;
					}
				}				
			}
			else if(iSubPage == SUBPAGE_INDEX_BLACK)
			{
				if(pControl == m_pRemove)
				{
					OnDeleteBlkList();
					return true;
				}
				else if(pControl == m_pAddToBlkList)
				{
					g_pControl->PopupWindow(MSG_GUILDADDMEMBER_WND,OPER_UPDATE,2);
					return true;
				}
			}
			else if (iSubPage == SUBPAGE_INDEX_JIEYI)
			{
				if (pControl == m_pJieYiTrans)
				{
					OnClickTrans();
					return true;
				}
				else if(pControl == m_pDeleteJieYi)
				{
					OnDeleteJieYi();
					return true;
				}
				else if(pControl == m_pJieYiTeamInvite)
				{
					OnTeamInvite();
					return true;
				}
			}

// 			for (int i = 0;i<5;i++)
// 			{
// 				if (pControl == m_paTabRelBtn[i])
// 				{
// 					if (iSubPage != i)
// 					{
// 						iSubPage = i;
// 						m_paTabRelBtn[iSubPage]->ReloadTex(17616,17617,17618);
// 						m_paTabRelBtn[i]->ReloadTex(17618,17617,17616);
// 
// 						SwitchToPage(0,i);
// 						break;
// 					}						
// 				}
// 			}			
		}
		break;
	case MSG_CTRL_GRID_DBCLICK:
		OnLeftButtonDClick(HIWORD(dwData),LOWORD(dwData));
		return true;
		break;
	default:
		return false;
	}

	return false;
}

void CRelationWnd::OnDeleteBlkList()
{
	int iSelLine = m_pGrid->GetSelLine();
	if(iSelLine < 0 || iSelLine >= m_vRelation.size())
		return ;

	std::string strName = m_vRelation[iSelLine].sName;
	bool bRet = g_BossCfgMgr.RemoveBlackName(strName.c_str());
	if(bRet) 
		m_vRelation.erase(m_vRelation.begin()+iSelLine);

	m_pGrid->SetTotalCount(m_vRelation.size());
}

void CRelationWnd::OnToMaster(DWORD& dwTime)
{
	int iSelLine = m_pGrid->GetSelLine();
	if(iSelLine < 0 || iSelLine >= m_vRelation.size())
		return ;

	std::string sName = m_vRelation[iSelLine].sName;
	if(GetTickCount() - dwTime>5000)
	{
		dwTime = GetTickCount();
		//g_pGameControl->Send_Borrow_Yuanshen(5,0,sName.c_str());
	}
	else
	{
		g_TalkMgr.PushSysTalk("每隔一段时间才可以感应到你的师尊令牌。");
	}
}

void CRelationWnd::OnBackMaster(DWORD& dwTime)
{
	int iSelLine = m_pGrid->GetSelLine();
	if(iSelLine < 0 || iSelLine >= m_vRelation.size())
		return ;

	std::string sName = m_vRelation[iSelLine].sName;
	if(GetTickCount() - dwTime>5000)
	{
		dwTime = GetTickCount();
		//g_pGameControl->Send_Borrow_Yuanshen(6,0,sName.c_str());
	}
	else
	{
		g_TalkMgr.PushSysTalk("每隔一段时间才可以感应到你的师尊令牌。");
	}
}

void CRelationWnd::OnTeamInvite()
{
	if( g_pGameData->GetTroopMembers() > 0 && 
		(strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) != 0))//组队了但不是队长，立即反回
	{
		g_MsgBoxMgr.PopSimpleAlert("你已经在队伍中，不是队长不可以邀请其它玩家组队。");
		return;
	}

	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(iCurPage == 0 && iSubPage != SUBPAGE_INDEX_PHYLE)
	{
		int iSelLine = m_pGrid->GetSelLine();
		if(iSelLine < 0 || iSelLine >= m_vRelation.size())
			return ;

		if( g_pGameData->GetTroopMembers() <= 0 ) //未组队
		{
			g_pGameControl->SEND_Group_Create( m_vRelation[iSelLine].sName.c_str() ); 
		}
		else//组队了是队长
		{
			g_pGameControl->SEND_Group_Add( m_vRelation[iSelLine].sName.c_str() );
		}
	}
}

void CRelationWnd::OnLeaveNote()
{
	int iSelLine = m_pGrid->GetSelLine();
	if(iSelLine < 0 || iSelLine >= m_vRelation.size())
	{
		g_MsgBoxMgr.PopSimpleAlert("请先选中一个玩家");
		return;
	}

	const char *p = m_vRelation[iSelLine].sName.c_str();
	g_pControl->PopupWindow(MSG_CTRL_LEAVE_NOTE_WND,OPER_CREATE,DWORD(__int64(p)));
}

bool CRelationWnd::DealGuildMsg(DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
// 			for (int i = 0;i < 4;i++)
// 			{
// 				if (pControl == m_paTabRelBtn[i])
// 				{
// 					if (iSubPage != i)
// 					{
// 						iSubPage = i;
// 						m_paTabRelBtn[iSubPage]->ReloadTex(17616,17617,17618);
// 						m_paTabRelBtn[i]->ReloadTex(17618,17617,17616);
// 						SwitchToPage(1,i);						
// 						return true;
// 					}						
// 				}
// 			}

			if (iSubPage == 0 || iSubPage == 1)
			{
				if(pControl == m_pAddMemberBtn)
				{
					g_pControl->PopupWindow(MSG_GUILDADDMEMBER_WND,OPER_UPDATE,0);
					return true;
				}else if(pControl == m_pDelMemberBtn)
				{
					g_pControl->PopupWindow(MSG_GUILDDELMEMBER_WND,OPER_CREATE,0);
					return true;
				}else if(pControl == m_pEditPublicBtn)
				{
					g_pControl->PopupWindow(MSG_GUILD_EDIT,OPER_RECREATE,2);
					return true;
				}else if(pControl == m_pEditTitleBtn)
				{
					g_pControl->PopupWindow(MSG_GUILD_EDIT,OPER_RECREATE,1);
					return true;
				}else if(pControl == m_pSetUnionBtn)
				{
					g_pControl->Msg(MSG_CTRL_GUILD_ALLYWND,OPER_CREATE);
					return true;
				}else if(pControl == m_pCancelUnionBtn)
				{
					//获是选取中的行会名称
					if(iCurPage == 1 && iSubPage == 1)
					{
						int iPos = m_pGrid1->GetSelLine();
						if(iPos < 0 || iPos >= g_GuildData.GetAlly().size()) 
							return true;
						const char* pChar = g_GuildData.GetAlly()[iPos].c_str();
						if(pChar && strlen(pChar) > 0)
						{
							char str_temp[1024]={0};
							sprintf(str_temp,"@取消联盟 %s",pChar);
							g_pGameControl->SEND_Guild_Ally(str_temp);
						}					
					}
					else g_MsgBoxMgr.PopSimpleAlert("请先选择你想取消联盟的行会!");/////////////////////////////
					
					return true;
				}else if(pControl == m_pGuildNoChatBtn)
				{
					string str = "@禁止全行会聊天";
					g_pGameControl->SEND_Message_Send(str.c_str(),str.size());
					return true;
				}else if(pControl == m_pPersonNoChatBtn)
				{
					g_pControl->PopupWindow(MSG_GUILDDELMEMBER_WND,OPER_CREATE,1);
					return true;
				}
				else if(pControl == m_pQuitGuildBtn)
				{
					char ctemp[256]={0};
					if (g_OtherData.IsPhyleMaster())
					{	
						//////////////////////////////////////////////////////////////////////////
						sprintf(ctemp,"你是%s宗族的宗主，你离开行会后，你宗族下的所有成员\n都会离开行会，你确定退出吗？",g_OtherData.GetPhyleName().c_str());
						g_MsgBoxMgr.PopOkCancelBox(ctemp,MSG_CTRL_LEAVE_GUILD,0);
					}
					else if (g_OtherData.GetPhyleName().length() > 0)
					{
						sprintf(ctemp,"你是%s宗族的弟子，你离开行会后，你也将离开这个宗族，\n你确定退出吗？",g_OtherData.GetPhyleName().c_str());
						g_MsgBoxMgr.PopOkCancelBox(ctemp,MSG_CTRL_LEAVE_GUILD,0);
					}
					else
					{
						g_MsgBoxMgr.PopSimpleComfirm("你是否确定离开行会？",MSG_CTRL_LEAVE_GUILD,0);
					}

					return true;
				}
			}	
			else if(iSubPage == 2 && IsSonCtrl(pControl))
			{
				//查找
				for (int i = 0;i<m_vHR.size();i++)
				{
					sHR& hrData = m_vHR[i];
					WORD wJob = hrData.wJob;
					if(hrData.pBtnAppoint == pControl || hrData.pBtnFavorite == pControl)//任命
					{
						m_sStrAppointName = "";
						if (hrData.wJob != 0xFF)
						{
							if (hrData.pBtnFavorite == pControl)
							{
								wJob = hrData.wJob + 1;
								m_sStrAppointName = hrData.strFavName;
							}
							else
							{
								wJob = hrData.wJob;
								m_sStrAppointName = hrData.strName;
							}
						}
						else
						{
							if(hrData.pBtnAppoint == pControl)
								m_sStrAppointName = hrData.strName;
							else m_sStrAppointName = hrData.strFavName;
						}

						if(m_sStrAppointName.compare("未任命") == 0)
							m_sStrAppointName = "";
						if (wJob == 0xFF && m_sStrAppointName.size() <= 0)
							wJob += 1;

						DWORD dwWndType = wJob << 16;
						dwWndType = dwWndType | 0x0003;
						g_pControl->PopupWindow(MSG_GUILDDELMEMBER_WND,OPER_CREATE,dwWndType);
						return true;
					}									
				}
			}
		}
		break;
	case MSG_CTRL_SCROLL_MOVE:
		{
			if (iSubPage == 2)
			{
				ReshHRScroll();
				return true;
			}
			break;
		}
	case MSG_CTRL_GUILD_CHANGE_INFO:
		{
			if ( iSubPage != 1)
			{
				if (dwData != 1) 
					SwitchToPage(iSubPage,&(m_TabPage.vSubTabPage[iCurPage]),true);
				else			 
					DealGuildMemberOnOrOffline((CGuildData::sTileNode*)pControl);//处理相关数据					
				
				return true;
			}
			break;
		}
	case MSG_INPUT_LEFTBT_DOWN:
		{
			if(iCurPage == 1)
			{
				m_iClickY = HIWORD( dwData );
				m_iClickX = LOWORD( dwData );
			}
			return false;
			break;
		}
	default:
		return false;
	}
	
	return false;
}

void CRelationWnd::DealGuildMemberOnOrOffline(CGuildData::sTileNode* pNode)
{
	if (pNode == NULL || pNode->vMember.size() <= 0) return;

	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	WORD wNum = 0;

	if (iCurPage == 1 && iSubPage != 1)
	{
		WORD wNum = pNode->wNum;
		CGuildData::_NewMember& mem = pNode->vMember[0];
		if (mem.bAssist) wNum--;

		if (iSubPage == 0)//行会成员列表
		{
			if (!m_pGuildTree) return;

			CTreeNode* pNode = m_pGuildTree->GetRootNode();
			if (!pNode) return;
			bool bOnline = false;
			if (mem.bAssist)
			{
				pNode = pNode->FindNode(mem.strAssist.c_str());
				bOnline = mem.bAssOnline;
			}
			else 
			{
				pNode = pNode->FindNode(mem.strName.c_str());
				bOnline = mem.bOnline;
			}

			if (pNode)
			{
				if (bOnline)
					pNode->SetColor(COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL);
				else pNode->SetColor(COLOR_TEXT_NORMAL,0xFF7F7F7F);
			}
		}
		else if (iSubPage == 2)//行会官职列表
		{
			if (pNode->wNum <= 10)
			{
				for (int i = 0;i<m_vHR.size();i++)
				{
					sHR& shr = m_vHR[i];
					if (shr.wJob == wNum)
					{
						if (!mem.bAssist && !shr.bStile && (shr.strName.compare(mem.strName.c_str()) == 0))
						{
							shr.bOnline = mem.bOnline;
							shr.dwColorName = shr.bOnline?COLOR_TEXT_NORMAL:0xFF7F7F7F;
							break;
						}
						else if (mem.bAssist && !shr.bStile && (shr.strFavName.compare(mem.strAssist.c_str()) == 0))
						{
							shr.bAssOnline = mem.bAssOnline;
							shr.dwColorFavName = shr.bAssOnline?COLOR_TEXT_NORMAL:0xFF7F7F7F;
							break;
						}					
					}
				}
			}
		}
	}
}


void CRelationWnd::ContributeRes(bool bView)
{
	DWORD dwID = 0;
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if (!bView)
	{
		bool bSend = false;
		CGoodArray& goodArray = SELF.GetDevGuildGood();
		if (goodArray.Size() < 3)
			return;

		if (goodArray.Get(0).GetID() + goodArray.Get(1).GetID() + goodArray.Get(2).GetID() != 0)
		{
			if(iSubPage != 10)
			{
				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@ViewMaintain");
				Sleep(1);
				g_pGameControl->SEND_Guild_Contribute();
				Sleep(1);
			}

			g_pGameControl->SEND_Guild_Contribute(goodArray.Get(0).GetID(),goodArray.Get(1).GetID(),goodArray.Get(2).GetID());
		}
		else
		{
			g_MsgBoxMgr.PopSimpleAlert("请先放置要捐献的物品！");
			return;
		}
	}
	else
	{
		if(iSubPage != 2)
		{
			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@ViewMaintain");
			Sleep(1);
		}

		g_pGameControl->SEND_Guild_Contribute();
	}

	if (iSubPage != 2)
	{
		SwitchToPage(2,&(m_TabPage.vSubTabPage[2].vSubTabPage[1]),true);
	}
}

bool CRelationWnd::DealGuildTowerMsg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}

	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if (iSubPage == 0)   //所有的返回结果信息窗口
		{
		}		
		else if (iSubPage == 1)//主灵塔
		{
			if (iGrandsonPage == 0)
			{
				if (pControl == m_pContributeBtn)//捐献资源
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@HandIn");
					return true;
				}
				else if (pControl == m_pUpdateCons)//升级建筑
				{
					UpdateConstruct();
					return true;
				}		
			}
			else if (iGrandsonPage == 1)   //捐献资源
			{
				if (pControl == m_pContributeBtn)//捐献资源
				{
					ContributeRes();
					return true;
				}
				else if (pControl == m_pViewRes)//查看资源
				{
					ContributeRes(true);
					return true;
				}		
			} 
			else if (iGrandsonPage == 2)   //所捐献的资源
			{
				if (pControl == m_pContributeBtn)//捐献资源
				{
					ContributeRes();			
					return true;
				}
				else if(pControl == m_pUpdateCons)//升级建筑
				{
					UpdateConstruct();
					return true;
				}
			}						   
			else if (iGrandsonPage == 3)   //升级建筑介绍
			{
				if (pControl == m_pUpdateCons)
				{
					UpdateConstruct();
					return true;
				}
				else if (pControl == m_pContributeBtn)
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@HandIn");
					return true;
				}
			}
		}
		else if (iSubPage == 2)   //珍宝阁介绍
		{
			if (iGrandsonPage == 0)
			{
				if (pControl == m_pEnterTrea)//进入珍宝阁
				{
					g_pGameControl->SEND_ZHENBAO_INFO(0);	
					return true;
				}
			}
			else if (iGrandsonPage == 1)   //珍宝阁
			{
			}						   
			else if (iSubPage == 2)   //极品阁
			{						   
			}		
		}					   
		else if (iSubPage == 4)   //议事厅
		{
			char ctemp[120] = {0};
			if (pControl == m_pAcceptTask)
			{//接取任务				

				if (m_wSelTask)
				{
					sprintf(ctemp,"@AcceptTask_%d",m_wSelTask);
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),ctemp);
					m_wSelTask = 0;
				}
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("请先选择你所要接取的任务！");
				}
				return true;
			}
			else if (pControl == m_pFinishTask)
			{
				if (m_wSelTask)
				{
					sprintf(ctemp,"@FinishTask_%d",m_wSelTask);
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),ctemp);
					m_wSelTask = 0;
				}
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("请先选择你所要完成的任务！");
				}
				return true;
			}
			else if (pControl == m_pOpenPrice)
			{
				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@OpenWelfare");///开启每日福利
				return true;
			}
			else if (pControl == m_pGetPrice)
			{
				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@GetWelfare");///领取每日福利
				return true;
			}
// 			else if (pControl == m_pUpTask)
// 			{
// 				m_pScrollTask->GoDown();
// 				return true;
// 			}
// 			else if (pControl == m_pDownTask)
// 			{
// 				m_pScrollTask->GoUp();
// 				return true;
// 			}
// 			else if (pControl == m_pUpDes)
// 			{
// 				m_pScrollDes->GoDown();
// 				return true;
// 			}
// 			else if (pControl == m_pDownDes)
// 			{
// 				m_pScrollDes->GoUp();
// 				return true;
// 			}
		}						 
		//else if (iSubPage == 5)   //神工坊简介
		//{
		//	if(pControl == m_pGetProperty)
		//	{
		//		g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@GetPropertyInfo");
		//		return true;
		//	}
		//	else if(pControl == m_pPropertyRefine)
		//	{
		//		g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@GetPropertyRefineInfo");
		//		return true;
		//	}
		//}						   
// 		else if (iSubPage == 6)   //获得盾牌
// 		{		
// 			if (pControl == m_pGetShield)
// 			{
// 				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@GetGuildEquip");
// 				return true;
// 			}
// 		}						   
// 		else if (iSubPage == 7 || iSubPage == 18)   //提升品质
// 		{	
// 			if(pControl == m_pRefineShield)
// 			{
// 				if(g_NPC.GetShenGongFangGoods().m_Shield.GetID() == 0)
// 				{
// 					g_MsgBoxMgr.PopSimpleAlert("请放入需要炼制的物品");
// 					return true;
// 				}
// 
// 				if(g_NPC.GetShenGongFangGoods().m_Wood.GetID() == 0)
// 				{
// 					g_MsgBoxMgr.PopSimpleAlert("请放入炼制需要的材料");
// 					return true;
// 				}
// 
// 				g_pGameControl->SEND_SHENGONGFANG_INFO(g_NPC.GetShenGongFangGoods().m_Shield.GetID(),g_NPC.GetShenGongFangGoods().m_Wood.GetID(),g_NPC.GetShenGongFangGoods().m_Luck.GetID());
// 				g_NPC.GetShenGongFangGoods().bSend = true;							
// 				m_bSendButtonClick = true;
// 				if(g_NPC.GetShenGongFangGoods().m_Shield.GetStdMode() != 13)		//魂珠炼制成功直接返回包裹
// 				{
// 					m_tmpShield = g_NPC.GetShenGongFangGoods().m_Shield;
// 				}
// 				return true;
// 			}
// 		}						   
// 		else if (iSubPage == 13)
// 		{//挑战仙灵大陆
// 			if (pControl == m_pSteleLand)
// 			{
// 				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@tiaozhanXLDL");
// 			}
// 		}
// 		else if (iSubPage == 14)
// 		{
// 			if (pControl == m_pSteleHero)
// 			{				
// 				g_pGameControl->SEND_General_Protocol(CS_STELE_HERO);//发送挑选精英战将
// 				SwitchToPage(15,&(m_TabPage.vSubTabPage[2]),true);
// 			}
// 		}
// 		else if (iSubPage == 15)
// 		{
// 			if (pControl == m_pSteleBani)
// 			{
// 				//发送选中的精英战将
// 				size_t iNum = m_vHeroList.size();
// 				if (iNum <= 0)
// 				{
// 					g_MsgBoxMgr.PopSimpleAlert("必须要有人参加！");
// 				}
// 				else if (iNum > g_GuildData.GetGuildSteleHeroNum())
// 				{
// 					g_MsgBoxMgr.PopSimpleAlert("你选择的人数超出参加人数的上限！");
// 				}
// 				else
// 				{
// 					g_MsgBoxMgr.PopOkCancelBox("你确定要动员这些精英战将，跟随你挑战天绝魔域吗？（一旦确定在本月无法更改）",MSG_CTRL_GUILD_MOBILIZE,0);
// 				}
// 			}
// 			else if (pControl == m_pAddHero)
// 			{//增加精英
// 
// 				if (m_vHeroList.size() >= g_GuildData.GetGuildSteleHeroNum())
// 				{
// 					g_MsgBoxMgr.PopSimpleAlert("你选择的人数超出参加人数的上限！");
// 				}
// 				else if (m_iSelectGuild >= 0 && m_iSelectGuild < m_vGuildMember.size())
// 				{
// 					m_vHeroList.push_back(m_vGuildMember[m_iSelectGuild]);
// 					m_vGuildMember.erase(m_vGuildMember.begin() + m_iSelectGuild);
// 
// 					if (m_iSelectGuild >= m_vGuildMember.size())
// 						m_iSelectGuild = m_vGuildMember.size() - 1;
// 
// 					if (m_iSelectGuild < 0)
// 						m_iSelectGuild = 0;
// 
// 					m_pScrollGuildStel->SetRange(m_vGuildMember.size());
// 					m_pScrollStelHero->SetRange(m_vHeroList.size());
// 				}
// 			}
// 			else if (pControl == m_pRemoveHero)
// 			{//移除精英
// 				if (m_iSelectHero >= 0 && m_iSelectHero < m_vHeroList.size())
// 				{
// 					if (strcmp(m_vHeroList[m_iSelectHero].c_str(),SELF.GetName()) == 0)
// 					{
// 						g_MsgBoxMgr.PopSimpleAlert("行会会长必须参加！");
// 						return true;
// 					}
// 					m_vGuildMember.push_back(m_vHeroList[m_iSelectHero]);
// 					m_vHeroList.erase(m_vHeroList.begin() + m_iSelectHero);
// 
// 					if (m_iSelectHero >= m_vHeroList.size())
// 						m_iSelectHero = m_vHeroList.size() - 1;
// 
// 					if (m_iSelectHero < 0)
// 						m_iSelectHero = 0;
// 
// 					m_pScrollGuildStel->SetRange(m_vGuildMember.size());
// 					m_pScrollStelHero->SetRange(m_vHeroList.size());
// 				}
// 			}
// 			else if (pControl == m_pStelHeroUpBtn)
// 			{
// 				m_pScrollStelHero->GoDown();
// 			}
// 			else if (pControl == m_pStelHeroDwnBtn)
// 			{
// 				m_pScrollStelHero->GoUp();
// 			}
// 			else if (pControl == m_pStelGuildUpBtn)
// 			{
// 				m_pScrollGuildStel->GoDown();
// 			}
// 			else if (pControl == m_pStelGuildDwnBtn)
// 			{
// 				m_pScrollGuildStel->GoUp();
// 			}
// 		}
// 		else if (iSubPage == 16)
// 		{
// 			if (pControl == m_pSteleAssess)
// 			{
// 				bool bCanAssess = true;
// 				std::vector<CGuildData::_GuildStele>& guildSteleVec = g_GuildData.GetGuildSteleMember();
// 				for (int i = 0;i<guildSteleVec.size();i++)
// 				{
// 					if (!guildSteleVec[i].bPart)
// 					{
// 						bCanAssess = false;
// 						break;
// 					}
// 				}
// 				if (!bCanAssess)
// 				{					
// 					g_MsgBoxMgr.PopOkCancelBox("您的行会精英战将尚未全部报到，您是否确认评定行会实力？（一旦评定，本月内无法修改）",MSG_CTRL_ACCESSFGHVALUE_ANNWAY,0);
// 				}
// 				else
// 				{//评定战斗值					
// 					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@assessChallenge");
// 				}
// 			}
// 			else if (pControl == m_pFghValueUpBtn)
// 			{
// 				m_pFghValueScroll->GoDown();
// 			}
// 			else if (pControl == m_pFghValueDwnBtn)
// 			{
// 				m_pFghValueScroll->GoUp();
// 			}
// 
// 
// 			for (int i = 0;i < 8;i++)
// 			{
// 				if (pControl == m_pBtnArray[i])
// 				{
// 					//g_pGameControl->SEND_Guild_Stele_Register();
// 					g_MsgBoxMgr.PopOkCancelBox("你确认已经穿戴最强装备，并放出元神了吗？（因为你的综合实力，将直接影响你们行会在天绝魔域的层级）",MSG_CTRL_JOIN_CONFIRM,0);////////////////////////////////////
// 					break;
// 				}
// 			}			
// 		}
// 		else if (iSubPage == 17)
// 		{
// 			if (pControl == m_pSteleMercenary)
// 			{
// 				//暂时不管
// 				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@LiaojieXianLing");
// 			}
// 		}
// 		else if (iSubPage == 19)
// 		{
// 			if (pControl == m_pEnterMagicAera)
// 			{
// 				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@liaojieXLDL");
// 			}
// 		}
// 		else if (iSubPage == 20)   //获得道具
// 		{
// 			if (pControl == m_pGetShield)
// 			{
// 				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@GuildEquipInfo");
// 				return true;
// 			}
// 		}						   
// 		else if (iSubPage == 21)   //道具炼制
// 		{		
// 			if(pControl == m_pUpdateShield)
// 			{
// 				SwitchToPage(7,&(m_TabPage.vSubTabPage[2]),true);
// 				return true;
// 			}
// 			else if(pControl == m_pShengWangUpdate)
// 			{
// 				SwitchToPage(18,&(m_TabPage.vSubTabPage[2]),true);
// 				return true;
// 			}
// 		}						   
	}
	else if (dwMsg == MSG_CTRL_GUILD_MOBILIZE_CONFIRM)
	{
		Send_Hero_List();
	}
	return false;
}

void CRelationWnd::UpdateConstruct()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}

	std::vector<CGuildData::sTileNode>& guildOff = g_GuildData.GetGuildMemberOfficer();

	if (guildOff.size() > 0 && guildOff[0].vMember.size() > 0)
	{
		CGuildData::_NewMember& newMem = guildOff[0].vMember[0];			

		if (newMem.strName.compare(SELF.GetName()) != 0)
		{
			g_MsgBoxMgr.PopSimpleAlert("你不是会长不能升级建筑");
			return;
		}

		if (iSubPage == 1 && iGrandsonPage == 3)
			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@StartTask");///升级行会建筑
		else
			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@OpenTask");///升级行会建筑
	}
}

bool CRelationWnd::OnLeftButtonUp(int iX, int iY)
{
	int iCurPage = m_TabPage.iCurPage;
	if(iCurPage == 1)
	{
		m_iClickX = m_iScreenX + iX;
		m_iClickY = m_iScreenY + iY;
	}

	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}

	//if (iX > 20 && iX < 42)
	//{
	//	if (iY > 60 && iY < 135)
	//	{
	//		if(m_iShowWnd == 0) return true;
// 	//		m_bTemp = true;

	//		if(iCurPage == 2 && iSubPage == 4 && !g_NPC.GetZhenBaoState().bInViewMode)
	//		{
	//			g_MsgBoxMgr.PopSimpleComfirm("你确定要离开极品阁吗？",MSG_CTRL_LEAVE_JIPINGE,104);
	//			return true;
	//		}			

	//      SwitchToPage(0,&(m_TabPage.vSubTabPage[0]),true);
	//		return true;
	//	}
	//	else if (iY > 155 && iY < 230)
	//	{
	//		//显示行会
	//		if(iCurPage == 1) 
	//			return true;

// 	//		m_bTemp = true;

	//		if(iCurPage == 2 && iSubPage == 4 && !g_NPC.GetZhenBaoState().bInViewMode)
	//		{
	//			g_MsgBoxMgr.PopSimpleComfirm("你确定要离开极品阁吗？",MSG_CTRL_LEAVE_JIPINGE,105);
	//			return true;
	//		}			

	//      SwitchToPage(0,&(m_TabPage.vSubTabPage[1]),true);
	//		return true;
	//	}
	//	else if (iY > 230 && iY < 325)
	//	{
	//		if (iCurPage == 2) return true;
// 	//		m_bTemp = true;
	//		g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@main");
	//		return true;
	//	}
	//}

	//if(iCurPage == 0)//江湖人脉
	//{
	//	return CCtrlWindowX::OnLeftButtonUp(iX,iY);

	//	if(iX >= PAGE_STARTX && iX <= PAGE_STARTX + PAGE_COUNT * PAGE_WIDTH && iY >= PAGE_STARTY && iY <= PAGE_STARTY + PAGE_HEIGHT)
	//	{
	//		int iPage = (iX - PAGE_STARTX) / PAGE_WIDTH;
	//		if(iPage < 0) iPage = 0;
	//		if(iPage >= PAGE_COUNT) iPage = PAGE_COUNT - 1;
	//		if(iPage == iSubPage)
	//		{
	//			return true;
	//		}
	//		iSubPage = iPage;
	//      SwitchToPage(iSubPage,&(m_TabPage.vSubTabPage[iCurPage]),true);
	//		return true;
	//	}
	//}
	//else if(iCurPage == 1)//行会信息
	//{
	//	return CCtrlWindowX::OnLeftButtonUp(iX,iY);

	//	bool bChange = false;
	//	if(iY > 83 && iY < 97)
	//	{
	//		if(iX > 56 && iX < 120)
	//		{
	//			if(iSubPage == 0) return true;
	//			iSubPage = 0;
	//			bChange = true;
	//		}
	//		else if(iX > 125 && iX < 188 )
	//		{
	//			if(iSubPage == 1) return true;
	//			iSubPage = 1;
	//			bChange = true;
	//		}
	//		else if (iX > 189 && iX < 256)
	//		{
	//			if (iSubPage == 2) return true;
	//			iSubPage = 2;
	//			bChange = true;
	//		}
	//		else if (iX >259 && iX < 324)
	//		{
	//			if (iSubPage == 3) return true;
	//			iSubPage = 3;
	//			bChange = true;
	//			if (strlen(SELF.GetTitle()) > 0 && g_PhyleData.GetPhyle().size() <= 0)
	//			{
	//				g_pGameControl->SEND_General_Protocol(CS_GUILD_PHYLE_LIST);
	//			}
	//		}
	//		else if(iX > 325 && iX < 389)
	//		{
	//			if (iSubPage == 4) return true;
	//			if(strlen(g_GuildData.GetGuildName()) != 0)
	//			{
	//				iSubPage = 4;
	//				bChange = true;
	//			}
	//			else
	//			{
	//				return true;
	//			}
	//		}
	//	}
	//	else if(iY > 250 && iY < 266 )
	//	{
	//		if(iX > 56 && iX < 120)
	//		{
	//			if(m_iPublicChat == 0) return true;
	//			m_iPublicChat = 0;
	//			bChange = true;
	//		}
	//		else if(iX > 125 && iX < 188 )
	//		{
	//			if(m_iPublicChat == 1) return true;
	//			m_iPublicChat = 1;
	//			bChange = true;
	//		}
	//	}

	//	if (bChange)
	//	{
// 	//		m_bTemp = false;
    //      SwitchToPage(iSubPage,&(m_TabPage.vSubTabPage[iCurPage]),true);
	//	}
	//	return true;
	//}
	//else if (iCurPage == 2)
	//{
	//	m_bClick = false;

	//	//切换页面
	//	if (iX >= 227 && iX <= 573 && iY >= 89 && iY <= 112)
	//	{
	//		int iPage = (iX - 227)/69;
	//		if (iPage >= 5)
	//			iPage = 4;

	//		if(iSubPage == 4 && !g_NPC.GetZhenBaoState().bInViewMode)
	//		{
	//			g_MsgBoxMgr.PopSimpleComfirm("你确定要离开极品阁吗？",MSG_CTRL_LEAVE_JIPINGE,100 + iPage);
	//		}
	//		else if (iPage == 0)
	//		{
	//			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@main");
	//		}
	//		else if (iPage == 1)
	//		{
	//			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@OpenZBGrid");
	//		}
	//		else if (iPage == 2)
	//		{
	//			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@shengongfang");
	//		}
	//		else if (iPage == 3)
	//		{
	//			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@RecTask");
	//		}
	//		else if (iPage == 4)
	//		{
	//			//仙灵碑
	//			g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@XianLingBei");
	//		}
	//	}

		if (m_pMarkViewer)
		{
			string strCommand = m_pMarkViewer->GetCommand();
			if(!strCommand.empty())
			{
				//if(strCommand.substr(2,7) == "LoginXL")
				//{
				//	g_Login.SetKfzLoginString(strCommand.substr(10,strCommand.length() - 10));
				//	g_MsgBoxMgr.PopOkCancelBox("现在可以进入天绝魔域,进入天绝魔域后将使用您在报到时所穿的装备,离开天绝魔域后将还原回您进入天绝魔域前的装备状态,是否进入?",MSG_CTRL_LOGIN_XL_SERVER,0);
				//}
				//else
				if(strCommand == "@@FoundTower")
				{
					g_pGameControl->SEND_SHENGONGFANG_INFO(0,0,0,3);
				}
				else if(strCommand == "@@RefineFlyStone")
				{
					g_pGameControl->SEND_SHENGONGFANG_INFO(0,0,0,4);
				}
				else
				{
					g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),strCommand.c_str());
				}
				return true;
			}
		}

		if(iCurPage == 2)//行会塔
		{
			if (iSubPage == 1 && (iGrandsonPage == 1 || iGrandsonPage == 2))//捐献资源,所捐献的资源
			{
				if (iX >= 230 && iX <= 330 + 32 && iY >= 357 && iY <= 357 + 32)
				{
					//在范围中
					CGood& good = CGoodGrid::GetDropGoodFrom().DropGood;
					if(CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd)
					{
						return CCtrlWindowX::OnLeftButtonUp(iX,iY);
					}

					int iSel = 0;
					if (iX >= 230 && iX <= 230 + 32)
						iSel = 1;
					else if (iX >= 280 && iX <= 280 + 32)
						iSel = 2;
					else if (iX >= 330 && iX <= 330 + 32)
						iSel = 3;

					if (iSel == 0)
						return CCtrlWindowX::OnLeftButtonUp(iX,iY);

					CGoodArray& goodArray = SELF.GetDevGuildGood();
					CGood& goodTemp = goodArray.Get(iSel - 1);
					if (good.GetID() != 0)
					{//鼠标上有物品
						if (good.GetID() != 0)
						{
							stGoodFrom stgood;
							stgood.DropGood = goodTemp;
							stgood.eFromWnd = Package_Wnd;
							stgood.iWndPos = -1;
							goodTemp = good;
							CGoodGrid::SetDropGoodFrom(stgood);					
						}
						else
						{
							goodTemp = good;
							good.SetID(0);
						}
					}
					else
					{//鼠标上没有物品
						if (goodTemp.GetID() != 0)
						{
							stGoodFrom stgood;
							stgood.DropGood = goodTemp;
							stgood.eFromWnd = Package_Wnd;
							stgood.iWndPos = -1;
							goodTemp = good;
							CGoodGrid::SetDropGoodFrom(stgood);
							goodTemp.SetID(0);
						}
					}			
				}
			}
			else if(iSubPage == 2 && (iGrandsonPage == 1 || iGrandsonPage == 2))//珍宝阁,极品阁
			{
				int iGrid = GetGrid(iX,iY);
				_ZhenBaoState& state = g_NPC.GetZhenBaoState();
				if(iGrid != -1)	
				{
					if(iGrandsonPage == 1)
					{
						if(state.grid[iGrid].iState != 2 && state.grid[iGrid].iState != 100)	//选中格子为被开启
						{
							if(state.grid[iGrid].iState == 1)
							{
								g_MsgBoxMgr.PopSimpleComfirm("你发现了一个极品阁，阁内珍藏着极品装备和图腾。你要花费一个灵皓石进入极品阁吗？进入极品阁后无需花费任何行会银两就可获得阁内珍宝。",MSG_CTRL_ENTER_JIPINGE,iGrid);
							}
							else
							{			
								state.grid[iGrid].iState = 100;
								state.grid[iGrid].dwOpenTime = GetTickCount();
							}
						}
						else if(state.grid[iGrid].iType == 4)		//点击开启的极品阁的门
						{
							for(int i = 0;i < 77;i++)			//去极品阁观赏只有在所有格子都被开启的情况下
							{
								if(state.grid[i].iState != 2)
									return true;
							}

							g_MsgBoxMgr.PopSimpleComfirm("该极品阁已经被人开启！你是否有兴趣进去看看？",MSG_CTRL_ENTER_JIPINGE,iGrid + 10000);
						}
					}
					else if(iGrandsonPage == 2 && state.jipingrid[iGrid].iState != 2 && state.jipingrid[iGrid].iState != 100) //选中格子为被开启
					{		
						state.jipingrid[iGrid].iState = 100;
						state.jipingrid[iGrid].dwOpenTime = GetTickCount();
					}

					return true;
				}
			}
			else if (iSubPage == 4)//议事厅
			{
				if (iX >= 218 && iX<= 575 && iY >= 112 && iY <= 210)
				{
					int iSelRow = m_pScrollTask->GetPos() + (iY - 112)/20;
					//[2009-9-17]
					if (iSelRow < g_TaskData.GetGuildTaskList().size())
					{
						WORD wID = g_TaskData.GetGuildTaskList()[iSelRow]->wTaskID;
						if (wID != m_wSelTask)
						{
							m_wSelTask = wID;
							m_pMarkViewer->SetTagText(&(g_TaskData.GetGuildTaskList()[iSelRow]->strDesc));
							//m_pScrollDes->SetRange(g_TaskData.GetGuildTaskList()[iSelRow]->strDesc.GetRow() - 1);
						}
					}
					else
					{
						m_wSelTask = 0;
					}
				}		
			}
		}



	//	else if(iSubPage == 7 || iSubPage == 18)
	//	{	
	//		if(!IsStop() || m_iFireFrame < 102)
	//		{
	//			return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	//		}

	//		CGood * pGood = NULL;

	//		CGood& goodInMouse = CGoodGrid::GetDropGoodFrom().DropGood;	
	//		int iStdMode = goodInMouse.GetStdMode();
	//		int iShape = goodInMouse.GetShape();

	//		if(iX > 449 && iX < (449 + 40) && iY > 185 && iY < (185 + 35))		//炼制物品
	//		{
	//			if(goodInMouse.GetID() != 0)
	//			{
	//				if(iSubPage == 7 && (iStdMode == 13 || iStdMode == 12))
	//				{

	//				}
	//				else if(iSubPage == 18 && ((iStdMode >= 18 && iStdMode <= 26) || iStdMode == 15 || iStdMode == 58 || iStdMode == 81))
	//				{

	//				}
	//				else
	//				{
	//					return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	//				}
	//			}
	//			pGood = &(g_NPC.GetShenGongFangGoods().m_Shield);
	//		}
	//		else if(iX > 421 && iX < (421 + 40) && iY > 259 && iY < (259 + 35))		//材料
	//		{
	//			if(goodInMouse.GetID() != 0)
	//			{
	//				if(iSubPage == 7 && iStdMode == 48 && iShape == 0)
	//				{

	//				}
	//				else if(iSubPage == 18 && (iStdMode == 43 && iShape == 11))
	//				{

	//				}
	//				else
	//				{
	//					return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	//				}
	//			}

	//			pGood = &(g_NPC.GetShenGongFangGoods().m_Wood);


	//		}
	//		else if(iX > 483 && iX < (483 + 40) && iY > 259 && iY < (259 + 35))		//幸运符
	//		{
	//			if(goodInMouse.GetID() != 0)
	//			{
	//				if(iStdMode == 44 && (iShape == 3 || iShape == 4 || iShape == 5))
	//				{

	//				}
	//				else
	//				{
	//					return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	//				}
	//			}

	//			pGood = &(g_NPC.GetShenGongFangGoods().m_Luck);


	//		}

	//		if(pGood != NULL)
	//		{
	//			CGood& goodInMouse = CGoodGrid::GetDropGoodFrom().DropGood;
	//			if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() != 0 && CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd)
	//			{
	//				return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	//			}



	//			if (goodInMouse.GetID() != 0)
	//			{
	//				m_bSendButtonClick = false;

	//				//鼠标上有物品
	//				if (pGood->GetID() != 0)
	//				{
	//					stGoodFrom stgood;
	//					stgood.DropGood = *pGood;
	//					stgood.eFromWnd = Package_Wnd;
	//					stgood.iWndPos = -1;
	//					*pGood = goodInMouse;
	//					CGoodGrid::SetDropGoodFrom(stgood);					
	//				}
	//				else
	//				{
	//					*pGood = goodInMouse;
	//					goodInMouse.SetID(0);
	//				}
	//			}
	//			else
	//			{
	//				///////////////////////////////////////鼠标上没有物品
	//				if (pGood->GetID() != 0)
	//				{
	//					stGoodFrom stgood;
	//					stgood.DropGood = *pGood;
	//					stgood.eFromWnd = Package_Wnd;
	//					stgood.iWndPos = -1;
	//					CGoodGrid::SetDropGoodFrom(stgood);
	//					pGood->SetID(0);
	//				}
	//			}
	//		}
	//	}
	//	else if (iSubPage == 15)
	//	{//选中行设置，仙灵碑

	//	}
	//}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CRelationWnd::OnLeftButtonDown(int iX,int iY)
{
	int iCurPage = m_TabPage.iCurPage;
	//if (iCurPage == 2)
	//{
	//	if (iSubPage == 8)
	//	{
	//		m_bClickDes = false;
	//		if (iX >= 227 && iX <= 552 && iY >= 232 && iY <= 313)
	//		{
	//			m_bClickDes = true;
	//		}
	//	}	
	//	else if (iSubPage == 15)
	//	{
	//		//进行选择
	//		if (iX >= 227 && iX<=339 && iY >= 147 && iY <= 312)
	//		{
	//			m_iSelectGuild = m_pScrollGuildStel->GetPos() + (iY - 147)/24;//行会选中
	//		}
	//		else if (iX >= 445 && iX <= 553 && iY >= 147 && iY <= 312)
	//		{
	//			m_iSelectHero = m_pScrollStelHero->GetPos() + (iY - 147)/24;//英雄选中
	//		}
	//	}
	//}
	//else if (iCurPage == 1)
	//{
	//	if (iSubPage == 2)
	//	{
	//		m_bClickDes = false;
	//		if (iX >= 58 && iX <= 551 && iY >= 308 && iY <= 380)
	//		{
	//			m_bClickDes = true;
	//		}
	//	}
	//}	

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CRelationWnd::OnLeftButtonDClick(int iX,int iY)
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(iCurPage == 0)
	{
		int iCurSelRow = iY;
		if(iCurSelRow >= 0 && iCurSelRow < m_vRelation.size())
		{
			string strTemp = m_vRelation[iCurSelRow].sName;
			strTemp += " ";

			g_pControl->Msg(MSG_CTRL_INSERT_TEXT, 1,(CControl*)strTemp.c_str());
			return true;
		}
	}
 	else if(iCurPage == 2)
 	{
 		if(iSubPage == 1 && g_GuildData.GetGuildTowerLevel() > 0 && g_GuildData.IsHeader(false))
 		{
 			if(iX > 500 && iY > 320 && iX < 500 + 71 && iY < 320 + 83)
 			{
 				g_pControl->PopupWindow(MSG_CTRL_GUILD_FLAG_SEL_WND,OPER_CREATE);
 				return true;
 			}
 		}
 	}

	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

void CRelationWnd::Draw()
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iCurPage = m_TabPage.iCurPage;
	if(iCurPage == 0)
	{
		DrawRelationWnd();
	}
	else if(iCurPage == 1)
	{
		DrawGuildWnd();
	}
	else if(iCurPage == 2)
	{
		DrawGuildTower();
	}
}

void   CRelationWnd::DrawRelationWnd()
{
	DWORD iType = 0;
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	switch(iSubPage)
	{
	case SUBPAGE_INDEX_FRIEND:
		iType |= RT_FRIEND;
		break;
	case SUBPAGE_INDEX_HUSBAND_WIFE:
		iType |= RT_HUSBAND | RT_WIFE;
		break;
	case SUBPAGE_INDEX_MASTER_PRENTICE:
		iType |= RT_MASTER | RT_PRENTICE;
		break;
	case SUBPAGE_INDEX_PHYLE:
		iType |= RT_PHYLE;
	case SUBPAGE_INDEX_JIEYI:
		iType |= RT_JIEYI;
	default:
		break;
	}

	g_pFont->DrawText(m_iScreenX + 288,m_iScreenY + 9,"江湖人脉",COLOR_TEXT_NORMAL,FONT_YAHEI,16);

	int iDevY = (iSubPage != SUBPAGE_INDEX_PHYLE)?63:90;
	if(iSubPage == SUBPAGE_INDEX_JIEYI)
	{
		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + iDevY,"名字",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 205,m_iScreenY + iDevY,"职业",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 300,m_iScreenY + iDevY,"等级",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 370,m_iScreenY + iDevY,"当日活跃度",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 460,m_iScreenY + iDevY,"昨日活跃度",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);

		//g_pFont->DrawText(m_iScreenX + 105,m_iScreenY + 225,"达到30级以后就能与好友进行结义（等级越高，可结义好友越多）。",0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT);
		//g_pFont->DrawText(m_iScreenX + 105,m_iScreenY + 245,"结义关系中的好友每点亮一颗珍宝珠就能增加1点当日活跃度。",0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT);
		//g_pFont->DrawText(m_iScreenX + 105,m_iScreenY + 265,"每当结义关系中增加1个当日活跃度≥4的结义好友，结义关系中所有人次日通过任",0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT);
		//g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 285,"意途径获得经验值时都会额外增加：1. 如果结义好友加入结义时等级＜20级，最多+5%；",0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT);
		//g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 305,"2. 如果结义好友加入结义时等级≥20级，最多+3%。",0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT);
		
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 92,m_iScreenY + iDevY,"名字",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 223,m_iScreenY + iDevY,"职业",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 320,m_iScreenY + iDevY,"等级",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 450,m_iScreenY + iDevY,"行会",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
	}
	
	//绘制数据
	int iCurShowRow = m_pGrid->GetScrollPos();
	int iNumTotal = m_vRelation.size();
	int iDrawTotalNum = (iSubPage!= SUBPAGE_INDEX_PHYLE)?14:13;
	if(iSubPage == SUBPAGE_INDEX_JIEYI)	iDrawTotalNum = 6;

	int iLive4 = (m_dwYesterLive >= 4) ? 1 : 0;

	int iX = m_iScreenX;
	int iY = m_iScreenY;
	char cTemp[50]={0};
	DWORD dwColor = COLOR_TEXT_NORMAL;
	for( unsigned int i = 0;i < iDrawTotalNum && i + iCurShowRow < iNumTotal; i++)
	{
		_SRelation& srl = m_vRelation[i+iCurShowRow];		

		if(srl.iOnline != 1)	
			 dwColor = COLOR_TEXT_DISABLE;
		else dwColor = COLOR_TEXT_NORMAL;

		bool bCenter = iSubPage == SUBPAGE_INDEX_PHYLE ? false:true;
		int iDevX = bCenter?0:25,iDevY = 0;

		if (iSubPage == SUBPAGE_INDEX_JIEYI)
		{
			m_pGrid->DrawGrid(i,0,srl.sName.c_str(),bCenter,dwColor,0,iDevX,iDevY);

			if(srl.iLevel > 0)
			{
				if(srl.iJob == 0)		memcpy(cTemp,"战士",4);
				else if(srl.iJob == 1)	memcpy(cTemp,"法师",4);
				else if(srl.iJob == 2)	memcpy(cTemp,"道士",4);

				m_pGrid->DrawGrid(i,1,cTemp,true,dwColor);
				memset(cTemp,0,50);
				m_pGrid->DrawGrid(i,2,itoa(srl.iLevel,cTemp,10),true,dwColor);				
				m_pGrid->DrawGrid(i,3,itoa(srl.iCurLive,cTemp,10),true,dwColor);
				m_pGrid->DrawGrid(i,4,itoa(srl.iYesterLive,cTemp,10),true,dwColor);
			}

			if (srl.iYesterLive >= 4)
			{
				iLive4++;
			}
		}
		else
		{
			int col = 0;
			m_pGrid->DrawGrid(i,col,srl.sName.c_str(),bCenter,dwColor,0,iDevX,iDevY);
			++col;
			//if (iSubPage == SUBPAGE_INDEX_FRIEND)
			//{
			//	m_pGrid->DrawGrid(i,col,itoa(srl.iMuoQi,cTemp,10),true,dwColor);
			//	memset(cTemp,0,50);
			//	++col;
			//}

			if(srl.iLevel > 0)
			{
				if(srl.iJob == 0)		memcpy(cTemp,"战士",4);
				else if(srl.iJob == 1)	memcpy(cTemp,"法师",4);
				else if(srl.iJob == 2)	memcpy(cTemp,"道士",4);	

				m_pGrid->DrawGrid(i,col,cTemp,true,dwColor);
				memset(cTemp,0,50);
				++col;
				m_pGrid->DrawGrid(i,col,itoa(srl.iLevel,cTemp,10),true,dwColor);
				++col;
				m_pGrid->DrawGrid(i,col,srl.sGuild.c_str(),true,dwColor);
			}
		}
		
		

		//if (iSubPage == SUBPAGE_INDEX_PHYLE)
		//{//神魔图片
		//	WORD wID = (srl.iPhyle == 1 || srl.iPhyle == 2) ? srl.iPhyle + 16820 - 1: 0;
		//	LPTexture pTex = (wID != 0) ? g_pTexMgr->GetTex(PACKAGE_INTERFACE,wID):NULL;
		//	if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 65, m_iScreenY + 123 + i * 20,pTex);
		//}		
	}

	if (iSubPage == SUBPAGE_INDEX_JIEYI && m_bJieYi)
	{
		char strtemp[64] = {0};
		sprintf(strtemp, "昨日活跃度≥4的人数：%d人", iLive4);
		g_pFont->DrawText(m_iScreenX + 85,m_iScreenY + 345,strtemp,0xFFFABF8F,FONT_DEFAULT,FONTSIZE_DEFAULT);
		sprintf(strtemp, "每人获得经验值加成比例：%d%%", g_OtherData.GetJieYiExp());
		g_pFont->DrawText(m_iScreenX + 305,m_iScreenY + 345,strtemp,0xFFFABF8F,FONT_DEFAULT,FONTSIZE_DEFAULT);
		
		sprintf(strtemp, "您的当日活跃度：%u", m_dwCurLive);
		g_pFont->DrawText(m_iScreenX + 85,m_iScreenY + 325,strtemp,0xFFFABF8F,FONT_DEFAULT,FONTSIZE_DEFAULT);
		sprintf(strtemp, "昨日活跃度：%u", m_dwYesterLive);
		g_pFont->DrawText(m_iScreenX + 305,m_iScreenY + 325,strtemp,0xFFFABF8F,FONT_DEFAULT,FONTSIZE_DEFAULT);
	}

	if (iSubPage == SUBPAGE_INDEX_PHYLE && iNumTotal > 0)
	{//宗派
		char ctemp[10] = {0};
		itoa(iNumTotal,ctemp,10);
		//BYTE byPhyleType = g_OtherData.GetPhyleType();

		//WORD wID = (byPhyleType == 1 || byPhyleType == 2) ? byPhyleType + 16820 - 1: 0;
		//LPTexture pTex = (wID != 0) ? g_pTexMgr->GetTex(PACKAGE_INTERFACE,wID):NULL;
		//if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 114, m_iScreenY + 87,pTex);

		char szTemp[128] = {0};
		sprintf(szTemp,"宗族：%s",g_OtherData.GetPhyleName().c_str());
		g_pFont->DrawText(60 + m_iScreenX,66 + m_iScreenY,szTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,14);

		sprintf(szTemp,"宗主：%s",g_OtherData.GetPhyleMaster().c_str());
		g_pFont->DrawText(275 + m_iScreenX,66 + m_iScreenY,szTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,14);

		sprintf(szTemp,"宗族成员：%s",ctemp);
		g_pFont->DrawText(480 + m_iScreenX,66 + m_iScreenY,szTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,14);
	}
}

void   CRelationWnd::DrawGuildWnd()
{
	string str = "行会风云";
	if (strlen(g_GuildData.GetGuildName()) > 0)
	{
		str = "行会风云- ";
		str += g_GuildData.GetGuildName();
	}

	int iStartPos = (202 - (str.length() * 8))/2;
	g_pFont->DrawText(m_iScreenX + 219 + iStartPos,m_iScreenY + 9,str.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,16);

	if (g_GuildData.GetGuildTowerLevel() <= 0 && (g_GuildData.GetMember().size() <= 0 && g_GuildData.GetGuildMemberOfficer().size() <= 0))
	{
		return;
	}
	else if (g_GuildData.GetGuildTowerLevel() > 0 && g_GuildData.GetGuildMemberOfficer().size() <= 0)
	{
		return;
	}

	//绘制公共信息
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;


	char pTemp[10]={0};
	int iTotalNum = 0,iScrollRange = 0,iLines = 4;

	if (!g_GuildData.IsNewGuildData())
	{
		for(int m = 0;m<g_GuildData.GetMember().size();m++)
		{
			iScrollRange += (1 + (g_GuildData.GetMember()[m].name.size()+4)/iLines);
			iTotalNum += g_GuildData.GetMember()[m].name.size();
		}
	}
	else
	{
		for (int i = 0;i<g_GuildData.GetGuildMemberOfficer().size();i++)
		{
			for (int j = 0;j< g_GuildData.GetGuildMemberOfficer()[i].vMember.size();j++)
			{
				CGuildData::_NewMember& newMem = g_GuildData.GetGuildMemberOfficer()[i].vMember[j];
				if (!newMem.bAssist)
				{
					if ( newMem.strName.length() > 0)
					{
						iTotalNum++;
					}					
				}
				else
				{
					if (newMem.strAssist.length() > 0)
						iTotalNum += 2;
					else iTotalNum++;
				}
			}
		}
		for (int i = 0;i<g_GuildData.GetGuildMemberNormal().size();i++)
		{
			iTotalNum += g_GuildData.GetGuildMemberNormal()[i].vMember.size();
		}
	}

	std::string strName;
	if (!g_GuildData.IsNewGuildData())
	{
		if(g_GuildData.GetMember().size() >0 && g_GuildData.GetMember()[0].name.size() > 0)
		{
			strName= g_GuildData.GetMember()[0].name[0].c_str();
		}
	}
	else
	{
		//采用新的行会数据
		if (g_GuildData.GetGuildMemberOfficer().size() > 0 && g_GuildData.GetGuildMemberOfficer()[0].vMember.size() > 0)
		{
			CGuildData::_NewMember& newMem = g_GuildData.GetGuildMemberOfficer()[0].vMember[0];			
			strName = newMem.strName;
		}
	}

	char ctemp[256] = {0};
	sprintf(ctemp,"行会成员：%d          会长：%s          行会等级：%s         行会经验值：%s",iTotalNum,strName.c_str(),g_GuildData.GetGuildLevel(),g_GuildData.GetGuildExp());
	g_pFont->DrawText(m_iScreenX + 58,m_iScreenY + 36,ctemp,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);


	if (iSubPage ==0 || iSubPage == 1)//绘制会员和外交数据
	{
		//if(m_iPublicChat == 1 && m_pTextureChat != NULL)
		//	g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,m_pTextureChat);
		//绘制行会信息
		if(iSubPage == 0 && m_pGuildTree)
		{
			m_pGuildTree->Draw(m_iClickX,m_iClickY);
		}
		else if(iSubPage == 1)
		{
			//
			g_pFont->DrawText(m_iScreenX + 149,m_iScreenY + 86,"联盟行会",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 432,m_iScreenY + 86,"敌对行会",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14,DTF_BlackFrame);

			m_pGrid1->SetTotalCount(g_GuildData.GetAlly().size());
			m_pGrid2->SetTotalCount(g_GuildData.GetKillAlly().size()); 
			int iRight1Pos = m_pGrid1->GetScrollPos();
			int iRight2Pos = m_pGrid2->GetScrollPos();
			//m_pGrid1->SetFont(FONT_YAHEI);
			//m_pGrid2->SetFont(FONT_YAHEI);

			int iNumTotal = g_GuildData.GetAlly().size();
			for( unsigned int i = 0;i < 7 && i + iRight1Pos < iNumTotal; i ++)
			{
				m_pGrid1->DrawGrid(i,0,g_GuildData.GetAlly()[i+iRight1Pos].c_str(),false,COLOR_TEXT_NORMAL);
			}

			iNumTotal = g_GuildData.GetKillAlly().size();
			for( unsigned int i = 0; i < 7 && i + iRight2Pos < iNumTotal ; i ++)
			{
				m_pGrid2->DrawGrid(i,0,g_GuildData.GetKillAlly()[i+iRight2Pos].c_str(),false,COLOR_TEXT_NORMAL);
			}
		}

		int iNum = 0;
		if(m_iPublicChat == 0)//公告
		{
			int iLeftPos = m_pGuildScroll01->GetPos() + 2;
			m_pGuildScroll01->SetRange(g_GuildData.GetWeb().size());

			if(g_GuildData.GetWeb().size() > 0)
			{
				int iLeftY = 255;
				for(unsigned int i = iLeftPos; i < iLeftPos + 20; i++)
				{
					if(i >=  g_GuildData.GetWeb().size()) break;

					string& str = g_GuildData.GetWeb().at(i);

					g_pFont->DrawText(GetX() + 50,GetY() + (i - iLeftPos)*14 + iLeftY,str.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI);
					iNum++;
					if(iNum >= 10) break;
				}
			}
		}
		else if(m_iPublicChat == 1)//聊天
		{
			int iLeftPos2 = m_pGuildScroll01->GetPos();
			int iLeftY = 255;
			int iTalkSize = g_TalkMgr.GetGuildTalk().size();
			m_pGuildScroll01->SetRange( iTalkSize - 1);
			if(iTalkSize > 0)
			{
				for(unsigned int i = iLeftPos2; i < iLeftPos2 + 20 && i < iTalkSize; i++)
				{
					WORD color = 0;
					int iMultiLine = -1;

					CStringLine* pLine = g_TalkMgr.GetGuildTalk()[i];
					if(pLine == NULL) 
						continue;
					const char* pChar = pLine->getBuf();

					std::vector<std::string>   vDrawStr;
					CutByUnicode(pChar,vDrawStr,52);
					for(size_t i = 0; i < vDrawStr.size(); i++)
					{
						g_pFont->DrawText(GetX() + 50,GetY() + iNum*14 + iLeftY,vDrawStr[i].c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI);
						iNum++;
						if(iNum >= 10) break;
					}

					if(iNum >= 10) 
						break;
				}
			}
		}
	}
	else if (iSubPage == 2 && g_GuildData.GetGuildTowerLevel() > 0)
	{	//绘制人事数据
		int iScrollPage = 9;
		int iTotalNum = m_vHR.size();
		int iStart = 0;
		if (m_pScrollHR)
			iStart = m_pScrollHR->GetPos();//绘制的起始位置
		else return;
		iStart = (iStart/3)*3;
		int iEnd = iStart + iScrollPage - 1;
		if (iEnd > (int)(m_vHR.size() -1))
		{
			iEnd = m_vHR.size() - 1;
		}
		int iDevY = 201/iScrollPage;
		bool bMouseInJob = false;
		for (int i = iStart;i <= iEnd;i++)
		{
			sHR& sEle = m_vHR[i];
			int ix = GetScreenX() + 56;
			int iy = GetScreenY()+ 96 + (i - iStart) * iDevY;
			if (sEle.bStile)//标题
			{				
				DWORD dwColorTemp = COLOR_TEXT_SBU_TITLE;
				if (m_iMX >= 56 && m_iMX <= 56 + 12 * 3 && m_iMY >= iy - GetScreenY() && m_iMY <= iy - GetScreenY() + 12)
				{
					int iTemp = i/3;
					bMouseInJob = true;
					if (m_iMouseInJob != iTemp && iTemp >= 1 && iTemp <= 5)
					{						
						m_iMouseInJob = iTemp;
						m_TagText.Clear();
						m_TagText.setText(m_vStrJobDes[m_iMouseInJob]);
						m_TagText.Parse(m_vStrJobDes[m_iMouseInJob],0,"\\");
					}
					dwColorTemp = 0xFF0088FF;
				}

				g_pFont->DrawText(ix,iy,sEle.strName.c_str(),dwColorTemp,FONT_YAHEI);
			}
			else//不是标题,绘制封号人的姓名，及亲信名
			{
				iy += 4;
				bool bStile = false;
				if (sEle.strName.length() > 0)//封号姓名
				{
					//有封号姓名
					bStile = true;
					ix += 15;					
					g_pFont->DrawText(ix,iy,sEle.strName.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI);
				}

				if (sEle.strFavName.length() > 0)//亲信名
				{
					//有亲信名
					ix += 238;
					g_pFont->DrawText(ix,iy,"亲信：",0xffff8000);
					g_pFont->DrawText(ix + 35,iy,sEle.strFavName.c_str(),sEle.dwColorFavName,FONT_YAHEI);
				}
			}
		}

		if (!bMouseInJob &&  m_iMouseInJob != 0)
		{
			m_iMouseInJob = 0;
			m_TagText.Clear();
			m_TagText.setText(m_vStrJobDes[m_iMouseInJob]);
			m_TagText.Parse(m_vStrJobDes[0],0,"\\");
		}
// 		//行会公共银两
// 		char ctemp[100]={0};
// 		sprintf(ctemp,"目前行会公共银两：%d",g_GuildData.GetGuildTower().dwTaelPub);
// 
// 		g_pFont->DrawText(m_iScreenX + 157,m_iScreenY + 393,ctemp,COLOR_TEXT_NORMAL,FONT_YAHEI);
	}
	else if (iSubPage == 3)
	{
		if (m_pGuildTree)
			m_pGuildTree->Draw(m_iClickX,m_iClickY);
	}
	else if (iSubPage == 4)
	{
		if(g_WidgetMgr.IsShow(EWT_GREAT_EVENT))
		{
			g_WidgetMgr.RenderWidget(EWT_GREAT_EVENT);
		}
	}

}

void CRelationWnd::DrawGuildTower()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}


	string str = "行会风云";
	if (strlen(g_GuildData.GetGuildName()) > 0)
	{
		str = "行会风云- ";
		str += g_GuildData.GetGuildName();
	}

	int iStartPos = (202 - (str.length() * 8))/2;
	g_pFont->DrawText(m_iScreenX + 219 + iStartPos,m_iScreenY + 9,str.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,16);

	
	//绘制行会塔
	LPTexture pFlagTex = NULL;
	int iFlag = g_GuildData.GetGuildFlag();	

	if (iSubPage != 3)//神工坊不要
	{
		if(iFlag > 0 && iFlag < 5)
		{
			pFlagTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16149 + iFlag,EP_UI);
		}
		else
		{
			pFlagTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16149 + 1,EP_UI);
		}
		g_pGfx->DrawTextureNL(m_iScreenX + 513,m_iScreenY + 312,pFlagTex);
	}


	char szTemp[256] = "";
	sprintf(szTemp,"层级:%d        行会名字:%s",g_GuildData.GetGuildTowerLevel(),g_GuildData.GetGuildName());
	g_pFont->DrawText(m_iScreenX + 53,m_iScreenY + 38,szTemp,-1);

	if (g_GuildData.GetGuildMemberOfficer().size() > 0 && g_GuildData.GetGuildMemberOfficer()[0].vMember.size() > 0)
	{			
		CGuildData::_NewMember& newMem = g_GuildData.GetGuildMemberOfficer()[0].vMember[0];			
		sprintf(szTemp,"会长名字:%s",newMem.strName.c_str());
		g_pFont->DrawText(m_iScreenX + 450,m_iScreenY + 38,szTemp,-1);	//会长名字
	}


	if(!(iSubPage == 2 && iGrandsonPage == 1))//珍宝阁
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16615,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 48,m_iScreenY + 90,pTex);

		int iTowerLevel = g_GuildData.GetGuildTowerLevel();		//通灵塔层数
		if(iTowerLevel > 0 && iTowerLevel <= 100)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16144 + (iTowerLevel + 19) / 20,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 20,m_iScreenY + 145,pTex);
			if (iTowerLevel == 100)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16646,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX - 26,m_iScreenY + 115,pTex);
				g_pGfx->SetRenderMode();
			}

// 			if(g_GuildData.GetGuildPhyleState() > 0)
// 			{
// 				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,1 + g_GuildData.GetGuildPhyleState());
// 				g_pGfx->DrawTextureNL(m_iScreenX + 60,m_iScreenY + 40,pTex);
// 			}
		}

		if(m_iTowerLevelUpFrame < 260)
		{
			LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16401,EP_UI);	
			if(pEffectTex)
			{
				pEffectTex->EnableSysAnim(FALSE);
				pEffectTex->SetCurFrame(m_iTowerLevelUpFrame / 10);
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pEffectTex);
				g_pGfx->SetRenderMode();
				m_iTowerLevelUpFrame++;
			}	

			if(m_iTowerLevelUpFrame < 156)
			{
				pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16428,EP_UI);	
				if(pEffectTex)
				{
					pEffectTex->EnableSysAnim(FALSE);
					pEffectTex->SetCurFrame(m_iTowerLevelUpFrame / 6);
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pEffectTex);
					g_pGfx->SetRenderMode();
					m_iTowerLevelUpFrame++;
				}	
			}
		}		
	}

	if(iSubPage == 2 && (iGrandsonPage == 1 || iGrandsonPage == 2))//珍宝阁,极品阁	
	{
		int ix,iy;
		g_pControl->GetMouseXY(ix,iy);
		int iMouseOnGrid = GetGrid(ix - m_iScreenX,iy - m_iScreenY);		//鼠标所在的格子		

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16134,EP_UI);
		LPTexture pMouseOnTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16135,EP_UI);
		LPTexture pPrizeTex = NULL;
		LPTexture pSkeletonTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16174,EP_UI);

		if(iGrandsonPage == 1)
		{
			_ZhenBaoState& state = g_NPC.GetZhenBaoState();		
			int iGrids = GetZhenBaoGeGrids();
			int iGridWidth = 39,iGridHeight = 39;
			int iLeft = 58,iTop = 103;

			for(int i = 0;i < 77;i++)
			{
				if(state.grid[i].iState == 0)
				{
					if(rand() % 400 == 1)
					{
						state.grid[i].dwOpenTime = GetTickCount();
						state.grid[i].iState = 101;
					}
				}

				//int iRow = i / 11;
				//int iCol = i % 11;
				int iRow = i % 7;
				int iCol = i / 7;

				if (i >= iGrids)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17538,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft + iCol * iGridWidth,m_iScreenY + iTop + iRow * iGridHeight,pTex);
					continue;
				}

				if(GetTickCount() - state.grid[i].dwOpenTime < 1600 && state.grid[i].iState == 101)
				{
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft + iCol * iGridWidth,m_iScreenY + iTop + iRow * iGridHeight,pTex);
					LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16287 + ((GetTickCount() - state.grid[i].dwOpenTime) / 200),EP_UI);
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft - 15 + iCol * iGridWidth,m_iScreenY + iTop - 16 + iRow * iGridHeight,pEffectTex);
					g_pGfx->SetRenderMode();					
					continue;
				}
				if(GetTickCount() - state.grid[i].dwOpenTime < 800 && state.grid[i].iState == 100)	//绘制格子开启特效
				{
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft + iCol * iGridWidth,m_iScreenY + iTop + iRow * iGridHeight,pTex);
					LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16287 + ((GetTickCount() - state.grid[i].dwOpenTime) / 100),EP_UI);
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft - 15 + iCol * iGridWidth,m_iScreenY + iTop - 16 + iRow * iGridHeight,pEffectTex);
					g_pGfx->SetRenderMode();					
					continue;
				}
				else if(state.grid[i].iState == 100)
				{
					state.grid[i].iState = 0;
					g_pGameControl->SEND_ZHENBAO_INFO(1,i);
				}
				else if(state.grid[i].iState == 101)
				{
					state.grid[i].iState = 0;
				}

				if(state.grid[i].iState == 2)		//已打开（既不能点击）
				{
					if(state.grid[i].bShine)
					{
						LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16296,EP_UI);
						g_pGfx->SetRenderMode(RM_ADD2);
						g_pGfx->DrawTextureNL(m_iScreenX + iLeft - 45 + iCol * iGridWidth,m_iScreenY + iTop - 47 + iRow * iGridHeight,pEffectTex);
						g_pGfx->SetRenderMode();
					}

					if(state.grid[i].iType == 4)		//开启的极品阁格子
					{
						pPrizeTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16179,EP_UI);
					}
					else
					{
						pPrizeTex = g_pTexMgr->GetTex(PACKAGE_items,state.grid[i].uLooks,EP_UI);
					}
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft + iCol * iGridWidth,m_iScreenY + iTop + iRow * iGridHeight,pPrizeTex);
				}
				else if(state.grid[i].iState == 1)	    //未开启的极品阁格子
				{
					if(state.grid[i].bShine)
					{
						LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16296,EP_UI);
						g_pGfx->SetRenderMode(RM_ADD2);
						g_pGfx->DrawTextureNL(m_iScreenX + iLeft - 45 + iCol * iGridWidth,m_iScreenY + iTop - 47 + iRow * iGridHeight,pEffectTex);
						g_pGfx->SetRenderMode();
					}

					pPrizeTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16178,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft + iCol * iGridWidth,m_iScreenY + iTop + iRow * iGridHeight,pPrizeTex,EP_UI);
				}
				else if(i != iMouseOnGrid)					//鼠标未在格子上,格子未打开
				{
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft + iCol * iGridWidth,m_iScreenY + iTop + iRow * iGridHeight,pTex);
				}
				else								//鼠标在格子上,格子未打开
				{
					g_pGfx->DrawTextureNL(m_iScreenX + iLeft + iCol * iGridWidth,m_iScreenY + iTop + iRow * iGridHeight,pMouseOnTex);
				}
			}

			//LPTexture pShineTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16406);
			//g_pGfx->SetRenderMode(RM_ADD2);
			//g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pShineTex);
			//g_pGfx->SetRenderMode();

			g_pFont->DrawText(m_iScreenX + 75,m_iScreenY + 385,state.strHint.c_str(),0xffffff00,FONT_DEFAULT,14);
			g_pGfx->DrawTextureNL(m_iScreenX + 520,m_iScreenY + 111,g_pTexMgr->GetTex(PACKAGE_INTERFACE,16644,EP_UI));
		}
		else if(iGrandsonPage == 2)
		{
			_ZhenBaoState& state = g_NPC.GetZhenBaoState();

			for(int i = 0;i < 36;i++)
			{
				if(state.jipingrid[i].iState == 0)
				{
					if(rand() % 300 == 1)
					{
						state.jipingrid[i].dwOpenTime = GetTickCount();
						state.jipingrid[i].iState = 101;
					}
				}

				if(GetTickCount() - state.jipingrid[i].dwOpenTime < 1600 && state.jipingrid[i].iState == 101)
				{
					g_pGfx->DrawTextureNL(m_iScreenX + 246 + i % 6 * 39,m_iScreenY + 132 + i / 6 * 35,pTex);
					LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16287 + ((GetTickCount() - state.jipingrid[i].dwOpenTime) / 200),EP_UI);
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + 231 + i % 6 * 39,m_iScreenY + 116 + i / 6 * 35,pEffectTex);
					g_pGfx->SetRenderMode();					
					continue;
				}
				if(GetTickCount() - state.jipingrid[i].dwOpenTime < 800 && state.jipingrid[i].iState == 100)		//绘制格子开启特效
				{
					g_pGfx->DrawTextureNL(m_iScreenX + 246 + i % 6 * 39,m_iScreenY + 132 + i / 6 * 35,pTex);
					LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16287 + ((GetTickCount() - state.jipingrid[i].dwOpenTime) / 100),EP_UI);
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + 231 + i % 6 * 39,m_iScreenY + 116 + i / 6 * 35,pEffectTex);
					g_pGfx->SetRenderMode();					
					continue;
				}
				else if(state.jipingrid[i].iState == 100)
				{
					state.jipingrid[i].iState = 0;
					g_pGameControl->SEND_ZHENBAO_INFO(3,i);
				}
				else if(state.jipingrid[i].iState == 101)
				{
					state.jipingrid[i].iState = 0;
				}

				if(state.jipingrid[i].iState == 2)		//已打开（既不能点击）
				{
					if(state.jipingrid[i].bShine)
					{
						LPTexture pEffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16296,EP_UI);
						g_pGfx->SetRenderMode(RM_ADD2);
						g_pGfx->DrawTextureNL(m_iScreenX + 202 + i % 6 * 39,m_iScreenY + 86 + i / 6 * 35,pEffectTex);
						g_pGfx->SetRenderMode();
					}

					if(state.jipingrid[i].iType == 1)
					{
						g_pGfx->DrawTextureNL(m_iScreenX + 246 + i % 6 * 39,m_iScreenY + 132 + i / 6 * 35,pSkeletonTex);
					}
					else
					{
						CGoodGrid::DrawOneGood(m_iScreenX + 264 + i % 6 * 39,m_iScreenY + 149 + i / 6 * 35,state.jipingrid[i].prize);
					}
				}
				else if(i != iMouseOnGrid)					//鼠标未在格子上,格子未打开
				{
					g_pGfx->DrawTextureNL(m_iScreenX + 246 + i % 6 * 39,m_iScreenY + 132 + i / 6 * 35,pTex);
				}
				else								//鼠标在格子上,格子未打开
				{
					g_pGfx->DrawTextureNL(m_iScreenX + 246 + i % 6 * 39,m_iScreenY + 132 + i / 6 * 35,pMouseOnTex);
				}
			}

			if(m_iFrame < 60)
			{
				LPTexture pEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16177,EP_UI);		

				if(pEffect)
				{
					pEffect->EnableSysAnim(FALSE);
					pEffect->SetCurFrame(m_iFrame / 6);
					g_pGfx->SetRenderMode(RM_ADD1);
					g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pEffect);
					g_pGfx->SetRenderMode();
				}

				m_iFrame++;
			}

			if(m_iSkullFrame < 54)
			{
				LPTexture pEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16400,EP_UI);		

				if(pEffect)
				{
					pEffect->EnableSysAnim(FALSE);
					pEffect->SetCurFrame(m_iSkullFrame / 6);
					g_pGfx->SetRenderMode(RM_ADD1);
					g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pEffect);
					g_pGfx->SetRenderMode();
				}
				m_iSkullFrame++;
			}
			else if(m_iSkullFrame == 54)
			{
				m_iSkullFrame = 100;
				g_MsgBoxMgr.PopSimpleAlert("你不小心触发了骷髅的机关，将你弹出了极品阁。",MSG_CTRL_LEAVE_JIPINGE);
			}
			g_pGfx->DrawTextureNL(m_iScreenX + 560,m_iScreenY + 111,g_pTexMgr->GetTex(PACKAGE_INTERFACE,16643,EP_UI));
		}		

		//目前拥有的行会银两
		char szTemp[100]={0};
		char szCount[48] = {0};
		ToCommaStr(szCount,g_GuildData.GetGuildTael());
		sprintf(szTemp,"你目前拥有行会银两:%s两",szCount);
		//g_pFont->DrawText(GetScreenX() + 224,GetScreenY() + 306,"你目前拥有行会银两：",0xFFFF0000);
		g_pFont->DrawText(GetScreenX() + 64,GetScreenY() + 382,szTemp,0xFF907349);

	}
// 	else if(iSubPage == 7 || iSubPage == 18)
// 	{
// 		if(g_NPC.GetShenGongFangGoods().m_Shield.GetID() != 0 && g_NPC.GetShenGongFangGoods().m_Wood.GetID() != 0 && !m_bSendButtonClick)
// 		{
// 			m_iFrame = 0;
// 		}
// 
// 		if(!IsStop())
// 		{
// 			LPTexture pFire = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16402); 
// 			if(pFire)
// 			{
// 				pFire->EnableSysAnim(FALSE);
// 				pFire->SetCurFrame((m_iFrame % 42) / 6);
// 				g_pGfx->SetRenderMode(RM_ADD1);
// 				g_pGfx->DrawTextureNL(m_iScreenX + 5,m_iScreenY - 27,pFire);
// 				g_pGfx->SetRenderMode();	
// 			}			
// 			m_iFrame++;
// 
// 			if(iSubPage == 18 && (g_NPC.GetShenGongFangGoods().iResult == 1 || g_NPC.GetShenGongFangGoods().iResult == 3))	//圣王炼制成功物品直接删除并且新物品出现在包裹中
// 			{
// 
// 			}
// 			else
// 			{
// 				CGoodGrid::DrawOneGood(m_iScreenX + 469,m_iScreenY + 203,m_tmpShield);
// 			}
// 			CGoodGrid::DrawOneGood(m_iScreenX + 441,m_iScreenY + 277,g_NPC.GetShenGongFangGoods().m_Wood);
// 			CGoodGrid::DrawOneGood(m_iScreenX + 503,m_iScreenY + 277,g_NPC.GetShenGongFangGoods().m_Luck);
// 		}
// 		else
// 		{		
// 
// 
// 			if(m_iFireFrame < 102)
// 			{
// 				LPTexture pFire = NULL;
// 				if(g_NPC.GetShenGongFangGoods().iResult == 1)
// 				{
// 					pFire = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16403); 
// 				}
// 				else if(g_NPC.GetShenGongFangGoods().iResult == 0 || g_NPC.GetShenGongFangGoods().iResult == 2 || g_NPC.GetShenGongFangGoods().iResult == 4)
// 				{
// 					pFire = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16404); 
// 				}
// 
// 				if(pFire)
// 				{
// 					pFire->EnableSysAnim(FALSE);
// 					pFire->SetCurFrame(m_iFireFrame / 3);
// 					g_pGfx->SetRenderMode(RM_ADD1);
// 					g_pGfx->DrawTextureNL(m_iScreenX + 5,m_iScreenY - 27,pFire);
// 					g_pGfx->SetRenderMode();
// 					m_iFireFrame++;
// 				}
// 
// 			}
// 			else if(m_iFireFrame == 102)
// 			{				
// 				NoteResult();
// 				m_iFireFrame++;
// 			}
// 
// 			CGoodGrid::DrawOneGood(m_iScreenX + 441,m_iScreenY + 277,g_NPC.GetShenGongFangGoods().m_Wood);
// 			CGoodGrid::DrawOneGood(m_iScreenX + 503,m_iScreenY + 277,g_NPC.GetShenGongFangGoods().m_Luck);
// 			if(m_iFireFrame <= 102)
// 			{
// 				if(iSubPage == 18 && g_NPC.GetShenGongFangGoods().iResult == 1)	//圣王炼制成功物品直接删除并且新物品出现在包裹中
// 				{
// 
// 				}
// 				else
// 				{
// 					CGoodGrid::DrawOneGood(m_iScreenX + 469,m_iScreenY + 203,m_tmpShield);
// 				}
// 			}
// 			else
// 			{
// 				CGoodGrid::DrawOneGood(m_iScreenX + 469,m_iScreenY + 203,g_NPC.GetShenGongFangGoods().m_Shield);
// 			}
// 		}		
// 	}
	else if (iSubPage == 4)//议事厅,行会任务
	{//绘制列表	

		g_pFont->DrawText(m_iScreenX + 261,m_iScreenY + 95,"任 务 名 称           每日任务总数     今日已接任务数",0xFFFF0000);


		CTaskData::VOldTaskList& vTask = g_TaskData.GetGuildTaskList();
		if (vTask.size() <= 0) return;
		//m_pMarkViewer->SetDisLine(m_pScrollDes->GetPos());
		int iStartRow = m_pScrollTask->GetPos();
		if (iStartRow < 0) iStartRow = 0;		

		if (iStartRow >= vTask.size())
		{
			return;
		}
		int iEndRow = iStartRow + 4;
		if (iEndRow >= vTask.size())
		{
			iEndRow = vTask.size() - 1;
		}
		for (int i = iStartRow;i <= iEndRow;i++)
		{//绘制数据
			CTaskData::_OldTask* pTask = vTask[i];

			//任务名称
			int ix = m_iScreenX + 225 ;
			int iy = m_iScreenY + 115 + ( i - iStartRow) * 20;
			DWORD dwColor = 0xFFFFFFFF;
			LPTexture pTex = NULL;
			if (m_wSelTask == pTask->wTaskID)
			{//绘制一个选中框				
				dwColor = 0xFFFF0000;
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16647,EP_UI);
			}
			g_pFont->DrawText(ix,iy,pTask->strTaskName.c_str(),dwColor);
			if (pTex)
				g_pGfx->DrawTextureNL(ix - 3,iy - 2,pTex);

			ix += 200;
			char ctemp[10]={0};
			sprintf(ctemp,"%d",pTask->wTotalNum);
			g_pFont->DrawText(ix,iy,ctemp,dwColor);

			memset(ctemp,0,10);
			ix += 80;
			sprintf(ctemp,"%d",pTask->wRevNum);
			g_pFont->DrawText(ix,iy,ctemp,dwColor);
		}			
	}
	else if (iSubPage == 1 && (iGrandsonPage == 1 || iGrandsonPage == 2))//捐献资源,所捐献的资源
	{
		CGoodArray& goodArray = SELF.GetDevGuildGood();

		if (goodArray.Get(0).GetID() != 0)
		{
			CGoodGrid::DrawOneGood(GetScreenX() + 247,GetScreenY() + 375,goodArray.Get(0));
		}

		if (goodArray.Get(1).GetID() != 0)
		{
			CGoodGrid::DrawOneGood(GetScreenX() + 296,GetScreenY() + 375,goodArray.Get(1));
		}

		if (goodArray.Get(2).GetID() != 0)
		{
			CGoodGrid::DrawOneGood(GetScreenX() + 346,GetScreenY() + 375,goodArray.Get(2));
		}

		if (iGrandsonPage == 2)
		{
			//资源数据
			CGuildData::sGuildTower& guildTower = g_GuildData.GetGuildTower();
			g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 94,"资源名称",0xFFFFFFFF);
			g_pFont->DrawText(m_iScreenX + 446,m_iScreenY + 94,"资源值",0xFFFFFFFF);

			string szResName[2] = {"行会宝石","金  币"};

			for (int i = 0;i < 2;i++)
			{
				g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 128 + i*21,szResName[i].c_str(),0xFFFFFFFF);

				char cTemp[128]={0};
				sprintf(cTemp,"%d",guildTower.dwResource[i]);
				g_pFont->DrawText(m_iScreenX + 446,m_iScreenY + 129 + i*21,cTemp,0xFFFFFFFF);	

				LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16636 + i,EP_UI);
				int iTemp;
				if (guildTower.dwResourceMax[i] > 0)
					iTemp = (guildTower.dwResource[i] * 154) / guildTower.dwResourceMax[i];
				else iTemp = 0;
				if (iTemp > 154) iTemp = 154;
				if (pTex) g_pGfx->DrawPartTexture(m_iScreenX + 290,m_iScreenY + 133 + i*21,pTex,0,0,iTemp);				
			}
		}

		//目前拥有的行会银两
		char szTemp[100]={0};
		char szCount[48] = {0};
		ToCommaStr(szCount,g_GuildData.GetGuildTael());
		sprintf(szTemp,"%s两",szCount);
		g_pFont->DrawText(GetScreenX() + 224,GetScreenY() + 304,"你目前拥有行会银两：",0xFFFF0000);
		g_pFont->DrawText(GetScreenX() + 230,GetScreenY() + 325,szTemp,0xFF907349);
	}
// 	else if (iSubPage == 15)
// 	{
// 		//动员令窗口
// 		int iShowRowTotal = 7;
// 		DWORD dwColor = 0xFFFFFFFF;
// 		int iPos = m_pScrollGuildStel->GetPos();
// 		if (m_vGuildMember.size() > iPos && iPos >= 0)
// 		{
// 			//开始绘制相关数据
// 			int iShowNum = 0;
// 			if (iPos + iShowRowTotal <= m_vGuildMember.size())
// 				iShowNum = iShowRowTotal;
// 			else iShowNum = m_vGuildMember.size() - iPos;
// 
// 			for (int i = 0;i<iShowNum;i++)
// 			{
// 				dwColor = 0xFFFFFFFF;
// 				if (iPos + i == m_iSelectGuild)
// 				{
// 					LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16655);
// 					if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 227,m_iScreenY + 147 + i*24,pTex);
// 					dwColor = 0xFFFF0000;
// 				}
// 				g_pFont->DrawText(GetScreenX() + 245,GetScreenY() + 150 + i*24,m_vGuildMember[iPos + i].c_str(),dwColor);
// 			}
// 		}
// 
// 		iPos = m_pScrollStelHero->GetPos();
// 		if (iPos >= 0 && m_vHeroList.size() > iPos)
// 		{
// 			//开始绘制数据
// 			int iShowNum = 0;
// 			if (iPos + iShowRowTotal <= m_vHeroList.size())
// 				iShowNum = iShowRowTotal;
// 			else iShowNum = m_vHeroList.size() - iPos;
// 
// 			for (int i = 0;i<iShowNum;i++)
// 			{
// 				dwColor = 0xFFFFFFFF;
// 				if (iPos + i == m_iSelectHero)
// 				{
// 					LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16655);
// 					if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 443,m_iScreenY + 147 + i*24,pTex);
// 					dwColor = 0xFFFF0000;
// 				}
// 				g_pFont->DrawText(GetScreenX() + 450,GetScreenY() + 150 + i*24,m_vHeroList[iPos + i].c_str(),dwColor);
// 			}
// 		}
// 
// 		char strTemp[128]={0};
// 		if(m_vHeroList.size() < g_GuildData.GetGuildSteleHeroNum())
// 		{
// 			sprintf(strTemp,"已挑选%d名战将，还有%d名可选",m_vHeroList.size(),g_GuildData.GetGuildSteleHeroNum() - m_vHeroList.size());
// 			g_pFont->DrawText(GetScreenX() + 283,GetScreenY() + 325,strTemp,0xFFFFFFFF);
// 		}
// 		else
// 		{
// 			sprintf(strTemp,"已挑选满%d名战将，请发出动员令",g_GuildData.GetGuildSteleHeroNum());
// 			g_pFont->DrawText(GetScreenX() + 277,GetScreenY() + 325,strTemp,0xFFFFFFFF);
// 		}
// 
// 	}
// 	else if (iSubPage == 16)
// 	{
// 		//评定战斗值，前来报到窗口
// 		int iPos = m_pFghValueScroll->GetPos();
// 		int iShowMax = 7;
// 		DWORD dwColor = 0xFFCEC194;
// 		if (iPos >= 0 && iPos < g_GuildData.GetGuildSteleMember().size())
// 		{
// 			int iShow = 0;
// 			if (iPos + iShowMax <= g_GuildData.GetGuildSteleMember().size())
// 				iShow = iShowMax;
// 			else iShow = g_GuildData.GetGuildSteleMember().size() - iPos;
// 
// 			for (int i = 0;i<iShow;i++)
// 			{
// 				CGuildData::_GuildStele& guildMem = g_GuildData.GetGuildSteleMember()[i + iPos];
// 				m_pBtnArray[i]->SetShow(true);
// 				if (guildMem.bPart)
// 				{	
// 					dwColor = 0xFFCEC194;
// 					m_pBtnArray[i]->Disable();
// 					m_pBtnArray[i]->SetDisableTex(16648);
// 					if (strcmp(SELF.GetName(),guildMem.strName.c_str()) == 0)
// 						dwColor = 0xFF00FF00;
// 				}
// 				else
// 				{			
// 					if (strcmp(SELF.GetName(),guildMem.strName.c_str()) == 0)
// 					{
// 						dwColor = 0xFFFF0000;
// 						m_pBtnArray[i]->SetEnable(true);
// 						m_pBtnArray[i]->SetSwitchTex(16640,16641,16642,16643);
// 					}
// 					else
// 					{
// 						dwColor = 0xFFCEC194;
// 						m_pBtnArray[i]->SetEnable(false);
// 						m_pBtnArray[i]->SetDisableTex(16643);
// 					}					
// 				}
// 
// 				g_pFont->DrawText(GetScreenX() + 233,GetScreenY() + 142 + i*24,guildMem.strName.c_str(),dwColor);
// 			}
// 
// 			for (int j = iShow;j<iShowMax;j++)
// 			{
// 				m_pBtnArray[j]->SetShow(false);
// 			}
// 		}
// 		else
// 		{
// 			for (int j = 0;j<iShowMax;j++)
// 			{
// 				m_pBtnArray[j]->SetShow(false);
// 			}
// 		}
// 		char ctemp[10] = {0};
// 		itoa(g_GuildData.GetGuildSteleFghValue(),ctemp,10);
// 		int iLen = strlen(ctemp);
// 		if (iLen > 0)
// 		{
// 			for (int i = 0;i<iLen;i++)
// 			{
// 				if (ctemp[i] >= '0' && ctemp[i] <= '9')
// 				{
// 					LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16660 + ctemp[i] - '0');
// 					if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 415 + i * 15,m_iScreenY + 325,pTex);
// 				}
// 			}
// 		}
// 	}
// 	else if (iSubPage == 17)
// 	{//仙级级别和人数
// 		DWORD wTemp = g_GuildData.GetGuildSteleLevelNum();
// 		WORD wNum = HIWORD(wTemp);
// 		WORD wLevel = LOWORD(wTemp);
// 		if (wLevel > 2 || wNum >= 10000) return;
// 
// 		char ctemp[10] = {0};
// 		itoa(wNum,ctemp,10);
// 		int iLen = strlen(ctemp);
// 
// 		if (wLevel >= 0 && wLevel <= 2)
// 		{
// 			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16656 + wLevel);
// 			if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 207,m_iScreenY + 130,pTex);
// 
// 			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16659);
// 			if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 270,m_iScreenY + 250,pTex);
// 
// 			itoa(g_GuildData.GetGuildSteleFghValue(),ctemp,10);
// 			iLen = strlen(ctemp);
// 			if (iLen > 0)
// 			{
// 				for (int i = 0;i<iLen;i++)
// 				{
// 					if (ctemp[i] >= '0' && ctemp[i] <= '9')
// 					{
// 						LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16660 + ctemp[i] - '0');
// 						if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 440 + i * 15,m_iScreenY + 250,pTex);
// 					}
// 				}
// 			}
// 		}
// 	}
}

void CRelationWnd::RequestGuildMember()
{
	if (g_GuildData.GetGuildMemberOfficer().size() > 0)
		return;
	g_pGameControl->SEND_Guild_Look_Member_List();
}


int CRelationWnd::GetGrid(int iX, int iY,int *pRealMouseOnGrid)
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
	int iGrandsonPage = -1;
	if (m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].vSubTabPage.size() > 0)
	{
		iGrandsonPage = m_TabPage.vSubTabPage[iCurPage].vSubTabPage[iSubPage].iCurPage;
	}

	int iGridWidth = 39,iGridHeight = 39;
	int iLeft = 58,iTop = 103;
	int iGrids = GetZhenBaoGeGrids();
	int iRealMouseOnGrid = -1;
	

	if(iSubPage == 2 && iGrandsonPage == 1)	//珍宝阁
	{
		if(iX > iLeft && iY > iTop && iX < iLeft + iGridWidth * (/*iGrids*/77 / 7) && iY < iTop + iGridHeight * 7)
		{
			iRealMouseOnGrid = ((iX - iLeft) / iGridWidth) * 7 + ((iY - iTop) / iGridHeight);
		}
		else
		{
			iRealMouseOnGrid = -1;
		}
	}
	else if(iSubPage == 2 && iGrandsonPage == 2)	//极品阁
	{
		if(iX > 244 && iY > 130 && iX < 478 && iY < 340)
		{
			iRealMouseOnGrid = ((iX - 244) / 39) + ((iY - 130) / 35) * 6;
		}
		else
		{
			iRealMouseOnGrid = -1;
		}
	}
	else
	{
		iRealMouseOnGrid = -1;
	}

	if (pRealMouseOnGrid)
	{
		*pRealMouseOnGrid = iRealMouseOnGrid;
	}
	if (iRealMouseOnGrid == -1 || iRealMouseOnGrid >= iGrids)
	{
		return -1;
	}
	else
	{
		return iRealMouseOnGrid;
	}
}

bool CRelationWnd::IsStop()
{
	if(!m_bSendButtonClick)
		return true;

	if(g_NPC.GetShenGongFangGoods().iResult == -1)
		return false;

	if(g_NPC.GetShenGongFangGoods().iResult == 3 && m_iFrame > 20)
		return true;

	if(m_iFrame >= 84)
	{
		m_bSendButtonClick = false;
		m_iFireFrame = 0;		
		return true;
	}
	else
	{
		return false;
	}
}

void CRelationWnd::OnClickCloseButton()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	//if(iCurPage == 2 && iSubPage == 2 && iGrandsonPage == 2 && !g_NPC.GetZhenBaoState().bInViewMode)
	//{
	//	g_MsgBoxMgr.PopSimpleComfirm("你确定要离开极品阁吗？",MSG_CTRL_LEAVE_JIPINGE,1);
	//	return;
	//}

	//if(iCurPage == 2 && iSubPage == 7 && (!IsStop() || m_iFireFrame <= 102))   //炼制动画未结束，不能关闭界面
	//{
	//	return;
	//}	

	CCtrlWindowX::OnClickCloseButton();
}

bool CRelationWnd::DelAllySuccess(const char * strName)
{
	if(!strName)
		return false;

	vector<string>::iterator ite;
	vector<string> VAlly = g_GuildData.GetAlly();
	for(ite = VAlly.begin(); ite != VAlly.end(); ite++)
	{
		if((*ite) == string(strName))
		{
			VAlly.erase(ite);
			return true;
		}
	}

	return false;
}

bool CRelationWnd::AddAllySuccess(const char * strName)
{
	if(!strName)
		return false;

	g_GuildData.GetAlly().push_back(strName); 
	return true;
}
void CRelationWnd::Send_Hero_List()
{//选中的战将名单
	int iNum = m_vHeroList.size();
	char cBuff[12 + 14 * 256] = {0};
	*((WORD*)(cBuff + 4)) = CS_STELE_BANI;
	*((BYTE*)(cBuff + 6)) = iNum;

	int iLen = 12;
	for(size_t i = 0; i < iNum; i++)
	{		
		memcpy(cBuff + iLen,m_vHeroList[i].c_str(),m_vHeroList[i].size());
		iLen += 14;
	}
	g_pNet->SendBuf(SERVER_GAME,cBuff,iLen + 12);
}

void CRelationWnd::NoteResult()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(g_NPC.GetShenGongFangGoods().iResult == 1)
	{
		if(iSubPage == 7)
		{
			if(m_tmpShield.GetStdMode() == 12)
			{
				g_TalkMgr.PushSysTalk("恭喜你，盾牌炼制成功，品级上升一级，攻击、防御属性提高。");
			}
			else
			{
				g_TalkMgr.PushSysTalk("恭喜你，魂珠升级成功");
			}
		}
		else if(iSubPage == 18)
		{
			g_TalkMgr.PushSysTalk("恭喜你，圣王炼制成功");
		}
	}
	else if(g_NPC.GetShenGongFangGoods().iResult == 0)
	{
		if(m_tmpShield.GetStdMode() == 12)
		{
			g_TalkMgr.PushSysTalk("火候太小，炼制失败。");
		}
		else
		{
			g_TalkMgr.PushSysTalk("炼制失败。");
		}
	}
	else if(g_NPC.GetShenGongFangGoods().iResult == 2)
	{
		if(iSubPage == 7)
			g_TalkMgr.PushSysTalk("火候太大 ，炼制失败，盾牌破碎。");
		else if(iSubPage == 18)
			g_TalkMgr.PushSysTalk("火候太大 ，炼制失败，首饰破碎。");
	}
	else if(g_NPC.GetShenGongFangGoods().iResult == 4)
	{
		if(iSubPage == 7)
			g_TalkMgr.PushSysTalk("火候太大 ，炼制失败，由于有护宝符的保护，盾牌没有破碎。");
		else if(iSubPage == 18)
			g_TalkMgr.PushSysTalk("火候太大 ，炼制失败，由于有护宝符的保护，首饰没有破碎。");
	}
}

void  CRelationWnd::OnSetFocus()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(iCurPage == 1 && iSubPage == 4)
	{
		g_WidgetMgr.SetFocus(EWT_GREAT_EVENT,true);
	}
}

void CRelationWnd::OnKillFocus()
{
	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(iCurPage == 1 && iSubPage == 4)
	{
		g_WidgetMgr.SetFocus(EWT_GREAT_EVENT,false);
	}
}

void CRelationWnd::OnMove()
{
	CCtrlWindowX::OnMove();

	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(iCurPage == 1 && iSubPage == 4)
	{		
		g_WidgetMgr.MoveWindow(EWT_GREAT_EVENT,m_iScreenX + 50,m_iScreenY + 92);
	}
}

int  CRelationWnd::GetZhenBaoGeGrids()
{
	int iTowerLevel = g_GuildData.GetGuildTowerLevel();		//通灵塔层数
	if(iTowerLevel >= 1 && iTowerLevel <= 9)
	{
		return 42;
	}
	else if(iTowerLevel >= 10 && iTowerLevel <= 29)
	{
		return 49;
	}
	else if(iTowerLevel >= 30 && iTowerLevel <= 49)
	{
		return 56;
	}
	else if(iTowerLevel >= 50 && iTowerLevel <= 69)
	{
		return 63;
	}
	else if(iTowerLevel >= 70 && iTowerLevel <= 89)
	{
		return 70;
	}
	else if(iTowerLevel >= 90)
	{
		return 77;
	}

	return 0;
}

void CRelationWnd::OnClickTrans()
{
	int iPos = -1;
	for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
	{
		CGood& good = SELF.GetPackageGood(i);

		if(good.GetID() == 0)
			continue;

		if( (strcmp(good.GetName(),"结义圣链") == 0 && good.GetDura() > 0 ) )
		{
			iPos = i;
			break;
		}
	}

	if(iPos == -1)
	{
		g_WooolStoreMgr.SetQuickBuyItem("结义圣链");
		CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

		if(QuickBuyData.pItem)
		{
			char cTemp[128]={0};
			sprintf(cTemp,"你没有结义圣链，需要马上购买结义圣链并使用吗？单价%d元宝",
				QuickBuyData.pItem->iPrice);

			QuickBuyData.strMsg = cTemp;
			QuickBuyData.iType = 1;
			QuickBuyData.iUseAfterBuyType = 1;
			QuickBuyData.iObjPos = iPos;

			g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);
		}
		else
		{
			g_MsgBoxMgr.PopSimpleAlert("您身上没有结义圣链，请购买结义圣链后使用。");
		}
	}
	else
	{

		int iCurPage = m_TabPage.iCurPage;
		int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

		if(iCurPage == 0 && iSubPage == SUBPAGE_INDEX_JIEYI)
		{
			int iSelLine = m_pGrid->GetSelLine();
			if(iSelLine < 0 || iSelLine >= m_vRelation.size())
				return;

			_SRelation& lRelation = m_vRelation[iSelLine];
			if (lRelation.iOnline != 1)
			{
				g_MsgBoxMgr.PopSimpleAlert("对方当前不在线，无法传送。");
				return;
			}

			std::string str = "@手足合一 " + lRelation.sName;
			g_pGameControl->SEND_Message_Send(str.c_str(),str.length());

		}
	}
}

void CRelationWnd::OnDeleteJieYi()
{
	if (m_vRelation.empty())
		return;

	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(iCurPage == 0 && iSubPage == SUBPAGE_INDEX_JIEYI)
	{
		// 如果自己是结义队长
		if (strcmp(g_OtherData.GetJieYiLeader().c_str(), SELF.GetName()) == 0)
		{
			int iSelLine = m_pGrid->GetSelLine();
			if(iSelLine < 0 || iSelLine >= m_vRelation.size())
				return;

			_SRelation& lRelation = m_vRelation[iSelLine];

			g_pGameControl->SEND_Del_Swear(lRelation.sName.c_str());
		}
		else
		{
			g_pGameControl->SEND_Del_Swear(SELF.GetName());
		}
	}
}

void CRelationWnd::OnAddJieYi()
{
	if (SELF.GetLevel() < 30)
	{
		g_MsgBoxMgr.PopSimpleAlert("请到达30级以后再发起结义。");
		return;
	}

	int iCurPage = m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if(iCurPage == 0 && iSubPage == SUBPAGE_INDEX_FRIEND)
	{
		int iSelLine = m_pGrid->GetSelLine();
		if(iSelLine < 0 || iSelLine >= m_vRelation.size())
			return;

		_SRelation& lRelation = m_vRelation[iSelLine];

		for(UINT i = 0; i < g_OtherData.GetRelationVector().size();i++)
		{
			_RelationStruct &SRelation = g_OtherData.GetRelationVector()[i];

			if(SRelation.strName == lRelation.sName && (RT_JIEYI & SRelation.iRelType) != 0 )
			{
				g_MsgBoxMgr.PopSimpleAlert("您已经和该玩家结义。");
				return;
			}
		}

		if (lRelation.iOnline != 1)
		{
			g_MsgBoxMgr.PopSimpleAlert("对方当前不在线，无法进行结义。");
			return;
		}
	
		g_pGameControl->SEND_Add_Swear(lRelation.sName.c_str());
	}
}