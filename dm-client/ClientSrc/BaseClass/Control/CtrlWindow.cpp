#include "Control.h"
#include "CtrlButton.h"
#include "Baseclass/Control/ParserTip.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ArrowTip.h"
#include "GameData/GameData.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DWORD CCtrlWindow::sm_dwWindowType = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DTI_IMPLEMENT(CCtrlWindow, CControlContainer)
CCtrlWindow::CCtrlWindow()
{
	m_iControlMode		= CTRL_MODE_WINDOW;
	m_pCloseButton		= NULL;
	m_dwLifeTime        = 0;
	m_iOldx  = m_iOldy  = -1;
	m_bDisableEscape    = false;
}

CCtrlWindow::~CCtrlWindow()
{
}

void CCtrlWindow::SetCloseButton(int iX,int iY,int iTexIndex)
{
	if(m_pCloseButton)
		return;

	m_pCloseButton = new CCtrlButton();
	AddControl(m_pCloseButton);
	if(iTexIndex > 0)
		m_pCloseButton->Create(this,iX,iY,iTexIndex);
	else
		m_pCloseButton->CreateEx(this,iX,iY,0,1,2);

	if(iTexIndex)
	{
		m_pCloseButton->SetMask(iTexIndex);
	}
}

// 处理拖动
bool CCtrlWindow::OnMouseMove(int iX,int iY)
{
	g_pControl->GetTipWnd()->SetShow(false);//CtrlWindow的继承类如果没有处理OnMouseMove统统不显示tips

	if(g_dwMouseType != MOUSE_NORMAL)
		g_dwMouseType = MOUSE_NORMAL;

	if( m_bNoMove ) return true;

	int x,y;
	g_pInput->GetMousePos(x,y);
	if( m_bClick && !g_pInput->IsLeftButton() )
		m_bClick = false;
	if( m_bRBClick && !g_pInput->IsRightButton() )
		m_bRBClick = false;

	if( m_bClick || m_bRBClick )				// 有待改进
	{
		if( m_iOldx!=x || m_iOldy!=y ) 
			Move(m_iX+(x-m_iOldx),m_iY+(y-m_iOldy));
	}

	m_iOldx = x;
	m_iOldy = y;
	
	return true;
}


void CCtrlWindow::Draw()
{
	if( !m_bShow ) return;

	// 绘制所有子控件
	CControlContainer::Draw();
}

void CCtrlWindow::OnClickCloseButton()
{
}

bool CCtrlWindow::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if( m_pCloseButton && dwMsg==MSG_CTRL_BUTTON_CLICK && pControl==(CControl *)m_pCloseButton )
	{
		//关闭窗口音乐/////////////////////////////
		g_pAudio->Play(EAT_OTHER,1,g_pAudio->GetRand()++);	
	
		OnClickCloseButton();
		return true;
	}
	else if (dwMsg == MSG_REMOVE_ARROWTIP_CONTROL)
	{
		if ((int)dwData < 0 || dwData >= 64)
		{
			return true;
		}

		CControl * p = m_pLastDrawControls;
		while (p)
		{
			CArrowTip *pArrow = dynamic_cast<CArrowTip *>(p);
			if (pArrow && pArrow->GetTipID() == (int)dwData)
			{
				p = p->GetControlNext();
				CControl *pRemoveControl = (CControl *)pArrow;
				if (m_pParent)
				{
					m_pParent->Msg(MSG_ARROWTIP_CLICK_CLOSEBTN,pArrow->GetTipID(),pArrow);
				}

				RemoveControl(&pRemoveControl,true);
				continue;
			}

			p = p->GetControlNext();
		}

		return true;
	}

	return CControl::Msg(dwMsg,dwData,pControl);	
}

bool CCtrlWindow::OnKeyDown(WORD wState, UCHAR cKey)
{
	switch(cKey)
	{
	case VK_ESCAPE:
		if(m_bDisableEscape)
			return false;

		OnEscape();
		return true;
	}
	return false; 
}

void CCtrlWindow::OnEscape(void)
{
	if(m_bDisableEscape)
		return;

	OnClickCloseButton();
}

bool CCtrlWindow::AddArrowTip(int iTipID,int iX,int iY,int iAlignType,bool bShowNow,int iTipDir,int iTipType,DWORD dwData,DWORD dwLastTime,bool bNeedMsg)
{
	if (iTipID < 0 || iTipID >= 64)
	{
		return false;
	}

	//已经弹过这个提示了
	if (g_pGameData->HasPaoPaoStatus(ePaoPaoPromptStatus(iTipID)))
	{
		return false;
	}

	CArrowTip * pTip = new CArrowTip();

	if (!bShowNow)
	{
		DWORD dwStartTime = GetTickCount() - ARROW_LAST_TIME;
		CControl * p = m_pLastDrawControls;
		while (p)
		{
			CArrowTip *pArrow = dynamic_cast<CArrowTip *>(p);
			if (pArrow && dwStartTime < pArrow->GetStartTime())
			{
				dwStartTime = pArrow->GetStartTime();
			}

			p = p->GetControlNext();
		}

		pTip->SetStartTime(dwStartTime + ARROW_LAST_TIME);
	}

	pTip->SetTipID(iTipID);
	AddControl(pTip,-1,true);
	pTip->Create(this,iX,iY,iAlignType,iTipDir,iTipType,dwData,dwLastTime,bNeedMsg);

	return true;
}
