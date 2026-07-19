#include "TreasureMapWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Control/ParserTip.h"

INIT_WND_POSX(CTreasureMapWnd,POS_AUTO,POS_AUTO)

CTreasureMapWnd::CTreasureMapWnd(void)
{
	m_iIndex = 16965;
	m_iFlashPart = -1;
	m_dwGoodID = sm_dwWindowType;

	int iFromWnd = NO_Wnd;
	CGood * pGood = g_pGameData->SearchGoodByID(m_dwGoodID,iFromWnd);
	if(pGood != NULL)
	{
		if(pGood->GetShape() == 7)
		{
			m_iType = 1;
		}
		else
		{
			m_iType = 2;
		}
	}
}

CTreasureMapWnd::~CTreasureMapWnd(void)
{
}

void CTreasureMapWnd::OnCreate()
{
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,194,354,16966,16967,16968,16969);
	m_pOKButton->SetEnable(false);

	SetCloseButton(523,13,16970);
}

void CTreasureMapWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	LPTexture pMask = NULL;
	if(m_iType == 1)
	{
		pMask = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17013,EP_UI);		
	}
	else
	{
		pMask = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16973,EP_UI);		
	}
	g_pGfx->DrawTextureNL(m_iScreenX + 177,m_iScreenY + 28,pMask);

	LPTexture pTex = NULL;
	DWORD dwCollectedPiece = 0;	
	DWORD dwMask = 1;

	int iFromWnd = NO_Wnd;
	CGood * pGood = g_pGameData->SearchGoodByID(m_dwGoodID,iFromWnd);
	if(pGood != NULL)
	{
		dwCollectedPiece = pGood->GetRecordTime();
		if(dwCollectedPiece == 0x3fffffff)
		{
			m_pOKButton->SetEnable(true);
		}
	}

	int iStartIndex = 16975;
	if(m_iType == 1)
	{
		iStartIndex = 17014;
	}

	g_pGfx->SetRenderMode(RM_ALPHAADD);
	for(int i = 0;i < 30;i++)
	{
		if((dwCollectedPiece & dwMask) != 0)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,iStartIndex + i,EP_UI);			
			g_pGfx->DrawTextureNL(m_iScreenX + 46,m_iScreenY + 65,pTex);
		}
		dwMask <<= 1;		
	}	
	g_pGfx->SetRenderMode();

	int iX,iY;
	g_pControl->GetMouseXY(iX,iY);
	iX -= m_iScreenX;
	iY -= m_iScreenY;

	if(iX > 46 && iX < 499 + 46 && iY > 65 && iY < 65 + 282 && m_iFlashPart != -1)
	{
		if((dwCollectedPiece & (1 << m_iFlashPart)) == 0)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,iStartIndex + m_iFlashPart,EP_UI);				
			g_pGfx->DrawTextureNL(m_iScreenX + 46,m_iScreenY + 65,pTex,0xffffffff);			
		}
	}	
	else
	{
		m_iFlashPart = -1;
	}
}

bool CTreasureMapWnd::OnMouseMove(int iX,int iY)
{
	bool bReturnTrue = false;
	int iStartIndex = 16975;
	char str[256] = "";
	if(m_iType == 1)
	{
		iStartIndex = 17014;
	}

	g_pControl->GetTipWnd()->Clear();
	g_pControl->GetTipWnd()->SetShow(false);
	for(int i = 0;i < 30;i++)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,iStartIndex + i,EP_UI);
		if(pTex && pTex->IsPointInTex(iX - 46,iY - 65) == 2)
		{
			sprintf(str,"%d号碎片",i + 1);
			g_pControl->GetTipWnd()->AddText(str);
			g_pControl->GetTipWnd()->SetShow(true);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			g_pControl->GetTipWnd()->AdjustXY(x,y);
			g_pControl->GetTipWnd()->Move(x,y);

			bReturnTrue = true;
			break;
		}
	}	


	if(iX > 46 && iX < 499 + 46 && iY > 65 && iY < 65 + 282)
	{
		stGoodFrom& stGood = CGoodGrid::GetDropGoodFrom();
		if(stGood.DropGood.GetID() != 0 && stGood.eFromWnd == Package_Wnd)
		{
			int iFromWnd = NO_Wnd;
			CGood * pGood = g_pGameData->SearchGoodByID(m_dwGoodID,iFromWnd);
			if(pGood != NULL)
			{
				if(pGood->GetStdMode() == 46 && pGood->GetShape() == stGood.DropGood.GetShape() && stGood.DropGood.GetDura() != 0)
				{
					m_iFlashPart = stGood.DropGood.GetDura() - 1;
					return true;					
				}
			}		
		}
	}

	m_iFlashPart = -1;

	if(bReturnTrue)
	{
		return true;
	}
	else
	{
		return CCtrlWindowX::OnMouseMove(iX,iY);
	}
}

bool CTreasureMapWnd::OnLeftButtonUp(int iX, int iY)
{
	if(iX > 46 && iX < 499 + 46 && iY > 65 && iY < 65 + 282)
	{
		stGoodFrom& stGood = CGoodGrid::GetDropGoodFrom();
		if(stGood.DropGood.GetID() != 0 && stGood.eFromWnd == Package_Wnd)
		{
			int iFromWnd = NO_Wnd;
			CGood * pGood = g_pGameData->SearchGoodByID(m_dwGoodID,iFromWnd);
			if(pGood != NULL)
			{
				if(pGood->GetStdMode() == 46 && pGood->GetShape() == stGood.DropGood.GetShape() && stGood.DropGood.GetDura() != 0)
				{
					DWORD dwCollectedPiece = pGood->GetRecordTime();
					if(1 << (stGood.DropGood.GetDura() - 1) & dwCollectedPiece)
					{
						//地图里已经有了这块碎片
					}
					else	
					{
						//将碎片放入地图
						g_pGameControl->SEND_AddObjToObj(stGood.DropGood,*pGood);
						return true;
					}
				}
			}
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CTreasureMapWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOKButton)//确认
			{	
				int iPos = SELF.PackageGood().FindGoodPos(m_dwGoodID);

				if(m_iType == 1 && iPos != -1)
				{
					g_MsgBoxMgr.PopSimpleComfirm("    封印在银月中的怪物身上都携带着大量宝藏。\n    你可召唤神圣大门，打开通往银月之路，带领你的宗族所有成员一起进入。记住，每次进入只有30分钟，只有将地图内的怪物全部杀死，你才能打开银月宝箱获得珍宝。\n    你确定现在要召唤通往银月的神圣大门吗？",MSG_CTRL_USE_TREASURE_MAP,iPos);
				}
				else if(iPos != -1)
				{
					g_MsgBoxMgr.PopSimpleComfirm("    封印在炙日中的怪物身上都携带着大量宝藏。\n    你可召唤神圣大门，打开通往炙日之路，带领你的宗族所有成员一起进入。记住，每次进入只有30分钟，只有将地图内的怪物全部杀死，你才能打开炙日宝箱获得珍宝。\n    你确定现在要召唤通往炙日的神圣大门吗？",MSG_CTRL_USE_TREASURE_MAP,iPos);
				}

				return true;
			}			
		}
		break;	
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}