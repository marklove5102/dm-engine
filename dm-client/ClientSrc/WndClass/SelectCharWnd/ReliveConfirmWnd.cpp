#include "reliveconfirmwnd.h"
#include "GameControl\GameControl.h"

INIT_WND_POS(CReliveConfirmWnd,POS_AUTO,POS_AUTO)

CReliveConfirmWnd::CReliveConfirmWnd(void)
{
	m_iControlMode = CTRL_MODE_MESSAGEBOX;
	m_iIndex = 12825;
	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CReliveConfirmWnd::~CReliveConfirmWnd(void)
{
}

void CReliveConfirmWnd::OnCreate(void)
{
	m_pOKButton = new CCtrlButton;
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,50,100,4843,4844,4845);

	m_pCancelButton = new CCtrlButton;
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,200,100,4846,4847,4848);
}

bool CReliveConfirmWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl==m_pOKButton )
		{
			OnClickOKButton();
			return true;
		}
		else if(pControl==m_pCancelButton )
		{
			OnClickCancelButton();
			return true;
		}
	}

	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

void CReliveConfirmWnd::OnDraw(void)
{
    TextOut(30,40,"需要立即购买还魂神符并直接复活吗？",0xFFFFFF00,16,DTF_BlackFrame);
}

void CReliveConfirmWnd::OnClickOKButton(void)
{
	g_pControl->Msg(MSG_CTRL_RELIVE_WND,MSG_CTRL_RELIVE_USEYANBAO_OK);
	g_pControl->Msg(MSG_CTRL_RELIVECONFIRM_WND,0);
}

void CReliveConfirmWnd::OnClickCancelButton(void)
{
	g_pControl->Msg(MSG_CTRL_RELIVE_WND,MSG_CTRL_RELIVE_USEYANBAO_CANCEL);
	g_pControl->Msg(MSG_CTRL_RELIVECONFIRM_WND,0);
}
