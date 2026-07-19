#include "FirmWnd.h"
#include "GameData/GameData.h"
#include "GameData/LoginData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/NpcData.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Baseclass/Control/ParserTip.h"
#include "Global/Interface/AudioInterface.h"
#include "BaseClass/Control/CtrlMenuWnd.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TalkMgr.h"


INIT_WND_POSX(CFirmWnd,POS_VARIABLE,POS_VARIABLE)


#define OFFX_TABPAGE 21     //加了页签后导致整个的位置偏移量


CFirmWnd::CFirmWnd(void):
m_pItemGrid(NULL),
m_pMBKindOfItem(NULL),
m_pGoLastPage(NULL),
m_pGoNextPage(NULL),
m_pENumPerUnit(NULL),
m_pEUnitPrice(NULL),
m_pConfirmSend(NULL),
m_pTakeSellGoods(NULL),
m_pTakeSellMoney(NULL),
m_pTakeBuyGoods(NULL),
m_pTakeBuyMoney(NULL),
m_pConfirmSell(NULL),
m_pCancelSell(NULL),
m_pReturnButton(NULL)
{
	m_iVersion = 2;	


	m_iSelectiRow = m_iSelectCol = -1;
	m_iStartX = 101;
	m_iStartY = 55;
	m_iCellW = 37;
	m_iCellH = 37;
	m_iIntervalW = 2;
	m_iIntervalH = 2;
	m_iRows = 5;
	m_iCols = 10;



	S_TabPage* pFirmPage = AddTabPage(0,0,MAKELONG(4621,PACKAGE_INTERFACE),0,35,85,86,87,88,"商 行",NULL,true);


	AddTabPage(0,0,MAKELONG(4621,PACKAGE_INTERFACE),32,35,4617,4618,4619,4620,"查看收购信息",pFirmPage,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);//点第一个和第三个页签要发消息,所以不是立即切换
	AddTabPage(0,0,MAKELONG(4614,PACKAGE_INTERFACE),140,35,4617,4618,4619,4620,"发布收购信息",pFirmPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(4613,PACKAGE_INTERFACE),248,35,4617,4618,4619,4620,"取消收购信息",pFirmPage,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);//点第一个和第三个页签要发消息,所以不是立即切换
	AddTabPage(0,0,MAKELONG(4615,PACKAGE_INTERFACE),356,35,4617,4618,4619,4620,"领取收购/出售所得",pFirmPage,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);

	S_TabPage* pTradePage = AddTabPage(0,0,MAKELONG(4623,PACKAGE_INTERFACE),0,120,85,86,87,88,"交易行",NULL,true);

  
	m_TabPage.iCurPage = sm_dwWindowType;
	
	m_pGoods.clear();
	m_iAlignType = XAL_CENTER;

	m_dwLastRequireTime = 0;

	m_pVTradeGood = &(g_NPC.GetVipTradeGood());
}

CFirmWnd::~CFirmWnd(void)
{
	if (m_pGoods.GetID() != 0)
	{
		SELF.PackageGood().BackToArray(m_pGoods);
	}

	g_NPC.GetVipTradeGood().Clear();
	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

	if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == VipTrade_Wnd)
	{
		CGoodGrid::ClearGoodFrom();
	}
}

bool CFirmWnd::Create(CControl* pParent,int iCurPage)
{
	if (g_Login.GetLoginInExpType() == 1)
	{
		g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_RECREATE);
		return false;
	}

	return CCtrlWindowX::Create(pParent, iCurPage);
}


void CFirmWnd::OnCreate()
{
	SetCloseButton(495 + OFFX_TABPAGE,4,80);

	m_pItemGrid = NULL;
	m_pMBKindOfItem = NULL;
	m_pGoLastPage = NULL;
	m_pGoNextPage = NULL;
	m_pConfirmSell = NULL;
	m_pCancelSell = NULL;
	m_pReturnButton = NULL;
	m_pENumPerUnit = NULL;
	m_pEUnitPrice = NULL;
	m_pMBTradeType = NULL;
	m_pEUnitNum = NULL;
	m_pConfirmSend = NULL;
	for (int i = 0; i < REMOVEITEMNUMBER;i++)
	{
		m_pRemoveItem[i] = NULL;
		m_pDetailItem[i] = NULL;
	}
	m_pTakeSellGoods = NULL;	
	m_pTakeSellMoney = NULL;	
	m_pTakeBuyGoods = NULL;	
	m_pTakeBuyMoney = NULL;	
	m_pTradeBuyBtn = NULL;
	m_pScroll = NULL;
	m_pOnlyShowMySelf = NULL;


	int iCurPage =  m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;


	m_pMBKindOfItem = NULL;
	if (iCurPage == 0)
	{
		//收购信息
		if (iSubPage == 0)
		{
			m_pMBTradeType = new CCtrlMenuButton();
			AddControl(m_pMBTradeType);
			m_pMBTradeType->Create(this,408 + OFFX_TABPAGE,63,95,55/*,0,0,0,0,0,1030,1031,1032,1033,1,18,true*/);
			m_pMBTradeType->SetFont(FONT_YAHEI);
			m_pMBTradeType->SetMenuTextColor(0xFFFFFFFF);
			m_pMBTradeType->AddString("金币");
			m_pMBTradeType->AddString("元宝");
			m_pMBTradeType->SetCurSel(1);

			m_pMBKindOfItem = new CCtrlMenuButton();//下拉菜单,选择收购种类
			AddControl(m_pMBKindOfItem);
			m_pMBKindOfItem->Create(this,335 + OFFX_TABPAGE,289,145,150/*,0,0,0,0,0,1030,1031,1032,1033,1,18,true*/);
			m_pMBKindOfItem->SetFont(FONT_YAHEI);
			m_pMBKindOfItem->SetMenuTextColor(0xFFFFFFFF);
			m_pMBKindOfItem->SetText("查看收购物品的种类");

			char temp_str[256] = {0};
			vector<_FirmData::_ItemNameID>::iterator iter = g_NPC.GetFirmData().vecItemNameID.begin();

			for(; iter != g_NPC.GetFirmData().vecItemNameID.end(); ++iter)
			{
				if (iter->nLevel != 0)
				{
					sprintf(temp_str,"%u阶%s",iter->nLevel,iter->strItemName.c_str());
				}
				else
				{
					sprintf(temp_str,"%s",iter->strItemName.c_str());
				}
				m_pMBKindOfItem->AddString(temp_str);
			}

			m_pItemGrid = new CCtrlGrid();//收购信息列表
			AddControl(m_pItemGrid);
			m_pItemGrid->Create(this,20 + OFFX_TABPAGE,84,446,272,9,21.0f);//注意参数
			//m_pItemGrid->AddScrollEx(444,0,16,244);
			m_pItemGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);
			m_pItemGrid->SetTextColor(COLOR_TEXT_NORMAL,0xFFFF0000);


			m_pItemGrid->SetSelAllCols(true);
			m_pItemGrid->PushColumn(100);
			m_pItemGrid->PushColumn(100);
			m_pItemGrid->PushColumn(80);
			m_pItemGrid->PushColumn(140);
			//m_pItemGrid->SetTotalCount(8);//需要判断一下,是否需要滚动条
			m_pItemGrid->SetDrawOffXY(0,6);

			m_pGoLastPage = new CCtrlButton();
			AddControl(m_pGoLastPage);
			m_pGoLastPage->CreateEx(this, 175 + OFFX_TABPAGE,288, 95,96,97,98);
			m_pGoLastPage->SetText("上一页", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

			m_pGoNextPage = new CCtrlButton();
			AddControl(m_pGoNextPage);
			m_pGoNextPage->CreateEx(this, 265 + OFFX_TABPAGE,288, 95,96,97,98);
			m_pGoNextPage->SetText("下一页", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

			int vecSize = g_NPC.GetFirmData().vecFirmItemList.size();
			for (int i = 0; i != REMOVEITEMNUMBER; ++i)
			{
				m_pDetailItem[i] = new CCtrlButton();
				AddControl(m_pDetailItem[i]);
				m_pDetailItem[i]->CreateEx(this, 430,85 + i* 21, 95,96,97,98);
				m_pDetailItem[i]->SetText("出售", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
				if (i >= vecSize)
				{
					m_pDetailItem[i]->SetEnable(false);
				}
			}
		}
		else if (iSubPage == 1)
		{
			m_message = "请选择您要收购的物品";

			m_pMBKindOfItem = new CCtrlMenuButton();//下拉菜单,选择收购种类
			AddControl(m_pMBKindOfItem);
			m_pMBKindOfItem->Create(this,40 + OFFX_TABPAGE,83,146,150/*,0,0,0,0,0,1030,1031,1032,1033,1,18,true*/);
			m_pMBKindOfItem->SetFont(FONT_YAHEI);
			m_pMBKindOfItem->SetMenuTextColor(0xFFFFFFFF);
			m_pMBKindOfItem->SetText("选择需要收购的物品");

			char temp_str[256] = {0};
			vector<_FirmData::_ItemNameID>::iterator iter = g_NPC.GetFirmData().vecItemNameID.begin();

			for(; iter != g_NPC.GetFirmData().vecItemNameID.end(); ++iter)
			{
				if (iter->nLevel != 0)
				{
					sprintf(temp_str,"%u阶%s",iter->nLevel,iter->strItemName.c_str());
				}
				else
				{
					sprintf(temp_str,"%s",iter->strItemName.c_str());
				}
				m_pMBKindOfItem->AddString(temp_str);
			}

			m_pENumPerUnit = new CCtrlEdit();
			AddControl(m_pENumPerUnit);
			m_pENumPerUnit->CreateEx(this,215 + OFFX_TABPAGE,83,45,19);
			m_pENumPerUnit->SetText("0",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_DISABLE,FONTSIZE_DEFAULT,0,FONT_YAHEI);
			m_pENumPerUnit->SetDigital(true);
			m_pENumPerUnit->SetMaxLength(4);
			m_pENumPerUnit->SetText("0");

			m_pEUnitPrice = new CCtrlEdit();
			AddControl(m_pEUnitPrice);
			m_pEUnitPrice->CreateEx(this,280 + OFFX_TABPAGE,83,45,19);
			m_pEUnitPrice->SetText("0",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_DISABLE,FONTSIZE_DEFAULT,0,FONT_YAHEI);
			m_pEUnitPrice->SetDigital(true);
			m_pEUnitPrice->SetMaxLength(7);
			m_pEUnitPrice->SetText("0");

			m_pMBTradeType = new CCtrlMenuButton();
			AddControl(m_pMBTradeType);
			m_pMBTradeType->Create(this,326 + OFFX_TABPAGE,84,47,55/*,0,0,0,0,0,1030,1031,1032,1033,1,18,true*/);
			m_pMBTradeType->SetFont(FONT_YAHEI);
			m_pMBTradeType->SetMenuTextColor(0xFFFFFFFF);
			m_pMBTradeType->AddString("金币");
			m_pMBTradeType->AddString("元宝");
			m_pMBTradeType->SetCurSel(0);


			m_pEUnitNum = new CCtrlEdit();
			AddControl(m_pEUnitNum);
			m_pEUnitNum->CreateEx(this,427 + OFFX_TABPAGE,83,116,18);
			m_pEUnitNum->SetText("0",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_DISABLE,FONTSIZE_DEFAULT,0,FONT_YAHEI);
			m_pEUnitNum->SetDigital(true);
			m_pEUnitNum->SetMaxLength(4);
			m_pEUnitNum->SetText("0");

			m_pConfirmSend = new CCtrlButton();
			AddControl(m_pConfirmSend);
			m_pConfirmSend->CreateEx(this, 226 + OFFX_TABPAGE,292, 132, 133, 134, 135);
			m_pConfirmSend->SetText("确认发布", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		}
		else if (iSubPage == 2)//取消收购信息
		{
			int vecSize = g_NPC.GetFirmData().vecFirmItemList.size();
			for (int i = 0; i != REMOVEITEMNUMBER; ++i)
			{
				m_pRemoveItem[i] = new CCtrlButton();
				AddControl(m_pRemoveItem[i]);
				m_pRemoveItem[i]->CreateEx(this, 430 + OFFX_TABPAGE,85 + i* 21, 95,96,97,98);
				m_pRemoveItem[i]->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
				if (i >= vecSize)
				{
					m_pRemoveItem[i]->SetEnable(false);
				}
			}

			m_pGoLastPage = new CCtrlButton();
			AddControl(m_pGoLastPage);
			m_pGoLastPage->CreateEx(this, 175 + OFFX_TABPAGE,288, 95,96,97,98);
			m_pGoLastPage->SetText("上一页", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

			m_pGoNextPage = new CCtrlButton();
			AddControl(m_pGoNextPage);
			m_pGoNextPage->CreateEx(this, 265 + OFFX_TABPAGE,288, 95,96,97,98);
			m_pGoNextPage->SetText("下一页", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		}
		else if (iSubPage == 3)//商铺
		{
			m_pTakeSellGoods = new CCtrlButton();
			AddControl(m_pTakeSellGoods);
			m_pTakeSellGoods->CreateEx(this, 360 + OFFX_TABPAGE,162, 95,96,97,98);
			m_pTakeSellGoods->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);


			m_pTakeSellMoney = new CCtrlButton();
			AddControl(m_pTakeSellMoney);
			m_pTakeSellMoney->CreateEx(this, 360 + OFFX_TABPAGE,260, 95,96,97,98);
			m_pTakeSellMoney->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);


			m_pTakeBuyGoods = new CCtrlButton();
			AddControl(m_pTakeBuyGoods);
			m_pTakeBuyGoods->CreateEx(this, 108 + OFFX_TABPAGE,162, 95,96,97,98);
			m_pTakeBuyGoods->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

			m_pTakeBuyMoney = new CCtrlButton();
			AddControl(m_pTakeBuyMoney);
			m_pTakeBuyMoney->CreateEx(this, 108 + OFFX_TABPAGE,260, 95,96,97,98);
			m_pTakeBuyMoney->SetText("领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		}
	}
	else if (iCurPage == 1)
	{
		m_pTradeBuyBtn = new CCtrlButton(); 
		AddControl(m_pTradeBuyBtn);
		m_pTradeBuyBtn->CreateEx(this, 449,289, 95, 96, 97, 98);
		m_pTradeBuyBtn->SetText("购买", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	
		m_pScroll = new CCtrlScroll();
		AddControl(m_pScroll);
		m_pScroll->CreateEx(this,499,50,14,252 - 51);

		if (SELF.GetVipTradeLevel() > 0)
		{
			m_pOnlyShowMySelf = new CCtrlRadio();
			AddControl(m_pOnlyShowMySelf);
			m_pOnlyShowMySelf->Create(this,395,260,125,126,127,128,50,20);
			m_pOnlyShowMySelf->SetText("只显示自己出售物品",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		}
		else
		{
			m_pOnlyShowMySelf = NULL;
		}
	}

	CCtrlWindowX::OnCreate();
}
void CFirmWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iCurPage =  m_TabPage.iCurPage;
	int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

	if (iCurPage == 0)
	{
		g_pFont->DrawText(m_iScreenX + 238 + OFFX_TABPAGE,10+ m_iScreenY,"商   行",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
	
		if (iSubPage == 0)
		{
			int vecSize = g_NPC.GetFirmData().vecFirmItemList.size();
			for (int i = 0; i != REMOVEITEMNUMBER; ++i)
			{
				if (i >= vecSize)
				{
					m_pDetailItem[i]->SetEnable(false);
				}
				else
				{
					m_pDetailItem[i]->SetEnable(true);
				}
			}

			if (g_NPC.GetFirmData().bEnd)
			{
				m_pGoNextPage->SetEnable(false);
			}
			else
			{
				m_pGoNextPage->SetEnable(true);
			}
			if (g_NPC.GetFirmData().iCurPage == 1)
			{
				m_pGoLastPage->SetEnable(false);
			}
			else
			{
				m_pGoLastPage->SetEnable(true);
			}

			g_pFont->DrawText(m_iScreenX + 41 + OFFX_TABPAGE,64+ m_iScreenY,"玩家姓名",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			g_pFont->DrawText(m_iScreenX + 138 + OFFX_TABPAGE,64+ m_iScreenY,"收购物品",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			g_pFont->DrawText(m_iScreenX + 226 + OFFX_TABPAGE,64+ m_iScreenY,"收购数量",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			g_pFont->DrawText(m_iScreenX + 320 + OFFX_TABPAGE,64+ m_iScreenY,"收购价格",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			if(m_pMBKindOfItem != NULL)
				m_pMBKindOfItem->Draw();

			if(m_pItemGrid)
			{
				vector<_FirmData::_ItemListInfo>::iterator iter = g_NPC.GetFirmData().vecFirmItemList.begin();

				char temp[256] = {0};
				int i = 0;
				for(; iter != g_NPC.GetFirmData().vecFirmItemList.end(); ++iter)
				{
					DWORD dwColor = 0;
					if (iter->byTradeType != 0)//元宝
					{
						dwColor = 0xFFFFFF00;
					}

					m_pItemGrid->DrawGrid(i,0,iter->strPlayerName.c_str(),false,dwColor);	//玩家名字

					if (iter->nLevel != 0)//存在阶数
					{
						sprintf(temp,"%u阶%s",iter->nLevel,iter->strItemName.c_str());
					}
					else
					{
						sprintf(temp,"%s",iter->strItemName.c_str());
					}
					m_pItemGrid->DrawGrid(i,1,temp,false,dwColor);//物品名字

					DWORD totalCount = iter->nNumPerUnit * iter->nUnitNum;
					sprintf(temp,"%u个",totalCount);
					m_pItemGrid->DrawGrid(i,2,temp,false,dwColor);//收购个数

					if (iter->byTradeType == 0)//0 - 金币 1-元宝
					{
						sprintf(temp,"每%u个%u金币",iter->nNumPerUnit,iter->dwPrice);
					}
					else if (iter->byTradeType == 1)
					{
						sprintf(temp,"每%u个%u元宝",iter->nNumPerUnit,iter->dwPrice);
					}
					m_pItemGrid->DrawGrid(i,3,temp,false,dwColor);//收购价格
					++i;
				}
			}
		}
		else if (iSubPage == 1)
		{
			g_pFont->DrawText(m_iScreenX + 197 + OFFX_TABPAGE,84+ m_iScreenY,"每",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
			g_pFont->DrawText(m_iScreenX + 263 + OFFX_TABPAGE,84+ m_iScreenY,"个",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
			g_pFont->DrawText(m_iScreenX + 378 + OFFX_TABPAGE,84+ m_iScreenY,"总共收购",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
			g_pFont->DrawText(m_iScreenX + 475 + OFFX_TABPAGE,84+ m_iScreenY,"份",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);


			int iCurSel = m_pMBKindOfItem->GetCurSel();
			if (iCurSel >= 0)
			{
				char temp_str[256] = {0};
				sprintf(temp_str,"您准备收购%s。",m_pMBKindOfItem->GetSelText());
				g_pFont->DrawText(m_iScreenX + 55 + OFFX_TABPAGE,137+ m_iScreenY,temp_str,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);

				DWORD selectItemID = g_NPC.GetFirmData().vecItemNameID.at(iCurSel).dwItemID;
				DWORD Level = g_NPC.GetFirmData().vecItemNameID.at(iCurSel).nLevel;
				DWORD numPerUnit = m_pENumPerUnit->GetNum();
				DWORD price = m_pEUnitPrice->GetNum();
				BYTE  tradeType = m_pMBTradeType->GetCurSel();
				DWORD unitNum = m_pEUnitNum->GetNum();

				if (g_NPC.GetFirmData().vecItemNameID.at(iCurSel).bCanOverlap)
				{
					m_pENumPerUnit->SetEnable(true);
				}
				else
				{
					m_pENumPerUnit->SetText("1");
					m_pENumPerUnit->SetEnable(false);
				}

				DWORD64 total = price*unitNum;

				if (m_pMBTradeType->GetCurSel() == 0)//金币
				{
					sprintf(temp_str,"每%u个%u金币，总共收购%u份。",numPerUnit,price,unitNum);
					g_pFont->DrawText(m_iScreenX + 55 + OFFX_TABPAGE,157+ m_iScreenY,temp_str,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
					sprintf(temp_str,"您需要预先支付收购金额%I64d金币，取消收购信息可取回剩余的预付金额。", total);
					g_pFont->DrawText(m_iScreenX + 55 + OFFX_TABPAGE,177+ m_iScreenY,temp_str,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
				} 
				else//元宝
				{
					sprintf(temp_str,"每%u个%u元宝，总共收购%u份。",numPerUnit,price,unitNum);
					g_pFont->DrawText(m_iScreenX + 55 + OFFX_TABPAGE,157+ m_iScreenY,temp_str,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
					sprintf(temp_str,"您需要预先支付收购金额%I64d元宝，取消收购信息可取回剩余的预付金额。", total);
					g_pFont->DrawText(m_iScreenX + 55 + OFFX_TABPAGE,177+ m_iScreenY,temp_str,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
				}			
				g_pFont->DrawText(m_iScreenX + 55 + OFFX_TABPAGE,197+ m_iScreenY,"发布此收购信息需要支付100000金币。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
				g_pFont->DrawText(m_iScreenX + 55 + OFFX_TABPAGE,217+ m_iScreenY,"此信息有效期为3天。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
			}
			else
			{
				g_pFont->DrawText(m_iScreenX + 210 + OFFX_TABPAGE,177+ m_iScreenY,m_message.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
			}

			if(m_pMBKindOfItem != NULL)
				m_pMBKindOfItem->Draw();

		}
		else if (iSubPage == 2)
		{
			int vecSize = g_NPC.GetFirmData().vecFirmItemList.size();
			for (int i = 0; i != REMOVEITEMNUMBER; ++i)
			{
				if (i >= vecSize)
				{
					m_pRemoveItem[i]->SetEnable(false);
				}
				else
				{
					m_pRemoveItem[i]->SetEnable(true);
				}
			}
			if (g_NPC.GetFirmData().bEnd == 1)
			{
				m_pGoNextPage->SetEnable(false);
			}
			else
			{
				m_pGoNextPage->SetEnable(true);
			}
			if (g_NPC.GetFirmData().iCurPage == 1)
			{
				m_pGoLastPage->SetEnable(false);
			}
			else
			{
				m_pGoLastPage->SetEnable(true);
			}
			char goodsName[256] = {0};
			char Price[256] = {0};
			char FinalStr[256] = {0};
			char tempTitle[256] = {0};

			if (vecSize > 0)
			{
				sprintf(tempTitle,"%s发布的收购信息",g_NPC.GetFirmData().vecFirmItemList.front().strPlayerName.c_str());
				g_pFont->DrawText(182+m_iScreenX + OFFX_TABPAGE,63+ m_iScreenY,tempTitle,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			}

			for(int i = 0; i != vecSize; ++i)
			{
				_FirmData::_ItemListInfo* SelfPurchaseInfo = &(g_NPC.GetFirmData().vecFirmItemList.at(i));
				if (SelfPurchaseInfo->nLevel != 0)
				{
					sprintf(goodsName,"%u阶%s",SelfPurchaseInfo->nLevel,SelfPurchaseInfo->strItemName.c_str());
				} 
				else
				{
					sprintf(goodsName,"%s",SelfPurchaseInfo->strItemName.c_str());
				}

				if (SelfPurchaseInfo->byTradeType == 0)//金币
				{
					sprintf(Price,"%u金币",SelfPurchaseInfo->dwPrice);
				}
				else
				{
					sprintf(Price,"%u元宝",SelfPurchaseInfo->dwPrice);
				}

				sprintf(FinalStr,"%s,每%u个%s,总共收购%u个即%u份.",goodsName,
					SelfPurchaseInfo->nNumPerUnit,Price,SelfPurchaseInfo->nNumPerUnit * SelfPurchaseInfo->nUnitNum,
					SelfPurchaseInfo->nUnitNum);
				g_pFont->DrawText(42+m_iScreenX + OFFX_TABPAGE,90 + i* 21+ m_iScreenY,FinalStr,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);

			}
		}
		else if (iSubPage == 3)
		{
			g_pFont->DrawText(82+m_iScreenX + OFFX_TABPAGE,126+ m_iScreenY,"领取收购所得物品",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
			g_pFont->DrawText(82+m_iScreenX + OFFX_TABPAGE,224+ m_iScreenY,"领取收购预付金额",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
			g_pFont->DrawText(332+m_iScreenX + OFFX_TABPAGE,126+ m_iScreenY,"领取商铺寄卖物品",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
			g_pFont->DrawText(332+m_iScreenX + OFFX_TABPAGE,224+ m_iScreenY,"领取商铺寄卖所得",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
		}

	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 260,10+ m_iScreenY,"交易行",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
		
		int iPos = m_pScroll->GetPos();
		int iCount = m_pVTradeGood->iVipPersons;

		int iPrintX = m_iScreenX + m_iStartX;
		int iPrintY = m_iScreenY + m_iStartY;


		//画选中背景框
		if(m_iSelectiRow >= 0 && m_iSelectCol >= 0 && m_iSelectiRow < iCount)
		{
			MVipGood::iterator itr = m_pVTradeGood->mMVipGood.find(m_iSelectiRow);
			if (itr != m_pVTradeGood->mMVipGood.end())
			{
				vector<CGood> &VGood = itr->second.VGoods;
				if (m_iSelectCol < VGood.size())
				{
					int iSelRow = m_iSelectiRow - iPos;
					int iSelCol = m_iSelectCol;

					if(iSelRow >= 0 && iSelRow < PERONS_PER_PAGE_VIPTRADEWND)
					{
						int iLeft = iSelCol*(m_iCellW+m_iIntervalW)+iPrintX;
						int iTop = iSelRow*(m_iCellH + m_iIntervalH)+iPrintY;

						g_pGfx->DrawFillRect(iLeft,iTop,m_iCellW,m_iCellH,0x44FF0000);
					}
				}
			}
		}
		//画物品
		for(int i = iPos,iRow = 0; i < iCount && iRow < PERONS_PER_PAGE_VIPTRADEWND; i ++,iRow ++)
		{
			MVipGood::iterator itr = m_pVTradeGood->mMVipGood.find(i);
			if (itr != m_pVTradeGood->mMVipGood.end())
			{
				int iVipLevel = GetVipLevelByExp(itr->second.iVipExp);
				if (iVipLevel > 0)
				{
					LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,17464 + iVipLevel,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + 68,m_iScreenY + 63 + iRow * (m_iCellH + m_iIntervalH),pTex);
				}

				vector<CGood> & VGoods = itr->second.VGoods;
				int iGoodNum = VGoods.size();
				for (int iCol = 0; iCol < 10 && iCol < iGoodNum; iCol++)
				{
					CGoodGrid::DrawOneGood(iPrintX + 19 + iCol*(m_iCellW+m_iIntervalW),iPrintY + 18 + iRow * (m_iCellH + m_iIntervalH),VGoods.at(iCol));
				}
			}
			else if(GetTickCount() - m_dwLastRequireTime > 2000)
			{
				m_dwLastRequireTime = GetTickCount();
				g_pGameControl->SEND_Player_Vip_Option(1,0,i / PERONS_PER_PAGE_VIPTRADEWND);//请求商行出售物品数据
				break;
			}
		}
	}
	
}
bool CFirmWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_FIRM_WND:
		{			
			if(dwData == 1)//刷新滚动条位置
			{
				UpdateScrollPos();
				return true;
			}
			else if (dwData == 10)
			{
				m_message = "恭喜您成功发布收购信息";
				m_pMBKindOfItem->SetCurSel(-1);
				m_pMBKindOfItem->SetText("选择需要收购的物品");
				m_pENumPerUnit->SetText("0");
				m_pEUnitPrice->SetText("0");
				m_pMBTradeType->SetCurSel(0);
				m_pEUnitNum->SetText("0");

				return true;
			}
			return false;
		}
		break;
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			S_TabPage * pParentTabpage = (S_TabPage * )pControl;
			if (pParentTabpage == &(m_TabPage.vSubTabPage[0]))
			{
				if (dwData == 0)//0页签
				{
					g_pGameControl->Send_Purchase_Request(0,1,1,2,0,1);
					g_NPC.GetFirmData().vecFirmItemList.clear();
				}
				else if (dwData == 2)//2页签
				{
					g_pGameControl->Send_Purchase_Request(0,1,2,1);
					g_NPC.GetFirmData().vecFirmItemList.clear();
				}
			}
			return false;
		}
		break;
	case MSG_CTRL_MENU_SELCHANGED:
		{
			int iCurPage =  m_TabPage.iCurPage;
			int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

			if (iCurPage == 0 && iSubPage == 0)
			{
				if(pControl == m_pMBKindOfItem)
				{
					int iCurSel = m_pMBKindOfItem->GetCurSel();
					g_pGameControl->Send_Purchase_Request(g_NPC.GetFirmData().vecItemNameID.at(iCurSel).dwItemID,1,2,2
						,(BYTE)(g_NPC.GetFirmData().vecItemNameID.at(iCurSel).nLevel),m_pMBTradeType->GetCurSel());
				}
				else if(pControl == m_pMBTradeType)
				{
					int iCurSel = m_pMBKindOfItem->GetCurSel();

					if (iCurSel == -1)
					{
						g_pGameControl->Send_Purchase_Request(0,1,1,2,0,m_pMBTradeType->GetCurSel());
					} 
					else
					{
						g_pGameControl->Send_Purchase_Request(g_NPC.GetFirmData().vecItemNameID.at(iCurSel).dwItemID,1,2,2,(BYTE)(g_NPC.GetFirmData().vecItemNameID.at(iCurSel).nLevel),m_pMBTradeType->GetCurSel());
					}
				}
			}
		}
		break;
	case MSG_CTRL_GRID_DBCLICK:                //CtrlGrid双击某行某列,dwData低word为行号,高word为列号
		{
			int iCurPage =  m_TabPage.iCurPage;
			int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

			if (iCurPage == 0 && iSubPage == 0)
			{
				g_pControl->PopupWindow(MSG_CTRL_FIRMDETAIL_WND,OPER_RECREATE,LOWORD(dwData));
			}	
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			int iCurPage =  m_TabPage.iCurPage;
			int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

			if (iCurPage == 0)
			{
				if (iSubPage == 0)
				{
					for (int i = 0; i != REMOVEITEMNUMBER; ++i)
					{
						if (pControl == m_pDetailItem[i])
						{
							g_pControl->PopupWindow(MSG_CTRL_FIRMDETAIL_WND,OPER_RECREATE,i);
							return true;
						}
					}
					WORD CurPage = g_NPC.GetFirmData().iCurPage;
					int iCurSel = m_pMBKindOfItem->GetCurSel();
					DWORD selectItemID = 0;
					DWORD level = 0;
					if (iCurSel != -1)
					{
						selectItemID = g_NPC.GetFirmData().vecItemNameID.at(iCurSel).dwItemID;
						level = g_NPC.GetFirmData().vecItemNameID.at(iCurSel).nLevel;
					} 

					if (pControl == m_pGoNextPage)//下一页
					{
						CurPage++;
						if (selectItemID == 0)
						{
							g_pGameControl->Send_Purchase_Request(selectItemID,CurPage,1,2,(BYTE)level,m_pMBTradeType->GetCurSel());
						}
						else
						{
							g_pGameControl->Send_Purchase_Request(selectItemID,CurPage,2,2,(BYTE)level,m_pMBTradeType->GetCurSel());
						}
						return true;
					}
					if (pControl == m_pGoLastPage)//上一页
					{
						CurPage--;
						if (selectItemID == 0)
						{
							g_pGameControl->Send_Purchase_Request(selectItemID,CurPage,1,2,(BYTE)level,m_pMBTradeType->GetCurSel());
						}
						else
						{
							g_pGameControl->Send_Purchase_Request(selectItemID,CurPage,2,2,(BYTE)level,m_pMBTradeType->GetCurSel());
						}
						return true;
					}
				}
				else if (iSubPage == 1)
				{
					if (pControl == m_pConfirmSend)//发布收购信息
					{
						//发消息
						int iCurSel = m_pMBKindOfItem->GetCurSel();
						if (iCurSel < 0)
						{
							g_MsgBoxMgr.PopSimpleAlert("选择需要收购的物品");
							return true;
						}

						DWORD selectItemID = g_NPC.GetFirmData().vecItemNameID.at(iCurSel).dwItemID;
						DWORD Level = g_NPC.GetFirmData().vecItemNameID.at(iCurSel).nLevel;
						int numPerUnit = m_pENumPerUnit->GetNum();
						int price = m_pEUnitPrice->GetNum();
						BYTE  tradeType = m_pMBTradeType->GetCurSel();
						int unitNum = m_pEUnitNum->GetNum();

						if (numPerUnit <= 0 || price <= 0 || unitNum <= 0)
						{
							g_MsgBoxMgr.PopSimpleAlert("输入数据有误，请重新输入。");
							return true;
						}

						g_pGameControl->Send_MyPurchaseInfo(selectItemID,Level,unitNum,numPerUnit,price,tradeType);				
						return true;
					}

				}
				else if (iSubPage == 2)
				{
					WORD CurPage = g_NPC.GetFirmData().iCurPage;
					if (pControl == m_pGoNextPage)//下一页
					{
						CurPage++;
						g_pGameControl->Send_Purchase_Request(0,CurPage,1,1);
						return true;
					}
					if (pControl == m_pGoLastPage)//上一页
					{
						CurPage--;
						g_pGameControl->Send_Purchase_Request(0,CurPage,1,1);
						return true;
					}
					for (DWORD i = 0; i != REMOVEITEMNUMBER; ++i)//取消哪个
					{
						if (pControl == m_pRemoveItem[i])
						{
							g_pGameControl->Send_CancelPurchase(g_NPC.GetFirmData().vecFirmItemList.at(i).dwRowID);
							return true;
						}
					}
				}
				else if (iSubPage == 3)
				{
					if (pControl == m_pTakeSellGoods)//领取寄卖物品
					{
						g_pGameControl->Send_LeaseBooth_TakeMoneyBack(1);
						return true;
					}
					if (pControl == m_pTakeSellMoney)//领取寄卖所得
					{
						g_pGameControl->Send_LeaseBooth_TakeMoneyBack(0);
						return true;
					}
					if (pControl == m_pTakeBuyGoods)//领取收购到的物品
					{
						g_pGameControl->Send_TakeBackGoods();
						return true;
					}
					if (pControl == m_pTakeBuyMoney)//领取收购预付金额
					{
						g_pGameControl->Send_GetMoneyOver();
						return true;
					}
				}
			}
			else if (iCurPage == 1)
			{
				if (pControl == m_pTradeBuyBtn)
				{
					if(m_iSelectiRow >= 0 && m_iSelectiRow < m_pVTradeGood->iVipPersons && m_iSelectCol >= 0)
					{
						MVipGood::iterator itr = m_pVTradeGood->mMVipGood.find(m_iSelectiRow);
						if (itr != m_pVTradeGood->mMVipGood.end())
						{
							vector<CGood> &VGood = itr->second.VGoods;
							if (m_iSelectCol < VGood.size())
							{
								g_pGameControl->SEND_Player_Vip_Option(5,VGood[m_iSelectCol].GetID(),0,0,itr->second.strOwner.c_str());
							}
						}
					}
					return true;
				}
				else if (pControl == m_pOnlyShowMySelf)
				{
					m_iSelectiRow = m_iSelectCol = -1;
					UpdateScrollPos();
				}
			}
		}
		break;

	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


bool CFirmWnd::OnLeftButtonDown(int iX, int iY)
{
	int iCurPage =  m_TabPage.iCurPage;
	if (iCurPage == 1)
	{
		GetIndex(iX,iY,m_iSelectiRow,m_iSelectCol);
	}

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CFirmWnd::OnLeftButtonUp(int iX, int iY)
{
	CCtrlWindowX::OnLeftButtonUp(iX,iY);

	int iCurPage =  m_TabPage.iCurPage;
	if (iCurPage == 1)
	{
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == Package_Wnd || GoodFrom.eFromWnd == Panel_Wnd))
		{
			SELF.PackageGood().BackToArray(GoodFrom.DropGood,GoodFrom.iWndPos);

			if(GoodFrom.DropGood.IsBind()) //绑定物品不能放到商行
			{
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				g_TalkMgr.PushSysTalk("绑定的物品不能出售");
				return true;
			}

			if (!g_BoothData.IsInBooth(GoodFrom.DropGood.GetID(),GoodFrom.DropGood.GetName()))
			{
				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);


				g_BoothData.SetReadyGood(GoodFrom.DropGood.GetID(),
					GoodFrom.DropGood.GetName(),GoodFrom.iWndPos);
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);

				//弹出输入价格的对话框
				g_pControl->PopupWindow(MSG_CTRL_BOOTH_ADD_GOOD,OPER_CREATE,MSG_CTRL_FIRM_WND);

				return true;
			}

			GoodFrom.DropGood.SetID(0);
			CGoodGrid::SetDropGoodFrom(GoodFrom);
			return true;

		}
		else
		{
			GetIndex(iX,iY,m_iSelectiRow,m_iSelectCol);

			if(GoodFrom.DropGood.GetID() == 0)
			{
				if(m_iSelectiRow >= 0 && m_iSelectiRow < m_pVTradeGood->iVipPersons && m_iSelectCol >= 0)
				{
					MVipGood::iterator itr = m_pVTradeGood->mMVipGood.find(m_iSelectiRow);
					if (itr != m_pVTradeGood->mMVipGood.end())
					{
						if (strcmp(itr->second.strOwner.c_str(),SELF.GetName()) != 0)//只有自己的物品才能拿起来
						{
							return true;
						}
						vector<CGood> &VGood = itr->second.VGoods;
						if (m_iSelectCol < VGood.size())
						{
							GoodFrom.DropGood = VGood[m_iSelectCol];
							GoodFrom.eFromWnd = VipTrade_Wnd;
							CGoodGrid::SetDropGoodFrom(GoodFrom);
						}
					}
				}
			}
			else if(GoodFrom.eFromWnd == VipTrade_Wnd)
			{
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);
			}
		}
	}

	return true;
}

bool CFirmWnd::OnMouseMove(int iX, int iY)
{
	CCtrlWindowX::OnMouseMove(iX,iY);

	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->SetShow(false);
	pTip->Clear();

	int iCurPage =  m_TabPage.iCurPage;
	if (iCurPage == 1)
	{
		int iSelectiRow = -1,iSelectCol = -1;
		if (iX >= 68 && iX <= 68 + 32 && iY >= 63 && iY <= 63 + PERONS_PER_PAGE_VIPTRADEWND * (m_iCellH + m_iIntervalH))
		{
			int iVipRow = m_pScroll->GetPos() + (iY - 63)/ (m_iCellH + m_iIntervalH);
			if (iVipRow >= 0 && iVipRow < m_pVTradeGood->iVipPersons)
			{
				MVipGood::iterator itr = m_pVTradeGood->mMVipGood.find(iVipRow);
				if (itr != m_pVTradeGood->mMVipGood.end() && itr->second.iVipExp > 0)
				{
					char strTemp[32] = {0};

					pTip->AddText(itr->second.strOwner.c_str());
					sprintf(strTemp,"神武殿等级：%d级",GetVipLevelByExp(itr->second.iVipExp));
					pTip->AddText(strTemp);
					sprintf(strTemp,"神武殿积分：%d",itr->second.iVipExp);
					pTip->AddText(strTemp);

					int x = m_iScreenX + iX + 10;
					int y = m_iScreenY + iY + 10;
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
					return true;
				}
			}
		}

		GetIndex(iX,iY,iSelectiRow,iSelectCol);

		if(iSelectiRow < 0 || iSelectCol < 0 || (g_pControl->GetMenuWnd()->GetCaller() == this && g_pControl->GetMenuWnd()->IsShow()))
		{
			return CCtrlWindowX::OnMouseMove(iX, iY);
		}

		if(iSelectiRow >= 0 && iSelectCol >= 0)
		{
			MVipGood::iterator itr = m_pVTradeGood->mMVipGood.find(iSelectiRow);
			if (itr != m_pVTradeGood->mMVipGood.end())
			{
				vector<CGood> &VGood = itr->second.VGoods;
				if (iSelectCol < VGood.size())
				{
					CGood & good = VGood[iSelectCol];

					char strTemp[32] = {0};
					sprintf(strTemp,"出售者:%s",itr->second.strOwner.c_str());
					pTip->AddText(strTemp);

					pTip->ParseWithPrice(good,false);

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

bool CFirmWnd::OnLeftButtonDClick(int iX,int iY)
{

	//if(m_iSelectIndex >= 0 && m_iSelectIndex < g_NPC.GetNpcQuickVector().size())
	//{
	//	CGood &temp = g_NPC.GetNpcQuickVector()[m_iSelectIndex];
	//	g_pGameControl->SEND_Exchange_Getback_Ok(g_NPC.GetID(),temp.GetID(),temp.GetName(),1);
	//}
	//else if (m_iSelectIndex >= g_NPC.GetNpcQuickVector().size())
	//{
	//	char strConfirm[128];
	//	int needPay = 0;
	//	if (g_NPC.GetStorageMaxCount() <= 40)//基础40,第一个10格-1元宝,第二个-10元宝,之后的-20元宝
	//	{
	//		needPay = 1;
	//	}
	//	else if (g_NPC.GetStorageMaxCount() <= 50)
	//	{
	//		needPay = 10;
	//	} 
	//	else
	//	{
	//		needPay = 20;
	//	}

	//	sprintf(strConfirm,"点击确认获得此区域共10格扩展仓库的永久使用权，并自动扣除%d元宝，是否继续？",needPay);
	//}

	return true;
}

int CFirmWnd::GetIndex(int iX,int iY,int &iRow,int &iCol)
{
	if(iY <= m_iStartY || iY >= m_iStartY+m_iRows*(m_iCellH + m_iIntervalH))
	{
		iRow = - 1;
		iCol = -1;
		return -1;
	}
	if(iX <= m_iStartX || iX >= m_iStartX+m_iCols*(m_iCellW+m_iIntervalW))
	{
		iRow = - 1;
		iCol = -1;
		return -1;
	}

	iRow = (iY-m_iStartY)/(m_iCellH + m_iIntervalH);
	iCol = (iX-m_iStartX)/(m_iCellW+m_iIntervalW);
	iRow += m_pScroll->GetPos();

	int index = iRow * m_iCols + iCol;

	if(iRow >= 0 && iRow < m_pVTradeGood->iVipPersons)
	{
		return index;
	}

	iRow = - 1;
	iCol = -1;
	return -1;
}

bool CFirmWnd::OnWheel(int iWheel)
{
	int iCurPage =  m_TabPage.iCurPage;
	if (iCurPage == 1)
	{
		return m_pScroll->OnWheel(iWheel);
	}

	return CCtrlWindowX::OnWheel(iWheel);
}

void CFirmWnd::UpdateScrollPos()
{
	int iCurPage =  m_TabPage.iCurPage;
	if (iCurPage != 1)
		return;

	if (m_pOnlyShowMySelf && m_pOnlyShowMySelf->IsChecked())
	{
		mSelfVTradeGood.Clear();
		MVipGood &MVGood = g_NPC.GetVipTradeGood().mMVipGood;
		for (MVipGood::iterator itr = MVGood.begin(); itr != MVGood.end(); itr++)
		{
			if (strcmp(SELF.GetName(),itr->second.strOwner.c_str()) == 0)
			{
				mSelfVTradeGood.mMVipGood[0] = itr->second;
				mSelfVTradeGood.iVipPersons = 1;
				mSelfVTradeGood.bHaveRequireData = true;
				break;
			}
		}

		m_pVTradeGood = &mSelfVTradeGood;
	}
	else
	{
		m_pVTradeGood = &(g_NPC.GetVipTradeGood());
	}


	int iRange = m_pVTradeGood->iVipPersons;

	m_pScroll->SetRange(iRange);
	if(m_pScroll->GetPos() >= iRange)
	{
		int iNewPos = iRange - 1;
		if(iNewPos < 0)
			iNewPos = 0;

		m_pScroll->SetPos(iNewPos);
	}
}

void CFirmWnd::SwitchToPage(int iPage,S_TabPage * pParentTabPage,bool bReCreate)
{
	CCtrlWindowX::SwitchToPage(iPage,pParentTabPage,bReCreate);

	int iCurPage =  m_TabPage.iCurPage;

	if (iCurPage == 1)
	{
		SVipTradGood &VipTradeGood = g_NPC.GetVipTradeGood();
		if(!VipTradeGood.bHaveRequireData)
		{
			VipTradeGood.bHaveRequireData = true;
			g_pGameControl->SEND_Player_Vip_Option(1,0,0);//请求商行出售物品数据
		}
	}
}

int CFirmWnd::GetVipLevelByExp(DWORD dwExp)
{
	if (dwExp >= 10800)
		return 7;
	if (dwExp >= 6000)
		return 6;
	if (dwExp >= 3600)
		return 5;
	if (dwExp >= 1800)
		return 4;
	if (dwExp >= 900)
		return 3;
	if (dwExp >= 300)
		return 2;
	if (dwExp > 0)
		return 1;

	return 0;
}