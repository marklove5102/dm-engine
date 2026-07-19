#include "OtherPlayerFaBaoEquip.h"
#include "GameData/OtherData.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"
#include "FaBaoShowWnd.h"


INIT_WND_POSX(COtherPlayerFaBaoEquip,POS_AUTO,POS_AUTO)

COtherPlayerFaBaoEquip::COtherPlayerFaBaoEquip(void)
{

	m_iVersion = 2;
	AddTabPage(0,0,MAKELONG(22530,PACKAGE_INTERFACE),0,67,85,86,87,88,"人物",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	AddTabPage(0,0,MAKELONG(22530,PACKAGE_INTERFACE),0,152,85,86,87,88,"法宝",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);

	m_TabPage.iCurPage = 1;
}

COtherPlayerFaBaoEquip::~COtherPlayerFaBaoEquip(void)
{
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"OtherPlayerEquip");
}

void COtherPlayerFaBaoEquip::OnCreate()
{
	CCtrlWindowX::OnCreate();
	SetCloseButton(255 + 22,4, 80);
}

void COtherPlayerFaBaoEquip::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 138,m_iScreenY + 9,"圣殿法宝",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

	char temp[64] = {0};

	//sprintf(temp,"%d级法阵",g_OtherData.OtherPlayer().GetFaZhenLevel());
	//g_pFont->DrawText(m_iScreenX+ 142,m_iScreenY + 58,temp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	g_pGfx->SetRenderMode(RM_ADD2);
	LPTexture m_pDiwen = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22979,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 8,m_iScreenY + 199,m_pDiwen);
	g_pGfx->SetRenderMode();
	LPTexture CharacterPicture = NULL;
	LPTexture FaBaoPicture = NULL;
	LPTexture FaBaoEff = NULL;

	/*if (g_OtherData.OtherPlayer().GetEquipGood(ITEM_POS_FABAOLEFT).GetID() != 0 && g_OtherData.OtherPlayer().GetFaBaoTypeLeft() > 0)
	{
		CharacterPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22992+CFaBaoShowWnd::TianDiRen(g_OtherData.OtherPlayer().GetFaBaoTypeLeft()),EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 62,m_iScreenY + 64,CharacterPicture); 

		FaBaoPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE,g_OtherData.OtherPlayer().GetFaBaoTypeLeft() + 22669,EP_UI);
		g_pGfx->DrawTextureFX(m_iScreenX - 25,m_iScreenY + 86,FaBaoPicture,-1,NULL,&POS(0.6f,0.6f));

		g_pGfx->SetRenderMode(RM_ADD2);
		int FabaoLevel = g_OtherData.OtherPlayer().GetEquipGood(ITEM_POS_FABAOLEFT).GetLevelUpTimes();
		int esLevelColor = FabaoLevel >= 9?2:(FabaoLevel >= 6?1:0);
		FaBaoEff = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22810 + (CFaBaoShowWnd::GetFaBaoEffType(g_OtherData.OtherPlayer().GetFaBaoTypeLeft()) - 1) * 3 + esLevelColor,EP_UI);
		g_pGfx->DrawTextureFX(m_iScreenX + 32,m_iScreenY + 122,FaBaoEff,-1,NULL,&POS(0.6f,0.6f));
		g_pGfx->SetRenderMode();
	}*/

	/*if (g_OtherData.OtherPlayer().GetEquipGood(ITEM_POS_FABAORIGHT).GetID() != 0 && g_OtherData.OtherPlayer().GetFaBaoTypeRight() > 0)
	{
		CharacterPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22992+CFaBaoShowWnd::TianDiRen(g_OtherData.OtherPlayer().GetFaBaoTypeRight()),EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 208,m_iScreenY + 64,CharacterPicture); 

		FaBaoPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE,g_OtherData.OtherPlayer().GetFaBaoTypeRight() + 22669,EP_UI);
		g_pGfx->DrawTextureFX(m_iScreenX + 120,m_iScreenY + 86,FaBaoPicture,-1,NULL,&POS(0.6f,0.6f));
		
		g_pGfx->SetRenderMode(RM_ADD2);
		int FabaoLevel = g_OtherData.OtherPlayer().GetEquipGood(ITEM_POS_FABAORIGHT).GetLevelUpTimes();
		int esLevelColor = FabaoLevel >= 9?2:(FabaoLevel >= 6?1:0);
		FaBaoEff = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22810 + (CFaBaoShowWnd::GetFaBaoEffType(g_OtherData.OtherPlayer().GetFaBaoTypeRight()) - 1) * 3 + esLevelColor,EP_UI);
		g_pGfx->DrawTextureFX(m_iScreenX + 177,m_iScreenY + 122,FaBaoEff,-1,NULL,&POS(0.6f,0.6f));
		g_pGfx->SetRenderMode();
	}*/

	if (g_OtherData.OtherPlayer().GetEquipGood(ITEM_POS_FABAO).GetID() != 0 && g_OtherData.OtherPlayer().GetFaBaoType() > 0)
	{
		//CharacterPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22992+CFaBaoShowWnd::TianDiRen(g_OtherData.OtherPlayer().GetFaBaoType()),EP_UI);
		//g_pGfx->DrawTextureNL(m_iScreenX + 135,m_iScreenY + 64,CharacterPicture); 

		FaBaoPicture = g_pTexMgr->GetTex(PACKAGE_INTERFACE,g_OtherData.OtherPlayer().GetFaBaoType() + 22669,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX - 30,m_iScreenY + 46,FaBaoPicture);

		int iWuXing = g_AIGoodMgr.GetFabaoWuXing(g_OtherData.OtherPlayer().GetEquipGood(ITEM_POS_FABAO));
		g_pGfx->SetRenderMode(RM_ADD2);
		LPTexture backGround = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22575 + iWuXing,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 40,m_iScreenY + 0,backGround);

		int esLevelColor = g_OtherData.OtherPlayer().GetFaBaoLevel() >= 9?2:(g_OtherData.OtherPlayer().GetFaBaoLevel() >= 6?1:0);
		FaBaoEff = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22810 + (CFaBaoShowWnd::GetFaBaoEffType(g_OtherData.OtherPlayer().GetFaBaoType()) - 1) * 3 + esLevelColor,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 65,m_iScreenY + 108,FaBaoEff);

		g_pGfx->SetRenderMode();
	}
	
}

bool COtherPlayerFaBaoEquip::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
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
					g_pControl->PopupWindow(MSG_CTRL_OTHER_PLAYER_INFO,OPER_CREATE);
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

bool COtherPlayerFaBaoEquip::OnMouseMove(int iX,int iY)
{
	int itemPos = 0;

	if((iX > 126 && iX < 205 && iY > 137 && iY < 265))
	{
		itemPos = ITEM_POS_FABAO;
		if (g_OtherData.OtherPlayer().GetFaBaoType() <= 0)
		{
			return true;
		}
	}
	/*else if((iX > 62 && iX < 125 && iY > 137 && iY < 265))
	{
		itemPos = ITEM_POS_FABAOLEFT;
		if (g_OtherData.OtherPlayer().GetFaBaoTypeLeft() <= 0)
		{
			return true;
		}
	}
	else if((iX > 206 && iX < 268 && iY > 137 && iY < 265))
	{
		itemPos = ITEM_POS_FABAORIGHT;
		if (g_OtherData.OtherPlayer().GetFaBaoTypeRight() <= 0)
		{
			return true;
		}
	}*/

	if (itemPos > 0)
	{
		CParserTip* pTip = g_pControl->GetTipWnd();
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->SetShow(false);

		if(g_OtherData.OtherPlayer().GetEquipGood(itemPos).GetID() != 0)
		{
			pTip->Clear();
			pTip->Parse(g_OtherData.OtherPlayer().GetEquipGood(itemPos));
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);

		}
		return true;
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}
