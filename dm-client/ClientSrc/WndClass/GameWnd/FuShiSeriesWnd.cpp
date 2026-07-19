#include "FuShiSeriesWnd.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/NpcData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/GlobalStrMgr.h"
#include "GameAI/AIGoodMgr.h"
#include "BaseClass/Control/ArrowTip.h"
#include "BaseClass/Audio/Audio.h"

#define DrillEquip 1
#define EmbedFuShi 2
#define ChangeHole 3
#define UpgradeFuShi 4
#define CombineFuShi 5
#define RemoveFuShi 6
#define UpgradeEquip 7
#define DegradeFuShi 8

//------------------------------------------//

#define EquipmentToDrill_PosX 121
#define EquipmentToDrill_PosY 80
#define Driller_PosX 121
#define Driller_PosY 280

#define EmbedEquipment_PosX 121
#define EmbedEquipment_PosY 80
#define EmbedFushi_PosX 121
#define EmbedFushi_PosY 165
#define XiangQianFu_PosX 121
#define XiangQianFu_PosY 249

#define EquipmentToConvert_PosX 121
#define EquipmentToConvert_PosY 124
#define QianKunFu_PosX 121
#define QianKunFu_PosY 290

#define FuShiToUpgrade_PosX 121
#define FuShiToUpgrade_PosY 80
#define DiaoZhuoFu_PosX 121
#define DiaoZhuoFu_PosY 280

//#define FuShiToCombine_PosX 121
//#define FuShiToCombine_PosY 80
#define HeChengFu_PosX 121
#define HeChengFu_PosY 249

#define RemoveFuShiEquip_PosX 121
#define RemoveFuShiEquip_PosY 80
#define ZhaiChuFu_PosX 121
#define ZhaiChuFu_PosY 280

#define EquipToUpgrade_PosX 121
#define EquipToUpgrade_PosY 80
#define QiangHuaFu_PosX 121
#define QiangHuaFu_PosY 165
#define XingYunFu_PosX 121
#define XingYunFu_PosY 249

#define FuShiToDegrade_PosX 121
#define FuShiToDegrade_PosY 80
#define HuanYuanFu_PosX 121
#define HuanYuanFu_PosY 280

//--------------------------------------------------//

#define FrameSpeed 9
#define TotalFrame (FrameSpeed * 15)

INIT_WND_POSX(CFuShiSeriesWnd,POS_AUTO,POS_AUTO)

CFuShiSeriesWnd::CFuShiSeriesWnd(void)
{
	m_iType = sm_dwWindowType;

	switch(m_iType)
	{
	case DrillEquip:
	case UpgradeFuShi:	
	case DegradeFuShi:
		m_iIndex = 20910;
		break;
	case EmbedFuShi:
	case UpgradeEquip:	
		m_iIndex = 20911;
		break;
	case ChangeHole:
		m_iIndex = 20912;
		break;		
	case CombineFuShi:
		m_iIndex = 20913;
		break;
	case RemoveFuShi:
		m_iIndex = 20914;
		break;
	}
	

	m_bConditionSatisfy = false;

	m_iNeedMoney = 100000;	

	m_ptHoleToConvert[0].x = 128;
	m_ptHoleToConvert[0].y = 76;

	m_ptHoleToConvert[1].x = 160;
	m_ptHoleToConvert[1].y = 85;

	m_ptHoleToConvert[2].x = 180;
	m_ptHoleToConvert[2].y = 114;

	m_ptHoleToConvert[3].x = 182;
	m_ptHoleToConvert[3].y = 147;

	m_ptHoleToConvert[4].x = 160;
	m_ptHoleToConvert[4].y = 176;

	m_ptHoleToConvert[5].x = 128;
	m_ptHoleToConvert[5].y = 187;

	m_ptHoleToConvert[6].x = 94;
 	m_ptHoleToConvert[6].y = 176;

	m_ptHoleToConvert[7].x = 76;
	m_ptHoleToConvert[7].y = 147;

	m_ptHoleToConvert[8].x = 77;
	m_ptHoleToConvert[8].y = 114;

	m_ptHoleToConvert[9].x = 96;
	m_ptHoleToConvert[9].y = 84;


	m_ptGridPos[EquipToDrill].x = EquipmentToDrill_PosX;
	m_ptGridPos[EquipToDrill].y = EquipmentToDrill_PosY;

	m_ptGridPos[EquipToEmbed].x = EmbedEquipment_PosX;
	m_ptGridPos[EquipToEmbed].y = EmbedEquipment_PosY;

	m_ptGridPos[EquipToChangeHole].x = EquipmentToConvert_PosX;
	m_ptGridPos[EquipToChangeHole].y = EquipmentToConvert_PosY;

	m_ptGridPos[EquipToRemoveFuShi].x = RemoveFuShiEquip_PosX;
	m_ptGridPos[EquipToRemoveFuShi].y = RemoveFuShiEquip_PosY;

	m_ptGridPos[FuShiToEmbed].x = EmbedFushi_PosX;
	m_ptGridPos[FuShiToEmbed].y = EmbedFushi_PosY;

	m_ptGridPos[FuShiToUpgrade].x = FuShiToUpgrade_PosX;
	m_ptGridPos[FuShiToUpgrade].y = FuShiToUpgrade_PosY;

	m_ptGridPos[FuShiToCombine1].x = 121;
	m_ptGridPos[FuShiToCombine1].y = 80;

	m_ptGridPos[FuShiToCombine2].x = 76;
	m_ptGridPos[FuShiToCombine2].y = 125;

	m_ptGridPos[FuShiToCombine3].x = 121;
	m_ptGridPos[FuShiToCombine3].y = 125;

	m_ptGridPos[FuShiToCombine4].x = 166;
	m_ptGridPos[FuShiToCombine4].y = 125;

	m_ptGridPos[FuShiToCombine5].x = 121;
	m_ptGridPos[FuShiToCombine5].y = 170;

	m_ptGridPos[Drill].x = Driller_PosX;
	m_ptGridPos[Drill].y = Driller_PosY;

	m_ptGridPos[XianQianFu].x = XiangQianFu_PosX;
	m_ptGridPos[XianQianFu].y = XiangQianFu_PosY;

	m_ptGridPos[QianKunFu].x = QianKunFu_PosX;
	m_ptGridPos[QianKunFu].y = QianKunFu_PosY;

	m_ptGridPos[DiaoZhuoFu].x = DiaoZhuoFu_PosX;
	m_ptGridPos[DiaoZhuoFu].y = DiaoZhuoFu_PosY;

	m_ptGridPos[HeChengFu].x = HeChengFu_PosX;
	m_ptGridPos[HeChengFu].y = HeChengFu_PosY;

	m_ptGridPos[ZhaiChuFu].x = ZhaiChuFu_PosX;
	m_ptGridPos[ZhaiChuFu].y = ZhaiChuFu_PosY;

	m_ptGridPos[EquipToUpgrade].x = EquipToUpgrade_PosX;
	m_ptGridPos[EquipToUpgrade].y = EquipToUpgrade_PosY;

	m_ptGridPos[QiangHuaFu].x = QiangHuaFu_PosX;
	m_ptGridPos[QiangHuaFu].y = QiangHuaFu_PosY;

	m_ptGridPos[XingYunFu].x = XingYunFu_PosX;
	m_ptGridPos[XingYunFu].y = XingYunFu_PosY;

	m_ptGridPos[FuShiToDegrade].x = FuShiToDegrade_PosX;
	m_ptGridPos[FuShiToDegrade].y = FuShiToDegrade_PosY;

	m_ptGridPos[HuanYuanFu].x = HuanYuanFu_PosX;
	m_ptGridPos[HuanYuanFu].y = HuanYuanFu_PosY;

	m_iSuccessFrame = 10000;
	m_iFailFrame = 10000;
	m_iFrame = 10000;

	m_bFirstClick = false;
	m_bWithHuBaoFu = false;
}

CFuShiSeriesWnd::~CFuShiSeriesWnd(void)
{
	SELF.PackageGood().BackToArray(g_NPC.GetDrillResult().m_equip);
	g_NPC.GetDrillResult().m_equip.SetID(0);

	SELF.PackageGood().BackToArray(g_NPC.GetDrillResult().m_Fushi);
	g_NPC.GetDrillResult().m_Fushi.SetID(0);

	SELF.PackageGood().BackToArray(g_NPC.GetDrillResult().m_Luck);
	g_NPC.GetDrillResult().m_Luck.SetID(0);

	for(int i = 0;i < 5;i++)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetDrillResult().m_FuShiToCombine[i]);
		g_NPC.GetDrillResult().m_FuShiToCombine[i].SetID(0);
	}	
}

void CFuShiSeriesWnd::Draw()
{
	m_bConditionSatisfy = IsConditionSatisfied();

	if(m_bConditionSatisfy)
	{
		m_pOkBtn->SetShow(true);
		m_pOkBtn->SetEnable(true);

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

	char str[256] = "";

	LPTexture pTex = NULL;

	if(m_iType == DrillEquip)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"装备打孔",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 43,m_iScreenY + 55,g_StrMgr.GetGlobalStr(FuShiWnd_Put_Equipment),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + EquipmentToDrill_PosX,m_iScreenY + EquipmentToDrill_PosY,pTex);

		if(g_NPC.GetDrillResult().m_equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + EquipmentToDrill_PosX + 20,m_iScreenY + EquipmentToDrill_PosY + 20,g_NPC.GetDrillResult().m_equip);
		}

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17503);
		//g_pGfx->DrawTextureNL(m_iScreenX + 32,m_iScreenY + 104,pTex);

		if(g_NPC.GetDrillResult().m_equip.GetID())
		{
			int iCanDigHole = g_AIGoodMgr.GetCanDigHoles(g_NPC.GetDrillResult().m_equip);
			sprintf(str,"装备可打孔数量: %d",iCanDigHole);
			g_pFont->DrawText(m_iScreenX + 89,m_iScreenY + 148,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

			int iDigHole = g_AIGoodMgr.GetDigHole(g_NPC.GetDrillResult().m_equip);
			sprintf(str,"已打孔数量: %d",iDigHole);
			g_pFont->DrawText(m_iScreenX + 89,m_iScreenY + 168,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

			sprintf(str,"可继续打孔数量: %d",iCanDigHole - iDigHole);
			g_pFont->DrawText(m_iScreenX + 89,m_iScreenY + 188,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
		}

		g_pFont->DrawText(m_iScreenX + 104,m_iScreenY + 258,g_StrMgr.GetGlobalStr(FuShiWnd_Put_Driller),COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + Driller_PosX,m_iScreenY + Driller_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + Driller_PosX + 20,m_iScreenY + Driller_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、装备可以打孔; 2、放入了打孔材料; 3、金币足够支持打孔所需费用。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 393,"满足条件后，界面会出现确认按钮,无论打孔成功还是失败,打孔材料都会被收取。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if(m_iType == EmbedFuShi)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"符石镶嵌",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 75,m_iScreenY + 60,"请放入您需要镶嵌的装备",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + EmbedEquipment_PosX,m_iScreenY + EmbedEquipment_PosY,pTex);

		if(g_NPC.GetDrillResult().m_equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + EmbedEquipment_PosX + 20,m_iScreenY + EmbedEquipment_PosY + 20,g_NPC.GetDrillResult().m_equip);
		}

		g_pFont->DrawText(m_iScreenX + 85,m_iScreenY + 135,"放入想要镶嵌的符石",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//g_pGfx->DrawTextureNL(m_iScreenX + EmbedFushi_PosX,m_iScreenY + EmbedFushi_PosY,pTex);		

		if(g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + EmbedFushi_PosX + 20,m_iScreenY + EmbedFushi_PosY + 20,g_NPC.GetDrillResult().m_Fushi);
		}

		g_pFont->DrawText(m_iScreenX + 98,m_iScreenY + 220,"放入符石镶嵌符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//g_pGfx->DrawTextureNL(m_iScreenX + XiangQianFu_PosX,m_iScreenY + XiangQianFu_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + XiangQianFu_PosX + 20,m_iScreenY + XiangQianFu_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}	

		float fSuccessPercent = 0.0f;

		if(g_NPC.GetDrillResult().m_equip.GetID() && g_NPC.GetDrillResult().m_Fushi.GetID() && g_NPC.GetDrillResult().m_Luck.GetID())
		{
			int wLevel = g_AIGoodMgr.GetLevel(g_NPC.GetDrillResult().m_Luck);

			if (wLevel == 1)
			{
				fSuccessPercent = 0.5f;
			}
			else if (wLevel == 2)
			{
				fSuccessPercent = 1.0f;
			}
			else if (wLevel >= 3 && wLevel <= 7)
			{
				fSuccessPercent = 1.0f;
			}			
		}

		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 313,"成功率：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17528);
		//g_pGfx->DrawTextureNL(m_iScreenX + 23,m_iScreenY +240,pTex);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17527,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX + 105,m_iScreenY + 316,pTex,0,0,(int)(122 * fSuccessPercent));

		int iPercent = (int)(fSuccessPercent * 100);
		sprintf(str,"%d%%",iPercent);
		g_pFont->DrawText(m_iScreenX + 155,m_iScreenY + 303,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、放入的装备已经打好孔洞; 2、放入的符石与装备上至少一个孔洞属性一致;",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51 - 6,m_iScreenY + 393," 3、放入的符石镶嵌符与符石等级一致; 4、金币足够支付镶嵌费用。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if(m_iType == ChangeHole)
	{
		g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 10,"更改打孔属性",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);

		CGood & equipment = g_NPC.GetDrillResult().m_equip;

		for(int i = 0;i < 10;i++)
		{
			int iHoleType = 0;
			if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
			{
				iHoleType = ((equipment.GetDemonDark1() >> (i * 3)) & 0x7);
			}
			else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
			{
				WORD wHole = (WORD(equipment.GetResvEx3(0))) | (WORD(equipment.GetResvEx3(1)) << 8);
				iHoleType = ((wHole >> ((i - 5)* 3)) & 0x7);
			}

			if(equipment.GetID() && iHoleType != 0 && equipment.GetResvEx(i) == 0)
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
			WORD wHoles = LOWORD(equipment.GetDemonDark1());
			int iHoleType = (wHoles >> (iHoleNum * 3) & 0x7);
			if(iHoleType >= 1 && iHoleType <= 5)
			{
				m_pHole[iHoleType - 1]->SetChecked(false);
				m_pHole[iHoleType - 1]->SetEnable(false);
			}
		}

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 32,m_iScreenY + 50,"放入您的装备并选择需要转换属性的孔洞",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + EquipmentToConvert_PosX,m_iScreenY + EquipmentToConvert_PosY,pTex);

		if(g_NPC.GetDrillResult().m_equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + EquipmentToConvert_PosX + 20,m_iScreenY + EquipmentToConvert_PosY + 20,g_NPC.GetDrillResult().m_equip);
		}

		g_pFont->DrawText(m_iScreenX + 92,m_iScreenY + 270,"放入转换乾坤道具",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + QianKunFu_PosX,m_iScreenY + QianKunFu_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + QianKunFu_PosX + 20,m_iScreenY + QianKunFu_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}		

		g_pFont->DrawText(m_iScreenX + 52,m_iScreenY + 220,"请选择您希望该孔洞转化为的属性",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		for(int i = 0;i < 10;i++)
		{			
			m_pHoleToConvert[i]->Draw();
		}

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、需要转化属性的孔洞内未镶嵌符石; 2、所选孔洞的属性不与希望转换的属性相同;",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51 - 6,m_iScreenY + 393," 3、金币足够支付转换孔洞属性的费用。转换孔洞属性100%成功。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);		
		}
	}
	else if(m_iType == UpgradeFuShi)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"符石雕琢",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 82,m_iScreenY + 55,"放入您需要雕琢的符石",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + FuShiToUpgrade_PosX,m_iScreenY + FuShiToUpgrade_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + FuShiToUpgrade_PosX + 20,m_iScreenY + FuShiToUpgrade_PosY + 20,g_NPC.GetDrillResult().m_Fushi);
		}

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17503);
		//g_pGfx->DrawTextureNL(m_iScreenX + 32,m_iScreenY + 104,pTex);

		if(g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			int iLevel = g_NPC.GetDrillResult().m_Fushi.GetAC2();			

			sprintf(str,"%s,%d级",g_NPC.GetDrillResult().m_Fushi.GetName(),iLevel);
			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 110 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

			//g_pFont->DrawText(m_iScreenX + 105,m_iScreenY + 124 + 38,"雕琢前属性：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

			char * p1[5] = {"金","木","土","水","火"};
			int iType = (g_NPC.GetDrillResult().m_Fushi.GetShape() - 1) / 6;
			int iKind = (g_NPC.GetDrillResult().m_Fushi.GetShape() - 1) % 6 + 1;

			if (iKind == 1)
			{
				sprintf(str,"雕琢前属性： %s攻击:0-%d",p1[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
			}
			else if(iKind == 2)
			{
				sprintf(str,"雕琢前属性： +%s防御: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));
			}
			else if (iKind == 3)
			{
				char *pLingFuAddAttr[5] = {"破防","反弹伤害","抵抗","吸血","暴击"};
				sprintf(str,"雕琢前属性： +%s: %d",pLingFuAddAttr[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
			}

			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 130 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
							

			iKind += 3;
			//g_pFont->DrawText(m_iScreenX + 105,m_iScreenY + 152 + 38,"雕琢后属性：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);
				
			if (iKind == 4)
			{
				int iAdd = g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel);
				sprintf(str,"雕琢后属性： %s攻击:%d-%d",p1[iType],iAdd,iAdd);
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
			}
			else if(iKind == 5)
			{
				sprintf(str,"雕琢后属性： +%s防御: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				sprintf(str,"雕琢后属性： +血上限: %d",g_AIGoodMgr.GetFuShiAddHP(iKind,iLevel));	
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 170 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
			}				
			else if (iKind == 6)
			{
				char *pLingFuAddAttr[5] = {"破防","反弹伤害","抵抗","吸血","暴击"};
				sprintf(str,"雕琢后属性： +%s: %d",pLingFuAddAttr[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				sprintf(str,"雕琢后属性： +%s攻击: %d",p1[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));	
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 170 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
			}			
		}

		g_pFont->DrawText(m_iScreenX + 98,m_iScreenY + 258,"放入符石雕琢符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + DiaoZhuoFu_PosX,m_iScreenY + DiaoZhuoFu_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + DiaoZhuoFu_PosX + 20,m_iScreenY + DiaoZhuoFu_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}		

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、放入的符石从未被雕琢; 2、放入的符石雕琢符与当前符石等级一致;",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51 - 6,m_iScreenY + 393," 3、金币足够支付当前雕琢费用。符石雕琢成功率为100%。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);	
		}
	}
	else if(m_iType == CombineFuShi)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"符石合成",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 50,g_StrMgr.GetGlobalStr(FuShiWnd_PutFuShi_To_Combine),COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);

		for(int i = 0;i < 5;i++)
		{
			//g_pGfx->DrawTextureNL(m_iScreenX + m_ptGridPos[FuShiToCombine1 + i].x,m_iScreenY + m_ptGridPos[FuShiToCombine1 + i].y,pTex);

			if(g_NPC.GetDrillResult().m_FuShiToCombine[i].GetID())
			{
				CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[FuShiToCombine1 + i].x + 20,m_iScreenY + m_ptGridPos[FuShiToCombine1 + i].y + 20,g_NPC.GetDrillResult().m_FuShiToCombine[i]);
			}
		}

		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 225,g_StrMgr.GetGlobalStr(FuShiWnd_PutHeChengFu),COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//g_pGfx->DrawTextureNL(m_iScreenX + HeChengFu_PosX,m_iScreenY + HeChengFu_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + HeChengFu_PosX + 20,m_iScreenY + HeChengFu_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}

		float fSuccessPercent = 0.0f;

		int iCount = 0;

		for(int i = 0;i < 5;i++)
		{
			if(g_NPC.GetDrillResult().m_FuShiToCombine[i].GetID())
			{
				iCount++;
			}
		}

		if(iCount >= 3)
		{
			fSuccessPercent = (iCount - 2) * 0.25f;
		}

		if(g_NPC.GetDrillResult().m_Luck.GetID() && iCount >= 3)
		{
			fSuccessPercent += 0.25f;
		}

		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 313,"成功率：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17528);
		//g_pGfx->DrawTextureNL(m_iScreenX + 23,m_iScreenY +240,pTex);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17527,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX + 105,m_iScreenY + 316,pTex,0,0,(int)(122 * fSuccessPercent));

		int iPercent = (int)(fSuccessPercent * 100);
		sprintf(str,"%d%%",iPercent);
		g_pFont->DrawText(m_iScreenX + 155,m_iScreenY + 303,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、放入的符石等级相同且数目>=3个; 2、放入的符石合成符与当前符石等级一致;",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51 - 6,m_iScreenY + 393," 3、金币足够支付本次合成费用。无论合成成功与否，符石、符石合成符都会被收取。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if(m_iType == RemoveFuShi)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"符石摘除",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);

		CGood & equipment = g_NPC.GetDrillResult().m_equip;
		

		//WORD wHoles = LOWORD(equipment.GetDemonDark1());
		BYTE byHoleType = 0;
		int iKind = 0;

		for(int i = 0;i < 10;i++)
		{			
		
			//if((wHoles & 0x7) >= 1 && (wHoles & 0x7) <= 5)
			//{
			//	byHoleType = wHoles & 0x7;
			//}

			//wHoles >>= 3;

			//int iKind = (equipment.GetResvEx(i) & 0xf0) >> 4;

			if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
			{
				byHoleType = ((equipment.GetDemonDark1() >> (i * 3)) & 0x7);
			}
			else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
			{
				WORD wHole = (WORD(equipment.GetResvEx3(0))) | (WORD(equipment.GetResvEx3(1)) << 8);
				byHoleType = ((wHole >> ((i - 5) * 3)) & 0x7);
			}

			if (i < 5)//前5个孔放在GetResvEx(i)
			{
				iKind = (equipment.GetResvEx(i) & 0xf0) >> 4;
			}
			else//后5个孔放在 GetResvEx3(2)-GetResvEx3(6)
			{							
				iKind = (equipment.GetResvEx3(i - 5 + 2) & 0xf0) >> 4;
			}

					

			if(equipment.GetID() && /*equipment.GetResvEx(i) != 0*/ iKind > 0)
			{
				m_pHole[i]->SetEnable(true);
				
				LPTexture pTex = NULL;
				
				if (iKind > 3 && iKind < 7)//纯净的
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_items,1455 + byHoleType - 1 + (iKind - 4 ) * 5,EP_UI);					
				}
				else
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_items,1413 + byHoleType - 1 + (iKind - 1 ) * 5,EP_UI);	
				}

				g_pGfx->DrawTextureNL(m_pHole[i]->GetScreenX() + 4,m_pHole[i]->GetScreenY() + (m_pHole[i]->IsChecked()? 3 : 2),pTex);
			}
			else
			{
				m_pHole[i]->SetChecked(false);
				m_pHole[i]->SetEnable(false);
			}
		}

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 67,m_iScreenY + 60,"请放入您需要摘除符石的装备",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + RemoveFuShiEquip_PosX,m_iScreenY + RemoveFuShiEquip_PosY,pTex);

		if(g_NPC.GetDrillResult().m_equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + RemoveFuShiEquip_PosX + 20,m_iScreenY + RemoveFuShiEquip_PosY + 20,g_NPC.GetDrillResult().m_equip);
		}

		g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 256,"放入符石摘除符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + ZhaiChuFu_PosX,m_iScreenY + ZhaiChuFu_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + ZhaiChuFu_PosX + 20,m_iScreenY + ZhaiChuFu_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}

		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 130,"选择您需要摘除的符石",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、放入的装备有符石可以摘除; 2、放入的符石摘除符与装备上符石等级一致;",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51 - 6,m_iScreenY + 393," 3、金币足够支付摘除费用。符石摘除后为绑定状态。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if(m_iType == UpgradeEquip)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"装备强化",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 60,"请放入您需要强化的装备",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + EquipToUpgrade_PosX,m_iScreenY + EquipToUpgrade_PosY,pTex);

		if(g_NPC.GetDrillResult().m_equip.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + EquipToUpgrade_PosX + 20,m_iScreenY + EquipToUpgrade_PosY + 20,g_NPC.GetDrillResult().m_equip);
		}

		g_pFont->DrawText(m_iScreenX + 110,m_iScreenY + 137,"放入强化符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//g_pGfx->DrawTextureNL(m_iScreenX + QiangHuaFu_PosX,m_iScreenY + QiangHuaFu_PosY,pTex);		

		if(g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + QiangHuaFu_PosX + 20,m_iScreenY + QiangHuaFu_PosY + 20,g_NPC.GetDrillResult().m_Fushi);
		}

		g_pFont->DrawText(m_iScreenX + 85,m_iScreenY + 220,"放入护宝符或幸运符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//g_pGfx->DrawTextureNL(m_iScreenX + XingYunFu_PosX,m_iScreenY + XingYunFu_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + XingYunFu_PosX + 20,m_iScreenY + XingYunFu_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}	

		float fSuccessPercent = 0.0f;

		float fPercent[8] = {0.6f,0.5000009f,0.4f,0.3f,0.2f,0.1f,0.05f,0.010000001f};

		if(g_NPC.GetDrillResult().m_equip.GetID() && g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			int iLevelUpTimes = g_NPC.GetDrillResult().m_equip.GetLevelUpTimes();

			if(iLevelUpTimes >= 0 && iLevelUpTimes < 8)
			{
				fSuccessPercent = fPercent[iLevelUpTimes];
			}

			if(g_NPC.GetDrillResult().m_Luck.GetID())
			{
				if (g_NPC.GetDrillResult().m_Luck.GetShape() == 3)	//极品幸运符
				{
					fSuccessPercent += 0.05f;
				}
				else if(g_NPC.GetDrillResult().m_Luck.GetShape() == 4)	//幸运符
				{
					fSuccessPercent += 0.02f;
				}
			}
		}

		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 313,"成功率：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17528);
		//g_pGfx->DrawTextureNL(m_iScreenX + 23,m_iScreenY +240,pTex);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17527,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX + 105,m_iScreenY + 316,pTex,0,0,(int)(122 * fSuccessPercent));

		int iPercent = (int)(fSuccessPercent * 100);
		sprintf(str,"%d%%",iPercent);
		g_pFont->DrawText(m_iScreenX + 155,m_iScreenY + 303,str,COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保: 1、放入的装备不为武器、勋章、宝石; 2、强化符的品级与装备品级相符;",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			////////////////////
			g_pFont->DrawText(m_iScreenX + 51 - 6,m_iScreenY + 393," 3、您的金币足够支付本次强化费用。注：强化失败装备会破碎，符石完好但变为绑定状态。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
		}
	}
	else if(m_iType == DegradeFuShi)
	{
		g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"符石还原",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17500,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		g_pFont->DrawText(m_iScreenX + 82,m_iScreenY + 55,"放入您需要还原的符石",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17509);
		//g_pGfx->DrawTextureNL(m_iScreenX + FuShiToUpgrade_PosX,m_iScreenY + FuShiToUpgrade_PosY,pTex);

		if(g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + FuShiToDegrade_PosX + 20,m_iScreenY + FuShiToDegrade_PosY + 20,g_NPC.GetDrillResult().m_Fushi);
		}

		//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17503);
		//g_pGfx->DrawTextureNL(m_iScreenX + 32,m_iScreenY + 104,pTex);

		if(g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			int iLevel = g_NPC.GetDrillResult().m_Fushi.GetAC2();			

			sprintf(str,"%s,%d级",g_NPC.GetDrillResult().m_Fushi.GetName(),iLevel);
			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 110 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

			//g_pFont->DrawText(m_iScreenX + 105,m_iScreenY + 124 + 38,"雕琢前属性：",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

			char * p1[5] = {"金","木","土","水","火"};
			int iType = (g_NPC.GetDrillResult().m_Fushi.GetShape() - 1) / 6;
			int iKind = (g_NPC.GetDrillResult().m_Fushi.GetShape() - 1) % 6 + 1;

			int iStartY = 150;

			if (iKind == 4)
			{
				sprintf(str,"还原前属性： +%s攻击: %d-%d",p1[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel),g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 130 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);
			}
			else if(iKind == 5)
			{
				sprintf(str,"还原前属性： +%s防御: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 130 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				sprintf(str,"还原前属性： +血上限: %d",g_AIGoodMgr.GetFuShiAddHP(iKind,iLevel));	
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				iStartY = 170;
			}	
			else if(iKind == 6)
			{
				char *pLingFuAddAttr[5] = {"破防","反弹伤害","抵抗","吸血","暴击"};

				sprintf(str,"还原前属性： +%s: %d",pLingFuAddAttr[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 130 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				sprintf(str,"还原前属性： +%s攻击: %d",p1[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 150 + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

				iStartY = 170;
			}	
			
			iKind -= 3;	

			if (iKind == 1)
			{
				sprintf(str,"还原后属性： +%s攻击: 0-%d",p1[iType],g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel));
			}
			else if(iKind == 2)
			{
				sprintf(str,"还原后属性： +%s防御: %d%%",p1[iType],g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel));
			}
			else if(iKind == 3)
			{
				char *pLingFuAddAttr[5] = {"破防","反弹伤害","抵抗","吸血","暴击"};
				sprintf(str,"还原后属性： +%s: %d",pLingFuAddAttr[iType],iLevel);
			}

			g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + iStartY + 38,str,0xff5aa0a0,FONT_YAHEI,12,DTF_BlackFrame);

		}

		g_pFont->DrawText(m_iScreenX + 98,m_iScreenY + 258,"放入符石还原符",COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

		if(g_NPC.GetDrillResult().m_Luck.GetID())
		{
			CGoodGrid::DrawOneGood(m_iScreenX + HuanYuanFu_PosX + 20,m_iScreenY + HuanYuanFu_PosY + 20,g_NPC.GetDrillResult().m_Luck);
		}		

		if(!m_bConditionSatisfy)
		{
			g_pFont->DrawText(m_iScreenX + 51,m_iScreenY + 373,"请确保：1、放入的符石为纯净符石；2、放入的符石还原符与当前纯净符石等级一致；",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 51 - 6,m_iScreenY + 393," 3、金币足够支持当前还原费用。符石还原成功率为100%，还原后的符石与您绑定。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);	
		}
	}

	g_pFont->DrawText(m_iScreenX + 49,m_iScreenY + 336,g_StrMgr.GetGlobalStr(FuShiWnd_Show_Expense),COLOR_TEXT_NORMAL,FONT_YAHEI,12,DTF_BlackFrame);

	//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17502);
	//g_pGfx->DrawTextureNL(m_iScreenX + 105,m_iScreenY + 278,pTex);

	//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17501);
	//g_pGfx->DrawTextureNL(m_iScreenX + 187,m_iScreenY + 275,pTex);

	ToCommaStr(str,m_iNeedMoney);

	if(SELF.GetGold() < m_iNeedMoney)
	{
		g_pFont->DrawText(m_iScreenX + 139,m_iScreenY + 336,str,0xffff0000,FONT_YAHEI,12,DTF_BlackFrame | DTF_Center);
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 139,m_iScreenY + 336,str,0xfffdca66,FONT_YAHEI,12,DTF_BlackFrame | DTF_Center);
	}

	if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() && m_iGoodOnGrid >= EquipToDrill && m_iGoodOnGrid < GridNum)
	{
		if(m_bInValidGoodOnGrid)
		{
			g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[m_iGoodOnGrid].x + 3,m_iScreenY + m_ptGridPos[m_iGoodOnGrid].y + 3,33,34,0x80ff0000);
		}
		else
		{
			g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[m_iGoodOnGrid].x + 3,m_iScreenY + m_ptGridPos[m_iGoodOnGrid].y + 3,33,34,0x8000ff00);
		}
	}

	ShowEffect();
}

void CFuShiSeriesWnd::OnCreate()
{
	m_pOkBtn = new CCtrlButton;
	AddControl(m_pOkBtn);
	m_pOkBtn->CreateEx(this,150,378,165,166,167);	
	m_pOkBtn->SetText("确 认",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
	//m_pOkBtn->SetTextOff(8,7);
	m_pOkBtn->SetShow(false);
	m_pOkBtn->SetEnable(false);

	m_pCancelBtn = new CCtrlButton;
	AddControl(m_pCancelBtn);
	m_pCancelBtn->CreateEx(this,380,378,165,166,167);	
	m_pCancelBtn->SetText("取 消",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
	//m_pCancelBtn->SetTextOff(8,7);
	m_pCancelBtn->SetShow(false);
	m_pCancelBtn->SetEnable(false);

	SetCloseButton(584,4,80);

	if(m_iType == ChangeHole)
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
	else if(m_iType == RemoveFuShi)
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

bool CFuShiSeriesWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */ )
{
	switch ( dwMsg )
	{
	case MSG_INPUT_MOVE:
		if(m_iType == RemoveFuShi)
		{
			
			CGood & equipment = g_NPC.GetDrillResult().m_equip;

			if(equipment.GetID())
			{
				WORD w2 = HIWORD( dwData );
				WORD w1 = LOWORD( dwData );

				CParserTip *pTip = g_pControl->GetTipWnd();				

				WORD wHoles = LOWORD(equipment.GetDemonDark1());

				char * p1[5] = {"金","木","土","水","火"};

				char str[256] = "";

				for(int i = 0;i < 10;i++)
				{
					if(m_pHole[i]->IsEnable() && w1 > m_pHole[i]->GetScreenX() && w2 > m_pHole[i]->GetScreenY() && w1 < m_pHole[i]->GetScreenX() + m_pHole[i]->GetWidth() && w2 < m_pHole[i]->GetScreenY() + m_pHole[i]->GetHeight())
					{
						pTip->Clear();					
						
						BYTE byHoleType = 0;								
						int iKind = 0,iLevel = 0;

						//if((wHoles & 0x7) >= 1 && (wHoles & 0x7) <= 5)
						//{
						//	byHoleType = wHoles & 0x7;
						//}		
						if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
						{
							byHoleType = ((equipment.GetDemonDark1() >> (i * 3)) & 0x7);
							iKind = (equipment.GetResvEx(i) & 0xf0) >> 4;//前5个孔放在GetResvEx(i)
							iLevel = equipment.GetResvEx(i) & 0x0f;			
						}
						else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
						{
							WORD wHole = (WORD(equipment.GetResvEx3(0))) | (WORD(equipment.GetResvEx3(1)) << 8);
							byHoleType = ((wHole >> ((i - 5) * 3)) & 0x7);
							iKind = (equipment.GetResvEx3(i - 5 + 2) & 0xf0) >> 4;//后5个孔放在 GetResvEx3(2)-GetResvEx3(6)
							iLevel = equipment.GetResvEx3(i - 5 + 2) & 0x0f;			
						}



						if(/*equipment.GetResvEx(i)*/iKind != 0)
						{								

							if(iKind > 3 && iKind < 7)
							{
								if (iKind == 4)
								{
									sprintf(str,"%d级纯净%s尖符石",iLevel,p1[byHoleType - 1]);
								}
								else if(iKind == 5)
								{
									sprintf(str,"%d级纯净%s刚符石",iLevel,p1[byHoleType - 1]);																									
								}
								else if(iKind == 6)
								{
									sprintf(str,"%d级纯净%s灵符石",iLevel,p1[byHoleType - 1]);
								}

								pTip->AddText(str,-1,-1);
							}
							else if(iKind > 0 && iKind < 4)
							{
								if (iKind == 1)
								{
									sprintf(str,"%d级%s尖符石",iLevel,p1[byHoleType - 1]);
								}
								else if(iKind == 2)
								{
									sprintf(str,"%d级%s刚符石",iLevel,p1[byHoleType - 1]);
								}
								else if (iKind == 3)
								{
									sprintf(str,"%d级%s灵符石",iLevel,p1[byHoleType - 1]);
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
				if(m_bFirstClick)
				{
					m_bFirstClick = false;
					m_iFrame = 90;					
				}
				else
				{
					m_iFrame = 0;
					m_bFirstClick = true;
				}
				
				return true;
			}
			else if(pControl == m_pCancelBtn)
			{
				OnClickCloseButton();
				return true;
			}
		}
		break;		
	case MSG_FUSHI_WND:
		{
			if(dwData == OPER_CUSTOM)
			{
				m_iSuccessFrame = 0;
				m_iFailFrame = 10000;
				m_bWithHuBaoFu = false;
				g_pAudio->Play(EAT_OTHER,918,g_pAudio->GetRand()++);
			}
			else if(dwData == OPER_CUSTOM + 1)
			{
				m_iFailFrame = 0;
				m_iSuccessFrame = 10000;
				m_bWithHuBaoFu = false;
				g_pAudio->Play(EAT_OTHER,919,g_pAudio->GetRand()++);
			}
			else if(dwData == OPER_CUSTOM + 10)	//带有护宝符成功
			{
				m_iSuccessFrame = 0;
				m_iFailFrame = 10000;
				m_bWithHuBaoFu = true;
				g_pAudio->Play(EAT_OTHER,918,g_pAudio->GetRand()++);
			}
			else if(dwData == OPER_CUSTOM + 11)	//带有护宝符失败
			{
				m_iFailFrame = 0;
				m_iSuccessFrame = 10000;
				m_bWithHuBaoFu = true;
				g_pAudio->Play(EAT_OTHER,919,g_pAudio->GetRand()++);
			}

			return true;
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CFuShiSeriesWnd::OnLeftButtonUp( int iX, int iY )
{
	int grid = 0;
	CGood * pGood = GetGridGood(iX,iY,grid);

	if(pGood)
	{
		if(CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd && CGoodGrid::GetDropGoodFrom().DropGood.GetID())
		{
			g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() && IsInValidGoodOnGrid(grid,GoodFrom.DropGood))
		{
			return true;
		}		

		g_NPC.GetDrillResult().iResult = -1;

		CGood temp = *pGood;

		*pGood = GoodFrom.DropGood;		

		GoodFrom.DropGood = temp;

		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}
bool CFuShiSeriesWnd::OnMouseMove(int iX,int iY)
{
	m_iGoodOnGrid = 0;
	CGood * pGood = GetGridGood(iX,iY,m_iGoodOnGrid);
	m_bInValidGoodOnGrid = false;

	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();

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

	char str[256];

	if(m_iGoodOnGrid > 0)
	{
		if(m_iGoodOnGrid == EquipToDrill)
		{
			pTip->AddText("请把您需要打孔的装备放入格子中，请确保它还有可以打孔的位置。");
		}
		else if(m_iGoodOnGrid == EquipToEmbed)
		{
			pTip->AddText("请放入需要镶嵌符石的装备，它必须是已经打过孔的。");
		}
		else if(m_iGoodOnGrid == EquipToChangeHole)
		{
			pTip->AddText("请把您需要更改孔洞属性的装备放入格子中，请确保它已经打过孔。");
		}
		else if(m_iGoodOnGrid == EquipToRemoveFuShi)
		{
			pTip->AddText("请放入您需要摘除符石的装备，它必须是已经镶嵌了符石的。");
		}
		else if(m_iGoodOnGrid == FuShiToEmbed)
		{
			pTip->AddText("请放入需要镶嵌在装备上的符石或纯净的符石，请保证装备上有满足它镶嵌属性的孔洞。");
		}
		else if(m_iGoodOnGrid == FuShiToUpgrade)
		{
			pTip->AddText("请放入你希望被雕琢的符石，它必须是从未被雕琢过的。放入的符石与雕琢好的纯净符石等级一致。");
		}
		else if(m_iGoodOnGrid >= FuShiToCombine1 && m_iGoodOnGrid <= FuShiToCombine5)
		{
			pTip->AddText("请放入等级相同的符石，最少3颗。经过雕琢的符石不能合成。");
		}		
		else if(m_iGoodOnGrid == Drill)
		{
			pTip->AddText("请放入打孔材料。打孔材料的属性决定了打出来的孔洞的属性。五行箭打出来的孔洞属性随机。您可以从商城中买到它们，部分活动BOSS也会掉落。");
		}
		else if(m_iGoodOnGrid == XianQianFu)
		{
			pTip->AddText("请放入符石镶嵌符。放入低级符石镶嵌符，镶嵌成功率为50%；放入高级符石镶嵌符，镶嵌成功率为100%。您可以从商城中买到它们，部分活动BOSS也会掉落。");
		}
		else if(m_iGoodOnGrid == QianKunFu)
		{
			pTip->AddText("请放入转换乾坤道具。您可以从商城中买到它，或者是参加活动部分BOSS也会掉落。");
		}
		else if(m_iGoodOnGrid == DiaoZhuoFu)
		{
			pTip->AddText("请放入符石雕琢符。雕琢符的等级必须与需要雕琢的符石一致。您可以从商城中买到它，部分活动BOSS也会掉落。");
		}	
		else if(m_iGoodOnGrid == HeChengFu)
		{
			pTip->AddText("请放入符石合成符。合成符的等级必须与需要合成的符石一致。您可以从商城中买到它，部分活动BOSS也会掉落。");
		}
		else if(m_iGoodOnGrid == ZhaiChuFu)
		{
			pTip->AddText("请放入符石摘除符。摘除符的等级必须与您需要摘除符石的等级一致。你可以通过生产技能制造出来不同等级的摘除符，部分活动BOSS也会掉落。当然，向其它玩家购买也是一个不错的选择。");
		}
		else if(m_iGoodOnGrid == EquipToUpgrade)
		{
			pTip->AddText("请放入除武器、勋章、宝石之外的装备。升级武器请至NPC铁匠处选择“黑铁升级武器”；勋章与宝石暂未开放强化功能。注：强化失败装备会破碎，符石完好但变为绑定状态。");
		}
		else if(m_iGoodOnGrid == QiangHuaFu)
		{
			pTip->AddText("强化需要消耗强化符。孔痕≥3的装备需要使用“天级强化符”来强化；孔痕＜3的装备使用“地级强化符”来强化。你可以通过生产技能制造出来天级或地级的各种装备强化符，部分活动BOSS也会掉落。当然，向其它玩家购买也是一个不错的选择。");
		}
		else if(m_iGoodOnGrid == XingYunFu)
		{
			pTip->AddText("强化若失败，装备会破碎。放入护宝符可以保证强化失败的装备完好无损。放入幸运符可以提高强化成功率。");
		}
		else if(m_iGoodOnGrid == FuShiToDegrade)
		{
			pTip->AddText("请放入纯净的符石，还原前的纯净符石与还原后的符石等级一致。并且还原后的符石与您绑定。");
		}
		else if(m_iGoodOnGrid == HuanYuanFu)
		{
			pTip->AddText("请放入符石还原符，还原符的等级必须与需要还原的纯净符石等级一致。您可以从商城中买到它，部分活动BOSS也会掉落。");
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);

		return true;

	}

	char * p[5] = {"金","木","土","水","火"};

	CGood & equipment = g_NPC.GetDrillResult().m_equip;

	if(m_iType == ChangeHole && equipment.GetID())
	{
		//WORD wHoles = LOWORD(equipment.GetDemonDark1());
		//BYTE byHoleType[10] = {0,0,0,0,0};

		//for(int i = 0;i < 10;i++)
		//{
		//	//if((wHoles & 0x7) >= 1 && (wHoles & 0x7) <= 5)
		//	//{
		//	//	byHoleType[i] = wHoles & 0x7;
		//	//}

		//	//wHoles >>= 3;

		//	if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
		//	{
		//		byHoleType[i] = ((equipment.GetDemonDark1() >> (i * 3)) & 0x7);
		//	}
		//	else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
		//	{
		//		WORD wHole = (WORD(equipment.GetResvEx3(0))) | (WORD(equipment.GetResvEx3(1)) << 8);
		//		byHoleType[i] = ((wHole >> ((i - 5) * 3)) & 0x7);
		//	}

		//}

		for(int i = 0;i < 10;i++)
		{
			if(iX > m_ptHoleToConvert[i].x && iX < m_ptHoleToConvert[i].x + 24 && iY > m_ptHoleToConvert[i].y && iY < m_ptHoleToConvert[i].y + 24)
			{
				BYTE byHoleType = 0;
				int iKind = 0;
				if(i < 5)//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
				{
					byHoleType = ((equipment.GetDemonDark1() >> (i * 3)) & 0x7);
					iKind = (equipment.GetResvEx(i) & 0xf0) >> 4;//前5个孔放在GetResvEx(i)
				}
				else//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
				{
					WORD wHole = (WORD(equipment.GetResvEx3(0))) | (WORD(equipment.GetResvEx3(1)) << 8);
					byHoleType = ((wHole >> ((i - 5) * 3)) & 0x7);
					iKind = (equipment.GetResvEx3(i - 5 + 2) & 0xf0) >> 4;//后5个孔放在 GetResvEx3(2)-GetResvEx3(6)
				}


				if(byHoleType && /*equipment.GetResvEx(i)*/iKind == 0)
				{					
					sprintf(str,"%s属性孔洞，只能镶嵌%s属性符石",p[byHoleType - 1],p[byHoleType - 1]);

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

CGood* CFuShiSeriesWnd::GetGridGood( int iX,int iY,int& grid )
{
	if(m_iType == DrillEquip)
	{
		if(iX > EquipmentToDrill_PosX && iX < EquipmentToDrill_PosX + 36 && iY > EquipmentToDrill_PosY && iY < EquipmentToDrill_PosY + 36) 
		{
			grid = EquipToDrill;
			return &(g_NPC.GetDrillResult().m_equip);
		}
		else if(iX > Driller_PosX && iX < Driller_PosX + 36 && iY > Driller_PosY && iY < Driller_PosY + 36)
		{
			grid = Drill;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
		else
		{
			return NULL;
		}
	}
	else if(m_iType == EmbedFuShi)
	{
		if(iX > EmbedEquipment_PosX && iX < EmbedEquipment_PosX + 36 && iY > EmbedEquipment_PosY && iY < EmbedEquipment_PosY + 36)
		{
			grid = EquipToEmbed;
			return &(g_NPC.GetDrillResult().m_equip);
		}
		else if(iX > EmbedFushi_PosX && iX < EmbedFushi_PosX + 36 && iY > EmbedFushi_PosY && iY < EmbedFushi_PosY + 36)
		{
			grid = FuShiToEmbed;
			return &(g_NPC.GetDrillResult().m_Fushi);
		}
		else if(iX > XiangQianFu_PosX && iX < XiangQianFu_PosX + 36 && iY > XiangQianFu_PosY && iY < XiangQianFu_PosY + 36)
		{
			grid = XianQianFu;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
		else 
		{
			return NULL;
		}
	}
	else if(m_iType == ChangeHole)
	{
		if(iX > EquipmentToConvert_PosX && iX < EquipmentToConvert_PosX + 36 && iY > EquipmentToConvert_PosY && iY < EquipmentToConvert_PosY + 36) 
		{
			grid = EquipToChangeHole;
			return &(g_NPC.GetDrillResult().m_equip);
		}
		else if(iX > QianKunFu_PosX && iX < QianKunFu_PosX + 36 && iY > QianKunFu_PosY && iY < QianKunFu_PosY + 36)
		{
			grid = QianKunFu;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
		else
		{
			return NULL;
		}
	}
	else if(m_iType == UpgradeFuShi)
	{
		if(iX > FuShiToUpgrade_PosX && iX < FuShiToUpgrade_PosX + 36 && iY > FuShiToUpgrade_PosY && iY < FuShiToUpgrade_PosY + 36) 
		{
			grid = FuShiToUpgrade;
			return &(g_NPC.GetDrillResult().m_Fushi);
		}
		else if(iX > DiaoZhuoFu_PosX && iX < DiaoZhuoFu_PosX + 36 && iY > DiaoZhuoFu_PosY && iY < DiaoZhuoFu_PosY + 36)
		{
			grid = DiaoZhuoFu;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
		else
		{
			return NULL;
		}
	}
	else if(m_iType == CombineFuShi)
	{

		for(int i = 0;i < 5;i++)
		{
			if(iX > m_ptGridPos[FuShiToCombine1 + i].x && iX < m_ptGridPos[FuShiToCombine1 + i].x + 36 && iY > m_ptGridPos[FuShiToCombine1 + i].y && iY < m_ptGridPos[FuShiToCombine1 + i].y + 36)
			{
				grid = FuShiToCombine1 + i;
				return &(g_NPC.GetDrillResult().m_FuShiToCombine[i]);
			}
		}	

		if(iX > HeChengFu_PosX && iX < HeChengFu_PosX + 36 && iY > HeChengFu_PosY && iY < HeChengFu_PosY + 36)
		{
			grid = HeChengFu;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
	}
	else if(m_iType == RemoveFuShi)
	{
		if(iX > RemoveFuShiEquip_PosX && iX < RemoveFuShiEquip_PosX + 36 && iY > RemoveFuShiEquip_PosY && iY < RemoveFuShiEquip_PosY + 36) 
		{
			grid = EquipToRemoveFuShi;
			return &(g_NPC.GetDrillResult().m_equip);
		}
		else if(iX > ZhaiChuFu_PosX && iX < ZhaiChuFu_PosX + 36 && iY > ZhaiChuFu_PosY && iY < ZhaiChuFu_PosY + 36)
		{
			grid = ZhaiChuFu;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
		else
		{
			return NULL;
		}
	}
	else if(m_iType == UpgradeEquip)
	{
		if(iX > EquipToUpgrade_PosX && iX < EquipToUpgrade_PosX + 36 && iY > EquipToUpgrade_PosY && iY < EquipToUpgrade_PosY + 36)
		{
			grid = EquipToUpgrade;
			return &(g_NPC.GetDrillResult().m_equip);
		}
		else if(iX > QiangHuaFu_PosX && iX < QiangHuaFu_PosX + 36 && iY > QiangHuaFu_PosY && iY < QiangHuaFu_PosY + 36)
		{
			grid = QiangHuaFu;
			return &(g_NPC.GetDrillResult().m_Fushi);
		}
		else if(iX > XingYunFu_PosX && iX < XingYunFu_PosX + 36 && iY > XingYunFu_PosY && iY < XingYunFu_PosY + 36)
		{
			grid = XingYunFu;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
		else 
		{
			return NULL;
		}
	}
	else if(m_iType == DegradeFuShi)
	{
		if(iX > FuShiToDegrade_PosX && iX < FuShiToDegrade_PosX + 36 && iY > FuShiToDegrade_PosY && iY < FuShiToDegrade_PosY + 36) 
		{
			grid = FuShiToDegrade;
			return &(g_NPC.GetDrillResult().m_Fushi);
		}
		else if(iX > HuanYuanFu_PosX && iX < HuanYuanFu_PosX + 36 && iY > HuanYuanFu_PosY && iY < HuanYuanFu_PosY + 36)
		{
			grid = HuanYuanFu;
			return &(g_NPC.GetDrillResult().m_Luck);
		}
		else
		{
			return NULL;
		}
	}

	return NULL;
}

bool CFuShiSeriesWnd::IsConditionSatisfied()
{
	if(m_iType == UpgradeEquip)
	{
		m_iNeedMoney = 10000;
	}
	else
	{
		m_iNeedMoney = 1000;
	}

	if(SELF.GetGold() < m_iNeedMoney)
	{
		return false;
	}

	if(m_iType == DrillEquip)
	{
		if(g_NPC.GetDrillResult().m_equip.GetID() && g_NPC.GetDrillResult().m_Luck.GetID())
		{
			return true;
		}
	}
	else if(m_iType == EmbedFuShi)
	{
		if(g_NPC.GetDrillResult().m_equip.GetID() && g_NPC.GetDrillResult().m_Fushi.GetID() && g_NPC.GetDrillResult().m_Luck.GetID())
		{
			return true;
		}
	}
	else if(m_iType == ChangeHole)
	{
		if(g_NPC.GetDrillResult().m_equip.GetID() && g_NPC.GetDrillResult().m_Luck.GetID())
		{
			BYTE byHoleNum = m_pHole[0]->GetRadio();
			BYTE byHoleToConverNum = m_pHoleToConvert[0]->GetRadio();

			if(byHoleNum >= 0 && byHoleNum <= 4 && byHoleToConverNum >= 0 && byHoleToConverNum <= 9)
			{
				return true;
			}
		}
	}
	else if(m_iType == UpgradeFuShi)
	{
		if(g_NPC.GetDrillResult().m_Fushi.GetID() && g_NPC.GetDrillResult().m_Luck.GetID())
		{
			return true;
		}
	}
	else if(m_iType == CombineFuShi)
	{
		int iCount = 0;

		for(int i = 0;i < 5;i++)
		{
			if(g_NPC.GetDrillResult().m_FuShiToCombine[i].GetID())
			{
				iCount++;
			}
		}

		if(iCount >= 3)
		{
			return true;
		}
	}
	else if(m_iType == RemoveFuShi)
	{
		if(g_NPC.GetDrillResult().m_equip.GetID() && g_NPC.GetDrillResult().m_Luck.GetID())
		{
			BYTE byHoleNum = m_pHole[0]->GetRadio();
			if(byHoleNum >= 0 && byHoleNum <= 9)
			{
				return true;
			}
		}
	}
	else if(m_iType == UpgradeEquip)
	{
		if(g_NPC.GetDrillResult().m_equip.GetID() && g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			return true;
		}
	}
	else if(m_iType == DegradeFuShi)
	{
		if(g_NPC.GetDrillResult().m_Fushi.GetID() && g_NPC.GetDrillResult().m_Luck.GetID())
		{
			return true;
		}
	}

	return false;
}

bool CFuShiSeriesWnd::IsInValidGoodOnGrid( int grid,CGood& good )
{
	int iStdMode = good.GetStdMode();
	int iShape = good.GetShape();

	if(grid == EquipToDrill || grid == EquipToRemoveFuShi || grid == EquipToChangeHole || grid == EquipToEmbed)	//应该放置可以打孔的装备
	{
		if(!g_AIGoodMgr.IsCanDigHoleEquip(good))
		{
			return true;
		}		

		if ( g_AIGoodMgr.IsEquipShield(good) && g_NPC.GetDrillResult().m_Fushi.GetID() && !g_AIGoodMgr.IsLingFuShi(g_NPC.GetDrillResult().m_Fushi))//盾牌只能用灵符石
		{
			return true;
		}	
	}	
	else if(grid >= FuShiToEmbed && grid <= FuShiToCombine5 || grid == FuShiToDegrade)		//应该放置符石
	{
		if(!g_AIGoodMgr.IsFuShi(good))
		{
			return true;
		}

		if (g_NPC.GetDrillResult().m_equip.GetID() && g_AIGoodMgr.IsEquipShield(g_NPC.GetDrillResult().m_equip) && !g_AIGoodMgr.IsLingFuShi(good))//盾牌只能用灵符石
		{
			return true;
		}

		if(grid == FuShiToUpgrade)
		{
			if(g_NPC.GetDrillResult().m_Luck.GetID() && g_NPC.GetDrillResult().m_Luck.GetAC2() != good.GetAC2())
			{
				return true;
			}

			if(((good.GetShape() - 1) % 6 + 1) > 3)
			{
				return true;
			}
		}
		else if(grid == FuShiToDegrade)
		{
			if(g_NPC.GetDrillResult().m_Luck.GetID() && g_NPC.GetDrillResult().m_Luck.GetAC2() != good.GetAC2())
			{
				return true;
			}

			if(((good.GetShape() - 1) % 6 + 1) <= 3)
			{
				return true;
			}
		}

		if(grid >= FuShiToCombine1 && grid <= FuShiToCombine5)
		{
			if(((good.GetShape() - 1) % 6 + 1) > 3)	//纯净符石无法合成
			{
				return true;
			}

			for(int i = 0;i < 5;i++)
			{
				if(grid - FuShiToCombine1 == i)	//替换符石
				{
					continue;
				}

				if(g_NPC.GetDrillResult().m_FuShiToCombine[i].GetID())
				{
					if(good.GetAC2() != g_NPC.GetDrillResult().m_FuShiToCombine[i].GetAC2() || good.GetShape() != g_NPC.GetDrillResult().m_FuShiToCombine[i].GetShape())
					{
						return true;
					}					
					else 
					{
						break;
					}
				}

				if(g_NPC.GetDrillResult().m_Luck.GetID() && g_NPC.GetDrillResult().m_Luck.GetAC2() != good.GetAC2())
				{
					return true;
				}
			}
		}
	}
	else if(grid == Drill)		//应该放置打孔材料
	{
		if(!(iStdMode == 79 && iShape >= 1 && iShape <= 6))
		{
			return true;
		}
	}
	else if(grid == XianQianFu)
	{
		if(!(iStdMode == 44 && iShape == 6))
		{
			return true;
		}
	}
	else if(grid == QianKunFu)
	{
		if(!(iStdMode == 79 && iShape == 8))
		{
			return true;
		}
	}
	else if(grid == DiaoZhuoFu)
	{
		if(!(iStdMode == 44 && iShape == 9))
		{
			return true;
		}

		if(g_NPC.GetDrillResult().m_Fushi.GetID() && good.GetAC2() != g_NPC.GetDrillResult().m_Fushi.GetAC2())
		{
			return true;
		}
	}
	else if(grid == HeChengFu)
	{
		if(!(iStdMode == 44 && iShape == 7))
		{
			return true;
		}

		for(int i = 0;i < 5;i++)
		{
			if(g_NPC.GetDrillResult().m_FuShiToCombine[i].GetID())
			{
				if(good.GetAC2() != g_NPC.GetDrillResult().m_FuShiToCombine[i].GetAC2())
				{
					return true;
				}
				else 
				{
					break;
				}
			}
		}
	}
	else if(grid == ZhaiChuFu)
	{
		if(!(iStdMode == 44 && iShape == 8))
		{
			return true;
		}
	}
	else if(grid == EquipToUpgrade)
	{
		if(!(g_AIGoodMgr.IsEquip(good) && iStdMode != 5 && iStdMode != 6 && iStdMode != 59 && iStdMode != 60 && iStdMode != 61))
		{
			return true;
		}

		if(g_NPC.GetDrillResult().m_Fushi.GetID())
		{
			if(g_NPC.GetDrillResult().m_Fushi.GetShape() <= 3 && g_AIGoodMgr.GetLevel(good) < 40)
			{
				return true;
			}

			if(g_NPC.GetDrillResult().m_Fushi.GetShape() >= 4 && g_AIGoodMgr.GetLevel(good) >= 40)
			{
				return true;
			}
		}
	}
	else if(grid == QiangHuaFu)
	{
		if(!(iStdMode == 39 && iShape >= 1 && iShape <= 6))
		{
			return true;
		}

		if(g_NPC.GetDrillResult().m_equip.GetID())
		{
			if(iShape <= 3 && g_AIGoodMgr.GetLevel(g_NPC.GetDrillResult().m_equip) < 40)
			{
				return true;
			}

			if(iShape >= 4 && g_AIGoodMgr.GetLevel(g_NPC.GetDrillResult().m_equip) >= 40)
			{
				return true;
			}
		}
	}
	else if(grid == XingYunFu)
	{
		if(!(iStdMode == 44 && iShape >= 3 && iShape <= 5))
		{
			return true;
		}
	}
	else if(grid == HuanYuanFu)
	{
		if(!(iStdMode == 44 && iShape == 10))
		{
			return true;
		}

		if(g_NPC.GetDrillResult().m_Fushi.GetID() && good.GetAC2() != g_NPC.GetDrillResult().m_Fushi.GetAC2())
		{
			return true;
		}
	}

	return false;
}

void CFuShiSeriesWnd::ShowEffect()
{
	LPTexture pTex = NULL;

	bool bDrawStand = true;	//绘制待机的NPC

	if(m_iSuccessFrame < 15 * 8)
	{
		m_iFrame = 100000;

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20951,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		if(m_iType == DrillEquip)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您打孔成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == EmbedFuShi)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"一阵宝光闪过，您的装备被成功的镶嵌",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"了一颗符石，它的能力将得到极大的提升！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == ChangeHole)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"您成功的更改了装备上孔洞的属性，",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"现在去找符合孔洞属性的符石来镶嵌吧！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == UpgradeFuShi)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"您的符石雕琢成功！一颗闪亮的新符石诞生，",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"它的属性得到了极大的提升。赶快去镶嵌吧！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == CombineFuShi)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"英雄今天真是吉星高照！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"您的符石合成成功啦！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == RemoveFuShi)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"叮叮叮，",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"一颗符石被您成功的从装备上摘除了下来。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == UpgradeEquip)
		{
			if(m_bWithHuBaoFu)
			{
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"在幸运符的护佑下，一阵宝光闪过，",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"您的装备强化成功！装备上的星星增加了一颗。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			}
			else
			{
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"一阵宝光闪过，",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"您的装备强化成功！装备上的星星增加了一颗。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			}
		}
		else if(m_iType == DegradeFuShi)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 60,"恭喜您还原符石成功！",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
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
		
		
		if(m_iSuccessFrame < 17 * 3)
		{
			bDrawStand = false;

			LPTexture pSuccessTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20935,EP_UI);

			if(pSuccessTex)
			{
				pSuccessTex->EnableSysAnim(false);
				pSuccessTex->SetCurFrame(m_iSuccessFrame / 3);	
				g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pSuccessTex);
			}
		}

		m_iSuccessFrame++;		
	}
	else if(m_iFailFrame < 15 * 8)
	{
		m_iFrame = 100000;

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20951,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 266,m_iScreenY + 44,pTex);

		if(m_iType == DrillEquip)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"这次似乎不太幸运，",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"打孔失败了，打孔材料也破碎掉了。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == EmbedFuShi)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"镶嵌失败了！符石也消失了。放入",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"符石镶嵌符（高级）可以保证您的镶嵌100%成功。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == CombineFuShi)
		{
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"符石合成失败！参与合成的符石数量与",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"正确等级的符石合成符是影响符石合成成败的关键。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
		}
		else if(m_iType == UpgradeEquip)
		{
			if(m_bWithHuBaoFu)
			{
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 55,"这次似乎不太幸运啊。强化失败了，",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 70,"但是在护宝符的护佑下，你的装备完好无损。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			}
			else
			{
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 50,"这次似乎不太幸运啊。",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 65,"强化失败了，没有护宝符的保护，装备也破碎了",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
				g_pFont->DrawText(m_iScreenX + 435,m_iScreenY + 80,"（符石完好无损但变为绑定状态还回您的包裹里）",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
			}
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
		

		if(m_iFailFrame < 17 * 4)
		{
			bDrawStand = false;

			LPTexture pFailTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20936,EP_UI);

			if(pFailTex)
			{
				pFailTex->EnableSysAnim(false);
				pFailTex->SetCurFrame(m_iFailFrame / 4);	
				g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pFailTex);
			}
		}

		m_iFailFrame++;
	}	

	if(pTex && m_iType != CombineFuShi)
	{
		g_pGfx->SetRenderMode(RM_ADD2);

		int iPosX = 0;
		int iPosY = 0;

		if(m_iType == DrillEquip)
		{
			iPosX = EquipmentToDrill_PosX;
			iPosY = EquipmentToDrill_PosY;				
		}
		else if(m_iType == EmbedFuShi)
		{
			iPosX = EmbedEquipment_PosX;
			iPosY = EmbedEquipment_PosY;	
		}
		else if(m_iType == ChangeHole)
		{
			iPosX = EquipmentToConvert_PosX;
			iPosY = EquipmentToConvert_PosY;	
		}
		else if(m_iType == UpgradeFuShi)
		{
			iPosX = FuShiToUpgrade_PosX;
			iPosY = FuShiToUpgrade_PosY;	
		}
		else if(m_iType == RemoveFuShi)
		{
			iPosX = RemoveFuShiEquip_PosX;
			iPosY = RemoveFuShiEquip_PosY;	
		}
		else if(m_iType == UpgradeEquip)
		{
			iPosX = EquipToUpgrade_PosX;
			iPosY = EquipToUpgrade_PosY;	
		}
		else if(m_iType == DegradeFuShi)
		{
			iPosX = FuShiToDegrade_PosX;
			iPosY = FuShiToDegrade_PosY;
		}

		g_pGfx->DrawTextureNL(m_iScreenX + iPosX,m_iScreenY + iPosY,pTex);

		g_pGfx->SetRenderMode();
	}

	if(m_iFrame < TotalFrame)
	{
		if(m_iFrame == 10 * FrameSpeed)
		{
			m_bFirstClick = false;

			if(m_bConditionSatisfy)
			{
				if(m_iType == DrillEquip)
				{
					g_pGameControl->Send_Drill_Equip(g_NPC.GetDrillResult().m_equip.GetID(),g_NPC.GetDrillResult().m_Luck.GetID(),false);
				}
				else if(m_iType == EmbedFuShi)
				{
					g_pGameControl->Send_Embed_Fushi(g_NPC.GetDrillResult().m_equip.GetID(),g_NPC.GetDrillResult().m_Fushi.GetID(),g_NPC.GetDrillResult().m_Luck.GetID(),false);
				}
				else if(m_iType == ChangeHole)
				{
					g_pGameControl->Send_Change_Hole(g_NPC.GetDrillResult().m_equip.GetID(),m_pHoleToConvert[0]->GetRadio(),m_pHole[0]->GetRadio(),g_NPC.GetDrillResult().m_Luck.GetID(),false);
				}
				else if(m_iType == UpgradeFuShi)
				{
					g_pGameControl->Send_Upgrade_Fushi(g_NPC.GetDrillResult().m_Fushi.GetID(),g_NPC.GetDrillResult().m_Luck.GetID(),false);
				}
				else if(m_iType == CombineFuShi)
				{
					g_pGameControl->Send_Combine_Fushi(g_NPC.GetDrillResult().m_Luck.GetID(),g_NPC.GetDrillResult().m_FuShiToCombine[0].GetID(),g_NPC.GetDrillResult().m_FuShiToCombine[1].GetID(),
						g_NPC.GetDrillResult().m_FuShiToCombine[2].GetID(),g_NPC.GetDrillResult().m_FuShiToCombine[3].GetID(),g_NPC.GetDrillResult().m_FuShiToCombine[4].GetID(),false);
				}
				else if(m_iType == RemoveFuShi)
				{
					g_pGameControl->Send_Remove_Fushi(g_NPC.GetDrillResult().m_equip.GetID(),m_pHole[0]->GetRadio(),g_NPC.GetDrillResult().m_Luck.GetID(),false);
				}
				else if(m_iType == UpgradeEquip)
				{
					g_pGameControl->Send_Upgrade_Equip(g_NPC.GetDrillResult().m_equip.GetID(),g_NPC.GetDrillResult().m_Fushi.GetID(),g_NPC.GetDrillResult().m_Luck.GetID());
				}
				else if(m_iType == DegradeFuShi)
				{
					g_pGameControl->Send_Degrade_Fushi(g_NPC.GetDrillResult().m_Fushi.GetID(),g_NPC.GetDrillResult().m_Luck.GetID());
				}
			}
		}

		if(m_iFrame == 0)
		{
			g_pAudio->Play(EAT_OTHER,920,g_pAudio->GetRand()++);
		}

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20934,EP_UI);
		if(pTex)
		{
			pTex->EnableSysAnim(false);
			pTex->SetCurFrame(m_iFrame / FrameSpeed % 9);
			g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pTex);
		}
		m_iFrame++;
	}
	else if(bDrawStand)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20933,EP_UI);	
		g_pGfx->DrawTextureNL(m_iScreenX + 325,m_iScreenY + 125,pTex);
	}
}
