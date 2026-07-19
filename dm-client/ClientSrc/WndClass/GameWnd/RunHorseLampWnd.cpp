#include "RunHorseLampWnd.h"
#include "GameData/GameData.h"

INIT_WND_POSX(CRunHorseLampWnd,POS_VARIABLE,POS_VARIABLE)

CRunHorseLampWnd::CRunHorseLampWnd(void)
{
	m_iIndex = 17668;

	m_bNoChangeLevel = true;
	m_bDisableEscape = true;
	m_iAlignType = XAL_TOP;

	m_iOffX = -80;//中心偏左80像素
	m_iOffY = 80;
	m_iHorseDrawX = 0;
}

CRunHorseLampWnd::~CRunHorseLampWnd(void)
{
}

void CRunHorseLampWnd::Draw(void)
{
	HorseRunToDead* pHRTD = g_pGameData->GetFirstHorseRunToDead();	
	
	if (pHRTD == NULL) return;
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	//绘制文字
	static DWORD dwTime = GetTickCount();
	DWORD dwCurTm = GetTickCount();
	if (dwCurTm - dwTime >= 30)
	{
		m_iHorseDrawX++;
		dwTime = dwCurTm;
	}
	
	int isize = 7 - pHRTD->strText.length() * 7;

	if ( 519 - 17 - m_iHorseDrawX <= isize)
	{
		m_iHorseDrawX = 0;
		g_pGameData->RemoveHorseRunToDead();
		
		if (!g_pGameData->HasHorseRunToDead())
			g_pControl->Msg(MSG_CTRL_RUNHORSELAMP_WND,OPER_CLOSE);
		
		return;
	}

	g_pFont->DrawTextEx(m_iScreenX + 519 - 17 - m_iHorseDrawX,m_iScreenY + 5,m_iScreenX  + 20,m_iScreenX + 519 - 17,pHRTD->strText.c_str(),0xFF87c23a,FONT_YAHEI,14,DTF_BlackFrame);
}

bool CRunHorseLampWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CRunHorseLampWnd::OnMouseMove(int iX,int iY)
{
	return CCtrlWindowX::OnMouseMove(iX,iY);
}
