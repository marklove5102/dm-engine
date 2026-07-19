#include "EquipSoulWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/TipsCfg.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/GameGlobal.h"


const POINT CEquipSoulWnd::m_ptGridPos[EquipSoulGridNum] = {
	{ 0, 0 },
	{ 121, 80 },	// ESG_Wake_Equip
	{ 121, 280 },	// ESG_Wake_Hunshi
	{ 121, 80 },	// ESG_LevelUp_Equip
	{ 35, 280 },	// ESG_LevelUp_JieJing1
	{ 78, 280 },	// ESG_LevelUp_JieJing2
	{ 121, 280 },	// ESG_LevelUp_JieJing3
	{ 164, 280 },	// ESG_LevelUp_JieJing4
	{ 207, 280 },	// ESG_LevelUp_JieJing5
	{ 121, 80 },	// ESG_Peel_Equip
	{ 121, 280 },	// ESG_Peel_JieJing
};

//const DWORD CEquipSoulWnd::m_xp[5][3] = {
//	{ 100, 200, 300 },
//	{ 500, 800, 1000 },
//	{ 2000, 3000, 5000 },
//	{ 10000, 15000, 20000 },
//	{ 50000, 70000, 100000 },
//};


INIT_WND_POSX(CEquipSoulWnd,POS_AUTO,POS_AUTO)

CEquipSoulWnd::CEquipSoulWnd(void)
{
	m_pOkBtn = NULL;
	m_pCancelBtn = NULL;
	m_pMarkViewer = NULL;

	m_dwType = sm_dwWindowType;

	if (m_dwType == EST_LevelUp)
		m_iIndex = 21661;
	else
		m_iIndex = 20910;

	m_iGoodOnGrid = 0;
	m_bInValidGoodOnGrid = false;
	m_iNeedMoney = 0;
	m_iConditionSatisfy = 0;

	m_iOKClick = 0;

	m_iSuccessFrame = 10000;
	m_UseBind = 0;

	//预读
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,21662,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,21663,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,21664,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,21666,EP_UI);
}

CEquipSoulWnd::~CEquipSoulWnd(void)
{
	BackToPackage();	
}

void CEquipSoulWnd::OnCreate()
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

	m_pMarkViewer = new CMarkViewer(20,5);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,56,145,150,120);
	m_pMarkViewer->SetTagText(&m_Tag);
	m_pMarkViewer->SetFont(FONT_YAHEI,12);

}

void CEquipSoulWnd::Draw()
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

	m_Tag.Clear();

	LPTexture pTex = NULL;

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21662,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

	char str[256] = "";
	if (m_dwType == EST_Wake)
	{
		g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 10,"器魂觉醒仪式",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 73,m_iScreenY + 55,"放入需要觉醒器魂的装备",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

		if(g_NPC.GetEquipSoulInfo().m_Equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[ESG_Wake_Equip].x + 20,m_iScreenY + m_ptGridPos[ESG_Wake_Equip].y + 20,g_NPC.GetEquipSoulInfo().m_Equip);
			
			if (m_iConditionSatisfy >= 0)
			{			
				int iMaxAddAtrrItems = 0;
				DWORD dwSoulLevelUpExp = g_AIGoodMgr.GetSoulLevelUpExp(g_NPC.GetEquipSoulInfo().m_Equip,iMaxAddAtrrItems);
				if (iMaxAddAtrrItems > 0)
				{
					sprintf(str,"你的%s的器魂觉醒后：",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
					g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 148,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
				}
				char szTemp[128] = {0};
				for(int i = 0;i <iMaxAddAtrrItems; i++)
				{
					string szAttr = g_AIGoodMgr.GetSoulEquipAttr(g_NPC.GetEquipSoulInfo().m_Equip,i);
					if (!szAttr.empty())
					{
						sprintf(szTemp,"%d级器魂：%s",i + 1,szAttr.c_str());
						g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 168 + i * 20,szTemp,0xff9cbfbf,FONT_YAHEI,12);
					}
				}
			}
		}

		g_pFont->DrawText(m_iScreenX + 110,m_iScreenY + 258,"放入妖魂石",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if(g_NPC.GetEquipSoulInfo().m_Hunshi.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[ESG_Wake_Hunshi].x + 20,m_iScreenY + m_ptGridPos[ESG_Wake_Hunshi].y + 20,g_NPC.GetEquipSoulInfo().m_Hunshi);
		}

		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请注意: 1、您放入的装备，必需从未进行过器魂觉醒仪式; 2、放入了“妖魂石”; 3、您所拥有的",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"金币足够支付器魂觉醒的费用。以上条件被满足后，“确定/取消”按钮会出现。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}

		if (m_iConditionSatisfy == -1)
		{
			string str = StringUtil::format("<color=FF92CDDC 您放入的><color=FFFFFF99 %s已经觉醒>\\<color=FFFFFF99 过器魂了><color=FF92CDDC ，无法进行重复觉醒。>\\<color=FF92CDDC 您可以选择“器魂升级”来提>\\<color=FF92CDDC 升它的器魂等级以获得更多属>\\<color=FF92CDDC 性加成。>",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
			m_Tag.Parse(str);
		}
		else if (m_iConditionSatisfy == -2)
		{
			string str = StringUtil::format("<color=FF92CDDC 您放入的><color=FFFFFF99 %s等级太低>\\<color=FF92CDDC ，无法进行器魂觉醒。>\\<color=FF92CDDC 只有圣战级以上装备才能进行>\\<color=FF92CDDC 器魂觉醒。>",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
			m_Tag.Parse(str);
		}		
	}
	else if (m_dwType == EST_LevelUp)
	{
		g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 10,"器魂等级提升",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX + 63,m_iScreenY + 55,"放入需要提升器魂等级的装备",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

		if(g_NPC.GetEquipSoulInfo().m_Equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[ESG_LevelUp_Equip].x + 20,m_iScreenY + m_ptGridPos[ESG_LevelUp_Equip].y + 20,g_NPC.GetEquipSoulInfo().m_Equip);

			if (m_iConditionSatisfy >= 0)
			{
				int iLevel = g_NPC.GetEquipSoulInfo().m_Equip.GetSoulLevel();
				DWORD dwSoleExp = g_NPC.GetEquipSoulInfo().m_Equip.GetSoleExp();
				int iMaxAddAtrrItems;
				DWORD dwSoulLevelUpExp = g_AIGoodMgr.GetSoulLevelUpExp(g_NPC.GetEquipSoulInfo().m_Equip,iMaxAddAtrrItems);

				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 148,"当前器魂经验值：",0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
				if (dwSoulLevelUpExp == 0)
					sprintf(str,"%d级 100%% ",iLevel);
				else
					sprintf(str,"%d级 %d%% ",iLevel,dwSoleExp * 100 / dwSoulLevelUpExp);

				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 168,str,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
				sprintf(str,"           %u/%u",dwSoleExp,dwSoulLevelUpExp);
				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 168,str,0xff9cbfbf,FONT_YAHEI,12,DTF_BlackFrame);

				if (m_iConditionSatisfy > 0)
				{
					int iNextLevel = iLevel;
					DWORD dwSoulLevelUpExp = g_AIGoodMgr.GetSoulLevelUpExp(g_NPC.GetEquipSoulInfo().m_Equip,iMaxAddAtrrItems,iNextLevel);
					DWORD dwAddExp = m_iConditionSatisfy + dwSoleExp;

					while (dwSoulLevelUpExp > 0 && dwAddExp > dwSoulLevelUpExp && iNextLevel < MAX_EQUIP_SOUL_LEVEL)
					{
						dwAddExp -= dwSoulLevelUpExp;
						iNextLevel += 1;
						dwSoulLevelUpExp = g_AIGoodMgr.GetSoulLevelUpExp(g_NPC.GetEquipSoulInfo().m_Equip,iMaxAddAtrrItems,iNextLevel);
					}

					g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 198,"提升后器魂经验值：",0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
					if (dwSoulLevelUpExp == 0)
					{
						dwAddExp = 0;
						sprintf(str,"%d级 -",iNextLevel);
						g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 218,str,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
					}
					else
					{
						sprintf(str,"%d级 %d%% ",iNextLevel,dwAddExp * 100 / dwSoulLevelUpExp);
						g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 218,str,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
						sprintf(str,"           %u/%u",dwAddExp,dwSoulLevelUpExp);
						g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 218,str,0xff9cbfbf,FONT_YAHEI,12,DTF_BlackFrame);
					}

				}
			}
		}

		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 258,"放入“器魂结晶”",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		for(int i = 0;i < 5;i++)
		{
			if(g_NPC.GetEquipSoulInfo().m_Jiejing[i].GetID())
			{
				CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[ESG_LevelUp_JieJing1 + i].x + 20,m_iScreenY + m_ptGridPos[ESG_LevelUp_JieJing1 + i].y + 20,g_NPC.GetEquipSoulInfo().m_Jiejing[i]);
			}
		}

		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请注意：1、提升后器魂等级经验，与您放入的器魂结晶数量相关；2、提升器魂等级需要消",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"耗的游戏币，与您放入的器魂结晶数量相关。以上条件满足后，“确定/取消”按钮会出现。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
		
		if (m_iConditionSatisfy == -1)
		{
			if(g_NPC.GetEquipSoulInfo().m_Equip.GetID())
			{
				//sprintf(str,"您的%s没有进行过觉醒仪式，尚未拥有器魂，请至NPC处先完成器魂觉醒仪式。",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
				//g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,str,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);				

				string str = StringUtil::format("<color=FF92CDDC 您的><color=FFFFFF99 %s还未进行过觉>\\<color=FFFFFF99 醒仪式><color=FF92CDDC ，无法提升器魂等级。>\\<color=FF92CDDC 您可以选择“器魂觉醒”先对>\\<color=FF92CDDC 它进行觉醒后再来提升器魂等>\\<color=FF92CDDC 级。>",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
				m_Tag.Parse(str);
			}
		}
		else if (m_iConditionSatisfy == -2)
		{
			if(g_NPC.GetEquipSoulInfo().m_Equip.GetID())
			{
				//sprintf(str,"您的%s的器魂等级已到达上限，无法再进行提升。",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
				//g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,str,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);

				string str = StringUtil::format("<color=FF92CDDC 您的><color=FFFFFF99 %s已将器魂等级>\\<color=FFFFFF99 提升至上限><color=FF92CDDC ，无法再进行提升。>\\<color=FF92CDDC 越高级的装备，可提升的器魂>\\<color=FF92CDDC 等级上限越多。>",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
				m_Tag.Parse(str);
			}
		}
		else if (m_iConditionSatisfy == -4)
		{
			// 放入的晶石类型不符合
			g_pFont->DrawText(m_iScreenX + 56,m_iScreenY + 145,"您放入的“器魂结晶”类型不",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 56,m_iScreenY + 165,"符合。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if (m_dwType == EST_Peel)
	{
		g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 10,"器魂结晶剥离",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
		g_pFont->DrawText(m_iScreenX + 63,m_iScreenY + 55,"放入需要剥离器魂结晶的装备",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT);

		if(g_NPC.GetEquipSoulInfo().m_Equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[ESG_Peel_Equip].x + 20,m_iScreenY + m_ptGridPos[ESG_Peel_Equip].y + 20,g_NPC.GetEquipSoulInfo().m_Equip);

			if (m_iConditionSatisfy >= 0)
			{			
				int iMaxAddAtrrItems = 0;
				int iLevel = g_NPC.GetEquipSoulInfo().m_Equip.GetSoulLevel();
				DWORD dwSoulExp = g_NPC.GetEquipSoulInfo().m_Equip.GetSoleExp();
				DWORD dwSoulLevelUpExp = 0;

				for (int i = 0; i < iLevel; i++)
				{
					dwSoulLevelUpExp = g_AIGoodMgr.GetSoulLevelUpExp(g_NPC.GetEquipSoulInfo().m_Equip,iMaxAddAtrrItems,i);
					dwSoulExp += dwSoulLevelUpExp;
				}

				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 138,"当前装备共拥有器魂经验值：",0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
				sprintf(str,"%u点",dwSoulExp);
				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 154,str,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);

				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 170,"剥离共可获得器魂结晶：",0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
				
				dwSoulExp -= 1;
				dwSoulExp = dwSoulExp * 70 / 100;				

				if (dwSoulExp == 0)
				{
					dwSoulExp = 1;
				}

				sprintf(str,"%u点",dwSoulExp);
				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 186,str,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);

				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 202,"单次操作最多可剥离5000点",0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
				g_pFont->DrawText(m_iScreenX + 59,m_iScreenY + 218,"器魂结晶",0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				int iJiejing = 0;
				CGood &tmpgood = g_NPC.GetEquipSoulInfo().m_Equip;

				if (g_AIGoodMgr.IsEquipHelmet(tmpgood)) // 头盔
					iJiejing = 1504;
				else if (g_AIGoodMgr.IsEquipNecklace(tmpgood)) // 项链
					iJiejing = 1506;
				else if (g_AIGoodMgr.IsEquipBangle(tmpgood)) // 手镯
					iJiejing = 1503;
				else if (g_AIGoodMgr.IsEquipRing(tmpgood)) // 戒指
					iJiejing = 1502;		
				else if (g_AIGoodMgr.IsEquipCloth(tmpgood) ) // 衣服
					iJiejing = 1509;		
				else if (g_AIGoodMgr.IsEquipBelt(tmpgood)) // 腰带
					iJiejing = 1508;
				else if (g_AIGoodMgr.IsEquipBoot(tmpgood)) // 靴子
					iJiejing = 1507;
				else if (g_AIGoodMgr.IsEquipWeapon(tmpgood)) // 武器
					iJiejing = 1505;

				if (iJiejing != 0)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_items,iJiejing,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + m_ptGridPos[ESG_Peel_JieJing].x + 4,m_iScreenY + m_ptGridPos[ESG_Peel_JieJing].y + 4,pTex);
				}
			}
		}

		g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 258,"可剥离出来的器魂结晶数量",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if (m_iConditionSatisfy <= 0)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请注意: 1、剥离器魂，您只能获得原装备器魂结晶的70%；2、剥离器魂需要消耗的游戏币，",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"与您可获得的器魂结晶数量相关；3、剥离器魂所获得的器魂结晶，将与您绑定。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}

		if (m_iConditionSatisfy == -2)
		{
			string str = StringUtil::format("<color=FF92CDDC 您的><color=FFFFFF99 %s仅拥有1点原>\\<color=FFFFFF99 始器魂经验值，><color=FF92CDDC 无法进行剥离。>\\<color=FF92CDDC 拥有1点器魂经验值的装备可以>\\<color=FF92CDDC 直接进行分解。>",g_NPC.GetEquipSoulInfo().m_Equip.GetName());
			m_Tag.Parse(str);
		}
	}

	g_pFont->DrawText(m_iScreenX + 49,m_iScreenY + 336,"需要花费：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

	ToCommaStr(str,m_iNeedMoney);

	DWORD dwMoneyColor = 0xfffdca66;
	if (m_UseBind == 1)
	{
		sprintf(str, "%s 绑定金币", str);

		if (SELF.GetBindGold() < m_iNeedMoney)
			dwMoneyColor = 0xffff0000;
	}
	else
	{
		sprintf(str, "%s 金币", str);
		if (SELF.GetGold() < m_iNeedMoney)
			dwMoneyColor = 0xffff0000;
	}
	
	g_pFont->DrawText(m_iScreenX + 118,m_iScreenY + 336,str,dwMoneyColor,FONT_YAHEI,12,DTF_BlackFrame);

	if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() && m_iGoodOnGrid >= ESG_Wake_Equip && m_iGoodOnGrid < ESG_Peel_JieJing)
	{
		if(!m_bInValidGoodOnGrid)
		{
			g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[m_iGoodOnGrid].x + 3,m_iScreenY + m_ptGridPos[m_iGoodOnGrid].y + 3,33,34,0x80ff0000);
		}
		else
		{
			g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[m_iGoodOnGrid].x + 3,m_iScreenY + m_ptGridPos[m_iGoodOnGrid].y + 3,33,34,0x8000ff00);
		}
	}


	pTex = NULL;
	bool bDrawStand = true;	//绘制待机的NPC

	if (m_iSuccessFrame < 90)
	{
		LPTexture pTextBackTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20951,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTextBackTex);

		if (m_dwType == EST_Wake)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 50,"引妖之气，聚器之魂，浩瀚无穷，为我所命。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 65,"妖气难寻，器魂难测，魂之觉醒，所向披靡！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 80,"恭喜您装备觉醒成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_dwType == EST_LevelUp)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您成功的增加了装备的器魂经验值！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if (m_dwType == EST_Peel)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"器魂结晶从装备中成功剥离了出来。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}

		if(m_dwType != EST_Peel && m_iSuccessFrame < 15 * 4)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20916,EP_UI);

			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(m_iSuccessFrame / 4);
				g_pGfx->SetRenderMode(RM_ADD2);		
				g_pGfx->DrawTextureNL(m_iScreenX + 121,m_iScreenY + 80,pTex);
				g_pGfx->SetRenderMode();
			}
		}

		if(m_iSuccessFrame < 25 * 3)
		{
			bDrawStand = false;

			LPTexture pSuccessTexNpc = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21663,EP_UI);
			if(pSuccessTexNpc)
			{
				pSuccessTexNpc->EnableSysAnim(false);
				pSuccessTexNpc->SetCurFrame(m_iSuccessFrame / 3);
				g_pGfx->DrawTextureNL(m_iScreenX + 300,m_iScreenY + 80,pSuccessTexNpc);
			}

			LPTexture pSuccessTexEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21664,EP_UI);
			if(pSuccessTexEffect)
			{
				pSuccessTexEffect->EnableSysAnim(false);
				pSuccessTexEffect->SetCurFrame(m_iSuccessFrame / 3);
				g_pGfx->SetRenderMode(RM_ADD2);	
				g_pGfx->DrawTextureNL(m_iScreenX + 267,m_iScreenY + 45,pSuccessTexEffect);
				g_pGfx->SetRenderMode();
			}
		}

		++m_iSuccessFrame;

		if (m_iSuccessFrame == 90)
		{
			if (m_dwType == EST_Wake || (m_dwType == EST_Peel && g_NPC.GetEquipSoulInfo().m_Equip.GetSoulLevel() == 0 && g_NPC.GetEquipSoulInfo().m_Equip.GetSoleExp() == 1))
			{
				BackToPackage();
			}

			m_iOKClick = 0;
		}
	}	

	if(bDrawStand)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21666,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 300,m_iScreenY + 80,pTex);
	}
}

bool CEquipSoulWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */ )
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOkBtn)
			{				
				m_iOKClick = 2;
				m_pOkBtn->SetEnable(false);
				SendEquipSoulMsg();
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
	case MSG_CTRL_EQUIPSOUL_WND:
		{
			if (dwData == 100)
			{
				m_UseBind = 0;
			}
			else if(dwData == 101)
			{
				m_UseBind = 1;
			}
			else
			{
				WORD wResult = HIWORD(dwData);
				WORD wType = LOWORD(dwData);

				if (wResult == 0)	//成功
				{
					m_iSuccessFrame = 0;
				}
				else
				{
					m_iOKClick = 0;
					if (pControl)
					{
						g_MsgBoxMgr.PopSimpleAlert((const char *)pControl);
					}
				}
			}
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CEquipSoulWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();
	pTip->SetShow(false);
	pTip->GetSelfEquipTips()->SetShow(false);

	if (m_dwType == EST_Peel && IsInGrid(iX, iY, ESG_Peel_JieJing))
	{
		CGood &tmp = g_NPC.GetEquipSoulInfo().m_Equip;
		int iSoulLevel = tmp.GetSoulLevel();
		DWORD dwSoulExp = tmp.GetSoleExp();
		if (tmp.GetID() && (iSoulLevel == 0 && dwSoulExp == 1))
		{
			// 仅拥有1点原始器魂经验值，无法进行剥离。
		}		
		else if(tmp.GetID() && (iSoulLevel > 0 || dwSoulExp > 0))
		{
			// 计算剥离数量，70%
			int iMaxAddAtrrItems = 0;
			for (int i = iSoulLevel; i > 0; i--)
			{
				dwSoulExp += g_AIGoodMgr.GetSoulLevelUpExp(tmp,iMaxAddAtrrItems,i - 1);
			}

			dwSoulExp -= 1;
			dwSoulExp = dwSoulExp * 70 / 100;
			if (dwSoulExp == 0)
			{
				dwSoulExp = 1;
			}
			else if (dwSoulExp > 5000)//最多一次剥5000,否则金币不够用
			{
				dwSoulExp = 5000;
			}

			pTip->AddText("器魂结晶",TIPS_GOODNAME,-1,0,-1,FONT_DEFAULT,FONTSIZE_MIDDLE);

			char str[256] = "";
			sprintf(str, "数量：%d", dwSoulExp);
			pTip->AddText(str,TIPS_GREEN);

			//配置物品说明
			sTipsCfg * pTipCfg = g_TipsCfg.GetCfgTips("器魂结晶");
			if(pTipCfg)
			{
				pTip->AddCfgTips(pTipCfg);
			}

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}
	else
	{
		m_iGoodOnGrid = 0;
		CGood * pGood = GetGoodOnGrid(iX,iY,m_iGoodOnGrid);
		m_bInValidGoodOnGrid = true;

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(pGood && GoodFrom.DropGood.GetID())
		{
			m_bInValidGoodOnGrid = IsInValidGoodOnGrid(m_iGoodOnGrid,GoodFrom.DropGood);
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

		if (m_iGoodOnGrid > 0)
		{
			switch (m_iGoodOnGrid)
			{
			case ESG_Wake_Equip:
				pTip->AddText("请把您需要进行觉醒仪式的装备放入格子中，请确保它之前没有进行过觉醒仪式。");
				break;
			case ESG_Wake_Hunshi:
				pTip->AddText("请放入妖魂石，您可以从商城中买到它，部分BOSS也会掉落。");
				break;
			case ESG_LevelUp_Equip:
				pTip->AddText("请把您需要提升器魂等级的装备放入格子中，请确保它之前已经进行过觉醒仪式。");
				break;			
			case ESG_LevelUp_JieJing1:				
			case ESG_LevelUp_JieJing2:				
			case ESG_LevelUp_JieJing3:
			case ESG_LevelUp_JieJing4:
			case ESG_LevelUp_JieJing5:
				pTip->AddText("请放入同类型的装备结晶，您可以通过分解同类型装备获得同类型的装备结晶，群英宝鉴也会产出各类型的装备结晶。");
				break;
			case ESG_Peel_Equip:
				pTip->AddText("请放入需要剥离器魂晶石的装备，请确保它进行过觉醒仪式。");
				break;
			default:
				break;
			}			

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);

			return true;
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);

}

bool CEquipSoulWnd::OnLeftButtonUp( int iX, int iY )
{
	int grid = 0;
	CGood * pGood = GetGoodOnGrid(iX,iY,grid);

	if(pGood)
	{
		if(CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd && CGoodGrid::GetDropGoodFrom().DropGood.GetID())
		{
			g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() && !IsInValidGoodOnGrid(grid,GoodFrom.DropGood))
		{
			return true;
		}

		CGood temp = *pGood;
		*pGood = GoodFrom.DropGood;
		GoodFrom.DropGood = temp;
		m_iOKClick = 0;

		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}

CGood* CEquipSoulWnd::GetGoodOnGrid(int iX,int iY,int& grid)
{
	grid = 0;
	if (m_dwType == EST_Wake)
	{
		if (IsInGrid(iX, iY, ESG_Wake_Equip))
		{
			grid = ESG_Wake_Equip;
			return &(g_NPC.GetEquipSoulInfo().m_Equip);
		}
		else if (IsInGrid(iX, iY, ESG_Wake_Hunshi))
		{
			grid = ESG_Wake_Hunshi;
			return &(g_NPC.GetEquipSoulInfo().m_Hunshi);
		}
	}
	else if (m_dwType == EST_LevelUp)
	{
		if (IsInGrid(iX, iY, ESG_LevelUp_Equip))
		{
			grid = ESG_LevelUp_Equip;
			return &(g_NPC.GetEquipSoulInfo().m_Equip);
		}
		
		for(int i = 0;i < 5;i++)
		{
			if(IsInGrid(iX, iY, ESG_LevelUp_JieJing1 + i))
			{
				grid = ESG_LevelUp_JieJing1 + i;
				return &(g_NPC.GetEquipSoulInfo().m_Jiejing[i]);
			}
		}
	}
	else if (m_dwType == EST_Peel)
	{
		if (IsInGrid(iX, iY, ESG_Peel_Equip))
		{
			grid = ESG_Peel_Equip;
			return &(g_NPC.GetEquipSoulInfo().m_Equip);
		}
	}
	return NULL;
}

int CEquipSoulWnd::IsConditionSatisfied()
{
	m_iNeedMoney = 0;
	if (m_dwType == EST_Wake)
	{
		m_iNeedMoney = 10000;

		if (m_UseBind == 1)
		{
			if(SELF.GetBindGold() < m_iNeedMoney)
			{
				// 游戏币不够
				return 0;
			}
		}
		else
		{
			if(SELF.GetGold() < m_iNeedMoney)
			{
				// 游戏币不够
				return 0;
			}
		}

		CGood &lEquipGood = g_NPC.GetEquipSoulInfo().m_Equip;

		if(lEquipGood.GetID() == 0)
			return -3;

		// 已觉醒
		if (lEquipGood.HaveWakeUpSole())
			return -1;

		// 不能觉醒
		int iMaxAddAtrrItems;
		DWORD dwSoulLevelUpExp = g_AIGoodMgr.GetSoulLevelUpExp(lEquipGood,iMaxAddAtrrItems);
		if (iMaxAddAtrrItems == 0)
			return -2;

		if (g_NPC.GetEquipSoulInfo().m_Hunshi.GetID() == 0)
			return 0;

		return 1;
	}
	else if (m_dwType == EST_LevelUp)
	{
		if(g_NPC.GetEquipSoulInfo().m_Equip.GetID())
		{
			// 未觉醒 return -1;
			if (!g_NPC.GetEquipSoulInfo().m_Equip.HaveWakeUpSole())
				return -1;

			// 等级已到上限 return -2;
			int iMaxAddAtrrItems;
			DWORD dwSoulLevelUpExp = g_AIGoodMgr.GetSoulLevelUpExp(g_NPC.GetEquipSoulInfo().m_Equip,iMaxAddAtrrItems);
			if (dwSoulLevelUpExp == 0 || g_NPC.GetEquipSoulInfo().m_Equip.GetSoulLevel() + 1 > iMaxAddAtrrItems)
				return -2;
		}
		else
		{
			return -3;
		}

		// 判断是否放入器魂结晶,返回数量
		BYTE lValidJiejingShape = 0;
		CGood &tmp = g_NPC.GetEquipSoulInfo().m_Equip;

		if (g_AIGoodMgr.IsEquipHelmet(tmp)) // 头盔
			lValidJiejingShape = 1;
		else if (g_AIGoodMgr.IsEquipNecklace(tmp)) // 项链
			lValidJiejingShape = 2;
		else if (g_AIGoodMgr.IsEquipBangle(tmp)) // 手镯
			lValidJiejingShape = 3;
		else if (g_AIGoodMgr.IsEquipRing(tmp)) // 戒指
			lValidJiejingShape = 4;		
		else if (g_AIGoodMgr.IsEquipCloth(tmp) ) // 衣服
			lValidJiejingShape = 5;		
		else if (g_AIGoodMgr.IsEquipBelt(tmp)) // 腰带
			lValidJiejingShape = 6;
		else if (g_AIGoodMgr.IsEquipBoot(tmp)) // 靴子
			lValidJiejingShape = 7;
		else if (g_AIGoodMgr.IsEquipWeapon(tmp)) // 武器
			lValidJiejingShape = 8;		

		int iCount = 0;
		for(int i = 0;i < 5;++i)
		{
			if(g_NPC.GetEquipSoulInfo().m_Jiejing[i].GetID())
			{
				CGood &tmp = g_NPC.GetEquipSoulInfo().m_Jiejing[i];
				if (tmp.GetShape() == 0)//完美结晶,用ac表示个数
				{
					iCount += tmp.GetAC() * 100 * tmp.GetDura();
				}
				else if (tmp.GetShape() == lValidJiejingShape)
				{
					iCount += tmp.GetDura();
				}
				else
					return -4;	// 放入的晶石类型不符合
			}
		}	

		m_iNeedMoney = 1000 * iCount;
		if (m_UseBind == 1)
		{
			if(SELF.GetBindGold() < m_iNeedMoney)
			{
				// 游戏币不够
				return 0;
			}
		}
		else
		{
			if(SELF.GetGold() < m_iNeedMoney)
			{
				// 游戏币不够
				return 0;
			}
		}

		return iCount;
	}
	else if (m_dwType == EST_Peel)
	{
		CGood &tmp = g_NPC.GetEquipSoulInfo().m_Equip;
		int iSoulLevel = tmp.GetSoulLevel();
		DWORD dwSoulExp = tmp.GetSoleExp();
		if(tmp.GetID() == 0 || (iSoulLevel == 0 && dwSoulExp == 0))
		{
			return -1;
		}

		if (iSoulLevel == 0 && dwSoulExp == 1)
		{
			// 仅拥有1点原始器魂经验值，无法进行剥离。
			return -2;
		}

		int iMaxAddAtrrItems = 0;
		for (int i = iSoulLevel; i > 0; i--)
		{
			dwSoulExp += g_AIGoodMgr.GetSoulLevelUpExp(tmp,iMaxAddAtrrItems,i - 1);
		}

		dwSoulExp -= 1;
		dwSoulExp = dwSoulExp * 70 / 100;
		if (dwSoulExp == 0)
		{
			dwSoulExp = 1;
		}
		else if (dwSoulExp > 5000)//最多一次剥5000,否则金币不够用
		{
			dwSoulExp = 5000;
		}

		m_iNeedMoney = 1000 * dwSoulExp;
		if (m_UseBind == 1)
		{
			if(SELF.GetBindGold() < m_iNeedMoney)
			{
				// 游戏币不够
				return 0;
			}
		}
		else
		{
			if(SELF.GetGold() < m_iNeedMoney)
			{
				// 游戏币不够
				return 0;
			}
		}

		return dwSoulExp;
	}

	return 0;
}

bool CEquipSoulWnd::IsInValidGoodOnGrid(int grid,CGood& good)
{
	if (grid == ESG_Wake_Equip)
	{
		// 判断装备是否能够器魂觉醒
		if (!g_AIGoodMgr.IsEquip(good))
			return false;		
	}
	else if (grid == ESG_Wake_Hunshi)
	{
		// 判断物品是否是妖魂石
		if (strcmp(good.GetName(),"妖魂石") != 0)
			return false;
	}
	else if (grid == ESG_LevelUp_Equip)
	{
		if (!g_AIGoodMgr.IsEquip(good)/* || !good.HaveWakeUpSole()*/)
			return false;
	}
	else if(grid >= ESG_LevelUp_JieJing1 && grid <= ESG_LevelUp_JieJing5)
	{
		// 判断物品是否是器魂结晶
		if (!g_AIGoodMgr.IsEquipSoulCrystal(good))
		{
			return false;
		}

		CGood &equip = g_NPC.GetEquipSoulInfo().m_Equip;

		//所有结晶，stdmode = 93
		//	shape
		//	头盔 1
		//	项链 2
		//	手镯 3
		//	戒指 4
		//	衣服 5
		//	腰带 6
		//	靴子 7
		//	武器 8
		//	完美 0
		int iShape = good.GetShape();
		if(iShape == 1 && !g_AIGoodMgr.IsEquipHelmet(equip))
			return false;
		if(iShape == 2 && !g_AIGoodMgr.IsEquipNecklace(equip))
			return false;
		if(iShape == 3 && !g_AIGoodMgr.IsEquipBangle(equip))
			return false;
		if(iShape == 4 && !g_AIGoodMgr.IsEquipRing(equip))
			return false;
		if(iShape == 5 && !g_AIGoodMgr.IsEquipCloth(equip))
			return false;
		if(iShape == 6 && !g_AIGoodMgr.IsEquipBelt(equip))
			return false;
		if(iShape == 7 && !g_AIGoodMgr.IsEquipBoot(equip))
			return false;
		if(iShape == 8 && !g_AIGoodMgr.IsEquipWeapon(equip))
			return false;		

	}
	else if (grid == ESG_Peel_Equip)
	{
		// 判断装备是否能够剥离
		if (!g_AIGoodMgr.IsEquip(good))
			return false;
		if (!good.HaveWakeUpSole())
			return false;
	}
	
	return true;
}

bool CEquipSoulWnd::IsInGrid(int iX,int iY, int gridx, int gridy)
{
	if(iX > gridx && iX < gridx + 36 && iY > gridy && iY < gridy + 36)
		return true;

	return false;
}

bool CEquipSoulWnd::IsInGrid(int iX,int iY, int grid)
{
	if (grid >= ESG_Wake_Equip && grid < EquipSoulGridNum)
	{
		return IsInGrid(iX, iY, m_ptGridPos[grid].x, m_ptGridPos[grid].y);
	}
	return false;
}

void CEquipSoulWnd::BackToPackage()
{
	SELF.PackageGood().BackToArray(g_NPC.GetEquipSoulInfo().m_Equip);
	g_NPC.GetEquipSoulInfo().m_Equip.SetID(0);

	SELF.PackageGood().BackToArray(g_NPC.GetEquipSoulInfo().m_Hunshi);
	g_NPC.GetEquipSoulInfo().m_Hunshi.SetID(0);

	for(int i = 0;i < 5;i++)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetEquipSoulInfo().m_Jiejing[i]);
		g_NPC.GetEquipSoulInfo().m_Jiejing[i].SetID(0);
	}
}

void CEquipSoulWnd::SendEquipSoulMsg()
{
	if(m_iConditionSatisfy > 0)
	{
		if (m_dwType == EST_Wake)
		{
			g_pGameControl->Send_EquipSoul(1, g_NPC.GetEquipSoulInfo().m_Equip.GetID(), g_NPC.GetEquipSoulInfo().m_Hunshi.GetID());
		}
		else if (m_dwType == EST_LevelUp)
		{
			g_pGameControl->Send_EquipSoulLevelUp(m_UseBind, g_NPC.GetEquipSoulInfo().m_Equip.GetID(), g_NPC.GetEquipSoulInfo().m_Jiejing[0].GetID(), g_NPC.GetEquipSoulInfo().m_Jiejing[1].GetID(), 
				g_NPC.GetEquipSoulInfo().m_Jiejing[2].GetID(), g_NPC.GetEquipSoulInfo().m_Jiejing[3].GetID(), g_NPC.GetEquipSoulInfo().m_Jiejing[4].GetID());
		}
		else if (m_dwType == EST_Peel)
		{
			g_pGameControl->Send_EquipSoul(2, g_NPC.GetEquipSoulInfo().m_Equip.GetID());
		}
	}
}
