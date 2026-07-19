#include "NpcRecruit.h"
#include "GameControl/GameControl.h"
#include <Shellapi.h>
#include "GameData/NpcData.h"
#include "Global/Interface/AudioInterface.h"

INIT_WND_POSX(CNpcRecruit,POS_AUTO,POS_AUTO)

CNpcRecruit::CNpcRecruit(void)
{
	int iType = (int)sm_dwWindowType;

	g_pControl->Msg(MSG_CTRL_NPCWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_TRADEWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCBOOK_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_LUXNPCWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_YUANBAO_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_RELATION_WND,OPER_CUSTOM);

	for (int i = 0; i < NPCRECRUIT_MAX_ROWS; i++)
	{
		m_pCommitBtn[i] = NULL;
	}


	m_pPrePage = m_pNextPage = NULL;
	m_pApply = NULL;

	m_iVersion = 2;

	S_TabPage* pPageMentor = AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),0,35,85,86,87,88,"师 徒",NULL,true,2,0,1);

	AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),47, 34,115,116,117, 118,"拜 师",pPageMentor,false, 0, 0, 1);
	AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),100, 34,115,116,117, 118,"收 徒",pPageMentor,false, 0, 0, 1);

	S_TabPage* pPageClan = AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),0,120,85,86,87,88,"宗 族",NULL,true,2,0,1);

	AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),41, 34,120,121,122, 123,"加入宗族",pPageClan,false, 0, 0, 1);
	AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),115, 34,120,121,122, 123,"招募弟子",pPageClan,false, 0, 0, 1);

	//S_TabPage* pPageGuild = AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),0,205,85,86,87,88,"行 会",NULL,true,2,0,1);

	//AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),41, 34,120,121,122, 123,"加入行会",pPageGuild,false, 0, 0, 1);
	//AddTabPage(33,0,MAKELONG(17690,PACKAGE_INTERFACE),115, 34,120,121,122, 123,"招募成员",pPageGuild,false, 0, 0, 1);

	m_TabPage.iCurPage = iType / 2;
	if (m_TabPage.iCurPage>1)
	{
		m_TabPage.iCurPage = 1;
	}
	m_TabPage.vSubTabPage[m_TabPage.iCurPage].iCurPage = iType % 2;
}

CNpcRecruit::~CNpcRecruit(void)
{

}

void CNpcRecruit::OnClickCloseButton()
{
	g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@exit");
	CCtrlWindowX::OnClickCloseButton();
}

void CNpcRecruit::OnCreate()
{
	//string str = "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//str += "123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\123   22  --  abcd  efgh 2009.5  </@123>\\";
	//g_TagTextMgr.GetNpcText().Parse(str);

	CCtrlWindowX::OnCreate();

	//SetPageRect(0,20,59,50,118);
	//SetPageRect(1,20,129,50,181);
	//SetPageRect(2,20,191,50,244);

	//数据显示框
	m_pMarkViewer = new CMarkViewer(21,NPCRECRUIT_MAX_ROWS);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,50,84,360,270);
	m_pMarkViewer->SetTagText(&g_TagTextMgr.GetNpcText());
	m_pMarkViewer->SetDefaultColor(COLOR_TEXT_NORMAL);

	char* pStr[] = {"拜 师", "收 徒", "加 入", "招 募", "申请拜师", "申请收徒", "申请加入", "申请招募"};

	m_iPage = m_TabPage.iCurPage;
	m_iSubPage = m_TabPage.vSubTabPage[m_TabPage.iCurPage].iCurPage;

	//按钮
	int iRow = g_TagTextMgr.GetNpcText().GetRow();
	int iStartTexID = 2;//加入
	if(m_iPage == 0)
	{
		if(m_iSubPage == 0)
			iStartTexID = 0;//拜师
		else
			iStartTexID = 1;//收徒
	}
	else if(m_iPage == 1)
	{
		if(m_iSubPage == 0)
			iStartTexID = 2;//加入
		else
			iStartTexID = 3;//招募
	}
	else if(m_iPage == 2)
	{
		if(m_iSubPage == 0)
			iStartTexID = 2;//加入
		else
			iStartTexID = 3;//招募
	}

	for (int i = 0; i < iRow - 2 && i < NPCRECRUIT_MAX_ROWS; i++)//最后一行是隐藏命令,倒数第二行是上一页,下页
	{
		m_pCommitBtn[i] = new CCtrlButton();
		AddControl(m_pCommitBtn[i]);
		m_pCommitBtn[i]->CreateEx(this,528,82 + i*21,95,96,97,98);
		m_pCommitBtn[i]->SetText(pStr[iStartTexID]);
	}

	//上一页,下一页
	m_pPrePage = new CCtrlButton();
	AddControl(m_pPrePage);
	m_pPrePage->CreateEx(this,220,358,95,96,97,98);
	m_pPrePage->SetText("上一页");
	m_pPrePage->SetEnable(false);
	m_pNextPage = new CCtrlButton();
	AddControl(m_pNextPage);
	m_pNextPage->CreateEx(this,356,358,95,96,97,98);
	m_pNextPage->SetText("下一页");
	m_pNextPage->SetEnable(false);

	if(iRow >= 2)//最后一行是隐藏命令,倒数第二行是上一页,下页
	{
		int iCols = g_TagTextMgr.GetNpcText().GetColumn(iRow - 2);
		if(iCols >= 2)
		{
			string strCommand = g_TagTextMgr.GetNpcText().GetCommand(iRow - 2,0);
			if(strCommand.length() > 1)//脚本用一个空格表示有上一页或下一页
			{
				m_pPrePage->SetEnable(true);
			}
			strCommand = g_TagTextMgr.GetNpcText().GetCommand(iRow - 2,1);
			if(strCommand.length() > 1)
			{
				m_pNextPage->SetEnable(true);
			}
		}
	}
	//中间按钮
	iStartTexID =  0;
	if (m_iPage == 0 && m_iSubPage == 0)
	{
		iStartTexID = 5;
	}
	else if (m_iPage == 1 && m_iSubPage == 0)
	{
		iStartTexID = 7;
	}
	else if (m_iPage == 2 && m_iSubPage == 0)
	{
		iStartTexID = 7;
	}

	if(iStartTexID != 0)
	{
		m_pApply = new CCtrlButton();
		AddControl(m_pApply);
		m_pApply->CreateEx(this, 280,385, 90,91,92,93);
		m_pApply->SetText(pStr[iStartTexID], COLOR_BTN_NORMAL, COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE);
	}

	//SetMask(16370);
	SetCloseButton(586,2, 80);
}

bool CNpcRecruit::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	switch( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			int iRow = g_TagTextMgr.GetNpcText().GetRow();
			for (int i = 0; i < NPCRECRUIT_MAX_ROWS; i++)
			{
				if(pControl == m_pCommitBtn[i])
				{
					if(i < iRow)
					{
						int iCols = g_TagTextMgr.GetNpcText().GetColumn(i);
						if(iCols > 0)
						{
							string strCommand = g_TagTextMgr.GetNpcText().GetCommand(i,iCols - 1);
							g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
						}
					}

					return true;
				}
			}	

			if(pControl == m_pPrePage)
			{
				if(iRow >= 2)
				{
					int iCols = g_TagTextMgr.GetNpcText().GetColumn(iRow - 2);
					if(iCols > 0)
					{
						string strCommand = g_TagTextMgr.GetNpcText().GetCommand(iRow - 2,0);
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
					}
				}

				return true;
			}

			if(pControl == m_pNextPage)
			{
				if(iRow >= 2)
				{
					int iCols = g_TagTextMgr.GetNpcText().GetColumn(iRow - 2);
					if(iCols > 1)
					{
						string strCommand = g_TagTextMgr.GetNpcText().GetCommand(iRow - 2,1);
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
					}
				}

				return true;
			}

			if (pControl == m_pApply)
			{
				if(m_iPage == 0 && m_iSubPage == 0)
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_teachrecruit_request");
				else if(m_iPage == 1 && m_iSubPage == 0)
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_clanrecruit_request");
				else if(m_iPage == 2 && m_iSubPage == 0)
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_guildrecruit_request");

				return true;
			}

		}
		break;

	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if(pControl == (CControl*)&m_TabPage && dwData != m_TabPage.iCurPage)
			{
				if(dwData == 0)
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_teachrecruit_viewall");
				else if(dwData == 1)
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_clanrecruit_viewall");
				else if(dwData == 2)
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_guildrecruit_viewall");
			}

			if(pControl != (CControl*)&m_TabPage)
			{
				if(pControl == (CControl*)&(m_TabPage.vSubTabPage[0]))
				{
					if(dwData == 0)
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_teachrecruit_viewall");
					else
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_teachrecruit_recruitchoice");
				}
				else if(pControl == (CControl*)&(m_TabPage.vSubTabPage[1]))
				{
					if(dwData == 0)
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_clanrecruit_viewall");
					else
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_clanrecruit_recruitchoice");
				}
				//else if(pControl == (CControl*)&(m_TabPage.vSubTabPage[2]))
				//{
				//	if(dwData == 0)
				//		g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_guildrecruit_viewall");
				//	else
				//		g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_guildrecruit_recruitchoice");
				//}
			}
		}
		break;

	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


bool CNpcRecruit::OnLeftButtonUp(int iX, int iY)
{
	string strCommand = m_pMarkViewer->GetCommand();
	if(!strCommand.empty())
	{
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
			else if(strCommand.substr(2,3)=="url")
			{
				ShellExecute(NULL,"open","iexplore.exe",strCommand.substr(5).c_str(),NULL,SW_SHOW);
			}
		}
		else
		{
			g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
			if(strCommand == "@exit")
				g_pControl->Msg(MSG_CTRL_NPCRECRUIT,OPER_CLOSE);
		}

		return true;
	}

	return CControl::OnLeftButtonUp(iX,iY);
}

void CNpcRecruit::Draw()
{
	CCtrlWindowS::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX+296, m_iScreenY+9, "招  募", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);

	if(m_TabPage.iCurPage == 0)
	{
		g_pFont->DrawText(m_iScreenX+86, m_iScreenY+61, "名字       等级         宗族          行会         申请日期", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	} 
	else if(m_TabPage.iCurPage == 1)
	{
		if(m_TabPage.vSubTabPage[m_TabPage.iCurPage].iCurPage == 1)
			g_pFont->DrawText(m_iScreenX+86, m_iScreenY+61, "名字             等级               行会           申请日期", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
		else
			g_pFont->DrawText(m_iScreenX+86, m_iScreenY+61, "宗族       宗主         等级        隶属行会       申请日期", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	}
	//else
	//{
	//	if(m_TabPage.vSubTabPage[m_TabPage.iCurPage].iCurPage == 1)
	//		g_pFont->DrawText(m_iScreenX+86, m_iScreenY+61, "名字             等级               宗族           申请日期", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	//	else
	//		g_pFont->DrawText(m_iScreenX+86, m_iScreenY+61, "行会             会长               等级           申请日期", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	//}
}
