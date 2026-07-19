#include "readquitwnd.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CReadQuitWnd,POS_VARIABLE,POS_VARIABLE)

CReadQuitWnd::CReadQuitWnd()
{
	m_dwStartTime = GetTickCount();
	SetLifeTime(m_dwStartTime + QUIT_TIME_DELAY);
	m_bNoMove   = true;


	m_iIndex = 4860;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 4;
	m_iOffY = ((g_pGfx->GetWidth() > 800))?428:(g_pGfx->GetHeight() - 290);

	m_bDisableEscape = true;
	g_pGameControl->SEND_Ready_Quit(sm_dwWindowType);
}

CReadQuitWnd::~CReadQuitWnd(void)
{
	if(sm_dwWindowType == 1)
	{
		if(g_pControl && (g_pControl->GetGameState() == MSG_CTRL_GAMEWND))
			g_pGameControl->SEND_Objects_Position();

		PostMessage(g_hWnd,WM_DESTROY,0,0);
	}
	else if(sm_dwWindowType == 2)
	{
		if(g_pControl)
			g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CUSTOM);
	}
	else
	{
		if(g_pGameControl)
			g_pGameControl->SEND_Out_Game();	
	}
}

void CReadQuitWnd::OnCreate()
{
}

void CReadQuitWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	DWORD dwCount = GetTickCount();

	int iLeftTime = ((m_dwStartTime + QUIT_TIME_DELAY) - dwCount + 500) / 1000;

	if(iLeftTime<0)
		iLeftTime = 0;

	char temp[1024]={0};
	sprintf(temp,"%4d",iLeftTime);
	g_pFont->DrawText(m_iScreenX+66,m_iScreenY+42,temp,0xFFFF0000);
}

bool CReadQuitWnd::Msg(DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	if(dwMsg == MSG_INPUT_KEYDOWN)
	{
		WORD w1 = HIWORD( dwData );
		WORD w2 = LOWORD( dwData );

		if(w2 == VK_ESCAPE)
			return true;
	}
	else if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		return true;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}