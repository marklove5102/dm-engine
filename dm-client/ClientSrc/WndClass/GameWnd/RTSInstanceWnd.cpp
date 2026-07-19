#include "RTSInstanceWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"

INIT_WND_POSX(CRTSInstanceWnd,POS_AUTO,POS_AUTO)

bool CRTSInstanceWnd::s_bState = false;

CRTSInstanceWnd::CRTSInstanceWnd(void)
{
	m_sName = "RTSInstanceWnd";
	m_bDisableEscape = true;
	
	if (!s_bState)
	{
		m_iIndex = 777;
	} 
	else
	{
		m_iIndex = 776;
	}

	m_pAttackSoldier = 0;
	m_pDefendSoldier = 0;
	m_pTower = 0;
	m_pChangeState = 0;
}

CRTSInstanceWnd::~CRTSInstanceWnd(void)
{
}

void CRTSInstanceWnd::OnCreate()
{
	//
	if (!s_bState)//展开
	{
		m_pAttackSoldier = new CCtrlButton;
		AddControl(m_pAttackSoldier);
		m_pAttackSoldier->CreateEx(this,42,61,778,779,780,781);
		m_pAttackSoldier->SetText("召唤进攻卫兵",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pAttackSoldier->SetTips("用100个资源换取5个进攻石人自动进攻敌对营地.一次性召唤太多石人，有可能会绕路.");

		m_pDefendSoldier = new CCtrlButton;
		AddControl(m_pDefendSoldier);
		m_pDefendSoldier->CreateEx(this,42,99,778,779,780,781);
		m_pDefendSoldier->SetText("召唤守护卫兵",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pDefendSoldier->SetTips("用100个资源换取5个守护铜人守护在图腾基石周围.");

		m_pTower = new CCtrlButton;
		AddControl(m_pTower);
		m_pTower->CreateEx(this,42,137,778,779,780,781);
		m_pTower->SetText("换取箭塔",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		m_pTower->SetTips("用300个资源换取1个箭塔图腾，可以放置在地图上的八卦阵上.");

		m_pChangeState = new CCtrlButton;
		AddControl(m_pChangeState);
		m_pChangeState->CreateEx(this,81,173,784,785,784);

	} 
	else
	{
		m_pChangeState = new CCtrlButton;
		AddControl(m_pChangeState);
		m_pChangeState->CreateEx(this,79,52,782,783,782);
	}
	
	CCtrlWindowX::OnCreate();
}

void CRTSInstanceWnd::Draw(void)
{
	//在何时关闭掉窗口
	sTXZG &txzg = g_OtherData.GetTXZG();
	if (!txzg.bStarted)
	{
		CloseWindow();
		return;
	}

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	//
	g_pFont->DrawText(m_iScreenX + 40,m_iScreenY + 30,"资源换取列表",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
}

bool CRTSInstanceWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	//
	if (dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if (pControl == m_pChangeState)
		{
			s_bState = !s_bState;
			g_pControl->PopupWindow(MSG_CTRL_RTSINSTANCE_WND,OPER_RECREATE);
			return true;
		}
		else if (m_pAttackSoldier && pControl == m_pAttackSoldier)
		{
			g_pGameControl->Send_RTSInstance_Exchange_ByResource(1);
			return true;
		} 
		else if (m_pDefendSoldier && pControl == m_pDefendSoldier)
		{
			g_pGameControl->Send_RTSInstance_Exchange_ByResource(2);
			return true;
		}
		else if (m_pTower && pControl == m_pTower)
		{
			g_pGameControl->Send_RTSInstance_Exchange_ByResource(3);
			return true;
		}
	}
	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

