#include "GuildFlagSelWnd.h"
#include "GameData/GuildData.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CGuildFlagSelWnd,POS_AUTO,POS_AUTO)

CGuildFlagSelWnd::CGuildFlagSelWnd(void)
{
	m_iIndex = 16280;
	m_dwPos = 0x80000000;	
}

void CGuildFlagSelWnd::OnCreate()
{
	m_pOKBtn = new CCtrlButton();
	AddControl(m_pOKBtn); 
	m_pOKBtn->CreateEx(this,98,301,95,96,97);
	m_pOKBtn->SetText("确 定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12);

	m_pCancelBtn = new CCtrlButton();
	AddControl(m_pCancelBtn); 
	m_pCancelBtn->CreateEx(this,293,301,95,96,97);
	m_pCancelBtn->SetText("取 消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12);


	m_pFlag1 = new CCtrlRadio();
	AddControl(m_pFlag1);
	m_pFlag1->Create(this,101,161,0,16144);

	m_pFlag2 = new CCtrlRadio();
	AddControl(m_pFlag2);
	m_pFlag2->Create(this,185,161,0,16144);

	m_pFlag3 = new CCtrlRadio();
	AddControl(m_pFlag3);
	m_pFlag3->Create(this,262,161,0,16144);

	m_pFlag4 = new CCtrlRadio();
	AddControl(m_pFlag4);
	m_pFlag4->Create(this,346,161,0,16144);

	m_pFlag1->AddBuddy(m_pFlag2);
	m_pFlag1->AddBuddy(m_pFlag3);
	m_pFlag1->AddBuddy(m_pFlag4);	

	m_pFlag1->SetTextAlignType(XAL_CENTER);
	m_pFlag2->SetTextAlignType(XAL_CENTER);
	m_pFlag3->SetTextAlignType(XAL_CENTER);
	m_pFlag4->SetTextAlignType(XAL_CENTER);


	if(g_GuildData.GetGuildFlag() <= 0 || g_GuildData.GetGuildFlag() >= 5)
	{
		m_pFlag1->SetChecked(TRUE);
	}
	else
	{
		m_pFlag1->SetRadio(g_GuildData.GetGuildFlag() - 1);
	}

	m_pGoLeftButton = new CCtrlButton();
	AddControl(m_pGoLeftButton);
	m_pGoLeftButton->CreateEx(this,33,79,16283,16283,16284);

	m_pGoRightButton = new CCtrlButton();
	AddControl(m_pGoRightButton);
	m_pGoRightButton->CreateEx(this,406,79,16281,16281,16282);

	//SetMask(16280);
	SetCloseButton(427,3,80);
}

CGuildFlagSelWnd::~CGuildFlagSelWnd(void)
{
}

void CGuildFlagSelWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 193,m_iScreenY + 10,"行会旗帜",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);


	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16150 + m_dwPos % 4,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 60,m_iScreenY + 53,pTex);
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16150 + (m_dwPos + 1) % 4,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 60 + 88,m_iScreenY + 53,pTex);
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16150 + (m_dwPos + 2) % 4,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 60 + 88 * 2,m_iScreenY + 53,pTex);
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16150 + (m_dwPos + 3) % 4,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 60 + 88 * 3,m_iScreenY + 53,pTex);

	g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 202,"请选择你行会的标志，每个行会只有一次免费选择的机会。",0xffffff00);
	g_pFont->DrawText(m_iScreenX + 39,m_iScreenY + 220,"再次修改旗帜会消耗行会宝石资源20点。",0xffffff00);
}

bool CGuildFlagSelWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pCancelBtn)
		{
			OnClickCloseButton();
			return true;
		}	
		else if(pControl == m_pOKBtn)
		{
			int iResult = (m_dwPos + m_pFlag1->GetRadio()) % 4 + 1;
			if(!(iResult == g_GuildData.GetGuildFlag()))
			{
				g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-6),"@guildlogo",1,iResult);
			}
			CloseWindow();
			return true;
		}
		else if(pControl == m_pGoLeftButton)
		{
			m_dwPos--;
		}
		else if(pControl == m_pGoRightButton)
		{
			m_dwPos++;
		}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
