#include "BoxWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Control/ParserTip.h"


#define  BOXWND_GRID_WIDTH 39
#define  BOXWND_GRID_HEIGHT 42


DWORD CBoxWnd::sm_LastAccessTime = 0;


INIT_WND_POSX(CBoxWnd,POS_AUTO,POS_AUTO)
CBoxWnd::CBoxWnd(void)
{
	m_iIndex = 16407;	
	m_pOpenBox = NULL;
}

CBoxWnd::~CBoxWnd(void)
{
}

void CBoxWnd::OnCreate()
{
	SetCloseButton(427,4,80);

	m_pOpenBox = new CCtrlButton();
	AddControl(m_pOpenBox);
	m_pOpenBox->CreateEx(this,190,304,95,96,97,98);
	m_pOpenBox->SetText("租  赁", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
}

void CBoxWnd::Draw()
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//if (g_pGameData->GetBoxInfo().iPayGoodCount != 0)
	//{
	//	m_pOpenBox->SetEnable(false);
	//	m_pOpenBox->SetShow(false);
	//}
	g_pFont->DrawText(m_iScreenX + 203,m_iScreenY + 10,"至尊宝",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
	
	stBoxInfo& boxInfo = g_pGameData->GetBoxInfo();
	if(boxInfo.iPayCapacity)
	{
		g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 140,g_pGameData->GetBoxInfo().szExpireTime,0xff00ff00);
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 140,"至尊宝未租赁",0xffff0000);
	}

	g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 153,"放入至尊宝中的物品不会爆出，使用10个绑定元宝即可租赁并获得一周的",-1);
	g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 166,"使用时间。如果至尊宝到期，你就不能从其中取出和存入物品，重新租赁",-1);
	g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 179,"后可在此存取。",-1);
	g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 192,"人物等级达到42级以后即可租赁至尊宝，可租赁的储物格子数量随人物等",0xFF87C23A);
	g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 205,"级提升而增加",0xFF87C23A);
	

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17538,EP_UI);		
	for(int i = 0;i < 10;i++)
	{
		if (i < boxInfo.iFreeCapacity)
		{
			CGoodGrid::DrawOneGood(m_iScreenX + 59 + BOXWND_GRID_WIDTH * (i % 10),m_iScreenY + 68 + i / 10 * BOXWND_GRID_HEIGHT,boxInfo.freegoods.Get(i));
		}
		else
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 59 - 16 + BOXWND_GRID_WIDTH * (i % 10),m_iScreenY - 16 + 68 + i / 10 * BOXWND_GRID_HEIGHT,pTex);
		}
	}

	for(int i = 0;i < 10;i++)
	{
		if (i < boxInfo.iPayCapacity)
		{
			CGoodGrid::DrawOneGood(m_iScreenX + 59 + BOXWND_GRID_WIDTH * (i % 10),m_iScreenY + 108 + i / 10 * BOXWND_GRID_HEIGHT,boxInfo.paygoods.Get(i));
		}
		else
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 59 - 16 + BOXWND_GRID_WIDTH * (i % 10),m_iScreenY - 16 + 108 + i / 10 * BOXWND_GRID_HEIGHT,pTex);
		}
	}	
}

int CBoxWnd::GetGrid(int iX,int iY)
{
	int iGrid = -1;

	if(iX > 40 && iY > 48 && iX < 428 && iY < 125)
	{
		 iGrid = (iY - 48) / BOXWND_GRID_HEIGHT * 10 + (iX - 40) / BOXWND_GRID_WIDTH;			 
	}

	//if (iGrid >= 10 && iGrid - 10 > g_pGameData->GetBoxInfo().iPayCapacity)
	//{
	//	iGrid = -1;
	//}

	return iGrid;
}

bool CBoxWnd::OnLeftButtonUp(int iX, int iY)
{
	stBoxInfo& boxInfo = g_pGameData->GetBoxInfo();

	int iGrid = GetGrid(iX,iY);
	if(iGrid != -1)
	{
		BYTE byArea = iGrid / 10;
		iGrid = iGrid % 10;
		CGood *pGoodInGrid = NULL;
		if (byArea == 0 && iGrid < boxInfo.freegoods.Size())
		{
			pGoodInGrid = &(boxInfo.freegoods.Get(iGrid));
		}
		else if (byArea == 1 && iGrid < boxInfo.paygoods.Size())
		{
			pGoodInGrid = &(boxInfo.paygoods.Get(iGrid));
		}
		if (!pGoodInGrid)
		{
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == Package_Wnd || GoodFrom.eFromWnd == LingXiBox_Free_Wnd || GoodFrom.eFromWnd == LingXiBox_Pay_Wnd))
		{
			int toWnd = (byArea==0)?LingXiBox_Free_Wnd:LingXiBox_Pay_Wnd;
			if (GoodFrom.eFromWnd == toWnd)
			{
				if (toWnd == LingXiBox_Free_Wnd)
				{
					boxInfo.freegoods.Add(GoodFrom.DropGood);
				}
				else if (toWnd == LingXiBox_Pay_Wnd)
				{
					boxInfo.paygoods.Add(GoodFrom.DropGood);
				}
				GoodFrom.DropGood.SetID(0);
				return true;
			}

			if(GetTickCount() - sm_LastAccessTime > 3000)
			{
				if(!boxInfo.IsExpired())
				{
					sm_LastAccessTime = GetTickCount();
					GoodFrom.DropGood.SetToWnd(toWnd);
					if (GoodFrom.eFromWnd == Package_Wnd)
					{
						g_pGameControl->SEND_BOX_INFO(6,GoodFrom.DropGood.GetID(),byArea);	
					}
					else
					{
						g_pGameControl->SEND_BOX_INFO(7,GoodFrom.DropGood.GetID(),byArea);//免费区和收费区交换
					}
					SELF.GetUsingTemp().Add(GoodFrom.DropGood);
					GoodFrom.DropGood.SetID(0);
				}
				else
				{
					CGoodGrid::ReleaseDrop();
					m_bClick = false;
					g_MsgBoxMgr.PopSimpleAlert("至尊宝未租赁，不能向其中存入和取出物品。");
					return true;
				}
			}
			else
			{
				CGoodGrid::ReleaseDrop();
				m_bClick = false;
				g_TalkMgr.PushSysTalk("请稍候再使用至尊宝");
				return true;
			}
		}
		else if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == LingXiBox_Free_Wnd || GoodFrom.eFromWnd == LingXiBox_Pay_Wnd))
		{
			CGood temp;
			eGoodFrom goodfrom = GoodFrom.eFromWnd;
			temp = *pGoodInGrid;
			*pGoodInGrid = GoodFrom.DropGood;
			GoodFrom.DropGood = temp;
			GoodFrom.eFromWnd = goodfrom;	
			GoodFrom.iWndPos = iGrid;
		}
		else if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd != LingXiBox_Free_Wnd && GoodFrom.eFromWnd != LingXiBox_Pay_Wnd && GoodFrom.eFromWnd != Package_Wnd)
		{
			CGoodGrid::ReleaseDrop();
			m_bClick = false;
			g_MsgBoxMgr.PopSimpleAlert("请先把物品放到角色包裹，然后再存放到至尊宝。");
			return true;
		}
		else if(GoodFrom.DropGood.GetID() == 0)
		{
			GoodFrom.DropGood = *pGoodInGrid;
			GoodFrom.eFromWnd = (byArea==0)?LingXiBox_Free_Wnd:LingXiBox_Pay_Wnd;
			GoodFrom.iWndPos = iGrid;
			pGoodInGrid->SetID(0);
		}		

		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CBoxWnd::OnLeftButtonDClick(int iX,int iY)
{
	stBoxInfo& boxInfo = g_pGameData->GetBoxInfo();
	int iGrid = GetGrid(iX,iY);

	if(iGrid != -1)
	{
		BYTE byArea = iGrid / 10;
		iGrid = iGrid % 10;
		CGood *pGoodInGrid = NULL;
		if (byArea == 0 && iGrid < boxInfo.freegoods.Size())
		{
			pGoodInGrid = &(boxInfo.freegoods.Get(iGrid));
		}
		else if (byArea == 1 && iGrid < boxInfo.paygoods.Size())
		{
			pGoodInGrid = &(boxInfo.paygoods.Get(iGrid));
		}
		if (!pGoodInGrid)
		{
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();	

		if(GoodFrom.DropGood.GetID() != 0 && pGoodInGrid->GetID() != 0 )     //鼠标上有东西的话再双击物品无效
		{
			return true;
		}

		if(GoodFrom.DropGood.GetID() != 0)
		{
			*pGoodInGrid = GoodFrom.DropGood;
			GoodFrom.DropGood.SetID(0);
			g_MsgBoxMgr.PopSimpleAlert("至尊宝中无法使用该物品，请拖至包裹栏中使用");
			return true;
		}
	}

	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

bool CBoxWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();

	int x = m_iScreenX + iX + 10;
	int y = m_iScreenY + iY + 10;

	stBoxInfo& boxInfo = g_pGameData->GetBoxInfo();
	int iGrid = GetGrid(iX,iY);
	if(iGrid != -1)
	{
		pTip->Clear();
		pTip->SetShow(false);
		BYTE byArea = iGrid / 10;
		iGrid = iGrid % 10;
		CGood *pGoodInGrid = NULL;
		if (byArea == 0)
		{
			if(iGrid >= boxInfo.iFreeCapacity)
			{
				pTip->AddText("暂未开启");
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
				return true;
			}
			else if(iGrid < boxInfo.freegoods.Size())
			{
				pGoodInGrid = &(boxInfo.freegoods.Get(iGrid));
			}
		}
		else if (byArea == 1)
		{
			int iNeedLevel = GetGridOrLevel(iGrid + 1,1);
			if (SELF.GetLevel() < iNeedLevel)
			{
				char szTemp[128] = {0};
				sprintf(szTemp,"人物等级达到%d级后才可租赁",iNeedLevel);
				pTip->AddText(szTemp);
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
				return true;
			}
			else if(boxInfo.iPayCapacity == 0)
			{
				pTip->AddText("未租赁");
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
				return true;
			}
			else if(iGrid < boxInfo.paygoods.Size())
			{
				pGoodInGrid = &(boxInfo.paygoods.Get(iGrid));
		    }
		}
		if (!pGoodInGrid)
		{
			return true;
		}

		if(pGoodInGrid->GetID() != 0)
		{
			pTip->Parse(*pGoodInGrid);
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}
	else
	{
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

int CBoxWnd::GetGridOrLevel(int i,int iType)
{
	if (iType == 1)//指定格子需要多少等级
	{
		if (i >= 10)
		{
			return 62;
		}
		else if (i >= 9)
		{
			return 60;
		}
		else if (i >= 8)
		{
			return 58;
		}
		else if (i >= 7)
		{
			return 56;
		}
		else if (i >= 6)
		{
			return 54;
		}
		else if (i >= 5)
		{
			return 52;
		}
		else if (i >= 4)
		{
			return 50;
		}
		else if (i >= 3)
		{
			return 46;
		}
		else
		{
			return 42;
		}
	}
	else if (iType == 2)//指定等级可以租多少格
	{
		if (i >= 62)
		{
			return 10;
		}
		else if (i >= 60)
		{
			return 9;
		}
		else if (i >= 58)
		{
			return 8;
		}
		else if (i >= 56)
		{
			return 7;
		}
		else if (i >= 54)
		{
			return 6;
		}
		else if (i >= 52)
		{
			return 5;
		}
		else if (i >= 50)
		{
			return 4;
		}
		else if (i >= 46)
		{
			return 3;
		}
		else if(i >= 42)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

bool CBoxWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(MSG_CTRL_LINGXIBOX_WND == dwMsg)
	{
		if(dwData == OPER_CUSTOM)
		{
			g_pControl->PopupWindow(MSG_CTRL_LINGXIBOX_WND,OPER_RECREATE);
			return true;
		}
	}

	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
        stBoxInfo& boxInfo = g_pGameData->GetBoxInfo();
		if(pControl == m_pOpenBox)
		{
			if (SELF.GetLevel() < 42)
			{
				m_bClick = false;
				g_MsgBoxMgr.PopSimpleAlert("您的等级没有达到42级,无法租赁");
			}
			else
			{
				m_bClick = false;
				int iNum = GetGridOrLevel(SELF.GetLevel(),2);
				string strTemp = StringUtil::format("\\你可以用<color=green 10个绑定元宝>开启它的储物功能。至尊宝租赁后，你会获得\\<color=green %d个储物格子>和<color=green 一周的使用时间>，你确定租赁至尊宝吗？",iNum);
				g_MsgBoxMgr.PopTagOkCancelBox(strTemp.c_str(),MSG_CTRL_OPEN_BOX,0);
			}
			return true;
		}
		//else if(pControl == m_pEnlarge)
		//{
		//	/*string strTemp = StringUtil::format("\\你当前已启用了<color=green %d个格子>，你可以再用<color=green 10点灵力>将至尊宝格子再扩充<color=green 2格>。你确\\定要扩充至尊宝吗？",boxInfo.iCapacity);
		//	g_MsgBoxMgr.PopTagOkCancelBox(strTemp.c_str(),MSG_CTRL_ENLARGE_BOX,0);
		//	m_bClick = false;*/
		//	g_pGameControl->SEND_BOX_INFO(7,0);
		//	return true;
		//}		
		//else if(pControl == m_pFill)
		//{
		//	g_MsgBoxMgr.PopEditBox("你需要注入多少灵力？",MSG_CTRL_FILL_BOX,0,0,true,5);
		//	m_bClick = false;
		//	return true;
		//}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
