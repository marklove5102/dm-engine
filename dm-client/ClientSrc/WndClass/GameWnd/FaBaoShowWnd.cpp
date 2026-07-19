#include "FaBaoShowWnd.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/GameData.h"
#include "GameAI/AIGoodMgr.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Global/Interface/AudioInterface.h"
#include "GameControl\GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/DirtyWords.h"
#include "GameData/OtherData.h"

INIT_WND_POSX(CFaBaoShowWnd,POS_AUTO,POS_AUTO)

CFaBaoShowWnd::CFaBaoShowWnd(void):
m_pLianHua(NULL)
{
	m_iBlankColor = 0;
	m_bHasFaZhen = false;
	m_iVersion = 2;

	if (strcmp(g_strVersion,"1.2.5.8") >= 0)
	{
		m_bHasFaZhen = true;
	}

	/*if (m_bHasFaZhen)
	{
		AddTabPage(0,0,MAKELONG(22965,PACKAGE_INTERFACE),10,67,85,86,87,88,"人物",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
		AddTabPage(0,0,MAKELONG(22965,PACKAGE_INTERFACE),10,152,85,86,87,88,"法宝",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	}
	else*/

	AddTabPage(0,0,MAKELONG(22520,PACKAGE_INTERFACE),10,67,85,86,87,88,"人物",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	AddTabPage(0,0,MAKELONG(22520,PACKAGE_INTERFACE),10,152,85,86,87,88,"法宝",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);

	if (g_OtherData.GetHorseManShipInfo().Grade > 0)
	{
		AddTabPage(0,0,MAKELONG(22520,PACKAGE_INTERFACE),10,237,85,86,87,88,"骑术",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	}

	m_TabPage.iCurPage = 1;
}

CFaBaoShowWnd::~CFaBaoShowWnd(void)
{
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"AvatarWnd");
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"QiShouWnd");
}

void CFaBaoShowWnd::OnCreate()
{
	SetCloseButton(466,4,80);

	/*if (m_bHasFaZhen)
	{
		m_pEdFaZhenName = new CCtrlEdit();
		AddControl(m_pEdFaZhenName);
		m_pEdFaZhenName->CreateEx(this,305,89,115,18);

		if (strlen(SELF.GetYIHUOINFO().strName) != 0)
		{
			m_pEdFaZhenName->SetText(SELF.GetYIHUOINFO().strName,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);

		} 
		else
		{
			m_pEdFaZhenName->SetText("法诀无双",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		}

		m_pEdFaZhenName->SetDigital(false);
		m_pEdFaZhenName->SetMaxLength(8);
		m_pEdFaZhenName->SetFocus();
		m_pEdFaZhenName->SetTips("此处可输入自定义的法诀名称(最多四个中文)");

		m_pFaZhenShortKey = new CCtrlButton();
		AddControl(m_pFaZhenShortKey);
		m_pFaZhenShortKey->CreateEx(this,153,391, 22967,22967,22967,22967);
		RefreshButtonState();
		m_pFaZhenShortKey->SetTips("天、地、人三类法宝各装备1件即可激活\n使用技能后下一次攻击即可出发法诀技能\n技能效果和技能伤害与法阵等级以及3件已装备法宝的战灵值相关。");

	}
	else*/
	/*{
		m_pEdFaZhenName = NULL;
		m_pFaZhenShortKey = NULL;
	}*/


	CCtrlWindowX::OnCreate();
}

void CFaBaoShowWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	static char* TIANDIREN[] = 
	{
		"",
		"天",
		"地",
		"人",
	};
	
	char temp[64] = {0};

	g_pFont->DrawText(m_iScreenX + 228,m_iScreenY + 11,"圣殿法宝",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX+ 97,m_iScreenY + 68,"基本属性",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 97,m_iScreenY + 177,"特殊属性",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 97,m_iScreenY + 253,"五行加成",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX+ 60,m_iScreenY + 88,"契合度",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 60,m_iScreenY + 88 + 21 * 1,"法宝伤害",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 60,m_iScreenY + 88 + 21 * 2,"命 中",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 60,m_iScreenY + 88 + 21 * 3,"幸 运",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);


	g_pFont->DrawText(m_iScreenX+ 64,m_iScreenY + 197,"暴 击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 64,m_iScreenY + 197 + 21,"破 防",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	
	g_pFont->DrawText(m_iScreenX+ 54,m_iScreenY + 272,"金攻加成",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 54,m_iScreenY + 272 + 21 * 1,"木攻加成",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 54,m_iScreenY + 272 + 21 * 2,"土攻加成",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 54,m_iScreenY + 272 + 21 * 3,"水攻加成",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 54,m_iScreenY + 272 + 21 * 4,"火攻加成",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX+ 224,m_iScreenY + 439,"金攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 224,m_iScreenY + 439 + 18 * 1,"木攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 224,m_iScreenY + 439 + 18 * 2,"土攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 224,m_iScreenY + 439 + 18 * 3,"水攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 224,m_iScreenY + 439 + 18 * 4,"火攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX+ 334,m_iScreenY + 439,"金防御减免",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 334,m_iScreenY + 439 + 18 * 1,"木防御减免",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 334,m_iScreenY + 439 + 18 * 2,"土防御减免",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 334,m_iScreenY + 439 + 18 * 3,"水防御减免",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 334,m_iScreenY + 439 + 18 * 4,"火防御减免",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pGfx->DrawTextureNL(m_iScreenX + 47,m_iScreenY + 438,g_pTexMgr->GetTex(PACKAGE_INTERFACE,23205,EP_UI)); 

	//////////////////////////异火///////////////////////

	//if (m_bHasFaZhen)
	//{
	//	g_pFont->DrawText(m_iScreenX +70,m_iScreenY +398,"法诀技能",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
	//}

	//g_pGfx->SetRenderMode(RM_ADD2);
	//LPTexture m_pDiwen = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22979,EP_UI);
	//g_pGfx->DrawTextureNL(m_iScreenX + 177,m_iScreenY + 199,m_pDiwen);
	//
	//g_pGfx->SetRenderMode();
	//
	//if (m_bHasFaZhen && SELF.GetYIHUOINFO().dwLevel > 0)
	//{
	//	sprintf(temp,"%d级法阵",SELF.GetYIHUOINFO().dwLevel);
	//	g_pFont->DrawText(m_iScreenX+ 312,m_iScreenY + 67,temp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	//	int curBk = (SELF.GetYIHUOINFO().dwLevel)%7 + 1;
	//	LPTexture m_pYihuo = NULL;
	//	
	//	if (curBk > 2)
	//	{
	//		m_pYihuo = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22971 + curBk - 1,EP_UI);
	//		g_pGfx->DrawTextureNL(m_iScreenX + 81,m_iScreenY + 74,m_pYihuo); 
	//	}
	//	else if (SELF.GetYIHUOINFO().dwLevel > 6)
	//	{
	//		m_pYihuo = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22978,EP_UI);
	//		g_pGfx->DrawTextureNL(m_iScreenX + 81,m_iScreenY + 74,m_pYihuo); 
	//	}

	//	m_pYihuo = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22971 + curBk,EP_UI);
	//	float Angle = (float)((double)SELF.GetYIHUOINFO().i64CurExp / (double)SELF.GetYIHUOINFO().i64CurMaxExp);
	//	g_pGfx->DrawArcTexture(m_iScreenX + 337,m_iScreenY + 330,m_pYihuo,0.0f,Angle * 360.0f);

	//	g_pGfx->SetRenderMode(RM_ADD2);
	//	m_pYihuo = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22982,EP_UI);
	//	g_pGfx->DrawTextureNL(m_iScreenX + 81,m_iScreenY + 74,m_pYihuo); 
	//	g_pGfx->SetRenderMode();

	//}
	//

	CGood* pGood = NULL;
	//
	//////////////////////////左法宝/////////////////////
	//pGood = SELF.EquipGood().GetPtr(ITEM_POS_FABAOLEFT);
	//
	//if (pGood != NULL && g_AIGoodMgr.IsFaBao(*pGood) && SELF.GetFaBaoType()->dwLeftFaBaoType > 0)
	//{
	//	LPTexture CharacterPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22992+TianDiRen(SELF.GetFaBaoType()->dwLeftFaBaoType),EP_UI);
	//	g_pGfx->DrawTextureNL(m_iScreenX + 230,m_iScreenY + 98,CharacterPicture); 
	//	
	//	LPTexture fabaoPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, SELF.GetFaBaoType()->dwLeftFaBaoType + 22669,EP_UI);
	//	g_pGfx->DrawTextureFX(m_iScreenX+142,m_iScreenY +106,fabaoPicture,-1,NULL,&POS(0.6f,0.6f));

	//	g_pGfx->SetRenderMode(RM_ADD2);
	//	int FabaoLevel = pGood->GetLevelUpTimes();
	//	int esLevelColor = FabaoLevel >= 9?2:(FabaoLevel >= 6?1:0);
	//	LPTexture FaBaoEff = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22810 + (GetFaBaoEffType(SELF.GetFaBaoType()->dwLeftFaBaoType) - 1) * 3 + esLevelColor,EP_UI);
	//	g_pGfx->DrawTextureFX(m_iScreenX +199,m_iScreenY +139,FaBaoEff,-1,NULL,&POS(0.6f,0.6f));
	//	g_pGfx->SetRenderMode();
	//}
	//else
	//{
	//	LPTexture m_pBlank = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22980,EP_UI);
	//
	//	g_pGfx->DrawTextureNL(m_iScreenX + 240,m_iScreenY + 160,m_pBlank); 

	//	if (m_iBlankColor == 1)//左红
	//	{
	//		g_pGfx->DrawFillRect(m_iScreenX + 240,m_iScreenY + 160,40,40,0x80ff0000);
	//	}
	//	else if (m_iBlankColor == 2)//左绿
	//	{
	//		g_pGfx->DrawFillRect(m_iScreenX + 240,m_iScreenY + 160,40,40,0x8000FF00);
	//	}
	//}

	//////////////////////////右法宝/////////////////////
	//pGood = SELF.EquipGood().GetPtr(ITEM_POS_FABAORIGHT);
	//
	//if (pGood != NULL && g_AIGoodMgr.IsFaBao(*pGood) && SELF.GetFaBaoType()->dwRightFaBaoType > 0)
	//{
	//	LPTexture CharacterPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22992+TianDiRen(SELF.GetFaBaoType()->dwRightFaBaoType),EP_UI);
	//	g_pGfx->DrawTextureNL(m_iScreenX + 390,m_iScreenY + 95,CharacterPicture); 

	//	LPTexture fabaoPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, SELF.GetFaBaoType()->dwRightFaBaoType + 22669,EP_UI);
	//	g_pGfx->DrawTextureFX(m_iScreenX + 304,m_iScreenY + 106,fabaoPicture,-1,NULL,&POS(0.6f,0.6f));

	//	g_pGfx->SetRenderMode(RM_ADD2);
	//	int FabaoLevel = pGood->GetLevelUpTimes();
	//	int esLevelColor = FabaoLevel >= 9?2:(FabaoLevel >= 6?1:0);
	//	LPTexture FaBaoEff = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22810 + (GetFaBaoEffType(SELF.GetFaBaoType()->dwRightFaBaoType) - 1) * 3 + esLevelColor,EP_UI);
	//	g_pGfx->DrawTextureFX(m_iScreenX + 359,m_iScreenY + 141,FaBaoEff,-1,NULL,&POS(0.6f,0.6f));
	//	g_pGfx->SetRenderMode();
	//}
	//else
	//{
	//	LPTexture m_pBlank = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22980,EP_UI);

	//	g_pGfx->DrawTextureNL(m_iScreenX + 394,m_iScreenY + 160,m_pBlank); 
	//	
	//	if (m_iBlankColor == 3)//左红
	//	{
	//		g_pGfx->DrawFillRect(m_iScreenX + 394,m_iScreenY + 160,40,40,0x80ff0000);
	//	}
	//	else if (m_iBlankColor == 4)//左绿
	//	{
	//		g_pGfx->DrawFillRect(m_iScreenX + 394,m_iScreenY + 160,40,40,0x8000FF00);
	//	}
	//}

	
	//////////////////////////主法宝/////////////////////
	pGood = SELF.EquipGood().GetPtr(ITEM_POS_FABAO);
	int iWuXing = 0;
	if ((pGood != NULL) && (g_AIGoodMgr.IsFaBao(*pGood)) && (SELF.GetFaBaoType()->dwCurFaBaoType >= 1))
	{
		/*LPTexture CharacterPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22992+TianDiRen(SELF.GetFaBaoType()->dwCurFaBaoType),EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 310,m_iScreenY + 95,CharacterPicture); */
		
		LPTexture fabaoPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, SELF.GetFaBaoType()->dwCurFaBaoType + 22669,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 143,m_iScreenY + 66,fabaoPicture);

		iWuXing = g_AIGoodMgr.GetFabaoWuXing(*pGood);
		g_pGfx->SetRenderMode(RM_ADD2);
		LPTexture backGround = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22575 + iWuXing,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 210,m_iScreenY + 10,backGround);


		int esLevelColor = SELF.GetFaBaoType()->iFaBaoLevel >= 9?2:(SELF.GetFaBaoType()->iFaBaoLevel >= 6?1:0);
		LPTexture FaBaoEff = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22810 + (GetFaBaoEffType(SELF.GetFaBaoType()->dwCurFaBaoType) - 1) * 3 + esLevelColor,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 238,m_iScreenY + 124,FaBaoEff);
		g_pGfx->SetRenderMode();
	}


	//契合度
	
	//YIHUO_INFO& YiHuo = SELF.GetYIHUOINFO();
	//攻击
	sprintf(temp,"%d-%d",g_AIGoodMgr.GetFabaoMinAttack(*pGood),g_AIGoodMgr.GetFabaoMaxAttack(*pGood));
	g_pFont->DrawText(m_iScreenX+ 64 + 50,m_iScreenY + 88 + 21,temp,COLOR_TEXT_NORMAL);
	/*sprintf(temp,"+%d%%",SELF.GetFaBaoType()->iFaBaoLevel * 5);
	g_pFont->DrawText(m_iScreenX+ 64 + 96,m_iScreenY + 88 + 21,temp,0xff00ff00);*/
	//命中
	sprintf(temp,"%d",g_AIGoodMgr.GetFaBaoMinZhong(*pGood));
	g_pFont->DrawText(m_iScreenX+ 64 + 50,m_iScreenY + 88 + 42,temp,COLOR_TEXT_NORMAL);
	//幸运
	sprintf(temp,"%d",g_AIGoodMgr.GetFaBaoXingYun(*pGood));
	g_pFont->DrawText(m_iScreenX+ 64 + 50,m_iScreenY + 88 + 63,temp,COLOR_TEXT_NORMAL);
	//暴击
	sprintf(temp,"%d",g_AIGoodMgr.GetFaBaoBaoJi(*pGood));
	g_pFont->DrawText(m_iScreenX+ 64 + 50,m_iScreenY + 197,temp,COLOR_TEXT_NORMAL);

	//破防
	sprintf(temp,"%d",g_AIGoodMgr.GetFaBaoPoFang(*pGood));
	g_pFont->DrawText(m_iScreenX+ 64 + 50,m_iScreenY + 197 + 21,temp,COLOR_TEXT_NORMAL);
	//攻击加成
	
	int defendReduce[5]={0,0,0,0,0};

	if (iWuXing >= 0 && iWuXing <= 5)
	{
		for (int ii = 0; ii != 5; ++ii)
		{
			BYTE add = 0;

			if (iWuXing > 0 && ii == iWuXing - 1)
			{
				sprintf(temp,"%d%%",pGood->GetFaBaoLingGenLevel() + g_AIGoodMgr.GetFaBaoWuXingAddAttack(*pGood) + add);
				defendReduce[ii] += g_AIGoodMgr.GetFaBaoWuXingReduceDefence(*pGood);
			} 
			else
			{
				sprintf(temp,"%d%%",add);
				
			}

			g_pFont->DrawText(m_iScreenX+ 64 + 50,m_iScreenY + 272 + ii * 21,temp,COLOR_TEXT_NORMAL);
		}
	}

	//攻击,防御减免
	int attackMin[5]={0,0,0,0,0};
	int attackMax[5]={0,0,0,0,0};
	
	//int qiHeDu = 0;//圣灵契合度加成

	int iTotalHoleNum = g_AIGoodMgr.GetCanDigHoles(*pGood);		
	if (iTotalHoleNum > 0)//能镶嵌的必定是洞的个数 > 0
	{

		int iOpenHoleNum = g_AIGoodMgr.GetDigHole(*pGood);
		char cXiangQianHoleNum[5] = {0,0,0,0,0};//不同属性骧嵌了的洞的个数

		WORD wHoles = LOWORD(pGood->GetDemonDark1());
		BYTE byHoleType[10] = {0,0,0,0,0,0,0,0,0,0};

		//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
		for(int i = 0;i < 5;i++)
		{
			if((wHoles & 0x7) >= 1 && (wHoles & 0x7) <= 5)
			{
				byHoleType[i] = wHoles & 0x7;
			}

			wHoles >>= 3;
		}
		//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
		wHoles = (WORD(pGood->GetResvEx3(0))) | (WORD(pGood->GetResvEx3(1)) << 8);
		for(int i = 5;i < 10;i++)
		{
			if((wHoles & 0x7) >= 1 && (wHoles & 0x7) <= 5)
			{
				byHoleType[i] = wHoles & 0x7;
			}

			wHoles >>= 3;
		}		


		for(int i = 0;i < 10;i++)
		{
			int iKind = 0,iLevel = 0;
			if (i < 5)//前5个孔放在GetResvEx(i)
			{
				iKind = (pGood->GetResvEx(i) & 0xf0) >> 4;
				iLevel = pGood->GetResvEx(i) & 0x0f;			
			}
			else//后5个孔放在 GetResvEx3(2)-GetResvEx3(6)
			{							
				iKind = (pGood->GetResvEx3(i - 5 + 2) & 0xf0) >> 4;
				iLevel = pGood->GetResvEx3(i - 5 + 2) & 0x0f;			
			}

			//if(FocusGood.GetResvEx(i) && byHoleType[i] >= 1 && byHoleType[i] <= 5)
			if(iKind > 0 && byHoleType[i] >= 1 && byHoleType[i] <= 5)
			{
				if (iKind == 1)
				{
					attackMax[byHoleType[i] - 1] += g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel);
				}
				else if(iKind == 2 || iKind == 4)
				{					
					defendReduce[byHoleType[i] - 1] += g_AIGoodMgr.GetFuShiAddDefense(iKind,iLevel);
					
					/*if(iKind == 4)
						qiHeDu += iLevel;*/
				}
				else if ( iKind == 3)
				{
					attackMin[byHoleType[i] - 1] += g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel);
					attackMax[byHoleType[i] - 1] += g_AIGoodMgr.GetFuShiAddAttack(iKind,iLevel);
				}
			}
		}
	}
	
	for (int ii = 0; ii != 5; ++ii)
	{
		sprintf(temp,"%d-%d",attackMin[ii],attackMax[ii]);
		g_pFont->DrawText(m_iScreenX+ 200 + 75,m_iScreenY + 439 + 18* ii,temp,COLOR_TEXT_NORMAL);

		sprintf(temp,"%d%%",defendReduce[ii]);
		g_pFont->DrawText(m_iScreenX+ 334 + 75,m_iScreenY + 439 + 18* ii,temp,COLOR_TEXT_NORMAL);
	}

	//契合度
	sprintf(temp,"%.1f%%",g_AIGoodMgr.GetFaBaoQiHeDu(*pGood));
	g_pFont->DrawText(m_iScreenX+ 64 + 50,m_iScreenY + 88,temp,COLOR_TEXT_NORMAL);
}

bool CFaBaoShowWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if (pControl)
			{
				S_TabPage * pParentTabpage = (S_TabPage * )pControl;
				if (pParentTabpage == &m_TabPage && dwData == 0)//点了纸娃娃页签
				{
					CloseWindow();
					g_pControl->PopupWindow(MSG_CTRL_CHARWND,OPER_CREATE);
					return true;
				}

				if (pParentTabpage == &m_TabPage && dwData == 2)//点了法宝页签
				{
					CloseWindow();
					g_pControl->PopupWindow(MSG_CTRL_QISHOU_WND,OPER_CREATE);
					return true;
				}
			}
		}
		break;
	//case MSG_CTRL_EDIT_ONCHAR:
	////case MSG_CTRL_EDIT_ENTRY:
	//	{
	//		if(pControl == m_pEdFaZhenName)
	//		{
	//			const char* FaZhenName = m_pEdFaZhenName->GetText();
	//			char cleanedWord[64] = {0};
	//			g_DirtyWords.ClearWords(FaZhenName,cleanedWord);
	//			g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(SELF.GetYIHUOINFO().byShortCut,cleanedWord);
	//			SELF.SetYiHuoName(cleanedWord);
	//			return true;
	//		}
	//	}
	/*case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pFaZhenShortKey)
			{
				g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CREATE,3);
				return true;
			}
		}*/
	/*case MSG_CTRL_FABAOSHOW_UPDATE:
		{
			RefreshButtonState();
			break;
		}*/
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


bool CFaBaoShowWnd::OnMouseMove(int iX,int iY)
{
	if((iX > 305 && iX < 370 && iY > 135 && iY < 265))
	{
		CParserTip* pTip = g_pControl->GetTipWnd();
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->SetShow(false);
		

		//DWORD dwColor = 0xFFFFCF63;
		if( SELF.GetEquipGood(ITEM_POS_FABAO).GetID() != 0)
		{
			pTip->Clear();
			pTip->Parse(SELF.GetEquipGood(ITEM_POS_FABAO));
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			
		}
		return true;
	}
		//左法宝
	//if(m_bHasFaZhen && (iX > 236 && iX < 280 && iY > 150 && iY < 250))
	//{
	//	CParserTip* pTip = g_pControl->GetTipWnd();
	//	int x = m_iScreenX + iX + 10;
	//	int y = m_iScreenY + iY + 10;
	//	pTip->SetShow(false);


	//	//DWORD dwColor = 0xFFFFCF63;
	//	if( SELF.GetEquipGood(ITEM_POS_FABAOLEFT).GetID() != 0)
	//	{
	//		pTip->Clear();
	//		pTip->Parse(SELF.GetEquipGood(ITEM_POS_FABAOLEFT));
	//		pTip->AdjustXY(x,y);
	//		pTip->Move(x,y);
	//		pTip->SetShow(true);

	//	}
	//	else if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() == 0)
	//	{
	//		pTip->Clear();
	//		pTip->AddText("此处可放入辅法宝");
	//		pTip->AdjustXY(x,y);
	//		pTip->Move(x,y);
	//		pTip->SetShow(true);

	//		m_iBlankColor = 0;
	//	}
	//	else if(g_AIGoodMgr.IsFaBao(CGoodGrid::GetDropGoodFrom().DropGood))
	//	{
	//		m_iBlankColor = 2;
	//	}
	//	else
	//	{
	//		m_iBlankColor = 1;
	//	}

	//	return true;
	//}
	//else
	//{
	//	if (m_iBlankColor == 1 || m_iBlankColor == 2)
	//	{
	//		m_iBlankColor = 0;
	//	}
	//}
		
		//右法宝
	//if(m_bHasFaZhen && (iX > 390 && iX < 450 && iY > 150 && iY < 250))
	//{
	//	CParserTip* pTip = g_pControl->GetTipWnd();
	//	int x = m_iScreenX + iX + 10;
	//	int y = m_iScreenY + iY + 10;
	//	pTip->SetShow(false);


	//	//DWORD dwColor = 0xFFFFCF63;
	//	if( SELF.GetEquipGood(ITEM_POS_FABAORIGHT).GetID() != 0)
	//	{
	//		pTip->Clear();
	//		pTip->Parse(SELF.GetEquipGood(ITEM_POS_FABAORIGHT));
	//		pTip->AdjustXY(x,y);
	//		pTip->Move(x,y);
	//		pTip->SetShow(true);

	//	}
	//	else if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() == 0)
	//	{
	//		pTip->Clear();
	//		pTip->AddText("此处可放入辅法宝");
	//		pTip->AdjustXY(x,y);
	//		pTip->Move(x,y);
	//		pTip->SetShow(true);

	//		m_iBlankColor = 0;
	//	}
	//	else if(g_AIGoodMgr.IsFaBao(CGoodGrid::GetDropGoodFrom().DropGood))
	//	{
	//		m_iBlankColor = 4;
	//	}
	//	else
	//	{
	//		m_iBlankColor = 3;
	//	}
	//
	//	return true;
	//}
	//else
	//{
	//	if (m_iBlankColor == 3 || m_iBlankColor == 4)
	//	{
	//		m_iBlankColor = 0;
	//	}
	//}

	//法阵
	//if(m_bHasFaZhen && (iX > 218 && iX < 451 && iY > 269 && iY < 389))
	//{
	//	
	//	CParserTip* pTip = g_pControl->GetTipWnd();
	//	int x = m_iScreenX + iX + 10;
	//	int y = m_iScreenY + iY + 10;
	//	pTip->SetShow(false);

	//	if (SELF.GetYIHUOINFO().dwLevel > 0)
	//	{
	//		YIHUO_INFO& YiHuo = SELF.GetYIHUOINFO();
	//		char temp[50] = {0};
	//		//经验值
	//		LPTexture m_pYihuo = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22972,EP_UI);
	//		if (m_pYihuo && m_pYihuo->IsPointInTex(iX - 221+141,iY - 267+194) > 1)
	//		{
	//			pTip->Clear();
	//			float Angle = (float)((double)YiHuo.i64CurExp / (double)YiHuo.i64CurMaxExp);
	//			sprintf(temp,"%.3f%%",Angle*100.0f);
	//			pTip->AddText(temp);
	//			pTip->AdjustXY(x,y);
	//			pTip->Move(x,y);
	//			pTip->SetShow(true);
	//			return true;
	//		}


	//		pTip->Clear();

	//		sprintf(temp,"%d级法阵",YiHuo.dwLevel);
	//		pTip->AddText(temp);

	//		sprintf(temp,"契合度+%.1f%%",YiHuo.fQiHeDu);
	//		pTip->AddText(temp);

	//		sprintf(temp,"法宝伤害下限+%d",YiHuo.wAttackLower);
	//		pTip->AddText(temp);

	//		sprintf(temp,"法宝伤害上限+%d",YiHuo.wAttackUpper);
	//		pTip->AddText(temp);

	//		sprintf(temp,"命中+%d",YiHuo.wHit);
	//		pTip->AddText(temp);

	//		sprintf(temp,"暴击+%d",YiHuo.wCruelAttack);
	//		pTip->AddText(temp);

	//		sprintf(temp,"破防+%d",YiHuo.wAbsDefend);
	//		pTip->AddText(temp);

	//		sprintf(temp,"金攻加成+%d%%",YiHuo.AttackAddJin);
	//		pTip->AddText(temp);

	//		sprintf(temp,"木攻加成+%d%%",YiHuo.AttackAddMu);
	//		pTip->AddText(temp);

	//		sprintf(temp,"土攻加成+%d%%",YiHuo.AttackAddTu);
	//		pTip->AddText(temp);

	//		sprintf(temp,"水攻加成+%d%%",YiHuo.AttackAddShui);
	//		pTip->AddText(temp);

	//		sprintf(temp,"火攻加成+%d%%",YiHuo.AttackAddHuo);
	//		pTip->AddText(temp);

	//		sprintf(temp,"幸运+%d",YiHuo.wLucy);
	//		pTip->AddText(temp);

	//		pTip->AdjustXY(x,y);
	//		pTip->Move(x,y);
	//		pTip->SetShow(true);
	//	}	

	//	return true;
	//}

	if((iX > 55 && iX < 102 && iY > 85 && iY < 105))
	{
		CParserTip* pTip = g_pControl->GetTipWnd();
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->SetShow(false);


		pTip->Clear();
		pTip->AddText("决定法宝攻击时继承使用者伤害的百分比");
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);

		return true;
	}

	if((iX > 55 && iX < 102 && iY > 272 && iY < 368))
	{
		CParserTip* pTip = g_pControl->GetTipWnd();
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->SetShow(false);


		pTip->Clear();
		pTip->AddText("依比例加成法宝的对应五行伤害");
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);

		return true;
	}
	
	if((iX > 330 && iX < 393 && iY > 439 && iY < 530))
	{
		CParserTip* pTip = g_pControl->GetTipWnd();
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->SetShow(false);


		pTip->Clear();
		pTip->AddText("依数值减少攻击对象的对应五行防御");
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);

		return true;
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CFaBaoShowWnd::OnLeftButtonUp(int iX,int iY)
{
	int itemPos = 0;

	if((iX > 305 && iX < 370 && iY > 135 && iY < 265))
	{
		itemPos = ITEM_POS_FABAO;
	}
	/*else if(m_bHasFaZhen)
	{
		if (iX > 236 && iX < 280 && iY > 150 && iY < 250)
		{
			itemPos = ITEM_POS_FABAOLEFT;
		} 
		else if(iX > 390 && iX < 450 && iY > 150 && iY < 250)
		{
			itemPos = ITEM_POS_FABAORIGHT;
		}
	}*/

	if (itemPos != 0)
	{
		stGoodFrom& sm_stGoodFrom = CGoodGrid::GetDropGoodFrom();
		if (g_pInput->IsShift())
		{
			g_pControl->Msg(MSG_CTRL_INSERT_OBJECTLINK,0,(CControl*)(&SELF.GetEquipGood(itemPos)));
			return true;
		}

		CGood &temp = SELF.GetEquipGood(itemPos);


		//
		if(sm_stGoodFrom.DropGood.GetID() != 0 && g_AIGoodMgr.IsFaBao(sm_stGoodFrom.DropGood))                      //有拖动物品
		{
			if(sm_stGoodFrom.eFromWnd == Package_Wnd)		//包裹中拖过来的物品
			{
				//装备物品音乐，不管成功失败都发声音
				int i = sm_stGoodFrom.DropGood.GetStdMode()+1;
				if(sm_stGoodFrom.DropGood.GetID()!=0)
				{
					g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
				}

				//看其是否是破坏的
				//if(sm_stGoodFrom.DropGood.IsDamaged())
				//{
				// //一切按当初发展，放回包中
				// CGood* pGood = SELF.PackageGood().FindGood(0);
				// if(pGood)
				// {
				//	 *pGood = sm_stGoodFrom.DropGood;
				//	 g_NPC.GetDamageGood() = sm_stGoodFrom.DropGood;
				//	 g_pGameControl->SEND_Exchange_Repair(0,sm_stGoodFrom.DropGood.GetID(),sm_stGoodFrom.DropGood.GetName(),3);
				//	 CGoodGrid::ClearGoodFrom();
				// }
				// return;
				//}

				if (!sm_stGoodFrom.DropGood.IsBind())
				{
					g_MsgBoxMgr.PopOkCancelBox("滴血认主后法宝将会与您绑定, 你确定吗?", MSG_CTRL_RENZHU_FABAO, sm_stGoodFrom.DropGood.GetID());//法宝认主

					//放回包裹
					int pos = sm_stGoodFrom.DropGood.GetPos();
					CGood* pGood = SELF.PackageGood().GetPtr(pos);
					if(pGood && pGood->GetID() == 0)
					{
						pGood->Assign(sm_stGoodFrom.DropGood);
					}
					else
					{
						pGood = SELF.PackageGood().FindGood(0); //找到空位
						if(pGood)
						{
							pGood->Assign(sm_stGoodFrom.DropGood);
						}
					}

					CGoodGrid::ClearGoodFrom();
					//
				}
				else if(SELF.GetEquipTemp().GetID() == 0)
				{
					g_pGameControl->SEND_Request_FaBaoZhuangBei(sm_stGoodFrom.DropGood,itemPos);
					CGoodGrid::ClearGoodFrom();
				}

			}
			else if(sm_stGoodFrom.eFromWnd == Arm_Wnd && sm_stGoodFrom.iWndPos == itemPos)       //装备窗口本身拖出来的物品
			{
				int i = sm_stGoodFrom.DropGood.GetStdMode()+1;
				if(sm_stGoodFrom.DropGood.GetID()!=0)
				{
					g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
				}

				//放回装备品
				SELF.GetEquipGood(itemPos) = sm_stGoodFrom.DropGood;
				CGoodGrid::ClearGoodFrom();            
			}
		}
		else if(sm_stGoodFrom.DropGood.GetID() == 0)                                                   //没有拖动物品
		{
			if(g_pInput->IsShift())
			{

			}
			else
			{
				sm_stGoodFrom.DropGood = temp;
				sm_stGoodFrom.eFromWnd = Arm_Wnd;
				sm_stGoodFrom.iWndPos = itemPos;            
				temp.SetID(0);
				int i = sm_stGoodFrom.DropGood.GetStdMode()+1;
				if(sm_stGoodFrom.DropGood.GetID()!=0)
				{
					g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
				}
			}		
		}
		//
		//RefreshButtonState();
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

//void CFaBaoShowWnd::RefreshButtonState()
//{
//	if (SELF.GetEquipGood(ITEM_POS_FABAORIGHT).GetID() != 0 
//		&& SELF.GetEquipGood(ITEM_POS_FABAOLEFT).GetID() != 0 
//		&& SELF.GetEquipGood(ITEM_POS_FABAO).GetID() != 0)
//	{
//		m_pFaZhenShortKey->ReloadTex(22967,22967,22967,22967);
//	}
//	else
//	{
//		m_pFaZhenShortKey->ReloadTex(22968,22968,22968,22968);
//	}
//}

int CFaBaoShowWnd::GetFaBaoEffType(int FaBaoType)
{
	switch (FaBaoType)
	{
	case 1:
	case 12:
	case 13:
	case 14:
		return 1;//飞剑
	case 2:
		return 2;//双刃
	case 3:
	case 22:
	case 23:
	case 24:
	case 25:
		return 3;//摇铃
	case 4:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
		return 4;//翼杖
	case 5:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		return 5;//圆轮
	case 6:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
		return 6;//月刃
	case 7:
		return 7;//八卦盘
	case 8:
		return 8;//如意
	case 9:
		return 9;//天师符
	case 10:
		return 10;//照妖镜
	case 11:
		return 11;//镇妖塔


	}

	return 1;
}

//int CFaBaoShowWnd::TianDiRen(int FaBaoType)
//{
//	switch (FaBaoType)
//	{
//	case 1:
//	case 5:
//	case 4:
//	case 10:
//		return 1;//天
//	case 2:
//	case 3:
//	case 9:
//	case 7:
//		return 2;//地
//	case 6:
//	case 8:
//	case 11:
//	case 38:
//		return 3;//人
//	}
//
//	return 0;
//}

