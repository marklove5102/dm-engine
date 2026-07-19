#include "TodayActivityWnd.h"
#include "Global/Interface/GraphicInterface.h"

INIT_WND_POSX(CTodayActivityWnd,POS_VARIABLE,POS_VARIABLE)

CTodayActivityWnd::CTodayActivityWnd(void)
{
	m_bModel		= true;
	//m_bNoFocus = true;
	m_bNoMove  = true;
	//m_bNoChangeLevel=true;
	m_bScaleWidthAndHeight = false;

 	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 0;
	m_iWidth = g_pGfx->GetWidth();
	m_iHeight = g_pGfx->GetHeight();
	m_bScale = g_bNeedScale;


	if (m_iWidth == 800 || m_bScale)
	{
		m_iIndex = 15500;
	}
	else if (m_iWidth == 1024)
	{
		m_iIndex = 15501;
	}
	else
	{
		m_iIndex = 15502;
	}

	m_pActivityLocalWnd = NULL;
	m_pActivityIEWnd = NULL;

	FILE *fp = fopen("..\\Data\\config\\gamehint.txt","rb");
	if (fp)
	{
		char buff[512];
		while(fgets(buff,512,fp))
		{
			m_VMessages.push_back(buff);
		}
	}

	m_dwTime = GetTickCount();
	m_iDrawLine = -1;
	m_iHorseDrawX = 0;

	if (m_VMessages.size() > 0)
	{
		m_iDrawLine = rand() % m_VMessages.size();
	}

}

CTodayActivityWnd::~CTodayActivityWnd(void)
{
}

void CTodayActivityWnd::OnCreate()
{
	m_pActivityLocalWnd = new CTodayActivityLocalWnd();
	AddControl(m_pActivityLocalWnd);
	m_pActivityLocalWnd->Create(this);

	m_pActivityIEWnd = new CTodayActivityIEWnd();
	AddControl(m_pActivityIEWnd);
	m_pActivityIEWnd->Create(this);

	m_pStartButton = new CCtrlButton;
	AddControl(m_pStartButton);
	m_pStartButton->SetInheritScaleFromParent(false);
	if (g_pGfx->GetWidth() == 800)
	{
		m_pStartButton->CreateEx(this,-(1280 - g_pGfx->GetWidth()) / 2 + 600,g_pGfx->GetHeight() - 130 + 2,21594,21595,21596);
	}
	else
	{
		m_pStartButton->CreateEx(this,-(1280 - g_pGfx->GetWidth()) / 2 + 600,g_pGfx->GetHeight() - 160 + 2,21594,21595,21596);
	}

}

void CTodayActivityWnd::Draw(void)
{
	//CCtrlWindowX::Draw();

	if(!m_bShow) return;

	//绘制背景
	DrawBkg();

// 	if(g_pGfx->GetWidth() == 1024)
// 	{
// 		DrawTexture(0,0,15504);
// 	}
// 	else if(g_pGfx->GetWidth() == 800)
// 	{
// 		DrawTexture(0,0,15505);
// 	}
// 	else
// 	{
// 		DrawTexture(0,0,15506);
// 	} 	

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21593,EP_UI);

	if (g_pGfx->GetWidth() == 800)
	{
		g_pGfx->DrawTextureNL(-(1280 - g_pGfx->GetWidth()) / 2,g_pGfx->GetHeight() - 130,pTex);
	}
	else
	{
		g_pGfx->DrawTextureNL(-(1280 - g_pGfx->GetWidth()) / 2,g_pGfx->GetHeight() - 160,pTex);
	}	
	
	// 绘制所有子控件
	CControlContainer::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//绘制文字
	DWORD dwCurTm = GetTickCount();
	if (dwCurTm - m_dwTime >= 10)
	{
		m_iHorseDrawX ++;
		m_dwTime = dwCurTm;
	}



	if (m_iDrawLine >= 0 && m_iDrawLine < m_VMessages.size())
	{
		int iSize = - (int)(m_VMessages[m_iDrawLine].length()) * FONTSIZE_MIDDLE / 2;
		if (g_pGfx->GetWidth() - 200 * 2 - m_iHorseDrawX <= iSize)
		{
			int iNewName = rand() % m_VMessages.size();
			if (iNewName != m_iDrawLine)
			{
				m_iHorseDrawX = 0;
				m_iDrawLine = iNewName;
			}

			return;
		}

		if (g_pGfx->GetWidth() == 800)
		{
			g_pFont->DrawTextEx(g_pGfx->GetWidth() - 200 - m_iHorseDrawX,g_pGfx->GetHeight() - 30,200,g_pGfx->GetWidth() - 200,m_VMessages[m_iDrawLine].c_str(),0xFFFFFF00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
		}
		else
		{
			g_pFont->DrawTextEx(g_pGfx->GetWidth() - 200 - m_iHorseDrawX,g_pGfx->GetHeight() - 60,200,g_pGfx->GetWidth() - 200,m_VMessages[m_iDrawLine].c_str(),0xFFFFFF00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
		}			
	}

}

bool CTodayActivityWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(m_pStartButton == pControl )
			{
				CloseWindow();
				return true;
			}
		}
		break;
	case MSG_CTRL_ON_SET_FOCUS:
		{
			if (pControl == m_pActivityLocalWnd || pControl == m_pActivityIEWnd)
			{
				this->SwitchToTop(pControl);
			}
			return true;
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CTodayActivityWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	switch(cKey)
	{
	case VK_RETURN:
		{
			OnClickCloseButton();
			return true;
		}
	} 

	//return CCtrlWindowX::OnKeyDown(wState,cKey);
	return true;//截获所有按键
}

void CTodayActivityWnd::SetControlState()
{
	m_iOriginalWidth = g_pGfx->GetWidth();
	m_iOriginalHeight = g_pGfx->GetHeight();

	if (g_pGfx->GetWidth() == 800 || m_bScale)
	{
		SetPageTex(0,PACKAGE_INTERFACE,15500);
	}
	else if (g_pGfx->GetWidth() == 1024)
	{
		SetPageTex(0,PACKAGE_INTERFACE,15501);
	}
	else
	{
		SetPageTex(0,PACKAGE_INTERFACE,15502);
	}

	m_pStartButton->Destroy();

	if (g_pGfx->GetWidth() == 800)
	{
		m_pStartButton->CreateEx(this,-(1280 - g_pGfx->GetWidth()) / 2 + 600,g_pGfx->GetHeight() - 130 + 2,21594,21595,21596);
	}
	else
	{
		m_pStartButton->CreateEx(this,-(1280 - g_pGfx->GetWidth()) / 2 + 600,g_pGfx->GetHeight() - 160 + 2,21594,21595,21596);
	}
}

void CTodayActivityWnd::ResetControlPos()
{
	SetControlState();
	m_bScale = g_bNeedScale;
	CCtrlWindowX::ResetControlPos();
}
