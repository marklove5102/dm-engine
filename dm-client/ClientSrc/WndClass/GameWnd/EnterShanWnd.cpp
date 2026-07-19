#include "EnterShanWnd.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CEnterShanWnd,POS_AUTO,POS_AUTO)

CEnterShanWnd::CEnterShanWnd(void)
{
	m_iIndex = 22360;
	m_bModel = true;

	m_pEnter = NULL;

}

CEnterShanWnd::~CEnterShanWnd(void)
{

}

void CEnterShanWnd::OnCreate()
{
	SetCloseButton(805,35, 80);

	m_pEnter = new CCtrlButton;
	m_pEnter->CreateEx(this,333,507,22365,22366,22367,22365);
	AddControl(m_pEnter);
}

bool CEnterShanWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if (pControl == m_pEnter)
		{
			g_pGameControl->SEND_EnterShan();
			CloseWindow();
			return true;
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CEnterShanWnd::Draw()
{
	if(!m_bShow) return;

	//绘制背景
	DrawBkg();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22361,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 38,m_iScreenY + 30,pTex);
	}

	// 绘制所有子控件
	CControlContainer::Draw();

	g_pFont->DrawText(m_iScreenX + 167,m_iScreenY + 430,"传说中三圣王之一孟虎在西域奇境北部的圣殿山封印了一种至强而又危险的力量。为了防止",0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
	g_pFont->DrawText(m_iScreenX + 167,m_iScreenY + 447,"这种力量被滥用，孟虎特地派遣座下一支“风虎卫”镇守圣殿山，后人便称这支军队为“圣",0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
	g_pFont->DrawText(m_iScreenX + 167,m_iScreenY + 464,"殿守卫军”。只有真正通过“圣殿守卫军”的封锁进入圣殿山的人，才配拥有那种力量！",0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);

}