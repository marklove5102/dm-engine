#include "LeaveMessageWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"

INIT_WND_POSX(CLeaveMessageWnd,POS_AUTO,POS_AUTO)

//字符串去空格
CLeaveMessageWnd::CLeaveMessageWnd(void)
{
	m_pOKButton = NULL;
	m_pCancelButton = NULL;
	m_pUserName = NULL;
	m_pAddress = NULL;
	m_pLeavWords = NULL;

	m_iIndex = 10988;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

}

CLeaveMessageWnd::~CLeaveMessageWnd(void)
{
}

void CLeaveMessageWnd::OnCreate()
{
	m_pUserName = new CCtrlEdit();
	AddControl(m_pUserName);
	m_pUserName->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,116,60,115,16);
	m_pUserName->SetDigital(false);
	m_pUserName->SetMaxLength(20);
	m_pUserName->SetFocus();


	m_pAddress = new CCtrlEdit();
	AddControl(m_pAddress);
	m_pAddress->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,116,87,115,16);
	m_pAddress->SetDigital(false);
	m_pAddress->SetMaxLength(40);

	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,52,183,4843,4844,4845);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,177,183,4846,4847,4848);

	m_pLeavWords = new CCtrlMultiEdit(64,32,2);
	AddControl(m_pLeavWords);
	m_pLeavWords->Create(this,14,38,132,200,28);
	m_pLeavWords->SetText("");

}

void CLeaveMessageWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
}

bool CLeaveMessageWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_LEAV_MESSAGE_FOCUS:
		{
			m_pUserName->SetFocus();
			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pOKButton)
		{
			char strUserName[32] = {0};
			strcpy(strUserName,m_pUserName->GetText());
			StringUtil::trim(strUserName);
			if(NULL == strUserName || strlen(strUserName) == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("请输入对方角色名！");
				return true;
			}
			if(IsInvalidCharacter(strUserName))
			{
				g_MsgBoxMgr.PopSimpleAlert("对方角色名中包含非法字符，请重新输入！");
				return true;
			}

			char strAddress[32] = {0};
			strcpy(strAddress,m_pAddress->GetText());
			StringUtil::trim(strAddress);
			if(NULL == strAddress || strlen(strAddress) == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("请输入对方所在区服！");
				return true;
			}
			if(IsInvalidCharacter(strAddress))
			{
				g_MsgBoxMgr.PopSimpleAlert("对方区服地址中包含非法字符，请重新输入！");
				return true;
			}
			char strContent[128] = "";
			strcpy(strContent,m_pLeavWords->GetText());
			StringUtil::trim(strContent);
			if(NULL == strContent || strlen(strContent) == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("请留言！");
				return true;
			}
			g_pGameControl->SEND_Leave_Message_Info(strUserName,strAddress,strContent);

			CloseWindow();
			return true;
		}
		else if(pControl == m_pCancelButton)
		{
			CloseWindow();
			return true;
		}
		break;
	default:
		break;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CLeaveMessageWnd::OnLeftButtonDown(int iX, int iY)
{
	if(iX >= 33 && iX <= 243 && iY>= 40 && iY<= 174)
		return true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CLeaveMessageWnd::IsInvalidCharacter(std::string str)
{
	for(int i = 0;i<str.size();i++)
	{
		if(str[i] == '\'' || str[i] == '\"')
			return true;
	}
	return false;
}
