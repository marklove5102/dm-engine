#include "autosaywnd.h"
#include "GameControl/GameControl.h"


INIT_WND_POSX(CAutoSayWnd,POS_SETPOS_AUTO,POS_SETPOS_AUTO)

CAutoSayWnd::CAutoSayWnd(void)
{
	m_pOKButton = NULL;
	m_pStopButton = NULL;

	m_iIndex = 4861;
	m_iPages = 1;
	m_uStyle = CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX;
}

CAutoSayWnd::~CAutoSayWnd(void)
{
}

void CAutoSayWnd::OnCreate()
{
	//确定按钮
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,290,115,4843,4844,4845);

	//停止按钮
	m_pStopButton = new CCtrlButton();
	AddControl(m_pStopButton);
	m_pStopButton->CreateEx(this,227,115,4862,4863,4864,4865);

	m_pEdit = new CCtrlMultiEdit(100,48);
	AddControl(m_pEdit);
	m_pEdit->Create(this,14,31,47,313,56);
	m_pEdit->SetDisplayRows(4);
	m_pEdit->SetFocus();

	SetCloseButton(345,11);
}

void CAutoSayWnd::Draw(void)
{
	CCtrlWindowX::Draw();
}

void CAutoSayWnd::OnClickCloseButton()
{
	g_pControl->PopupWindow(OPER_CLOSE,"AutoSayWnd");
}

bool CAutoSayWnd::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pStopButton)
		{
			g_pControl->PopupWindow(OPER_CLOSE,"AutoSayWnd");
			return true;
		}
		else if(pControl == m_pOKButton)
		{
			g_pControl->PopupWindow(OPER_CLOSE,"AutoSayWnd");
			return true;
		}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
