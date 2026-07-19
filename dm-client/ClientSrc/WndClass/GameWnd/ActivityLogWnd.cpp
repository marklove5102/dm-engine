#include "ActivityLogWnd.h"
#include "GameData/ItemCfgMgr.h"
#include "GameControl/GameControl.h"
#include <stdio.h>
#include <fstream>
#include "Global/StringUtil.h"
#include <algorithm> 
#include "GameData/ConfigData.h"
#include "Global/Interface/StreamInterface.h"
#include "GameAI/AIConfigMgr.h"
#include "GameData/GameData.h"
#include "GameData/XmlCfg.h"

INIT_WND_POSX(CActivityLogWnd,POS_AUTO,POS_AUTO)
CActivityLogWnd::CActivityLogWnd(void)
{
	m_pMVContent = NULL;
	m_pMVPrize = NULL;
	m_pMVTheme = NULL;
	m_pMVNPC   = NULL;
	m_pGrid = NULL;
	m_pAutoShow = NULL;

	m_wActivityID = 0;
	//m_iIndex = 17636;

	//以下为新版页签相关
	m_iVersion = 2;
	AddTabPage(0,0,MAKELONG(23011,PACKAGE_INTERFACE),9,37,85,86,87,88,"今日活跃",NULL,true,0,0,1);
	AddTabPage(0,0,MAKELONG(23010,PACKAGE_INTERFACE),9,122,85,86,87,88,"活动日历",NULL,true,0,0,1);

}

CActivityLogWnd::~CActivityLogWnd(void)
{
}

void CActivityLogWnd::Draw(void)
{
	CCtrlWindowS::Draw();


	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iCurPage = m_TabPage.iCurPage;
	//当前时间
	__time32_t t_time;
	struct tm *newtime = NULL;
	
	_time32(&t_time);
	t_time += g_dwServerTime;
	newtime = _localtime32(&t_time);
	if (!newtime)
	{
		return;
	}
	
	char ctemp[128] = {0};	
	int iDay = newtime->tm_wday;	
	string str[7] = {"日","一","二","三","四","五","六"};

	if (iCurPage == 0)
	{
		//ThemeInfo* pTheme = g_PromptInfoMgr.GetGameThemeInfo(iDay);
		//if (pTheme) 
		//{

		//	g_pFont->DrawText(m_iScreenX + 404,m_iScreenY + 41,str[iDay].c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,14);

		//	sprintf(ctemp,"%s",pTheme->strSec.c_str());
		//	g_pFont->DrawText(m_iScreenX + 580,m_iScreenY + 41,ctemp,0xffb48c5a,FONT_YAHEI,14);
		//}
		
		g_pFont->DrawText(m_iScreenX + 404,m_iScreenY + 41,str[iDay].c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,14);

		strftime(ctemp,128," %Y    %m    %d             %H:%M:%S",newtime);
		g_pFont->DrawText(m_iScreenX + 217,m_iScreenY + 41,ctemp,COLOR_BTN_PRESS,FONT_YAHEI,14);

		g_pFont->DrawText(m_iScreenX + 368,m_iScreenY + 12,"今日活跃",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
		g_pFont->DrawText(m_iScreenX + 70 ,m_iScreenY + 116," 时间      任务名称      NPC       等级    活跃度    状态",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 164,m_iScreenY + 41,"当前时间      年    月    日  星期 ",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);	
		
		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 62,"    每天完成以下任务可以获得活跃度，活跃度达到50、80后可以领取大量经验，活跃度达到100可以抽取一次",0xFFFF0000,FONT_YAHEI,14);	
		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 78,"大奖，活跃度每天0点清零。",0xFFFF0000,FONT_YAHEI,14);	
		

		g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 126,"活动NPC:",0xffb48c5a,FONT_YAHEI,14);
		//g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 151,"双击左侧NPC或者点击下划线",0xffb48c5a,FONT_YAHEI,14);
		//g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 168,"文字可以自动前往",0xffb48c5a,FONT_YAHEI,14);

		DWORD dwActiveRate = SELF.GetActiveRate();
		sprintf(ctemp,"目前活跃度: %d",dwActiveRate);
		g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 215,ctemp,0xFFFF0000,FONT_YAHEI,14);
		int iStars = SELF.GetActiveRate() / 10;
		for (int i = 0;i < 10; i++)
		{
			if (i < iStars)
			{
				g_pFont->DrawText(m_iScreenX + 530 + i * 14,m_iScreenY + 235,"★",0xFFFF0000,FONT_YAHEI,14);
			}
			else 
			{
				g_pFont->DrawText(m_iScreenX + 530 + i * 14,m_iScreenY + 235,"☆",0xFFFF0000,FONT_YAHEI,14);
			}
		}


		g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 278,"活跃度	   奖励经验",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 544,m_iScreenY + 300,"50        经验",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 544,m_iScreenY + 326,"80      大量经验",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 540,m_iScreenY + 352,"100	选择一张卡牌抽取大奖",0xFFFF0000,FONT_YAHEI,14);

		//g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 378,"最高奖励：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		//g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 398,"最高可能获得千万经验、40万真火、",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,12);
		//g_pFont->DrawText(m_iScreenX + 530,m_iScreenY + 414,"金砖等",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,12);


		if (m_pGrid)
		{
			int iPos = m_pGrid->GetScrollPos();
			int iDrawLines = 15;

			VActivityRate& vActivity = g_XmlCfg.GetActivityRateVector();
			if (iDrawLines > vActivity.size() - iPos) 
			{
				iDrawLines = vActivity.size() - iPos;
			}

			DWORD dwState = SELF.GetPrizeBit();
			m_pGetPrize1->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
			if ((dwState & 0x01) || dwActiveRate < 50)
			{
				if (dwState & 0x01)
				{
					m_pGetPrize1->SetText("已领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
				}
				m_pGetPrize1->SetEnable(false);
			}
			else
			{
				m_pGetPrize1->SetEnable(true);
			}

			m_pGetPrize2->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
			if ((dwState & 0x02) || dwActiveRate < 80)
			{
				if (dwState & 0x02)
				{
					m_pGetPrize2->SetText("已领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
				}
				m_pGetPrize2->SetEnable(false);
			}
			else
			{
				m_pGetPrize2->SetEnable(true);
			}

			if ((dwState & 0x04) || dwActiveRate < 100)
			{
				m_pPrize1->SetEnable(false);
				m_pPrize2->SetEnable(false);
				m_pPrize3->SetEnable(false);
				m_pPrize4->SetEnable(false);

				if ((dwState & 0x04))
				{
					g_pFont->DrawText(m_iScreenX + 538,m_iScreenY + 440,"已经领取",0xFFFF0000,FONT_YAHEI,14);
				}
			}
			else
			{
				m_pPrize1->SetEnable(true);
				m_pPrize2->SetEnable(true);
				m_pPrize3->SetEnable(true);
				m_pPrize4->SetEnable(true);
			}


			for (int i = iPos;i < iPos + iDrawLines;i++)
			{
				S_ActivityRate& act = vActivity.at(i);

				m_pGrid->DrawGrid(i - iPos,0,act.strTime.c_str(),true,0xffb48c5a);
				m_pGrid->DrawGrid(i - iPos,1,act.strName.c_str(),true,0xffb48c5a);
				m_pGrid->DrawGrid(i - iPos,2,act.strNPCName.c_str(),true,0xffb48c5a);
				m_pGrid->DrawGrid(i - iPos,3,act.strNeedLevel.c_str(),true,0xffb48c5a);
				m_pGrid->DrawGrid(i - iPos,4,act.strAward.c_str(),true,0xffb48c5a);

				if (dwState & (1 << (i + 3)))//前面3个是领奖情况
				{
					m_pGrid->DrawGrid(i - iPos,5,"完成",true,0xffb48c5a);
				}
				else
				{
					m_pGrid->DrawGrid(i - iPos,5,"未完成",true,0xff646464);
				}				
			}

			g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 462,"\"Ctrl+N\"", 0xff87c23a,FONT_YAHEI,FONTSIZE_DEFAULT);

		}
	}
	else if (iCurPage == 1)
	{
		ThemeInfo* pTheme = g_PromptInfoMgr.GetGameThemeInfo(iDay);
		if (pTheme) 
		{
			strftime(ctemp,128," %Y    %m    %d             %H:%M:%S",newtime);
			g_pFont->DrawText(m_iScreenX + 217,m_iScreenY + 41,ctemp,COLOR_BTN_PRESS,FONT_YAHEI,14);

			g_pFont->DrawText(m_iScreenX + 404,m_iScreenY + 41,str[iDay].c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,14);

			sprintf(ctemp,"%s",pTheme->strSec.c_str());
			g_pFont->DrawText(m_iScreenX + 580,m_iScreenY + 41,ctemp,0xffb48c5a,FONT_YAHEI,14);
		}

		g_pFont->DrawText(m_iScreenX + 368,m_iScreenY + 12,"活动日历",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
		g_pFont->DrawText(m_iScreenX + 80 ,m_iScreenY + 116,"活动时间        活动名称      等级     完成次数     状态                详细介绍",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 164,m_iScreenY + 41,"当前时间      年    月    日  星期 ",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);	
		g_pFont->DrawText(m_iScreenX + 490,m_iScreenY + 41,"今日主题活动",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);

		g_pFont->DrawText(m_iScreenX + 548,m_iScreenY + 148,"活动NPC",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);
		g_pFont->DrawText(m_iScreenX + 603,m_iScreenY + 381,"活动奖励",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,14);

		if (m_pGrid)
		{
			int iPos = m_pGrid->GetScrollPos();
			int iDrawLines = 15;
			std::vector<_ActivityInfo>& vActivity = g_PromptInfoMgr.GetActivityInfo();
			if (iDrawLines > vActivity.size() - iPos) iDrawLines = vActivity.size() - iPos;

			for (int i = iPos;i<iPos + iDrawLines;i++)
			{
				_ActivityInfo& act = vActivity.at(i);

				m_pGrid->DrawGrid(i - iPos,0,act.strTime.c_str(),true,0xffb48c5a);
				m_pGrid->DrawGrid(i - iPos,1,act.strName.c_str(),true,0xffb48c5a);
				m_pGrid->DrawGrid(i - iPos,2,act.strConfine.c_str(),true,0xffb48c5a);
				m_pGrid->DrawGrid(i - iPos,3,act.strFinishNum.c_str(),true,0xffb48c5a);

				string str;
				float fTm1,fTm2;
				DWORD dwColor = 0;
				string strTm = act.strTime;
				if (strTm.compare("全天")== 0)
				{
					dwColor = 0xffffe650;
					str = "进行中";
				}
				else
				{
					std::replace(strTm.begin(),strTm.end(),':','.');
					sscanf(strTm.c_str(),"%f-%f",&fTm1,&fTm2);
					float fTm = newtime->tm_hour + (float)newtime->tm_min/(float)100;


					if (fTm >= fTm1 && fTm <= fTm2)
					{
						dwColor = 0xffffe650;
						str = "进行中";
					}
					else if (fTm > fTm2)
					{
						dwColor = 0xff87c23a;
						str = "已结束";
					}
					else
					{
						dwColor = 0xff646464;
						str = "未开始";
					}
				}

				m_pGrid->DrawGrid(i - iPos,4,str.c_str(),true,dwColor);
			}
		}
	}


}

bool CActivityLogWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	int iCurPage = m_TabPage.iCurPage;
	switch (dwMsg)
	{
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			WORD wRow = LOWORD(dwData);
			if (iCurPage == 0)
			{
				VActivityRate& vActivity = g_XmlCfg.GetActivityRateVector();
				if (wRow < vActivity.size()) 
				{
					m_TagNPC.Parse(vActivity[wRow].strNpcCommand,0,"\\",-1,0xFFb48c5a);
					m_TagContent.Parse(vActivity[wRow].strTips,0,"\\",-1,0xFFb48c5a);
				}
			}
			else if (iCurPage == 1)
			{
				std::vector<_ActivityInfo>& vAct = g_PromptInfoMgr.GetActivityInfo();
				if (wRow < vAct.size())
				{
					_ActivityInfo& act = vAct.at(wRow);
					m_TagContent.Clear();
					m_TagContent.Parse(act.strNPCCont,0,"\\",-1,0xFFb48c5a);

					m_TagPrize.Clear();
					m_wActivityID = (WORD)act.dwID - 1;
					m_TagPrize.Parse(act.strAward,0,"\\",-1,0xFFb48c5a);

					m_TagNPC.Clear();
					m_TagNPC.Parse(act.strNPCName,0,"\\",-1,0xFFb48c5a);
				}
			}
		}
		break;
	case MSG_CTRL_GRID_DBCLICK:                //CtrlGrid双击某行某列,dwData低word为行号,高word为列号
		{
			WORD wRow = LOWORD(dwData);
			if (iCurPage == 0)
			{
				VActivityRate& vActivity = g_XmlCfg.GetActivityRateVector();
				if (wRow < vActivity.size()) 
				{
					string strCommand = m_TagNPC.GetCommand(0,0);
					g_pGameControl->DealGotoCommand(strCommand);
				}
			}
			else if (iCurPage == 1)
			{
				std::vector<_ActivityInfo>& vAct = g_PromptInfoMgr.GetActivityInfo();
				if (wRow < vAct.size())
				{
					string strCommand = m_TagNPC.GetCommand(0,0);
					g_pGameControl->DealGotoCommand(strCommand);
				}
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pAutoShow)
			{
				bool bAutoShow = !m_pAutoShow->IsChecked();
				g_AICfgMgr.SetAutoShowActLog(bAutoShow);
			}
			else if (pControl == m_pGetPrize1)
			{
				g_pGameControl->SEND_Activerate_Prize(0,0);
			}
			else if (pControl == m_pGetPrize2)
			{
				g_pGameControl->SEND_Activerate_Prize(1,0);
			}
			else if (pControl == m_pPrize1)
			{
				g_pGameControl->SEND_Activerate_Prize(2,0);
			}
			else if (pControl == m_pPrize2)
			{
				g_pGameControl->SEND_Activerate_Prize(2,1);
			}
			else if (pControl == m_pPrize3)
			{
				g_pGameControl->SEND_Activerate_Prize(2,2);
			}
			else if (pControl == m_pPrize4)
			{
				g_pGameControl->SEND_Activerate_Prize(2,3);
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowS::Msg(dwMsg,dwData,pControl);
}

void CActivityLogWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	g_pControl->MsgToWnd(MSG_CTRL_ACTIVITY_LOG_WND,MSG_CTRL_FLASH_BTN_WND,1001);

	//获得今天主题
	SetCloseButton(734,4,80);

	int iCurPage = m_TabPage.iCurPage;
	m_pMVTheme = NULL;
	m_pMVNPC = NULL;
	m_pMVContent = NULL;
	m_pMVPrize = NULL;
	m_pGrid = NULL;
	m_pAutoShow = NULL;
	m_pGetPrize1 = m_pGetPrize2 = m_pPrize1 = m_pPrize2 = m_pPrize3 = m_pPrize4 = NULL;

	if(iCurPage == 0)
	{
		m_pGetPrize1 = new CCtrlButton();
		AddControl(m_pGetPrize1);
		m_pGetPrize1->CreateEx(this, 663,295, 90, 91, 92, 93);
		m_pGetPrize1->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	
		m_pGetPrize2 = new CCtrlButton();
		AddControl(m_pGetPrize2);
		m_pGetPrize2->CreateEx(this, 663,321, 90, 91, 92, 93);
		m_pGetPrize2->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pPrize1 = new CCtrlButton();
		AddControl(m_pPrize1);
		m_pPrize1->CreateEx(this, 538,384, 23014, 23015, 23016, 23017);
		m_pPrize1->SetText("大量&经验", 0xFFFF0000 ,0xFFFF0000, 0xFFFF0000 ,0xFF000000, FONTSIZE_SMALL,0,FONT_YAHEI);
	
		m_pPrize2 = new CCtrlButton();
		AddControl(m_pPrize2);
		m_pPrize2->CreateEx(this, 584,384, 23014, 23015, 23016, 23017);
		m_pPrize2->SetText("大量&真火", 0xFFFF0000 ,0xFFFF0000, 0xFFFF0000 ,0xFF000000, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pPrize3 = new CCtrlButton();
		AddControl(m_pPrize3);
		m_pPrize3->CreateEx(this, 630,384, 23014, 23015, 23016, 23017);
		m_pPrize3->SetText("绑定&元宝", 0xFFFF0000 ,0xFFFF0000, 0xFFFF0000 ,0xFF000000, FONTSIZE_SMALL,0,FONT_YAHEI);
	
		m_pPrize4 = new CCtrlButton();
		AddControl(m_pPrize4);
		m_pPrize4->CreateEx(this, 676,384, 23014, 23015, 23016, 23017);
		m_pPrize4->SetText("道具&奖励", 0xFFFF0000 ,0xFFFF0000, 0xFFFF0000 ,0xFF000000, FONTSIZE_SMALL,0,FONT_YAHEI);
	
	
		m_pGrid = new CCtrlGrid;
		m_pGrid->CreateEx(this,47,141,515,457,15,21,17637);
		AddControl(m_pGrid);
		m_pGrid->SetDrawOffXY(0,-3);
		m_pGrid->SetFont(FONT_YAHEI,12);
		m_pGrid->SetTotalCount(g_XmlCfg.GetActivityRateVector().size());

		if (g_PromptInfoMgr.GetActivityInfo().size() > 0)
			m_pGrid->SetSelLine(0);

		m_pGrid->PushColumn(70);
		m_pGrid->PushColumn(108);
		m_pGrid->PushColumn(64);
		m_pGrid->PushColumn(64);
		m_pGrid->PushColumn(63);
		m_pGrid->PushColumn(63);


		VActivityRate& vActivity = g_XmlCfg.GetActivityRateVector();
		if (vActivity.size() > 0) 
		{
			m_pMVNPC = new CMarkViewer(16,1);
			m_pMVNPC->Create(this,613,126,735 - 613,141 - 126);
			AddControl(m_pMVNPC);
			m_pMVNPC->SetTagText(&m_TagNPC);
			m_TagNPC.Parse(vActivity[0].strNpcCommand,0,"\\",-1,0xFFb48c5a);
			m_pMVNPC->SetFont(FONT_YAHEI);


			m_pMVContent = new CMarkViewer(16,15);
			m_pMVContent->Create(this,529,148,734 - 529,201 - 148);
			AddControl(m_pMVContent);
			m_TagContent.Parse(vActivity[0].strTips,0,"\\",-1,0xFFb48c5a);
			m_pMVContent->SetTagText(&m_TagContent);
			m_pMVContent->SetFont(FONT_YAHEI);
		}


		m_pAutoShow = new CCtrlRadio();
		AddControl(m_pAutoShow);
		m_pAutoShow->Create(this, 75, 458, 125, 126, 127, 128, 340, 20);
		m_pAutoShow->SetText("选中后下次将不再自动弹出，可通过         打开今日活跃界面", COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT, 0, FONT_YAHEI);
		m_pAutoShow->SetChecked(!g_AICfgMgr.IsAutoShowActLog());	

	
	}
	else if (iCurPage == 1)
	{
		int iDay = GetWeekDay();
		ThemeInfo* pTheme = g_PromptInfoMgr.GetGameThemeInfo(iDay);
		if (pTheme)
		{
			m_pMVTheme = new CMarkViewer(16,2);
			m_pMVTheme->Create(this,55,64,695,37);
			AddControl(m_pMVTheme);
			string str = pTheme->strTheme;
			m_TagTheme.Parse(str,0,"\\",114,0xFFb48c5a);
			m_pMVTheme->SetTagText(&m_TagTheme);
			m_pMVTheme->SetFont(FONT_YAHEI);
		}

		_ActivityInfo* pActivity = g_PromptInfoMgr.GetActivityInfo((DWORD)0);
		if (pActivity)
		{
			m_pMVNPC = new CMarkViewer(16,1);
			m_pMVNPC->Create(this,608,148,120,18);
			AddControl(m_pMVNPC);
			m_pMVNPC->SetTagText(&m_TagNPC);
			m_TagNPC.Parse(pActivity->strNPCName,0,"\\",-1,0xFFb48c5a);
			m_pMVNPC->SetFont(FONT_YAHEI);

			m_pMVContent = new CMarkViewer(16,15);
			m_pMVContent->Create(this,545,172,194,198);
			AddControl(m_pMVContent);
			m_TagContent.Parse(pActivity->strNPCCont,0,"\\",-1,0xFFb48c5a);
			m_pMVContent->SetTagText(&m_TagContent);
			m_pMVContent->SetFont(FONT_YAHEI);

			m_pMVPrize = new CMarkViewer(16,3);
			m_pMVPrize->Create(this,545,399,185,68);
			AddControl(m_pMVPrize);
			m_TagPrize.Parse(pActivity->strAward,0,"\\",-1,0xFFb48c5a);
			m_pMVPrize->SetTagText(&m_TagPrize);
			m_pMVPrize->SetFont(FONT_YAHEI);
		}

		m_pGrid = new CCtrlGrid;
		m_pGrid->CreateEx(this,47,141,515,457,15,21,17637);
		AddControl(m_pGrid);
		m_pGrid->SetDrawOffXY(0,-3);
		m_pGrid->SetFont(FONT_YAHEI,12);
		m_pGrid->SetTotalCount(g_PromptInfoMgr.GetActivityInfo().size());

		if (g_PromptInfoMgr.GetActivityInfo().size() > 0)
			m_pGrid->SetSelLine(0);

		m_pGrid->PushColumn(110);
		m_pGrid->PushColumn(108);
		m_pGrid->PushColumn(64);
		m_pGrid->PushColumn(93);
		m_pGrid->PushColumn(63);
	}

}

bool CActivityLogWnd::OnLeftButtonDown(int iX, int iY)
{
	return CCtrlWindowS::OnLeftButtonDown(iX,iY);
}

bool CActivityLogWnd::OnLeftButtonUp(int iX, int iY)
{
	string strCommand = "";
	if (m_pMVTheme) strCommand = m_pMVTheme->GetCommand();
	
	if (strCommand.empty() && m_pMVNPC) strCommand = m_pMVNPC->GetCommand();
	
	m_bClick = false;//否则后面弹出的模式框获得不到焦点
	bool bRet = g_pGameControl->DealGotoCommand(strCommand);
	if (bRet) return true;

	return CCtrlWindowS::OnLeftButtonUp(iX,iY);
}

bool CActivityLogWnd::OnMouseMove(int iX,int iY)
{
	return CCtrlWindowS::OnMouseMove(iX,iY);
}

void CActivityLogWnd::OnSetFocus()
{

}

void CActivityLogWnd::OnKillFocus()
{

}

bool CActivityLogWnd::OnWheel(int iWheel)
{
	if (m_pGrid)
		return m_pGrid->OnWheel(iWheel);

	return CCtrlWindowS::OnWheel(iWheel);
}

int CActivityLogWnd::GetWeekDay()
{
	struct tm *newtime = NULL;
	__time32_t t_time;
	_time32(&t_time);
	t_time += g_dwServerTime;
	int iDay = 0;
	newtime = _localtime32(&t_time);
	if(newtime)
		iDay = newtime->tm_wday;

	if(iDay < 0 || iDay >= 7)
		iDay = 0;

	return iDay;
}
