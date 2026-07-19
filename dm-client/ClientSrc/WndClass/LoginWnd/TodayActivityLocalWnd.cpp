#include "TodayActivityLocalWnd.h"

INIT_WND_POSX(CTodayActivityLocalWnd,POS_VARIABLE,POS_VARIABLE)

CTodayActivityLocalWnd::CTodayActivityLocalWnd(void)
{
	m_bInheritScaleFromParent = false;
	m_iIndex = 17635;
	m_pAct = NULL;
	//m_bNoMove = true;

	m_iAlignType = XAL_LEFT;
	//m_iOffX = (g_pGfx->GetWidth() - 240 - 240 - 40) / 2;
	m_iOffX = 0;
	m_iOffY = -40;
}

CTodayActivityLocalWnd::~CTodayActivityLocalWnd(void)
{
	SAFE_DELETE_ARRAY(m_pAct);
}

void CTodayActivityLocalWnd::OnCreate()
{
	m_pAct = new _ActivityInfo*[4];
	for (int i = 0;i<4;i++)
	{
		m_pAct[i] = NULL;
	}
	g_PromptInfoMgr.GetActivityInfo(m_pAct);

	struct tm *newtime = NULL;	
	__time32_t tTime;
	_time32(&tTime);
	tTime += g_dwServerTime;
	int iDay = 0;
	newtime = _localtime32(&tTime);
	if (newtime)
	{
		iDay = newtime->tm_wday;
	}

	ThemeInfo* pTheme = g_PromptInfoMgr.GetThemeInfo(iDay);
	if (pTheme)
	{		
		m_Tag.Parse(pTheme->strTheme,0,"\\",28);
	}

	m_pMarkTheme = new CMarkViewer(16,3);
	m_pMarkTheme->Create(this,34,47,170,52);
	AddControl(m_pMarkTheme);
	m_pMarkTheme->SetTagText(&m_Tag);

	//if (g_pGfx->GetWidth() == 800)
	//	 this->Move(800,100);
	//else this->Move(1024,150);
}

void CTodayActivityLocalWnd::Draw(void)
{
	//static DWORD dwTime = GetTickCount();
	//if (dwTime + 3000 > GetTickCount())
	//{
	//	int idevX = (int)(g_pGfx->GetWidth() - this->GetWidth() * (GetTickCount() - dwTime) / (float)3000);
	//	this->Move(idevX,0);
	//}

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	for (int i = 0;i<4;i++)
	{
		if (m_pAct[i])
		{
			string str = m_pAct[i]->strName + ":";
			str += m_pAct[i]->strTime;
			int iLen = (187 - str.length() * FONTSIZE_SMALL/2)/2;
			g_pFont->DrawText(m_iScreenX + 25 + iLen ,m_iScreenY + 163 + i * 34,str.c_str(),0xFFFFFFFF);
		}
	}

	char* pTm = g_Timer.GetDateTime("%Y年%m月%d日",g_Timer.GetCurTime() + g_dwServerTime);
	string str = pTm;

	string str1 = g_Timer.GetDateTime("%H:%M:%S",g_Timer.GetCurTime() + g_dwServerTime);

	g_pFont->DrawText(m_iScreenX + 25,m_iScreenY + 307,"今日时间",0xffff6600,FONT_YAHEI,FONTSIZE_DEFAULT);
	g_pFont->DrawText(m_iScreenX + 85,m_iScreenY + 307,str.c_str(),0xFFFFFF00);
	g_pFont->DrawText(m_iScreenX + 95,m_iScreenY + 327,str1.c_str(),0xFFFFFF00);

	g_pFont->DrawText(87+m_iScreenX, 18+m_iScreenY, "今日主题", 0xFFFAC896, FONT_YAHEI, FONTSIZE_BIG);
	g_pFont->DrawText(87+m_iScreenX, 127+m_iScreenY, "今日活动", 0xFFFAC896, FONT_YAHEI, FONTSIZE_BIG);
}

void CTodayActivityLocalWnd::OnSetFocus()
{
	if (m_pParent)
	{
		m_pParent->Msg(MSG_CTRL_ON_SET_FOCUS,0,this);
	}
}

bool CTodayActivityLocalWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	switch(cKey)
	{
	case VK_RETURN:
		{
			if (m_pParent)
			{
				return m_pParent->OnKeyDown(wState,cKey);
			}
		}
	} 

	return CCtrlWindowX::OnKeyDown(wState,cKey);
}