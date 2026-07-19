#include "ReservePasswordWnd.h"
#include "Global/GlobalMsg.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CReservePasswordWnd,POS_AUTO,POS_AUTO)
CReservePasswordWnd::CReservePasswordWnd(void)
{
	m_iType = sm_dwWindowType;
	m_uStyle &= ~CTRL_STYLE_TRANS;

	if (m_iType == 0)
		m_iIndex = 761;
	else
		m_iIndex = 15004;

}

CReservePasswordWnd::~CReservePasswordWnd(void)
{
}


void CReservePasswordWnd::OnCreate(void)
{
	if(m_iType == 0)
	{
		SetCloseButton(283,14);

		m_pReservePassword = new CCtrlEdit;
		AddControl(m_pReservePassword);
		m_pReservePassword->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,117,120,141,18,CTRL_STYLE_BACKMODE_NODRAW|CTRL_STYLE_SELFDELETE_TEX);
		m_pReservePassword->SetMaxLength(6);
		m_pReservePassword->SetDigital(true);

		m_pOK = new CCtrlButton;
		AddControl(m_pOK);
		m_pOK->CreateEx(this,74,153,95,96,97);
		m_pOK->SetText("确定");

		m_pCancel = new CCtrlButton;
		AddControl(m_pCancel);
		m_pCancel->CreateEx(this,191,154,95,96,97);
		m_pCancel->SetText("取消");
	}
	else if(m_iType == 1)
	{
		SetCloseButton(292,7);

		m_pReservePassword = new CCtrlEdit;
		AddControl(m_pReservePassword);
		m_pReservePassword->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,119,119,141,18,CTRL_STYLE_BACKMODE_NODRAW|CTRL_STYLE_SELFDELETE_TEX);
		m_pReservePassword->SetMaxLength(30);
		m_pReservePassword->SetDigital(true);

		m_pOK = new CCtrlButton;
		AddControl(m_pOK);
		m_pOK->CreateEx(this,74,153,95,96,97);
		m_pOK->SetText("确定");

		m_pCancel = new CCtrlButton;
		AddControl(m_pCancel);
		m_pCancel->CreateEx(this,191,154,95,96,97);
		m_pCancel->SetText("取消");
	}
}

bool CReservePasswordWnd::Msg(DWORD dwMsg, DWORD dwData, CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(!pControl)
			return true;
		else if(pControl == m_pOK)
		{
			if(strlen(m_pReservePassword->GetText()) > 0)
			{
				//if(m_iType == 1)
				//{
				//	g_pGameControl->SEND_Assert_Reserve(m_pReservePassword->GetText(),true);
				//}
				//else if(m_iType == 0)
				//{
				//	g_pGameControl->SEND_Assert_Reserve(m_pReservePassword->GetText());
				//}

				g_pGameControl->SEND_Challenge_Ack(m_pReservePassword->GetText());

				g_pControl->Msg(MSG_CTRL_ENABLELOGINWND,0);
				g_pControl->PopupWindow(MSG_CTRL_RESERVEPASSWORD_WND,OPER_CLOSE);				
			}
			return true;
		}
		else if(pControl == m_pCancel)
		{	
			g_pControl->Msg(MSG_CTRL_ENABLELOGINWND,0);
			g_pControl->PopupWindow(MSG_CTRL_RESERVEPASSWORD_WND,OPER_CLOSE);			
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CReservePasswordWnd::OnClickCloseButton(void)
{
	g_pControl->Msg(MSG_CTRL_ENABLELOGINWND,0);
	g_pControl->PopupWindow(MSG_CTRL_RESERVEPASSWORD_WND,OPER_CLOSE);		
}

