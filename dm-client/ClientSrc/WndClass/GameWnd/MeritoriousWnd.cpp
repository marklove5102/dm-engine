#include "MeritoriousWnd.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/GameData.h"
#include "GameData/OtherData.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CMeritoriousWnd,POS_AUTO,POS_AUTO)

CMeritoriousWnd::CMeritoriousWnd(void)
{
	m_sName = "MeritoriousWnd";

	m_bModel = false;

	int iType = sm_dwWindowType;

	m_iVersion = 2;

	S_TabPage* pExpExchangePage = AddTabPage(0,0,MAKELONG(17822,PACKAGE_INTERFACE),0,34, 17824,17825,17826,17827,"经验兑换",NULL,true);
	//AddTabPage(0,0,MAKELONG(17822,PACKAGE_INTERFACE),55,35, 115,116,117,118,"功勋值",pExpExchangePage,false,0,0,0,FONT_YAHEI, FONTSIZE_SMALL);
	//AddTabPage(0,0,MAKELONG(17822,PACKAGE_INTERFACE),108,35,115,116,117,118,"声望",pExpExchangePage,false,0,0,0,FONT_YAHEI, FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(17823,PACKAGE_INTERFACE),0,138,17824,17825,17826,17827,"提升功勋等级",NULL,true);		
	
	m_TabPage.iCurPage = (iType == 3)?1:0;
	m_TabPage.vSubTabPage[0].iCurPage = iType - 1;

	g_pControl->Msg(MSG_CTRL_TRADEWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCBOOK_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_LUXNPCWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_NPCRECRUIT,OPER_CLOSE);	
	g_pControl->Msg(MSG_CTRL_NPCWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_RELATION_WND,OPER_CUSTOM);
	
}

CMeritoriousWnd::~CMeritoriousWnd(void)
{
}

void CMeritoriousWnd::OnCreate()
{
	SetCloseButton(366,3,80); 
	int iCurPage = m_TabPage.iCurPage;
	//int iSubPage = m_TabPage.vSubTabPage[0].iCurPage;

	if(iCurPage == 0/* && iSubPage == 0*/)
	{
		m_pMaxBtn = new CCtrlButton;
		AddControl(m_pMaxBtn);
		m_pMaxBtn->CreateEx(this,305,172,95,96,97,98);
		m_pMaxBtn->SetText("最大",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pMaxBtn->SetTextOff(0,0);

		m_pTakeBtn = new CCtrlButton;
		AddControl(m_pTakeBtn);
		m_pTakeBtn->CreateEx(this,120,213,90,91,92,93);
		m_pTakeBtn->SetText("领取",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pTakeBtn->SetTextOff(0,0);
		m_pTakeBtn->SetEnable(false);

		m_pCloseBtn = new CCtrlButton;
		AddControl(m_pCloseBtn);
		m_pCloseBtn->CreateEx(this,228,213,90,91,92,93);
		m_pCloseBtn->SetText("关闭",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pCloseBtn->SetTextOff(0,0);

		m_pExpEdit = new CCtrlEdit;
		AddControl(m_pExpEdit);
		m_pExpEdit->CreateEx(this,62,172,228,18);
		m_pExpEdit->SetDigital(true);
		m_pExpEdit->SetMaxLength(16);
		m_pExpEdit->SetText("",-1,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,12);
		m_pExpEdit->SetTextOff(2,-1);
	}
	//else if(iCurPage == 0 && iSubPage == 1)
	//{
	//	m_pMaxBtn = new CCtrlButton;
	//	AddControl(m_pMaxBtn);
	//	m_pMaxBtn->CreateEx(this,305,172,95,96,97,98);
	//	m_pMaxBtn->SetText("最大",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
	//	m_pMaxBtn->SetTextOff(0,0);

	//	m_pTakeBtn = new CCtrlButton;
	//	AddControl(m_pTakeBtn);
	//	m_pTakeBtn->CreateEx(this,120,213,90,91,92,93);
	//	m_pTakeBtn->SetText("领取",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
	//	m_pTakeBtn->SetTextOff(0,0);
	//	m_pTakeBtn->SetEnable(false);

	//	m_pCloseBtn = new CCtrlButton;
	//	AddControl(m_pCloseBtn);
	//	m_pCloseBtn->CreateEx(this,228,213,90,91,92,93);
	//	m_pCloseBtn->SetText("关闭",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
	//	m_pCloseBtn->SetTextOff(0,0);

	//	m_pExpEdit = new CCtrlEdit;
	//	AddControl(m_pExpEdit);
	//	m_pExpEdit->CreateEx(this,62,172,228,18);
	//	m_pExpEdit->SetDigital(true);
	//	m_pExpEdit->SetMaxLength(16);
	//	m_pExpEdit->SetText("",-1,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,12);
	//	m_pExpEdit->SetTextOff(2,-1);
	//}
	else if(iCurPage == 1)
	{
		m_pTakeBtn = new CCtrlButton;
		AddControl(m_pTakeBtn);
		m_pTakeBtn->CreateEx(this,120,213,90,91,92,93);
		m_pTakeBtn->SetText("升级",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pTakeBtn->SetTextOff(0,0);

		m_pCloseBtn = new CCtrlButton;
		AddControl(m_pCloseBtn);
		m_pCloseBtn->CreateEx(this,228,213,90,91,92,93);
		m_pCloseBtn->SetText("关闭",0xfff1af65,0xfffac897,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12,DTF_BlackFrame);
		m_pCloseBtn->SetTextOff(0,0);
	}

	CCtrlWindowX::OnCreate();
}

void CMeritoriousWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}


	int iCurPage = m_TabPage.iCurPage;
	//int iSubPage = m_TabPage.vSubTabPage[0].iCurPage;
	char strTemp[1024]={0};
	if (iCurPage == 0/* && iSubPage == 0*/)
	{
		g_pFont->DrawText(m_iScreenX + 167,m_iScreenY + 9,"经验兑换",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);		
		g_pFont->DrawText(m_iScreenX + 153,m_iScreenY + 62,"功勋值兑换经验",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
		
		sprintf(strTemp,"您现在累积的群英经验值为：%u",SELF.GetMeritoriousnessExp());
		g_pFont->DrawText(m_iScreenX + 72,m_iScreenY + 89,strTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
		
		sprintf(strTemp,"您现在拥有的功勋值为：%u",SELF.GetMeritoriousnessValue());
		g_pFont->DrawText(m_iScreenX + 72,m_iScreenY + 106,strTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
		
		sprintf(strTemp,"功勋值兑换经验比例： 1:%u",g_OtherData.GetMeritoriousnessUpdate().dwMeriToExp);
		g_pFont->DrawText(m_iScreenX + 72,m_iScreenY + 124,strTemp,-1,FONT_YAHEI,FONTSIZE_SMALL);

		g_pFont->DrawText(m_iScreenX + 61,m_iScreenY + 151,"请输入您希望用来兑换经验的功勋值数量：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
		
		const char* szTitle = m_pExpEdit->GetText();
		__int64	tempInt = strtoul(szTitle,0,10);
		if (tempInt > 0)
		{
			m_pTakeBtn->SetEnable(true);
		}
		else
		{
			m_pTakeBtn->SetEnable(false);
		}

		if (tempInt > SELF.GetMeritoriousnessValue())
		{
			tempInt = SELF.GetMeritoriousnessValue();
			sprintf(strTemp,"%I64u",tempInt);
			m_pExpEdit->SetText(strTemp,-1,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,12);
		}		

		sprintf(strTemp,"可兑换经验数量：%I64d",tempInt * g_OtherData.GetMeritoriousnessUpdate().dwMeriToExp);
		g_pFont->DrawText(m_iScreenX + 61,m_iScreenY + 197,strTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	
	}
	//else if (iCurPage == 0 && iSubPage == 1)
	//{
	//	g_pFont->DrawText(m_iScreenX + 167,m_iScreenY + 9,"经验兑换",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
	//	g_pFont->DrawText(m_iScreenX + 153,m_iScreenY + 62,"声望兑换经验",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);

	//	sprintf(strTemp,"您现在累积的群英经验值为：%u",SELF.GetMeritoriousnessExp());
	//	g_pFont->DrawText(m_iScreenX + 72,m_iScreenY + 89,strTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);

	//	sprintf(strTemp,"您拥有的声望为：%u",SELF.GetFame());
	//	g_pFont->DrawText(m_iScreenX + 72,m_iScreenY + 106,strTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	//
	//	sprintf(strTemp,"声望兑换经验比例： 1:%u",g_OtherData.GetMeritoriousnessUpdate().dwPresToExp);
	//	g_pFont->DrawText(m_iScreenX + 72,m_iScreenY + 124,strTemp,-1,FONT_YAHEI,FONTSIZE_SMALL);

	//	g_pFont->DrawText(m_iScreenX + 61,m_iScreenY + 151,"请输入您希望用来兑换经验的声望数量：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
	//	const char* szTitle = m_pExpEdit->GetText();
	//	__int64 tempInt = strtoul(szTitle,0,10);
	//	if (tempInt > 0)
	//	{
	//		m_pTakeBtn->SetEnable(true);
	//	}
	//	else
	//	{
	//		m_pTakeBtn->SetEnable(false);
	//	}

	//	if (tempInt > SELF.GetFame())
	//	{
	//		tempInt = SELF.GetFame();
	//		sprintf(strTemp,"%u",tempInt);
	//		m_pExpEdit->SetText(strTemp,-1,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,12);
	//	}

	//	sprintf(strTemp,"可兑换经验数量：%I64d",tempInt * g_OtherData.GetMeritoriousnessUpdate().dwPresToExp);
	//	g_pFont->DrawText(m_iScreenX + 61,m_iScreenY + 197,strTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);

	//}
	else if (iCurPage == 1)
	{
		g_pFont->DrawText(m_iScreenX + 167,m_iScreenY + 9,"提升功勋等级",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
		g_pFont->DrawText(m_iScreenX + 84,m_iScreenY + 58,"当前功勋等级：",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
		itoa(SELF.GetMeritoriousnessLevel(),strTemp,10);
		g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 58,strTemp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_SMALL);

		g_pFont->DrawText(m_iScreenX + 84,m_iScreenY + 82,"功勋值兑换比例：",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
		sprintf(strTemp,"1:%u",SELF.GetMeritoriousnessAddSpeed());
		g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 82,strTemp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_SMALL);

		g_pFont->DrawText(m_iScreenX + 84,m_iScreenY + 133,"提升下一级需要功勋值：",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
		if (g_OtherData.GetMeritoriousnessUpdate().dwNextLevel > 0 && g_OtherData.GetMeritoriousnessUpdate().dwNextLevel != -1)
		{
			sprintf(strTemp,"%u",g_OtherData.GetMeritoriousnessUpdate().dwNextLevel);
			g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 133,strTemp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 133,"暂未开放",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
		}

		g_pFont->DrawText(m_iScreenX + 84,m_iScreenY + 157,"功勋值兑换比例：",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);

		if (g_OtherData.GetMeritoriousnessUpdate().dwMostPerHour > 0 && g_OtherData.GetMeritoriousnessUpdate().dwMostPerHour != -1)
		{
			sprintf(strTemp,"1:%u",g_OtherData.GetMeritoriousnessUpdate().dwMostPerHour);
			g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 157,strTemp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 157,"暂未开放",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
		}

		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 196,"当前人物拥有功勋值:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
		itoa(SELF.GetMeritoriousnessValue(),strTemp,10);
		g_pFont->DrawText(m_iScreenX + 220,m_iScreenY + 196,strTemp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_SMALL);
	}
}

bool CMeritoriousWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		int iCurPage = m_TabPage.iCurPage;
		//int iSubPage = m_TabPage.vSubTabPage[0].iCurPage;
		if(iCurPage == 0/* && iSubPage == 0*/)
		{
			if(pControl == m_pMaxBtn)
			{
				char strTemp[128]={0};
				itoa(SELF.GetMeritoriousnessValue(),strTemp,10);
				m_pExpEdit->SetText(strTemp);
				return true;
			}

			if(pControl == m_pTakeBtn)
			{
				const char* szTitle = m_pExpEdit->GetText();
				DWORD tempInt = strtoul(szTitle,0,10);
				if (tempInt > SELF.GetMeritoriousnessExp())
				{
					//tempInt = SELF.GetMeritoriousnessExp();
				    g_MsgBoxMgr.PopSimpleAlert("群英经验值不足，无法兑换！");
					return true;
				}

				g_pGameControl->Send_Meritoriousness_Update(1, 0, tempInt);
				return true;
			}

			if(pControl == m_pCloseBtn)
			{
				CloseWindow();
				return true;
			}
			if(pControl == m_pExpEdit)
			{
				return true;
			}
		}
//		else if(iCurPage == 0 && iSubPage == 1)
//		{
//			if(pControl == m_pMaxBtn)
//			{
//				char strTemp[128]={0};
//				itoa(SELF.GetFame(),strTemp,10);
//				m_pExpEdit->SetText(strTemp);
//				return true;
//			}
//
//			if(pControl == m_pTakeBtn)
//			{
//				const char* szTitle = m_pExpEdit->GetText();
//				DWORD tempInt = strtoul(szTitle,0,10);
//				if (tempInt > SELF.GetMeritoriousnessExp())
//				{
//					tempInt = SELF.GetMeritoriousnessExp();
//				}
//				g_pGameControl->Send_Meritoriousness_Update(1, 1, tempInt);
//				return true;
//			}
//
//			if(pControl == m_pCloseBtn)
//			{
//				CloseWindow();
//				return true;
//			}
//			if(pControl == m_pExpEdit)
//			{
//				return true;
//			}
//
//		}
		else if(iCurPage == 1)
		{
			if(pControl == m_pTakeBtn)
			{
				g_pGameControl->Send_Meritoriousness_Update(2);
				return true;
			}

			if(pControl == m_pCloseBtn)
			{
				CloseWindow();
				return true;
			}
		}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
