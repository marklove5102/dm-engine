#include "splitstonewnd.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/MsgBoxMgr.h"
#include "Gamedata/GameData.h"
#include "GameAI/AIGoodMgr.h"


INIT_WND_POSX(CSplitStoneWnd,POS_VARIABLE,POS_VARIABLE)

CSplitStoneWnd::CSplitStoneWnd(void)
{
	m_pOKButton		= NULL;
	m_pCancel       = NULL;
	m_pEdit			= NULL;
	m_FuShiGood.SetID(0);
	m_iTypeForSplit = sm_dwWindowType;


	m_iIndex = 40;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffY = 10;
	m_iOffY = 220;

	//设置包裹位置
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);
	g_pControl->SetWindowPos(336,138,"PackageWnd");

}

CSplitStoneWnd::~CSplitStoneWnd(void)
{
}

void CSplitStoneWnd::OnClickCloseButton()
{
	if(m_FuShiGood.GetID() != 0)
	{
		for (int ip = 0;ip < MAX_PACKAGE_ELEMENT;ip++)
		{
			if (SELF.GetPackageGood(ip).GetID() == 0)
			{
				SELF.GetPackageGood(ip) = m_FuShiGood;
				m_FuShiGood.SetID(0);
				break;
			}
		}
	}

	CloseWindow();
}

void CSplitStoneWnd::OnCreate()
{
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,100,125,90,91,92);
	m_pOKButton->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	
	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,240,125,90,91,92);
	m_pCancel->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);


	m_pEdit = new CCtrlEdit();
	AddControl(m_pEdit);
	m_pEdit->CreateEx(this,170,82,84,18,0,39,39,39,39);
	m_pEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pEdit->SetDigital(true);
}

void CSplitStoneWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	DrawTexture(120, 63, 78);
	g_pFont->DrawText(m_iScreenX+170, m_iScreenY+63, "请输入要提取的数目", COLOR_TEXT_NORMAL, FONT_YAHEI);

	//绘制符石
	if(m_FuShiGood.GetID() != 0)
		CGoodGrid::DrawOneGood(m_iScreenX + 138 ,m_iScreenY + 81,m_FuShiGood);
}

bool CSplitStoneWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pOKButton)
		{
			//发送相关协议进行拆分符石
			if(m_FuShiGood.GetID() != 0)
			{
				DWORD dwNum = 0;
				if(m_FuShiGood.GetStdMode() == 37 && m_FuShiGood.GetShape() == 214 && m_iTypeForSplit == 1)
				{
					BYTE* pByte = (BYTE*)&dwNum;
					*pByte			= m_FuShiGood.GetAC();		
					*(pByte + 1)	= m_FuShiGood.GetAC2();
					*(pByte + 2)	= m_FuShiGood.GetMAC();
					*(pByte + 3)	= m_FuShiGood.GetMAC2();
				}
				else if(m_FuShiGood.GetStdMode() == 37 && m_FuShiGood.GetShape() == 212  && m_iTypeForSplit == 2)
				{
					dwNum = m_FuShiGood.GetDura();
				}
                else if(m_FuShiGood.GetStdMode() == 3 && m_FuShiGood.GetShape() == 205  && m_iTypeForSplit == 3)
                {
                    dwNum = m_FuShiGood.GetDura();
                }
				else if(m_iTypeForSplit == 4 && g_AIGoodMgr.IsCanZhang(m_FuShiGood))
				{
					dwNum = m_FuShiGood.GetDura();
				}
				else if (m_iTypeForSplit == 5 && m_FuShiGood.GetShape() == 211 && m_FuShiGood.GetStdMode() == 3)
				{
					dwNum = m_FuShiGood.GetDura();
				}
				else if(m_iTypeForSplit == 6 && g_AIGoodMgr.IsYuanShi(m_FuShiGood))
				{
					dwNum = m_FuShiGood.GetDura();
				}
				else if(m_iTypeForSplit == 7 && ((m_FuShiGood.GetStdMode() == 43 && m_FuShiGood.GetShape() == 0) || m_FuShiGood.GetStdMode() == 48 || m_FuShiGood.GetStdMode() == 63))
				{
					dwNum = m_FuShiGood.GetAC();
				}
				else if(m_iTypeForSplit == 8)//通用接口,不限物品类型
				{
					dwNum = m_FuShiGood.GetDura();
				}
				else if(m_iTypeForSplit == 9)//通用接口,不限物品类型
				{
					dwNum = m_FuShiGood.GetAC();
				}
				else return true;


				DWORD dwNumToSplit = atoi(m_pEdit->GetText());
				DWORD dwID = m_FuShiGood.GetID();
				if(dwNum > 0 && dwNumToSplit>0 && dwNumToSplit < dwNum)
				{
					for (int ip = 0;ip < MAX_PACKAGE_ELEMENT;ip++)
					{
						if (SELF.GetPackageGood(ip).GetID() == 0)
						{
							SELF.GetPackageGood(ip) = m_FuShiGood;
							m_FuShiGood.SetID(0);
							break;
						}
					}

					g_pGameControl->SEND_Stone_Split_Request(dwID,dwNumToSplit,m_iTypeForSplit);
					OnClickCloseButton();
				}
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("输入的数目不正确，请重新输入");
					return true;
				}
			}
			return true;
		}
		else if(pControl == m_pCancel)
		{
			OnClickCloseButton();
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CSplitStoneWnd::OnLeftButtonUp(int iX,int iY)
{
	stGoodFrom stgoodfrom =  CGoodGrid::GetDropGoodFrom();
	if(stgoodfrom.DropGood.GetID() != 0 )
	{
		if((stgoodfrom.DropGood.GetStdMode() == 37 && stgoodfrom.DropGood.GetShape() == 214 && m_iTypeForSplit == 1) ||
			(stgoodfrom.DropGood.GetStdMode() == 37 && stgoodfrom.DropGood.GetShape() == 212 && m_iTypeForSplit == 2)||
			(stgoodfrom.DropGood.GetStdMode() == 3 && stgoodfrom.DropGood.GetShape() == 205 && m_iTypeForSplit == 3)||
			(g_AIGoodMgr.IsCanZhang(stgoodfrom.DropGood) && m_iTypeForSplit == 4) ||
			(stgoodfrom.DropGood.GetStdMode() == 3 && stgoodfrom.DropGood.GetShape() == 211 && m_iTypeForSplit == 5) ||
			(g_AIGoodMgr.IsYuanShi(stgoodfrom.DropGood) && m_iTypeForSplit == 6) ||
			(m_iTypeForSplit == 7 && ((stgoodfrom.DropGood.GetStdMode() == 43 && stgoodfrom.DropGood.GetShape() == 0) || stgoodfrom.DropGood.GetStdMode() == 48 || stgoodfrom.DropGood.GetStdMode() == 63)) ||
			m_iTypeForSplit == 8 ||
			m_iTypeForSplit == 9			
			)
			if(m_FuShiGood.GetID() == 0)
			{
				if(iX < 157 && iX > 120 && iY < 96 && iY > 63)
				{
					m_FuShiGood = stgoodfrom.DropGood;

					stgoodfrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(stgoodfrom);
					return true;
				}
			}		
	}
	else if(m_FuShiGood.GetID() != 0 && stgoodfrom.DropGood.GetID() ==0)//拿回去
	{
		if(iX < 157 && iX > 120 && iY < 96 && iY > 63)
		{
			stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
			GoodFrom.DropGood = m_FuShiGood;
			GoodFrom.eFromWnd = SplitStone_Wnd;
			GoodFrom.iWndPos = 0;
			CGoodGrid::SetDropGoodFrom(GoodFrom);
			m_FuShiGood.SetID(0);
			return true;
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CSplitStoneWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();

	int x = m_iScreenX + iX + 10;
	int y = m_iScreenY + iY + 10;
	pTip->AdjustXY(x,y);
	pTip->Move(x,y);

	if(iX < 157 && iX > 120 && iY < 96 && iY > 63)
	{
		pTip->Clear();

		if(m_FuShiGood.GetID() == 0) 
		{
			if(m_iTypeForSplit == 1)//神秘符石
			{
				pTip->AddText("放入要拆分的符石并输入要提取的数目",0xFFFFFF00);
			}
			else if(m_iTypeForSplit == 2)//圣殿精华
			{
				pTip->AddText("放入要拆分的圣殿精华并输入要提取的数目",0xFFFFFF00);
			}
            else if(m_iTypeForSplit == 3)	//灵符袋
            {
                pTip->AddText("放入要拆分的灵符袋并输入要提取的数目",0xFFFFFF00);
            }
			else if(m_iTypeForSplit == 4)		//残章
			{
				pTip->AddText("放入要拆分的残章并输入要提取的数目",0xFFFFFF00);
			}
			else if (m_iTypeForSplit == 5)
			{
				pTip->AddText("放入要拆分的图腾并输入要提取的数目",0xFFFFFF00);
			}
			else if(m_iTypeForSplit == 6)
			{
				pTip->AddText("放入要拆分的原石并输入要提取的数目",0xFFFFFF00);
			}
			else if(m_iTypeForSplit == 7)
			{
				pTip->AddText("放入要拆分的矿石、木材或金盒并输入要提取的数目",0xFFFFFF00);
			}
			else if(m_iTypeForSplit == 8)//通用接口,不限物品类型
			{
				pTip->AddText("放入要拆分的物品并输入要提取的数目",0xFFFFFF00);
			}
			else if(m_iTypeForSplit == 9)//通用接口,不限物品类型
			{
				pTip->AddText("放入要拆分的物品并输入要提取的数目",0xFFFFFF00);
			}

		}		
		else
		{			
			pTip->Parse(m_FuShiGood);
		}		

		pTip->SetShow(true);
		return true;
	}

	pTip->SetShow(false);
	return CCtrlWindowX::OnMouseMove(iX,iY);
}



