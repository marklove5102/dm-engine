#include "CtrlMultiEdit.h"
#include "BaseClass\Misc\Input.h"
#include "Global/GlobalMsg.h"
#include "GameData/GameGlobal.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CCtrlMultiEdit, CControl)

CCtrlMultiEdit::CCtrlMultiEdit(int iMaxSize,int iRowSize, int iRows, bool bCanEdit,int iRowHeigh):
m_iMaxSize(iMaxSize),
m_iRowSize(iRowSize)
{
	m_pScroll = NULL;
	m_iControlMode = CTRL_MODE_MULIEDIT;

 	m_bCanEdit		= bCanEdit;
	m_iDisRows		= iRows;

	m_iCursor		= 0;
	m_iLines		= 0;
	m_iDisLine		= 0;	
	m_dwColor		= 0xFFFFFFFF;
	m_dwHighColor	= 0xFF0000FF;

	m_bSelecting	= false;
	m_iSelBegin		= 0;
	m_iSelEnd		= 0;

	m_bAsc			= false;
	m_bChanged		= false;
	m_iLineHeight   = iRowHeigh;
	memset(&m_stLineTemp,0,sizeof(stLine));

	m_pUpButton = NULL;
	m_pDownButton = NULL;
	m_pScroll = NULL;

	UpdateLineData();
}

CCtrlMultiEdit::~CCtrlMultiEdit(void)
{

}

bool CCtrlMultiEdit::Create(CControl * pParent, int iFontSize, int iX, int iY, int iW, int iH,bool bNewVersion)
{
	if( !pParent )
		return false;
	m_iFontSize = 12;
	SetFocus();
	m_bNewVersion = bNewVersion;
	return CControl::Create(pParent,iX,iY,iW,iH);
}

void CCtrlMultiEdit::OnCreate(void)
{
	if (m_bNewVersion && !m_pScroll)
	{
		m_pScroll = new CCtrlScroll();
		AddControl(m_pScroll);		
		m_pScroll->CreateEx(this,m_iWidth - 16,0,16,m_iHeight);
	}
}

void CCtrlMultiEdit::Draw(void)
{
	CControlContainer::Draw();

    int x,y;
	x = m_iScreenX + 4;
	y = m_iScreenY + 4;

	if(m_pScroll)
	{
		m_iDisLine = m_pScroll->GetPos();
	}

	for(int i = 0; i < m_iDisRows; i++)
	{
		
		if(i + m_iDisLine >= m_iLines )
		{
			OnDraw();			
			return;
		}
		DrawHighArea(i + m_iDisLine,x,y);										//高亮选中区域

		stLine& st = m_ArrayLine[i + m_iDisLine];

		if(st.iEPos > st.iBPos)
		{
			std::string sss = m_sEditText.substr(st.iBPos,st.iEPos-st.iBPos);
			g_pFont->DrawText(x, y, sss.c_str(), m_dwColor,m_iFont,m_iFontSize);			//输出文字			
		}

		if(m_bCanEdit && m_bFocus && (GetTickCount()/500)%2 != 0 && CursorInLine(i + m_iDisLine))	//光标                                              // 有焦点就输出光标
		{
			g_pFont->DrawText(x+STRLEN(m_sEditText.c_str()+st.iBPos,m_sEditText.c_str()+m_iCursor,m_iFontSize)-4, y, "|", 0xFF00FF00);			//输出文字
		}
		y += m_iLineHeight;
		
	}
	OnDraw();	
}

bool CCtrlMultiEdit::OnLeftButtonDown(int iX, int iY)
{
	int x,y;
	GetMousePos(x,y,iX,iY);
	if(m_iDisLine + y >= m_iLines)
		y = m_iLines - m_iDisLine - 1;
	m_bSelecting = true;
	m_iCursor = m_ArrayLine[m_iDisLine + y].iBPos + x;
	if(m_iCursor > m_ArrayLine[m_iDisLine + y].iEPos)
		m_iCursor = m_ArrayLine[m_iDisLine + y].iEPos;
	if(m_iCursor < m_ArrayAsc.size() && m_ArrayAsc[m_iCursor] == false)
		m_iCursor--;
	m_iSelBegin = m_iSelEnd = m_iCursor;
	return CControl::OnLeftButtonDown(iX,iY) ;
}

bool CCtrlMultiEdit::OnLeftButtonUp(int iX, int iY)
{
	m_bSelecting = false;
	return CControl::OnLeftButtonUp(iX,iY) ;
}

bool CCtrlMultiEdit::OnMouseMove(int iX, int iY)
{
	if(m_bSelecting)
	{
		int x,y;
		GetMousePos(x,y,iX,iY);
		if(m_iDisLine + y >= m_iLines)
			y = m_iLines - m_iDisLine - 1;
		m_iCursor = m_ArrayLine[m_iDisLine + y].iBPos + x;
		if(m_iCursor > m_ArrayLine[m_iDisLine + y].iEPos)
			m_iCursor = m_ArrayLine[m_iDisLine + y].iEPos;
		if(m_iCursor < m_ArrayAsc.size() && m_ArrayAsc[m_iCursor] == false)
			m_iCursor--;
		m_iSelEnd = m_iCursor;
	}
	return true;
}
void CCtrlMultiEdit::AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pUpButton)
		RemoveControl((CControl**)&m_pUpButton);

	m_pUpButton = new CCtrlButton();
	AddControl(m_pUpButton);
	m_pUpButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CCtrlMultiEdit::AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pDownButton)
		RemoveControl((CControl**)&m_pDownButton);

	m_pDownButton = new CCtrlButton();
	AddControl(m_pDownButton);
	m_pDownButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CCtrlMultiEdit::AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx)
{
	if(m_pScroll)
		RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,iX,iY,iW,iH,iScrollIdx);
	m_pScroll->SetPos(m_iDisLine);
	m_pScroll->SetRange(m_iLines - m_iDisRows);
}

void CCtrlMultiEdit::AddScrollEx(int iX,int iY,int iW,int iH,bool bShow,int iStep)
{
	if(m_pScroll)
		RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,iX,iY,iW,iH);
	m_pScroll->SetShow(bShow);

	if (iStep > 0)
	{
		m_pScroll->SetStep(iStep);
	}
	m_pScroll->SetPos(m_iDisLine);
	m_pScroll->SetRange(m_iLines - m_iDisRows);
}

bool CCtrlMultiEdit::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pUpButton)
			{
				OnWheel(WHEEL_DELTA);
				return true;
			}
			else if(pControl == m_pDownButton)
			{
				OnWheel(-WHEEL_DELTA);
				return true;
			}
			break;
		}
	default:
		break;
	}

	return CControlContainer::Msg(dwMsg,dwData,pControl);
}

bool CCtrlMultiEdit::OnChar(UCHAR cChar)
{
	if(!m_bCanEdit || (cChar < 0x20 && cChar != 13) || cChar == 127)   // 特殊字符不接收
		return false;
	if(m_iSelBegin != m_iSelEnd && m_iSelBegin >= 0 && m_iSelEnd >= 0)
		OnDel();
	bool bInserted = false;
	bInserted = InsertChar(cChar);
	if(cChar == 13)
	{
		char c = 10;
		InsertChar(c);
	}
	if(bInserted)
        UpdateLineData();
	return true;
}

bool CCtrlMultiEdit::OnWheel(int iWheel)
{
	if(m_pScroll)
	{
		m_pScroll->OnWheel(iWheel);
	}
	else
	{
		short iWheelSize = (short)iWheel;
		iWheelSize /= WHEEL_DELTA;
		m_iDisLine -= iWheelSize;
		if(m_iDisLine < 0 || m_iLines <= m_iDisRows)
			m_iDisLine = 0;
		else if(m_iDisLine + m_iDisRows > m_iLines)
			m_iDisLine = m_iLines - m_iDisRows;

	}

	return true;
}

void CCtrlMultiEdit::SetText(const char * sText,int iFont,int iFontSize,DWORD dwFlag,int iOffX,int iOffY,DWORD dwColor,DWORD dwSelectColor,DWORD dwSelectBackColor,DWORD dwDisableColor,int iGap)
{
//	if(sText == NULL || strlen(sText) == 0 || strlen(sText) > m_iMaxSize)
	if(sText == NULL || strlen(sText) > m_iMaxSize)
		return;

	m_sEditText = sText;
	m_iFont = iFont;
	m_iFontSize = iFontSize;
	m_dwFontFlag = dwFlag;
	m_iTextOffX = iOffX;
	m_iTextOffY = iOffY;
	m_dwColor = dwColor;
	m_dwHighColor = dwSelectBackColor;

	bool bAsc = false;
	int i = 0;
	int iLen = strlen(sText);
	m_ArrayAsc.resize(iLen);
	while(i < iLen)
	{
		
		unsigned char uChar = sText[i];
		if(bAsc || uChar == 10)
		{
			bAsc = false;
			m_ArrayAsc[i] = false;
		}
		else if(uChar >= 128)
		{
			bAsc = true;
			m_ArrayAsc[i] = true;
		}
		else
		{
			bAsc = false;
			m_ArrayAsc[i] = true;
		}
		i++;
		
	}
	UpdateLineData();
	m_iCursor = 0;
	m_iDisLine = 0;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
		m_pScroll->SetRange(m_iLines - m_iDisRows);
	}
	m_bSelecting = false;   
	m_iSelBegin = m_iSelEnd = 0;
}

const char * CCtrlMultiEdit::GetText(void)
{
	return m_sEditText.c_str();
}

void CCtrlMultiEdit::GetLineText(char * sText,int iLine)
{
	if(iLine >= m_iLines || iLine < 0 || sText == NULL)
		return;
	if(m_ArrayLine[iLine].iBPos >= m_ArrayLine[iLine].iEPos)
	{
		sText[0] = 0;
		return;
	}
	memcpy(sText,m_sEditText.c_str() + m_ArrayLine[iLine].iBPos,m_ArrayLine[iLine].iEPos - m_ArrayLine[iLine].iBPos);
	sText[m_ArrayLine[iLine].iEPos - m_ArrayLine[iLine].iBPos] = 0;
}

int CCtrlMultiEdit::GetLines(void)
{
	return m_iLines;
}

bool CCtrlMultiEdit::OnKeyDown(WORD wState, UCHAR cKey)
{
	switch(cKey)
	{
	case VK_UP:			// Up键
		{
			OnUp();
			break;
		}
	case VK_DOWN:		// Down键
		{
			OnDown();
			break;
		}
	case VK_LEFT:		// 处理"<-"键
		{
			OnLeft();
			break;
		}
	case VK_RIGHT:		// 处理"->"键 
		{
			OnRight();
			break;
		}
	case VK_BACK:		// BACK键处理
		{
			OnBack();
			break;
		}
	case VK_DELETE:		// 处理Del键
		{
			OnDel();
			break;
		}   
	case VK_HOME:		// 处理Home键
		{
			OnHome();
			break;
		}
	case VK_END:		// 处理End键
		{  
			OnEnd();
			break;
		}
	case 'c':
	case 'C':
		{
			if(g_pInput->IsCtrl())
                OnCopy();
			break;
		}
	case 'x':
	case 'X':
		{
			if(g_pInput->IsCtrl())
                OnCut();
			break;
		}
	case 'v':
	case 'V':
		{
			if(g_pInput->IsCtrl())
                OnPaste();
			break;
		}
	case 'a':
	case 'A':
		{
			if(g_pInput->IsCtrl())
				OnSelectAll();
			break;
		}
	default:
		break;
	}					// switch(cKey)结束
	return true;
}

void CCtrlMultiEdit::OnUp(void)								// Up键
{
	int iLine = GetCursorLine();
	if(iLine == 0)
		return;

	UpdateSelect();

	int n = m_iCursor - m_ArrayLine[iLine].iBPos;
	m_iCursor = m_ArrayLine[iLine - 1].iBPos + n;
	if(m_iCursor > m_ArrayLine[iLine - 1].iEPos)
		m_iCursor = m_ArrayLine[iLine - 1].iEPos;
	if(m_iCursor < m_ArrayAsc.size() && m_ArrayAsc[m_iCursor] == false)		//判断是否是汉字
		m_iCursor--;
	if(m_iDisLine > iLine - 1)
		m_iDisLine = iLine - 1;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
	}

	UpdateSelect();
}

void CCtrlMultiEdit::OnDown(void)							// Down键
{
	int iLine = GetCursorLine();
	if(iLine + 1 >= m_iLines)
		return;
	UpdateSelect();
	int n = m_iCursor - m_ArrayLine[iLine].iBPos;
	m_iCursor = m_ArrayLine[iLine + 1].iBPos + n;
	if(m_iCursor > m_ArrayLine[iLine + 1].iEPos)
		m_iCursor = m_ArrayLine[iLine + 1].iEPos;
	if(m_iCursor < m_ArrayAsc.size() && m_ArrayAsc[m_iCursor] == false)		//判断是否是汉字	
		m_iCursor--;
	if(m_iDisLine + m_iDisRows <= iLine + 1)
		m_iDisLine = iLine - m_iDisRows + 2;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
	}

	UpdateSelect();
}

void CCtrlMultiEdit::OnLeft(void)							// <-键
{
	if(m_iCursor <= 0)
		return;
	UpdateSelect();
	m_iCursor--;
	if(m_ArrayAsc[m_iCursor] == false)		//判断是否是汉字
		m_iCursor--;

	int iLine = GetCursorLine();
	if(m_iDisLine > iLine)
		m_iDisLine = iLine;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
	}

	UpdateSelect();
}

void CCtrlMultiEdit::OnRight(void)							// ->键
{
	if(m_iCursor >= m_sEditText.size())
	{
		if(m_iCursor > m_sEditText.size())
			m_iCursor = m_sEditText.size();
		return;
	}
	UpdateSelect();
	m_iCursor++;
	if(m_iCursor < m_ArrayAsc.size() && m_ArrayAsc[m_iCursor] == false)		//判断是否是汉字
		m_iCursor++;

	int iLine = GetCursorLine();
	if(m_iDisLine + m_iDisRows <= iLine)
		m_iDisLine = iLine - m_iDisRows + 1;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
	}

	UpdateSelect();
}

void CCtrlMultiEdit::OnBack(void)							// Back键
{
	if(!m_bCanEdit)
		return;
	m_bChanged = true;
	int iDel = 0;
	if(m_iSelBegin != m_iSelEnd)			//删除选中的字符
	{
		if(m_iSelBegin > m_iSelEnd)
		{
			int iTemp   = m_iSelBegin;
			m_iSelBegin = m_iSelEnd;
			m_iSelEnd   = iTemp; 
		}
		m_iCursor = m_iSelBegin;
		iDel = m_iSelEnd - m_iSelBegin;
	}
	else									//删除光标后面的字符
	{
		
		if(m_iCursor == 0)
			return;
		iDel = 1;
		if(m_ArrayAsc[m_iCursor - 1] == false)
			iDel++;
		m_iCursor -= iDel;
		
	}
	DelStr(m_iCursor,iDel);
	m_bSelecting = false;
	m_iSelBegin = m_iSelEnd = 0;

	int iLine = GetCursorLine();
	if(m_iDisLine > iLine)
		m_iDisLine = iLine;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
	}

}

void CCtrlMultiEdit::OnDel(void)							// Del键
{
	if(!m_bCanEdit)
		return;
	m_bChanged = true;
	int iDel;
	if(m_iSelBegin != m_iSelEnd)			//删除选中的字符
	{
		if(m_iSelBegin > m_iSelEnd)
		{
			int iTemp   = m_iSelBegin;
			m_iSelBegin = m_iSelEnd;
			m_iSelEnd   = iTemp; 
		}
		m_iCursor = m_iSelBegin;
		iDel = m_iSelEnd - m_iSelBegin;
	}
	else							//删除光标后面的字符
	{
		
		if(m_iCursor == m_sEditText.size())
			return;
		iDel = 1;
		if(m_iCursor + 1 < m_ArrayAsc.size() && m_ArrayAsc[m_iCursor + 1] == false)
			iDel++;
		
	}
	DelStr(m_iCursor,iDel);
	m_bSelecting = false;
	m_iSelBegin = m_iSelEnd = 0;

	int iLine = GetCursorLine();
	if(m_iDisLine > iLine)
		m_iDisLine = iLine;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
	}

}

void CCtrlMultiEdit::OnHome(void)							// Home键
{
	UpdateSelect();

	int iOff	= 1;
	char *p		= (char *)(m_sEditText.c_str() + m_iCursor);
	while(true)
	{
		p--;
		if(*p == 10)
			break;
		else if(p <= m_sEditText.c_str())
		{
			iOff = 0;
			break;
		}
	}
	m_iCursor = (int)(p - m_sEditText.c_str() + iOff);
	UpdateSelect();
}

void CCtrlMultiEdit::OnEnd(void)							// End键
{
	UpdateSelect();
	m_iCursor = m_ArrayLine[GetCursorLine()].iEPos;
	UpdateSelect();
}

void CCtrlMultiEdit::OnCopy(void)							// 拷贝
{
	if(m_iSelBegin == m_iSelEnd)
		return;
	Copy(m_iSelBegin,m_iSelEnd);
}

void CCtrlMultiEdit::OnPaste(void)							// 粘贴
{
	if(!m_bCanEdit)
		return;
	if(m_iSelBegin != m_iSelEnd)
		OnDel();

	string str;
	if(!PasteFromClipboard(str))
		return;

	InsertStr(m_iCursor,(char*)str.c_str());

	if(m_iLines > m_iDisRows)
	{
		int iLine = GetCursorLine();
		if( m_iDisLine < iLine - m_iDisRows + 1)
		{
			m_iDisLine = iLine - m_iDisRows + 1;
			if(m_pScroll)
			{
				m_pScroll->SetPos(m_iDisLine);
			}
		}
	}
}

void CCtrlMultiEdit::OnCut(void)							// 剪切
{
	if(!m_bCanEdit)
		return;
	OnCopy();
	OnDel();
}

void CCtrlMultiEdit::OnSelectAll(void)						// 全部选中
{
	m_iSelBegin = 0;
	m_iSelEnd	= m_sText.size();
	m_iCursor   = m_iSelEnd; 

	int iLine = GetCursorLine();
	if(m_iDisLine + m_iDisRows <= iLine)
	{
		m_iDisLine = iLine - m_iDisRows + 1;
		if(m_pScroll)
		{
			m_pScroll->SetPos(m_iDisLine);
		}
	}
}

void CCtrlMultiEdit::DrawHighArea(int iLine,int ix,int iy)
{
	if(m_iSelEnd == m_iSelBegin || iLine >= m_iLines || iLine < 0)
		return;
	int iB,iE;
	if(m_iSelEnd < m_iSelBegin)
	{
		iB = m_iSelEnd;
		iE = m_iSelBegin;
	}
	else
	{
		iE = m_iSelEnd;
		iB = m_iSelBegin;
	}

	if(m_ArrayLine[iLine].iBPos > iE || m_ArrayLine[iLine].iEPos < iB)
		return;

    int x1,y1,x2,y2;
	x1 = ix;
	y1 = iy;
	y2 = y1 + m_iLineHeight;

	if(iB > m_ArrayLine[iLine].iBPos)
		x1 += STRLEN(m_sEditText.c_str() + m_ArrayLine[iLine].iBPos,m_sEditText.c_str() + iB,m_iFontSize);

	if(iE < m_ArrayLine[iLine].iEPos)
		x2 = ix + STRLEN(m_sEditText.c_str() + m_ArrayLine[iLine].iBPos,m_sEditText.c_str() + iE,m_iFontSize);
	else
		x2 = ix + STRLEN(m_sEditText.c_str() + m_ArrayLine[iLine].iBPos,m_sEditText.c_str() + m_ArrayLine[iLine].iEPos,m_iFontSize);

	if(x2 > x1)
	{
		//g_pCG->DrawFilledRectangle(m_dwHighColor,x1,y1,x2,y2);
		g_pGfx->DrawFillRect(x1,y1,x2-x1,y2-y1,m_dwHighColor);
	}
}

bool CCtrlMultiEdit::InsertChar(UCHAR cChar)
{
	if(!m_bCanEdit)
		return false;
	int iLen = m_sEditText.size();
	if(iLen >= m_iMaxSize || m_iCursor < 0 || m_iCursor > iLen)
		return false;
	m_bChanged = true;
    if(m_iCursor > iLen)
		m_iCursor = iLen;
	if(m_iCursor < 0)
		m_iCursor = 0;
	m_sEditText.insert(m_iCursor,1,cChar);
	if(m_bAsc || cChar == 10)
	{
		m_bAsc = false;
		m_ArrayAsc.insert(m_ArrayAsc.begin()+m_iCursor,false);
	}
	else if(cChar >= 128)
	{
		m_bAsc = true;
		m_ArrayAsc.insert(m_ArrayAsc.begin()+m_iCursor,true);
	}
	else
	{
		m_bAsc = false;
		m_ArrayAsc.insert(m_ArrayAsc.begin()+m_iCursor,true);
	}
	m_iCursor++;
	return true;
}

void CCtrlMultiEdit::UpdateLineData(void)
{
	ClearLineData();

	m_iLines = 0;


	const char *pText = m_sEditText.c_str();
	char *p = (char *)pText;

	while(1)
	{
		
		if(m_iLines >= m_ArrayLine.size())
			m_ArrayLine.push_back(m_stLineTemp);
		m_ArrayLine[m_iLines].iBPos = (int)(p - pText);
		if(*p == 0)											// 行首在结束位置
		{
			
			m_ArrayLine[m_iLines].iEPos	= m_ArrayLine[m_iLines].iBPos;
			m_ArrayLine[m_iLines].cEndType = 0;
			m_iLines++;
			break;
			
		}
		else if(*p == 13)									// 行首直接换行
		{
			
			m_ArrayLine[m_iLines].iEPos	= m_ArrayLine[m_iLines].iBPos;
			m_ArrayLine[m_iLines].cEndType = 1;
			m_iLines++;
			p += 2;
			
		}
		else												// 中途结束，硬换行，软换行
		{
			char *pp = p;
			while(1)
			{
				
				if(*p == 0)									// 中途结束
				{
					m_ArrayLine[m_iLines].iEPos = (int)(p - pText);
					m_ArrayLine[m_iLines].cEndType = 0;
					m_iLines++;
                    break;                                        
				}
				else if(*p == 13)							// 中途硬换行
				{
					m_ArrayLine[m_iLines].iEPos = (int)(p - pText);
					m_ArrayLine[m_iLines].cEndType = 1;
					m_iLines++;
					break;
				}
				else if(STRLEN(pp,p,m_iFontSize) >= (m_iRowSize * m_iFontSize / 2))		// 中途软换行
				{
					if(m_ArrayAsc[(int)(p - pText)] == false)	// 汉字需要退格
						p--;
					m_ArrayLine[m_iLines].iEPos = (int)(p - pText);
					m_ArrayLine[m_iLines].cEndType = 2;
					m_iLines++;
					break;
				}
				else
					p++;
				
			}
			if(*p == 0)
				break;
			if(*p == 13)
			{
				p += 1;
				if (*p == 0x0A && (p - pText < m_sEditText.size() - 1))
				{
					p += 1;
				}
			}
		} 
		
	}

	int iLine = GetCursorLine();
	if(m_iDisLine + m_iDisRows < iLine + 1)
		m_iDisLine = iLine - m_iDisRows + 1;

	if(m_iDisLine < 0)
		m_iDisLine = 0;

	if(m_pScroll)
	{
		m_pScroll->SetRange(m_iLines - m_iDisRows);
		m_pScroll->SetPos(m_iDisLine);
	}
}

void CCtrlMultiEdit::UpdateSelect(void)
{
	if(g_pInput->IsShift())
	{
		if(m_bSelecting)				// 更新选中区域
			m_iSelEnd = m_iCursor;
		else							// 开始纪录选中区域
		{
			m_bSelecting = true;
			m_iSelEnd	= m_iCursor ;
			m_iSelBegin = m_iSelEnd;
		}
	}
	else
	{
		m_bSelecting = false;
		m_iSelBegin = m_iSelEnd = 0;
	}
}

int  CCtrlMultiEdit::GetCursorLine(void)
{
	if(m_iLines > m_ArrayLine.size())
	{
		m_iLines = m_ArrayLine.size();
		if(m_pScroll)
		{
			m_pScroll->SetRange(m_iLines - m_iDisRows);
		}
	}

	if(m_iCursor == m_sEditText.size())
		return m_iLines - 1;
	for(int i = 0; i < m_iLines; i++)
	{
		if(m_ArrayLine[i].iBPos == m_ArrayLine[i].iEPos)
		{
			if(m_iCursor == m_ArrayLine[i].iBPos)
				return i;
		}
		else
		{
			if((m_iCursor >= m_ArrayLine[i].iBPos && m_iCursor < m_ArrayLine[i].iEPos) || (m_iCursor == m_ArrayLine[i].iEPos && m_ArrayLine[i].cEndType != 2))
				return i;
			if(m_iCursor == m_ArrayLine[i].iEPos && m_ArrayLine[i].cEndType == 2)
				return i + 1;
		}
	}
	return m_iLines - 1;
}

bool CCtrlMultiEdit::CursorInLine(int iLine)
{
	if(iLine >= m_iLines)
		return false;
	if((m_iCursor >= m_ArrayLine[iLine].iBPos && m_iCursor < m_ArrayLine[iLine].iEPos) || (m_iCursor == m_ArrayLine[iLine].iEPos && m_ArrayLine[iLine].cEndType != 2))
		return true;
	else 
		return false;
}

bool CCtrlMultiEdit::Copy(int iBegin, int iEnd)
{
	if(iBegin == iEnd)                                          
		return false;
	if(iBegin > iEnd)
	{
		int iTemp = iBegin;
		iBegin = iEnd;
		iEnd = iTemp;
	}
	if(iEnd > m_sEditText.size())
		iEnd = m_sEditText.size();

	if(iEnd - iBegin > 1024)
		return false;

	string str = m_sEditText.substr(iBegin,iEnd - iBegin);

	CopyText2Clip(str.c_str(),str.size());

	return true;
}

void CCtrlMultiEdit::InsertStr(int iPos, char * sText)
{
	int n = strlen(sText);
	if(!m_bCanEdit || sText == NULL || iPos < 0 || iPos > m_sEditText.size() || m_sEditText.size() + n > m_iMaxSize)
		return;

	m_sEditText.insert(iPos,sText);
	
	if(m_iCursor >= iPos)
		m_iCursor += n;

	m_bChanged = true;
	bool bAsc = false;
	m_ArrayAsc.insert(m_ArrayAsc.begin()+iPos,n,true);

	for(int i = 0; i < n; i++)
	{
		BYTE uChar = sText[i];
		if(bAsc || uChar == 10)
		{
			bAsc = false;
			m_ArrayAsc[iPos + i] = false;
		}
		else if(uChar >= 128)
		{
			bAsc = true;
			m_ArrayAsc[iPos + i] = true;
		}
		else
		{
			bAsc = false;
			m_ArrayAsc[iPos + i] = true;
		}
	}
 	UpdateLineData();
}

void CCtrlMultiEdit::DelStr(int iPos, int iNum)
{
	if(!m_bCanEdit || iPos >= m_sEditText.size() || iPos + iNum > m_sEditText.size() 
		|| m_sEditText.size() != m_ArrayAsc.size())
		return;

	m_sEditText.erase(m_sEditText.begin()+iPos,m_sEditText.begin()+iPos+iNum);
	m_ArrayAsc.erase(m_ArrayAsc.begin()+iPos,m_ArrayAsc.begin()+iPos+iNum);

	UpdateLineData();
}

void CCtrlMultiEdit::ClearLineData(void)
{
	m_ArrayLine.assign(m_iLines,m_stLineTemp);
}

void CCtrlMultiEdit::GetMousePos(int&x, int&y, int iX, int iY)
{
	if(iX < 4)
		iX = 0;
	if(iX > m_iWidth - 4)
		iX = m_iWidth - 4;
	if(iY < 4)
		iY = 0;
	if(iY > m_iHeight - 4)
		iY = m_iHeight - 4;
	y = iY / m_iLineHeight;

	if(m_iDisLine + y >= m_iLines)
		y = m_iLines - m_iDisLine - 1;

	assert(y >= 0);
	x = m_ArrayLine[y].iEPos - m_ArrayLine[y].iBPos;
	for(int i = m_ArrayLine[y].iBPos; i < m_ArrayLine[y].iEPos; i++)
	{
		if(iX >= STRLEN(m_sEditText.c_str() + m_ArrayLine[y].iBPos,m_sEditText.c_str() + i,m_iFontSize)
			&& iX < STRLEN(m_sEditText.c_str() + m_ArrayLine[y].iBPos,m_sEditText.c_str() + i + 1,m_iFontSize))
		{
			x = i - m_ArrayLine[y].iBPos;
			break;
		}
	}
}

void CCtrlMultiEdit::SetDisplayRows(int iDisRows)
{
	m_iDisRows = iDisRows;
}

void CCtrlMultiEdit::SetHighColor(DWORD dwHighColor)
{
	m_dwHighColor = dwHighColor;
}
bool CCtrlMultiEdit::IsChanged(void)
{
	return m_bChanged;
}

bool CCtrlMultiEdit::OnLeftButtonDClick(int iX, int iY)
{
	if(m_pParent)
		m_pParent->Msg(MSG_INPUT_LEFTBT_DCLICK,iY,this);
	return true;
}

void CCtrlMultiEdit::SetDisLine(int iPos)
{
	if(iPos < 0 || iPos >= m_iLines)
		return;
	m_iDisLine = iPos;
	if(m_pScroll)
	{
		m_pScroll->SetPos(m_iDisLine);
	}

}

void CCtrlMultiEdit::SetNoFocus(bool bNoFocus)
{
	m_bNoFocus = bNoFocus;
}

int	CCtrlMultiEdit::GetDisRows()			
{ 
	return m_iDisRows;
}			//取得屏幕可显示的行数

int	CCtrlMultiEdit::GetDisLine()			
{ 
	return m_iDisLine;
}

void CCtrlMultiEdit::SetEditable(bool b)	
{ 
	m_bCanEdit = b;	
}

bool CCtrlMultiEdit::IsEditable()	
{ 
	return m_bCanEdit;
}

void CCtrlMultiEdit::SetSelRange(int iBeginPos,int iEndPos)
{
	int iSize = m_sEditText.size();
	if(iBeginPos>= iSize)
		iBeginPos = iSize - 1;
	if(iBeginPos<0)
		iBeginPos = 0;

	if(iEndPos>= iSize)
		iEndPos = iSize - 1;
	if(iEndPos<0)
		iEndPos = 0;

	if(iBeginPos<= iEndPos)
	{
		m_iSelBegin = iBeginPos;
		m_iSelEnd = iEndPos;
	}else
	{
		m_iSelBegin = iEndPos;
		m_iSelEnd = iBeginPos;
	}
}