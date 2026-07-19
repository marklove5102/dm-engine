#include "CtrlScroll.h"
#include "GameData/GameData.h"
#include "Global/MathUtil.h"
#include "CtrlButton.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

#define INTERVAL_BUTTON_BAR 1//按钮和滑块之间保持的间隔

DTI_IMPLEMENT(CCtrlScroll, CControl)

WORD CCtrlScroll::m_wUpBtnTex[BTEX_NUMS] = {1026,1027,1028,1020};
WORD CCtrlScroll::m_wDownBtnTex[BTEX_NUMS] = {1030,1031,1032,1033};
WORD CCtrlScroll::m_wBackTex[BTEX_NUMS] = {1034,1035,1036,1037};
WORD CCtrlScroll::m_wBarTex = 1025;


CCtrlScroll::CCtrlScroll()
{
	m_iControlMode	= CTRL_MODE_SCROLL;
	m_bMoving		= false;
	m_iButtonHeight = 0;

	m_fBarX = m_fBarY = 0;
	m_iBarW = m_iBarH = 0;
	m_bVertical = true;
	m_iCurColor		= 0;

	m_iIndex  = 0;
	//m_pScrollTex = NULL;
	m_wScrollTexID = 0;
	m_iRange	= 0;
	m_pParent = NULL;
	m_iPos		= 0;

	m_bNewVersion = false;
	m_pUpButton = NULL;
	m_pDownButton = NULL;
}

CCtrlScroll::~CCtrlScroll()
{
	//g_pTexMgr->ReleaseTex(m_pScrollTex);
}

bool CCtrlScroll::Create(CControl * pParent,int iX,int iY,int iW,int iH,int iScrollIdx)
{
	if(iScrollIdx <= 0)
		iScrollIdx = 4629;

	m_wScrollTexID = iScrollIdx;

	DWORD dwIndex = MAKELONG(iScrollIdx,PACKAGE_INTERFACE);
	LPTexture pTex = g_pTexMgr->GetTexImm(dwIndex,EP_UI);

	if(pTex)
	{
		m_iBarW = pTex->GetWidth();
		m_iBarH = pTex->GetHeight();

		if(m_bVertical)
			iW = pTex->GetWidth();
		else
			iH = pTex->GetHeight();
	}
	else	//无图片按钮
	{
		m_iBarW = iW;
		m_iBarH	= min(16, max(4, iH/8));
	}

	if(m_iIndex > 0)
	{
		dwIndex = MAKELONG(m_iIndex, PACKAGE_INTERFACE);
		//m_pBackTex = g_pTexMgr->LoadTex(dwIndex);
	}

	UINT uFlag = CTRL_STYLE_SELFDELETE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX | CTRL_STYLE_BACKMODE_TEX;

	return	CControl::Create(pParent,iX,iY,iW,iH,uFlag,0xFFFFFFFF,0);
}

void  CCtrlScroll::SetScrollTex(WORD wUpTex,WORD wUpMTex,WORD wUpCTex,WORD wUpDTex,WORD wDownTex,WORD wDownMTex,WORD wDownCTex,WORD wDownDTex,WORD wBarTex,WORD wBarMTex,WORD wBarCTex,WORD wBarDTex,WORD wBackTex)
{
	m_wUpBtnTex[BTEX_NORMAL] = wUpTex;
	m_wUpBtnTex[BTEX_HIGHLIGHT] = wUpMTex;
	m_wUpBtnTex[BTEX_PUSHED] = wUpCTex;
	m_wUpBtnTex[BTEX_DISABLED] = wUpDTex;

	m_wDownBtnTex[BTEX_NORMAL] = wDownTex;
	m_wDownBtnTex[BTEX_HIGHLIGHT] = wDownMTex;
	m_wDownBtnTex[BTEX_PUSHED] = wDownCTex;
	m_wDownBtnTex[BTEX_DISABLED] = wDownDTex;

	m_wBackTex[BTEX_NORMAL] = wBarTex;
	m_wBackTex[BTEX_HIGHLIGHT] = wBarMTex;
	m_wBackTex[BTEX_PUSHED] = wBarCTex;
	m_wBackTex[BTEX_DISABLED] = wBarDTex;

	m_wBarTex = wBackTex;
}

bool CCtrlScroll::CreateEx(CControl * pParent,int iX,int iY,int iW,int iH)
{
	m_bNewVersion = true;
	m_iButtonHeight = 16 + INTERVAL_BUTTON_BAR;
	DWORD dwIndex = MAKELONG(m_wBackTex[BTEX_NORMAL],PACKAGE_INTERFACE);
	m_wScrollTexID = m_wBackTex[BTEX_NORMAL];
	
	if (m_bVertical)
	{
		m_fBarY = (float)m_iButtonHeight;
	}
	else
	{
		m_fBarX = (float)m_iButtonHeight;
	}

	LPTexture pTex = g_pTexMgr->GetTexImm(dwIndex,EP_UI);
	if(pTex)
	{
		m_iBarW = pTex->GetWidth();
		m_iBarH = pTex->GetHeight();

		if(m_bVertical)
			iW = pTex->GetWidth();
		else
			iH = pTex->GetHeight();
	}
	else	//无图片按钮
	{
		m_iBarW = iW;
		m_iBarH	= min(16, max(4, iH/8));
	}

	pTex = g_pTexMgr->GetTexImm(MAKELONG(m_wUpBtnTex[BTEX_NORMAL],PACKAGE_INTERFACE),EP_UI);
	if (pTex)
	{
		m_iHeight = pTex->GetHeight0();
	}

	if (m_bVertical)
	{
		if (m_iBarH > (iH - 2 * m_iButtonHeight) / 2)
		{
			m_iBarH = (iH - 2 * m_iButtonHeight) / 2;
		}
		if (m_iBarH < 5)
		{
			m_iBarH = 5;
		}
	}
	else
	{
		if (m_iBarW > (iW - 2 * m_iButtonHeight) / 2)
		{
			m_iBarW = (iW - 2 * m_iButtonHeight) / 2;
		}
		if (m_iBarW < 5)
		{
			m_iBarW = 5;
		}
	}


	UINT uFlag = CTRL_STYLE_SELFDELETE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX | CTRL_STYLE_BACKMODE_TEX;

	return	CControl::Create(pParent,iX,iY,iW,iH,uFlag,0xFFFFFFFF);
}



bool CCtrlScroll::CreateXML(CControl* pParent,CXmlScroll* scroll)
{
	if(!pParent || !scroll)
		return false;

	int iScrollIdx = scroll->GetScroll().GetIndex();
	int iW = 0,iH = 0;
	scroll->GetSize(iW,iH);

	AssignXML(scroll);

	if(!this->Create(pParent,0,0,iW,iH,iScrollIdx))
		return false;

	return true;
}

void CCtrlScroll::Draw(void)
{
	if (m_bNewVersion)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_wBarTex,EP_UI);
		if (pTex)
		{
			POS scale((float)(m_iWidth) / pTex->GetWidth(),(float)(m_iHeight - m_iButtonHeight * 2 + 1) / pTex->GetHeight());
			g_pGfx->DrawTextureFX(m_iScreenX + 1,m_iScreenY + m_iButtonHeight,pTex,-1,NULL,&scale);
		}
	}


	CControlContainer::Draw();

	if(!m_bShow)
		return;

	SPoint sPos(m_iScreenX + (int)m_fBarX,m_iScreenY + (int)m_fBarY);

	int iMouseX = 0, iMouseY = 0;
	g_pControl->GetMouseXY(iMouseX, iMouseY);
	iMouseX -= m_iScreenX;
	iMouseY -= m_iScreenY;

	if(iMouseX > 0 && iMouseX < m_iWidth && iMouseY > 0 && iMouseY < m_iHeight)
		m_bMouseOn	= true;
	else
		m_bMouseOn	= false;

	bool bMouseOnBar;
	if(m_bMouseOn)
	{
		bMouseOnBar = (iMouseX>m_fBarX && iMouseX<(m_fBarX+m_iBarH) && iMouseY>m_fBarY && iMouseY<(m_fBarY+m_iBarH));
	}
	else
	{
		bMouseOnBar = false;
	}

	LPTexture pTex = NULL;
	float fZoom = 1.0f;//
	if (m_wScrollTexID)
	{
		pTex = g_pTexMgr->GetTexImm(MAKELONG(m_wScrollTexID,PACKAGE_INTERFACE),EP_UI);
		if (pTex)
		{
			fZoom = float(m_bVertical?m_iBarH:m_iBarW)/float(pTex->GetHeight());


			if (m_iBarW <= 0)
			{
				m_iBarW = pTex->GetWidth();
			}
			if (m_iBarH <= 0)
			{
				m_iBarH = pTex->GetHeight();
			}
		}
	}

	LPTexture pBarTex = pTex;

	if(pTex)
	{
		if (m_bNewVersion)
		{
			if (!m_bEnable)
			{
				pBarTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_wBackTex[BTEX_DISABLED],EP_UI);
			}
			else if(bMouseOnBar || m_bMoving)
			{
				pBarTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_bMoving?m_wBackTex[BTEX_PUSHED]:m_wBackTex[BTEX_HIGHLIGHT],EP_UI);
			}

			if (m_bVertical)
			{
				DrawTextureZoom((int)m_fBarX + 1, (int)m_fBarY, pBarTex, 1.0f, fZoom);
			}
			else
			{
				DrawTextureZoom((int)m_fBarX, (int)m_fBarY, pBarTex, fZoom, 1.0f);
			}
		}
		else
		{
			DrawTexture((int)m_fBarX, (int)m_fBarY, pTex);
		}
	}
	else
	{
		DWORD dwCtrlColor = 0xFFB0B0B0;
		if(m_bMouseOn)
		{
			if(m_bClick)
			{
				dwCtrlColor -= 0x00202020;
			}
			else
			{
				dwCtrlColor += 0x00202020;
			}
		}
		g_pGfx->DrawFillRect(m_iScreenX, m_iScreenY,m_iWidth,m_iHeight,0xD0808080);
		g_pGfx->DrawFillRect(m_iScreenX+(int)m_fBarX, m_iScreenY+(int)m_fBarY,m_iBarW, m_iBarH,dwCtrlColor);

		DWORD dwLColor = dwCtrlColor+0x00303030;
		DWORD dwDColor = dwCtrlColor-0x00505050;

		SPoint pt1;
		pt1.x = m_iScreenX+(int)m_fBarX;
		pt1.y = m_iScreenY+(int)m_fBarY;
		SPoint pt2 = pt1;
		pt2.x += m_iBarW;
		g_pGfx->DrawLine(pt1.y, pt1.x, pt2.x, m_bClick?dwDColor:dwLColor, TRUE);
		pt1.y += m_iBarH;
		pt2.y += m_iBarH;
		g_pGfx->DrawLine( pt1.y, pt1.x, pt2.x, m_bClick?dwDColor:dwLColor, TRUE);
	}
}

void CCtrlScroll::OnCreate(void)
{
	ReSize(GetWidth(), GetHeight());

	m_iStep  = 4;
	if (m_bNewVersion)
	{
		if(m_pUpButton)
			RemoveControl((CControl**)&m_pUpButton);

		m_pUpButton = new CCtrlButton();
		AddControl(m_pUpButton);
		m_pUpButton->CreateEx(this,0,0,m_wUpBtnTex[BTEX_NORMAL],m_wUpBtnTex[BTEX_HIGHLIGHT],m_wUpBtnTex[BTEX_PUSHED],m_wUpBtnTex[BTEX_DISABLED]);

		if(m_pDownButton)
			RemoveControl((CControl**)&m_pDownButton);

		m_pDownButton = new CCtrlButton();
		AddControl(m_pDownButton);
		m_pDownButton->CreateEx(this,0,m_iHeight - m_iButtonHeight + INTERVAL_BUTTON_BAR,m_wDownBtnTex[BTEX_NORMAL],m_wDownBtnTex[BTEX_HIGHLIGHT],m_wDownBtnTex[BTEX_PUSHED],m_wDownBtnTex[BTEX_DISABLED]);
	}

}

bool CCtrlScroll::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pUpButton)
			{
				GoDown();
				return true;
			}
			else if(pControl == m_pDownButton)
			{
				GoUp();
				return true;
			}
		}
		break;
	default:
		break;
	}

	return CControlContainer::Msg(dwMsg,dwData,pControl);
}

bool CCtrlScroll::OnLeftButtonDown(int iX, int iY)
{
	if(iX > m_fBarX && iX < m_fBarX + m_iBarW && iY > m_fBarY && iY < m_fBarY + m_iBarH)
	{
		m_bMoving = true;
		m_iLastMousePosX = iX;
		m_iLastMousePosY = iY;
	}
	else if(iX > 0 && iX < m_iWidth && m_bVertical)
	{
		if(iY > 0 && iY < m_fBarY)
		{
			GoDown();
		}
		else if(iY > m_fBarY + m_iBarH && iY < m_iHeight)
		{
			GoUp();
		}
	}
	else if( iY > 0 && iY < m_iHeight && !m_bVertical)
	{
		if(iX > 0 && iX < m_fBarX)
		{
			GoDown();
		}
		else if( iX > m_fBarX + m_iBarW && iX < m_iWidth)
		{
			GoUp();
		}
	}
	return CControl::OnLeftButtonDown(iX,iY) ;
}

bool CCtrlScroll::ReSize(int iW, int iH)
{
	if(iW < 0)
		iW = m_iWidth;
	if(iH < 0)
		iH = m_iHeight;

	int iOldPos = GetPos();
	if(!m_wScrollTexID)
	{
		m_iBarW = iW;
		m_iBarH	= min(16, max(4, iH/8));
	}
	if(m_bVertical)
	{
		m_iScrollRange = iH - 2 * m_iButtonHeight - m_iBarH;
	}
	else
	{
		m_iScrollRange = iW - 2 * m_iButtonHeight - m_iBarW;
	}
	m_iScrollRange = max(1, m_iScrollRange);
	SetPos(iOldPos);
	return CControl::ReSize(iW, iH);
}

bool CCtrlScroll::OnLeftButtonUp(int iX, int iY)
{
	m_bMoving = false;
	return CControl::OnLeftButtonUp(iX,iY) ;	
}

bool CCtrlScroll::OnMouseMove(int iX, int iY)
{
	if(m_bMoving)
	{
		int iOldPos = GetPos();
		if(m_bVertical)
		{
			//m_fBarY = (float)(iY - m_iButtonHeight);
			m_fBarY += (iY - m_iLastMousePosY);
			if(m_fBarY > m_iHeight - m_iBarH - m_iButtonHeight)
			{
				m_fBarY =(float)(max(0, m_iHeight - m_iBarH - m_iButtonHeight));
			}
			if(m_fBarY < m_iButtonHeight)
			{
				m_fBarY = (float)m_iButtonHeight;
			}
		}
		else
		{
			//m_fBarX =(float)(iX - m_iButtonHeight);
			m_fBarX += (iX - m_iLastMousePosX);
			if(m_fBarX > m_iWidth - m_iBarW - m_iButtonHeight)
			{
				m_fBarX = (float)(m_iWidth - m_iBarW - m_iButtonHeight);
			}
			if(m_fBarX < m_iButtonHeight)
			{
				m_fBarX = (float)m_iButtonHeight;
			}
		}
		int iNewPos = GetPos();
		if(iNewPos != iOldPos)
		{
			if(m_pParent)
			{
				m_pParent->Msg(MSG_CTRL_SCROLL_MOVE, iNewPos, this);
			}
		}
		m_iLastMousePosX = iX;
		m_iLastMousePosY = iY;
	}
	return true;
}

void CCtrlScroll::GoUp(void)
{
	int iOldPos = GetPos();
	if(m_iRange == 0)
	{
		if(m_bVertical)
		{
			m_fBarY = (float)(m_iButtonHeight + m_iScrollRange);
		}
		else
		{
			m_fBarX = (float)(m_iButtonHeight + m_iScrollRange);
		}
		return;
	}
	if(m_bVertical)
	{
		m_fBarY += max(m_iStep * m_iScrollRange/m_iRange, 1);
		if(m_fBarY > m_iButtonHeight + m_iScrollRange)
		{
			m_fBarY = (float)(m_iButtonHeight + m_iScrollRange);
		}
	}
	else
	{
		m_fBarX += max(m_iStep * m_iScrollRange/m_iRange, 1);
		if(m_fBarX > m_iButtonHeight + m_iScrollRange)
		{
			m_fBarX = (float)(m_iButtonHeight + m_iScrollRange);
		}
	}
	int iNewPos = GetPos();
	if(iNewPos != iOldPos)
	{
		if(m_pParent)
		{
			m_pParent->Msg(MSG_CTRL_SCROLL_MOVE, iNewPos, this);
		}
	}
}

void CCtrlScroll::GoDown(void)
{
	int iOldPos = GetPos();
	if(m_iRange == 0)
	{
		if(m_bVertical)
		{
			m_fBarY = (float)m_iButtonHeight;
		}
		else
		{
			m_fBarX = (float)m_iButtonHeight;
		}
		return;
	}
	if(m_bVertical)
	{
		m_fBarY -= (float)(max(m_iStep * m_iScrollRange/m_iRange, 1));
		if(m_fBarY < m_iButtonHeight )
		{
			m_fBarY = (float)m_iButtonHeight;
		}
	}
	else
	{
		m_fBarX -= max(m_iStep * m_iScrollRange/m_iRange, 1);
		if(m_fBarX < m_iButtonHeight)
		{
			m_fBarX = (float)m_iButtonHeight;
		}
	}
	int iNewPos = GetPos();
	if(iNewPos != iOldPos)
	{
		if(m_pParent)
		{
			m_pParent->Msg(MSG_CTRL_SCROLL_MOVE, iNewPos, this);
		}
	}
}

bool CCtrlScroll::OnWheel(int iWheel)
{
	short iWheelSize = (short)iWheel;
	iWheelSize /= WHEEL_DELTA;
	if(m_iRange == 0) return true;
	int tmp = iWheelSize * m_iScrollRange/m_iRange;
	if(tmp == 0)
	{
		tmp = ((short)iWheel > 0)?1:-1;
	}
	if(m_bVertical)
	{
		int iNewY = (int)m_fBarY;
		iNewY -= tmp;
		if(iNewY > m_iButtonHeight + m_iScrollRange)
			iNewY = m_iButtonHeight + m_iScrollRange;
		if(iNewY < m_iButtonHeight )
			iNewY = m_iButtonHeight;
		if(iNewY != m_fBarY)
		{
			m_fBarY = (float)iNewY;
			if(m_pParent)
			{
				m_pParent->Msg(MSG_CTRL_SCROLL_MOVE, GetPos(), this);
			}
		}
	}
	else
	{
		int iNewX = (int)m_fBarX;
		iNewX -= tmp;
		if(iNewX > m_iButtonHeight + m_iScrollRange)
			iNewX = m_iButtonHeight + m_iScrollRange;
		if(iNewX < m_iButtonHeight )
			iNewX = m_iButtonHeight;
		if(iNewX != m_fBarX)
		{
			m_fBarX = (float)iNewX;
			if(m_pParent)
			{
				m_pParent->Msg(MSG_CTRL_SCROLL_MOVE, GetPos(), this);
			}
		}
	}
	return true;
}

void CCtrlScroll::SetButtonHeight(int iHeight)
{
	m_iButtonHeight = iHeight;
	if(m_bVertical)
	{
		m_fBarY = (float)m_iButtonHeight;
	}
	else
	{
		m_fBarX = (float)m_iButtonHeight;
	}
	OnCreate();
}


void CCtrlScroll::SetPos(int iPos)
{
	if(iPos == m_iPos)
		return;

	if(m_bVertical)
	{
		m_fBarY = (float)m_iButtonHeight;
	}
	else
	{
		m_fBarX = (float)m_iButtonHeight;
	}
	m_iPos = min(m_iRange, max(0, iPos));
	if(m_iRange)
	{
		float iOff = (((float)iPos * m_iScrollRange)/m_iRange);
		if(m_bVertical)
		{
			m_fBarY += iOff;
		}
		else
		{
			m_fBarX += iOff;
		}
	}
}


int CCtrlScroll::GetPos()
{
	if(m_iScrollRange == 0)
		return 0;

	//获得并返回当前滑块位置,但是诸如位置3会变成2.9xxx,进而变成2,所以+0.5是4舍5入,以得到正确的预期结果
	bool bEnable = m_bVertical?(m_iHeight>m_iBarH):(m_iWidth>m_iBarW);
	SetEnable(bEnable);

	m_iPos = ((int)((((m_bVertical?m_fBarY:m_fBarX) -m_iButtonHeight) * ((float)m_iRange)/m_iScrollRange)+0.5f));
	if (m_iPos < 0)
	{
		m_iPos = 0;
	}

	return m_iPos;
}


void CCtrlScroll::SetRange(int iRange, int iDisplayRange)
{
	if(iRange == m_iRange && m_iDisplayRange == iDisplayRange) return;
	m_iRange = max(0, iRange);

	if(m_iRange == 0)
	{
		SetPos(0);
	}
	else
	{
		SetPos(((int)((float)GetPos() * iRange / m_iRange + 0.5f)));
	}

// 	if(iDisplayRange >= 0 && m_bNewVersion)
// 	{
// 		if(iDisplayRange == 0)
// 		{
// 			if(m_bVertical)
// 				m_iDisplayRange = m_iHeight/14;
// 			else
// 				m_iDisplayRange = m_iWidth/14;
// 		}
// 		else
// 			m_iDisplayRange = iDisplayRange;
// 
// 		if(m_bVertical)
// 		{
// 			m_iBarH = int(float(m_iScrollRange * m_iDisplayRange) / float(m_iRange));
// 			m_iBarH = 10 > m_iBarH ? 13:m_iBarH;
// 		}
// 		else
// 		{
// 			m_iBarW = int(float(m_iScrollRange * m_iDisplayRange) / float(m_iRange));
// 			m_iBarW = 10 > m_iBarW ? 13:m_iBarW;
// 		}
// 	}

	bool bEnable = m_bVertical?(m_iHeight>m_iBarH):(m_iWidth>m_iBarW);

	SetEnable(bEnable);
}

void CCtrlScroll::ReloadScrollTex(WORD wTex)
{
	//DWORD dwIndex = MAKELONG(wTex,PACKAGE_INTERFACE);
	//if(m_pScrollTex && m_pScrollTex->GetID() == dwIndex)
	//	return;

	//if(m_pScrollTex)
	//	g_pTexMgr->ReleaseTex(m_pScrollTex);

	//if(wTex <= 0)
	//{
	//	dwIndex = MAKELONG(4629,PACKAGE_INTERFACE);
	//}

	//m_pScrollTex = g_pTexMgr->LoadTex(dwIndex);

	m_wScrollTexID = wTex;
}
