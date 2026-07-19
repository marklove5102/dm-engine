#include "otherboothwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/BoothData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/TalkMgr.h"
#include "GameData/WooolStoreData.h"
#include "GameClient/WidgetManager.h"

INIT_WND_POSX(COtherBoothWnd,POS_VARIABLE,POS_VARIABLE)

COtherBoothWnd::COtherBoothWnd()
{
	//m_iBoothType = (int)sm_dwWindowType;//包裹格子数类别

	m_iWCells = 6;
	m_iHCells = 5;
	m_iCellW  = 37;
	m_iCellH  = 37;
	m_iCellDisW = 2;
	m_iCellDisH = 2;
	m_bDClicked = false;
	m_ptGoodPoint.x = 26;
	m_ptGoodPoint.y = 47;
	m_pBuyButton = NULL;

	m_nChannel  = TALKTYPE_BUY;


	m_iIndex = 4989;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

	if((g_pGfx->GetWidth() > 800))
	{
		m_iOffX = 500;
		m_iOffY = 50;
	}
	else
	{
		m_iOffX = 300;
		m_iOffY = 50;
	}
}

COtherBoothWnd::~COtherBoothWnd(void)
{

}

void COtherBoothWnd::OnCreate()
{
	//先确保租赁窗口是关闭的,因为共用g_OtherBoothData
	g_pControl->PopupWindow(MSG_CTRL_LEASEOTHERBOOTH_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_BOOTH_WND,OPER_CLOSE);


	g_TalkMgr.GetBuyTalk().clear();
	m_dwLastBuyGoodID = 0;
	m_dwLastClickBuyTime = 0;

	SetCloseButton(252,2,80);


	m_pMoreOtherbooth = new CCtrlButton();
	AddControl(m_pMoreOtherbooth);
	m_pMoreOtherbooth->CreateEx(this,35,255,95,96,97);
	m_pMoreOtherbooth->SetText("其它摊位");

	//购买
	m_pBuyButton = new CCtrlButton();
	AddControl(m_pBuyButton);
	m_pBuyButton->CreateEx(this,195,255,95,96,97);
	m_pBuyButton->SetText("购 买");

	//聊天输入框
	m_pChatEdit = new CCtrlEdit();
	AddControl(m_pChatEdit);
	m_pChatEdit->CreateEx(this,30,370,186,23,0,39,39,39,39);
	m_pChatEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pChatEdit->SetMaxLength(100);

	g_pGameControl->SEND_Request_Server_Time();

	//m_pSearchGoodName = new CCtrlEdit();
	//AddControl(m_pSearchGoodName);
	//m_pSearchGoodName->CreateEx(this,90,206,124,20,0,39,39,39,39);
	//m_pSearchGoodName->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	//m_pSearchGoodName->SetMaxLength(100);

	//m_pSearchButton = new CCtrlButton();
	//AddControl(m_pSearchButton);
	//m_pSearchButton->CreateEx(this,75,234,95,96,97);
	//m_pSearchButton->SetText("搜 索");

	//m_pGoButton  = new CCtrlButton();
	//AddControl(m_pGoButton);
	//m_pGoButton->CreateEx(this,135,234,4963,4964,4965);
	//m_pGoButton->SetText("进入拍卖行");

	m_pTalkViewer = new CTalkViewer();
	AddControl(m_pTalkViewer);
	m_pTalkViewer->Create(this,31,307,210,55);	
	m_pTalkViewer->AddScrollEx(193, 0, 17, 55);
	m_pTalkViewer->SetTalkType(&g_TalkMgr.GetBuyTalk());	
}

//取得点所在的格子,(ix,iy)是相对本窗口的坐标
void COtherBoothWnd::GetGoodGrid(int* iX, int* iY, int ix, int iy)
{
	int w = ix - m_ptGoodPoint.x;
	int h = iy - m_ptGoodPoint.y;

	if( !(IsInGoodGrid(ix,iy))|| (w % (m_iCellW + m_iCellDisW)) > m_iCellW || (h % (m_iCellH + m_iCellDisH)) > m_iCellH)
	{
		*iX = -1;
		*iY = -1;
		return;
	}
	*iX = w / (m_iCellW + m_iCellDisW);
	*iY = h / (m_iCellH + m_iCellDisH);
}

//判断某点是否在单元格区域内
bool COtherBoothWnd::IsInGoodGrid(int ix, int iy)
{
	int w,h;
	w = ix - m_ptGoodPoint.x;
	h = iy - m_ptGoodPoint.y;

	if(w < 0 || h < 0 || w >= (m_iCellW + m_iCellDisW) * m_iWCells || h >= (m_iCellH + m_iCellDisH) * m_iHCells)
	{
		return false;
	}

	return true;
}

void COtherBoothWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//摊位名字
	unsigned long nID = g_OtherBoothData.GetOtherPlayerID();
	CSimpleCharacterNode* pChar = g_pGameData->FindSimpleCharacter(nID);
	if(pChar)
	{
		char temp[256] = {0};
		sprintf(temp,"[%s]的摊位",pChar->GetName());
		int iXX = m_iScreenX+130 - strlen(temp)*FONTSIZE_DEFAULT/4;
		g_pFont->DrawText(145 + m_iScreenX, 7 + m_iScreenY, temp, COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG, DTF_Center);
	}
	else
	{
		CloseWindow();
		return;
	}

	//绘制物品栏内物品
	for(int nh = 0; nh < m_iHCells; nh++)
	{
		for(int nw = 0; nw < m_iWCells; nw++)
		{
			CGood temp = g_OtherBoothData.GetGoods(nh * m_iWCells +nw);

			if (temp.GetID() == 0)
				continue;

			int x = m_iScreenX + m_ptGoodPoint.x + nw * (m_iCellW + m_iCellDisW);
			int y = m_iScreenY + m_ptGoodPoint.y + nh * (m_iCellH + m_iCellDisH);

			if (g_OtherBoothData.GetReadyBuy().GetID() == temp.GetID())
			{
				g_pGfx->DrawFillRect(x+1,y+1,m_iCellW-2, m_iCellH-3,0x4400FF00);
			}

			if(g_pControl) //绘制单个物品
			{
				CGoodGrid::DrawOneGood(x+20,y+18,temp);
			}
		}        
	}

	//g_pFont->DrawText(m_iScreenX+31, m_iScreenY+178, "支持金砖、金条自动结算", COLOR_TEXT_NORMAL, FONT_YAHEI);
	g_pFont->DrawText(115 + m_iScreenX, 275 + m_iScreenY, "买卖留言", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	//g_pFont->DrawText(m_iScreenX+35, m_iScreenY+209, "道具名称", COLOR_TEXT_NORMAL, FONT_YAHEI);

}

bool COtherBoothWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pBuyButton)
		{
			if (SELF.GetBoothState())
				return true;

			if (g_OtherBoothData.GetReadyBuy().GetID() != 0)
			{
				//6秒之内点了同一个物品不响应
				if(m_dwLastBuyGoodID == g_OtherBoothData.GetReadyBuy().GetID() && GetTickCount() - m_dwLastClickBuyTime < 6000)
				{
					return true;
				}

				m_dwLastClickBuyTime = GetTickCount();
				m_dwLastBuyGoodID = 0;
				g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CREATE);
			}
			return true;
		}
		else if (pControl == m_pMoreOtherbooth)
		{
			g_pControl->PopupWindow(MSG_CTRL_BOOTHGUILD_WND,OPER_RECREATE);
		}
// 		else if(pControl == m_pGoButton)
// 		{
// 			if(g_PetBoothData.IsPaimaiClosed()) //判断是否关闭了拍卖行
// 				return true;
// 
// 			g_WidgetMgr.LoginPaiMai();
// 			g_WooolStoreMgr.SetWooolStorePage(WOOOLSTORE_PAIMAI);
// 			g_WooolStoreMgr.SetPaiMaiStoreType(1);
// 
// 			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);
// 			return true;
// 		}
// 		else if(pControl == m_pSearchButton)
// 		{
// 			if(g_PetBoothData.IsPaimaiClosed()) //判断是否关闭了拍卖行
// 				return true;
// 
// 			g_WooolStoreMgr.SetWooolStorePage(WOOOLSTORE_PAIMAI);
// 			g_WooolStoreMgr.SetPaiMaiStoreType(5);
// 
// 			std::string str = "Search#";
// 			char tmp[64] = {0};
// 			string szGoodName  = m_pSearchGoodName->GetText();
// 			StringUtil::trim(szGoodName);
// 
// 			if(!szGoodName.empty())
// 			{
// 				sprintf(tmp,":Keyword$%s",szGoodName.c_str());
// 				str.append(tmp);
// 			}
// 			g_PetBoothData.SetRedirectPage(str.c_str());
// 			g_WidgetMgr.LoginPaiMai();
// 			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);
// 			return true;
// 		}
		break;
	case MSG_CTRL_EDIT_ENTRY:
		{
			char chatline[256] = {0};
			strcpy(chatline,m_pChatEdit->GetText());
			StringUtil::trim(chatline);

			if (strlen(chatline) <=0)
				return true;

			const char* szPlayerName = NULL;
			std::string strTemp  ;
			unsigned long nPlayerID = g_OtherBoothData.GetOtherPlayerID();
			if (nPlayerID>0)
			{

				CSimpleCharacterNode* pChar = g_pGameData->FindSimpleCharacter(nPlayerID);
				if (pChar)
					szPlayerName = pChar->GetName();
			}
			if(szPlayerName)
			{
				char temp[256] = {0};
				sprintf(temp,"%s %s",szPlayerName,chatline);
				g_pGameControl->SEND_Message_Booth(temp,strlen(temp),true);

				sprintf(temp,"%s: %s",SELF.GetName(),chatline);
				g_TalkMgr.PushTalk(TALKTYPE_BUY,temp,0xFF04);
			}
			m_pChatEdit->Clear();
			return true;
		}
		break;
	case MSG_CTRL_OTHER_BOOTH:
		{
			if(dwData == 1)
			{
				m_dwLastBuyGoodID = g_OtherBoothData.GetReadyBuy().GetID();
			}
			else if(dwData == 2)
			{
				m_dwLastBuyGoodID = 0;
			}
		}
		break;
	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool COtherBoothWnd::OnLeftButtonDown(int iX, int iY)
{
	if(IsInGoodGrid(iX,iY))		//点击中表格不纪录拖动
		return true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool COtherBoothWnd::OnLeftButtonUp(int iX, int iY)
{
	if (IsInGoodGrid(iX,iY))
	{
		int X,Y;
		GetGoodGrid(&X,&Y,iX,iY);
		if (X != -1 && Y != -1)
		{
			int iPos = Y*m_iWCells+X;
			if(iPos < 0 || iPos >= MAX_BOOTH_GOODS_NUM)
				return true;

			CGood temp = g_OtherBoothData.GetGoods(iPos);
			if(temp.GetID() == 0)
				return true;

			if(g_pInput->IsShift())  //按物品
			{
				g_pControl->Msg(MSG_CTRL_INSERT_OBJECTLINK, 0,(CControl*)(&temp));	
				return true;
			}

			if (g_OtherBoothData.GetReadyBuy().GetID() == temp.GetID())
			{
				g_OtherBoothData.GetReadyBuy().SetID(0); //取消选中
			}
			else
			{
				g_OtherBoothData.GetReadyBuy() = temp;   //设置选中
			}
			return true;
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool COtherBoothWnd::OnLeftButtonDClick(int iX, int iY)
{
	m_bDClicked = true;
	if (IsInGoodGrid(iX,iY))
	{
		m_bDClicked = false;
		return COtherBoothWnd::OnLeftButtonUp(iX,iY);
	}

	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

bool COtherBoothWnd::OnMouseMove(int iX,int iY)
{
	CGood* pTalkGood = NULL;
	CParserTip *pTip = g_pControl->GetTipWnd();

	//if(m_pTalkContainer->ClickMove(iX,iY,pTalkGood))
	//{
	//	if(pTalkGood)
	//	{
	//		pTip->Parse(*pTalkGood);
	//int x = m_iScreenX + iX + 10;
	//int y = m_iScreenY + iY + 10;
	//pTip->AdjustXY(x,y);
	//pTip->Move(x,y);
	//		pTip->SetShow(true);
	//	}
	//	else
	//	{
	//		pTip->SetShow(false);
	//	}
	//	return true;
	//}

	int X,Y;
	GetGoodGrid(&X,&Y,iX,iY);

	if(X != -1 && Y != -1)
	{
		CGood temp = g_OtherBoothData.GetGoods(Y * m_iWCells + X);
		if (temp.GetID() == 0)
		{
			pTip->SetShow(false);
			return true;
		}

		pTip->Clear();
		pTip->ParseWithPrice(temp);
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x ,y);
		pTip->SetShow(true);
		return true;
	}
	else
	{
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

INIT_WND_POSX(CBoothBuyConfirmWnd,POS_AUTO,POS_AUTO)

//确认是否购买的对话框
CBoothBuyConfirmWnd::CBoothBuyConfirmWnd()
{
	m_pOKButton = NULL;
	m_pCancelButton = NULL;
	m_pGoButton = NULL;

	m_iIndex = 40;

	m_iPages = 1;
	
	m_dwFlag = sm_dwWindowType;
}

CBoothBuyConfirmWnd::~CBoothBuyConfirmWnd()
{
}

void CBoothBuyConfirmWnd::OnCreate()
{
	//确定按钮
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,100,125,90,91,92);
	m_pOKButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//取消按钮
	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,220,125,90,91,92);
	m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//设置，在弹出窗口的地方已经验证过数据的合法性了
	CGood& ReadyBuy = g_OtherBoothData.GetReadyBuy();

	char tempMsg[256] = {0};
	char str[64] = {0};
	if (ReadyBuy.GetPayType())
	{
		ToCommaStr(str,ReadyBuy.GetPrice());
		sprintf(tempMsg,"你将以%s元宝的价格购买“%s”物品，",str,ReadyBuy.GetName());
		m_iPos=(int)(strstr(tempMsg,"元宝") - tempMsg);
		m_iBuyType = BUY_YUANBAO;
	}
	else
	{
		ToCommaStr(str,ReadyBuy.GetPrice());

		sprintf(tempMsg,"你将以%s金币的价格购买“%s”物品，",str,ReadyBuy.GetName());
		m_iPos=(int)(strstr(tempMsg,"金币") - tempMsg);
		m_iBuyType = BUY_MONEY;
	}
	m_strMsg.assign(tempMsg);
}

void CBoothBuyConfirmWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 76,m_iScreenY+32,m_strMsg.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI);

	if (m_iPos > 0)
	{
		if(m_iBuyType == BUY_YUANBAO)
			g_pFont->DrawText(m_iScreenX + 76 + m_iPos*6,m_iScreenY+32,"元宝",0xFF00FF00,FONT_YAHEI);
		else
			g_pFont->DrawText(m_iScreenX + 76 + m_iPos*6,m_iScreenY+32,"金币",0xFF00FF00,FONT_YAHEI);
	}
	g_pFont->DrawText(m_iScreenX+76,m_iScreenY+46,"你确定吗?",COLOR_TEXT_NORMAL,FONT_YAHEI);

	//g_pFont->DrawText(m_iScreenX+76,m_iScreenY + 70,"温馨提示：拍卖行内物品可能更便宜！",COLOR_TEXT_NORMAL,FONT_YAHEI);
}

bool CBoothBuyConfirmWnd::OnKeyDown(WORD wState, UCHAR cKey)
{
	switch(cKey)
	{
	case VK_RETURN:
		{
			Msg(MSG_CTRL_BUTTON_CLICK,0,m_pOKButton);
			return true;
		}
	case VK_ESCAPE:
		{
			Msg(MSG_CTRL_BUTTON_CLICK,0,m_pCancelButton);
			return true;
		}
	}

	return CCtrlWindowX::OnKeyDown(wState,cKey);
}


bool CBoothBuyConfirmWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pOKButton)
		{
			//确定购买
			unsigned long ulPlayerID = g_OtherBoothData.GetOtherPlayerID();

			CGood& ReadBuyGood = g_OtherBoothData.GetReadyBuy();
			g_OtherBoothData.GetAlreadyBuy() = ReadBuyGood;
			
			if (ReadBuyGood.GetID() > 0)
			{
				if (m_dwFlag == 0)//玩家摊位
				{
					g_pGameControl->SEND_Exchange_Buy_Ok(ulPlayerID,ReadBuyGood.GetID(),ReadBuyGood.GetName());
					g_pControl->MsgToWnd("OtherBoothWnd",MSG_CTRL_OTHER_BOOTH,1);
				}
				else//租赁摊位
				{
					//发送购买消息
					g_pGameControl->Send_LeaseBooth_BuyOthers(ulPlayerID,ReadBuyGood.GetID(),1,ReadBuyGood.GetName());
					g_pControl->MsgToWnd("LeaseOtherBoothWnd",MSG_CTRL_OTHER_BOOTH,1);
				}
			}

			CloseWindow();
			return true;
		}
		else if(pControl == m_pCancelButton)
		{
			g_OtherBoothData.GetReadyBuy().SetID(0);
			CloseWindow();
			return true;
		}
		//else if(pControl == m_pGoButton)
		//{
		//	if(g_PetBoothData.IsPaimaiClosed()) //判断是否关闭了拍卖行
		//		return true;

		//	g_OtherBoothData.GetReadyBuy().SetID(0);
		//	g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CLOSE);

		//	g_WooolStoreMgr.SetWooolStorePage(WOOOLSTORE_PAIMAI);
		//	g_WooolStoreMgr.SetPaiMaiStoreType(0);
		//	g_WidgetMgr.LoginPaiMai();
		//	g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);

		//	return true;
		//}
		break;
	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
