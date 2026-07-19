#include "YuShouLevelUpWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"
#include "GameData/NpcData.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameAI/AIGoodMgr.h"



#define YuShouGood_PosX 300
#define YuShouGood_PosY 343


INIT_WND_POSX(CYuShouLevelUpWnd,POS_AUTO,POS_AUTO)

CYuShouLevelUpWnd::CYuShouLevelUpWnd(void)
{
	SetAll0();

	//以下为新版页签相关
	m_iVersion = 2;
	AddTabPage(0,0,MAKELONG(23215,PACKAGE_INTERFACE),27,36,120,121,122,123,"自身修炼",NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(23216,PACKAGE_INTERFACE),98,36,120,121,122,123,"好友修炼",NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);
}

CYuShouLevelUpWnd::~CYuShouLevelUpWnd(void)
{
	g_NPC.GetHorsemanshipWatchInfo().clear();
	g_NPC.GetOtherHorsemanshipWatchInfo().clear();


	if(g_NPC.GetGoodHorsemanship().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetGoodHorsemanship());
		g_NPC.GetGoodHorsemanship().SetID(0);
	}
}

void CYuShouLevelUpWnd::SetAll0()
{
	m_pStartButton = NULL;
	m_pChanZaButton = NULL;
	m_pTiChunButton = NULL;
	m_pPlayerGrid = NULL;
	m_pMarkViewer = NULL;
}

void CYuShouLevelUpWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	SetCloseButton(496,3,80);

	m_pMarkViewer = new CMarkViewer(16,20,0,0);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,37,105,231-39,416-105);
	m_pMarkViewer->AddScrollEx(182,0,17, 310);
	m_pMarkViewer->SetFont(FONT_YAHEI,FONTSIZE_DEFAULT);
	m_pMarkViewer->SetShow(true);
	m_pMarkViewer->SetEnable(true);

	m_pMarkViewer->SetTagText(&m_kText);

	if (m_TabPage.iCurPage == 0)
	{
		m_pStartButton = new CCtrlButton();
		AddControl(m_pStartButton);
		m_pStartButton->CreateEx(this,380,350,165,166,167,168);

		UpdateBtnState();
	}
	else if (m_TabPage.iCurPage == 1)
	{
		m_pChaButton = new CCtrlButton();
		AddControl(m_pChaButton);
		m_pChaButton->CreateEx(this,253,405,90,91,92);
		m_pChaButton->SetText("查 看",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		m_pChanZaButton = new CCtrlButton();
		AddControl(m_pChanZaButton);
		m_pChanZaButton->CreateEx(this,340,405,90,91,92);
		m_pChanZaButton->SetText("掺杂修炼",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		m_pTiChunButton = new CCtrlButton();
		AddControl(m_pTiChunButton);
		m_pTiChunButton->CreateEx(this,427,405,90,91,92);
		m_pTiChunButton->SetText("提纯修炼",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
		


		m_pPlayerGrid = new CCtrlGrid();
		AddControl(m_pPlayerGrid);
		m_pPlayerGrid->Create(this,265,304,500,392,M_FRIEND_LINE_COUNT,21.0f);
		m_pPlayerGrid->AddScrollEx(218,-2,16,95);
		m_pPlayerGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

		m_pPlayerGrid->SetLinesPerPage(M_FRIEND_LINE_COUNT);
		m_pPlayerGrid->PushColumn(118);
		m_pPlayerGrid->SetDrawOffXY(0,7);		
	}

	

	UpdateFriendList();
	UpdateLog();
}

void CYuShouLevelUpWnd::UpdateBtnState()
{
	if (m_TabPage.iCurPage == 0)
	{
		if (m_pStartButton)
		{
			m_pStartButton->SetShow(true);
			m_pStartButton->SetEnable(true);
			HorsemanshipWatchInfo& hInfo = g_NPC.GetHorsemanshipWatchInfo();
			if (hInfo.m_bFlag == 0)// 未修炼
			{
				m_pStartButton->SetText("开始修炼",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
			}
			else if (hInfo.m_bFlag == 1)//训练中
			{
				m_pStartButton->SetText("提取经验",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
			}
			else if (hInfo.m_bFlag == 2)//已训练
			{
				m_pStartButton->SetText("提取经验",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
			}
			else if (hInfo.m_bFlag == 3)//已领取经验
			{
				m_pStartButton->SetText("开始修炼",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
			}
		}
	}
	
}


void CYuShouLevelUpWnd::DoChaFriend()
{
	int iSelLine = m_pPlayerGrid->GetSelLine();
	if(iSelLine < 0 || iSelLine >= m_vRelation.size())
	{
		g_MsgBoxMgr.PopSimpleAlert("请选择一位好友!");
	}
	else
	{
		g_pGameControl->Send_HorseManShip_Watch_Req(4,m_vRelation[iSelLine].sName.c_str(),0);
	}
}



bool CYuShouLevelUpWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_GRID_DBCLICK:
		{
			if(pControl == m_pPlayerGrid)
			{
				DoChaFriend();
				return true;
			}
		}
		break;
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if (pControl)
			{
				S_TabPage * pParentTabpage = (S_TabPage * )pControl;
				if (pParentTabpage == &m_TabPage)
				{
					g_NPC.GetOtherHorsemanshipWatchInfo().clear();
					UpdateLog();
					return false;
				}
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (m_TabPage.iCurPage == 0)
			{
				if (pControl == m_pStartButton)
				{
					HorsemanshipWatchInfo& hInfo = g_NPC.GetHorsemanshipWatchInfo();
					if (hInfo.m_bFlag == 0 || hInfo.m_bFlag == 3)// 未修炼
					{
						g_pGameControl->Send_HorseManShip_Watch_Req(0,SELF.GetName(),g_NPC.GetGoodHorsemanship().GetID());
					}
					else if (hInfo.m_bFlag == 1)//训练中
					{
						g_MsgBoxMgr.PopOkCancelBox("现在领取经验将会损失部分修炼经验，是否继续？\n（注意：每天只能领取一次骑术修炼经验）",MSG_CTRL_YUSHOULEVELUP,0);
					}
					else if (hInfo.m_bFlag == 2)//已训练
					{
						g_MsgBoxMgr.PopOkCancelBox("您已经充分完成了骑术修炼，是否现在领取经验？\n（注意：每日只能领取一次骑术经验，领取后将不会再受到好友的影响）",MSG_CTRL_YUSHOULEVELUP,0);
					}					
					return true;
				}
				
			}
			else if (m_TabPage.iCurPage == 1)
			{
				
				if (pControl == m_pChanZaButton)
				{
					int iSelLine = m_pPlayerGrid->GetSelLine();
					if(iSelLine < 0 || iSelLine >= m_vRelation.size())
					{
						g_MsgBoxMgr.PopSimpleAlert("请选择一位好友!");
						return true;
					}

					HorsemanshipWatchInfo& hInfo = g_NPC.GetHorsemanshipWatchInfo();
					if (hInfo.m_dwActionPoint == 0)
					{
						g_MsgBoxMgr.PopSimpleAlert("你的影响点数为0，不可执行好友修炼影响");
						return true;
					}

					g_NPC.SelFriendName(m_vRelation[iSelLine].sName);
					g_MsgBoxMgr.PopOkCancelBox("是否对你的好友进行掺杂（提纯度-1%）？\n注意：执行此操作可能会同时掺杂你的自身修炼",MSG_CTRL_YUSHOULEVELUP,1);
					return true;
				}
				else if (pControl == m_pTiChunButton)
				{
					int iSelLine = m_pPlayerGrid->GetSelLine();
					if(iSelLine < 0 || iSelLine >= m_vRelation.size())
					{
						g_MsgBoxMgr.PopSimpleAlert("请选择一位好友!");
						return true;
					}

					HorsemanshipWatchInfo& hInfo = g_NPC.GetHorsemanshipWatchInfo();
					if (hInfo.m_dwActionPoint == 0)
					{
						g_MsgBoxMgr.PopSimpleAlert("你的影响点数为0，不可执行好友修炼影响");
						return true;
					}

					g_NPC.SelFriendName(m_vRelation[iSelLine].sName);
					g_MsgBoxMgr.PopOkCancelBox("是否对你的好友进行提纯（提纯度+5%）？\n注意：执行此操作可能会同时提纯你的自身修炼",MSG_CTRL_YUSHOULEVELUP,2);		
					return true;
				}
				else if (pControl == m_pChaButton)
				{
					DoChaFriend();
					return true;
				}
			}
		}
		break;
	case MSG_CTRL_YUSHOULEVELUP_WND:
		{
			if (dwData == 10)
			{
				UpdateFriendList();
				return true;
			}
			else if (dwData == 11)
			{
				UpdateBtnState();
				return true;
			}
			else if (dwData == 12)
			{
				UpdateLog();
				return true;
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CYuShouLevelUpWnd::OnLeftButtonUp( int iX, int iY )
{
	m_bClick = false;

	if (m_TabPage.iCurPage == 0)
	{
		if(iX > YuShouGood_PosX && iX < YuShouGood_PosX + 36 && iY > YuShouGood_PosY && iY < YuShouGood_PosY + 36)
		{
			if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() && CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd)
			{
				CGoodGrid::ReleaseDrop();
				g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
				return true;
			}

			stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

			HorsemanshipWatchInfo& hInfo = g_NPC.GetHorsemanshipWatchInfo();
			if (hInfo.m_bFlag != 0)
			{
				if (GoodFrom.DropGood.GetID())
				{
					CGoodGrid::ReleaseDrop();
					g_MsgBoxMgr.PopSimpleAlert("您已经开始骑术修炼，不可增加骑术修炼书");
					return true;
				}
			}

			if (GoodFrom.DropGood.GetID() && !g_AIGoodMgr.IsQiShuXiuLianBook(GoodFrom.DropGood))
			{
				g_TalkMgr.PushSysTalk("请放入请放入骑术修炼书！");
				return true;
			}

			CGood tmp = GoodFrom.DropGood;
			GoodFrom.DropGood = g_NPC.GetGoodHorsemanship();
			g_NPC.GetGoodHorsemanship() = tmp;
			CGoodGrid::SetDropGoodFrom(GoodFrom);

			return true;
		}
	}

	std::string strCommand = m_pMarkViewer->GetCommand();
	if(!strCommand.empty())
	{
		if(strCommand.substr(0,2) == "@@")
		{
			string strTemp = strCommand.substr(2);
			strTemp += " ";

			g_pControl->Msg(MSG_CTRL_INSERT_TEXT, 1,(CControl*)strTemp.c_str());

			return true;
		}		
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}

bool CYuShouLevelUpWnd::OnLeftButtonDown(int iX, int iY)
{
	m_bClick = true;
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CYuShouLevelUpWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);
	pTip->GetSelfEquipTips()->SetShow(false);

	if (m_TabPage.iCurPage == 0)
	{
		if(iX > YuShouGood_PosX && iX < YuShouGood_PosX + 36 && iY > YuShouGood_PosY && iY < YuShouGood_PosY + 36)
		{
			if(g_NPC.GetGoodHorsemanship().GetID() != 0)
			{
				pTip->Parse(g_NPC.GetGoodHorsemanship());

				int x = m_iScreenX + iX + 10;
				int y = m_iScreenY + iY + 10;
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
				return true;
			}
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CYuShouLevelUpWnd::OnWheel(int iWheel)
{
	int iMouseX, iMouseY;
	g_pControl->GetMouseXY(iMouseX, iMouseY);

	if (iMouseX > m_iScreenX + 35 && iMouseX < m_iScreenX + 232 && iMouseY > m_iScreenY + 100 && iMouseY < m_iScreenY + 416)
	{
		return m_pMarkViewer->OnWheel(iWheel);
	}

	return CCtrlWindowX::OnWheel(iWheel);
}

void CYuShouLevelUpWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 265,m_iScreenY + 10,"骑术修炼",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	LPTexture pTex = NULL;

	char str[256] = {0};

	if (m_TabPage.iCurPage == 0)
	{
		HorsemanshipWatchInfo& hInfo = g_NPC.GetHorsemanshipWatchInfo();


		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23218,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 345,m_iScreenY + 103,pTex);

		if (hInfo.m_bFlag == 1)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23219,EP_UI);
			if (pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 221,m_iScreenY + 35,pTex);
				g_pGfx->SetRenderMode();
			}
		}
		else if (hInfo.m_bFlag == 2)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23220,EP_UI);
			if (pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 218,m_iScreenY + 5,pTex);
				g_pGfx->SetRenderMode();
			}
		}

		g_pFont->DrawText(m_iScreenX+37, m_iScreenY+77, "修炼日志", COLOR_TEXT_SBU_TITLE, FONT_YAHEI);

		g_pFont->DrawText(m_iScreenX+306, m_iScreenY+221, "当前提纯度", COLOR_TEXT_NORMAL, FONT_YAHEI);
		sprintf(str, "%d%%", hInfo.m_wPure);
		g_pFont->DrawText(m_iScreenX+422, m_iScreenY+221, str, 0xFFFF0000, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);

		g_pFont->DrawText(m_iScreenX+270, m_iScreenY+261, "初始提纯度", COLOR_TEXT_SBU_TITLE, FONT_YAHEI);

		if (g_AIGoodMgr.IsQiShuXiuLianBook(g_NPC.GetGoodHorsemanship()))
		{
			sprintf(str, "%d%%", hInfo.m_wBasePure + g_NPC.GetGoodHorsemanship().GetAC());
		}
		else
		{
			sprintf(str, "%d%%", hInfo.m_wBasePure);
		}
		
		g_pFont->DrawText(m_iScreenX+422, m_iScreenY+261, str, 0xFFFFFF00, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);

		g_pFont->DrawText(m_iScreenX+270, m_iScreenY+290, "剩余修炼时间", COLOR_TEXT_SBU_TITLE, FONT_YAHEI);

		if (hInfo.m_bFlag == 1)
		{
			DWORD dwCurTime = GetTickCount();
			DWORD ltime = dwCurTime - hInfo.m_dwTrainClientStartTime + hInfo.m_dwTrainTime * 1000;
			if (ltime > hInfo.m_dwAllTrainTime)
				ltime = hInfo.m_dwAllTrainTime;

			if (ltime <= hInfo.m_dwAllTrainTime)
			{
				float fPercent = (float)((double)ltime / (double)hInfo.m_dwAllTrainTime);

				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23217,EP_UI);
				if (pTex)
				{
					
					int iTop = (int)(pTex->GetHeight()* (1.0 - fPercent));

					if (iTop >= 0 && iTop < pTex->GetHeight())
					{
						g_pGfx->DrawPartTexture(m_iScreenX + 456,m_iScreenY + 79,pTex,0,iTop,-1,-1);
					}
				}

				int iShowPercent = (int)(fPercent * 100);
				if (iShowPercent == 100)
					iShowPercent = 99;
				sprintf(str,"%d%%",iShowPercent);
				g_pFont->DrawText(m_iScreenX+468, m_iScreenY+138, str, 0xFFFFCC00, FONT_DEFAULT, FONTSIZE_DEFAULT, DTF_Center);

				ltime = hInfo.m_dwAllTrainTime - ltime;
				ltime /= 1000;
				if (ltime >= 60)
				{
					sprintf(str, "%d:%02d", ltime/3600, (ltime/60)%60);
					g_pFont->DrawText(m_iScreenX+422, m_iScreenY+290, str, 0xFFFFFF00, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);
				}
				else
				{
					g_pFont->DrawText(m_iScreenX+422, m_iScreenY+290, "小于1分钟", 0xFFFFFF00, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);
				}
			}
		}
		else if (hInfo.m_bFlag == 2)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23217,EP_UI);
			if (pTex)
				g_pGfx->DrawTextureNL(m_iScreenX + 456,m_iScreenY + 79,pTex);

			g_pFont->DrawText(m_iScreenX+468, m_iScreenY+138, "100%", 0xFFFFCC00, FONT_DEFAULT, FONTSIZE_DEFAULT, DTF_Center);

			g_pFont->DrawText(m_iScreenX+422, m_iScreenY+290, "无", 0xFFFFFF00, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX+422, m_iScreenY+290, "无", 0xFFFFFF00, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);
		}
		
		

		g_pFont->DrawText(m_iScreenX+273, m_iScreenY+390, "放入骑术修炼书", COLOR_TEXT_NORMAL, FONT_YAHEI);

		//g_pGfx->DrawFillRect(m_iScreenX + YuShouGood_PosX,m_iScreenY + YuShouGood_PosX,33,34,0x8000ff00);
		if(g_NPC.GetGoodHorsemanship().GetID()!=0)
			CGoodGrid::DrawOneGood(m_iScreenX+YuShouGood_PosX+17, m_iScreenY+YuShouGood_PosY+17, g_NPC.GetGoodHorsemanship());


		
	}
	else if (m_TabPage.iCurPage == 1)
	{
		HorsemanshipWatchInfo& hInfo = g_NPC.GetHorsemanshipWatchInfo();
		HorsemanshipWatchInfo& hOtherInfo = g_NPC.GetOtherHorsemanshipWatchInfo();

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23218,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 345,m_iScreenY + 103,pTex);

		if (hOtherInfo.m_bFlag == 1)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23219,EP_UI);
			if (pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 221,m_iScreenY + 35,pTex);
				g_pGfx->SetRenderMode();
			}
		}
		else if (hOtherInfo.m_bFlag == 2)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23220,EP_UI);
			if (pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 218,m_iScreenY + 5,pTex);
				g_pGfx->SetRenderMode();
			}
		}

		g_pFont->DrawText(m_iScreenX+37, m_iScreenY+77, "修炼日志", COLOR_TEXT_SBU_TITLE, FONT_YAHEI);

		g_pFont->DrawText(m_iScreenX+306, m_iScreenY+221, "好友当前提纯度", COLOR_TEXT_NORMAL, FONT_YAHEI);
		sprintf(str, "%d%%", hOtherInfo.m_wPure);
		g_pFont->DrawText(m_iScreenX+425, m_iScreenY+221, str, 0xFFFF0000, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);

		g_pFont->DrawText(m_iScreenX+290, m_iScreenY+256, "剩余行动点数", COLOR_TEXT_SBU_TITLE, FONT_YAHEI);
		sprintf(str, "%u", hInfo.m_dwActionPoint);
		g_pFont->DrawText(m_iScreenX+425, m_iScreenY+256, str, 0xFFFF0000, FONT_YAHEI, FONTSIZE_DEFAULT, DTF_Center);

		g_pFont->DrawText(m_iScreenX+264, m_iScreenY+284, "好友列表", COLOR_TEXT_SBU_TITLE, FONT_YAHEI);


		DrawFriendList();
	}
}

void CYuShouLevelUpWnd::DrawFriendList()
{
	size_t friendNum = m_vRelation.size();
	int pos = m_pPlayerGrid->GetScrollPos();
	char strtemp[256];

	DWORD dwColor = COLOR_TEXT_NORMAL;

	for(size_t i = 0; i < M_FRIEND_LINE_COUNT && i + pos < friendNum; i ++)
	{
		_SRelation& srl = m_vRelation[i+pos];

		if(srl.iOnline != 1)	
			dwColor = COLOR_TEXT_DISABLE;
		else dwColor = COLOR_TEXT_NORMAL;

		sprintf(strtemp,"%s",srl.sName.c_str());
		m_pPlayerGrid->DrawGrid(i,0,strtemp,false,dwColor);		
	}
}

void CYuShouLevelUpWnd::UpdateFriendList()
{
	if (m_TabPage.iCurPage == 1)
	{
		m_vRelation.clear();

		DWORD iRelation = RT_FRIEND;

		int iSize = g_OtherData.GetRelationVector().size();
		for(int i = 0;i< iSize;i++)
		{
			_RelationStruct& rl = g_OtherData.GetRelationVector()[i];

			if((rl.iRelType&iRelation)!=0)
			{
				_SRelation srl;
				srl.sName = rl.strName;
				srl.iOnline = rl.iOnline;

				if(rl.iOnline == 1)
					m_vRelation.insert(m_vRelation.begin(),srl);
				else m_vRelation.push_back(srl);
			}
		}

		m_pPlayerGrid->SetTotalCount(m_vRelation.size());
	}
}

void CYuShouLevelUpWnd::UpdateLog()
{
	HorsemanshipWatchInfo* pInfo = NULL;

	if (m_TabPage.iCurPage == 0)
	{
		pInfo = &g_NPC.GetHorsemanshipWatchInfo();
	}
	else if (m_TabPage.iCurPage == 1)
	{
		pInfo = &g_NPC.GetOtherHorsemanshipWatchInfo();

		int iSelLine = m_pPlayerGrid->GetSelLine();
		if(iSelLine < 0 || iSelLine >= m_vRelation.size() || (strcmp(m_vRelation[iSelLine].sName.c_str(), pInfo->m_szPlayerName) != 0))
		{
			m_kText.Clear();

			if (m_pMarkViewer)
			{
				m_pMarkViewer->SetTagText(&m_kText);
			}

			return;
		}		
	}


	std::string lTexts;
	char str[512] = {0};
	string strDataTime;

	std::vector<HmsWatchEventProtocal>& lEvents = pInfo->m_event;
	for (int i = 0; i < lEvents.size(); ++i)
	{
		memset(str, 0, 512);

		HmsWatchEventProtocal& lEvent = lEvents[i];

		if (strDataTime.empty() || strcmp(strDataTime.c_str(), g_Timer.GetDateTime("%Y年%m月%d日",lEvent.ActiveTime)) != 0 )
		{
			strDataTime = g_Timer.GetDateTime("%Y年%m月%d日",lEvent.ActiveTime);
			sprintf(str, "<color=0xFFB48C5A -----%s------>\\", strDataTime.c_str());
			lTexts += str;
		}

		if (lEvent.Type == 0)
		{
			sprintf(str, "<color=0xFFB48C5A %s 开始进行骑术修炼>", g_Timer.GetDateTime("%H:%M",lEvent.ActiveTime));
		}
		else if (lEvent.Type == 1)
		{
			if (strcmp(lEvent.ActivePlayerName, pInfo->m_szPlayerName) == 0)
			{
				sprintf(str, "<color=0xFFB48C5A %s 由于对好友的影响，骑术修>\\<color=0xFFB48C5A 炼提纯><color=green +%d%%>"
					, g_Timer.GetDateTime("%H:%M",lEvent.ActiveTime), lEvent.Value);
			}
			else if(strcmp(lEvent.ToPlayerName, pInfo->m_szPlayerName) == 0)
			{
				sprintf(str, "<color=0xFFB48C5A %s 受到好友><%s/@@%s><color=0xFFB48C5A 的影响，>\\<color=0xFFB48C5A 骑术修炼提纯><color=green +%d%%>"
					, g_Timer.GetDateTime("%H:%M",lEvent.ActiveTime), lEvent.ActivePlayerName, lEvent.ActivePlayerName, lEvent.Value);
			}
		}
		else if (lEvent.Type == 2)
		{
			if (strcmp(lEvent.ActivePlayerName, pInfo->m_szPlayerName) == 0)
			{
				sprintf(str, "<color=0xFFB48C5A %s 由于对好友的影响，骑术修>\\<color=0xFFB48C5A 炼掺杂><color=red -%d%%>"
					, g_Timer.GetDateTime("%H:%M",lEvent.ActiveTime), lEvent.Value);
			}
			else if(strcmp(lEvent.ToPlayerName, pInfo->m_szPlayerName) == 0)
			{
				sprintf(str, "<color=0xFFB48C5A %s 受到好友><%s/@@%s><color=0xFFB48C5A 的影响，>\\<color=0xFFB48C5A 骑术修炼掺杂><color=red -%d%%>"
					, g_Timer.GetDateTime("%H:%M",lEvent.ActiveTime), lEvent.ActivePlayerName, lEvent.ActivePlayerName, lEvent.Value);				
			}
		}
		else if (lEvent.Type == 3 || lEvent.Type == 4)
		{
			sprintf(str, "<color=0xFFB48C5A %s 完成骑术修炼>", g_Timer.GetDateTime("%H:%M",lEvent.ActiveTime) );
		}		

		lTexts += str;
		if (i != lEvents.size()-1)
			lTexts += "\\";
	}

	m_kText.Parse(lTexts,0,"\\",lTexts.length());

	if (m_pMarkViewer)
	{
		m_pMarkViewer->SetTagText(&m_kText);

		int iDisLine = m_kText.GetRow() - 20;
		if (iDisLine < 0)
			iDisLine = 0;

		m_pMarkViewer->SetScrollPos(iDisLine);
	}
	
}
