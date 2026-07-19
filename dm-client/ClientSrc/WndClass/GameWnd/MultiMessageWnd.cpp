#include "multimessagewnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"

#define TIP_MSG_LEN		20

INIT_WND_POSX(CMultiMessageWnd,POS_VARIABLE,POS_VARIABLE)

CMultiMessageWnd::CMultiMessageWnd()
{
	m_iMultiEditSize = sm_dwWindowType;
	m_dwMsg		= 0;//消息
	m_dwData	= 0;//

	m_iIndex = 721;	
	m_iPages = 1;
	m_uStyle = CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX;
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 200;
	m_iOffY = 100;
}

CMultiMessageWnd::~CMultiMessageWnd(void)
{
}

void CMultiMessageWnd::SetMultiEditText(char * sText)
{
	m_pEdit->SetText(sText);
}

bool CMultiMessageWnd::Msg(DWORD dwMsg, DWORD dwData, CControl * pControl)
{
	if( m_pOKButton && dwMsg == MSG_CTRL_BUTTON_CLICK && pControl==(CControl *)m_pOKButton )
	{
		OnClickOKButton();
		return true;
	}
	else if( m_pCancelButton && dwMsg == MSG_CTRL_BUTTON_CLICK && pControl==(CControl *)m_pCancelButton )
	{
		OnClickCancelButton();
		return true;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CMultiMessageWnd::OnClickCancelButton(void)
{
	CloseWindow();
}

void CMultiMessageWnd::OnClickOKButton(void)
{
	switch(m_dwMsg)
	{
	case MSG_CTRL_WISH:
		{
			DWORD dwNpcID = NPC.GetID();
			if(dwNpcID && strlen(m_pEdit->GetText()) > 0)
			{
				g_pGameControl->SEND_Message_Submit(dwNpcID,1,0,(char *)m_pEdit->GetText());
			}
			CloseWindow();
			return;
		}
	default:
		{
			CloseWindow();
			return;
		}
	}
}

void CMultiMessageWnd::OnCreate(void)
{
	SetCloseButton(289,14);

	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,87,209,0,6,7);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->Create(this,189,209,0,8,9);

	m_pEdit = new CCtrlMultiEdit(m_iMultiEditSize,44);
	AddControl(m_pEdit);
	m_pEdit->Create(this,FONT_SMALL,22,68,278,130);
	m_pEdit->SetDisplayRows(10);
	m_pEdit->SetFocus();
	if(m_dwMsg == MSG_CTRL_WISH)
		m_pEdit->SetText("请在此写下100字以内的祝福：");
}

void CMultiMessageWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	if(!m_sText.empty())
	{
		int j = 0;
		const char *p0,*p1 = m_sText.c_str();

		p0 = p1;
		while(*p1)
		{
			if( *p1 == '\n')
			{
				if(p1 > p0)
				{
					char sTemp[1024] = {0};
					memcpy(sTemp,p0,p1-p0);
					g_Font.DrawText(m_iScreenX + 23,m_iScreenY + 23 + 16 * j,sTemp,0xFFFFFFFF,FONT_SMALL);
				}
				j++;
				p0 = p1 + 1;
			}
			p1++;
		}
		if(p1 > p0)
		{
			char sTemp[1024] = {0};
			memcpy(sTemp,p0,p1-p0);
			g_Font.DrawText(m_iScreenX + 23,m_iScreenY + 23 + 16 * j,sTemp,0xFFFFFFFF,FONT_SMALL);
		}
	}
}
