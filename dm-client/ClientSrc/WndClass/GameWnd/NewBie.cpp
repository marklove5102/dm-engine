#include "newbie.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "BaseClass/Audio/Audio.h"
#include "BaseClass/Misc/Stream.h"


CInternet CNewBie::m_inter;
INIT_WND_POSX(CNewBie,POS_SETPOS_VARIABLE,POS_SETPOS_VARIABLE)

CNewBie::CNewBie(void)
{
	m_bClick = false;
	string strUrl = g_StreamMgr.GetWebsite("NewChar","");
	m_inter.InternetGet(strUrl.c_str(),2);


	m_iIndex = 13993;
	m_iPages = 1;
	m_uStyle = CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX;
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = (g_Gfx.GetWidth() - 410)/2;
	m_iOffY = 100;

}

CNewBie::~CNewBie(void)
{
}

void CNewBie::Draw(void)
{
	CCtrlWindowX::Draw();
}

bool CNewBie::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CNewBie::OnCreate()
{
	SetCloseButton(377,10); 

	m_pMarkViewer = new CMarkViewer(16,10);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,30,26,374,180);
//	m_pMarkViewer->SetTagText(&g_pGameData->GetNewBieData());

}

void CNewBie::OnClickCloseButton()
{
	g_pControl->PopupWindow(OPER_CLOSE,"NewBie");
}

bool CNewBie::OnLeftButtonDown(int iX, int iY)
{
	m_bClick = true;
	return true;
}

bool CNewBie::OnLeftButtonUp(int iX, int iY)
{
	string strCommand = m_pMarkViewer->GetCommand();

	if(!strCommand.empty())
	{
		//up ÒôÀÖ
		g_pAudio->Play(EAT_OTHER,5,g_nRand++);

		m_inter.InternetGet(strCommand.c_str(),2);
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX, iY);
}
