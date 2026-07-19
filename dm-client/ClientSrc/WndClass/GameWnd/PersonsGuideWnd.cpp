#include "personsguidewnd.h"
#include "GameControl\GameControl.h"
#include "GameMap/MinMap.h"
#include "GameMap/GameMap.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Control/CtrlMenuWnd.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/GameData.h"
#include "GameData/MapArray.h"
#include "GameData/GuildData.h"
#include "GameData/OtherData.h"
#include <algorithm>

 
INIT_WND_POSX(CPersonsGuideWnd,POS_AUTO,POS_AUTO)

//int CPersonsGuideWnd::m_iLastStayPage = 0;

//用来排序的比较函数
bool SPersonsGreater ( S_Persons &elem1, S_Persons &elem2 )
{
	return elem1.strName < elem2.strName;
}
//用来排序的比较函数
bool SNpcGreater ( S_Npc &elem1, S_Npc &elem2 )
{
	return elem1.strName < elem2.strName;
}

CPersonsGuideWnd::CPersonsGuideWnd(void)
{
	//m_iCurPage = 0;
	m_dwLastRefrashPersonsData = 0;

	m_iIndex = 13230;
	m_iPages = 1;

	//m_iVersion = 2;
	//AddTabPage(0,0,MAKELONG(13230,PACKAGE_INTERFACE),21,36,120,121,122,123,"附近玩家",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	//AddTabPage(0,0,MAKELONG(13230,PACKAGE_INTERFACE),99,36,120,121,122,123,"NPC",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);

	//m_TabPage.iCurPage = m_iLastStayPage;
}	

CPersonsGuideWnd::~CPersonsGuideWnd(void)
{
	//m_iLastStayPage = m_TabPage.iCurPage;
	g_pControl->PopupWindow(MSG_CTRL_MINMAP_WND,OPER_CLOSE);
}

void CPersonsGuideWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//m_iCurPage = m_TabPage.iCurPage;

	g_pFont->DrawText(m_iScreenX + 130,m_iScreenY + 10,"自动寻路",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	DrawPersonsLineData();

	DrawNpcLineData();

	DrawMonsterLineData();
}

void CPersonsGuideWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	//m_iCurPage = m_TabPage.iCurPage;
	//列表控件
	m_pPlayerGrid = new CCtrlGrid();
	AddControl(m_pPlayerGrid);
	m_pPlayerGrid->Create(this,25,188,245,298,PERSONS_LINE_COUNT,21.0f);
	m_pPlayerGrid->AddScrollEx(202,0,16,110);
	m_pPlayerGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

	m_pPlayerGrid->SetLinesPerPage(PERSONS_LINE_COUNT);
	m_pPlayerGrid->PushColumn(118);
	m_pPlayerGrid->PushColumn(110);
	m_pPlayerGrid->SetDrawOffXY(0,7);

	m_pNPCGrid = new CCtrlGrid();
	AddControl(m_pNPCGrid);
	m_pNPCGrid->Create(this,25,57,245,166,PERSONS_LINE_COUNT,21.0f);
	m_pNPCGrid->AddScrollEx(202,0,16,110);
	m_pNPCGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

	m_pNPCGrid->SetLinesPerPage(PERSONS_LINE_COUNT);
	m_pNPCGrid->PushColumn(140);
	m_pNPCGrid->PushColumn(110);
	m_pNPCGrid->SetDrawOffXY(0,7);

	m_pMonsterGrid = new CCtrlGrid();
	AddControl(m_pMonsterGrid);
	m_pMonsterGrid->Create(this,25,323,245,413,PERSONS_LINE_COUNT,21.0f);
	m_pMonsterGrid->AddScrollEx(202,0,16,91);
	m_pMonsterGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

	m_pMonsterGrid->SetLinesPerPage(MONSTERS_LINE_COUNT);
	m_pMonsterGrid->PushColumn(140);
	m_pMonsterGrid->PushColumn(110);
	m_pMonsterGrid->SetDrawOffXY(0,7);

	//人脉
	m_pSociety1 = new CCtrlButton();
	AddControl(m_pSociety1);
	m_pSociety1->CreateEx(this,50,418,95,96,97,98);
	m_pSociety1->SetText("人脉", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	
	//加为好友
	m_pAddFriend = new CCtrlButton();
	AddControl(m_pAddFriend);
	m_pAddFriend->CreateEx(this,160,418,95,96,97,98);
	m_pAddFriend->SetText("加好友", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);


	SetCloseButton(235,3,80);	
		
		
	//更新Persons数据
	CreatePersonsMap();		

	if(m_VNpc.size() == 0)//没有导入Npc数据
	{
		LocalmapInfo_s* pAreaMap = g_pMinMap->GetLocalMap(g_pGameMap->GetMapName());
		if(pAreaMap)
		{
			S_Npc npc;
			vector<NpcInfo_s> & VNpc = pAreaMap->VNpc;
			int iSize = VNpc.size();
			for(int i = 0;i < iSize;i++)
			{
				npc.pt.x = VNpc[i].iX;
				npc.pt.y = VNpc[i].iY;
				npc.strName = VNpc[i].strNpcName;
				npc.strDesp = VNpc[i].strDesc;

				m_VNpc.push_back(npc);
			}

			//sort(m_VNpc.begin(), m_VNpc.end(), SNpcGreater);
		}
	}

	int iRange = (int)m_VNpc.size() - PERSONS_LINE_COUNT;
	if(iRange < 0)
		iRange = 0;

	m_pNPCGrid->SetTotalCount((int)m_VNpc.size());

	vector<MonsterInfo_s>* pMapMonster = g_pMinMap->GetLocalMonster(g_pGameMap->GetMapName());

	if(pMapMonster)
	{
		for(int i = 0;i < pMapMonster->size();i++)
		{
			S_Monsters monster;

			monster.strName = (*pMapMonster)[i].strName;
			monster.strLevel = (*pMapMonster)[i].strLevel;
			monster.bYouHuo = (*pMapMonster)[i].bYouHuo;

			m_vMonsters.push_back(monster);
		}
	}

	m_pMonsterGrid->SetTotalCount((int)m_vMonsters.size());	
}	

bool CPersonsGuideWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_GRID_RCLICK_LINE_COL:
		{
			OnClickRow();
		}
		break;
	case MSG_CTRL_GRID_DBCLICK:
		{
			//如果是NPC页面，双击时自动寻路到该NPC处
			if(pControl == m_pNPCGrid)
			{
				OnMoveToPlace();
			}
			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pSociety1)
			{
				g_pControl->PopupWindow(MSG_CTRL_RELATION_WND,OPER_CREATE);
				return true;
			}			
			else if(pControl == m_pAddFriend)//加为好友
			{
				OnFriend();
				return true;
			}


		}
		break;
	case MSG_CTRL_POP_MENU:
		{
			int iSelLine = m_pPlayerGrid->GetSelLine();
			if(iSelLine < 0 || iSelLine > m_VPersons.size())
			{
				g_MsgBoxMgr.PopSimpleAlert("请先选中一个玩家。");
				return true;
			}

			switch(dwData)
			{
			case POP_MENU_ITEM_TEAM:
				OnTeam();
				break;
			case POP_MENU_ITEM_TRADE:
				OnTrade();
				break;
			case POP_MENU_ITEM_WATCH_EQUIP:
				OnWatchEquip();
				break;
			case POP_MENU_ITEM_CHATSECRET:
				OnChatSecret();
				break;
			case POP_MENU_ITEM_FRIEND:
				OnFriend();
				break;
			case POP_MENU_ITEM_GUILD:
				OnTakeInGuild();
				break;
			case POP_MENU_ITEM_TAKEIN_PRENTICE:
				OnTakeinPrentice();
				break;
			case POP_MENU_ITEM_WATCH_BOOTH:
				OnWatchBooth();
				break;
			case POP_MENU_ITEM_COPYNAME:
				OnCopyName();
				break;
			case POP_MENU_ITEM_TAKEIN_MASTER:
				OnTakeInMaster();
				break;
			case POP_MENU_ITEM_PHYLE:
				OnTakeInPhyle();
				break;
			//case POP_MENU_ITEM_INVITE_TNEUP_FREE:
			//case POP_MENU_ITEM_INVITE_TNEUP_AREA:
			//case POP_MENU_ITEM_APPLY_TNEUP_FREE:
			//case POP_MENU_ITEM_APPLY_TNEUP_AREA:
			//	OnTneup(dwData);
			//	break;


			default:
				break;
			}

			return true;
		}
		break;	
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CPersonsGuideWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();

	int iHotLine = m_pNPCGrid->GetHotLine();	

	if(m_pNPCGrid->IsMouseOn(m_iScreenX + iX,m_iScreenY + iY) && iHotLine >= 0 && iHotLine < m_VNpc.size())
	{
		pTip->Clear();
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		
		if(m_VNpc[iHotLine].strDesp.length() > 0)
			pTip->AddText(m_VNpc[iHotLine].strDesp.c_str(),0xFFFFFF00);

		pTip->AddText("双击该NPC,自动寻路到该NPC处",0xFFFFFF00);
		
		return true;
	}

	iHotLine = m_pPlayerGrid->GetHotLine();

	if(m_pPlayerGrid->IsMouseOn(m_iScreenX + iX,m_iScreenY + iY) && iHotLine >= 0 && iHotLine < m_VPersons.size())
	{

		if(g_pControl->GetMenuWnd()->GetCaller() != this)
		{
			pTip->Clear();
			pTip->AddText("点击右键弹出操作菜单",0xFFFFFF00);			
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}		
	}

	iHotLine = m_pMonsterGrid->GetHotLine();

	if(m_pMonsterGrid->IsMouseOn(m_iScreenX + iX,m_iScreenY + iY) && iHotLine >= 0 && iHotLine < m_vMonsters.size())
	{
		if(m_vMonsters[iHotLine].bYouHuo)
		{
			pTip->Clear();
			pTip->AddText("有概率被法师诱惑，成功率与玩家等级相关。",0xFFFFFF00);			
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}
	
	pTip->SetShow(false);

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CPersonsGuideWnd::OnLeftButtonUp(int iX, int iY)
{
	return CCtrlWindowX::OnLeftButtonUp(iX, iY);
}

void CPersonsGuideWnd::OnClickRow()
{
	int row = m_pPlayerGrid->GetSelLine();
	if(row >= 0 && g_pControl->GetFocusCtrl() == m_pPlayerGrid)
	{
		CCtrlMenuWnd* pMenuWnd = g_pControl->GetMenuWnd();
		pMenuWnd->Clear();

		S_POP_MENU_DATA & PopMenuData = pMenuWnd->GetData();
		//PopMenuData.wBackTex = 13240;
		//PopMenuData.fItemHeight = 18.2f;
		PopMenuData.fItemHeight = 16.0f;
		PopMenuData.iW = 128;

		pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHATSECRET,"密聊");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_FRIEND,"加为好友");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_TEAM,"邀请组队");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_GUILD,"邀请加入行会");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_TAKEIN_PRENTICE,"招收徒弟");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_TRADE,"交易");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_WATCH_BOOTH,"查看摊位");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_WATCH_EQUIP,"查看装备");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_COPYNAME,"复制人名");
		pMenuWnd->AddMenuItem(POP_MENU_ITEM_TAKEIN_MASTER,"请求拜师");

		pMenuWnd->AddMenuItem(POP_MENU_ITEM_PHYLE,"邀请加入宗族");

// 		int iSelLine = m_pPlayerGrid->GetSelLine();
// 		CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
// 		if(pChar)
// 		{
// 			int nNum = g_pGameData->GetTroopMembers();
// 			if (nNum > 0 && g_TrusteeshipData.IsTeamTrusteeship()) //组队中并且队长佩戴了阵谱
// 			{
// 				if (strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) == 0)//自己是队长
// 				{
// 					if(g_TrusteeshipData.FindMemberByID(pChar->GetID()) == NULL)//对方未托管
// 					{
// 						pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_FREE, "邀请加入自由群英模式");
// 						pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_AREA, "邀请加入安全群英模式");
// 					}
// 				}
// 				else//不是队长
// 				{
// 					if ((strcmp(g_pGameData->GetMemberName(0), pChar->GetName()) == 0) && (!g_TrusteeshipData.IsTrusteeship()))//对方是队长并且自己未托管
// 					{
// 						pMenuWnd->AddMenuItem(POP_MENU_ITEM_APPLY_TNEUP_FREE, "申请加入自由群英模式");
// 						pMenuWnd->AddMenuItem(POP_MENU_ITEM_APPLY_TNEUP_AREA, "申请加入安全群英模式");
// 					}
// 				}
// 			}
// 			else if (SELF.GetEquipGood(ITEM_TNEUPCHART).GetID()!=0)//放宽条件，只要自己佩戴了阵谱，未组队也能邀请托管
// 			{
// 				pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_FREE, "邀请加入自由群英模式");
// 				pMenuWnd->AddMenuItem(POP_MENU_ITEM_INVITE_TNEUP_AREA, "邀请加入安全群英模式");
// 			}
// 		}


		m_bClick = false;
		m_bRBClick = false;//否则MenuWnd获得不到焦点
		pMenuWnd->SetCaller(this);
		pMenuWnd->ShowMenu();
		g_pControl->GetTipWnd()->SetShow(false);		
	}

    
}

void CPersonsGuideWnd::RefrashPersonsData()
{
	DWORD dwCount = GetTickCount();

	if(dwCount - m_dwLastRefrashPersonsData < 500) //每500ms更新一下数据
		return;

	m_dwLastRefrashPersonsData = dwCount;

	MiniChar& _chars = MapArray.GetMiniChar();
	MiniChar::iterator it;
	int iPersons = 0;
	bool bNeedRefrash = false;

	int iSize = m_VOldPersonsID.size();
	for(it = _chars.begin(); it != _chars.end(); it++)
	{
		CSimpleCharacter* pChar = it->second;
		if(NULL == pChar || !pChar->IsHuman()) 
			continue;

		if(iPersons >= iSize || m_VOldPersonsID[iPersons] != pChar->GetID())
		{
			bNeedRefrash = true;
			break;
		}

		iPersons++;
	}

	if(iSize != iPersons || bNeedRefrash)
	{
		CreatePersonsMap();
	}
}

void CPersonsGuideWnd::DrawPersonsLineData()
{
	g_pFont->DrawText(m_iScreenX + 110,m_iScreenY + 170,"附近玩家",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
	//g_pFont->DrawText(m_iScreenX + 194,m_iScreenY + 61,"关系",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_SMALL);

	RefrashPersonsData();

	size_t personNum = m_VPersons.size();
	int pos = m_pPlayerGrid->GetScrollPos();
	int iLine = 0;

	for(size_t i = 0; iLine < PERSONS_LINE_COUNT && i + pos < personNum; i ++)
	{
		CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[i+pos].dwID);
		if(!pChar)
			continue;

		DWORD dwColor = 0xFFFF8000;
		if (pChar->GetCareer() == 1)
		{
			dwColor = 0xFF5AA0A0;
		}
		else if(pChar->GetCareer() == 2)
		{
			dwColor = 0xFF87C23A;
		}

		m_pPlayerGrid->DrawGrid(iLine,0,m_VPersons[iLine+pos].strName.c_str(),false,dwColor);
		string str;	

		DWORD dwRelation = pChar->GetRelationType();
		if( (dwRelation & RT_WIFE) != 0)
			str += "妻子 ";
		else if( (dwRelation & RT_HUSBAND) != 0)
			str += "丈夫 ";

		if( (dwRelation & RT_PRENTICE) != 0)
			str += "徒弟 ";
		else if( (dwRelation & RT_MASTER) != 0)
			str += "师父 ";

		if( (dwRelation & RT_FRIEND) != 0)
			str += "好友 ";

		if( (dwRelation & RT_GUILD) != 0)
			str += "行会 ";

		if((dwRelation & RT_GROUP) != 0 && str.length() < 19)//如果前面的关系都有了不显示队友关系，否则放不下
			str += "队友 ";

		if(str.length() == 0)
			str = "陌生人";

		int iOffX = (19-max(str.length(),14))*6;

		m_pPlayerGrid->DrawGrid(iLine,1,str.c_str(),false,dwColor);
		iLine ++;
	}
}

bool CPersonsGuideWnd::OnWheel(int iWheel)
{
	return CCtrlWindowX::OnWheel(iWheel);
}

void CPersonsGuideWnd::DrawNpcLineData()
{
	g_pFont->DrawText(m_iScreenX + 110,m_iScreenY + 40,"附近NPC",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
	//g_pFont->DrawText(m_iScreenX + 194,m_iScreenY + 61,"坐标",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_SMALL);

	size_t npcNum = m_VNpc.size();
	int pos = m_pNPCGrid->GetScrollPos();

	//先画选中效果底框
	for(size_t i = 0; i < PERSONS_LINE_COUNT && i + pos < npcNum; i ++)
	{
		string szTemp = m_VNpc[i+pos].strDesp;
		if (szTemp.length() > 18)
		{
			StringUtil::AutoCut(szTemp,18);
			szTemp += "...";
		}
		if (szTemp.length() > 0)
		{
			szTemp = m_VNpc[i+pos].strName + "(" + szTemp + ")";
			m_pNPCGrid->DrawGrid(i,0,szTemp.c_str());
		}
		else
		{
			m_pNPCGrid->DrawGrid(i,0,m_VNpc[i+pos].strName.c_str());
		}

		//sprintf(strtemp,"%d,%d",m_VNpc[i+pos].pt.x,m_VNpc[i+pos].pt.y);
		//m_pNPCGrid->DrawGrid(i,1,strtemp);
	}
}


void CPersonsGuideWnd::OnMoveToPlace()
{
	/*if(m_iCurPage != 1)
		return;*/

	int iSelLine = m_pNPCGrid->GetSelLine();
	if( iSelLine >= 0 && iSelLine < m_VNpc.size())
	{
		//<老兵/@@goto mapName npcName 20,20>\\";
		//strCommand = @@goto mapName npcName 20,20

		char strTemp[256];
		sprintf(strTemp,"@@goto %s %s %d,%d",g_pGameMap->GetMapName(),m_VNpc[iSelLine].strName.c_str(),m_VNpc[iSelLine].pt.x,m_VNpc[iSelLine].pt.y);
		string strCommand = strTemp; 

		g_pGameControl->DealGotoNpcCommand(strCommand);

	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("请先选中一个NPC。");
	}

}

void CPersonsGuideWnd::OnTeam()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();

	if( g_pGameData->GetTroopMembers() > 0 && 
		(strcmp(g_pGameData->GetMemberName(0),SELF.GetName()) != 0))//组队了但不是队长，立即反回
	{
		g_MsgBoxMgr.PopSimpleAlert("你已经在队伍中，不是队长不可以邀请其它玩家组队。");
		return;
	}
	else
	{
		if( g_pGameData->GetTroopMembers() <= 0 ) //未组队
		{
			g_pGameControl->SEND_Group_Create( m_VPersons[iSelLine].strName.c_str() ); 
		}
		else//组队了是队长
		{
			g_pGameControl->SEND_Group_Add( m_VPersons[iSelLine].strName.c_str() );
		}
	}
}

void CPersonsGuideWnd::OnTrade()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
	if(!pChar)
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里，无法交易。");
	}

	g_pGameControl->SEND_Player_Trade(0,pChar->GetID());
}

void CPersonsGuideWnd::OnWatchEquip()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
	if(pChar)
	{
		int x,y;
		pChar->GetRealXY(x,y);
		g_pGameControl->SEND_Look_Player_Info(pChar->GetID(),x,y,pChar->GetDir()); 
	}
}

void CPersonsGuideWnd::OnChatSecret()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();

	if(iSelLine >= 0)
	{
		string strTemp = m_VPersons[iSelLine].strName;
		strTemp += " ";

		g_pControl->Msg(MSG_CTRL_INSERT_TEXT, 1,(CControl*)strTemp.c_str());
	}
}

void CPersonsGuideWnd::OnFriend()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	if(iSelLine < 0)
		return;

	g_pGameControl->SEND_Add_Friend(m_VPersons[iSelLine].strName.c_str());
}

void CPersonsGuideWnd::OnTakeInGuild()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	if(strlen(SELF.GetTitle()) <= 0 || (!g_GuildData.IsHeader() && (g_GuildData.GetMember().size() > 0  || g_GuildData.GetGuildMemberOfficer().size() > 0)) )
	{
		g_MsgBoxMgr.PopSimpleAlert("只有行会会长才能邀请其它玩家加入行会。");
	}
	else
	{
		g_pGameControl->SEND_Guild_Add_Member(m_VPersons[iSelLine].strName.c_str());
	}
}

void CPersonsGuideWnd::OnTakeinPrentice()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
	if(!pChar)
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里,无法进行收徒操作。");
		return;
	}

	g_pGameControl->SEND_Master_Prentice_Request(pChar->GetID(),1);
}

void CPersonsGuideWnd::OnTakeInMaster()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
	if(!pChar)
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里,无法进行拜师操作。");
		return;
	}

	g_pGameControl->SEND_Master_Prentice_Request(pChar->GetID(),2);
}

void CPersonsGuideWnd::OnWatchBooth()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
	if(pChar)
	{
		if(pChar->GetBoothState())
			g_pGameControl->SEND_Exchange_Goin(pChar->GetID());
		else
		{
			g_MsgBoxMgr.PopSimpleAlert("该玩家没有摆摊。");
		}
	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里,无法查看其摊位。");
	}

}

void CPersonsGuideWnd::OnCopyName()
{
	if(!IsDataValid())
		return;

	HGLOBAL hClip = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,MAX_EDIT_WORDS);
	if(!hClip)
		return;

	char *temp = (char*)GlobalLock(hClip);
	if(!temp)
	{
		GlobalUnlock(hClip);
		return;
	}

	int iSelLine = m_pPlayerGrid->GetSelLine();
	sprintf(temp,"%s",m_VPersons[iSelLine].strName.c_str());
	GlobalUnlock(hClip);

	if(!OpenClipboard(NULL))
		return;

	if(!EmptyClipboard())
		return;

	if(SetClipboardData( CF_TEXT,hClip) == NULL )
		return;

	CloseClipboard();
}

void CPersonsGuideWnd::CreatePersonsMap()
{
	/*if(m_iCurPage != 0)
		return;*/

	int iSelLine = m_pPlayerGrid->GetSelLine();
	DWORD dwSelID = 0;//选中了的人物ＩＤ
	if(iSelLine >= m_VPersons.size())
	{
		iSelLine = -1;
		m_pPlayerGrid->SetSelLine(-1);
	}

	if(iSelLine >= 0)
		dwSelID = m_VPersons[iSelLine].dwID;

	m_VPersons.clear();
	m_VOldPersonsID.clear();
	char temp_str[1024] ={0};
	MiniChar& _chars = MapArray.GetMiniChar();
	MiniChar::iterator it;
	S_Persons person;
	for(it = _chars.begin(); it != _chars.end(); it++)
	{
		CSimpleCharacter* pChar = it->second;
		if(NULL == pChar || !pChar->IsHuman()) 
			continue;

		person.strName = pChar->GetName();
		person.dwID = pChar->GetID();
		m_VPersons.push_back(person);
		m_VOldPersonsID.push_back(person.dwID);
	}

	int iPos = m_pPlayerGrid->GetSelLine();
	sort(m_VPersons.begin(), m_VPersons.end(), SPersonsGreater);
	size_t personNum = m_VPersons.size();
	m_pPlayerGrid->SetTotalCount((int)personNum);

	int iNewPos = iPos;
	//重新选中刚才选中的人
	int iNewSelLine = -1;
	if(dwSelID != 0)
	{
		size_t i = 0;
		for(; i < personNum; i ++)
		{
			if(dwSelID == m_VPersons[i].dwID)
			{
				iNewSelLine = i;
				if(i < iPos || i >= iPos + PERSONS_LINE_COUNT )
				{
					iNewPos = (int)i;
				}

				break;
			}
		}

		if(i >= personNum)//刚才选中的人已经走了
		{
			m_pPlayerGrid->SetSelLine(-1);
			//如果弹出了菜单则取消菜单
			if(g_pControl->GetMenuWnd()->GetCaller() == this)
			{
				g_pControl->GetMenuWnd()->CloseMenu();
			}
		}
	}

	if(iNewPos > m_pPlayerGrid->GetTotalCount() - PERSONS_LINE_COUNT)
	{
		iNewPos = m_pPlayerGrid->GetTotalCount() - PERSONS_LINE_COUNT;
	}

	if(iNewPos < 0)
		iNewPos = 0;

	m_pPlayerGrid->SetDisLine(iNewPos);
	m_pPlayerGrid->SetSelLine(iNewSelLine);

}

bool CPersonsGuideWnd::IsDataValid()
{
	/*if(m_iCurPage != 0)
		return false;*/

	int iSelLine = m_pPlayerGrid->GetSelLine();
	if(iSelLine >= m_pPlayerGrid->GetTotalCount())
	{
		iSelLine = -1;
		m_pPlayerGrid->SetSelLine(-1);
	}

	if( iSelLine < 0 || iSelLine >= m_VPersons.size())
	{
		g_MsgBoxMgr.PopSimpleAlert("请先选中一个玩家。");	
		return false;
	}

	return true;
}

int CPersonsGuideWnd::getFirstChar(BYTE * strName)
{
	if(!strName)
		return 0;

	if(strName[0] < 0xA0)//英文字母
		return toupper(strName[0]);

	if(strlen((const char *)strName) < 2)
		return ' ';

	WORD wN = MAKEWORD(BYTE(strName[1]),BYTE(strName[0]));

	if ( 0xB0A1<=wN && wN<=0xB0C4 ) return 'A';
	if ( 0XB0C5<=wN && wN<=0XB2C0 ) return 'B';
	if ( 0xB2C1<=wN && wN<=0xB4ED ) return 'C';
	if ( 0xB4EE<=wN && wN<=0xB6E9 ) return 'D';
	if ( 0xB6EA<=wN && wN<=0xB7A1 ) return 'E';
	if ( 0xB7A2<=wN && wN<=0xB8C0 ) return 'F';
	if ( 0xB8C1<=wN && wN<=0xB9FD ) return 'G';
	if ( 0xB9FE<=wN && wN<=0xBBF6 ) return 'H';
	if ( 0xBBF7<=wN && wN<=0xBFA5 ) return 'J';
	if ( 0xBFA6<=wN && wN<=0xC0AB ) return 'K';
	if ( 0xC0AC<=wN && wN<=0xC2E7 ) return 'L';
	if ( 0xC2E8<=wN && wN<=0xC4C2 ) return 'M';
	if ( 0xC4C3<=wN && wN<=0xC5B5 ) return 'N';
	if ( 0xC5B6<=wN && wN<=0xC5BD ) return 'O';
	if ( 0xC5BE<=wN && wN<=0xC6D9 ) return 'P';
	if ( 0xC6DA<=wN && wN<=0xC8BA ) return 'Q';
	if ( 0xC8BB<=wN && wN<=0xC8F5 ) return 'R';
	if ( 0xC8F6<=wN && wN<=0xCBF0 ) return 'S';
	if ( 0xCBFA<=wN && wN<=0xCDD9 ) return 'T';
	if ( 0xCDDA<=wN && wN<=0xCEF3 ) return 'W';
	if ( 0xCEF4<=wN && wN<=0xD188 ) return 'X';
	if ( 0xD1B9<=wN && wN<=0xD4D0 ) return 'Y';
	if ( 0xD4D1<=wN && wN<=0xD7F9 ) return 'Z';
	return ' ';
}

bool CPersonsGuideWnd::OnChar(UCHAR cChar)
{
	CCtrlGrid * m_pGrid = NULL;

	int iReallyCount = 0;		

	if(g_pControl->GetFocusCtrl() == m_pPlayerGrid)
	{
		m_pGrid = m_pPlayerGrid;
		iReallyCount = m_VPersons.size();
	}
	else if(g_pControl->GetFocusCtrl() == m_pNPCGrid)
	{
		m_pGrid = m_pNPCGrid;
		iReallyCount = m_VNpc.size();
	}
	else
	{
		return true;
	}

	int iSelLine = m_pGrid->GetSelLine();
	int iCount = m_pGrid->GetTotalCount();

	int iSerchLine = iSelLine+1;
	if(iSelLine < 0)
		iSelLine = 0;
	if(iSerchLine < 0)
		iSerchLine = 0;	

	if(iSelLine >= iReallyCount || iCount != iReallyCount)
		return true;

	cChar = toupper(cChar);

	for(; iSerchLine < iCount; iSerchLine ++)
	{
		int c = 0;
		if(m_pGrid == m_pPlayerGrid)
			c = getFirstChar((BYTE *)m_VPersons[iSerchLine].strName.c_str());
		else if(m_pGrid == m_pNPCGrid)
			c = getFirstChar((BYTE *)m_VNpc[iSerchLine].strName.c_str());

		if(c == cChar)
			break;
	}

	int iPos = m_pGrid->GetSelLine();
	if(iSerchLine > iSelLine && iSerchLine < iCount)//找到了则选中他
	{
		m_pGrid->SetSelLine(iSerchLine);
		//if(iSerchLine < iPos || iSerchLine >= iPos + PERSONS_LINE_COUNT )
		//{
		//	if(iSerchLine > iCount - PERSONS_LINE_COUNT)
		//		iSerchLine = iCount - PERSONS_LINE_COUNT;

		//	m_pGrid->SetSelLine(iSerchLine);
		//}
	}
	else//到最后都没有找到，再回过头来从开始找
	{
		for(iSerchLine = 0; iSerchLine <= iSelLine; iSerchLine ++)
		{
			int c = 0;
			if(m_pGrid == m_pPlayerGrid)
				c = getFirstChar((BYTE *)m_VPersons[iSerchLine].strName.c_str());
			else if(m_pGrid == m_pNPCGrid)
				c = getFirstChar((BYTE *)m_VNpc[iSerchLine].strName.c_str());

			if(c == cChar)
				break;
		}

		if( iSerchLine <= iSelLine)//找到了则选中他
		{
			m_pGrid->SetSelLine(iSerchLine);
			//if(iSerchLine < iPos || iSerchLine >= iPos + PERSONS_LINE_COUNT)
			//{
			//	if(iSerchLine > iCount - PERSONS_LINE_COUNT)
			//		iSerchLine = iCount - PERSONS_LINE_COUNT;

			//	m_pGrid->SetSelLine(iSerchLine);
			//}
		}
		else//一直没有找到则清空选中项
		{
			m_pGrid->SetSelLine(-1);
			m_pGrid->SetScrollPos(0);
		}

	}

	return true;

	//return CCtrlWindowX::OnChar(cChar);
}

bool CPersonsGuideWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	return CCtrlWindowX::OnKeyDown(wState,cKey);
}

void CPersonsGuideWnd::DrawMonsterLineData()
{
	g_pFont->DrawText(m_iScreenX + 104,m_iScreenY + 304,"本地图怪物",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_SMALL);

	size_t monsterNum = m_vMonsters.size();
	int pos = m_pMonsterGrid->GetScrollPos();
	char strtemp[256];

	for(size_t i = 0; i < MONSTERS_LINE_COUNT && i + pos < monsterNum; i ++)
	{
		sprintf(strtemp,"%s(Lv%s)",m_vMonsters[i+pos].strName.c_str(),m_vMonsters[i+pos].strLevel.c_str());		
		m_pMonsterGrid->DrawGrid(i,0,strtemp);

		if(m_vMonsters[i+pos].bYouHuo)
		{
			int iOffX = strlen(strtemp) * m_pMonsterGrid->GetFontSize() / 2 + 3;
			m_pMonsterGrid->DrawGrid(i,0,PACKAGE_INTERFACE,16298,false,iOffX,2);
		}
	}
}

void CPersonsGuideWnd::OnMove()
{
	CCtrlWindowX::OnMove();

	CControl * pControl = g_pControl->FindWindowByName("MinMapWnd");
	if(g_pControl->GetFocusCtrl() == this && pControl)
	{
		pControl->Move(m_iScreenX - 681,m_iScreenY);
	}	
}

void CPersonsGuideWnd::OnSwitchToTop()
{
	CControl * pControl = g_pControl->FindWindowByName("MinMapWnd");
	if(pControl)
	{
		g_pControl->SwitchToTop(pControl,false);
	}
}

void CPersonsGuideWnd::OnTakeInPhyle()
{
	if(!IsDataValid())
		return;

	int iSelLine = m_pPlayerGrid->GetSelLine();
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
	if(!pChar)
	{
		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里。");
		return;
	}

	if(!g_OtherData.IsPhyleMaster())
	{
		g_MsgBoxMgr.PopSimpleAlert("只有宗族族长才能邀请其它玩家加入宗族。");
	}
	else
	{
		g_pGameControl->SEND_Phyle_Operate(pChar->GetName(),0);		
	}
}

// void CPersonsGuideWnd::OnTneup(DWORD dwData)
// {
// 	if(!IsDataValid())
// 		return;
// 
// 	int iSelLine = m_pPlayerGrid->GetSelLine();
// 	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_VPersons[iSelLine].dwID);
// 	if(!pChar)
// 	{
// 		g_MsgBoxMgr.PopSimpleAlert("该玩家不在你的视野范围里。");
// 		return;
// 	}
// 
// 	switch (dwData)
// 	{
// 	case POP_MENU_ITEM_INVITE_TNEUP_FREE:
// 		g_pGameControl->Send_Trusteeship(pChar->GetName(), 1);
// 		break;
// 	case POP_MENU_ITEM_INVITE_TNEUP_AREA:
// 		g_pGameControl->Send_Trusteeship(pChar->GetName(), 0);
// 		break;
// 	case POP_MENU_ITEM_APPLY_TNEUP_FREE:
// 		g_pGameControl->Send_Trusteeship(NULL,1);
// 		break;
// 	case POP_MENU_ITEM_APPLY_TNEUP_AREA:
// 		g_pGameControl->Send_Trusteeship(NULL, 0);
// 		break;
// 	}
// }


void CPersonsGuideWnd::ResetWidthAndHeight(int iW,int iH)
{
	CCtrlWindowX::ResetWidthAndHeight(iW,iH);

	CControl * pMinMapWnd = g_pControl->FindWindowByName("MinMapWnd");
	if (pMinMapWnd)
	{
		int iX = pMinMapWnd->GetScreenX();
		int iY = pMinMapWnd->GetScreenY();
		int iW = pMinMapWnd->GetWidth();

		Move(iX + iW - 3,iY);
		SetOriginalOffX(iX + iW - 3);
		SetOriginalOffY(iY);
	}
}
