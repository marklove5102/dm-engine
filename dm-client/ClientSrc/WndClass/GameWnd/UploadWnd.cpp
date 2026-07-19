/*#include "uploadwnd.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/WooolStoreData.h"
#include "GameData/TalkMgr.h"
#include "GameClient/WidgetManager.h"
#include "GameData/OtherData.h"
#include "GameClient/IGWLoaderDx9.h"

INIT_WND_POSX(CUploadWnd,POS_VARIABLE,POS_VARIABLE)

CUploadWnd::CUploadWnd(void)
{
	m_iWCells = 5;
	m_iHCells = 2;

	m_iCellW  = 39;
	m_iCellH  = 35;

	m_iCellDisW = 2;
	m_iCellDisH = 1;

	if(MAX_PACKAGE_ELEMENT == 66)
	{
		m_ptHeadPoint.x = 342;
		m_ptHeadPoint.y = 140;

		m_iWHeads  = 10;
		m_iHHeads  = 6;
	}
	else
	{
		m_ptHeadPoint.x = 376;
		m_ptHeadPoint.y = 140;

		m_iWHeads  = 8;
		m_iHHeads  = 5;
	}


	m_bDClicked = false;

	m_ptGoodPoint.x = 92;
	m_ptGoodPoint.y = 141;

	m_pTalkViewer = NULL;      

	m_pLeaveStore = NULL;
	m_pSNDAJiFen = NULL;
	m_pClose = NULL;

	if(g_dwServerSwitch & SS_CREDIT)
	{
		m_iIndex = 4930;
	}
	else
	{
		m_iIndex = 4960;
	}

	m_iPages = 1;

	m_iAlignType = XAL_TOPLEFT;

	g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CLOSE);
	if(!g_pControl->GetWindowPos(m_iOffX,m_iOffY,"WooolStoreWnd"))
	{
		m_iOffX = (g_pGfx->GetWidth() - 800) / 2;
		m_iOffY = ((g_pGfx->GetWidth() > 800))?40:0;
	}

	m_bNeedSavePos = false;
}

CUploadWnd::~CUploadWnd(void)
{
	g_pControl->SetWindowPos(m_iScreenX,m_iScreenY,"WooolStoreWnd");
}

void CUploadWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
	return;
	}

	if(MAX_PACKAGE_ELEMENT == 66)
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,4982);
		if(pTex)
		{
			g_pGfx->DrawTextureNL(m_iScreenX+330,m_iScreenY+72,pTex);
		}
	}

	char str[256] = {0};
	sprintf(str,"[%s]的拍卖行",SELF.GetName());

	g_pFont->DrawText(m_iScreenX+184-strlen(str)*3,m_iScreenY+108,str,0xFFFFFFFF);


	//绘制物品栏内物品
	for(int nh = 0; nh < m_iHCells; nh++)
	{
		for(int nw = 0; nw < m_iWCells; nw++)
		{
			int iPos = nh*m_iWCells+nw;
			if(iPos < 0 || iPos >= MAX_UPLOAD_GOODS_NUM)
				continue;

			//物品
			CGood temp = g_PetBoothData.GetC2CGood(iPos);

			if (temp.GetID() == 0)
				continue;

			int x = m_iScreenX + m_ptGoodPoint.x + nw * (m_iCellW + m_iCellDisW) + 3;
			int y = m_iScreenY + m_ptGoodPoint.y + nh * (m_iCellH + m_iCellDisH) + 2;

			if(g_pControl) //绘制单个物品
			{
				CGoodGrid::DrawOneGood(x+16,y+16,temp);
			}
		}
	}

	for(int nh = 0; nh < m_iHHeads; nh++)
	{
		for(int nw = 0; nw < m_iWHeads; nw++)
		{
			int iPos = nh*m_iWHeads+nw;
			if(iPos < 0 || iPos >= MAX_PACKAGE_ELEMENT - 6)
				continue;

			//物品
			CGood temp = SELF.GetPackageGood(iPos);

			if (temp.GetID() == 0)
				continue;

			int x = m_iScreenX + m_ptHeadPoint.x + nw * m_iCellW;
			int y = m_iScreenY + m_ptHeadPoint.y + nh * m_iCellH;


			//如果处于摆摊状态，则背景为红色
			if(g_BoothData.IsInBooth(temp.GetID(),temp.GetName()))
			{
				if(SELF.GetBoothState())
					g_pGfx->DrawFillRect(x+1,y+1,m_iCellW-2, m_iCellH-2,0x44FF0000);
				else
					g_pGfx->DrawFillRect(x+1,y+1,m_iCellW-2, m_iCellH-2,0x4400FF00);
			}
			if(g_pControl) //绘制单个物品
			{
				CGoodGrid::DrawOneGood(x+20,y+17,temp);
			}
		}
	}

	DrawSysInfo();
}

void CUploadWnd::OnCreate()
{
	m_pUploadButton = new CCtrlButton();
	AddControl(m_pUploadButton);
	m_pUploadButton->CreateEx(this,88,247,4966,4967,4968);

	m_pGoButton = new CCtrlButton();
	AddControl(m_pGoButton);
	m_pGoButton->CreateEx(this,200,247,4963,4964,4965);

	//聊天显示框
	m_pTalkViewer = new CTalkViewer();
	AddControl(m_pTalkViewer);
	m_pTalkViewer->Create(this,95,274,196,65);
	m_pTalkViewer->AddScroll(181,19,16,30,4629);
	m_pTalkViewer->AddUpButton(179,0,4626,4627,4628);
	m_pTalkViewer->AddDownButton(179,48,4623,4624,4625);
	m_pTalkViewer->SetTalkType(&g_TalkMgr.GetPaiMaiTalk());

	m_pSNDAJiFen = new CCtrlButton();
	AddControl(m_pSNDAJiFen);
	m_pSNDAJiFen->CreateEx(this, 43, 418, 0, 10471, 10472);

	// 我要冲值按钮冲值
	m_pAddYuanBao = new CCtrlButton();
	AddControl(m_pAddYuanBao);
	if(g_dwServerSwitch & SS_CREDIT)
	{
		m_pAddYuanBao->CreateEx(this, 528, 419, 10407, 10408, 10409);
	}
	else
	{
		m_pAddYuanBao->CreateEx(this, 394, 419, 10407, 10408, 10409);
	}

	// 退出商城按钮
	m_pLeaveStore = new CCtrlButton();
	AddControl(m_pLeaveStore);
	m_pLeaveStore->CreateEx(this, 700, 419, 10410, 10411, 10412);

	// 关闭按钮
	m_pClose = new CCtrlButton();
	AddControl(m_pClose);
	m_pClose->CreateEx(this, 782, 2, 10492, 10493, 10494) ;

}

bool CUploadWnd::OnLeftButtonDown(int iX, int iY)
{
	m_bDClicked = false;

	if(iX > 10 && iX < 32)
	{
		if(iY > 9 && iY < 100)
		{
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,2);
			return true;
		}
		else if(iY >= 100 && iY < 190)
		{
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,5);
			return true;
		}
		else if(iY >= 190 && iY < 270)
		{
			if(g_PetBoothData.IsPaimaiClosed())
				return true;

			g_WidgetMgr.LoginPaiMai();
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);
			return true;
		}
		else if(iY > 360 && iY < 450)
		{
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,8);
			return true;
		}
	}

	if(IsInGoodGrid(iX,iY))
		return true;

	int X = -1,Y = -1;
	GetPackageGrid(&X,&Y,iX,iY);
	if(X != -1 && Y != -1)
		return true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CUploadWnd::OnLeftButtonUp(int iX, int iY)
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
			&& GoodFrom.eFromWnd != Upload_Wnd)
			return true;

		//将包裹中物品放入摊位中

		if(GoodFrom.DropGood.GetID() != 0)
		{
			if(GoodFrom.eFromWnd == Package_Wnd && GoodFrom.DropGood.IsBind()) //绑定物品不能放到摊位上
			{
				//放回包裹
				SELF.GetPackageGood(GoodFrom.iWndPos) = GoodFrom.DropGood;

				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				g_TalkMgr.PushSysTalk("绑定的物品不能通过拍卖行出售"); 
				return true;
			}

			//放物品音乐////////////////////////////
			int i = GoodFrom.DropGood.GetStdMode()+1;
			g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);

			if(GoodFrom.eFromWnd == Package_Wnd)
			{
				int ii = 0;
				for(ii = 0;ii < MAX_UPLOAD_GOODS_NUM;ii++)
				{
					if(g_PetBoothData.GetC2CGood(ii).GetID() == 0)
					{
						break;
					}
				}

				if(ii == MAX_UPLOAD_GOODS_NUM)
				{
					//放回包裹
					SELF.GetPackageGood(GoodFrom.iWndPos) = GoodFrom.DropGood;
					GoodFrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(GoodFrom);
					return true;
				}

				//放回包裹
				SELF.GetPackageGood(GoodFrom.iWndPos) = GoodFrom.DropGood;

				//设置需要设计物品的价格
				g_BoothData.SetReadyGood(GoodFrom.DropGood.GetID(),
					GoodFrom.DropGood.GetName(),GoodFrom.iWndPos);

				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);

				g_pControl->PopupWindow(MSG_CTRL_BOOTH_ADD_GOOD,OPER_CREATE,MSG_CTRL_UPLOAD_ADD_GOOD);
				return true;
			}
			else
			{
				//将物品放下来
				for(int ii = 0;ii < MAX_UPLOAD_GOODS_NUM;ii++)
				{
					if(g_PetBoothData.GetC2CGood(ii).GetID() == 0)
					{
						g_PetBoothData.GetC2CGood(ii) = GoodFrom.DropGood;
						g_PetBoothData.GetPrice(ii) = g_BoothData.GetDropPrice();
						GoodFrom.DropGood.SetID(0);
						CGoodGrid::SetDropGoodFrom(GoodFrom);
						g_BoothData.GetDropPrice().clear();
						return true;
					}
				}
			}

			return true;
		}


		//取出摊位上的东西

		int iPos = Y*m_iWCells+X;
		CGood temp = g_PetBoothData.GetC2CGood(iPos);
		if(temp.GetID() == 0)
			return true;

		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = Upload_Wnd;
		GoodFrom.iWndPos = iPos;
		CGoodGrid::SetDropGoodFrom(GoodFrom);
		g_PetBoothData.GetC2CGood(iPos).SetID(0);
		g_BoothData.GetDropPrice() = g_PetBoothData.GetPrice(iPos);
		g_PetBoothData.GetPrice(iPos).clear();



		return true;
	}

	GetPackageGrid(&X,&Y,iX,iY);

	//表格位置正确
	if(X != -1 && Y != -1)
	{
		if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE)
			return true;

		int iPos = Y*m_iWHeads+X;
		if(iPos < 0 || iPos >= MAX_PACKAGE_ELEMENT)
			return true;

		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

		if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd != Upload_Wnd && GoodFrom.eFromWnd != Package_Wnd))
			return true;

		//////////////////////////////////////////////////////////
		//摆摊物品不能使用
		CGood tempGood = SELF.GetPackageGood(iPos);

		if(SELF.GetBoothState() && g_BoothData.IsInBooth(tempGood.GetID(),tempGood.GetName()))
			return true;

		//从包裹中取出东西
		CGood temp = SELF.GetPackageGood(iPos);

		if(GoodFrom.DropGood.GetID() != 0)
		{
			//从上传栏目拿回时候，需要将上传栏目压缩
			if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Upload_Wnd)
			{
				g_PetBoothData.C2CFullFill();
				g_BoothData.GetDropPrice().clear();
			}

			SELF.GetPackageGood(iPos) = GoodFrom.DropGood;
			GoodFrom.DropGood = temp;
			GoodFrom.eFromWnd = Package_Wnd;
			GoodFrom.iWndPos = iPos;
			CGoodGrid::SetDropGoodFrom(GoodFrom);
		}
		else
		{
			if(g_pInput->IsShift())  //按物品
			{
				g_pControl->Msg(MSG_CTRL_INSERT_OBJECTLINK, 0,(CControl*)(&temp));
			}
			else
			{
				GoodFrom.DropGood = temp;
				GoodFrom.eFromWnd = Package_Wnd;
				GoodFrom.iWndPos = iPos;
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				SELF.GetPackageGood(iPos).SetID(0);
			}
		}
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CUploadWnd::OnMouseMove(int iX, int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->SetShow(false);
	pTip->Clear();
	if(iX > 43 && iX < 247 && iY > 420 && iY<438)
	{
		char strTemp[256]="  盛大积分是通过向盛大游戏等应用充值后所获\n"
			"  拥有盛大积分，即可参加丰富礼品兑换、积分\n"
			"  活动、淘金游戏等多彩内容！";

		pTip->AddText(strTemp,0xFFFFFFFF,-1);
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	if(g_dwServerSwitch & SS_CREDIT && iX > 451 && iX < 520 && iY > 423 && iY < 437)
	{
		char strTemp[256]={0};

		if (SELF.GetCreditState() == 0)//未开通信用
		{
			sprintf(strTemp,"信用额度%d信用点，等值于%.2f元宝",SELF.GetCreditMax(),(float)(SELF.GetCreditMax()) / 100);
			pTip->AddText(strTemp,0xFFFFFFFF,-1);
			pTip->AddText("您尚未开通信用服务，进入圈圈信用服务可以申请开通",0xFFFFFFFF,-1);
		}
		else if (SELF.GetCredit() > 0)//有可用信用额度
		{
			sprintf(strTemp,"可透支额为%d信用点，等值于%.2f元宝",SELF.GetCredit(),(float)(SELF.GetCredit())/100);
			pTip->AddText(strTemp,0xFFFFFFFF,-1);
		}
		else// if (SELF.GetCredit() == 0)
		{
			if (SELF.GetCreditArrearage() > 0)//有欠款
			{
				pTip->AddText("可透支额为0信用点，请及时充值还款",0xFFFFFFFF,-1);
			}
			else
			{
				pTip->AddText("手机未认证，可用透支额已被冻结，请进入圈圈信用服务进行认证",0xFFFFFFFF,-1);
			}
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	int X,Y;

	GetGoodGrid(&X,&Y,iX,iY);
	if(X != -1 && Y != -1)
	{
		CGood temp = g_PetBoothData.GetC2CGood(Y*m_iWCells + X);
		GoodPrice_t price = g_PetBoothData.GetPrice(Y*m_iWCells + X);

		if(temp.GetID() != 0)
		{
			pTip->ParseWithPrice(temp,price); 
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
		GetPackageGrid(&X,&Y,iX,iY);
		if(X != -1 && Y != -1)
		{
			CGood temp = SELF.GetPackageGood(Y*m_iWHeads + X);
			if(temp.GetID() != 0)
			{
				pTip->Parse(temp);            
				int x = m_iScreenX + iX + 10;
				int y = m_iScreenY + iY + 10;
				pTip->AdjustXY(x,y);          
				pTip->Move(x,y);
				pTip->SetShow(true);          
				return true;
			}
		}
	}
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

//取得点所在的格子,(ix,iy)是相对本窗口的坐标
void CUploadWnd::GetGoodGrid(int* iX, int* iY, int ix, int iy)
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
bool CUploadWnd::IsInGoodGrid(int ix, int iy)
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

void CUploadWnd::GetPackageGrid(int* iX, int* iY, int ix, int iy)
{
	int w = ix - m_ptHeadPoint.x;
	int h = iy - m_ptHeadPoint.y;

	*iX = w / m_iCellW;
	*iY = h / m_iCellH;

	if(*iX >= m_iWHeads || *iX < 0 || *iY >= m_iHHeads || *iY < 0)
	{
		*iX = -1;
		*iY = -1;
	}
}

bool CUploadWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pUploadButton)
		{
			int ii = 0;
			for(ii = 0;ii < MAX_UPLOAD_GOODS_NUM;ii++)
			{
				if(g_PetBoothData.GetC2CGood(ii).GetID() != 0)
					break;
			}

			if(ii == MAX_UPLOAD_GOODS_NUM)
				return true;

			g_WooolStoreMgr.SetWooolStorePage(WOOOLSTORE_PAIMAI);
			g_WooolStoreMgr.SetPaiMaiStoreType(4);
			g_WidgetMgr.MoveWindow(EWT_FIRST,1,1,3,3);
			g_WidgetMgr.LoginPaiMai();

			return true;
		}
		else if(pControl == m_pGoButton)
		{
			if(g_PetBoothData.IsPaimaiClosed()) //判断是否关闭了拍卖行
				return true;

			//拍卖行
			g_WooolStoreMgr.SetWooolStorePage(WOOOLSTORE_PAIMAI);
			g_WooolStoreMgr.SetPaiMaiStoreType(5);
			g_PetBoothData.SetRedirectPage("upload@#$%^");
			g_WidgetMgr.LoginPaiMai();
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);
			return true;
		}
		else if (pControl == m_pClose) 
		{// 离开商店
			g_WooolStoreMgr.SetWooolStorePage(-1);  
			g_pControl->PopupWindow(MSG_CTRL_UPLOAD_WND,OPER_CLOSE);
			return true;
		}
		else if (pControl == m_pAddYuanBao)
		{
			// 增加点券
			//SwitchSubPage(WOOOLSTORE_PAY);
			if(g_pSDOAInterface)
			{
				if(SDOA_FALSE == g_pSDOAInterface->OpenWidgetA("sdoNewPay"))
				{
					g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,6);
				}
			}
			else
			{			
				g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,6);
			}
			return true;
		}
		else if (pControl == m_pLeaveStore) 
		{// 离开商店
			g_WooolStoreMgr.SetWooolStorePage(-1);
			g_pControl->PopupWindow(MSG_CTRL_UPLOAD_WND,OPER_CLOSE);
			return true;
		}
		else if(pControl == m_pSNDAJiFen)
		{
			//打开商城积分
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,4);
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CUploadWnd::DrawSysInfo() //绘制盛大积分，元宝等信息
{
	char szTemp[128] = {0};

	// 绘制元宝数量
	DWORD lYuanBao = SELF.GetYuanBao(); 

	sprintf(szTemp, "%d", lYuanBao);
	g_pFont->DrawText(m_iScreenX + 332, m_iScreenY + 423, szTemp, -1);

	// 绘制盛大积分
	sprintf(szTemp, "%d", SELF.GetSndaMark()); 
	g_pFont->DrawText(m_iScreenX + 130, m_iScreenY + 423, szTemp, -1);


	//信用点
	if(g_dwServerSwitch & SS_CREDIT)
	{
		sprintf(szTemp, "%d/%d", SELF.GetCredit(),SELF.GetCreditMax()); 
		g_pFont->DrawText(m_iScreenX + 451, m_iScreenY + 423, szTemp, -1);		
	}
	else// 绘制滚动的提示信息
	{
		static DWORD isTime = 0;
		DWORD dwCount = GetTickCount();
		if(isTime == 0)
			isTime = dwCount;

		int iLine;
		const char* pTextShow = NULL;

		if (5000 != 0 && g_WooolStoreMgr.GetScrollTextLines() != 0)
		{
			iLine = ((dwCount - isTime) / 5000) % g_WooolStoreMgr.GetScrollTextLines();
			pTextShow = g_WooolStoreMgr.GetScrollTextContent(iLine);
			if (pTextShow != NULL)
				g_pFont->DrawText(m_iScreenX + 482, m_iScreenY + 424, pTextShow, -1);
		}
	}

}
*/