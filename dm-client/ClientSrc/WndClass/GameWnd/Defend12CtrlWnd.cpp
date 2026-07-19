#include "Defend12CtrlWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameManager.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/GameGlobal.h"
#include "GameAI/AIMgr.h"


INIT_WND_POSX(CDefend12CtrlWnd, POS_VARIABLE,POS_VARIABLE)

CDefend12CtrlWnd::CDefend12CtrlWnd()
{
	m_bDisableEscape = true;

	m_pGenSuiButton = NULL;
	m_pGenSuiAttackButton = NULL;
	m_pZiYouButton = NULL;
	m_pDaiJiButton = NULL;

	m_iIndex = 20975;
	m_iMask = 20975;

	m_iAlignType = XAL_BOTTOM;
	m_iOffY = -220 + 97;
}

CDefend12CtrlWnd::~CDefend12CtrlWnd()
{	
}

void CDefend12CtrlWnd::Draw()
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	DrawTexture(60, 65, 21573);
	DrawTexture(280, 65, 21573);

	int nCount = 0;
	if (nCount == 0)
	{
		//显示空头像
		DrawTexture(167, -1, 20980);
	}
	else if (nCount == 1)
	{
		//显示怪物头像
		//DrawTexture(167, -1, 13471+pMember->byJob+pMember->bySex*3);		

	}
	else
	{
		DrawTexture(167, -1, 20994);
	}
}

bool CDefend12CtrlWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pGenSuiAttackButton)
			{
				g_pGameControl->SEND_BingControl(0);
				return true;
			}			
			else if (pControl == m_pGenSuiButton)
			{
				g_pGameControl->SEND_BingControl(1);
				return true;
			}
			else if (pControl == m_pDaiJiButton)
			{
				g_pGameControl->SEND_BingControl(2);
				return true;
			}			 
			else if (pControl == m_pZiYouButton)
			{
				g_pGameControl->SEND_BingControl(3);
				return true;
			}
			
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg, dwData, pControl);
}

void CDefend12CtrlWnd::OnCreate()
{
	SetCloseButton(449,44, 23000);

	m_pGenSuiButton = new CCtrlButton();
	m_pGenSuiButton->CreateEx(this, 50, 38, 95,96,97,98);
	AddControl(m_pGenSuiButton);
	m_pGenSuiButton->SetText("跟 随");
	

	m_pGenSuiAttackButton = new CCtrlButton();
	m_pGenSuiAttackButton->CreateEx(this, 110, 38, 95,96,97,98);
	AddControl(m_pGenSuiAttackButton);
	m_pGenSuiAttackButton->SetText("跟随战斗");
	

	m_pZiYouButton = new CCtrlButton();
	m_pZiYouButton->CreateEx(this, 305, 38, 95,96,97,98);
	AddControl(m_pZiYouButton);
	m_pZiYouButton->SetText("自由行动");

	m_pDaiJiButton = new CCtrlButton();
	m_pDaiJiButton->CreateEx(this, 365, 38, 95,96,97,98);
	AddControl(m_pDaiJiButton);
	m_pDaiJiButton->SetText("待 机");
}

void CDefend12CtrlWnd::OnClickCloseButton()
{
	//显示12宫按扭
	g_pControl->Msg(MSG_CTRL_DEFEND12GONGBTN,1);

	CloseWindow();
}

bool CDefend12CtrlWnd::OnLeftButtonDClick(int iX,int iY)
{
	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

bool CDefend12CtrlWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);

	if (iX > m_pGenSuiButton->GetX() && iX < m_pGenSuiButton->GetX() + m_pGenSuiButton->GetWidth() &&
		iY > m_pGenSuiButton->GetY() && iY < m_pGenSuiButton->GetY() + m_pGenSuiButton->GetHeight())
	{
		pTip->AddText("跟随模式：所有佣兵向玩家考虑，同时停止攻击。",COLOR_GREEN);
		pTip->SetShow(true);		
	}
	else if (iX > m_pGenSuiAttackButton->GetX() && iX < m_pGenSuiAttackButton->GetX() + m_pGenSuiAttackButton->GetWidth() &&
		iY > m_pGenSuiAttackButton->GetY() && iY < m_pGenSuiAttackButton->GetY() + m_pGenSuiAttackButton->GetHeight())
	{
		pTip->AddText("跟随战斗模式：使屏幕可见范围内（一屏）的属于自己的兵种跟随自己移动及攻击，其余未选种的兵种则自由行动。",COLOR_GREEN);
		pTip->SetShow(true);
	}
	else if (iX > m_pZiYouButton->GetX() && iX < m_pZiYouButton->GetX() + m_pZiYouButton->GetWidth() &&
		iY > m_pZiYouButton->GetY() && iY < m_pZiYouButton->GetY() + m_pZiYouButton->GetHeight())
	{
		pTip->AddText("自由行动模式：取消对当前所有兵种的控制，以默认AI进行行动。",COLOR_GREEN);
		pTip->AddText("默认AI：攻击一切出现在各兵种视野范围内的第一目标。",COLOR_GREEN);
		pTip->SetShow(true);
	}
	else if (iX > m_pDaiJiButton->GetX() && iX < m_pDaiJiButton->GetX() + m_pDaiJiButton->GetWidth() &&
		iY > m_pDaiJiButton->GetY() && iY < m_pDaiJiButton->GetY() + m_pDaiJiButton->GetHeight())
	{
		pTip->AddText("待机模式：所有佣兵停止移动，同时停止攻击。",COLOR_GREEN);
		pTip->SetShow(true);		
	}

	if (pTip->IsShow())
	{
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		return true;
	}
	return CCtrlWindowX::OnMouseMove(iX,iY);	
}
