#include "CtrlTip.h"
#include "GameData/GameDefine.h"
#include "Global/StringUtil.h"


#define MAX_TIP_WIDTH	64				// Tip最多可以输出64个字符

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CCtrlTip, CControl)

CCtrlTip::CCtrlTip(void)
{
    m_iControlMode	= CTRL_MODE_TIP;
	m_iFont			= FONT_DEFAULT;
	m_iFontSize		= FONTSIZE_DEFAULT;
	m_iFrame		= 1;		
	m_dwFrameColor	= 0xFF000000;
	m_dwColor       = 0xFFFFFFFF;
	m_dwBackColor   = 0xFFFFFFE7;
	m_iMarge		= 3;
	m_sText.clear();
	m_bAutoAdjust   = true;

	m_iFont = FONT_DEFAULT;
	m_iFontSize = FONTSIZE_DEFAULT;
	m_dwFontFlag = 0;
	m_iMaxWidth = 0;
	m_iMaxHeight = 0;
	m_iLastCurLineX = 0;
	m_iLastCurLineY = 0;
	m_iCurLineMaxHeight = 0;
}

CCtrlTip::~CCtrlTip(void)
{
	m_Tips.clear();
}


bool CCtrlTip::Create(CControl *pParent)
{
	return CControl::Create(pParent,0,0,1,1,CTRL_STYLE_BACKMODE_COLOR|CTRL_STYLE_AUTOSIZE,m_dwBackColor);
}

bool CCtrlTip::CreateXML(CControl* pParent,CXmlTip* tip)
{
	AssignXML(tip);

    if(!CControl::Create(pParent,0,0,1,1,CTRL_STYLE_BACKMODE_COLOR|CTRL_STYLE_AUTOSIZE,m_dwBackColor))
    {
        return false;
    }

    VXmlFontString& lines = tip->GetLines();
    VXmlFontString::iterator itr;
    for(itr = lines.begin();itr != lines.end();itr++)
    {
        CXmlFontString& line = *itr;

		DWORD dwColor = line.GetXmlColor().GetColor();
		if(dwColor == 0)
			dwColor = COLOR_DEFAULT;

		bool bCenter = false;
		if(line.GetCenter())
			bCenter = true;

        AddText(line.GetText().c_str(),dwColor,bCenter);
    }

    return true;
}

void CCtrlTip::Clear() 
{
	m_Tips.clear();
	m_iMaxWidth = 0;
	m_iMaxHeight = 0;
	m_iLastCurLineX = 0; 
	m_iLastCurLineY = 0;
	m_iCurLineMaxHeight = 0; 
	m_sText.clear();
}

void CCtrlTip::OnAutoSize()
{
	CalSize();
}

void CCtrlTip::OnMove()
{
	int iW = m_pMainWnd->GetWidth();
	int iH = m_pMainWnd->GetHeight();

	if(m_bAutoAdjust)
	{
		if(m_iX < 0)
			m_iX = 0;
		else if(m_iX + m_iWidth > iW)
		{
			m_iX = iW - m_iWidth;
		}

		if(m_iY < 0)
			m_iY = 0;
		else if(m_iY + m_iHeight > iH)
		{
			m_iY = iH - m_iHeight;
		}
	}
	CControlContainer::OnMove();
}

void CCtrlTip::Draw()
{
	CControlContainer::Draw();
	
	// 以下为具体显示的内容
	int iCurDealPicLine = 0;
	int iDrawX = m_iFrame + m_iMarge;
	int iDrawY = m_iFrame + m_iMarge;
	int iPicWidth = 0,iPicHeigth = 0;//有的时候图片加进来的时候不指定图的大小,读出来后要自动调整后面文字的位置
	int iW = 0,iH = 0;

	int iSize = m_Tips.size();
	if(iSize == 0)
	{
		TextOut(iDrawX,iDrawY,m_sText.c_str(),m_dwColor,m_dwFontFlag);
	}
	else
	{
		for(int i = 0; i < iSize; i++)
		{
			TipElement& inf = m_Tips.at(i);

			int iAlignType = inf.iAlignType;
			iDrawX = inf.iX + m_iFrame + m_iMarge + iPicWidth;
			iDrawY = inf.iY + m_iFrame + m_iMarge + iPicHeigth;


			//暂时不考虑对齐方式
			//if(iAlignType == XAL_CENTER || iAlignType == XAL_TOP || iAlignType == XAL_BOTTOM)
			//{
			//	iDrawX = (iDrawX - inf.iWidth) / 2;
			//}
			//else if(iAlignType == XAL_TOPRIGHT || iAlignType == XAL_RIGHT || iAlignType == XAL_BOTTOMRIGHT)
			//{
			//	iDrawX = iDrawX - inf.iWidth;
			//}

			//if(iAlignType == XAL_CENTER || iAlignType == XAL_LEFT || iAlignType == XAL_RIGHT)
			//{
			//	iDrawY = (iDrawY - inf.iHeight) / 2;
			//}
			//else if(iAlignType == XAL_BOTTOMLEFT || iAlignType == XAL_BOTTOM || iAlignType == XAL_BOTTOMRIGHT)
			//{
			//	iDrawY = iDrawY - inf.iHeight;
			//}

			if(inf.dwPicID > 0)
			{
				if (inf.dwPicID == 1)//画线
				{
					//g_pGfx->DrawLine(m_iScreenY + iDrawY,m_iScreenX + iDrawX,m_iScreenX + iDrawX + (inf.iWidth > 0?inf.iWidth:m_iWidth - m_iFrame - m_iMarge - iDrawX),inf.dwColor);
					g_pGfx->DrawFillRect(m_iScreenX + iDrawX,m_iScreenY + iDrawY,inf.iWidth > 0?inf.iWidth:m_iWidth - m_iFrame - m_iMarge - iDrawX,2,inf.dwColor);

					iW = inf.iWidth + m_iMarge;
					iH = inf.iHeight + m_iMarge;
				}
				else
				{
					LPTexture pTex = g_pTexMgr->GetTexFromID(inf.dwPicID,EP_UI);
					if(!pTex)
						continue;

					if (inf.iRenderMode != RM_ALPHA)
					{
						g_pGfx->SetRenderMode(RenderMode(inf.iRenderMode));
					}

					if (inf.dwFontFlag & ETF_DRAWPART)
					{
						g_pGfx->DrawPartTexture(m_iScreenX + iDrawX, m_iScreenY + iDrawY, pTex,0,0,inf.iWidth,inf.iHeight,inf.dwColor);
					}
					else
					{
						g_pGfx->DrawTextureNL(m_iScreenX + iDrawX, m_iScreenY + iDrawY, pTex);
					}

					if (inf.iRenderMode != RM_ALPHA)
					{
						g_pGfx->SetRenderMode();
					}

					if (inf.iWidth < 0)
					{
						iW = pTex->GetWidth() + m_iMarge;
						iPicWidth += pTex->GetWidth();
					}
					else
					{
						iW = inf.iWidth + m_iMarge;
					}

					if (inf.bNewLineNext && inf.iHeight < 0)
					{
						iH = pTex->GetHeight() + m_iMarge;
						iPicHeigth += pTex->GetHeight();
					}
					else
					{
						iH = inf.iHeight + m_iMarge;
					}
				}
			}
			else
			{
				g_pFont->DrawText(m_iScreenX + iDrawX,m_iScreenY + iDrawY,inf.strText.c_str(),inf.dwColor,inf.iFont,inf.iFontSize,inf.dwFontFlag,inf.dwBackColor,inf.dwFrameColor);

				iW = inf.iWidth + m_iMarge;
				iH = inf.iHeight + m_iMarge;
			}	

//  			if (iW + iDrawX + m_iFrame + m_iMarge > m_iWidth)
//  			{
//  				m_iWidth = iW + iDrawX + m_iFrame + m_iMarge;
//  			}
// 
//  			if (iH + iDrawY + m_iFrame + m_iMarge > m_iHeight)
//  			{
// 				m_iHeight = iH + iDrawY + m_iFrame + m_iMarge;
//  			}

			if (inf.bNewLineNext)
			{
				iPicWidth = 0;
			}
		}
	}
}

// 消息响应函数
bool CCtrlTip::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	return false;
}


void CCtrlTip::SetText(const char * sText,int iFont,int iFontSize,DWORD color,DWORD dwFontFlag,char cEndChar)
{
	m_Tips.clear();
	CControl::SetText(sText);

	m_dwColor = color;
	m_dwFontFlag = dwFontFlag;
	m_iFont = iFont;
	m_iFontSize = iFontSize;

	m_iMaxWidth = 0;
	m_iMaxHeight = 0;


	int iStartPos = 0;
	int iSize = strlen(sText);
	while (iStartPos < iSize - 1)
	{
		string strtemp;
		strtemp = StringUtil::toStr(sText,iStartPos,cEndChar);
		if (strtemp.size() > 0)
			AddText(strtemp.c_str(),iFont,iFontSize,color,dwFontFlag);
	}


	CalSize();
}

void CCtrlTip::AddText(const char * str,int iFont,int iFontSize,DWORD color,DWORD dwFontFlag)
{
	if(str == NULL)
		return;

	int iW = strlen(str) * iFontSize/2;
	if (iW > m_iMaxWidth)
	{
		m_iMaxWidth = iW;
	}

	m_Tips.push_back(TipElement(str,0,0,m_iMaxHeight,iW,iFontSize,iFont,iFontSize,dwFontFlag,color,XAL_TOPLEFT));
	m_iMaxHeight += m_iFontSize + 3;

	CalSize();
}

void CCtrlTip::CalSize()
{
	int iW = m_iMaxWidth + m_iFrame*2 + m_iMarge*2;
	int iH = m_iMaxHeight + m_iFrame*2 + m_iMarge*2;

	ReSize(iW,iH);
}
