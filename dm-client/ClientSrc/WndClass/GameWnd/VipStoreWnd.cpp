
#include "VipStoreWnd.h"
#include "GameData\WooolStoreData.h"
#include "GameData\GameData.h"
#include "GameControl\GameControl.h"
#include "GameData\MsgBoxMgr.h"
#include "GameData\GameGlobal.h"

INIT_WND_POSX(CVipStoreWnd,POS_AUTO,POS_AUTO)

CVipStoreWnd::CVipStoreWnd(void)
{
	m_iIndex = 17400;
	m_iPages = 1;
	m_dwScrollTipTime = 0;
	m_byFirstVipType = SELF.GetVipCardType();
	m_bHaveGetGift = false;


	CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();

	////for test
	//__time32_t tNow;
	//_time32(&tNow);
	//vipData.tOverdueTime = tNow;
	//vipData.strServiceMsg = "\\1.test1     2.test2 \\3.test3    \\4.test4\\何国辉何国辉何国辉\\何国辉何国辉\\何国辉何国辉何国辉\\何国辉何国辉\\何国辉何国辉何国辉何\\何国辉何国辉何国辉";
	//vipData.strTips = "珍宝秘市小帖市:何国辉何国辉何国辉\\何国辉何国辉何国辉何\\国辉何国辉何国辉";


	//CVipStoreItem item;
	//item.m_strName = "垃圾";
	//item.m_wItemLooks = 100;
	//item.m_wPrice = 10;
	//item.m_wGoldCardPrice = 8;
	//item.m_wSilverCardPrice = 9;
	//item.m_iItemCount = item.m_iLimitRemain = 3;
	//item.m_strDesc = "没什么用的东西";
	//vipData.vPresentItems.push_back(item);


 //	for(int i = 0; i < 17; i++)
 //	{
 //		CVipStoreItem item;
 //		item.m_strName = "垃圾";
 //		item.m_wItemLooks = 100 + i;
 //		item.m_wPrice = 10;
 //		item.m_wGoldCardPrice = 8;
 //		item.m_wSilverCardPrice = 9;
 //		item.m_iItemCount = item.m_iLimitRemain = 3;
 //		item.m_strDesc = "没什么用的东西";
 //
 //		vipData.vSellItems.push_back(item);
 //		vipData.vPresentItems.push_back(item);
 //	}
	////end for test



	m_ptBuy[0].x = 237;
	m_ptBuy[0].y = 204;
	m_ptBuy[1].x = 237;
	m_ptBuy[1].y = 314;
}

CVipStoreWnd::~CVipStoreWnd(void)
{

}
void CVipStoreWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	g_pFont->DrawText(m_iScreenX+265, m_iScreenY+20, "龙虎阁", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
}

void CVipStoreWnd::OnDraw(void)
{
	std::vector<std::string>   vDrawStr;

	vDrawStr.clear();
	CutByUnicode((char*)(m_strHeader.c_str()),vDrawStr,90);
	int iStartY = vDrawStr.size()>2?59:68;
	for(size_t k = 0; k < vDrawStr.size(); k++)
	{
		g_pFont->DrawText(m_iScreenX + 20,m_iScreenY + iStartY + k * 14,vDrawStr[k].c_str(),0xFFFFFF00, FONT_YAHEI);
	}

	//g_pFont->DrawText(m_iScreenX + 33,m_iScreenY + 82,m_strHeader.c_str(),0xFFFFFF00);

	CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();
	size_t size = vipData.vSellItems.size();
	int iPos = m_pScroll->GetPos();
	int iStart = iPos * VIPSTOREWND_MAXITEM_PER_PAGE_SELL;
	char szTemp[128] = {0};
	LPTexture pTex = NULL;

	BYTE byVipType = SELF.GetVipCardType();
	if (byVipType != m_byFirstVipType)
	{
		m_byFirstVipType = byVipType;
		g_pGameControl->SEND_PT_Store_Info(12, "", 0,false,false);
	}

	if (byVipType == 0)//不是vip行会显示金卡的内容
	{
		byVipType = 2;
	}

	for (int i = iStart,j = 0; i < size && j < VIPSTOREWND_MAXITEM_PER_PAGE_SELL; i++,j++)
	{
		CVipStoreItem &item = vipData.vSellItems[i];

		pTex = g_pTexMgr->GetTex(PACKAGE_items,item.m_wItemLooks,EP_UI);
		if (pTex)
		{
			g_pGfx->DrawTextureNL(m_iScreenX + m_ptBuy[j].x+14,m_iScreenY + m_ptBuy[j].y - 45,pTex);
		}

		g_pFont->DrawText(m_iScreenX + m_ptBuy[j].x + 80,m_iScreenY + m_ptBuy[j].y - 38,item.m_strName.c_str(),0xFFFFFF00, FONT_YAHEI);
		sprintf(szTemp," %d元宝",(byVipType == 2)?item.m_wGoldCardPrice:item.m_wSilverCardPrice);
		g_pFont->DrawText(m_iScreenX + m_ptBuy[j].x + 80 + strlen(item.m_strName.c_str()) * 12 / 2,m_iScreenY + m_ptBuy[j].y - 38,szTemp,COLOR_TEXT_NORMAL, FONT_YAHEI);
		sprintf(szTemp,"剩余购买次数:%d次",item.m_iLimitRemain);
		g_pFont->DrawText(m_iScreenX + m_ptBuy[j].x + 80,m_iScreenY + m_ptBuy[j].y - 24,szTemp,COLOR_TEXT_NORMAL, FONT_YAHEI);

		vDrawStr.clear();
		CutByUnicode((char*)(item.m_strDesc.c_str()),vDrawStr,31);
		for(size_t k = 0; k < vDrawStr.size(); k++)
		{
			g_pFont->DrawText(m_iScreenX + m_ptBuy[j].x + 80,m_iScreenY + m_ptBuy[j].y - 10 + k * 14,vDrawStr[k].c_str(),COLOR_TEXT_NORMAL, FONT_YAHEI);
		}
	}

	int iCursel = m_pGetGiftMBtn->GetCurSel();
	if (iCursel >= 0 && iCursel < vipData.vPresentItems.size())
	{
		CVipStoreItem &item = vipData.vPresentItems[iCursel];
		pTex = g_pTexMgr->GetTex(PACKAGE_items,item.m_wItemLooks,EP_UI);
		if (pTex)
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 38,m_iScreenY + 270,pTex);
		}

		if(SELF.GetVipCardType() > 0 && SELF.GetVipCardType() <= 2)
		{
			if (m_bHaveGetGift)
			{
				g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 330,"已领",-1, FONT_YAHEI);
			}
			else
			{
				g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 330,"未领",-1, FONT_YAHEI);
			}
		}

		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 294,item.m_strName.c_str(),0xFFFFFF00, FONT_YAHEI);
		sprintf(szTemp,"可领取数量：%d",m_bHaveGetGift?0:item.m_iLimitRemain);
		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 332 - 20,szTemp,COLOR_TEXT_NORMAL, FONT_YAHEI);
	}


	if (m_vScrollText.size() != 0)
	{
		DWORD dwCount = GetTickCount();
		int iLine = ((dwCount - m_dwScrollTipTime) / 5000) % m_vScrollText.size();
		g_pFont->DrawText(m_iScreenX + 33, m_iScreenY + 361, m_vScrollText[iLine].c_str(), 0xFFFFFF00, FONT_YAHEI);
	}
}

void CVipStoreWnd::OnCreate()
{
	SetCloseButton(544,16, 80);
	m_dwScrollTipTime = GetTickCount();

	m_pGetGiftMBtn = new CCtrlMenuButton();
	AddControl(m_pGetGiftMBtn);
	m_pGetGiftMBtn->Create(this,91,268,118,140);
	m_pGetGiftMBtn->SetMenuTextColor(0xFFE4C800);
	m_pGetGiftMBtn->SetSelColor(0xFFE4C800,0xFF2F8406);
	m_pGetGiftMBtn->SetFont(FONT_YAHEI);


	m_pGetGiftBtn = new CCtrlButton();
	AddControl(m_pGetGiftBtn);
	m_pGetGiftBtn->CreateEx(this,25,314,95,96,97,98);
	m_pGetGiftBtn->SetText("领取");


	m_pMarkViewer = new CMarkViewer(14,7,2,7);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,30,123,159,133);
	m_pMarkViewer->AddScrollEx(165,0,13,115);
	m_pMarkViewer->SetFont(FONT_YAHEI);
	m_pMarkViewer->SetDefaultColor(COLOR_TEXT_NORMAL);
	

	for (int i = 0; i < VIPSTOREWND_MAXITEM_PER_PAGE_SELL; i++)
	{
		m_pBuyBtn[i] = new CCtrlButton();
		AddControl(m_pBuyBtn[i]);
		m_pBuyBtn[i]->CreateEx(this,m_ptBuy[i].x,m_ptBuy[i].y,95,96,97,98);
		m_pBuyBtn[i]->SetText("购买");
	}

	//m_pUpButton = new CCtrlButton();
	//AddControl(m_pUpButton);
	//m_pUpButton->CreateEx(this,549,188,51,52,53);

	//m_pDownButton = new CCtrlButton();
	//AddControl(m_pDownButton);
	//m_pDownButton->CreateEx(this,549,341,56,57,58);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,548,132,13,224);
	m_pScroll->SetStep(1);

	
	__time32_t tNow;
	_time32(&tNow);
	tNow += g_dwServerTime;
	int iDays = (int)((tNow - g_WooolStoreMgr.GetVipStoreData().tLastRequireTime) / 3600)/24;
	//不在同一天要重新请求数据
	if (iDays != 0 || (g_WooolStoreMgr.GetVipStoreData().tOverdueTime == 0 && SELF.GetVipCardType() > 0 && SELF.GetVipCardType() <= 2))//如果是非vip请求过tOverdueTime还是0,这时再使用vip卡,要重新请求数据
	{
		g_pGameControl->SEND_PT_Store_Info(12, "", 0,false,false);
		g_WooolStoreMgr.GetVipStoreData().tLastRequireTime = tNow;
	}

	InitData();
}

bool CVipStoreWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_SCROLL_MOVE:
		{
			ReflashBtnState();
			return true;
		}
		break;
	case MSG_CTRL_MENU_SELCHANGED:
		{
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			//if (m_pUpButton == pControl)
			//{
			//	m_pScroll->GoDown();
			//	ReflashBtnState();
			//	return true;
			//}

			//if (m_pDownButton == pControl)
			//{
			//	m_pScroll->GoUp();
			//	ReflashBtnState();
			//	return true;
			//}

			if (pControl == m_pGetGiftBtn)
			{
				if(SELF.GetVipCardType() == 0)
				{
					g_MsgBoxMgr.PopSimpleAlert("你不是龙虎阁成员，无法领取免费道具，商城中可购买龙骧金卡或虎烈银卡加入龙虎阁。");
				}
				else
				{
					if (m_bHaveGetGift)
					{
						g_MsgBoxMgr.PopSimpleAlert("你今天的免费道具已经领取过了。");
					}
					else
					{
						int iCurSel = m_pGetGiftMBtn->GetCurSel();
						if(iCurSel >= 0)				
						{
							CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();
							if (iCurSel < vipData.vPresentItems.size())
							{
								if (vipData.vPresentItems[iCurSel].m_iLimitRemain <= 0)
								{
									g_MsgBoxMgr.PopSimpleAlert("该物品已经领完。");
								}
								else
								{
									g_pGameControl->SEND_PT_Store_Info(14, vipData.vPresentItems[iCurSel].m_strItemID.c_str(), vipData.vPresentItems[iCurSel].m_strItemID.size(),false,false);
								}
							}
						}
					}
				}
				return true;								
			}

			for (int i = 0; i < VIPSTOREWND_MAXITEM_PER_PAGE_SELL; i++)
			{
				if(pControl == m_pBuyBtn[i])
				{
					CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();
					size_t size = vipData.vSellItems.size();
					int iIndex = m_pScroll->GetPos() * VIPSTOREWND_MAXITEM_PER_PAGE_SELL + i;
					if (iIndex < size)
					{
						if (vipData.vSellItems[iIndex].m_iLimitRemain <= 0)
						{							
							g_MsgBoxMgr.PopSimpleAlert("该物品已经卖完。");
						}
						else
						{
							g_pGameControl->SEND_PT_Store_Info(13, vipData.vSellItems[iIndex].m_strItemID.c_str(), strlen(vipData.vSellItems[iIndex].m_strItemID.c_str()),false,false);
						}
					}
					return true;
				}
			}
		}
		break;
	case MSG_CTRL_VIPSTORE_WND:
		if (dwData == 1)
		{
			InitData();
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CVipStoreWnd::OnWheel(int iWheel)
{
	int iMouseX, iMouseY;
	g_pControl->GetMouseXY(iMouseX, iMouseY);

	if(iMouseX >= m_pMarkViewer->GetScreenX() && iMouseX <= m_pMarkViewer->GetScreenX() + m_pMarkViewer->GetWidth() &&
		iMouseY >= m_pMarkViewer->GetScreenY() && iMouseY <= m_pMarkViewer->GetScreenY() + m_pMarkViewer->GetHeight() )
	{
		return m_pMarkViewer->OnWheel(iWheel);
	}
	else if(iMouseX >= m_iScreenX + 256 && iMouseX <= m_iScreenX + 545 && iMouseY >= m_iScreenY + 146 && iMouseY <= m_iScreenY + 354 )
	{
		return m_pScroll->OnWheel(iWheel);
	}

	return false;
}

void CVipStoreWnd::InitData()
{
	CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();
	__time32_t tNow;
	_time32(&tNow);

	BYTE byVipType = SELF.GetVipCardType();
	string strMsg = "";
	if (byVipType == 0)
	{
		m_strHeader = "您不是龙虎阁的成员，暂时不能享有专属特权。您可以购买商城中的龙骧金卡或虎烈银卡。双击开启后，可立刻获得商城道具赠送，并且每日还可免费领取物品，高额经验回馈，极品道具特惠等特权。";
		strMsg = "龙虎阁为龙骧金卡持有者\\提供的服务：\\";
		//m_pGetGiftBtn->SetEnable(false);
	}
	else if(byVipType <= 2)
	{
		strMsg = "龙虎阁为您提供的服务:\\";

		char szTemp[256]={0};
		//int iDays = 0;
		//tNow += g_dwServerTime;
		//if (vipData.tOverdueTime > tNow)
		//{
		//	iDays = (int)((vipData.tOverdueTime - tNow) / 3600)/24;
		//}

		if (byVipType == 2)
		{
			sprintf(szTemp,"龙虎阁迎您的驾临，您持有的是龙骧金卡，到期时间：%s零点\n龙骧金卡尊贵特权：每日获赠商城道具，经验宝玉释放经验时所获得的经验额外增加20%%，超值购买各种商城道具等",g_Timer.GetDateTime("%Y-%m-%d",(DWORD)vipData.tOverdueTime));
		}
		else
		{
			sprintf(szTemp,"龙虎阁迎您的光临，您持有的是虎烈银卡，到期时间：%s零点\n虎烈银卡尊贵特权：每日获赠商城道具，勇士任务经验翻倍，超值购买各种商城道具等",g_Timer.GetDateTime("%Y-%m-%d",(DWORD)vipData.tOverdueTime));
		}

		m_strHeader = szTemp;
		//m_pGetGiftBtn->SetEnable(true);
	}

	strMsg += vipData.strServiceMsg;
	m_ServiceTagText.Parse(strMsg);

	m_vScrollText.clear();
	int ioffset = 0;
	string strTemp = vipData.strTips + "\\";
	string strEle;
	const char* pData = strTemp.c_str();
	const char* pTemp = NULL;

	while ((pTemp = strstr(pData, "\\")) != NULL)
	{
		strEle.assign(pData, pTemp);
		if (strEle.size() > 0)
			m_vScrollText.push_back(strEle);
		pData = pTemp + 1;
	}

	m_pScroll->SetRange((vipData.vSellItems.size() + VIPSTOREWND_MAXITEM_PER_PAGE_SELL - 1) / VIPSTOREWND_MAXITEM_PER_PAGE_SELL - 1);
	m_pMarkViewer->SetTagText(&m_ServiceTagText);


	m_pGetGiftMBtn->ResetContent();
	int iSize = (int)(vipData.vPresentItems.size());
	for (int i = 0; i < iSize; i++)
	{
		if (vipData.vPresentItems[i].m_iLimitRemain <= 0)
		{
			//如果不是vip让它点，到时再弹提示，是vip领过后就不让点了
			if (byVipType != 0)
			{
				m_pGetGiftBtn->SetEnable(false);
				m_pGetGiftMBtn->SetEnable(false);
				m_bHaveGetGift = true;
			}
		}

		m_pGetGiftMBtn->AddString(vipData.vPresentItems[i].m_strName.c_str());
	}

	int iCursel = m_pGetGiftMBtn->GetCurSel();
	if (iSize > 0 && (iCursel < 0 || iCursel >= iSize))
	{
		m_pGetGiftMBtn->SetCurSel(0);
	}


	ReflashBtnState();
}

void CVipStoreWnd::ReflashBtnState()
{

	CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();
	BYTE byVipType = SELF.GetVipCardType();
	if (byVipType == 0)
	{
		for (int i = 0;i < VIPSTOREWND_MAXITEM_PER_PAGE_SELL; i++)
		{
			m_pBuyBtn[i]->SetEnable(false);
		}
	}
	else if (byVipType <= 2)
	{
		int iPos = m_pScroll->GetPos();
		int iSize = (int)(vipData.vSellItems.size());
		int iStart = iPos * VIPSTOREWND_MAXITEM_PER_PAGE_SELL;
		int i = 0;

		for (i = 0; i + iStart < iSize && i < VIPSTOREWND_MAXITEM_PER_PAGE_SELL; i++)
		{
			if (vipData.vSellItems[i + iStart].m_iLimitRemain <= 0)
				m_pBuyBtn[i]->SetEnable(false);
			else
				m_pBuyBtn[i]->SetEnable(true);
		}

		for (; i < VIPSTOREWND_MAXITEM_PER_PAGE_SELL; i++)
		{
			m_pBuyBtn[i]->SetEnable(false);
		}
	}	

}
