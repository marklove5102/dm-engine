
#include "ctrlbutton.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"


#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CCtrlButton, CControl)

CCtrlButton::CCtrlButton(void)
{
	m_iControlMode	= CTRL_MODE_BUTTON;
	m_iIndex		= -1;
	m_bSwitchButton	= false;
	m_bSwitchState	= false;
	//memset(m_vTexs,0,sizeof(LPTexture) * BTEX_NUMS);
	memset(m_vTexID,0,sizeof(WORD)*BTEX_NUMS*2);
	m_bAutoSwitch   = false;
	//m_pTexEff       = NULL;
	m_dwTexEffID = 0;
	m_dwColor = 0xFFFF0000;
	m_dwBackColor = 0;
	m_iEffOffX  = m_iEffOffY = 0;
	m_eBackState = BTEX_NONE;
	m_bVertical = false;
	m_iGap = 0;
	m_iLines = 0;
	m_iTextAlignType = XAL_CENTER;
	m_iBtnType = 0;
	m_dwFlashTexID = 0;

	m_dwColor = COLOR_BTN_NORMAL;
	m_dwMouseColor = COLOR_BTN_MOUSEON;
	m_dwClickColor = COLOR_BTN_PRESS;
	m_dwDisableColor = COLOR_BTN_DISABLE;
}

CCtrlButton::~CCtrlButton(void)
{
	Destroy();
}

void CCtrlButton::Destroy()
{
	//g_pTexMgr->ReleaseTex(m_pTexEff); //释放特效
	//ReleaseButtonTex();

	CControl::Destroy();
}

// 创建
bool CCtrlButton::CreateEx(CControl* pParent,int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex)
{
	if(!pParent ) 
		return false;

	m_bSwitchButton = false;
	int iW = 0,iH = 0;
	m_iIndex = 0;
	m_vTexID[0] = wTex;
	m_vTexID[1] = wMTex;
	m_vTexID[2] = wCTex;
	m_vTexID[3] = wDTex;

	LoadButtonTex(iW,iH);

	return CControl::Create(pParent,iX,iY,iW,iH,CTRL_STYLE_DEFAULT,m_dwBackColor);
}

bool CCtrlButton::CreateXML(CControl* pParent,CXmlButton* btn)
{
	if(!btn || !pParent) 
		return false;

	int iW = 0,iH = 0;
	m_iIndex = btn->GetIndex();

	if(m_iIndex > 0 && btn->GetSwitch())  //通过开始索引才能设置双态按钮
		m_bSwitchButton = true;

	for(int ii = 0;ii < 4;ii++)
	{
		m_vTexID[ii] = btn->GetTexture(ii).GetIndex();
	}
	LoadButtonTex(iW,iH);

	AssignXML(btn);

	if(!CControl::Create(pParent,0,0,iW,iH,CTRL_STYLE_DEFAULT,m_dwBackColor))
		return false;


	//设置特效图片
	CXmlTexture& tex = btn->GetTexture(BTEX_EFFECT);
	int iOffX,iOffY;
	tex.GetOffset(iOffX,iOffY);
    SetEffectTex(tex.GetIndex(),iOffX,iOffY);

	CXmlFontString& fontstring = btn->GetFontString();
	m_sText = fontstring.GetText();
	SetText(fontstring.GetText().c_str());
	ParseText(fontstring.GetText().c_str());

	const string & strFont = fontstring.GetXmlFont().GetName();
	if (strFont == "FONT_DEFAULT")
	{
		m_iFont = FONT_DEFAULT;
	}
	else if (strFont == "FONT_SONGTI")
	{
		m_iFont = FONT_SONGTI;
	}
	else if (strFont == "FONT_YAHEI")
	{
		m_iFont = FONT_YAHEI;
	}
	else if (strFont == "FONT_LISHU")
	{
		m_iFont = FONT_LISHU;
	}

	m_iFontSize = fontstring.GetXmlFont().GetSize();
	if (m_iFontSize <= 0 || m_iFontSize > 72)
	{
		m_iFontSize = FONTSIZE_DEFAULT;
	}


	return true;
}

bool CCtrlButton::Create(CControl * pParent, int iX, int iY, int iIndex, bool bSwitch)
{
	if( !pParent ) 
		return false;

	m_bSwitchButton = bSwitch;
	int iW = 0,iH = 0;
	m_iIndex = iIndex;
	LoadButtonTex(iW,iH);
	return CControl::Create(pParent,iX,iY,iW,iH,CTRL_STYLE_DEFAULT,m_dwBackColor);
}

bool CCtrlButton::Create(CControl * pParent,int iX,int iY,UINT uStyle,int iW,int iH)
{
	if( !pParent ) 
		return false;
	
	return CControl::Create(pParent,iX,iY,iW,iH,uStyle,m_dwBackColor);
}

void CCtrlButton::Draw(void)
{
	if( !g_pInput) return;

	if(!m_bShow)
		return;
	
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
	
	LPTexture pTex = NULL;
	if (m_eBackState >= BTEX_NORMAL && m_eBackState < BTEX_NUMS)
	{
		//if(m_vTexs[m_eBackState] != NULL)
		//	DrawTexture(1,1,m_vTexs[m_eBackState]);
		if(m_vTexID[m_eBackState])
			pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_vTexID[m_eBackState],EP_UI);
	}
	else
	{
		int iState;
		if(!m_bEnable && m_vTexID[BTEX_DISABLED])
			iState = BTEX_DISABLED;
		else if(m_bClick )
			iState = BTEX_PUSHED;
		else if(m_bMouseOn)
			iState = BTEX_HIGHLIGHT;
		else
			iState = BTEX_NORMAL;

		if(m_vTexID[iState])
			pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_vTexID[iState],EP_UI);
		else
			pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_vTexID[0],EP_UI);

		if(!pTex && ((m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0) && (m_vTexID[iState] == 0 && m_vTexID[0] == 0)))//如果这个状态没有图,找一个有图的状态,得到大小信息,否则得不到大小信息后也无法接收到事件,本地有图不存在这个问题,如果从服务器下载图片回来有此问题
		{
			for(int i = BTEX_NORMAL; i < BTEX_DISABLED; i++)
			{
				if (m_vTexID[i] > 0)
				{
					pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_vTexID[i],EP_UI);
					break;
				}
			}
		}
	}

	if (pTex)
	{
		if(m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth == 0 || m_iHeight == 0)
		{
			ResetWidthAndHeight(pTex->GetWidth(),pTex->GetHeight());
		}

		DrawTexture(1,1,pTex);
	}

	//闪烁按钮
	if (m_dwFlashTexID != 0)
	{
		if (m_dwFlashTexID == 1)
		{
			m_dwFlashFrameControl ++;
			if (m_dwFlashFrameControl % 20 >= 0 && m_dwFlashFrameControl % 20 < 10)
			{
				//if(m_vTexs[BTEX_HIGHLIGHT] != NULL)
				//	DrawTexture(1,1,m_vTexs[BTEX_HIGHLIGHT]);
				if(m_vTexID[BTEX_HIGHLIGHT])
					DrawTexture(1,1,MAKELONG(m_vTexID[BTEX_HIGHLIGHT],PACKAGE_INTERFACE));
			}
		}
		else if (m_dwFlashTexID > 0x0000FFFF)
		{
			g_pGfx->SetRenderMode(RM_ADD2);			
			DrawTexture(1,1,g_pTexMgr->GetTexFromID(m_dwFlashTexID,EP_UI));
			g_pGfx->SetRenderMode();
		}
	}

	
	if( m_vText.size() > 0)
	{
		if (m_bVertical)
		{
			int x = m_iTextOffX;
			int y = (m_iHeight - m_iLines * m_iFontSize) / 2 + m_iTextOffY;
			if (m_iTextAlignType == XAL_CENTER || m_iTextAlignType == XAL_TOP || m_iTextAlignType == XAL_BOTTOM)
			{
				x += (m_iWidth - m_iFontSize) /2;
			}

			std::vector<std::string>   vDrawStr;

			string& str = m_vText.at(0);
			CutByUnicode((char*)str.c_str(),vDrawStr,1);

			for(size_t i = 0,j = 0; i < vDrawStr.size(); i++,j += m_iFontSize)
			{
				if(!m_bEnable)
					TextOut(x,y + j,vDrawStr[i].c_str(),m_dwDisableColor,m_dwFontFlag,m_dwBackColor);
				else if(m_bClick)
					TextOut(x,y + 1 + j,vDrawStr[i].c_str(),m_dwClickColor,m_dwFontFlag,m_dwBackColor);
				else if(m_bMouseOn || (m_eBackState >= BTEX_NORMAL && m_eBackState < BTEX_NUMS))
					TextOut(x,y + j,vDrawStr[i].c_str(),m_dwMouseColor,m_dwFontFlag,m_dwBackColor);
				else
					TextOut(x,y + j,vDrawStr[i].c_str(),m_dwColor,m_dwFontFlag,m_dwBackColor);		
			}			
		}
		else
		{
			for (int i = 0;i<m_vText.size();i++)
			{
				string& str = m_vText.at(i);
				int x = m_iTextOffX;
				if (m_iTextAlignType == XAL_CENTER || m_iTextAlignType == XAL_TOP || m_iTextAlignType == XAL_BOTTOM)
				{
					x += (m_iWidth - (int)(str.size() * m_iFontSize / 2)) /2;
				}

				int y = (m_iHeight - (m_iFontSize + 2) * m_vText.size())/2 + m_iTextOffY + i * (m_iFontSize + 2);
				
				if(!m_bEnable)
					TextOut(x,y,str.c_str(),m_dwDisableColor,m_dwFontFlag,m_dwBackColor);
				else if(m_bClick)
				{
					if (m_iBtnType == 1)
						TextOut(x,y,str.c_str(),m_dwClickColor,m_dwFontFlag,m_dwBackColor);
					else
						TextOut(x,y + 1,str.c_str(),m_dwClickColor,m_dwFontFlag,m_dwBackColor);
				}
				else if(m_bMouseOn || (m_eBackState >= BTEX_NORMAL && m_eBackState < BTEX_NUMS))
					TextOut(x,y,str.c_str(),m_dwMouseColor,m_dwFontFlag,m_dwBackColor);
				else
					TextOut(x,y,str.c_str(),m_dwColor,m_dwFontFlag,m_dwBackColor);
			}			
		}
	}

	if(m_bMouseOn && m_dwTexEffID)
	{
        g_pGfx->SetRenderMode(RM_ADD1);
        DrawTexture(1-m_iEffOffX,1-m_iEffOffY,m_dwTexEffID);
        g_pGfx->SetRenderMode();
	}	
}

// 鼠标左键单击的响应
bool CCtrlButton::OnClick(int iButton)
{
	m_dwFlashTexID = 0;

	if( !m_pParent ) 
		return false;
	
	// 双态按钮的处理
	if(m_bSwitchButton && m_bAutoSwitch)
	{
		m_bSwitchState = !m_bSwitchState;
		ReleaseButtonTex();
		LoadButtonTex(m_iWidth,m_iHeight);		
	}

	//处理单击脚本
	ExecuteScript(XSE_OnClick);
	m_pParent->Msg(MSG_CTRL_BUTTON_CLICK,INPUT_MOUSE_LEFTBT,this);
	return true;
}

// 重新装载纹理
void CCtrlButton::LoadButtonTex(int &iW,int &iH)
{
	iW = 0;
	iH = 0;
	for(int i = 0; i < BTEX_NUMS; i++)
	{
		int iPos = ((m_bSwitchState && m_bSwitchButton)? BTEX_NUMS : 0) + i;

		int iOffset = 0;
		if(m_iIndex > 0)
			iOffset += m_iIndex + iPos;
		else
			iOffset = m_vTexID[iPos];

		m_vTexID[i] = iOffset;

		// 取得纹理的大小
		if(iW == 0 || iH == 0)
		{
			LPTexture pT = g_pTexMgr->GetTexImm(MAKELONG(iOffset, PACKAGE_INTERFACE),EP_UI);
			if (pT)
			{
				iW = pT->GetWidth();
				iH = pT->GetHeight();
			}
		}
	}
}

// 释放纹理
void CCtrlButton::ReleaseButtonTex(void)
{
	//for(int k = 1; k < BTEX_NUMS; ++k)
	//{
	//	for(int j = k-1; j >= 0; --j)
	//	{
	//		if(m_vTexs[k] == m_vTexs[j])
	//			m_vTexs[k] = NULL;
	//	}
	//}

	//for(int i = 0; i < BTEX_NUMS; i++)
	//{
	//	if(m_vTexs[i])
	//	{
	//		g_pTexMgr->ReleaseTex(m_vTexs[i]);
	//	}
	//}	
}

//
bool CCtrlButton::IsSmall(void)
{
	if(m_bSwitchButton && m_bSwitchState)		// 在最小化态
		return true;
	else
        return false;
}

void CCtrlButton::ReloadTex(int _iTexIndex)
{
	if(_iTexIndex == m_iIndex)
		return;

	//ReleaseButtonTex();
	m_iIndex = _iTexIndex;
	LoadButtonTex(m_iWidth, m_iHeight);
}

void CCtrlButton::ReloadTex(WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex)
{
	if(m_vTexID[0] == wTex && m_vTexID[1] == wMTex && m_vTexID[2] == wCTex)
		return;

	//ReleaseButtonTex();

	m_vTexID[0] = wTex;
	m_vTexID[1] = wMTex;
	m_vTexID[2] = wCTex;
	m_vTexID[3] = wDTex;

	LoadButtonTex(m_iWidth,m_iHeight);
}

void CCtrlButton::SetTex(E_BUTTON_TEX iIndex,WORD wMTex)
{
	if(iIndex < BTEX_NORMAL || iIndex > BTEX_EFFECT || wMTex == m_vTexID[iIndex])
		return;

	m_vTexID[iIndex] = wMTex;

	LPTexture pT = g_pTexMgr->GetTexImm(MAKELONG(wMTex, PACKAGE_INTERFACE),EP_UI);
	//if(m_vTexs[iIndex])
	//{
	//	g_pTexMgr->ReleaseTex(m_vTexs[iIndex]);
	//}

	//m_vTexs[iIndex] = pT;
	if(pT)	// 取得纹理的大小
	{
		m_iWidth = pT->GetWidth();
		m_iHeight = pT->GetHeight();
	}

}

void CCtrlButton::SetSwitchTex(WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex)
{
	int i = BTEX_NUMS;

	if(m_bSwitchButton && m_bSwitchState) //更新
	{
		if(m_vTexID[i] == wTex && m_vTexID[i+1] == wMTex && m_vTexID[i+2] == wCTex)
			return;

		//ReleaseButtonTex();

		m_vTexID[i] = wTex;
		m_vTexID[i+1] = wMTex;
		m_vTexID[i+2] = wCTex;
		m_vTexID[i+3] = wDTex;

		LoadButtonTex(m_iWidth,m_iHeight);
	}
	else
	{
		m_bSwitchButton = true;
		m_vTexID[i] = wTex;
		m_vTexID[i+1] = wMTex;
		m_vTexID[i+2] = wCTex;
		m_vTexID[i+3] = wDTex;
	}
}

void CCtrlButton::SetState(bool _bSwitchState)
{
	if(!m_bSwitchButton)
		return;
	if(m_bSwitchState == _bSwitchState)
		return;
	else
	{
		m_bSwitchState = _bSwitchState;
		//ReleaseButtonTex();
		LoadButtonTex(m_iWidth,m_iHeight);		
	}
}

void CCtrlButton::SetEffectTex(WORD wEff,int iOffX,int iOffY)
{
	if(wEff == 0)
		return;

	//g_pTexMgr->ReleaseTex(m_pTexEff);

	//m_pTexEff = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,wEff);

	m_dwTexEffID = MAKELONG(wEff,PACKAGE_INTERFACE);

	m_iEffOffX = iOffX;
	m_iEffOffY = iOffY;
}

void CCtrlButton::ParseText(const char* pText)
{
	if (!pText) return;
	m_vText.clear();
	char* pTemp = strtok(const_cast<char*>(pText),"&");
	while(pTemp)
	{
		m_vText.push_back(pTemp);
		pTemp = strtok(NULL,"&");
	}
}

void CCtrlButton::SetText( const char * sText,DWORD dwColor,DWORD dwMouseColor,DWORD dwClickColor,DWORD dwDisableColor,int iFontSize,DWORD dwFlag,int iFont,bool bVertical,int iOffX,int iOffY,int iGap)
{
	string str = sText;
	ParseText(str.c_str());
	CControl::SetText(sText);

	m_dwColor = dwColor;
	m_dwMouseColor = dwMouseColor;
	m_dwClickColor = dwClickColor;
	m_dwDisableColor = dwDisableColor;

	SetFont(iFont,iFontSize,dwFlag);

	m_bVertical = bVertical;

	m_iGap = iGap;
	m_iTextOffX = iOffX;
	m_iTextOffY = iOffY;


	if (bVertical)
	{
		m_iLines = 0;
		BYTE c1,c2;
		int len = (int)strlen(sText);
		for(int i = 0;i < len;i++)
		{
			c1 = (BYTE)sText[i];
			c2 = (BYTE)sText[i + 1];

			if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
			{
				m_iLines ++;
				i++;
			}
			else
			{
				m_iLines ++;
			}
		}	
	}
}