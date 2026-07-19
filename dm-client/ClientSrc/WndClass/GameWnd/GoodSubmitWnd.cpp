#include "GoodSubmitWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include "GameData/PetData.h"
#include "Baseclass/Control/ParserTip.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Global/Interface/AudioInterface.h"
#include <time.h>
#include "GameData/NpcData.h"

INIT_WND_POSX(CGoodSubmitWnd,POS_VARIABLE,POS_VARIABLE)

CGoodSubmitWnd::CGoodSubmitWnd()
{
	m_pOK = NULL;
	m_pCancel = NULL;

	m_iIndex = 11121;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 100;

	g_pControl->SetWindowPos((g_pGfx->GetWidth() > 800)?450:400,100,"PackageWnd");
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);
}

CGoodSubmitWnd::~CGoodSubmitWnd(void)
{
}

void CGoodSubmitWnd::OnCreate()
{
	SetCloseButton(377,10);

	m_pOK = new CCtrlButton();
	if(m_pOK)
	{
		AddControl(m_pOK);
		m_pOK->CreateEx(this,252,290,4843,4844,4845);
	}
	//取消按钮
	m_pCancel = new CCtrlButton();
	if(m_pCancel)
	{
		AddControl(m_pCancel);
		m_pCancel->CreateEx(this, 359,179,4846,4847,4848);
	}

	SetMask(11121);

}


void CGoodSubmitWnd::OnDraw(void)
{
	CGood& temp = g_NPC.GetSubmitGood();
	std::string strInfo = g_NPC.GetSubmitGoodMsg1();

	std::vector<std::string> vecInfo;
	CutByUnicode((char*)strInfo.c_str(),vecInfo,52);

	for(int ii = 0; ii < (int)vecInfo.size();ii++)
	{
		std::string str = vecInfo.at(ii);
		g_pFont->DrawText(m_iScreenX+36,m_iScreenY+22+20*ii,str.c_str(),-1,FONT_DEFAULT,FONTSIZE_DEFAULT);
	}

	g_pFont->DrawText(m_iScreenX+240,m_iScreenY+184,g_NPC.GetSubmitGoodMsg2(),-1,FONT_DEFAULT,FONTSIZE_DEFAULT);

	if (temp.GetID() > 0)
	{
		int x = m_iScreenX + 260;
		int y = m_iScreenY + 232;

		if(g_pControl) //绘制单个物品
		{
			CGoodGrid::DrawOneGood(x+16,y+16,temp);
		}
	}

}

bool CGoodSubmitWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOK)
			{
				CGood& tmp = g_NPC.GetSubmitGood();
				if(tmp.GetID() == 0)
					return true;

				//发送消息给gs
				switch(g_NPC.GetSubmitGoodType())
				{
				case 1://请求还回领养了的宠物
					g_pGameControl->SEND_Pet_Adopt_Request(7,tmp.GetID());
					break;

				default:
					break;
				}

				OnClickCloseButton();
				return true;
			}

			if(pControl == m_pCancel)
			{
				//将物品放回包裹 关闭当前窗口
				OnClickCloseButton();
				return true;
			}
		}
		break;
	case MSG_SUBMIT_GOOD_WND:
		{
			if(dwData == 0)
			{
				BackToPackage();
			}
			else if(dwData == 2)
			{
				g_pControl->SetWindowPos((g_pGfx->GetWidth() > 800)?450:400,100,"PackageWnd");
				g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);
			}
			else
			{
				CloseWindow();
			}
			return true;

		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CGoodSubmitWnd::OnClickCloseButton()
{

	g_NPC.GetSubmitGood().SetID(0);
	g_NPC.SetSubmitGoodMsg1("");
	g_NPC.SetSubmitGoodMsg2("");

	CloseWindow();
}


bool CGoodSubmitWnd::OnMouseMove(int iX,int iY)
{
	CGood temp;
	CParserTip *pTip = g_pControl->GetTipWnd();

	if(InSubmitPos(iX,iY))
	{
		temp =g_NPC.GetSubmitGood();

		if(temp.GetID()!=0)
		{
			pTip->Clear();
			pTip->Parse(temp);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
		}
	}
	else
	{
		temp.SetID(0);
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CGoodSubmitWnd::OnLeftButtonUp(int iX, int iY)
{
	if (InSubmitPos(iX,iY))
	{
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		CGood temp = g_NPC.GetSubmitGood();

		if (GoodFrom.DropGood.GetID() != 0)
		{
			if (GoodFrom.eFromWnd == Package_Wnd ||	GoodFrom.eFromWnd == Panel_Wnd )
			{
				//限制放物品
				int iStdMode = GoodFrom.DropGood.GetStdMode();
				int iShape   = GoodFrom.DropGood.GetShape();

				switch(g_NPC.GetSubmitGoodType())
				{
				case 1://请求还回领养了的宠物只能放豹魔石
					{
						if(iStdMode != 49 || iShape != 51)
							return true;
						//看看是不是领养自别人的豹子
						map<DWORD,PET_ADOPT> & MyAdopMap = g_PetData.GetMyAdoptMap();
						if( MyAdopMap.find(GoodFrom.DropGood.GetID()) == MyAdopMap.end())
							return true;
					}
					break;

				default:
					break;
				}

				g_NPC.GetSubmitGood()  = GoodFrom.DropGood;

				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				if(GoodFrom.DropGood.GetID() != 0)
				{
					g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);
				}
			}
			else
			{
				return true;
			}
		}else
		{
			g_NPC.GetSubmitGood().SetID(0);
		}

		//取出托盘上的东西
		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = NO_Wnd;
		GoodFrom.iWndPos = 1;
		CGoodGrid::SetDropGoodFrom(GoodFrom);
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}


bool CGoodSubmitWnd::InSubmitPos(int iX, int iY)
{
	return  (iX>226 && iY>203 && iX <314 && iY <287);
}

void CGoodSubmitWnd::BackToPackage()
{
	//放回到包裹里面去。
	CGood tGem = g_NPC.GetSubmitGood();
	if (tGem.GetID() > 0)
	{
		for (int ip = 0;ip < MAX_PACKAGE_ELEMENT;ip++)
		{
			if (SELF.GetPackageGood(ip).GetID() == 0)
			{
				SELF.GetPackageGood(ip) = tGem;
				break;
			}
		}

		g_NPC.GetSubmitGood().SetID(0);
	}
}

