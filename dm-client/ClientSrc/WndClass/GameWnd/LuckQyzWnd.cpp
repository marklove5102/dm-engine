#include "LuckQyzWnd.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/TalkMgr.h"

INIT_WND_POSX(CLuckQyzWnd,POS_AUTO,POS_AUTO)


CLuckQyzWnd::CLuckQyzWnd(void)
{
	m_pOK = NULL;
	m_iPages = 1;
	m_iIndex = 20600;
	m_iLastCards = 3;
	m_iTurnedCards = 0;
	m_bLuckQYZDoubleChance = false;
	m_dwCompleteTime = 0;
	m_dwLastOpenTime = 0;

	m_nCardWidth = 37;
	m_nCardHeight = 38;
	m_rcCardArea.left = 49;
	m_rcCardArea.top = 55;
	m_rcCardArea.right = 49+6*m_nCardWidth;
	m_rcCardArea.bottom = 55+5*m_nCardHeight;

	for(int i=0; i<30; i++)
	{
		m_dwOpenTimes[i] = 0;
		m_bOpened[i] = false;
		m_nExps[i] = -1;
		m_bEffectBox[i] = false;
		m_bClicked[i] = false;
	}

	m_dwPushSysTalkTime[0] = 0;
	m_dwPushSysTalkTime[1] = 0;
	m_dwPushSysTalkTime[2] = 0;
}

CLuckQyzWnd::~CLuckQyzWnd(void)
{
}

void CLuckQyzWnd::OnCreate()
{
	m_iLastCards = 3;
	SetCloseButton(283,4, 80);
	m_pOK = new CCtrlButton();
	AddControl(m_pOK);
	m_pOK->CreateEx(this, 113, 335, 90, 91, 92, 93);
	m_pOK->SetText("完成抽奖", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
}

void CLuckQyzWnd::OnClickCloseButton()
{
	if(m_iLastCards>0)
	{
		g_MsgBoxMgr.PopOkCancelBox("您今天还有没用完的抽奖机会，关闭本窗口后今天将没有机会再使用，确定要关闭吗？", MSG_CTRL_LUCKQYZ_CLOSEOK, 0, this, MSG_CTRL_LUCKQYZ_CLOSECANCEL, 0, this, true);
	}
	else
	{

		if (m_bLuckQYZDoubleChance)//有两次机会
		{
			g_MsgBoxMgr.PopOkCancelBox("今天为星期一，受“普天同庆”的影响，您还可以再进行一次抽奖。是否继续？", MSG_CTRL_LUCJQYZ_CONTINUE, 0, this, MSG_CTRL_LUCKQYZ_CLOSEOK, 0, this, true);
		}

		CCtrlWindowX::OnClickCloseButton();
	}
}

bool CLuckQyzWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOK)
			{
				OnClickCloseButton();
				return true;
			}
		}
		break;
	case MSG_INPUT_LEFTBT_UP:
		{
			POINT pt;
			pt.x = LOWORD(dwData) - m_iScreenX;
			pt.y = HIWORD(dwData) - m_iScreenY;
			if(PtInRect(&m_rcCardArea, pt))
			{
				if(m_iLastCards>0)
				{
					int iOffX = pt.x - m_rcCardArea.left, iOffY = pt.y - m_rcCardArea.top;
					if((iOffX%m_nCardWidth) < 32 && (iOffY%m_nCardHeight) <32)
					{
						BYTE CardIndex = iOffX/m_nCardWidth + (iOffY/m_nCardHeight)*6;
						if(!m_bClicked[CardIndex])
						{
							g_pGameControl->Send_LuckQYYZ_Select(CardIndex);
							m_iLastCards--;
							m_bClicked[CardIndex] = true;
						}
					}
				}
				return true;
			}
		}
		break;
	case MSG_CTRL_LUCKQYZ_PRIZE:
		{
			if(m_iTurnedCards<3)
				TurnCard(dwData, *(DWORD*)pControl);
			else
				m_nExps[dwData] = *(DWORD*)pControl;
			return true;
		}
	case MSG_CTRL_LUCKQYZ_CHANCE:
		{
			m_bLuckQYZDoubleChance = true;
			return true;
		}
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CLuckQyzWnd::TurnCard(DWORD nCard, DWORD nExp)
{
	if(nCard>29 || nCard<0)
		return;
	m_nExps[nCard] = nExp;
	m_bOpened[nCard] = true;
	m_iTurnedCards++;
	m_dwOpenTimes[nCard] = m_dwLastOpenTime = GetTickCount();
	if(m_iTurnedCards == 3)
	{
		m_dwCompleteTime = GetTickCount();
	}

	if (m_iTurnedCards < 4 && nExp == 0)
	{
		m_dwPushSysTalkTime[m_iTurnedCards-1] = GetTickCount();
	}
}

void CLuckQyzWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int nWindowX=GetScreenX(), nWindowY=GetScreenY();
	g_pFont->DrawText(118+nWindowX, 11+nWindowY, "幸运珍宝阁", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
	g_pFont->DrawText(38+nWindowX, 263+nWindowY, "每人每天可以通过“幸运珍宝阁”进行一次抽", 0xFFA79F79, FONT_YAHEI);
	g_pFont->DrawText(38+nWindowX, 278+nWindowY, "奖，每次抽奖都可以选取三格打开。", 0xFFA79F79, FONT_YAHEI);
	g_pFont->DrawText(222+nWindowX, 278+nWindowY, "（星期一", 0xFFFFFF00, FONT_YAHEI);
	g_pFont->DrawText(38+nWindowX, 294+nWindowY, "受普天同庆影响，每人可额外抽奖一次）", 0xFFFFFF00, FONT_YAHEI);

	DWORD dwNow = GetTickCount();

	g_pGfx->SetRenderMode(RM_ADD2);

	LPTexture pTexEffect, pTexExpEffect, pTexLock, pTexExp, pTexNothing;
	pTexExpEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 16296,EP_UI);
	pTexLock = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 20601,EP_UI);
	pTexExp = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 20602,EP_UI);
	pTexNothing = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 20604,EP_UI);

	for (int j=0; j<30; j++)
	{
		if (m_bEffectBox[j])
		{
			if((dwNow - m_dwEffectMoveTime[j])>1600)
			{
				m_bEffectBox[j] = false;
			}
		}
		else
		{
			m_bEffectBox[j] = (rand()%200 == 1);
			if(m_bEffectBox[j])
			{
				m_dwEffectMoveTime[j] = dwNow;
			}
		}
	}

	if(m_iTurnedCards<4)
	{
		for (int i=0; i<30; i++)
		{
			if (!m_bOpened[i] && m_bEffectBox[i])
			{
				pTexEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16287+(dwNow - m_dwEffectMoveTime[i])/200,EP_UI);
				g_pGfx->DrawTextureNL((i%6)*m_nCardWidth+m_rcCardArea.left-16+m_iScreenX, (i/6)*m_nCardHeight+m_rcCardArea.top-16+m_iScreenY, pTexEffect);
			}
		}
	}

	for(int i=0; i<30; i++)
	{
		int x = (i%6)*m_nCardWidth+m_rcCardArea.left;
		int y = (i/6)*m_nCardHeight+m_rcCardArea.top;
		if(m_bOpened[i])
		{
			if((dwNow-m_dwOpenTimes[i]) < 800)
			{
				pTexEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16287+(dwNow-m_dwOpenTimes[i])/100,EP_UI);
				g_pGfx->DrawTextureNL(x-16+m_iScreenX, y-16+m_iScreenY, pTexEffect);
				g_pGfx->DrawTextureNL(x+m_iScreenX, y+m_iScreenY, pTexLock);
			}
			else
			{
				if(m_nExps[i] > 0)
				{
					g_pGfx->DrawTextureNL(x-46+m_iScreenX, y-47+m_iScreenY, pTexExpEffect);
					g_pGfx->DrawTextureNL(x+1+m_iScreenX, y+m_iScreenY, pTexExp);
				}
				else
				{
					g_pGfx->DrawTextureNL(x-3+m_iScreenX, y-2+m_iScreenY, pTexNothing);
				}
			}
		}
		else
		{
			g_pGfx->DrawTextureNL(x+m_iScreenX, y+m_iScreenY, pTexLock);
		}


	}

	g_pGfx->SetRenderMode();

	for(int i=0; i<30; i++)
	{
		int x = (i%6)*m_nCardWidth+m_rcCardArea.left - 7;
		int y = (i/6)*m_nCardHeight+m_rcCardArea.top - 7;

		if (m_bClicked[i])
		{
			DrawTexture(x, y, 20605);
		}
	}

	if(m_dwCompleteTime !=0 )
	{
		if((GetTickCount()-m_dwCompleteTime) > 2000)
		{
			for (DWORD i=0; i<30; i++)
			{
				if(!m_bOpened[i])
					TurnCard(i, m_nExps[i]);
			}
			m_dwCompleteTime = 0;
		}
	}

	for (int i=0 ;i<3; i++)
	{
		if (m_dwPushSysTalkTime[i] != 0 && (dwNow - m_dwPushSysTalkTime[i]) > 800)
		{
			g_TalkMgr.PushSysTalk("这次好像不太幸运，什么也没有");
			m_dwPushSysTalkTime[i] = 0;
		}
	}
}

bool CLuckQyzWnd::OnMouseMove(int iX, int iY)
{
	TRY_BEGIN
		CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	if (iX>48 && iX<267 && iY> 55 && iY<245)
	{
		pTip->Clear();
		pTip->SetShow(false);

		RECT rc;
		POINT pt = {iX, iY};
		char str[30];

		for(int i=0;i<30;i++)
		{
			if(m_nExps[i]>0 && m_bOpened[i])
			{
				rc.left = (i%6)*m_nCardWidth+49;
				rc.top = (i/6)*m_nCardHeight+55;
				rc.right = rc.left+32;
				rc.bottom = rc.top+32;

				if(PtInRect(&rc, pt))
				{
					sprintf_s(str, 30, "%d经验值", m_nExps[i]);
					pTip->AddText(str);
					int x = m_iScreenX + iX + 10;
					int y = m_iScreenY + iY + 10;
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
					return true;
				}
			}
		}

	}
	else
	{
		pTip->SetShow(false);
	}
	TRY_END

		return CCtrlWindowX::OnMouseMove(iX,iY);
}