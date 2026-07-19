#include "otherplayerequip.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/OtherData.h"
#include "GameData/NpcData.h"
#include "GameData/PetData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "DivinityWingWnd.h"

INIT_WND_POSX(COtherPlayerEquip,POS_VARIABLE,POS_VARIABLE)

#define TABPAGE_ADD_X    22 //加了页签后导致x方向的偏移

COtherPlayerEquip::COtherPlayerEquip(void)
{
	m_bRead = false;
	//装备位置
	//身体
	m_ptEquipmentPos[0].x = 92;
	m_ptEquipmentPos[0].y = 125 + 21;
	//武器
	m_ptEquipmentPos[1].x = 22;
	m_ptEquipmentPos[1].y = 55 + 21;
	//马牌
	m_ptEquipmentPos[2].x = 228 + 20;
	m_ptEquipmentPos[2].y = 194 + 17 + 24; 
	//项链
	m_ptEquipmentPos[3].x = 228 + 20;
	m_ptEquipmentPos[3].y = 152 + 17 + 21;

	//头盔
	m_ptEquipmentPos[4].x = 120;
	m_ptEquipmentPos[4].y = 79 + 21;
	//手镯
	m_ptEquipmentPos[5].x = 228 + 18;
	m_ptEquipmentPos[5].y = 237 + 17 + 21;
	m_ptEquipmentPos[6].x = 26 + 18;
	m_ptEquipmentPos[6].y = 237	 + 17 + 21;
	//戒指
	m_ptEquipmentPos[7].x = 228 + 18;
	m_ptEquipmentPos[7].y = 279 + 17 + 21;
	m_ptEquipmentPos[8].x = 26 + 18;
	m_ptEquipmentPos[8].y = 279 + 17 + 21;
	m_pLogo = NULL;	

	m_ptEquipmentPos[9].x = 26 + 18;
	m_ptEquipmentPos[9].y = 319 + 17 + 21;

	m_ptEquipmentPos[10].x = 94 + 20;
	m_ptEquipmentPos[10].y = 319 + 17 + 21;

	m_ptEquipmentPos[11].x = 161 + 21;
	m_ptEquipmentPos[11].y = 319 + 17 + 21;

	m_ptEquipmentPos[12].x = 228 + 20;
	m_ptEquipmentPos[12].y = 319 + 17 + 21;

	m_ptEquipmentPos[13].x = 145;
	m_ptEquipmentPos[13].y = 150 + 21;

	m_ptEquipmentPos[ITEM_POS_WING].x = 170;
	m_ptEquipmentPos[ITEM_POS_WING].y = 79 + 40;

	//纹佩
	m_ptEquipmentPos[ITEM_POS_WENPEI].x = 28;
	m_ptEquipmentPos[ITEM_POS_WENPEI].y = 199;

	//群英阵谱
	m_ptEquipmentPos[ITEM_TNEUPCHART].x = 228 + 20;
	m_ptEquipmentPos[ITEM_TNEUPCHART].y = 112 + 17 + 21;
	
	m_mask_level = 0;
	m_draw_wing = FALSE;

	m_weapon_looks_frame = 0;
	m_frame_speed = 0;
	m_logo_pos_x = m_logo_pos_y = 0;

	m_logo_pos_x = -1;
	m_logo_pos_y = -4;

	m_byVipCardType = 0;
	m_byVipTradeLevel = 0;



	const char * name = g_OtherData.OtherPlayer().GetName();
	CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter((char*)name);

	if (pChar)
	{
		m_byVipCardType = pChar->GetVipCardType();
		m_byVipTradeLevel = pChar->GetVipTradeLevel();
	}

	//m_iIndex = 20963;
	m_iPages = 1;

	if(!g_pControl->GetWindowPos(m_iOffX,m_iOffY,"OtherPlayerEquip"))
	{
		//m_iAlignType = XAL_TOPLEFT;
		m_iOffX = 50;
		m_iOffY = 50;
	}
	

	m_iVersion = 2;
	AddTabPage(0,0,MAKELONG(22531,PACKAGE_INTERFACE),0,67,85,86,87,88,"人物",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	AddTabPage(0,0,MAKELONG(22531,PACKAGE_INTERFACE),0,152,85,86,87,88,"法宝",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	m_TabPage.iCurPage = 0;
}

COtherPlayerEquip::~COtherPlayerEquip(void)
{
	g_pControl->SetWindowPos(m_iScreenX ,m_iScreenY,"OtherPlayerFaBaoEquip");
}

void COtherPlayerEquip::OnCreate()
{
	CCtrlWindowX::OnCreate();
	//SetCloseButton(255,4, 80);
	SetCloseButton(255 + TABPAGE_ADD_X,4, 80);
}

void COtherPlayerEquip::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	COtherPlayer& otherplayer = g_OtherData.OtherPlayer();

	//绘制行会标志
	int iFlag = otherplayer.GetGuildLogoID();

	if(iFlag > 0 && iFlag < 5)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16149 + iFlag,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X + 202,m_iScreenY + 39,pTex);	       
	}	

	std::string strTitleStr = otherplayer.GetIndividualStr();//个性签名

	if(strTitleStr.size() > 36)
	{
		int i=0;
		while (i < 36 && strTitleStr.c_str()[i] != '\0')
		{
			char* p = CharNext(strTitleStr.c_str()+i);
			if (p <= strTitleStr.c_str()+i)
			{
				break;
			}

			i = (int)(p - strTitleStr.c_str());
		}
		if (i > 0)
		{
			string str = strTitleStr.substr(0,i)+"...";
			g_pFont->DrawText(m_iScreenX + TABPAGE_ADD_X+140-str.size()*3, m_iScreenY+36,str.c_str(),COLOR_TEXT_NORMAL, FONT_YAHEI);
		}
	}
	else if(strTitleStr.size() > 0)
	{
		g_pFont->DrawText(m_iScreenX + TABPAGE_ADD_X+140-strTitleStr.size()*3, m_iScreenY+36,strTitleStr.c_str(),COLOR_TEXT_NORMAL, FONT_YAHEI);
	}

	// 绘制纸娃娃界面

	////绘制魂珠
	//LPTexture pTex = NULL; 	

	//if(!otherplayer.IsYuanShenMale() && nYuanShenInfo < 0xFF && nYuanShenInfo > 0)
	//{
	//	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16437);
	//}
	//else
	//{
	//	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16436);
	//}

	//g_pGfx->SetRenderMode(RM_ADD1);	
	//g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 7 + 21,m_iScreenY - 9 + 190,pTex);
	//g_pGfx->SetRenderMode();

	CGoodGrid::DrawOneGood(m_iScreenX + TABPAGE_ADD_X + 21 + 21,m_iScreenY + 18 + 190,otherplayer.GetEquipGood(ITEM_POS_BALL));

	int iBody = otherplayer.GetLooks().Player.wBody;	
	int iWearGold = otherplayer.GetWearGold();

	int iSex = otherplayer.IsMale()?0:1;

	int iBodyColor = otherplayer.GetLooks().Player.byColor;
	DWORD color = g_OtherData.GetBodyColor(iBodyColor,iSex);

	int iArmID,iCloseID;	
	GetArmCloseIDByPetType(iArmID,iCloseID,otherplayer.GetLooks().Player.wHorse,iSex);

	CGoodGrid::DrawCloth(m_iScreenX + TABPAGE_ADD_X - 150,m_iScreenY - 15 + 21,false,otherplayer.IsOnBigLepoard(),iWearGold,iArmID,
		iCloseID,iBody,iSex,otherplayer.HaveWing(),color,otherplayer.GetLooks().Player.byHairType,otherplayer.GetLooks().Player.byHairColor,
		otherplayer.GetEquipGood(ITEM_POS_CLOTH),otherplayer.GetEquipGood(ITEM_POS_SHIELD),otherplayer.GetEquipGood(ITEM_POS_HELMET),false,
		otherplayer.GetHolyWingLevel(),otherplayer.GetHolyWingStrongLevel());

	//纹佩
	CGoodGrid::DrawWenPei(m_iScreenX + TABPAGE_ADD_X+m_ptEquipmentPos[ITEM_POS_WENPEI].x,m_iScreenY+m_ptEquipmentPos[ITEM_POS_WENPEI].y,otherplayer.GetEquipGood(ITEM_POS_WENPEI));

	//显示装备物品
	for(int i = 1; i < MAX_EQUIPMENT;i++)
	{			

		if(i == ITEM_POS_SHIELD/* && !otherplayer.IsOnBigLepoard()*/)	//骑大豹子的话盾牌在画豹子的地方画//无双子娃娃界面不画豹子
		{	
			if(otherplayer.GetPutOnShield())
			{
				LPTexture pTex = NULL;

				CGood& lShieldGood = otherplayer.GetEquipGood(ITEM_POS_SHIELD);
				if (lShieldGood.GetShape() >= 17 && lShieldGood.GetShape() <= 19)
				{
					//血月虎王盾
					pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1176,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 112, m_iScreenY - 50,pTex);

					pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,1177,EP_UI);
					g_pGfx->SetRenderMode(RM_ADD1);
					g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 112, m_iScreenY - 50,pTex);//特效
					g_pGfx->SetRenderMode();
				}
				else
				{
					pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1164 + ((lShieldGood.GetShape() - 1) / 4 + 1) * 2 + (otherplayer.IsMale()? 0 : 1),EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 112,m_iScreenY - 50,pTex);
				}
			}

			continue;			
		}
		else if(i == ITEM_POS_SHIELD)		//骑大豹子的话盾牌在画豹子的地方画
		{
			continue;
		}

		if(i == ITEM_POS_HELMET)			//头盔和衣服一起画
		{
			continue;
		}
		if(i == ITEM_POS_WING)			//翅膀不画
		{
			continue;
		}
		if (i == ITEM_POS_WENPEI)//纹佩不画
		{
			continue;
		}
		if (i == ITEM_POS_FABAO)//法宝不画
		{
			continue;
		}

		DWORD dwTemp;
		int iPos = (int)SELF.GetReserveData(plyDBClickOtherQuipPos) - 100;
		if(iPos >= 0 && iPos < MAX_EQUIPMENT && iPos == i && iPos != 2)
			dwTemp = 0xFFFF0000;					// 击破选中的颜色
		else
			dwTemp = 0xFFFFFFFF;

		LPTexture pTex = NULL;

		if(otherplayer.GetEquipGood(i).GetID() == 0 )
		{
			continue;	
		}
		else
		{
			pTex = CGoodGrid::GetBasicEquipTex(otherplayer.GetEquipGood(i));
		}

		DWORD looks = otherplayer.GetEquipGood(i).GetLooks();
		if(i == 1)//武器
		{
			if(pTex) 
			{
				g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 150,m_iScreenY - 15 + 21,pTex,dwTemp);
			}
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_items,otherplayer.GetEquipGood(i).GetLooks(),EP_UI);
			//1.907命运石改变过的勋章
			unsigned long ii = otherplayer.GetEquipGood(i).GetFlag();
			char * p =(char *) & ii;
			if( otherplayer.GetEquipGood(i).GetStdMode() == 30 

				&& otherplayer.GetEquipGood(i).GetShape() == 0 )
			{
				if(p[2]>0 && p[2]<6)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,6935+p[2],EP_UI);
				}
				else if(otherplayer.GetEquipGood(i).GetLooks() == 8020)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,8020 + otherplayer.GetEquipGood(i).GetMedalAtt(),EP_UI);
				}
			}
			if( otherplayer.GetEquipGood(i).GetStdMode() == 30 
				&& otherplayer.GetEquipGood(i).GetShape() == 11 )
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,7908,EP_UI);
			}
			if(pTex)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X+m_ptEquipmentPos[i].x-18 ,m_iScreenY+m_ptEquipmentPos[i].y-17,pTex,dwTemp);

				//////////////////////////////////////////////////////////////////////////////////////////////////
				//物品的叠加特效(采用读取配置文件的办法)

				char strTempLooks[32]="";
				sprintf(strTempLooks,"%d",looks);
				if(p[2]>=1 && looks == 214)  //有属性勋章，因为LOOK为荣誉勋章，无法区分，写死
				{
					sprintf(strTempLooks,"%d",6966);
				}
				if( otherplayer.GetEquipGood(i).GetStdMode()==30 && 
					otherplayer.GetEquipGood(i).GetShape()==11)//天下第一元神的灵佩 写死				
					sprintf(strTempLooks,"%d",7909);

				bool bEffect = g_ItemCfgMgr.ParseItemEffect("equip",strTempLooks,m_sItemEffect);
				if(bEffect)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,m_sItemEffect.iTextureID,EP_UI);
					g_pGfx->SetRenderMode(RenderMode(m_sItemEffect.iRenderMode));
					if(pTex)
						g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X+m_ptEquipmentPos[i].x+m_sItemEffect.iOffsetX - 2,m_iScreenY+m_ptEquipmentPos[i].y+m_sItemEffect.iOffsetY - 2,pTex);
					g_pGfx->SetRenderMode();
				}

				///////////////////////////////////////////////////////////////////////////////////////////////////////////

			}
		}	

	}

	int  weaponlooks = 0;
	weaponlooks = otherplayer.GetEquipGood(1).GetLooks();

	if(weaponlooks == 493)
	{
		g_pGfx->SetRenderMode(RM_ADD1);
		LPTexture p = g_pTexMgr->GetTex(PACKAGE_stateitem,5600,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X- 150,m_iScreenY - 15 + 21,p);
		g_pGfx->SetRenderMode();
	}
	else if(weaponlooks == 738 )
	{
		LPTexture  pTexture = NULL;
		g_pGfx->SetRenderMode(RM_ADD1);
		pTexture = g_pTexMgr->GetTex(PACKAGE_magic1,14041+m_weapon_looks_frame,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X- 150,m_iScreenY - 15 + 21,pTexture);
		g_pGfx->SetRenderMode();
		if(m_weapon_looks_frame >= 25)
			m_weapon_looks_frame = 0;
		if(m_frame_speed % 3 == 0)
		{
			m_weapon_looks_frame++;
			if(m_frame_speed > 100)
				m_frame_speed = 0;
		}
		m_frame_speed++;

	}
	else if(weaponlooks == 943 || weaponlooks == 944 )
	{
		g_pGfx->SetRenderMode(RM_ADD1);
		LPTexture p = g_pTexMgr->GetTex(PACKAGE_stateitem,7000 + weaponlooks,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X- 150,m_iScreenY - 15 + 21,p);
		g_pGfx->SetRenderMode();
	}
	else if(weaponlooks >= 1155 && weaponlooks <= 1157)	//夺魄法杖，聚灵扇，无赦刀
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,5865 + weaponlooks,EP_UI);
		if(pTex)
		{
			if(weaponlooks == 1156)
				g_pGfx->SetRenderMode(RM_ADD1);
			else
				g_pGfx->SetRenderMode(RM_ALPHAADD);

			g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 150,GetY() - 15 + 21,pTex);
			g_pGfx->SetRenderMode();
		}
	}	
	else if(weaponlooks == 1165)//霸之屠龙
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,7025,EP_UI);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 150,GetY() - 15 + 21,pTex);
			g_pGfx->SetRenderMode();
		}
	}
	else if(weaponlooks == 1161 || weaponlooks == 1162 || weaponlooks == 1163)//降魔杵
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,5865 + weaponlooks,EP_UI);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X - 150,GetY() - 15 + 21,pTex);
			g_pGfx->SetRenderMode();
		}
	}

	int x = strlen(otherplayer.GetName())*FONTSIZE_BIG/4;
	if (m_byVipCardType > 0)
	{
		LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,17423 + m_byVipCardType,EP_UI);//"虎","龙"
		g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X + 156 - x - 36,m_iScreenY+8,pTex);
	}

	g_pFont->DrawText(m_iScreenX + TABPAGE_ADD_X+141 - x ,m_iScreenY+10,	otherplayer.GetName(),otherplayer.GetNameColor(), FONT_YAHEI, FONTSIZE_BIG);

	if (m_byVipTradeLevel > 0)
	{
		LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,17464 + m_byVipTradeLevel,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X + 141 + x + 3,m_iScreenY + 8,pTex);
	}

	x = strlen(otherplayer.GetTitle())*FONTSIZE_DEFAULT/4;
	char strTemp[100]={0};
	sprintf(strTemp,"%s %s",otherplayer.GetGuildName(),otherplayer.GetTitle());
	x = strlen(strTemp)*FONTSIZE_DEFAULT/4;
	g_pFont->DrawText(m_iScreenX + TABPAGE_ADD_X +140 -x, m_iScreenY + 60,strTemp,0xFF00FF00, FONT_YAHEI);

	if(strlen(otherplayer.GetPhyleName()) != 0)
	{
		g_pFont->DrawText(m_iScreenX + TABPAGE_ADD_X +140, m_iScreenY + 60 + 15,otherplayer.GetPhyleName(),0xFF00FF00, FONT_YAHEI,FONTSIZE_DEFAULT,DTF_Center);
	}

	ShowSmallLock(); // 显示小锁

	//g_pGfx->SetRenderMode(RM_ADD2);
	//DrawTexture(m_ptEquipmentPos[15].x-30, m_ptEquipmentPos[15].y-30, 16437);
	//g_pGfx->SetRenderMode(RM_ALPHA);
}

int COtherPlayerEquip::InGrid(int iX, int iY)
{	
	iX -= 22;
	//装备标示
	if((iX > m_ptEquipmentPos[ITEM_POS_WENPEI].x) && iX < (m_ptEquipmentPos[ITEM_POS_WENPEI].x + 48) &&
		(iY > m_ptEquipmentPos[ITEM_POS_WENPEI].y) && iY < (m_ptEquipmentPos[ITEM_POS_WENPEI].y + 48))
		return ITEM_POS_WENPEI;
	if((iX > m_ptEquipmentPos[15].x - 20) && iX < (m_ptEquipmentPos[15].x + 16) &&
		(iY > m_ptEquipmentPos[15].y - 20) && iY < (m_ptEquipmentPos[15].y + 16))
		return 15;
	if((iX > m_ptEquipmentPos[13].x) && iX < (m_ptEquipmentPos[13].x + 67) &&
		(iY > m_ptEquipmentPos[13].y) && iY < (m_ptEquipmentPos[13].y + 100))
		return 13;
	/*if((iX > m_ptEquipmentPos[14].x) && iX < (m_ptEquipmentPos[14].x + 42) &&
		(iY > m_ptEquipmentPos[14].y) && iY < (m_ptEquipmentPos[14].y + 42))
		return 14;*/
	if((iX > m_ptEquipmentPos[0].x) && iX < (m_ptEquipmentPos[0].x + 75) &&
		(iY > m_ptEquipmentPos[0].y) && iY < (m_ptEquipmentPos[0].y + 143))
		return 0;
	if((iX > m_ptEquipmentPos[1].x) && iX < (m_ptEquipmentPos[1].x + 58) &&
		(iY > m_ptEquipmentPos[1].y) && iY < (m_ptEquipmentPos[1].y + 114))
		return 1;
	if((iX > m_ptEquipmentPos[2].x - 18) && iX < (m_ptEquipmentPos[2].x + 18) &&
		(iY > m_ptEquipmentPos[2].y - 15) && iY < (m_ptEquipmentPos[2].y + 15))
		return 2;
	if((iX > m_ptEquipmentPos[3].x - 18) && iX < (m_ptEquipmentPos[3].x + 18) &&
		(iY > m_ptEquipmentPos[3].y - 15) && iY < (m_ptEquipmentPos[3].y + 15))
		return 3;
	if((iX > m_ptEquipmentPos[4].x) && iX < (m_ptEquipmentPos[4].x + 30) &&
		(iY > m_ptEquipmentPos[4].y) && iY < (m_ptEquipmentPos[4].y + 35))
		return 4;
	if((iX > m_ptEquipmentPos[5].x - 18) && iX < (m_ptEquipmentPos[5].x + 18) &&
		(iY > m_ptEquipmentPos[5].y - 15) && iY < (m_ptEquipmentPos[5].y + 15))
		return 5;
	if((iX > m_ptEquipmentPos[6].x - 18) && iX < (m_ptEquipmentPos[6].x + 18) &&
		(iY > m_ptEquipmentPos[6].y - 15) && iY < (m_ptEquipmentPos[6].y + 15))
		return 6;
	if((iX > m_ptEquipmentPos[7].x - 18) && iX < (m_ptEquipmentPos[7].x + 18) &&
		(iY > m_ptEquipmentPos[7].y - 15) && iY < (m_ptEquipmentPos[7].y + 15))
		return 7;
	if((iX > m_ptEquipmentPos[8].x - 18) && iX < (m_ptEquipmentPos[8].x + 18) &&
		(iY > m_ptEquipmentPos[8].y - 15) && iY < (m_ptEquipmentPos[8].y + 15))
		return 8;
	if((iX > m_ptEquipmentPos[9].x - 18) && iX < (m_ptEquipmentPos[9].x + 18) &&
		(iY > m_ptEquipmentPos[9].y - 15) && iY < (m_ptEquipmentPos[9].y + 15))
		return 9;
	if((iX > m_ptEquipmentPos[10].x - 18) && iX < (m_ptEquipmentPos[10].x + 18) &&
		(iY > m_ptEquipmentPos[10].y - 15) && iY < (m_ptEquipmentPos[10].y + 15))
		return 10;
	if((iX > m_ptEquipmentPos[11].x - 18) && iX < (m_ptEquipmentPos[11].x + 18) &&
		(iY > m_ptEquipmentPos[11].y - 15) && iY < (m_ptEquipmentPos[11].y + 15))
		return 11;
	if((iX > m_ptEquipmentPos[12].x - 18) && iX < (m_ptEquipmentPos[12].x + 18) &&
		(iY > m_ptEquipmentPos[12].y - 15) && iY < (m_ptEquipmentPos[12].y + 15))
		return 12;	
	if((iX > m_ptEquipmentPos[ITEM_POS_WING].x) && iX < (m_ptEquipmentPos[ITEM_POS_WING].x + 60) &&
		(iY > m_ptEquipmentPos[ITEM_POS_WING].y) && iY < (m_ptEquipmentPos[ITEM_POS_WING].y + 120))
		return ITEM_POS_WING;
	return -1;
}

bool COtherPlayerEquip::OnMouseMove(int iX, int iY)
{
	int iPos = InGrid(iX,iY);
	m_mouse_x = iX;
	m_mouse_y = iY;

	COtherPlayer& otherplayer = g_OtherData.OtherPlayer();

	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();

	int iNameX = strlen(otherplayer.GetName())*FONTSIZE_DEFAULT/4;

	if(iX >= 150 - 36 - iNameX && iY >= 8 && iX <= 150 - 36 - iNameX + 20 && iY <= 8 + 20)
	{
		if (m_byVipCardType > 0)
		{
			if (m_byVipCardType == 2)
			{
				pTip->SetText("龙虎阁龙骧金卡成员");
			}
			else if (m_byVipCardType == 1)
			{
				pTip->SetText("龙虎阁虎烈银卡成员");
			}

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}
	else if(otherplayer.IsOnBigLepoard() && iX > 75 && iY > 200 && iX < 225 && iY < 325)
	{
		if(pTip)
		{
			map<DWORD,CGoodPetInfo>::iterator it;
			it = g_PetData.GetGoodPetInfoMap().find(otherplayer.GetID());
			if(it != g_PetData.GetGoodPetInfoMap().end())
			{
				CGood PlayerPet;
				PlayerPet.SetID(otherplayer.GetID());
				PlayerPet.SetStdMode(49);
				PlayerPet.SetShape(51);
				PlayerPet.SetSC((it->second).GetType());
				PlayerPet.SetDC((it->second).GetLevel());

				int x = m_iScreenX + iX + 10;
				int y = m_iScreenY + iY + 10;
				pTip->ParsePetInfo(PlayerPet,true);
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);

			}

		}
		return true;
	}

	if(iPos != -1 )
	{
		CGood& tmp = otherplayer.GetEquipGood(iPos);
		if(tmp.GetID() > 0)
		{
			pTip->Parse(tmp);
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
		}
		else
		{
			pTip->SetShow(false);
		}
		return true;
	}
	else
	{
		pTip->SetShow(false);
	}

	if( iX > 22 && iX  < 249 && iY > 29 && iY < 47)
	{
		string strTitleStr = otherplayer.GetIndividualStr();
		if(!strTitleStr.empty() && strlen(strTitleStr.c_str()) > 0)
		{
			while(strTitleStr.size() > 40 && !strTitleStr.empty() && strlen(strTitleStr.c_str()) > 0)
			{
				int i=0;
				while (i < 40 && strTitleStr.c_str()[i] != '\0')
				{
					char* p = CharNext(strTitleStr.c_str()+i);
					i = (int)(p - strTitleStr.c_str());
				}

				if (i > 0)
				{
					std::string str = strTitleStr.substr(0,i);
					pTip->AddText(str.c_str(),0xFFFFFFFF);
				}

				strTitleStr = strTitleStr.substr(i,strTitleStr.size());
			}

			if(!strTitleStr.empty())
				pTip->AddText(strTitleStr.c_str(),0xFFFFFFFF);

			int x = m_iScreenX + iX- 20;
			int y = m_iScreenY + iY + 5;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;

		}
		else
		{
			pTip->SetShow(false);
		}
	}


	if(otherplayer.GetGuildLogoID() != 0 && iX > 202 && iY > 39 && iX < 202 + 71 && iY < 39 + 83)
	{
		pTip->Clear();

		pTip->AddText(otherplayer.GetGuildName(),0xFFFFFF00);

		char str[128] = "";
		sprintf(str,"行会宝塔等级: %d",otherplayer.GetGuildTowerLevel());
		pTip->AddText(str,0xFFFF0000);

		sprintf(str,"%s属性加成：",otherplayer.GetOfficer());
		pTip->AddText(str,0xFF00FF00,-1);		

		if(!otherplayer.IsGuildTowerSwitch())
		{
			pTip->AddText("行会宝塔资源不足，附加属性被封闭");
		}
		else
		{
			DWORD dwTextColor = -1;

			if(!otherplayer.IsSelfGuildBufferEff())
			{
				pTip->AddText("必须为行会作出贡献才能享受以下属性加成",0xFFFF0000,-1);
				dwTextColor = 0xFF888888;
			}

			CGuildData::_GuildBuffer &buffer = otherplayer.GetGuildBuffer();
			if(buffer.m_wHP != 0)
			{
				sprintf(str,"生命值上限 %d         ",buffer.m_wHP);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_wMP != 0)
			{
				sprintf(str,"魔法值上限 %d         ",buffer.m_wMP);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byAttackFgh[0] + buffer.m_byAttackFgh[1] != 0 && otherplayer.GetCareer() == 0)
			{
				sprintf(str,"攻击 %d-%d",buffer.m_byAttackFgh[0],buffer.m_byAttackFgh[1]);	
				pTip->AddText(str,dwTextColor,-1);
			}
			else if(buffer.m_byAttackRab[0] + buffer.m_byAttackRab[1] != 0 && otherplayer.GetCareer() == 1)
			{
				sprintf(str,"魔法攻击 %d-%d",buffer.m_byAttackRab[0],buffer.m_byAttackRab[1]);	
				pTip->AddText(str,dwTextColor,-1);
			}
			else if(buffer.m_byAttackDao[0] + buffer.m_byAttackDao[1] != 0 && otherplayer.GetCareer() == 2)
			{
				sprintf(str,"道术攻击 %d-%d",buffer.m_byAttackDao[0],buffer.m_byAttackDao[1]);	
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byPhyDef[0] + buffer.m_byPhyDef[1] != 0)
			{
				sprintf(str,"防御 %d-%d",buffer.m_byPhyDef[0],buffer.m_byPhyDef[1]);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byMagicDef[0] + buffer.m_byMagicDef[1] != 0)
			{
				sprintf(str,"魔防 %d-%d",buffer.m_byMagicDef[0],buffer.m_byMagicDef[1]);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byMagicHit != 0)
			{
				sprintf(str,"魔法命中 +%d",buffer.m_byMagicHit);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byPhyHit != 0)
			{
				sprintf(str,"物理命中 +%d",buffer.m_byPhyHit);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byMagicAvoid != 0)
			{
				sprintf(str,"魔法躲避 +%d",buffer.m_byMagicAvoid);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byPhyAvoid != 0)
			{
				sprintf(str,"物理躲避 +%d",buffer.m_byPhyAvoid);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byJinDefend != 0)
			{
				sprintf(str,"金防 %d%%         ",buffer.m_byJinDefend);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byMuDefend != 0)
			{
				sprintf(str,"木防 %d%%      ",buffer.m_byMuDefend);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byTuDefend != 0)
			{
				sprintf(str,"土防 %d%%      ",buffer.m_byTuDefend);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byShuiDefend != 0)
			{
				sprintf(str,"水防 %d%%      ",buffer.m_byShuiDefend);
				pTip->AddText(str,dwTextColor,-1);
			}

			if(buffer.m_byHuoDefend != 0)
			{
				sprintf(str,"火防 %d%%      ",buffer.m_byHuoDefend);
				pTip->AddText(str,dwTextColor,-1);
			}


// 			//神魔buf加成
// 			BYTE byGuildPhyle = otherplayer.GetGuildPhyleState();
// 			if(byGuildPhyle != 0)
// 			{
// 				if(!g_GuildData.IsSelfGuildBufferEff())
// 				{
// 					pTip->AddText("必须为行会作出贡献才能享受以下供奉属性加成",0xFFFF0000,-1);
// 				}
// 				else
// 				{
// 					if(byGuildPhyle == 1)
// 					{
// 						pTip->AddText("供奉光明能享受以下属性加成",0xFF00FF00,-1);
// 					}
// 					else
// 					{
// 						pTip->AddText("供奉黑暗能享受以下属性加成",0xFF00FF00,-1);
// 					}
// 				}
// 
// 				strcpy(str,"");
// 
// 				CGuildData::_GuildBuffer &buffer = otherplayer.GetGuildBuffer();
// 				if(buffer.m_byAttackFgh_Phyle[0] + buffer.m_byAttackFgh_Phyle[1] != 0 && SELF.GetCareer() == 0)
// 				{
// 					sprintf(str,"本体攻击 %d-%d         ",buffer.m_byAttackFgh_Phyle[0],buffer.m_byAttackFgh_Phyle[1]);				
// 				}
// 				else if(buffer.m_byAttackRab_Phyle[0] + buffer.m_byAttackRab_Phyle[1] != 0 && SELF.GetCareer() == 1)
// 				{
// 					sprintf(str,"本体魔法攻击 %d-%d     ",buffer.m_byAttackRab_Phyle[0],buffer.m_byAttackRab_Phyle[1]);				
// 				}
// 				else if(buffer.m_byAttackDao_Phyle[0] + buffer.m_byAttackDao_Phyle[1] != 0 && SELF.GetCareer() == 2)
// 				{
// 					sprintf(str,"本体道术攻击 %d-%d     ",buffer.m_byAttackDao_Phyle[0],buffer.m_byAttackDao_Phyle[1]);				
// 				}
// 
// 				if(strlen(str) != 0)
// 				{
// 					pTip->AddText(str,dwTextColor,-1);
// 				}
// 
// 				if(buffer.m_byPhyDef_Phyle[0] + buffer.m_byPhyDef_Phyle[1] != 0)
// 				{
// 					sprintf(str,"本体防御 %d-%d         ",buffer.m_byPhyDef_Phyle[0],buffer.m_byPhyDef_Phyle[1]);
// 					pTip->AddText(str,dwTextColor,-1);
// 				}
// 
// 
// 				if(buffer.m_byMagicDef_Phyle[0] + buffer.m_byMagicDef_Phyle[1] != 0)
// 				{
// 					sprintf(str,"本体魔防 %d-%d         ",buffer.m_byMagicDef_Phyle[0],buffer.m_byMagicDef_Phyle[1]);
// 					pTip->AddText(str,dwTextColor,-1);
// 				}
// 				if(buffer.m_byMagicHit_Phyle != 0)
// 				{
// 					sprintf(str,"本体魔法命中 +%d      ",buffer.m_byMagicHit_Phyle);
// 					pTip->AddText(str,dwTextColor,-1);
// 				}
// 				if(buffer.m_byPhyHit_Phyle != 0)
// 				{
// 					sprintf(str,"本体物理命中 +%d      ",buffer.m_byPhyHit_Phyle);
// 					pTip->AddText(str,dwTextColor,-1);
// 				}
// 				if(buffer.m_byMagicAvoid_Phyle != 0)
// 				{
// 					sprintf(str,"本体魔法躲避 +%d      ",buffer.m_byMagicAvoid_Phyle);
// 					pTip->AddText(str,dwTextColor,-1);
// 				}
// 				if(buffer.m_byPhyAvoid_Phyle != 0)
// 				{
// 					sprintf(str,"本体物理躲避 +%d      ",buffer.m_byPhyAvoid_Phyle);
// 					pTip->AddText(str,dwTextColor,-1);
// 				}
// 
// 			}
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool COtherPlayerEquip::OnLeftButtonDClick(int iX, int iY)
{
	char cTemp = InGrid(iX,iY);
	if(cTemp != -1 && cTemp != 2 && SELF.GetReserveData(plyDBClickOtherQuipPos) >= 99)
	{
		SELF.SetReserveData(plyDBClickOtherQuipPos,cTemp+100);
		g_pGameControl->SEND_Player_Attack_Position(cTemp);		
	}
	return true;
}

bool COtherPlayerEquip::OnLeftButtonUp(int iX, int iY)
{
	int iPos = InGrid(iX,iY);
	if(iPos != -1 )
	{
		if(g_pInput->IsShift())  //按物品
		{
			CGood& temp = g_OtherData.OtherPlayer().GetEquipGood(iPos);
			g_pControl->Msg(MSG_CTRL_INSERT_OBJECTLINK, 0,(CControl*)(&temp));

			return true;
		}
		return true;
	}

	char strTemp[100]={0};
	sprintf(strTemp,"%s %s",g_OtherData.OtherPlayer().GetGuildName(),g_OtherData.OtherPlayer().GetTitle());
	int x = strlen(strTemp)*FONTSIZE_DEFAULT;
	if(iX >= 123 - x/4 && iX <= 123 - x/4 + x && iY >= 46 && iY <= 62)
	{
		g_pControl->Msg(MSG_CTRL_INSERT_TEXT,2,(CControl *)(g_OtherData.OtherPlayer().GetGuildName()));
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}


//显示小锁
void COtherPlayerEquip::ShowSmallLock()
{
	LPTexture pTexAuth = g_pTexMgr->GetTex(PACKAGE_stateitem,6921,EP_UI);

	for(int i = 0; i < MAX_EQUIPMENT; i++)
	{
		CGood& temp = g_OtherData.OtherPlayer().GetEquipGood(i);

		if(temp.GetID() > 0)
		{
			if (!g_AIGoodMgr.IsCanLock(temp))
			{
				continue;
			}
			if(temp.GetFlag2() > 0)
			{
				if(pTexAuth)
				{
					g_pGfx->SetRenderMode(RM_ADD1);
					if(i==0)
					{
						if (temp.GetID() == g_NPC.GetSuoPackGoodID())
						{
							g_NPC.SetSuoPackGoodID(0);
						}
						g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X + m_ptEquipmentPos[i].x+ 120,m_iScreenY + m_ptEquipmentPos[i].y+180,pTexAuth);
						g_pGfx->SetRenderMode();
					}
					else if(i==1)
					{
						if (temp.GetID() == g_NPC.GetSuoPackGoodID())
						{
							g_NPC.SetSuoPackGoodID(0);
						}
						g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X + m_ptEquipmentPos[i].x+ 40,m_iScreenY + m_ptEquipmentPos[i].y+140,pTexAuth);
						g_pGfx->SetRenderMode();
					}
					else if(i==4)
					{
						if (temp.GetID() == g_NPC.GetSuoPackGoodID())
						{
							g_NPC.SetSuoPackGoodID(0);
						}
						g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X + m_ptEquipmentPos[i].x+ 36,m_iScreenY + m_ptEquipmentPos[i].y-6,pTexAuth);
						g_pGfx->SetRenderMode();
					}
					else
					{
						//小锁
						g_pGfx->DrawTextureNL(m_iScreenX + TABPAGE_ADD_X + m_ptEquipmentPos[i].x+ 12,m_iScreenY + m_ptEquipmentPos[i].y+10,pTexAuth);
						g_pGfx->SetRenderMode();
					}
				}
			}
		}
	}
}

bool COtherPlayerEquip::OnRightButtonDown(int iX, int iY)
{
	COtherPlayer& otherplayer = g_OtherData.OtherPlayer();

	char strTemp[100]={0};
	sprintf(strTemp,"%s %s",otherplayer.GetGuildName(),otherplayer.GetTitle());
	int iLen = strlen(strTemp);

	int iLeft = m_iScreenX + TABPAGE_ADD_X + 123 - iLen*FONTSIZE_DEFAULT/4;
	int iRight = iLeft + iLen*FONTSIZE_DEFAULT/2;
	int iTop = m_iScreenY + 46;
	int iBottom = iTop + FONTSIZE_DEFAULT/2;

	if(g_pInput->IsAlt() && iX >= iLeft && iX <= iRight && iY >= iTop && iY <= iBottom)
	{
		string strTemp = otherplayer.GetGuildName();
		g_pControl->Msg(MSG_CTRL_INSERT_TEXT,1,(CControl*)strTemp.c_str());
		return true;
	}

	return CCtrlWindowX::OnRightButtonDown(iX,iY);
}

bool COtherPlayerEquip::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if (pControl)
			{
				S_TabPage * pParentTabpage = (S_TabPage * )pControl;
				if (pParentTabpage == &m_TabPage && dwData == 1)//点了纸娃娃页签
				{
					CloseWindow();
					g_pControl->PopupWindow(MSG_CTRL_OTHERPLAYER_FABAOSHOW_WND,OPER_CREATE);
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
