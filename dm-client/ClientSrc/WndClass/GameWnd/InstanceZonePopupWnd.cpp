#include "InstanceZonePopupWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "BaseClass/Audio/Audio.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/WooolStoreData.h"
#include "BaseClass/Control/ParserTip.h"

INIT_WND_POSX(CInstanceZonePopupWnd,POS_AUTO,POS_AUTO)

CInstanceZonePopupWnd::CInstanceZonePopupWnd()
{
	m_bModel = false;	

	m_iAlignType = XAL_TOPLEFT;
	m_iType = sm_dwWindowType;  
	
	if(m_iType == 101)
	{
		m_iIndex = 15183;
	}
	else if(m_iType == 100)
	{
		m_iIndex = 17561;
	}
	else
	{
		m_iIndex = 17560;
	}	

	m_pOkBtn = NULL;

	m_pTuTeng = NULL;
	m_pJinBi = NULL;
	m_pYuanBao = NULL;
	m_pBaiHu = NULL;
	m_pJinLong = NULL;

	m_ptCardPos[0].x = 141;
	m_ptCardPos[0].y = 71;
	m_ptCardPos[1].x = 251;
	m_ptCardPos[1].y = 71;
	m_ptCardPos[2].x = 89;
	m_ptCardPos[2].y = 196;
	m_ptCardPos[3].x = 199;
	m_ptCardPos[3].y = 196;
	m_ptCardPos[4].x = 311;
	m_ptCardPos[4].y = 196;

}

CInstanceZonePopupWnd::~CInstanceZonePopupWnd(void)
{

}

void CInstanceZonePopupWnd::Draw( void )
{
// 	//托管了以后自动翻免费的牌
// 	if (m_iType == 100 && g_TrusteeshipData.IsTrusteeship() && !g_TrusteeshipData.IsCaptain())
// 	{
// 		g_pGameControl->SEND_INSTANCEZONE_POPUP(1,1);
// 		CloseWindow();
// 		return;
// 	}

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_iType == 1)
	{
		g_pFont->DrawText(m_iScreenX + 205,m_iScreenY + 37,"机关迷城",COLOR_BTN_MOUSEON,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 80,"战斗介绍：机关迷城乃神工遗族藏匿之地，如今国王有令",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 69,m_iScreenY + 94,"必须将其全部清除，如此重任就由你来完成。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		

		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 128,"战斗目的：30分钟内找到机关符印，开启机关门，并杀死",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 69,m_iScreenY + 142,"门内机关巨兽。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 172,"获胜奖励：大量经验，技能书，装备图纸等",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 185,m_iScreenY + 265,"限制时间：30分钟",0xffffe650,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
	}
	else if(m_iType == 2)
	{
		g_pFont->DrawText(m_iScreenX + 205,m_iScreenY + 37,"逆魔古冢",COLOR_BTN_MOUSEON,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
		
		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 80,"逆魔古冢内怪物皆为土属性，受木属性攻击加成伤害。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		
		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 110,"副本规则：杀死第10个、30个、60个、100个怪物会各",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 69,m_iScreenY + 125,"掉落一把“机关符印”分别用于开启地图四角的“机关门”；",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 69,m_iScreenY + 140,"副本开始后第5、10、15、20分钟，机关门内会各刷出1个逆",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 69,m_iScreenY + 155,"魔；消灭4个逆魔视为最终战斗胜利。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);


		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 185,"获胜奖励：大量经验，技能书，装备图纸等",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 215,"注：玩家以任何形式离开副本皆不能再重新进入。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 185,m_iScreenY + 265,"限制时间：30分钟",0xffffe650,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
	}
	else if(m_iType == 3)
	{
		g_pFont->DrawText(m_iScreenX + 205,m_iScreenY + 37,"保卫中州",COLOR_BTN_MOUSEON,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
		
		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 80,"中州是我们的王城，更是我们的家园。为了身后亲人的",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 69,m_iScreenY + 94,"安全，哪怕战死，也绝不后退一步！",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		
		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 130,"战斗目的：30分钟内保证四根中州砥柱不被摧毁，并杀",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 69,m_iScreenY + 144,"死所有侵犯之敌就能获胜。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 94,m_iScreenY + 172,"获胜奖励：大量经验，技能书，装备图纸等",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 185,m_iScreenY + 265,"限制时间：30分钟",0xffffe650,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
	}
	else if(m_iType == 4)
	{
		g_pFont->DrawText(m_iScreenX + 205,m_iScreenY + 37," 通天塔",COLOR_BTN_MOUSEON,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
		
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 80,"通天旋塔每突破10层时间限制为30分钟，每突破10层系统将会",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 95, "重新计时。玩家可以组建队伍不断向塔的顶层冲击。通天旋塔分为",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 110,"三种关卡，每5层为首领关卡，击败兽骑兵守门卫士后进入首领房",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 125,"间击败首领即可进入下一层。每10层为宝藏关卡，开启4个石柱即可",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 140,"开启宝藏获得奖励，并进入下一层。其余关卡为怪物关卡，消灭这",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 155,"所有的怪物即可进入下一层。层数越高，怪物的能力越强。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 172,"怪物会掉落：通天旋塔令牌，队伍中任何玩家使用可以让在通",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 187,"天旋塔中的全队的人随机向上传送1-5层，95层以上不得使用。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		
		g_pFont->DrawText(m_iScreenX + 110,m_iScreenY + 210,"失败条件：队伍人员全部死亡或者主动传送出去。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);		

		g_pFont->DrawText(m_iScreenX + 110,m_iScreenY + 230,"获胜奖励：大量经验，技能书，装备图纸等",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 168,m_iScreenY + 265,"每十层限制时间：30分钟",0xffffe650,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
	}
	else if(m_iType == 5)
	{
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 80,"战斗将在10秒之后开始。玩家分为猛虎和神龙两个阵营。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 95, "胜利条件：",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 110,"1.击杀怪物所得积分达到1000点。（通过杀怪物获得）",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 125,"2.战场30分钟结束后，所得积分高于对方。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 140,"3.敌方护城主被击毁。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 155,"奖励：会根据击杀怪物所得积分的多少来换取奖励。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 172,"玩法：",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 190,"1.资源区：地图总共有3块资源区，中间的一块中立资源区",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 102,m_iScreenY + 205,"所产资源最多。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);		
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 220,"2.玩家可通过产得的资源在NPC处换取进攻怪物，守护怪物，",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 102,m_iScreenY + 235,"或是守护箭塔（在指定地点可放置）。",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
	}
	else if(m_iType == 100)
	{
		DWORD dwTime = GetTickCount();


		if(dwTime > m_dwStartTime && dwTime - m_dwStartTime < 120000)
		{
			int iLeft = 119 - (dwTime - m_dwStartTime) / 1000;

			int i1 = iLeft / 100;
			int i2 = (iLeft % 100) / 10;
			int i3 = iLeft % 10;

			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17570 + i1,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 202,m_iScreenY + 30,pTex);

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17570 + i2,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 228,m_iScreenY + 30,pTex);

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17570 + i3,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 254,m_iScreenY + 30,pTex);

			_InstanceZonePrize& prize = g_NPC.GetInstancePrize();

			CCtrlButton * pBtn[5] = {m_pTuTeng,m_pJinBi,m_pYuanBao,m_pBaiHu,m_pJinLong};			

			CGood good;
			good.SetID(1);

			for(int i = 0;i < 5;i++)
			{
				if(prize.bOpen[i])
				{
					pBtn[i]->SetShow(false);
					pBtn[i]->SetEnable(false);
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20926,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + m_ptCardPos[i].x,m_iScreenY + m_ptCardPos[i].y,pTex);

					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,78,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + m_ptCardPos[i].x + 18,m_iScreenY + m_ptCardPos[i].y + 35,pTex);

					good.SetLooks(prize.wlooks[i]);
					CGoodGrid::DrawOneGood(m_iScreenX + m_ptCardPos[i].x + 37,m_iScreenY + m_ptCardPos[i].y + 53,good);
				}
				else
				{
					pBtn[i]->SetShow(true);
					pBtn[i]->SetEnable(true);
				}
			}			
		}
	}
	else if (m_iType == 101)
	{
		if(m_iResult == 1)
		{
			g_pFont->DrawText(m_iScreenX + 199,m_iScreenY + 50,"战斗胜利",0xffff0000,FONT_YAHEI,18,DTF_Center|DTF_BlackFrame);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 199,m_iScreenY + 50,"战斗失败",0xffff0000,FONT_YAHEI,18,DTF_Center|DTF_BlackFrame);
		}
	}

}

void CInstanceZonePopupWnd::OnCreate()
{
	if(m_iType >= 1 && m_iType <= 5)
	{
		m_pOkBtn = new CCtrlButton;
		AddControl(m_pOkBtn);
		m_pOkBtn->CreateEx(this,187,284,17581,17582,17583);	
		m_pOkBtn->SetText("开始战斗",0xfffeaf65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		m_pOkBtn->SetTextOff(1,-1);	
	}
	else if(m_iType == 100)
	{
		m_pTuTeng = new CCtrlButton;
		AddControl(m_pTuTeng);
		m_pTuTeng->CreateEx(this,140,70,20927,20927,20927);	
		//m_pTuTeng->SetText("图  腾",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		m_pTuTeng->SetTips("免费开启，不需要任何条件");
		//m_pTuTeng->SetTextOff(2,0);

		m_pJinBi = new CCtrlButton;
		AddControl(m_pJinBi);
		m_pJinBi->CreateEx(this,250,70,20928,20928,20928);	
		//m_pJinBi->SetText("金  币",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		m_pJinBi->SetTips("需要上交一定游戏币才能开启");
		//m_pTuTeng->SetTextOff(2,0);

		m_pYuanBao = new CCtrlButton;
		AddControl(m_pYuanBao);
		m_pYuanBao->CreateEx(this,88,195,20929,20929,20929);	
		//m_pYuanBao->SetText("绑定元宝",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		m_pYuanBao->SetTips("需要上交一定数量绑定元宝才能开启");
		//m_pTuTeng->SetTextOff(2,0);
		
		m_pBaiHu = new CCtrlButton;
		AddControl(m_pBaiHu);
		m_pBaiHu->CreateEx(this,198,195,20930,20930,20930);	
		//m_pBaiHu->SetText("白  虎",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		m_pBaiHu->SetTips("需要虎烈银卡用户才能开启");
		//m_pTuTeng->SetTextOff(2,0);

		m_pJinLong = new CCtrlButton;
		AddControl(m_pJinLong);
		m_pJinLong->CreateEx(this,310,195,20931,20931,20931);	
		//m_pJinLong->SetText("金  龙",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		m_pJinLong->SetTips("需要龙骧金卡用户才能开启");
		//m_pTuTeng->SetTextOff(2,0);

		SetLifeTime(m_dwStartTime + 120000);
	}
	else if (m_iType == 101)
	{
		m_pOkBtn = new CCtrlButton;
		AddControl(m_pOkBtn);
		m_pOkBtn->CreateEx(this,160,204,17581,17582,17583);	
		m_pOkBtn->SetText("确 定",0xfffeaf65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		m_pOkBtn->SetTextOff(1,-1);	

		m_pMarkViewer = new CMarkViewer(18,15);
		AddControl(m_pMarkViewer);
		m_pMarkViewer->Create(this,86,92,318 - 86,227 - 92);
		m_pMarkViewer->SetTagText(&m_TagText);
	}
}

bool CInstanceZonePopupWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /*= NULL*/ )
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			_InstanceZonePrize & stPrize = g_NPC.GetInstancePrize();

			if(pControl == m_pOkBtn)
			{
				OnClickCloseButton();
				return true;
			}			
			else if(pControl == m_pTuTeng)
			{				
				g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
				g_pGameControl->SEND_INSTANCEZONE_POPUP(1,1);				
				return true;
			}
			else if(pControl == m_pJinBi)
			{
				if(stPrize.iNeedGold > SELF.GetGold())
				{
					g_MsgBoxMgr.PopTagAlert("你包裹中金币数量不足，无法开启该卡片");
				}
				else
				{
					//g_pGameControl->SEND_INSTANCEZONE_POPUP(1,2);
					g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
					char str[256] = "";
					sprintf(str,"你要使用%d金币开启卡片吗？",stPrize.iNeedGold);
					g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_OPENDOOR,2);
				}
							
				return true;
			}
			else if(pControl == m_pYuanBao)
			{
				if(stPrize.byNeedYuanBao > SELF.GetBindYuanBao())
				{
					g_MsgBoxMgr.PopTagAlert("你包裹中的绑定元宝数量不足，无法开启该卡片");
				}
				else
				{
					//g_pGameControl->SEND_INSTANCEZONE_POPUP(1,4);	
					g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
					char str[256] = "";
					sprintf(str,"你要使用%d个绑定元宝开启卡片吗？",stPrize.byNeedYuanBao);
					g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_OPENDOOR,4);
				}

				return true;
			}			
			else if(pControl == m_pBaiHu)
			{
				if(SELF.GetVipCardType() != 1 && SELF.GetVipCardType() != 2)
				{
					__time32_t t_time;
					_time32(&t_time);
					t_time += g_dwServerTime;

					tm* ptm = _localtime32(&t_time);
					if (ptm)
					{
						if(ptm->tm_mon != 12)
						{
							ptm->tm_sec = 0;
							ptm->tm_min = 0;
							ptm->tm_hour = 0;
							ptm->tm_mday = 1;
							ptm->tm_mon++;
						}
						else
						{
							ptm->tm_sec = 0;
							ptm->tm_min = 0;
							ptm->tm_hour = 0;
							ptm->tm_mday = 1;
							ptm->tm_mon = 1;
							ptm->tm_year++;
						}
					}

					__time32_t t_NextMonth = _mktime32(ptm);

					int days = int(t_NextMonth - t_time) / 3600 / 24;

					

					g_WooolStoreMgr.SetQuickBuyItem("虎烈银卡");
					CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

					if(QuickBuyData.pItem)
					{
						char str[256] = "";
						sprintf(str,"你不是虎烈银卡会员，无法开启该卡牌。虎烈银卡按自然月结算，今天距离本月结束还剩%d天。你是否要支付%d元宝，立刻成为虎烈银卡会员？",days,QuickBuyData.pItem->iPrice);
						QuickBuyData.strMsg = str;
						QuickBuyData.iType = 1;
						QuickBuyData.iUseAfterBuyType = 2;
						g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);						
					}
				}
				else if(SELF.GetVipCardType() == 2)
				{
					g_MsgBoxMgr.PopSimpleAlert("你不是虎烈银卡会员，无法开启该卡牌。");
				}
				else
				{
					g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
					g_pGameControl->SEND_INSTANCEZONE_POPUP(1,8);	
				}

				return true;
			}	
			else if(pControl == m_pJinLong)
			{
				if(SELF.GetVipCardType() != 2 && SELF.GetVipCardType() != 1)
				{
					__time32_t t_time;
					_time32(&t_time);
					t_time += g_dwServerTime;

					tm* ptm = _localtime32(&t_time);
					if (ptm)
					{
						if(ptm->tm_mon != 12)
						{
							ptm->tm_sec = 0;
							ptm->tm_min = 0;
							ptm->tm_hour = 0;
							ptm->tm_mday = 1;
							ptm->tm_mon++;
						}
						else
						{
							ptm->tm_sec = 0;
							ptm->tm_min = 0;
							ptm->tm_hour = 0;
							ptm->tm_mday = 1;
							ptm->tm_mon = 1;
							ptm->tm_year++;
						}
					}

					__time32_t t_NextMonth = _mktime32(ptm);

					int days = int(t_NextMonth - t_time) / 3600 / 24;

					g_WooolStoreMgr.SetQuickBuyItem("龙骧金卡");
					CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

					if(QuickBuyData.pItem)
					{
						char str[256] = "";
						sprintf(str,"你不是龙骧金卡会员，无法开启该卡牌。龙骧金卡按自然月结算，今天距离本月结束还剩%d天。你是否要支付%d元宝，立刻成为龙骧金卡会员？",days,QuickBuyData.pItem->iPrice);	
						QuickBuyData.strMsg = str;
						QuickBuyData.iType = 1;
						QuickBuyData.iUseAfterBuyType = 1;
						g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);						
					}
				}
				else if(SELF.GetVipCardType() == 1)
				{
					g_MsgBoxMgr.PopSimpleAlert("你不是龙骧金卡会员，无法开启该卡牌。");
				}
				else
				{
					g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
					g_pGameControl->SEND_INSTANCEZONE_POPUP(1,16);		
				}

				return true;
			}		
		}
		break;
	case MSG_CTRL_InstanceZonePopUp_Wnd:
		{
			if ((dwData == 0 || dwData == 1) && pControl)
			{
				m_iResult = dwData;
				string strShowMsg = (char *) pControl;
				m_TagText.Parse(strShowMsg);
			}
		}
		break;

	default:
		break;
	}	

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CInstanceZonePopupWnd::OnMouseMove( int iX,int iY )
{
	if(m_iType == 100)
	{
		for(int i = 0;i < 5;i++)
		{
			if(g_NPC.GetInstancePrize().bOpen[i] && iX > m_ptCardPos[i].x && iY > m_ptCardPos[i].y && iX < m_ptCardPos[i].x + 74 && iY < m_ptCardPos[i].y + 104)
			{
				CParserTip * pTip = g_pControl->GetTipWnd();
				pTip->Clear();
				pTip->AddText(g_NPC.GetInstancePrize().name[i].c_str());			

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