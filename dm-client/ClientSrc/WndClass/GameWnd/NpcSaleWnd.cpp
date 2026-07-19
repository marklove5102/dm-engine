#include "NpcSaleWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/Good.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/NpcData.h"
#include "GameData/MsgBoxMgr.h"
#include "NpcWnd.h"

INIT_WND_POSX(CNpcSaleWnd,POS_VARIABLE,POS_VARIABLE)

CNpcSaleWnd::CNpcSaleWnd(void)
{
	m_iSale = sm_dwWindowType;
	 
	m_iIndex = 105;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 166;
	m_iOffY = 198;
	m_bNoMove = true;
	m_bNeedSavePage = false;
	m_bNeedSavePos = false;

	//打开包裹窗口
	if(g_pControl->GetWindowPos(m_iOffX,m_iOffY,"NpcWnd"))
	{
		m_iOffX += 146;
		m_iOffY += (CNpcWnd::m_sbHeaderNpc?170+201:201);
	}

	g_pControl->SetWindowPos(430,30,"PackageWnd");
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_CREATE);
	m_bBackToPackage = true;
	m_dwLastClickOkBtn = 0;

	m_UseBind = 0;
}

CNpcSaleWnd::~CNpcSaleWnd(void)
{
	if (m_bBackToPackage)
		g_NPC.BackToPackage();
}

void CNpcSaleWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iPrintX,iPrintY;
	iPrintX = m_iScreenX + 41;
	iPrintY = m_iScreenY + 9;

	std::string strJinbi = "金币";
	if ((m_iSale == 1 || m_iSale == 2)&& m_UseBind == 1)
	{
		strJinbi = "绑定金币";
	}

	char ch[50];
	//最终将按照服务器发回的字串显示////////////////////////
	switch(m_iSale)
	{
	case 1:
		strcpy(ch,"卖出:");
		break;
	case 2:
		strcpy(ch,"修理:");
		break;
	case 3:
		strcpy(ch,"保管物品:");
		break;
	case 4:
		strcpy(ch,"所需经验:");
		break;
	case 5:
		strcpy(ch,"所需经验");
		break;
	case 6:
		strcpy(ch,g_NPC.GetTrayTitle());
		break;
	case 7:
		strcpy(ch,"鉴定:");
		break;
	case 8:
		strcpy(ch,g_NPC.GetTrayTitle());
		break;
	default:
		strcpy(ch,"");
		break;
	}
	g_pFont->DrawText(iPrintX,iPrintY,ch,COLOR_TEXT_NORMAL, FONT_YAHEI);


	//画物品
	CGood &tempGood = g_NPC.GetGood();
	if(tempGood.GetID() == 0) 
		return;

	int x = m_iScreenX + 100;
	int y = m_iScreenY + 70;

	if(g_pControl) //绘制单个物品
	{
		CGoodGrid::DrawOneGood(x,y,tempGood);
	}

	//画钱
	unsigned long lMoney = g_NPC.GetSaleGold();
	char str[32];
	if(lMoney > 0)
	{
		if(lMoney!= -1)
		{
			ToCommaStr(str,lMoney);
		}
		else
			strcpy(str,"  ????");

		strcat(str,strJinbi.c_str());
	}
	else if(lMoney ==0&&g_NPC.GetGood().GetID()!=0 &&
		!g_NPC.GetStart())
	{
		//消息已到
		if(m_iSale == 1)
		{
			sprintf(str,"????%s",strJinbi.c_str());
		}
		else if(m_iSale == 2)
		{
			sprintf(str,"0%s",strJinbi.c_str());
		}
		else if(m_iSale ==4)
		{
			DWORD dw = g_NPC.GetForgeExp();
			sprintf(str,"%2.2f%%",(float)dw/100);
		}
		else if(m_iSale ==5)
		{
			DWORD dw = g_NPC.GetForgeExp();
			sprintf(str,"%2.2f%%",(float)dw/100);
		}
		else 
			strcpy(str,"");
	}
	else
		str[0] = 0;

	int len = (strlen(ch)+2) * FONTSIZE_DEFAULT/2;
	g_pFont->DrawText(iPrintX+len,iPrintY ,str,COLOR_TEXT_NORMAL, FONT_YAHEI);

}



bool CNpcSaleWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl )
{

	DWORD dwNpcID,dwGoodID;
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK :
		if(pControl == m_pOkButton)
		{
			//up 音乐
			g_pAudio->Play(EAT_OTHER,8,g_pAudio->GetRand()++);
			
			dwNpcID = g_NPC.GetID();
			dwGoodID = g_NPC.GetGood().GetID();
			g_NPC.SetRepairWindow(false);
			if(  dwGoodID && dwNpcID )
			{
				m_dwLastClickOkBtn = GetTickCount();
				switch(m_iSale)
				{
				case 1:
					g_pGameControl->SEND_Exchange_Sale_Ok(dwNpcID,dwGoodID,g_NPC.GetGood().GetName());
					break;
				case 2:
					g_pGameControl->SEND_Exchange_Repair_Ok(dwNpcID,dwGoodID,g_NPC.GetGood().GetName(),0,m_UseBind);
					g_NPC.SetRepairWindow(true);
					break;
				case 3:
					g_pGameControl->SEND_Exchange_Storage_Ok(dwNpcID,dwGoodID,g_NPC.GetGood().GetName());
					break;
				case 4:
					g_pGameControl->SEND_Exchange_Forge_Ok(dwNpcID,dwGoodID,g_NPC.GetGood().GetName(),0x01);
					break;
				case 5:
					g_pGameControl->SEND_Exchange_Forge_Ok(dwNpcID,dwGoodID,g_NPC.GetGood().GetName(),0x02);
					break;
				case 6:
					g_pGameControl->SEND_Exchange_Submit(dwNpcID,dwGoodID);
					break;
				case 7:
					g_pGameControl->SEND_Exchange_Repair_Ok(dwNpcID,dwGoodID,g_NPC.GetGood().GetName());
					g_NPC.SetRepairWindow(true);
					break;
				case 8:
					g_pGameControl->SEND_Exchange_Submit(dwNpcID,dwGoodID);
					g_NPC.SetRepairWindow(true);
					break;
				}
			}

			switch(m_iSale)
			{
			case 1:
				this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_Sell);
				break;
			case 2:
				this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_Repair);
				break;
			default:
				break;
			}


			return true;
		}
		else if(pControl == m_pCancelButton)
		{
			OnClickCloseButton();
			return true;
		}
		break;
	case OPER_CREATE:
		m_iSale = sm_dwWindowType;
		break;
	case MSG_CTRL_NPCSALEWND:
		{
			if (dwData == 10)
			{
				m_UseBind = 0;
				return true;
			}
			else if(dwData == 11)
			{
				m_UseBind = 1;
				return true;
			}			
		}
		break;

	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);

}

void CNpcSaleWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

 	m_pOkButton = new CCtrlButton();
	AddControl(m_pOkButton);
	m_pOkButton->CreateEx(this,153,29,90, 91, 92, 93);
	m_pOkButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,153,4,90, 91, 92, 93);
	m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	switch(m_iSale)
	{
	case 1:
		if (!g_pGameData->HasPaoPaoStatus(EP_First_Sell))
		{
			AddArrowTip(EP_First_Sell,97,77,XAL_TOPLEFT,false,XAL_TOPLEFT);
		}
		break;
	case 2:
		if (!g_pGameData->HasPaoPaoStatus(EP_First_Repair))
		{
			AddArrowTip(EP_First_Repair,97,77,XAL_TOPLEFT,false,XAL_TOPLEFT);
		}
		break;
	default:
		break;
	}
}

void CNpcSaleWnd::OnClickCloseButton()
{
	//点了确定按钮后100ms内不能关闭，如果还没有回应之前关闭回出现假物品
	if(GetTickCount() - m_dwLastClickOkBtn < 100)
	{
		return;
	}

	g_pAudio->Play(EAT_OTHER,1,g_pAudio->GetRand()++);
	CloseWindow();
}

bool CNpcSaleWnd::OnLeftButtonDown(int iX, int iY)
{
	m_iClickX = iX;
	m_iClickY = iY;
	m_bClick = true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CNpcSaleWnd::OnLeftButtonUp(int iX, int iY)
{
	CCtrlWindowX::OnLeftButtonUp(iX,iY);

	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
	DWORD dwNpcID,dwGoodID;	

	if(!(iX>65 && iX<127 && iY>38 && iY< 103))
		return true;

	if(GoodFrom.eFromWnd == Arm_Wnd ||
		GoodFrom.eFromWnd == Trade_Wnd || 
		GoodFrom.eFromWnd == Gem_Wnd ||
		GoodFrom.eFromWnd == Booth_Wnd)
		return true;

	//卸载装备
	if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == Package_Wnd||GoodFrom.eFromWnd == Panel_Wnd)&&
		g_NPC.GetGood().GetID()==0)
	{
		g_NPC.GetGood() = GoodFrom.DropGood;

		//放物品音乐////////////////////////////

		if(GoodFrom.DropGood.GetID() != 0)
		{
			g_pAudio->Play(EAT_GOODS,GoodFrom.DropGood.GetStdMode()+1,g_pAudio->GetRand()++);
		}

		GoodFrom.DropGood.SetID(0);
		CGoodGrid::SetDropGoodFrom(GoodFrom);
	}
	else	// 判断是否处于等待服务器状态
	{ 
		CGood temp = g_NPC.GetGood();
		g_NPC.GetGood() = GoodFrom.DropGood;
		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = NpcSale_Wnd;
		CGoodGrid::SetDropGoodFrom(GoodFrom);
	}
	dwNpcID = g_NPC.GetID();
	dwGoodID = g_NPC.GetGood().GetID();
	if(!dwGoodID)
	{
		g_NPC.SetSaleGold(0);
		return true;
	}

	g_NPC.SetStart(true); // 设置消息未到标识
	if(m_iSale == 1)
		g_pGameControl->SEND_Exchange_Sale_Takein(dwNpcID,dwGoodID,g_NPC.GetGood().GetName());
	else if(m_iSale == 2)
		g_pGameControl->SEND_Exchange_Repair(dwNpcID,dwGoodID,g_NPC.GetGood().GetName(),0,m_UseBind);
	else if(m_iSale == 4)//锻造
		g_pGameControl->SEND_Exchange_Forge(dwNpcID,dwGoodID,g_NPC.GetGood().GetName(),0x01);
	else if(m_iSale ==5)
		g_pGameControl->SEND_Exchange_Forge(dwNpcID,dwGoodID,g_NPC.GetGood().GetName(),0x02);
	else if(m_iSale ==7)
		g_pGameControl->SEND_Exchange_Repair(dwNpcID,dwGoodID,g_NPC.GetGood().GetName());
	
	return true;
}

bool CNpcSaleWnd::OnMouseMove(int iX, int iY)
{
	CCtrlWindowX::OnMouseMove(iX,iY);
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->SetShow(false);
	pTip->Clear();
	if(iX>73 && iX<117 && iY>45 && iY< 90)
	{
		CGood &temp =g_NPC.GetGood();

		if(temp.GetID()!=0)
		{
			pTip->Parse(temp);
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
		}
	}

	return true;
}