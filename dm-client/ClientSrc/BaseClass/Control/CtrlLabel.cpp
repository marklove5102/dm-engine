#include "CtrlLabel.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"


#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CCtrlLabel, CControl)

CCtrlLabel::CCtrlLabel(void)
{
	m_iControlMode	= CTRL_MODE_LABEL;
	m_iIndex		= -1;
	//memset(m_vTexs,0,sizeof(LPTexture) * BTEX_NUMS);
	memset(m_vTexID,0,sizeof(WORD)*BTEX_NUMS*2);
	//m_pTexEff       = NULL;
	m_dwTexEffID = 0;
	m_dwColor = 0xFFFF0000;
	m_dwBackColor = 0;
	m_iEffOffX  = m_iEffOffY = 0;
	m_bVertical = false;
	m_iLines = 0;
	m_iTextAlignType = XAL_CENTER;

	m_dwColor = COLOR_TEXT_NORMAL;
	m_dwMouseColor = COLOR_TEXT_NORMAL;
	m_dwClickColor = COLOR_TEXT_NORMAL;
	m_dwDisableColor = COLOR_TEXT_DISABLE;

	m_dwMouseOnFontFlag = 0;
	m_dwClickFlag = 0;

}

CCtrlLabel::~CCtrlLabel(void)
{
	Destroy();
}

void CCtrlLabel::Destroy()
{
	//g_pTexMgr->ReleaseTex(m_pTexEff); //释放特效
// 	ReleaseLabelTex();

	CControl::Destroy();
}

// 创建
bool CCtrlLabel::Create(CControl* pParent,int iX,int iY,int iW,int iH,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex)
{
	if(!pParent ) 
		return false;

	int iWidth = iW;
	int iHeight = iH;
	m_iIndex = 0;
	m_vTexID[0] = wTex;
	m_vTexID[1] = wMTex;
	m_vTexID[2] = wCTex;
	m_vTexID[3] = wDTex;

	LoadLabelTex(iWidth,iHeight);
	if (iW != 0 && iH != 0)
	{
		iWidth = iW;
		iHeight = iH;
	}

	return CControl::Create(pParent,iX,iY,iWidth,iHeight,CTRL_STYLE_DEFAULT,m_dwBackColor);
}

bool CCtrlLabel::CreateXML(CControl* pParent,CXmlLabel* label)
{
	if(!label || !pParent) 
		return false;

	int iW,iH;
	m_iIndex = label->GetIndex();

	for(int ii = 0;ii < 4;ii++)
	{
		m_vTexID[ii] = label->GetTexture(ii).GetIndex();
	}
	LoadLabelTex(iW,iH);

	AssignXML(label);

	if(!CControl::Create(pParent,0,0,iW,iH,CTRL_STYLE_DEFAULT,m_dwBackColor))
		return false;


	//设置特效图片
	CXmlTexture& tex = label->GetTexture(BTEX_EFFECT);
	int iOffX,iOffY;
	tex.GetOffset(iOffX,iOffY);
	SetEffectTex(tex.GetIndex(),iOffX,iOffY);

	CXmlFontString& fontstring = label->GetFontString();
	m_sText = fontstring.GetText();
	SetText(fontstring.GetText().c_str());

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


void CCtrlLabel::Draw(void)
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

	int iState;
	if(!m_bEnable && m_vTexID[3])
		iState = 3;
	else if(m_bClick )
		iState = 2;
	else if(m_bMouseOn)
		iState = 1;
	else
		iState = 0;

	DWORD dwTexID = 0;
	LPTexture pTex = NULL;

	if(m_vTexID[iState] != 0)
		dwTexID = (PACKAGE_INTERFACE,m_vTexID[iState]);
	else
		dwTexID = (PACKAGE_INTERFACE,m_vTexID[0]);

	if (dwTexID)
	{
		pTex = g_pTexMgr->GetTexFromID(dwTexID,EP_UI);
		if (pTex)
		{
			if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0)
			{
				ResetWidthAndHeight(pTex->GetWidth(),pTex->GetHeight());
			}

			DrawTexture(1,1,pTex);
		}
	}



	if (m_bVertical)
	{
		int x = m_iTextOffX;
		int y = (m_iHeight - m_iLines * m_iFontSize) / 2 + m_iTextOffY;
		if (m_iTextAlignType == XAL_CENTER || m_iTextAlignType == XAL_TOP || m_iTextAlignType == XAL_BOTTOM)
		{
			x += (m_iWidth - m_iFontSize) /2;
		}

		std::vector<std::string>   vDrawStr;

		CutByUnicode((char*)m_sText.c_str(),vDrawStr,1);

		for(size_t i = 0,j = 0; i < vDrawStr.size(); i++,j += m_iFontSize)
		{
			if(!m_bEnable)
				TextOut(x,y + j,vDrawStr[i].c_str(),m_dwDisableColor,m_dwFontFlag,m_dwBackColor);
			else if(m_bClick)
				TextOut(x,y + 1 + j,vDrawStr[i].c_str(),m_dwClickColor,m_dwClickFlag,m_dwBackColor);
			else if(m_bMouseOn)
				TextOut(x,y + j,vDrawStr[i].c_str(),m_dwMouseColor,m_dwMouseOnFontFlag,m_dwBackColor);
			else
				TextOut(x,y + j,vDrawStr[i].c_str(),m_dwColor,m_dwFontFlag,m_dwBackColor);		
		}			
	}
	else
	{
		int x = m_iTextOffX;
		if (m_iTextAlignType == XAL_CENTER || m_iTextAlignType == XAL_TOP || m_iTextAlignType == XAL_BOTTOM)
		{
			x += (m_iWidth - (int)(m_sText.size() * m_iFontSize / 2)) /2;
		}

		int y = (m_iHeight - (m_iFontSize + 2))/2 + m_iTextOffY;

		if(!m_bEnable)
			TextOut(x,y,m_sText.c_str(),m_dwDisableColor,m_dwFontFlag,m_dwBackColor);
		else if(m_bClick)
			TextOut(x,y,m_sText.c_str(),m_dwClickColor,m_dwClickFlag,m_dwBackColor);
		else if(m_bMouseOn)
			TextOut(x,y,m_sText.c_str(),m_dwMouseColor,m_dwMouseOnFontFlag,m_dwBackColor);
		else
			TextOut(x,y,m_sText.c_str(),m_dwColor,m_dwFontFlag,m_dwBackColor);
	}


	if(m_bMouseOn && m_dwTexEffID)
	{
		g_pGfx->SetRenderMode(RM_ADD1);
		DrawTexture(1-m_iEffOffX,1-m_iEffOffY,g_pTexMgr->GetTexFromID(m_dwTexEffID,EP_UI));
		g_pGfx->SetRenderMode();
	}	
}

// 鼠标左键单击的响应
bool CCtrlLabel::OnClick(int iLabel)
{
	if( !m_pParent ) 
		return false;

	//处理单击脚本
	ExecuteScript(XSE_OnClick);
	m_pParent->Msg(MSG_CTRL_LABEL_CLICK,INPUT_MOUSE_LEFTBT,this);
	return true;
}

// 重新装载纹理
void CCtrlLabel::LoadLabelTex(int &iW,int &iH)
{
	iW = 0;
	iH = 0;
	for(int i = 0; i < BTEX_NUMS; i++)
	{
		int iOffset = 0;
		if(m_iIndex > 0)
			iOffset += m_iIndex + i;
		else
			iOffset = m_vTexID[i];

		LPTexture pT = g_pTexMgr->GetTexImm(MAKELONG(iOffset, PACKAGE_INTERFACE),EP_UI);
		//m_vTexs[i] = pT;

		// 取得纹理的大小
		if((iW == 0 || iH == 0) && pT)
		{
			iW = pT->GetWidth();
			iH = pT->GetHeight();
		}
	}
}

// 释放纹理
//void CCtrlLabel::ReleaseLabelTex(void)
//{
//	for(int k = 1; k < BTEX_NUMS; ++k)
//	{
//		for(int j = k-1; j >= 0; --j)
//		{
//			if(m_vTexs[k] == m_vTexs[j])
//				m_vTexs[k] = NULL;
//		}
//	}
//
//	for(int i = 0; i < BTEX_NUMS; i++)
//	{
//		if(m_vTexs[i])
//		{
//			g_pTexMgr->ReleaseTex(m_vTexs[i]);
//		}
//	}	
//}

void CCtrlLabel::ReloadTex(int _iTexIndex)
{
	if(_iTexIndex == m_iIndex)
		return;

// 	ReleaseLabelTex();
	m_iIndex = _iTexIndex;
	LoadLabelTex(m_iWidth, m_iHeight);
}

void CCtrlLabel::ReloadTex(WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex)
{
	if(m_vTexID[0] == wTex && m_vTexID[1] == wMTex && m_vTexID[2] == wCTex)
		return;

// 	ReleaseLabelTex();

	m_vTexID[0] = wTex;
	m_vTexID[1] = wMTex;
	m_vTexID[2] = wCTex;
	m_vTexID[3] = wDTex;

	LoadLabelTex(m_iWidth,m_iHeight);
}

void CCtrlLabel::SetTex(E_BUTTON_TEX iIndex,WORD wMTex)
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

void CCtrlLabel::SetEffectTex(WORD wEff,int iOffX,int iOffY)
{
	if(wEff == 0)
		return;

	//g_pTexMgr->ReleaseTex(m_pTexEff);

	//m_pTexEff = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,wEff);

	m_dwTexEffID = MAKELONG(wEff,PACKAGE_INTERFACE);
	m_iEffOffX = iOffX;
	m_iEffOffY = iOffY;
}

void CCtrlLabel::SetText( const char * sText,DWORD dwColor,DWORD dwMouseOnColor,DWORD dwClickColor,DWORD dwDisableColor,int iFontSize,DWORD dwFlag,DWORD dwMouseOnFlag,DWORD dwClickFlag,int iFont,bool bVertical,int iOffX,int iOffY)
{
	CControl::SetText(sText);

	m_dwColor = dwColor;
	m_dwMouseColor = dwMouseOnColor;
	m_dwClickColor = dwClickColor;
	m_dwDisableColor = dwDisableColor;

	SetFont(iFont,iFontSize,dwFlag);

	m_dwMouseOnFontFlag = dwMouseOnFlag;
	m_dwClickFlag = dwClickFlag;


	m_bVertical = bVertical;
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