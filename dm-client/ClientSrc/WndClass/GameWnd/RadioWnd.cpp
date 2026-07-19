#include "radiownd.h"
#include "Global/Interface/StreamInterface.h"
#include "BaseClass/Misc/Internet.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include <Shellapi.h>
#include "GameClient/WidgetManager.h"
#include "GameData/ConfigData.h"


INIT_WND_POSX(CRadioWnd,POS_VARIABLE,POS_VARIABLE)

CRadioWnd::CRadioWnd()
{
	m_iCurPage = 0;
	m_dwStartLoadRadioTime = 0;
	m_pContainer = NULL;

	string strUrl = g_pStreamMgr->GetWebsite("Radio","");//电台
	if(strUrl.empty())
		strUrl = "http://61.172.252.96/home/project08/diantai/link.asp";

	g_HttpMgr.InternetGet(strUrl.c_str(),INET_RADIO);

	m_iIndex = 15945;
	m_iPages = 2;
	m_iWidgetX = 20,m_iWidgetY = 40;

	
	m_iAlignType = XAL_CENTER;
	if(!g_pControl->GetWindowPos(m_iOffX,m_iOffY,"ReceiveMsgWnd"))
	{
		m_iOffX	= -74;
		m_iOffY	= -44;
	}

	//默认关闭游戏音效
	g_TempConfig.SetSndOn(false);
	SaveSoundOnOff();

	m_iVersion = 2;
	AddTabPage(0,0,MAKELONG(15945,PACKAGE_INTERFACE),35,36,120,121,122,123,"接收消息",NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(15945,PACKAGE_INTERFACE),107,36,120,121,122,123,"网络电台",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);

	m_TabPage.iCurPage = 1;
}

CRadioWnd::~CRadioWnd(void)
{
	if(!g_OtherData.GetRadioOpen())
		g_WidgetMgr.LogoutRadio();

	g_WidgetMgr.SetShow(EWT_RADIO,false);

	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"ReceiveMsgWnd");
}

void CRadioWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();
	SetCloseButton(405,4,80);

	m_vRect.clear();
	RECT rc;
	rc.left		= 16;
	rc.top		= 15;
	rc.right	= 80;
	rc.bottom	= 30;
	m_vRect.push_back(rc);
	rc.left		= 81;
	rc.top		= 15;
	rc.right	= 141;
	rc.bottom	= 30;
	m_vRect.push_back(rc);


	m_pCloseRadioBtn = new CCtrlButton();
	m_pCloseRadioBtn->CreateEx(this, 335,285, 95, 96, 97, 98);
	m_pCloseRadioBtn->SetText("退出电台", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	AddControl(m_pCloseRadioBtn);

	m_pContainer = new CMarkViewer(16,12);
	AddControl(m_pContainer);
	m_pContainer->Create(this,30,70,377,204);
	m_pContainer->SetTagText(&g_TagTextMgr.GetRadioData());

	if(g_OtherData.GetRadioOpen())
	{
		m_pContainer->SetShow(false);
		m_pContainer->SetEnable(false);
		m_pCloseRadioBtn->SetEnable(true);
		m_pCloseRadioBtn->SetShow(true);   
	}
	else
	{
		m_pContainer->SetShow(true);
		m_pContainer->SetEnable(true);
		m_pCloseRadioBtn->SetEnable(false);
		m_pCloseRadioBtn->SetShow(false);
	}


	m_dwStartLoadRadioTime = 0;

	bool bSoundOn= g_TempConfig.GetSndOn();

	m_pRDSoundOn = new CCtrlRadio();
	AddControl(m_pRDSoundOn);
	m_pRDSoundOn->Create(this,171,322,125,126,127,128,50,20);
	m_pRDSoundOn->SetText("开",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_MIDDLE,0,FONT_YAHEI,8);
	
	m_pRDSoundOn->SetChecked(bSoundOn);		

	m_pRDSoundOff = new CCtrlRadio();
	AddControl(m_pRDSoundOff);
	m_pRDSoundOff->Create(this,232,322,125,126,127,128,50,20);
	m_pRDSoundOff->SetText("关",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_MIDDLE,0,FONT_YAHEI,8);
	m_pRDSoundOff->SetChecked(!bSoundOn);

	if(g_OtherData.GetRadioOpen())
		g_WidgetMgr.SetShow(EWT_RADIO,true);

}

void CRadioWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 219,m_iScreenY + 10,"电  台",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	if(g_OtherData.GetRadioOpen())
	{
		m_pContainer->SetShow(false);
		m_pContainer->SetEnable(false);
		m_pCloseRadioBtn->SetEnable(true);
		m_pCloseRadioBtn->SetShow(true);   
		//g_pFont->DrawText(m_iScreenX + 60, m_iScreenY + 150,"你正在收听传世电台，离开传世电台请点击关闭按钮",0xFFFFFFFF,FONTSIZE_DEFAULT);
		g_WidgetMgr.RenderWidget(EWT_RADIO);
	}
	else
	{
		m_pCloseRadioBtn->SetEnable(false);
		m_pCloseRadioBtn->SetShow(false);

		if(m_dwStartLoadRadioTime > 0)
		{
			m_pContainer->SetShow(false);
			m_pContainer->SetEnable(false);

			DWORD dwPastTime = GetTickCount() - m_dwStartLoadRadioTime;
			if(dwPastTime > 10000)
				g_pFont->DrawText(m_iScreenX + 170, m_iScreenY + 150,"连接超时！",0xFFFFFFFF);
			else
			{
				if(dwPastTime % 1000 < 250)
					g_pFont->DrawText(m_iScreenX + 120, m_iScreenY + 150,"正在连接传世电台，请稍候",0xFFFFFFFF);
				else if(dwPastTime % 1000 < 500)
					g_pFont->DrawText(m_iScreenX + 120, m_iScreenY + 150,"正在连接传世电台，请稍候。",0xFFFFFFFF);
				else if(dwPastTime % 1000 < 750)
					g_pFont->DrawText(m_iScreenX + 120, m_iScreenY + 150,"正在连接传世电台，请稍候。。",0xFFFFFFFF);
				else
					g_pFont->DrawText(m_iScreenX + 120, m_iScreenY + 150,"正在连接传世电台，请稍候。。。",0xFFFFFFFF);
			}
		}
		else
		{
			m_pContainer->SetShow(true);
			m_pContainer->SetEnable(true);
		}
	}

	g_pFont->DrawText(m_iScreenX + 135,m_iScreenY + 324,"游戏音效",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_Center);
}

bool CRadioWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			g_pControl->PopupWindow(MSG_CTRL_RECEIVE_MSG_WND,OPER_CREATE);
			CloseWindow();
			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK :
		if(pControl == m_pCloseRadioBtn)
		{
			g_WidgetMgr.LogoutRadio();
			m_iCurPage = -1;
			SwitchToPage(0);
			return true;
		}
		else if(pControl == m_pRDSoundOn)
		{
			//打开声音按钮
			m_pRDSoundOn->SetChecked(true);
			m_pRDSoundOff->SetChecked(false);
			g_TempConfig.SetSndOn(m_pRDSoundOn->IsChecked());

			SaveSoundOnOff();
		}
		else if(pControl == m_pRDSoundOff)
		{
			//关闭声音按钮
			m_pRDSoundOn->SetChecked(false);
			m_pRDSoundOff->SetChecked(true);
			g_TempConfig.SetSndOn(m_pRDSoundOn->IsChecked());

			SaveSoundOnOff();
		}
	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


bool CRadioWnd::OnLeftButtonUp(int iX, int iY)
{
	if(m_iCurPage == 0 && !g_OtherData.GetRadioOpen())
	{
		string strCommand = m_pContainer->GetCommand();
		if(!strCommand.empty())
		{
			g_WidgetMgr.LoginRadio(strCommand.c_str());
			g_WidgetMgr.MoveWindow(EWT_RADIO,m_iScreenX+m_iWidgetX,m_iScreenY+m_iWidgetY,352,197);
			m_dwStartLoadRadioTime = GetTickCount();
			g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);
			return true;
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX, iY);
}

void CRadioWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	g_WidgetMgr.MoveWindow(EWT_RADIO,m_iScreenX+m_iWidgetX,m_iScreenY+m_iWidgetY);
}

void CRadioWnd::SaveSoundOnOff()
{
	// 音效开关
	if( g_TempConfig.GetSndOn() != g_Config.GetSndOn() )
	{
		g_Config.SetSndOn( g_TempConfig.GetSndOn() );
		if( g_Config.GetSndOn() )
		{
			g_pStreamMgr->SetConfigStr("Sound","Yes");
		}
		else
		{
			g_pStreamMgr->SetConfigStr("Sound","No");
			g_pAudio->StopAll();
		}

		g_pAudio->EnableSound(g_TempConfig.GetSndOn());
	}
}

void  CRadioWnd::OnSetFocus()
{
	if(g_OtherData.GetRadioOpen() && m_iCurPage == 0)
		g_WidgetMgr.SetFocus(EWT_RADIO,true);
}

void CRadioWnd::OnKillFocus()
{
	if(g_OtherData.GetRadioOpen() && m_iCurPage == 0)
		g_WidgetMgr.SetFocus(EWT_RADIO,false);
}
