#include "RegisterInfoWnd.h"
#include "GameControl/GameControl.h"
#include "Global/StringUtil.h"

INIT_WND_POSX(CRegisterInfoWnd,POS_VARIABLE,POS_VARIABLE)

CRegisterInfoWnd::CRegisterInfoWnd(void)
{
	m_pIdentityCard = NULL;
	m_pRealName = NULL;
	m_iIndex = 11553;
	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CRegisterInfoWnd::~CRegisterInfoWnd(void)
{
}

void CRegisterInfoWnd::Draw(void)
{
	CCtrlWindowX::Draw(); 
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 160,m_iScreenY + 20,"身份验证",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
	g_pFont->DrawText(m_iScreenX + 66,m_iScreenY + 64,"真实姓名",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
	g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 102,"身份证号码",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

	if(m_strMessage.size() > 0)
	{
		TextOut(45,130 + 30,m_strMessage.c_str(),0xFFFF0000);
	}
	else
	{
		TextOut(45,130 + 30,"    您填写的信息将被提交验证。",COLOR_TEXT_SBU_TITLE);
		TextOut(45,150 + 30,"    若验证为未满18周岁，则您的帐号将被纳入防沉迷，",COLOR_TEXT_SBU_TITLE);
		TextOut(45,166 + 30,"您的游戏收益将会受到时间限制！若验证为已满18周岁，",COLOR_TEXT_SBU_TITLE);
		TextOut(45,182 + 30,"则不会被纳入防沉迷系统。",COLOR_TEXT_SBU_TITLE);
		TextOut(45,202 + 30,"    请认真填写有效的实名身份信息并提交。已提交的",COLOR_TEXT_SBU_TITLE);
		TextOut(45,218 + 30,"身份信息如需修改，可重新登录游戏，在角色选择界面",COLOR_TEXT_SBU_TITLE);
		TextOut(45,234 + 30,"更换身份信息再次提交。",COLOR_TEXT_SBU_TITLE);
	}
}


bool CRegisterInfoWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pSubmitButton)
		{
			string szRealName = m_pRealName->GetText();
			StringUtil::trim(szRealName);

			int iNameLen = szRealName.size();
			if(iNameLen == 0)
			{
				m_strMessage = "姓名不能为空，请重新输入";
				return true;
			}

			//判断是否合法字符
			bool bInvalidChar = false;

			for(int ii = 0;ii < iNameLen;ii++)
			{
				if(szRealName[ii] == '/')
				{
					bInvalidChar = true;
					break;
				}
			}

			if(bInvalidChar)
			{
				m_strMessage = "姓名中包含非法字符，请正确输入";
				return true;
			}

			string szIdentiryCard = m_pIdentityCard->GetText();
			StringUtil::trim(szIdentiryCard);

			int iIDLen = szIdentiryCard.size();

			bInvalidChar = false;

			for(int ii = 0;ii < iIDLen;ii++)
			{
				if(szIdentiryCard[ii] == '/')
				{
					bInvalidChar = true;
					break;
				}

				if(ii < 17 && !isdigit(szIdentiryCard[ii]))
				{
					bInvalidChar = true;
					break;
				}
			}

			if(bInvalidChar)
			{
				m_strMessage = "身份证中包含非法字符，请正确输入";
				return true;
			}

			if(iIDLen != 18 && iIDLen != 15)
			{
				m_strMessage = "身份证位数不正确，请正确输入";
				return true;
			}

			
			//g_pGameControl->SEND_Login_Register_Info(szRealName.c_str(),szIdentiryCard.c_str(),1);
			g_pGameControl->SEND_FCMFillInfo_Req(szRealName.c_str(),szIdentiryCard.c_str(),1);
			g_pControl->Msg(MSG_CTRL_REGISTERINFO_BTN_HIDE,0);
			CloseWindow();
			return true;
		}
		else
		{
			CloseWindow();
			return true;
		}
	}
	else if(dwMsg == MSG_CTRL_REGISTER_INFO_WND)
	{
		if (dwData == MSG_CTRL_REGISTER_INFO_WND_FOCUS)
		{
			m_pRealName->SetFocus();
		}	
		return true;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CRegisterInfoWnd::OnCreate(void)
{
	//SetCloseButton(340,4);

	//真实姓名
	m_pRealName = new CCtrlEdit;
	AddControl(m_pRealName);
	m_pRealName->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,130,62,190,20);
	m_pRealName->SetMaxLength(20);

	//身份证号码
	m_pIdentityCard = new CCtrlEdit;
	AddControl(m_pIdentityCard);
	m_pIdentityCard->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,130,98,190,20);
	m_pIdentityCard->SetMaxLength(18);

	//提交
	m_pSubmitButton = new CCtrlButton;
	AddControl(m_pSubmitButton);
	m_pSubmitButton->CreateEx(this,96,320,59,60,61);
	m_pSubmitButton->SetText("提交",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_MIDDLE);

	//关闭
	m_pCancelButton = new CCtrlButton;
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,207,320,59,60,61);
	m_pCancelButton->SetText("取消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_MIDDLE);

	SetFont(FONT_YAHEI,FONTSIZE_DEFAULT);
}

void CRegisterInfoWnd::OnClickCloseButton()
{
	CloseWindow();
}
