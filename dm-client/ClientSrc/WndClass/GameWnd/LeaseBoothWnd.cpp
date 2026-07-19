#include "LeaseBoothWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameData/LoginData.h"
#include "GameData/TalkMgr.h"
#include "GameData/BoothData.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/NpcData.h"

INIT_WND_POSX(CLeaseBoothWnd,POS_VARIABLE,POS_VARIABLE)

CLeaseBoothWnd::CLeaseBoothWnd(void):
m_bFistTimeUse(false)
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

	m_iPages = 1;

	m_iAlignType = XAL_TOPLEFT;

	m_pStartButton = NULL;
	m_pEndButton = NULL;

	m_dwtype = sm_dwWindowType;
	
	if (m_dwtype == 0)//无人租
	{
		m_iIndex = 4610;
		if((g_pGfx->GetWidth() > 800))
		{
			m_iOffX = 400;
			m_iOffY = 50;
		}
		else
		{
			m_iOffX = 300;
			m_iOffY = 50;
		}
	}
	else if (m_dwtype == 1)//购买后第一次打开,可以一次添加多个物品
	{
		m_iIndex = 4612;
		m_bFistTimeUse = true;
		g_OtherBoothData.SetLeaseBoothWndOpen(true);
		m_iOffX = 100;
		m_iOffY = 50;
	}
	else if (m_dwtype == 2)//自己租
	{
		m_iIndex = 4612;
		g_OtherBoothData.SetLeaseBoothWndOpen(true);
		m_iOffX = 100;
		m_iOffY = 50;
	}
}

CLeaseBoothWnd::~CLeaseBoothWnd(void)
{
	g_OtherBoothData.SetLeaseBoothWndOpen(false);
}

void CLeaseBoothWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	//
	//太远了，关闭窗口
	if(!g_pGameData->FindSimpleCharacter(g_OtherBoothData.GetOtherPlayerID()))
	{
		CloseWindow();
		return;
	}

	g_pFont->DrawText(120+m_iScreenX, 11+m_iScreenY, "商 铺", COLOR_TEXT_NORMAL, FONT_YAHEI, FONTSIZE_BIG);	

	if (m_dwtype == 0)
	{
		g_pFont->DrawText(106 + m_iScreenX, 275 + m_iScreenY, "商铺信息", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
		g_pFont->DrawText(52 + m_iScreenX, 310 + m_iScreenY, "请选择要租赁的商铺类型", COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
		return;
	}
	g_pFont->DrawText(45 + m_iScreenX, 46 + m_iScreenY, "当前所有人:", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
	g_pFont->DrawText(m_iScreenX + 135,m_iScreenY + 46,g_OtherBoothData.GetBoothName(), COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
	char tempstr[256]={0};
	sprintf(tempstr,"距离租赁到期还剩%u天!",g_OtherBoothData.GetDaysRemain());
	g_pFont->DrawText(70 + m_iScreenX, 335 + m_iScreenY, tempstr, COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
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
			
			if(g_pControl) //绘制单个物品
			{
				CGoodGrid::DrawOneGood(x+20,y+18,temp);
			}
		}        
	}
}

bool CLeaseBoothWnd::Create(CControl* pParent,int iCurPage)
{
	if (g_Login.GetLoginInExpType() == 1)
	{
		g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_RECREATE);
		return false;
	}

	return CCtrlWindowX::Create(pParent, iCurPage);
}

void CLeaseBoothWnd::OnCreate()
{
	//复用全局变量g_boothData
	//与boothwnd是互斥的,应在打开之前,关闭它
	{
		g_pControl->PopupWindow(MSG_CTRL_BOOTH_WND,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_OTHER_BOOTH,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_LEASEOTHERBOOTH_WND,OPER_CLOSE);

		if (m_dwtype == 2)
		{
			g_pControl->SetWindowPos((g_pGfx->GetWidth() > 800)?400:360,50,"PackageWnd");
			g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_RECREATE);
		}
	}

	SetCloseButton(251,4,80);

	if (m_dwtype == 0)
	{
		//菜单
		m_pMType = new CCtrlMenuButton();
		AddControl(m_pMType);
		m_pMType->Create(this,52, 310 + 18,170,110);
		m_pMType->SetMenuTextColor(0xFFFFFFFF);
		m_pMType->SetFont(FONT_YAHEI);

		vector<S_LeaseBoothType> &VType = g_OtherBoothData.GetLeaseBoothTypeVector();
		char szTemp[128] = {0};
		for (int i = 0; i < VType.size(); i++)
		{
			sprintf(szTemp,"租期%d天,费用%d元宝",VType[i].wDays,VType[i].wPrice);
			m_pMType->AddString(szTemp);
		}

		m_pMType->SetCurSel(0);


		m_pStartButton = new CCtrlButton();
		AddControl(m_pStartButton);
		//m_pStartButton->CreateEx(this,95,170,4611,4612,4613);
		m_pStartButton->CreateEx(this,45,408,90,91,92);
		m_pStartButton->SetText("确认租赁",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		m_pEndButton = new CCtrlButton();
		AddControl(m_pEndButton);
		//m_pEndButton->CreateEx(this,167,170,4614,4615,4616);
		m_pEndButton->CreateEx(this,165,408,90,91,92);
		m_pEndButton->SetText("取消租赁",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	}
}
bool CLeaseBoothWnd::OnLeftButtonDown(int iX, int iY)
{
	if(IsInGoodGrid(iX,iY))		//点击中表格不纪录拖动
		return true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}
bool CLeaseBoothWnd::OnLeftButtonUp(int iX, int iY)
{
	if(m_bDClicked)
	{
		m_bDClicked = false;
		return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	}

	//还没有租赁的摊位
	if (m_dwtype == 0)
	{
		return true;
	}

	int X,Y;
	GetGoodGrid(&X,&Y,iX,iY);

	//表格位置正确
	if(X != -1 && Y != -1)
	{
		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd != Package_Wnd 
			&& GoodFrom.eFromWnd != LeaseBooth_Wnd)
			return true;

		//将包裹中物品放入摊位中
		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Package_Wnd)
		{
			//物品还是放到原来的位置
			SELF.PackageGood().BackToArray(GoodFrom.DropGood,GoodFrom.iWndPos);

			if(GoodFrom.DropGood.IsBind()) //绑定物品不能放到摊位上
			{
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				g_TalkMgr.PushSysTalk("绑定的物品不能摆摊");
				return true;
			}

			if (!g_OtherBoothData.IsInBooth(GoodFrom.DropGood.GetID(),GoodFrom.DropGood.GetName()))
			{

				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);

				int nPos = -1;
				for(int ig=0;ig<MAX_BOOTH_GOODS_NUM;ig++)
				{
					if(g_OtherBoothData.GetGoods(ig).GetID()==0)
					{
						nPos = ig;
						break;
					}
				}


				if (nPos >= 0 && nPos < MAX_BOOTH_GOODS_NUM)
				{
					g_OtherBoothData.SetReadyGood(GoodFrom.DropGood.GetID(),
						GoodFrom.DropGood.GetName(),GoodFrom.iWndPos);
					GoodFrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(GoodFrom);

					//弹出输入价格的对话框
					if (m_bFistTimeUse)
					{
						g_pControl->PopupWindow(MSG_CTRL_BOOTH_ADD_GOOD,OPER_CREATE,MSG_CTRL_LEASEBOOTH_FIRSTTIME);
					} 
					else
					{
						g_pControl->PopupWindow(MSG_CTRL_BOOTH_ADD_GOOD,OPER_CREATE,MSG_CTRL_LEASEBOOTH_ADD_GOOD);
					}
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
		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == LeaseBooth_Wnd)
		{
			int nPos = -1;
			for(int ig = 0; ig<MAX_BOOTH_GOODS_NUM;ig++)
			{
				if (g_OtherBoothData.GetGoods(ig).GetID() == 0)
				{
					nPos = ig;
					break;
				}
			}
			if(nPos>=0)
			{
				g_OtherBoothData.GetGoods(nPos) = GoodFrom.DropGood;
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				return true;
			}
		}

		//取出摊位上的东西
		CGood temp = g_OtherBoothData.GetGoods(Y*m_iWCells+X);
		unsigned long nGoodID = temp.GetID();
		if (nGoodID != 0)
		{
			g_OtherBoothData.GetGoods(Y*m_iWCells+X).SetID(0);
		}

		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = LeaseBooth_Wnd;
		GoodFrom.iWndPos = Y * m_iWCells + X;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		//如果不是第一打开,就要把拿起的物品放入readybuygoods中
		if (!m_bFistTimeUse)
		{
			g_OtherBoothData.GetReadyBuy() = temp;
		}

		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}
bool CLeaseBoothWnd::OnMouseMove(int iX, int iY)
{
	CGood* pTalkGood = NULL;
	CParserTip *pTip = g_pControl->GetTipWnd();

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
bool CLeaseBoothWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{

	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pStartButton)//开始摆摊,需要发送协议,将物品列表发送给服务器
		{
			//
			if (m_dwtype == 0)
			{
				vector<S_LeaseBoothType> &VType = g_OtherBoothData.GetLeaseBoothTypeVector();
				int i = m_pMType->GetCurSel();

				if (i < 0 || i >= VType.size())
				{
					return true;
				}

				char szTemp[256]={0};
				sprintf(szTemp,"此商铺当前未被租赁.\n租赁商铺需%d元宝,租期为%d天.\n是否租赁此商铺?",VType[i].wPrice,VType[i].wDays);
				g_MsgBoxMgr.PopSimpleComfirm(szTemp,MSG_CTRL_LEASEBOOTH_LEASE_CONFIRM,i);
				return true;
			}

			bool bExistGoods = false;

			for(int i=0;i<MAX_BOOTH_GOODS_NUM;i++)
			{
				CGood& goods = g_OtherBoothData.GetGoods(i);
				if (0 == goods.GetID())
					continue;

				if(NULL != SELF.PackageGood().FindGood(goods.GetID()))
				{
					bExistGoods = true;
					break;
				}
			}

			if (!bExistGoods)
			{
				g_MsgBoxMgr.PopSimpleAlert("没有物品需要寄卖。");
				return true;
			}

			//SELF.GetMapPathFinder().SetOnRoute(0);//啥意思?
			g_pGameControl->Send_LeaseBooth_Start(g_NPC.GetID());

			return true;
		}
		else if(pControl == m_pEndButton)
		{
			CloseWindow();
			return true;
		}
		break;
	default:
		break;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

//取得点所在的格子,(ix,iy)是相对本窗口的坐标
void CLeaseBoothWnd::GetGoodGrid(int* iX, int* iY, int ix, int iy)
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
bool CLeaseBoothWnd::IsInGoodGrid(int ix, int iy)
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