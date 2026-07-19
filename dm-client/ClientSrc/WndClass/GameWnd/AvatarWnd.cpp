#include "AvatarWnd.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/UiManager.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameControl/GameControl.h"
#include "GameData/ItemCfgMgr.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/OtherData.h"
#include "GameData/ConfigData.h"
#include "GameData/LoginData.h"
#include "Global/Interface/AudioInterface.h"
#include "Global/ColorModel.h"
#include "GameData/PetData.h"
#include "GameData/GoodPet.h"
#include "GameData/TaskData.h"
#include "GameData/GuildData.h"
#include "DivinityWingWnd.h"



CStateViewer::CStateViewer()
{
	//m_iShowDemonState = 1;

	//m_pWhite = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,476);
	//m_pYellow = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,477);
	//m_pBrown = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,478);
	//m_pRed = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,479); 

	m_dwWhiteID = MAKELONG(476,PACKAGE_INTERFACE);
	m_dwBrownID = MAKELONG(477,PACKAGE_INTERFACE);
	m_dwYellowID = MAKELONG(478,PACKAGE_INTERFACE);
	m_dwRedID = MAKELONG(479,PACKAGE_INTERFACE);



	//m_pStateEx1 = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,17311);
	//m_pStateEx2 = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,17310);
	//m_pStateEx3 = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,17315);
	//m_pStateEx4 = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,17312);

	//m_pLowDarkExp = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10258);//黑暗经验条下限
	//m_pHighDarkExp = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10259);//黑暗经验条上限

	//m_rcHighDarkExp.left = 0;
	//m_rcHighDarkExp.top = 0;
	//m_rcHighDarkExp.bottom = m_pHighDarkExp->GetHeight();

	//m_rcLowDarkExp.left = 0;
	//m_rcLowDarkExp.top = 0;
	//m_rcLowDarkExp.bottom = m_pLowDarkExp->GetHeight();
	m_bPickRiderClothes = false;
	m_iHighLightWuXing = 0;

	m_iFrame = 0;
}

CStateViewer::~CStateViewer()
{
	//g_pTexMgr->ReleaseTex(m_pWhite);
	//g_pTexMgr->ReleaseTex(m_pYellow);
	//g_pTexMgr->ReleaseTex(m_pBrown);
	//g_pTexMgr->ReleaseTex(m_pRed); 
	//g_pTexMgr->ReleaseTex(m_pStateEx1);
	//g_pTexMgr->ReleaseTex(m_pStateEx2);
	//g_pTexMgr->ReleaseTex(m_pStateEx3);
	//g_pTexMgr->ReleaseTex(m_pStateEx4);
	//g_pTexMgr->ReleaseTex(m_pLowDarkExp);
	//g_pTexMgr->ReleaseTex(m_pHighDarkExp);
}

//void CStateViewer::SetShowDemonState(int iState)
//{
//	m_iShowDemonState = iState;
//}

//int CStateViewer::GetShowDemonState()
//{
//	return m_iShowDemonState;
//}

void CStateViewer::OnDraw()
{
	//绘制打开装备栏时的特效
	int iCareer = SELF.GetCareer();
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,850 + iCareer,EP_UI);

	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X - 121,m_iScreenY + 33,pTex);
	}

	if(m_iFrame < 36) 
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,853 + iCareer,EP_UI);
		if(pTex)
		{
			pTex->EnableSysAnim(false);
			pTex->SetCurFrame(m_iFrame / 4);
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X - 111,m_iScreenY + 33,pTex);
			g_pGfx->SetRenderMode();
		}

		m_iFrame++;
	}

	DrawBackground();
}

void CStateViewer::PrintStateInfoPair(int iX,int iY,char* szFormat,DWORD dwArg1,DWORD dwArg2,DWORD dwColor)
{
	char temp[50] = {0};
	sprintf(temp,szFormat,dwArg1,dwArg2);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + iX,m_iScreenY + iY,temp,dwColor,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
}

void CStateViewer::PrintStateInfoSingle(int iX,int iY,char* szFormat,DWORD dwArg,DWORD dwColor)
{
	char temp[50] = {0};
	sprintf(temp,szFormat,dwArg);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + iX,m_iScreenY + iY,temp,dwColor,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
}

//void CStateViewer::ShowExpChart(int iX, int iY, DWORD dwLowDarkExp, DWORD dwHighDarkExp)
//{
//	float fScale1,fScale2;
//	fScale1 = CalRate(dwLowDarkExp%100,100);
//	fScale2 = CalRate(dwHighDarkExp%100,100);    
//
//	m_rcLowDarkExp.right = (int)(m_pLowDarkExp->GetWidth() * fScale1);
//	g_pGfx->DrawTextureFX(m_iScreenX  + NEWWND_DELTA_X + iX,m_iScreenY + iY + 9,m_pLowDarkExp,-1,&m_rcLowDarkExp);
//
//	m_rcHighDarkExp.right = (int)(m_pHighDarkExp->GetWidth() * fScale2);    
//	g_pGfx->DrawTextureFX(m_iScreenX + NEWWND_DELTA_X + iX,m_iScreenY + iY,m_pHighDarkExp,-1,&m_rcHighDarkExp);
//}

void CAvatarWnd::ShowPKState()
{
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 175,m_iScreenY + 410,"P K 状态",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	unsigned char cColor = SELF.GetNameColor();

	DWORD dwID = 0;

	if(cColor == 0xFF || cColor ==0xFD)//白色
	{
		dwID = m_dwWhiteID;
	}
	else if(cColor == 0x97) //黄色 
	{
		dwID = m_dwBrownID;
	}
	else if(cColor == 0x2F)//褐色
	{
		dwID = m_dwYellowID;
	}
	else if(cColor == 0x38)//红名
	{
		dwID = m_dwRedID;
	}


	LPTexture pTex = g_pTexMgr->GetTexFromID(dwID,EP_UI);
	if (pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 227 ,m_iScreenY + 408,pTex);
	}


}

void CAvatarWnd::ShowPlayerInfo()
{
	//玩家名字
	const char* szPlayerName = SELF.GetName();
	int iNameX = g_pFont->GetLen(szPlayerName,0,FONTSIZE_BIG) / 2;
	BYTE dwNameColor = SELF.GetNameColor();

	if (SELF.GetVipCardType() > 0 && SELF.GetVipCardType() <= 2)
	{
		LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,17423 + SELF.GetVipCardType(),EP_UI);//"虎","龙"
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 234 - iNameX - 22,m_iScreenY + 7,pTex);
	}
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 234 - iNameX,m_iScreenY + 11,szPlayerName,g_pGameData->GetMirColor(dwNameColor),FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

	if (SELF.GetVipTradeLevel() > 0)
	{
		LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,17464 + SELF.GetVipTradeLevel(),EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 234 + iNameX + 3,m_iScreenY + 9,pTex);
	}

	char strTemp[100] ={0};

	//数字帐号
	int iOffsetY = 0;
	if(g_Login.IsValidPoptang_str())
	{
		const char* str = g_Login.GetPoptang_str();
		sprintf(strTemp,"您的数字帐号是：%s",str);
		int iTileX = g_pFont->GetLen(strTemp) / 2;
		g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 300 - iTileX,m_iScreenY + 65,strTemp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iOffsetY = 15;
	}

	//行会名字
	sprintf(strTemp,"%s %s",SELF.GetTitle(),SELF.GetGuildTitle());
	int iTileNameX = g_pFont->GetLen(strTemp) / 2;
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 300 - iTileNameX,m_iScreenY + 65 + iOffsetY,strTemp,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);


	if(strcmp(g_OtherData.GetPhyleName().c_str(),"") != 0)
	{
		if(strcmp(g_OtherData.GetPhyleMaster().c_str(),SELF.GetName()) == 0)
		{
			sprintf(strTemp,"%s  宗族宗主",g_OtherData.GetPhyleName().c_str());
		}
		else
		{
			sprintf(strTemp,"%s  宗族弟子",g_OtherData.GetPhyleName().c_str());
		}
		g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 300,m_iScreenY + 80 + iOffsetY,strTemp,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame | DTF_Center);
	}
	//会旗
	/*int iFlag = g_GuildData.GetGuildFlag();
	if(iFlag > 0 && iFlag < 5)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16149 + iFlag);
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 52 - 1,m_iScreenY + 409 + 33,pTex);	
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16194);
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 55 - 1,m_iScreenY + 405 + 33,pTex);	
	}
	else*/
	{
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 16 - 1,m_iScreenY + 408 + 30,g_pTexMgr->GetTexFromID(m_dwLogoID,EP_UI)); 		
	} 

	//显示个性化签名
	std::string strTitleStr = g_Config.GetTitleStr();
	int offset = 220;
	if(strTitleStr.length() > 60)
	{
		StringUtil::AutoCut(strTitleStr,56);
		strTitleStr += "...";
	}
	else if(strTitleStr.size() > 50)
	{
		StringUtil::AutoCut(strTitleStr,46);
		strTitleStr += "...";
	}

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 10 + offset, m_iScreenY + 37,strTitleStr.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame | DTF_Center);
}

void CAvatarWnd::ShowCharAttr()
{
	//if(!m_pStateEx1)         //缺少图片
	//{
	//	return;
	//}

	//g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X,m_iScreenY,m_pStateEx1);

	DWORD dwExpColor;
	DWORD dwPackageWeightColor;
	DWORD dwWeightColor;
	DWORD dwWristColor;
	DWORD dwEnergyColor;


	if(SELF.GetExp() >= SELF.GetLevelUpExp())
	{
		dwExpColor = 0xFFFF0000;
	}
	else
	{
		dwExpColor = 0xFFFFFFFF;
	}

	if(SELF.GetPackageWeight() >= SELF.GetPackageWeightMax())
	{
		dwPackageWeightColor = 0xFFFF0000;
	}
	else
	{
		dwPackageWeightColor = 0xFFFFFFFF;        
	}

	if(SELF.GetWeight() >= SELF.GetWeightMax())
	{
		dwWeightColor = 0xFFFF0000;    
	}
	else
	{
		dwWeightColor = 0xFFFFFFFF;  
	}

	if(SELF.GetWrist() >= SELF.GetWristMax())
	{        
		dwWristColor = 0xFFFF0000; 
	}
	else
	{
		dwWristColor = 0xFFFFFFFF; 
	}

	if(SELF.GetEnergy() <= 0)
	{
		SELF.SetEnergy(0);
	}
	if(SELF.GetEnergyMax() <= 0)
	{
		SELF.SetEnergyMax(0);
	}
	if(SELF.GetEnergyEnable())
	{        
		dwEnergyColor = 0xFF00FF00; 
	}
	else
	{
		dwEnergyColor = 0xFFFFFFFF;  
	}

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 65,m_iScreenY + 68,"基本属性",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 88,"经验值",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	//g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 109,"灵  力",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	//g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 109,"背包重量",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 109,"负重量",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 130,"腕  力",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 151,"精力值",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 172,"功勋值",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 65,m_iScreenY + 196,"敏捷属性",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 216,"命  中",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 237,"躲  避",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 258,"魔法命中",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 279,"魔法躲避",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	//g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 279,"中毒躲避",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	//g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 0,"移  动",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	//g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 303,"中毒命中",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 65,m_iScreenY + 302,"特殊属性",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 322,"暴  击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 343,"反弹伤害",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 364,"吸  血",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 385,"抵  抗",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 26,m_iScreenY + 406,"破  防",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	
	
	//经验值的计算
	char temp[50] = {0};
	if(SELF.GetLevelUpExp() != 0)
	{ 
		if(SELF.GetExp() > SELF.GetLevelUpExp())
		{            
			sprintf(temp,"100.00%%");            
		}
		else
		{
			sprintf(temp,"%2.2f%%",(double)SELF.GetExp() /SELF.GetLevelUpExp() * 100);
		}
	}
	else
	{
		sprintf(temp,"0.00%%");
	} 
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 90 - 10,m_iScreenY + 88,temp,dwExpColor,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);


	////灵力
	//int iLingli = SELF.GetLingLi();
	//DWORD dwColor_Lingli = 0xFFFFFFFF;

	//if(iLingli >= 0 && iLingli <= 999)	//红
	//	dwColor_Lingli = 0xFFFF0000;
	//else								//紫
	//	dwColor_Lingli = 0xFFFF00FF;

	//PrintStateInfoSingle(91 - 10,88 + 5,"%d",iLingli,dwColor_Lingli);
	
	////背包重量
	//PrintStateInfoPair(80,109,"%d/%d",SELF.GetPackageWeight(),SELF.GetPackageWeightMax(),dwPackageWeightColor);

	//负重量
	PrintStateInfoPair(80,109,"%d/%d",SELF.GetWeight(),SELF.GetWeightMax(),dwWeightColor);

	//腕力
	PrintStateInfoPair(80,130,"%d/%d",SELF.GetWrist(),SELF.GetWristMax(),dwWristColor); 

	//精力值
	PrintStateInfoSingle(80,151,"%d",SELF.GetJingLi(),0xFFFFFFFF);

	//功勋值
	PrintStateInfoSingle(80,172,"%d",SELF.GetMeritoriousnessValue(),0xFFFFFFFF);


	
	// 命中
	PrintStateInfoSingle(80,216,"+%d",SELF.GetPrecision(),0xFFA5E0D7);

	//躲避
	PrintStateInfoSingle(80,237,"+%d",SELF.GetSmartness(),0xFFA5E0D7);

	//魔法命中
	PrintStateInfoSingle(80,258,"+%d%%",SELF.GetMagicRate(),0xFFA5E0D7);
	
	//魔法躲避
	PrintStateInfoSingle(80,279,"+%d%%",SELF.GetMagicDef(),0xFFA5E0D7);

	////中毒躲避
	//PrintStateInfoSingle(80,226 + 5,"+%d%%",SELF.GetPoisonDef());
	////移动
	//int iHp = SELF.GetHP();
	//if( SELF.GetPackageWeight() > SELF.GetPackageWeightMax() || iHp < 10 )
	//{
	//	PrintStateInfoSingle(80,249 + 5,"%d",0);
	//}
	//else 
	//{        
	//	PrintStateInfoSingle(80,249 + 5,"%d",(iHp - 10 ) / 16 + 1);
	//}  

	////中毒命中
	//PrintStateInfoSingle(80,295 + 5,"+%d%%",SELF.GetPoisonRate());


	// 活力
	//PrintStateInfoPair(80,353,"%d/%d",SELF.GetEnergy(),SELF.GetEnergyMax(),dwEnergyColor);

	//极品修炼值
	//PrintStateInfoSingle(80,377,"%u",SELF.GetBestItemValue());    


	// 暴击
	PrintStateInfoSingle(80,322,"+%d",SELF.GetCruelAttack(),0xFFF3D279);

	//反弹伤害
	PrintStateInfoSingle(80,343,"+%d",SELF.GetReboundHurt(),0xFFF3D279);

	// 吸血
	PrintStateInfoSingle(80,364,"+%d",SELF.GetAbsordBlood(),0xFFF3D279);

	//绝对防御
	PrintStateInfoSingle(80,385,"+%d",SELF.GetAbsDefend(),0xFFF3D279);

	//破防
	PrintStateInfoSingle(80,406,"+%d",SELF.GetDestroyDefend(),0xFFF3D279);
}

void CStateViewer::ShowCharState()
{
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 182,m_iScreenY + 439,"生命值",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 317,m_iScreenY + 439,"魔法值",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 182,m_iScreenY + 457,"攻击力",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 317,m_iScreenY + 457,"防御力",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 182,m_iScreenY + 475,"道术攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 317,m_iScreenY + 475,"魔防力",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 182,m_iScreenY + 493,"魔法攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 317,m_iScreenY + 493,"声望",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 182,m_iScreenY + 511,"幸运",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 317,m_iScreenY + 511,"诅咒",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	//生命值
	PrintStateInfoPair(240,438,"%4d/%d",SELF.GetHP(),SELF.GetHPMax());

	//魔法值
	PrintStateInfoPair(374,438,"%4d/%d",SELF.GetMP(),SELF.GetMPMax());

	//攻击力
	PrintStateInfoPair(240,456,"%4d-%d",SELF.GetDC(),SELF.GetDC2());

	//防御力
	PrintStateInfoPair(374,456,"%4d-%d",SELF.GetAC(),SELF.GetAC2());

	//道术攻击
	PrintStateInfoPair(240,474,"%4d-%d",SELF.GetSC(),SELF.GetSC2());

	//魔防力
	PrintStateInfoPair(374,474,"%4d-%d",SELF.GetMAC(),SELF.GetMAC2());

	//魔法攻击
	PrintStateInfoPair(240,492,"%4d-%d",SELF.GetMC(),SELF.GetMC2());

	//声望
	PrintStateInfoSingle(374,492,"%6d",SELF.GetFame());    

	//幸运
	PrintStateInfoSingle(240,510,"%5d",(int)SELF.GetLuckyValue());

	//诅咒
	PrintStateInfoSingle(374,510,"%6d",(int)SELF.GetCurseValue());

}

INIT_WND_POSX(CAvatarWnd,POS_AUTO,POS_AUTO)

CAvatarWnd::CAvatarWnd(void)
:CStateViewer()
{
	g_pControl->PopupWindow(MSG_CTRL_FABAOSHOW_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_QISHOU_WND,OPER_CLOSE);
	m_sName = "AvatarWnd";
	
	int iType = 0;
	//if(SELF.GetLevel() < 22)
	//	iType  = 1;

	m_pXmlWindow = g_pUiMgr->GetXmlWindow(m_sName,iType);

	//if(SELF.GetLevel() < 22)
	//	m_iIndex = 15163;
	//else
	//m_iIndex = 15164;
	//m_iIndex = 15150;

	//m_pSinglePlayerMaskTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,14715);

	m_bDClicked = false; 
	m_iButtonFrame = 0;
	m_ptButton.x = 404;
	m_ptButton.y = 155;

	m_bRead = false;
	m_iPlayEffect = 0;
	m_bSwitch = false; 
	m_iEffectFrameCount = 0;


	//m_pLogo = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,481);
	//m_pLogoBac = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,490);  
	//m_pWuXingTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,15165);
	//m_pGuildLogo = NULL;

	m_dwLogoID = MAKELONG(23205,PACKAGE_INTERFACE);
	m_dwWuXingTexID = MAKELONG(15165,PACKAGE_INTERFACE);

	m_iEffectCurFrame = 0;	

	m_iAlignType = XAL_TOPLEFT;
	if(!g_pControl->GetWindowPos(m_iOffX,m_iOffY,"AvatarWnd"))
	{
		m_iOffX = (g_pGfx->GetWidth() - 545)/2;
		m_iOffY = (g_pGfx->GetHeight() - 442)/2;
	}

	m_iVersion = 2;

	AddTabPage(0,0,MAKELONG(15151,PACKAGE_INTERFACE),10,67,85,86,87,88,"人物",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	AddTabPage(0,0,MAKELONG(15151,PACKAGE_INTERFACE),10,152,85,86,87,88,"法宝",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	if (g_OtherData.GetHorseManShipInfo().Grade > 0)
	{
		AddTabPage(0,0,MAKELONG(15151,PACKAGE_INTERFACE),10,237,85,86,87,88,"骑术",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	}
	m_TabPage.iCurPage = 0;
}

void CAvatarWnd::OnCreate()
{  
	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_FLASH_BTN,1005);//停止按钮闪烁

	m_pShowWuXing = new CCtrlButton();
	AddControl(m_pShowWuXing);
	m_pShowWuXing->CreateEx(this,452  + NEWWND_DELTA_X,214,10706,10707,10708);

	SetCloseButton(434  + NEWWND_DELTA_X,4,80);
	CCtrlWindowX::OnCreate();


	if(!g_pGameData->HasPaoPaoStatus(EP_First_WeigthFull_PaoPao) && SELF.GetWeight() >= SELF.GetWeightMax())
	{
		AddArrowTip(EP_First_WeigthFull_PaoPao,81  + NEWWND_DELTA_X,109 + 8,XAL_TOPLEFT,false,XAL_TOPRIGHT);
	}

	if(!g_pGameData->HasPaoPaoStatus(EP_First_WanLiFull_PaoPao) && SELF.GetWrist() >= SELF.GetWristMax())
	{
		AddArrowTip(EP_First_WanLiFull_PaoPao,81  + NEWWND_DELTA_X,130 + 8,XAL_TOPLEFT,false,XAL_TOPRIGHT);
	}
}

CAvatarWnd::~CAvatarWnd(void)
{    
	//g_pTexMgr->ReleaseTex(m_pLogo);
	//g_pTexMgr->ReleaseTex(m_pLogoBac);   
	//g_pTexMgr->ReleaseTex(m_pWuXingTex);
	//g_pTexMgr->ReleaseTex(m_pGuildLogo);

	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"FaBaoShowWnd");
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"QiShouWnd");
}

void CAvatarWnd::Draw()
{    
	CStateViewer::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//g_pGfx->SetRenderMode(RM_ADD2);
	//DrawTexture(392, 211, 16437);
	//g_pGfx->SetRenderMode(RM_ALPHA);

	if(m_iPlayEffect)
	{
		PlayEffect(m_iPlayEffect);
	}

	ShowPlayerInfo();

	if(g_pGameData->GetShowWuXing())
	{		
		g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 465,m_iScreenY + 5,g_pTexMgr->GetTexFromID(m_dwWuXingTexID,EP_UI));

		ShowWuXingState();
	}	
}

void CStateViewer::ShowWuXingState()
{
	//if(!pTex)       //缺少图片
	//{
	//	return;
	//}
	//g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X,m_iScreenY,pTex);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 230,"金属性攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 254,"木属性攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 278,"土属性攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 302,"水属性攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 326,"火属性攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 374,"金属性防御",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 398,"木属性防御",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 422,"土属性防御",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 446,"水属性防御",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + NEWWND_DELTA_X + 495,m_iScreenY + 470,"火属性防御",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);


	DWORD dwColorRed = 0xffff0000;
	DWORD dwColorGreed = 0xff00ff00;

	//伤害
	PrintStateInfoPair(98 + 474,83 + 147,"%u-%u",SELF.GetJinAttack_Low(),SELF.GetJinAttack(),SELF.GetJinAttack() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoPair(98 + 474,83 + 147 + 24,"%u-%u",SELF.GetMuAttack_Low(),SELF.GetMuAttack(),SELF.GetMuAttack() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoPair(98 + 474,83 + 147 + 2 * 24,"%u-%u",SELF.GetTuAttack_Low(),SELF.GetTuAttack(),SELF.GetTuAttack() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoPair(98 + 474,83 + 147 + 3 * 24,"%u-%u",SELF.GetShuiAttack_Low(),SELF.GetShuiAttack(),SELF.GetShuiAttack() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoPair(98 + 474,83 + 147 + 4 * 24,"%u-%u",SELF.GetHuoAttack_Low(),SELF.GetHuoAttack(),SELF.GetHuoAttack() > 0 ? dwColorGreed : dwColorRed);

	//防
	PrintStateInfoSingle(98 + 474, 244 + 130,"+%u%%",SELF.GetJinDef(),SELF.GetJinDef() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoSingle(98 + 474, 244 + 130 + 24,"+%u%%",SELF.GetMuDef(),SELF.GetMuDef() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoSingle(98 + 474, 244 + 130 + 2 * 24,"+%u%%",SELF.GetTuDef(),SELF.GetTuDef() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoSingle(98 + 474, 244 + 130 + 3 * 24,"+%u%%",SELF.GetShuiDef(),SELF.GetShuiDef() > 0 ? dwColorGreed : dwColorRed);
	PrintStateInfoSingle(98 + 474, 244 + 130 + 4 * 24,"+%u%%",SELF.GetHuoDef(),SELF.GetHuoDef() > 0 ? dwColorGreed : dwColorRed);

	LPTexture pTex = NULL;
	int ix = 0,iy = 0;
	g_pControl->GetMouseXY(ix,iy);
	ix -= m_iScreenX - NEWWND_DELTA_X;
	iy -= m_iScreenY;

	if(SELF.GetJinAttack() > 0 || SELF.GetJinDef() > 0)
	{
		if(m_iHighLightWuXing == 1)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20942,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 551,m_iScreenY + 55,pTex);
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20943,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 551,m_iScreenY + 55,pTex);			
		}
	}

	if(SELF.GetMuAttack() > 0 || SELF.GetMuDef() > 0)
	{
		if(m_iHighLightWuXing == 2)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20944,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 610,m_iScreenY + 98,pTex);
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20945,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 610,m_iScreenY + 98,pTex);			
		}
	}

	if(SELF.GetTuAttack() > 0 || SELF.GetTuDef() > 0)
	{
		if(m_iHighLightWuXing == 3)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20948,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 492,m_iScreenY + 98,pTex);
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20949,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 492,m_iScreenY + 98,pTex);			
		}
	}

	if(SELF.GetShuiAttack() > 0 || SELF.GetShuiDef() > 0)
	{
		if(m_iHighLightWuXing == 4)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20946,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 586,m_iScreenY + 167,pTex);
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20947,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 586,m_iScreenY + 167,pTex);			
		}
	}

	if(SELF.GetHuoAttack() > 0 || SELF.GetHuoDef() > 0)
	{
		if(m_iHighLightWuXing == 5)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20940,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 514,m_iScreenY + 167,pTex);
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20941,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X + 514,m_iScreenY + 167,pTex);			
		}
	}
}

void CAvatarWnd::OnDraw()
{

	ShowCharAttr();



	ShowCharState();
	ShowPKState();	

	CStateViewer::OnDraw();	
}

bool CAvatarWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_EQUIPMENT_EFFECT:
		if(dwData == 1)
		{
			m_iPlayEffect = 9;
			m_iEffectCurFrame = 0;
		}
		else if(dwData == 2)
		{
			m_bSwitch = true;
		}
		else if(dwData == 3)
		{
			m_iPlayEffect = 10;
			m_iEffectCurFrame = 0;
		}

		break;
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pShowWuXing)
		{
			g_pGameData->SetShowWuXing(!g_pGameData->GetShowWuXing());			

			if(g_pGameData->GetShowWuXing())
			{
				m_pShowWuXing->ReloadTex(10709,10710,10711);
			}
			else
			{
				m_pShowWuXing->ReloadTex(10706,10707,10708);
			}

			g_pControl->Msg(MSG_INPUT_MOVE,MAKELONG(g_pControl->GetMouseX(),g_pControl->GetMouseY()));

			return true;
		}
		break;
	case MSG_INPUT_MOVE:
		{

			int x = (short)(LOWORD( dwData )) - m_iScreenX - NEWWND_DELTA_X;
			int y = (short)(HIWORD( dwData )) - m_iScreenY;	

			//if(SELF.IsOnBigLepoard() && x > 210 && y > 210 && x < 395 && y < 320)
			//{
			//	CParserTip *pTip = g_pControl->GetTipWnd();
			//	if(pTip)
			//	{
			//		PET_SENDOUT_MAP& petMap = g_PetData.GetSendOutPetList();
			//		PET_SENDOUT_MAP::iterator petItr = petMap.begin();
			//		for(; petItr != petMap.end();petItr++)
			//		{
			//			if((petItr->second).szReserved[0] == 6 || (petItr->second).szReserved[0] == 2)	//==6本体骑战
			//			{
			//				break;
			//			}
			//		}
			//		CGood * pGood = NULL;
			//		if(petItr != petMap.end())
			//		{
			//			pGood = SELF.PackageGood().FindGood((petItr->second).dwPetItemID);
			//		}

			//		if(pGood)
			//		{
			//			x = m_iScreenX + NEWWND_DELTA_X + x + 10;
			//			y = m_iScreenY + y + 10;
			//			pTip->Clear();
			//			pTip->ParsePetInfo(*pGood);
			//			pTip->AdjustXY(x,y);
			//			pTip->Move(x,y);
			//			pTip->SetShow(true);
			//		}

			//	}
			//	return true;
			//}			
			
			if(x > 452 && y > 214 && x < 452 + 12 && y < 214 + 46)
			{
				CParserTip *pTip = g_pControl->GetTipWnd();
				if(pTip)
				{
					pTip->Clear();

					x = m_iScreenX + NEWWND_DELTA_X + x + 10;
					y = m_iScreenY + y + 10;
					
					if(g_pGameData->GetShowWuXing())
					{
						pTip->AddText("单击收回页面。");
					}
					else
					{
						pTip->AddText("单击展开页面，查看您的五行属性。");
					}

					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
				}

				return true;
			}
		}
		break;
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if (pControl)
			{
				S_TabPage * pParentTabpage = (S_TabPage * )pControl;
				if (pParentTabpage == &m_TabPage && dwData == 1)//点了法宝页签
				{
					CloseWindow();
					g_pControl->PopupWindow(MSG_CTRL_FABAOSHOW_WND,OPER_CREATE);
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
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CStateViewer::DrawBackground()
{      

}

bool CAvatarWnd::OnLeftButtonDown(int iX, int iY)
{
	/*if(iX > 19 && iX < 159 && iY > 33 && iY < 55)
	{
	if(iX < 50 )
	{
	SetShowDemonState(1);
	}
	else if(iX < 86)
	{
	SetShowDemonState(0);
	}
	else if(iX < 123)
	{
	SetShowDemonState(2);
	}
	else if(iX < 161)
	{
	SetShowDemonState(3);
	}
	}  */

	return CCtrlWindowS::OnLeftButtonDown(iX,iY);    
}

bool CAvatarWnd::OnMouseMove(int iX, int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();

	pTip->Clear();

	const char* szPlayerName = SELF.GetName();
	int iNameX = g_pFont->GetLen(szPlayerName) / 2;

	if(iX >= 250  + NEWWND_DELTA_X - 44 - iNameX && iY >= 7 && iX <= 250  + NEWWND_DELTA_X - 44 - iNameX + 20 && iY <= 7 + 20)
	{
		if (SELF.GetVipCardType() > 0 && SELF.GetVipCardType() <= 2)
		{
			if (SELF.GetVipCardType() == 2)
			{
				pTip->SetText("龙虎阁龙骧金卡成员");
			}
			else if (SELF.GetVipCardType() == 1)
			{
				pTip->SetText("龙虎阁虎烈银卡成员");
			}

			int x = m_iScreenX + NEWWND_DELTA_X + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}	
	

	//if(g_GuildData.GetGuildFlag() != 0 && iX > 52 && iY > 409 && iX < 52 + 71 && iY < 409 + 83)
	//{
	//	pTip->Clear();

	//	pTip->AddText(g_GuildData.GetGuildName(),0xFFFFFF00);

	//	char str[128] = "";
    //  sprintf(str,"行会宝塔等级: %d",g_GuildData.GetGuildTowerLevel());
	//	pTip->AddText(str,0xFFFF0000);

	//	const char * title = SELF.GetGuildOfficer();
	//	if(title != NULL)
	//	{
	//		sprintf(str,"%s属性加成：",title);
	//	}
	//	else
	//	{
	//		strcpy(str,"行会成员属性加成：");
	//	}

	//	pTip->AddText(str,0xFF00FF00,-1);

	//	if(!g_GuildData.IsGuildBufferEff())
	//	{
	//		pTip->AddText("行会宝塔资源不足，附加属性被封闭");
	//	}
	//	else
	//	{
	//		DWORD dwTextColor = -1;

	//		if(!g_GuildData.IsSelfGuildBufferEff())
	//		{
	//			pTip->AddText("必须为行会作出贡献才能享受以下属性加成",0xFFFF0000);
	//			dwTextColor = 0xFF888888;
	//		}

	//		strcpy(str,"");

	//		CGuildData::_GuildBuffer &buffer = g_GuildData.GetGuildBuffer();
	//		if(buffer.m_byAttackFgh[0] + buffer.m_byAttackFgh[1] != 0 && SELF.GetCareer() == 0)
	//		{
	//			sprintf(str,"本体攻击 %d-%d         ",buffer.m_byAttackFgh[0],buffer.m_byAttackFgh[1]);				
	//		}
	//		else if(buffer.m_byAttackRab[0] + buffer.m_byAttackRab[1] != 0 && SELF.GetCareer() == 1)
	//		{
	//			sprintf(str,"本体魔法攻击 %d-%d     ",buffer.m_byAttackRab[0],buffer.m_byAttackRab[1]);				
	//		}
	//		else if(buffer.m_byAttackDao[0] + buffer.m_byAttackDao[1] != 0 && SELF.GetCareer() == 2)
	//		{
	//			sprintf(str,"本体道术攻击 %d-%d     ",buffer.m_byAttackDao[0],buffer.m_byAttackDao[1]);				
	//		}
	//		/*
	//		if(g_pMerChar && g_pMerChar->GetID() != 0)
	//		{			
	//		if(buffer.m_byMAttackFgh[0] + buffer.m_byMAttackFgh[1] != 0 && MCHAR.GetCareer() == 0)
	//		{
	//		sprintf(str,"%s元神攻击 %d-%d",str,buffer.m_byMAttackFgh[0],buffer.m_byMAttackFgh[1]);					
	//		}
	//		else if(buffer.m_byMAttackRab[0] + buffer.m_byMAttackRab[1] != 0 && MCHAR.GetCareer() == 1)
	//		{
	//		sprintf(str,"%s元神魔法攻击 %d-%d",str,buffer.m_byMAttackRab[0],buffer.m_byMAttackRab[1]);					
	//		}
	//		else if(buffer.m_byMAttackDao[0] + buffer.m_byMAttackDao[1] != 0 && MCHAR.GetCareer() == 2)
	//		{
	//		sprintf(str,"%s元神道术攻击 %d-%d",str,buffer.m_byMAttackDao[0],buffer.m_byMAttackDao[1]);					
	//		}	
	//		}
	//		*/
	//		if(strlen(str) != 0)
	//		{
	//			pTip->AddText(str,dwTextColor,-1);	
	//		}
	//		/*
	//		if(buffer.m_byPhyDef[0] + buffer.m_byPhyDef[1] != 0)
	//		{
	//		sprintf(str,"本体防御 %d-%d         ",buffer.m_byPhyDef[0],buffer.m_byPhyDef[1]);

	//		if(g_pMerChar && g_pMerChar->GetID() != 0)
	//		{
	//		sprintf(str,"%s元神防御 %d-%d",str,buffer.m_byMPhyDef[0],buffer.m_byMPhyDef[1]);					
	//		}

	//		pTip->AddText(str,dwTextColor,-1);
	//		}

	//		*/
	//		if(buffer.m_byMagicDef[0] + buffer.m_byMagicDef[1] != 0)
	//		{
	//			sprintf(str,"本体魔防 %d-%d         ",buffer.m_byMagicDef[0],buffer.m_byMagicDef[1]);

	//			// 				if(g_pMerChar && g_pMerChar->GetID() != 0)
	//			// 				{
	//			// 					sprintf(str,"%s元神魔防 %d-%d",str,buffer.m_byMMagicDef[0],buffer.m_byMMagicDef[1]);									
	//			// 				}

	//			pTip->AddText(str,dwTextColor,-1);
	//		}

	//		if(buffer.m_byMagicHit != 0)
	//		{
	//			sprintf(str,"本体魔法命中 +%d      ",buffer.m_byMagicHit);

	//			// 				if(g_pMerChar && g_pMerChar->GetID() != 0)
	//			// 				{
	//			// 					sprintf(str,"%s元神魔法命中 +%d",str,buffer.m_byMMagicHit);
	//			// 				}

	//			pTip->AddText(str,dwTextColor,-1);
	//		}

	//		if(buffer.m_byPhyHit != 0)
	//		{
	//			sprintf(str,"本体物理命中 +%d      ",buffer.m_byPhyHit);

	//			// 				if(g_pMerChar && g_pMerChar->GetID() != 0)
	//			// 				{
	//			// 					sprintf(str,"%s元神物理命中 +%d",str,buffer.m_byMPhyHit);
	//			// 				}

	//			pTip->AddText(str,dwTextColor,-1);
	//		}

	//		if(buffer.m_byMagicAvoid != 0)
	//		{
	//			sprintf(str,"本体魔法躲避 +%d      ",buffer.m_byMagicAvoid);

	//			// 				if(g_pMerChar && g_pMerChar->GetID() != 0)
	//			// 				{
	//			// 					sprintf(str,"%s元神魔法躲避 +%d",str,buffer.m_byMMagicAvoid);
	//			// 				}

	//			pTip->AddText(str,dwTextColor,-1);
	//		}

	//		if(buffer.m_byPhyAvoid != 0)
	//		{
	//			sprintf(str,"本体物理躲避 +%d      ",buffer.m_byPhyAvoid);

	//			// 				if(g_pMerChar && g_pMerChar->GetID() != 0)
	//			// 				{
	//			// 					sprintf(str,"%s元神物理躲避 +%d",str,buffer.m_byMPhyAvoid);
	//			// 				}

	//			pTip->AddText(str,dwTextColor,-1);
	//		}


	//		//神魔buf加成
	//		BYTE byGuildPhyle = g_GuildData.GetGuildPhyleState();
	//		if(byGuildPhyle != 0)
	//		{
	//			if(!g_GuildData.IsSelfGuildBufferEff())
	//			{
	//				pTip->AddText("必须为行会作出贡献才能享受以下供奉属性加成",0xFFFF0000,-1);
	//			}
	//			else
	//			{
	//				if(byGuildPhyle == 1)
	//				{
	//					pTip->AddText("供奉光明能享受以下属性加成",0xFF00FF00,-1);
	//				}
	//				else
	//				{
	//					pTip->AddText("供奉黑暗能享受以下属性加成",0xFF00FF00,-1);
	//				}
	//			}

	//			strcpy(str,"");

	//			CGuildData::_GuildBuffer &buffer = g_GuildData.GetGuildBuffer();
	//			if(buffer.m_byAttackFgh_Phyle[0] + buffer.m_byAttackFgh_Phyle[1] != 0 && SELF.GetCareer() == 0)
	//			{
	//				sprintf(str,"本体攻击 %d-%d         ",buffer.m_byAttackFgh_Phyle[0],buffer.m_byAttackFgh_Phyle[1]);				
	//			}
	//			else if(buffer.m_byAttackRab_Phyle[0] + buffer.m_byAttackRab_Phyle[1] != 0 && SELF.GetCareer() == 1)
	//			{
	//				sprintf(str,"本体魔法攻击 %d-%d     ",buffer.m_byAttackRab_Phyle[0],buffer.m_byAttackRab_Phyle[1]);				
	//			}
	//			else if(buffer.m_byAttackDao_Phyle[0] + buffer.m_byAttackDao_Phyle[1] != 0 && SELF.GetCareer() == 2)
	//			{
	//				sprintf(str,"本体道术攻击 %d-%d     ",buffer.m_byAttackDao_Phyle[0],buffer.m_byAttackDao_Phyle[1]);				
	//			}

	//			// 				if(g_pMerChar && g_pMerChar->GetID() != 0)
	//			// 				{			
	//			// 					if(buffer.m_byMAttackFgh_Phyle[0] + buffer.m_byMAttackFgh_Phyle[1] != 0 && MCHAR.GetCareer() == 0)
	//			// 					{
	//			// 						sprintf(str,"%s元神攻击 %d-%d",str,buffer.m_byMAttackFgh_Phyle[0],buffer.m_byMAttackFgh_Phyle[1]);					
	//			// 					}
	//			// 					else if(buffer.m_byMAttackRab_Phyle[0] + buffer.m_byMAttackRab_Phyle[1] != 0 && MCHAR.GetCareer() == 1)
	//			// 					{
	//			// 						sprintf(str,"%s元神魔法攻击 %d-%d",str,buffer.m_byMAttackRab_Phyle[0],buffer.m_byMAttackRab_Phyle[1]);					
	//			// 					}
	//			// 					else if(buffer.m_byMAttackDao_Phyle[0] + buffer.m_byMAttackDao_Phyle[1] != 0 && MCHAR.GetCareer() == 2)
	//			// 					{
	//			// 						sprintf(str,"%s元神道术攻击 %d-%d",str,buffer.m_byMAttackDao_Phyle[0],buffer.m_byMAttackDao_Phyle[1]);					
	//			// 					}	
	//			// 				}

	//			if(strlen(str) != 0)
	//			{
	//				pTip->AddText(str,dwTextColor,-1);
	//			}

	//			if(buffer.m_byPhyDef_Phyle[0] + buffer.m_byPhyDef_Phyle[1] != 0)
	//			{
	//				sprintf(str,"本体防御 %d-%d         ",buffer.m_byPhyDef_Phyle[0],buffer.m_byPhyDef_Phyle[1]);

	//				// 					if(g_pMerChar && g_pMerChar->GetID() != 0)
	//				// 					{
	//				// 						sprintf(str,"%s元神防御 %d-%d",str,buffer.m_byMPhyDef_Phyle[0],buffer.m_byMPhyDef_Phyle[1]);					
	//				// 					}

	//				pTip->AddText(str,dwTextColor,-1);
	//			}


	//			if(buffer.m_byMagicDef_Phyle[0] + buffer.m_byMagicDef_Phyle[1] != 0)
	//			{
	//				///////////////////////////////////////////////////////////////
	//				sprintf(str,"本体魔防 %d-%d         ",buffer.m_byMagicDef_Phyle[0],buffer.m_byMagicDef_Phyle[1]);
	//				////////////////

	//				// 					if(g_pMerChar && g_pMerChar->GetID() != 0)
	//				// 					{
	//				// 						sprintf(str,"%s元神魔防 %d-%d",str,buffer.m_byMMagicDef_Phyle[0],buffer.m_byMMagicDef_Phyle[1]);									
	//				// 					}

	//				pTip->AddText(str,dwTextColor,-1);
	//			}

	//			if(buffer.m_byMagicHit_Phyle != 0)
	//			{
	//				sprintf(str,"本体魔法命中 +%d      ",buffer.m_byMagicHit_Phyle);

	//				// 					if(g_pMerChar && g_pMerChar->GetID() != 0)
	//				// 					{
	//				// 						sprintf(str,"%s元神魔法命中 +%d",str,buffer.m_byMMagicHit_Phyle);
	//				// 					}

	//				pTip->AddText(str,dwTextColor,-1);
	//			}

	//			if(buffer.m_byPhyHit_Phyle != 0)
	//			{
	//				sprintf(str,"本体物理命中 +%d      ",buffer.m_byPhyHit_Phyle);

	//				// 					if(g_pMerChar && g_pMerChar->GetID() != 0)
	//				// 					{
	//				// 						sprintf(str,"%s元神物理命中 +%d",str,buffer.m_byMPhyHit_Phyle);
	//				// 					}

	//				pTip->AddText(str,dwTextColor,-1);
	//			}

	//			if(buffer.m_byMagicAvoid_Phyle != 0)
	//			{
	//				sprintf(str,"本体魔法躲避 +%d      ",buffer.m_byMagicAvoid_Phyle);

	//				// 					if(g_pMerChar && g_pMerChar->GetID() != 0)
	//				// 					{
	//				// 						sprintf(str,"%s元神魔法躲避 +%d",str,buffer.m_byMMagicAvoid_Phyle);
	//				// 					}

	//				pTip->AddText(str,dwTextColor,-1);
	//			}

	//			if(buffer.m_byPhyAvoid_Phyle != 0)
	//			{
	//				sprintf(str,"本体物理躲避 +%d      ",buffer.m_byPhyAvoid_Phyle);

	//				// 					if(g_pMerChar && g_pMerChar->GetID() != 0)
	//				// 					{
	//				// 						sprintf(str,"%s元神物理躲避 +%d",str,buffer.m_byMPhyAvoid_Phyle);
	//				// 					}

	//				pTip->AddText(str,dwTextColor,-1);
	//			}					
	//		}
	//	}

	//	int x = m_iScreenX + NEWWND_DELTA_X + iX + 10;
	//	int y = m_iScreenY + iY + 10;
	//	pTip->AdjustXY(x,y);
	//	pTip->Move(x,y);
	//	pTip->SetShow(true);
	//	return true;
	//}
	//else 
	if(g_pGameData->GetShowWuXing())
	{

		m_iHighLightWuXing = 0;

		if(iX > 551  + NEWWND_DELTA_X && iY > 55 && iX < 551  + NEWWND_DELTA_X + 27 && iY < 55 + 27)
		{
			pTip->AddText("金：",0xFFFFFF00);
			pTip->AddText("金生水，克木，受克于火。",-1,-1);
			pTip->AddText("1点金属性攻击=1点普通",-1,-1);
			pTip->AddText("攻击。");
			pTip->AddText("PVE：金属性攻击对木属性",0xFFFFCF63,-1);
			pTip->AddText("怪物有额外伤害加成",0xFFFFCF63);
			pTip->AddText("PVP：金属性攻击只能被金",0xFFFFCF63,-1);
			pTip->AddText("属性防御所防御。",0xFFFFCF63);

			m_iHighLightWuXing = 1;
		}
		else if(iX > 610  + NEWWND_DELTA_X && iY > 98 && iX < 610  + NEWWND_DELTA_X + 27 && iY < 98 + 27)
		{
			pTip->AddText("木：",0xFF00FF00);
			pTip->AddText("木生火，克土，受克于金。",-1,-1);
			pTip->AddText("1点木属性攻击=1点普通",-1,-1);
			pTip->AddText("攻击。");
			pTip->AddText("PVE：木属性攻击对土属性",0xFFFFCF63,-1);
			pTip->AddText("怪物有额外伤害加成",0xFFFFCF63);
			pTip->AddText("PVP：木属性攻击只能被木",0xFFFFCF63,-1);
			pTip->AddText("属性防御所防御。",0xFFFFCF63);

			m_iHighLightWuXing = 2;
		}
		else if(iX > 492  + NEWWND_DELTA_X && iY > 98 && iX < 492  + NEWWND_DELTA_X + 27 && iY < 98 + 27)
		{
			pTip->AddText("土：",0xFFFFCF63);
			pTip->AddText("土生金，克水，受克于木。",-1,-1);
			pTip->AddText("1点土属性攻击=1点普通",-1,-1);
			pTip->AddText("攻击。");
			pTip->AddText("PVE：土属性攻击对水属性",0xFFFFCF63,-1);
			pTip->AddText("怪物有额外伤害加成",0xFFFFCF63);
			pTip->AddText("PVP：土属性攻击只能被土",0xFFFFCF63,-1);
			pTip->AddText("属性防御所防御。",0xFFFFCF63);

			m_iHighLightWuXing = 3;
		}
		else if(iX > 586  + NEWWND_DELTA_X && iY > 167 && iX < 586  + NEWWND_DELTA_X + 27 && iY < 167 + 27)
		{
			pTip->AddText("水：",0xff0084ff);
			pTip->AddText("水生木，克火，受克于土。",-1,-1);
			pTip->AddText("1点水属性攻击=1点普通",-1,-1);
			pTip->AddText("攻击。");
			pTip->AddText("PVE：水属性攻击对火属性",0xFFFFCF63,-1);
			pTip->AddText("怪物有额外伤害加成",0xFFFFCF63);
			pTip->AddText("PVP：水属性攻击只能被水",0xFFFFCF63,-1);
			pTip->AddText("属性防御所防御。",0xFFFFCF63);

			m_iHighLightWuXing = 4;
		}
		else if(iX > 514  + NEWWND_DELTA_X && iY > 167 && iX < 514  + NEWWND_DELTA_X + 27 && iY < 167 + 27)
		{
			pTip->AddText("火：",0xFFFF0000);
			pTip->AddText("火生土，克金，受克于水。",-1,-1);
			pTip->AddText("1点火属性攻击=1点普通攻",-1,-1);
			pTip->AddText("攻击。");
			pTip->AddText("PVE：火属性攻击对金属性",0xFFFFCF63,-1);
			pTip->AddText("怪物有额外伤害加成",0xFFFFCF63);
			pTip->AddText("PVP：火属性攻击只能被火",0xFFFFCF63,-1);
			pTip->AddText("属性防御所防御。",0xFFFFCF63);

			m_iHighLightWuXing = 5;
		}

		if(m_iHighLightWuXing != 0)
		{
			int x = m_iScreenX + NEWWND_DELTA_X + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);	
			pTip->SetShow(true);
			return true;
		}
		else
		{
			pTip->Hide();
		}
	}

/*	if (iX >= 77 && iX <= 77 + 70 && iY >= 170 && iY <= 172 + 16)
	{
		char szTemp[128] = {0};
		sprintf(szTemp,"当前功勋等级：%u",SELF.GetMeritoriousnessLevel());
		pTip->AddText(szTemp,0xFFFFFF00,-1);
		sprintf(szTemp,"当前等级兑换功勋值比例：1:%u",SELF.GetMeritoriousnessAddSpeed());
		pTip->AddText(szTemp,0xFFFFFF00,-1);
		sprintf(szTemp,"当前共累积群英经验值：%u",SELF.GetMeritoriousnessExp());
		pTip->AddText(szTemp,0xFFFFFF00,-1);

		int x = m_iScreenX + NEWWND_DELTA_X + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);	
		pTip->SetShow(true);
		return true;
	}
	else */if (iX >= 77  + NEWWND_DELTA_X && iX <= 77  + NEWWND_DELTA_X + 70 && iY >= 320 && iY <= 320 + 100)
	{
		bool bShowTips = true;
		if (iY >= 320 && iY < 320 + 16)
		{
			pTip->AddText("攻击时有几率造成1.5倍伤害；暴击值越高触发几率越高",0xFFFFFF00);
		}
		else if (iY >= 342 && iY < 342 + 16)
		{
			pTip->AddText("自身受到伤害的同时可对攻击方造成伤害(包括远程、群体攻击)；反弹伤害值越高对攻击方造成的伤害越高",0xFFFFFF00);
		}
		else if (iY >= 362 && iY < 362 + 16)
		{
			pTip->AddText("攻击目标并造成伤害时可为自身恢复生命值；群体技能可多次触发吸血效果",0xFFFFFF00);
		}
		else if (iY >= 382 && iY < 382 + 16)
		{
			pTip->AddText("被攻击时有几率完全不受伤害；抵抗值越高触发几率越高",0xFFFFFF00);
		}
		else if (iY >= 404 && iY < 404 + 16)
		{
			pTip->AddText("攻击时有几率无视对方物理防御、魔法防御、五行防御、金刚护体、魔法盾、斗转星移，直接造成伤害；破防值越高，触发几率越高",0xFFFFFF00);
		}
		else 
		{
			bShowTips = false;
		}

		if (bShowTips)
		{
			int x = m_iScreenX + NEWWND_DELTA_X + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);	
			pTip->SetShow(true);
			return true;
		}
	}
	else
	{
		if(pTip)
			pTip->Hide();
	}
	return CCtrlWindowS::OnMouseMove(iX,iY);
}

bool CAvatarWnd::OnLeftButtonUp(int iX, int iY)
{
	if(m_bDClicked)
	{
		m_bDClicked = false;        
	}

	//行会名字
	char strTemp[100] ={0};
	sprintf(strTemp,"%s %s",SELF.GetTitle(),SELF.GetGuildTitle());
	int iTileNameX = g_pFont->GetLen(strTemp);
	int iOffsetY = 0;
	if(g_Login.IsValidPoptang_str())
		iOffsetY = 14;

	if(iX >= 292  + NEWWND_DELTA_X - iTileNameX/2 && iX <= 292  + NEWWND_DELTA_X + iTileNameX/2 && iY >= 50 + iOffsetY && iY <= 50 + iOffsetY + 12)
	{
		g_pControl->Msg(MSG_CTRL_INSERT_TEXT,2,(CControl *)(SELF.GetTitle()));
		return true;
	}


	return CCtrlWindowS::OnLeftButtonUp(iX,iY);
}

CGood& CAvatarWnd::GetCharacterEquipment(int i)
{
	return SELF.EquipGood().Get(i);
}

__int64 CAvatarWnd::GetCharacterLooks()
{
	return SELF.GetLooksValue();
}

bool CAvatarWnd::IsCharacterGirl()
{
	return !SELF.IsMale();
}

int CAvatarWnd::GetCharacterMaskLevel()
{
	return SELF.GetMaskLevel();
}

void CAvatarWnd::PlayEffect(DWORD dwID)
{
	if(dwID == 9 || dwID == 10)
	{
		//播放圣灵宝石特效
		if(m_iEffectCurFrame == 0)
		{
			g_pTexMgr->PreLoad(PACKAGE_magic2,2820,2848,EP_MAGIC);
		}

		LPTexture pTexAuth = g_pTexMgr->GetTex(PACKAGE_magic2,2820 + m_iEffectCurFrame,EP_MAGIC);
		if(pTexAuth)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			if(m_iEffectCurFrame < 19)
			{
				if(dwID == 9)
				{
					g_pGfx->DrawTextureNL(m_iScreenX + 80 + NEWWND_DELTA_X, m_iScreenY + 50, pTexAuth);
				}
				else if(dwID == 10)
				{
					g_pGfx->DrawTextureNL(m_iScreenX + 175 + NEWWND_DELTA_X, m_iScreenY + 70, pTexAuth);
				}
			}
			else 
			{
				g_pGfx->DrawTextureNL(m_iScreenX + 168 + NEWWND_DELTA_X, m_iScreenY + 77, pTexAuth);
			}
			g_pGfx->SetRenderMode();
		}				

		static int iEffFrameCounter_Top = 0;
		iEffFrameCounter_Top++;
		if(iEffFrameCounter_Top == 6)
		{
			iEffFrameCounter_Top = 0;
			m_iEffectCurFrame++;
			if(m_iEffectCurFrame > 29)
			{
				m_iPlayEffect = 0;
				m_iEffectCurFrame = 0;
			}
		}
	}

}


bool CAvatarWnd::IsInControl( int iX,int iY )
{
	if(g_pGameData->GetShowWuXing())
	{
		LPTexture pTex = g_pTexMgr->GetTexFromID(m_dwWuXingTexID,EP_UI);
		if(pTex && pTex->IsPointInTex(iX - 465 - NEWWND_DELTA_X,iY - 5) == 2)
		{
			return true;
		}

		return CCtrlWindowX::IsInControl(iX,iY);
	}
	else
	{
		return CCtrlWindowX::IsInControl(iX,iY);
	}
}
//void CStateViewer::ShowHairEquip()
//{
//	DWORD hair = (DWORD)(((GetCharacterLooks() & 0x000F0000) >> 32)  + 400);
//
//	if(GetCharacterEquipment(4).GetID() != 0)
//		hair += 32;
//
//	DWORD haircolor = (DWORD)((GetCharacterLooks() & 0x00F00000) >> 36);
//	LPTexture pTex1 = g_pTexMgr->GetTex(PACKAGE_stateitem,(WORD)hair);
//	if(pTex1 == NULL)
//		return;
//
//	DWORD dwHairColor = g_OtherData.GetHairColor(haircolor);
//	g_pGfx->DrawTextureNL(GetX(),GetY(),pTex1,dwHairColor);
//
//}

//void CStateViewer::ShowProduct()
//{
//	if(!m_pStateEx4)         //缺少图片
//	{
//		return;
//	}		
//
//	g_pGfx->DrawTextureNL(m_iScreenX + NEWWND_DELTA_X,m_iScreenY,m_pStateEx4);
//
//	PrintStateInfoPair(79,79,"%5d/%d",SELF.GetJingLi(),SELF.GetJingLiMax());
//
//	float fPercent = 0.0f;
//	if(SELF.GetJingLiMax() == 0)
//	{
//		fPercent = 0.0f;
//	}
//	else
//	{
//		fPercent = float(SELF.GetJingLi()) / SELF.GetJingLiMax();
//	}
//
//	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17313);
//	g_pGfx->DrawPartTexture(m_iScreenX + NEWWND_DELTA_X + 31,m_iScreenY + 104,pTex,0,0,(int)(113 * fPercent),-1);
//
//	//PrintStateInfoPair(79,165,"%5d/%d",SELF.GetLianHuaValue(),SELF.GetLianHuaMax());
//
//	//if(SELF.GetLianHuaMax() == 0)
//	//{
//	//	fPercent = 0.0f;
//	//}
//	//else
//	//{
//	//	fPercent = float(SELF.GetLianHuaValue()) / SELF.GetLianHuaMax();
//	//}
//
//	//pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17314);
//	//g_pGfx->DrawPartTexture(m_iScreenX + NEWWND_DELTA_X + 31,m_iScreenY + 191,pTex,0,0,(int)(113 * fPercent),-1);
//
//
//	//PrintStateInfoSingle(113,145,"%d",SELF.GetLianHuaLevel());
//}

//
//bool CAvatarWnd::OnLeftButtonDClick(int iX,int iY)
//{
//	if (iX >= 321 && iY >= 107 && iX <= 380 && iY <= 275)
//	{
//		g_pControl->PopupWindow(MSG_CTRL_DIVINITYWING_WND,OPER_CREATE);
//	}
//
//	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
//}
