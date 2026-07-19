#include "ArrowTip.h"
#include <string>
#include "GameData/GameGlobal.h"
#include "BaseClass/Misc/Input.h"
#include "GameData/SimpleCharacter.h"
#include "GameData/GameData.h"
#include "WndClass/GameWnd/GSceneManager.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"

using namespace std;

CArrowTip::CArrowTip(void)
{
	m_bNoFocus = true;
	m_iTipType = 0;
	m_dwData = 0;
	m_dwLastTime = 0;
	m_iWidth = 125;
	m_iHeight = 88;
	m_bLBClickOnCloseBtn = false;
	m_bNoMove = true;
	m_bNeedMsg = true;
	m_iTipID = -1;
}

CArrowTip::~CArrowTip(void)
{
	g_pGameData->AddPaoPaoStatus_Closed(ePaoPaoPromptStatus(m_iTipID));
}

bool CArrowTip::Create( CControl * pParent,int iPointX,int iPointY,int iAlignType,int iTipDir,int iTipType,DWORD dwData,DWORD dwLastTime,bool bNeedMsg)
{
	m_iTipDir = iTipDir;
	m_iTipType = iTipType;
	m_dwData = dwData;
	m_dwLastTime = dwLastTime;
	m_bNeedMsg = bNeedMsg;

	int iParentScreenX = pParent->GetScreenX();
	int iParentScreenY = pParent->GetScreenY();
	int iW = pParent->GetWidth();
	int iH = pParent->GetHeight();

	if(iAlignType == XAL_CENTER || iAlignType == XAL_TOP || iAlignType == XAL_BOTTOM)
	{
		iPointX = (iW - m_iWidth) / 2 + iPointX;
	}
	else if(iAlignType == XAL_TOPRIGHT || iAlignType == XAL_RIGHT || iAlignType == XAL_BOTTOMRIGHT)
	{
		iPointX = iW - iPointX;
	}

	if(iAlignType == XAL_CENTER || iAlignType == XAL_LEFT || iAlignType == XAL_RIGHT)
	{
		iPointY = (iH - m_iHeight) / 2 + iPointY;
	}
	else if(iAlignType == XAL_BOTTOMLEFT || iAlignType == XAL_BOTTOM || iAlignType == XAL_BOTTOMRIGHT)
	{
		iPointY = iH - iPointY;
	}

	m_ptPoint.x = iPointX;
	m_ptPoint.y = iPointY;
	int iX = m_ptPoint.x;
	int iY = m_ptPoint.y;

	//让箭头恰好批向m_ptPoint;
	if (m_iTipDir == XAL_TOPLEFT || m_iTipDir == XAL_BOTTOMLEFT)
	{
		iX = m_ptPoint.x -  12;
	}
	else
	{
		iX = m_ptPoint.x - 113;
	}

	if (m_iTipDir == XAL_BOTTOMLEFT || m_iTipDir == XAL_BOTTOMRIGHT)
	{
		iY = m_ptPoint.y - 88;
	}

	bool bRtn =  CControl::Create(pParent,iX,iY,m_iWidth,m_iHeight,CTRL_STYLE_AUTOSIZE);
	OnMove();
	return bRtn;
}

void CArrowTip::SetTipID(int val) 
{ 
	if (val < 0 || val >= 64)
	{
		return;
	}

	m_iTipID = val; 

	map<int,string>::iterator itr = g_OtherData.GetArrorTipsMap().find(val);
	if (itr != g_OtherData.GetArrorTipsMap().end())
	{
		SetText(itr->second.c_str());
	}

	g_pGameControl->Send_Player_Prompt_Status(m_iTipID);	
}

void CArrowTip::Draw()
{
	DWORD dwTickCout = GetTickCount();
	if (dwTickCout < m_dwStartTime)//还没有到开始时间
	{
		return;
	}	

	if (m_iTipType == 1)//指向怪物或人物
	{
		CCharacterAttr *pChar = g_pGameData->FindCharacterByID(m_dwData);
		if (!pChar || pChar->GetReserveData(pubBoneState) != 0)//怪物不见了,关闭泡泡
		{
			OnClickCloseBtn();
			return;
		}

		int iCharX,iCharY,iCharOffX,iCharOffY,iDrawOffX,iDrawOffY,iTileStartX,iTileStartY;
		pChar->GetXY(iCharX,iCharY);
		pChar->GetOffset(iCharOffX,iCharOffY);
		g_pSceneMgr->GetDrawOffXY(iDrawOffX,iDrawOffY);
		g_pSceneMgr->GetTileStartXY(iTileStartX,iTileStartY);

		int iX = (iCharX - iTileStartX)*TILE_WIDTH - iDrawOffX + iCharOffX;
		int iY = (iCharY - iTileStartY)*TILE_HEIGHT - iDrawOffY + iCharOffY;

		m_ptPoint.x = iX + TILE_WIDTH / 2;// - 96;
		m_ptPoint.y = iY + TILE_HEIGHT / 2;// - 132;
		
		OnMove();
	}
	else if (m_iTipType == 2)//指向物品
	{
		CSimpleGoodNode* pGood = g_pGameData->FindSimpleGood(m_dwData);
		if (!pGood)//怪物不见了,关闭泡泡
		{
			OnClickCloseBtn();
			return;
		}

		int tx,ty;
		int iDrawOffX,iDrawOffY,iTileStartX,iTileStartY;
		pGood->GetXY(tx,ty);
		g_pSceneMgr->GetDrawOffXY(iDrawOffX,iDrawOffY);
		g_pSceneMgr->GetTileStartXY(iTileStartX,iTileStartY);

		int iX = (tx - iTileStartX)*TILE_WIDTH - iDrawOffX;
		int iY = (ty - iTileStartY)*TILE_HEIGHT - iDrawOffY;
		
		m_ptPoint.x = iX + TILE_WIDTH / 2;
		m_ptPoint.y = iY + TILE_HEIGHT / 2;

		OnMove();
	}



	int iTextX = 4,iTextY = 19;	
	WORD wBackTex = 0;
	//画背景    
	if (m_iTipDir == XAL_BOTTOMLEFT || m_iTipDir == XAL_BOTTOMRIGHT)
	{
		iTextY = 19;
		wBackTex = (m_iTipDir == XAL_BOTTOMLEFT)?235:237;
	}
	else//XAL_TOPLEFT,XAL_BOTTOMLEFT
	{
		iTextY = 38;
		wBackTex = (m_iTipDir == XAL_TOPLEFT)?234:236;
	}

	g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,g_pTexMgr->GetTex(PACKAGE_INTERFACE,wBackTex,EP_UI));
	//画按钮
	int x = 0,y = 0;
	bool bMouseOnCloseBtn = false;
	m_pMainWnd->GetMouseXY(x,y);
	x -= m_iScreenX;
	y -= m_iScreenY;
	if( x >= 103 && y >= iTextY - 17 && x <= 103 + 11 && y <= iTextY - 17 + 10) 
		bMouseOnCloseBtn = true;

	if (g_pInput->IsLeftButton())
	{
		if (bMouseOnCloseBtn)
		{
			m_bLBClickOnCloseBtn = true;
		}
	}
	else
	{
		if (m_bLBClickOnCloseBtn)//点下又放开了
		{
			OnClickCloseBtn();
			return;
		}
	}
	WORD wClcoseBtnTex = 240;
	if (m_bLBClickOnCloseBtn)
	{
		wClcoseBtnTex = 242;
	}
	else if (bMouseOnCloseBtn)
	{
		wClcoseBtnTex = 241;
	}

	g_pGfx->DrawTextureNL(m_iScreenX + 103,m_iScreenY + iTextY - 17,g_pTexMgr->GetTex(PACKAGE_INTERFACE,wClcoseBtnTex,EP_UI));


	//画文字
	vector<string> vecInfo;
	CutByUnicode((char*)m_sText.c_str(),vecInfo,18);

	for(int i = 0; i < (int)vecInfo.size();i++)
	{
		string &str = vecInfo.at(i);
		g_pFont->DrawText(m_iScreenX + iTextX,m_iScreenY + iTextY + 14 * i,str.c_str(),0xFF000000,FONT_YAHEI,FONTSIZE_DEFAULT,0);
	}




	if(m_dwLastTime > 0 && dwTickCout - m_dwStartTime > m_dwLastTime)
	{
		CControlContainer *pWnd = dynamic_cast<CControlContainer *>(GetParent());
		if(pWnd)
		{
			CControl * pThis = this;
			pWnd->RemoveControl(&pThis,true);
		}
	}
}

bool CArrowTip::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if (!m_bNeedMsg)
	{
		return false;
	}

	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

bool CArrowTip::IsInControl(int iX,int iY)
{
	if (iY < 15 && (m_iTipDir == XAL_TOPLEFT || m_iTipDir == XAL_TOPRIGHT))
		return false;
	else if (iY > 70 && (m_iTipDir == XAL_BOTTOMLEFT || m_iTipDir == XAL_BOTTOMRIGHT))
		return false;

	return CCtrlWindow::IsInControl(iX,iY);
}

void CArrowTip::OnMove()
{
	int iParentScreenX = m_pParent->GetScreenX();
	int iParentScreenY = m_pParent->GetScreenY();

	if (m_iX + iParentScreenX < 0 && (m_iTipDir == XAL_TOPRIGHT || m_iTipDir == XAL_BOTTOMRIGHT))
	{
		m_iTipDir = ((m_iTipDir == XAL_TOPRIGHT)?XAL_TOPLEFT:XAL_BOTTOMLEFT);
	}
	else if (m_iX + m_iWidth + iParentScreenX > g_pControl->GetWidth() && (m_iTipDir == XAL_TOPLEFT || m_iTipDir == XAL_BOTTOMLEFT))
	{
		m_iTipDir = ((m_iTipDir == XAL_TOPLEFT)?XAL_TOPRIGHT:XAL_BOTTOMRIGHT);
	}

	if (m_iY + iParentScreenY < 0 && (m_iTipDir == XAL_BOTTOMLEFT || m_iTipDir == XAL_BOTTOMRIGHT))
	{
		m_iTipDir = (m_iTipDir == XAL_BOTTOMLEFT)?XAL_TOPLEFT:XAL_TOPRIGHT;
	}
	else if (m_iY + m_iHeight + iParentScreenY > g_pControl->GetHeight() &&(m_iTipDir == XAL_TOPLEFT || m_iTipDir == XAL_TOPRIGHT))
	{
		m_iTipDir = ((m_iTipDir == XAL_TOPLEFT)?XAL_BOTTOMLEFT:XAL_BOTTOMRIGHT);
	}

	if (m_iTipDir == XAL_TOPLEFT || m_iTipDir == XAL_BOTTOMLEFT)
	{
		m_iX = m_ptPoint.x -  12;
	}
	else
	{
		m_iX = m_ptPoint.x - 113;
	}

	if (m_iTipDir == XAL_BOTTOMLEFT || m_iTipDir == XAL_BOTTOMRIGHT)
	{
		m_iY = m_ptPoint.y - 88;
	}
	else
	{
		m_iY = m_ptPoint.y;
	}

	CControl::OnMove();
}

void CArrowTip::OnClickCloseBtn()
{
	if (m_pParent && m_pParent->Msg(MSG_ARROWTIP_CLICK_CLOSEBTN,m_iTipID,this))
	{
		return;
	}

	//close
	CControlContainer *pWnd = dynamic_cast<CControlContainer *>(GetParent());
	if(pWnd)
	{
		CControl * pThis = this;
		pWnd->RemoveControl(&pThis,true);
	}
}
