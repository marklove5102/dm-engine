#include "boothwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/BoothData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/TalkMgr.h"
#include "GameAI/PathFinder.h"


INIT_WND_POSX(CBoothWnd,POS_VARIABLE,POS_VARIABLE)

CBoothWnd::CBoothWnd()
{
	m_iWCells = 6;
	m_iHCells = 5;
	m_iCellW  = 37;
	m_iCellH  = 37;
	m_iCellDisW = 2;
	m_iCellDisH = 2;
	m_bDClicked = false;
	m_ptGoodPoint.x = 27;
	m_ptGoodPoint.y = 72;
	m_pStartButton = NULL;
	m_pEndButton = NULL;
	m_pGetGoodButton = NULL;
	m_pGetSaleButton = NULL;

	m_pGetSaleButton = NULL;
	m_pGetGoodButton = NULL;


	m_iIndex = 4606;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

	if(g_pGfx->GetWidth() > 800)
	{
		m_iOffX = 210;
		m_iOffY = 50;
	}
	else
	{
		m_iOffX = 40;
		m_iOffY = 50;
	}
}

CBoothWnd::~CBoothWnd(void)
{
	g_pControl->PopupWindow(MSG_CTRL_BOOTH_NAME,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_BOOTH_ADD_GOOD,OPER_CLOSE);
}

void CBoothWnd::OnCreate()
{
	//if (!SELF.GetBoothState())
	{
		//如果包裹窗口没有显示则显示包裹窗口
		//先确保租赁窗口是关闭的,因为共用g_BoothData
		g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_LEASEOTHERBOOTH_WND,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_OTHER_BOOTH,OPER_CLOSE);


		g_pControl->SetWindowPos((g_pGfx->GetWidth() > 800)?500:360,50,"PackageWnd");
		g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);
	}


	SetCloseButton(252,3,80);

	//摊位名称按钮
	m_pNameButton = new CCtrlButton();
	AddControl(m_pNameButton);
	m_pNameButton->CreateEx(this,30,42,95,96,97);
	m_pNameButton->SetText("摊位名称");

	//开始摆摊按钮
	m_pStartButton = new CCtrlButton();
	AddControl(m_pStartButton);
	//m_pStartButton->CreateEx(this,95,170,4611,4612,4613);
	m_pStartButton->CreateEx(this,45,408,90,91,92);
	m_pStartButton->SetText("开始摆摊",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//停止摆摊按钮
	m_pEndButton = new CCtrlButton();
	AddControl(m_pEndButton);
	//m_pEndButton->CreateEx(this,167,170,4614,4615,4616);
	m_pEndButton->CreateEx(this,165,408,90,91,92);
	m_pEndButton->SetText("结束摆摊",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//摊位名
	if (strlen(g_BoothData.GetBoothName()) == 0
		&& strlen(SELF.GetName()) > 0)
	{
		char temp[512];
		sprintf(temp,"[%s]的摊位",SELF.GetName());
		g_BoothData.SetBoothName(temp);
	}

	//聊天输入框
	m_pChatEdit = new CCtrlEdit();
	AddControl(m_pChatEdit);
	m_pChatEdit->CreateEx(this,30,364,186,23,0,39,39,39,39);
	m_pChatEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pChatEdit->SetMaxLength(100);

	m_pTalkViewer = new CTalkViewer();
	AddControl(m_pTalkViewer);
	m_pTalkViewer->Create(this,31,302,210,55);
	//m_pTalkViewer->AddScroll(176,14,16,23,4629);
	//m_pTalkViewer->AddUpButton(174,-3,4626,4627,4628);
	//m_pTalkViewer->AddDownButton(174,34,4623,4624,4625);
	m_pTalkViewer->AddScrollEx(193, 0, 17, 55);
	m_pTalkViewer->SetTalkType(&g_TalkMgr.GetBoothTalk());
}


//取得点所在的格子,(ix,iy)是相对本窗口的坐标
void CBoothWnd::GetGoodGrid(int* iX, int* iY, int ix, int iy)
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
bool CBoothWnd::IsInGoodGrid(int ix, int iy)
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

void CBoothWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//摊位名字
	string szBoothName = g_BoothData.GetBoothName();
	if (szBoothName.size() > 18)
	{
		StringUtil::AutoCut(szBoothName,14);
		szBoothName += "...";
	}

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 39,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 94, m_iScreenY + 41, pTex);

	g_pFont->DrawText(m_iScreenX + 97,m_iScreenY + 44,szBoothName.c_str(),COLOR_TEXT_NORMAL, FONT_YAHEI);

	g_pFont->DrawText(110 + m_iScreenX, 9 + m_iScreenY, "玩家摊位", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);

	g_pFont->DrawText(117 + m_iScreenX, 274 + m_iScreenY, "买卖留言", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);

	bool bNeedFullFill = false;
	
	LPTexture pLockedTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17538,EP_UI);

	//绘制物品栏内物品
	for(int nh = 0; nh < m_iHCells; nh++)
	{
		for(int nw = 0; nw < m_iWCells; nw++)
		{
			int iGridNum = nh * m_iWCells + nw;
			if(iGridNum >= g_BoothData.GetMaxGoodNum())
			{				
				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGoodPoint.x + (iGridNum % 6) * (m_iCellW + m_iCellDisW) + 3,m_iScreenY + m_ptGoodPoint.y + (iGridNum / 6) * (m_iCellH + m_iCellDisH) + 3,pLockedTex);
				continue;
			}

			//物品
			GoodPrice_t& price = g_BoothData.GetPrice(nh * m_iWCells + nw);
			if(price.GetID() == 0)
				continue;

			//在包裹栏找到相应的Goods
			CGood* pGood = SELF.PackageGood().FindGood(price.GetID());
			if(pGood == NULL)
			{
				price.SetID(0);
				bNeedFullFill = true;
				continue;
			}

			int x = m_iScreenX + m_ptGoodPoint.x + nw * (m_iCellW + m_iCellDisW) + 3;
			int y = m_iScreenY + m_ptGoodPoint.y + nh * (m_iCellH + m_iCellDisH) + 2; 

			CGoodGrid::DrawOneGood(x+16,y+16,*pGood);
		}        
	}
	if(bNeedFullFill)
		g_BoothData.FullFill();

}

bool CBoothWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_TALKVIEW_LCLICK:
		{
			string strLine;
			if(m_pTalkViewer->getLineHeader(strLine))
			{
				m_pChatEdit->SetText(strLine.c_str() + 1);
				m_pChatEdit->SetFocus();
			}	
			break;
		}
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pNameButton)
		{
			if(!SELF.GetBoothState())
			{
				g_pControl->PopupWindow(MSG_CTRL_BOOTH_NAME,OPER_CREATE,MSG_CTRL_BOOTH_NAME);
			}
			return true;
		}
		else if(pControl == m_pStartButton)
		{
			if (SELF.GetBoothState())
				return true;

			bool bExistGoods = false;

			for(int i=0;i<MAX_BOOTH_GOODS_NUM;i++)
			{
				GoodPrice_t price = g_BoothData.GetPrice(i);
				if (0 == price.GetID())
					continue;

				if(NULL != SELF.PackageGood().FindGood(price.GetID()))
				{
					bExistGoods = true;
					break;
				}
			}

			if (!bExistGoods)
			{
				g_MsgBoxMgr.PopSimpleAlert("摊位上没有物品不能摆摊。");
				return true;
			}

			SELF.GetMapPathFinder().SetOnRoute(0);
			SELF.GetCNextAction().Clear();
			g_pGameControl->SEND_Booth_Request_Stall();


			return true;
		}
		else if(pControl == m_pEndButton)
		{
			if (!SELF.GetBoothState())
				return true;
			g_pGameControl->SEND_Booth_Cancel_Stall();
			return true;
		}
		break;
	case MSG_CTRL_EDIT_ENTRY:
		{
			char chatline[256] = {0};
			strcpy(chatline,m_pChatEdit->GetText());
			StringUtil::trim(chatline);

			if (strlen(chatline) ==0)
				return true;

			if (!SELF.GetBoothState())
			{
				m_pChatEdit->Clear();
				return true;
			}

			char* ch = strchr(chatline,' ');
			if (ch)
			{
				char tmp[256] = {0};
				strncpy(tmp,chatline,ch-chatline);
				if (g_BoothTalkName.IsInList(tmp))
				{
					g_pGameControl->SEND_Message_Booth(chatline,strlen(chatline),false);

					sprintf(tmp,"%s: %s",SELF.GetName(),ch);

					g_TalkMgr.PushTalk(TALKTYPE_BOOTH,tmp,0xFF04);
				}
			}

			m_pChatEdit->Clear();

			return true;
		}
	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CBoothWnd::OnLeftButtonDown(int iX, int iY)
{
	if(IsInGoodGrid(iX,iY))		//点击中表格不纪录拖动
		return true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CBoothWnd::OnLeftButtonUp(int iX, int iY)
{

	if(m_bDClicked)
	{
		m_bDClicked = false;
		return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	}

	int X,Y;
	GetGoodGrid(&X,&Y,iX,iY);

	//表格位置正确
	if(X != -1 && Y != -1)
	{
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd != Package_Wnd 
			&& GoodFrom.eFromWnd != Booth_Wnd)
			return true;

		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Package_Wnd && 
			(GoodFrom.iWndPos<0 || GoodFrom.iWndPos >= MAX_PACKAGE_ELEMENT-6))
			return true;

		//将包裹中物品放入摊位中

		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Package_Wnd)
		{
			//物品还是放到原来的位置
			//SELF.GetPackageGood(GoodFrom.iWndPos) = GoodFrom.DropGood;
			SELF.PackageGood().BackToArray(GoodFrom.DropGood,GoodFrom.iWndPos);

			if(GoodFrom.DropGood.IsBind()) //绑定物品不能放到摊位上
			{
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				g_TalkMgr.PushSysTalk("绑定的物品不能摆摊");

				return true;
			}

			if (!g_BoothData.IsInBooth(GoodFrom.DropGood.GetID(),GoodFrom.DropGood.GetName()))
			{

				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);

				int nPos = -1;
				for(int ig=0;ig<MAX_BOOTH_GOODS_NUM;ig++)
				{
					if(g_BoothData.GetPrice(ig).GetID()==0)
					{
						nPos = ig;
						break;
					}
				}


				if (nPos >= 0 && nPos < g_BoothData.GetMaxGoodNum())
				{
					g_BoothData.SetReadyGood(GoodFrom.DropGood.GetID(),
						GoodFrom.DropGood.GetName(),GoodFrom.iWndPos);
					GoodFrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(GoodFrom);

					//弹出输入价格的对话框
					g_pControl->PopupWindow(MSG_CTRL_BOOTH_ADD_GOOD,OPER_CREATE,MSG_CTRL_BOOTH_ADD_GOOD);
				}
				else
				{
					GoodFrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(GoodFrom);

					g_MsgBoxMgr.PopSimpleAlert("摊位已满，无法添加货物！");
				}

				return true;
			}
			GoodFrom.DropGood.SetID(0);
			CGoodGrid::SetDropGoodFrom(GoodFrom);
			return true;
		}


		//将东西放到摊位上

		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Booth_Wnd)
		{
			int nPos = -1;
			for(int ig = 0; ig<MAX_BOOTH_GOODS_NUM;ig++)
			{
				if (g_BoothData.GetPrice(ig).GetID() == 0)
				{
					nPos = ig;
					break;
				}
			}
			if(nPos>=0)
			{
				g_BoothData.GetPrice(nPos) = g_BoothData.GetDropPrice();
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				g_BoothData.GetDropPrice().clear();

				return true;
			}
		}


		//取出摊位上的东西

		CGood temp;
		GoodPrice_t price;
		unsigned long nGoodID = g_BoothData.GetPrice(Y*m_iWCells+X).GetID();
		if (nGoodID != 0)
		{
			for (int i=0; i< MAX_PACKAGE_ELEMENT-6; i++)
			{
				CGood tempGood = SELF.GetPackageGood(i);
				if (nGoodID == tempGood.GetID())
				{
					temp = tempGood;
					break;
				}
			}
			if (temp.GetID() != 0)
				price = g_BoothData.GetPrice(Y*m_iWCells+X);

			g_BoothData.GetPrice(Y*m_iWCells+X).SetID(0);
		}

		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = Booth_Wnd;
		GoodFrom.iWndPos = Y * m_iWCells + X;
		CGoodGrid::SetDropGoodFrom(GoodFrom);
		g_BoothData.GetDropPrice() = price;


		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CBoothWnd::OnMouseMove(int iX, int iY)
{

	int w = iX - m_ptGoodPoint.x;
	int h = iY - m_ptGoodPoint.y;
    CParserTip *pTip = g_pControl->GetTipWnd();

	//移动
	CGood* pTalkGood = NULL;
	//if(m_pTalkContainer->ClickMove(iX,iY,pTalkGood))
	//{
	//	if(pTalkGood)
	//	{
	//		pTip->Parse(*pTalkGood);

	//		int x = m_iScreenX + iX + 10;
	//		int y = m_iScreenY + iY + 10;
	//		pTip->AdjustXY(x,y);
	//		pTip->Move(x,y);
	//		pTip->SetShow(true);
	//	}
	//	else
	//	{
	//		pTip->SetShow(false);
	//	}
	//	return true;
	//}

	int X,Y;
	CGood temp;
	GoodPrice_t price;

	GetGoodGrid(&X,&Y,iX,iY);

	if(Y * m_iWCells + X >= g_BoothData.GetMaxGoodNum())
	{
		pTip->Clear();
		pTip->AddText("双击此处可付费开启");

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	if(X != -1 && Y != -1)
	{

		price =g_BoothData.GetPrice(Y * m_iWCells + X);
		if (price.GetID() > 0)
		{
			temp.SetID(0);
			for (int i = 0; i < MAX_PACKAGE_ELEMENT;i++)
			{
				CGood& tempGood = SELF.GetPackageGood(i);
				if (price.GetID() == tempGood.GetID())
				{
					temp  = tempGood;
					break;
				}
			}
		}
		else
		{
			temp.SetID(0);
		}

	}

	if(X != -1 && Y != -1 && temp.GetID()!=0)
	{

		pTip->Clear();
		pTip->ParseWithPrice(temp,price);
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;

	}
	else
	{
		temp.SetID(0);
		pTip->SetShow(false);

	}
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CBoothWnd::OnLeftButtonDClick( int iX,int iY )
{
	int X = 0,Y = 0;
	GetGoodGrid(&X,&Y,iX,iY);
	if(Y * m_iWCells + X >= g_BoothData.GetMaxGoodNum())
	{
		g_MsgBoxMgr.PopOkCancelBox("    本次支付1元宝,您将可以开启4个摊位空格，并获得永久使用权。是否确认开启？",MSG_CTRL_BOOTH,0);
		return true;
	}

	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}