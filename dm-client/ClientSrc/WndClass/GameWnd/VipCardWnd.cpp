#include "VipCardWnd.h"
#include "GameData\GameGlobal.h"
#include "GameData\OtherData.h"
#include "GameControl\GameControl.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/MsgBoxMgr.h"
#include "GameClient/WidgetManager.h"



INIT_WND_POSX(CVipCardWnd,POS_AUTO,POS_AUTO)

CVipCardWnd::CVipCardWnd(void)
{
	m_iIndex = 17450;
	m_iPages = 1;


	m_iLevelExp[0] = 0;
	m_iLevelExp[1] = 300;
	m_iLevelExp[2] = 900;
	m_iLevelExp[3] = 1800;
	m_iLevelExp[4] = 3600;
	m_iLevelExp[5] = 6000;
	m_iLevelExp[6] = 10800;

	m_pTiyan = NULL;
	m_pMonth = NULL;
	m_pQuater = NULL;
	m_pYear = NULL;
	m_pAutoExtern = NULL;
	m_pBuyBtn = NULL;
	m_pUpgradeBtn = NULL;
	m_pExternBtn = NULL;
	m_pBuyTabBtn = m_pWatchPrivilegeTabBtn = m_pMemberTabBtn = m_pUpgradeExternTabBtn = NULL;

	S_VipTrade &Vip = g_OtherData.GetVipTradeStruct();
	if (Vip.byVipType >= 0 && Vip.byEffected)
	{
		m_iShowPage = EVS_VIPMEMBER_PAGE;
	}
	else
	{
		m_iShowPage = EVS_BUY;
	}

	m_strVipTypToName[0] = "";
	m_strVipTypToName[1] = "体验";
	m_strVipTypToName[2] = "月";
	m_strVipTypToName[3] = "季";
	m_strVipTypToName[4] = "年";

	m_bWillOverdue = false;
	//当前时间
	__time32_t t_time;

	_time32(&t_time);
	t_time += g_dwServerTime;

	if (Vip.tEndTime < t_time + 3 * 24 * 3600)//还有3天以内到期
	{
		m_bWillOverdue = true;
	}

}

CVipCardWnd::~CVipCardWnd(void)
{
	g_WidgetMgr.SetShow(EWT_WATCH_PRIVILEGE,false);
	g_WidgetMgr.SetFocus(EWT_WATCH_PRIVILEGE,false);
}

void CVipCardWnd::Draw()
{
	SetControlState();

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if (m_pBuyTabBtn)
	{
		m_pBuyTabBtn->SetBackState((m_iShowPage == EVS_BUY)?BTEX_PUSHED:BTEX_NORMAL);
	}
	if (m_pWatchPrivilegeTabBtn)
	{
		m_pWatchPrivilegeTabBtn->SetBackState((m_iShowPage == EVS_WATCH_PRIVILEGE)?BTEX_PUSHED:BTEX_NORMAL);
	}
	if (m_pMemberTabBtn)
	{
		m_pMemberTabBtn->SetBackState((m_iShowPage == EVS_VIPMEMBER_PAGE)?BTEX_PUSHED:BTEX_NORMAL);
	}
	if (m_pUpgradeExternTabBtn)
	{
		m_pUpgradeExternTabBtn->SetBackState((m_iShowPage == EVS_UPGRADE_EXTERN)?BTEX_PUSHED:BTEX_NORMAL);
	}


	if (m_iShowPage == EVS_WATCH_PRIVILEGE)
	{
		if(m_bFocus)
		{
			g_WidgetMgr.SetFocus(EWT_WATCH_PRIVILEGE,true);
		}

		if(g_WidgetMgr.IsShow(EWT_WATCH_PRIVILEGE))
		{
			g_WidgetMgr.RenderWidget(EWT_WATCH_PRIVILEGE);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 205,m_iScreenY + 325,"正在加载数据，请稍候",COLOR_TEXT_NORMAL,FONT_YAHEI);
		}
	}
	else
	{
		g_WidgetMgr.SetFocus(EWT_WATCH_PRIVILEGE,false);
	}

	S_VipTrade &Vip = g_OtherData.GetVipTradeStruct();
	
	g_pFont->DrawText(m_iScreenX + 243, m_iScreenY + 15, "神武殿", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);

	//画等级
	LPTexture pTex = NULL;
	char strTemp[128] = {0};
	for (int i = 0; i < 7; i++)
	{
		if (i < 4)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17453 + (i % 2) * 2,EP_UI);
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17454 + (i % 2) * 2,EP_UI);
		}

		int iPos = (int)((float)m_iLevelExp[i] / m_iLevelExp[6] * 481);
		sprintf(strTemp,"%d级",i + 1);
		if (i % 2 == 0)
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 15 + iPos,m_iScreenY + 59,pTex);
			g_pFont->DrawText(m_iScreenX + 21 + iPos,m_iScreenY + 61,strTemp,-1);
		}
		else
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 15 + iPos,m_iScreenY + 59 + 47,pTex);
			g_pFont->DrawText(m_iScreenX + 21 + iPos,m_iScreenY + 61 + 55,strTemp,-1);
		}
		
	}


	//进度条
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17451,EP_UI);
	g_pGfx->DrawPartTexture(m_iScreenX + 32,m_iScreenY + 88,pTex,0,0,(int)((float)Vip.iVipExp / m_iLevelExp[6] * 481));


	//画等级,积分等
	g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 145,"您当前神武殿等级为：       当前积分：       下一级所需积分：",COLOR_TEXT_NORMAL,FONT_YAHEI);
	sprintf(strTemp,"%d级",Vip.byVipLevel);
	g_pFont->DrawText(m_iScreenX + 143,m_iScreenY + 145,strTemp,COLOR_TEXT_NUMBER,FONT_YAHEI);
	sprintf(strTemp,"%d",Vip.iVipExp);
	g_pFont->DrawText(m_iScreenX + 245,m_iScreenY + 145,strTemp,COLOR_TEXT_NUMBER,FONT_YAHEI);

	if (Vip.byVipLevel >= 0 && Vip.byVipLevel < 7)
	{
		sprintf(strTemp,"%d",m_iLevelExp[Vip.byVipLevel] - Vip.iVipExp);
	}
	else
	{
		sprintf(strTemp,"-");
	}

	g_pFont->DrawText(m_iScreenX + 386,m_iScreenY + 145,strTemp,COLOR_TEXT_NUMBER,FONT_YAHEI);



	if ((Vip.byVipType <= 0 || !Vip.byEffected) && m_iShowPage == EVS_BUY)
	{
		g_pFont->DrawText(m_iScreenX + 87,m_iScreenY + 305,"（限第1次）",0xFFC13A3A,FONT_YAHEI);
	}

	if (Vip.byVipType > 0 && Vip.byEffected)
	{
		g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 167,"您是       用户，积分成长速度：+   点/天",COLOR_TEXT_NORMAL,FONT_YAHEI);
		int iAddExp = 0;
		switch (Vip.byVipType)
		{
		case 1:
			sprintf(strTemp," 体验");
			iAddExp = 2;
			break;
		case 2:
			sprintf(strTemp," 包月");
			iAddExp = 5;
			break;
		case 3:
			sprintf(strTemp," 包季");
			iAddExp = 10;
			break;
		case 4:
			sprintf(strTemp," 包年");
			iAddExp = 15;
			break;
		default:
			sprintf(strTemp,"未开通");
			break;
		}
		g_pFont->DrawText(m_iScreenX + 53,m_iScreenY + 167,strTemp,0xFFC13A3A,FONT_YAHEI);

		sprintf(strTemp,"%d",iAddExp);
		g_pFont->DrawText(m_iScreenX + 220,m_iScreenY + 167,strTemp,0xFFC13A3A,FONT_YAHEI);


		g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 188,"您的会员到期时间为：",COLOR_TEXT_NORMAL,FONT_YAHEI);
		sprintf(strTemp,"%s",g_Timer.GetDateTime("%Y-%m-%d日",Vip.tEndTime));
		g_pFont->DrawText(m_iScreenX + 146,m_iScreenY + 188,strTemp,m_bWillOverdue?0xFFC13A3A:COLOR_TEXT_NUMBER,FONT_YAHEI);


		if (m_iShowPage == EVS_VIPMEMBER_PAGE)
		{
			for (int i = 0; i < 5; i ++)
			{
				if (Vip.byBoxOpened & (1 << i))
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17458,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + 60 + i * 90,m_iScreenY + 262,pTex);
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17459,EP_UI);
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + 31 + i * 90,m_iScreenY + 242,pTex);
					g_pGfx->SetRenderMode();
				}
				else
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17457,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + 60 + i * 90,m_iScreenY + 272,pTex);
				}
			}
		}

		if (m_iShowPage != EVS_WATCH_PRIVILEGE)
		{
			if (m_iShowPage == EVS_UPGRADE_EXTERN)
			{
				g_pFont->DrawText(m_iScreenX + 177 + 50 - 15,m_iScreenY + 243,"续费神武殿会员",0xFFC13A3A,FONT_YAHEI);
				
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 356,"无论选择升级或续费，都能获得对应的礼包奖励（请保证有1格空余包裹）。",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 373,"当前支付方式低于勾选的支付方式时才能升级，并从升级当天开始重新计算神武殿会员到期",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 390,"时间（之前未到期时间作废）。",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 407,"当前支付方式与勾选的支付方式相同时才能续费，并在当前神武殿会员到期时间上额外延长",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 424,"（之前未到期时间不作废）。",COLOR_TEXT_NORMAL,FONT_YAHEI);
			}
			else
			{					
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 356,"每次开通神武殿会员或续费时，都能获得对应的礼包奖励（请保证有1格空余包裹）。",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 373,"每次消费10元宝即可获得1点积分（低于10元宝不获得积分，每天最多获得10点积分）。",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 390,"提升神武殿会员等级，可获得更好的特权服务。",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 407,"勾选自动续费，即可享受9折续费优惠。",COLOR_TEXT_NORMAL,FONT_YAHEI);
				g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 424,"使用神武殿密钥即可开启神武殿宝箱获取奖励，每天最多开启5次。",COLOR_TEXT_NORMAL,FONT_YAHEI);
			}
		}
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 167,"您不是神武殿会员",COLOR_TEXT_NORMAL,FONT_YAHEI);
 
		if (m_iShowPage == EVS_BUY)
		{
			g_pFont->DrawText(m_iScreenX + 177 + 50 - 18,m_iScreenY + 243,"开通神武殿会员",0xFFC13A3A,FONT_YAHEI);
	
			g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 358,"每次开通神武殿会员或续费时，都能获得对应的礼包奖励（请保证有1格空余包裹）。",COLOR_TEXT_NORMAL,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 375,"提升神武殿等级，可获得更好的特权服务。",COLOR_TEXT_NORMAL,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 392,"成为神武殿会员后每天可免费开启5次神武殿宝箱，获取各种奖励。",COLOR_TEXT_NORMAL,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 26,m_iScreenY + 409,"一旦成为过神武殿会员，就不能再使用“10元宝体验”途径开通神武殿会员。",COLOR_TEXT_NORMAL,FONT_YAHEI);
		}
	}

}

void CVipCardWnd::OnCreate()
{
	SetCloseButton(510,11, 80);

	m_pAutoExtern = new CCtrlRadio();
	AddControl(m_pAutoExtern);
	m_pAutoExtern->Create(this,238,186,125,126,127,128,110,20);
	m_pAutoExtern->SetText("自动续费（9折）",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);


	S_VipTrade &Vip = g_OtherData.GetVipTradeStruct();


	m_pMonth = new CCtrlRadio();
	AddControl(m_pMonth);

	m_pQuater = new CCtrlRadio();
	AddControl(m_pQuater);

	m_pYear = new CCtrlRadio();
	AddControl(m_pYear);
	


	m_pWatchPrivilegeTabBtn = new CCtrlButton();
	AddControl(m_pWatchPrivilegeTabBtn);
	m_pWatchPrivilegeTabBtn->SetText("查看特权", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);


	if (Vip.byVipType > 0 && Vip.byEffected)
	{
		m_pMonth->AddBuddy(m_pQuater);
		m_pMonth->AddBuddy(m_pYear);	
		m_pMonth->SetRadio((Vip.byVipType >= 2)?(Vip.byVipType - 2):0);

		m_pMemberTabBtn = new CCtrlButton();
		AddControl(m_pMemberTabBtn);
		m_pMemberTabBtn->CreateEx(this, 22,215, 120,121,122,123);
		m_pMemberTabBtn->SetText("会员首页", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);


		m_pMonth->Create(this,147,283,125,126,127,128,50,20);
		m_pQuater->Create(this,247,283,125,126,127,128,50,20);
		m_pYear->Create(this,347,283,125,126,127,128,50,20);

		m_pUpgradeExternTabBtn = new CCtrlButton();
		AddControl(m_pUpgradeExternTabBtn);
		m_pUpgradeExternTabBtn->CreateEx(this, 22 + 75,215, 120,121,122,123);
		m_pUpgradeExternTabBtn->SetText("会员升级", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pWatchPrivilegeTabBtn->CreateEx(this, 22 + 75 + 75,215, 120,121,122,123);


		m_pUpgradeBtn = new CCtrlButton();
		AddControl(m_pUpgradeBtn);
		m_pUpgradeBtn->CreateEx(this, 255,312, 95, 96, 97, 98);
		m_pUpgradeBtn->SetText("升 级", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		
		m_pExternBtn = new CCtrlButton();
		AddControl(m_pExternBtn);
		m_pExternBtn->CreateEx(this, 421,175, 165,166,167,168);
		m_pExternBtn->SetText("续 费", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE,FONTSIZE_MIDDLE,0,FONT_YAHEI);
	}
	else
	{
		m_pBuyTabBtn = new CCtrlButton();
		AddControl(m_pBuyTabBtn);
		m_pBuyTabBtn->CreateEx(this, 22,215, 120,121,122,123);
		m_pBuyTabBtn->SetText("开通会员", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		
		m_pWatchPrivilegeTabBtn->CreateEx(this, 22 + 75,215, 120,121,122,123);
		
		
		m_pTiyan = new CCtrlRadio();
		AddControl(m_pTiyan);
		m_pTiyan->Create(this,67,283,125,126,127,128,50,20);
		m_pTiyan->SetText("10元宝体验",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		
		m_pMonth->Create(this,167,283,125,126,127,128,50,20);
		m_pQuater->Create(this,267,283,125,126,127,128,50,20);
		m_pYear->Create(this,367,283,125,126,127,128,50,20);
	
		m_pTiyan->AddBuddy(m_pMonth);
		m_pTiyan->AddBuddy(m_pQuater);
		m_pTiyan->AddBuddy(m_pYear);	
		m_pTiyan->SetRadio(0);

		m_pTiyan->SetTextAlignType(XAL_CENTER);


		m_pBuyBtn = new CCtrlButton();
		AddControl(m_pBuyBtn);
		m_pBuyBtn->CreateEx(this, 244,312, 95, 96, 97, 98);
		m_pBuyBtn->SetText("购 买", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	}

	m_pMonth->SetText("按月支付",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	m_pQuater->SetText("按季支付",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	m_pYear->SetText("按年支付",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);

	m_pMonth->SetTextAlignType(XAL_CENTER);
	m_pQuater->SetTextAlignType(XAL_CENTER);
	m_pYear->SetTextAlignType(XAL_CENTER);


	g_WidgetMgr.LoginVipPrivilege();
	g_WidgetMgr.MoveWindow(EWT_WATCH_PRIVILEGE,m_iScreenX + 23,m_iScreenY + 259,518 - 23,436 - 259);	 	


}

bool CVipCardWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pAutoExtern)
			{
				if (g_OtherData.GetVipTradeStruct().byVipType > 0)
				{
					g_OtherData.GetVipTradeStruct().bAutoExtern = !g_OtherData.GetVipTradeStruct().bAutoExtern;
					g_pGameControl->SEND_Player_Vip_Option(2,g_OtherData.GetVipTradeStruct().bAutoExtern);
				}
				return true;
			}

			if (pControl == m_pBuyBtn)
			{
				int iSelType = m_pTiyan->GetRadio() + 1;
				char strTemp[256] ={0};
				if (g_OtherData.GetVipTradeStruct().byVipType == 0 || !(g_OtherData.GetVipTradeStruct().byEffected))
				{
					switch (iSelType)
					{
					case 1:
						sprintf(strTemp,"您选择了10元宝体验的方式开通会员，您确定吗？");
						break;
					case 2:
						sprintf(strTemp,"您选择了30元宝包月的方式开通会员，您确定吗？");
						break;
					case 3:
						sprintf(strTemp,"您选择了90元宝包季的方式开通会员，您确定吗？");
						break;
					case 4:
						sprintf(strTemp,"您选择了360元宝包年的方式开通会员，您确定吗？");
						break;
					default:
						return true;
						break;
					}

					g_MsgBoxMgr.PopOkCancelBox(strTemp,MSG_CTRL_BUY_VIPTRADE,iSelType);
				}

				return true;
			}

			if (pControl == m_pUpgradeBtn)
			{
				int iSelType = m_pMonth->GetRadio() + 2;
				char strTemp[256] ={0};
				int iVipTypeNow = g_OtherData.GetVipTradeStruct().byVipType;
				if (iVipTypeNow > 0 && iSelType > iVipTypeNow)
				{
					string strNowVipType = m_strVipTypToName[iVipTypeNow] + "费";
					if (iVipTypeNow == 1)
					{
						strNowVipType = "体验";
					}
					sprintf(strTemp,"您当前为%s用户，您选择了按%s支付的方式升级为%s费会员，您确定吗？",strNowVipType.c_str(),m_strVipTypToName[iSelType].c_str(),m_strVipTypToName[iSelType].c_str());
					g_MsgBoxMgr.PopOkCancelBox(strTemp,MSG_CTRL_BUY_VIPTRADE,iSelType);
				}

				return true;
			}

			if (pControl == m_pExternBtn)
			{
				int iSelType = m_pMonth->GetRadio() + 2;
				char strTemp[256] ={0};
				int iVipTypeNow = g_OtherData.GetVipTradeStruct().byVipType;
				if (iVipTypeNow == 0)
				{
					g_MsgBoxMgr.PopSimpleAlert("您不是神武殿会员，无法继费");
					return true;
				}
				if (iVipTypeNow == 1)
				{
					g_MsgBoxMgr.PopSimpleAlert("您是体验用户，无法继费");
					return true;
				}

				if (iVipTypeNow > 0)
				{
					if (iSelType == iVipTypeNow)
					{
						sprintf(strTemp,"您当前为%s费用户，您选择了按%s支付的方式进行续费，您确定吗？",m_strVipTypToName[iVipTypeNow].c_str(),m_strVipTypToName[iVipTypeNow].c_str());
						g_MsgBoxMgr.PopOkCancelBox(strTemp,MSG_CTRL_BUY_VIPTRADE,iSelType);
					}
				}

				return true;
			}

			if (pControl == m_pBuyTabBtn)
			{
				m_iShowPage = EVS_BUY;
				return true;
			}
			if (pControl == m_pWatchPrivilegeTabBtn)
			{
				m_iShowPage = EVS_WATCH_PRIVILEGE;
				return true;
			}
			if (pControl == m_pMemberTabBtn)
			{
				m_iShowPage = EVS_VIPMEMBER_PAGE;
				return true;
			}
			if (pControl == m_pUpgradeExternTabBtn)
			{
				m_iShowPage = EVS_UPGRADE_EXTERN;
				return true;
			}

		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CVipCardWnd::OnLeftButtonDClick(int iX,int iY)
{
	S_VipTrade &Vip = g_OtherData.GetVipTradeStruct();
	if (Vip.byVipType <= 0 || !Vip.byEffected || m_iShowPage != EVS_VIPMEMBER_PAGE)
	{
		return true;
	}

	int iBox = (iX - 60) / 90;
	if (iX >= iBox * 90 + 60 && iX < iBox * 90 + 60 + 64 && iY >= 262 && iY <= 262 + 72)
	{
		if (iBox >= 0 && iBox < 5 && (Vip.byBoxOpened & (1 << iBox)) == 0)
		{
			g_pGameControl->SEND_Player_Vip_Option(6,iBox);
		}
	}

	return true;
}

bool CVipCardWnd::OnMouseMove(int iX, int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);

	char szName[32] ={0};

	for (int i = 0; i < 7; i++)
	{
		int iPosX = (int)((float)m_iLevelExp[i] / m_iLevelExp[6] * 481) + 15;
		int iPosY = 59;
		if (i % 2 == 0)
		{
		}
		else
		{
			iPosY += 47;
		}

		if (iX >= iPosX && iX <= iPosX + 35 && iY >= iPosY && iY <= iPosY + 26)
		{
			sprintf(szName,"%d",i + 109);
			break;
		}
	}

	if (m_pTiyan && m_pTiyan->IsShow() && iX >= m_pTiyan->GetX() && iX <= m_pTiyan->GetX() + 50 && iY >= m_pTiyan->GetY() && iY <= m_pTiyan->GetY() + 20)
	{
		sprintf(szName,"%d",116);
	}
	else if (m_pMonth && m_pMonth->IsShow() && iX >= m_pMonth->GetX() && iX <= m_pMonth->GetX() + 50 && iY >= m_pMonth->GetY() && iY <= m_pMonth->GetY() + 20)
	{
		sprintf(szName,"%d",117);
	}
	else if (m_pQuater && m_pQuater->IsShow() && iX >= m_pQuater->GetX() && iX <= m_pQuater->GetX() + 50 && iY >= m_pQuater->GetY() && iY <= m_pQuater->GetY() + 20)
	{
		sprintf(szName,"%d",118);
	}
	else if (m_pYear && m_pYear->IsShow() && iX >= m_pYear->GetX() && iX <= m_pYear->GetX() + 50 && iY >= m_pYear->GetY() && iY <= m_pYear->GetY() + 20)
	{
		sprintf(szName,"%d",119);
	}


	sTipsCfg * pTipCfg = g_TipsCfg.GetOtherTips(szName);
	if(pTipCfg)
	{
		pTip->AddCfgTips(pTipCfg);

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	S_VipTrade &Vip = g_OtherData.GetVipTradeStruct();
	if (Vip.byVipType > 0 && Vip.byEffected && m_iShowPage == EVS_VIPMEMBER_PAGE)
	{
		int iBox = (iX - 60) / 90;
		if (iX >= iBox * 90 + 60 && iX < iBox * 90 + 60 + 64 && iY >= 272 && iY <= 272 + 72)
		{
			if (iBox >= 0 && iBox < 5)
			{
				if ((Vip.byBoxOpened & (1 << iBox)) == 0)
				{
					pTip->AddText("双击即可开启神武殿宝箱（需要1个神武殿密钥）");
				}
				else
				{
					pTip->AddText("您已开启此神武殿宝箱");
				}

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

void CVipCardWnd::SetControlState()
{
	S_VipTrade &Vip = g_OtherData.GetVipTradeStruct();



	if (m_pBuyBtn)
	{
		m_pBuyBtn->SetShow(false);
		m_pBuyBtn->Disable();
	}
	if (m_pUpgradeBtn)
	{
		m_pUpgradeBtn->SetShow(false);
		m_pUpgradeBtn->Disable();
	}
	//if (m_pExternBtn)
	//{
	//	m_pExternBtn->SetShow(false);
	//	m_pExternBtn->Disable();
	//}
	if (m_pAutoExtern)
	{
		m_pAutoExtern->SetShow(false);
		m_pAutoExtern->Disable();
	}
	if (m_pTiyan)
	{
		m_pTiyan->SetShow(false);
		m_pTiyan->Disable();
	}
	if (m_pMonth)
	{
		m_pMonth->SetShow(false);
		m_pMonth->Disable();
	}
	if (m_pQuater)
	{
		m_pQuater->SetShow(false);
		m_pQuater->Disable();
	}
	if (m_pYear)
	{
		m_pYear->SetShow(false);
		m_pYear->Disable();
	}


	m_pAutoExtern->SetChecked(Vip.bAutoExtern);		

	
	if (m_iShowPage == EVS_WATCH_PRIVILEGE)
	{
	}

	if (Vip.byVipType > 0 && Vip.byEffected)
	{
		if (m_pBuyTabBtn)
		{
			m_pBuyTabBtn->SetShow(false);
			m_pBuyTabBtn->SetEnable(false);
		}

		m_pAutoExtern->SetEnable(true);
		m_pAutoExtern->SetShow(true);

		if (m_iShowPage == EVS_UPGRADE_EXTERN)
		{
			m_pMonth->SetShow(true);
			m_pMonth->SetEnable(true);

			m_pQuater->SetShow(true);
			m_pQuater->SetEnable(true);

			m_pYear->SetShow(true);
			m_pYear->SetEnable(true);

			m_pUpgradeBtn->SetShow(true);
			m_pUpgradeBtn->SetEnable(true);

			//if (m_pExternBtn)
			//{
			//	m_pExternBtn->SetShow(true);
			//	m_pExternBtn->SetEnable(true);
			//}
		}


		if (m_pExternBtn)
		{
			if (Vip.byVipType == m_pMonth->GetRadio() + 2)
			{
				m_pExternBtn->SetEnable(true);
			}
			else
			{
				m_pExternBtn->SetEnable(false);
			}
		}
		if (Vip.byVipType >= m_pMonth->GetRadio() + 2)
		{
			if (m_pUpgradeBtn)
			{
				m_pUpgradeBtn->SetEnable(false);
			}
		}
	}
	else
	{
		if (m_pMemberTabBtn)
		{
			m_pMemberTabBtn->SetShow(false);
			m_pMemberTabBtn->SetEnable(false);
		}
		if (m_pUpgradeExternTabBtn)
		{
			m_pUpgradeExternTabBtn->SetShow(false);
			m_pUpgradeExternTabBtn->SetEnable(false);
		}


		m_pAutoExtern->SetEnable(false);
		m_pAutoExtern->SetShow(false);


		if (m_iShowPage == EVS_BUY)
		{
			m_pBuyBtn->SetShow(true);
			m_pBuyBtn->SetEnable(true);

			m_pTiyan->SetEnable(true);
			m_pTiyan->SetShow(true);

			m_pMonth->SetShow(true);
			m_pMonth->SetEnable(true);

			m_pQuater->SetShow(true);
			m_pQuater->SetEnable(true);

			m_pYear->SetShow(true);
			m_pYear->SetEnable(true);
		}
	}

}

void CVipCardWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_WATCH_PRIVILEGE,m_iScreenX + 23,m_iScreenY + 259);
}

void  CVipCardWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_WATCH_PRIVILEGE,true);

	if (m_pParent)
	{
		m_pParent->Msg(MSG_CTRL_ON_SET_FOCUS,0,this);
	}
}

void CVipCardWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_WATCH_PRIVILEGE,false);
}