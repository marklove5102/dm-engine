#include "EquipLevelUpWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/ConfigData.h"
#include "GameData/TalkMgr.h"
#include "GameData/NpcData.h"
#include "GameAI/AIGoodMgr.h"

#define WEAPON_LEVEL_UP_TYPE 7 // 协议定为类型7 请求装备升级
#define CLOTH_LEVEL_UP_TYPE 9 // 协议定为类型9 请求升级衣服
#define SPRITEBALL_LEVEL_UP_TYPE 10 // 协议定为类型10 请求升级魂珠

INIT_WND_POSX(CEquipLevelUpWnd,POS_VARIABLE,POS_VARIABLE)

CEquipLevelUpWnd::CEquipLevelUpWnd()
{
	//    +0+
	//+5+     +1+
	//    +6+
	//+4+     +2+ 
	//    +3+

	m_rcCommonGood[0].left = 115; 
	m_rcCommonGood[0].top  = 63;
	m_rcCommonGood[1].left = 174;
	m_rcCommonGood[1].top  = 98;
	m_rcCommonGood[2].left = 176;
	m_rcCommonGood[2].top  = 168;
	m_rcCommonGood[3].left = 114;
	m_rcCommonGood[3].top  = 202;
	m_rcCommonGood[4].left = 53;
	m_rcCommonGood[4].top  = 168;
	m_rcCommonGood[5].left = 56;
	m_rcCommonGood[5].top  = 97;
	m_rcCommonGood[6].left = 132;
	m_rcCommonGood[6].top  = 142;

	for(int i = 0;i<MAX_LEVELUP_COUNT;i++)
	{
		m_rcCommonGood[i].right = m_rcCommonGood[i].left+38;
		m_rcCommonGood[i].bottom = m_rcCommonGood[i].top+38;
	}

	m_bIsCondensing = false;
	m_iMouseOnGood = -1;
	m_iMouseType = 0;
	m_iFrame = 0;
	m_bEnableUse = true;
	m_bIsStart = false;
	m_bEndEff = false;
	m_iResulEffType = 0;
	m_bGoodOn = false;
	m_iEffFrameSpeed = 0;
	m_iEffFrameNow = 0 ;
	m_iShowLionType = 0;
	m_iLionFadeCurFram = 0;
	m_iFrameCount2 = 0;
	dwFrequent = GetTickCount();
	iLightPos0_Elp = 0;
	iLightPos1_Elp = 0;
	iLightPos2_Elp = 0;
	iLightPos3_Elp = 0;
	lastMusicRand = 0;

	for(int i = 0 ; i < MAX_LEVELUP_COUNT-1 ; i++)
		bLionFrash_Elp[i] = false;


	m_iLevelUpType  = sm_dwWindowType;
	m_iPages = 1;

	//6淬炼7拆解
	if(m_iLevelUpType == 6 || m_iLevelUpType == 7)
	{
		m_iIndex = 17307;
	}
	else if(m_iLevelUpType == 5)
	{
		m_iIndex = 16440;
	}
	else if(m_iLevelUpType == 4)
	{
		m_iIndex = 16030;
	}
	else
	{
		m_iIndex = 4674;
	}
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 120;

	g_pControl->SetWindowPos(g_pGfx->GetWidth() / 2 - 40,120,"PackageWnd");
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);

	m_bDrawLionHead[0] = 1;
	m_bDrawLionHead[1] = 1;
	m_bDrawLionHead[2] = 1;
	m_bDrawLionHead[3] = 1;
	m_bDrawLionHead[4] = 1;
	m_bDrawLionHead[5] = 1;

	m_iSelectedYuanShi = -1;
}

CEquipLevelUpWnd::~CEquipLevelUpWnd(void)
{
	BackToPackage();

	for(int i=0;i<MAX_LEVELUP_COUNT;i++)
	{
		g_NPC.GetEquLUpGood(i).SetID(0);
	}
	g_NPC.SetIsLUpintEqu(false);
}

void CEquipLevelUpWnd::OnCreate()
{
	m_pStartButton = new CCtrlButton();
	AddControl(m_pStartButton);
	if(m_iLevelUpType == 6)
		m_pStartButton->CreateEx(this,264,76,17304,17305,17306);
	else if(m_iLevelUpType == 7)
		m_pStartButton->CreateEx(this,264,76,17301,17302,17303);
	else if(m_iLevelUpType == 5)
		m_pStartButton->CreateEx(this,264,76,16442,16443,16444);
	else if(m_iLevelUpType == 4)
		m_pStartButton->CreateEx(this,264,76,16031,16032,16033);
	else
		m_pStartButton->CreateEx(this,264,76,4675,4676,4677);

	g_NPC.SetIsLUpintEqu(false);
	SELF.SetReserveData(plyJumpMap,0);//设置跨地图无效的标志

	if(m_iLevelUpType == 5)
	{
		SetMask(16440);
	}
	else
	{
		SetMask(4662);
	}

	SetCloseButton(327,49);
}

//单击关闭按钮
void CEquipLevelUpWnd::OnClickCloseButton()
{
	if(g_NPC.IsLUpintEqu())
		return;

	CloseWindow();
}

void CEquipLevelUpWnd::Draw(void)
{
	m_dwTickCount = GetTickCount();

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//绘制狮子
	DrawGoods();

	//绘制掩盖图表明是否可以放物品
	if (m_iMouseOnGood >=0 && m_iMouseOnGood < MAX_LEVELUP_COUNT)
	{
		LPTexture pTex = NULL;
		if (m_iMouseType == 1)//鼠标上的物品为不可放置的物品
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4672,EP_UI);
		}
		else if(m_iMouseType == 2)
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4673,EP_UI);

		if( pTex)
		{
			if(m_iMouseOnGood == 6)
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[m_iMouseOnGood].left-3,m_iScreenY+m_rcCommonGood[m_iMouseOnGood].top-3,pTex,0x40FFFFFF);
			else
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[m_iMouseOnGood].left+13,m_iScreenY+m_rcCommonGood[m_iMouseOnGood].top+8,pTex,0x40FFFFFF);
		}
	}

	//升级结果特效
	if(m_iResulEffType && m_iEffFrameNow >=0)
	{
		if( m_iEffFrameNow == 0 )
		{
			if (m_iResulEffType == 1)
			{
				for(int _i=0;_i<24;_i++)
				{
					g_pTexMgr->GetTex(PACKAGE_INTERFACE,12725+_i,EP_MAGIC);
				}
			}

			if (m_iResulEffType == 2)
			{
				for(int _i=0;_i<24;_i++)
				{
					g_pTexMgr->GetTex(PACKAGE_INTERFACE,12750+_i,EP_MAGIC);
					g_pTexMgr->GetTex(PACKAGE_INTERFACE,12775+_i,EP_MAGIC);
				}
			}
		}

		if(m_iEffFrameNow <= 23)
		{
			LPTexture pTex = NULL;
			LPTexture pTex2 = NULL;

			if (m_iResulEffType == 1)
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12725+m_iEffFrameNow,EP_MAGIC);
			if (m_iResulEffType == 2)
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12750+m_iEffFrameNow,EP_MAGIC);
				pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12775+m_iEffFrameNow,EP_MAGIC);
			}
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(m_iScreenX-80,m_iScreenY-80,pTex);
				g_pGfx->SetRenderMode();
			}
			if(pTex2)
			{
				g_pGfx->DrawTextureNL(m_iScreenX-80,m_iScreenY-80,pTex2);
			}
		}
		m_iEffFrameSpeed++;
		if(m_iEffFrameSpeed == 6)
		{
			m_iEffFrameSpeed=0;
			m_iEffFrameNow++;
		}
		if(m_iEffFrameNow==23)
		{
			m_iEffFrameNow =  0;
			m_iResulEffType = 0;
		}
	}
	if(m_bGoodOn)
	{
		LPTexture pTex = NULL;
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12622,EP_UI);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX-80,m_iScreenY-80,pTex,0xFFFFFFFF);
			g_pGfx->SetRenderMode();
		}
	}
}

bool CEquipLevelUpWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pStartButton)
			{
				bool bIsReady = true;
				if (g_NPC.GetEquLUpGood(6).GetID()==0)
				{
					bIsReady = false;
				}
				else if(m_iLevelUpType != 5 && m_iLevelUpType != 6 && m_iLevelUpType != 7 && g_NPC.GetEquLUpGood(6).GetDemonDark1() == 0)
				{
					g_TalkMgr.PushSysTalk("没有黑暗魔力,不可升级");
					return true;
				}
				if (bIsReady)
				{
					if(m_bIsStart)
						m_bEndEff = true;

					m_bIsStart = true;

					if( m_bEndEff && m_iShowLionType == 3)//第二次点击并且第一的测试成功了,如果第一次点击后的测试成功了而没有再第二次点击该按钮那么到物资播放完后会自动发协议
					{
						m_iFrameCount2 = 500;
						m_bIsStart = false;
						m_bEndEff  = false;
						m_bEnableUse = false;
						g_NPC.SetIsLUpintEqu(true);
						g_NPC.SetLUpFrame(-1);
						//第二次开始请求升级!
						if(m_iLevelUpType == 6)
						{
							g_pGameControl->Send_Extract_Gem();
						}
						else if(m_iLevelUpType == 7)
						{
							g_pGameControl->Send_Remove_Gem(g_NPC.GetEquLUpGood(6).GetID(),(BYTE)g_NPC.GetEquLUpGood(m_iSelectedYuanShi).GetID());
						}
						else if(m_iLevelUpType == 5)
							g_pGameControl->SEND_Exchange_Gem_Ok(SPRITEBALL_LEVEL_UP_TYPE ,false);
						else if(m_iLevelUpType == 4)
							g_pGameControl->SEND_Exchange_Gem_Ok(CLOTH_LEVEL_UP_TYPE ,false);
						else
							g_pGameControl->SEND_Exchange_Gem_Ok(WEAPON_LEVEL_UP_TYPE ,false);

						m_pStartButton->SetEnable(false);
						if (lastMusicRand > 0)
						{
							g_pAudio->Stop(EAT_OTHER,200,lastMusicRand);
							lastMusicRand = 0;
						}

					}
					else
					{
						m_iEffFrameNow =  0;
						m_iResulEffType = 0;
						//第一次开始播放特效!
						//向服务器发送请求预先判断
						if(m_iLevelUpType == 6 || m_iLevelUpType == 7)
						{
							if(m_iLevelUpType == 7 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(0) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(1) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(2) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(3) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(4) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(5) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(6) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(7) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(8) == 0 &&
								g_NPC.GetEquLUpGood(6).GetResvEx(9) == 0)
							{
								g_MsgBoxMgr.PopSimpleAlert("该宝石并未镶嵌原石");
								return true;
							}

							if(m_iLevelUpType == 7 && m_iSelectedYuanShi == -1)
							{
								g_MsgBoxMgr.PopSimpleAlert("请选择你要拆卸的原石");
								return true;
							}
							else if(m_iLevelUpType == 6 
								&& g_NPC.GetEquLUpGood(0).GetID() == 0 
								&& g_NPC.GetEquLUpGood(1).GetID() == 0 
								&& g_NPC.GetEquLUpGood(2).GetID() == 0 
								&& g_NPC.GetEquLUpGood(3).GetID() == 0 
								&& g_NPC.GetEquLUpGood(4).GetID() == 0)
							{
								g_MsgBoxMgr.PopSimpleAlert("请放入原石");
								return true;
							}

							g_NPC.SetIsLUpintEqu(true);
							g_NPC.SetCondenseFrame(0);
							g_pAudio->Play(EAT_OTHER,200,g_pAudio->GetRand()++);							
						}
						else if(m_iLevelUpType == 5)
							g_pGameControl->SEND_Exchange_Gem_Ok( SPRITEBALL_LEVEL_UP_TYPE ,true);
						else if(m_iLevelUpType == 4)
							g_pGameControl->SEND_Exchange_Gem_Ok( CLOTH_LEVEL_UP_TYPE ,true);
						else
							g_pGameControl->SEND_Exchange_Gem_Ok( WEAPON_LEVEL_UP_TYPE ,true);
					}
				}
				else
				{
					if(m_iLevelUpType == 7)
					{
						g_TalkMgr.PushSysTalk("请放入需要拆解的宝石");
					}
					else if(m_iLevelUpType == 6)
					{
						g_TalkMgr.PushSysTalk("请放入需要淬炼的宝石");
					}
					else if(m_iLevelUpType == 5)
					{
						g_TalkMgr.PushSysTalk("请放入魂珠");
					}
					else
					{
						g_TalkMgr.PushSysTalk("请放入装备");
					}
				}
				return true;
			}
		}
		break;
	case MSG_CTRL_EQUIP_LEVEL_UP_WND:
		{
			if (dwData == 4 )
			{
				SetReslutEffType(1);
				SetEnableUse(true);
				//弹出提示框 升级成功!
				if(m_iLevelUpType == 5)
					g_MsgBoxMgr.PopSimpleAlert("恭喜您，您的魂珠修炼成功！\n");
				else if(m_iLevelUpType == 4)
					g_MsgBoxMgr.PopSimpleAlert("恭喜您，您的衣服充分炼化了黑暗魔力，升级成功！\n");
				else
					g_MsgBoxMgr.PopSimpleAlert("恭喜您，您的武器充分炼化了黑暗魔力，升级成功！\n");

				return true;
			}
			else if (dwData == 6 || dwData == 5 )
			{
				SetReslutEffType(2);
				SetEnableUse(true);
				return true;
			}
			else if(dwData == 10)	//成功
			{
				m_iResulEffType = 1;
				m_bIsStart = false;
				m_iSelectedYuanShi = -1;

				m_bEnableUse = true;
				g_NPC.SetIsLUpintEqu(false);				
				m_pStartButton->SetEnable(true);
				m_bIsStart = false;
				m_bEndEff  = false;
				m_iShowLionType = 2 ;	
				m_iFrameCount2 = 0;

				g_pAudio->Play(EAT_OTHER,201,g_pAudio->GetRand()++);

				UpdateLionHead(g_NPC.GetEquLUpGood(6));
				return true;
			}
			else if(dwData == 11)
			{
				m_iResulEffType = 2;
				m_bIsStart = false;
				m_iSelectedYuanShi = -1;
				
				m_bEnableUse = true;
				g_NPC.SetIsLUpintEqu(false);				
				m_pStartButton->SetEnable(true);
				m_bIsStart = false;
				m_bEndEff  = false;
				m_iShowLionType = 2 ;
				m_iFrameCount2 = 0;

				g_pAudio->Play(EAT_OTHER,202,g_pAudio->GetRand()++);
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CEquipLevelUpWnd::OnLeftButtonDown(int iX, int iY)
{
	int iCommonGoodIndex = GetCommonGoodIndex(iX,iY);
	if (iCommonGoodIndex >= 0)
	{
		return true;
	}
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CEquipLevelUpWnd::OnLeftButtonUp(int iX, int iY)
{

	if(!m_bEnableUse )
		return true;

	int iCommonGoodIndex = GetCommonGoodIndex(iX,iY);
	if (iCommonGoodIndex >= 0)
	{
		if(m_iLevelUpType == 7 && iCommonGoodIndex != 6)
		{
			m_iSelectedYuanShi = iCommonGoodIndex;
			return true;
		}

		if(g_NPC.IsLUpintEqu())
			return true;

		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		CGood temp = g_NPC.GetEquLUpGood(iCommonGoodIndex);

		if (GoodFrom.DropGood.GetID() != 0)
		{
			if (GoodFrom.eFromWnd == Package_Wnd ||
				GoodFrom.eFromWnd == Panel_Wnd || GoodFrom.eFromWnd == Gem_Wnd)
			{

				int iStdMode = GoodFrom.DropGood.GetStdMode();
				int iShape   = GoodFrom.DropGood.GetShape();

				if ( iCommonGoodIndex == 6)//这个位置放武器
				{
					if(!CanLevelUp(iStdMode,iShape))
						return true;
					m_bGoodOn = true;
					m_iShowLionType = 1;

					UpdateLionHead(GoodFrom.DropGood);
				}
				else
				{
					if(m_iLevelUpType == 7)
					{
						return true;
					}
					else if(m_iLevelUpType == 6)
					{
						//放置原石
						if(!g_AIGoodMgr.IsYuanShi(GoodFrom.DropGood))
							return true;	

						if(g_NPC.GetEquLUpGood(6).GetResvEx(iShape - 225) != 0)
						{
							g_MsgBoxMgr.PopSimpleAlert("同属性原石无需重复淬炼");
							return true;
						}

						for(int i = 0;i < 6;i++)
						{
							if(g_NPC.GetEquLUpGood(i).GetID() && g_NPC.GetEquLUpGood(i).GetShape() == iShape)
							{
								g_MsgBoxMgr.PopSimpleAlert("您已经放入了相同属性的原石");
								return true;
							}
						}
					}
					else if(m_iLevelUpType == 5)
					{
						//可以放置
					}
					else
					{
						if (iStdMode != 59 && iStdMode != 60 && //宝石,
							!(iStdMode == 44 && (iShape == 3 || iShape == 4)) && //幸运符
							!(iStdMode == 43 && iShape == 0) //矿石
							)
							return true;
					}

					if(m_iShowLionType != 2)
						return true;
				}

				g_NPC.GetEquLUpGood(iCommonGoodIndex) = GoodFrom.DropGood;
				m_bIsStart = false;
				m_bEndEff = false;


				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				if(GoodFrom.DropGood.GetID() != 0)
				{
					g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
				}
			}else
				return true;
		}
		else
		{
			g_NPC.GetEquLUpGood(iCommonGoodIndex).SetID(0);
			if(iCommonGoodIndex == 6)
			{
				m_bGoodOn = false;
				m_iShowLionType = 0;
				m_iFrameCount2 = 0;
				m_iLionFadeCurFram = 0;
				BackToPackage(false);
			}
		}

		//取出托盘上的东西
		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = Gem_Wnd;
		GoodFrom.iWndPos = iCommonGoodIndex;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		m_bIsStart = false;
		m_bEndEff = false;

		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

int CEquipLevelUpWnd::GetCommonGoodIndex(int iX,int iY)
{

	int i = 0;
	for (i=0;i<MAX_LEVELUP_COUNT-1;i++)
	{
		if (iX>=m_rcCommonGood[i].left+15 && iX<=m_rcCommonGood[i].right+15 && iY>=m_rcCommonGood[i].top +5&& iY<=m_rcCommonGood[i].bottom+5)
		{
			if(i < 6 && m_iShowLionType != 2)
				return -1;
			else if(i < 6 && m_bDrawLionHead[i])	//画狮子头的地方无法选择
				return -1;
			else
				return i;
		}
	}

	if (iX >= m_rcCommonGood[MAX_LEVELUP_COUNT-1].left && iX<=m_rcCommonGood[i].right && iY>=m_rcCommonGood[i].top && iY<=m_rcCommonGood[i].bottom)
		return MAX_LEVELUP_COUNT-1;

	return -1;

}

bool CEquipLevelUpWnd::OnMouseMove(int iX, int iY)
{
	CGood temp;
	CParserTip *pTip = g_pControl->GetTipWnd();
	int iCommonGoodIndex = GetCommonGoodIndex(iX,iY);
	if (iCommonGoodIndex>=0)
	{
		//显示限制类物品
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if (GoodFrom.DropGood.GetID() !=0)
		{
			int iStdMode = GoodFrom.DropGood.GetStdMode();
			int iShape  = GoodFrom.DropGood.GetShape();
			m_iMouseType = 2;//可以放置

			if( iCommonGoodIndex == 6)
			{
				if( !CanLevelUp(iStdMode,iShape))
					m_iMouseType = 1;
			}
			else
			{
				if(m_iLevelUpType == 7)
				{
					m_iMouseType = 0;
				}
				else if(m_iLevelUpType == 6)
				{
					//放置原石
					if(!g_AIGoodMgr.IsYuanShi(GoodFrom.DropGood))
						m_iMouseType = 1;
				}
				else if(m_iLevelUpType == 5)
				{
					m_iMouseType = 2;//可以放置
				}
				else
				{
					if (iStdMode != 59 && iStdMode != 60 && //宝石,
						!(iStdMode == 44 && (iShape == 3 || iShape == 4)) && //幸运符
						!(iStdMode == 43 && iShape == 0)//矿石
						)
						m_iMouseType = 1;
				}

			}

			if(!m_bEnableUse)
				m_iMouseType = 1;//不能放置
			m_iMouseOnGood = iCommonGoodIndex;

		}
		else
		{
			m_iMouseOnGood = -1;
		}

		temp = g_NPC.GetEquLUpGood(iCommonGoodIndex);

		if(temp.GetID() != 0 && !(iCommonGoodIndex != 6 && m_iLevelUpType == 7))
		{
			pTip->Clear();
			pTip->Parse(temp);
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
		else
		{
			pTip->Clear();
			if(iCommonGoodIndex == 6 )
			{
				if(m_iLevelUpType == 7)
					pTip->AddText("请放入需要拆解的宝石");
				else if(m_iLevelUpType == 6)
					pTip->AddText("请放入需要淬炼的宝石");
				else if(m_iLevelUpType == 5)
					pTip->AddText("  放置魂珠  ");
				else if(m_iLevelUpType == 4)
					pTip->AddText("  放置衣服  ");
				else
					pTip->AddText("  放置武器  ");
			}
			else if(m_iShowLionType == 2)
			{
				if(m_iLevelUpType == 7)
					pTip->AddText("  选择需要拆解的原石  ",0xFFFFFFFF);
				else if(m_iLevelUpType == 6)
					pTip->AddText("  放置原石进行淬炼  ",0xFFFFFFFF);
				else if(m_iLevelUpType == 5)
					pTip->AddText("  放置首饰、灵皓石进行炼制  ",0xFFFFFFFF);
				else
					pTip->AddText("  放置矿石、宝石、幸运符  ");
			}

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
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

void  CEquipLevelUpWnd::SetEnableUse(bool b)
{
	m_bEnableUse = b;
	g_NPC.SetIsLUpintEqu(false);
	if(b)
	{
		m_pStartButton->SetEnable(true);
		m_bIsStart = false;
		m_bEndEff  = false;
		m_iShowLionType = 0 ;
		m_iFrameCount2 = 0;
		m_iLionFadeCurFram = 0;
		BackToPackage(false);
	}
}

bool  CEquipLevelUpWnd::GetEnableUse()
{
	return m_bEnableUse;
}

void CEquipLevelUpWnd::BackToPackage(bool backWeapon)
{
	//放回包裹中
	for(int i = 0;i<MAX_LEVELUP_COUNT;i++)
	{
		if(i != 6 && m_iLevelUpType == 7)	//拆解的原石是构造出来的
		{
			continue;
		}

		CGood& tEqu = g_NPC.GetEquLUpGood(i);
		if (tEqu.GetID() == 0)
			continue;

		if(!backWeapon && i == 6)
			continue;

		if(backWeapon && i == 6)
			m_bGoodOn = false;
		if(tEqu.GetStdMode() == 44 && tEqu.GetShape() ==4 && tEqu.GetDura()<=0)
		{
			g_NPC.GetEquLUpGood(i).SetID(0);
			continue;
		}

		for (int ip = 0;ip < MAX_PACKAGE_ELEMENT;ip++)
		{
			if (SELF.GetPackageGood(ip).GetID() == 0)
			{
				SELF.GetPackageGood(ip) = tEqu;
				break;
			}
		}
		g_NPC.GetEquLUpGood(i).SetID(0);
	}

	if(g_NPC.GetEquLUpGood(6).GetID() != 0)//武器没有放回包时，显示为托盘状态
		m_iShowLionType = 2;

	g_NPC.SetIsCondensing(false);
	g_NPC.SetLUpFrame(-1);
}

//iType = 0 
//@@equiplevel  只升武器
//@@jewitemlevel  只升项链、手镯、戒指
//@@defitemlevel  只升衣服、头盔、靴子、腰带
//@@hayifu 黑暗魔力升级衣服
//可以放的物品
/*
5 武器6 武器10衣服 11衣服 15头盔 19项链 20项链 21项链 22戒指 
23戒指 24手镯 26手镯58腰带59石头61天地石81靴子
13:魂珠
*/
bool CEquipLevelUpWnd:: CanLevelUp(int iStdmod,int iShape)
{
	if(m_iLevelUpType == 1)
	{
		if( iStdmod == 5 || iStdmod == 6  )
			return true;
	}
	else if(m_iLevelUpType == 2)
	{
		if( (iStdmod >= 19 && iStdmod <= 24 )|| iStdmod == 26 )
			return true;
	}
	else if(m_iLevelUpType == 3)
	{
		if(iStdmod == 10 ||	iStdmod == 11 || iStdmod == 15 || iStdmod == 58 || iStdmod == 81)
			return true;
	}
	else if(m_iLevelUpType == 4)
	{
		if(iStdmod == 10 ||	iStdmod == 11)
			return true;
	}
	else if(m_iLevelUpType == 5)
	{
		if(iStdmod == 13)
			return true;
	}
	else if(m_iLevelUpType == 6 || m_iLevelUpType == 7)
	{
		if(iStdmod == 59 && iShape < 10)
			return true;
	}

	return false;
}

void CEquipLevelUpWnd::DrawGoods()
{
	if(m_iShowLionType == 2 && g_NPC.IsLUpintEqu())
		m_iShowLionType = 3;

	if(m_iShowLionType == 0 || m_iShowLionType == 3)//画狮子头
	{
		LPTexture pTex = NULL;

		if(m_iLevelUpType == 6 || m_iLevelUpType == 7)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17308,EP_UI);

			for(int i = 0;i < MAX_LEVELUP_COUNT - 1;i++)
				g_pGfx->DrawTextureNL(m_iScreenX + m_rcCommonGood[i].left + 13,m_iScreenY + m_rcCommonGood[i].top + 6,pTex);
		}
		else
		{
			if(m_iLevelUpType == 5)
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16441,EP_UI);
			}
			else
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4678,EP_UI);
			}
			
			g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pTex);
		}
	}	

	if(m_iShowLionType == 1)//狮子淡出
	{
		if(++m_iLionFadeCurFram >= ELW_LION_FADE_FRAMES)
		{
			m_iShowLionType = 2;
		}

		DWORD dwcolor = ((DWORD)((float)(ELW_LION_FADE_FRAMES - m_iLionFadeCurFram)/ELW_LION_FADE_FRAMES * 255)) << 24 ; 
		dwcolor |= 0x00FFFFFF;

		if(m_iLevelUpType == 6 || m_iLevelUpType == 7)	//淬炼宝石时需要画狮子头
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17308,EP_UI);

			for(int i = 0;i < MAX_LEVELUP_COUNT - 1;i++)
			{
				DWORD tmp = dwcolor;
				if(m_bDrawLionHead[i] == 1)
				{
					tmp = 0xffffffff;
				}

				g_pGfx->DrawTextureNL(m_iScreenX + m_rcCommonGood[i].left + 13,m_iScreenY + m_rcCommonGood[i].top + 6,pTex,tmp);
			}
		}
		else
		{
			LPTexture pTex =g_pTexMgr->GetTex(PACKAGE_INTERFACE,4678,EP_UI);			
			g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pTex,dwcolor);
		}
	}

	int x ,y;

	for(int i = 0;i < MAX_LEVELUP_COUNT;i++)
	{
		if(i != 6)
		{
			if(m_iShowLionType == 2)//画托盘
			{
				if(m_iLevelUpType == 6 || m_iLevelUpType == 7)	//淬炼宝石时需要画狮子头
				{
					if(m_bDrawLionHead[i] == 1)
					{
						LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17308,EP_UI);					
						g_pGfx->DrawTextureNL(m_iScreenX + m_rcCommonGood[i].left + 13,m_iScreenY + m_rcCommonGood[i].top + 6,pTex);
						continue;
					}		

					if(m_iLevelUpType == 7 && i == m_iSelectedYuanShi)
					{
						LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,10915,EP_UI);
						g_pGfx->SetRenderMode(RM_ADD2);
						g_pGfx->DrawTextureNL(m_iScreenX + m_rcCommonGood[i].left - 26,m_iScreenY + m_rcCommonGood[i].top - 29,pTex);
						g_pGfx->SetRenderMode();
					}
				}

				if(m_iLevelUpType == 6 && m_bDrawLionHead[i] > 1)	//画原石
				{
					
					WORD dwLooks = m_bDrawLionHead[i];

					LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_items,dwLooks,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + m_rcCommonGood[i].left + 22,m_iScreenY + m_rcCommonGood[i].top + 14,pTex,0xff7f7f7f);					
				}
			}
			else if(m_iShowLionType == 0)//还没有放武器，画普通狮子,也就是原图
			{
				continue;
			}
			else if(m_iShowLionType == 3)//开始升级了，画眼睛发亮的狮子
			{
				continue;//开始升级后物品被狮子头覆盖不显示
			}
			else if(m_iShowLionType == 1)//狮子淡出
			{
				continue;
			}
		}

		CGood& temp = g_NPC.GetEquLUpGood(i);

		if (temp.GetID() )//画物品
		{
			if(i == 6)
			{
				x = m_iScreenX + m_rcCommonGood[i].left + 4;
				y = m_iScreenY + m_rcCommonGood[i].top + 4;
			}
			else
			{
				x = m_iScreenX + m_rcCommonGood[i].left+21;
				y = m_iScreenY + m_rcCommonGood[i].top+15;
			}

			if(g_pControl) //绘制单个物品
			{
				CGoodGrid::DrawOneGood(x+16,y+16,temp);
			}
		}
	}

	if(m_iShowLionType == 3)//绘制红色的圆环时间条及狮子闪光
	{
		if(m_iFrameCount2 <500)
		{
			if(m_iFrameCount2 == 0)
			{
				for(int _i=0;_i<24;_i++)
				{
					g_pTexMgr->GetTex(PACKAGE_INTERFACE,10920+_i,EP_UI);
				}
				for(int _i=0;_i<60;_i++)
				{
					g_pTexMgr->GetTex(PACKAGE_magic2,26020+_i,EP_UI);
				}
				for(int _i=0;_i<4;_i++)
				{
					g_pTexMgr->GetTex(PACKAGE_magic2,26001+_i,EP_UI);
				}
				for(int _i=0;_i<4;_i++)
				{
					g_pTexMgr->GetTex(PACKAGE_magic2,26010+_i,EP_UI);
				}

				dwFrequent = m_dwTickCount;
				iLightPos0_Elp = ( m_dwTickCount+rand() ) % 6;
				iLightPos1_Elp = ( m_dwTickCount+rand() ) % 6;
				iLightPos2_Elp = ( m_dwTickCount+rand() ) % 6;
				iLightPos3_Elp = ( m_dwTickCount+rand() ) % 6;

				for(int i = 0 ; i < MAX_LEVELUP_COUNT-1 ; i++)
					bLionFrash_Elp[i] = ( m_dwTickCount+rand() ) % 2 == 0?true:false;

			}
			LPTexture pTex =g_pTexMgr->GetTex(PACKAGE_INTERFACE,10920+m_iFrameCount2/22,EP_UI);
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(m_iScreenX-78 , m_iScreenY-107,pTex);
				g_pGfx->SetRenderMode();
			}

			if(m_iFrameCount2%100 == 0)
			{
				UINT uSoundID = 200;

				if(lastMusicRand != 0 )
					g_pAudio->Play(EAT_OTHER,uSoundID,lastMusicRand);
				else
				{
					lastMusicRand = g_pAudio->GetRand();
					g_pAudio->Play(EAT_OTHER,uSoundID,g_pAudio->GetRand()++);
				}
			}

			//处理狮子头闪光
			DWORD dwCount = m_dwTickCount;

			//设置物品闪烁位置的随机变量,最多4个，进度条越到后面越快
			if(m_dwTickCount - dwFrequent >1010 - m_iFrameCount2*2 )
			{
				dwFrequent = dwCount;
				iLightPos0_Elp = (dwCount+rand() ) % 6;
				iLightPos1_Elp = (dwCount+rand() ) % 6;
				iLightPos2_Elp = (dwCount+rand() ) % 6;
				iLightPos3_Elp = (dwCount+rand() ) % 6;

				for(int i = 0 ; i < MAX_LEVELUP_COUNT-1 ; i++)
					bLionFrash_Elp[i] = (dwCount+rand() ) % 2 == 0?true:false;
			}

			int itemp = m_iFrameCount2/10;
			int itemp2 = m_iFrameCount2/2;
			//闪光
			LPTexture pTex2 = g_pTexMgr->GetTex(PACKAGE_magic2,26001+itemp%4,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos0_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos0_Elp].top,pTex2);
			if(iLightPos1_Elp != iLightPos0_Elp)
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos1_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos1_Elp].top,pTex2);
			if(iLightPos2_Elp != iLightPos0_Elp && iLightPos2_Elp != iLightPos1_Elp)
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos2_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos2_Elp].top,pTex2);
			if(iLightPos3_Elp != iLightPos0_Elp && iLightPos3_Elp != iLightPos1_Elp && iLightPos3_Elp != iLightPos2_Elp)
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos3_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos3_Elp].top,pTex2);

			//闪光的add部分及狮子头在闪光时，有几率向武器发射光芒，几率为1/2。
			g_pGfx->SetRenderMode(RM_ADD1);
			pTex2 = g_pTexMgr->GetTex(PACKAGE_magic2,26010+itemp%4,EP_UI);
			LPTexture pTex3 = NULL;
			g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos0_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos0_Elp].top,pTex2);
			if(bLionFrash_Elp[iLightPos0_Elp])
			{
				pTex3 = g_pTexMgr->GetTex(PACKAGE_magic2,26020+iLightPos0_Elp*10+itemp2%10,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX-80,m_iScreenY-80,pTex3);
			}

			if(iLightPos1_Elp != iLightPos0_Elp)
			{
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos1_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos1_Elp].top,pTex2);
				if(bLionFrash_Elp[iLightPos1_Elp])
				{
					pTex3 = g_pTexMgr->GetTex(PACKAGE_magic2,26020+iLightPos1_Elp*10+itemp2%10,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX-80,m_iScreenY-80,pTex3);
				}
			}
			if(iLightPos2_Elp != iLightPos0_Elp && iLightPos2_Elp != iLightPos1_Elp)
			{
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos2_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos2_Elp].top,pTex2);
				if(bLionFrash_Elp[iLightPos2_Elp])
				{
					pTex3 = g_pTexMgr->GetTex(PACKAGE_magic2,26020+iLightPos2_Elp*10+itemp2%10,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX-80,m_iScreenY-80,pTex3);
				}

			}
			if(iLightPos3_Elp != iLightPos0_Elp && iLightPos3_Elp != iLightPos1_Elp && iLightPos3_Elp != iLightPos2_Elp)
			{
				g_pGfx->DrawTextureNL(m_iScreenX+m_rcCommonGood[iLightPos3_Elp].left,m_iScreenY+m_rcCommonGood[iLightPos3_Elp].top,pTex2);
				if(bLionFrash_Elp[iLightPos3_Elp])
				{
					pTex3 = g_pTexMgr->GetTex(PACKAGE_magic2,26020+iLightPos3_Elp*10+itemp2%10,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX-80,m_iScreenY-80,pTex3);
				}

			}

			g_pGfx->SetRenderMode();

			m_iFrameCount2++;
			if(m_iFrameCount2 == 500)
			{
				m_bIsStart = false;
				m_bEndEff  = false;
				m_bEnableUse = false;

				g_NPC.SetIsLUpintEqu(false);
				g_NPC.SetLUpFrame(-1);
				if(m_iLevelUpType == 6)
				{
					g_pGameControl->Send_Extract_Gem();
				}
				else if(m_iLevelUpType == 7)
				{
					g_pGameControl->Send_Remove_Gem(g_NPC.GetEquLUpGood(6).GetID(),(BYTE)g_NPC.GetEquLUpGood(m_iSelectedYuanShi).GetID());
				}
				else if(m_iLevelUpType == 5)
					g_pGameControl->SEND_Exchange_Gem_Ok( SPRITEBALL_LEVEL_UP_TYPE ,false);
				else if(m_iLevelUpType == 4)
					g_pGameControl->SEND_Exchange_Gem_Ok( CLOTH_LEVEL_UP_TYPE ,false);
				else
					g_pGameControl->SEND_Exchange_Gem_Ok( WEAPON_LEVEL_UP_TYPE ,false);

				m_pStartButton->SetEnable(false);

				if (lastMusicRand > 0)
				{
					g_pAudio->Stop(EAT_OTHER,200,lastMusicRand);
					lastMusicRand = 0;			
				}
			}

		}
		else
		{
			LPTexture pTex2 =g_pTexMgr->GetTex(PACKAGE_INTERFACE,10943,EP_UI);
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX-78 , m_iScreenY-107,pTex2);
			g_pGfx->SetRenderMode();
		}

	}

}

void  CEquipLevelUpWnd::SetReslutEffType(int iType)
{
	m_iResulEffType = iType;
	m_bIsStart = false;

	//成功或失败了都把除中间的武器之外的其它物体收掉
	for(int i = 0;i<MAX_LEVELUP_COUNT-1;i++)
	{
		CGood& good = g_NPC.GetEquLUpGood(i);
		if(good.GetStdMode() == 44 && good.GetShape() == 4)
		{
			continue;
		}
		g_NPC.GetEquLUpGood(i).SetID(0);
	}
}

void CEquipLevelUpWnd::UpdateLionHead(CGood& good)
{
	m_bDrawLionHead[0] = m_bDrawLionHead[1] = m_bDrawLionHead[2] = m_bDrawLionHead[3] = m_bDrawLionHead[4] = 1;

	if(m_iLevelUpType == 6)	//淬炼
	{
		int iCapacity = good.GetWeight();

		int index = 0;
		for(int i = 0;i < 10;i++)
		{
			if(good.GetResvEx(i) != 0)
			{
				m_bDrawLionHead[index] = g_AIGoodMgr.GetYuanShiLooks(i + 225,good.GetResvEx(i));		//画原石
				index++;
				iCapacity--;
			}
		}

		int i = 0;
		while(iCapacity > 0 && i < 5)
		{
			if(m_bDrawLionHead[i] == 1)
			{
				m_bDrawLionHead[i] = 0;		//空着
				iCapacity--;
			}
			i++;
		}

		vector<CGood> vecGood;
		for(int i = 0;i < 6;i++)
		{
			if(g_NPC.GetEquLUpGood(i).GetID())
			{
				if(g_AIGoodMgr.IsYuanShi(g_NPC.GetEquLUpGood(i)) && g_NPC.GetEquLUpGood(6).GetResvEx(g_NPC.GetEquLUpGood(i).GetShape() - 225) == 0)  //宝石未镶嵌这类原石
				{
					vecGood.push_back(g_NPC.GetEquLUpGood(i));
					g_NPC.GetEquLUpGood(i).SetID(0);
				}
				else			//已经镶嵌同类原石，放回包裹
				{
					SELF.PackageGood().BackToArray(g_NPC.GetEquLUpGood(i));
					g_NPC.GetEquLUpGood(i).SetID(0);
				}
			}
		}

		i = 4;
		while(vecGood.size() && i >= 0)
		{
			if(m_bDrawLionHead[i] == 0)
			{
				g_NPC.GetEquLUpGood(i) = vecGood.back();
				vecGood.pop_back();
			}
			i--;
		}

	}
	else if(m_iLevelUpType == 7)	//拆解
	{
		int iContainedWuXing = 0;
		int index = 0;

		for(int i = 0;i < 10;i++)
		{
			if(good.GetResvEx(i) != 0)
			{
				m_bDrawLionHead[index] = 0;		//空着

				DWORD dwLooks = g_AIGoodMgr.GetYuanShiLooks(i + 225,good.GetResvEx(i));
				g_NPC.GetEquLUpGood(index).SetLooks(dwLooks);
				g_NPC.GetEquLUpGood(index).SetID(i + 1);
				index++;
			}							
		}						
	}
}