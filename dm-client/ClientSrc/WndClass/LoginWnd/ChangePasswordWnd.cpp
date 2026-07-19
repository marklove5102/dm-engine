#include "ChangePasswordWnd.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CChangePasswordWnd,POS_VARIABLE,POS_VARIABLE)
CChangePasswordWnd::CChangePasswordWnd()
{

	m_bModel		= true;
	m_pUserID		= NULL;
	m_pOldPassword	= NULL;
	m_pNewPassword1	= NULL;
	m_pNewPassword2	= NULL;
	m_pSubmit		= NULL;
	m_pCancel		= NULL;
	m_dwState		= sm_dwWindowType;
	m_iAlignType = XAL_CENTER;
	m_iPages = 1;
	if(m_dwState == 0)
		m_iIndex = 261;
	else
		m_iIndex = 794;

	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CChangePasswordWnd::~CChangePasswordWnd()
{
}

void CChangePasswordWnd::OnCreate()
{
	SetCloseButton(341,11);


	m_pNewPassword2 = new CCtrlEdit;
	AddControl(m_pNewPassword2);
	m_pNewPassword2->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,171,159,138,19);
	m_pNewPassword2->SetPwd(true);
	m_pNewPassword2->SetMaxLength(10);

	m_pNewPassword1 = new CCtrlEdit;
	AddControl(m_pNewPassword1);
	m_pNewPassword1->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,171,127,138,19);
	m_pNewPassword1->SetPwd(true);
	m_pNewPassword1->SetMaxLength(10);

	m_pOldPassword = new CCtrlEdit;
	AddControl(m_pOldPassword);
	m_pOldPassword->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,171,94,138,19);
	m_pOldPassword->SetPwd(true);
	m_pOldPassword->SetMaxLength(20);

	m_pUserID = new CCtrlEdit;
	AddControl(m_pUserID);
	m_pUserID->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,171,62,138,19);
	if(m_dwState > 0)
		m_pUserID->SetMaxLength(20);
	else
		m_pUserID->SetMaxLength(16);


	m_pSubmit = new CCtrlButton;
	AddControl(m_pSubmit);
	if(m_dwState > 0)
	{
		m_pSubmit->CreateEx(this,98,202,0,264,265);
	}
	else
	{
		m_pSubmit->CreateEx(this,98,208,0,264,265);
	}

	m_pCancel = new CCtrlButton;
	AddControl(m_pCancel);

	if(m_dwState > 0)
	{
		m_pCancel->CreateEx(this,229,202,0,262,263);
	}
	else
	{
		m_pCancel->CreateEx(this,229,208,0,262,263);
	}

	m_pUserID->SetFocus();
}

bool CChangePasswordWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
		case MSG_CTRL_BUTTON_CLICK:
			if(pControl == m_pSubmit)
			{
				OnSubmit();			
				return true;
			}
			else if(pControl == m_pCancel)
			{
				OnCancel();			
				return true;
			}
			break;
		case MSG_CTRL_EDIT_ENTRY:
			{
				OnSubmit();
				return true;
			}
			break;
		case MSG_CTRL_EDIT_ESCAPE:
			{
				if(pControl!=NULL)
					((CCtrlEdit*)pControl)->Clear();
				return true;
			}
			break;
		default:
			break;
		}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CChangePasswordWnd::OnSubmit()
{
	if(strcmp(m_pNewPassword1->GetText(),m_pNewPassword2->GetText()) == 0)
	{
		if(strlen(m_pNewPassword1->GetText()) < 4)
		{
			m_pNewPassword1->OnSelectAll();
			m_pNewPassword1->SetFocus();
			return false;
		}

		string tmp = m_pUserID->GetText();
		tmp += 0x09;
		tmp += m_pOldPassword->GetText();
		tmp += 0x09;
		tmp += m_pNewPassword1->GetText();

		g_pGameControl->SEND_Login_Userpwd_Change(tmp.c_str(),m_dwState);

		CloseWindow();
		return true;
	}
	else
	{
		CloseWindow();
		g_MsgBoxMgr.PopSimpleComfirm("重复密码和新密码不一致，请重新输入！",0,0);
		return true;
	}
}

bool CChangePasswordWnd::OnCancel()
{
	CloseWindow();
	return true;
}

bool CChangePasswordWnd::HasSpecChar(const char *pText)
{
	int len = strlen(pText);
	int i = 0;
	for(i=0; i<len; i++)
	{
		if((*(pText + i) >= '0' && *(pText + i) <= '9') ||
			(*(pText + i) >= 'a' && *(pText + i) <= 'z') ||
			(*(pText + i) >= 'A' && *(pText + i) <='Z')
			)
		{

		}
		else
		{
			return true;
		}
	}
	return false;
}