#include "Commoncondensewnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/Good.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/ConfigData.h"
#include "GameData/TalkMgr.h"

#define COMMON_CONDENSE_TYPE 5 // 协议定为类型5

INIT_WND_POSX(CCommonCondenseWnd,POS_AUTO,POS_AUTO)

CCommonCondenseWnd::CCommonCondenseWnd()
{
	//    +6+
	// +3+    +0+
	// +4+    +1+
	// +5+    +3+ 
	//    +7+

	m_rcCommonGood[0].left = 35  +4;
	m_rcCommonGood[0].top  = 123 +3;
	m_rcCommonGood[1].left = 59  +4;
	m_rcCommonGood[1].top  = 184 +3;
	m_rcCommonGood[2].left = 184 +4;
	m_rcCommonGood[2].top  = 186 +3;


	m_rcCommonGood[3].left = 60  +4;
	m_rcCommonGood[3].top  = 62  +3;
	m_rcCommonGood[4].left = 209 +4;
	m_rcCommonGood[4].top  = 124 +3;
	m_rcCommonGood[5].left = 184 +4;
	m_rcCommonGood[5].top  = 63  +3;



	m_rcCommonGood[6].left = 122 +4;//幸运符
	m_rcCommonGood[6].top  = 37  +3;
	m_rcCommonGood[7].left = 121 +4;//宝石
	m_rcCommonGood[7].top  = 211 +3;


	for(int i = 0;i<MAX_COMMON_GOOD_COUNT;i++)
	{
		m_rcCommonGood[i].right = m_rcCommonGood[i].left+48;
		m_rcCommonGood[i].bottom = m_rcCommonGood[i].top+48;
	}

	m_bIsCondensing = false;
	m_iMouseOnGood = -1;
	m_iMouseType = 0;
	m_iFrame = 0;
	m_bEnableUse = true;
	m_bIsStart = false;

	m_iType  = sm_dwWindowType;
	m_iIndex = 0;
	if (1 == m_iType)
		m_iIndex = 4955;

	m_iPages = 1;
	

}

CCommonCondenseWnd::~CCommonCondenseWnd(void)
{
	BackToPackage();
}

void CCommonCondenseWnd::OnCreate()
{
	m_pStartButton = new CCtrlButton();
	AddControl(m_pStartButton);
	if(m_iType == 1)
	{
		m_pStartButton->CreateEx(this,113,112,11040,11041,11042);
	}
	SetCloseButton(263,40);

	g_NPC.SetIsCondensing(false);
	SELF.SetReserveData(plyJumpMap,0);//设置跨地图无效的标志

	SetMask(10901);
}

//单击关闭按钮
void CCommonCondenseWnd::OnClickCloseButton()
{
	if(g_NPC.IsCondensing())
		return;
	CloseWindow();
}

void CCommonCondenseWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	for(int i = 0;i<MAX_COMMON_GOOD_COUNT;i++)
	{
		CGood& temp = g_NPC.GetGemGood(i);
		if (temp.GetID() == 0)
			continue;

		int x = m_iScreenX+m_rcCommonGood[i].left+4;
		int y = m_iScreenY+m_rcCommonGood[i].top+4;

		if(g_pControl) //绘制单个物品
		{
			CGoodGrid::DrawOneGood(x+16,y+16,temp); 
		}
	}

	//绘制掩盖图表明是否可以放物品
	if (m_iMouseOnGood >=0 && m_iMouseOnGood < MAX_COMMON_GOOD_COUNT)
	{
		LPTexture pTex = NULL;
		if (m_iMouseType == 1)//鼠标上的物品为不可放置的物品
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4672,EP_UI);
		}
		else if(m_iMouseType == 2)
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4673,EP_UI);
		if(pTex)
			g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[m_iMouseOnGood].left-3,m_iScreenY+m_rcCommonGood[m_iMouseOnGood].top-3,pTex,0x40FFFFFF);
	}


	//开始凝练特效
	static int iFrameCounter = 0;
	int iFrameNow = g_NPC.GetCondenseFrame();
	if(iFrameNow>=0 && g_NPC.IsCondensing() && m_bIsStart)
	{
		if( iFrameNow == 0 )
		{
			for(int _i=0;_i<32;_i++)
			{
				g_pTexMgr->GetTex(PACKAGE_INTERFACE,11044+_i,EP_UI);
			}

		}

		if(iFrameNow <= 30)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11044+iFrameNow,EP_UI);
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(m_iScreenX-108,m_iScreenY-110,pTex);
				g_pGfx->SetRenderMode();
			}
		}


		iFrameCounter++;
		if(iFrameCounter == 6)
		{
			iFrameCounter=0;
			g_NPC.SetCondenseFrame(++iFrameNow);
		}

		if(iFrameNow==30)
		{
			m_bIsStart = false;
			g_NPC.SetIsCondensing(false);
			g_NPC.SetCondenseFrame(-1);
			g_pGameControl->SEND_Exchange_Gem_Ok( COMMON_CONDENSE_TYPE ,false);
			m_dwLifeTime = GetTickCount() + 800;
			return;
		}
	}
}

bool CCommonCondenseWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pStartButton)
			{
				if(g_NPC.IsCondensing())
					return true;

				bool bIsReady = true;
				int  iCheckCount = 0;
				if (m_iType == 1 )
					iCheckCount = 6;

				for (int i=0;i<iCheckCount;i++)
				{
					if (g_NPC.GetGemGood(i).GetID()==0)
					{
						bIsReady = false;
						break;
					}
				}
				if (bIsReady)
				{
					m_bIsStart = true;
					g_NPC.SetIsCondensing(true);
					g_pGameControl->SEND_Exchange_Gem_Ok( COMMON_CONDENSE_TYPE ,true);
				}
				else
				{
					g_TalkMgr.PushSysTalk("装备数目不正确，不能合成");
				}
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CCommonCondenseWnd::OnLeftButtonDown(int iX, int iY)
{
	int iCommonGoodIndex = GetCommonGoodIndex(iX,iY);
	if (iCommonGoodIndex >= 0)
	{
		return true;
	}
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CCommonCondenseWnd::OnLeftButtonUp(int iX, int iY)
{
	if(!m_bEnableUse )
		return true;
	int iCommonGoodIndex = GetCommonGoodIndex(iX,iY);
	if (iCommonGoodIndex>=0)
	{
		if(g_NPC.IsCondensing())
			return true;

		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		CGood temp = g_NPC.GetGemGood(iCommonGoodIndex);

		if (GoodFrom.DropGood.GetID() != 0)
		{
			if (GoodFrom.eFromWnd == Package_Wnd ||
				GoodFrom.eFromWnd == Panel_Wnd || GoodFrom.eFromWnd == Gem_Wnd)
			{
				//限制放物品
				int iStdMode = GoodFrom.DropGood.GetStdMode();
				int iShape   = GoodFrom.DropGood.GetShape();
				if(m_iType == 1)
				{
					if ( iCommonGoodIndex == 7)//这个位置放宝石
					{
						if (iStdMode != 59 && iStdMode != 60)
							return true;
					}
					else if( iCommonGoodIndex ==6) //这个位置放幸运符
					{
						if(iStdMode == 44 && (iShape == 3 || iShape == 4 ))
						{
						}
						else
						{
							return true;		
						}

					}
					else if(iCommonGoodIndex <6)
					{
						if( (iStdMode == 44 && iShape == 3)  || 
							(iStdMode == 44 && iShape == 4) || 
							iStdMode == 5  ||
							iStdMode == 6  ||
							iStdMode == 45 ||
							iStdMode == 59 || 
							iStdMode == 60   )
							return true;
					}

				}

				g_NPC.GetGemGood(iCommonGoodIndex) = GoodFrom.DropGood;

				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				if(GoodFrom.DropGood.GetID() != 0)
				{
					g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
				}
			}
			else
				return true;
		}
		else
		{
			g_NPC.GetGemGood(iCommonGoodIndex).SetID(0);
		}

		//取出托盘上的东西
		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = Gem_Wnd;
		GoodFrom.iWndPos = iCommonGoodIndex;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

int CCommonCondenseWnd::GetCommonGoodIndex(int iX,int iY)
{
	for (int i=0;i<MAX_COMMON_GOOD_COUNT;i++)
	{
		if (iX>=m_rcCommonGood[i].left && iX<=m_rcCommonGood[i].right && iY>=m_rcCommonGood[i].top && iY<=m_rcCommonGood[i].bottom)
			return i;
	}
	return -1;

}

bool CCommonCondenseWnd::OnMouseMove(int iX, int iY)
{
	CGood temp;
	CParserTip * pTip = g_pControl->GetTipWnd();
	int iCommonGoodIndex = GetCommonGoodIndex(iX,iY);
	if (iCommonGoodIndex>=0)
	{
		pTip->Clear();
		//显示限制类物品
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if (GoodFrom.DropGood.GetID() !=0)
		{
			int iStdMode = GoodFrom.DropGood.GetStdMode();
			int iShape  = GoodFrom.DropGood.GetShape();
			m_iMouseType = 2;//可以放置

			if (m_iType == 1 && iCommonGoodIndex == 7)//这个位置放宝石
			{
				if (iStdMode != 59 && iStdMode != 60)
					m_iMouseType = 1;
			}
			else if(m_iType == 1 && iCommonGoodIndex ==6) //这个位置放幸运符
			{
				if(iStdMode == 44 && (iShape == 3 || iShape == 4 ))
				{
				}
				else
				{
					m_iMouseType = 1;		
				}
			}
			else if(m_iType == 1 && iCommonGoodIndex<6)
			{
				if( (iStdMode == 44 && iShape == 3)  || 
					(iStdMode == 44 && iShape == 4) || 
					iStdMode == 5  ||
					iStdMode == 6  ||
					iStdMode == 45 ||
					iStdMode == 59 || 
					iStdMode == 60   )
					m_iMouseType = 1;
			}
			if(!m_bEnableUse)
				m_iMouseType = 1;
			m_iMouseOnGood = iCommonGoodIndex;
		}
		else
		{
			m_iMouseOnGood = -1;
		}

		temp = g_NPC.GetGemGood(iCommonGoodIndex);

		if(temp.GetID()!=0)
		{
			m_iMouseOnGood = -1;
			pTip->Parse(temp);

			int x = m_iScreenX + iX;
			int y = m_iScreenY + iY;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
		else
		{
			if (m_iType == 1 && iCommonGoodIndex == 7)//这个位置放宝石
			{
				pTip->SetText("  放置宝石  ");
			}
			else if(m_iType == 1 && iCommonGoodIndex ==6) //这个位置放幸运符
			{
				pTip->SetText("  放置幸运符  ");	
			}
			else if(m_iType == 1 && iCommonGoodIndex<6)
			{
				pTip->SetText("  放置首饰、衣服、矿石  ");
			}

			int x = m_iScreenX + iX;
			int y = m_iScreenY + iY;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}
	else
	{
		m_iMouseOnGood = -1;

		temp.SetID(0);
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void  CCommonCondenseWnd::SetEnableUse(bool b)
{
	m_bEnableUse = b;
}

bool  CCommonCondenseWnd::GetEnableUse()
{
	return m_bEnableUse;
}

void CCommonCondenseWnd::BackToPackage()
{
	//放回包裹中
	for(int i =0;i<MAX_COMMON_GOOD_COUNT;i++)
	{
		CGood tGem = g_NPC.GetGemGood(i);
		if (tGem.GetID() == 0)
			continue;

		for (int ip = 0;ip < MAX_PACKAGE_ELEMENT;ip++)
		{
			if (SELF.GetPackageGood(ip).GetID() == 0)
			{
				SELF.GetPackageGood(ip) = tGem;
				break;
			}
		}
		g_NPC.GetGemGood(i).SetID(0);
	}

	g_NPC.SetIsCondensing(false);
	g_NPC.SetCondenseFrame(-1);
}
