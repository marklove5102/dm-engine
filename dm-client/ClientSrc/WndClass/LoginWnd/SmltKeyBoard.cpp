#include "smltkeyboard.h"
#include "GameControl/GameControl.h"
#include "Global/GlobalMsg.h"

INIT_WND_POSX(CSmltKeyBoard,POS_VARIABLE,POS_VARIABLE)

#define COLOR_BTN_NORMAL_SKB	0xFFB47D3D
#define COLOR_BTN_MOUSEON_SKB	0xFFFF6600
#define COLOR_BTN_PRESS_SKB		0xFF5AA0A0
#define COLOR_BTN_DISABLE_SKB	0xFF646464

CSmltKeyBoard::CSmltKeyBoard(void)
{
	m_bModel = false;
	m_bNoMove = false;
	m_bCapsLock = false;
	m_bShift = false;
	m_cCurChar[0] = m_cCurChar[1] = 0;

	m_pEnter = NULL;
	m_pBack = NULL;
	m_pCapsLock = NULL;
	m_pShift = NULL;

	m_pUnderLine = NULL;
	m_pDot = NULL;

	for(int i = 0;i < 10;i++)
	{
		m_pNumber[i] = NULL;
	}

	for(int i = 0;i < 26;i++)
	{
		m_pLetter[i] = NULL;
	}

	m_iIndex = 1655;
	m_iPages = 1;
	m_iAlignType = XAL_TOPLEFT;
	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CSmltKeyBoard::~CSmltKeyBoard(void)
{
}

void CSmltKeyBoard::OnDraw()
{
	g_pFont->DrawText(m_iScreenX + 43,m_iScreenY + 131,"建议采用键盘和软键盘两种方式混合输入密码",0xffff6600,FONT_YAHEI);
}

void CSmltKeyBoard::OnCreate()
{	
	m_pEnter = new CCtrlButton;
	AddControl(m_pEnter);
	m_pEnter->CreateEx(this,271,72,1640,1641,1642);
	m_pEnter->SetMask(1640);

	m_pBack = new CCtrlButton;
	AddControl(m_pBack);
	m_pBack->CreateEx(this,289,22,1643,1644,1645);
	
	m_pCapsLock = new CCtrlButton;
	AddControl(m_pCapsLock);
	m_pCapsLock->CreateEx(this,14,46,1646,1647,1648);
	m_pCapsLock->SetMask(1646);

	m_pShift = new CCtrlButton;
	AddControl(m_pShift);
	m_pShift->CreateEx(this,14,97,1649,1650,1651);

	char c[2] = {0};
	for(int ii = 0;ii < 10;ii++)
	{
		m_pNumber[ii] = new CCtrlButton;
		AddControl(m_pNumber[ii]);
		c[0] =(char) ('1' + ii);
		if (ii == 9) c[0] = '0';
		m_pNumber[ii]->SetText(c,COLOR_BTN_NORMAL_SKB,COLOR_BTN_MOUSEON_SKB,COLOR_BTN_PRESS_SKB,COLOR_BTN_DISABLE_SKB,12,DTF_BlackFrame);
		m_pNumber[ii]->SetFont(FONT_YAHEI);
		m_pNumber[ii]->CreateEx(this,14+ii*25,22,1652,1653,1654);
	}

	//下划线
	m_pUnderLine = new CCtrlButton;
	c[0] = '_';
	AddControl(m_pUnderLine);	
	m_pUnderLine->CreateEx(this,14+10*25,22,1652,1653,1654);
	m_pUnderLine->SetText(c,COLOR_BTN_NORMAL_SKB,COLOR_BTN_MOUSEON_SKB,COLOR_BTN_PRESS_SKB,COLOR_BTN_DISABLE_SKB,12,DTF_BlackFrame);

	char* szKeyboard[3] = 
	{
		"QWERTYUIOP",
		"ASDFGHJKL",
		"ZXCVBNM"
	};

	for(int j = 0;j < 3;j++)
	{
		int iLen = strlen(szKeyboard[j]);
		for(int i = 0;i < iLen; i++)
		{
			int pos = szKeyboard[j][i] - 'A'; //字符

			int x = 39+j*17+i*25; //调整一下
			if(j == 2) x -= 1;
			c[0] = szKeyboard[j][i] + 'a' - 'A';
			m_pLetter[pos] = new CCtrlButton();
			AddControl(m_pLetter[pos]);
			m_pLetter[pos]->CreateEx(this,x,47+j*25,1652,1653,1654);
			m_pLetter[pos]->SetText(c,COLOR_BTN_NORMAL_SKB,COLOR_BTN_MOUSEON_SKB,COLOR_BTN_PRESS_SKB,COLOR_BTN_DISABLE_SKB,12,DTF_BlackFrame);
		}
	}

	//点
	m_pDot = new CCtrlButton;
	AddControl(m_pDot);
	m_pDot->CreateEx(this,246,97,1652,1653,1654);
	c[0] = '.';
	m_pDot->SetText(c,COLOR_BTN_NORMAL_SKB,COLOR_BTN_MOUSEON_SKB,COLOR_BTN_PRESS_SKB,COLOR_BTN_DISABLE_SKB,12,DTF_BlackFrame);
}


bool CSmltKeyBoard::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{		
	case MSG_CTRL_BUTTON_CLICK:		
		if(pControl == m_pEnter)
		{
			g_pControl->Msg(MSG_CTRL_SOFT_KEYBOARD_INPUT,2,this);			
		}
		else if(pControl == m_pBack)
		{
			g_pControl->Msg(MSG_CTRL_SOFT_KEYBOARD_INPUT,1,this);							
		}
		else if(pControl == m_pShift)
		{
			if(!m_bShift)
			{
				m_bShift = true;
				ChangeCaps();
			}
		}
		else if(pControl == m_pCapsLock)
		{				
			m_bCapsLock = !m_bCapsLock;
			ChangeCaps();
		}
		else if(pControl == m_pUnderLine)
		{
			m_cCurChar[0] = '_';
			g_pControl->Msg(MSG_CTRL_SOFT_KEYBOARD_INPUT,0,(CControl*)m_cCurChar);
		}
		else if(pControl == m_pDot)
		{
			m_cCurChar[0] = '.';
			g_pControl->Msg(MSG_CTRL_SOFT_KEYBOARD_INPUT,0,(CControl*)m_cCurChar);
		}
	    else
		{
			for(int i = 0;i < 10;i++) //判断是否数字
			{
				if(pControl == m_pNumber[i])
				{
					m_cCurChar[0] = '0' + (i+1) % 10;
					g_pControl->Msg(MSG_CTRL_SOFT_KEYBOARD_INPUT,0,(CControl*)m_cCurChar);
					return true;
				}
			}

			for(int i = 0;i < 26;i++)
			{
				if(pControl == m_pLetter[i]) //判断是否字母
				{
					if(m_bCapsLock ^ m_bShift)
						m_cCurChar[0] = 'A' + i; //大写
					else
						m_cCurChar[0] = 'a' + i; //小写

					g_pControl->Msg(MSG_CTRL_SOFT_KEYBOARD_INPUT,0,(CControl*)m_cCurChar);

					if(m_bShift)
					{
						m_bShift = false;
						ChangeCaps();
					}

					return true;
				}
			}
		}
		return true;
    default:
		break;
	}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CSmltKeyBoard::ChangeCaps()
{
	char c[2] = {0};
	BOOL bCS = m_bCapsLock ^ m_bShift;
	for(int ii = 0;ii < 26;ii++)
	{
		if(!m_pLetter[ii]) continue;
		
		if(bCS) c[0] = char('A' + ii);
		else c[0] = char('a' + ii);

		m_pLetter[ii]->SetText(c,COLOR_BTN_NORMAL_SKB,COLOR_BTN_MOUSEON_SKB,COLOR_BTN_PRESS_SKB,COLOR_BTN_DISABLE_SKB,12,DTF_BlackFrame);		
	}
}
