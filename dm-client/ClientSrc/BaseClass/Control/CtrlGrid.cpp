#include "CtrlGrid.h"


DTI_IMPLEMENT(CCtrlGrid, CControl)

CCtrlGrid::CCtrlGrid()
{
	m_dwHotColor = 0xFFFF0000;
	m_dwTextColor = 0xFFFFFFFF;
	m_dwSelBackColor = 0xFF2F8405;
	m_dwSelTextColor = 0xFFFFFF00;
	m_dwHotBackColor = 0;//0xFF216C4D;
	m_dwBackColor = 0;
	m_iHotLine = -1;
	m_iTotalCount = 0;
	m_iSelLine = -1;
	m_iSelCol = 0;
	m_iFontSize = FONTSIZE_DEFAULT;
	m_iFont = FONT_DEFAULT;
	m_bDrawSelectedFrame = true;
	m_bDrawHotLineEffect = true;
	m_bCenter = false;
	m_bSelAllCols = true;
	m_iDrawOffX = 0;
	m_iDrawOffY = 14 + 4;//14:向上按钮的高度
	m_iScrollWidth = 0;
	m_iDevSelectedTex = 0;
	m_fRatio = 1.0;

	m_wSelectTex = 0;
	m_iLineNumType = ECT_NONE;
	m_pUpButton = NULL;
	m_pDownButton = NULL;
	m_pScroll = NULL;
	m_bNewVersion = false;
}

CCtrlGrid::~CCtrlGrid(void)
{
	m_vColInfo.clear();
}

bool CCtrlGrid::Create(CControl * pParent,int l,int t,int r,int b,int lines,float h)
{
	m_fLineHeight = h;
	m_iLinesPerPage = lines;

	return	CControl::Create(pParent,l,t,r - l,b - t,CTRL_STYLE_BACKMODE_NODRAW,0x00FFFFFF,NULL);
}

bool CCtrlGrid::CreateEx(CControl * pParent,int l,int t,int r,int b,int lines,float h,WORD wSelectedTex,int iDev,float fRatio)
{
	m_wSelectTex = wSelectedTex;
	m_bNewVersion = true;
	m_iDevSelectedTex = iDev;
	m_fRatio = fRatio;
	return Create(pParent,l,t,r,b,lines,h);
}

void CCtrlGrid::OnCreate(void)
{
	if (m_bNewVersion && !m_pScroll)
	{
		m_pScroll = new CCtrlScroll();
		AddControl(m_pScroll);
		m_pScroll->CreateEx(this,m_iWidth - 14,0,14,m_iHeight);
	}
}

void CCtrlGrid::AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pUpButton)
		RemoveControl((CControl**)&m_pUpButton);

	m_pUpButton = new CCtrlButton();
	AddControl(m_pUpButton);
	m_pUpButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CCtrlGrid::AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pDownButton)
		RemoveControl((CControl**)&m_pDownButton);

	m_pDownButton = new CCtrlButton();
	AddControl(m_pDownButton);
	m_pDownButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CCtrlGrid::AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx,bool bShow)
{
	if(m_pScroll)
		RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,iX,iY,iW,iH,iScrollIdx);
	m_iScrollWidth = iW;

	m_pScroll->SetShow(bShow);
}

void CCtrlGrid::AddScrollEx(int iX,int iY,int iW,int iH,bool bShow,int iStep)
{
	if(m_pScroll)
		RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,iX,iY,iW,iH);
	m_iScrollWidth = iW;

	m_pScroll->SetShow(bShow);
	if (iStep > 0)
	{
		m_pScroll->SetStep(iStep);
	}
}

void CCtrlGrid::Draw(void)
{
	CControlContainer::Draw();
	
	//先画选中行，列效果底框
	int iPos = 0;
	if(m_pScroll)
		iPos = m_pScroll->GetPos();

	if(m_bDrawSelectedFrame &&  m_iSelLine >= 0 && m_iSelLine >= iPos && m_iSelLine < m_iTotalCount && m_iSelLine < iPos + m_iLinesPerPage)
	{
		int iX = m_iScreenX + 1;
		int iY = m_iScreenY + m_iDrawOffY + (int)((m_iSelLine - iPos) * m_fLineHeight) - 1;
		int iWidth = m_iWidth - m_iScrollWidth - 2;

		if(!m_bSelAllCols && m_iSelCol > 0 && m_iSelCol < (int)m_vColInfo.size())
		{
			iX = m_iScreenX + m_vColInfo[m_iSelCol].iX;
			iWidth = m_vColInfo[m_iSelCol].iWidth - 1;
		}


		if (m_bNewVersion) iWidth -= 14;
		if (!m_wSelectTex)
			g_pGfx->DrawFillRect(iX + m_iDevSelectedTex,iY,iWidth,m_iFontSize + 2,m_dwSelBackColor);
		else
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,m_wSelectTex,EP_UI);
			if (pTex)
			{
				g_pGfx->DrawTextureNL_Trans(iX + m_iDevSelectedTex,iY + 17,pTex,m_fRatio);
				//g_pGfx->DrawTextureNL(iX + m_iDevSelectedTex,iY - 2,pTex);
			}
		}
	}
	//画鼠标指向的行背景效果
	if(m_bDrawHotLineEffect && m_iHotLine != m_iSelLine && m_iHotLine >= 0 && m_iHotLine >= iPos && m_iHotLine < m_iTotalCount && m_iHotLine < iPos + m_iLinesPerPage)
	{
		int iY = m_iScreenY + m_iDrawOffY + (int)((m_iHotLine - iPos) * m_fLineHeight);
		g_pGfx->DrawFillRect(m_iScreenX + 1,iY - 1,m_iWidth - m_iScrollWidth - 2,m_iFontSize + 2,m_dwHotBackColor);
	}
}

bool CCtrlGrid::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pUpButton)
			{
				if(m_pScroll)
					m_pScroll->GoDown();

				return true;
			}
			else if(pControl == m_pDownButton)
			{
				if(m_pScroll)
					m_pScroll->GoUp();

				return true;
			}
			break;
		}
	default:
		break;
	}

	return CControlContainer::Msg(dwMsg,dwData,pControl);
}

bool CCtrlGrid::OnMouseMove(int iX,int iY)
{
	if(iX > m_iWidth - m_iScrollWidth || iY < m_iDrawOffY)
	{
		m_iHotLine = -1;
	}
	else
	{
		iY -= m_iDrawOffY;
		m_iHotLine = (int)(iY / m_fLineHeight);
		if(m_pScroll)
			m_iHotLine += m_pScroll->GetPos();

		if(m_iHotLine >= m_iTotalCount || m_iHotLine < 0)
			m_iHotLine = -1;
	}
	
	return CControlContainer::OnMouseMove(iX,iY);
}

bool CCtrlGrid::OnWheel(int iWheel)
{
	if(m_pScroll)
		m_pScroll->OnWheel(iWheel);

	return true;
}

bool CCtrlGrid::OnLeftButtonDown(int iX,int iY)
{
	m_bClick = true;
	Click(iX,iY);
	return true;
}

bool CCtrlGrid::OnLeftButtonUp(int iX,int iY)
{
	m_bClick = false;
	if(m_pParent && m_iSelLine >= 0 && m_iSelCol >= 0 && m_iSelLine < m_iTotalCount && m_iSelCol < m_vColInfo.size())
		m_pParent->Msg(MSG_CTRL_GRID_SELECT_LINE_COL,MAKELONG(m_iSelLine,m_iSelCol),this);

	return true;
}

bool CCtrlGrid::OnLeftButtonDClick(int iX,int iY)
{
	m_bClick = false;
	if(m_pParent && m_iSelLine >= 0 && m_iSelCol >= 0 && m_iSelLine < m_iTotalCount && m_iSelCol < m_vColInfo.size())
		m_pParent->Msg(MSG_CTRL_GRID_DBCLICK,MAKELONG(m_iSelLine,m_iSelCol),this);

	return true;
}

bool CCtrlGrid::OnRightButtonDown(int iX,int iY)
{
	m_bRBClick = true;
	Click(iX,iY);
	return true;
}

bool CCtrlGrid::OnRightButtonUp(int iX,int iY)
{
	m_bRBClick = false;
	if(m_pParent && m_iSelLine >= 0 && m_iSelCol >= 0 && m_iSelLine < m_iTotalCount && m_iSelCol < m_vColInfo.size())
	{
		m_pParent->Msg(MSG_CTRL_GRID_SELECT_LINE_COL,MAKELONG(m_iSelLine,m_iSelCol),this);
		m_pParent->Msg(MSG_CTRL_GRID_RCLICK_LINE_COL,MAKELONG(m_iSelLine,m_iSelCol),this);
	}

	return true;
}

void CCtrlGrid::Click(int iX,int iY)
{
	if(iX > m_iWidth - m_iScrollWidth || iY < m_iDrawOffY)
	{
		m_iSelLine = -1;
		return;
	}

	iY -= m_iDrawOffY;
	//调整选中的行
	m_iSelLine = (int)(iY / m_fLineHeight);
	if(m_pScroll)
		m_iSelLine += m_pScroll->GetPos();

	if(m_iSelLine >= m_iTotalCount || m_iSelLine < 0)
		m_iSelLine = -1;

	if(m_iSelLine >= 0)
	{
		//在列表中的位置
		m_iSelCol = 0;
		for(int i = 0;i < (int)m_vColInfo.size();i++)
		{
			if(iX >=  m_vColInfo[i].iX && iX <= m_vColInfo[i].iX + m_vColInfo[i].iWidth)
			{
				//选中的列
				m_iSelCol = i;
				break;
			}
		}
	}
}

void  CCtrlGrid::SetSelLine(int iLine)
{
	if(iLine < 0 || iLine >= m_iTotalCount)
	{
		m_iSelLine = -1;
		return;
	}

	m_iSelLine = iLine;
	if(!m_pScroll)
		return;
	//自动调整滚动条的位置，使选中行在当前页中
	if( iLine >= m_pScroll->GetPos() && iLine < m_pScroll->GetPos() + m_iLinesPerPage)
	{
	}
	else if(iLine < m_pScroll->GetPos())
	{
		m_pScroll->SetPos(iLine);
	}
	else if(iLine >= m_pScroll->GetPos() + m_iLinesPerPage)
	{
		if(iLine < m_iTotalCount - m_iLinesPerPage)
		{
			m_pScroll->SetPos(iLine);
		}
		else
		{
			m_pScroll->SetPos(m_iTotalCount - m_iLinesPerPage);
		}
	}
}

void CCtrlGrid::SetTotalCount(int i)
{
	if(i == m_iTotalCount)
		return;

	m_iTotalCount = i;

	if(m_pScroll)
	{
		int iTemp = i - m_iLinesPerPage;
		if(iTemp < 0)
			iTemp = 0;

		m_pScroll->SetRange(iTemp);

		if(m_pScroll->GetPos() >= i)
			m_pScroll->SetPos(0);
		if(m_iSelLine >= m_iTotalCount)
			m_iSelLine = -1;
	}
}

void  CCtrlGrid::SetDisLine(int i)
{
	if(i < m_iTotalCount && m_pScroll)
		m_pScroll->SetPos(i);		
}

void CCtrlGrid::ClearColumn()
{
	m_vColInfo.clear();
}

void CCtrlGrid::PushColumn(int w)
{
	stColInfo tmp(0,w);
	for(int i = 0;i < (int)m_vColInfo.size();i++)
	{
		tmp.iX += m_vColInfo[i].iWidth;
	}
	m_vColInfo.push_back(tmp);
}

void  CCtrlGrid::SetScrollPos(int i)
{
	if(m_pScroll && i >= 0 && i < m_pScroll->GetRange())
		m_pScroll->SetPos(i);
}

bool CCtrlGrid::GetGridXY(int row,int col,int& x,int& y)
{
	if(row < 0 || col < 0 || row >= m_iLinesPerPage || col >= (int)m_vColInfo.size())
		return false;

	x = m_iScreenX + 2 + m_vColInfo[col].iX;
	y = m_iScreenY + m_iDrawOffY + (int)(row * m_fLineHeight);

	return true;
}

void  CCtrlGrid::DrawGridEx(int row,int col,const char* str,bool bCenter,WORD wBackTex,WORD wBotTex,DWORD dwColor,DWORD dwFlag,int iDevX,int iDevY)
{
	int _xx,_yy;
	if(!GetGridXY(row,col,_xx,_yy))
		return;

	int ix = _xx,iy = _yy;
	if(bCenter)
		_xx += (m_vColInfo[col].iWidth - strlen(str) * m_iFontSize/2) / 2;

	if(wBackTex == 0 && m_dwBackColor)
	{
		int _y = (int)(m_iScreenY + row * m_fLineHeight);
		if(col == 0)
			g_pGfx->DrawFillRect(m_iScreenX + 1,_y+1,m_iWidth - m_iScrollWidth - 2,(int)m_fLineHeight - 2,m_dwBackColor);
	}

	//如果是用数字图片做第一列
	if(m_iLineNumType == ECT_PIC && col == 0)
	{
		string strNum(str);
		for(size_t i=0;i < strNum.size();i++)
		{
			string strChar = string(&strNum.at(i),1);
			int num = atoi(strChar.c_str());
			LPTexture pNumTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,10800+num,EP_UI);
			if(pNumTex)
			{
				g_pGfx->DrawTextureNL(_xx + 4 + i*8 ,_yy,pNumTex);
			}
		}
	}
	else //正常的绘制
	{
		int iPos = 0;
		if(m_pScroll)
			iPos = m_pScroll->GetPos();

		if (wBackTex != 0)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,wBackTex,EP_UI);

			if (pTex) 
			{
				if (bCenter)
				{
					ix += (m_iWidth -  pTex->GetWidth())/2;
					iy -= (m_iHeight / m_iLinesPerPage - pTex->GetHeight())/2 - 3;
				}

				g_pGfx->DrawTextureNL(ix,iy,pTex);
				
				ix += pTex->GetWidth()/2;
				iy += pTex->GetHeight();
			}

            pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,wBotTex,EP_UI);
			if (pTex) g_pGfx->DrawTextureNL(ix - 15,iy + 2,pTex);			
		}

		//如果外面传了颜色过来以传过来的为准，因为有可能单格要用特殊的颜色
		if(dwColor != 0)
		{
			g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,dwColor,m_iFont,m_iFontSize,dwFlag);
		}
		else
		{
			if(row + iPos == m_iHotLine)
				g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,m_dwHotColor,m_iFont,m_iFontSize,dwFlag);
			else if(row + iPos == m_iSelLine && (m_bSelAllCols || m_iSelCol == 0 || m_iSelCol == col) )
				g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,m_dwSelTextColor,m_iFont,m_iFontSize,dwFlag);
			else
				g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,m_dwTextColor,m_iFont,m_iFontSize,dwFlag);
		}
	}
}
void CCtrlGrid::DrawGrid(int row,int col,const char* str,bool bCenter,DWORD dwColor,DWORD dwFlag,int iDevX,int iDevY)
{
	int _xx,_yy;
	if(!GetGridXY(row,col,_xx,_yy))
		return;

	if(bCenter)
		_xx += (m_vColInfo[col].iWidth - (int)(strlen(str)) * m_iFontSize/2) / 2;

	if(m_dwBackColor)
	{
		int _y = (int)(m_iScreenY + row * m_fLineHeight);
		if(col == 0)
			g_pGfx->DrawFillRect(m_iScreenX + 1,_y+1,m_iWidth - m_iScrollWidth - 2,(int)m_fLineHeight - 2,m_dwBackColor);
	}

	//如果是用数字图片做第一列
	if(m_iLineNumType == ECT_PIC && col == 0)
	{
		string strNum(str);
		for(size_t i=0;i < strNum.size();i++)
		{
			string strChar = string(&strNum.at(i),1);
			int num = atoi(strChar.c_str());
			LPTexture pNumTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,10800+num,EP_UI);
			if(pNumTex)
			{
				g_pGfx->DrawTextureNL(_xx + 4 + i*8 ,_yy,pNumTex);
			}
		}
	}
	else //正常的绘制
	{
		int iPos = 0;
		if(m_pScroll)
			iPos = m_pScroll->GetPos();

		//如果外面传了颜色过来以传过来的为准，因为有可能单格要用特殊的颜色
		if(dwColor != 0)
		{
			if(row + iPos == m_iHotLine)
				 g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,m_dwHotColor,m_iFont,m_iFontSize,dwFlag);
			else g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,dwColor,m_iFont,m_iFontSize,dwFlag);
		}
		else
		{
			if(row + iPos == m_iHotLine)
				g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,m_dwHotColor,m_iFont,m_iFontSize,dwFlag);
			else if(row + iPos == m_iSelLine && (m_bSelAllCols || m_iSelCol == 0 || m_iSelCol == col) )
				g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,m_dwSelTextColor,m_iFont,m_iFontSize,dwFlag);
			else
				g_pFont->DrawText(_xx + 3 + iDevX,_yy + iDevY,str,m_dwTextColor,m_iFont,m_iFontSize,dwFlag);
		}
	}
}

void CCtrlGrid::DrawGrid(int row,int col,int iPackage,WORD wTex,bool bCenter,int iOffX,int iOffY)
{
	int _xx,_yy;
	if(!GetGridXY(row,col,_xx,_yy))
		return;

	if(m_dwBackColor)
	{
		int _y = (int)(m_iScreenY + row * m_fLineHeight);
		if(col == 0)
			g_pGfx->DrawFillRect(m_iScreenX + 1,_y+1,m_iWidth - m_iScrollWidth - 2,(int)m_fLineHeight - 2,m_dwBackColor);
	}

	LPTexture pTex = g_pTexMgr->GetTexImm(iPackage,wTex,EP_UI);
	if(pTex == NULL)
		return;

	if(m_bCenter)
		_xx += (m_vColInfo[col].iWidth - pTex->GetWidth0() * m_iFontSize/2) / 2;

	int iPos = 0;
	if(m_pScroll)
		iPos = m_pScroll->GetPos();
	if(row + iPos == m_iHotLine)
		g_pGfx->DrawTextureNL(_xx + 3 + iOffX,_yy + iOffY,pTex,m_dwHotColor);
	else if(row + iPos == m_iSelLine && (m_bSelAllCols || m_iSelCol == 0 || m_iSelCol == col) )
		g_pGfx->DrawTextureNL(_xx + 3 + iOffX,_yy + iOffY,pTex,m_dwSelTextColor);
	else
		g_pGfx->DrawTextureNL(_xx + 3 + iOffX,_yy + iOffY,pTex,m_dwTextColor);
}

void CCtrlGrid::MoveAsNeed(int iPos)
{
	int iDisLine = this->GetDisLine();

	if(iDisLine + m_iLinesPerPage >= m_iTotalCount)
		iDisLine = m_iTotalCount - m_iLinesPerPage;

	if(iPos >= 0 && iPos < iDisLine)
		iDisLine = iPos;
	else if(iPos >= iDisLine + m_iLinesPerPage)
		iDisLine = iPos - m_iLinesPerPage + 1;

	SetDisLine(iDisLine);

	m_iSelLine = iPos;
	m_iSelCol = 0;
}