#include "ctrlmenubutton.h"

#pragma warning(disable : 4018)

static  DWORD MB_COLOR_TABLE[] =
{
	0xFFFF0000,
	0xFF00FF00,
	0xFFFFFF00,
	0xFF0000FF,
	0xFFFF00FF,
	0xFF548EF6,
	0xFFC19BF4,
	0xFFEFAE00,
	0xFFFFFFFF,
	0xFF50A600,
	0xFF92312F,
	0xFFC0C0C0,
	0xFF747474,
	0xFF404040,
	0xFF000000
};

CCtrlMenuButton::CCtrlMenuButton()
{
	m_bMenu = false;
	m_pScroll = NULL;
	m_pButton = NULL;

	m_dwMenuTextColor = 0xFFFFFFFF;
	m_dwMenuSelTextColor = 0xFFFFFF00;
	m_dwMenuSelBackColor = 0x660000FF;
	m_iBeginPos = 0;
	m_iCurSel = -1;
	m_iMouseOnLine = -1;
	m_iDisplayLines = 6;
	m_bVAlignTop = false;
	m_bColor = false;
	m_iItemHeight = m_iFontSize + 2;
	m_wMenuBackTex = 0;
	m_dwBackColor = 0xFF000000;
	m_pParentMask = NULL;
	m_iBtnOffX = 0;
	m_iBtnOffY = 0;


	m_iControlMode = CTRL_MODE_MENU_BUTTON;//该控件不能失去焦点，一失去就关闭
}

CCtrlMenuButton::~CCtrlMenuButton(void)
{
}

bool CCtrlMenuButton::Create(CControl* pParent,int iX,int iY,int iW,int iH,WORD wMenuBackTex,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex,WORD wBtnTex,WORD wBtnMTex,WORD wBtnCTex,WORD wBtnDTex,int iMenuOffX,int iMenuOffY,bool bColor,int iBtnOffX,int iBtnOffY)
{
	if(!pParent ) 
		return false;

	m_iOffX = iMenuOffX;
	m_iOffY = iMenuOffY;
	m_iBtnOffX = iBtnOffX;
	m_iBtnOffY = iBtnOffY;

	m_vBackTexID[BTEX_NORMAL] = wTex;
	m_vBackTexID[BTEX_HIGHLIGHT] = wMTex;
	m_vBackTexID[BTEX_PUSHED] = wCTex;
	m_vBackTexID[BTEX_DISABLED] = wDTex;

	m_vBtnTexID[BTEX_NORMAL] = wBtnTex;
	m_vBtnTexID[BTEX_HIGHLIGHT] = wBtnMTex;
	m_vBtnTexID[BTEX_PUSHED] = wBtnCTex;
	m_vBtnTexID[BTEX_DISABLED] = wBtnDTex;

	m_wMenuBackTex = wMenuBackTex;

	m_iWidth	= iW;                        // 宽度
	m_iHeight	= iH;                        // 高度
	

	if(!CControl::Create(pParent,iX,iY,iW,iH,CTRL_STYLE_DEFAULT,m_dwBackColor))
		return false;

	m_iParentW = pParent->GetWidth();
	m_iParentH = pParent->GetHeight();

	if (bColor)
	{
		char szColor[32] = {0};
		int iColorCount = sizeof(MB_COLOR_TABLE) / sizeof(DWORD);

		for(int i = 0; i < iColorCount ;i++)
		{
			ltoa(MB_COLOR_TABLE[i],szColor,16);
			this->AddString(szColor);
		}
		m_bColor = true;
		//不显示文字
		m_iFont = 0;
		m_iFontSize = 0;
	}

	return true;
}

void CCtrlMenuButton::Draw()
{
	if(!m_bShow) 
		return;

	DrawMenu();
	CControlContainer::Draw();
}

bool CCtrlMenuButton::KillFocus(CControl * pNewFocus)
{
	bool b = CControl::KillFocus(pNewFocus);

	CControl *p = pNewFocus;
	bool bIsFocusOnSonOrSelf = false;
	while (p)
	{
		if (p == this)
		{
			bIsFocusOnSonOrSelf = true;
			break;
		}

		p = p->GetParent();
	}

	if(b && pNewFocus && !bIsFocusOnSonOrSelf)
	{
		OpenOrCloseMenu(false);
		m_bClick = m_bRBClick = false;
	}

	if (b && pNewFocus == m_pParent)
	{
		int iX = g_pControl->GetMouseX() - m_pParent->GetScreenX();
		int iY = g_pControl->GetMouseY() - m_pParent->GetScreenY();

		if( !m_pParent->IsInControl(iX,iY) )
			return false;
	}

	return b;
}

void CCtrlMenuButton::DrawMenu()
{
	if( m_bMouseOn )
	{
		int x,y;
		m_pMainWnd->GetMouseXY(x,y);
		x -= m_iScreenX;
		y -= m_iScreenY;
		if( x<0 || y<0 || x>=m_iWidth || y>=m_iHeight ) 
			m_bMouseOn = false;
	}

	if(m_pMainWnd && m_bMouseOn && !m_sTips.empty())
		m_pMainWnd->SetMouseOnID(MOUSE_ON_CONTROL,this);

	int iState;
	if(!m_bEnable && m_vBackTexID[3])
		iState = 3;
	else if(m_bClick )
		iState = 2;
	else if(m_bMouseOn)
		iState = 1;
	else
		iState = 0;


	//背景
	WORD wBackTexID = m_vBackTexID[iState];
	if (wBackTexID == 0)
	{
		wBackTexID = m_vBackTexID[0];
	}

	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,wBackTexID,EP_UI);
	if (pTex)
	{
		POS scale((float)(m_iWidth) / pTex->GetWidth(),(float)(m_iOffY) / pTex->GetHeight());
		g_pGfx->DrawTextureFX(m_iScreenX,m_iScreenY,pTex,-1,NULL,&scale);
	}
	else
	{
		g_pGfx->DrawRect(m_iScreenX,m_iScreenY,m_iWidth ,m_iOffX,0xFF8C7C40,1);
		g_pGfx->DrawFillRect(m_iScreenX + 1,m_iScreenY + 1,m_iWidth - 2,m_iOffX - 2,m_dwBackColor);
	}


	if (!m_bMenu)
		return;

	//弹出菜单背景
	if (m_wMenuBackTex)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_wMenuBackTex,EP_UI);
		if (pTex)
		{
			POS scale((float)(m_iWidth) / pTex->GetWidth(),(float)(m_iHeight) / pTex->GetHeight());
			g_pGfx->DrawTextureFX(m_iScreenX + m_iOffX,m_iScreenY + m_iOffY,pTex,-1,NULL,&scale);
		}
	}
	else
	{
		g_pGfx->DrawRect(m_iScreenX + m_iOffX,m_iScreenY + m_iOffY,m_iWidth - m_iOffX,m_iHeight - m_iOffY,0xFF8C7C40,1);
		g_pGfx->DrawFillRect(m_iScreenX + m_iOffX + 1,m_iScreenY + m_iOffY + 1,m_iWidth - m_iOffX - m_iButtonWidth - 2,m_iHeight - m_iOffY - 2,m_dwBackColor);
	}

	//文字
	int iFontSize = FONTSIZE_DEFAULT;
	int iSize = m_vecCNT.size();

	m_iBeginPos = m_pScroll->GetPos();
	int iY = m_iScreenY + m_iOffY;
	for(int i=m_iBeginPos;i< iSize && i<m_iBeginPos + m_iDisplayLines;i++)
	{
		if (m_vecCNT[i].empty())
			continue;

		int iX = m_iScreenX + m_iOffX + 1;

		DWORD dwColor = m_dwMenuTextColor;
		if (i == m_iMouseOnLine)
		{
			if(m_bColor)
			{
				g_pGfx->DrawFillRect(iX,iY+3,m_iWidth - m_iButtonWidth - 2,m_iItemHeight,0xFF000000);
			}
			else
			{
				g_pGfx->DrawFillRect(iX,iY+3,m_iWidth - m_iButtonWidth - 2,m_iItemHeight,m_dwMenuSelBackColor);
				dwColor = m_dwMenuSelTextColor;
			}
		}

		if(m_bColor)
		{
			DWORD dwColor = (strtoul(m_vecCNT[i].c_str(),NULL,16));
			g_pGfx->DrawFillRect(iX,iY + 4,m_iWidth - m_iButtonWidth - 2,m_iItemHeight - 2,dwColor);
		}
		else
		{
			g_pFont->DrawText(iX,iY+3,m_vecCNT[i].c_str(),dwColor,m_iFont,m_iFontSize);
		}

		iY += m_iItemHeight;
	}
}

void CCtrlMenuButton::AddString(const char* strStr,bool bEnable)
{
	if (NULL == strStr)
		return;

	m_vecCNT.push_back(strStr);
	m_vEnable.push_back(bEnable);

	bool bIsEnableOld = m_pScroll->IsEnable();
	m_pScroll->SetRange(m_vecCNT.size() - m_iDisplayLines);

	if (!bIsEnableOld)
	{
		m_pScroll->SetEnable(false);
	}
}

void CCtrlMenuButton::ResetContent()
{
	m_vecCNT.clear();
	m_vEnable.clear();
	m_iBeginPos = 0;
	m_pScroll->SetPos(0);
	m_pScroll->SetRange(0);
}

bool CCtrlMenuButton::IsInMenuArea(int iX,int iY)
{
	if (!m_bMenu || !m_bEnable)
		return false;

	int x = iX- m_iScreenX;
	int y = iY- m_iScreenY;

	if (x >= m_iOffX && x<= m_iWidth - 18 && y >= m_iOffY && y<= m_iHeight)//-18,右边滚动符的宽度
		return true;

	return false;
}

int CCtrlMenuButton::GetMouseLine(int iX,int iY)
{
	if (!m_bMenu || !m_bEnable)
		return -1;

	int x = iX - m_iScreenX - m_iOffX;
	int y = iY - m_iScreenY - m_iOffY;

	if (x >= 0 && x<= m_iWidth && y >= 0 && y <= m_iHeight)
	{
		int iSel = y / m_iItemHeight;
		if (iSel >= m_vecCNT.size() - m_iBeginPos)
			return -1;

		return iSel+m_iBeginPos;
	}
	return -1;
}

void CCtrlMenuButton::OpenOrCloseMenu(bool bOpen)
{
	if (m_bMenu == bOpen)
	{
		return ;
	}

	m_bMenu = bOpen;
	m_pScroll->SetShow(bOpen);
	m_pScroll->SetEnable(bOpen);

	if(bOpen)
	{
		m_iParentW = m_pParent->GetWidth();
		m_iParentH = m_pParent->GetHeight();
		m_pParentMask = m_pParent->GetMaskBuf();

		if (this->GetX() + m_iWidth > m_iParentW)
		{
			m_pParent->SetWidth(this->GetX() + m_iWidth);
		}
		if (this->GetY() + m_iHeight > m_iParentH)
		{
			m_pParent->SetHeight(this->GetY() + m_iHeight);
		}

		m_pParent->OnReSize();
		m_pParent->SetMaskBuf(NULL);
	}
	else
	{
		m_pParent->SetWidth(m_iParentW);
		m_pParent->SetHeight(m_iParentH);

		if (m_pParentMask)
		{
			m_pParent->SetMaskBuf(m_pParentMask);
		}
	}

}

bool CCtrlMenuButton::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	WORD w1 = HIWORD(dwData);
	WORD w2 = LOWORD(dwData);
	int x = (short)w2;
	int y = (short)w1;

	if (!m_bEnable)
		return CControlContainer::Msg(dwMsg,dwData,pControl);

	//菜单打开时
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pButton)
			{
				if (!m_bMenu)
				{
					if (m_vecCNT.size()>0)
					{
						OpenOrCloseMenu(true);
						m_iBeginPos = 0;

						//点了以后要把这个控件放到最前，画在最上面，免得被其它控件覆盖
						CControlContainer * pParent = dynamic_cast<CControlContainer *>(this->GetParent());
						if(pParent)
						{
							CControl *p, *prev;
							p = pParent->GetControls();
							prev = NULL;

							while( p && !p->IsNoChangeLevel()) 
							{
								if(this == p)
								{
									CControl *pNext = p->GetControlNext();
									if( prev ) 
										prev->SetControlNext(pNext);
									if( pNext )	
										pNext->SetControlPrev(prev);

									if( pParent->GetControls() == p ) 
										pParent->SetControls(p->GetControlNext());

									pParent->GetControls()->SetControlPrev(this);
									this->SetControlNext(pParent->GetControls());
									this->SetControlPrev(NULL);
									pParent->SetControls(this);
									break;
								}
								else
									prev = p;

								p = p->GetControlNext();
							}

						}
					}

					m_pParent->Msg(MSG_CTRL_BUTTON_CLICK,INPUT_MOUSE_LEFTBT,this);

				}
				else
				{
					OpenOrCloseMenu(false);
				}	

				return true;
			}
		}
	case MSG_INPUT_WHEEL:
		{
			if (IsInMenuArea(x,y))
				return OnWheel((short)w2);
			break;
		}
	case MSG_INPUT_MOVE:
		{
			if (IsInMenuArea(x,y))
			{
				int iSel = GetMouseLine(x,y);
				if (iSel >= 0)
				{
					if (m_iMouseOnLine != iSel)
					{
						m_iMouseOnLine = iSel;
						m_pParent->Msg(MSG_CTRL_MENU_SELCHANGING,m_iCurSel,this);
					}
					return true;
				}

				return true;
			}
			else
			{
				//禁用时
				if (!m_bEnable || !m_pButton)
					return false;
				//菜单没有弹出来,只有下拉按钮可以响应消息
				if (!m_bMenu && !(x >= m_pButton->GetScreenX() && x <= m_pButton->GetScreenX() + m_pButton->GetWidth() && y >= m_pButton->GetScreenY() && y <= m_pButton->GetScreenY() + m_pButton->GetHeight()))
					return false;

				return CControlContainer::Msg(dwMsg,dwData,pControl);
			}
			break;
		}
	case MSG_INPUT_LEFTBT_UP:
		{
			if(IsInMenuArea(x,y))
			{
				int iSel = GetMouseLine(x,y);
				if (iSel >= 0)
				{
					SetCurSel(iSel);
					m_pParent->Msg(MSG_CTRL_MENU_SELCHANGED,m_iCurSel,this);
					OpenOrCloseMenu(false);
					m_bClick = false;
					return true;
				}

				return true;
			}
			return CControlContainer::Msg(dwMsg,dwData,pControl);
		}
	case MSG_INPUT_RIGHTBT_UP:
		{
			if(IsInMenuArea(x,y))
			{
				int iSel = GetMouseLine(x,y);
				if (iSel >= 0)
				{
					SetCurSel(iSel);
					m_pParent->Msg(MSG_CTRL_MENU_RSELCHANGED,m_iCurSel,this);
					OpenOrCloseMenu(false);
					m_bRBClick = false;
					return true;
				}
				return true;
			}
			return CControlContainer::Msg(dwMsg,dwData,pControl);
		}
	}

	return CControlContainer::Msg(dwMsg,dwData,pControl);
}

bool CCtrlMenuButton::OnLeftButtonDown(int iX,int iY)
{
	if (IsInMenuArea(iX + m_iScreenX,iY + m_iScreenY))
	{
		return CControl::OnLeftButtonDown(iX,iY);
	}

	return false;
}

int CCtrlMenuButton::GetCurSel()
{
	if (m_iCurSel >= m_vecCNT.size())
	{
		return -1;
	}

	return m_iCurSel;
}

void CCtrlMenuButton::SetCurSel(int i)
{
	m_iCurSel = i;

	if(m_iCurSel>=0 && m_iCurSel<m_vecCNT.size() && m_pButton)
	{
		if (m_bColor)
		{
			SetText(m_vecCNT.at(m_iCurSel).c_str(),0,0,0,0);
		}
		else
		{
			SetText(m_vecCNT.at(m_iCurSel).c_str(),m_pButton->GetColor(),m_pButton->GetMouseOnColor(),m_pButton->GetClickColor(),m_pButton->GetDisableColor(),m_pButton->GetFontSize(),m_pButton->GetFontFlag(),m_pButton->GetFont());
		}		
	}
}

void CCtrlMenuButton::SetCurSel(const string &strSelText)
{
	int iSize = m_vecCNT.size();

	for(int i = m_iBeginPos;i < iSize;i++)
	{
		if(m_vecCNT[i] == strSelText)
			return SetCurSel(i);
	}
}

const char* CCtrlMenuButton::GetSelText()
{
	if(m_iCurSel>=0 && m_iCurSel<m_vecCNT.size())
		return m_vecCNT.at(m_iCurSel).c_str();

	return "";
}

void CCtrlMenuButton::SetMenuTextColor(DWORD dwColor)
{
	m_dwMenuTextColor = dwColor;
}

void CCtrlMenuButton::SetSelColor(DWORD dwSelTextColor,DWORD dwSelColor)
{
	m_dwMenuSelBackColor = dwSelColor;
	m_dwMenuSelTextColor = dwSelTextColor;
}

void CCtrlMenuButton::OnCreate()
{
	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,m_iWidth - 18,m_iOffY,18,m_iHeight - m_iOffY);
	m_pScroll->SetShow(false);
	m_pScroll->SetEnable(false);

	m_iDisplayLines = (m_iHeight - m_iOffY - 6) / m_iItemHeight;
	if(m_iDisplayLines <= 0)
		m_iDisplayLines = 1;

	m_iButtonWidth = 18;
	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_vBtnTexID[BTEX_NORMAL],EP_UI);
	if (pTex)
	{
		m_iButtonWidth = pTex->GetWidth0();
	}

	m_pButton = new CCtrlButton();
	AddControl(m_pButton);
	m_pButton->CreateEx(this,m_iWidth - m_iButtonWidth + m_iBtnOffX,0 + m_iBtnOffY,m_vBtnTexID[BTEX_NORMAL],m_vBtnTexID[BTEX_HIGHLIGHT],m_vBtnTexID[BTEX_PUSHED],m_vBtnTexID[BTEX_DISABLED]);
	if (m_bColor)
	{
		SetText("",0,0,0,0);
	}
	else
	{
		SetText("",COLOR_BTN_NORMAL,COLOR_BTN_NORMAL,COLOR_BTN_NORMAL,COLOR_BTN_DISABLE,FONTSIZE_SMALL,0,FONT_YAHEI);
	}
	m_pButton->SetTextAlignType(XAL_LEFT);
	m_pButton->SetButtonType(1);
}

bool CCtrlMenuButton::OnWheel(int iWheel)
{
	if (!m_bMenu)
		return CControl::OnWheel(iWheel);

	if(m_pScroll)
	{
		return m_pScroll->OnWheel(iWheel);
	}
	return true;

}

DWORD CCtrlMenuButton::GetSelColor()
{
	if(!m_bColor)
		return 0x0;

	if(m_iCurSel<0 || m_iCurSel>=m_vecCNT.size())
		return 0x0;

	return strtoul(m_vecCNT.at(m_iCurSel).c_str(),NULL,16);
}

void CCtrlMenuButton::ChangeString(int i,const char* strStr)
{
	if(i>=0 && i<m_vecCNT.size())
	{
		if(m_vecCNT.at(i) != strStr)
			m_vecCNT.at(i)=strStr;
	}
}

void CCtrlMenuButton::ChangeEnable(int i,bool bEnable)
{
	if(i>=0 && i<m_vEnable.size())
	{
		m_vEnable.at(i) = bEnable;
	}
}

bool CCtrlMenuButton::IsMenuDown()
{
	return m_bMenu;
}

void CCtrlMenuButton::SetText(const char * sText,DWORD dwColor,DWORD dwMouseOnColor,DWORD dwClickColor,DWORD dwDisableColor,int iFontSize,DWORD dwFlag ,int iFont,bool bVertical,int iOffX,int iOffY,int iGap)
{
	m_sText = sText;

	if (m_pButton)
	{
		m_pButton->SetText(sText,dwColor,dwMouseOnColor,dwClickColor,dwDisableColor,iFontSize,dwFlag,iFont,bVertical,iOffX -(m_iWidth - m_iButtonWidth) + 2,iOffY,iGap);
	}
}

void CCtrlMenuButton::SetFont(int iFont,int iFontSize,DWORD dwFontFlag)
{
	CControl::SetFont(iFont,iFontSize,dwFontFlag);

	m_iItemHeight = m_iFontSize + 2;
}
