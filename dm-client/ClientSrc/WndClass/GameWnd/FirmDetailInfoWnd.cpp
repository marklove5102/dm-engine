#include "FirmDetailInfoWnd.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/NpcData.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/GameGlobal.h"

INIT_WND_POSX(CFirmDetailInfoWnd,POS_VARIABLE,POS_VARIABLE)

CFirmDetailInfoWnd::CFirmDetailInfoWnd(void):
m_pConfirmSell(NULL),
m_pCancelSell(NULL)
{
	m_iRow = sm_dwWindowType;
	m_iIndex = 4616;
	m_pGoods.clear();
	m_iOffX = 60;
	m_iOffY = 250;
	m_warnMessage[0]=0;
	m_bRightGood = false;
}

CFirmDetailInfoWnd::~CFirmDetailInfoWnd(void)
{
	if (m_pGoods.GetID() != 0)
	{
		SELF.PackageGood().BackToArray(m_pGoods);
	}
}

void CFirmDetailInfoWnd::OnCreate()
{
	g_pControl->SetWindowPos((g_pGfx->GetWidth() > 800)?460:420,250,"PackageWnd");
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);

	SetCloseButton(376,1,80);

	m_pConfirmSell = new CCtrlButton();
	AddControl(m_pConfirmSell);
	m_pConfirmSell->CreateEx(this, 110,194, 90, 91, 92, 93);
	m_pConfirmSell->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);

	m_pCancelSell = new CCtrlButton();
	AddControl(m_pCancelSell);
	m_pCancelSell->CreateEx(this, 215,194, 90, 91, 92, 93);
	m_pCancelSell->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);

	CCtrlWindowX::OnCreate();
}

void CFirmDetailInfoWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	
	if (m_iRow < 0)
	{
		return;
	}
	_FirmData::_ItemListInfo itemInfo = g_NPC.GetFirmData().vecFirmItemList.at(m_iRow);
	char temp_str[1024];	
	sprintf(temp_str,"%s发布的消息",itemInfo.strPlayerName.c_str());
	g_pFont->DrawText(145+m_iScreenX,7+ m_iScreenY,temp_str,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);	

	if (itemInfo.nLevel != 0)
	{
		sprintf(temp_str,"%s收购%u阶%s",itemInfo.strPlayerName.c_str(),itemInfo.nLevel,itemInfo.strItemName.c_str());
	} 
	else
	{
		sprintf(temp_str,"%s收购%s",itemInfo.strPlayerName.c_str(),itemInfo.strItemName.c_str());
	}
	g_pFont->DrawText(120+m_iScreenX,45+ m_iScreenY,temp_str,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);

	if (m_pGoods.GetID() == 0)
	{
		char price_str[256];
		if (itemInfo.byTradeType == 0)//0 - 金币 1-元宝
		{
			sprintf(price_str,"每%u个%u金币",itemInfo.nNumPerUnit,itemInfo.dwPrice);
		}
		else if (itemInfo.byTradeType == 1)
		{
			sprintf(price_str,"每%u个%u元宝",itemInfo.nNumPerUnit,itemInfo.dwPrice);
		}

		sprintf(temp_str,"%s,总收购%u个", price_str,itemInfo.nNumPerUnit * itemInfo.nUnitNum);		
		g_pFont->DrawText(125+m_iScreenX,90+ m_iScreenY,temp_str,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
		g_pFont->DrawText(125+m_iScreenX,110+ m_iScreenY,"请在左侧窗口放入需要出售的物品",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
	}
	else
	{
		VString  vTipsStr;
		CutByUnicode(m_warnMessage,vTipsStr,10000,true);
		for(int i = 0; i < vTipsStr.size(); i++)
		{
			g_pFont->DrawText(100+m_iScreenX,100+ m_iScreenY + i * 16,vTipsStr[i].c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
		}
	}

	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_items,m_pGoods.GetLooks(),EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(54+m_iScreenX ,92 + m_iScreenY,pTex);
	}
}

bool CFirmDetailInfoWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if (pControl == m_pConfirmSell)
		{
			if (m_pGoods.GetID() == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("请放入物品");
				return true;
			}		
			if (!m_bRightGood)
			{
				g_MsgBoxMgr.PopSimpleAlert("放入物品与收购方要求不符，不能出售");
				return true;
			}		

			g_pGameControl->Send_SellItem(g_NPC.GetFirmData().vecFirmItemList.at(m_iRow).dwRowID,m_pGoods.GetID());
			SELF.PackageGood().BackToArray(m_pGoods);
			m_pGoods.clear();
			CloseWindow();
			return true;
		}
		if (pControl == m_pCancelSell)
		{
			SELF.PackageGood().BackToArray(m_pGoods);
			m_pGoods.clear();
			CloseWindow();
			return true;
		}
		
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CFirmDetailInfoWnd::OnLeftButtonUp(int iX,int iY)
{
	stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();
	if (GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Package_Wnd)
	{
		if (IsInSellGoodsArea(iX,iY))
		{
			if (m_pGoods.GetID() != 0)//有物品
			{
				CGood temp = m_pGoods;
				m_pGoods = GoodFrom.DropGood;
				GoodFrom.DropGood = temp;
				warnMessage();
				return true;
			} 
			else
			{
				m_pGoods = GoodFrom.DropGood;
				GoodFrom.DropGood.SetID(0);
				warnMessage();
				return true;
			}
			
		}
	}
	else if (GoodFrom.DropGood.GetID() == 0)
	{
		if (IsInSellGoodsArea(iX,iY))
		{
			GoodFrom.DropGood = m_pGoods;
			GoodFrom.eFromWnd = Package_Wnd;
			m_pGoods.SetID(0);
			warnMessage();
			return true;
		}
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CFirmDetailInfoWnd::IsInSellGoodsArea(int iX,int iY)
{
	if (iX < 50 || iX > 86 || iY < 90 || iY > 126)
	{
		return false;
	}
	return true;
}
void	CFirmDetailInfoWnd::warnMessage()
{	
	m_bRightGood = true;

	bool bRightType = true;

	if (m_pGoods.GetID() == 0)
	{
		m_bRightGood = false;
		sprintf(m_warnMessage,"");
		return;
	}

	bool bCanOverlap = false;

	_FirmData::_ItemListInfo tempItemInfo = g_NPC.GetFirmData().vecFirmItemList.at(m_iRow);
	vector<_FirmData::_ItemNameID> & VItemNameID = g_NPC.GetFirmData().vecItemNameID;
	for(int i = 0; i < VItemNameID.size(); i++)
	{
		if (VItemNameID[i].dwItemID == tempItemInfo.dwItemIdx)
		{
			bCanOverlap = VItemNameID[i].bCanOverlap;
			break;
		}
	}


	if (strcmp(m_pGoods.GetName(), tempItemInfo.strItemName.c_str()) != 0 )
	{
		m_bRightGood = false;
		bRightType = false;
	}
	else if (tempItemInfo.nLevel > 0 && ((m_pGoods.GetDura()) / 10000 + 1) != tempItemInfo.nLevel)//矿石
	{
		m_bRightGood = false;
	}
	else if (tempItemInfo.nLevel > 0 && m_pGoods.GetAC() < tempItemInfo.nNumPerUnit)//矿石
	{
		m_bRightGood = false;
	}
	else if (tempItemInfo.nLevel == 0 && bCanOverlap && m_pGoods.GetDura() < tempItemInfo.nNumPerUnit)//封元神力果
	{
		m_bRightGood = false;
	}
	//其它tempItemInfo.nLevel == 0 离火令,金条,金砖

	if (!bRightType)
	{
		sprintf(m_warnMessage,"放入物品与收购方要求不符，不能出售.");
	}
	else if (!m_bRightGood)
	{
		sprintf(m_warnMessage,"放入物品数量与收购方要求不符，不能出售.");
	}
	else//正确,计算数量,
	{
		int units = 1;
		
		if(tempItemInfo.nLevel)//矿石
			units = m_pGoods.GetAC() / tempItemInfo.nNumPerUnit;
		else if (tempItemInfo.nLevel == 0 && bCanOverlap)//封元神力果
			units = m_pGoods.GetDura() / tempItemInfo.nNumPerUnit;
		//else//"离火令","金条","金砖"

		units = min(units,tempItemInfo.nUnitNum);
		int number = units * tempItemInfo.nNumPerUnit;
		int price = units * tempItemInfo.dwPrice;
		char temp[256] = {0};

		if (tempItemInfo.nLevel != 0)//有阶数
		{
			sprintf(temp,"%u阶%s",tempItemInfo.nLevel,tempItemInfo.strItemName.c_str());
		}
		else
		{
			sprintf(temp,"%s",tempItemInfo.strItemName.c_str());
		}

		if (tempItemInfo.byTradeType == 0)//金币
		{
			sprintf(m_warnMessage,"本次出售%s%u个\n共获得%u金币",temp,number,price);
		} 
		else//元宝
		{
			sprintf(m_warnMessage,"本次出售%s%u个\n共获得%u元宝",temp,number,price);
		}
	}
}