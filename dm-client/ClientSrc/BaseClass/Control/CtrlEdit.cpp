#include "ctrledit.h"
#include "Global/GlobalMsg.h"
#include "GameData/GameGlobal.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CCtrlEdit, CControl)

// 默认初始化
CCtrlEdit::CCtrlEdit(void)
{
	m_iControlMode	= CTRL_MODE_EDIT;
	m_bPwd      = false;
	m_bSelected = false;
	m_bDigital	= false;
	m_sText.reserve(MAX_EDIT_WORDS);   
	m_iDisplay  = 0;
	m_iSelBegin = 0;
	m_iSelEnd   = 0;
	m_iCursor   = 0;
	m_iMarge    = 0;
	m_iMaxLen   = MAX_EDIT_WORDS - 1;
	m_iFont		= FONT_DEFAULT;
	m_iFontSize	= FONTSIZE_DEFAULT;
	m_dwColor	= 0xFFFFFFFF;
    m_dwMouseColor = 0xFFFFFFFF;
    m_dwClickColor = 0xFFFFFFFF;
    m_dwDisableColor = 0xFFFFFFFF;

	m_dwCursorColor = 0xFFFFFFFF;
	m_sPrompt.clear();

	m_vTexID[0] = 0;
	m_vTexID[1] = 0;
	m_vTexID[2] = 0;
	m_vTexID[3] = 0;
}

CCtrlEdit::~CCtrlEdit(void)
{
	m_sText.clear();
}

// 创建函数
bool CCtrlEdit::Create(CControl * pParent,int iFontSize,DWORD dwColor,int iX,int iY,int iW,int iH,UINT uStyle)
{
	if( !pParent ) 
		return false;

	m_iWidth	= iW;                        // 宽度
	m_iHeight	= iH;                        // 高度
	SetFont(m_iFont,iFontSize);                // 字体大小
	SetColor(dwColor);                       // 字体颜色
	SetFocus();
	uStyle |= CTRL_STYLE_BACKMODE_NODRAW | CTRL_STYLE_SELFDELETE_TEX;

	m_iVisualWords = (iW-2*(m_iMarge+m_iFrame))*2/m_iFontSize;   // 计算可见字的个数(英文字,汉字除2)
	if(m_iVisualWords % 2)                   // 为了显示方便可见字设为偶数个
		m_iVisualWords -= 1;
	return CControl::Create(pParent,iX,iY,m_iWidth,m_iHeight,uStyle,0xFF000000);
}

bool CCtrlEdit::CreateEx(CControl * pParent,int iX,int iY,int iW,int iH,UINT uStyle,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex)
{
	if( !pParent ) 
		return false;

	m_iWidth	= iW;                        // 宽度
	m_iHeight	= iH;                        // 高度
	m_vTexID[0] = wTex;
	m_vTexID[1] = wMTex;
	m_vTexID[2] = wCTex;
	m_vTexID[3] = wDTex;

	SetFocus();
	uStyle |= CTRL_STYLE_BACKMODE_NODRAW | CTRL_STYLE_SELFDELETE_TEX;

	m_iVisualWords = (iW-m_iTextOffX-2*(m_iMarge+m_iFrame) - 2)*2/m_iFontSize;   // 计算可见字的个数(英文字,汉字除2)
	if(m_iVisualWords % 2)                   // 为了显示方便可见字设为偶数个
		m_iVisualWords -= 1;

	return CControl::Create(pParent,iX,iY,m_iWidth,m_iHeight,uStyle,0xFF000000);
}


bool CCtrlEdit::CreateXML(CControl* pParent,CXmlEdit* edit)
{
    if(!edit || !pParent) 
        return false;

    int iW, iH;
    
    edit->GetSize(iW,iH);
	AssignXML(edit);

    if(!Create(pParent,m_iFontSize,edit->GetXmlColor().GetColor(),0,0,iW,iH))
        return false;

    return true;
}

// 按键消息响应函数
bool CCtrlEdit::OnKeyDown(WORD wState, UCHAR cKey)
{
	switch(cKey)
	{
	case VK_UP:
		{
			return OnUp();
		}
	case VK_DOWN:
		{
			return OnDown();
		}
	case VK_LEFT:                                       // 处理"<-"键
		{
			OnLeft();
			return true;
		}
	case VK_RIGHT:                                      // 处理"->"键 
		{
			OnRight();
			return true;
		}
	case VK_RETURN:										// 回车处理                          
		{
			OnEntry();
			return true;
		}
	case VK_ESCAPE:
		{
			OnEscape();
			return true;
		}
	case VK_BACK:										// BACK键处理
		{
			OnBack();
			return true;
		}
	case VK_DELETE:                                     // 处理Del键
		{
			OnDelete();
			return true;
		}   
	case VK_HOME:										// 处理Home键
		{
			OnHome();
			return true;
		}
	case VK_END:                                        // 处理End键
		{  
			OnEnd();
			return true;
		}
	case 'C':
		{
			if(g_pInput->IsCtrl())
				OnCopy();
			return true;
		}
	case 'X':
		{
			if(g_pInput->IsCtrl())
				OnCut();
			return true;
		}
	case 'V':
		{
			if(g_pInput->IsCtrl())
				OnPaste();
			return true;
		}
	case 'A':
		{
			if(g_pInput->IsCtrl())
				OnSelectAll();
			return true;
		}
	}
	return false;
}
// 绘制函数
void CCtrlEdit::Draw(void)
{
	if(!m_bShow)
		return;

	CControl::Draw();

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
	if(!m_bEnable && m_vTexID[3] != 0)
		iState = 3;
	else if(m_bClick )
		iState = 2;
	else if(m_bMouseOn)
		iState = 1;
	else
		iState = 0;


	if(m_vTexID[iState] == 0)
		iState = 0;

	if (m_vTexID[iState] != 0)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_vTexID[iState],EP_UI);
		if (pTex)
		{
			POS scale((float)(m_iWidth) / pTex->GetWidth(),(float)(m_iHeight) / pTex->GetHeight());
			g_pGfx->DrawTextureFX(m_iScreenX,m_iScreenY,pTex,-1,NULL,&scale);
		}
	}
	

	//绘制文字
	if(m_iCursor > m_iVisualWords)
	{
		m_iCursor = m_iVisualWords;
		if(!CanStayCursor())
			m_iCursor--;
	}

	int x,y;											// 文字输出位置
	x = m_iScreenX + m_iMarge + m_iFrame + m_iTextOffX;                   
	y = m_iScreenY + m_iMarge + m_iFrame +(m_iHeight - m_iFontSize)/2 + m_iTextOffY;

	int iLen;											// 显示字符的个数(汉字占2格)


	if((m_sText.size() - m_iDisplay) > m_iVisualWords)    
	{
		iLen = m_iVisualWords;
		if(!CanStay(m_iDisplay + iLen))
			iLen--;
	}
	else
	{
		iLen = (m_sText.size() - m_iDisplay);
	}

	string display;

	if(!m_bFocus && m_sText.empty() && !m_sPrompt.empty())
	{
		g_pFont->DrawText(x,y,m_sPrompt.c_str(),0xFFFFFFFF,m_iFont,m_iFontSize,0,0xFF0000FF);
	}

	if(!m_bPwd)                                  // 一般编辑框的绘制
	{
		display.assign(m_sText.c_str()+m_iDisplay,iLen);
	}
	else                                         // 密码框全部输出*
	{
		display.assign(iLen,'*');
	}

	if(m_iSelBegin != m_iSelEnd)                              // 绘制选中高亮区
	{
		int iBegin,iEnd;
		if(m_iSelEnd > m_iSelBegin)
		{
			iBegin = (int)(m_iSelBegin - m_iDisplay);
			iEnd   = (int)(m_iSelEnd - m_iDisplay);
		}
		else
		{
			iBegin = (int)(m_iSelEnd - m_iDisplay);
			iEnd   = (int)(m_iSelBegin - m_iDisplay);
		}
		if(iBegin < 0)
			iBegin = 0;
		if(iBegin > m_iVisualWords)
			iBegin = m_iVisualWords;
		if(iEnd < 0)
			iEnd = 0;
		if(iEnd > m_iVisualWords)
			iEnd = m_iVisualWords;
		g_pGfx->DrawFillRect(x + iBegin * m_iFontSize/2,y,(iEnd - iBegin)*m_iFontSize/2,m_iFontSize,0xEF0000FF);
	}

	g_pFont->DrawText(x,y,display.c_str(),m_dwColor,m_iFont,m_iFontSize);       // 输出文字

	if(m_bFocus)													// 有焦点就输出光标
	{
		if((GetTickCount() / 500) % 2 > 0)
			g_pFont->DrawText((int)(x+(m_iCursor-0.5)*m_iFontSize/2),y,"|",m_dwCursorColor,m_iFont,m_iFontSize);
	}
	else if(m_iSelBegin != m_iSelEnd)
	{
		m_iSelBegin = m_iSelEnd = 0;
	}
}

// 输入字符响应函数
bool CCtrlEdit::OnChar(UCHAR cChar)
{
	if(cChar < 0x20 || cChar == 127 || (m_bDigital && (cChar < 0x30 || cChar > 0x39)))			// 特殊字符不接收
		return false;

	if(m_iSelBegin != m_iSelEnd)
	{
		OnDelete();
	}
	// 接收其他字符
	if(m_sText.size() >= m_iMaxLen )	// 大于最大设定值就不接收输入
		return false;

	if(m_bPwd)
	{
		if(cChar == ' ')
			return false;
	}

	InsertChar(m_iDisplay+m_iCursor,cChar);
	m_iCursor++;
	if(m_iCursor > m_iVisualWords)				// 自动伸近
	{
		if(m_sText[m_iDisplay] >= 0x80)
		{
			m_iDisplay++;
			m_iCursor = m_iVisualWords - 1;
		}
		else
			m_iCursor = m_iVisualWords;
		m_iDisplay++;
	}

	return true;
}

// 设置最长显示字符个数(密码框或者登陆框会用到,一般此值需要小于m_iVisualWords)
void CCtrlEdit::SetMaxLength(int iLength)              
{
	if(iLength <= 0 || iLength > MAX_EDIT_WORDS)
		m_iMaxLen = MAX_EDIT_WORDS;
	else
		m_iMaxLen = iLength;
}

// 删除sPos开始的num字符
void CCtrlEdit::DelChar(int iPos, int num)
{
	if(iPos < 0 || iPos >= m_sText.size() || num < 0)
		return;

	m_sText.erase(iPos,num);

	if(m_pParent)
		m_pParent->Msg(MSG_CTRL_EDIT_ONCHAR,0,this);
}

// 在sPos位置加入一个字符
void CCtrlEdit::InsertChar(int iPos, char cChar)  
{
	if(iPos >= m_sText.size())
		m_sText.push_back(cChar);
	else
		m_sText.insert(iPos,1,cChar);

	if(m_pParent)
		m_pParent->Msg(MSG_CTRL_EDIT_ONCHAR,0,this);
}

// 把m_sText  [iBegin->iEnd]放入剪贴板
bool CCtrlEdit::CopyToClipboard(int iBegin, int iEnd)
{
	if(iBegin == iEnd)                                          
		return false;

	// 调整选中区域
	if(iBegin > iEnd) 
	{
		int iTemp	= iBegin;
		iBegin		= iEnd;
		iEnd		= iTemp;
	}

	// 纠正非法区域字符
	if(iBegin >= (int)m_sText.size())
		return false;

	if(iEnd > (int)m_sText.size())
		iEnd = (int)m_sText.size();

	if(CopyText2Clip(m_sText.c_str() + iBegin,iEnd - iBegin))
		return true;
	return false;
}

// Ctrl + C 响应函数
void CCtrlEdit::OnCopy(void)
{
	if(!m_bPwd)
		CopyToClipboard(m_iSelBegin,m_iSelEnd);
}
// Ctrl + X 响应函数
void CCtrlEdit::OnCut(void)
{
	if(!m_bPwd)
	{
		CopyToClipboard(m_iSelBegin,m_iSelEnd);
		OnDelete();
	}
}

// 回车键响应函数
void CCtrlEdit::OnEntry(void)
{
	m_iSelBegin = m_iSelEnd = 0;
	CControl * p = GetParent();
	if(p != NULL)
		p->Msg(MSG_CTRL_EDIT_ENTRY,0,this);
}


// Ctrl + V 响应函数
void CCtrlEdit::OnPaste(void)
{
	if(m_iSelBegin != m_iSelEnd)
		OnDelete();

	if(!OpenClipboard(NULL))
		return;
	HGLOBAL hClip=NULL;

	if(!(hClip=GetClipboardData(CF_TEXT)))
	{
		return;
	}
	char *srcClip = (char*)GlobalLock(hClip);
	InsertStr(m_iDisplay + m_iCursor,srcClip);

	GlobalUnlock(hClip);
	CloseClipboard();
}

// '<--'键响应函数
void CCtrlEdit::OnLeft(void)       
{
	int iCursor = m_iCursor;                      // 保存移动前光标的位置

	RefreshSelect();

	LPCSTR lpCur = m_sText.c_str()+m_iDisplay + m_iCursor;
	LPCSTR lpPrev = CharPrev(m_sText.c_str(),lpCur);
	int sep = (int)(lpPrev - lpCur);

	if( sep >= 0 || sep < -2 )
		return;

	m_iCursor += sep;

	if(m_iCursor < 0)
	{
		if(m_iDisplay > 0)
		{
			m_iDisplay--;
			if(sep == -2)
				m_iDisplay--;

			if(m_iDisplay < 0)
				m_iDisplay = 0;
		}
		else
			m_iDisplay = 0  ;
		m_iCursor = 0;
	}

	RefreshSelect();
	if(m_iSelBegin != m_iSelEnd)
		m_bSelected = true;
	else 
		m_bSelected = false;
}

// '-->'键响应函数
void CCtrlEdit::OnRight(void)
{
	int iCursor = m_iCursor;                        // 保存移动前光标的位置

	RefreshSelect();

	LPCSTR lpCur = m_sText.c_str()+m_iDisplay + m_iCursor;
	LPCSTR lpNext = CharNext(lpCur);
	int sep = (int)(lpNext - lpCur);

	if(sep > 2 || sep < 0)
		return;

	m_iCursor += sep;

	int m = m_sText.size() - m_iDisplay;
	if(m <= m_iVisualWords)						// 不需要移动m_sDisplay
	{
		if(m_iCursor > m)
			m_iCursor = m;
	}
	else if(m_iCursor > m_iVisualWords)			// 需要移动m_sDisplay
	{
		m_iDisplay += (m_iCursor - m_iVisualWords);
		m_iCursor = m_iVisualWords;
		if(!CanStayDisplay())
		{
			m_iDisplay++;
			m_iCursor--;
		}
		if(!CanStayCursor())
			m_iCursor--;
	}

	RefreshSelect();
	if(m_iSelBegin != m_iSelEnd)
		m_bSelected = true;
	else 
		m_bSelected = false;

}

// Delete键响应函数
void CCtrlEdit::OnDelete(void)
{
	if((m_iDisplay + m_iCursor) > m_sText.size())
		return ;

	if(m_iSelBegin != m_iSelEnd)                            // 有选中区域的话
	{
		if(m_iSelBegin > m_iSelEnd)
		{
			DelChar(m_iSelEnd,m_iSelBegin - m_iSelEnd);
			if(m_iDisplay > m_iSelEnd )
			{
				m_iDisplay = m_iSelEnd;
				m_iCursor  = 0;
			}
			else
			{
				m_iCursor = m_iSelEnd - m_iDisplay;
			}
		}
		else
		{
			DelChar(m_iSelBegin,m_iSelEnd - m_iSelBegin);
			if(m_iDisplay > m_iSelBegin )
			{
				m_iDisplay = m_iSelBegin;
				m_iCursor  = 0;
			}
			else
			{
				m_iCursor = m_iSelBegin - m_iDisplay;
			}
		}
		m_iSelBegin = m_iSelEnd = m_iDisplay + m_iCursor;
		m_bSelected = false;
	}
	else                                                    // 没有选中区域
	{
		if((BYTE)(m_sText[m_iDisplay + m_iCursor]) >= 0x80)
		{
			DelChar(m_iDisplay + m_iCursor ,2);
		}
		else
		{
			DelChar(m_iDisplay + m_iCursor ,1);
		}
	}
}

// Back键响应函数
void CCtrlEdit::OnBack(void)
{
	if((m_iDisplay + m_iCursor) < 0)
		return;

	if(m_iSelBegin != m_iSelEnd)                            // 有选中区域的话
	{
		if(m_iSelBegin > m_iSelEnd)
		{
			DelChar(m_iSelEnd ,m_iSelBegin - m_iSelEnd);
			if(m_iDisplay > m_iSelEnd )
			{
				m_iDisplay = m_iSelEnd;
				m_iCursor  = 0;
			}
			else
			{
				m_iCursor = m_iSelEnd - m_iDisplay;
			}
		}
		else
		{
			DelChar(m_iSelBegin,m_iSelEnd - m_iSelBegin);
			if(m_iDisplay > m_iSelBegin)
			{
				m_iDisplay = m_iSelBegin;
				m_iCursor  = 0;
			}
			else
			{
				m_iCursor = m_iSelBegin - m_iDisplay;
			}
		}
		m_iSelBegin = m_iSelEnd = m_iDisplay + m_iCursor;
		m_bSelected = false;
	}
	else                                                    // 没有选中区域
	{
		LPCTSTR lpCur = m_sText.c_str() + m_iDisplay + m_iCursor;
		LPCTSTR lpPrev = CharPrev(m_sText.c_str(),lpCur);
		long sep = lpPrev - lpCur;
		if( sep >= 0 || sep < -2)
			return;

		if(m_iCursor > 0)                                            // 删除可见区内的字符                                     
		{
			if(sep == -2)
			{
				if(m_iDisplay + m_iCursor - 2 < 0)
					return;
				DelChar(m_iDisplay + m_iCursor -2,2);
				m_iCursor -= 2;
			}
			else                                                     // 删除可见区内的字母(退后1格)
			{
				DelChar(m_iDisplay + m_iCursor -1,1);
				m_iCursor -= 1;
			}
		}
		else                                                          // 删除的是不可见区的字符                               
		{
			if(sep == -2)
			{
				if(m_iDisplay -2 < 0)
					return;
				DelChar(m_iDisplay -2,2);
				m_iDisplay -=2;
			}
			else                                                       // 删除不可见区内的字母(退后1格)
			{
				DelChar(m_iDisplay -1,1);
				m_iDisplay--;
			}
		}
		if(m_iCursor < 0)
			m_iCursor = 0;
	}
}

// 在sDest位置插入字符串sSrc
void CCtrlEdit::InsertStr(int iPos, char * sSrc)
{
	int n = (int)(strlen(sSrc));
	if(n == 0)
		return;
	if((int)(n + m_sText.size()) > m_iMaxLen)
		return;

	m_sText.insert(iPos,sSrc);
	m_iCursor += n;

	int ii = m_iCursor - m_iVisualWords;

	if(m_iCursor > m_iVisualWords) 
	{
		m_iDisplay	+= ii;
		m_iCursor	-= ii;
		if(!CanStayDisplay())
		{
			m_iDisplay++;
			m_iCursor--;
		}

		if(!CanStayCursor())
			m_iCursor--;
	}
	else if(!CanStayCursor())
		m_iCursor--;

	if(m_pParent)
		m_pParent->Msg(MSG_CTRL_EDIT_ONCHAR,0,this);

}

// HOME键响应函数
void CCtrlEdit::OnHome(void)
{
	RefreshSelect();
	m_iDisplay  = 0;
	m_iCursor   = 0;
	RefreshSelect();
}

// END键响应函数
void CCtrlEdit::OnEnd(void)
{
	RefreshSelect();

	m_iDisplay = m_sText.size() - m_iVisualWords;
	if(m_iDisplay < 0)
	{
		m_iDisplay = 0;
		m_iCursor =(int)(m_sText.size() - m_iDisplay);
		if(m_iCursor > m_iVisualWords)
			m_iCursor = m_iVisualWords;

		if(!CanStayCursor())
			m_iCursor--;
	}
	else                                                              // 字符数多余可见字符
	{
		m_iCursor = m_iVisualWords;

		if(!CanStayDisplay())
			m_iDisplay++;

		if(!CanStayCursor())
			m_iCursor--;
	}
	RefreshSelect();
}

void CCtrlEdit::OnSelectAll(void)
{
	m_iSelBegin = 0;
	m_iSelEnd   = (int)m_sText.size(  );
}

void CCtrlEdit::Clear()
{
	m_sText.clear();
	m_iCursor = 0;
	m_iDisplay = 0;

	if(m_pParent)
		m_pParent->Msg(MSG_CTRL_EDIT_ONCHAR,0,this);
}



void CCtrlEdit::SetText(const char * sText,DWORD dwColor,DWORD dwMouseColor,DWORD dwClickColor,DWORD dwDisableColor,int iFontSize,DWORD dwFlag,int iFont,int iOffX,int iOffY,int iGap)
{
	if(sText == NULL || strlen(sText) > MAX_EDIT_WORDS)
		return;


	m_dwColor = dwColor;
	m_dwMouseColor = dwMouseColor;
	m_dwClickColor = dwClickColor;
	m_dwDisableColor = dwDisableColor;

	SetFont(iFont,iFontSize,dwFlag);

	m_iTextOffX = iOffX;
	m_iTextOffY = iOffY;


	m_sText.assign(sText);
	m_iDisplay  = 0;
	m_bSelected = false;
	m_iSelBegin = 0;
	m_iSelEnd   = 0;
	m_iCursor   = m_sText.size();

	m_iVisualWords = (m_iWidth-m_iTextOffX-2*(m_iMarge+m_iFrame) - 2)*2/m_iFontSize;   // 计算可见字的个数(英文字,汉字除2)
	if(m_iVisualWords % 2)                   // 为了显示方便可见字设为偶数个
		m_iVisualWords -= 1;

	if(m_iCursor > m_iVisualWords)
	{
		m_iCursor = m_iVisualWords;
		if(CanStay(m_iCursor) == false)
			m_iCursor--;
	}
}

void CCtrlEdit::SetVisualWords(int iWords)
{
	m_iVisualWords = iWords;
}

bool CCtrlEdit::SetFocus(void)
{
	m_iCursor = (int)(m_sText.size());
	return CControl::SetFocus();
}

void CCtrlEdit::SetCursorColor(DWORD dwColor)
{
	m_dwCursorColor = dwColor;
}

bool CCtrlEdit::OnLeftButtonDown(int iX, int iY)
{
	m_bSelected = true;
	m_iSelBegin = (int)(m_iDisplay +(iX * 2 / m_iFontSize));
	if(m_iSelBegin > m_sText.size())
		m_iSelBegin = m_sText.size();
	if(!CanStay(m_iSelBegin))
		m_iSelBegin--;
	m_iSelEnd = m_iSelBegin;
	m_iCursor = m_iSelEnd - m_iDisplay;

	return CControl::OnLeftButtonDown(iX,iY) ;
}

bool CCtrlEdit::OnLeftButtonUp(int iX, int iY)
{
	if(m_iSelBegin != m_iSelEnd)
		m_bSelected = true;
	else 
		m_bSelected = false;
	return CControl::OnLeftButtonUp(iX,iY) ;
}

bool CCtrlEdit::OnMouseMove(int iX, int iY)
{
	if(m_bSelected && m_bClick)
	{
		if(iX < 0)
			iX = 0;
		if(iX > m_iWidth)
			iX = m_iWidth;
		m_iSelEnd = m_iDisplay + (iX * 2 / m_iFontSize);
		if(m_iSelEnd > m_sText.size())
			m_iSelEnd = m_sText.size();
		if(!CanStay(m_iSelEnd))
			m_iSelEnd--;
		m_iCursor = m_iSelEnd - m_iDisplay;
	}
	return true;
}

void CCtrlEdit::RefreshSelect()
{
	if(g_pInput->IsShift())
	{
		if(m_bSelected)                           // 更新选中区域
		{
			m_iSelEnd  = m_iDisplay+m_iCursor;
		}
		else                                     // 开始纪录选中区域
		{
			m_bSelected = true;
			m_iSelBegin = m_iDisplay+m_iCursor;
			m_iSelEnd   = m_iDisplay+m_iCursor;
		}
	}
	else
	{
		m_bSelected = false;
		m_iSelBegin = m_iSelEnd =0;
	}
}
bool CCtrlEdit::OnUp(void)
{
	return false;
}

bool CCtrlEdit::OnDown(void)
{
	return false;
}

bool CCtrlEdit::CanStay(int iPos)
{
	int i = 0;
	LPCSTR lpCur = (char*)m_sText.c_str();
	while( i < iPos )
	{
		LPCSTR lpNext = CharNext(lpCur);
		int sep = lpNext - lpCur;
		if(sep > 2 || sep <= 0)
			break;
		i += sep;
		lpCur = lpNext;
	}
	if( i == iPos )
		return true;
	else
		return false;    	
}

bool CCtrlEdit::CanStayCursor()
{
	return CanStay(m_iDisplay + m_iCursor);
}

bool CCtrlEdit::CanStayDisplay()
{
	return CanStay(m_iDisplay);
}

bool CCtrlEdit::OnLeftButtonDClick(int iX, int iY)
{
	OnSelectAll();
	return CControl::OnLeftButtonDClick(iX,iY);
}

bool CCtrlEdit::IsFocus(void)
{
	return m_bFocus;
}

void CCtrlEdit::OnEscape()
{
	Clear();

	m_bSelected = false;
	m_iSelBegin = m_iSelEnd =0;

	CControl * p = GetParent();
	if(p != NULL)
		p->Msg(MSG_CTRL_EDIT_ESCAPE,0,this);
}

void CCtrlEdit::SetDigital(bool bDigital)
{
	m_bDigital = bDigital;
}

void CCtrlEdit::InsertText(char* sText)
{
	if(m_iSelBegin != m_iSelEnd)
		OnDelete();

	InsertStr(m_iDisplay + m_iCursor,sText);
}

void CCtrlEdit::OnSetFocus()
{
	if(m_pParent)
		m_pParent->Msg(MSG_CTRL_ON_SET_FOCUS,0,this);
}

void CCtrlEdit::SetWidth(int w)
{ 
	m_iWidth = w; 

	m_iVisualWords = (m_iWidth-2*(m_iMarge+m_iFrame))*2/m_iFontSize;   // 计算可见字的个数(英文字,汉字除2)
	if(m_iVisualWords % 2)                   // 为了显示方便可见字设为偶数个
		m_iVisualWords -= 1;
}

void CCtrlEdit::SetHeight(int h)
{ 
	m_iHeight = h; 
}

