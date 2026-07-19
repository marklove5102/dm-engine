#include "VoiceWnd.h"
#include "GameClient/VoiceAdapter.h"
#include "Global/Interface/StreamInterface.h"
#include "BaseClass/Control/ParserTip.h"

#ifdef _CHAT

INIT_WND_POSX(CVoiceCfgWnd,POS_AUTO,POS_AUTO)

CVoiceCfgWnd::CVoiceCfgWnd()
{
	m_pButtonActivate = NULL;
	m_pVoiceActivate = NULL;
	m_pMicEnhance = NULL;
	m_pConfirmButton = NULL;
	m_pCancelButton = NULL;
	m_iIndex = 14054;

	int iVoiceActLevel = g_VoiceAdapter.GetSpeakVoiceActiLevel();
	if (iVoiceActLevel <= 4 && iVoiceActLevel >= 1)
		m_ArrowPosition = iVoiceActLevel;
	else
		m_ArrowPosition = 4;
}

CVoiceCfgWnd::~CVoiceCfgWnd()
{
}

void CVoiceCfgWnd::OnCreate()
{
	SetCloseButton(217,1,80);

	m_pConfirmButton = new CCtrlButton();
	m_pConfirmButton->CreateEx(this, 44,197, 95, 96, 97, 98);
	m_pConfirmButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	AddControl(m_pConfirmButton);

	m_pCancelButton = new CCtrlButton();
	m_pCancelButton->CreateEx(this, 127,197, 95, 96, 97, 98);
	m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	AddControl(m_pCancelButton);

	m_pButtonActivate = new CCtrlRadio();
	m_pButtonActivate->Create(this,19,67,125,126,127,128,30,20);
	m_pButtonActivate->SetText("按键激活",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	AddControl(m_pButtonActivate);

	m_pVoiceActivate = new CCtrlRadio();
	m_pVoiceActivate->Create(this,19,96,125,126,127,128,30,20);
	m_pVoiceActivate->SetText("语音激活",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	AddControl(m_pVoiceActivate);

	m_pMicEnhance = new CCtrlRadio();
	m_pMicEnhance->Create(this,19,157,125,126,127,128,110,20);
	m_pMicEnhance->SetText("麦克风增强",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
	AddControl(m_pMicEnhance);

	m_pMicEnhance->SetChecked(false);

	if(g_VoiceAdapter.GetSpeakWay() == 2)
		m_pButtonActivate->SetChecked(true);
	else
		m_pButtonActivate->SetChecked(false);

	if (g_VoiceAdapter.GetSpeakWay() == 3)
		m_pVoiceActivate->SetChecked(true);
	else 
		m_pVoiceActivate->SetChecked(false);

	//if(g_pSpeak)
	//{
	//	if (g_pSpeak->GetSuperMic() == 0)
	//		m_pMicEnhance->SetEnable(false);
	//	else if(g_pSpeak->GetSuperMic() == 2)
	//		g_pSpeak->SetSuperMic(FALSE);
	//}
	m_pMicEnhance->SetEnable(false);

}

void CVoiceCfgWnd::OnClickCloseButton()
{
	CloseWindow();
}

bool CVoiceCfgWnd::OnLeftButtonDown(int iX,int iY)
{
	if(iY> 102 && iY < 112 && m_pVoiceActivate->IsChecked())
	{
		if(iX > 106 && iX < 130)
		{
			m_ArrowPosition = 1;
		}
		else if(iX >= 130 && iX < 156)
		{
			m_ArrowPosition = 2;
		}
		else if(iX >= 156 && iX < 182)
		{
			m_ArrowPosition = 3;
		}
		else if(iX >= 182 && iX < 212)
		{
			m_ArrowPosition = 4;
		}
	}

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

void CVoiceCfgWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	LPTexture pArrow = g_pTexMgr->GetTex(PACKAGE_INTERFACE,138,EP_UI);
	if(m_ArrowPosition == 1)
		g_pGfx->DrawTextureNL(m_iScreenX + 106, m_iScreenY + 102,pArrow);
	else if(m_ArrowPosition == 2)
		g_pGfx->DrawTextureNL(m_iScreenX + 138, m_iScreenY + 102,pArrow);
	else if(m_ArrowPosition == 3)
		g_pGfx->DrawTextureNL(m_iScreenX + 170, m_iScreenY + 102,pArrow);
	else if(m_ArrowPosition == 4)
		g_pGfx->DrawTextureNL(m_iScreenX + 202, m_iScreenY + 102,pArrow);

	g_pFont->DrawText(m_iScreenX + 122,m_iScreenY + 6,"语音设置",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
	g_pFont->DrawText(m_iScreenX + 52,m_iScreenY + 122,"灵敏度:  高   中  中低  低",0xff5aa0a0,FONT_YAHEI,FONTSIZE_SMALL);
}

bool CVoiceCfgWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if (pControl == m_pMicEnhance)
		{
			return true;
		}
		else if (pControl == m_pVoiceActivate)
		{
			m_pButtonActivate->SetChecked(!m_pButtonActivate->IsChecked());
			return true;
		}
		else if (pControl == m_pButtonActivate)
		{
			m_pVoiceActivate->SetChecked(!m_pVoiceActivate->IsChecked());
			return true;
		}
		else if (pControl == m_pConfirmButton)
		{
			if (m_pButtonActivate->IsChecked())
			{
				g_VoiceAdapter.SetSpeakWay(2,17);
				g_pStreamMgr->SetConfigInt("ActivateMode",2);
				g_pStreamMgr->SetConfigInt("ActivateLevel",17);
			}
			else if (m_pVoiceActivate->IsChecked())
			{
				g_VoiceAdapter.SetSpeakWay(3,m_ArrowPosition);
				g_pStreamMgr->SetConfigInt("ActivateMode",3);
				g_pStreamMgr->SetConfigInt("ActivateLevel",m_ArrowPosition);
			}

			if(g_pSpeak && g_pSpeak->GetSuperMic() != 0)
			{
				if (m_pMicEnhance->IsChecked())
					g_pSpeak->SetSuperMic(true);
				else
					g_pSpeak->SetSuperMic(false);
			}

			CloseWindow();
			return true;
		}
		else  if (pControl == m_pCancelButton)
		{
			CloseWindow();
			return true;		
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


bool CVoiceCfgWnd::OnMouseMove(int iX,int iY)
{	
	//if(iX> 23 && iX < 41 && iY > 202 && iY < 218)
	//{
	//	CParserTip *pTip = g_pControl->GetTipWnd();
	//	pTip->Clear();
	//	int x = m_iScreenX + iX + 10;
	//	int y = m_iScreenY + iY + 10;
	//	pTip->AdjustXY(x,y);
	//	pTip->Move(x,y);
	//	pTip->SetShow(true);
	//	pTip->AddText("如点击使用后出现声音异常，请关闭此功能");
	//	return true;
	//}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

#endif
