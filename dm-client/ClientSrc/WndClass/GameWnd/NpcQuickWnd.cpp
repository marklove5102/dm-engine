#include "NpcQuickWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/Good.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/CtrlMenuWnd.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/WooolStoreData.h"
#include "GameData/OtherData.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "GameData/ConfigData.h"
#include "NpcWnd.h"

INIT_WND_POSX(CNpcQuickWnd,POS_VARIABLE,POS_VARIABLE)

#define GOODSNUMBERPERPAGE 40 //仓库界面每页最大显示物品数量

DWORD CNpcQuickWnd::s_vGoodPosition[MAXNUMBERSTORAGE];

CNpcQuickWnd::CNpcQuickWnd(void)
{
	m_iType = sm_dwWindowType;

	m_iSelectIndex = -1;
	if(m_iType == 1)
	{
		m_iStartX = 22;
		m_iStartY = 32;
		m_iCellW = 38;
		m_iCellH = 38;
		m_iIntervalW = 2;
		m_iIntervalH = 2;
		m_iRows = 4;
		m_iCols = 8;
	}
	else if(m_iType == 2)
	{
		m_iStartX = 33;
		m_iStartY = 71;
		m_iCellW = 38;
		m_iCellH = 38;
		m_iIntervalW = 2;
		m_iIntervalH = 2;
		m_iRows = 7;
		m_iCols = 4;
	}

	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 10;
	m_iOffY = 199+182;
	m_bNeedSavePos = false;
	m_bNeedSavePage = false;
	memset(s_vGoodPosition, 0, sizeof(DWORD)*(MAXNUMBERSTORAGE));


	if(m_iType == 1)
	{
		m_iIndex = 14508;
		if(g_pControl->GetWindowPos(m_iOffX,m_iOffY,"NpcWnd"))
			m_iOffY += 201;
	}
	else if(m_iType == 2)
	{
		//m_iIndex = 14509;
		m_iOffX = 430-219;
		m_iOffY = 10;

		m_iVersion = 2;
		AddTabPage(0,0,MAKELONG(14510,PACKAGE_INTERFACE),17,36,115,116,117,118,"仓库一",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL,false,"点击切换至1号仓库");
		AddTabPage(0,0,MAKELONG(14510,PACKAGE_INTERFACE),69,36,115,116,117,118,"仓库二",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL,false,"点击切换至2号仓库");
		AddTabPage(0,0,MAKELONG(14510,PACKAGE_INTERFACE),121,36,115,116,117,118,"仓库三",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL,false,"点击切换至3号仓库");
		m_TabPage.iCurPage = 0;
	}

	if (m_iType == 1 || m_iType == 2)
	{
		m_iOffY += CNpcWnd::m_sbHeaderNpc?170:0;
	}

	//打开包裹窗口
	g_pControl->SetWindowPos(430,10,"PackageWnd");
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_CREATE);
}

CNpcQuickWnd::~CNpcQuickWnd(void)
{
	if(g_pGameData)
	{
		if(g_pGameData->GetMouseType() == MOUSE_REPAIR || g_pGameData->GetMouseType() == MOUSE_SPECIALREPAIR)
			g_pGameData->SetMouseType(MOUSE_STANDARD);

		ClearDropGood();
	}
	if(m_iType == 2)
		g_pGameControl->SEND_Objects_Storage_Position();
}

void CNpcQuickWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iPos = m_pScroll->GetPos();
	int iStart = iPos*m_iCols;
	int iEnd = iStart + m_iRows*m_iCols;

	int iPrintX,iPrintY;
	iPrintX = m_iScreenX + m_iStartX;
	iPrintY = m_iScreenY + m_iStartY;

	std::vector<CGood>	&QuickVector = g_NPC.GetNpcQuickVector();
	int iCount = QuickVector.size();

	//如果是仓库,有些格子不可用
	if(m_iType == 2)
	{
		g_pFont->DrawText(m_iScreenX + 120,m_iScreenY + 12,"仓   库",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);


		for(int i = g_NPC.GetStorageMaxCount(); i < GOODSNUMBERPERPAGE * (m_TabPage.iCurPage + 1); i++)
		{
			int index = i - GOODSNUMBERPERPAGE * m_TabPage.iCurPage - iStart;
			if (index < 0)
			{
				continue;
			}

			int iRow = index/m_iCols;
			int iCol = index%m_iCols;
			if(iRow >= m_iRows)
				break;

			//如果这个位置有物品,说明是仓库缩小之前放进去的,这个位置就不要画锁
			if (s_vGoodPosition[i] != 0)
			{
				CGood* findGood = NULL;
				if (FindGoodById(s_vGoodPosition[i], findGood) && findGood != NULL)
				{
					continue;
				}
			}
			

			int iLeft = iCol * (m_iCellW+m_iIntervalW)+iPrintX;
			int iTop = iRow * (m_iCellH + m_iIntervalH)+iPrintY;

			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17538,EP_UI);		

			g_pGfx->DrawTextureNL(iLeft+2,iTop+2,pTex);

		}
	}
	//画选中背景框
	int selectIndex = -1;
	if (m_iType == 2)
	{
		selectIndex = m_iSelectIndex - GOODSNUMBERPERPAGE * m_TabPage.iCurPage;
	}
	else
		selectIndex = m_iSelectIndex;

	if(selectIndex >= 0 && selectIndex < iCount)
	{
		int iSelectIndex = selectIndex - iStart;
		if(iSelectIndex >= 0 && iSelectIndex < m_iRows*m_iCols)
		{
			int iSelRow = iSelectIndex/m_iCols;
			int iSelCol = iSelectIndex%m_iCols;
			int iLeft = iSelCol*(m_iCellW+m_iIntervalW)+iPrintX;
			int iTop = iSelRow*(m_iCellH + m_iIntervalH)+iPrintY;

			g_pGfx->DrawFillRect(iLeft,iTop,m_iCellW,m_iCellH,0x44FF0000);
		}
	}
	//画物品
	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
	if (m_iType == 1)
	{
		for(int i = iStart,index = 0; i < iCount && i < iEnd; i ++,index++)
		{
			if(g_pControl) //绘制单个物品
			{
				int iRow = index/m_iCols;
				int iCol = index%m_iCols;
				if(iRow >= m_iRows)
					break;

				CGoodGrid::DrawOneGood(iPrintX + 19 + iCol*(m_iCellW+m_iIntervalW),iPrintY + 18 + iRow*(m_iCellH + m_iIntervalH),QuickVector[i]);
			}
		}
	} 
	else if (m_iType == 2)
	{
		for (int i = iStart,index = 0; /*i < (iCount - GOODSNUMBERPERPAGE * m_TabPage.iCurPage) &&*/ i < iEnd; i ++,index++)
		{
			if(g_pControl) //绘制单个物品
			{
				int iRow = index/m_iCols;
				int iCol = index%m_iCols;
				if(iRow >= m_iRows)
					break;

				if (s_vGoodPosition[index + GOODSNUMBERPERPAGE * m_TabPage.iCurPage + iStart] != 0)
				{
					CGood* findGood = NULL;

					if (!FindGoodById(s_vGoodPosition[index+ GOODSNUMBERPERPAGE * m_TabPage.iCurPage + iStart], findGood) || findGood == NULL)
						continue;

					if(GoodFrom.DropGood.GetID() == findGood->GetID())//从仓库里拿起来的物品就不画
						continue;

					CGoodGrid::DrawOneGood(iPrintX + 19 + iCol*(m_iCellW+m_iIntervalW),iPrintY + 18 + iRow*(m_iCellH + m_iIntervalH),*findGood/*QuickVector[i + GOODSNUMBERPERPAGE * m_TabPage.iCurPage]*/);
					continue;

				}
			}
		}
	}

}



bool CNpcQuickWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK :
		{
			if(pControl == m_pOkButton)
			{
				if(m_iType == 1)
				{
					BuyGoods();
				}
				else if(m_iType == 2)
				{
					CarryBackGoods();
				}
				return true;
			}
			else if(pControl == m_pCancelButton)
			{
				OnClickCloseButton();
				return true;
			}
			else if(pControl == m_pBodyRepair)
			{
				OnClickBodyRepair();
				return true;
			}
			else if(pControl == m_pCommonRepair)
			{
				if(g_pGameData->GetMouseType() == MOUSE_REPAIR)
					g_pGameData->SetMouseType(MOUSE_STANDARD);
				else
					g_pGameData->SetMouseType(MOUSE_REPAIR);

				return true;
			}
			else if(pControl == m_pSpecialRepair)
			{
				if(g_pGameData->GetMouseType() == MOUSE_SPECIALREPAIR)
					g_pGameData->SetMouseType(MOUSE_STANDARD);
				else
					g_pGameData->SetMouseType(MOUSE_SPECIALREPAIR);

				return true;
			}
			else if(pControl == m_pUpButton )
			{
				m_pScroll->GoDown();
				return true;
			}
			else if(pControl == m_pDownButton )
			{
				m_pScroll->GoUp();
				return true;
			}
			break;
		}
	case MSG_CTRL_POP_MENU:
		{
			switch(dwData)
			{
			case POP_MENU_ITEM_NPC_QUICK_BUY:
				BuyGoods();
				break;
			case POP_MENU_ITEM_NPC_QUICK_CARRYBACK:
				CarryBackGoods();
				break;

			default:
				break;
			}

			return true;
		}
		break;
	case MSG_CTRL_NPC_QUICK_WND:
		{
			if(dwData == 201)//刷新滚动条位置
			{
				UpdateScrollPos();
				return true;
			}

			return false;
		}
		break;

	default:
		break;

	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);

}

void CNpcQuickWnd::OnCreate()
{
	//确认按钮
	m_pOkButton = new CCtrlButton();
	AddControl(m_pOkButton);
	//取消按钮
	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);

	////上下按钮
	//m_pUpButton = new CCtrlButton();
	//AddControl(m_pUpButton);

	//m_pDownButton = new CCtrlButton();
	//AddControl(m_pDownButton);
	//滚动条
	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	if(m_iType == 1)
	{
		m_pOkButton->CreateEx(this,72,200,90,91,92,93);
		m_pOkButton->SetText("购  买", 0xFFF0AF64 ,0xFFFAC896, 0xFF5AA0A0, 0xFF646464, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
		m_pCancelButton->CreateEx(this,172,200,90,91,92,93);
		m_pCancelButton->SetText("取  消", 0xFFF0AF64 ,0xFFFAC896, 0xFF5AA0A0, 0xFF646464, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
		//m_pUpButton->CreateEx(this,336,34,51,52,53);
		//m_pDownButton->CreateEx(this,336,158,56,57,58);
 		m_pScroll->CreateEx(this, 347, 30, 16,162);

		SetCloseButton(348,2, 80);

	}
	else if(m_iType == 2)
	{
		m_pOkButton->CreateEx(this, 32,359, 90, 91, 92, 93);
		m_pOkButton->SetText("取  出", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);

		m_pCancelButton->CreateEx(this,130,359,90, 91, 92, 93);
		m_pCancelButton->SetText("取  消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);


		m_pScroll = new CCtrlScroll();
		AddControl(m_pScroll);
		m_pScroll->CreateEx(this,194,69,14,331 - 52);

		SetCloseButton(206,4,80);
	}
	int iRange = g_NPC.GetNpcQuickVector().size()/m_iCols;
	if(g_NPC.GetNpcQuickVector().size()%m_iCols != 0)
		iRange += 1;

	if (m_iType == 2)
	{
		iRange = 10;
	}
	iRange = iRange - m_iRows;
	if(iRange < 0)
		iRange = 0;

	m_pScroll->SetRange(iRange);
	m_pScroll->SetPos(0);

	if(m_iType == 1)
	{
		m_pBodyRepair = new CCtrlButton();
		AddControl(m_pBodyRepair);
		m_pBodyRepair->CreateEx(this,268,199,14540,14541,14542);
		m_pBodyRepair->SetTips("全身特修(使用冰泉圣水)");

		m_pCommonRepair = new CCtrlButton();
		AddControl(m_pCommonRepair);
		m_pCommonRepair->CreateEx(this,298,199,14523,14524,14525);
		m_pCommonRepair->SetTips("使用锤子点击包裹装备进行普通修理");

		m_pSpecialRepair = new CCtrlButton();
		AddControl(m_pSpecialRepair);
		m_pSpecialRepair->CreateEx(this,328,199,14519,14520,14521);
		m_pSpecialRepair->SetTips("使用锤子点击包裹装备进行特殊修理");
	}
	else
	{
		m_pBodyRepair = NULL;
		m_pCommonRepair = NULL;
		m_pSpecialRepair = NULL;
	}

	if (m_iType == 2)
	{
		CCtrlWindowX::OnCreate();
	}

}

void CNpcQuickWnd::OnClickCloseButton()
{
	g_pAudio->Play(EAT_OTHER,1,g_pAudio->GetRand()++);

	CloseWindow();
}

bool CNpcQuickWnd::OnLeftButtonDown(int iX, int iY)
{
	if(g_pGameData->GetMouseType() == MOUSE_REPAIR || g_pGameData->GetMouseType() == MOUSE_SPECIALREPAIR)
		g_pGameData->SetMouseType(MOUSE_STANDARD);
	m_iSelectIndex = GetIndex(iX,iY);

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CNpcQuickWnd::OnLeftButtonUp(int iX, int iY)
{
	CCtrlWindowX::OnLeftButtonUp(iX,iY);

	
	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

	m_iSelectIndex = GetIndex(iX,iY);

	if (m_iSelectIndex < 0)
		return true;

	if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == Package_Wnd||GoodFrom.eFromWnd == Panel_Wnd)&&
		g_NPC.GetGood().GetID()==0)
	{
		//放物品音乐////////////////////////////
		int i = GoodFrom.DropGood.GetStdMode()+1;

		if(GoodFrom.DropGood.GetID() != 0)
		{
			g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
		}

		g_NPC.GetGood() = GoodFrom.DropGood;
		if(m_iType == 1)//卖物品
		{
			g_NPC.SetExchangeFromQuickNpcWnd(true); // 设置是通过图形化NPC窗口进行交易
			g_pGameControl->SEND_Exchange_Sale_Takein(g_NPC.GetID(),GoodFrom.DropGood.GetID(),GoodFrom.DropGood.GetName());
			GoodFrom.DropGood.SetID(0);
			 CGoodGrid::SetDropGoodFrom(GoodFrom);
			g_NPC.SetStart(true); // 设置消息未到标识
		}
		else if(m_iType == 2)//存物品
		{
			if (m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetStorageMaxCount())
			{
				g_NPC.SetExchangeFromQuickNpcWnd(true);
				g_pGameControl->SEND_Exchange_Storage_Ok(g_NPC.GetID(),GoodFrom.DropGood.GetID(),GoodFrom.DropGood.GetName(),1);
				
				CGood* temp = NULL;
				if (FindGoodById(s_vGoodPosition[m_iSelectIndex], temp) && temp != NULL)
				{
					s_vGoodPosition[m_iSelectIndex] = GoodFrom.DropGood.GetID();
					GoodFrom.DropGood = *temp;
					GoodFrom.eFromWnd = NpcQuickStorage_Wnd;
				}
				else
				{
					s_vGoodPosition[m_iSelectIndex] = GoodFrom.DropGood.GetID();
					GoodFrom.DropGood.SetID(0);
				}
					
				CGoodGrid::SetDropGoodFrom(GoodFrom);
			}
			 
		}

	}
	else		// 判断是否处于等待服务器状态
	{ 
		
		if(GoodFrom.DropGood.GetID() == 0)
		{
			if (m_iType == 1)
			{
				if(m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetNpcQuickVector().size())
				{
					CGood &temp = g_NPC.GetNpcQuickVector()[m_iSelectIndex];
					GoodFrom.DropGood = temp;

						GoodFrom.eFromWnd = NpcQuickSale_Wnd;
		
					CGoodGrid::SetDropGoodFrom(GoodFrom);
				}
			} 
			else
			{
				if(m_iSelectIndex >= 0 && m_iSelectIndex < MAXNUMBERSTORAGE)
				{
					CGood* temp = NULL;

					
					if (FindGoodById(s_vGoodPosition[m_iSelectIndex], temp) && temp != NULL)
					{
						GoodFrom.DropGood = *temp;


						GoodFrom.eFromWnd = NpcQuickStorage_Wnd;

						CGoodGrid::SetDropGoodFrom(GoodFrom);

						s_vGoodPosition[m_iSelectIndex] = 0;
					}
					
				}
			}
			
		}
		else if(GoodFrom.eFromWnd == NpcQuickSale_Wnd)
		{
			if(m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetNpcQuickVector().size())
			{
				CGood &tempGood = g_NPC.GetNpcQuickVector()[m_iSelectIndex];
				if(GoodFrom.DropGood.GetID() != tempGood.GetID())
					GoodFrom.DropGood = tempGood;
				else
					GoodFrom.DropGood.SetID(0);
			}
			else
			{
				GoodFrom.DropGood.SetID(0);
			}

			CGoodGrid::SetDropGoodFrom(GoodFrom);

		}
		else if(GoodFrom.eFromWnd == NpcQuickStorage_Wnd)
		{
			if(m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetStorageMaxCount())
			{
				CGood* tempGood = NULL;
				bool flag = FindGoodById(s_vGoodPosition[m_iSelectIndex], tempGood);
				if(flag && tempGood != NULL)
				{
					if(GoodFrom.DropGood.GetID() != tempGood->GetID())
					{

						s_vGoodPosition[m_iSelectIndex] = GoodFrom.DropGood.GetID();
						//CGood temp = GoodFrom.DropGood;
						GoodFrom.DropGood = *tempGood;
						//*tempGood = temp;
					}
						
					else
					{
						GoodFrom.DropGood.SetID(0);
						s_vGoodPosition[m_iSelectIndex] = tempGood->GetID();
					}
						

					
				}
				else 
				{
					s_vGoodPosition[m_iSelectIndex] = GoodFrom.DropGood.GetID();
					GoodFrom.DropGood.SetID(0);
				}
				
			}
			/*else
			{
				GoodFrom.DropGood.SetID(0);
			}*/

			CGoodGrid::SetDropGoodFrom(GoodFrom);

		}

	}

	return true;
}

bool CNpcQuickWnd::OnMouseMove(int iX, int iY)
{
	CCtrlWindowX::OnMouseMove(iX,iY);

	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->SetShow(false);
	pTip->Clear();

	if(g_pControl->GetMenuWnd()->GetCaller() == this && g_pControl->GetMenuWnd()->IsShow())
	{
		return CCtrlWindowX::OnMouseMove(iX, iY);
	}

	int index = GetIndex(iX,iY);
	if (m_iType == 2)
	{
		//当鼠标移动到对应仓库内的格子位时，TIPS提示
		if (index >= g_NPC.GetStorageMaxCount())
		{
			if (index >= GOODSNUMBERPERPAGE)
			{
				pTip->AddText("提升神武殿会员等级可开启更多包裹格",-1,-1);
				pTip->AddText("双击开启神武殿",-1,-1);
			} 
			else
			{
				pTip->AddText("双击获得此区域共10格扩展仓库永久使用权");
			}
			
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}
	
	if(index < 0)
		return CCtrlWindowX::OnMouseMove(iX, iY);

	std::vector<CGood>	&QuickSellVector = g_NPC.GetNpcQuickVector();
	if (m_iType == 1)
	{
		if(index < QuickSellVector.size() && QuickSellVector[index].GetID() != 0)
		{
			if(m_iType == 1)
				pTip->ParseWithPrice(QuickSellVector[index]);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
		}
	} 
	else
	{
		if (index < g_NPC.GetStorageMaxCount())
		{
			if (s_vGoodPosition[index] != 0)
			{
				CGood* tempgood = NULL;
				if (FindGoodById(s_vGoodPosition[index], tempgood) && tempgood != NULL)
				{
					pTip->Parse(*tempgood);

					int x = m_iScreenX + iX + 10;
					int y = m_iScreenY + iY + 10;
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
				}
				
				
			}
		}

	}

	
	
	return true;
}

int CNpcQuickWnd::GetIndex(int iX,int iY/*,int *pMouseOnIdx*/)
{
	if(iY <= m_iStartY || iY >= m_iStartY+m_iRows*(m_iCellH + m_iIntervalH))
		return -1;
	if(iX <= m_iStartX || iX >= m_iStartX+m_iCols*(m_iCellW+m_iIntervalW))
		return -1;


	int iRow = (iY-m_iStartY)/(m_iCellH + m_iIntervalH);
	int iCol = (iX-m_iStartX)/(m_iCellW+m_iIntervalW);
	iRow += m_pScroll->GetPos();

	int index = iRow * m_iCols + iCol;
	if (m_iType == 2)
	{
		index += GOODSNUMBERPERPAGE * m_TabPage.iCurPage;
		return index;
	} 
	else if (index >= 0 && index < g_NPC.GetNpcQuickVector().size() )
	{
		return index;
	}

	return -1;
	/*if (pMouseOnIdx)
	{
		*pMouseOnIdx = index;
		if (m_iType == 2)
		{
			*pMouseOnIdx += GOODSNUMBERPERPAGE * m_TabPage.iCurPage;
		}
	}
	if(index >= 0 && index < g_NPC.GetNpcQuickVector().size() )
	{
		if (m_iType == 2)
		{
			return index + GOODSNUMBERPERPAGE * m_TabPage.iCurPage;
		}
		else
			return index;
	}*/

}

void CNpcQuickWnd::BuyGoods()
{
	if(m_iType != 1)
		return;

	ClearDropGood();

	if(m_iSelectIndex < 0 || m_iSelectIndex >= g_NPC.GetNpcQuickVector().size())
		return;

	//up 音乐
	g_pAudio->Play(EAT_OTHER,8,g_pAudio->GetRand()++);

	DWORD dwNpcID = g_NPC.GetID();

	if(dwNpcID )
	{
		CGood &temp = g_NPC.GetNpcQuickVector()[m_iSelectIndex];
		if(temp.GetPayType())
		{
			//通过PT商城购买
			CQuickItem qitem;
			qitem.strName = temp.GetName();
			char strTemp[24]={0};
			sprintf(strTemp,"%u",temp.GetID());
			qitem.strItemID = strTemp;

			if(g_Config.GetCheckQuick())
			{
				g_WooolStoreMgr.GetTmpQuickItem() = qitem;
				g_pControl->PopupWindow(MSG_CTRL_CHECK_QUICK_WND,OPER_CREATE,100);
			}
			else
			{
				g_WooolStoreMgr.BuyQuickItem(qitem,false);
			}
		}
		else
		{
			g_pGameControl->SEND_Npc_Quick_Exchange(0,dwNpcID,temp.GetID(),temp.GetName());
		}
	}
}
void CNpcQuickWnd::CarryBackGoods()
{
	if(m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetStorageMaxCount())
	{
		DWORD goodid = s_vGoodPosition[m_iSelectIndex];
		if (goodid != 0)
		{
			CGood* temp = NULL;
			if (FindGoodById(goodid, temp) && temp != NULL)
			{
				g_pGameControl->SEND_Exchange_Getback_Ok(g_NPC.GetID(),temp->GetID(),temp->GetName(),1);
			}
		}

	}
	///
	/*if(m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetNpcQuickVector().size())
	{
		CGood &temp = g_NPC.GetNpcQuickVector()[m_iSelectIndex];
		g_pGameControl->SEND_Exchange_Getback_Ok(g_NPC.GetID(),temp.GetID(),temp.GetName(),1);
	}*/

	ClearDropGood();
}


bool CNpcQuickWnd::OnLeftButtonDClick(int iX,int iY)
{
	if(m_iType == 1)
	{
		BuyGoods();
	}
	else if(m_iType == 2)
	{
		
		//ClearDropGood();

		int index = GetIndex(iX,iY);
		if(index >= 0 && index < g_NPC.GetStorageMaxCount())
		{
			DWORD goodid = s_vGoodPosition[index];

			if( CGoodGrid::GetDropGoodFrom().DropGood.GetID() != 0 &&  goodid != 0 )     //鼠标上有东西的话再双击物品无效
			{
				return true;
			}	

			if (goodid == 0)
			{
				if (CGoodGrid::GetDropGoodFrom().eFromWnd == NpcQuickStorage_Wnd)
				{
					goodid = CGoodGrid::GetDropGoodFrom().DropGood.GetID();
					CGoodGrid::GetDropGoodFrom().DropGood.SetID(0);
				}
				else
					return true;
			}

			if (goodid != 0)
			{
				CGood* temp = NULL;
				if (FindGoodById(goodid, temp) && temp != NULL)
				{
					
					g_pGameControl->SEND_Exchange_Getback_Ok(g_NPC.GetID(),temp->GetID(),temp->GetName(),1);
				}

				
			}
			
		}
		else if (index >= g_NPC.GetStorageMaxCount())
		{
			//char strConfirm[128];
			//int needPay = 0;
			//if (g_NPC.GetStorageMaxCount() <= 40)//基础40,第一个10格-1元宝,第二个-10元宝,之后的-20元宝
			//{
			//	needPay = 1;
			//}
			//else if (g_NPC.GetStorageMaxCount() <= 50)
			//{
			//	needPay = 10;
			//} 
			//else
			//{
			//	needPay = 20;
			//}

			//sprintf(strConfirm,"点击确认获得此区域共10格扩展仓库的永久使用权，并自动扣除%d元宝，是否继续？",needPay);
			//g_MsgBoxMgr.PopOkCancelBox(strConfirm,MSG_CTRL_MORE_STORAGE_GRID,0);

			g_pControl->PopupWindow(MSG_CTRL_VIPCARD_WND,OPER_CREATE);
		}
	}

	return true;
}

bool CNpcQuickWnd::OnRightButtonUp(int iX,int iY)
{
	if(g_pGameData->GetMouseType() == MOUSE_REPAIR || g_pGameData->GetMouseType() == MOUSE_SPECIALREPAIR)
		g_pGameData->SetMouseType(MOUSE_STANDARD);

	ClearDropGood();

	m_iSelectIndex = GetIndex(iX,iY);

	if (m_iSelectIndex >= 0)
	{
			
		if ((m_iType == 1 && m_iSelectIndex < g_NPC.GetNpcQuickVector().size())
			|| (m_iType == 2 && m_iSelectIndex < g_NPC.GetStorageMaxCount()))
		{
			if(g_pControl->GetTipWnd())
				g_pControl->GetTipWnd()->SetShow(false);

			CCtrlMenuWnd* pMenuWnd = g_pControl->GetMenuWnd();
			pMenuWnd->Clear();

			S_POP_MENU_DATA & PopMenuData = pMenuWnd->GetData();
			PopMenuData.fItemHeight = 16.0f;
			PopMenuData.iTopOffY = 4;
			PopMenuData.iBottomOffY = 0;
			PopMenuData.iW = 80;


			if(m_iType == 1)
			{
				pMenuWnd->AddMenuItem(POP_MENU_ITEM_NPC_QUICK_BUY,"购买");
			}
			else
			{
				pMenuWnd->AddMenuItem(POP_MENU_ITEM_NPC_QUICK_CARRYBACK,"取回");
			}

			m_bClick = false;
			m_bRBClick = false;//否则MenuWnd获得不到焦点
			pMenuWnd->SetCaller(this);
			pMenuWnd->ShowMenu();
			return true;
		} 
		
	}
	


	//if(m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetNpcQuickVector().size())
	//{
	//	if(g_pControl->GetTipWnd())
	//		g_pControl->GetTipWnd()->SetShow(false);

	//	CCtrlMenuWnd* pMenuWnd = g_pControl->GetMenuWnd();
	//	pMenuWnd->Clear();

	//	S_POP_MENU_DATA & PopMenuData = pMenuWnd->GetData();
	//	PopMenuData.fItemHeight = 16.0f;
	//	PopMenuData.iTopOffY = 4;
	//	PopMenuData.iBottomOffY = 0;
	//	PopMenuData.iW = 80;
	//	

	//	if(m_iType == 1)
	//	{
	//		pMenuWnd->AddMenuItem(POP_MENU_ITEM_NPC_QUICK_BUY,"购买");
	//	}
	//	else
	//	{
	//		pMenuWnd->AddMenuItem(POP_MENU_ITEM_NPC_QUICK_CARRYBACK,"取回");
	//	}

	//	m_bClick = false;
	//	m_bRBClick = false;//否则MenuWnd获得不到焦点
	//	pMenuWnd->SetCaller(this);
	//	pMenuWnd->ShowMenu();
	//	return true;
	//}

	return CCtrlWindowX::OnRightButtonUp(iX,iY);
}

bool CNpcQuickWnd::OnWheel(int iWheel)
{
	return m_pScroll->OnWheel(iWheel);
}

void CNpcQuickWnd::UpdateScrollPos()
{
	int iRange = g_NPC.GetNpcQuickVector().size()/m_iCols;
	if(g_NPC.GetNpcQuickVector().size()%m_iCols != 0)
		iRange += 1;

	if (m_iType == 2)
	{
		iRange = 10;
	}
	iRange = iRange - m_iRows;
	if(iRange < 0)
		iRange = 0;

	m_pScroll->SetRange(iRange);
	if(m_pScroll->GetPos() > iRange)
	{
		int iNewPos = iRange - 1;
		if(iNewPos < 0)
			iNewPos = 0;

		m_pScroll->SetPos(iNewPos);
	}
}

void CNpcQuickWnd::OnClickBodyRepair()
{
	//快捷消费2.0
	int iPos = -1;
	for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
	{
		CGood& good = SELF.GetPackageGood(i);

		if(good.GetID() == 0)
			continue;

		if( (strcmp(good.GetName(),"超级冰泉圣水") == 0 && good.GetDura() > 0 ) || strcmp(good.GetName(),"冰泉圣水") == 0  )
		{
			iPos = i;
			break;
		}
	}

	if(iPos == -1)
	{
		g_WooolStoreMgr.SetQuickBuyItem("超级冰泉圣水");
		CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

		if(QuickBuyData.pItem)
		{
			char cTemp[128]={0};
			sprintf(cTemp,"您身上没有冰泉圣水或超级冰泉圣水，需要立即购买超级冰泉圣水并使用吗？单价%d元宝",
				QuickBuyData.pItem->iPrice);

			QuickBuyData.strMsg = cTemp;
			QuickBuyData.iType = 1;
			QuickBuyData.iUseAfterBuyType = 1;
			QuickBuyData.iObjPos = iPos;

			g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);
		}
		else
		{
			g_MsgBoxMgr.PopSimpleAlert("您身上没有冰泉圣水或超级冰泉圣水，请购买超级冰泉圣水配方炼制成功后使用。");
		}
	}
	else
	{
		g_pGameControl->SEND_Use_Object(iPos);
	}

}

void CNpcQuickWnd::ClearDropGood()
{
	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
	if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == NpcQuickSale_Wnd || GoodFrom.eFromWnd == NpcQuickStorage_Wnd) )
	{
		GoodFrom.DropGood.SetID(0);
		GoodFrom.eFromWnd = NO_Wnd;
		CGoodGrid::SetDropGoodFrom(GoodFrom);
	}
}
int CNpcQuickWnd::IsMouseOnTabPage(int x,int y)
{
	if(m_iType != 2)
		return -1;
	if (y > 36 && y < 54)
	{
		if (x >17 && x < 66)
		{
			return 0;
		}
		if (x >69 && x < 118)
		{
			return 1;
		}
		if (x >121 && x < 170)
		{
			return 2;
		}
	}
	return -1;
}


bool CNpcQuickWnd::FindGoodById(DWORD goodid, CGood*& result)
{

	for (std::vector<CGood>::iterator itrPackageGood = g_NPC.GetNpcQuickVector().begin(); itrPackageGood != g_NPC.GetNpcQuickVector().end(); ++itrPackageGood)
	{
		if (itrPackageGood->GetID() != goodid)
		{
			continue;
		}
		result = &(*itrPackageGood);
		return true;
	}
	return false;
}