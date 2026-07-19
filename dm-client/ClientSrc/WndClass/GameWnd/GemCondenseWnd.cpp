#include "gemcondensewnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/TalkMgr.h"
#include "GameData/NpcData.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/MsgBoxMgr.h"

//m_iType =1    小宝石提炼成大宝石   
//原来的界面-不修改-6个连在一起的凹槽,没有花纹- 旁边一个"炼"字
//(全部是宝石(6颗) 6颗小宝石提炼成一个大宝石)

//m_iType = 2    天地宝石合成
//修改界面   -中间加一个凹槽
//-土黄色底盘表示泥土的颜色  - 上下有两个带花纹凹槽- 旁边一个"合"字
//(宝石(2颗) + 装备或矿石(4个)   +幸运符(1个可选))

//m_iType =3 矿石合成武器-武器铸造 
//修改界面 -中间加一个凹槽
//-绿色底盘- 没有凹槽-6个连在一起的凹槽,没有花纹 - 旁边一个"铸"字
//(   高纯度矿石+宝石(可选,数量不定,总数6个) + 幸运符(1个可选))  

//m_iType =4 超级武器碎片合成     
//修改界面-中间加一个凹槽
//-紫色底盘带神秘感 - 上下有两个带花纹凹槽 - 旁边一个"合"字
//(宝石(2颗) + 战神/道神/魔神碎片(4个) + 幸运符(1个可选)) 

//m_iType =7 灵犀宝盒强化

//12合原石

INIT_WND_POSX(CGemCondenseWnd,POS_VARIABLE,POS_VARIABLE)

CGemCondenseWnd::CGemCondenseWnd()
{
	//    +0+
	// +4+    +2+
	//    +6+
	// +5+    +3+
	//    +1+
	m_rcGem[0].left = 152;
	m_rcGem[0].top = 92;
	m_rcGem[2].left = 212;
	m_rcGem[2].top = 126;
	m_rcGem[3].left = 211;
	m_rcGem[3].top = 197;
	m_rcGem[1].left = 152;
	m_rcGem[1].top = 231;
	m_rcGem[4].left = 90;
	m_rcGem[4].top = 126;
	m_rcGem[5].left = 90;
	m_rcGem[5].top = 196;
	m_rcGem[6].left = 150;
	m_rcGem[6].top = 161;
	for(int i = 0;i<MAX_GEM_COUNT+1;i++)
	{
		m_rcGem[i].right = m_rcGem[i].left+20;
		m_rcGem[i].left -= 20;
		m_rcGem[i].bottom = m_rcGem[i].top+20;
		m_rcGem[i].top -= 20;
	}
	m_bIsCondensing = false;
	m_iMouseOnGem = -1;
	m_iMouseType = 0;
	m_iFrame = 0;

	m_iType  = sm_dwWindowType;
	if(m_iType == 6) 
	{
		m_iType = 3;
		m_bIsFuShi = true;
	}
	else 
		m_bIsFuShi = false;

	if (1 == m_iType  )
		m_iIndex = 4662;
	else if(2 == m_iType)
		m_iIndex = 11030;
	else if(3 == m_iType)
		m_iIndex = 11032;
	else if(4 == m_iType)
		m_iIndex = 11031;
	else if(7 == m_iType)
		m_iIndex = 16429;
	else
		m_iIndex = 4662;

	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 120;
	m_iResult = -1;
}

CGemCondenseWnd::~CGemCondenseWnd(void)
{
	BackToPackage();
}

void CGemCondenseWnd::OnCreate()
{
	SetCloseButton(327,47);
	m_pStartButton = new CCtrlButton();
	AddControl(m_pStartButton);
	//开始凝练/合成按钮
	if (m_iType == 1)//"炼"
	{
		m_pStartButton->CreateEx(this,262,74,4664,4665,4666);
	}
	else if(m_iType == 3)// "铸"
	{
		m_pStartButton->CreateEx(this,262,74,11033,11034,11035);
	}
	else if(m_iType == 7)	//"炼"
	{
		m_pStartButton->CreateEx(this,263,74,4675,4676,4677);
	}
	else //if(m_iType == 2 || m_iType == 4)//"合"
	{
		m_pStartButton->CreateEx(this,262,74,4667,4668,4669);
	}

	g_NPC.SetIsCondensing(false);
	SetMask(4662);
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);
	if(g_pGfx->GetWidth() == 1024)
		g_pControl->SetWindowPos(500,120,"PackageWnd");
	else
		g_pControl->SetWindowPos(360,120,"PackageWnd");

}

//单击关闭按钮
void CGemCondenseWnd::OnClickCloseButton()
{
	if(g_NPC.IsCondensing())
		return;

	CloseWindow();
}

void CGemCondenseWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	for(int i = 0;i<MAX_GEM_COUNT+1;i++)
	{
		CGood& temp = g_NPC.GetGemGood(i);
		if (temp.GetID() == 0)
			continue;

		int x = m_iScreenX+m_rcGem[i].left+4;
		int y = m_iScreenY+m_rcGem[i].top +4;

		if(g_pControl) //绘制单个物品
		{
			CGoodGrid::DrawOneGood(x+16,y+16,temp);
		}
	}

	//绘制掩盖图表明是否可以放物品
	if (m_iMouseOnGem >=0 && m_iMouseOnGem < MAX_GEM_COUNT || (m_iType >1 && m_iMouseOnGem == MAX_GEM_COUNT) )
	{

		LPTexture pTex = NULL;
		if (m_iMouseType == 1)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4672,EP_UI);
		}
		else if (m_iMouseType == 2)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4673,EP_UI);
		}
		if(pTex)
			g_pGfx->DrawTextureNL(m_iScreenX+m_rcGem[m_iMouseOnGem].left-3,m_iScreenY+m_rcGem[m_iMouseOnGem].top-3,pTex,0x40FFFFFF);
	}

	//框上的特效
	if(m_iType == 7)
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
	else
	{
		LPTexture pEffect = NULL;
		if (m_iType == 1 || m_iType == 12)
			pEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4670,EP_UI);
		else
			pEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4671,EP_UI);

		if (pEffect)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX-46,m_iScreenY-95,pEffect);
			g_pGfx->SetRenderMode();
		}
	}

	//凝练结果特效
	static int iFrameCounter = 0;

	if(m_iType == 7)
	{
		int iFrameNow = g_NPC.GetCondenseFrame();
		if(m_iResult)
		{
			if( m_iResult == 0 )
			{
				if (m_iResult == 1)
				{
					for(int _i=0;_i<24;_i++)
					{
						g_pTexMgr->GetTex(PACKAGE_INTERFACE,12725+_i,EP_UI);
					}
				}

				if (m_iResult == 2)
				{
					for(int _i=0;_i<24;_i++)
					{
						g_pTexMgr->GetTex(PACKAGE_INTERFACE,12750+_i,EP_UI);
						g_pTexMgr->GetTex(PACKAGE_INTERFACE,12775+_i,EP_UI);
					}
				}
			}

			if(iFrameNow <= 23)
			{
				LPTexture pTex = NULL;
				LPTexture pTex2 = NULL;

				if (m_iResult == 1)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12725 + iFrameNow,EP_UI);
				}
				if (m_iResult == 2)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12750 + iFrameNow,EP_UI);
					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12775 + iFrameNow,EP_UI);
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

			iFrameCounter++;
			if(iFrameCounter == 6)
			{
				iFrameCounter=0;
				g_NPC.SetCondenseFrame(++iFrameNow);
			}
			
			if(g_NPC.GetCondenseFrame() == 23)
			{
				g_NPC.SetCondenseFrame(0);
				m_iResult = 0;
			}
		}
	}
	else
	{
		//开始凝练特效		
		int iFrameNow = g_NPC.GetCondenseFrame();
		if(iFrameNow>=0 && g_NPC.IsCondensing())
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4700+iFrameNow,EP_UI);
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(m_iScreenX-46,m_iScreenY-95,pTex);
				g_pGfx->SetRenderMode();
			}

			iFrameCounter++;
			if(iFrameCounter == 6)
			{
				iFrameCounter=0;
				g_NPC.SetCondenseFrame(++iFrameNow);
			}

			if(iFrameNow==32)
			{
				g_NPC.SetCondenseFrame(-1);
				if(!m_bIsFuShi)
					g_pGameControl->SEND_Exchange_Gem_Ok(m_iType-1,false);
				else g_pGameControl->SEND_Exchange_Gem_Ok(8,false);
			}
		}
		else
		{
			for (int i=0;i<32;i++)
				g_pTexMgr->GetTex(PACKAGE_INTERFACE,4700+i,EP_UI);
		}
	}
}

bool CGemCondenseWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pStartButton && m_iType == 7)
			{
				bool bIsReady = false; 
				for (int i=0;i < 6;i++)
				{
					if(strcmp(g_NPC.GetGemGood(i).GetName(),"元灵晶石") == 0)
					{
						bIsReady = true;
					}
				}

				if(bIsReady)
				{
					g_NPC.SetIsCondensing(true);
					g_pGameControl->SEND_BOX_INFO(3,0,0);
				}
				else
				{
					g_TalkMgr.PushSysTalk("你没有放入元灵晶石，不能进行强化。");
				}

				return true;
			}
			else if(pControl == m_pStartButton && m_iType == 12)
			{
				if(g_NPC.IsCondensing())
					return true;

				int iNum = 0;
				for(int i = 0;i < 6;i++)
				{
					if(g_NPC.GetGemGood(i).GetID())
					{
						iNum += g_NPC.GetGemGood(i).GetDura();
					}
				}

				if(iNum < 6)
				{
					g_MsgBoxMgr.PopSimpleAlert("原石数量少于6，无法进行合成。");
					return true;
				}

				char str[256];
				sprintf(str,"是否要将你放入的所有的原石合成为高一阶的原石？你本次需要合成的次数为%d,总共需要消耗的精力值为%d。",iNum / 6,iNum / 6 * 10);
				g_MsgBoxMgr.PopSimpleComfirm(str,MSG_UPGRADE_YUANSHI,0);
				return true;
			}
			else if(pControl == m_pStartButton)
			{
				if(g_NPC.IsCondensing())
					return true;

				bool bIsReady = true;
				int  iCheckCount = 2;
				if (m_iType == 1 || m_iType == 3 || m_iType == 4)
					iCheckCount = MAX_GEM_COUNT;

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
					g_NPC.SetIsCondensing(true);

					if(!m_bIsFuShi)
						g_pGameControl->SEND_Exchange_Gem_Ok(m_iType-1,true);
					else 
						g_pGameControl->SEND_Exchange_Gem_Ok(8,true);
				}
				else
				{
					std::string strTemp;
					switch(m_iType)
					{
					case 1:
						strTemp = "宝石数目不正确，不能凝炼";
						break;
					case 3:
						strTemp = "数目不正确，不能合成武器";
						break;
					case 4:
						strTemp = "合成碎片需要2个宝石和四个相同的碎片，请放入";
						break;
					case 5:
						strTemp = "数目不正确，不能合成装备";
						break;
					default:
						strTemp = "宝石数目不正确，不能合成";
						break;
					}

					if(m_bIsFuShi)
					{
						CGood& temp = g_NPC.GetGemGood(6);
						if(temp.GetID() == 0 || !(temp.GetStdMode() == 44 && (temp.GetShape() == 3 || temp.GetShape() == 4)))
						{
							strTemp = "合成必须要有幸运符";
						}
					}

					g_TalkMgr.PushSysTalk(strTemp.c_str());
				}
				return true;
			}
		}
		break;
	case MSG_CTRL_GEMCONDENSE_WND:
		if(dwData == OPER_CUSTOM + 1)
		{
			m_iResult = 1;
		}
		else if(dwData == OPER_CUSTOM + 2)
		{
			m_iResult = 2;
		}
		else if(dwData == OPER_CUSTOM + 3)		//确认合成原石
		{
			g_NPC.SetIsCondensing(true);
			g_pGameControl->SEND_Exchange_Gem_Ok(11,true);
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CGemCondenseWnd::OnLeftButtonDown(int iX, int iY)
{
	int iGemIndex = GetGemIndex(iX,iY);
	if (iGemIndex>=0)
	{
		return true;
	}
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CGemCondenseWnd::OnLeftButtonUp(int iX, int iY)
{
	int iGemIndex = GetGemIndex(iX,iY);
	if (iGemIndex>=0)
	{
		if(g_NPC.IsCondensing())
			return true;

		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		CGood temp = g_NPC.GetGemGood(iGemIndex);

		if (GoodFrom.DropGood.GetID() != 0)
		{
			if (GoodFrom.eFromWnd == Package_Wnd ||
				GoodFrom.eFromWnd == Panel_Wnd || GoodFrom.eFromWnd == Gem_Wnd)
			{
				//限制放物品
				int iStdMode = GoodFrom.DropGood.GetStdMode();
				int iShape   = GoodFrom.DropGood.GetShape();
				if( iGemIndex ==6  )
				{
					if((iStdMode == 44 && iShape == 4 )||
						(iStdMode == 44 && iShape == 3 ) ||
						stricmp(temp.GetName(),"神秘符石") ==0 ||
						(iStdMode == 46 && iShape == 2 ))
					{
					}
					else
					{
						return true;
					}

					if(m_iType == 1)
						return true;
				}
				if ((m_iType == 1) || (m_iType ==2 && iGemIndex < 2) || (m_iType == 4 && iGemIndex<2))
				{
					if (iStdMode != 59 && iStdMode != 60)
						return true;
				}
				else if(m_iType == 2 && (iGemIndex == 2 || iGemIndex == 3 || iGemIndex == 5 || iGemIndex == 4))
				{
					if(m_iMouseType == 1)
						return true;
				}
				else if( m_iType == 2 && iGemIndex >=2 &&  iGemIndex < 6 )
				{
					if((iStdMode == 44 && iShape == 3 )||
						(iStdMode == 44 && iShape == 4 ) 
						)
					{
						return true;
					}
				}
				else if(m_iType == 3 && iGemIndex<MAX_GEM_COUNT)
				{
					//火晶石、冰月石、水云石、金刚石、圣魔石、青木石、紫心石
					//金 银 铜 黑铁矿石
					//合成武器 放置 宝石 + 高纯度矿石
					if(iStdMode == 43 && iShape == 1)
						return true;
					if ( (iStdMode !=59 && iStdMode != 43) && ((iStdMode != 46 && iShape == 2) ||
						(stricmp(temp.GetName(),"神秘符石") ==0)))
						return true;

				}
				else if(m_iType == 4 && iGemIndex>=2 && iGemIndex<MAX_GEM_COUNT)
				{
					//超级武器合成(放战神道神魔神碎片)
					if(iStdMode == 43 && iShape != 1)
						return true;
					if (   iStdMode != 43   )
						return true;

				}
				else if(m_iType == 5)
				{

				}
				else if(m_iType == 7)
				{
					if(strcmp(GoodFrom.DropGood.GetName(),"元灵晶石") != 0 && 
						strcmp(GoodFrom.DropGood.GetName(),"幸运符") != 0 &&
						strcmp(GoodFrom.DropGood.GetName(),"极品幸运符") != 0)
					{
						return true;
					}
				}
				else if(m_iType == 12)
				{
					if(!g_AIGoodMgr.IsYuanShi(GoodFrom.DropGood))
					{
						return true;
					}

					if(!IsYuanShiSameType(GoodFrom.DropGood))
					{
						g_MsgBoxMgr.PopSimpleAlert("请在合成界面中的六个孔中放入同阶层同属性的原石。");
						return true;
					}
				}
				else
				{
					if (iStdMode !=19 && iStdMode != 20 && iStdMode !=21 //项链
						&& iStdMode !=24 && iStdMode != 26  //手镯
						&& iStdMode != 22 && iStdMode != 23 && iStdMode != 43 && iStdMode != 44)//戒指和黑铁矿
						return true;
				}

				g_NPC.GetGemGood(iGemIndex) = GoodFrom.DropGood;

				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				if(GoodFrom.DropGood.GetID() != 0 )
				{
					g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
				}
			}else
				return true;
		}else
		{
			g_NPC.GetGemGood(iGemIndex).SetID(0);
		}

		//取出托盘上的东西
		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = Gem_Wnd;
		GoodFrom.iWndPos = iGemIndex;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

int CGemCondenseWnd::GetGemIndex(int iX,int iY)
{
	int iResult = -1;

	for (int i=0;i<MAX_GEM_COUNT+1;i++)
	{
		if (iX>=m_rcGem[i].left && iX<=m_rcGem[i].right && iY>=m_rcGem[i].top && iY<=m_rcGem[i].bottom)
			iResult = i;
	}

	if((m_iType == 7 || m_iType == 12) && iResult == 6)
	{
		iResult = -1;
	}

	return iResult;

}

bool CGemCondenseWnd::OnMouseMove(int iX, int iY)
{
	CGood temp;
	char strTemp[128]="";
	int iGemIndex = GetGemIndex(iX,iY);
	CParserTip *pTip = g_pControl->GetTipWnd();

	if (iGemIndex>=0)
	{
		pTip->Clear();
		//显示限制类物品
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if (GoodFrom.DropGood.GetID() !=0)
		{
			int iStdMode = GoodFrom.DropGood.GetStdMode();
			int iShape  = GoodFrom.DropGood.GetShape();
			m_iMouseType = 2;
			if( (m_iType>1 && iGemIndex ==6 ) )
			{
				if((iStdMode == 44 && iShape == 4 )||(iStdMode == 44 && iShape == 3 ) )
				{
				}
				else
				{
					m_iMouseType = 1;
				}

			}
			if ((m_iType == 1) || (m_iType ==2 && iGemIndex < 2) || (m_iType == 4 && iGemIndex <2))
			{
				if (iStdMode != 59 && iStdMode != 60)
				{
					m_iMouseType = 1;
				}
			}
			else if(m_iType == 2 && (iGemIndex == 2 || iGemIndex == 3 || iGemIndex == 5 || iGemIndex == 4))
			{
				if((iStdMode > 15 && iStdMode < 30 && iStdMode != 25) || (iStdMode == 43 && iShape == 0))
					m_iMouseType = 2;
				else
					m_iMouseType = 1;
			}
			else if( m_iType == 2 && iGemIndex >=2 &&  iGemIndex < 6 )
			{
				if((iStdMode == 44 && iShape == 4 )||(iStdMode == 44 && iShape == 3 ) )
				{
					m_iMouseType = 1;
				}
			}
			else if(m_iType == 3 && iGemIndex<MAX_GEM_COUNT)
			{
				if (   (iStdMode != 59 &&  iStdMode != 43) && ((iStdMode != 46 && iShape == 2) ||
					(stricmp(temp.GetName(),"神秘符石") ==0) ))//宝石 矿石
				{
					m_iMouseType = 1;
				}
				if (iStdMode == 43 && iShape ==1)
					m_iMouseType = 1;
			}
			else if(m_iType == 4 && iGemIndex>=2 && iGemIndex<MAX_GEM_COUNT)//只能为装备碎片
			{
				if (  iStdMode != 43 )
				{
					m_iMouseType = 1;
				}
				if (iStdMode == 43 && iShape !=1)
					m_iMouseType = 1;
			}
			else if( m_iType == 5)
			{
				
			}
			else if(m_iType == 7)
			{
				if(strcmp(GoodFrom.DropGood.GetName(),"元灵晶石") != 0 && 
					strcmp(GoodFrom.DropGood.GetName(),"幸运符") != 0 &&
					strcmp(GoodFrom.DropGood.GetName(),"极品幸运符") != 0)
				{
					m_iMouseType = 1;
				}				
			}
			else if(m_iType == 12)
			{
				if(!g_AIGoodMgr.IsYuanShi(GoodFrom.DropGood))
				{
					m_iMouseType = 1;
				}
			}
			else
			{
				if (iStdMode !=19 && iStdMode != 20 && iStdMode !=21//项链
					&& iStdMode != 24 && iStdMode != 26  //手镯
					&& iStdMode != 22 && iStdMode != 23 && iStdMode != 43 && iStdMode != 44)//戒指和矿石 幸运符
				{
					m_iMouseType = 1;
				}
			}
			m_iMouseOnGem = iGemIndex;
		}else
		{
			m_iMouseOnGem = -1;
		}

		temp = g_NPC.GetGemGood(iGemIndex);

		if(temp.GetID()!=0)
		{
			m_iMouseOnGem = -1;
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
			if( m_iType>1 && iGemIndex ==6)
			{
				sprintf(strTemp,"  %s  ","放置幸运符");
			}
			if ((m_iType == 1&& iGemIndex < MAX_GEM_COUNT) || (m_iType ==2 && iGemIndex < 2) || (m_iType == 4 && iGemIndex <2))
			{
				sprintf(strTemp,"  %s  ","放置宝石");
			}
			else if(m_iType == 3 && iGemIndex<MAX_GEM_COUNT)
			{
				if(m_bIsFuShi)
					sprintf(strTemp,"  %s  ","放置高纯度矿石、宝石、精铁或符石");
				else
					sprintf(strTemp,"  %s  ","放置高纯度矿石或宝石");
			}
			else if(m_iType == 4 && iGemIndex>=2 && iGemIndex<MAX_GEM_COUNT)//只能为装备碎片
			{
				sprintf(strTemp,"  %s  ","放置神级装备碎片");
			}
			else if(m_iType == 2 && (iGemIndex == 2 || iGemIndex == 3 || iGemIndex == 5 || iGemIndex == 4))
			{
				sprintf(strTemp,"  %s  ","放置饰品或高纯度矿石");
			}
			else if(m_iType == 7)
			{
				sprintf(strTemp,"  %s  ","放置元灵晶石和幸运符");
			}
			else if(m_iType == 12)
			{
				sprintf(strTemp,"  %s  ","放置原石");
			}

			if (stricmp(strTemp,"") != 0)
			{
				pTip->SetText(strTemp,false);
				int x = m_iScreenX + iX;
				int y = m_iScreenY + iY;
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
				return true;
			}
			else
				pTip->SetShow(false);

		}
	}
	else
	{
		m_iMouseOnGem = -1;

		temp.SetID(0);
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void CGemCondenseWnd::BackToPackage()
{
	//放回包裹中
	for(int i =0;i<MAX_GEM_COUNT+1;i++)
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

bool CGemCondenseWnd::IsYuanShiSameType( CGood& tmp )
{
	for(int i = 0;i < 6;i++)
	{
		if(g_NPC.GetGemGood(i).GetID())
		{
			if(g_NPC.GetGemGood(i).GetStdMode() != tmp.GetStdMode() || 
				g_NPC.GetGemGood(i).GetShape() != tmp.GetShape() ||
				g_NPC.GetGemGood(i).GetAC() != tmp.GetAC())
			{
				return false;
			}
		}
	}

	return true;
}


