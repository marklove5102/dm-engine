#include "smallgemcon.h"
#include "Gamedata/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "Global/Interface/AudioInterface.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/NpcData.h"

INIT_WND_POSX(CSmallGemCon,POS_AUTO,POS_AUTO)

CSmallGemCon::CSmallGemCon(void)
{
	m_iGemCount_Red = 0 ;
	m_iGemCount_Blue = 0 ;
	m_iGemCount_Green = 0 ;

	m_iMaxRedGem = SELF.GetReserveData(plyRedGemCount); 
	m_iMaxBlueGem = SELF.GetReserveData(plyBlueGemCount); 
	m_iMaxGreenGem = SELF.GetReserveData(plyGreenGemCount); 

	m_rc[0].left = 112;
	m_rc[0].top = 186;
	m_rc[0].bottom = 218;
	m_rc[0].right = 145;
	for(int i = 0;i<3;i++)
	{
		for(int j =1;j<=5;j++)
		{
			if( (i*5+j) < 8 )
			{
				m_rc[i*5 +j].left = 32 + (j-1)*40;
				m_rc[i*5 +j].top = 144 + i*40;
				m_rc[i*5 +j].right = m_rc[i*5 +j].left + 35;
				m_rc[i*5 +j].bottom = m_rc[i*5 +j].top + 35;
			}
			else if((i*5+j) > 8) 
			{
				m_rc[i*5 +j-1].left = 32 + (j-1)*40;
				m_rc[i*5 +j-1].top = 144 + i*40;
				m_rc[i*5 +j-1].right  = m_rc[i*5 +j-1].left + 35;
				m_rc[i*5 +j-1].bottom = m_rc[i*5 +j-1].top + 35;
			}
		}
	}
	m_iMouseOnGood = -1;
	m_iMouseType = 0;

	m_SlidePos1.x =120;
	m_SlidePos1.y =47;
	m_SlidePos2.x =120;
	m_SlidePos2.y =78;
	m_SlidePos3.x =120;
	m_SlidePos3.y =107;

	m_bIsSlide = false;
	m_bIsInRightArea = false;
	m_bIsbeenMelt = false;

	//获得宝石数量
	g_pGameControl->SEND_SmallGem_Count_Request();


	m_iIndex = 17698;
	m_iPages = 1;
	
}

CSmallGemCon::~CSmallGemCon(void)
{
}

void CSmallGemCon::OnCreate()
{
	//关闭按钮
	SetCloseButton(225,3,80);
	//合成按钮
	m_pCondense = new CCtrlButton() ;
	AddControl(m_pCondense);
	m_pCondense->CreateEx(this,93,281,90,91,92);
	m_pCondense->SetText("熔 炼",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_DISABLE);
	//红宝石向左按钮
	m_pLeftButton_Red = new CCtrlButton() ;
	AddControl(m_pLeftButton_Red);
	m_pLeftButton_Red->CreateEx(this,101,50,155,156,157 );
	//红宝石向右按钮
	m_pRightButton_Red = new CCtrlButton() ;
	AddControl(m_pRightButton_Red);
	m_pRightButton_Red->CreateEx(this,217,50,152,153,154 );
	//兰宝石向左按钮
	m_pLeftButton_Blue = new CCtrlButton() ;
	AddControl(m_pLeftButton_Blue);
	m_pLeftButton_Blue->CreateEx(this,101,80,155,156,157 );
	//兰宝石向右按钮	
	m_pRightButton_Blue = new CCtrlButton() ;
	AddControl(m_pRightButton_Blue);
	m_pRightButton_Blue->CreateEx(this,217,80,152,153,154 );
	//绿宝石向左按钮
	m_pLeftButton_Green = new CCtrlButton() ;
	AddControl(m_pLeftButton_Green);
	m_pLeftButton_Green->CreateEx(this,101,109,155,156,157 );
	//绿宝石向右按钮
	m_pRightButton_Green = new CCtrlButton() ;
	AddControl(m_pRightButton_Green);
	m_pRightButton_Green->CreateEx(this,217,109,152,153,154 );


	SELF.SetReserveData(plyJumpMap,0);//设置跨地图无效的标志


	m_pEditRedGem = new CCtrlEdit();
	AddControl(m_pEditRedGem);
	m_pEditRedGem->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,70,50,16,16);
	m_pEditRedGem->SetDigital(true);
	m_pEditRedGem->SetMaxLength(2);
	m_pEditRedGem->SetText("0");

	m_pEditBlueGem = new CCtrlEdit();
	AddControl(m_pEditBlueGem);
	m_pEditBlueGem->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,70,80,16,16);
	m_pEditBlueGem->SetDigital(true);
	m_pEditBlueGem->SetMaxLength(2);
	m_pEditBlueGem->SetText("0");

	m_pEditGreenGem = new CCtrlEdit();
	AddControl(m_pEditGreenGem);
	m_pEditGreenGem->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,70,110,16,16);
	m_pEditGreenGem->SetDigital(true);
	m_pEditGreenGem->SetMaxLength(2);
	m_pEditGreenGem->SetText("0");
}


void CSmallGemCon::Draw(void)
{
	m_iMaxRedGem = SELF.GetReserveData(plyRedGemCount); 
	m_iMaxBlueGem = SELF.GetReserveData(plyBlueGemCount); 
	m_iMaxGreenGem = SELF.GetReserveData(plyGreenGemCount); 

	CCtrlWindowX::Draw() ;
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	char str[128] = {0};
	int i1 = min(m_iMaxRedGem,20);
	int iTemp1 = atoi(m_pEditRedGem->GetText());
	m_pEditRedGem->SetEnable(i1>0);
	if(iTemp1 > i1)
	{
		sprintf(str,"%d",i1);
		m_pEditRedGem->SetText(str);
	}

	int i2 = min(m_iMaxBlueGem,20);
	int iTemp2 = atoi(m_pEditBlueGem->GetText());
	m_pEditBlueGem->SetEnable(i2>0);
	if(iTemp2  > i2 )
	{
		sprintf(str,"%d" ,i2 );
		m_pEditBlueGem->SetText(str);
	}

	int i3 = min(m_iMaxGreenGem,20);
	int iTemp3 = atoi(m_pEditGreenGem->GetText());
	m_pEditGreenGem->SetEnable(i3>0);
	if(iTemp3  > i3 )
	{
		sprintf(str,"%d",i3 );
		m_pEditGreenGem->SetText(str);
	}

	if(!m_bIsInRightArea)
	{
		m_iGemCount_Red = min(iTemp1,i1);
		m_iGemCount_Blue = min(iTemp2,i2);
		m_iGemCount_Green = min(iTemp3,i3);
	}

	g_pFont->DrawText(m_iScreenX + 108,m_iScreenY + 10,"熔 炼",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);

	for(int i = 0;i<15;i++)
	{
		CGood& temp = g_NPC.GetOreGood(i);
		if (temp.GetID() == 0)
			continue;

		int x = m_iScreenX+m_rc[i].left + 4;
		int y = m_iScreenY+m_rc[i].top + 1;

		if(g_pControl) //绘制单个物品
		{
			CGoodGrid::DrawOneGood(x+16,y+16,temp);
		}

		if(m_bIsbeenMelt)
			g_pGfx->DrawFillRect(m_iScreenX + m_rc[i].left, m_iScreenY + m_rc[i].top, m_rc[i].right - m_rc[i].left, m_rc[i].bottom - m_rc[i].top,0x44FF0000);
	}

	//绘制掩盖图表明是否可以放物品
	if (m_iMouseOnGood >=0 && m_iMouseOnGood < 16)
	{

		if (m_iMouseType == 1)//鼠标上的物品为不可放置的物品
		{
			g_pGfx->DrawFillRect(m_iScreenX + m_rc[m_iMouseOnGood].left, m_iScreenY + m_rc[m_iMouseOnGood].top, m_rc[m_iMouseOnGood].right - m_rc[m_iMouseOnGood].left,m_rc[m_iMouseOnGood].bottom - m_rc[m_iMouseOnGood].top,0x44FF0000);
		}
		else if(m_iMouseType == 2)
			g_pGfx->DrawFillRect(m_iScreenX + m_rc[m_iMouseOnGood].left, m_iScreenY + m_rc[m_iMouseOnGood].top, m_rc[m_iMouseOnGood].right - m_rc[m_iMouseOnGood].left,m_rc[m_iMouseOnGood].bottom - m_rc[m_iMouseOnGood].top,0x4400FF00);
	}


	LPTexture pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,151,EP_UI);
	LPTexture pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,150,EP_UI);
	
	if (!g_pInput->IsLeftButton() || !m_bIsSlide)
		m_iSlidePos = -1;		 

	if(pTex1 && pTex2)
	{
		int iPos = 0;
		if(m_iMaxRedGem)
			iPos = static_cast<int>(85.0*m_iGemCount_Red/min(m_iMaxRedGem,20));
		g_pGfx->DrawTextureNL(m_iScreenX+119+iPos,m_iScreenY+m_SlidePos1.y,(m_iSlidePos == 0)?pTex1:pTex2);
		iPos = 0;
		if(m_iMaxBlueGem)
			iPos = static_cast<int>(85.0*m_iGemCount_Blue/min(m_iMaxBlueGem,20));
		g_pGfx->DrawTextureNL(m_iScreenX+119+iPos,m_iScreenY+m_SlidePos2.y,(m_iSlidePos == 1)?pTex1:pTex2);
		iPos = 0;
		if(m_iMaxGreenGem)
			iPos = static_cast<int>(85.0*m_iGemCount_Green/min(m_iMaxGreenGem,20));
		g_pGfx->DrawTextureNL(m_iScreenX+119+iPos,m_iScreenY+m_SlidePos3.y,(m_iSlidePos == 2)?pTex1:pTex2);
	}

	for(int i = 0;i<3;i++)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_items,659 + i,EP_UI);
		if (pTex)
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 25,m_iScreenY + 45 + i * 30,pTex);
		}
	}

	//开始凝练特效
	static int iFrameCounter = 0;
	int iFrameNow = g_NPC.GetCondenseFrame();
	if(iFrameNow>=0 && g_NPC.IsCondensing() && m_bIsbeenMelt)
	{

		if( iFrameNow == 0 )
		{
			g_pTexMgr->PreLoad(PACKAGE_INTERFACE,11080,11110,EP_UI);
		}
		if(iFrameNow <= 30)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11080+iFrameNow,EP_UI);
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(m_iScreenX+5,m_iScreenY+79,pTex);
				g_pGfx->SetRenderMode();
			}
		}
		iFrameCounter++;
		if(iFrameCounter == 8)
		{
			iFrameCounter=0;
			g_NPC.SetCondenseFrame(++iFrameNow);
		}
		if(iFrameNow==28)
		{
			g_pGameControl->SEND_SmallGem_Condense_Request(m_iGemCount_Red,m_iGemCount_Blue,m_iGemCount_Green );
			for(int i=0;i<16;i++)
			{
				if( strcmp(g_NPC.GetOreGood(i).GetName(),"幸运符(捆)" ) != 0  )
					g_NPC.GetOreGood(i).SetID(0);
			}
			g_NPC.SetIsCondensing(false);
			g_NPC.SetCondenseFrame(-1);
			m_bIsbeenMelt = false;
		}
		LPTexture	pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11111,EP_UI);
		LPTexture	pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11112,EP_UI);
		LPTexture	pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11113,EP_UI);
		if(pTex1)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + 14 , m_iScreenY + 28, pTex1);
			g_pGfx->DrawTextureNL(m_iScreenX + 14 , m_iScreenY + 59, pTex2);
			g_pGfx->DrawTextureNL(m_iScreenX + 14 , m_iScreenY + 89, pTex3);
			g_pGfx->SetRenderMode();
		}
	}
}

void CSmallGemCon::OnClickCloseButton()
{
	if(g_NPC.IsCondensing())
		return;

	//如果已经熔炼过,那么就是假物品了
	if(m_bIsbeenMelt)
	{
		for(int i=0;i<16;i++)
			g_NPC.GetOreGood(i).SetID(0);
	}
	CloseWindow();
}

bool CSmallGemCon::OnLeftButtonDown(int iX, int iY)
{
	if(bInSlidArea(iX,iY) )
	{
		if(iX > 119 && iY > 49 && iX < 215 && iY < 126)
		{
			int iTemp = 0;
			if(iX > 119 && iY > 49 && iX < 215 && iY < 65)
			{
				m_iSlidePos = 0;
				iTemp = min(m_iMaxRedGem,20);
				if(iTemp == 0)
				{
					g_MsgBoxMgr.PopSimpleAlert("\n\n对不起,您的仓库和包裹中现在还没有红宝石可供投入熔炼。");
					return true;
				}
			}
			if(iX > 119 && iY > 79 && iX < 215 && iY < 95)
			{
				m_iSlidePos = 1;
				iTemp = min(m_iMaxBlueGem,20);
				if(iTemp == 0)
				{
					g_MsgBoxMgr.PopSimpleAlert("\n\n对不起,您的仓库和包裹中现在还没有蓝宝石可供投入熔炼。");
					return true;
				}
			}
			if(iX > 119 && iY > 108 && iX < 215 && iY < 125)
			{
				m_iSlidePos = 2;
				iTemp = min(m_iMaxGreenGem,20);
				if(iTemp == 0)
				{
					g_MsgBoxMgr.PopSimpleAlert("\n\n对不起,您的仓库和包裹中现在还没有绿宝石可供投入熔炼。");
					return true;
				}
			}
			m_bIsSlide = true;
		}
		else
		{
			m_iSlidePos = -1;
			m_bIsSlide = false;
		}
		return true;
	}

	return CCtrlWindowX::OnLeftButtonDown( iX, iY);
}

bool CSmallGemCon::OnLeftButtonUp(int iX, int iY)
{
	m_bIsSlide = false;
	int iIndex =  IsInGrid(iX,iY);
	if (iIndex>=0 && !m_bIsbeenMelt)
	{
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		CGood temp = g_NPC.GetOreGood(iIndex);

		if (GoodFrom.DropGood.GetID() != 0)
		{
			if (GoodFrom.eFromWnd == Package_Wnd ||
				GoodFrom.eFromWnd == Panel_Wnd || GoodFrom.eFromWnd == Gem_Wnd)
			{
				//限制放物品
				int iStdMode = GoodFrom.DropGood.GetStdMode();
				int iShape   = GoodFrom.DropGood.GetShape();
				if ( iIndex >0)//这个位置放宝石
				{
					if (iStdMode != 43)
						return true;
					if( strcmp(GoodFrom.DropGood.GetName(),"赤金") == 0 ||
						strcmp(GoodFrom.DropGood.GetName(),"雪银") == 0 ||
						strcmp(GoodFrom.DropGood.GetName(),"玄铁") == 0 
						)
						return true;
				}
				else if( iIndex ==0) //这个位置放幸运符
				{
					if((iStdMode == 44 && iShape == 4 )||(iStdMode == 44 && iShape == 3 ) )
					{
					}
					else
					{
						return true;
					}		
				}
				g_NPC.GetOreGood(iIndex) = GoodFrom.DropGood;
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
			g_NPC.GetOreGood(iIndex).SetID(0);
		}

		//取出托盘上的东西
		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = Gem_Wnd;
		GoodFrom.iWndPos = iIndex;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp( iX, iY);
}

bool CSmallGemCon::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_INPUT_CHAR:
		{
			if(pControl == m_pEditRedGem)
			{

			}
			else if(pControl == m_pEditBlueGem)
			{

			}
			else if(pControl == m_pEditGreenGem)
			{

			}

		}
	case MSG_CTRL_BUTTON_CLICK:
		{      
			if(pControl == m_pCondense) 
			{
				if( (m_iGemCount_Red + m_iGemCount_Blue + m_iGemCount_Green) < 10 )
				{
					g_MsgBoxMgr.PopSimpleAlert("\n\n对不起,您投入的宝石总数不足10个,无法成功熔炼。");
					return true;
				}
				m_bIsbeenMelt = true;
				g_NPC.SetCondenseFrame(0);
				g_NPC.SetIsCondensing(true);
				g_pAudio->Play(EAT_OTHER,200,g_pAudio->GetRand()++);
				return true;
			}
			else if(pControl == m_pLeftButton_Red)
			{
				if(m_iGemCount_Red>0)
					m_iGemCount_Red--;
			}
			else if(pControl == m_pRightButton_Red)
			{
				if(m_iGemCount_Red < min(m_iMaxRedGem,20))
					m_iGemCount_Red++;
			}
			else if(pControl == m_pLeftButton_Blue)
			{
				if(m_iGemCount_Blue>0)
					m_iGemCount_Blue--;
			}
			else if(pControl == m_pRightButton_Blue)
			{
				if(m_iGemCount_Blue < min(m_iMaxBlueGem,20))
					m_iGemCount_Blue++;
			}
			else if(pControl == m_pLeftButton_Green)
			{
				if(m_iGemCount_Green>0)
					m_iGemCount_Green--;
			}
			else if(pControl == m_pRightButton_Green)
			{
				if(m_iGemCount_Green < min(m_iMaxGreenGem,20))
					m_iGemCount_Green++;
			}
			char str[32] = {0};
			sprintf(str,"%d",m_iGemCount_Red);
			m_pEditRedGem->SetText(str);
			sprintf(str,"%d",m_iGemCount_Blue);
			m_pEditBlueGem->SetText(str);
			sprintf(str,"%d",m_iGemCount_Green);
			m_pEditGreenGem->SetText(str);
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg( dwMsg, dwData, pControl);
}

bool CSmallGemCon::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();

	if(IsInRightArea(iX,iY))
	{
		m_bIsInRightArea = true;
	}
	else 
		m_bIsInRightArea = false;

	CGood temp;
	int iIndex = IsInGrid(iX,iY);
	if (iIndex>=0)
	{
		//显示限制类物品
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if (GoodFrom.DropGood.GetID() !=0)
		{
			int iStdMode = GoodFrom.DropGood.GetStdMode();
			int iShape  = GoodFrom.DropGood.GetShape();
			m_iMouseType = 2;//可以放置


			if (iIndex > 0)//这个位置放矿石
			{
				if (iStdMode !=43 )
					m_iMouseType = 1;
				if( strcmp(GoodFrom.DropGood.GetName(),"赤金") == 0 ||
					strcmp(GoodFrom.DropGood.GetName(),"雪银") == 0 ||
					strcmp(GoodFrom.DropGood.GetName(),"玄铁") == 0 
					)
					m_iMouseType = 1;
			}
			else if(iIndex ==0) //这个位置放幸运符
			{
				if((iStdMode == 44 && iShape == 4 )||(iStdMode == 44 && iShape == 3 ) )
				{
				}
				else
				{
					m_iMouseType = 1;
				}
			}

			m_iMouseOnGood = iIndex;
		}else
		{
			m_iMouseOnGood = -1;
		}

		temp = g_NPC.GetOreGood(iIndex);
		if(temp.GetID()!=0)
		{
			m_iMouseOnGood = -1;
			pTip->Parse(temp);
			int x = m_iScreenX + iX;
			int y = m_iScreenY + iY;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
		}
		else
		{
			pTip->Clear();
			char strTemp[32]="";
			if(iIndex == 0)
				sprintf(strTemp,"  %s  ","放置幸运符");
			else
				sprintf(strTemp,"  %s  ","放置矿石");

			pTip->SetText(strTemp,false);
			int x = m_iScreenX + iX;
			int y = m_iScreenY + iY;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
		}
	}
	else
	{
		m_iMouseOnGood = -1;
		temp.SetID(0);
		pTip->SetShow(false);

		int indexPos = IsInGemGrid(iX,iY);
		if(indexPos)
		{
			pTip->Clear();
			char strTemp[32]="";
			if(indexPos == 1)
				sprintf(strTemp,"  库存(%d)颗  ",m_iMaxRedGem );
			else if(indexPos == 2)
				sprintf(strTemp,"  库存(%d)颗  ",m_iMaxBlueGem );
			else if(indexPos == 3)
				sprintf(strTemp,"  库存(%d)颗  ",m_iMaxGreenGem );
			pTip->SetText(strTemp,false);
			int x = m_iScreenX + iX;
			int y = m_iScreenY + iY;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			if(indexPos > 0)
				pTip->SetShow(true);
			else 
				pTip->SetShow(false);
		}
		else
			pTip->SetShow(false);

	}

	if(!bInSlidArea(iX,iY) && m_bIsSlide == false)
	{
		return CCtrlWindowX::OnMouseMove(iX,iY);
	}
	if(m_bIsSlide)
	{
		int iTemp = 0;
		char str[32] = {0};
		if(iX > 119 && iY > 49 && iX < 215 && iY < 65)
		{
			iTemp = min(m_iMaxRedGem,20);
			if(iTemp == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("\n\n对不起,您的仓库和包裹中现在还没有红宝石可供投入熔炼。");
				return true;
			}
			m_iGemCount_Red = static_cast<int>(iTemp*(iX - 119)/85.0);
			if(m_iGemCount_Red < 0)
				m_iGemCount_Red = 0;
			if(m_iGemCount_Red > iTemp )
				m_iGemCount_Red = iTemp ;
			sprintf(str,"%d",m_iGemCount_Red);
			m_pEditRedGem->SetText(str);
			return true;
		}
		if(iX > 119 && iY > 79 && iX < 215 && iY < 95)
		{
			iTemp = min(m_iMaxBlueGem,20);
			if(iTemp == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("\n\n对不起,您的仓库和包裹中现在还没有蓝宝石可供投入熔炼。");
				return true;
			}
			m_iGemCount_Blue = static_cast<int>(iTemp*(iX - 119)/85.0);
			if(m_iGemCount_Blue < 0)
				m_iGemCount_Blue = 0;
			if(m_iGemCount_Blue > iTemp )
				m_iGemCount_Blue = iTemp ;
			sprintf(str,"%d",m_iGemCount_Blue);
			m_pEditBlueGem->SetText(str);
			return true;
		}
		if(iX > 119 && iY > 108 && iX < 215 && iY < 125)
		{
			iTemp = min(m_iMaxGreenGem,20);
			if(iTemp == 0)
			{
				g_MsgBoxMgr.PopSimpleAlert("对不起,您的仓库和包裹中现在还没有绿宝石可供投入熔炼。");
				return true;
			}
			m_iGemCount_Green = static_cast<int>(iTemp*(iX - 119)/85.0);
			if(m_iGemCount_Green < 0)
				m_iGemCount_Green = 0;
			if(m_iGemCount_Green > iTemp )
				m_iGemCount_Green = iTemp ;
			sprintf(str,"%d",m_iGemCount_Green);
			m_pEditGreenGem->SetText(str);
			return true;
		}

		m_bIsSlide = false;

	}

	return true;
}

int CSmallGemCon::IsInGrid(int iX,int iY)
{
	for (int i=0;i<15;i++)
	{
		if (iX>=m_rc[i].left && iX<=m_rc[i].right && iY>=m_rc[i].top && iY<=m_rc[i].bottom)
			return i;
	}

	return -1;
}

int CSmallGemCon::IsInGemGrid(int iX,int iY)
{

	if (iX>=37 && iX<=62 && iY>=57 && iY<=80)
		return 1;
	else if (iX>=37 && iX<=62 && iY>=90 && iY<=108)
		return 2;
	else if (iX>=37 && iX<=62 && iY>=116 && iY<=139)
		return 3;

	return -1;
}

bool CSmallGemCon::bInSlidArea(int iX,int iY)
{
	if (iX>118 && iY>48 && iX<217 && iY<126)
		return true;

	return false;
}

bool CSmallGemCon::IsInRightArea(int iX,int iY)
{
	if (iX>104 && iY>55 && iX<268 && iY<139)
		return true;

	return false;
}

void CSmallGemCon::CheckSlideAear(int iX,int iY)
{
	if(iX>150 && iX<223)
	{
		if(iY>61 && iY <73 && m_iMaxRedGem>0)
		{
			if(iX<m_SlidePos1.x)
			{
				if( m_iGemCount_Red > 0)
					m_iGemCount_Red--;
			}
			else if(iX > m_SlidePos1.x+4)
			{
				if( m_iGemCount_Red < m_iMaxRedGem)
					m_iGemCount_Red++;
			}
			m_SlidePos1.x = 150+ static_cast<int>(70.0*m_iGemCount_Red/m_iMaxRedGem);
			return;
		}

		if(iY>88 && iY <104 && m_iMaxBlueGem>0)
		{
			if(iX<m_SlidePos2.x)
			{
				if( m_iGemCount_Blue > 0)
					m_iGemCount_Blue--;
			}
			else if(iX > m_SlidePos2.x+4)
			{
				if( m_iGemCount_Blue < m_iMaxBlueGem)
					m_iGemCount_Blue++;
			}
			m_SlidePos2.x = 150+static_cast<int>(70.0*m_iGemCount_Blue/m_iMaxBlueGem);
			return;
		}
		if(iY>119 && iY <132 && m_iMaxGreenGem>0)
		{
			if(iX<m_SlidePos3.x)
			{
				if( m_iGemCount_Green > 0)
					m_iGemCount_Green--;
			}
			else if(iX > m_SlidePos3.x+4)
			{
				if( m_iGemCount_Green < m_iMaxGreenGem)
					m_iGemCount_Green++;
			}
			m_SlidePos3.x = 150+static_cast<int>(70.0*m_iGemCount_Green/m_iMaxGreenGem);
			return;
		}
	}
}

void CSmallGemCon::BackToPackage()
{
	//放回包裹中
	for(int i =0;i<16;i++)
	{
		CGood tGem = g_NPC.GetOreGood(i);
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
		g_NPC.GetOreGood(i).SetID(0);
	}
}
