#include "NpcWnd.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "NpcQuickWnd.h"
#include <Shellapi.h>
#include "GameData/MsgBoxMgr.h"
#include "GameData/NpcData.h"
#include "Global/Interface/AudioInterface.h"
#include "GameClient/SDOAInterface.h"
#include "GameData/LoginData.h"
#include "GameData/OtherData.h"
#include "GameData/WooolStoreData.h"
#include "GameData/TalkMgr.h"

INIT_WND_POSX(CNpcWnd,10,30)
bool CNpcWnd::m_sbHeaderNpc = false;
CNpcWnd::CNpcWnd(void)
{
	m_sbHeaderNpc = true;
	m_iIndex = 20804;
	m_dwAppr = sm_dwWindowType;


	m_sName = "NpcWnd";

	//m_iHeight = 373;
	//m_iWidth = 378;

	m_iNpcHeadTex = 0;
	CSimpleCharacterNode * pChar = NULL;
	if (m_dwAppr == 0)
	{
		pChar = g_pGameData->FindSimpleCharacter(g_NPC.GetID());
		if (pChar)
		{
			m_dwAppr = pChar->GetRaceNo();
		}
	}


	switch (m_dwAppr)
	{
	case 0:
		m_iNpcHeadTex = 20817;
		break;
	case 1:
		m_iNpcHeadTex = 20825;
		break;
	case 2:
		m_iNpcHeadTex = 20806;
		break;
	case 3:
		m_iNpcHeadTex = 20805;
		break;
	case 4:
		m_iNpcHeadTex = 20808;
		break;
	case 5:
		if(pChar && strcmp(pChar->GetName(), "魔法阵祭祀") == 0)
			m_iNpcHeadTex = 20818;
		else
			m_iNpcHeadTex = 20822;
		break;
	case 6:
		m_iNpcHeadTex = 20823;
		break;
	case 7:
		m_iNpcHeadTex = 20812;
		break;
	case 8:
		m_iNpcHeadTex = 20815;
		break;
	case 9:
		m_iNpcHeadTex = 20807;
		break;
	case 10:
		m_iNpcHeadTex = 20828;
		break;
	case 11:
		m_iNpcHeadTex = 20811;
		break;
	case 12:
		m_iNpcHeadTex = 20819;
		break;
	case 13:
		m_iNpcHeadTex = 20809;
		break;
	case 16:
		m_iNpcHeadTex = 20813;
		break;
	case 32:
		m_iNpcHeadTex = 20816;
		break;
	case 34:
		m_iNpcHeadTex = 20820;
		break;
	case 35:
		m_iNpcHeadTex = 20821;
		break;
	case 41:
		m_iNpcHeadTex = 20814;
		break;
	case 42:
		m_iNpcHeadTex = 20824;
		break;
	case 48:
		m_iNpcHeadTex = 20830;
		break;
	case 50:
		m_iNpcHeadTex = 20810;
		break;
	case 55:
		m_iNpcHeadTex = 20829;
		break;
	case 191:
		m_iNpcHeadTex = 20827;
		break;
	case 192:
		m_iNpcHeadTex = 20826;
		break;
	case 257:
		m_iNpcHeadTex = 20831;		
		break;
	default:
		m_iNpcHeadTex = 0;
		m_iIndex = 41;
		//m_iHeight = 203;
		m_sbHeaderNpc = false;
		break;
	}


	m_iChildX = m_iScreenX;
	m_iChildY = m_iScreenY;

	m_VChildName.push_back("NpcBuyWnd");
	m_VChildName.push_back("NpcSaleWnd");
	m_VChildName.push_back("NpcQuickWnd");

	g_pControl->Msg(MSG_CTRL_TRADEWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCBOOK_WND,OPER_CLOSE);
    g_pControl->Msg(MSG_CTRL_LUXNPCWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCRECRUIT,OPER_CLOSE);	
	g_pControl->Msg(MSG_CTRL_YUANBAO_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_RELATION_WND,OPER_CUSTOM);

	m_dwLastClickCmdTime = 0;
	m_pRainbowButton = NULL;
}

CNpcWnd::~CNpcWnd(void)
{
	g_pControl->Msg(MSG_CTRL_NPCBUYWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCSALEWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPC_QUICK_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_GEMCONDENSE_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_SMALL_GEM_CON_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCLISTWND,OPER_CLOSE);
	g_pControl->Msg(MSG_SUBMIT_GOOD_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CLOSE);
	SELF.RemoveMagicState(MS_NPCWAITSERVER);
}

void CNpcWnd::OnClickCloseButton()
{
	g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@exit");
	CCtrlWindowX::OnClickCloseButton();
}

void CNpcWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	if (!g_pGameData->HasPaoPaoStatus(EP_First_TalkWithNPC))
	{
		CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(g_NPC.GetID());
		if (pChar)
		{
			if(strcmp(pChar->GetName(), "神武军将领") == 0)
			{
				AddArrowTip(EP_First_TalkWithNPC,70,95);
			}
		}
	}


	//数据显示框

	int iX = 0;
	int iY = m_sbHeaderNpc?170:0;

	m_pMarkViewer = new CMarkViewer(16,12,iX,iY);

	AddControl(m_pMarkViewer);
	m_pMarkViewer->CreateXML(this,m_pXmlWindow->FindChild("MarkViewer"));
	m_pMarkViewer->SetTagText(&g_TagTextMgr.GetNpcText());

	m_pMarkViewer->SetFont(FONT_YAHEI);

	SetCloseButton(349,m_sbHeaderNpc?173:3, 80);

	//AP不要彩虹帮助,里面有广告,彩虹无法去掉,平台不接受
	if (g_strChannelName.empty())
	{
		m_pRainbowButton = new CCtrlButton();
		AddControl(m_pRainbowButton);
		DWORD dwX = 349;
		DWORD dwY = m_sbHeaderNpc?344:344 - 170;
		m_pRainbowButton->CreateEx(this,349,dwY,20800,20801,20802);
		m_pRainbowButton->SetTips("彩虹帮助");
	}
}

bool CNpcWnd::OnLeftButtonDown(int iX, int iY)
{
	if (m_iNpcHeadTex != 0 && iY < 170)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_iNpcHeadTex,EP_UI);
		if (pTex && !pTex->IsPointInTex(iX,iY))
		{
			return false;
		}
	}

	m_bClick = true;

	//记录按下窗口时的屏幕位置
	m_iChildX = m_iScreenX;
	m_iChildY = m_iScreenY;

	return true;
}

bool CNpcWnd::OnLeftButtonUp(int iX, int iY)
{
	if (m_iNpcHeadTex != 0 && iY < 170)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_iNpcHeadTex,EP_UI);
		if (pTex && !pTex->IsPointInTex(iX,iY))
		{
			return false;
		}
	}

	m_bClick = false;

	MoveChildWindow();

 	string strCommand = m_pMarkViewer->GetCommand();
	//strCommand = "@@OpenWnd KfzRegisterFlow";
	//strCommand = "@@LoginKfz test005005/0/新王月/222.73.12.164/7000/降龙/降龙";
	//strCommand = "@@LoginKfz test005005/0/新王月/61.172.247.80/7000/演示/演示";
	//strCommand = "@@BackFromKfz";
	//strCommand = "@@hayifu";


	if(!strCommand.empty())
	{
		if (!g_pGameData->HasPaoPaoStatus_Closed(EP_First_TalkWithNPC))
		{
			this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_TalkWithNPC);
		}

		if(GetTickCount() - m_dwLastClickCmdTime < 200)//点太快
		{
			return true;
		}

		m_dwLastClickCmdTime = GetTickCount();

		g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);
		if(strCommand.substr(0,2) == "@@")
		{
			g_NPC.SetRepairWindow(false);
			g_NPC.SetLastCommand(strCommand);

			if(strCommand.substr(2,4)=="goto")
			{
				g_pGameControl->DealGotoNpcCommand(strCommand);
			}
			else if(strCommand.substr(2,7) == "goplace")
			{
				g_pGameControl->DealGoPlaceCommand(strCommand);
			}
			//else if(strCommand.substr(2,8)=="guildwar")
			//{
			//	g_MsgBoxMgr.PopEditBox("请输入敌对行会的名称：",MSG_CTRL_NPCGUILDBOX,0);
			//}
			//else if(strCommand.substr(2,10)=="withdrawal")
			//{
			//	g_pControl->Msg(MSG_CTRL_EDITMESSAGEBOX,MSG_CTRL_NPCGUILDBOX,this);
			//	((CMainWnd *)g_pControl)->SetMsgText("请输入您要取回资金的数目：");

			//}
			//else if(strCommand.substr(2,8)=="receipts")
			//{
			//	g_pControl->Msg(MSG_CTRL_EDITMESSAGEBOX,MSG_CTRL_NPCGUILDBOX,this);
			//	((CMainWnd *)g_pControl)->SetMsgText("请输入您要存储资金的数目：");
			//}
			//else if(strCommand.substr(2,7)=="teacher")
			//{
			//	g_pControl->Msg(MSG_CTRL_EDITMESSAGEBOX,MSG_CTRL_NPCGUILDBOX,this);
			//	((CMainWnd *)g_pControl)->SetMsgText("请输入：");
			//}
			//else if(strCommand.substr(2,5)=="marry")
			//{
			//	g_pControl->Msg(MSG_CTRL_EDITMESSAGEBOX,MSG_CTRL_NPCGUILDBOX,this);
			//	((CMainWnd *)g_pControl)->SetMsgText("请输入：");
			//}
			else if(strCommand.substr(2,8) == "makehole")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,1);
			}
			else if(strCommand.substr(2,9) == "embedrune")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,2);
			}
			else if(strCommand.substr(2,14) == "changeholeattr")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,3);
			}
			else if(strCommand.substr(2,9) == "carverune")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,4);
			}
			else if(strCommand.substr(2,11) == "composerune")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,5);
			}
			else if(strCommand.substr(2,10) == "removerune")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,6);
			}
			else if(strCommand.substr(2,12) == "upgradeequip")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,7);
			}
			else if(strCommand.substr(2,10) == "runerevert")
			{
				g_pControl->PopupWindow(MSG_FUSHI_WND,OPER_RECREATE,8);
			}
			else if(strCommand.substr(2,3)=="url")
			{
				ShellExecute(NULL,"open","iexplore.exe",strCommand.substr(5).c_str(),NULL,SW_SHOW);
			}
			else if (strCommand.substr(2, 14) == "changematerial")
			{
				g_pControl->PopupWindow(MSG_CTRL_BLESSZHONGZHOU_WND, OPER_RECREATE, 1);
			}
			//else if(strCommand.substr(2,9)=="guildlogo")
			//{
			//	g_pControl->Msg(MSG_CTRL_EDITMESSAGEBOX,MSG_CTRL_GUILD_LOGO_ID,this);
			//	((CMainWnd *)g_pControl)->SetMsgText("请输入行会会旗的编号：");
			//	((CMainWnd*)g_pControl)->SetMaxLength(8);
			//}
			else if(strCommand.substr(2,11) == "gemcondense")
			{
				if(g_NPC.IsCondensing())
				{
					g_MsgBoxMgr.PopSimpleComfirm("\n\n对不起,系统正忙,请稍后再试。",0,0);
					return true;
				}
				g_pControl->Msg(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
				g_pControl->Msg(MSG_CTRL_SMALL_GEM_CON_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_RECREATE,1);
			}
			else if(strCommand.substr(2,5) == "FUSHI")
			{
				if(g_NPC.IsCondensing())
				{
					g_MsgBoxMgr.PopSimpleComfirm("\n\n对不起,系统正忙,请稍后再试。",0,0);
					return true;
				}

				std::string strTemp001 = strCommand.substr(8,strCommand.length() - 8);
				int itype = atoi(strTemp001.c_str());
				g_pGameData->SetSuperArmCondenseType(itype);
				g_pControl->PopupWindow(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_SMALL_GEM_CON_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_RECREATE,6);
			}
			else if(strCommand.substr(2,11) == "gemcompound")
			{
				if(g_NPC.IsCondensing())
				{
					g_MsgBoxMgr.PopSimpleAlert("\n\n对不起,系统正忙,请稍后再试。",0,0);
					return true;
				}
				g_pControl->PopupWindow(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_SMALL_GEM_CON_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_RECREATE,2);
			}
			////矿石合成武器
			else if(strCommand.substr(2,14) == "weaponcompound")
			{
				if(g_NPC.IsCondensing())
				{
					g_MsgBoxMgr.PopSimpleComfirm("\n\n对不起,系统正忙,请稍后再试。",0,0);
					return true;
				}
				g_pControl->PopupWindow(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_SMALL_GEM_CON_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_RECREATE,3);
			}
			////超级武器合成
			else if(strCommand.substr(2,13) == "supercompound")
			{
				if(g_NPC.IsCondensing())
				{
					g_MsgBoxMgr.PopSimpleComfirm("\n\n对不起,系统正忙,请稍后再试。",0,0);
					return true;
				}
				g_pControl->PopupWindow(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_SMALL_GEM_CON_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_RECREATE,4);
			}
			////消耗装备的接口 将装备升为高级武器
			//else if(strCommand.substr(2,14) == "commoncodense")
			//{
			//	if(g_NPC.IsCondensing())
			//	{
			//		g_pControl->Msg(MSG_CTRL_MESSAGEBOX,MS_OK,NULL);
			//		((CMainWnd *)g_pControl)->SetMsgText("\n\n对不起,系统正忙,请稍后再试。");
			//		return true;
			//	}
			//	g_pControl->Msg(MSG_CTRL_GEMCONDENSE_WND,0,NULL);	
			//	g_pControl->Msg(MSG_CTRL_SMALL_GEM_CON_WND,0,NULL);
			//	g_pControl->Msg(MSG_CTRL_COMMONCONDENSE_WND,1,NULL);
			//}
			////宝石熔炼接口
			else if(strCommand.substr(2,8) == "gemsmelt")
			{
				if(g_NPC.IsCondensing())
				{
					g_MsgBoxMgr.PopSimpleComfirm("\n\n对不起,系统正忙,请稍后再试。",0,0);
					return true;
				}
				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_SMALL_GEM_CON_WND,OPER_UPDATE);
			}
			else if(strCommand.substr(2,9) == "decompose")
			{
				g_pControl->PopupWindow(MSG_CTRL_EXTRACTUPGRADE_YUANSHI,OPER_RECREATE);
			}
			else if(strCommand.substr(2,14) == "upgradeyuanshi")
			{
				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_RECREATE,12);
			}
			//装备升级接口 黑暗残留值 换取装备升级的机会
			else if(strCommand.substr(2,10) == "equiplevel" || 
				strCommand.substr(2,12) == "jewitemlevel" ||
				strCommand.substr(2,12) == "defitemlevel" ||
				strCommand.substr(2,6) == "hayifu" ||
				strCommand.substr(2,12) == "spriterefine" ||//黑暗魔力升级衣服
				strCommand.substr(2,10) == "gemextract" ||
				strCommand.substr(2,9) == "gemremove")
			{
				if(g_NPC.IsCondensing())
				{
					g_MsgBoxMgr.PopSimpleComfirm("\n\n对不起,系统正忙,请稍后再试。",0,0);
					return true;
				}
				int iType = 1;
				if(strCommand.substr(2,10) == "equiplevel")
					iType = 1;
				else if(strCommand.substr(2,12) == "jewitemlevel" )
					iType = 2;
				else if(strCommand.substr(2,12) == "defitemlevel" )
					iType = 3;
				else if(strCommand.substr(2,6) == "hayifu")//黑暗魔力升衣服
					iType = 4;
				else if(strCommand.substr(2,12) == "spriterefine")//升魂珠
					iType = 5;
				else if(strCommand.substr(2,10) == "gemextract")
					iType = 6;
				else if(strCommand.substr(2,9) == "gemremove")
					iType = 7;

				g_pControl->PopupWindow(MSG_CTRL_GEMCONDENSE_WND,OPER_CLOSE); 
				g_pControl->PopupWindow(MSG_CTRL_COMMONCONDENSE_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_SMALL_GEM_CON_WND,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_EQUIP_LEVEL_UP_WND,OPER_CREATE,iType);

			}
			else if(strCommand == "@@REALINFO")
			{
				g_pControl->PopupWindow(MSG_CTRL_USERINFO_WND,OPER_CREATE);
			}
			else if(strCommand == "@@RAINBOW")
			{
				//AP不要彩虹帮助,里面有广告,彩虹无法去掉,平台不接受
				if (g_strChannelName.empty())
				{
					StartRainBowSpirt();
				}
			}
			else if(strCommand == "@@STORE")
			{
				g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,2);
			}
            else if(strCommand == "@@HUANHUA")
            {
                g_pControl->PopupWindow(MSG_CTRL_HUANHUA_WND,OPER_CREATE);
            }
			else if(strCommand == "@@linghaoshi")
			{
				if(SELF.PackageGood().FindGoodByName("灵皓石") < 0)
				{
					//弹出快捷消费
					g_WooolStoreMgr.SetQuickBuyItem("灵皓石");
					CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

					if(QuickBuyData.pItem)
					{
						char cTemp[128]={0};
						sprintf(cTemp,"你包裹里没有灵皓石，是否需要购买？单价%d元宝",QuickBuyData.pItem->iPrice);
						QuickBuyData.iUseAfterBuyType = 9;
						QuickBuyData.strMsg = cTemp;
						QuickBuyData.iType = 1;
						//QuickBuyData.iObjPos = dwData;
						g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);						
					}
					else
					{
						g_TalkMgr.PushSysTalk("你包裹里没有灵皓石，请到商城购买。",0xFDFF);
					}

				}
				else
				{
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
				}
			}
			else if(strCommand.substr(2,8)  == "LoginKfz")//<登录跨服战服务器/@@LoginKfz ptAccount/AreaNo/AreaName/LoginGateIp/LoginGatePort/GroupName/GroupNickName>;
			{
				g_Login.SetKfzLoginString(strCommand.substr(11,strCommand.length() - 11));
				g_MsgBoxMgr.PopOkCancelBox("现在可以进入跨服争霸区,在进行跨服争霸赛时将使用您在昨天前报到时所穿的装备,离开跨服争霸准备场后将还原回您进入跨服争霸准备场前的装备状态,是否进入?",MSG_CTRL_LOGIN_KEZ_SERVER,0);
			}
			else if(strCommand == "@@BackFromKfz")
			{
				g_MsgBoxMgr.PopOkCancelBox("您即将离开跨服争霸区并进入原来的服务器,离开跨服争霸准备场后将还原回您进入跨服争霸准备场前的装备状态,是否离开?",MSG_CTRL_BACK_FROM_KFZ,0);
			}
			//else if(strCommand == "@@LeaveXL")
			//{
			//	g_MsgBoxMgr.PopOkCancelBox("您即将离开天绝魔域并返回中洲大陆,离开天绝魔域后将还原回您进入天绝魔域前的装备状态,是否离开?",MSG_CTRL_BACK_FROM_XL,0);
			//}
			//else if(strCommand.substr(2,7)  == "LoginXL")//<登录跨服战服务器/@@LoginKfz ptAccount/AreaNo/AreaName/LoginGateIp/LoginGatePort/GroupName/GroupNickName>;
			//{
			//	g_Login.SetKfzLoginString(strCommand.substr(10,strCommand.length() - 10));
			//	g_MsgBoxMgr.PopOkCancelBox("现在可以进入天绝魔域,进入天绝魔域后将使用您在报到时所穿的装备,离开天绝魔域后将还原回您进入天绝魔域前的装备状态,是否进入?",MSG_CTRL_LOGIN_XL_SERVER,0);
			//}
			else if(strCommand.substr(2,7)  == "OpenWnd")//<查看跨服战排名/@@OpenWnd KfzRange>;
			{
				string strWndName = strCommand.substr(10,strCommand.length() - 10);
				if(strWndName == "KfzRange")//跨服战排名
				{
					g_pControl->PopupWindow(MSG_CTRL_KFZ_IE_WND,OPER_RECREATE,1);
				}
				//else if(strWndName == "KfzVSMap")//跨服战对阵图
				//{
				//	g_pControl->PopupWindow(MSG_CTRL_KFZ_IE_WND,OPER_RECREATE,2);
				//}
				//else if(strWndName == "KfzRegisterFlow")//跨服战报名流程
				//{
				//	g_pControl->PopupWindow(MSG_CTRL_KFZ_FLOW_WND,OPER_RECREATE,1);
				//}
				//else if(strWndName == "KfzIntegralMatchFlow1")//跨服战积分赛自由挑战模式流程
				//{
				//	g_pControl->PopupWindow(MSG_CTRL_KFZ_FLOW_WND,OPER_RECREATE,2);
				//}
				//else if(strWndName == "KfzIntegralMatchFlow2")//跨服战积分赛擂台模式流程
				//{
				//	g_pControl->PopupWindow(MSG_CTRL_KFZ_FLOW_WND,OPER_RECREATE,3);
				//}
				//else if(strWndName == "KfzWashOutFlow")//跨服战淘汰赛流程
				//{
				//	g_pControl->PopupWindow(MSG_CTRL_KFZ_FLOW_WND,OPER_RECREATE,4);
				//}
				//else if(strWndName == "XLRange")//仙灵大陆排名
				//{
				//	g_pControl->PopupWindow(MSG_CTRL_KFZ_IE_WND,OPER_RECREATE,3);
				//}
				else if(strWndName == "VipStore")//Vip商城界面
				{
					g_pControl->PopupWindow(MSG_CTRL_VIPSTORE_WND,OPER_RECREATE);
				}
// 				else if(strWndName == "RtsGuide")//rts教程界面
// 				{
// 					g_pControl->PopupWindow(MSG_CTRL_RTSGUIDE_WND,OPER_RECREATE);
// 				}
				else if(strWndName == "EquipSoulWakeUpWnd")//魂器觉醒
				{
					g_pControl->PopupWindow(MSG_CTRL_EQUIPSOUL_WND,OPER_RECREATE,1);
				}
				else if(strWndName == "EquipSoulLevelUpWnd")//魂器升级
				{
					g_pControl->PopupWindow(MSG_CTRL_EQUIPSOUL_WND,OPER_RECREATE,2);
					g_pControl->MsgToWnd("EquipSoulWnd",MSG_CTRL_EQUIPSOUL_WND,100);
				}
				else if(strWndName == "EquipSoulLevelUpWndWithBindGold")//绑定金币魂器升级
				{
					g_pControl->PopupWindow(MSG_CTRL_EQUIPSOUL_WND,OPER_RECREATE,2);
					g_pControl->MsgToWnd("EquipSoulWnd",MSG_CTRL_EQUIPSOUL_WND,101);
				}
				else if(strWndName == "EquipSoulPeelWnd")//魂器剥离
				{
					g_pControl->PopupWindow(MSG_CTRL_EQUIPSOUL_WND,OPER_RECREATE,3);
				}
				else if(strWndName == "WingLevelUpWnd")//翅膀升级
				{
					if (SELF.GetHolyWingLevel() >= 0)		
						g_pControl->PopupWindow(MSG_CTRL_DIVINITYWING_WND,OPER_CREATE);
				}
			}
			else if (strCommand =="@@refine")
			{//炼制
				if(!SELF.IsLearnMagic(MAGICID_SMELTSKILL))
				{
					g_MsgBoxMgr.PopSimpleAlert("你没有掌握炼制技能，不能进行炼制！");
					return true;
				}
				//弹出铸造窗口
				g_pControl->PopupWindow(MSG_CTRL_PRODUCE_WND,OPER_RECREATE,1);//炼制
			}
			else if (strCommand == "@@found")
			{//铸造
				if(!SELF.IsLearnMagic(MAGICID_FOUNDSKILL))
				{
					g_MsgBoxMgr.PopSimpleAlert("你没有掌握炼铸造技能，不能进行铸造！");
					return true;
				}
				//弹出炼制窗口
				g_pControl->PopupWindow(MSG_CTRL_PRODUCE_WND,OPER_RECREATE,2);//铸造
			}
			else if (strCommand == "@@blessatti")
			{//神佑属性融合				
				g_pControl->PopupWindow(MSG_CTRL_BLESSCOMPOUND_WND,OPER_RECREATE,1);
			}
			else if (strCommand == "@@blessfun")
			{//神佑功能融合
				g_pControl->PopupWindow(MSG_CTRL_BLESSCOMPOUND_WND,OPER_RECREATE,2);
			}
			else if (strCommand == "@@blessextend")
			{//神佑修复
				g_pControl->PopupWindow(MSG_CTRL_BLESSCOMPOUND_WND,OPER_RECREATE,3);
			}
			else if (strCommand == "@@wenpeiflush")
			{//纹佩刷新
				g_pControl->PopupWindow(MSG_CTRL_BLESSCOMPOUND_WND,OPER_RECREATE, 4);
			}
			else if (strCommand == "@@entersjs")
			{//进入圣殿山
				g_pControl->PopupWindow(MSG_CTRL_ENTERSHAN_WND,OPER_RECREATE);
			}
			else if (strCommand == "@@equiphunupgrade")
			{//装备 圣战之魂 合成
				g_pControl->PopupWindow( MSG_CTRL_COMPOSEEQUIP_WND,OPER_RECREATE,1);
			}
			else if (strCommand == "@@fireart")
			{//真火炼化
				g_pControl->PopupWindow( MSG_CTRL_FIREARTIFICE_WND,OPER_RECREATE);
			}
			else if (strCommand == "@@fabaofaqh")
			{//法宝强化
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,1);
			}
			else if (strCommand == "@@fabaoopenls")
			{//开启灵识
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,2);
			}
			else if (strCommand == "@@fabaosljh")
			{//圣灵净化
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,3);
			}
			else if (strCommand == "@@fabaoslzr")
			{//圣灵注入
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,4);
			}
			else if (strCommand == "@@fabaoslhy")
			{//圣灵还原
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,5);
			}
			else if (strCommand == "@@fabaochangels")
			{//更改灵识属性
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,6);
			}
			else if (strCommand == "@@fabaoslrh")
			{//圣灵融合
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,7);
			}
			else if (strCommand == "@@fabaosltq")
			{//圣灵提取
				g_pControl->PopupWindow( MSG_CTRL_FABAOLEVELUP_WND,OPER_RECREATE,8);
			}
			else if (strCommand == "@@lingwufire")
			{//领悟三味真火
				g_pControl->PopupWindow( MSG_CTRL_LINGWUSANWEIFIRE_WND,OPER_RECREATE);
			}
			else if (strCommand == "@@wenpeironghe")
			{//纹佩融合
				g_pControl->PopupWindow(MSG_CTRL_BLESSCOMPOUND_WND,OPER_RECREATE, 5);
			}
			else if (strCommand == "@@open12gong")
			{//12宫布兵
				g_pControl->PopupWindow(MSG_CTRL_DEFEND12GONG_WND,OPER_RECREATE);
			}
			else if (strCommand == "@@equipchaifen")
			{//装备拆分
				g_pControl->PopupWindow(MSG_CTRL_EQUIPCHAIFEN_WND,OPER_RECREATE);
			}
			else if (strCommand == "@@chilianflush")
			{//赤炼护符刷新
				g_pControl->PopupWindow(MSG_CTRL_HUFUFLUSH_WND,OPER_RECREATE);
			}

		}
		else
		{
			g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
			if(strCommand == "@exit")
				g_pControl->Msg(MSG_CTRL_NPCWND,OPER_CLOSE);
		}
	}

    //m_pMarkViewer->DoCommand();
    return true;
}

bool CNpcWnd::OnRightButtonDown(int iX,int iY)
{
	if (m_iNpcHeadTex != 0 && iY < 170)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_iNpcHeadTex,EP_UI);
		if (pTex && !pTex->IsPointInTex(iX,iY))
		{
			return false;
		}
	}

	m_bRBClick = true;

	//记录按下窗口时的屏幕位置
	m_iChildX = m_iScreenX;
	m_iChildY = m_iScreenY;

	return true;
}

bool CNpcWnd::OnRightButtonUp(int iX,int iY)
{
	if (m_iNpcHeadTex != 0 && iY < 170)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_iNpcHeadTex,EP_UI);
		if (pTex && !pTex->IsPointInTex(iX,iY))
		{
			return false;
		}
	}

	m_bRBClick = false;

	MoveChildWindow();

	return true;
}

bool CNpcWnd::OnMouseMove(int iX, int iY)
{
	CCtrlWindowX::OnMouseMove(iX,iY);

	if(m_bClick || m_bRBClick)
	{
		MoveChildWindow();
	}
	return true;
}

bool CNpcWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	switch( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pRainbowButton)
			{
				StartRainBowSpirt();
				return true;
			}
		}
		break;

	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


void CNpcWnd::MoveChildWindow()
{
	int deltaX = m_iScreenX-m_iChildX;
	int deltaY = m_iScreenY-m_iChildY;

	if(deltaX == 0 && deltaY == 0)
		return;

	m_iChildX = m_iScreenX;
	m_iChildY = m_iScreenY;

	for(int ii = 0;ii < m_VChildName.size();ii++)
	{
		CCtrlWindow *pWnd = g_pControl->FindWindowByName(m_VChildName[ii].c_str());

		if(pWnd)
		{
			//快捷NPC框,快捷仓库不需要移动
			if(m_VChildName[ii] == "NpcQuickWnd")
			{
				CNpcQuickWnd *pTemp = dynamic_cast<CNpcQuickWnd *>(pWnd);
				if(pTemp && pTemp->GetType() == 2)
					return;
			}

			pWnd->Move(pWnd->GetX()+deltaX,pWnd->GetY()+deltaY);
		}
	}
}

void CNpcWnd::Draw(void)
{

	if(m_iNpcHeadTex != 0)
		DrawTexture(0, 0, m_iNpcHeadTex);

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
}

bool CNpcWnd::OnWheel(int iWheel)
{
	return false;
}