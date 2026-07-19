#include "TaskWuSeDailyWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/TalkMgr.h"
#include "Baseclass/Control/ParserTip.h"

#define WuSeGood_PosX 397
#define WuSeGood_PosY 150

#define WuSeColorTime 300


INIT_WND_POSX(CTaskWuSeDailyWnd,POS_AUTO,POS_AUTO)

CTaskWuSeDailyWnd::CTaskWuSeDailyWnd(void)
{
	m_pBtnFlush = NULL;
	m_pBtnTaskAuto = NULL;
	m_pMarkViewerUp = NULL;
	m_pMarkViewerDown = NULL;

	m_colors[0] = 0xFFFFFFFF;
	m_colors[1] = 0xFF559F00;
	m_colors[2] = 0xFF009AFF;
	m_colors[3] = 0xFF9A00FF;
	m_colors[4] = 0xFFD74500;

	m_ptHole[0].x = 37;
	m_ptHole[0].y = 164;

	m_ptHole[1].x = 74;
	m_ptHole[1].y = 164;

	m_ptHole[2].x = 111;
	m_ptHole[2].y = 164;

	m_ptHole[3].x = 148;
	m_ptHole[3].y = 164;

	m_ptHole[4].x = 185;
	m_ptHole[4].y = 164;

	m_iEnterTime = 0;

	m_iIndex = 21630;	
}

CTaskWuSeDailyWnd::~CTaskWuSeDailyWnd(void)
{
	if(g_NPC.GetWuSeGood().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetWuSeGood());
		g_NPC.GetWuSeGood().SetID(0);
	}
}

void CTaskWuSeDailyWnd::OnCreate()
{
	if (!g_pGameData->HasPaoPaoStatus(EP_First_OpenWuSeTaskWnd_PaoPao))
	{
		AddArrowTip(EP_First_OpenWuSeTaskWnd_PaoPao,237 + 50,158,XAL_TOPLEFT,false,XAL_BOTTOMLEFT);
	}



	SetCloseButton(447,5, 80);

	//0xFF559F00 绿
	//0xFFD74500 橙
	//0xFF009AFF 蓝
	//0xFF9A00FF 紫
	m_pLabTaskName = new CCtrlLabel;
	m_pLabTaskName->Create(this, 237, 151, 137, 22);
	m_pLabTaskName->SetText("五色任务(未接)",0xFF009AFF,0xFF009AFF,0xFF009AFF,0xFF009AFF,12, DTF_UnderLine, DTF_UnderLine, DTF_UnderLine);
	m_pLabTaskName->SetTips("点击领取任务");
	AddControl(m_pLabTaskName);

	m_pBtnFlush = new CCtrlButton;
	m_pBtnFlush->CreateEx(this,234,182,95,96,97,98);
	m_pBtnFlush->SetText("立即刷新",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
	m_pBtnFlush->SetEnable(false);
	AddControl(m_pBtnFlush);

	m_pBtnTaskAuto = new CCtrlButton;
	m_pBtnTaskAuto->CreateEx(this,316,182,95,96,97,98);
	m_pBtnTaskAuto->SetText("领取任务",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
	AddControl(m_pBtnTaskAuto);

	string str;

	m_pMarkViewerUp = new CMarkViewer(20,4);
	AddControl(m_pMarkViewerUp);
	m_pMarkViewerUp->Create(this,30,52,370,80);
	m_pMarkViewerUp->SetTagText(&m_TagUp);
	m_pMarkViewerUp->SetFont(FONT_YAHEI,12);
	//您还有10次刷新任务颜色的机会。\\系统每天免费赠送2次，虎烈银卡可以额外获得2次，龙骧金卡可以额外获得6次。
	//str = "<color=yellow 您还有10次刷新任务颜色的机会。>\\<color=yellow 系统每天免费赠送2次。>\\<color=yellow 虎烈银卡可以额外获得2次，龙骧金卡可以额外获得6次。>";
	//m_TagUp.Parse(str);

	m_pMarkViewerDown = new CMarkViewer(20,6);
	AddControl(m_pMarkViewerDown);
	m_pMarkViewerDown->Create(this,30,225,370,55);
	m_pMarkViewerDown->SetTagText(&m_TagDwon);
	m_pMarkViewerDown->SetFont(FONT_YAHEI,12);
	str =  "<color=ggreen 通过刷新后可以接到不同颜色的任务，获得不同倍数的经验值奖励。>\\";
	str	+= "<color=ggreen 注1：有未完成的五色任务时，不可以刷新任务颜色。>\\";
	str	+= "<color=ggreen 注2：每天最多完成5次五色任务，全部完成后会有额外经验值奖励。>\\";
	str	+= "<color=ggreen 注3：当日未完成的五色任务，次日不可领取经验值奖励。>\\";
	str	+= "<color=ggreen 注4：刷新时，任务颜色将会有概率以白、绿、蓝、紫、橙的顺序依次上升。>\\";
	str	+= "<color=ggreen 注5：刷新任务颜色的机会为0时进行刷新，将会扣除包裹内的转换乾坤。>\\";
	m_TagDwon.Parse(str);


	m_pHole[0] = new CCtrlButton;
	AddControl(m_pHole[0]);
	m_pHole[0]->CreateEx(this,m_ptHole[0].x,m_ptHole[0].y,0,21642,21642,0);
	m_pHole[0]->SetTips("完成白色任务，可获得1倍经验值奖励。");

	m_pHole[1] = new CCtrlButton;
	AddControl(m_pHole[1]);
	m_pHole[1]->CreateEx(this,m_ptHole[1].x,m_ptHole[1].y,0,21643,21643,0);	
	m_pHole[1]->SetTips("完成绿色任务，可获得多倍经验值奖励。");

	m_pHole[2] = new CCtrlButton;
	AddControl(m_pHole[2]);
	m_pHole[2]->CreateEx(this,m_ptHole[2].x,m_ptHole[2].y,0,21644,21644,0);
	m_pHole[2]->SetTips("完成蓝色任务，可获得多倍经验值奖励。");

	m_pHole[3] = new CCtrlButton;
	AddControl(m_pHole[3]);
	m_pHole[3]->CreateEx(this,m_ptHole[3].x,m_ptHole[3].y,0,21645,21645,0);	
	m_pHole[3]->SetTips("完成紫色任务，可获得多倍经验值奖励。");

	m_pHole[4] = new CCtrlButton;
	AddControl(m_pHole[4]);
	m_pHole[4]->CreateEx(this,m_ptHole[4].x,m_ptHole[4].y,0,21646,21646,0);	
	m_pHole[4]->SetTips("完成橙色任务，可获得多倍经验值奖励。");

	m_iEnterTime = 0;	
}

bool CTaskWuSeDailyWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
		case MSG_CTRL_TASKWUSEDAILY_WND:
			{
				if (dwData == 10)
				{
					m_task = *(_WuSeTaskStruct*)pControl;
					DWORD lcolor = 0xFFFFFFFF;
					if (m_task.color > 0)
					{
						lcolor = m_colors[m_task.color-1];
					}
					if (m_task.type == "0")
					{
						m_task.strName += "(未接)";
						m_pLabTaskName->SetTips("点击领取任务");
						m_pBtnTaskAuto->SetText("领取任务",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
						m_pBtnTaskAuto->SetEnable(true);
					}
					else if (m_task.type == "1")
					{
						m_task.strName += "(进行中)";
						m_pLabTaskName->SetTips("");
						m_pBtnTaskAuto->SetText("完成任务",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
						m_pBtnTaskAuto->SetEnable(false);
					}
					else if (m_task.type == "2")
					{
						m_task.strName += "(完成)";
						m_pLabTaskName->SetTips("点击完成任务");
						m_pBtnTaskAuto->SetText("完成任务",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
						m_pBtnTaskAuto->SetEnable(true);
					}

					m_pLabTaskName->SetText(m_task.strName.c_str(),lcolor,lcolor,lcolor,lcolor,12, DTF_UnderLine, DTF_UnderLine, DTF_UnderLine);					
					m_TagUp.Parse(m_task.strInfo);

					m_pBtnFlush->SetEnable(m_task.type == "0" && m_task.color != 5);

					m_iEnterTime = 0;

					return true;
				}
				else if (dwData == 11)
				{
					m_task.color = ((_WuSeTaskStruct*)pControl)->color;	
					m_task.fresh = ((_WuSeTaskStruct*)pControl)->fresh;
					m_task.strCommand2 = ((_WuSeTaskStruct*)pControl)->strCommand2;
					m_task.strInfo = ((_WuSeTaskStruct*)pControl)->strInfo;

					DWORD lcolor = 0xFFFFFFFF;
					if (m_task.color > 0)
					{
						lcolor = m_colors[m_task.color-1];
					}

					m_pLabTaskName->SetText(m_task.strName.c_str(),lcolor,lcolor,lcolor,lcolor,12, DTF_UnderLine, DTF_UnderLine, DTF_UnderLine);
					m_TagUp.Parse(m_task.strInfo);

					m_pBtnFlush->SetEnable(m_task.color != 5);
					m_pBtnTaskAuto->SetEnable(true);

					m_iEnterTime = 0;

					return true;
				}

				return false;
			}
			break;

		case MSG_CTRL_BUTTON_CLICK:
			{
				// 立即刷新
				if(pControl == m_pBtnFlush)
				{
					if (m_iEnterTime == 0 && !m_task.strName.empty() && m_task.type == "0" && !m_task.strCommand2.empty())
					{
						m_iEnterTime = GetTickCount() - (m_task.color - 1) * WuSeColorTime;
						m_pBtnFlush->SetEnable(false);
						m_pBtnTaskAuto->SetEnable(false);
					}
					
					return true;
				}
				else if (pControl == m_pBtnTaskAuto)
				{
					if (!m_task.strName.empty() && m_task.type != "1" && !m_task.strCommand1.empty())
					{
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),m_task.strCommand1.c_str());						
					}
					return true;
				}
			}
			break;
		case MSG_CTRL_LABEL_CLICK:
			{
				if (pControl == m_pLabTaskName)
				{
					this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_OpenWuSeTaskWnd_PaoPao,NULL);
					if (m_iEnterTime == 0 && !m_task.strName.empty() && m_task.type != "1" && !m_task.strCommand1.empty())
					{
						g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),m_task.strCommand1.c_str());
						//OnClickCloseButton();
					}
					return true;
				}
			}
			break;
		default:
			break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CTaskWuSeDailyWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 210,m_iScreenY + 11,"日常任务",COLOR_TEXT_NORMAL,FONT_YAHEI,16);
	
	g_pFont->DrawText(m_iScreenX + 382,m_iScreenY + 192,"放入五色卷轴",0xFFF0D4B4);
	
	if(m_iEnterTime != 0 && m_iEnterTime != -1)
	{		
		DWORD dwCount = GetTickCount();
		DWORD dwTime = dwCount - m_iEnterTime;

		int iTmp = dwTime / WuSeColorTime;
		if (iTmp >= 0 && iTmp <= 4)
		{
			LPTexture pTexC = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21631 + iTmp,EP_UI);
			if (pTexC)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + 37 + iTmp * 37,m_iScreenY + 164,pTexC);
			}
		}

		if (dwTime > WuSeColorTime * 5 - 100)
		{
			m_iEnterTime = -1;

			if (!m_task.strName.empty() && m_task.type == "0" && !m_task.strCommand2.empty())
			{
				if (g_NPC.GetWuSeGood().GetID() != 0)
					g_pGameControl->SEND_Exchange_Submit(g_NPC.GetID(), g_NPC.GetWuSeGood().GetID());
				else
					g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),m_task.strCommand2.c_str());
			}
		}
	}
	else if (m_iEnterTime == 0 )
	{
		if (m_task.color > 0 && m_task.color <= 5)
		{
			int iselect = m_task.color - 1;	
			LPTexture pTexC = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21631 + iselect,EP_UI);
			if (pTexC)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + 37 + iselect * 37,m_iScreenY + 164,pTexC);
			}
		}
	}

	if(g_NPC.GetWuSeGood().GetID()!=0)
		CGoodGrid::DrawOneGood(m_iScreenX+WuSeGood_PosX+20, m_iScreenY+WuSeGood_PosY+20, g_NPC.GetWuSeGood());
}

void CTaskWuSeDailyWnd::OnClickCloseButton()
{
	if(g_NPC.GetWuSeGood().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetWuSeGood());
		g_NPC.GetWuSeGood().SetID(0);
	}

	CCtrlWindowX::OnClickCloseButton();
}

bool CTaskWuSeDailyWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);
	pTip->GetSelfEquipTips()->SetShow(false);

	if(iX > WuSeGood_PosX && iX < WuSeGood_PosX + 36 && iY > WuSeGood_PosY && iY < WuSeGood_PosY + 36)
	{
		if(g_NPC.GetWuSeGood().GetID() != 0)
		{
			pTip->Parse(g_NPC.GetWuSeGood());
		}
		else
		{
			return CCtrlWindowX::OnMouseMove(iX,iY);
		}
	}	
	else
	{
		return CCtrlWindowX::OnMouseMove(iX,iY);
	}

	int x = m_iScreenX + iX + 10;
	int y = m_iScreenY + iY + 10;
	pTip->AdjustXY(x,y);
	pTip->Move(x,y);
	pTip->SetShow(true);
	return true;	
}

bool CTaskWuSeDailyWnd::OnLeftButtonUp( int iX, int iY )
{
	if(iX > WuSeGood_PosX && iX < WuSeGood_PosX + 36 && iY > WuSeGood_PosY && iY < WuSeGood_PosY + 36)
	{
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if (GoodFrom.DropGood.GetID() && (GoodFrom.DropGood.GetStdMode()!=46))
		{
			g_TalkMgr.PushSysTalk("请放入五色卷轴！");
		}
		else
		{
			CGood tmp = GoodFrom.DropGood;
			GoodFrom.DropGood = g_NPC.GetWuSeGood();
			g_NPC.GetWuSeGood() = tmp;
			CGoodGrid::SetDropGoodFrom(GoodFrom);
		}		
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}

