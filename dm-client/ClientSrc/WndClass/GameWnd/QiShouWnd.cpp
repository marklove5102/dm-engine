#include "QiShouWnd.h"
#include "GameData/OtherData.h"
#include "GameData/ActionInf.h"
#include "Baseclass/Control/ParserTip.h"

INIT_WND_POSX(CQiShouWnd,POS_AUTO,POS_AUTO)

CQiShouWnd::CQiShouWnd(void)
{
	m_iVersion = 2;

	AddTabPage(0,0,MAKELONG(23318,PACKAGE_INTERFACE),10,67,85,86,87,88,"人物",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	AddTabPage(0,0,MAKELONG(23318,PACKAGE_INTERFACE),10,152,85,86,87,88,"法宝",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);
	AddTabPage(0,0,MAKELONG(23318,PACKAGE_INTERFACE),10,237,85,86,87,88,"骑术",NULL,true,0,0,1,FONT_YAHEI,FONTSIZE_MIDDLE);

	m_TabPage.iCurPage = 2;

	m_bLastState = SELF.IsOnLepoard();
	m_bNeedFind = true;
	m_FocusGood.clear();
}

CQiShouWnd::~CQiShouWnd(void)
{
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"FaBaoShowWnd");
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"AvatarWnd");
}

void CQiShouWnd::OnCreate()
{
	SetCloseButton(466,4,80);

	

	CCtrlWindowX::OnCreate();
}

void CQiShouWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	static char* QiShuLevel[] = 
	{
		"",
		"新手",
		"初级",
		"中级",
		"高级",
		"专家",
	};

	char temp[64] = {0};

	HorsemanshipProtocalInfo& HorseManShipInfo = g_OtherData.GetHorseManShipInfo();

	
	if (HorseManShipInfo.Grade > 0 && HorseManShipInfo.Grade <= 100)
	{
		sprintf(temp,"%s骑师",QiShuLevel[(HorseManShipInfo.Grade + 19)/20]);
		g_pFont->DrawText(m_iScreenX + 228,m_iScreenY + 11,temp,COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
	}

	g_pFont->DrawText(m_iScreenX+ 84,m_iScreenY + 175,"骑术属性",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	
	if (SELF.GetCareer() == JOB_ZHANSHI)
	{
		g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205,"攻 击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	else if (SELF.GetCareer() == JOB_FASHI)
	{
		g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205,"魔法攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	else if (SELF.GetCareer() == JOB_DAO)
	{
		g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205,"道术攻击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205 + 28 * 1,"物理防御",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205 + 28 * 2,"魔法防御",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205 + 28 * 3,"暴 击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205 + 28 * 4,"反 弹",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205 + 28 * 5,"吸 血",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205 + 28 * 6,"抵 抗",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 205 + 28 * 7,"破 防",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	
	g_pGfx->DrawTextureNL(m_iScreenX + 47,m_iScreenY + 438,g_pTexMgr->GetTex(PACKAGE_INTERFACE,23205,EP_UI)); 


	LPTexture pTex = NULL;

	WORD wHorse = SELF.GetLooks().Player.wHorse;

	BYTE iReborn = 0;
	int iDrawMonsterAppr = CActionInf::GetDrawMonsterAppr(wHorse,iReborn);

	int iDeltaX = 80;
	int iDeltaY = -20;
	switch (iDrawMonsterAppr)
	{
	case 166:	//金豹子
	case 167:
	case 168:
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1108,EP_UI);
		}
		break;
	case 185:	//狮子
	case 186:
	case 187:
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1109,EP_UI);
		}
		break;
	case 39:	//麒麟
	case 40:
	case 41:
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1112,EP_UI);
		}
		break;
	case 373:
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1102,EP_UI);
		}
		break;
	case 301:	//凤凰
	case 303:
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1123,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1122,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1124,EP_UI);
			
		}
		break;
	case 302:
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1132,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1135,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

			pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1134,EP_UI);
			
		}
		break;
	}

	g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

	if (iReborn == 2)//二转
	{
		switch (iDrawMonsterAppr)
		{
		case 166:	//金豹子
		case 167:
		case 168:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1107,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
			}
			break;
		case 185:	//狮子
		case 186:
		case 187:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1106,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
			}
			break;
		case 39:	//麒麟
		case 40:
		case 41:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1105,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
			}
			break;
		case 301:	//凤凰
		case 303:
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1128,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1127,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1129,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
				g_pGfx->SetRenderMode();
			}
			break;
		case 302:
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1131,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1136,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1133,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
				g_pGfx->SetRenderMode();
			}
			break;
		}

		
	}
	else if (iReborn == 3)
	{
		switch (iDrawMonsterAppr)
		{
		case 166:	//金豹子
		case 167:
		case 168:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1115,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				g_pGfx->SetRenderMode(RM_ADD1);
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1140,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
				g_pGfx->SetRenderMode();
			}
			break;
		case 185:	//狮子
		case 186:
		case 187:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1116,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				g_pGfx->SetRenderMode(RM_ADD1);
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1141,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
				g_pGfx->SetRenderMode();
			}
			break;
		case 39:	//麒麟
		case 40:
		case 41:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1118,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				g_pGfx->SetRenderMode(RM_ADD1);
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1142,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
				g_pGfx->SetRenderMode();
			}
			break;
		case 373:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1117,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				g_pGfx->SetRenderMode(RM_ADD1);
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1143,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
				g_pGfx->SetRenderMode();
			}
			break;
		case 301:	//凤凰
		case 302:
		case 303:
			{
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1119,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1120,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);

				g_pGfx->SetRenderMode(RM_ADD1);
				pTex = g_pTexMgr->GetTexImm(PACKAGE_stateitem,1144,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX+iDeltaX,m_iScreenY+iDeltaY,pTex);
				g_pGfx->SetRenderMode();
			}
			break;
		}

	}

	g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 98,"骑乘状态: ",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	if (SELF.IsOnLepoard())
	{
		g_pFont->DrawText(m_iScreenX+ 125,m_iScreenY + 98,"骑乘",0xff00ff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 98+21,"骑术与骑兽的属性有效",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	else
	{
		g_pFont->DrawText(m_iScreenX+ 125,m_iScreenY + 98,"非骑乘",0xffff0000,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
		g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 98+21,"骑术与骑兽的属性无效",0xffff0000,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	}

	if (SELF.GetCareer() == JOB_ZHANSHI)
	{
		sprintf(temp,"%d",HorseManShipInfo.effect.dc);
	}
	else if (SELF.GetCareer() == JOB_FASHI)
	{
		sprintf(temp,"%d",HorseManShipInfo.effect.mc);
	}
	else if (SELF.GetCareer() == JOB_DAO)
	{
		sprintf(temp,"%d",HorseManShipInfo.effect.sc);
	}
	
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205,temp,COLOR_TEXT_NORMAL);
	
	sprintf(temp,"%d",HorseManShipInfo.effect.ac);
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205 + 28 * 1,temp,COLOR_TEXT_NORMAL);

	sprintf(temp,"%d",HorseManShipInfo.effect.mac);
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205 + 28 * 2,temp,COLOR_TEXT_NORMAL);

	sprintf(temp,"%d",HorseManShipInfo.effect.brust);
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205 + 28 * 3,temp,COLOR_TEXT_NORMAL);

	sprintf(temp,"%d",HorseManShipInfo.effect.rebound);
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205 + 28 * 4,temp,COLOR_TEXT_NORMAL);

	sprintf(temp,"%d",HorseManShipInfo.effect.absorbedHP);
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205 + 28 * 5,temp,COLOR_TEXT_NORMAL);

	sprintf(temp,"%d",HorseManShipInfo.effect.realDef);
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205 + 28 * 6,temp,COLOR_TEXT_NORMAL);

	sprintf(temp,"%d",HorseManShipInfo.effect.ignoredDef);
	g_pFont->DrawText(m_iScreenX+ 57 + 80,m_iScreenY + 205 + 28 * 7,temp,COLOR_TEXT_NORMAL);

	
	sprintf(temp,"骑乘等级: %d级",HorseManShipInfo.Grade);
	g_pFont->DrawText(m_iScreenX+ 218,m_iScreenY + 448,temp,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 218,m_iScreenY + 448 + 28 * 1,"骑术经验:",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	sprintf(temp,"%d/%d",HorseManShipInfo.Exp,HorseManShipInfo.MaxExp);
	g_pFont->DrawText(m_iScreenX+ 218 + 70,m_iScreenY + 448 + 28,temp,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	if (HorseManShipInfo.Exp != 0 && HorseManShipInfo.MaxExp != 0)
	{
		double percent = ((double)HorseManShipInfo.Exp)/((double)HorseManShipInfo.MaxExp);
		pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,23319,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX+217,m_iScreenY+509,pTex,0,0,(int)(percent * 180.0));
	}

	if (SELF.IsOnLepoard() != m_bLastState)
	{
		m_bNeedFind = true;
		m_bLastState = SELF.IsOnLepoard();
	}
}

bool CQiShouWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
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

				if (pParentTabpage == &m_TabPage && dwData == 1)//点了纸娃娃页签
				{
					CloseWindow();
					g_pControl->PopupWindow(MSG_CTRL_FABAOSHOW_WND,OPER_CREATE);
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

bool CQiShouWnd::OnMouseMove(int iX,int iY)
{

	if (iX >= 240 && iX <= 432 && iY >= 106 && iY <= 363 && SELF.IsOnLepoard())
	{
		if (m_bNeedFind)
		{
			int pos = 0;
			m_FocusGood.clear();

			while (pos >= 0)
			{
				pos = SELF.PackageGood().FindGoodByStdmode(69,0,pos);

				if (pos >= 0)
				{
					CGood& tempGood = SELF.GetPackageGood(pos);

					if (tempGood.GetResvEx3(0) != 0)
					{
						m_FocusGood = tempGood;
						break;
					}
				}
				else 
					break;

				pos++;
			}
			m_bNeedFind = false;
		}

		if (m_FocusGood.GetID() != 0)
		{
			char str[256] = {0};

			CParserTip* pTip = g_pControl->GetTipWnd();
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->SetShow(false);

			pTip->Clear();

			if (m_FocusGood.GetResvEx(0) != 0 || m_FocusGood.GetResvEx(1) != 0)
			{
				WORD life = (WORD(m_FocusGood.GetResvEx(0))) | (WORD(m_FocusGood.GetResvEx(1)) << 8);
				sprintf(str,"生命值+%d",life);
				pTip->AddText(str,TIPS_WHITE);
			}

			if (m_FocusGood.GetResvEx(2) != 0 || m_FocusGood.GetResvEx(3) != 0)
			{
				WORD magic = (WORD(m_FocusGood.GetResvEx(2))) | (WORD(m_FocusGood.GetResvEx(3)) << 8);
				sprintf(str,"魔法值+%d",magic);
				pTip->AddText(str,TIPS_WHITE);
			}

			if (m_FocusGood.GetAC() != 0)
			{
				sprintf(str,"防御+%d",m_FocusGood.GetAC());
				pTip->AddText(str,TIPS_WHITE);
			}

			if (m_FocusGood.GetMAC() != 0)
			{
				sprintf(str,"魔防+%d",m_FocusGood.GetMAC());
				pTip->AddText(str,TIPS_WHITE);
			}

			if (m_FocusGood.GetDC() != 0)
			{
				sprintf(str,"攻击+%d",m_FocusGood.GetDC());
				pTip->AddText(str,TIPS_WHITE);
			}			

			if (m_FocusGood.GetMC() != 0)
			{
				sprintf(str,"魔法攻击+%d",m_FocusGood.GetMC());
				pTip->AddText(str,TIPS_WHITE);
			}

			if (m_FocusGood.GetSC() != 0)
			{
				sprintf(str,"道术攻击+%d",m_FocusGood.GetSC());
				pTip->AddText(str,TIPS_WHITE);
			}

			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

