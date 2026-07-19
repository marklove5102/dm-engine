#include "FaBaoLevelUpWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/TipsCfg.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/GameGlobal.h"
#include "BaseClass/Audio/Audio.h"


const POINT CFaBaoLevelUpWnd::m_ptGridPos[EFaBaoGoodGridNum] = {	
	{ 121, 80 },	// EFBG_MainFaBao
	{ 121, 249 },	// EFBG_Fu
	{ 61, 165 },	// EFBG_SecondFaBao1
	{ 121, 165 },	// EFBG_SecondFaBao2
	{ 182, 165 },	// EFBG_SecondFaBao3
	{ 121, 80 },	// EFBG_OpenLS_FaBao
	{ 121, 280 },	// EFBG_OpenLS_CaiLiao
	{ 121, 80 },	// EFBG_SLJH_ShengLing
	{ 121, 280 },	// EFBG_SLJH_CaiLiao
	{ 121, 80 },	// EFBG_SLZR_FaBao
	{ 121, 165 },	// EFBG_SLZR_ShengLing
	{ 121, 249 },	// EFBG_SLZR_CaiLiao
	{ 121, 80 },	// EFBG_SLHY_ShengLing
	{ 121, 280 },	// EFBG_SLHY_CaiLiao
	{ 121, 124 },	// EFBG_ChangeLS_FaBao
	{ 121, 290 },	// EFBG_ChangeLS_CaiLiao
	{ 121, 80 },	// EFBG_SLRH_ShengLing1
	{ 77, 125 },	// EFBG_SLRH_ShengLing2
	{ 121, 125 },	// EFBG_SLRH_ShengLing3
	{ 166, 125 },	// EFBG_SLRH_ShengLing4
	{ 121, 170 },	// EFBG_SLRH_ShengLing5
	{ 121, 249 },	// EFBG_SLRH_CaiLiao
	{ 121, 80 },	// EFBG_SLTQ_FaBao
	{ 121, 280 },	// EFBG_SLTQ_CaiLiao
};

const POINT CFaBaoLevelUpWnd::m_ptHoleToConvert[10] = {
	{ 128, 76 },
	{ 160, 85 },
	{ 180, 114 },
	{ 182, 147 },
	{ 160, 176 },
	{ 128, 187 },
	{ 94, 176 },
	{ 76, 147 },
	{ 77, 114 },
	{ 96, 84 },
};



INIT_WND_POSX(CFaBaoLevelUpWnd,POS_AUTO,POS_AUTO)

CFaBaoLevelUpWnd::CFaBaoLevelUpWnd(void)
{
	m_iType = sm_dwWindowType;


	m_pOkBtn = NULL;
	m_pCancelBtn = NULL;

	switch (m_iType)
	{
	case EFB_FBQH:
		m_iIndex = 22600;
		break;
	case EFB_OpenLS:
	case EFB_SLJH:
	case EFB_SLHY:
		m_iIndex = 20910;
		break;
	case EFB_SLZR:
		m_iIndex = 20911;
		break;
	case EFB_ChangeLS:
		m_iIndex = 20912;
		break;
	case EFB_SLRH:
		m_iIndex = 20913;
		break;
	case EFB_SLTQ:
		m_iIndex = 20914;
		break;	
	default:
		m_iIndex = 22600;
		break;

	}	
	
	m_iNeedMoney = 0;
	m_iConditionSatisfy = 0;
	m_iOKClick = 0;
	m_iSuccessFrame = 10000;
	m_iFailFrame = 10000;

	m_iGoodOnGrid = -1;
	m_bInValidGoodOnGrid = false;

	//预读
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,20933,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,20934,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,20935,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,20936,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,20916,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,20917,EP_UI);
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22945,22963,EP_UI);
}

CFaBaoLevelUpWnd::~CFaBaoLevelUpWnd(void)
{
	BackToPackage();	
}

void CFaBaoLevelUpWnd::OnCreate()
{
	SetCloseButton(584,4,80);

	m_pOkBtn = new CCtrlButton;
	AddControl(m_pOkBtn);
	m_pOkBtn->CreateEx(this,150,378,165,166,167);	
	m_pOkBtn->SetText("确 认",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
	m_pOkBtn->SetShow(false);
	m_pOkBtn->SetEnable(false);

	m_pCancelBtn = new CCtrlButton;
	AddControl(m_pCancelBtn);
	m_pCancelBtn->CreateEx(this,380,378,165,166,167);	
	m_pCancelBtn->SetText("取 消",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
	m_pCancelBtn->SetShow(false);
	m_pCancelBtn->SetEnable(false);


	if(m_iType == EFB_ChangeLS)
	{
		m_pHole[0] = new CCtrlRadio;
		AddControl(m_pHole[0]);
		m_pHole[0]->Create(this,30 + 20,178 + 60,17515,17517);
		m_pHole[0]->SetText("金",0xffffba00,0xffab7d01,0xffab7d01,0xff646464,14,DTF_BlackFrame,FONT_YAHEI,-1,-2);
		m_pHole[0]->SetGroup(false);

		m_pHole[1] = new CCtrlRadio;
		AddControl(m_pHole[1]);
		m_pHole[1]->Create(this,70 + 20,178 + 60,17515,17517);
		m_pHole[1]->SetText("木",0xff00ff06,0xff00aa04,0xff00aa04,0xff646464,14,DTF_BlackFrame,FONT_YAHEI,-1,-2);
		m_pHole[1]->SetGroup(false);

		m_pHole[2] = new CCtrlRadio;
		AddControl(m_pHole[2]);
		m_pHole[2]->Create(this,110 + 20,178 + 60,17515,17517);
		m_pHole[2]->SetText("土",0xffffffff,0xffb0b0b0,0xffb0b0b0,0xff646464,14,DTF_BlackFrame,FONT_YAHEI,-1,-2);
		m_pHole[2]->SetGroup(false);

		m_pHole[3] = new CCtrlRadio;
		AddControl(m_pHole[3]);
		m_pHole[3]->Create(this,150 + 20,178 + 60,17515,17517);
		m_pHole[3]->SetText("水",0xff0084ff,0xff005baf,0xff005baf,0xff646464,14,DTF_BlackFrame,FONT_YAHEI,-1,-2);
		m_pHole[3]->SetGroup(false);

		m_pHole[4] = new CCtrlRadio;
		AddControl(m_pHole[4]);
		m_pHole[4]->Create(this,190 + 20,178 + 60,17515,17517);
		m_pHole[4]->SetText("火",0xffff0000,0xffaf0000,0xffaf0000,0xff646464,14,DTF_BlackFrame,FONT_YAHEI,-1,-2);
		m_pHole[4]->SetGroup(false);

		m_pHole[5] = m_pHole[6] = m_pHole[7] = m_pHole[8] = m_pHole[9] = NULL;


		m_pHole[0]->AddBuddy(m_pHole[1]);
		m_pHole[0]->AddBuddy(m_pHole[2]);
		m_pHole[0]->AddBuddy(m_pHole[3]);
		m_pHole[0]->AddBuddy(m_pHole[4]);

		m_pHole[0]->SetTextAlignType(XAL_CENTER);
		m_pHole[1]->SetTextAlignType(XAL_CENTER);
		m_pHole[2]->SetTextAlignType(XAL_CENTER);
		m_pHole[3]->SetTextAlignType(XAL_CENTER);
		m_pHole[4]->SetTextAlignType(XAL_CENTER);
		//------------------------------------------------------------------------------//			

		m_pHoleToConvert[0] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[0]);
		m_pHoleToConvert[0]->Create(this,m_ptHoleToConvert[0].x,m_ptHoleToConvert[0].y,17521,17530,17525,17550);		
		m_pHoleToConvert[0]->SetGroup(false);

		m_pHoleToConvert[1] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[1]);
		m_pHoleToConvert[1]->Create(this,m_ptHoleToConvert[1].x,m_ptHoleToConvert[1].y,17522,17531,17525,17551);
		m_pHoleToConvert[1]->SetGroup(false);

		m_pHoleToConvert[2] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[2]);
		m_pHoleToConvert[2]->Create(this,m_ptHoleToConvert[2].x,m_ptHoleToConvert[2].y,17524,17533,17525,17553);
		m_pHoleToConvert[2]->SetGroup(false);

		m_pHoleToConvert[3] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[3]);
		m_pHoleToConvert[3]->Create(this,m_ptHoleToConvert[3].x,m_ptHoleToConvert[3].y,17523,17532,17525,17552);
		m_pHoleToConvert[3]->SetGroup(false);

		m_pHoleToConvert[4] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[4]);
		m_pHoleToConvert[4]->Create(this,m_ptHoleToConvert[4].x,m_ptHoleToConvert[4].y,17520,17529,17525,17549);
		m_pHoleToConvert[4]->SetGroup(false);

		m_pHoleToConvert[5] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[5]);
		m_pHoleToConvert[5]->Create(this,m_ptHoleToConvert[5].x,m_ptHoleToConvert[5].y,17521,17530,17525,17550);		
		m_pHoleToConvert[5]->SetGroup(false);

		m_pHoleToConvert[6] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[6]);
		m_pHoleToConvert[6]->Create(this,m_ptHoleToConvert[6].x,m_ptHoleToConvert[6].y,17522,17531,17525,17551);
		m_pHoleToConvert[6]->SetGroup(false);

		m_pHoleToConvert[7] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[7]);
		m_pHoleToConvert[7]->Create(this,m_ptHoleToConvert[7].x,m_ptHoleToConvert[7].y,17524,17533,17525,17553);
		m_pHoleToConvert[7]->SetGroup(false);

		m_pHoleToConvert[8] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[8]);
		m_pHoleToConvert[8]->Create(this,m_ptHoleToConvert[8].x,m_ptHoleToConvert[8].y,17523,17532,17525,17552);
		m_pHoleToConvert[8]->SetGroup(false);

		m_pHoleToConvert[9] = new CCtrlRadio;
		AddControl(m_pHoleToConvert[9]);
		m_pHoleToConvert[9]->Create(this,m_ptHoleToConvert[9].x,m_ptHoleToConvert[9].y,17520,17529,17525,17549);
		m_pHoleToConvert[9]->SetGroup(false);

		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[1]);
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[2]);
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[3]);
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[4]);	
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[5]);
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[6]);
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[7]);
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[8]);	
		m_pHoleToConvert[0]->AddBuddy(m_pHoleToConvert[9]);	

		m_pHoleToConvert[0]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[1]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[2]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[3]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[4]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[5]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[6]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[7]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[8]->SetTextAlignType(XAL_CENTER);
		m_pHoleToConvert[9]->SetTextAlignType(XAL_CENTER);

	}
	else if (m_iType == EFB_SLTQ)
	{
		int iStartX = 28,iStartY = 157,iGridX = 75 - 28,iGridY = 209 - 157;

		for (int i = 0; i < 10; i++)
		{
			m_pHole[i] = new CCtrlRadio;
			AddControl(m_pHole[i]);
			m_pHole[i]->Create(this,iStartX + (i % 5) * iGridX,iStartY + (i / 5) * iGridY,0,17510,0,17548);		
			m_pHole[i]->SetTextOff(-1,0);
			m_pHole[i]->SetGroup(false);
			m_pHole[i]->SetEnable(false);

			if (i > 0)
			{
				m_pHole[0]->AddBuddy(m_pHole[i]);
			}
			m_pHole[i]->SetTextAlignType(XAL_CENTER);
		}
	}

}

void CFaBaoLevelUpWnd::Draw()
{	
	m_iConditionSatisfy = IsConditionSatisfied();

	if(m_iConditionSatisfy > 0)
	{
		m_pOkBtn->SetShow(true);
		m_pOkBtn->SetEnable(m_iOKClick != 2);

		m_pCancelBtn->SetShow(true);
		m_pCancelBtn->SetEnable(true);
	}
	else
	{
		m_pOkBtn->SetShow(false);
		m_pOkBtn->SetEnable(false);

		m_pCancelBtn->SetShow(false);
		m_pCancelBtn->SetEnable(false);
	}

	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	LPTexture pTex = NULL;

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22800,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

	char szstr[256] = {0};

	int gridstart = -1;
	int gridend = -1;
	GetGridStartEnd(gridstart, gridend);

	if (gridstart != -1 && gridend != -1)
	{
		for (int i = gridstart; i <= gridend; ++i)
		{
			//g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[i].x + 3,m_iScreenY + m_ptGridPos[i].y + 3,33,34,0x8000ff00);

			if(g_NPC.GetFaBaoGood(i).GetID())
			{
				CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[i].x + 20,m_iScreenY + m_ptGridPos[i].y + 20,g_NPC.GetFaBaoGood(i));
			}
		}
	}

	if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() && m_iGoodOnGrid >= 0 && m_iGoodOnGrid < EFaBaoGoodGridNum)
	{
		if(m_bInValidGoodOnGrid)
		{
			g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[m_iGoodOnGrid].x + 3,m_iScreenY + m_ptGridPos[m_iGoodOnGrid].y + 3,33,34,0x8000ff00);
		}
		else
		{
			g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[m_iGoodOnGrid].x + 3,m_iScreenY + m_ptGridPos[m_iGoodOnGrid].y + 3,33,34,0x80ff0000);			
		}
	}
	
	if (m_iType == EFB_FBQH)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"法宝强化",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 73,m_iScreenY + 55,"请放入您需要强化的主法宝",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 110,m_iScreenY + 140,"放入辅法宝",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 226,"放入法宝强化符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);	



		int iSuccessPercent = 0;

		int iPercent[9] = {100,90,80,70,60,50,50,50,50};

		if(g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetID() && g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao1).GetID()
			&& g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao2).GetID() && g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao3).GetID())
		{
			int iLevelUpTimes = g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetLevelUpTimes();

			if(iLevelUpTimes >= 0 && iLevelUpTimes < 9)
			{
				iSuccessPercent = iPercent[iLevelUpTimes];
			}

			if(g_NPC.GetFaBaoGood(EFBG_FBQH_Fu).GetID())
			{
				iSuccessPercent = 100;
			}
		}

		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 313,"成功率：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17527,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX + 105,m_iScreenY + 316,pTex,0,0,(122 * iSuccessPercent) / 100);
		
		sprintf(szstr,"%d%%",iSuccessPercent);
		g_pFont->DrawText(m_iScreenX + 155,m_iScreenY + 303,szstr,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);


		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请注意: 1、主法宝所有属性强化成功后保留；2、强化失败辅法宝有可能消失。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_iType == EFB_OpenLS)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"开启灵识",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 48,m_iScreenY + 57,"把需要开启灵识的法宝放入下面窗口",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 83,m_iScreenY + 256,"放入开启灵识的材料",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		CGood & goodFaBao = g_NPC.GetFaBaoGood(EFBG_OpenLS_FaBao);

		if(goodFaBao.GetID())
		{
			int iCanDigHole = g_AIGoodMgr.GetCanDigHoles(goodFaBao);
			sprintf(szstr,"法宝可开启灵识数量: %d",iCanDigHole);
			g_pFont->DrawText(m_iScreenX + 77,m_iScreenY + 148,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

			int iDigHole = g_AIGoodMgr.GetDigHole(goodFaBao);
			sprintf(szstr,"已开启灵识数量: %d",iDigHole);
			g_pFont->DrawText(m_iScreenX + 77,m_iScreenY + 168,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

			sprintf(szstr,"可继续开启灵识数量: %d",iCanDigHole - iDigHole);
			g_pFont->DrawText(m_iScreenX + 77,m_iScreenY + 188,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
		}

		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、法宝可以开启灵识；2、放入了开启灵识的材料；3、金币足够支持开启灵识所需费用。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"满足条件后，界面会出现确认按钮，无论开启灵识成功还是失败，材料都会被收取。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_iType == EFB_SLJH)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"圣灵净化",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 83,m_iScreenY + 57,"放入您需要净化的圣灵",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 93,m_iScreenY + 256,"放入圣灵净化符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		CGood & goodShengLing = g_NPC.GetFaBaoGood(EFBG_SLJH_ShengLing);

		if(goodShengLing.GetID())
		{
			char *pLingFuAddAttr[5] = {"破防","反弹伤害","抵抗","吸血","暴击"};

			int iLevel = goodShengLing.GetAC2();

			sprintf(szstr,"%s,%d级",goodShengLing.GetName(),iLevel);
			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 110 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

			const char * p1[5] = {"金","木","土","水","火"};
			int iType = (goodShengLing.GetShape() - 31) / 4;
			int iKind = (goodShengLing.GetShape() - 31) % 4 + 1;

			if (iKind == 1)
			{
				sprintf(szstr,"净化前属性： %s攻击:0-%d",p1[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
			}
			else if(iKind == 2)
			{
				sprintf(szstr,"净化前属性： +%s防御减免: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));
			}			

			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 130 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);


			iKind += 2;

			if (iKind == 3)
			{
				int iAdd = g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel);
				sprintf(szstr,"净化后属性： %s攻击:%d-%d",p1[iType],iAdd,iAdd);
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
			}
			else if(iKind == 4)
			{
				sprintf(szstr,"净化后属性： %s防御减免: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				sprintf(szstr,"净化后属性： +契合度: %.1f%%",iLevel * 0.2f);
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 170 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
			}
		}


		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保：1、放入的圣灵从未被净化；2、放入的圣灵净化符与当前圣灵等级一致；",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"3、金币足够支付当前净化费用。圣灵净化成功率为100%。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_iType == EFB_SLZR)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"圣灵注入",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 76,m_iScreenY + 57,"放入您需要注入的法宝",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 83,m_iScreenY + 140,"放入想要注入的圣灵",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 97,m_iScreenY + 228,"放入圣灵注入符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);


		float fSuccessPercent = 0.0f;

		if(g_NPC.GetFaBaoGood(EFBG_SLZR_FaBao).GetID() && g_NPC.GetFaBaoGood(EFBG_SLZR_ShengLing).GetID() && g_NPC.GetFaBaoGood(EFBG_SLZR_CaiLiao).GetID())
		{
			fSuccessPercent = 1.0f;
		}

		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 313,"成功率：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);		

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17527,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX + 105,m_iScreenY + 316,pTex,0,0,(int)(122 * fSuccessPercent));

		int iPercent = (int)(fSuccessPercent * 100);
		sprintf(szstr,"%d%%",iPercent);
		g_pFont->DrawText(m_iScreenX + 155,m_iScreenY + 303,szstr,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);


		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保：1、放入的圣灵已经开启灵识；2、放入的圣灵与法宝上至少一个灵识属性一致；",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"3、放入的圣灵注入符与圣灵等级一致；4、金币足够支付当前净化费用。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_iType == EFB_SLHY)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"圣灵还原",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 78,m_iScreenY + 57,"放入您需要还原的圣灵",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 97,m_iScreenY + 256,"放入圣灵还原符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		CGood & goodShengLing = g_NPC.GetFaBaoGood(EFBG_SLHY_ShengLing);

		if(goodShengLing.GetID())
		{
			int iLevel = goodShengLing.GetAC2();			

			sprintf(szstr,"%s,%d级",goodShengLing.GetName(),iLevel);
			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 110 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);


			char * p1[5] = {"金","木","土","水","火"};
			int iType = (goodShengLing.GetShape() - 31) / 4;
			int iKind = (goodShengLing.GetShape() - 31) % 4 + 1;

			int iStartY = 150;

			if (iKind == 3)
			{
				int iAdd = g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel);
				sprintf(szstr,"还原前属性： %s攻击:%d-%d",p1[iType],iAdd,iAdd);
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 130 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
			}
			else if(iKind == 4)
			{
				sprintf(szstr,"还原前属性： %s防御减免: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 130 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				sprintf(szstr,"还原前属性： +契合度: %.1f%%",iLevel * 0.2f);
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				iStartY = 170;
			}

			iKind -= 2;	

			if (iKind == 1)
			{
				sprintf(szstr,"还原后属性： %s攻击:0-%d",p1[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));				
			}
			else if(iKind == 2)
			{
				sprintf(szstr,"还原后属性： +%s防御减免: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));				
			}			

			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + iStartY + 38,szstr,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

		}
		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保：1、放入的圣灵为纯净圣灵；2、放入的圣灵还原符与当前纯净圣灵等级一致；",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"3、金币足够支付当前净化费用。圣灵还原成功率为100%。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_iType == EFB_ChangeLS)
	{
		g_pFont->DrawText(m_iScreenX + 263,m_iScreenY + 10,"更改灵识属性",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 33,m_iScreenY + 53,"放入您的法宝并选择需要转换属性的灵识",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 57,m_iScreenY + 220,"请选择您希望该灵识转化为的属性",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 88,m_iScreenY + 268,"放入转换乾坤道具",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);


		CGood & goodFaBao = g_NPC.GetFaBaoGood(EFBG_ChangeLS_FaBao);

		for(int i = 0;i < 10;i++)
		{
			int iHoleType = 0;
			int iKind = 0;
			if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
			{
				iHoleType = ((goodFaBao.GetDemonDark1() >> (i * 3)) & 0x7);
				iKind = (goodFaBao.GetResvEx(i) & 0xf0) >> 4;
			}
			else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
			{
				WORD wHole = (WORD(goodFaBao.GetResvEx3(0))) | (WORD(goodFaBao.GetResvEx3(1)) << 8);
				iHoleType = ((wHole >> ((i - 5)* 3)) & 0x7);
				iKind = (goodFaBao.GetResvEx3(i - 5 + 2) & 0xf0) >> 4;
			}

			if(goodFaBao.GetID() && iHoleType != 0 && iKind == 0)
			{
				m_pHoleToConvert[i]->SetEnable(true);
				m_pHoleToConvert[i]->ReloadTex(17519 + iHoleType,17548 + iHoleType,17528 + iHoleType,17525);
			}
			else
			{
				m_pHoleToConvert[i]->SetChecked(false);
				m_pHoleToConvert[i]->SetEnable(false);
			}

			if (i < 5)
			{
				m_pHole[i]->SetEnable(true);
			}
		}	

		int iHoleNum = m_pHoleToConvert[0]->GetRadio();
		if(iHoleNum != -1)
		{
			int iHoleType = 0;
			
			if(iHoleNum < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
			{
				iHoleType = ((goodFaBao.GetDemonDark1() >> (iHoleNum * 3)) & 0x7);				
			}
			else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
			{
				WORD wHole = (WORD(goodFaBao.GetResvEx3(0))) | (WORD(goodFaBao.GetResvEx3(1)) << 8);
				iHoleType = ((wHole >> ((iHoleNum - 5)* 3)) & 0x7);				
			}
			
			if(iHoleType >= 1 && iHoleType <= 5)
			{
				m_pHole[iHoleType - 1]->SetChecked(false);
				m_pHole[iHoleType - 1]->SetEnable(false);
			}
		}

		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保：1、需要转化属性的灵识内未注入圣灵；2、所选灵识的属性不与希望转换的属性相同；",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"3、金币足够支付转换灵识属性的费用。转换灵识属性100%成功。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_iType == EFB_SLRH)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"圣灵融合",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 76,m_iScreenY + 57,"放入您需要融合的圣灵",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 63,m_iScreenY + 228,"放入与圣灵等级相同圣灵融合符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);


		float fSuccessPercent = 0.0f;

		int iCount = 0;

		for(int i = EFBG_SLRH_ShengLing1;i <= EFBG_SLRH_ShengLing5;++i)
		{
			if(g_NPC.GetFaBaoGood(i).GetID())
			{
				iCount++;
			}
		}

		if(iCount >= 3)
		{
			fSuccessPercent = (iCount - 2) * 0.25f;
		}

		if(g_NPC.GetFaBaoGood(EFBG_SLRH_CaiLiao).GetID() && iCount >= 3)
		{
			fSuccessPercent += 0.25f;
		}

		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 313,"成功率：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);	

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17527,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX + 105,m_iScreenY + 316,pTex,0,0,(int)(122 * fSuccessPercent));

		int iPercent = (int)(fSuccessPercent * 100);
		sprintf(szstr,"%d%%",iPercent);
		g_pFont->DrawText(m_iScreenX + 155,m_iScreenY + 303,szstr,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保：1、放入的圣灵等级相同且数目≥3个；2、放入的圣灵融合符与当前圣灵等级一致；",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"3、金币足够支付本次融合费用。无论融合成功与否，圣灵、圣灵融合符都会被收取。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_iType == EFB_SLTQ)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"圣灵提取",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		g_pFont->DrawText(m_iScreenX + 76,m_iScreenY + 57,"请放入您需要提取圣灵的法宝",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 76,m_iScreenY + 135,"选择您需要提取的圣灵",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 97,m_iScreenY + 260,"放入圣灵提取符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		CGood & goodFaBao = g_NPC.GetFaBaoGood(EFBG_SLTQ_FaBao);

		BYTE byHoleType = 0;
		int iKind = 0;

		for(int i = 0;i < 10;i++)
		{
			if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
			{
				byHoleType = ((goodFaBao.GetDemonDark1() >> (i * 3)) & 0x7);
			}
			else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
			{
				WORD wHole = (WORD(goodFaBao.GetResvEx3(0))) | (WORD(goodFaBao.GetResvEx3(1)) << 8);
				byHoleType = ((wHole >> ((i - 5) * 3)) & 0x7);
			}

			if (i < 5)//前5个孔放在GetResvEx(i)
			{
				iKind = (goodFaBao.GetResvEx(i) & 0xf0) >> 4;
			}
			else//后5个孔放在 GetResvEx3(2)-GetResvEx3(6)
			{							
				iKind = (goodFaBao.GetResvEx3(i - 5 + 2) & 0xf0) >> 4;
			}



			if(goodFaBao.GetID() && iKind > 0)
			{
				m_pHole[i]->SetEnable(true);
				

				if (iKind >= 3 && iKind <= 4)//纯净的
					pTex = g_pTexMgr->GetTex(PACKAGE_items,1670 + byHoleType - 1 + (iKind - 3 ) * 5,EP_UI);					
				else
					pTex = g_pTexMgr->GetTex(PACKAGE_items,1650 + byHoleType - 1 + (iKind - 1 ) * 5,EP_UI);	

				g_pGfx->DrawTextureNL(m_pHole[i]->GetScreenX() + 4,m_pHole[i]->GetScreenY() + (m_pHole[i]->IsChecked()? 3 : 2),pTex);
			}
			else
			{
				m_pHole[i]->SetChecked(false);
				m_pHole[i]->SetEnable(false);
			}
		}


		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保：1、放入的法宝有圣灵可以提取；2、放入的圣灵提取符与装备上圣灵等级一致；",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"3、金币足够支付本次提取费用。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}






	if (m_iNeedMoney >= 0)
	{
		g_pFont->DrawText(m_iScreenX + 49,m_iScreenY + 336,"需要花费：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		ToCommaStr(szstr,m_iNeedMoney);
		if(SELF.GetGold() < m_iNeedMoney)
		{
			g_pFont->DrawText(m_iScreenX + 144,m_iScreenY + 336,szstr,0xffff0000,FONT_YAHEI,12,DTF_BlackFrame | DTF_Center);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 144,m_iScreenY + 336,szstr,0xfffdca66,FONT_YAHEI,12,DTF_BlackFrame | DTF_Center);
		}
	}

	ShowEffect();
}

void CFaBaoLevelUpWnd::ShowEffect()
{
	LPTexture pTex = NULL;

	bool bDrawStand = true;	//绘制待机的NPC

	if(m_iSuccessFrame < 15 * 8)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20951,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		if (m_iType == EFB_FBQH)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您法宝强化成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else  if (m_iType == EFB_OpenLS)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您开启灵识成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLJH)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您圣灵净化成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLZR)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您圣灵注入成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLHY)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您圣灵还原成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_ChangeLS)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您更改灵识属性成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLRH)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您圣灵融合成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLTQ)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您圣灵提取成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}


		if(m_iSuccessFrame < 15 * 4)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20916,EP_UI);

			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(m_iSuccessFrame / 4);			
			}
		}


		if(m_iSuccessFrame < 12 * 4)
		{
			bDrawStand = false;

			LPTexture pSuccessTex = g_pTexMgr->GetTex(PACKAGE_npc1,51564 + m_iSuccessFrame / 4,EP_UI);

			if(pSuccessTex)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pSuccessTex);
			}
		}

		if (m_iSuccessFrame < 18 * 4)
		{
			LPTexture pEffTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22945 + m_iSuccessFrame / 4,EP_UI);

			if(pEffTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pEffTex);
				g_pGfx->SetRenderMode();
			}
		}

		m_iSuccessFrame++;
	}
	else if(m_iFailFrame < 15 * 8)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20951,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		if (m_iType == EFB_FBQH)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"法宝强化失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else  if (m_iType == EFB_OpenLS)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"开启灵识失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLJH)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"圣灵净化失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLZR)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"圣灵注入失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLHY)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"圣灵还原失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_ChangeLS)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"更改灵识属性失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLRH)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"圣灵融合失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_iType == EFB_SLTQ)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"圣灵提取失败！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}

		if(m_iFailFrame < 15 * 4)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20917,EP_UI);	

			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(m_iFailFrame / 4);			
			}
		}


		//if(m_iFailFrame < 6 * 4)
		//{
		//	bDrawStand = false;

		//	LPTexture pFailTex = g_pTexMgr->GetTex(PACKAGE_npc1,51411 + m_iFailFrame / 4);

		//	if(pFailTex)
		//	{
		//		g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pFailTex);
		//	}
		//}

		m_iFailFrame++;
	}	

	if(pTex && m_iType != EFB_SLRH)
	{
		int iPosX = -1;
		int iPosY = -1;

		if(m_iType == EFB_FBQH)
		{
			iPosX = m_ptGridPos[EFBG_FBQH_MainFaBao].x;
			iPosY = m_ptGridPos[EFBG_FBQH_MainFaBao].y;
		}
		else if(m_iType == EFB_OpenLS)
		{
			iPosX = m_ptGridPos[EFBG_OpenLS_FaBao].x;
			iPosY = m_ptGridPos[EFBG_OpenLS_FaBao].y;
		}
		else if(m_iType == EFB_SLJH)
		{
			iPosX = m_ptGridPos[EFBG_SLJH_ShengLing].x;
			iPosY = m_ptGridPos[EFBG_SLJH_ShengLing].y;
		}
		else if(m_iType == EFB_SLZR)
		{
			iPosX = m_ptGridPos[EFBG_SLZR_FaBao].x;
			iPosY = m_ptGridPos[EFBG_SLZR_FaBao].y;
		}
		else if(m_iType == EFB_SLHY)
		{
			iPosX = m_ptGridPos[EFBG_SLHY_ShengLing].x;
			iPosY = m_ptGridPos[EFBG_SLHY_ShengLing].y;
		}
		else if(m_iType == EFB_ChangeLS)
		{
			iPosX = m_ptGridPos[EFBG_ChangeLS_FaBao].x;
			iPosY = m_ptGridPos[EFBG_ChangeLS_FaBao].y;
		}
		else if(m_iType == EFB_SLRH)
		{
			
		}
		else if(m_iType == EFB_SLTQ)
		{
			iPosX = m_ptGridPos[EFBG_SLTQ_FaBao].x;
			iPosY = m_ptGridPos[EFBG_SLTQ_FaBao].y;
		}

		if (iPosX != -1 && iPosY != -1)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX + iPosX,m_iScreenY + iPosY,pTex);
			g_pGfx->SetRenderMode();
		}
	}

	if(bDrawStand)
	{		
		pTex = g_pTexMgr->GetTex(PACKAGE_npc1,51411 + g_pGfx->GetFrameCount() / 8 % 6,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pTex);
	}

}

bool CFaBaoLevelUpWnd::SendMsg()
{
	if (m_iType == EFB_FBQH)
	{
		g_pGameControl->Send_Upgrade_FaBao(g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetID(), g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao1).GetID(),
			g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao2).GetID(), g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao3).GetID(), g_NPC.GetFaBaoGood(EFBG_FBQH_Fu).GetID());
	}
	else  if (m_iType == EFB_OpenLS)
	{
		g_pGameControl->Send_Drill_Equip(g_NPC.GetFaBaoGood(EFBG_OpenLS_FaBao).GetID(), g_NPC.GetFaBaoGood(EFBG_OpenLS_CaiLiao).GetID(), false);
	}
	else if (m_iType == EFB_SLJH)
	{
		g_pGameControl->Send_Upgrade_Fushi(g_NPC.GetFaBaoGood(EFBG_SLJH_ShengLing).GetID(), g_NPC.GetFaBaoGood(EFBG_SLJH_CaiLiao).GetID(), false);
	}
	else if (m_iType == EFB_SLZR)
	{
		g_pGameControl->Send_Embed_Fushi(g_NPC.GetFaBaoGood(EFBG_SLZR_FaBao).GetID(), g_NPC.GetFaBaoGood(EFBG_SLZR_ShengLing).GetID(), g_NPC.GetFaBaoGood(EFBG_SLZR_CaiLiao).GetID(), false);
	}
	else if (m_iType == EFB_SLHY)
	{
		g_pGameControl->Send_Degrade_Fushi(g_NPC.GetFaBaoGood(EFBG_SLHY_ShengLing).GetID(), g_NPC.GetFaBaoGood(EFBG_SLHY_CaiLiao).GetID());
	}
	else if (m_iType == EFB_ChangeLS)
	{
		g_pGameControl->Send_Change_Hole(g_NPC.GetFaBaoGood(EFBG_ChangeLS_FaBao).GetID(),m_pHoleToConvert[0]->GetRadio(),m_pHole[0]->GetRadio(), 
											g_NPC.GetFaBaoGood(EFBG_ChangeLS_CaiLiao).GetID(),false);
	}
	else if (m_iType == EFB_SLRH)
	{
		g_pGameControl->Send_Combine_Fushi(g_NPC.GetFaBaoGood(EFBG_SLRH_CaiLiao).GetID(),g_NPC.GetFaBaoGood(EFBG_SLRH_ShengLing1).GetID(), 
										g_NPC.GetFaBaoGood(EFBG_SLRH_ShengLing2).GetID(),g_NPC.GetFaBaoGood(EFBG_SLRH_ShengLing3).GetID(), 
										g_NPC.GetFaBaoGood(EFBG_SLRH_ShengLing4).GetID(), g_NPC.GetFaBaoGood(EFBG_SLRH_ShengLing5).GetID(),
										false);

	}
	else if (m_iType == EFB_SLTQ)
	{
		g_pGameControl->Send_Remove_Fushi(g_NPC.GetFaBaoGood(EFBG_SLTQ_FaBao).GetID(), m_pHole[0]->GetRadio(), 
										g_NPC.GetFaBaoGood(EFBG_SLTQ_CaiLiao).GetID(), false);
	}

	return true;
}


bool CFaBaoLevelUpWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */ )
{
	switch ( dwMsg )
	{
	case MSG_INPUT_MOVE:
		if(m_iType == EFB_SLTQ)
		{

			CGood & goodFaBao = g_NPC.GetFaBaoGood(EFBG_SLTQ_FaBao);

			if(goodFaBao.GetID())
			{
				WORD w2 = HIWORD( dwData );
				WORD w1 = LOWORD( dwData );

				CParserTip *pTip = g_pControl->GetTipWnd();				

				WORD wHoles = LOWORD(goodFaBao.GetDemonDark1());

				char * p1[5] = {"金","木","土","水","火"};

				char str[256] = "";

				for(int i = 0;i < 10;i++)
				{
					if(m_pHole[i]->IsEnable() && w1 > m_pHole[i]->GetScreenX() && w2 > m_pHole[i]->GetScreenY() 
						&& w1 < m_pHole[i]->GetScreenX() + m_pHole[i]->GetWidth() && w2 < m_pHole[i]->GetScreenY() + m_pHole[i]->GetHeight())
					{
						pTip->Clear();					

						BYTE byHoleType = 0;								
						int iKind = 0,iLevel = 0;
						
						if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
						{
							byHoleType = ((goodFaBao.GetDemonDark1() >> (i * 3)) & 0x7);
							iKind = (goodFaBao.GetResvEx(i) & 0xf0) >> 4;//前5个孔放在GetResvEx(i)
							iLevel = goodFaBao.GetResvEx(i) & 0x0f;			
						}
						else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
						{
							WORD wHole = (WORD(goodFaBao.GetResvEx3(0))) | (WORD(goodFaBao.GetResvEx3(1)) << 8);
							byHoleType = ((wHole >> ((i - 5) * 3)) & 0x7);
							iKind = (goodFaBao.GetResvEx3(i - 5 + 2) & 0xf0) >> 4;//后5个孔放在 GetResvEx3(2)-GetResvEx3(6)
							iLevel = goodFaBao.GetResvEx3(i - 5 + 2) & 0x0f;			
						}



						if(iKind != 0)
						{								

							if(iKind >= 3 && iKind <= 4)
							{
								if (iKind == 3)
								{
									sprintf(str,"%d级纯净%s尖圣灵",iLevel,p1[byHoleType - 1]);
								}
								else if(iKind == 4)
								{
									sprintf(str,"%d级纯净%s刚圣灵",iLevel,p1[byHoleType - 1]);																									
								}								

								pTip->AddText(str,-1,-1);
							}
							else if(iKind >= 1 && iKind <= 2)
							{
								if (iKind == 1)
								{
									sprintf(str,"%d级%s尖圣灵",iLevel,p1[byHoleType - 1]);
								}
								else if(iKind == 2)
								{
									sprintf(str,"%d级%s刚圣灵",iLevel,p1[byHoleType - 1]);
								}								

								pTip->AddText(str,-1,-1);
							}

						}

						int x = w1 + 10;
						int y = w2 + 10;
						pTip->AdjustXY(x,y);
						pTip->Move(x,y);
						pTip->SetShow(true);

						m_pHole[i]->Msg(dwMsg,dwData,pControl);

						return true;	
					}
					wHoles >>= 3;
				}
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOkBtn)
			{
				if(m_iConditionSatisfy > 0 && SendMsg())
				{
					m_iOKClick = 2;
					m_iSuccessFrame = 10000;
					m_iFailFrame = 10000;
				}
				else
				{
					m_iOKClick = 0;
					m_iSuccessFrame = 100000;
					m_iFailFrame = 100000;
				}

				return true;
			}
			else if(pControl == m_pCancelBtn)
			{
				// 取消
				OnClickCloseButton();
				return true;
			}
		}
		break;
	case MSG_CTRL_FABAOLEVELUP_WND:
		{
			if (dwData == 10)//成功
			{
				m_iOKClick = 0;
				m_iSuccessFrame = 0;
				m_iFailFrame = 10000;
				g_pAudio->Play(EAT_OTHER,918,g_pAudio->GetRand()++);
			}
			else if (dwData == 11)//失败
			{
				m_iOKClick = 0;
				m_iSuccessFrame = 10000;
				m_iFailFrame = 0;
				g_pAudio->Play(EAT_OTHER,919,g_pAudio->GetRand()++);
			}
			else if (dwData == 20)
			{
				m_iOKClick = 0;
			}
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CFaBaoLevelUpWnd::OnMouseMove(int iX,int iY)
{
	m_bInValidGoodOnGrid = false;
	m_iGoodOnGrid = -1;

	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);

	CGood * pGood = GetGoodOnGrid(iX,iY,m_iGoodOnGrid);

	stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

	if(pGood && GoodFrom.DropGood.GetID())
	{
		m_bInValidGoodOnGrid = CheckValidGoodOnGrid(m_iGoodOnGrid,GoodFrom.DropGood);
	}	

	if(pGood && pGood->GetID())
	{
		pTip->Parse(*pGood);

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if (m_iGoodOnGrid >= 0)
	{
		switch (m_iGoodOnGrid)
		{
		case EFBG_FBQH_MainFaBao:
			pTip->AddText("请放入主法宝");
			break;
		case EFBG_FBQH_Fu:
			pTip->AddText("放入法宝强化符");
			break;
		case EFBG_FBQH_SecondFaBao1:
		case EFBG_FBQH_SecondFaBao2:
		case EFBG_FBQH_SecondFaBao3:
			pTip->AddText("请放入辅法宝");
			break;
		case EFBG_OpenLS_FaBao:
			pTip->AddText("请把您需要开启灵识的法宝放入格子中，请确保它还有可以开启的灵识。");
			break;
		case EFBG_OpenLS_CaiLiao:
			pTip->AddText("请放入开启灵识的材料。开启灵识的材料的属性决定了开启出来的灵识的属性。");
			break;
		case EFBG_SLJH_ShengLing:
			pTip->AddText("请放入你希望被净化的圣灵，它必须是从未被净化过的。放入的圣灵与净化好的纯净圣灵等级一致。");
			break;
		case EFBG_SLJH_CaiLiao:
			pTip->AddText("请放入圣灵净化符。净化符的等级必须与需要净化符的圣灵一致。");
			break;
		case EFBG_SLZR_FaBao:
			pTip->AddText("请放入需要注入圣灵的法宝，它必须是已经开启过灵识的。");
			break;
		case EFBG_SLZR_ShengLing:
			pTip->AddText("请放入需要注入到法宝上的圣灵或纯净的圣灵，请保证法宝上有满足它注入属性的灵识。");
			break;
		case EFBG_SLZR_CaiLiao:
			pTip->AddText("请放入圣灵注入符。注入符的等级必须与需要注入的圣灵一致");
			break;
		case EFBG_SLHY_ShengLing:
			pTip->AddText("请放入纯净的圣灵，还原前的纯净圣灵与还原后的圣灵等级一致。");
			break;
		case EFBG_SLHY_CaiLiao:
			pTip->AddText("请放入圣灵还原符，还原符的等级必须与需要还原的纯净圣灵等级一致。");
			break;
		case EFBG_ChangeLS_FaBao:
			pTip->AddText("请把您需要更改灵识属性的法宝放入格子中，请确保它已经开启过灵识。");
			break;
		case EFBG_ChangeLS_CaiLiao:
			pTip->AddText("请放入转换乾坤道具。");
			break;
		case EFBG_SLRH_ShengLing1:
		case EFBG_SLRH_ShengLing2:
		case EFBG_SLRH_ShengLing3:
		case EFBG_SLRH_ShengLing4:
		case EFBG_SLRH_ShengLing5:
			pTip->AddText("请放入等级相同的圣灵，最少3颗。经过净化的圣灵不能合成。");
			break;
		case EFBG_SLRH_CaiLiao:
			pTip->AddText("请放入圣灵融合符。融合符的等级必须与需要融合的圣灵一致。");
			break;

		case EFBG_SLTQ_FaBao:
			pTip->AddText("请放入您需要提取圣灵的法宝，它必须是已经注入了圣灵的。");
			break;
		case EFBG_SLTQ_CaiLiao:
			pTip->AddText("请放入圣灵提取符。提取符的等级必须与您需要提取圣灵的等级一致。");
			break;
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	

	CGood & goodFaBao = g_NPC.GetFaBaoGood(EFBG_ChangeLS_FaBao);

	if(m_iType == EFB_ChangeLS && goodFaBao.GetID())
	{
		const char * p[5] = {"金","木","土","水","火"};
		char str[256];

		for(int i = 0;i < 10;i++)
		{
			if(iX > m_ptHoleToConvert[i].x && iX < m_ptHoleToConvert[i].x + 24 && iY > m_ptHoleToConvert[i].y && iY < m_ptHoleToConvert[i].y + 24)
			{
				BYTE byHoleType = 0;
				int iKind = 0;
				if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
				{
					byHoleType = ((goodFaBao.GetDemonDark1() >> (i * 3)) & 0x7);
					iKind = (goodFaBao.GetResvEx(i) & 0xf0) >> 4;//前5个孔放在GetResvEx(i)
				}
				else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
				{
					WORD wHole = (WORD(goodFaBao.GetResvEx3(0))) | (WORD(goodFaBao.GetResvEx3(1)) << 8);
					byHoleType = ((wHole >> ((i - 5) * 3)) & 0x7);
					iKind = (goodFaBao.GetResvEx3(i - 5 + 2) & 0xf0) >> 4;//后5个孔放在 GetResvEx3(2)-GetResvEx3(6)
				}


				if(byHoleType && iKind == 0)
				{					
					sprintf(str,"%s属性灵识，只能注入%s属性圣灵",p[byHoleType - 1],p[byHoleType - 1]);

					pTip->AddText(str);
					int x = m_iScreenX + iX + 10;
					int y = m_iScreenY + iY + 10;
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);

					return true;
				}
			}
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);

}

bool CFaBaoLevelUpWnd::OnLeftButtonUp( int iX, int iY )
{
	int iGoodOnGrid = -1;
	CGood * pGood = GetGoodOnGrid(iX,iY,iGoodOnGrid);

	if (pGood)
	{
		if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() && CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd)
		{
			CGoodGrid::ReleaseDrop();
			g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() && !CheckValidGoodOnGrid(iGoodOnGrid,GoodFrom.DropGood))
			return true;

		CGood temp = *pGood;
		*pGood = GoodFrom.DropGood;
		GoodFrom.DropGood = temp;
		m_iOKClick = 0;

		return true;
	}	
	
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}


int CFaBaoLevelUpWnd::IsConditionSatisfied()
{
	if(m_iType == EFB_FBQH)
		m_iNeedMoney = 10000;
	else
		m_iNeedMoney = 1000;

	if(SELF.GetGold() < m_iNeedMoney)
	{
		// 游戏币不够
		return 0;
	}


	if (m_iType == EFB_FBQH)
	{
		if (g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetID() != 0 
			&& g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao1).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao2).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_FBQH_SecondFaBao3).GetID() != 0)
		{
			return 1;
		}
	}
	else  if (m_iType == EFB_OpenLS)
	{
		if (g_NPC.GetFaBaoGood(EFBG_OpenLS_FaBao).GetID() != 0 
			&& g_NPC.GetFaBaoGood(EFBG_OpenLS_CaiLiao).GetID() != 0)
		{
			return 1;
		}
	}
	else if (m_iType == EFB_SLJH)
	{
		if (g_NPC.GetFaBaoGood(EFBG_SLJH_ShengLing).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_SLJH_CaiLiao).GetID() != 0)
		{
			return 1;
		}
	}
	else if (m_iType == EFB_SLZR)
	{
		if (g_NPC.GetFaBaoGood(EFBG_SLZR_FaBao).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_SLZR_ShengLing).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_SLZR_CaiLiao).GetID() != 0)
		{
			return 1;
		}
	}
	else if (m_iType == EFB_SLHY)
	{
		if (g_NPC.GetFaBaoGood(EFBG_SLHY_ShengLing).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_SLHY_CaiLiao).GetID() != 0)
		{
			return 1;
		}
	}
	else if (m_iType == EFB_ChangeLS)
	{
		if (g_NPC.GetFaBaoGood(EFBG_ChangeLS_FaBao).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_ChangeLS_CaiLiao).GetID() != 0)
		{
			BYTE byHoleNum = m_pHole[0]->GetRadio();
			BYTE byHoleToConverNum = m_pHoleToConvert[0]->GetRadio();

			if(byHoleNum >= 0 && byHoleNum <= 4 && byHoleToConverNum >= 0 && byHoleToConverNum <= 9)
			{
				return 1;
			}
		}
	}
	else if (m_iType == EFB_SLRH)
	{
		int iCount = 0;

		for(int i = EFBG_SLRH_ShengLing1;i <= EFBG_SLRH_ShengLing5;++i)
		{
			if(g_NPC.GetFaBaoGood(i).GetID() != 0)
			{
				iCount++;
			}
		}

		if(iCount >= 3)
		{
			return 1;
		}
	}
	else if (m_iType == EFB_SLTQ)
	{
		if (g_NPC.GetFaBaoGood(EFBG_SLTQ_FaBao).GetID() != 0
			&& g_NPC.GetFaBaoGood(EFBG_SLTQ_CaiLiao).GetID() != 0)
		{
			BYTE byHoleNum = m_pHole[0]->GetRadio();
			if(byHoleNum >= 0 && byHoleNum <= 9)
			{
				return 1;
			}			
		}
	}

	return -999;
}

bool CFaBaoLevelUpWnd::CheckValidGoodOnGrid(int grid,CGood& good)
{	
	if (grid == EFBG_FBQH_MainFaBao)
	{
		if (!g_AIGoodMgr.IsFaBao(good) && !(good.GetStdMode() == 104 && good.GetShape() == 1))
			return false;

		for(int i = EFBG_FBQH_SecondFaBao1;i <= EFBG_FBQH_SecondFaBao3;++i)
		{
			if(g_NPC.GetFaBaoGood(i).GetID())
			{
				if(g_AIGoodMgr.GetFabaoPinJie(good) != g_AIGoodMgr.GetFabaoPinJie(g_NPC.GetFaBaoGood(i)) )
					return false;

				////宝魂 Shape == 1
				if (g_NPC.GetFaBaoGood(i).GetShape() == 1)
				{
					if (good.GetShape() == 1)
					{
						if(good.GetMAC() != g_NPC.GetFaBaoGood(i).GetMAC())
							return false;
					}
					else
					{
						if(good.GetLevelUpTimes() != g_NPC.GetFaBaoGood(i).GetMAC() - 1)
							return false;
					}
				}
				else
				{
					if (good.GetShape() == 1)
					{
						if(good.GetMAC() != g_NPC.GetFaBaoGood(i).GetLevelUpTimes() - 1)
							return false;
					}
					else
					{
						if ( (good.GetShape() != g_NPC.GetFaBaoGood(i).GetShape())
							&& (good.GetLevelUpTimes() != g_NPC.GetFaBaoGood(i).GetLevelUpTimes()) )
							return false;
					}

					return true;
				}				
			}
		}
	}
	else if (grid == EFBG_FBQH_Fu)
	{
		//法宝强化符
		if (good.GetStdMode() != 105)
		{
			return false;
		}
	}
	else if (grid >= EFBG_FBQH_SecondFaBao1 && grid <= EFBG_FBQH_SecondFaBao3)
	{
		if (!g_AIGoodMgr.IsFaBao(good) && !(good.GetStdMode() == 104 && good.GetShape() == 1))
			return false;

		if (g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetID())
		{
			if (g_AIGoodMgr.GetFabaoPinJie(good) != g_AIGoodMgr.GetFabaoPinJie(g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao)) )
				return false;

			if (g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetShape() == 1)
			{
				if (good.GetShape() != 1)
					return false;

				if( good.GetMAC() != g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetMAC() )
					return false;
			}
			else
			{
				if (good.GetShape() == 1)
				{
					if(good.GetMAC() - 1 != g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetLevelUpTimes() )
						return false;
				}
				else if(good.GetLevelUpTimes() != g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetLevelUpTimes()
					|| good.GetShape() != g_NPC.GetFaBaoGood(EFBG_FBQH_MainFaBao).GetShape())
				{
					return false;
				}
			}
			return true;
		}

		for(int i = EFBG_FBQH_SecondFaBao1;i <= EFBG_FBQH_SecondFaBao3;++i)
		{
			if(grid == i)
				continue;

			if(g_NPC.GetFaBaoGood(i).GetID())
			{
				if (g_AIGoodMgr.GetFabaoPinJie(good) != g_AIGoodMgr.GetFabaoPinJie(g_NPC.GetFaBaoGood(i)) )
					return false;

				if (g_NPC.GetFaBaoGood(i).GetShape() == 1)
				{
					if (good.GetShape() == 1)
					{
						if(good.GetMAC() != g_NPC.GetFaBaoGood(i).GetMAC())
							return false;
					}
					else
					{
						if (good.GetLevelUpTimes() != g_NPC.GetFaBaoGood(i).GetMAC() - 1)
							return false;
					}
					return true;
				}
				else
				{
					if (good.GetShape() == 1)
					{
						if(good.GetMAC() - 1 != g_NPC.GetFaBaoGood(i).GetLevelUpTimes())
							return false;
					}
					else
					{
						if (good.GetLevelUpTimes() != g_NPC.GetFaBaoGood(i).GetLevelUpTimes())
							return false;
					}
					return true;
				}
			}
		}
	}
	else if (grid == EFBG_OpenLS_FaBao || grid == EFBG_SLZR_FaBao || grid == EFBG_ChangeLS_FaBao || grid == EFBG_SLTQ_FaBao)
	{
		if (!g_AIGoodMgr.IsFaBao(good))
			return false;
	}
	else if(grid == EFBG_OpenLS_CaiLiao)
	{
		if(!(good.GetStdMode() == 79 && good.GetShape() >= 1 && good.GetShape() <= 6))
		{
			return false;
		}
	}
	else if(grid == EFBG_SLJH_ShengLing)
	{
		if (!g_AIGoodMgr.IsShengLing(good))
		{
			return false;
		}

		if(g_NPC.GetFaBaoGood(EFBG_SLJH_CaiLiao).GetID() && g_NPC.GetFaBaoGood(EFBG_SLJH_CaiLiao).GetAC2() != good.GetAC2())
		{
			return false;
		}

		if(g_AIGoodMgr.IsJingShengLing(good))
		{
			return false;
		}
	}
	else if(grid == EFBG_SLJH_CaiLiao)
	{
		if(!(good.GetStdMode() == 44 && good.GetShape() == 14))
		{
			return false;
		}

		if(g_NPC.GetFaBaoGood(EFBG_SLJH_ShengLing).GetID() && good.GetAC2() != g_NPC.GetFaBaoGood(EFBG_SLJH_ShengLing).GetAC2())
		{
			return false;
		}
	}
	else if (grid == EFBG_SLZR_ShengLing)
	{
		if (!g_AIGoodMgr.IsShengLing(good))
		{
			return false;
		}
	}
	else if(grid == EFBG_SLZR_CaiLiao)
	{
		//圣灵注入符
		if(!(good.GetStdMode() == 44 && good.GetShape() == 11))
		{
			return false;
		}
	}
	else if (grid == EFBG_SLHY_ShengLing)
	{
		if (!g_AIGoodMgr.IsShengLing(good))
		{
			return false;
		}

		if(g_NPC.GetFaBaoGood(EFBG_SLHY_CaiLiao).GetID() && g_NPC.GetFaBaoGood(EFBG_SLHY_CaiLiao).GetAC2() != good.GetAC2())
		{
			return false;
		}

		if(!g_AIGoodMgr.IsJingShengLing(good))
		{
			return false;
		}
	}
	else if(grid == EFBG_SLHY_CaiLiao)
	{
		if(!(good.GetStdMode() == 44 && good.GetShape() == 15))
		{
			return false;
		}

		if(g_NPC.GetFaBaoGood(EFBG_SLHY_ShengLing).GetID() && good.GetAC2() != g_NPC.GetFaBaoGood(EFBG_SLHY_ShengLing).GetAC2())
		{
			return false;
		}
	}
	else if (grid == EFBG_ChangeLS_CaiLiao)
	{
		if(!(good.GetStdMode() == 79 && good.GetShape() == 8))
		{
			return false;
		}
	}	
	else if (grid >= EFBG_SLRH_ShengLing1 && grid <= EFBG_SLRH_ShengLing5)
	{
		if (!g_AIGoodMgr.IsShengLing(good))
		{
			return false;
		}

		if(g_AIGoodMgr.IsJingShengLing(good))	//纯净符石无法合成
		{
			return false;
		}

		for(int i = EFBG_SLRH_ShengLing1;i <= EFBG_SLRH_ShengLing5;++i)
		{
			if(grid == i)
				continue;

			if(g_NPC.GetFaBaoGood(i).GetID())
			{
				if(good.GetAC2() != g_NPC.GetFaBaoGood(i).GetAC2() || good.GetShape() != g_NPC.GetFaBaoGood(i).GetShape())
				{
					return false;
				}
				else 
				{
					break;
				}
			}

			if(g_NPC.GetFaBaoGood(EFBG_SLRH_CaiLiao).GetID() && g_NPC.GetFaBaoGood(EFBG_SLRH_CaiLiao).GetAC2() != good.GetAC2())
			{
				return false;
			}
		}
	}
	else if(grid == EFBG_SLRH_CaiLiao)
	{
		if(!(good.GetStdMode() == 44 && good.GetShape() == 12))
		{
			return false;
		}

		for(int i = EFBG_SLRH_ShengLing1;i <= EFBG_SLRH_ShengLing5;++i)
		{
			if(g_NPC.GetFaBaoGood(i).GetID())
			{
				if(good.GetAC2() != g_NPC.GetFaBaoGood(i).GetAC2())
				{
					return false;
				}
				else 
				{
					break;
				}
			}
		}
	}
	else if(grid == EFBG_SLTQ_CaiLiao)
	{
		if(!(good.GetStdMode() == 44 && good.GetShape() == 13))
		{
			return false;
		}
	}

	return true;
}


void CFaBaoLevelUpWnd::BackToPackage()
{
	for(int i = 0;i < MAX_FABAO_GOOD_COUNT;++i)
	{
		if (g_NPC.GetFaBaoGood(i).GetID() != 0)
		{
			SELF.PackageGood().BackToArray(g_NPC.GetFaBaoGood(i));
			g_NPC.GetFaBaoGood(i).SetID(0);
		}
	}
}

bool CFaBaoLevelUpWnd::IsInGrid(int iX,int iY, int gridx, int gridy)
{
	if(iX > gridx && iX < gridx + 36 && iY > gridy && iY < gridy + 36)
		return true;

	return false;
}

bool CFaBaoLevelUpWnd::IsInGrid(int iX,int iY, int grid)
{
	if (grid >= 0 && grid < EFaBaoGoodGridNum)
	{
		return IsInGrid(iX, iY, m_ptGridPos[grid].x, m_ptGridPos[grid].y);
	}
	return false;
}

CGood* CFaBaoLevelUpWnd::GetGoodOnGrid(int iX,int iY,int& grid)
{
	grid = -1;

	int gridstart = -1;
	int gridend = -1;

	GetGridStartEnd(gridstart, gridend);

	if (gridstart != -1 && gridend != -1)
	{
		for (int i = gridstart; i <= gridend; ++i)
		{
			if (IsInGrid(iX,iY,i))
			{
				grid = i;
				return &g_NPC.GetFaBaoGood(i);
			}
		}
	}

	return NULL;
}

void CFaBaoLevelUpWnd::GetGridStartEnd(int& gridstart, int& gridend)
{
	gridstart = -1;
	gridend = -1;

	if (m_iType == EFB_FBQH)
	{
		gridstart = EFBG_FBQH_MainFaBao;
		gridend = EFBG_FBQH_SecondFaBao3;
	}
	else  if (m_iType == EFB_OpenLS)
	{
		gridstart = EFBG_OpenLS_FaBao;
		gridend = EFBG_OpenLS_CaiLiao;
	}
	else if (m_iType == EFB_SLJH)
	{
		gridstart = EFBG_SLJH_ShengLing;
		gridend = EFBG_SLJH_CaiLiao;
	}
	else if (m_iType == EFB_SLZR)
	{
		gridstart = EFBG_SLZR_FaBao;
		gridend = EFBG_SLZR_CaiLiao;
	}
	else if (m_iType == EFB_SLHY)
	{
		gridstart = EFBG_SLHY_ShengLing;
		gridend = EFBG_SLHY_CaiLiao;
	}
	else if (m_iType == EFB_ChangeLS)
	{
		gridstart = EFBG_ChangeLS_FaBao;
		gridend = EFBG_ChangeLS_CaiLiao;
	}
	else if (m_iType == EFB_SLRH)
	{
		gridstart = EFBG_SLRH_ShengLing1;
		gridend = EFBG_SLRH_CaiLiao;
	}
	else if (m_iType == EFB_SLTQ)
	{
		gridstart = EFBG_SLTQ_FaBao;
		gridend = EFBG_SLTQ_CaiLiao;
	}
}
