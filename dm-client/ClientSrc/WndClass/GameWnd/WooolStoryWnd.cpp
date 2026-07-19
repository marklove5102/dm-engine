#include "WooolStoryWnd.h"
#include "GameAI/AIConfigMgr.h"
#include "GameControl/GameControl.h"


INIT_WND_POSX(CWooolStoryWnd,POS_VARIABLE,POS_VARIABLE)

CWooolStoryWnd::CWooolStoryWnd(void)
{
	m_bNoMove = true;
	m_iCurPage = 0;
	m_bScale = g_bNeedScale;


	m_iIndex = 0;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 0;
	m_iWidth = g_pGfx->GetWidth();
	m_iHeight = g_pGfx->GetHeight();

	//预读
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,14209,EP_UI);
	g_pTexMgr->GetTex(PACKAGE_INTERFACE,14210,EP_UI);

}

CWooolStoryWnd::~CWooolStoryWnd(void)
{
}

void CWooolStoryWnd::OnCreate()
{
	//下一页按钮
	m_pNextPageBtn = new CCtrlButton();
	AddControl(m_pNextPageBtn);
	m_pNextPageBtn->SetAlignType(XAL_BOTTOM);
	//上一页按钮
	m_pPrePageBtn = new CCtrlButton();
	AddControl(m_pPrePageBtn);
	m_pPrePageBtn->SetAlignType(XAL_BOTTOM);

	m_pCloseBtn = new CCtrlButton();
	AddControl(m_pCloseBtn);
	m_pCloseBtn->SetAlignType(XAL_BOTTOM);

	int iOffY = -60;
	if(g_pGfx->GetWidth() == 1024)
	{
		iOffY = -150;
	}
	else if(g_pGfx->GetWidth() == 1280)
	{
		iOffY = -180;
	}

	m_pPrePageBtn->CreateEx(this,-60,iOffY + 3,14245,14246,14247);
	m_pNextPageBtn->CreateEx(this,60,iOffY + 3,14248,14249,14250);
	m_pCloseBtn->CreateEx(this,0 ,iOffY,14251,14252,14253);
}

void CWooolStoryWnd::Draw(void)
{
	g_pGfx->DrawFillRect(m_iX,m_iY,m_iWidth,m_iHeight,0xD0000000);

	//画插画
	LPTexture pTex = NULL;
	int iOffY = 10;

	if(g_pGfx->GetWidth() > 800)
		iOffY = 88;

	int iBkgIdx = 14207;
	int iOffX = 0;
	if (m_iCurPage == WOOLSTORY_PAGES - 1)//最后一页
	{
		iBkgIdx = 14208;
		iOffX = -(1280 - g_pGfx->GetWidth());
	}
	else if (m_iCurPage == 0)//第一页
	{
		iBkgIdx = 14209;
	}
	else//中间的
	{
		iBkgIdx = 14208;
		iOffX = -(1280 - g_pGfx->GetWidth());
	}

	pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,iBkgIdx,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(iOffX,iOffY,pTex);
	}


	iOffX = 0;
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14210 + m_iCurPage,EP_UI);

	if (g_pGfx->GetWidth() == 1280)
	{
		iOffX = 130;
	}
	else if (g_pGfx->GetWidth() == 1024)
	{
		iOffX = -10;
	}
	else if (g_pGfx->GetWidth() == 800)
	{
		iOffX = -100;
	}

	g_pGfx->DrawTextureNL(iOffX,iOffY,pTex);


	CCtrlWindowX::Draw();  
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

}

bool CWooolStoryWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pNextPageBtn)
		{
			m_iCurPage ++;
			if(m_iCurPage >= WOOLSTORY_PAGES)
			{
				m_iCurPage = WOOLSTORY_PAGES - 1;
			}
			else//预读下一页
				g_pTexMgr->GetTex(PACKAGE_INTERFACE,14210 + m_iCurPage,EP_UI);

			return true;
		}
		else if(pControl == m_pPrePageBtn)
		{
			m_iCurPage --;
			if(m_iCurPage < 0)
			{
				m_iCurPage = 0;
			}
			else//预读上一页
				g_pTexMgr->GetTex(PACKAGE_INTERFACE,14210 + m_iCurPage,EP_UI);

			return true;
		}
		else if(pControl == m_pCloseBtn)
		{
			OnClickCloseButton();
			return true;
		}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CWooolStoryWnd::ResetControlPos()
{
	m_bScale = g_bNeedScale;
	CCtrlWindowX::ResetControlPos();
}

