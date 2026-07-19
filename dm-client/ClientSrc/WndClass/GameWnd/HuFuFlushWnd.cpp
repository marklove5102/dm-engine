#include "HuFuFlushWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/TipsCfg.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/GameGlobal.h"
#include "GameData/TalkMgr.h"


const POINT CHuFuFlushWnd::m_ptGridPos[EHuFuFlushGridNum] = {	
	{ 27, 58 },		// EFBG_HufuFlush_Hufu
	{ 316, 58 },	// EFBG_HufuFlush_AddGood	
};


INIT_WND_POSX(CHuFuFlushWnd,POS_AUTO,POS_AUTO)

CHuFuFlushWnd::CHuFuFlushWnd(void)
{
	m_pBaoliuBtn = NULL;
	m_pShuaxinBtn = NULL;

	m_iIndex = 23030;	

	m_bShowRet = false;
	memset(&m_kRet, 0, sizeof(SChiJinRetPacket));
	m_iSuccessFrame = 10000;

	g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22315,22331,EP_UI);
}

CHuFuFlushWnd::~CHuFuFlushWnd(void)
{
	BackToPackage();	
}

void CHuFuFlushWnd::OnCreate()
{
	SetCloseButton(584,4,80);

	m_pBaoliuBtn = new CCtrlButton;
	AddControl(m_pBaoliuBtn);
	m_pBaoliuBtn->CreateEx(this,366,288,95,96,97,98);	
	m_pBaoliuBtn->SetText("保 留");
	m_pBaoliuBtn->SetEnable(false);

	m_pShuaxinBtn = new CCtrlButton;
	AddControl(m_pShuaxinBtn);
	m_pShuaxinBtn->CreateEx(this,490,288,95,96,97,98);
	m_pShuaxinBtn->SetText("刷 新");
}

void CHuFuFlushWnd::Draw()
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}


	int gridstart = -1;
	int gridend = -1;
	GetGridStartEnd(gridstart, gridend);

	if (gridstart != -1 && gridend != -1)
	{
		for (int i = gridstart; i <= gridend; ++i)
		{
			//g_pGfx->DrawFillRect(m_iScreenX + m_ptGridPos[i].x,m_iScreenY + m_ptGridPos[i].y,33,34,0x8000ff00);
			CGood* pGood = GetGoodByGrid(i);
			if(pGood && pGood->GetID())
			{
				CGoodGrid::DrawOneGood(m_iScreenX + m_ptGridPos[i].x + 17,m_iScreenY + m_ptGridPos[i].y + 17,*pGood);
			}
		}
	}

	g_pFont->DrawText(m_iScreenX + 310,m_iScreenY + 10,"赤金护符刷新",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
	g_pFont->DrawText(m_iScreenX + 88,m_iScreenY + 65,"请在左边放入赤金护符",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + 380,m_iScreenY + 65,"请在左边放入赤金结晶",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	char str[128] = {0};
	CGood* pHufuGood = GetGoodByGrid(EFBG_HufuFlush_Hufu);
	if (g_AIGoodMgr.IsChiLianHuFu(*pHufuGood))
	{
		int propnum = 0;
		int colnum = 5;
		int rowju = 25;
		DWORD dwColor = 0xff5aa0a0;
		DWORD dwNormalColor = 0xff5aa0a0;
		DWORD dwUpColor = 0xff00ff00;
		DWORD dwDownColor = 0xffff0000;
		DWORD dwPerfectColor = 0xFFF8C993;
		int fontsize = FONTSIZE_MIDDLE;
		int posx = 48;
		int posy = 130;		

		//int iTotalDC = pHufuGood->GetDC();
		int iTotalDC2 = pHufuGood->GetDC2();

		if(iTotalDC2 != 0 )
		{
			sprintf(str,"攻击+%d",iTotalDC2);
			if (iTotalDC2 == 10)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}
			++propnum;
		}

		//int iTotalMC = pHufuGood->GetMC();
		int iTotalMC2 = pHufuGood->GetMC2();
		if(iTotalMC2 != 0 )
		{
			sprintf(str,"魔法+%d",iTotalMC2);

			if (iTotalMC2 == 10)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}
			++propnum;
		}

		//int iTotalSC = pHufuGood->GetSC();
		int iTotalSC2 = pHufuGood->GetSC2();
		if(iTotalSC2 != 0 )
		{
			sprintf(str,"道术+%d",iTotalSC2);
			if (iTotalSC2 == 10)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}
			++propnum;
		}

		//int iTotalAC = pHufuGood->GetAC();
		int iTotalAC2 = pHufuGood->GetAC2();
		if(iTotalAC2 != 0 )
		{
			sprintf(str,"防御+%d",iTotalAC2);

			if (iTotalAC2 == 10)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}

			++propnum;
		}

		//int iTotalMAC = pHufuGood->GetMAC();
		int iTotalMAC2 = pHufuGood->GetMAC2();
		if(iTotalMAC2 != 0 )
		{
			sprintf(str,"魔防+%d",iTotalMAC2);

			if (iTotalMAC2 == 10)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}
			++propnum;
		}


		if (pHufuGood->GetChiLianBaoji() != 0)
		{
			sprintf(str,"暴击+%d",pHufuGood->GetChiLianBaoji());

			if (pHufuGood->GetChiLianBaoji() == 20)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}

			++propnum;
		}

		if (pHufuGood->GetChiLianPoFang() != 0)
		{
			sprintf(str,"破防+%d",pHufuGood->GetChiLianPoFang());

			if (pHufuGood->GetChiLianPoFang() == 20)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}

			++propnum;
		}

		if (pHufuGood->GetChiLianJueFang() != 0)
		{
			sprintf(str,"抵抗+%d",pHufuGood->GetChiLianJueFang());

			if (pHufuGood->GetChiLianJueFang() == 10)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}
			++propnum;
		}

		if (pHufuGood->GetChiLianXiXie() != 0)
		{
			sprintf(str,"吸血+%d",pHufuGood->GetChiLianXiXie());

			if (pHufuGood->GetChiLianXiXie() == 20)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}
			++propnum;
		}

		if (pHufuGood->GetChiLianFanTan() != 0)
		{
			sprintf(str,"反弹+%d",pHufuGood->GetChiLianFanTan());

			if (pHufuGood->GetChiLianFanTan() == 20)
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwPerfectColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			} 
			else
			{
				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);

			}
			++propnum;
		}


		if (m_bShowRet)
		{
			posx = 340;
			posy = 130;
			propnum = 0;

			//int iTotalDC = m_kRet.byDC1;
			int iTotalDC2 = m_kRet.byDC2;

			if(iTotalDC2 != 0 || 0 != pHufuGood->GetDC2())
			{
				if (iTotalDC2 > pHufuGood->GetDC2())
				{
					dwColor = dwUpColor;
					sprintf(str,"攻击+%d ↑",iTotalDC2);
				}
				else if (iTotalDC2 < pHufuGood->GetDC2())
				{
					dwColor = dwDownColor;
					sprintf(str,"攻击+%d ↓",iTotalDC2);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"攻击+%d",iTotalDC2);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			//int iTotalMC = m_kRet.byMC1;
			int iTotalMC2 = m_kRet.byMC2;
			if(0 != pHufuGood->GetMC2() || iTotalMC2 != 0 )
			{
				if (iTotalMC2 > pHufuGood->GetMC2())
				{
					dwColor = dwUpColor;
					sprintf(str,"魔法+%d ↑",iTotalMC2);
				}
				else if (iTotalMC2 < pHufuGood->GetMC2())
				{
					dwColor = dwDownColor;
					sprintf(str,"魔法+%d ↓",iTotalMC2);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"魔法+%d",iTotalMC2);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			//int iTotalSC = m_kRet.bySC1;
			int iTotalSC2 = m_kRet.bySC2;
			if(0 != pHufuGood->GetSC2() || iTotalSC2 != 0 )
			{
				if (iTotalSC2 > pHufuGood->GetSC2())
				{
					dwColor = dwUpColor;
					sprintf(str,"道术+%d ↑",iTotalSC2);
				}
				else if (iTotalSC2 < pHufuGood->GetSC2())
				{
					dwColor = dwDownColor;
					sprintf(str,"道术+%d ↓",iTotalSC2);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"道术+%d",iTotalSC2);
				}


				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			//int iTotalAC = m_kRet.byAC1;
			int iTotalAC2 = m_kRet.byAC2;
			if(pHufuGood->GetAC2() != 0 ||  iTotalAC2 != 0 )
			{
				if (iTotalAC2 > pHufuGood->GetAC2())
				{
					dwColor = dwUpColor;
					sprintf(str,"防御+%d ↑",iTotalAC2);
				}
				else if (iTotalAC2 < pHufuGood->GetAC2())
				{
					dwColor = dwDownColor;
					sprintf(str,"防御+%d ↓",iTotalAC2);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"防御+%d",iTotalAC2);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			//int iTotalMAC = m_kRet.byMAC1;
			int iTotalMAC2 = m_kRet.byMAC2;
			if(pHufuGood->GetMAC2() != 0 || iTotalMAC2 != 0 )
			{
				if (iTotalMAC2 > pHufuGood->GetMAC2())
				{
					dwColor = dwUpColor;
					sprintf(str,"魔防+%d ↑",iTotalMAC2);
				}
				else if (iTotalMAC2 < pHufuGood->GetMAC2())
				{
					dwColor = dwDownColor;
					sprintf(str,"魔防+%d ↓",iTotalMAC2);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"魔防+%d",iTotalMAC2);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}


			if (m_kRet.byBaoJi != 0 || pHufuGood->GetChiLianBaoji() != 0)
			{
				if (m_kRet.byBaoJi > pHufuGood->GetChiLianBaoji())
				{
					dwColor = dwUpColor;
					sprintf(str,"暴击+%d ↑",m_kRet.byBaoJi);
				}
				else if (m_kRet.byBaoJi < pHufuGood->GetChiLianBaoji())
				{
					dwColor = dwDownColor;
					sprintf(str,"暴击+%d ↓",m_kRet.byBaoJi);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"暴击+%d",m_kRet.byBaoJi);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			if (m_kRet.byPoFang != 0 || pHufuGood->GetChiLianPoFang() != 0)
			{
				if (m_kRet.byPoFang > pHufuGood->GetChiLianPoFang())
				{
					dwColor = dwUpColor;
					sprintf(str,"破防+%d ↑",m_kRet.byPoFang);
				}
				else if (m_kRet.byPoFang < pHufuGood->GetChiLianPoFang())
				{
					dwColor = dwDownColor;
					sprintf(str,"破防+%d ↓",m_kRet.byPoFang);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"破防+%d",m_kRet.byPoFang);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			if (m_kRet.byJueFang != 0 || pHufuGood->GetChiLianJueFang() != 0)
			{
				if (m_kRet.byJueFang > pHufuGood->GetChiLianJueFang())
				{
					dwColor = dwUpColor;
					sprintf(str,"抵抗+%d ↑",m_kRet.byJueFang);
				}
				else if (m_kRet.byJueFang < pHufuGood->GetChiLianJueFang())
				{
					dwColor = dwDownColor;
					sprintf(str,"抵抗+%d ↓",m_kRet.byJueFang);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"抵抗+%d",m_kRet.byJueFang);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			if (m_kRet.byXiXie != 0 || pHufuGood->GetChiLianXiXie() != 0)
			{
				if (m_kRet.byXiXie > pHufuGood->GetChiLianXiXie())
				{
					dwColor = dwUpColor;
					sprintf(str,"吸血+%d ↑",m_kRet.byXiXie);
				}
				else if (m_kRet.byXiXie < pHufuGood->GetChiLianXiXie())
				{
					dwColor = dwDownColor;
					sprintf(str,"吸血+%d ↓",m_kRet.byXiXie);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"吸血+%d",m_kRet.byXiXie);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

			if (m_kRet.byFanTan != 0 || pHufuGood->GetChiLianFanTan() != 0)
			{
				if (m_kRet.byFanTan > pHufuGood->GetChiLianFanTan())
				{
					dwColor = dwUpColor;
					sprintf(str,"反弹+%d ↑",m_kRet.byFanTan);
				}
				else if (m_kRet.byFanTan < pHufuGood->GetChiLianFanTan())
				{
					dwColor = dwDownColor;
					sprintf(str,"反弹+%d ↓",m_kRet.byFanTan);
				}
				else
				{
					dwColor = dwNormalColor;
					sprintf(str,"反弹+%d",m_kRet.byFanTan);
				}

				g_pFont->DrawText(m_iScreenX + posx + (propnum/colnum) * 125,m_iScreenY + posy + (propnum%colnum) * rowju,str,dwColor,FONT_YAHEI,fontsize,DTF_BlackFrame);
				++propnum;
			}

		}

		//属性总量限制
		if (pHufuGood->GetDC2() + pHufuGood->GetMC2() + pHufuGood->GetSC2() + pHufuGood->GetAC2() + pHufuGood->GetMAC2() + pHufuGood->GetChiLianBaoji() + pHufuGood->GetChiLianPoFang() + pHufuGood->GetChiLianJueFang() + pHufuGood->GetChiLianXiXie() + pHufuGood->GetChiLianFanTan() >= 50)
		{
			g_pFont->DrawText(m_iScreenX + 30,m_iScreenY + 282,"你的赤金护符属性总量已达上限,继续刷新可调整",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 30,m_iScreenY + 296,"已有属性.",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}

	}

	ShowEffect();
}

void CHuFuFlushWnd::ShowEffect()
{
	LPTexture pTex = NULL;
	if (m_iSuccessFrame < 64)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22315+m_iSuccessFrame / 4,EP_UI);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX -101,m_iScreenY -86,pTex);
			g_pGfx->SetRenderMode();
		}

		++m_iSuccessFrame;
	}
}

bool CHuFuFlushWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */ )
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pBaoliuBtn)
			{
				CGood* pHufuGood = GetGoodByGrid(EFBG_HufuFlush_Hufu);				
				if (pHufuGood && (pHufuGood->GetID() != 0))
				{
					g_pGameControl->SEND_HufuFlush(2,pHufuGood->GetID(),0);
					m_bShowRet = false;
					if (m_pBaoliuBtn)
						m_pBaoliuBtn->SetEnable(false);
				}

				return true;
			}
			else if(pControl == m_pShuaxinBtn)
			{
				CGood* pHufuGood = GetGoodByGrid(EFBG_HufuFlush_Hufu);
				CGood* pAddGood = GetGoodByGrid(EFBG_HufuFlush_AddGood);
				if (pHufuGood && (pHufuGood->GetID() == 0))
				{
					g_TalkMgr.PushSysTalk("请放入赤金护符");
					return true;
				}

				if (pAddGood && (pAddGood->GetID() == 0))
				{
					g_TalkMgr.PushSysTalk("请放入赤金结晶");
					return true;
				}

				g_pGameControl->SEND_HufuFlush(1,pHufuGood->GetID(),pAddGood->GetID());
				return true;
			}
		}
		break;
	case MSG_CTRL_HUFUFLUSH_WND:
		{
			if (dwData == 10)//刷新
			{
				m_bShowRet = true;
				m_kRet = *(SChiJinRetPacket*)pControl;

				if (m_pBaoliuBtn)
					m_pBaoliuBtn->SetEnable(true);
			}
			else if (dwData == 11)
			{
				m_iSuccessFrame = 0;
			}
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CHuFuFlushWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);

	int iGoodOnGrid = -1;

	CGood * pGood = GetGoodOnGrid(iX,iY,iGoodOnGrid);

	stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

	if(pGood && pGood->GetID())
	{
		pTip->Parse(*pGood);

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if (iGoodOnGrid >= 0)
	{
		switch (iGoodOnGrid)
		{
		case EFBG_HufuFlush_Hufu:
			pTip->AddText("请放入赤金护符");
			break;
		case EFBG_HufuFlush_AddGood:
			pTip->AddText("请放入赤金结晶");
			break;		
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

bool CHuFuFlushWnd::OnLeftButtonUp( int iX, int iY )
{
	int iGoodOnGrid = -1;
	CGood * pGood = GetGoodOnGrid(iX,iY,iGoodOnGrid);

	if (pGood)
	{
		if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() && CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd)
		{
			CGoodGrid::ReleaseDrop();
			g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() && !CheckValidGoodOnGrid(iGoodOnGrid,GoodFrom.DropGood))
			return true;

		CGood temp = *pGood;
		*pGood = GoodFrom.DropGood;
		GoodFrom.DropGood = temp;

		if (iGoodOnGrid == EFBG_HufuFlush_Hufu)
		{
			m_bShowRet = false;
			if (m_pBaoliuBtn)
				m_pBaoliuBtn->SetEnable(false);
		}

		return true;
	}	

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}

bool CHuFuFlushWnd::CheckValidGoodOnGrid(int grid,CGood& good)
{
	if (grid == EFBG_HufuFlush_Hufu)
	{
		if (!g_AIGoodMgr.IsChiLianHuFu(good))
			return false;
	}
	return true;
}

void CHuFuFlushWnd::BackToPackage()
{
	for(int i = 0;i < MAX_HUFU_GOOD_COUNT;++i)
	{
		if (g_NPC.GetHufuGood(i).GetID() != 0)
		{
			SELF.PackageGood().BackToArray(g_NPC.GetHufuGood(i));
			g_NPC.GetHufuGood(i).SetID(0);
		}
	}
}

bool CHuFuFlushWnd::IsInGrid(int iX,int iY, int gridx, int gridy)
{
	if(iX > gridx && iX < gridx + 36 && iY > gridy && iY < gridy + 36)
		return true;

	return false;
}

bool CHuFuFlushWnd::IsInGrid(int iX,int iY, int grid)
{
	if (grid >= 0 && grid < EHuFuFlushGridNum)
	{
		return IsInGrid(iX, iY, m_ptGridPos[grid].x, m_ptGridPos[grid].y);
	}
	return false;
}


void CHuFuFlushWnd::GetGridStartEnd(int& gridstart, int& gridend)
{
	gridstart = -1;
	gridend = -1;

	gridstart = EFBG_HufuFlush_Hufu;
	gridend = EFBG_HufuFlush_AddGood;
}

CGood* CHuFuFlushWnd::GetGoodOnGrid(int iX,int iY,int& grid)
{
	grid = -1;

	int gridstart = -1;
	int gridend = -1;

	GetGridStartEnd(gridstart, gridend);

	if (gridstart != -1 && gridend != -1)
	{
		for (int i = gridstart; i <= gridend; ++i)
		{
			if (IsInGrid(iX,iY,i))
			{
				grid = i;
				return GetGoodByGrid(grid);
			}
		}
	}

	return NULL;
}

CGood* CHuFuFlushWnd::GetGoodByGrid(int grid)
{
	return &g_NPC.GetHufuGood(grid);
}