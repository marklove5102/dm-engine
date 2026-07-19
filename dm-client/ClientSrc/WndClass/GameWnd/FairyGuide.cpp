#include "FairyGuide.h"
#include "GameData/GameData.h"

INIT_WND_POSX(CFairyGuide,POS_VARIABLE,POS_VARIABLE)

CFairyGuide::CFairyGuide(void)
{
	m_iIndex = 17609;
	m_iAlignType = XAL_BOTTOMLEFT;

	m_iOffX = 0;
	m_iOffY = - 350;

	m_iCurLine = 0;
}

CFairyGuide::~CFairyGuide(void)
{
	g_pGameData->ClearCareForInfo();
	g_pControl->Msg(MSG_CTRL_UPDATE_INSTANCE_INFO,0);
}

void CFairyGuide::OnCreate()
{
	SetCloseButton(188,2,80);

	m_pPreBtn = new CCtrlButton();
	AddControl(m_pPreBtn);
	m_pPreBtn->CreateEx(this,25,100,95,96,97);
	m_pPreBtn->SetText("上一条", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pNextBtn = new CCtrlButton();
	AddControl(m_pNextBtn);
	m_pNextBtn->CreateEx(this,120,100,95,96,97);
	m_pNextBtn->SetText("下一条", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pContent = new CMarkViewer(14,4,0,0,true);
	AddControl(m_pContent);
	m_pContent->Create(this,16,34,186,122 - 37);
	m_pContent->SetFont(FONT_YAHEI, FONTSIZE_DEFAULT);
	m_pContent->SetDefaultColor(0xFFFAC897);
	m_pContent->SetDrawOffXY(0,3);
	m_pContent->SetTagText(&m_TagText);
	InitialFairyGuide();
}

void CFairyGuide::Draw(void)
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iDevX = (m_iWidth - m_strTile.size() * 16/2)/2 - 5;
	g_pFont->DrawText(m_iScreenX + iDevX,m_iScreenY + 7,m_strTile.c_str(),0xfff8c993,FONT_YAHEI,16);
}


bool CFairyGuide::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pPreBtn)
			{
				if (m_iCurLine > 0)
				{
					m_iCurLine --;
					InitialFairyGuide();
					return true;
				}
			}
			else if (pControl == m_pNextBtn)
			{
				if (m_iCurLine < (int)(g_pGameData->GetCareForInfo().size()) - 1)
				{
					m_iCurLine ++;
					InitialFairyGuide();
					return true;
				}
			}
		}
		break;
	case MSG_CTRL_UPDATE_PROMPT_INFO:
		{
			InitialFairyGuide();
		}
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CFairyGuide::InitialFairyGuide()
{
	VCAREFORINFO& vCInfo = g_pGameData->GetCareForInfo();	
	if (m_iCurLine >= 0 && m_iCurLine < vCInfo.size())
	{
		CareForInfo *pCareInfo = &(vCInfo[m_iCurLine]);
		if (pCareInfo)
		{
			m_strTile = pCareInfo->strTile;
			m_TagText.Clear();
			m_TagText.Parse(pCareInfo->strContent,0,"\\",27);
			m_pContent->SetTagText(&m_TagText);
			m_pContent->SetDisLine(0);
		}
	}
	else
	{
		m_strTile.clear();
		m_pContent->SetTagText(NULL);
	}

	if (m_iCurLine <= 0)
	{
		m_pPreBtn->SetEnable(false);
	}
	else
	{
		m_pPreBtn->SetEnable(true);
	}

	if (m_iCurLine >= (int)vCInfo.size() - 1)
	{
		m_pNextBtn->SetEnable(false);
	}
	else
	{
		m_pNextBtn->SetEnable(true);
	}
}


bool CFairyGuide::OnWheel(int iWheel)
{
	if (m_pContent)
	{
		return m_pContent->OnWheel(iWheel);
	}

	return false;
}
