#include "userinfownd.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"


INIT_WND_POSX(CUserInfoWnd,POS_AUTO,POS_AUTO)

//字符串去空格
CUserInfoWnd::CUserInfoWnd(void)
{
	m_pOKButton = NULL;
	m_pCancelButton = NULL;
	m_pUserName = NULL;
	m_pTelephone = NULL;
	m_pAddress = NULL;
	m_pPostCode = NULL;
	m_pFemale = NULL;
	m_pMale = NULL;
	m_bMale = true;

	m_iIndex = 4849;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
}

CUserInfoWnd::~CUserInfoWnd(void)
{
}

void CUserInfoWnd::OnCreate()
{
	m_pUserName = new CCtrlEdit();
	AddControl(m_pUserName);
	m_pUserName->CreateEx(this,77,59,116,18);
	m_pUserName->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pUserName->SetDigital(false);
	m_pUserName->SetMaxLength(40);
	m_pUserName->SetFocus();


	m_pTelephone = new CCtrlEdit();
	AddControl(m_pTelephone);
	m_pTelephone->CreateEx(this,77,108,116,18);
	m_pTelephone->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pTelephone->SetDigital(false);
	m_pTelephone->SetMaxLength(40);

	m_pAddress = new CCtrlEdit();
	AddControl(m_pAddress);
	m_pAddress->CreateEx(this,77,133,116,18);
	m_pAddress->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pAddress->SetDigital(false);
	m_pAddress->SetMaxLength(100);	
	
	m_pPostCode = new CCtrlEdit();
	AddControl(m_pPostCode);
	m_pPostCode->CreateEx(this,77,157,116,18);
	m_pPostCode->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pPostCode->SetDigital(true);
	m_pPostCode->SetMaxLength(6);

	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this, 20,196, 165, 166, 167, 168);
	m_pOKButton->SetText("确  定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this, 130,196, 165, 166, 167, 168);
	m_pCancelButton->SetText("取  消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pMale = new CCtrlRadio();
	AddControl(m_pMale);
	m_pMale->Create(this,81,84,125,126,127,128,50,20);
	m_pMale->SetText("男",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	m_pMale->SetChecked(true);

	m_pFemale = new CCtrlRadio();
	AddControl(m_pFemale);
	m_pFemale->Create(this,136,84,125,126,127,128,50,20);
	m_pFemale->SetText("女",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	m_pFemale->SetChecked(false);
}

void CUserInfoWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 120,m_iScreenY + 29,"联系方式登记栏",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	g_pFont->DrawText(m_iScreenX + 36,m_iScreenY + 62,"姓名：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 36,m_iScreenY + 87,"性别：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 36,m_iScreenY + 110,"电话：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 36,m_iScreenY + 136,"地址：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 36,m_iScreenY + 161,"邮编：",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);

}

bool CUserInfoWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_USERINFO_USERNAME:
		{
			m_pUserName->SetFocus();
			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pOKButton)
		{
			char strUserName[1024] = {0};
			strcpy(strUserName,m_pUserName->GetText());
			StringUtil::trim(strUserName);
			if(NULL == strUserName || strlen(strUserName) == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("请输入您的真实姓名！");
				return true;
			}
			if(IsInvalidCharacter(strUserName))
			{
				g_MsgBoxMgr.PopSimpleAlert("姓名中包含非法字符，请重新输入！");
				return true;
			}

			char strTelephone[1024] = {0};
			strcpy(strTelephone,m_pTelephone->GetText());
			StringUtil::trim(strTelephone);
			if(NULL == strTelephone || strlen(strTelephone) == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("请输入您的电话号码！");
				return true;
			}
			if(IsInvalidCharacter(strTelephone))
			{
				g_MsgBoxMgr.PopSimpleAlert("电话号码中包含非法字符，请重新输入！");
				return true;
			}

			char strPostCode[1024] = {0};
			strcpy(strPostCode,m_pPostCode->GetText());
			StringUtil::trim(strPostCode);;
			if(NULL == strPostCode || strlen(strPostCode) < 6)
			{
				g_MsgBoxMgr.PopSimpleAlert("请输入您的邮政编码！");
				return true;
			}

			char strAddress[1024] = {0};
			strcpy(strAddress,m_pAddress->GetText());
			StringUtil::trim(strAddress);
			if(NULL == strAddress || strlen(strAddress) == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("请输入您的通讯地址！");
				return true;
			}
			if(IsInvalidCharacter(strAddress))
			{
				g_MsgBoxMgr.PopSimpleAlert("通讯地址中包含非法字符，请重新输入！");
				return true;
			}
			g_pGameControl->SEND_User_Real_Info(strUserName,m_bMale,strTelephone,strAddress,strPostCode);
			CloseWindow();

			return true;
		}
		else if(pControl == m_pCancelButton)
		{
			CloseWindow();
			return true;
		}
		else if(pControl == m_pMale)
		{
			m_pFemale->SetChecked(false);
			m_pMale->SetChecked(true);
			m_bMale = true;
			return true;
		}
		else if(pControl == m_pFemale)
		{
			m_pFemale->SetChecked(true);
			m_pMale->SetChecked(false);
			m_bMale = false;
			return true;
		}

		break;
	default:
		break;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CUserInfoWnd::OnLeftButtonDown(int iX, int iY)
{
	if(iX >= 33 && iX <= 243 && iY>= 40 && iY<= 174)
		return true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CUserInfoWnd::IsInvalidCharacter(std::string str)
{
	for(int i = 0;i<str.size();i++)
	{
		if(str[i] == '\'' || str[i] == '\"')
			return true;
	}
	return false;
}
