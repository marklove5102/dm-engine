#include "InputMiBaoWnd.h"
#include "GameMap/GameMap.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameAI/AIMgr.h"
#include "GameData/MsgBoxMgr.h"

INIT_WND_POSX(CInputMiBaoWnd,POS_VARIABLE,POS_VARIABLE)

CInputMiBaoWnd::CInputMiBaoWnd()
{
	m_dwStartTime = GetTickCount();
	m_bNoMove   = true;
	m_bMinBao   = false;
	m_iMaxLen   = -1;

	m_iIndex = 30;
	m_iPages = 1;

	m_iType  = sm_dwWindowType;

	if(m_iType >= 100)
	{
		m_byUsbEKeyType = m_iType - 100;
		m_iType = 1;
	}

	if (m_iType == 0 || m_iType == 1)
	{
		SetLifeTime(m_dwStartTime + 180000);

		m_iAlignType = XAL_TOPLEFT;
		m_iOffX = 4;
		m_iOffY = (g_pGfx->GetWidth() > 800)?278:160;
	}
	else if (m_iType == 2)
	{
		SetLifeTime(m_dwStartTime + g_AIMgr.GetReserveTime(plyVerifyTotalTime) * 1000);
	
		m_iAlignType = XAL_BOTTOMRIGHT;
		m_iOffX = 0;
		m_iOffY = -160;
	}
	
	m_pChangePic = m_pOKButton = NULL;
	m_pEdit = NULL;
	m_pValidate = NULL;
	m_bDisableEscape = true;
	m_dwLastClickFrashBtnTime = 0;
}

CInputMiBaoWnd::~CInputMiBaoWnd(void)
{
	g_AIMgr.SetReserveTime(plyMibaoLateTime,0);
	g_AIMgr.SetReserveTime(plyMibaoTotalTime,0);
	g_AIMgr.SetReserveTime(plyVerifyLateTime,0);
	g_AIMgr.SetReserveTime(plyVerifyTotalTime,0);

	if (m_iType == 3)
	{
		if(m_pValidate)
		{
			g_pTexMgr->ReleaseTex(m_pValidate );
		}
		DeleteFile("tmp1.tex");
	}
}

void CInputMiBaoWnd::OnCreate()
{
	if (m_iType == 2)
	{
		m_pEdit = new CCtrlEdit();
		AddControl(m_pEdit);
		m_pEdit->CreateEx(this,37,57, 160, 20,0,39,39,39,39);
		m_pEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI,8);
		m_pEdit->SetEnable(false);
		m_pEdit->SetShow(false);

		m_pOKButton = new CCtrlButton();
		AddControl(m_pOKButton);
		m_pOKButton->CreateEx(this,30,113,1658,1659,1660);
		m_pOKButton->SetText("确定",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);
		m_pOKButton->SetEnable(false);
		m_pOKButton->SetShow(false);

		m_pChangePic = new CCtrlButton();
		AddControl(m_pChangePic);
		m_pChangePic->CreateEx(this,125,113,1658,1659,1660);
		m_pChangePic->SetText("换一张",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);
		m_pChangePic->SetEnable(false);
		m_pChangePic->SetShow(false);
	}
}

void CInputMiBaoWnd::Draw(void)
{
	DWORD dwCount = GetTickCount();

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	char temp[1024]={0};

	DWORD dwMibaoTime = g_AIMgr.GetReserveTime(plyMibaoLateTime);
	if(m_iType == 2 || m_iType == 3)
		dwMibaoTime = g_AIMgr.GetReserveTime(plyVerifyLateTime);

	if((dwCount - m_dwStartTime) < dwMibaoTime*1000)
	{
		int iLeftTime = ((m_dwStartTime + dwMibaoTime*1000) - dwCount + 500) / 1000;
		if(iLeftTime<0)
			iLeftTime = 0;

		int iFontSize = FONTSIZE_DEFAULT;

		if (m_iType == 0)
			g_pFont->DrawText(m_iScreenX+64,m_iScreenY+42,"秒后按提示输入密宝信息",0xFFFF0000, FONT_YAHEI);
		else if(m_iType == 1)
			g_pFont->DrawText(m_iScreenX+64,m_iScreenY+42,"秒后验证密宝信息",0xFFFF0000, FONT_YAHEI);
		else if(m_iType == 2)
		{
			g_pFont->DrawText(m_iScreenX+50,m_iScreenY + 62,"秒后按提示输入验证码信息",0xFFFF0000, FONT_YAHEI,FONTSIZE_MIDDLE);
			//g_pFont->DrawText(m_iScreenX + 15,m_iScreenY + 42 + 20,"未填写或填写错误将会返回到登录界面",0xFFFF0000, FONT_YAHEI);
		}

		sprintf(temp,"%3d",iLeftTime);
		if (m_iType == 0 || m_iType == 1)
		{
			g_pFont->DrawText(m_iScreenX+40,m_iScreenY+42,temp,0xFFFFFF00, FONT_YAHEI);
		}
		else if (m_iType == 2)
		{
			g_pFont->DrawText(m_iScreenX+20,m_iScreenY+62,temp,0xFFFFFF00, FONT_YAHEI,FONTSIZE_MIDDLE);
		}
	}
	else
	{
		if(m_iType == 1)
		{
			//g_pGameControl->SEND_UsbMb(m_byUsbEKeyType,1);
			CloseWindow();
			return;
		}

		DWORD dwMibaoTotalTime = 180000;
		if(m_iType == 2 || m_iType == 3)
			dwMibaoTotalTime = g_AIMgr.GetReserveTime(plyVerifyTotalTime) * 1000;

		if(!m_bMinBao)
		{
			if (m_iType == 0 || m_iType == 1)
			{
				g_pControl->PopupWindow(MSG_CTRL_INPUT_MIBAO_WND,OPER_UPDATE);
				g_pControl->Msg(MSG_CTRL_INPUT_MIBAO_WND,1,(CControl*)m_strText.c_str());
				g_pControl->Msg(MSG_CTRL_INPUT_MIBAO_WND,3);
				if (m_iMaxLen > 0)
				{
					g_pControl->MsgToWnd(MSG_CTRL_INPUT_MIBAO_WND,MSG_CTRL_INPUT_MIBAO_WND,4,(CControl*)(m_iMaxLen));
				}
			}
			else if (m_iType == 2)
			{
				//g_MsgBoxMgr.PopEditBox("请输入验证码:",MSG_CTRL_LEVEL_CHECK,1,NULL,false,24,0,0,NULL,false);
				m_iType = 3;
				if(m_pEdit)
				{
					m_pEdit->SetEnable(true);
					m_pEdit->SetShow(true);
				}
				if (m_pOKButton)
				{
					m_pOKButton->SetEnable(true);
					m_pOKButton->SetShow(true);
				}
				if (m_pChangePic)
				{
					m_pChangePic->SetEnable(true);
					m_pChangePic->SetShow(true);
				}
			}

			m_bMinBao = true;
		}

		int iLeftTime = ((m_dwStartTime + dwMibaoTotalTime) - dwCount+ 500) / 1000;
		if(iLeftTime<0)
			iLeftTime = 0;

		sprintf(temp,"%3d",iLeftTime);
		int iOffX = 36,iOffY = 42;
		if (m_iType == 0 || m_iType == 1)
		{
			g_pFont->DrawText(m_iScreenX + iOffX,m_iScreenY + iOffY,"请在    秒内输入密宝信息",0xFFFF0000, FONT_YAHEI);
		}
		else if (m_iType == 2)
		{
			g_pFont->DrawText(m_iScreenX + iOffX,m_iScreenY + iOffY,"请在    秒内输入验证码信息",0xFFFF0000, FONT_YAHEI);
		}
		else if (m_iType == 3)
		{
			iOffY = 40;
			g_pFont->DrawText(m_iScreenX + iOffX,m_iScreenY + iOffY,"请输入验证码：",0xFFFF0000, FONT_YAHEI);
			iOffY = 80;
			g_pFont->DrawText(m_iScreenX + iOffX,m_iScreenY + iOffY,"请在    秒内输入验证码信息",0xFFFF0000, FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + iOffX - 20,m_iScreenY + iOffY + 20,"未填写或填写错误将会返回到登录界面",0xFFFF0000, FONT_YAHEI);
			
			if(m_pValidate == NULL)
			{
				string strPath = GetGameDataDir();
				strPath += "\\tmp1.tex";
				FILE * fp = fopen(strPath.c_str(),"rb");
				if(fp)
				{
					fclose(fp);
					m_pValidate = g_pTexMgr->LoadTexFromDiskFile(strPath.c_str(),EP_UI);
				}
			}

			if(m_pValidate != NULL)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + 120,m_iScreenY + 11, m_pValidate);
			}
		}

		g_pFont->DrawText(m_iScreenX + iOffX + 24,m_iScreenY + iOffY,temp,0xFFFFFF00, FONT_YAHEI);
	}
}

void CInputMiBaoWnd::OnClickCloseButton()
{
	if (m_iType == 0 || m_iType == 1)
	{
		g_pControl->PopupWindow(MSG_CTRL_INPUT_MIBAO_WND,OPER_CLOSE);
	}

	CloseWindow();
}

bool CInputMiBaoWnd::Msg(DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOKButton)
			{
				OnClickOkButton();
				CloseWindow();
				return true;
			}
			else if (pControl == m_pChangePic)
			{
				if (GetTickCount() - m_dwLastClickFrashBtnTime > 3000)
				{
					g_pGameControl->SEND_FreshVerify_Ack();
					m_dwLastClickFrashBtnTime = GetTickCount();
				}
				return true;
			}
		}
		break;
	case MSG_CTRL_EDIT_ENTRY:
		{
			OnClickOkButton();
			CloseWindow();
			return true;
		}
		break;
	case MSG_INPUT_KEYDOWN:
		{
			WORD w1 = HIWORD( dwData );
			WORD w2 = LOWORD( dwData );

			if(w2 == VK_ESCAPE)
				return true;
		}
		break;
	case MSG_CTRL_INPUT_MIBAO_LATER_WND:
		{
			if(dwData == 1)
			{
				m_strText = (const char *)pControl;
			}
			else if(dwData == 2)
			{
				m_iMaxLen = int((__int64)pControl);
			}
			else if (m_iType == 3 && dwData == 3)
			{
				if(m_pValidate)
				{
					g_pTexMgr->ReleaseTex(m_pValidate );
					m_pValidate = NULL;
				}

				string strPath = GetGameDataDir();
				strPath += "\\tmp1.tex";
				FILE * fp = fopen(strPath.c_str(),"rb");
				if(fp)
				{
					fclose(fp);
					m_pValidate = g_pTexMgr->LoadTexFromDiskFile(strPath.c_str(),EP_UI);
				}
			}

		}
		break;
	}

	return CCtrlWindowX::Msg(dwMsg, dwData, pControl);

}


void CInputMiBaoWnd::OnClickOkButton()
{
	if (m_iType == 3)
	{
		g_pGameControl->SEND_InGameVerify_Ack(m_pEdit->GetText());
	}
}

