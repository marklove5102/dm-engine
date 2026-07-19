#include "ctrlradio.h"
#include "GameData/GameDefine.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CCtrlRadio, CControl)

CCtrlRadio::CCtrlRadio(void)
{
	m_iControlMode	= CTRL_MODE_RADIO;
	//memset(m_vTexs,0,sizeof(LPTexture) * 4);
	memset(m_vTexID,0,sizeof(WORD) * 4);
	m_bChecked	= false; 
	m_bGroup = true;	

	m_iBackTexWidth = 0;
	m_iBackTexHeight = 0;
	m_bUseBackTexWidth = false;
	m_bUseBackTexHeight = false;
	m_iRadioType = 0;
	m_iTextAlignType = XAL_LEFT;

	
    m_dwColor = -1;
    m_dwCheckedColor = -1;
    m_dwMouseOnColor = -1;
    m_dwDisableColor = -1;
}

CCtrlRadio::~CCtrlRadio(void)
{
	//ReleaseRadioTex();
}

bool CCtrlRadio::Create(CControl* pParent, int iX, int iY,WORD wNormal,WORD wChecked,WORD wDisabled,WORD wMouseOn,int iW,int iH)
{
	if( !pParent || (wNormal == 0 && wChecked == 0))
		return false;
	

	m_vTexID[0] = wNormal;
	m_vTexID[1] = wMouseOn;
	m_vTexID[2] = wChecked;
	m_vTexID[3] = wDisabled;

	if (iW <= 0)
	{
		m_bUseBackTexWidth = true;
	}
	if (iH <= 0)
	{
		m_bUseBackTexHeight = true;
	}

	LoadRadioTex(m_iBackTexWidth,m_iBackTexHeight);

	if (m_bUseBackTexWidth)
	{
		iW = m_iBackTexWidth;
	}
	if (m_bUseBackTexHeight)
	{
		iH = m_iBackTexHeight;
	}

	return CControl::Create(pParent,iX,iY,iW,iH);
}

bool CCtrlRadio::CreateEx(CControl* pParent, int iX, int iY,int iW,int iH,WORD wNormal,WORD wChecked,WORD wDisabled,WORD wMouseOn)
{
	return Create(pParent,iX,iY,wNormal,wChecked,wDisabled,wMouseOn,iW,iH);
}

bool CCtrlRadio::CreateXML(CControl* pParent,CXmlRadio* radio)
{
	if(!radio || !pParent)
		return false;

	int iNormal = radio->GetTexture(CXmlRadio::RTEX_NORMAL).GetIndex();
	int iChecked = radio->GetTexture(CXmlRadio::RTEX_CHECKED).GetIndex();

	// 调整位置
	AssignXML(radio);

	if(!Create(pParent,0,0,iNormal,iChecked))
		return false;

	if(radio->GetChecked())
		m_bChecked = true;

	return true;
}


bool CCtrlRadio::OnClick(int iButton)
{
	//单选框时只能选中
	if(m_VBuddy.size() != 0 && m_bChecked && m_bGroup)
		return true;

	m_bChecked = !m_bChecked;

	for(size_t i = 0;i < m_VBuddy.size();i++)
	{
		m_VBuddy.at(i)->SetChecked(false);
	}
	GetParent()->Msg(MSG_CTRL_BUTTON_CLICK,0,this);
	return true;
}

void CCtrlRadio::Draw(void)
{
	//if(!m_bEnable && m_vTexs[3])
	//	DrawTexture(0,0,m_vTexs[3]);
	//else if(m_bChecked)
	//	DrawTexture(0,0,m_vTexs[2]);
	//else if(m_bMouseOn && m_vTexs[1])
	//	DrawTexture(0,0,m_vTexs[1]);
	//else
	//	DrawTexture(0,0,m_vTexs[0]);

	DWORD dwTexID = 0;
	if(!m_bEnable && m_vTexID[3])
		dwTexID = MAKELONG(m_vTexID[3],PACKAGE_INTERFACE);
	else if(m_bChecked)
		dwTexID = MAKELONG(m_vTexID[2],PACKAGE_INTERFACE);
	else if(m_bMouseOn && m_vTexID[1])
		dwTexID = MAKELONG(m_vTexID[1],PACKAGE_INTERFACE);
	else
		dwTexID = MAKELONG(m_vTexID[0],PACKAGE_INTERFACE);

	LPTexture pTex = 0;

	if (m_iBackTexWidth <= 0 || m_iBackTexHeight <= 0 || m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0)
	{
		DWORD dwWHTexID = dwTexID;
		if (dwWHTexID == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (m_vTexID[i] != 0)
				{
					dwWHTexID = m_vTexID[i];
					break;
				}
			}
		}
		pTex = g_pTexMgr->GetTexImm(dwWHTexID,EP_UI);
		if (pTex)
		{
			ResetWidthAndHeight(pTex->GetWidth(),pTex->GetHeight());
		}
	}

	if (dwTexID > 0)
	{
		if (!pTex)
			pTex = g_pTexMgr->GetTexImm(dwTexID,EP_UI);
		DrawTexture(0,0,pTex);
	}


	if( m_bMouseOn )
	{
		int x,y;
		m_pMainWnd->GetMouseXY(x,y);
		x -= m_iScreenX;
		y -= m_iScreenY;
		if( x<0 || y<0 || x>=m_iWidth || y>=m_iHeight ) 
			m_bMouseOn = false;
	}

	if( m_sText.size() > 0)
	{
		int x = m_iTextOffX;
		if (!m_bUseBackTexWidth)
		{
			x += m_iBackTexWidth;
		}

		if (m_iTextAlignType == XAL_CENTER || m_iTextAlignType == XAL_TOP || m_iTextAlignType == XAL_BOTTOM)
		{
			x += (m_iWidth - (int)(m_sText.size() * m_iFontSize / 2)) /2;
		}

		int y = (m_iHeight - m_iFontSize) / 2 + m_iTextOffY;

		if(!m_bEnable)
			TextOut(x,y,m_sText.c_str(),m_dwDisableColor,m_dwFontFlag,m_dwBackColor);
		else if(m_bChecked)
		{
			if (m_iRadioType == 1)
			{
				TextOut(x,y + 1,m_sText.c_str(),m_dwCheckedColor,m_dwFontFlag,m_dwBackColor);		
			}
			else
			{
				TextOut(x,y,m_sText.c_str(),m_dwCheckedColor,m_dwFontFlag,m_dwBackColor);
			}
		}
		else if(m_bMouseOn)
		{
			TextOut(x,y,m_sText.c_str(),m_dwMouseOnColor,m_dwFontFlag,m_dwBackColor);	
		}
		else
			TextOut(x,y,m_sText.c_str(),m_dwColor,m_dwFontFlag,m_dwBackColor);
	}

	if( m_bMouseOn && !m_sTips.empty() && m_pMainWnd)
		m_pMainWnd->SetMouseOnID(MOUSE_ON_CONTROL,this);
}	

void CCtrlRadio::AddBuddy(CCtrlRadio* buddy)
{
	if(buddy == NULL)
		return;

	m_iRadioType = 1;
	buddy->SetRadioType(1);

	//已经添加了
	size_t i = 0;
	for(i = 0;i < m_VBuddy.size();i++)
	{
		if(m_VBuddy.at(i) == buddy)
			return;
	}

	buddy->m_VBuddy.clear();
	buddy->m_VBuddy.push_back(this);

	for(i = 0; i < m_VBuddy.size();i++)
	{
		buddy->m_VBuddy.push_back(m_VBuddy.at(i));
		m_VBuddy[i]->m_VBuddy.push_back(buddy);
	}
	m_VBuddy.push_back(buddy);
}

int CCtrlRadio::GetRadio()
{
	if(m_bChecked)
		return 0;

	for(size_t i = 0;i < m_VBuddy.size();i++)
	{
		if(m_VBuddy.at(i)->IsChecked())
			return (int)(i+1);
	}
	return -1;
}

void CCtrlRadio::SetRadio(int ii)
{
	m_bChecked = (ii == 0);

	for(size_t i = 0;i < m_VBuddy.size();i++)
	{
		m_VBuddy.at(i)->SetChecked(ii - 1 == i);
	}
}

void CCtrlRadio::SetText(const char * sText,DWORD dwColor,DWORD dwMouseOnColor,DWORD dwCheckedColor,DWORD dwDisableColor,int iFontSize,DWORD dwFlag,int iFont,int iOffX,int iOffY,int iGap)
{
	CControl::SetText(sText);

	m_dwColor = dwColor;
	m_dwMouseOnColor = dwMouseOnColor;
	m_dwCheckedColor = dwCheckedColor;
	m_dwDisableColor = dwDisableColor;

	SetFont(iFont,iFontSize,dwFlag);

	m_dwBackColor = 0;

	m_iTextOffX = iOffX;
	m_iTextOffY = iOffY;
}

//void CCtrlRadio::ReleaseRadioTex()
//{
//	for(int i = 0; i < 4; i++)
//	{
//		if(m_vTexs[i])
//		{
//			g_pTexMgr->ReleaseTex(m_vTexs[i]);
//		}
//	}
//}

void CCtrlRadio::LoadRadioTex( int &iW,int &iH )
{
	iW = 0;
	iH = 0;

	for(int i = 0; i < 4; i++)
	{
		if(m_vTexID[i])
		{
			LPTexture pT = g_pTexMgr->GetTexImm(MAKELONG(m_vTexID[i], PACKAGE_INTERFACE),EP_UI);
			//m_vTexs[i] = pT;

			// 取得纹理的大小
			if((iW == 0 || iH == 0) && pT)
			{
				iW = pT->GetWidth();
				iH = pT->GetHeight();
			}
		}
	}

	if (m_bUseBackTexWidth)
	{
		m_iWidth = iW;
	}
	if (m_bUseBackTexHeight)
	{
		m_iHeight = iH;
	}

}

void CCtrlRadio::ReloadTex( WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex /*= 0*/ )
{
	if(m_vTexID[0] == wTex && m_vTexID[1] == wMTex && m_vTexID[2] == wCTex)
		return;

	//ReleaseRadioTex();

	m_vTexID[0] = wTex;
	m_vTexID[1] = wMTex;
	m_vTexID[2] = wCTex;
	m_vTexID[3] = wDTex;

	LoadRadioTex(m_iBackTexWidth,m_iBackTexHeight);
}

void CCtrlRadio::ResetWidthAndHeight(int iW,int iH)
{
	if (m_iBackTexWidth == 0)
	{
		m_iBackTexWidth = iW;
	}
	if (m_iBackTexHeight == 0)
	{
		m_iBackTexHeight = iH;
	}

	int iNewW = m_iWidth,iNewH = m_iHeight;
	if (m_bUseBackTexWidth)
	{
		iNewW = m_iBackTexWidth;
	}
	if (m_bUseBackTexHeight)
	{
		iNewH = m_iBackTexHeight;
	}

	ReSize(iNewW,iNewH);

	SetMask((WORD)m_iMask);

	ResetControlPos();
}
