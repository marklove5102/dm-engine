#include "ShortCutKeyWnd.h"
#include "GameAI/AIConfigMgr.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameControl/GameControl.h"
#include "GameData/MagicDefine.h"

INIT_WND_POSX(CShortCutKeyWnd,POS_VARIABLE,POS_VARIABLE)

CShortCutKeyWnd::CShortCutKeyWnd(void)
{
	m_iIndex = 18062;
	m_iAlignType = XAL_TOP;
	m_bNoChangeLevel = true;
	//m_bNoMove = true;
	m_bShowEx = false;	
	m_bDisableEscape = true;
}

CShortCutKeyWnd::~CShortCutKeyWnd(void)
{
}

void CShortCutKeyWnd::OnCreate()
{
	m_pSwitchBtn = new CCtrlButton;
	AddControl(m_pSwitchBtn);
	m_pSwitchBtn->CreateEx(this,296,26,18067,18068,18069);

	if(m_bShowEx)
	{
		ReSize(m_iWidth,m_iHeight + 40);
	}
}

bool CShortCutKeyWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	switch (dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pSwitchBtn)
			{
				m_bShowEx = !m_bShowEx;

				if(m_bShowEx)
				{
					m_pSwitchBtn->ReloadTex(18064,18065,18066);
					ReSize(m_iWidth,82);				
				}
				else
				{
					m_pSwitchBtn->ReloadTex(18067,18068,18069);
					ReSize(m_iWidth,42);
				}

				g_AICfgMgr.SetShowExtendShortCut(m_bShowEx);

				g_pGameData->SetSkillShortCut(0,0,0,0);

				return true;
			}
		}
		break;
	case MSG_CTRL_SHORTCUTKEY_WND:
		{
			m_bShowEx = g_AICfgMgr.IsShowExtendShortCut();	

			if(m_bShowEx)
			{
				m_pSwitchBtn->ReloadTex(18064,18065,18066);
				ReSize(m_iWidth,82);				
			}
			else
			{
				m_pSwitchBtn->ReloadTex(18067,18068,18069);
				ReSize(m_iWidth,42);
			}

			return true;
		}
	case MSG_CTRL_POPUP_ARROWTIP://冒泡
		{
			int iTipID = -1;
			switch(dwData)
			{
			case MAGICID_ATTACK_BASE:
				iTipID = EP_First_SetShortcutKey_Base;
				break;
			case MAGICID_FIREBALL:
				iTipID = EP_First_SetShortcutKey_Fire;
				break;
			case MAGICID_POISON_MAGIC:
				iTipID = EP_First_SetShortcutKey_Poison;
				break;
			case MAGICID_PROTECT_SYMBOL:
				iTipID = EP_First_SetShortcutKey_Symbol;
				break;
			}
			if (iTipID > 0)
			{
				for(int i = 0; i < SELF.MagicArray().Size();i++)
				{
					CMagicData & magicData = SELF.GetMagic(i);

					if(magicData.GetMagicID() == dwData)
					{
						int iFNum = magicData.GetShortCutKey() - 0x31;
						int iX = 14 + iFNum * 35;
						AddArrowTip(iTipID,iX + 20,7 + 20,XAL_TOPLEFT,false,XAL_TOPLEFT);
						break;
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

void CShortCutKeyWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	LPTexture pTex = NULL;

	/*g_pGfx->DrawRect(m_iScreenX,m_iScreenY,m_iWidth,m_iHeight,-1);*/	

	WORD wShortCutMagic[8] = {0};
	WORD wShortCutExMagic[8] = {0};
	
	for(int i = 0; i < SELF.MagicArray().Size();i++)
	{
		CMagicData & magicData = SELF.GetMagic(i);

		if(magicData.GetMagicID() != 0)
		{
			if(magicData.GetShortCutKey() >= 0x31 && magicData.GetShortCutKey() <= 0x38)
			{
				wShortCutMagic[magicData.GetShortCutKey() - 0x31] = magicData.GetMagicID();
			}

			DWORD exkey = magicData.GetShortCutKeyEx();
			
			WORD wLowKey = LOWORD(exkey);

			if(HIWORD(exkey) == 1)
			{
				if(wLowKey >= VK_F1 && wLowKey <= VK_F8)
				{
					wShortCutExMagic[wLowKey - VK_F1] = magicData.GetMagicID();
				}
			}
		}
		else
		{
			/*if(SELF.GetYiHuoShortCutKey() >= 0x31 && SELF.GetYiHuoShortCutKey() <= 0x38)
			{
				wShortCutMagic[SELF.GetYiHuoShortCutKey() - 0x31] = MAGICID_FAZHEN_CARRYON;
			}*/

			/*DWORD exkey = SELF.GetYiHuoShortCutKeyEx();
			WORD wLowKey = LOWORD(exkey);
			if(HIWORD(exkey) == 1)
			{
				if(wLowKey >= VK_F1 && wLowKey <= VK_F8)
				{
					wShortCutExMagic[wLowKey - VK_F1] = MAGICID_FAZHEN_CARRYON;
				}
			}*/
		}		
	}

	ConSkillMap& kConSkills = g_AIMgr.GetConSkills();
	for (ConSkillMap::iterator i = kConSkills.begin(); i != kConSkills.end(); ++i)
	{
		ConSkillData* pConSkillData = i->second;
		if (pConSkillData && pConSkillData->dwKey != 0)
		{
			WORD wHighKey = HIWORD(pConSkillData->dwKey);
			WORD wLowKey = LOWORD(pConSkillData->dwKey);
			if (wHighKey == 0)
			{
				if(wLowKey >= VK_F1 && wLowKey <= VK_F8)
				{
					if(!(g_pGameData->GetSkillShortCut().iConSkillID == pConSkillData->iConSkillID && g_pGameData->GetSkillShortCut().iShortCut == wLowKey - VK_F1))
						wShortCutMagic[wLowKey - VK_F1] = pConSkillData->GetCurrentMagicID();
				}
			}
			else if (wHighKey == 1)
			{
				if(wLowKey >= VK_F1 && wLowKey <= VK_F8)
				{
					if(!(g_pGameData->GetSkillShortCut().iConSkillID == pConSkillData->iConSkillID && g_pGameData->GetSkillShortCut().iShortCutEx == wLowKey - VK_F1))
						wShortCutExMagic[wLowKey - VK_F1] = pConSkillData->GetCurrentMagicID();
				}
			}
		}
	}	

	int iStart = 14;

	for(int i = 0;i < 8;i++)
	{
		WORD mid = wShortCutMagic[i];	

		if(mid != 0)
		{
			if(g_pGameData->GetSkillShortCut().iMagicID == mid && g_pGameData->GetSkillShortCut().iShortCut == i)
			{

			}
			else
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,GetTexIDByMagicID(mid),EP_UI);

				//强袭烈焰盾攻击激活
				if (mid == MAGICID_FIRE_SHIELD_ATTACK && SELF.Is8DunShan())
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17290,EP_UI);
				}

				g_pGfx->DrawTextureNL(m_iScreenX + iStart,m_iScreenY + 7,pTex);	
			}
			
		}

		iStart += 35;
	}


	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,18070,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 2,m_iScreenY - 3,pTex);

	if(m_bShowEx)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,18063,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY + 41,pTex);

		int iStart = 14;

		for(int i = 0;i < 8;i++)
		{
			WORD mid = wShortCutExMagic[i];	

			if(mid != 0)
			{
				if(g_pGameData->GetSkillShortCut().iMagicID == mid && g_pGameData->GetSkillShortCut().iShortCutEx == i)
				{

				}
				else
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,GetTexIDByMagicID(mid),EP_UI);

					//强袭烈焰盾攻击激活
					if (mid == MAGICID_FIRE_SHIELD_ATTACK && SELF.Is8DunShan())
					{
						pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17290,EP_UI);
					}

					g_pGfx->DrawTextureNL(m_iScreenX + iStart,m_iScreenY + 46,pTex);	
				}
				
			}

			iStart += 35;
		}

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,18071,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 1,m_iScreenY + 41 - 2,pTex);				
	}
}

int CShortCutKeyWnd::GetGrid( int iX,int iY )
{
	int iStart = 12;

	for(int i = 0;i < 8;i++)
	{
		if(iX >= iStart && iX <= iStart + 32 && iY >= 5 && iY <= 37)
		{
			return i;
		}

		iStart += 35;
	}

	iStart = 12;

	for(int i = 0;i < 8;i++)
	{
		if(iX >= iStart && iX <= iStart + 32 && iY >= 45 && iY <= 77)
		{
			return i + 8;
		}

		iStart += 35;
	}

	return -1;
}

bool CShortCutKeyWnd::OnMouseMove( int iX,int iY )
{
	int iGrid = GetGrid(iX,iY);

	CParserTip* pTip = g_pControl->GetTipWnd();
	pTip->Clear();
	pTip->SetShow(false);

	char str[128] = "";
	bool showtips = false;

	if(iGrid >= 0 && iGrid <= 7)
	{
		/*if (SELF.GetYiHuoShortCutKey() == 0x31 + iGrid)
		{
			showtips = true;
			pTip->AddText(SELF.GetYIHUOINFO().strName,0xFFFFFF00);
		}
		else*/
		{		
			CMagicData * pMagicData = SELF.MagicArray().FindMagicDataByShortcut(0x31 + iGrid);

			ConSkillData* pConSkillData = 0;
			if (pMagicData == 0)
			{
				pConSkillData = g_AIMgr.FindConSkillByKey(MAKELONG(VK_F1 + iGrid,0));
				if (pConSkillData)
				{
					pMagicData = SELF.MagicArray().FindMagic(pConSkillData->GetCurrentMagicID());
				}
			}

			if(pMagicData)
			{
				showtips = true;
				pTip->AddText(pMagicData->GetMagicName(),0xFFFFFF00);
				if (pConSkillData)
				{
					pTip->AddText("连续技能");
				}
			}
		}

		if(showtips)
		{
			sprintf(str,"按F%d键快速释放该技能",iGrid + 1);
			pTip->AddText(str);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}
	else if(iGrid >= 8 && iGrid <= 15)
	{
		/*if (SELF.GetYiHuoShortCutKeyEx() ==MAKELONG(VK_F1 + iGrid - 8,1))
		{
			showtips = true;
			pTip->AddText(SELF.GetYIHUOINFO().strName,0xFFFFFF00);
		}
		else*/
		{
			CMagicData * pMagicData = SELF.MagicArray().FindMagicDataByShortcutEx(MAKELONG(VK_F1 + iGrid - 8,1));
			ConSkillData* pConSkillData = 0;
			if (pMagicData == 0)
			{
				pConSkillData = g_AIMgr.FindConSkillByKey(MAKELONG(VK_F1 + iGrid - 8,1));
				if (pConSkillData)
				{
					pMagicData = SELF.MagicArray().FindMagic(pConSkillData->GetCurrentMagicID());
				}
			}

			if(pMagicData)
			{
				showtips = true;
				pTip->AddText(pMagicData->GetMagicName(),0xFFFFFF00);
				if (pConSkillData)
				{
					pTip->AddText("连续技能");
				}
			}
		}
		

		if(showtips)
		{
			sprintf(str,"按ESC+F%d键快速释放该技能",iGrid - 8 + 1);
			pTip->AddText(str);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}

	if (g_pGameData->GetSkillShortCut().iMagicID != 0)
	{
		return true;
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CShortCutKeyWnd::OnLeftButtonDown( int iX, int iY )
{
	int iGrid = GetGrid(iX,iY);

	WORD wDestMagicID = 0;
	int iDestConSkillID = 0;
	int iDestShortCut = -1;
	int iDestShortCutEx = -1;

	WORD wSrcMagicID = g_pGameData->GetSkillShortCut().iMagicID;
	int iSrcConSkillID = g_pGameData->GetSkillShortCut().iConSkillID;
	int iSrcShortCut = g_pGameData->GetSkillShortCut().iShortCut;
	int iSrcShortCutEx = g_pGameData->GetSkillShortCut().iShortCutEx;	
	
	if(iGrid >= 0 && iGrid <= 7)
	{
		wDestMagicID = SELF.MagicArray().FindByShortcut(0x31 + iGrid);
		//法决
		/*if (SELF.GetYiHuoShortCutKey() == 0x31 + iGrid)
		{
			wDestMagicID = MAGICID_FAZHEN_CARRYON;
		}*/
		if (wDestMagicID <= 0)
		{
			//连续技能
			ConSkillData* pConSkillData = g_AIMgr.FindConSkillByKey(MAKELONG(VK_F1 + iGrid,0));
			if (pConSkillData)
			{
				iDestConSkillID = pConSkillData->iConSkillID;
			}
		}
		iDestShortCut = iGrid;
	}
	else if(iGrid >= 8 && iGrid <= 15)
	{
		DWORD cutkey = MAKELONG(VK_F1 + iGrid - 8,1);
		wDestMagicID = SELF.MagicArray().FindByShortcutEx(cutkey);
		//法决
		/*if (SELF.GetYiHuoShortCutKeyEx() == cutkey)
		{
			wDestMagicID = MAGICID_FAZHEN_CARRYON;
		}*/
		if (wDestMagicID <= 0)
		{
			ConSkillData* pConSkillData = g_AIMgr.FindConSkillByKey(cutkey);
			if (pConSkillData)
			{
				iDestConSkillID = pConSkillData->iConSkillID;
			}
		}

		iDestShortCutEx = iGrid - 8;
	}

	if( (wDestMagicID != 0 && wSrcMagicID != 0 && wDestMagicID == wSrcMagicID) 
		|| (iDestConSkillID != 0 && iSrcConSkillID != 0 && iDestConSkillID == iSrcConSkillID))	//放回图标
	{
		g_pGameData->SetSkillShortCut(0,0,-1,-1);
	}
	else if( (wSrcMagicID == 0 && iSrcConSkillID == 0) && (wDestMagicID != 0 || iDestConSkillID != 0))		//选中技能
	{
		g_pGameData->SetSkillShortCut(wDestMagicID,iDestConSkillID,iDestShortCut,iDestShortCutEx);
	}	
	else if((wSrcMagicID != 0 || iSrcConSkillID != 0) && (wDestMagicID == 0 && iDestConSkillID == 0))	//设置技能
	{
		if(iDestShortCut >= 0)		//设置到快捷栏
		{
			if (wSrcMagicID != 0)
			{
				//法决
				//if (wSrcMagicID == MAGICID_FAZHEN_CARRYON)
				//{
				//	g_pGameControl->SEND_YIHUO_NAME_SHORTCUT((char)(0x31 + iDestShortCut),SELF.GetYIHUOINFO().strName);
				//	SELF.SetYiHuoShortCutKey((char)(0x31 + iDestShortCut));

				//	if(iSrcShortCutEx >= 0)	//如果从扩展快捷栏移到快捷栏要把扩展快捷栏删掉
				//	{
				//		SELF.SetYiHuoShortCutKeyEx(0);
				//		g_AICfgMgr.SaveShortCutConfig();
				//	}
				//}
				//else
				{
					g_pGameControl->SEND_ShortCut_Key_Change(wSrcMagicID,(char)(0x31 + iDestShortCut));

					CMagicData * pMagic = SELF.FindMagic(wSrcMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKey((char)(0x31 + iDestShortCut));

						if(iSrcShortCutEx >= 0)	//如果从扩展快捷栏移到快捷栏要把扩展快捷栏删掉
						{
							pMagic->SetShortCutKeyEx(0);
							g_AICfgMgr.SaveShortCutConfig();
						}
					}
				}
			}
			else if (iSrcConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iSrcConSkillID);
				if (pConSkillData2)

				{
					pConSkillData2->dwKey = MAKELONG(VK_F1 + iDestShortCut,0);
					g_AICfgMgr.SaveShortCutConfig();
				}
			}

		}
		else if(iDestShortCutEx >= 0)	//设置到扩展快捷栏
		{
			if (wSrcMagicID != 0)
			{
				//法决
				//if (wSrcMagicID == MAGICID_FAZHEN_CARRYON)
				//{
				//	SELF.SetYiHuoShortCutKeyEx(MAKELONG(VK_F1 + iDestShortCutEx,1));
				//	g_AICfgMgr.SaveShortCutConfig();

				//	if(iSrcShortCut >= 0)	//如果从快捷栏移到扩展快捷栏要把快捷栏删掉
				//	{
				//		g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
				//		SELF.SetYiHuoShortCutKey(0);
				//	}
				//}
				//else
				{				
					CMagicData * pMagic = SELF.FindMagic(wSrcMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKeyEx(MAKELONG(VK_F1 + iDestShortCutEx,1));
						g_AICfgMgr.SaveShortCutConfig();

						if(iSrcShortCut >= 0)	//如果从快捷栏移到扩展快捷栏要把快捷栏删掉
						{
							g_pGameControl->SEND_ShortCut_Key_Change(wSrcMagicID,0);
							pMagic->SetShortCutKey(0);
						}
					}
				}
			}
			else if (iSrcConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iSrcConSkillID);
				if (pConSkillData2)
				{
					pConSkillData2->dwKey = MAKELONG(VK_F1 + iDestShortCutEx,1);
					g_AICfgMgr.SaveShortCutConfig();
				}
			}			
		}

		g_pGameData->SetSkillShortCut(0,0,-1,-1);

		g_pControl->Msg(MSG_CTRL_UPDATE_SKILLWND,0,0);
	}
	else if((wSrcMagicID != 0 || iSrcConSkillID != 0 )&& (wDestMagicID != 0 || iDestConSkillID != 0))	//交换技能
	{
		if(iSrcShortCut != -1)
		{
			if (wSrcMagicID != 0)
			{
				//法决
				/*if (wSrcMagicID == MAGICID_FAZHEN_CARRYON)
				{
					g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
					SELF.SetYiHuoShortCutKey(0);
				}
				else*/
				{
					g_pGameControl->SEND_ShortCut_Key_Change(wSrcMagicID,0);
					CMagicData * pMagic = SELF.FindMagic(wSrcMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKey(0);
					}
				}				
			}
			else if (iSrcConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iSrcConSkillID);
				if (pConSkillData2)
				{
					pConSkillData2->dwKey = 0;
					g_AICfgMgr.SaveShortCutConfig();
				}
			}
		}

		if(iSrcShortCutEx != -1)
		{
			if (wSrcMagicID != 0)
			{
				//法决
				/*if (wSrcMagicID == MAGICID_FAZHEN_CARRYON)
				{
					SELF.SetYiHuoShortCutKeyEx(0);
					g_AICfgMgr.SaveShortCutConfig();
				}*/
				//else
				{
					CMagicData * pMagic = SELF.FindMagic(wSrcMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKeyEx(0);
						g_AICfgMgr.SaveShortCutConfig();
					}
				}
			}
			else if (iSrcConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iSrcConSkillID);
				if (pConSkillData2)
				{
					pConSkillData2->dwKey = 0;
					g_AICfgMgr.SaveShortCutConfig();
				}
			}
		}

		if(iDestShortCut != -1)	//放到快捷键栏
		{
			CMagicData * pMagic = 0;
			if (wDestMagicID != 0)
			{
				//法决
				/*if (wDestMagicID == MAGICID_FAZHEN_CARRYON)
				{
					g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
					SELF.SetYiHuoShortCutKey(0);
				}
				else*/
				{
					g_pGameControl->SEND_ShortCut_Key_Change(wDestMagicID,0);
					pMagic = SELF.FindMagic(wDestMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKey(0);
					}
				}
			}
			else if (iDestConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iDestConSkillID);
				if (pConSkillData2)
				{
					pConSkillData2->dwKey = 0;
					g_AICfgMgr.SaveShortCutConfig();
				}
			}

			if (wSrcMagicID != 0)
			{
				//法决
				/*if (wSrcMagicID == MAGICID_FAZHEN_CARRYON)
				{
					g_pGameControl->SEND_YIHUO_NAME_SHORTCUT((char)(0x31 + iDestShortCut),SELF.GetYIHUOINFO().strName);
					SELF.SetYiHuoShortCutKey((char)(0x31 + iDestShortCut));
				}
				else*/
				{
					g_pGameControl->SEND_ShortCut_Key_Change(wSrcMagicID,(char)(0x31 + iDestShortCut));

					pMagic = SELF.FindMagic(wSrcMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKey((char)(0x31 + iDestShortCut));
					}
				}
				
			}
			else if (iSrcConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iSrcConSkillID);
				if (pConSkillData2)
				{
					pConSkillData2->dwKey = MAKELONG(VK_F1 + iDestShortCut,0);
					g_AICfgMgr.SaveShortCutConfig();
				}
			}

			if (wDestMagicID)
				g_pGameData->SetSkillShortCut(wDestMagicID,0,-1,-1);
			else if (iDestConSkillID)
				g_pGameData->SetSkillShortCut(0,iDestConSkillID,-1,-1);
		}
		else if(iDestShortCutEx != -1)	//放到扩展快捷栏
		{
			CMagicData * pMagic = 0;
			if (wDestMagicID)
			{
				//法决
				/*if (wDestMagicID == MAGICID_FAZHEN_CARRYON)
				{
					SELF.SetYiHuoShortCutKeyEx(0);
				}
				else*/
				{
					pMagic = SELF.FindMagic(wDestMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKeyEx(0);
					}
				}
			}
			else if (iDestConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iDestConSkillID);
				if (pConSkillData2)
				{
					pConSkillData2->dwKey = 0;
					g_AICfgMgr.SaveShortCutConfig();
				}
			}


			if (wSrcMagicID)
			{
				//法决
				/*if (wSrcMagicID == MAGICID_FAZHEN_CARRYON)
				{
					SELF.SetYiHuoShortCutKeyEx(MAKELONG(VK_F1 + iDestShortCutEx,1));
				}
				else*/
				{
					pMagic = SELF.FindMagic(wSrcMagicID);
					if(pMagic)
					{
						pMagic->SetShortCutKeyEx(MAKELONG(VK_F1 + iDestShortCutEx,1));
					}
				}

				g_AICfgMgr.SaveShortCutConfig();
			}
			else if (iSrcConSkillID != 0)
			{
				ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByID(iSrcConSkillID);
				if (pConSkillData2)
				{
					pConSkillData2->dwKey = MAKELONG(VK_F1 + iDestShortCutEx,1);
					g_AICfgMgr.SaveShortCutConfig();
				}
			}

			if (wDestMagicID)
				g_pGameData->SetSkillShortCut(wDestMagicID,0,-1,-1);
			else if (iDestConSkillID)
				g_pGameData->SetSkillShortCut(0,iDestConSkillID,-1,-1);
		}

		g_pControl->Msg(MSG_CTRL_UPDATE_SKILLWND,0,0);
	}

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}
