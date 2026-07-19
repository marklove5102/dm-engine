#include "License2Wnd.h"
#include "GameData/LoginData.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include <Shellapi.h>
#include "Global/Interface/AudioInterface.h"


INIT_WND_POSX(CLicense2Wnd,POS_AUTO,POS_AUTO)

CLicense2Wnd::CLicense2Wnd(void)
{
	m_iPages = 1;
	m_iIndex = 40;
	m_bDisableEscape = true;
	m_bModel = true;;
	m_bHaveSetUiType = false;
}

CLicense2Wnd::~CLicense2Wnd(void)
{

}

void CLicense2Wnd::OnCreate()
{
	m_pMarkViewer = new CMarkViewer(15,7,2,3);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,15,30,422,110);
	m_pMarkViewer->SetTagText(&m_TagText);
	m_pMarkViewer->SetFont(FONT_YAHEI);
	m_pMarkViewer->SetDefaultColor(COLOR_TEXT_SBU_TITLE);

	m_pOk = new CCtrlButton; 
	AddControl(m_pOk);
	m_pOk->CreateEx(this,153,139,1658,1659,1660);
	m_pOk->SetText("确定",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);

	m_pBtn = new CCtrlRadio();
	AddControl(m_pBtn);

	string strFileName = GetGameDataDir();
	strFileName += "\\config\\LoginIdCfg.ini";
	int iRtn = GetPrivateProfileInt("ChooseUi",g_Login.GetLoginID(),-1,strFileName.c_str());

	//网页内嵌,要缩放只能用经典
	if (g_hParentWnd != NULL)
	{
		iRtn = 0;
	}

	if (iRtn == 0)
	{
		g_EutUiType = EUT_CLASSIC;
	}
	else
	{
		g_EutUiType = EUT_FASHION;
	}

	//从来没有设置过
	if (iRtn == -1)
	{
		m_pBtn->Create(this,35,140,125,126,127,128,110,20);

		m_pFashional = new CCtrlRadio();
		AddControl(m_pFashional);
		m_pFashional->Create(this,100,110,125,126,127,128,110,20);
		m_pFashional->SetText("时尚版界面",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		m_pFashional->SetChecked(true);

		m_pClassic = new CCtrlRadio();
		AddControl(m_pClassic);
		m_pClassic->Create(this,210,110,125,126,127,128,110,20);
		m_pClassic->SetText("经典版界面",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
		m_pClassic->SetChecked(false);

		m_pFashional->AddBuddy(m_pClassic);
		if (g_pGfx->GetWidth() <= 800)
		{
			m_pFashional->SetRadio(1);
		}
		else
		{
			m_pFashional->SetRadio(0);
		}
		m_pFashional->SetRadioType(0);
		m_pClassic->SetRadioType(0);
	}
	else
	{
		m_pBtn->Create(this,35,110,125,126,127,128,110,20);
		m_pFashional = m_pClassic = NULL;
		m_bHaveSetUiType = true;
	}

	m_pBtn->SetText("同意自由对战",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	m_pBtn->SetChecked(true);
}

void CLicense2Wnd::AfterDraw()
{
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 15,m_iScreenY + 14,"本游戏区是自由对战模式，遵循游戏守则，可以获得畅快游戏体验。",0xFFFFFFFF,FONT_YAHEI);


	if (!m_bHaveSetUiType)
	{
		g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 113,"界面设定：",COLOR_TEXT_NORMAL,FONT_YAHEI);
	}
}

bool CLicense2Wnd::OnKeyDown(WORD wState, UCHAR cKey)
{
	switch (cKey)
	{
	case VK_RETURN:
		{
			Msg(MSG_CTRL_BUTTON_CLICK,0,m_pOk);
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindow::OnKeyDown(wState,cKey);
}

bool CLicense2Wnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOk)
			{
				if (m_pBtn->IsChecked())
				{
					CloseWindow(true);
				}
				else
				{
					PostMessage(g_hWnd,WM_DESTROY,0,0);
				}

				return true;
			}

			if(pControl == m_pBtn)
			{
				if (m_pBtn->IsChecked())
				{
					m_pOk->SetText("确定",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);
				}
				else
				{
					m_pOk->SetText("退出",COLOR_POPUP_BTN_NORMAL,COLOR_POPUP_BTN_MOUSEON,COLOR_POPUP_BTN_PRESS,COLOR_POPUP_BTN_DISABLE,14,DTF_BlackFrame,FONT_YAHEI);
				}
				return true;
			}

			if(pControl == m_pFashional || pControl == m_pClassic)
			{
				int iType = m_pClassic->IsChecked()?0:1;
				//g_pControl->ChangeUi((E_UITYPE)iType);

				if (g_pGfx->GetWidth() == 800 && (E_UITYPE)iType == EUT_FASHION)
				{
					g_MsgBoxMgr.PopSimpleAlert("800*600分辨率模式下不可使用时尚版界面");
					m_pClassic->SetChecked(true);
					m_pFashional->SetChecked(false);
					return true;
				}

				g_EutUiType = (E_UITYPE)iType;
				//写配置文件
				string strFileName = GetGameDataDir();
				strFileName += "\\config\\LoginIdCfg.ini";
				char strTemp[32]={0};
				sprintf(strTemp,"%d",iType);
				WritePrivateProfileString("ChooseUi",g_Login.GetLoginID(),strTemp,strFileName.c_str());

				return true;
			}

		}
		break;	
	case MSG_CTRL_LICENSE2WND_WND:
		if(dwData == 1 && pControl)
		{
			string str = (const char *)pControl;
			m_TagText.Parse(str,0,"|");
		}
		break;

	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CLicense2Wnd::OnLeftButtonUp(int iX, int iY)
{
	m_bClick = false;
	string strCommand = m_pMarkViewer->GetCommand();

	if(!strCommand.empty())
	{
		g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);
		if(strCommand.substr(0,2) == "@@")
		{
			if(strCommand.substr(2,3)=="url")
			{
				ShellExecute(NULL,"open","iexplore.exe",strCommand.substr(5).c_str(),NULL,SW_SHOW);
			}
		}
	}

	//m_pMarkViewer->DoCommand();
	return true;
}