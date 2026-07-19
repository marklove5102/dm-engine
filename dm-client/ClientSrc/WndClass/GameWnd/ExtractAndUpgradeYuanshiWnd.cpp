#include "ExtractAndUpgradeYuanshiWnd.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/ParserTip.h"
#include "baseclass/Control/GoodGrid.h"
#include "GameData/NpcData.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MagicDefine.h"


INIT_WND_POSX(CExtractAndUpgradeYuanshiWnd,POS_AUTO,POS_AUTO)

#define FrameSpeed 3
#define TotalFrame (FrameSpeed * 60)

CExtractAndUpgradeYuanshiWnd::CExtractAndUpgradeYuanshiWnd(void)
{
	m_iIndex = 17683;
	m_iAlignType = XAL_TOPLEFT;
	m_pOkBtn = NULL;
	g_NPC.GetYuanShiResult().iResult = -1;
	m_iFrame = 10000;
	m_iResultSuccessEffFrame = 10000;
	m_iResultFailEffFrame = 10000;
	m_bFirstClick = false;	

	m_strSkillLevel = "未学会拆卸技能";
	CMagicData* pMagicData = SELF.GetProduceMagicEx(MAGICID_DISASSEMBLE);
	if (pMagicData)
	{
		string astr[8] = {"初级","中级","高级","专家","大师","宗师","权威","王者"};
		int iLevel = pMagicData->GetMagicLevel();
		if (iLevel < 0) iLevel = 0;
		if (iLevel > 7) iLevel = 7;
		m_strSkillLevel = astr[iLevel];
	}
}

CExtractAndUpgradeYuanshiWnd::~CExtractAndUpgradeYuanshiWnd(void)
{
	SELF.PackageGood().BackToArray(g_NPC.GetYuanShiResult().m_Good);
	g_NPC.GetYuanShiResult().m_Good.SetID(0);
}

void CExtractAndUpgradeYuanshiWnd::OnCreate()
{
	//SetCloseButton(570,0,16101);

	SELF.PackageGood().BackToArray(g_NPC.GetYuanShiResult().m_Good);
	g_NPC.GetYuanShiResult().m_Good.SetID(0);
	g_NPC.GetYuanShiResult().iResult = -1;

	SetCloseButton(498,13,80);

	m_pOkBtn = new CCtrlButton;
	AddControl(m_pOkBtn);


	m_pOkBtn->CreateEx(this,37,300,95,96,97,98);
	m_pOkBtn->SetText("拆 卸",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE);	
	m_pOkBtn->SetFont(FONT_YAHEI,12);

	m_pMarkViewer = new CMarkViewer(16,4);
	AddControl(m_pMarkViewer);

	m_pMarkViewer->Create(this,115,262,388,79);
	m_pMarkViewer->SetFont(FONT_YAHEI,12);
	m_pMarkViewer->SetTagText(&g_NPC.GetYuanShiResult().tagText);	

}

void CExtractAndUpgradeYuanshiWnd::Draw()
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17685,EP_UI);
	if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 24,m_iScreenY + 46,pTex);

	g_pFont->DrawText(m_iScreenX + 244,m_iScreenY + 17,"拆  卸",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);

	CGoodGrid::DrawOneGood(m_iScreenX + 66,m_iScreenY + 264,g_NPC.GetYuanShiResult().m_Good);

	m_pMarkViewer->SetShow(false);

	if(g_NPC.GetYuanShiResult().iResult == 3)  //success
	{
		g_pFont->DrawText(m_iScreenX + 100 + 37,m_iScreenY + 248,"恭喜你成功从装备中拆卸出了材料。",0xffffff00,FONT_YAHEI);		
	}
	else if(g_NPC.GetYuanShiResult().iResult == 4)		//fail
	{
		g_pFont->DrawText(m_iScreenX + 100 + 37,m_iScreenY + 248,"本次装备拆卸失败。",0xffffff00,FONT_YAHEI);		
	}
	else if(g_NPC.GetYuanShiResult().m_Good.GetID() == 0 && m_iResultFailEffFrame >= 45 && m_iResultSuccessEffFrame >= 30)
	{
		g_pFont->DrawText(m_iScreenX + 80 + 37,m_iScreenY + 253,"请放入需要拆卸的装备，拆卸失败不会损失装备",0xffffff00,FONT_YAHEI);
		g_pFont->DrawText(m_iScreenX + 80 + 37,m_iScreenY + 270,"拆卸等级越高，拆卸出材料的数量就越多，品质就越好。拆卸需要消耗",0xffffff00,FONT_YAHEI);
		g_pFont->DrawText(m_iScreenX + 80 + 37,m_iScreenY + 286,"金币，每拆卸一次需要消耗金币数量为1000。",0xffffff00,FONT_YAHEI);
	}
	else if(g_NPC.GetYuanShiResult().iResult == 0)		//不可以操作
	{
		m_pMarkViewer->SetShow(true);		
	}
	else if(g_NPC.GetYuanShiResult().iResult == 1)	//可以操作
	{
		m_pMarkViewer->SetShow(true);
		
		char str[256] = "";
		int iLianHuaLevel = 0;
		CMagicData* pMagic = SELF.ProduceMagicArray().FindMagic(MAGICID_DISASSEMBLE);
		if (pMagic)
		{
			iLianHuaLevel = pMagic->GetMagicLevel();
		}

		sprintf(str,"您当前的拆卸等级为%s级，拆卸后，有概率获得以下材料：",m_strSkillLevel.c_str());
		g_pFont->DrawText(m_iScreenX + 115,m_iScreenY + 240,str,0xffffff00,FONT_YAHEI);		
	}
	

	if(m_iFrame < TotalFrame)
	{
		if(m_iFrame == 13 * FrameSpeed || m_iFrame == 21 * FrameSpeed || m_iFrame == 29 * FrameSpeed || m_iFrame == 37 * FrameSpeed || m_iFrame == 47 * FrameSpeed)
		{
			g_pAudio->Play(EAT_OTHER,915,g_pAudio->GetRand()++);
		}

		if(m_iFrame == 47 * FrameSpeed)
		{
			if(g_NPC.GetYuanShiResult().iResult == 1 || (g_NPC.GetYuanShiResult().iResult == 4 && g_NPC.GetYuanShiResult().m_Good.GetID()))
			{
				g_NPC.GetYuanShiResult().iResult = -1;	
				
				if(g_NPC.GetYuanShiResult().m_Good.GetID())
				{
					g_pGameControl->SEND_DISASSEMBLE_GOODS(g_NPC.GetYuanShiResult().m_Good.GetID(),false);
				}					
				
			}

			m_bFirstClick = false;
		}


		if(m_iFrame >= 8 * FrameSpeed && m_iFrame <= 47 * FrameSpeed)
		{	
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17337,EP_UI);
			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame((m_iFrame / 4) % 12);
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 310,m_iScreenY + 25,pTex);
				g_pGfx->SetRenderMode();
			}
		}

		if(m_iFrame >= 47 * FrameSpeed - 32 && m_iFrame < 47 * FrameSpeed)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17338,EP_UI);
			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(((m_iFrame - 47 * FrameSpeed + 32) / 2));
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY + 200,pTex);
				g_pGfx->SetRenderMode();				
			}
		}

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17336,EP_UI);
		if(pTex)
		{
			pTex->EnableSysAnim(false);
			pTex->SetCurFrame(m_iFrame / FrameSpeed);
			g_pGfx->DrawTextureNL(m_iScreenX - 53,m_iScreenY - 30,pTex);
			m_iFrame++;
		}
	}
	else
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17335,EP_UI);	
		if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 272,m_iScreenY + 36,pTex);
	}

	if(m_iResultSuccessEffFrame < 30)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17339,EP_UI);	

		if(pTex)
		{
			pTex->EnableSysAnim(false);
			pTex->SetCurFrame(m_iResultSuccessEffFrame / 3);
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX - 3,m_iScreenY + 195,pTex);
			g_pGfx->SetRenderMode();	
			m_iResultSuccessEffFrame++;
		}

		if(m_iResultSuccessEffFrame == 1)
		{
			g_pAudio->Play(EAT_OTHER,916,g_pAudio->GetRand()++);
		}
		
	}
	else if(m_iResultFailEffFrame < 45)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17340,EP_UI);

		if(pTex)
		{
			pTex->EnableSysAnim(false);
			pTex->SetCurFrame(m_iResultFailEffFrame / 3);
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX - 3,m_iScreenY + 195,pTex);
			g_pGfx->SetRenderMode();	
			m_iResultFailEffFrame++;
		}	

		if(m_iResultFailEffFrame == 1)
		{
			g_pAudio->Play(EAT_OTHER,917,g_pAudio->GetRand()++);
		}
	}
	else
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17341,EP_UI);
		g_pGfx->SetRenderMode(RM_ADD2);
		g_pGfx->DrawTextureNL(m_iScreenX + 33,m_iScreenY + 233,pTex);
		g_pGfx->SetRenderMode();	
	}
}

bool CExtractAndUpgradeYuanshiWnd::OnLeftButtonUp( int iX, int iY )
{
	if(iX > 50 && iX < 80 && iY > 245 && iY < 280)	
	{
		if(CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd)
		{
			g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
			return true;
		}

		g_NPC.GetYuanShiResult().iResult = -1;

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();
		CGood temp = g_NPC.GetYuanShiResult().m_Good;
		
		g_NPC.GetYuanShiResult().m_Good = GoodFrom.DropGood;
		GoodFrom.DropGood = temp;	

		if(g_NPC.GetYuanShiResult().m_Good.GetID())
		{
			g_pGameControl->SEND_DISASSEMBLE_GOODS(g_NPC.GetYuanShiResult().m_Good.GetID(),true);
		}

		return true;		
		
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CExtractAndUpgradeYuanshiWnd::OnMouseMove( int iX, int iY )
{
	if(iX > 50 && iX < 80 && iY > 245 && iY < 280)
	{
		CParserTip *pTip = g_pControl->GetTipWnd();
		pTip->Clear();
		if(g_NPC.GetYuanShiResult().m_Good.GetID())
		{				
			pTip->Parse(g_NPC.GetYuanShiResult().m_Good);			
		}
		else
		{
			pTip->AddText("请放入你需要拆卸的装备");			
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CExtractAndUpgradeYuanshiWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /*= NULL*/ )
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOkBtn)
			{
				if(m_bFirstClick)
				{
					m_bFirstClick = false;
					m_iFrame = 47 * FrameSpeed;					
				}
				else if(g_NPC.GetYuanShiResult().iResult == 1 || (g_NPC.GetYuanShiResult().iResult == 4 && g_NPC.GetYuanShiResult().m_Good.GetID()))
				{
					if(g_NPC.GetYuanShiResult().bExpensive)
					{
						g_MsgBoxMgr.PopSimpleComfirm("此装备为高级装备，你确定要拆卸吗？",MSG_CTRL_DISASSEMBLE_WEAPON,0);
					}
					else
					{
						m_iFrame = 0;
						m_bFirstClick = true;
					}
				}
				
				return true;
			}
		}
		break;	
	case MSG_CTRL_EXTRACTUPGRADE_YUANSHI:
		{
			if(dwData == OPER_CUSTOM)	//成功
			{
				m_iResultSuccessEffFrame = 0;
				return true;
			}
			else if(dwData == OPER_CUSTOM + 1)	//失败
			{
				m_iResultFailEffFrame = 0;
				return true;
			}
			else if(dwData == OPER_CUSTOM + 2)	//确认拆卸贵重物品
			{
				m_iFrame = 0;
				m_bFirstClick = true;
				return true;
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}