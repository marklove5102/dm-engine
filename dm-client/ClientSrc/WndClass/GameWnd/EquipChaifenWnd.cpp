#include "EquipChaifenWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/TipsCfg.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/GameGlobal.h"


#define EquipChaifen_PosX 37
#define EquipChaifen_PosY 238

#define FrameSpeed 3
#define TotalFrame FrameSpeed * 60


INIT_WND_POSX(CEquipChaifenWnd,POS_AUTO,POS_AUTO)

CEquipChaifenWnd::CEquipChaifenWnd(void)
{
	m_pOkBtn = NULL;
	m_pMarkViewer = NULL;

	m_iIndex = 23085;

	m_iFrame = 10000;
	m_bFirstClick = false;	
}

CEquipChaifenWnd::~CEquipChaifenWnd(void)
{
	BackToPackage();	
}

void CEquipChaifenWnd::OnCreate()
{
	SetCloseButton(484,4,80);

	m_pOkBtn = new CCtrlButton;
	AddControl(m_pOkBtn);
	m_pOkBtn->CreateEx(this,24,293,95,96,97,98);	
	m_pOkBtn->SetText("拆 分");
	

	m_pMarkViewer = new CMarkViewer(20,5);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,103,235,380,80);
	m_pMarkViewer->SetTagText(&m_Tag);
	m_pMarkViewer->SetFont(FONT_YAHEI,12);
}

void CEquipChaifenWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	
	LPTexture pTex = NULL;

	g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + 10,"装备拆分",0xFF550F0F,FONT_YAHEI,16,DTF_Center|DTF_BlackFrame,0,0xFFE0920C);

	if(g_NPC.GetChaifenGood().GetID())
	{
		CGoodGrid::DrawOneGood(m_iScreenX + EquipChaifen_PosX + 17,m_iScreenY + EquipChaifen_PosY + 18,g_NPC.GetChaifenGood());
	}


	if(m_iFrame < TotalFrame)
	{
		if(m_iFrame == 47 * FrameSpeed)
		{
			if(g_NPC.GetChaifenGood().GetID() != 0)
			{
				SendEquipSoulMsg();
			}

			m_bFirstClick = false;
		}


		if(m_iFrame >= 8 * FrameSpeed && m_iFrame <= 47 * FrameSpeed)
		{	
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23084,EP_UI);
			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame((m_iFrame / 4) % 12);
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 360 - 23 - 44 + 13,m_iScreenY + 40 - 63 + 37,pTex);
				g_pGfx->SetRenderMode();
			}
		}		

		pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,23083,EP_UI);
		if(pTex)
		{
			pTex->EnableSysAnim(false);
			pTex->SetCurFrame(m_iFrame / FrameSpeed);
			g_pGfx->DrawTextureNL(m_iScreenX - 23 - 44 + 13,m_iScreenY - 10 - 63 + 37,pTex);
		}
		m_iFrame++;
	}
	else
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23082,EP_UI);	
		g_pGfx->DrawTextureNL(m_iScreenX + 325 - 23 - 44 + 13,m_iScreenY + 56 - 63 + 37,pTex);
	}
}

bool CEquipChaifenWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */ )
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOkBtn)
			{				
				if(m_bFirstClick)
				{
					m_bFirstClick = false;
					m_iFrame = 47 * FrameSpeed;					
				}
				else if(g_NPC.GetChaifenGood().GetID())
				{
					m_iFrame = 0;
					m_bFirstClick = true;
				}
				return true;
			}			
		}
		break;
	case MSG_CTRL_EQUIPCHAIFEN_WND:
		{
			if (dwData == 100)
			{
				std::string str = (const char *)pControl;
				m_Tag.Parse(str);
				return true;
			}
			
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CEquipChaifenWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();
	pTip->SetShow(false);
	pTip->GetSelfEquipTips()->SetShow(false);

	
	bool grid = false;
	CGood * pGood = GetGoodOnGrid(iX,iY,grid);

	stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();	

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

	if (grid)
	{
		pTip->AddText("请放入需要拆分的装备");

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);

		return true;
	}
	

	return CCtrlWindowX::OnMouseMove(iX,iY);

}

bool CEquipChaifenWnd::OnLeftButtonUp( int iX, int iY )
{
	bool grid = false;
	CGood * pGood = GetGoodOnGrid(iX,iY,grid);

	if(pGood && m_iFrame > 120)
	{
		if(CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd && CGoodGrid::GetDropGoodFrom().DropGood.GetID())
		{
			g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() && !IsInValidGoodOnGrid(GoodFrom.DropGood))
		{
			return true;
		}

		CGood temp = *pGood;
		*pGood = GoodFrom.DropGood;
		GoodFrom.DropGood = temp;

		if (g_NPC.GetChaifenGood().GetID() != 0)
		{
			g_pGameControl->SEND_Equip_Chaifen(g_NPC.GetChaifenGood().GetID(), 0);
		}

		m_Tag.Clear();

		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}

CGood* CEquipChaifenWnd::GetGoodOnGrid(int iX,int iY,bool& grid)
{
	grid = false;

	if (IsInGrid(iX,iY,EquipChaifen_PosX,EquipChaifen_PosY))
	{
		grid = true;
		return &(g_NPC.GetChaifenGood());
	}
	
	return NULL;
}



bool CEquipChaifenWnd::IsInValidGoodOnGrid(CGood& good)
{
	return true;
}

bool CEquipChaifenWnd::IsInGrid(int iX,int iY, int gridx, int gridy)
{
	if(iX > gridx && iX < gridx + 36 && iY > gridy && iY < gridy + 36)
		return true;

	return false;
}


void CEquipChaifenWnd::BackToPackage()
{
	SELF.PackageGood().BackToArray(g_NPC.GetChaifenGood());
	g_NPC.GetChaifenGood().SetID(0);
}

void CEquipChaifenWnd::SendEquipSoulMsg()
{
	m_Tag.Clear();
	g_pGameControl->SEND_Equip_Chaifen(g_NPC.GetChaifenGood().GetID(), 1);
}
