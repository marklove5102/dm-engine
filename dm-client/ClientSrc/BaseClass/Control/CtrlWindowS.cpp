#include "CtrlWindowS.h"
#include "CtrlButton.h"
#include "GameData/ConfigData.h"

////////////////////////////////////////////////////////////////////////////////////
//只在第一次或者不保存上次位置属性的情况下：有XML,优先从Xml读取,没有Xml从宏定义读取
//其余情况都从上次保存的位置读取，并把对齐方式改为TOPLEFT

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DTI_IMPLEMENT(CCtrlWindowS, CCtrlWindow)

CCtrlWindowS::CCtrlWindowS(void):
CCtrlWindow()
{
	m_iControlMode		= CTRL_MODE_WINDOW;
	m_pCloseButton		= NULL;
	m_bMoving           = false;
	m_pCloseButton      = NULL;
	m_bNeedSavePage     = false;
	m_bNeedSavePos      = true;
	m_iIndex            = 0;
	m_iCurPage          = 0;
	m_iPages            = 1;
	m_iOldx = m_iOldy   = -1;
	m_iVersion          = 1;

	m_vTexturesID.clear();
	m_vRect.clear();
	m_iInputOffX = m_iInputOffY = POS_AUTO;

	m_uStyle = CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX | CTRL_STYLE_TRANS;
}

CCtrlWindowS::~CCtrlWindowS(void)
{
	//for(int i = 0; i < (int)(m_vTextures.size()); i++)
	//{
	//	LPTexture pT = m_vTextures[i];
	//	g_pTexMgr->ReleaseTex(pT);
	//}

	SaveWndPos();

}

// 创建窗口
bool CCtrlWindowS::Create(CControl* pParent,int iX,int iY,int iCurPage)
{
	if(pParent == NULL)
		return false;

	stSPos Pos;
	bool  bIsFirstRun = !(g_pControl->GetSPos(m_sName.c_str(),Pos));
	if(bIsFirstRun)
	{
		Pos.iLastX = GetWndSX();
		Pos.iLastY = GetWndSY();
	}

	//可以自动记录坐标
	if(bIsFirstRun || !m_bNeedSavePage)
	{
		// 取得页数以及当前页
		if(iCurPage >= m_iPages)
			iCurPage = 0;

		m_iCurPage = iCurPage;
	}
	else
	{
		m_iCurPage = Pos.iLastPage;
	}

	int iW = m_iWidth,iH = m_iHeight;
	m_iInputOffX = iX;
	m_iInputOffY = iY;

	// 计算新窗口的大小
	if (m_iVersion == 1)
	{
		// 读取纹理
		for(int i = 0; i < m_iPages; i++)
		{
			DWORD dwIndex = 0;
			int iIndex = 0;
			if(i < m_vIndex.size())
				dwIndex = MAKELONG(m_vIndex.at(i),PACKAGE_INTERFACE);
			else
				dwIndex = MAKELONG((m_iIndex+i), PACKAGE_INTERFACE);

			//pT = g_pTexMgr->LoadTex(dwIndex);
			m_vTexturesID.push_back(dwIndex);
		}

		LPTexture pT = g_pTexMgr->GetTexImm(m_vTexturesID[m_iCurPage],EP_DONT_DOWNLOAD);//这里如果取不到图不要去下载,因为很多SetMask在Oncreate里做的,mask要求比背景先下载到,这里不下载,在绘制背景的时候会下载的

		if(m_iWidth != 0)//如果程序内容设置死了大小以程序设置的为准
		{
			iW = m_iWidth;
		}
		else if(pT)
		{
			iW = pT->GetWidth();
		}

		if(m_iHeight != 0)//如果程序内容设置死了大小以程序设置的为准
		{
			iH = m_iHeight;
		}
		else if(pT)
		{
			iH = pT->GetHeight();
		}

		for(int i = 0; i < m_iPages; i++)
		{
			RECT rc;
			rc.left		= i * iW / m_iPages;
			rc.top		= 0;
			rc.right	= (i + 1) * iW / m_iPages;
			rc.bottom	= min(iH,20);

			m_vRect.push_back(rc);
		}
	}
	else
	{
		if(m_TabPage.iCurPage < m_TabPage.vSubTabPage.size())
		{
			LPTexture pT = g_pTexMgr->GetTexImm(m_TabPage.vSubTabPage[m_TabPage.iCurPage].dwBackTex,EP_UI);
			if(pT)
			{
				iW = pT->GetWidth();
				iH = pT->GetHeight();
			}
		}
	}


	//可以自动记录坐标
	if(bIsFirstRun || !m_bNeedSavePos)
	{
		if(m_pXmlControl == NULL)
		{
			if(iX == POS_AUTO || iY == POS_AUTO)
			{
				m_iAlignType = XAL_TOPLEFT;

				iX = Pos.iLastX;
				iY = Pos.iLastY;

				if(iX == POS_AUTO)
					iX = (pParent->GetWidth() - iW) / 2;
				else if(iX == POS_VARIABLE)
					iX = m_iOffX;

				if(iY == POS_AUTO)
					iY = (pParent->GetHeight() - 80 - iH) / 2;
				else if(iY == POS_VARIABLE)
					iY = m_iOffY;
			}
			else if(iX == POS_VARIABLE || iY == POS_VARIABLE)
			{
				iX = m_iOffX;
				iY = m_iOffY;
			}
		}
	}
	else
	{
		iX = Pos.iLastX;
		iY = Pos.iLastY;

		m_iOffX = iX;
		m_iOffY = iY;
		m_iAlignType = XAL_TOPLEFT;
	}

	if(!CControl::Create(pParent,iX,iY,iW,iH,m_uStyle))
		return false;


	return true;
}

// 切换到当前页
void CCtrlWindowS::SwitchToPage(int iPage,S_TabPage * pParentTabPage,bool bReCreate)
{
	if(m_iVersion == 1 && (iPage == m_iCurPage || iPage >= m_iPages))
		return;
	else if (m_iVersion == 2 && (pParentTabPage == NULL || iPage >= pParentTabPage->vSubTabPage.size() || (!bReCreate && iPage == pParentTabPage->iCurPage)))
		return;
	else if (pParentTabPage && (iPage >= pParentTabPage->vSubTabPage.size() || (!bReCreate && iPage == pParentTabPage->iCurPage)))//为了兼容m_iVersion == 1的时候也能使用AddTabPage
		return;

	// 清除上一页上的控件
	RemoveBodyChildControl();

	// 计算新窗口的大小
	if (m_iVersion == 1)
	{
		m_iCurPage = iPage;
	}
	else
	{
		pParentTabPage->iCurPage = iPage;
	}

	int iW = m_iWidth,iH = m_iHeight;

	if (m_iVersion == 1)
	{
		LPTexture pT = g_pTexMgr->GetTexImm(m_vTexturesID[m_iCurPage],EP_UI);
		if(pT)
		{
			iW = pT->GetWidth();
			iH = pT->GetHeight();
		}
	}
	else
	{
		if(m_TabPage.iCurPage < m_TabPage.vSubTabPage.size())
		{
			LPTexture pT = g_pTexMgr->GetTexImm(m_TabPage.vSubTabPage[m_TabPage.iCurPage].dwBackTex,EP_UI);
			if(pT)
			{
				iW = pT->GetWidth();
				iH = pT->GetHeight();
			}
		}
	}

	ReSize(iW,iH);

	// 添加该页的子控件
	OnCreate();
}

void CCtrlWindowS::ReCreateTabBtn()
{
	ClearTabBtn(&m_TabPage);
	this->OnCreate();
}

void CCtrlWindowS::DrawBkg()
{
	if(!m_bShow) 
	{
		return;
	}

	// 绘制背景
	if (m_iVersion == 1)
	{
		if(m_iCurPage < m_vTexturesID.size())
		{
			if(m_vTexturesID[m_iCurPage])
			{
				LPTexture pTex = g_pTexMgr->GetTexImm(m_vTexturesID[m_iCurPage],EP_UI);
				if (pTex)
				{
					//刚开始没有图,后来读到图了,重设大小及位置
					if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0)
					{
						ResetWidthAndHeight(pTex->GetWidth(),pTex->GetHeight());
					}

					DWORD dwColor = -1;
					if(m_uStyle & CTRL_STYLE_TRANS)
						dwColor = g_Config.GetTransColor();

					if (m_bScale)
					{
						g_pGfx->DrawTextureFX(m_iScreenX + (int)((m_iFrame + m_iTexOffX)*g_ScaleRate.fx),
							m_iScreenY + (int)((m_iFrame + m_iTexOffY)*g_ScaleRate.fy),
							pTex, dwColor, NULL, &g_ScaleRate);
					}
					else
					{
						g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pTex,dwColor);
					}
				}

			}
		}
	}
	else
	{
		S_TabPage * p = &m_TabPage;
		while(p)
		{
			if(p->vSubTabPage.size() > 0 && p->iCurPage < p->vSubTabPage.size())
			{
				if (p->vSubTabPage[p->iCurPage].dwBackTex > 0)
				{
					if (!p->pParentTabPage || (p->pParentTabPage->iCurPage < p->pParentTabPage->vSubTabPage.size() && p->vSubTabPage[p->iCurPage].dwBackTex != p->pParentTabPage->vSubTabPage[p->pParentTabPage->iCurPage].dwBackTex))
					{
						LPTexture pTex = g_pTexMgr->GetTexImm(p->vSubTabPage[p->iCurPage].dwBackTex,EP_UI);

						if(pTex)
						{
							//刚开始没有图,后来读到图了,重设大小及位置
							if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0)
							{
								ResetWidthAndHeight(pTex->GetWidth(),pTex->GetHeight());
							}

							DWORD dwColor = -1;
							if(m_uStyle & CTRL_STYLE_TRANS)
								dwColor = g_Config.GetTransColor();

							if (m_bScale)
							{
								g_pGfx->DrawTextureFX(m_iScreenX + (int)((m_iFrame  + p->iBackTexX)*g_ScaleRate.fx),
									m_iScreenY + (int)((m_iFrame + p->iBackTexY)*g_ScaleRate.fy),
									pTex, dwColor, NULL, &g_ScaleRate);
							}
							else
							{
								g_pGfx->DrawTextureNL(m_iScreenX + p->iBackTexX,m_iScreenY + p->iBackTexY,pTex,dwColor);
							}
						}
					}

				}

				p = &(p->vSubTabPage[p->iCurPage]);
			}
			else
			{
				break;
			}
		}
	}
}

void CCtrlWindowS::ResetWidthAndHeight(int iW,int iH)
{
	if(m_iVersion == 1)
	{
		m_vRect[m_iCurPage].left = m_iCurPage * iW / m_iPages;
		m_vRect[m_iCurPage].top		= 0;
		m_vRect[m_iCurPage].right	= (m_iCurPage + 1) * iW / m_iPages;
		m_vRect[m_iCurPage].bottom	= min(iH,20);
	}

	if(m_pXmlControl == NULL)//如果m_pXmlControl存在,那么以xml里设置的为准,在assignxml里已经设置好了,这里不用自动设置了
	{
		stSPos Pos;
		bool  bIsFirstRun = !(g_pControl->GetSPos(m_sName.c_str(),Pos));
		if(bIsFirstRun)
		{
			Pos.iLastX = GetWndSX();
			Pos.iLastY = GetWndSY();
		}

		if(m_iInputOffX == POS_AUTO && Pos.iLastX == POS_AUTO)
		{
			m_iOriginalOffX = (m_pParent->GetWidth() - iW) / 2;
		}
		if(m_iInputOffY == POS_AUTO && Pos.iLastY == POS_AUTO)
		{
			m_iOriginalOffY = (m_pParent->GetHeight() - 80 - iH) / 2;
		}
	}


	ReSize(iW,iH);

	SetMask((WORD)m_iMask);

	ResetControlPos();
}

bool CCtrlWindowS::RelativeMove(int iType)
{
	return CCtrlWindow::RelativeMove(iType);
}


void CCtrlWindowS::Draw()
{
	if(!m_bShow) return;

	//绘制背景
	DrawBkg();

	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	// 绘制所有子控件
	CControlContainer::Draw();

}

// 删除所有窗体中的控件
void CCtrlWindowS::RemoveBodyChildControl(void)
{
	CControl * p = m_pControls;
	CControl * pTemp = m_pControls;

	while( p )
	{
		pTemp = p->GetControlNext();
		if( p && p != m_pCloseButton)	// 删除关闭按钮以外的控件
		{
			RemoveControl(&p);	
		}
		p = pTemp;
	}

	p = m_pLastDrawControls;
	pTemp = m_pLastDrawControls;
	while( p )
	{
		pTemp = p->GetControlNext();
		if(p)
		{
			RemoveControl(&p,true);	
		}
		p = pTemp;
	}

	if (m_iVersion != 1)
	{
		ClearTabBtn(&m_TabPage);
	}
}

bool CCtrlWindowS::OnLeftButtonDown(int iX, int iY)
{
	if(m_iPages <= 1)
		return CControl::OnLeftButtonDown(iX,iY);

	m_bMoving = true;

	if (m_iVersion == 1)
	{
		int iPage = PointInPage(iX,iY);
		if(iPage >= 0)
		{
			SwitchToPage(iPage);
		}
	}

	return CControl::OnLeftButtonDown(iX,iY);
}

bool CCtrlWindowS::OnLeftButtonUp(int iX, int iY)
{
	if(m_bMoving)
		m_bMoving = false;
	return CControl::OnLeftButtonUp(iX,iY);
}

int CCtrlWindowS::PointInPage(int iX, int iY)
{
	for(int i = 0; i < m_iPages; i++)
	{
		if(iX > m_vRect[i].left && iX <= m_vRect[i].right && iY > m_vRect[i].top && iY <= m_vRect[i].bottom)
			return i;
	}

	LPTexture pTex = g_pTexMgr->GetTexImm(m_vTexturesID[0],EP_UI);
	if(pTex && iY < pTex->GetHeight())
		return -1;
	else
		return -2;
}

void CCtrlWindowS::SetPageRect(int iPage,int left,int top,int right,int bottom)
{
	if (m_iVersion == 1)
	{
		if(iPage < m_iPages && iPage >= 0)
		{
			m_vRect[iPage].left		= left;
			m_vRect[iPage].right	= right;
			m_vRect[iPage].top		= top;
			m_vRect[iPage].bottom	= bottom;
		}
	}
}

void CCtrlWindowS::SetPageTex(int iPage,int iPackage,WORD wIdx)
{
	if (m_iVersion == 1)
	{
		DWORD id = MAKELONG(wIdx,iPackage);

		if(iPage < 0 || iPage >= m_vTexturesID.size() || m_vTexturesID[iPage] == id)
			return;

		m_vTexturesID[iPage] = id;

		LPTexture pTex = g_pTexMgr->GetTexImm(id,EP_UI);
		//SetPageTex(iPage,pTex);
		if(pTex)
		{
			//if(m_iWidth == 0 || m_iHeight == 0)
			{
				m_iOriginalWidth = pTex->GetWidth();
				m_iWidth = m_iOriginalWidth;
				m_iOriginalHeight = pTex->GetHeight();
				m_iHeight = m_iOriginalHeight;
			}
		}
	}
}

//void CCtrlWindowS::SetPageTex(int iPage,LPTexture pTex)
//{
//	if (m_iVersion == 1)
//	{
//		if(iPage < 0 || iPage >= m_vTextures.size())
//			return;
//
//		if(m_vTextures[iPage])
//		{
//			g_pTexMgr->ReleaseTex(m_vTextures[iPage]);
//		}
//		m_vTextures[iPage] = pTex;
//		if(pTex)
//		{
//			//if(m_iWidth == 0 || m_iHeight == 0)
//			{
//				m_iWidth = pTex->GetWidth();
//				m_iHeight = pTex->GetHeight();
//			}
//		}
//	}
//}

int  CCtrlWindowS::GetCurPage()
{
	if (m_iVersion == 1)
	{
		return m_iCurPage;
	}
	else
	{
		return 0;
	}	
}

void CCtrlWindowS::OnCreate(void)
{
	S_TabPage * p = &m_TabPage;

	while(true)
	{
		//先创建当前页签并列的所有页签按钮
		for(int i = 0; i < p->vSubTabPage.size(); i++)
		{
			S_TabPage *pPage = &(p->vSubTabPage[i]);
			pPage->pTabBtn = new CCtrlButton();
			AddControl(pPage->pTabBtn);
			pPage->pTabBtn->CreateEx(this,pPage->iTabBtnTexX,pPage->iTabBtnTexY,pPage->wTabBtnTex,pPage->wTabBtnMonuseOnTex,pPage->wTabBtnClickedTex,pPage->wTabBtnTexDisableTex);
			pPage->pTabBtn->SetText(pPage->strTabText.c_str(),pPage->dwTabTextColor,pPage->dwTabTextMouseOnColor ,pPage->dwTabTextClickColor,pPage->dwTabTextDisableColor,pPage->iTabTextFontSize,pPage->dwTabTextFlag,pPage->iTabTextFont,pPage->bTabTextVertical,pPage->iTextOffX,pPage->iTextOffY,pPage->iTabTextGap);
			pPage->pTabBtn->SetTextOff(pPage->iTextOffX, pPage->iTextOffY);
			if (!pPage->szTips.empty())
			{
				pPage->pTabBtn->SetTips(pPage->szTips.c_str());
			}
			if (i == p->iCurPage)
			{
				pPage->pTabBtn->SetBackState(BTEX_PUSHED);
			}
		}

		if (p->vSubTabPage.size() > 0 && p->iCurPage < p->vSubTabPage.size())
		{
			p = &(p->vSubTabPage[p->iCurPage]);
		}
		else
		{
			break;
		}
	}

	CCtrlWindow::OnCreate();	
}

S_TabPage* CCtrlWindowS::AddTabPage(int iBackTexX,int iBackTexY,DWORD dwBackTex,int iTabBtnTexX,int iTabBtnTexY,WORD wTabBtnTex,WORD wTabBtnMonuseOnTex,WORD wTabBtnClickedTex,WORD wTabBtnTexDisableTex,const char *strTabText,S_TabPage *pParentTabPage,bool bTabTexVertical,int iTextOffX,int iTextOffY,int iSwitchPageType,int iTabTextFont,int iTabTextFontSize,bool bReCreateSamePageWhenClickTabBtn,const char * pTips)
{

	S_TabPage subPage;

	if (pParentTabPage == NULL)
	{
		pParentTabPage = &m_TabPage;
	}

	subPage.iBackTexX = iBackTexX;
	subPage.iBackTexY = iBackTexY;
	subPage.dwBackTex = dwBackTex;


	subPage.iTabBtnTexX = iTabBtnTexX;
	subPage.iTabBtnTexY = iTabBtnTexY;

	subPage.wTabBtnTex = wTabBtnTex;
	subPage.wTabBtnMonuseOnTex = wTabBtnMonuseOnTex;
	subPage.wTabBtnClickedTex = wTabBtnClickedTex;
	subPage.wTabBtnTexDisableTex = wTabBtnTexDisableTex;
	subPage.iTabTextFont = iTabTextFont;
	subPage.iTabTextFontSize = iTabTextFontSize;

	if (strTabText)
	{
		subPage.strTabText = strTabText;
	}

	subPage.bTabTextVertical = bTabTexVertical;
	subPage.iTextOffX = iTextOffX;
	subPage.iTextOffY = iTextOffY;
	subPage.iSwitchPageType = iSwitchPageType;
	subPage.bReCreateSamePageWhenClickTabBtn = bReCreateSamePageWhenClickTabBtn;

	if (pTips)
	{
		subPage.szTips = pTips;
	}

	subPage.pParentTabPage = pParentTabPage;
	pParentTabPage->vSubTabPage.push_back(subPage);
	return &(pParentTabPage->vSubTabPage[pParentTabPage->vSubTabPage.size() - 1]);
}

void CCtrlWindowS::SetTabText(S_TabPage *pTabPage,const char * sText,DWORD dwTabTextColor,DWORD dwTabTextMouseOnColor,DWORD dwTabTextClickColor,DWORD dwTabTextDisableColor,int iTabTextFontSize,DWORD dwTabTextFlag,int iTabTextFont,bool bTabTextVertical,int iTabTextGap,int iTextOffX,int iTextOffY,const char * pTips)
{
	pTabPage->dwTabTextColor = dwTabTextColor;
	pTabPage->dwTabTextMouseOnColor = dwTabTextMouseOnColor;
	pTabPage->dwTabTextClickColor = dwTabTextClickColor;
	pTabPage->dwTabTextDisableColor = dwTabTextDisableColor;

	if (sText)
	{
		pTabPage->strTabText = sText;
	}

	pTabPage->iTabTextFontSize = iTabTextFontSize;
	pTabPage->dwTabTextFlag = dwTabTextFlag;
	pTabPage->iTabTextFont = iTabTextFont;
	pTabPage->bTabTextVertical = bTabTextVertical;
	pTabPage->iTabTextGap = iTabTextGap;

	pTabPage->iTextOffX = iTextOffX;
	pTabPage->iTextOffY = iTextOffY;

	if (pTips)
	{
		pTabPage->szTips = pTips;
	}
}

void CCtrlWindowS::ClearTabBtn(S_TabPage *pTabPage)
{
	pTabPage->pTabBtn = NULL;

	for(int i = 0; i < pTabPage->vSubTabPage.size(); i++)
	{
		ClearTabBtn(&(pTabPage->vSubTabPage[i]));
	}
}

bool CCtrlWindowS::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			S_TabPage * p = &m_TabPage;
			while(true)
			{
				for(int i = 0; i < p->vSubTabPage.size(); i++)
				{
					if (pControl == p->vSubTabPage[i].pTabBtn)
					{
						if (p->vSubTabPage[i].iSwitchPageType == 0)
						{
							SwitchToPage(i,p,p->vSubTabPage[i].bReCreateSamePageWhenClickTabBtn);
						}
						else if (!(this->Msg(MSG_CTRL_TABPAGE_TABCHANGE, i, (CControl *)p)))
						{
							SwitchToPage(i,p,p->vSubTabPage[i].bReCreateSamePageWhenClickTabBtn);
						}

						return true;
					}
				}

				if (p->vSubTabPage.size() > 0 && p->iCurPage < p->vSubTabPage.size())
				{
					p = &(p->vSubTabPage[p->iCurPage]);
				}
				else
				{
					break;
				}
			}
		}

		break;
	default:
		break;
	}

	return  CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

void CCtrlWindowS::SaveWndPos()
{
	int iX = GetX();
	int iY = GetY();
	if(iX < -m_iWidth || iY < -m_iHeight || iX > g_pGfx->GetWidth() || iY > g_pGfx->GetHeight())//如果在屏幕外面那么不记录这一次的窗口位置,
	{
		g_pControl->DelSPos(m_sName.c_str());
		return;
	}

	stSPos Pos;
	Pos.iLastX = GetWndSX();
	Pos.iLastY = GetWndSY();

	if(m_bNeedSavePos)
	{
		Pos.iLastX = iX;
		Pos.iLastY = iY;
	}

	if(m_bNeedSavePage)
	{
		Pos.iLastPage = m_iCurPage;
	}

	g_pControl->SetSPos(m_sName.c_str(),Pos);
}
