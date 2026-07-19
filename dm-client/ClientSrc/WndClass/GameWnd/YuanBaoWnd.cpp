#include "YuanBaoWnd.h"
#include "GameData/LoginData.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"

INIT_WND_POSX(CYuanBaoWnd,POS_AUTO,POS_AUTO)

CYuanBaoWnd::CYuanBaoWnd()
{
	m_sName = "YuanBaoWnd";

	m_bModel = false;	

	m_iAlignType = XAL_TOPLEFT;

	m_iType = sm_dwWindowType;

	m_iVersion = 2;

	m_iHeaderHeight = 167;
	
	AddTabPage(0,0,MAKELONG(17590,PACKAGE_INTERFACE),20,32+m_iHeaderHeight, 17677,17678,17679,17680,"购买元宝",NULL,false,0,0,1);
	AddTabPage(0,0,MAKELONG(17591,PACKAGE_INTERFACE),100,32+m_iHeaderHeight,17677,17678,17679,17680,"寄售元宝",NULL,false,0,0,1);
	AddTabPage(0,0,MAKELONG(17592,PACKAGE_INTERFACE),180,32+m_iHeaderHeight,17677,17678,17679,17680,"领取金币",NULL,false,0,0,1);		

	m_TabPage.iCurPage = m_iType - 1;

	g_pControl->Msg(MSG_CTRL_TRADEWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCBOOK_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_LUXNPCWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCRECRUIT,OPER_CLOSE);	
	g_pControl->Msg(MSG_CTRL_NPCWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_RELATION_WND,OPER_CUSTOM);
}

CYuanBaoWnd::~CYuanBaoWnd(void)
{

}

bool CYuanBaoWnd::Create(CControl* pParent,int iCurPage)
{
	if (g_Login.GetLoginInExpType() == 1)
	{
		g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_RECREATE);
		return false;
	}

	return CCtrlWindowX::Create(pParent, iCurPage);
}

void CYuanBaoWnd::OnCreate()
{
	SetCloseButton(345,2+m_iHeaderHeight,80);
	SetMask(17590);

	if(m_iType == 1)
	{
		for(int i = 0;i < 6;i++)
		{
			m_pBuyBtn[i] = new CCtrlButton;
			AddControl(m_pBuyBtn[i]);
			m_pBuyBtn[i]->CreateEx(this,300,81+m_iHeaderHeight + 22 * i,95,96,97,98);
			m_pBuyBtn[i]->SetText("购  买",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
			m_pBuyBtn[i]->SetTextOff(0,0);
		}
	}
	else if(m_iType == 2)
	{
		m_pRemoveBtn = new CCtrlButton;
		AddControl(m_pRemoveBtn);
		m_pRemoveBtn->CreateEx(this,300,170+m_iHeaderHeight,95,96,97,98);
		m_pRemoveBtn->SetText("撤销寄售",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pRemoveBtn->SetTextOff(0,0);
		
		m_pSellBtn = new CCtrlButton;
		AddControl(m_pSellBtn);
		m_pSellBtn->CreateEx(this,300,90+m_iHeaderHeight,95,96,97,98);
		m_pSellBtn->SetText("确  定",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pSellBtn->SetTextOff(0,0);

		m_pNumEdit = new CCtrlEdit;
		AddControl(m_pNumEdit);
		m_pNumEdit->CreateEx(this,85,87+m_iHeaderHeight,60,24,0,17593);
		m_pNumEdit->SetDigital(true);
		m_pNumEdit->SetMaxLength(7);
		m_pNumEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,14);
		m_pNumEdit->SetTextOff(2,-2);

		m_pPriceEdit = new CCtrlEdit;
		AddControl(m_pPriceEdit);
		m_pPriceEdit->CreateEx(this,200,87+m_iHeaderHeight,60,24,0,17593);
		m_pPriceEdit->SetDigital(true);
		m_pPriceEdit->SetMaxLength(7);
		m_pPriceEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,14);
		m_pPriceEdit->SetTextOff(2,-2);
	}
	else if(m_iType  == 3)
	{
		m_pTakeMoneyBtn = new CCtrlButton;
		AddControl(m_pTakeMoneyBtn);
		m_pTakeMoneyBtn->CreateEx(this,270,90+m_iHeaderHeight,95,96,97,98);
		m_pTakeMoneyBtn->SetText("领取金币",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pTakeMoneyBtn->SetTextOff(0,0);
	}

	CCtrlWindowX::OnCreate();
}

void CYuanBaoWnd::Draw( void )
{
	_YuanBaoAllInfo::VYuanBaoSellInfo & vecYuanBaoInfo = g_NPC.GetYuanBaoSellInfo().vecYuanBaoSellInfo;

	if(m_iType == 1)
	{
		for(int i = vecYuanBaoInfo.size();i < 6;i++)
		{
			m_pBuyBtn[i]->SetEnable(false);
			m_pBuyBtn[i]->SetShow(false);
		}
	}

	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 158,m_iScreenY + 8+m_iHeaderHeight,"元宝交易",0xfff8c993,FONT_YAHEI,16,DTF_BlackFrame);

	char str[128] = "";

	if(m_iType == 1)
	{		
		g_pFont->DrawText(m_iScreenX + 66,m_iScreenY + 63+m_iHeaderHeight,"玩家姓名",0xffb46428,FONT_YAHEI,14,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 180,m_iScreenY + 63+m_iHeaderHeight,"元宝数量",0xffb46428,FONT_YAHEI,14,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 250,m_iScreenY + 63+m_iHeaderHeight,"单价",0xffb46428,FONT_YAHEI,14,DTF_BlackFrame);
		
		for(int i = 0;i < 6 && i < vecYuanBaoInfo.size();i++)
		{
			m_pBuyBtn[i]->SetEnable(true);
			m_pBuyBtn[i]->SetShow(true);

			_YuanBaoAllInfo::_YuanBaoSellInfo yuanBaoInfo = vecYuanBaoInfo[i];
			g_pFont->DrawText(m_iScreenX + 95,m_iScreenY+m_iHeaderHeight + 84 + 22 * i,yuanBaoInfo.strPlayerName.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame | DTF_Center);

			sprintf(str,"%d",yuanBaoInfo.dwYuanBaoCount);
			g_pFont->DrawText(m_iScreenX + 205,m_iScreenY+m_iHeaderHeight + 84 + 22 * i,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame | DTF_Center);

			sprintf(str,"%d",yuanBaoInfo.dwPrice);
			g_pFont->DrawText(m_iScreenX + 265,m_iScreenY+m_iHeaderHeight + 84 + 22 * i,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame | DTF_Center);
		}
	}
	else if(m_iType == 2)
	{
		g_pFont->DrawText(m_iScreenX + 160,m_iScreenY+m_iHeaderHeight + 61,"寄售元宝",0xffb46428,FONT_YAHEI,14,DTF_BlackFrame);		

		g_pFont->DrawText(m_iScreenX + 30,m_iScreenY+m_iHeaderHeight + 92,"寄售数量",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
		
		g_pFont->DrawText(m_iScreenX + 148,m_iScreenY+m_iHeaderHeight + 92,"寄售单价",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 270,m_iScreenY+m_iHeaderHeight + 92,"万",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 150,m_iScreenY+m_iHeaderHeight + 128,"我的寄售查询",0xffb46428,FONT_YAHEI,14,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 30,m_iScreenY+m_iHeaderHeight + 172,"寄售数量",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17593,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 85,m_iScreenY+m_iHeaderHeight + 167,pTex);
		sprintf(str,"%5d",g_NPC.GetYuanBaoSellInfo().dwMySellNum);
		g_pFont->DrawText(m_iScreenX + 88,m_iScreenY+m_iHeaderHeight + 172,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 148,m_iScreenY+m_iHeaderHeight + 172,"寄售单价",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17594,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 199,m_iScreenY+m_iHeaderHeight + 167,pTex);
		sprintf(str,"%5d",g_NPC.GetYuanBaoSellInfo().dwMySellPrice);
		g_pFont->DrawText(m_iScreenX + 205,m_iScreenY+m_iHeaderHeight + 172,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
	}	
	else if(m_iType == 3)
	{
		g_pFont->DrawText(m_iScreenX + 60,m_iScreenY+m_iHeaderHeight + 92,"可领取的金币数额",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17594,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 167,m_iScreenY+m_iHeaderHeight + 87,pTex);
		sprintf(str,"%5d",g_NPC.GetYuanBaoSellInfo().dwMoneyToTake);
		g_pFont->DrawText(m_iScreenX + 175,m_iScreenY+m_iHeaderHeight + 92,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
	}
}

bool CYuanBaoWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /*= NULL*/ )
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(m_iType == 1)
		{
			_YuanBaoAllInfo::VYuanBaoSellInfo & vecYuanBaoInfo = g_NPC.GetYuanBaoSellInfo().vecYuanBaoSellInfo;

			for(int i = 0;i < 6 && i < vecYuanBaoInfo.size();i++)
			{
				if(pControl == m_pBuyBtn[i])
				{
					m_YuanBaoSellInfo.strUserId = vecYuanBaoInfo[i].strUserId;
					m_YuanBaoSellInfo.strPlayerName = vecYuanBaoInfo[i].strPlayerName;
					m_YuanBaoSellInfo.dwPrice = vecYuanBaoInfo[i].dwPrice;

					g_MsgBoxMgr.PopEditBox("请输入你要购买的元宝数量",MSG_CTRL_BUY_YUANBAO,0,NULL,true);
					return true;
				}
			}
		}
		else if(m_iType == 2)
		{
			if(pControl == m_pRemoveBtn)
			{
				g_pGameControl->Send_YuanBao_Cancel();
				return true;
			}
			else if(pControl == m_pSellBtn)
			{
				if(m_pNumEdit->GetNum() != 0)
				{
					char str[256] = "";
					sprintf(str,"你确定要寄售%d个元宝，单价%d万？",m_pNumEdit->GetNum(),m_pPriceEdit->GetNum());
					g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_SELL_YUANBAO,0);
					return true;
				}
			}			
		}
		else if(m_iType == 3)
		{
			if(pControl == m_pTakeMoneyBtn)
			{
				g_pGameControl->Sned_Take_YuanBaoMoney();
				return true;
			}
		}
	}
	else if(dwMsg == MSG_CTRL_BUY_YUANBAO)
	{
		g_pGameControl->Send_Buy_YuanBao(m_YuanBaoSellInfo.strUserId,m_YuanBaoSellInfo.strPlayerName,atol(g_MsgBoxMgr.GetInstance()->GetInputString()),m_YuanBaoSellInfo.dwPrice);
		return true;
	}
	else if(dwMsg == MSG_CTRL_SELL_YUANBAO)
	{
		g_pGameControl->Send_YuanBao_Sell(m_pNumEdit->GetNum(),m_pPriceEdit->GetNum());
		m_pNumEdit->Clear();
		m_pPriceEdit->Clear();
		return true;
	}
	else if(dwMsg == MSG_CTRL_TABPAGE_TABCHANGE)
	{
		if(dwData != m_TabPage.iCurPage)
		{
			if(dwData == 0)
			{
				g_pGameControl->Send_OtherYuanBao_Query();
			}
			else if(dwData == 1)
			{
				g_pGameControl->Send_MyYuanBao_Query(1);
			}
			else if(dwData == 2)
			{
				g_pGameControl->Send_MyYuanBao_Query(2);
			}
		}

		return true;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

