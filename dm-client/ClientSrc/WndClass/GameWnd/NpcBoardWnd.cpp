#include "NpcBoardWnd.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include <Shellapi.h>
#include "GameData/NpcData.h"
#include "GameClient/SDOAInterface.h"


const int CNpcBoardWnd::MAX_ROWS = 15;
INIT_WND_POSX(CNpcBoardWnd,POS_VARIABLE,POS_VARIABLE)

CNpcBoardWnd::CNpcBoardWnd(void)
{
	m_pMarkViewerLeft = NULL;
	m_pMarkViewerRight = NULL;

	m_iIndex = 4687;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

	if((g_pGfx->GetWidth() > 800))
	{
		m_iOffX = 199;
		m_iOffY = 100;
	}
	else
	{
		m_iOffX = 87;
		m_iOffY = 55;
	}
}

CNpcBoardWnd::~CNpcBoardWnd(void)
{
}

void CNpcBoardWnd::OnCreate()
{
	SetCloseButton(572,17,80);  

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,291,68,15,240,4688);
	m_pScroll->SetRange(g_TagTextMgr.GetBoard().GetRow() - MAX_ROWS);

	m_pMarkViewerLeft = new CMarkViewer(16,MAX_ROWS);
	AddControl(m_pMarkViewerLeft);
	m_pMarkViewerLeft->Create(this,42,67,250,264);
	m_pMarkViewerLeft->SetTagText(&g_TagTextMgr.GetBoard());
	m_pMarkViewerLeft->SetFont(FONT_YAHEI, FONTSIZE_DEFAULT);
	m_pMarkViewerLeft->SetDefaultColor(0xFFFAC897);

	m_pMarkViewerRight = new CMarkViewer(16,MAX_ROWS);
	AddControl(m_pMarkViewerRight);
	m_pMarkViewerRight->Create(this,338,67,250,264);
	m_pMarkViewerRight->SetTagText(&g_TagTextMgr.GetNpcText());
	m_pMarkViewerRight->SetFont(FONT_YAHEI, FONTSIZE_DEFAULT);
	m_pMarkViewerRight->SetDefaultColor(0xFFFAC897);

}

void CNpcBoardWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	m_pMarkViewerLeft->SetDisLine(m_pScroll->GetPos());
	g_pFont->DrawText(280+m_iScreenX, 17+m_iScreenY, "公  告", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
}

bool CNpcBoardWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	if(dwMsg == MSG_CTRL_NPCBOARDWND && dwData == MSG_CTRL_NPCBOARD_UPDATE)
	{
		m_pMarkViewerLeft->SetDisLine(0);
		m_pScroll->SetPos(0);
		m_pScroll->SetRange(g_TagTextMgr.GetBoard().GetRow() - MAX_ROWS);
		return true;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CNpcBoardWnd::OnClickCloseButton()
{
	g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@exit");
	CloseWindow();
}

bool CNpcBoardWnd::OnLeftButtonUp(int iX, int iY)
{
	m_bClick = false;
	string strCommand = m_pMarkViewerLeft->GetCommand();
	if(strCommand.empty())
		strCommand = m_pMarkViewerRight->GetCommand();
	
	if(!strCommand.empty())
	{
		//up 音乐
		g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);

		g_NPC.SetRepairWindow(false);
		if(strCommand.substr(0,2) == "@@")
		{
			g_NPC.SetLastCommand(strCommand);

			if(strCommand.substr(2,3)=="url")
			{
				ShellExecute(NULL,"open","iexplore.exe",strCommand.substr(5).c_str(),NULL,SW_SHOW);
			}
			else if(strCommand == "@@REALINFO")
			{
				g_pControl->Msg(MSG_CTRL_USERINFO_WND,OPER_CREATE);
			}
			else if(strCommand == "@@RAINBOW")
			{
				//AP不要彩虹帮助,里面有广告,彩虹无法去掉,平台不接受
				if (g_strChannelName.empty())
				{
					StartRainBowSpirt();
				}
			}
			else if(strCommand == "@@STORE")
			{
				g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,2);
			}
			return true;

		}
		else if((int)strCommand.size() > 0)
		{
			g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
			return true;
		}
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CNpcBoardWnd::OnMouseMove(int iX, int iY)
{
	return  CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CNpcBoardWnd::OnLeftButtonDown(int iX, int iY)
{
	m_bClick = true;
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CNpcBoardWnd::OnWheel(int iWheel)
{
	if(m_pScroll->GetRange() == 0)
		return true;

	short iWheelSize = (short)iWheel;
	iWheelSize /= WHEEL_DELTA;

	int iPos = m_pScroll->GetPos();
	iPos -= iWheelSize;

	if(iPos > m_pScroll->GetRange())
		iPos = m_pScroll->GetRange();
	if(iPos < 0)
		iPos = 0;
	m_pScroll->SetPos(iPos);

	return true;
}