#include "glyphedit.h"
#include "GameData/GameGlobal.h"
#include "GameData/GameData.h"

DTI_IMPLEMENT(CGlyphEdit, CCtrlEdit)

CGlyphEdit::CGlyphEdit(void)
{
	mTemp = 0;
	m_iShowPos = 0;
	m_dwColor = 0xFFFFFFFF;
	m_dwBackColor = 0;
	m_bFakeFocus = false;

	m_iFontSize = FONTSIZE_DEFAULT;
	m_line = new CStringLine(m_dwColor,m_dwBackColor,FONT_SONGTI,m_iFontSize,0);

	//m_iHistorys		= 0;
	m_iDisNow		= 0;
	m_dwLastActionTime = 0;
	//m_HistoryLines.clear();
}

CGlyphEdit::~CGlyphEdit(void)
{
	SAFE_DELETE(m_line);
	//ClearHistory();
}

UINT CGlyphEdit::getGlyphByteLength()
{
	return m_line->size();
}
bool CGlyphEdit::OnChar(UCHAR cChar)
{
	if(cChar < 0x20 || cChar == 127 || (m_bDigital && (cChar < 0x30 || cChar > 0x39)))
	{
		// 特殊字符不接收
		return false;
	}

	if(m_iSelBegin != m_iSelEnd)
	{
		OnDelete();
	}

	// 如果大于max,  不接受输入
	if(getGlyphByteLength() >= (UINT)m_iMaxLen)
	{
		mTemp = 0;
		return false;
	}

	m_dwLastActionTime = GetTickCount();
	
	// 处理单双字节
	if(mTemp == 0)
	{
        if(cChar >= 0x80)
		{
			mTemp = cChar;
			return true;
		}
		else
		{
			char * lpPtr = (char *)&cChar;
			CChar * lpChar = new CChar(lpPtr,1);
			
            if(!insertGlyph(m_iCursor,lpChar))
			{
				delete lpChar;
				mTemp = 0;
				return true;
			}
			mTemp = 0;
		}
	}
	else
	{
		char dbyte[2] = { mTemp,cChar };
		CChar * lpChar = new CChar(dbyte,2);
		if(!insertGlyph(m_iCursor,lpChar))
		{
			delete lpChar;
			mTemp = 0;
			return true;
		}
		mTemp = 0;
	}
    return true;
}

// 插入一个glyph, 调整m_iCursor,m_iShowPos
bool CGlyphEdit::insertGlyph(int pose,CGlyph * lpGlyph)
{
	if(getGlyphByteLength() + lpGlyph->size() > m_iMaxLen)
	{
		return false;
	}
	if(m_iSelBegin != m_iSelEnd)
		OnDelete();

    m_line->insert(lpGlyph,pose);
	m_iCursor = pose + 1;

	ReSetShowPos();

	return true;
}

void CGlyphEdit::OnCopy(void)
{
	if(m_iSelBegin == m_iSelEnd)
		return;
	
	int iBegin = min(m_iSelBegin,m_iSelEnd);
	int iEnd = max(m_iSelBegin,m_iSelEnd);

	string tmp;
	for(int i = iBegin ; i < iEnd; i++)
	{
		CGlyph* glyph = m_line->getGlyph(i);
		tmp.append(glyph->getBuf(),glyph->getBufLength());
	}

	CopyText2Clip(tmp.c_str(),tmp.size());
}

void CGlyphEdit::OnEscape(void)
{
	if(m_line->getGlyphSize() > 0)
	{
		Clear();
		m_bSelected = false;
		m_iSelBegin = m_iSelEnd =0;
	}
	else
	{
		//OnEntry();
		DisableFocus();
	}
}

void CGlyphEdit::OnCut(void)
{
	OnCopy();

	if(m_iSelBegin != m_iSelEnd)
	{
        OnDelete();
	}
}

void CGlyphEdit::OnPaste(void)
{
	if(m_iSelBegin != m_iSelEnd)
		OnDelete();

	string str;
	if(!PasteFromClipboard(str))
		return;

	int iLenMLine = strlen(m_line->c_str()) + str.size();

	if(iLenMLine <= MAX_EDIT_WORDS && iLenMLine <= m_iMaxLen)
	{
		insertGlyph(str.c_str(),str.size());
	}
}

void CGlyphEdit::OnEntry(void)
{
	if(m_line->size()==0)
	{
		CCtrlEdit::OnEntry();
		return;
	}

	std::deque<CStringLine*>& VHistorys = g_pGameData->GetHistoryLines();

	if(VHistorys.size() == 0 || !m_line->equal(VHistorys.back())) 
	{//不一样的内容才加入
		//去掉前面的超出部分
		while(VHistorys.size() > MAX_HISTORY)
		{

			CStringLine* tempSL = VHistorys.front();
			SAFE_DELETE(tempSL);
			VHistorys.pop_front();
		}

		CStringLine* pLine = new CStringLine();
		pLine->assign(*m_line);
		VHistorys.push_back(pLine);
		//m_iHistorys = m_HistoryLines.size();
	}
	m_iDisNow = 0;

	if(m_line->getGlyphSize() == 0)
		return;

	CCtrlEdit::OnEntry();
}

void CGlyphEdit::OnLeft(void)
{
	RefreshSelect();

	if(m_iCursor > 0)
        m_iCursor --;
	if(m_iShowPos > m_iCursor)
		m_iShowPos = m_iCursor;

	RefreshSelect();
}

void CGlyphEdit::OnRight(void)
{
	RefreshSelect();

	if(m_iCursor < m_line->getGlyphSize() )
		m_iCursor ++;
	
	
	//if(m_line->getWidth(m_iShowPos,m_iCursor) > m_iWidth)
	//{
	//	m_iShowPos ++;
	//	m_iCursor --;
	//}

	ReSetShowPos();

	RefreshSelect();
}

void CGlyphEdit::OnDelete(void)
{
	if(m_iSelBegin != m_iSelEnd)
	{
		int x = min(m_iSelBegin,m_iSelEnd);
		int y = max(m_iSelBegin,m_iSelEnd);

		if( x < 0 )
			x = 0;
		if( y > m_line->getGlyphSize())
			y = m_line->getGlyphSize();
		m_line->remove(x,y);
		m_iSelBegin = m_iSelEnd = 0;
		m_iCursor = x;
		if(m_iShowPos > x)
			m_iShowPos = x;

		m_bSelected = false;
		
	}
	else
	{
		if(m_line->getGlyphSize() == 0)
			return;
		if( m_iCursor <= m_line->getGlyphSize()- 1)
            m_line->remove(m_iCursor);
	}
	mTemp = 0;
}

void CGlyphEdit::OnBack(void)
{
	if(m_iSelBegin != m_iSelEnd)
	{
		int x = min(m_iSelBegin,m_iSelEnd);
		int y = max(m_iSelBegin,m_iSelEnd);

		if( x < 0 )
			x = 0;
		if( y > m_line->getGlyphSize())
			y = m_line->getGlyphSize();
		m_line->remove(x,y);
		m_iSelBegin = m_iSelEnd = 0;
		m_iCursor = x;
		if(m_iShowPos > x)
			m_iShowPos = x;

		m_bSelected = false;
	}
	else
	{
		if(m_iCursor <= 0)
			return;

		m_line->remove(m_iCursor - 1);
		m_iCursor--;
		if(m_iCursor < 0)
			m_iCursor = 0;
	}
}

void CGlyphEdit::OnHome(void)
{
	RefreshSelect();

	m_iCursor = 0;
	m_iShowPos = 0;

	RefreshSelect();

}

void CGlyphEdit::OnEnd(void)
{
	RefreshSelect();

	m_iCursor = m_line->getGlyphSize();

	int width = 0;
	for(int i = m_iShowPos ; i < m_iCursor; i++)
	{
		width += m_line->getGlyphWidth(i);
	}
	if(width > m_iWidth)
	{
		width = 0;
		int j = int(m_line->getGlyphSize())- 1;

		while( width < m_iWidth)
		{
			width += m_line->getGlyphWidth(j--);
		}
		m_iShowPos = j + 1;
	}

	RefreshSelect();
}

void CGlyphEdit::OnSelectAll(void)
{
	m_iSelBegin = 0;
	m_iSelEnd = m_line->getGlyphSize();
}

bool CGlyphEdit::SetFocus(void)
{
	m_dwLastActionTime = GetTickCount();
	bool b = CControl::SetFocus();
	if(b)
	{
		m_bFakeFocus = true;
	}
	return b;
}

bool CGlyphEdit::OnLeftButtonDown(int iX, int iY)
{
	m_bSelected = true;
	int x = 0;
	int i = 0;

	for(i = m_iShowPos; i < m_line->getGlyphSize(); i++)
	{
		int iGlyphWidth = m_line->getGlyphWidth(i);

		if(x + iGlyphWidth < iX)
		{
			x += iGlyphWidth;
		}
		else
		{
			m_iSelBegin = i;
			break;
		}
	}
	if( i >= m_line->getGlyphSize() )
	{
		m_iSelBegin = m_line->getGlyphSize();
	}
	
	m_iCursor = m_iSelEnd = m_iSelBegin;

	return CControl::OnLeftButtonDown(iX,iY) ;
}

bool CGlyphEdit::OnLeftButtonUp(int iX, int iY)
{
	if(m_iSelBegin != m_iSelEnd)
		m_bSelected = true;
	else 
		m_bSelected = false;

	return CControl::OnLeftButtonUp(iX,iY) ;
}

//鼠标在编辑框内 选择和点击的时候 更新 编辑位置
bool CGlyphEdit::OnMouseMove(int iX, int iY)
{
	if(m_bSelected && m_bClick)
	{
		if(iX < 0)
			iX = 0;
		if(iX > m_iWidth)
			iX = m_iWidth;
		
		int x = 0;
		for(int i = m_iShowPos; i < m_line->getGlyphSize(); i++)
		{
			if( x < iX)
			{
  				x += m_line->getGlyphWidth(i);
			}
			else
			{
				m_iSelEnd = i;
				break;
			}
		}
		if( x < iX)
		{
			m_iSelEnd = m_line->getGlyphSize();
		}
		m_iCursor = m_iSelEnd;
	}
	return true;
}

void CGlyphEdit::ReSetShowPos()
{
	int width = 0;
	for(int i = m_iShowPos ; i < m_iCursor; i++)
	{
		width += m_line->getGlyphWidth(i);
	}

	if(width > m_iWidth)
	{
		width = 0;
		int j = m_iCursor - 1;

		while( width < m_iWidth)
		{
			width += m_line->getGlyphWidth(j);
			if (width < m_iWidth)
			{
				j--;
			}
		}
		m_iShowPos = j + 1;
	}
}

bool CGlyphEdit::OnUp(void)
{
	std::deque<CStringLine*>& VHistorys = g_pGameData->GetHistoryLines();

	if(g_pInput->IsShift())
	{
		if(m_iDisNow < VHistorys.size() && VHistorys.size() > 0)
		{
			int iCur = (VHistorys.size() - 1) - m_iDisNow;

			this->Clear();

			m_line->assign(*VHistorys[iCur]);
			m_iCursor = VHistorys[iCur]->getGlyphSize();
			m_iSelBegin = m_iSelEnd = 0;
			m_iDisNow++;

			if(m_iDisNow >= VHistorys.size())
				m_iDisNow = VHistorys.size() - 1;

			ReSetShowPos();
		}
		return true;
	}

	return false;
}

bool CGlyphEdit::OnDown(void)
{
	std::deque<CStringLine*>& VHistorys = g_pGameData->GetHistoryLines();
	if(g_pInput->IsShift())
	{
		if(m_iDisNow >= 0 && VHistorys.size() > 0)
		{
			int iCur = (VHistorys.size() - 1) - m_iDisNow;

			this->Clear();
			m_line->assign(*VHistorys[iCur]);
			m_iCursor = VHistorys[iCur]->getGlyphSize();
			m_iSelBegin = m_iSelEnd = 0;
			m_iDisNow--;

			if(m_iDisNow < 0)
				m_iDisNow = 0;

			ReSetShowPos();
		}
		return true;
	}
	return false;
}

bool CGlyphEdit::OnLeftButtonDClick(int iX, int iY)
{
    return true;
}


//绘制编辑框内容 包括简单文字 选择框 动态光标位置
void CGlyphEdit::Draw(void)
{
	//焦点判断计算
	//if((m_bFocus || m_bFakeFocus) && GetTickCount() >= m_dwLastActionTime+30000)
	//	DisableFocus();

	//原来会抢焦点并绘制，现在不抢了

	//绘制
 	CControl::Draw();
	int x = m_iScreenX + m_iMarge + m_iFrame;                   
	int y = m_iScreenY + m_iMarge + m_iFrame +(m_iHeight - m_iFontSize)/2;

	if(m_iSelBegin != m_iSelEnd)                              // 绘制选中高亮区
	{
		int iBegin,iEnd;
		if(m_iSelEnd > m_iSelBegin)
		{
			iBegin = m_iSelBegin;
			iEnd   = m_iSelEnd;
		}
		else
		{
			iBegin = m_iSelEnd;
			iEnd   = m_iSelBegin;
		}
		if(iBegin < m_iShowPos)
			iBegin = m_iShowPos;

		int xstart = m_line->getWidth(m_iShowPos,iBegin);
		int xend = m_line->getWidth(m_iShowPos,iEnd);

        int iiend=iEnd;
		while(xend > m_iWidth)
		{
			xend = m_line->getWidth(m_iShowPos,--iiend);
		}
		
		g_pGfx->DrawFillRect(x + xstart,y,xend-xstart,m_iFontSize,0xEF0000FF);
	}

	if(m_line->size() != 0)
	{
		int ix = 0;
		int ii = m_iShowPos;
		for(ii = m_iShowPos;ii < m_line->getGlyphSize();ii++)
		{
			int w = m_line->getGlyphWidth(ii);
			if(ix + w < m_iWidth)
				ix += w;
			else
				break;
		}
		m_line->DrawPart(x,y,m_iShowPos,ii);
	}

	if(m_bFocus || m_bFakeFocus)													// 有焦点就输出光标
	{
		if((GetTickCount() / 500) % 2 != 0)
		{
			for(int i = m_iShowPos; i < m_iCursor; i++)
			{
                x += m_line->getGlyphWidth(i);
			}
  			g_pFont->DrawText(x - 2,y,"|",m_dwCursorColor,m_iFont,m_iFontSize);
		}
	}
 	else if(m_iSelBegin != m_iSelEnd)
	{
		m_iSelBegin = m_iSelEnd = 0;
	}

	//恢复
	//if(pCtrl)
	//{
	//	pCtrl->SetFocus();
	//}
}

//清除编辑框内容
void  CGlyphEdit::Clear()
{
	if(m_line->getGlyphSize() > 0)
	{
		m_line->remove(0,-1);
		m_iSelBegin = m_iSelEnd = m_iCursor = m_iShowPos = 0;
		m_bSelected = false;
	}		
}

//获得 简单文字 或者 链接物体的名字
const char * CGlyphEdit::GetText()
{
	return m_line->c_str();
}


//插入文字编辑框的内容 可以是 链接物体信息 也可以是 简单文字
void CGlyphEdit::insertGlyph(const char * str, UINT len)
{
	int i = 0;

	char* pBuf  = (char*)str;
	char* pNext = pBuf;

	while(i < len)
	{
		char c = *pBuf;
		if(c == 0x01)
		{
			pNext = pBuf + pBuf[1] + 2;
			CObjectLink * link = new CObjectLink(pBuf+2,pBuf[1]);
			if(!insertGlyph(m_iCursor,link))
			{
				delete link;
				return;
			}
		}
		else
		{
			pNext = CharNext(pBuf);
			CChar * ch = new CChar(pBuf,(int)(pNext-pBuf));
			if(!insertGlyph(m_iCursor,ch))
			{
                delete ch;
				return;
			}
		}

		i += (int)(pNext - pBuf);
		pBuf = pNext;
	}
}

//设置编辑框内容 可以是 链接物体信息 也可以是 简单文字
void CGlyphEdit::SetText(const char * sText)
{
	Clear();
	insertGlyph((char *)sText,strlen(sText));	
}

//插入链接物体信息
bool CGlyphEdit::InsertObjectLink(CObjectLink * lpLink)
{
	if(m_line->getBufLength()+lpLink->getBufLength()>500) //超长了
	{
		SAFE_DELETE(lpLink);
		return false;
	}
	return insertGlyph(m_iCursor,lpLink);
}

//获得链接物体信息的指针
CStringLine* CGlyphEdit::getLine()
{
	return	m_line;
}

//更新编辑框选中区域,记录下开始和结束位置
void CGlyphEdit::RefreshSelect()
{
	if(g_pInput->IsShift())
	{
		if(m_bSelected)
		{
			m_iSelEnd  = m_iCursor;
		}
		else                                     // 开始纪录选中区域
		{
			m_bSelected = true;
			m_iSelBegin = m_iCursor;
			m_iSelEnd   = m_iCursor;
		}
	}
	else
	{
		m_bSelected = false;
		m_iSelBegin = m_iSelEnd =0;
	}
}

void CGlyphEdit::DisableFocus()
{
	m_bFakeFocus = false;

	if(g_pControl->SetTopWindowFocus()) //将焦点还给其他窗口
		return;

	//给父窗口
	if(m_pParent)
		m_pParent->SetFocus();
}

//void CGlyphEdit::ClearHistory(void)
//{
//	while(!m_HistoryLines.empty())
//	{
//		CStringLine* pLine = m_HistoryLines.back();
//		SAFE_DELETE(pLine);
//		m_HistoryLines.pop_back();
//	}
//	m_iHistorys		= 0;
//	m_iDisNow		= 0;
//}