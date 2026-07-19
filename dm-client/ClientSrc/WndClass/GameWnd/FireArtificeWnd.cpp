#include "FireArtificeWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/TipsCfg.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TalkMgr.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/GameGlobal.h"
#include "GameData/OtherData.h"



INIT_WND_POSX(CFireArtificeWnd,POS_AUTO,POS_AUTO)

CFireArtificeWnd::CFireArtificeWnd(void)
{
	m_pOkBtn = NULL;

	m_iGridStartX = 0;
	m_iGridStartY = 0;
	m_iGridCount = 0;
	
	m_iState = -3;
	m_iFrame = 100000;
	m_iYuLanFrame = 100000;
	m_iSussFrame = 100000;
	m_iFailFrame = 100000;

	m_iIndex = 22510;

	m_iType = CalcType();

	if (m_iType == 1)
	{
		m_iGridStartX = 222+100;
		m_iGridStartY = 134+100;
		m_iGridCount = 3;
	}
	else if (m_iType == 2)
	{
		m_iGridStartX = 222+80;
		m_iGridStartY = 134+80;
		m_iGridCount = 4;
	}
	else if (m_iType == 3)
	{
		m_iGridStartX = 222+60;
		m_iGridStartY = 134+60;
		m_iGridCount = 5;
	}
	else if (m_iType == 4)
	{
		m_iGridStartX = 222+40;
		m_iGridStartY = 134+40;
		m_iGridCount = 6;
	}

	for (int i = 0; i < 23; ++i)
	{
		g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22610 + i,EP_UI);
		g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22640 + i,EP_UI);
	}

	g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22640,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE, 22641,EP_UI);
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE, 22900, 22916,EP_UI);
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE, 22920, 22936,EP_UI);

	if (g_OtherData.GetFireArtTime() == 0)
	{
		m_iState = -3;
		m_iFrame = 0;
		m_iYuLanFrame = 100000;
	}
	else if (g_OtherData.GetFireArtTime() > 0)
	{
		m_iState = -1;
		m_iFrame = 100000;
		m_iYuLanFrame = 100000;
	}
}

CFireArtificeWnd::~CFireArtificeWnd(void)
{
	BackToPackage();
}

void CFireArtificeWnd::OnCreate()
{
	SetCloseButton(703,35,80);

	m_pTestBtn = new CCtrlButton;
	AddControl(m_pTestBtn);
	m_pTestBtn->CreateEx(this,203,546,22506,22507,22508);	


	m_pOkBtn = new CCtrlButton;
	AddControl(m_pOkBtn);
	m_pOkBtn->CreateEx(this,383,546,22511,22512,22513);	
}

void CFireArtificeWnd::Draw()
{
	if (m_iState == -1 || m_iState == -3)
	{
		m_pTestBtn->SetEnable(false);
		m_pOkBtn->SetEnable(false);
		m_pTestBtn->SetShow(false);
		m_pOkBtn->SetShow(false);
	}
	else
	{
		m_pTestBtn->SetShow(true);
		m_pOkBtn->SetShow(true);

		if (m_iState == 1 || m_iState == 2 || m_iState == 10)
			m_pTestBtn->SetEnable(false);
		else
			m_pTestBtn->SetEnable(true);

		if (m_iState == 10)
			m_pOkBtn->SetEnable(false);
		else
			m_pOkBtn->SetEnable(true);
	}
	

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	UpdateFireArtTime();

	LPTexture pTex = NULL;
	char szstr[64] = {0};

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22940,EP_UI);
	if (pTex)
	{
		g_pGfx->SetRenderMode(RM_ADD2);
		g_pGfx->DrawTextureNL(m_iScreenX + 85, m_iScreenY - 40, pTex);
		g_pGfx->SetRenderMode();
	}

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22941,EP_UI);
	if (pTex)
	{
		g_pGfx->SetRenderMode(RM_ADD2);
		g_pGfx->DrawTextureNL(m_iScreenX + 85, m_iScreenY - 40, pTex);
		g_pGfx->SetRenderMode();
	}

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22514+m_iType-1,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 222,m_iScreenY + 134,pTex);		
	

	for (int j = 0; j < m_iGridCount; ++j)
	{
		for (int i = 0; i < m_iGridCount; ++i)
		{
			//g_pGfx->DrawFillRect(m_iScreenX + m_iGridStartX + i * 40 + 3,m_iScreenY + m_iGridStartY + j * 40 + 3,33,34,0x8000ff00);

			if(g_NPC.GetFireArtGood(i,j).GetID())
			{
				CGoodGrid::DrawOneGood(m_iScreenX + m_iGridStartX + i * 40 + 20,m_iScreenY + m_iGridStartY + j * 40 + 20,g_NPC.GetFireArtGood(i,j));
			}
		}
	}
	

	
	if (m_kRet.wLooks != 0)
	{
		//g_pGfx->DrawFillRect(m_iScreenX + 368,m_iScreenY + 505,33,34,0x8000ff00);

		pTex = g_pTexMgr->GetTex(PACKAGE_items,m_kRet.wLooks,EP_UI);
		if (pTex)
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 368,m_iScreenY + 505,pTex);
		}


		sprintf(szstr,"成功率：%.2f%%",(float)m_kRet.iPercent / 100.0f);
		g_pFont->DrawText(m_iScreenX + 425,m_iScreenY + 505,szstr,0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);


		g_pFont->DrawText(m_iScreenX + 425,m_iScreenY + 525,"需要金币：",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);	
		ToCommaStr(szstr,m_kRet.dwNeedMoney);
		if(SELF.GetGold() < m_kRet.dwNeedMoney)
			g_pFont->DrawText(m_iScreenX + 486,m_iScreenY + 525,szstr,0xffff0000,FONT_YAHEI,12,DTF_BlackFrame);
		else
			g_pFont->DrawText(m_iScreenX + 486,m_iScreenY + 525,szstr,0xfffdca66,FONT_YAHEI,12,DTF_BlackFrame);
	}


	if (m_iState == 1)
	{
		if (m_iYuLanFrame < 50)
		{
			++m_iYuLanFrame;
			if (m_iYuLanFrame == 50)
			{
				m_iState = 2;

				SendFireArtifice(1);				
			}
		}
		g_pFont->DrawText(m_iScreenX + 425,m_iScreenY + 515,"预览中，请稍候。。。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
	}
	else if (m_iState == 2)
	{
		g_pFont->DrawText(m_iScreenX + 425,m_iScreenY + 515,"预览中，请稍候。。。",0xffffe650,FONT_YAHEI,12,DTF_BlackFrame);
	}



	if (m_iState == -3 || m_iState == -1)
	{
		if (m_iFrame < 24 * 4)
		{
			if (m_iState == -3)
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22610 + m_iFrame/4,EP_UI);
				if (pTex)
					g_pGfx->DrawTextureNL(m_iScreenX + 91, m_iScreenY, pTex);

				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22640 + m_iFrame/4,EP_UI);
				if (pTex)
				{
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + 91, m_iScreenY, pTex);
					g_pGfx->SetRenderMode();

					++m_iFrame;

					if (m_iFrame == 24 * 4)
					{
						m_iState = 0;
						m_iFrame = 100000;
						m_iYuLanFrame = 100000;
					}
				}
			}
			else if (m_iState == -1)
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22633 - m_iFrame/4,EP_UI);
				if (pTex)
					g_pGfx->DrawTextureNL(m_iScreenX + 91, m_iScreenY, pTex);

				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22663 - m_iFrame/4,EP_UI);
				if (pTex)
				{
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + 91, m_iScreenY, pTex);
					g_pGfx->SetRenderMode();

					++m_iFrame;	
				}
			}
			
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22610,EP_UI);
			if (pTex)
				g_pGfx->DrawTextureNL(m_iScreenX + 91, m_iScreenY, pTex);
		}
	}

	if (m_iSussFrame < 16 * 4)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22900 + m_iSussFrame/4,EP_UI);
		if (pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX + 255, m_iScreenY + 388, pTex);
			g_pGfx->SetRenderMode();

			++m_iSussFrame;
		}
	}

	if (m_iFailFrame < 16 * 4)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22920 + m_iFailFrame/4,EP_UI);
		if (pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX + 255, m_iScreenY + 388, pTex);
			g_pGfx->SetRenderMode();

			++m_iFailFrame;
		}
	}
}

bool CFireArtificeWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */ )
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pTestBtn)
			{
				if (g_OtherData.GetFireArtTime() == 0)
				{
					for (int j = 0; j < m_iGridCount; ++j)
					{
						for (int i = 0; i < m_iGridCount; ++i)
						{
							if (g_NPC.GetFireArtGood(i,j).GetID() != 0)
							{
								m_iState = 1;
								m_iYuLanFrame = 0;
								m_kRet.clear();
								return true;
							}
						}
					}

					g_MsgBoxMgr.PopSimpleAlert("炉内不能为空!");
				}
				
				return true;
			}
			else if(pControl == m_pOkBtn)
			{
				if (g_OtherData.GetFireArtTime() == 0)
				{
					for (int j = 0; j < m_iGridCount; ++j)
					{
						for (int i = 0; i < m_iGridCount; ++i)
						{
							if (g_NPC.GetFireArtGood(i,j).GetID() != 0)
							{
								g_MsgBoxMgr.PopOkCancelBox("无论炼化成功或失败，您放入的所有炼化材料都会消失，您确定么？",MSG_CTRL_FIREARTIFICE,0);
								return true;
							}
						}
					}

					g_MsgBoxMgr.PopSimpleAlert("炉内不能为空!");
				}
				
				return true;
			}
		}
		break;
	case MSG_CTRL_FIREARTIFICE_WND:
		{
			if (dwData == 1000)
			{
				if (m_iState == 2)
				{
					m_kRet = *(FireArtificeResult*)pControl;

					m_iState = 0;
					m_iYuLanFrame = 100000;
				}

				return true;
			}
			else if (dwData == 1001)
			{
				OnFireArtTimeSet();
				return true;
			}
			else if (dwData == 1002)
			{
				//确定炼化
				if (g_OtherData.GetFireArtTime() == 0 && SendFireArtifice(0))
				{
					m_iState = 10;
					m_iYuLanFrame = 100000;
				}
				return true;
			}


			//  0 成功
			//	1 物品错误(假物品之类的)
			//	2 系统错误
			//	3 有绑定的物品
			//	4 金币不够
			//	5 炼化失败
			//	6 真火不足
			//  7 CD时间未到
			//  8 物品数量不足

			WORD wResult = HIWORD(dwData);
			WORD wType = LOWORD(dwData);

			m_iState = 0;
			m_iYuLanFrame = 100000;

			if (wResult == 0)	//成功
			{
				m_iSussFrame = 0;
				m_iFailFrame = 100000;

				g_TalkMgr.PushSysTalk("炼化成功！");
			}
			else
			{
				m_kRet.clear();
				m_iFailFrame = 0;
				m_iSussFrame = 100000;

				std::string str;
				switch (wResult)
				{				
				case 1:
					str = "物品非法！";
					break;
				case 2:
					str = "系统错误！";
					break;
				case 3:
					str = "有固化物品！";
					break;
				case 4:
					str = "金币不够！";
					break;
				case 5:
					str = "炼化失败！";
					break;
				case 6:
					str = "真火不足！";
					break;
				case 7:
					str = "冷却时间未到！";
					break;
				case 8:
					str = "物品数量不足！";
					break;
				default:
					str = "系统忙，请稍后再试！";
				}

				g_TalkMgr.PushSysTalk(str.c_str());
			}
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CFireArtificeWnd::OnMouseMove(int iX,int iY)
{	
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);

	if(iX > 368 && iX < 368 + 36 && iY > 505 && iY < 505 + 36)	
	{
		if (!m_kRet.strGoodName.empty())
		{
			pTip->AddText(m_kRet.strGoodName.c_str(),TIPS_GOODNAME,-1,0,-1,FONT_DEFAULT,FONTSIZE_MIDDLE);
		}
		else
		{
			pTip->AddText("炼化结果预览格");
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	
	
	int iGoodOnGridX = -1;
	int iGoodOnGridY = -1;
	CGood * pGood = GetGoodOnGrid(iX,iY,iGoodOnGridX,iGoodOnGridY);

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

	if (m_iState >= 0)
	{	
		if (iX > m_iGridStartX && iX < m_iGridStartX + m_iGridCount * 40 && iY > m_iGridStartY && iY < m_iGridStartY + m_iGridCount * 40)
		{
			pTip->AddText("可放入任意道具材料进行炼化。",COLOR_TEXT_NORMAL);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}

		int rx = iX - 383;
		int ry = iY - 292;
		if (rx * rx + ry * ry < 180 * 180)
		{
			if (m_iType >= 1 && m_iType <= 3)
			{
				switch (m_iType)
				{
				case 1:
					pTip->AddText("10级真火可开启16格",COLOR_TEXT_NORMAL,-1);
					break;
				case 2:
					pTip->AddText("28级真火可开启25格",COLOR_TEXT_NORMAL,-1);
					break;
				case 3:
					pTip->AddText("55级真火可开启36格",COLOR_TEXT_NORMAL,-1);
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
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CFireArtificeWnd::OnLeftButtonUp( int iX, int iY )
{
	if (m_iState != -3 && m_iState != -1)
	{
		int iGoodOnGridX = -1;
		int iGoodOnGridY = -1;
		CGood * pGood = GetGoodOnGrid(iX,iY,iGoodOnGridX,iGoodOnGridY);

		if (pGood)
		{
			if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() && CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd)
			{
				CGoodGrid::ReleaseDrop();
				g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
				return true;
			}

			stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

			//if(GoodFrom.DropGood.GetID() && !CheckValidGoodOnGrid(iGoodOnGrid,GoodFrom.DropGood, true))
			//	return true;

			CGood temp = *pGood;
			*pGood = GoodFrom.DropGood;
			GoodFrom.DropGood = temp;

			m_kRet.clear();

			return true;
		}
	}	

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}


void CFireArtificeWnd::BackToPackage()
{
	for (int j = 0; j < 6; ++j)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (g_NPC.GetFireArtGood(i,j).GetID() != 0)
			{
				SELF.PackageGood().BackToArray(g_NPC.GetFireArtGood(i,j));
				g_NPC.GetFireArtGood(i,j).SetID(0);
			}
		}
	}
}

CGood* CFireArtificeWnd::GetGoodOnGrid(int iX,int iY,int& gridx, int& gridy)
{
	gridx = -1;
	gridy = -1;	
	for (int j = 0; j < m_iGridCount; ++j)
	{
		for (int i = 0; i < m_iGridCount; ++i)
		{
			if(iX > m_iGridStartX + i * 40 && iX < m_iGridStartX + i * 40 + 36 && iY > m_iGridStartY + j * 40 && iY < m_iGridStartY + j * 40 + 36)
			{
				gridx = i;
				gridy = j;
				return &g_NPC.GetFireArtGood(i,j);
			}
		}
	}

	return NULL;
}


int CFireArtificeWnd::CalcType()
{
	BYTE byFireLevel = g_OtherData.GetSanWeiZhenHuoInfo().byFireLevel;
	if (byFireLevel <= 9)
		return 1;
	else if (byFireLevel >= 10 && byFireLevel <= 27)
		return 2;
	else if (byFireLevel >= 28 && byFireLevel <= 54)
		return 3;
	else if (byFireLevel >= 55 && byFireLevel <= 81)
		return 4;

	return 4;
}

bool CFireArtificeWnd::SendFireArtifice(WORD test)
{
	for (int j = 0; j < m_iGridCount; ++j)
	{
		for (int i = 0; i < m_iGridCount; ++i)
		{
			if (g_NPC.GetFireArtGood(i,j).GetID() != 0)
			{
				g_pGameControl->SEND_FireArtifice(m_iGridCount,test);
				return true;
			}
		}
	}

	return false;
}

void CFireArtificeWnd::UpdateFireArtTime()
{
	if (g_OtherData.GetFireArtTime() > 0)
	{
		m_iState = -1;		
		m_iYuLanFrame = 100000;

		DWORD dwCurTime = GetTickCount();


		if ((dwCurTime - g_OtherData.GetFireArtStartTime()) >= g_OtherData.GetFireArtTime() * 1000)
		{
			g_OtherData.SetFireArtTime(0);
			m_iState = -3;
			if (m_iFrame < 24 * 4)
				m_iFrame = 24 * 4 - m_iFrame;
			else
				m_iFrame = 0;
		}
		else
		{
			int startshowx = 246;
			g_pFont->DrawText(m_iScreenX + startshowx,m_iScreenY + 560, "炼化炉冷却中：", 0xFFFFFFFF, FONT_YAHEI, FONTSIZE_MIDDLE);
			g_pFont->DrawText(m_iScreenX + startshowx + 160,m_iScreenY + 560, "后炼化炉冷却完成", 0xFFFFFFFF, FONT_YAHEI, FONTSIZE_MIDDLE);

			char strtemp[64] = {0};

			DWORD ltime = g_OtherData.GetFireArtTime() - (dwCurTime - g_OtherData.GetFireArtStartTime()) /1000;

			sprintf(strtemp, "%02d:%02d:%02d", ltime/3600, (ltime/60)%60, ltime%60);
			g_pFont->DrawText(m_iScreenX + startshowx + 100,m_iScreenY + 560, strtemp, 0xFF00B050, FONT_YAHEI, FONTSIZE_MIDDLE);
		}
	}
}

void CFireArtificeWnd::OnFireArtTimeSet()
{
	// 冷却时间更新
	if (g_OtherData.GetFireArtTime() == 0)
	{
		if (m_iState == -1 || m_iState == -3)
		{
			if (m_iState == -1)
			{
				if (m_iFrame < 24 * 4)
					m_iFrame = 24 * 4 - m_iFrame;
				else
					m_iFrame = 0;
			}
			m_iState = -3;

			m_iYuLanFrame = 100000;
		}

	}
	else if (g_OtherData.GetFireArtTime() > 0)
	{
		if (m_iState != -3 && m_iState != -1)
			m_iFrame = 0;
		else if (m_iState == -3 && m_iFrame < 24 * 4)
			m_iFrame = 24 * 4 - m_iFrame;

		m_iState = -1;
		m_iYuLanFrame = 100000;
		BackToPackage();
		m_kRet.clear();
	}
}
