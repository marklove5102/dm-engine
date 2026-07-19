#include "praytreewnd.h"
#include "GameData/GameData.h"
#include "GameData/NpcData.h"
#include "GameData/TalkMgr.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/WooolStoreData.h"
#include "GameData/MsgBoxMgr.h"

bool CPrayTreeWnd::sm_bSendForPrize = false;

INIT_WND_POSX(CPrayTreeWnd,POS_AUTO,POS_AUTO)

CPrayTreeWnd::CPrayTreeWnd(void)
{
	m_iCurrentLevel = 3;
	m_iFrameCount = 0;
	m_iStartPos = 0;
	m_iEffectFrameCount = 0;
	m_pThrowButton = NULL;
	m_pUpButton = 0;
	m_pDownButton = 0;
	m_bPlayEffect = false;	
	m_bStop = false;
	m_iIndex = 22250;
	g_pControl->PopupWindow(MSG_CTRL_NPCWND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_NPCRECRUIT,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_ADVENTURE_WND, OPER_CLOSE);
	m_bThrowButtonClick = false;
}

CPrayTreeWnd::~CPrayTreeWnd(void)
{
	if(g_NPC.GetGood().GetID() != 0 && !sm_bSendForPrize)
	{
		for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6; i++)
		{
			if(SELF.GetPackageGood(i).GetID() == 0)
			{
				SELF.GetPackageGood(i) = g_NPC.GetGood();
				break;
			}
		}
		g_NPC.GetGood().SetID(0);
	}

	CGood & good = g_NPC.GetPrayCharm();
	if((good.GetID() != 0 && !sm_bSendForPrize)	|| (good.GetID() != 0 && strcmp(good.GetName(),"祈愿符(捆)") == 0))
	{
		for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6; i++)
		{
			if(SELF.GetPackageGood(i).GetID() == 0)
			{
				SELF.GetPackageGood(i) = g_NPC.GetPrayCharm();
				break;
			}
		}
		g_NPC.GetPrayCharm().SetID(0);
	}	

	g_NPC.SetPrayTreeLevel(0xFF);
}


bool CPrayTreeWnd::SetFocus()
{
	return m_pScroll->SetFocus();
}

void CPrayTreeWnd::OnCreate()
{
	m_pThrowButton = new CCtrlButton();
	m_pThrowButton->CreateEx(this,270,364,21679,21680,21681,21682);
	m_pThrowButton->SetText("祈 愿", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	AddControl(m_pThrowButton);

	//m_pUpButton = new CCtrlButton();
	//AddControl(m_pUpButton);
	//m_pUpButton->CreateEx(this,483,81,1026,1027,1028);

	//m_pDownButton = new CCtrlButton();
	//AddControl(m_pDownButton);
	//m_pDownButton->CreateEx(this,483,380,1030,1031,1032);

	m_pScroll = new CCtrlScroll();
	m_pScroll->CreateEx(this, 482, 81, 17, 314);
	//m_pScroll->Create(this, 483, 97, 14,280,1034);
	m_pScroll->SetPos(0);	
	m_pScroll->SetStep(1);
	AddControl(m_pScroll);

	SetCloseButton(495,4,80);

	g_pGameControl->SEND_Open_Box_Requese(12,0);
}

void CPrayTreeWnd::OnClickCloseButton()
{
	if(m_bPlayEffect)		//放特效时不能关闭界面
		return;

	CloseWindow();
}

void CPrayTreeWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 240,m_iScreenY + 11,"祈愿树",COLOR_TEXT_NORMAL,FONT_YAHEI,16);

	g_pFont->DrawText(m_iScreenX + 380 ,m_iScreenY + 60, "祈愿树上的物品", 0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE, DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + 22 ,m_iScreenY + 412, "蓄力槽", 0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE, DTF_BlackFrame);

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22254 + m_iCurrentLevel,EP_UI);
	if(pTex)
		g_pGfx->DrawTextureNL(m_iScreenX + 18,m_iScreenY + 40,pTex);

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22263,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 67,m_iScreenY + 93,pTex);
	}

	//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22275);
	//if(pTex)
	//{
	//	g_pGfx->DrawTextureNL(m_iScreenX + 268,m_iScreenY + 343,pTex);
	//}

	//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22276);
	//if(pTex)
	//{
	//	g_pGfx->DrawTextureNL(m_iScreenX + 318,m_iScreenY + 343,pTex);
	//}

	if(g_NPC.GetPrayCharm().GetID() != 0)
	{
		//CGoodGrid::DrawOneGood(m_iScreenX+338,m_iScreenY + 366,g_NPC.GetPrayCharm());
		CGoodGrid::DrawOneGood(m_iScreenX+319,m_iScreenY + 337,g_NPC.GetPrayCharm());
	}

	DrawForceSlot();

	//放入的投掷物品合法，显示
	if(g_NPC.GetGood().GetID())
	{
		//CGoodGrid::DrawOneGood(m_iScreenX + 288,m_iScreenY + 364,g_NPC.GetGood());
		CGoodGrid::DrawOneGood(m_iScreenX + 280,m_iScreenY + 337,g_NPC.GetGood());
	}

	VTreeGift* pGifts = g_NPC.GetGiftsByTreeLevel(m_iCurrentLevel);

	m_pScroll->SetRange((pGifts->size() - 24 + 2) / 3);

	m_iStartPos = m_pScroll->GetPos() * 3;

	for(int i = m_iStartPos;i < pGifts->size() && (i - m_iStartPos) < 24;i++)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_items,(*pGifts)[i].wLooks,EP_UI);
		if(pTex)
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 366 + i % 3 * 39,m_iScreenY + 86 + (i - m_iStartPos) / 3 * 39,pTex);
		}
	}

	if(m_bPlayEffect)
	{
		PlayEffect();
	}

	static DWORD dwTime = 0;

	std::string& strTemp = g_NPC.GetPrayTreePrize();
	if(strTemp.length() > 0)
	{
		m_strPrizeInfo = g_NPC.GetPrayTreePrize();
		g_NPC.GetPrayTreePrize().clear();
		dwTime = GetTickCount();
	}

	if(m_strPrizeInfo.length() > 0 && GetTickCount() - dwTime < 5000)
	{
		DWORD dwTimeDev = GetTickCount() - dwTime;
		BYTE  byTemp = (BYTE)((1.0 - ((float)dwTimeDev / 5000.0) ) * 0xFF);
		DWORD dwTemp = (byTemp<<24);
		dwTemp |= 0x00FFFFFF;
		dwTemp &= 0xFFFFFF00;
		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 100,m_strPrizeInfo.c_str(),dwTemp,FONT_DEFAULT,FONTSIZE_MIDDLE);
		g_pFont->DrawText(m_iScreenX + 100 + (m_strPrizeInfo.size() - 22) * 5 / 2,m_iScreenY + 120,"看来今年你的运气不错哦",dwTemp,FONT_DEFAULT,FONTSIZE_MIDDLE);
	}
	else 
	{
		m_strPrizeInfo.clear();
	}
}

bool CPrayTreeWnd::OnLeftButtonUp(int iX,int iY)
{
	if(iX > 27 && iX < 357 && iY > 209 && iY < 295)
	{
		m_iCurrentLevel = 1;
		m_pScroll->SetPos(0);
	}
	else if(iX > 45 && iX < 337 && iY > 142 && iY < 210)
	{
		m_iCurrentLevel = 2;
		m_pScroll->SetPos(0);
	}
	else if(iX > 105 && iX < 256 && iY > 43 && iY < 138)
	{
		m_iCurrentLevel = 3;
		m_pScroll->SetPos(0);
	}
	else if((iX > 260 && iX < 300 && iY > 316 && iY < 356) || (iX > 300 && iX < 340 && iY > 316 && iY < 356))
	{
		CGood * good = NULL;

		if(iX < 300)	
		{
			good = &g_NPC.GetGood();
		}
		else		//放置祈愿符
		{
			good = &g_NPC.GetPrayCharm();
		}

		if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE || g_pGameData->GetDropYuanBaoFrom() != EDMYF_NONE)		//鼠标上有钱
			return true;

		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd != Package_Wnd)	//只接受来自本体包裹栏的物品
		{
			g_TalkMgr.PushSysTalk("请从包裹栏中取得物品");
			return true;
		}

		if(good->GetID() == 0 && GoodFrom.DropGood.GetID() == 0)	//鼠标上没东西并且喂食栏也没有东西
		{
			return true;
		}

		if(GoodFrom.DropGood.GetID() != 0 && good->GetID() == 0 && !m_bThrowButtonClick)	//鼠标上有东西投掷栏没有东西,放东西到投掷栏
		{
			if(iX > 300 && (GoodFrom.DropGood.GetStdMode() != 37 || GoodFrom.DropGood.GetShape() != 216))		//祈愿符栏位只能放置祈愿符
			{
				return true;
			}

			if(GoodFrom.DropGood.IsBind())
			{
				g_TalkMgr.PushSysTalk("绑定物品不能祈愿");
				return true;
			}

			*good = GoodFrom.DropGood;
			GoodFrom.DropGood.SetID(0);
			CGoodGrid::SetDropGoodFrom(GoodFrom);

			if(g_NPC.GetGood().GetID() != 0 && g_NPC.GetPrayCharm().GetID() != 0)
			{
				g_pGameControl->SEND_Open_Box_Requese(12,1);
			}

			sm_bSendForPrize = false;
			return true;
		}

		if((GoodFrom.DropGood.GetID() == 0 && good->GetID() != 0 && !m_bThrowButtonClick) || (GoodFrom.DropGood.GetID() == 0 && good->GetID() != 0 && strcmp(good->GetName(),"祈愿符(捆)") == 0 && !m_bPlayEffect))		//鼠标上没东西投掷栏有东西，取出东西
		{
			GoodFrom.DropGood = *good;
			good->SetID(0);
			GoodFrom.eFromWnd = Package_Wnd;	//此时的iWndPos不知道
			CGoodGrid::SetDropGoodFrom(GoodFrom);
			m_iFrameCount = 0;
			g_NPC.SetPrayTreeLevel(0xFF);
			return true;
		}

		if((GoodFrom.DropGood.GetID() != 0 && good->GetID() != 0 && !m_bThrowButtonClick) || (GoodFrom.DropGood.GetID() != 0 && good->GetID() != 0 && strcmp(good->GetName(),"祈愿符(捆)") == 0 && !m_bPlayEffect))	//鼠标上有东西并且投掷栏也有东西，交换
		{
			if(iX > 300 && (GoodFrom.DropGood.GetStdMode() != 37 || GoodFrom.DropGood.GetShape() != 216))		//祈愿符栏位只能放置祈愿符
			{
				return true;
			}

			if(GoodFrom.DropGood.IsBind())
			{
				g_TalkMgr.PushSysTalk("绑定物品不能祈愿");
				return true;
			}
			CGood temp = *good;
			*good = GoodFrom.DropGood;
			GoodFrom.DropGood = temp;
			CGoodGrid::SetDropGoodFrom(GoodFrom);
			g_NPC.SetPrayTreeLevel(0xFF);

			if(g_NPC.GetGood().GetID() != 0 && g_NPC.GetPrayCharm().GetID() != 0)
			{
				g_pGameControl->SEND_Open_Box_Requese(12,1);
			}

			return true;
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CPrayTreeWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pThrowButton)
			{	
				//如果包裹中没有祈愿符,则弹出快捷消费窗口
				if(!HasPrayCharm())
				{
					//快捷消费
					if(g_NPC.GetPrayCharm().GetID() == 0)
					{
						g_WooolStoreMgr.SetQuickBuyItem("祈愿符");
						CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

						if(QuickBuyData.pItem)
						{
							char cTemp[128]={0};
							sprintf(cTemp,"你包裹里没有祈愿符，需要立即购买吗？单价%d元宝",QuickBuyData.pItem->iPrice);
							QuickBuyData.iUseAfterBuyType = 1;
							QuickBuyData.strMsg = cTemp;
							QuickBuyData.iType = 1;

							g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);					
						}
						else
						{
							g_TalkMgr.PushSysTalk("你包裹里没有祈愿符，请到商城购买后再进行祈愿",0xFDFF);
						}
						return true;
					}
				}//如果窗口中没放祈愿符则提示玩家要放上祈愿意符
				else
				{
					if(g_NPC.GetPrayCharm().GetID() == 0)
					{
						g_MsgBoxMgr.PopSimpleAlert("没有祈愿符，无法为你祈愿。");
						return true;
					}
				}

				if(g_NPC.GetGood().GetID() != 0 && g_NPC.GetPrayCharm().GetID() != 0 && !m_bThrowButtonClick)
				{
					if(g_NPC.GetPrayTreeLevel() > 0 && g_NPC.GetPrayTreeLevel() < 4)
					{
						m_iRandomNum = rand() % 25;
						m_bThrowButtonClick = true;
						m_bStop = false;	
						g_pGameControl->SEND_Open_Box_Requese(12,2);		//请求奖品
						sm_bSendForPrize = true;
					}
					else
					{
						g_MsgBoxMgr.PopSimpleAlert("无法用该物品进行祈愿");						
					}
				}				
				return true;
			}
			else if(pControl == m_pUpButton)
			{
				m_pScroll->GoDown();				
				return true;
			}
			else if(pControl == m_pDownButton)
			{
				m_pScroll->GoUp();
				return true;
			}
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CPrayTreeWnd::DrawForceSlot()
{
	if(g_NPC.GetPrayTreeLevel() == 0xFF || (g_NPC.GetGood().GetID() == 0 && !m_bThrowButtonClick)) return;
	if(g_NPC.GetGood().GetID() != 0 && !m_bThrowButtonClick && g_NPC.GetPrayTreeLevel() > 0 && g_NPC.GetPrayTreeLevel() < 4)
	{
		m_iFrameCount += 11;

		int iCount = m_iFrameCount % 200;

		if(iCount > 100)
		{
			iCount = 200 - iCount;
		}

		float fPercent = (float)(iCount % 200) / 100;

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22258,EP_UI);
		if(pTex)
		{
			g_pGfx->DrawPartTexture(m_iScreenX + 72,m_iScreenY + 416,pTex,0,0,(int)(pTex->GetWidth() * fPercent),8);		
		}
	}
	else if(m_bThrowButtonClick)		
	{
		int level = g_NPC.GetPrayTreeLevel();


		int pos = 33 * (level - 1) + 2 + m_iRandomNum;

		int iCount = m_iFrameCount % 200;

		if(iCount > 100)
		{
			iCount = 200 - iCount;
		}

		if(pos == iCount && !m_bStop)
		{
			m_bPlayEffect = true;	//力量槽停下来后放特效
			m_bStop = true;
		}
		else if(abs(iCount - pos) < 11)
		{
			m_iFrameCount = pos;
		}
		else
		{
			m_iFrameCount += 11;
		}

		float fPercent = (float)(iCount % 200) / 100;		

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22258,EP_UI);
		if(pTex)
		{
			g_pGfx->DrawPartTexture(m_iScreenX + 72,m_iScreenY + 416,pTex,0,0,(int)(pTex->GetWidth() * fPercent),8);		
		}
	}	
}

void CPrayTreeWnd::PlayEffect()
{
	int level = g_NPC.GetPrayTreeLevel();
	int iOffX = 0,iOffY = 0;

	switch(level)
	{
	case 1:
		iOffX = 77,iOffY = 133;
		break;
	case 2:
		iOffX = 57,iOffY = 68;
		break;
	case 3:
		iOffX = 32,iOffY = -42;
		break;
	default:
		return;
	}

	LPTexture pPrayTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22262,EP_UI);
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22258 + level,EP_UI);
	LPTexture pHaloTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15160,EP_UI);

	if(m_iEffectFrameCount < 48)		//轨迹
	{
		if(pTex)
		{
			pTex->EnableSysAnim(FALSE);
			pTex->SetCurFrame(m_iEffectFrameCount / 6);
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + 112,m_iScreenY + 73,pTex);
			g_pGfx->SetRenderMode();
		}
	}
	else if(m_iEffectFrameCount < 90)		//光晕
	{	
		if(pHaloTex)
		{
			pTex->EnableSysAnim(FALSE);
			pTex->SetCurFrame((m_iEffectFrameCount - 48) / 6);
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + iOffX - 2,m_iScreenY + iOffY - 7,pHaloTex);
			g_pGfx->SetRenderMode();
		}
	}
	else if(m_iEffectFrameCount < 138)		//祈愿符出现
	{
		int x = 0,y = 0;
		if(m_iEffectFrameCount == 90)
		{
			x = rand() % 80 - 40;
			y = rand() % 50 - 25;
		}

		if(pPrayTex)
		{
			pPrayTex->EnableSysAnim(FALSE);
			pPrayTex->SetCurFrame((m_iEffectFrameCount - 90) / 6);
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX+iOffX+x,m_iScreenY+iOffY+y,pPrayTex);
			g_pGfx->SetRenderMode();
		}
	}
	else
	{
		m_iEffectFrameCount = 0;
		m_bPlayEffect = false;
		//g_pGameControl->SEND_Open_Box_Requese(12,2);		//特效结束，请求奖品
		g_NPC.SetPrayTreeLevel(0xFF);
		//sm_bSendForPrize = true;
		m_bThrowButtonClick = false;
		return;
	}

	m_iEffectFrameCount++;
}

bool CPrayTreeWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();

	int x = m_iScreenX + iX + 10;
	int y = m_iScreenY + iY + 10;

	if(iX > 364 && iY > 83 && iX < 479 && iY < 393)
	{
		VTreeGift* pGifts = g_NPC.GetGiftsByTreeLevel(m_iCurrentLevel);

		int pos = m_pScroll->GetPos() * 3 + (iY - 83) / 39 * 3 + (iX - 364) / 39;

		if(pos >= pGifts->size())
		{
			pTip->SetShow(false);
			return true;
		}
		pTip->Clear();
		pTip->AddText((*pGifts)[pos].strName.c_str());	
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if((iX > 27 && iX < 357 && iY > 209 && iY < 295) ||
		(iX > 45 && iX < 337 && iY > 142 && iY < 210) ||
		(iX > 105 && iX < 256 && iY > 43 && iY < 138))
	{
		pTip->Clear();
		pTip->AddText("祈愿树上挂有各种物品，点击查看每层奖励。");	
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if(iX > 260 && iX < 300 && iY > 316 && iY < 356)
	{
		if(g_NPC.GetGood().GetID() == 0)
		{
			pTip->Clear();
			pTip->AddText("请放入祈愿物品");	
		}
		else
		{
			pTip->Parse(g_NPC.GetGood());
		}
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if(iX > 300 && iX < 340 && iY > 316 && iY < 356)
	{
		if(g_NPC.GetPrayCharm().GetID() == 0)
		{
			pTip->Clear();
			pTip->AddText("请放入祈愿符");	
		}
		else
		{
			pTip->Parse(g_NPC.GetPrayCharm());
		}
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else
	{
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CPrayTreeWnd::HasPrayCharm()
{
	for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6; i++)
	{
		CGood& tmp = SELF.GetPackageGood(i);
		if(tmp.GetID() != 0 && tmp.GetStdMode() == 37 && tmp.GetShape() == 216)
		{
			return true;
		}
	}
	return false;
}