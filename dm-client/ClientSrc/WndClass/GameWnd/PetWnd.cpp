#include "petwnd.h"
#include <time.h>
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Control/GoodGrid.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/WooolStoreData.h"
#include "GameData/TalkMgr.h"
#include "GameData/NpcData.h"
#include "GameAI/AIMgr.h"

#define PET_ADOPT_LINE_COUNT 5 //每页显示的行数
#define PET_END_ADOPTED_LINE_COUNT 5 //每页显示的行数
#define PET_CARRYBACK_LINE_COUNT 5
#define PET_FOSTERAGE_LINE_COUNT 5


INIT_WND_POSX(CPetWnd,POS_AUTO,POS_AUTO)


CPetWnd::CPetWnd(void)
{
	m_iClothPos = -1;
	m_iCurPage = 1;
	m_pGridFosterage = NULL;
	m_pGridCarryBack = NULL;

	m_pGridEndAdopt = NULL;
	m_pGridAdopt = NULL;
	m_iHorsePos = -1;
	m_dwCurUsing = 0;
	m_iCurUsingPos = -1;
	m_pMenu = NULL;

	m_pHelpBtn = NULL;
	string str = "<color=orange 有关灵兽的介绍：>\\";
	str += "1.<灵兽的功能/@@PetFunction>\\";
	str += "2.<灵兽的操作/@@PetOperator>\\";
	str += "3.<寄养灵兽/@@AdoptPet>\\";
	str += "4.<领养灵兽/@@FosteragePet>\\";
	str += "5.<凤凰探宝/@@Explore>\\";

	m_npcText.Clear();
	m_npcText.Parse(str,0,"\\",str.length());

	m_iCurPage = sm_dwWindowType;
	if(m_iCurPage < 1 || m_iCurPage > 4) 
		m_iCurPage = 1;

	switch (m_iCurPage)
	{
	case 1:
		m_iIndex = 14400;
		break;
	case 2:
		m_iIndex = 14440;
		break;
	case 3:
		m_iIndex = 14450;
	case 4:
		m_iIndex = 14460;
		break;
	default:
		break;
	}
	m_iPages = 1;

}

CPetWnd::~CPetWnd(void)
{
	g_PetData.SetCarrayBackPetNum(0);
	g_PetData.GetPetAdoptMap().clear();
	g_PetData.SetAdoptPetNum(9888888);
}

void CPetWnd::Draw(void)
{
	m_dwTickCount = GetTickCount();

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	switch (m_iCurPage)
	{
	case 1:
		DrawPetListWnd();
		break;
	case 2:
		DrawPetPkgWnd();
		break;
	case 3:
		DrawPetAdoptWnd();
		break;
	case 4:
		DrawPetFosterWnd();
		break;
	default:
		break;
	}

	DrawHelpMsg();
}

void CPetWnd::OnCreate()
{
	g_pGameControl->SEND_Pet_Info_Request();
	//关闭按钮
	SetCloseButton(392,7);
	RemoveBodyChildControl();
	g_pGameControl->SEND_Pet_Adopt_Request(2,0);

	m_pMarkViewer = new CMarkViewer(16,29);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,440,16,216,436);
	m_pMarkViewer->SetTagText(&m_npcText);
	m_pMarkViewer->SetEnable(false);
	m_pMarkViewer->SetShow(false);

	CreateWnd();
}

void CPetWnd::CreateWnd()
{
	m_strID = "";
	g_WooolStoreMgr.GetDropQuickItem().clear();

	m_pHelpBtn = new CCtrlButton();
	m_pHelpBtn->CreateEx(this,375,44,14429,14430,14431);
	AddControl(m_pHelpBtn);

	m_pCloseHelpBtn = new CCtrlButton();
	m_pCloseHelpBtn->CreateEx(this,420+218,7,259,257,258);
	AddControl(m_pCloseHelpBtn);
	m_pCloseHelpBtn->SetShow(false);


	WORD wBgTexID = 0;
	if(m_iCurPage ==  1)
	{
		int iDisY = 121;
		m_iHorsePos = -1;
		for(int i = 0;i<3;i++)
		{
			m_pReleaseExpBtn[i] = new CCtrlButton();
			m_pReleaseExpBtn[i]->CreateEx(this,326,143 + iDisY*i,14401,14402,14403);
			AddControl(m_pReleaseExpBtn[i]);

			m_pFollowBackBtn[i] = new CCtrlButton();
			m_pFollowBackBtn[i]->CreateEx(this,50,182 + iDisY*i,16727,16728,16729);
			AddControl(m_pFollowBackBtn[i]);
			m_pFollowBackBtn[i]->SetTips("灵兽只有在跟随的状态下才能发挥\n作用。在包裹中双击灵兽石，或者点此处\n的跟随均能让灵兽处于释放状态。");


			m_pFeedBtn[i] = new CCtrlButton();
			m_pFeedBtn[i]->CreateEx(this,165,182 + iDisY*i,16731,16732,16733);
			AddControl(m_pFeedBtn[i]);

			m_pFunc[i] = new CCtrlButton();
			m_pFunc[i]->CreateEx(this,280,182 + iDisY*i,16735,16736,16737);
			AddControl(m_pFunc[i]);

			m_pBeckon[i] = new CCtrlButton();
			m_pBeckon[i]->CreateEx(this,247,155 + iDisY*i,16780,16781,16782);
			AddControl(m_pBeckon[i]);
			m_pBeckon[i]->SetEnable(false);
			m_pBeckon[i]->SetShow(false);

			m_pArrow[i] = new CCtrlButton();
			m_pArrow[i]->CreateEx(this,300,172 + iDisY*i,16788,16789,16789);
			AddControl(m_pArrow[i]);
		}

		m_pMenu = new CCtrlMenuButtonWnd;
		m_pMenu->Create(this,0,0);
		AddControl(m_pMenu);
		m_pMenu->AddMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16746,16747);
		m_pMenu->AddMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751);
		m_pMenu->AddMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759);		
		m_pMenu->SetShow(false);

		m_pPetListUpBtn = new CCtrlButton();
		m_pPetListUpBtn->CreateEx(this,387,86,51,52,53);
		AddControl(m_pPetListUpBtn);

		m_pPetListDownBtn = new CCtrlButton();
		m_pPetListDownBtn->CreateEx(this,387,433,56,57,58);
		AddControl(m_pPetListDownBtn);

		m_pPetListScroll = new CCtrlScroll();
		m_pPetListScroll->Create(this, 387, 103, 16,329,54);
		m_pPetListScroll->SetStep(1);
		AddControl(m_pPetListScroll);

		wBgTexID = 14400;

		UpdatePetList();

	}else if(m_iCurPage == 2)
	{
		if(!g_WooolStoreMgr.IsHaveRequestPetQuick())
		{
			g_pGameControl->SEND_PT_Store_Info(10);
		}
		//扩充包裹暂时不用
		//m_pExtendPkgBtn = new CCtrlButton();
		//m_pExtendPkgBtn->CreateEx(this,281,418,14441,14442,14443);
		//AddControl(m_pExtendPkgBtn);
		//pTex->LoadTex(3,14440);

		m_pMoreShorCut = new CCtrlButton();
		m_pMoreShorCut->CreateEx(this,330,140,14445,14446,14447);
		AddControl(m_pMoreShorCut);

		m_iWCells = 5;
		m_iHCells = MAX_PET_PACKAGE / 5;
		m_iCellW  = 44;
		m_iCellH  = 44;
		m_iCellDisW = 0;
		m_iCellDisH = 0;

		m_bDClicked = false;
		m_ptGoodPoint.x = 103;
		m_ptGoodPoint.y = 265;

		wBgTexID = 14440;

		m_QuickItem.clear();

	}else if(m_iCurPage == 3)
	{
		wBgTexID = 14450;

		m_LastSendTick = 0;
		m_nWaitReading = 0;
		m_bSending = false;

		m_pGridEndAdopt = new CCtrlGrid();
		AddControl(m_pGridEndAdopt);
		m_pGridEndAdopt->Create(this,28,117,343,249,5,22.60f);
		m_pGridEndAdopt->SetTextColor(0xffffffff,0xffff0000);
		m_pGridEndAdopt->SetSelTextColor(0xFFFFFF00);
		m_pGridEndAdopt->SetSelBackColor(0xFF2F8405);
		m_pGridEndAdopt->AddUpButton(350,0,51,52,53);
		m_pGridEndAdopt->AddDownButton(350,118,56,57,58);
		m_pGridEndAdopt->AddScroll(350,17,16,100,54);
		m_pGridEndAdopt->PushColumn(30);
		m_pGridEndAdopt->PushColumn(92);
		m_pGridEndAdopt->PushColumn(46);
		m_pGridEndAdopt->PushColumn(40);
		m_pGridEndAdopt->PushColumn(86);
		m_pGridEndAdopt->SetLinesPerPage(5);
		m_pGridEndAdopt->SetTotalCount(5);



		m_pGridAdopt = new CCtrlGrid();
		AddControl(m_pGridAdopt);
		m_pGridAdopt->Create(this,28,290,393,422,5,22.60f);
		m_pGridAdopt->SetTextColor(0xffffffff,0xffff0000);
		m_pGridAdopt->SetSelTextColor(0xFFFFFF00);
		m_pGridAdopt->SetSelBackColor(0xFF2F8405);
		m_pGridAdopt->AddUpButton(350,0,51,52,53);
		m_pGridAdopt->AddDownButton(350,118,56,57,58);
		m_pGridAdopt->AddScroll(350,17,16,100,54);
		m_pGridAdopt->PushColumn(30);
		m_pGridAdopt->PushColumn(92);
		m_pGridAdopt->PushColumn(46);
		m_pGridAdopt->PushColumn(40);
		m_pGridAdopt->PushColumn(86);
		m_pGridAdopt->SetLinesPerPage(5);
		m_pGridAdopt->SetTotalCount(5);

		for(int i = 0;i< 5;i++)
		{
			m_pEndAdoptBtn[i] = new CCtrlButton();
			m_pEndAdoptBtn[i]->CreateEx(this,326,133 + i* 24,14455,14456,14457);
			AddControl(m_pEndAdoptBtn[i]);
			m_pEndAdoptBtn[i]->SetShow(false);


			m_pAdoptBtn[i] = new CCtrlButton();
			m_pAdoptBtn[i]->CreateEx(this,326,305 + i*24,14451,14452,14453);
			AddControl(m_pAdoptBtn[i]);
			m_pAdoptBtn[i]->SetShow(false);

		}

		g_pGameControl->SEND_Pet_Adopt_Request(1,MAKELONG(0,20));//重新请求可以领养的宠物数据,协议头前4字节:起始序号(2字节)终止序号(2字节)

	}
	else if(m_iCurPage == 4)
	{
		wBgTexID = 14460;

		m_pGridFosterage = new CCtrlGrid();
		AddControl(m_pGridFosterage);
		m_pGridFosterage->Create(this,28,290,393,422,PET_FOSTERAGE_LINE_COUNT,22.60f);
		m_pGridFosterage->SetTextColor(0xffffffff,0xffff0000);
		m_pGridFosterage->SetSelTextColor(0xFFFFFF00);
		m_pGridFosterage->SetSelBackColor(0xFF2F8405);
		m_pGridFosterage->AddUpButton(350,0,51,52,53);
		m_pGridFosterage->AddDownButton(350,118,56,57,58);
		m_pGridFosterage->AddScroll(350,17,16,100,54);
		m_pGridFosterage->PushColumn(30);
		m_pGridFosterage->PushColumn(92);
		m_pGridFosterage->PushColumn(46);
		m_pGridFosterage->PushColumn(40);
		m_pGridFosterage->PushColumn(86);
		m_pGridFosterage->SetLinesPerPage(PET_FOSTERAGE_LINE_COUNT);
		m_pGridFosterage->SetTotalCount(PET_FOSTERAGE_LINE_COUNT);


		m_pGridCarryBack = new CCtrlGrid();
		AddControl(m_pGridCarryBack);
		m_pGridCarryBack->Create(this,28,117,343,249,PET_FOSTERAGE_LINE_COUNT,22.60f);
		m_pGridCarryBack->SetTextColor(0xffffffff,0xffff0000);
		m_pGridCarryBack->SetSelTextColor(0xFFFFFF00);
		m_pGridCarryBack->SetSelBackColor(0xFF2F8405);
		m_pGridCarryBack->AddUpButton(350,0,51,52,53);
		m_pGridCarryBack->AddDownButton(350,118,56,57,58);
		m_pGridCarryBack->AddScroll(350,17,16,100,54);
		m_pGridCarryBack->PushColumn(30);
		m_pGridCarryBack->PushColumn(92);
		m_pGridCarryBack->PushColumn(46);
		m_pGridCarryBack->PushColumn(40);
		m_pGridCarryBack->PushColumn(86);
		m_pGridCarryBack->SetLinesPerPage(PET_FOSTERAGE_LINE_COUNT);
		m_pGridCarryBack->SetTotalCount(PET_FOSTERAGE_LINE_COUNT);

		for(int i = 0;i< 5;i++)
		{
			m_pFosterBtn[i] = new CCtrlButton();
			m_pFosterBtn[i]->CreateEx(this,326,305 + i*24,14461,14462,14463);
			AddControl(m_pFosterBtn[i]);
			m_pFosterBtn[i]->SetShow(false);
			m_pFosterBtn[i]->SetTips("你可以寄养灵兽，寄养的灵兽不会出现在\n灵兽列表中。只有饱食的灵兽才能寄养。");


			m_pEndFosterBtn[i] = new CCtrlButton();
			m_pEndFosterBtn[i]->CreateEx(this,326,133 + i* 24,14455,14456,14457);
			AddControl(m_pEndFosterBtn[i]);
			m_pEndFosterBtn[i]->SetShow(false);
			m_pEndFosterBtn[i]->SetTips("你可以通过“结束”按钮召回你的灵兽。");

		}
	}

	SetPageTex(0,PACKAGE_INTERFACE,wBgTexID);
}

void CPetWnd::OnRideBtnClicked(int iIndex)
{
	int iPos = m_pPetListScroll->GetPos();
	iPos += iIndex;
	if(iPos < 0 || iPos >= m_vectorPet.size())
		return;

	if(m_vectorPet[iPos] == 0x00FFFFFF)
	{
		//为马,且马的状态为释放出来跟随
		if(g_PetData.GetMyHorse().bStatus == 1)
		{
			g_pGameControl->SEND_Player_Ride_Horse(2,0);
			return;
		}else if(g_PetData.GetMyHorse().bStatus == 2)
		{
			g_pGameControl->SEND_Player_Ride_Horse();
			return;
		}
	}

	typedef map<DWORD,PET_SENDOUT> MapPet;
	MapPet::iterator it;
	it = g_PetData.GetSendOutPetList().find(m_dwPetID[iIndex]);

	if(it != g_PetData.GetSendOutPetList().end())
	{
		g_AIMgr.PlayerRidePet(it);	
	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("请先放出你的灵兽，然后再骑!");
	}
}

void CPetWnd::OnFeedPetUseAllFood(int iIndex)
{
	int iPos = -1;
	DWORD dwID = m_dwPetID[iIndex];

	typedef map<DWORD,PET_SENDOUT> MapPet;
	MapPet::iterator it;
	it = g_PetData.GetSendOutPetList().find(dwID);
	if(it != g_PetData.GetSendOutPetList().end())
	{
		PET_SENDOUT &SendOutPet = it->second;
		if(SendOutPet.dwPetItemID == 0)
			return;

		if(m_dwTickCount - m_dwLastCheckFeedTime < 500 )
			return;

		m_dwLastCheckFeedTime = m_dwTickCount;


		DWORD dwGoodID = 0;
		int iPos = 0;
		if(FeedPet(dwID))
			return;

		//没有任何豹粮，看有没有豹粮捆
		if(dwGoodID == 0)
		{
			for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
			{
				CGood& tmp = SELF.GetPackageGood(i);

				if(tmp.GetID() == 0)
					continue;

				if(tmp.GetStdMode() == 37 && tmp.GetShape() == 10) 
				{
					dwGoodID = tmp.GetID();
					iPos = i;
					break;
				}
			}
		}

		//什么都没有，触发快捷消费
		if(dwGoodID == 0)
		{
			g_WooolStoreMgr.SetQuickBuyItem("精制兽粮");
			CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

			if(QuickBuyData.pItem)
			{
				char cTemp[128]={0};
				sprintf(cTemp,"你包裹里没有粮食，					需要立即购买粮食并马上给灵兽喂食吗？单价%d元宝",QuickBuyData.pItem->iPrice);
				QuickBuyData.iUseAfterBuyType = 1;
				QuickBuyData.strMsg = cTemp;
				QuickBuyData.iType = 1;

				g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);
			}
			else
			{
				g_TalkMgr.PushSysTalk("你包裹里没有任何粮食，请到商城购买粮食后再给灵兽喂食");
			}

		}
		else 
		{
			//解包
			g_pGameControl->SEND_Use_Object(iPos);
			//bWaitingFeed = true;
		}
	}
}

bool CPetWnd::FeedPet(DWORD dwID)
{
	DWORD dwGoodID = 0;
	int iPos = 0;
	int iFoodType = 100;//,豹粮,传统豹粮为１，精制豹粮，极品豹粮为２,用来表示喂食的优先级

	for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
	{
		CGood& tmp = SELF.GetPackageGood(i);

		if(tmp.GetID() == 0)
			continue;

		if( tmp.GetStdMode() == 49 && tmp.GetShape() == 52) 
		{
			dwGoodID = tmp.GetID();
			iPos = i;
			break;
		}
		else if(tmp.GetStdMode() == 53 && tmp.GetShape() == 0)//精制豹粮,极品豹粮喂食时要往外扔
		{
			if(iFoodType > 2)
			{
				dwGoodID = tmp.GetID();
				iPos = i;
				iFoodType = 2;
			}
		}
	}

	if(dwGoodID == 0)
	{
		return false;
	}
	else 
	{
		CGood *pGood = SELF.GetUsingTemp().Add(SELF.GetPackageGood(iPos));
		if(pGood)
			pGood->SetPos(iPos);
		SELF.GetPackageGood(iPos).SetID(0);

		g_pGameControl->SEND_Feed_Pet_From_Interface(dwID,dwGoodID);

		return true;
	}
}

void CPetWnd::OnFollowBack(int iIndex)
{
	if(m_iHorsePos == iIndex)//马
	{
		if(!SELF.IsDead())
		{
			if(g_PetData.GetMyHorse().bStatus == 1)
			{
				g_pGameControl->SEND_Player_Ride_Horse(2,0);
				//return;
			}else if(g_PetData.GetMyHorse().bStatus == 2)
			{
				g_pGameControl->SEND_Player_Ride_Horse();
				//return;
			}

			//SAction * pAction = SELF.GetAction();
			//if( pAction->wAction==ACTION_STAND || pAction->wAction==ACTION_HSTAND )
			//	g_pGameControl->SEND_Player_Ride_Horse();
			return;
		}
	}
	else//其它宠物
	{
		int iPos = -1;
		for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
		{
			CGood &tmpGood = SELF.GetPackageGood(i);

			if(tmpGood.GetID() == m_dwPetID[iIndex])
			{
				iPos = i;
				break;
			}
		}

		if(iPos >= 0) 
		{
			g_pGameControl->SEND_Use_Object(iPos);

			//把UsingTemp中的物品马上放回包裹,否则会导致m_vectorPet重排
			int iUsingPos = SELF.GetUsingTemp().FindGoodPos(m_dwPetID[iIndex]);
			if(iUsingPos >= 0)
			{
				SELF.GetPackageGood(iPos) = SELF.GetUsingTemp().Get(iUsingPos);
				SELF.GetUsingTemp().DeleteGood(m_dwPetID[iIndex]);
			}

			m_dwCurUsing = m_dwPetID[iIndex];
			m_iCurUsingPos = iPos;
		}
	}
}

bool CPetWnd::DealPetListMsg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK && pControl)
	{
		if(pControl == m_pPetListUpBtn)
		{
			m_pPetListScroll->GoDown();
			return true;
		}else if(pControl == m_pPetListDownBtn)
		{
			m_pPetListScroll->GoUp();
			return true;
		}
		for(int i = 0;i<3;i++)
		{
			if(pControl == m_pFollowBackBtn[i])
			{
				OnFollowBack(i);
				return true;
			}else if(pControl == m_pFeedBtn[i])
			{
				OnFeedPetUseAllFood(i);
				return true;
			}else if(pControl == m_pReleaseExpBtn[i])
			{
				OnReleaseExp(i);
				return true;
			}			
			else if(pControl == m_pFunc[i] || pControl == m_pArrow[i])
			{
				OnClickMenuButton(i);
				return true;
			}
			else if(pControl == m_pBeckon[i])
			{
				DWORD dwID = m_dwPetID[i];
				map<DWORD,PET_SENDOUT>& mappet = g_PetData.GetSendOutPetList();
				map<DWORD,PET_SENDOUT>::iterator it = mappet.find(dwID);
				if(it != mappet.end())
				{
					if(it->second.szReserved[0] == 9)	//探宝,召回
					{	
						int iType = it->second.byPetType;

						if(iType == 24)
						{
							g_MsgBoxMgr.PopSimpleComfirm("你的赤翎凤正在探宝呢，你是否需要召回它呢？",MSG_CTRL_ZHAO_HUAN,dwID);
						}
						else if(iType == 25)
						{
							g_MsgBoxMgr.PopSimpleComfirm("你的傲天凤正在探宝呢，你是否需要召回它呢？",MSG_CTRL_ZHAO_HUAN,dwID);
						}
						else if(iType == 26)
						{
							g_MsgBoxMgr.PopSimpleComfirm("你的炽天凤凰正在探宝呢，你是否需要召回它呢？",MSG_CTRL_ZHAO_HUAN,dwID);
						}	
					}
				}
			}
		}
	}
	else if(dwMsg == MSG_CTRL_POP_MENU)
	{
		if(dwData == POP_MENU_ITEM_RIDE)
		{
			OnRideBtnClicked(m_iPos);
			return true;
		}
		else if(dwData == POP_MENU_ITEM_MERRIDE)
		{
			//OnMerRideBtnClicked(m_iPos);
			return true;
		}
		else if(dwData == POP_MENU_ITEM_EXPLORE)
		{
			OnExplore(m_iPos);
			return true;
		}
	}
	return false;
}


void CPetWnd::OnHelpBtn()
{
	bool b = !m_pMarkViewer->IsEnable();
	if(b)
	{
		this->ReSize(m_iWidth+246,m_iHeight);
		m_pMarkViewer->SetEnable(true);
		m_pMarkViewer->SetShow(true);
	}
	else
	{	
		LPTexture pBackTex = NULL;
		DWORD dwTexID = 0;
		if(m_vTexturesID.size() > 0)
			dwTexID = m_vTexturesID[0];

		if (dwTexID > 0)
		{
			pBackTex = g_pTexMgr->GetTexImm(dwTexID,EP_UI);
		}

		if(pBackTex)
		{
			this->ReSize(pBackTex->GetWidth(),pBackTex->GetHeight());
			m_pMarkViewer->SetEnable(false);
			m_pMarkViewer->SetShow(false);
		}
	}
}


void CPetWnd::DrawHelpMsg()
{
	if(!m_pMarkViewer->IsShow())
		return;
	//底图
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14495,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX+420 , m_iScreenY,pTex);

	//重绘控件
	m_pCloseHelpBtn->Draw();
	m_pMarkViewer->Draw();
}

void CPetWnd::OnReleaseExp(int iIndex)
{	
	DWORD dwID = m_dwPetID[iIndex];
	map<DWORD,PET_SENDOUT>& maplist = g_PetData.GetSendOutPetList();
	map<DWORD,PET_SENDOUT>::iterator it;
	it = maplist.find(dwID);
	if(it == maplist.end()) return;

	PET_SENDOUT &SendOutPet = it->second;
	if(0 == SendOutPet.dwSorbExp)
	{
		g_MsgBoxMgr.PopSimpleAlert("你的灵兽没有可释放的经验，你可以通过灵兽寄养来快速累积经验，如有更多问题请查看帮助");
	}
	else
	{
		__time32_t t_time;
		_time32(&t_time);
		t_time += g_dwServerTime;
		//最后喂食时间＋4天-当前时间 = 饱食度(存活时间)(秒)
		DWORD dwLiveSeconds = DWORD(SendOutPet.tLastEatTime + 4*24*3600 - t_time);

		if(dwLiveSeconds < 48*3600)
		{
			g_MsgBoxMgr.PopSimpleAlert("你只有在灵兽处于饱食状态时才能释放经验。");
			return;
		}

		DWORD dwMaxRelaseExp = 0;
		if(SendOutPet.dwTodayReleasedExp > SendOutPet.dwMaxCheapExpPerDay)
		{
			dwMaxRelaseExp = (DWORD)( ((float)SendOutPet.dwExpensiveExpPerFullDay/(24*3600))*(dwLiveSeconds - 48*3600)) ;
		}
		else
		{
			dwMaxRelaseExp = (DWORD)( ((float)SendOutPet.dwCheapExpPerFullDay/(24*3600))*(dwLiveSeconds - 48*3600)) ;

			if(SendOutPet.dwTodayReleasedExp + dwMaxRelaseExp > SendOutPet.dwMaxCheapExpPerDay)
			{
				dwMaxRelaseExp = SendOutPet.dwMaxCheapExpPerDay - SendOutPet.dwTodayReleasedExp;
				dwLiveSeconds -= (int)( (float)dwMaxRelaseExp/((float)SendOutPet.dwCheapExpPerFullDay/(24*3600)) );
				dwMaxRelaseExp += (DWORD)( ((float)SendOutPet.dwExpensiveExpPerFullDay/(24*3600))*(dwLiveSeconds - 48*3600));
			}
		}

		if(dwMaxRelaseExp > 10000)
			dwMaxRelaseExp = dwMaxRelaseExp - dwMaxRelaseExp%10000;
		else if(dwMaxRelaseExp > 1000)
			dwMaxRelaseExp = dwMaxRelaseExp - dwMaxRelaseExp%1000;

		DWORD dwExp = min(SendOutPet.dwSorbExp,dwMaxRelaseExp);
		m_bClick = false;

		char strTemp[128]={0};
		sprintf(strTemp,"%u",dwExp);
		g_MsgBoxMgr.PopEditBox("请确定你要从灵兽身上释放多少经验?",MSG_CTRL_RELEASE_EXP,SendOutPet.dwPetItemID,NULL,true,12,0,0,NULL,true,strTemp);
		g_PetData.SetFeedID(dwID);
	}
}

bool CPetWnd::DealPetFosterMsg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			for(int i = 0; i < PET_CARRYBACK_LINE_COUNT; i++)
			{
				if(pControl == m_pEndFosterBtn[i])
				{
					PET_ADOPT_MAP::iterator itr;
					PET_ADOPT_MAP &MPetCarryBack = g_PetData.GetPetCarryBackMap();

					if((itr=MPetCarryBack.find(i) ) != MPetCarryBack.end())
					{
						PET_ADOPT & petAdopt = itr->second;
						if(petAdopt.dwPetItemID != 0)
						{
							g_pGameControl->SEND_Pet_Adopt_Request(4,petAdopt.dwPetItemID);//取回寄养了的宠物
							//MPetCarryBack.erase(itr);
							//g_PetData.SetCarrayBackPetNum(g_PetData.GetCarrayBackPetNum()-1);
							g_pGameControl->SEND_Pet_Adopt_Request(2,0);//重新请求寄养宠物数据
						}
					}

					return true;
				}
			}

			for(int i = 0; i < PET_FOSTERAGE_LINE_COUNT; i++)
			{
				if(pControl == m_pFosterBtn[i])
				{
					FosteragePet(i);
					return true;
				}
			}
		}
		break;
	}
	return true;
}

void CPetWnd::DrawFosterLineData(int i,PET_ADOPT &petData)
{
	char strtemp[24];
	int pos = m_pGridCarryBack->GetScrollPos() + 1;

	m_pGridCarryBack->DrawGrid(i,0,itoa(pos + i,strtemp,10));

	if(strlen(petData.szPetName) == 0)
	{
		std::string strPetName = g_PetData.GetPetName(petData.byPetType);
		m_pGridCarryBack->DrawGrid(i,1,strPetName.c_str(),true);
	}
	else
		m_pGridCarryBack->DrawGrid(i,1,petData.szPetName,true);

	sprintf(strtemp,"%d",petData.byPetLevel);

	m_pGridCarryBack->DrawGrid(i,2,strtemp,true);

	int iDays = g_Timer.GetServerHours(petData.tLastEatTime)/24;
	if(iDays >= 4)
		sprintf(strtemp,"死亡");
	else if(iDays == 3)
		sprintf(strtemp,"饥饿");
	else if(iDays >= 2)
		sprintf(strtemp,"微饿");
	else
		sprintf(strtemp,"饱");

	m_pGridCarryBack->DrawGrid(i,3,strtemp,true);

	if(strlen(petData.szLYPlayerName) == 0)
		m_pGridCarryBack->DrawGrid(i,4,"驯兽师",true);
	else
		m_pGridCarryBack->DrawGrid(i,4,petData.szLYPlayerName,true);
}


void CPetWnd::DrawFosterageLineData(int i,CGood &good)
{
	char strtemp[24];
	int pos = m_pGridFosterage->GetScrollPos() + 1;

	m_pGridFosterage->DrawGrid(i,0,itoa(pos + i,strtemp,10));
	//自定义名字
	if(good.GetExternString() !="")
		sprintf(strtemp,"%s",good.GetExternString().c_str());
	else
	{
		std::string strPetName = g_PetData.GetPetName(good.GetSC());
		sprintf(strtemp,"%s",strPetName.c_str());				
	}

	m_pGridFosterage->DrawGrid(i,1,strtemp,true);

	sprintf(strtemp,"%d",good.GetDC());
	m_pGridFosterage->DrawGrid(i,2,strtemp,true);

	//喂食时间
	DWORD m = good.GetRecordTime();
	if( m != 0 )
	{
		int iDays = g_Timer.GetServerHours(m)/24;
		if(iDays >= 4)
			sprintf(strtemp,"死亡");
		else if(iDays == 3)
			sprintf(strtemp,"饥饿");
		else if(iDays >= 2)
			sprintf(strtemp,"微饿");
		else
			sprintf(strtemp,"饱");
	}
	else
		sprintf(strtemp,"----");

	m_pGridFosterage->DrawGrid(i,3,strtemp,true);
	m_pGridFosterage->DrawGrid(i,4,SELF.GetName(),true);
}

void CPetWnd::FosteragePet(int i)
{
	//到包裹里找到要寄养的豹魔石
	int pos = m_pGridFosterage->GetScrollPos();
	int petNum = 0;
	for(int j = 0; j < MAX_PACKAGE_ELEMENT; j++)
	{
		CGood& tmp = SELF.GetPackageGood(j);

		if(tmp.GetID() == 0)
			continue;

		if(tmp.GetStdMode() == 49 && tmp.GetShape() == 51)
		{
			//看看是不是领养自别人的豹子
			map<DWORD,PET_ADOPT> & MyAdopMap = g_PetData.GetMyAdoptMap();
			if( MyAdopMap.find(tmp.GetID()) != MyAdopMap.end())
				continue;


			if(petNum == pos + i)
			{

				DWORD m = tmp.GetRecordTime();
				if( m != 0 )
				{
					int iDays = g_Timer.GetServerHours(m)/24;
					if(iDays >= 2)
					{
						g_MsgBoxMgr.PopSimpleAlert("只有饱食状态的灵兽才能寄养。");
					}
					else 
					{
						g_pGameControl->SEND_Pet_Adopt_Request(6,tmp.GetID());
						g_pGameControl->SEND_Pet_Adopt_Request(2,0);//重新请求寄养宠物数据
					}
				}
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("未开启的灵兽不能寄养,请双击包裹里的豹魔石或狮魔石开启。");
				}
				return;
			}

			petNum ++;
		}
	}
}

void CPetWnd::DrawCarryBackInfo()
{
	PET_ADOPT_MAP::iterator itr;
	PET_ADOPT_MAP &MPetCarryBack = g_PetData.GetPetCarryBackMap();
	int n = 0;

	for(itr = MPetCarryBack.begin();itr != MPetCarryBack.end(); itr++)
	{
		m_pEndFosterBtn[n]->SetEnable(true);
		m_pEndFosterBtn[n]->SetShow(true);
		DrawFosterLineData(n,itr->second);
		n++;
	}

	for(int i = n; i < PET_CARRYBACK_LINE_COUNT; i++)
	{
		m_pEndFosterBtn[i]->SetEnable(false);
		m_pEndFosterBtn[i]->SetShow(false);
	}

	m_pGridCarryBack->SetTotalCount(n);
}

bool CPetWnd::DealPetAdoptMsg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			for(int i = 0; i < 5; i++)//结束领养豹子
			{
				if(pControl == m_pEndAdoptBtn[i])
				{
					int pos = m_pGridEndAdopt->GetScrollPos();
					map<DWORD,PET_ADOPT>::iterator itr;
					map<DWORD,PET_ADOPT> &MPetAdopt = g_PetData.GetMyAdoptMap();

					itr = MPetAdopt.begin();// + pos;
					for(int j = 0 ; j < pos+i && itr != MPetAdopt.end(); j++,itr++)
					{
					}

					if(itr != MPetAdopt.end())
					{
						g_pGameControl->SEND_Pet_Adopt_Request(7,(itr->second).dwPetItemID);
						//MPetAdopt.erase(itr);

						int iRpos = m_pGridEndAdopt->GetScrollPos() + 1;
						g_pGameControl->SEND_Pet_Adopt_Request(1,MAKELONG(iRpos,iRpos+20));//重新请求可以领养的宠物数据,协议头前4字节:起始序号(2字节)终止序号(2字节)
					}

					return true;
				}
			}

			for(int i = 0; i < 5; i++)//领养豹子
			{
				if(pControl == m_pAdoptBtn[i])
				{
					int pos = m_pGridAdopt->GetScrollPos() + 1;
					PET_ADOPT_MAP::iterator itr;
					PET_ADOPT_MAP &MPetAdopt = g_PetData.GetPetAdoptMap();

					if((itr=MPetAdopt.find(pos+i)) != MPetAdopt.end())
					{
						PET_ADOPT & petAdopt = itr->second;
						if(petAdopt.dwPetItemID != 0)
						{
							if(strcmp(petAdopt.szJYPlayerName,SELF.GetName()) == 0)
							{
								g_MsgBoxMgr.PopSimpleAlert("你不能领养自己寄养的灵兽。");
							}
							else
							{
								g_pGameControl->SEND_Pet_Adopt_Request(3,petAdopt.dwPetItemID);//请求领养
								g_pGameControl->SEND_Pet_Adopt_Request(1,MAKELONG(pos,pos+20));//重新请求可以领养的宠物数据,协议头前4字节:起始序号(2字节)终止序号(2字节)
							}
						}
					}

					return true;
				}
			}
		}
		break;
	}
	return true;
}

bool CPetWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	//处理公共消息
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pHelpBtn || pControl == m_pCloseHelpBtn)
		{
			OnHelpBtn();
			return true;
		}
	}
	else if(dwMsg == MSG_CTRL_PET_WND)
	{
		if(dwData == 1)
			UpdatePetList();
	}
	/*else if(dwMsg == MSG_CTRL_START_USE_YSTPY)
	{
	StartUseCloth(dwData);
	return true;
	}
	else if(dwMsg == MSG_CTRL_MER_START_USE_YSTPY)
	{
	MerStartUseCloth(dwData);
	return true;
	}*/

	if(m_iCurPage == 1)
	{
		DealPetListMsg(dwMsg,dwData,pControl);
	}else if(m_iCurPage == 2)
	{	
		if(dwMsg == MSG_CTRL_BUTTON_CLICK)
		{
			if(pControl == m_pMoreShorCut)
			{				
				g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,0);				
			}
		}
	}else if(m_iCurPage == 3)
	{
		DealPetAdoptMsg(dwMsg,dwData,pControl);
	}else if(m_iCurPage == 4)
	{
		DealPetFosterMsg(dwMsg,dwData,pControl);
	}

	return	CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

//取得点所在的格子,(ix,iy)是相对本窗口的坐标
void CPetWnd::GetGoodGrid(int* iX, int* iY, int ix, int iy)
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

	if((*iY) * m_iWCells + (*iX) >= MAX_PET_PACKAGE)
	{
		*iX = -1;
		*iY = -1;
	}
}

//判断某点是否在单元格区域内
bool CPetWnd::IsInGoodGrid(int ix, int iy)
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


bool CPetWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();

	if(m_iCurPage == 2)
	{
		int X,Y;
		CGood temp;
		GetGoodGrid(&X,&Y,iX,iY);

		if(X != -1 && Y != -1)
		{
			temp = SELF.GetPetGood(Y*m_iWCells + X);
			if(temp.GetID() != 0)
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
				pTip->SetShow(false);
			}

		} 
		else if(iX > 102 && iX < 315 && iY > 126 && iY < 160)
		{
			int i = (iX - 102)/44;
			CQuickItem& qitem = g_WooolStoreMgr.GetQuickPetItem(i);
			if(qitem.strName.length() > 0)
			{
				pTip->Clear();
				pTip->AddText(qitem.strName.c_str(),0xFFFFFF00);
				char ctemp[100]={0};
				sprintf(ctemp,"价格：%d元宝",qitem.iPrice);
				pTip->AddText(ctemp);
				pTip->AddText("双击购买并使用"); 

				int x = m_iScreenX + iX + 10;
				int y = m_iScreenY + iY + 10;
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);

				pTip->SetShow(true);
				return true;

			}
			else
			{
				pTip->SetShow(false);
			}
		}
		else
		{
			temp.SetID(0);
			pTip->SetShow(false);
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CPetWnd::OnLeftButtonDClick(int iX,int iY)
{
	if(m_iCurPage == 2)//双击购买使用宠物快捷消费
	{
		m_strID = "";
		g_WooolStoreMgr.GetDropQuickItem().clear();

		if(iX > 102 && iX < 315 && iY > 126 && iY < 160)
		{
			int i = (iX - 102)/44;			
			g_WooolStoreMgr.UsePetQuickItem(i);
			return true;
		}
	}else if(m_iCurPage == 1)
	{
		if(iX>24 && iX < 100 && iY > 102 && iY < 403)
		{
			//判断Y值
			if(iY > 102 && iY < 179)
			{
				int iTemp = (iX - 62)*(iX-62) + (iY - 140)*(iY-140);
				if(iTemp < 62*62)
				{
					OnFollowBack(0);
				}
			}else if(iY > 225 && iY < 300)
			{
				int iTemp = (iX - 62)*(iX-62) + (iY - 262)*(iY-262);
				if(iTemp < 62*62)
				{
					OnFollowBack(1);
				}
			}else if(iY > 346 && iY < 422)
			{
				int iTemp = (iX - 62)*(iX-62) + (iY - 384)*(iY-384);
				if(iTemp < 62*62)
				{
					OnFollowBack(2);
				}
			}
		}
	}
	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}


bool CPetWnd::OnLeftButtonUp(int iX, int iY)
{
	int iLeft = 16,iRight = 278;
	int iTop = 64,iBottom = 80;

	int iIndexPage = -1;
	if(iX >= 16 && iX <= 278 && iY >= iTop && iY <= iBottom)
	{
		iIndexPage = (iX - iLeft + 65)/65;
		if(iIndexPage >= 1 && iIndexPage <= 4 && iIndexPage != m_iCurPage)
		{
			m_iCurPage = iIndexPage;
			OnCreate();
			return true;
		}
	}else if(m_iCurPage == 2)
	{
		if(m_bDClicked)
		{
			m_bDClicked = false;
			return CCtrlWindowX::OnLeftButtonUp(iX,iY);
		}

		if(iX > 102 && iX < 315 && iY > 260 && iY < 350)
		{
			int i = (iX - 102)/44;
			CQuickItem& quickitem = g_WooolStoreMgr.GetQuickPetItem(i);

			if(quickitem.strItemID.size() > 0)
			{
				CQuickItem& qitem = g_WooolStoreMgr.GetDropQuickItem();
				if(qitem.strItemID.size() > 0)
				{
					for(int ii = 0; ii < PETQUICKITEM_COUNT;ii++)
					{
						if(qitem.strItemID.compare(g_WooolStoreMgr.GetQuickPetItem(ii).strItemID) == 0)
						{
							g_WooolStoreMgr.UsePetQuickItem(ii , false);
							break;
						}
					}

					g_WooolStoreMgr.GetDropQuickItem().clear();
					m_strID = "";	
					return true;
				}
			}
			else
			{
				g_WooolStoreMgr.GetDropQuickItem().clear();
				m_strID = "";
			}
		}else if(iX > 102 && iX < 315 && iY > 135 && iY < 165)
		{
			return true;
		}

		int X,Y;
		GetGoodGrid(&X,&Y,iX,iY);

		//表格位置正确
		if(X != -1 && Y != -1)
		{
			//已经有东西了
			stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

			if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == LingXiBox_Free_Wnd || GoodFrom.eFromWnd == LingXiBox_Pay_Wnd))
			{
				CGoodGrid::ReleaseDrop();
				m_bClick = false;
				g_MsgBoxMgr.PopSimpleAlert("请将物品拖至本体包裹栏中");
				return true;
			}

			//将包裹中物品放入宠物包裹中			
			if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == Package_Wnd || GoodFrom.eFromWnd == Panel_Wnd))
			{
				//前一个东西发送放入后，服务器还没有回应呢
				if(SELF.GetPetTemp().GetBlankNumber() == 0)
					return true;

				//发送请求放入
				if(SELF.GetPetTemp().Add(GoodFrom.DropGood))
				{
					g_pGameControl->SEND_Pet_Object_Storage(GoodFrom.DropGood);
					GoodFrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(GoodFrom);
				}

				return true;
			}

			if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Pet_Package_Wnd)
			{
				int iPos = 0;
				while(iPos < MAX_PET_PACKAGE)
				{
					if(SELF.GetPetGood(iPos).GetID() != 0)
						iPos++;
					else
						break;
				}
				if(iPos == MAX_PET_PACKAGE)
				{
					//宠物包裹已经放满了，这种情况不太可能
					return true;
				}
				SELF.GetPetGood(iPos) = GoodFrom.DropGood;
				GoodFrom.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(GoodFrom);

				return true;
			}					

			//取出宠物包裹里的东西						
			CGood temp = SELF.GetPetGood(Y*m_iWCells + X);
			if(temp.GetID() == 0) return true;

			if(g_pInput->IsShift())  //按物品
			{
				g_pControl->Msg(MSG_CTRL_INSERT_OBJECTLINK, 0,(CControl*)(&temp));
			}
			else
			{
				GoodFrom.DropGood = temp;
				GoodFrom.eFromWnd = Pet_Package_Wnd;
				GoodFrom.iWndPos = Y * m_iWCells + X;
				CGoodGrid::SetDropGoodFrom(GoodFrom);
				SELF.GetPetGood(Y*m_iWCells + X).SetID(0);
			}						
		}
	}

	if(m_pMarkViewer->IsEnable())
	{
		string strCommand = m_pMarkViewer->GetCommand();

		if(!strCommand.empty())
		{
			//up 音乐
			g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);

			if(strCommand.substr(0,2) == "@@")
			{
				g_NPC.SetLastCommand(strCommand);

				if(strCommand == "@@AdoptPet")//寄养宠物豹
				{
					string str = "<color=orange 寄养灵兽>\\";
					str += "1.灵兽可以通过打开灵兽界面（快捷\\  键Ctrl+L）寄养\\";
					str += "2.只有饱食状态的灵兽才能寄养\\";
					str += "3.寄养后其他玩家可以领养它们，领养\\  了灵兽的玩家在获得的经验的同时将\\  额外产出一部分经验凝结于领养的灵\\  兽体内\\";
					str += "4.你在取回寄养的灵兽后可以点击灵兽\\  属性界面上的[释放]按钮来释放这经\\  验，但释放经验需要消耗灵兽的饱食\\  度\\";
					str += "5.可以点击[结束寄养]按钮来领回寄养\\  的灵兽\\";
					str += "6.每人最多可以寄养1只灵兽。\\";


					str += "\\<返回/@@Back>";

					m_npcText.Clear();
					m_npcText.Parse(str,0,"\\",str.length());
				}
				else if(strCommand == "@@FosteragePet")//领养宠物豹
				{
					string str = "<color=orange 领养灵兽>\\";
					str += "1.你可以点击[灵兽领养]按钮来领养灵\\  兽\\";
					str += "2.每领养一只灵兽，就可以使你在获得\\  的经验的同时额外产出一部分经验加\\  成\\";
					str += "3.领养的灵兽等级越高，经验加成越多，\\  可以从商城购买粮食喂食你的灵兽，\\  以提高它的等级\\";
					str += "4.可以点击界面上的[结束领养]还回领\\  养灵兽\\";
					str += "5.你最多可以领养2只灵兽。\\";


					str += "\\<返回/@@Back>";

					m_npcText.Clear();
					m_npcText.Parse(str,0,"\\",str.length());				

				}
				else if(strCommand == "@@PetOperator")//宠物豹的操作 
				{
					string str = "<color=orange 灵兽的操作>\\";
					str += "<color=orange 跟随：>\\";
					str += "  双击包裹中灵兽或点[跟随]按钮可放\\";
					str += "  出灵兽，按CTRL+A可控制灵兽走动。\\";
					str += "<color=orange 喂食：>\\";
					str += "  1.在包裹中双击食物或将食物丢出\\";
					str += "  2.在界面上点击[喂食]\\";
					str += "<color=orange 守摊：>\\";
					str += "  点击[守摊],3级以上小灵兽均可守摊\\";
					str += "<color=orange 骑乘：>\\";
					str += "  穿上御兽天衣或天袍，点击[骑乘]或\\";
					str += "  CTRL+R骑乘饱食状态下的转生灵兽。\\";
					str += "<color=orange 吼叫：>\\";
					str += "  骑在转生后的灵兽上，点击CTRL+S可\\";
					str += "  以控制灵兽吼叫震慑怪物。\\";
					str += "<color=orange 灵兽包裹：>\\";
					str += "  点击[灵兽包裹]，双击商城包裹中的\\";
					str += "  物品可快速购买灵兽的相关物品。\\";
					str += "<color=orange 灵兽转生：>\\";
					str += "  小灵兽7级的时候，可在死水沼泽处\\";
					str += "  寻找铁匠打造踏云铉铠并带上兽神\\";
					str += "  水,去驯兽师处进行转生。";
					str += "\\<返回/@@Back>";

					m_npcText.Clear();
					m_npcText.Parse(str,0,"\\",str.length());				


				}
				else if(strCommand == "@@PetFunction")//宠物豹其他功能
				{
					string str = "<color=orange 灵兽功能>\\";
					str += "<color=orange 喂食：>\\";
					str += "  可给灵兽吃兽粮、精致兽粮和极品\\";
					str += "  兽粮来增加灵兽的饱食度和经验值。\\";
					str += "<color=orange 捡物：>\\";
					str += "  1级以上的灵兽可以帮你捡取金币，\\";
					str += "  3级以上的灵兽可以帮你捡取物品。\\";
					str += "<color=orange 包裹：>\\";
					str += "  1级以上3级以下灵兽具有5格包裹，\\";
					str += "  3级以上的灵兽具有10格包裹\\";
					str += "<color=orange 守摊：>\\";
					str += "  3级以上的灵兽可以帮助玩家守摊，\\";
					str += "  转生之后的灵兽可以守住15个摊位\\";
					str += "<color=orange 骑乘：>\\";
					str += "  穿上御兽天衣或御兽天袍，按下\\";
					str += "  CTRL+R可骑饱食状态下的转生灵兽\\";
					str += "<color=orange 吼叫：>\\";
					str += "  转生后的灵兽具有吼叫震慑怪物的\\";
					str += "  能力，并且能在室内地图骑乘，宠\\";
					str += "  物不受怪物攻击。\\";
					str += "<color=orange 复活：>\\";
					str += "  死亡的灵兽可在驯兽师处使用兽神\\";
					str += "  水复活。\\";
					str += "<color=orange 寄养和领养：>\\";
					str += "  通过寄养和领养，玩家可释放存储\\";
					str += "  于灵兽身体中的经验\\";
					str += "<返回/@@Back>";

					m_npcText.Clear();
					m_npcText.Parse(str,0,"\\",str.length());				


				}
				else if(strCommand == "@@Explore")
				{
					string str = "<color=orange 凤凰探宝>\\";
					str += "<color=orange 驾驭凤凰：>\\";
					str += "  只有等级达到50级才能驾驭转生后的\\";
					str += "  凤凰。\\";						
					str += "<color=orange 探宝功能：>\\";
					str += "  凤凰在释放状态下才可进行探宝，探\\";
					str += "  宝时凤凰将消耗饱食度，一旦凤凰处\\";
					str += "  于微饿状态后，将停止探宝。当角色\\";
					str += "  下线时，凤凰将也能继续消耗饱食度\\";
					str += "  进行探宝。将凤凰召唤回后，就可领\\";
					str += "  取探宝获得的物品。\\";
					str += "<返回/@@Back>";

					m_npcText.Clear();
					m_npcText.Parse(str,0,"\\",str.length());
				}
				else if(strCommand == "@@Back")//返回
				{
					string str = "<color=orange 有关灵兽的介绍>\\";
					str += "1.<灵兽的功能/@@PetFunction>\\";
					str += "2.<灵兽的操作/@@PetOperator>\\";
					str += "3.<寄养灵兽/@@AdoptPet>\\";
					str += "4.<领养灵兽/@@FosteragePet>\\";
					str += "5.<凤凰探宝/@@Explore>\\";

					m_npcText.Clear();
					m_npcText.Parse(str,0,"\\",str.length());

				}


				return true;
			}
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CPetWnd::OnLeftButtonDown(int iX, int iY)
{
	if(m_iCurPage == 2)
	{
		if(IsInGoodGrid(iX,iY))		//点击中表格不纪录拖动
			return true;

		if(iX > 97 && iX < 315 && iY > 135 && iY < 165)
		{
			int iIndex = (iX - 97 )/44;
			CQuickItem& tmp = g_WooolStoreMgr.GetQuickPetItem(iIndex);

			if(tmp.strItemID == g_WooolStoreMgr.GetDropQuickItem().strItemID)
			{
				g_WooolStoreMgr.GetDropQuickItem().clear();
			}
			else if(tmp.strItemID.size() > 0)
			{
				g_WooolStoreMgr.GetDropQuickItem() = tmp;
			}
			else
			{
				g_WooolStoreMgr.GetDropQuickItem().clear();
			}

			return true;
		}
		else if(iX > 97 && iX < 321 && iY > 260 && iY < 349)
		{
			return true;
		}

	} 

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}


void CPetWnd::UpdateBtnState()
{
	map<DWORD,PET_SENDOUT>& mappet = g_PetData.GetSendOutPetList();
	map<DWORD,PET_SENDOUT>::iterator it,itEnd;
	it = mappet.begin();
	itEnd = mappet.end();
	int iType = -1;
	//char   szReserved[8];//szReserved[0] ==0未释放 ==1释放出来跟随 ==2骑上去，==3摆摊
	while(it != itEnd)
	{
		iType = it->second.szReserved[0];
		if(iType ==1) m_bFollow = true;
		else if(iType ==2) m_bRide = true;

		it++;
	}

	int iPos = m_pPetListScroll->GetPos();
	int iSize = iPos + 3;
	if(iSize > g_PetData.GetSendOutPetList().size())
		iSize = g_PetData.GetSendOutPetList().size();

	for(int i = 0;i<iSize - m_pPetListScroll->GetPos();i++)
	{
		PET_SENDOUT& pet = it->second;
		if(pet.szReserved[0] == 0)//未释放
		{
			m_pFollowBackBtn[i]->ReloadTex(16727,16728,16729);
			m_pFollowBackBtn[i]->SetEnable(true);
			m_pFollowBackBtn[i]->SetTips("灵兽只有在跟随的状态下才能发挥\n作用。在包裹中双击灵兽石，或者点此处\n的跟随均能让灵兽处于释放状态。");
		}
		else if(pet.szReserved[0] == 1)//跟随
		{


		}else if(pet.szReserved[0] == 2)//骑上去
		{
		}else if(pet.szReserved[0] == 3)//摆摊
		{
		}
		it++;
	}
}

void CPetWnd::DrawPetListWnd()
{
	//初始信息	
	int iType = -1;
	m_bFollow = false, m_bRide   = false;
	m_iPetNum = 0;
	m_bMerRide = false;

	for(int i = 0; i < MAX_PETS_NUM; i++)
		m_pGood[i] = NULL;

	for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
	{
		CGood &tmpGood = SELF.GetPackageGood(i);
		if(tmpGood.GetID() != 0 && tmpGood.GetStdMode() == 49 && tmpGood.GetShape() == 51 ) 
		{
			if(m_iPetNum < MAX_PETS_NUM)
			{
				m_pGood[m_iPetNum] = &tmpGood;
				m_iPkgPos[m_iPetNum] = i;
				m_iPetNum ++;
			}
			else
			{
				break;				
			}

			bool bFound = false;
			for(int i = 0;i<m_vectorPet.size();i++)
			{
				if(m_vectorPet[i] == tmpGood.GetID())
				{
					bFound = true;
					break;
				}
			}
			if(!bFound) m_vectorPet.push_back(tmpGood.GetID());
		}
	}


	bool bHaveHorse = false;
	for(int i = m_vectorPet.size()-1;i>= 0;i--)
	{
		bool bFind = false;
		if(m_vectorPet[i] == 0x00FFFFFF)
		{
			if(g_PetData.GetMyHorse().bStatus != 0)
			{
				bHaveHorse = true;
				continue;
			}
		}

		for(int j = 0;j < m_iPetNum;j++)
		{
			if(m_vectorPet[i] == m_pGood[j]->GetID())
			{
				bFind = true;
				break;
			}
		}

		if(!bFind) m_vectorPet.erase(m_vectorPet.begin()+i);
	}

	if(g_PetData.GetMyHorse().bStatus != 0  && !bHaveHorse)
		m_vectorPet.push_back(0x00FFFFFF);

	map<DWORD,PET_SENDOUT>& mappet = g_PetData.GetSendOutPetList();
	map<DWORD,PET_SENDOUT>::iterator it,itEnd;
	it = mappet.begin();
	itEnd = mappet.end();

	//char   szReserved[8];//szReserved[0] ==0未释放 ==1释放出来跟随 ==2骑上去，==3摆摊
	while(it != itEnd)
	{
		DWORD dwID = it->second.dwPetItemID;
		iType = it->second.szReserved[0];
		if(iType ==1) m_bFollow = true;
		else if(iType ==2 || iType == 6) m_bRide = true;  //iType==6表示本体骑战
		else if(iType == 7 || iType == 8) m_bMerRide = true;   //iType == 7表示元神骑乘，iType == 8表示元神骑战
		it++;
	}

	//是否骑马 0，未释放，1 跟随 2 骑上去
	if(g_PetData.GetMyHorse().bStatus == 2) m_bRide = true;

	char strTemp[128] ={0};
	int iRange = m_iPetNum;
	if(g_PetData.GetMyHorse().bStatus != 0) iRange++;
	else m_iHorsePos = -1;
	iRange -= 3;
	if(iRange < 0) iRange =0;
	m_pPetListScroll->SetRange(iRange);
	int iPos = m_pPetListScroll->GetPos();

	//绘制三个数据
	int iBeginNum = iPos;
	int iEndNum = iPos + 3;

	if(iEndNum > m_vectorPet.size())
	{
		iEndNum = m_vectorPet.size();
	}	

	for(int i = iBeginNum;i<iEndNum;i++)
	{
		DrawPetData(m_vectorPet[i],i - iBeginNum);
	}

	//绘制没有的数据
	for(int i = 0;i<(iPos+3 - (int)m_vectorPet.size());i++)
	{
		DrawPetEmptyData(iEndNum+i);		
	}

	m_pMenu->Draw();
}


void CPetWnd::DrawPetData(DWORD dwPetID,int iPos)
{
	// 首先
	if(dwPetID == 0x00FFFFFF)
	{
		//绘制马
		DrawOnLiveHorseData(iPos);
	}
	else
	{
		map<DWORD,PET_SENDOUT>& mappet = g_PetData.GetSendOutPetList();
		map<DWORD,PET_SENDOUT>::iterator it;
		it = mappet.find(dwPetID);
		if(it != mappet.end())
		{
			DrawOnLivePetData(dwPetID,iPos);
		}else
		{
			DrawOffLinePetData(dwPetID,iPos);
		}
	}	
}

void CPetWnd::DrawPetEmptyData(int iPos)
{
	int iTemp = iPos;
	if(iPos > 3) iPos = 2;
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14491,EP_UI);
	if(pTex) g_pGfx->DrawTextureNL(m_iScreenX+16,m_iScreenY+87+122*iPos,pTex);
}

//iPos 指处于当前页的序号,iIndex 指宠物索引值
void CPetWnd::DrawOnLivePetData(DWORD dwPetID,int iPos)
{
	map<DWORD,PET_SENDOUT>& mappet = g_PetData.GetSendOutPetList();
	map<DWORD,PET_SENDOUT>::iterator it,itEnd;
	it = mappet.find(dwPetID);
	itEnd = mappet.end();
	if(it == itEnd) return;

	PET_SENDOUT &SendOutPet = it->second;

	//颜色，种类
	int iColor = SendOutPet.byPetColor;
	int iType = SendOutPet.byPetType;
	m_dwPetID[iPos] = SendOutPet.dwPetItemID;

	int iOffsetY = 0;

	if(SendOutPet.szReserved[0] == 9)	//凤凰探宝
	{
		DWORD dwTime = (GetTickCount() / 20) % 707;
		LPTexture pBackTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17008,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX + 23 - dwTime,m_iScreenY + 95 + iPos*122,pBackTex,dwTime,0,dwTime + 352);
		pBackTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17007,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 17,m_iScreenY + 86 + iPos*122,pBackTex);
		pBackTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17009,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 247,m_iScreenY + 96 + iPos*122,pBackTex);
		pBackTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17010 + SendOutPet.byPetColor,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 60,m_iScreenY + 92 + iPos*122,pBackTex);
		iOffsetY = 4;
	}

	//状态
	char strTemp[256] = {0};

	__time32_t t_time;
	_time32(&t_time);
	t_time += g_dwServerTime;

	int iDay = int(t_time - SendOutPet.tLastEatTime)/(24 * 3600);
	if(iDay >= 4)
	{
		mappet.erase(it);
		return;
	}
	else if(iDay == 3)
		sprintf(strTemp,"%s","饥饿");
	else if(iDay == 2)
		sprintf(strTemp,"%s","微饿");
	else
		sprintf(strTemp,"%s","饱");	

	g_pFont->DrawText(m_iScreenX + 295,m_iScreenY + 99 + iPos*122,strTemp,0xFFFFFFFF);

	//显示饱食度
	//小时为单位
	int iLiveHours = int((SendOutPet.tLastEatTime + 4*24*3600 - t_time)/ 3600); 
	float fHungry = (float)(iLiveHours) / (SendOutPet.byFullTime*24);

	if(fHungry > 1.0f)		fHungry = 1.0f;
	else if(fHungry < 0)	fHungry = 0.0f;

	LPTexture pHungry = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14432,EP_UI);
	if(pHungry)
		g_pGfx->DrawPartTexture(m_iScreenX+294,m_iScreenY+126 + 122 * iPos + iOffsetY,pHungry,0,0,(int)(pHungry->GetWidth()*fHungry),-1);

	if(SendOutPet.szReserved[0] == 9)	//凤凰探宝
	{
		m_pArrow[iPos]->SetShow(false);
		m_pReleaseExpBtn[iPos]->SetShow(false);
		m_pFollowBackBtn[iPos]->SetShow(false);
		m_pFeedBtn[iPos]->SetShow(false);	
		m_pFunc[iPos]->SetShow(false);	

		m_pArrow[iPos]->SetEnable(false);
		m_pReleaseExpBtn[iPos]->SetEnable(false);
		m_pFollowBackBtn[iPos]->SetEnable(false);
		m_pFeedBtn[iPos]->SetEnable(false);
		m_pFunc[iPos]->SetEnable(false);

		m_pBeckon[iPos]->SetShow(true);
		m_pBeckon[iPos]->SetEnable(true);
		m_pBeckon[iPos]->Draw();

		return;
	}	
	else
	{
		m_pArrow[iPos]->SetShow(true);
		m_pReleaseExpBtn[iPos]->SetShow(true);
		m_pFollowBackBtn[iPos]->SetShow(true);
		m_pFeedBtn[iPos]->SetShow(true);
		m_pFunc[iPos]->SetShow(true);	

		m_pArrow[iPos]->SetEnable(true);
		m_pReleaseExpBtn[iPos]->SetEnable(true);
		m_pFollowBackBtn[iPos]->SetEnable(true);
		m_pFunc[iPos]->SetEnable(true);	

		m_pBeckon[iPos]->SetShow(false);
		m_pBeckon[iPos]->SetEnable(false);
	}

	//头像
	LPTexture pTex = NULL;
	switch (iType)
	{
	case 0:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14470,EP_UI);
		break;
	case 1:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14471 + iColor,EP_UI);
		break;
	case 2:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15660 + iColor,EP_UI);
		break;
	case 8:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14475,EP_UI);
		break;
	case 9:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14476 + iColor,EP_UI);
		break;
	case 10:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15666 + iColor,EP_UI);
		break;
	case 16:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15672,EP_UI);
		break;
	case 17:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15672 + iColor,EP_UI);
		break;
	case 18:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15672 + iColor,EP_UI);
		break;
	case 24:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16913,EP_UI);
		break;
	case 25:
	case 26:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16915 + iColor,EP_UI);
		break;
	default:
		break;
	}

	if(pTex) g_pGfx->DrawTextureNL(m_iScreenX + 28,m_iScreenY + 96 + 122 * iPos,pTex);

	LPTexture pTexTemp = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14469,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX+20,m_iScreenY + 147 + iPos*122,pTexTemp);

	unsigned int iLevel = SendOutPet.byPetLevel;
	if (iLevel < 10)
	{
		LPTexture pTexLevel = g_pTexMgr->GetTex(PACKAGE_INTERFACE,590 + iLevel,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 31,m_iScreenY + 151 + iPos*122,pTexLevel);
	}	
	else if (iLevel < 100)
	{
		LPTexture pTexLevel = g_pTexMgr->GetTex(PACKAGE_INTERFACE,590 + iLevel/10,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 25,m_iScreenY + 151 + iPos*122,pTexLevel);
		pTexLevel = g_pTexMgr->GetTex(PACKAGE_INTERFACE,590 + iLevel%10,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 36,m_iScreenY + 151 + iPos*122,pTexLevel);
	}
	else
	{
		LPTexture pTexLevel = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15990 + iLevel/100,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 27,m_iScreenY + 155 + iPos*122,pTexLevel);
		unsigned int iTmp = iLevel%100;
		pTexLevel = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15990 + iTmp/10,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 35,m_iScreenY + 155 + iPos*122,pTexLevel);
		pTexLevel = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15990 + iTmp%10,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 43,m_iScreenY + 155 + iPos*122,pTexLevel);
	}

	std::string strPetName;

	strPetName = it->second.szPetName;
	if(strPetName.length() <= 0)
		strPetName = g_PetData.GetPetName(iType,iColor);
	g_pFont->DrawText(m_iScreenX + 127, m_iScreenY + 100 + 122 *iPos,strPetName.c_str(),0xFFFFFFFF);


	//显示生命值
	float fLife = (float)SendOutPet.byHPPercent / 100;
	if(fLife > 1.0f) fLife = 1.0f;

	LPTexture pLife = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14433,EP_UI);
	if(pLife)
		g_pGfx->DrawPartTexture(m_iScreenX+162,m_iScreenY+128 + 122 * iPos,pLife,0,0,(int)(pLife->GetWidth()*fLife));


	//画可释放经验
	char strAddExp[12]={0};
	sprintf(strAddExp,"%u",SendOutPet.dwSorbExp);
	g_pFont->DrawText(m_iScreenX+217,m_iScreenY+146 + iPos * 122,strAddExp,0xFFFFFFFF);

	//更新按钮状态
	//char   szReserved[8];//szReserved[0] ==0未释放 ==1释放出来跟随 ==2骑上去，==3摆摊

	//跟随
	if(SendOutPet.szReserved[0] == 1)
	{
		m_pFollowBackBtn[iPos]->ReloadTex(16739,16740,16741);
		m_pFollowBackBtn[iPos]->SetEnable(true);
		m_pFollowBackBtn[iPos]->SetTips("你可以通过“收回”按钮，把灵兽收回包裹中。");

		m_pFeedBtn[iPos]->SetEnable(true);

		if(m_iPos == iPos)
		{
			if(iType >= 24 && iType <= 26)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16784,16785,16786,16787,true);	//探宝
			}
			else
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16784,16785,16786,16787,false);
			}
		}

		if(m_bRide || (iType&0x07) < 1)
		{
			if(m_iPos == iPos)
			{				
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16746,16747,false);
			}	
		}
		else
		{
			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16746,16747,true);
			}
		}
		if(m_bMerRide || (iType&0x07) <= 1) 
		{
			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,false);	
			}
		}
		else
		{
			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,true);				
			}
		}
	}
	else if(SendOutPet.szReserved[0] == 2)		//本体骑乘
	{
		m_pFollowBackBtn[iPos]->ReloadTex(16739,16740,16741);
		m_pFollowBackBtn[iPos]->SetEnable(true);
		m_pFollowBackBtn[iPos]->SetTips("你可以通过“收回”按钮，把灵兽收回包裹中。");

		m_pFeedBtn[iPos]->SetEnable(true);

		if((iType&0x07) > 1) 
		{
			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16760,16761,16762,16763,true);				
			}			
		}
		else
		{
			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16764,16765,16766,16767,true);				
			}
		}

		if(m_iPos == iPos)
		{
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,false);

			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,false);	//无法领宝
		}		
	}
	else if(SendOutPet.szReserved[0] == 3)//守摊
	{
		if(m_iPos == iPos)
		{
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16756,16747,false);
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,false);	

			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,false);	//无法领宝
		}

		m_pFeedBtn[iPos]->SetDisableTex(16734);
		m_pFeedBtn[iPos]->SetEnable(false);

		m_pFollowBackBtn[iPos]->SetEnable(false);
		m_pFollowBackBtn[iPos]->SetDisableTex(16730);	
	}
	else if(SendOutPet.szReserved[0] == 7)   //元神骑乘
	{
		m_pFollowBackBtn[iPos]->ReloadTex(16739,16740,16741);
		m_pFollowBackBtn[iPos]->SetEnable(true);
		m_pFollowBackBtn[iPos]->SetTips("你可以通过“收回”按钮，把灵兽收回包裹中。");

		m_pFeedBtn[iPos]->SetEnable(true);

		if(m_iPos == iPos)
		{
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16756,16747,false);
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16772,16773,16774,16775,true);		

			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,false);	//无法领宝
		}		
	}
	else if(SendOutPet.szReserved[0] == 8)   //元神骑战
	{
		m_pFollowBackBtn[iPos]->ReloadTex(16739,16740,16741);
		m_pFollowBackBtn[iPos]->SetEnable(true);
		m_pFollowBackBtn[iPos]->SetTips("你可以通过“收回”按钮，把灵兽收回包裹中。");

		m_pFeedBtn[iPos]->SetEnable(true);

		if(m_iPos == iPos)
		{
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16756,16747,false);
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16776,16777,16778,16779,true);		

			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,false);	//无法领宝
		}	
	}
	else if(SendOutPet.szReserved[0] == 6)   //本体骑战
	{
		m_pFollowBackBtn[iPos]->ReloadTex(16739,16740,16741);
		m_pFollowBackBtn[iPos]->SetEnable(true);
		m_pFollowBackBtn[iPos]->SetTips("你可以通过“收回”按钮，把灵兽收回包裹中。");

		m_pFeedBtn[iPos]->SetEnable(true);

		if(m_iPos == iPos)
		{
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16764,16765,16766,16767,true);
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,false);

			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,false);	//无法领宝
		}		
	}
	else
	{
		m_pFollowBackBtn[iPos]->ReloadTex(16727,16728,16729);
		m_pFollowBackBtn[iPos]->SetEnable(true);
		m_pFollowBackBtn[iPos]->SetTips("灵兽只有在跟随的状态下才能发挥\n作用。在包裹中双击灵兽石，或者点此处\n的跟随均能让灵兽处于释放状态。");

		m_pFeedBtn[iPos]->SetEnable(false);
		m_pFeedBtn[iPos]->SetDisableTex(16734);

		if(m_iPos == iPos)
		{
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16746,16747,false);
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,false);

			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,true);		//领宝
		}
	}
	m_pReleaseExpBtn[iPos]->SetEnable(true);
}

//iPos 指处于当前页的序号
void CPetWnd::DrawOnLiveHorseData(int iPos)			
{		
	PET_HORSE& horse = g_PetData.GetMyHorse();

	if(horse.bStatus == 0 || iPos > 2) return;

	m_iHorsePos = iPos;
	if(horse.bStatus == 2)//骑
	{
		if(m_iPos == iPos)
		{
			m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16764,16765,16766,16767,true);
		}
	}
	else if(horse.bStatus = 1)//跟随
	{
		if(m_bRide)
		{
			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16746,16747,false);				
			}
		}
		else
		{
			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16746,16747,true);				
			}
		}
	}
	std::string strName = "马";
	switch(horse.wAppr)
	{
	case 161:
		strName = "枣红马";
		break;
	case 162:
		strName = "雪龙";
		break;
	case 163:
		strName = "乌骓";
		break;
	case 164:
		strName = "高头大马";
		break;
	case 165:
		strName = "黄金宝马";
		break;
	}
	g_pFont->DrawText(m_iScreenX + 127, m_iScreenY + 100 + 122 *iPos,strName.c_str(),0xFFFFFFFF);

	float fLife = (float)horse.byHPPercent / 100;
	if(fLife > 1.0f) fLife = 1.0f;

	LPTexture pLife = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14433,EP_UI);
	if(pLife)
		g_pGfx->DrawPartTexture(m_iScreenX+162,m_iScreenY+128 + 122 * iPos,pLife,0,0,(int)(pLife->GetWidth()*fLife));

	m_pArrow[iPos]->SetShow(true);
	m_pReleaseExpBtn[iPos]->SetShow(true);
	m_pFollowBackBtn[iPos]->SetShow(true);
	m_pFeedBtn[iPos]->SetShow(true);
	m_pFunc[iPos]->SetShow(true);	

	m_pArrow[iPos]->SetEnable(true);		
	m_pFunc[iPos]->SetEnable(true);	

	m_pBeckon[iPos]->SetShow(false);
	m_pBeckon[iPos]->SetEnable(false);

	m_pFollowBackBtn[iPos]->SetDisableTex(16730);
	m_pFollowBackBtn[iPos]->SetEnable(false);

	m_pReleaseExpBtn[iPos]->SetDisableTex(14404);
	m_pReleaseExpBtn[iPos]->SetEnable(false);	

	if(m_iPos == iPos)
	{
		m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,false);	
		m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,false);	//无法领宝
	}		

	m_pFeedBtn[iPos]->SetDisableTex(16734);
	m_pFeedBtn[iPos]->SetEnable(false);	




	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14480 + horse.wAppr - 161,EP_UI);
	if(pTex) 
		g_pGfx->DrawTextureNL(m_iScreenX + 28,m_iScreenY + 95 + 122 * iPos,pTex);
	LPTexture pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14490,EP_UI);
	if(pTex1) 
		g_pGfx->DrawTextureNL(m_iScreenX + 118,m_iScreenY + 101 + 122 * iPos,pTex1);
}

void CPetWnd::DrawOffLinePetData(DWORD dwPetID,int iPos)
{
	CGood* pGood = NULL;
	LPTexture pTex = NULL;
	for(int i = 0;i<MAX_PETS_NUM;i++)
	{
		pGood = m_pGood[i];	
		if(pGood == NULL) continue;
		DWORD dwID = pGood->GetID();

		if(pGood->GetID() == dwPetID)
		{
			//没有找到
			unsigned char byte = pGood->GetDC2();
			bool bFlag = ((byte & 0x04) != 0);
			int iType = pGood->GetSC();
			if(bFlag) continue;

			m_dwPetID[iPos] = dwID;
			switch(iType)
			{
			case 0:
			case 1:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14474,EP_UI);
				break;
			case 2:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15663,EP_UI);
				break;
			case 8:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14486,EP_UI);
				break;
			case 9:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14479,EP_UI);
				break;
			case 10:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15669,EP_UI);
				break;
			case 16:
			case 17:
			case 18:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15675,EP_UI);
				break;
			case 24:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16912,EP_UI);
				break;
			case 25:
			case 26:
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16914,EP_UI);
				break;
			}

			if(pTex) g_pGfx->DrawTextureNL(m_iScreenX + 28,m_iScreenY + 96 + 122 * iPos,pTex);

			//状态
			char strTemp[256] = {0};
			std::string strPetName;

			__time32_t t_time;
			_time32(&t_time);
			t_time += g_dwServerTime;

			//喂食时间
			DWORD m = pGood->GetRecordTime();
			int iDays = g_Timer.GetServerHours(m)/24;

			if( m != 0 )
			{
				if(iDays >= 4)
					sprintf(strTemp,"%s","死亡");
				else if(iDays == 3)
					sprintf(strTemp,"%s","饥饿");
				else if(iDays == 2)
					sprintf(strTemp,"%s","微饿");
				else
					sprintf(strTemp,"%s","饱");
			}
			else
				sprintf(strTemp,"%s","未开启");


			int iX = m_iScreenX + 295;
			int iY = m_iScreenY + 99 + iPos*122;

			g_pFont->DrawText(iX,iY,strTemp,0xFFFFFFFF);
			strPetName = g_PetData.GetPetName(iType,pGood->GetDC2() && 0x30f >> 4);
			g_pFont->DrawText(m_iScreenX + 127, m_iScreenY + 100 + 122 *iPos,strPetName.c_str(),0xFFFFFFFF);

			m_pArrow[iPos]->SetShow(true);
			m_pReleaseExpBtn[iPos]->SetShow(true);
			m_pFollowBackBtn[iPos]->SetShow(true);
			m_pFeedBtn[iPos]->SetShow(true);
			m_pFunc[iPos]->SetShow(true);	

			m_pArrow[iPos]->SetEnable(true);
			m_pReleaseExpBtn[iPos]->SetEnable(true);
			m_pFollowBackBtn[iPos]->SetEnable(true);
			m_pFunc[iPos]->SetEnable(true);	

			m_pBeckon[iPos]->SetShow(false);
			m_pBeckon[iPos]->SetEnable(false);

			if(m_bFollow || (iDays >= 4 && m != 0) )
			{
				m_pFollowBackBtn[iPos]->SetDisableTex(16730);
				m_pFollowBackBtn[iPos]->SetEnable(false);
			}else
			{
				m_pFollowBackBtn[iPos]->ReloadTex(16727,16728,16729);
				m_pFollowBackBtn[iPos]->SetEnable(true);
				m_pFollowBackBtn[i]->SetTips("灵兽只有在跟随的状态下才能发挥\n作用。在包裹中双击灵兽石，或者点此处\n的跟随均能让灵兽处于释放状态。");
		    } 

			m_pFeedBtn[iPos]->SetDisableTex(16734);
			m_pFeedBtn[iPos]->SetEnable(false);


			m_pReleaseExpBtn[iPos]->SetDisableTex(14404);
			m_pReleaseExpBtn[iPos]->SetEnable(false);

			if(m_iPos == iPos)
			{
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_RIDE,16744,16745,16746,16747,false);
				m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_MERRIDE,16748,16749,16750,16751,false);

				if(iType >= 24 && iType <= 26)
				{
					m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,true);	//领宝
				}
				else
				{
					m_pMenu->SetMenuButtonItem(POP_MENU_ITEM_EXPLORE,16756,16757,16758,16759,false);
				}
			}		
		}		
	}			
}

void CPetWnd::DrawPetPkgWnd()
{
	//绘制宠物快捷消费
	DWORD dwTime = m_dwTickCount;
	for(int i = 0;i< QUICKITEM_PET;i++)
	{		
		CQuickItem& qitem = g_WooolStoreMgr.GetQuickPetItem(i);
		// 绘制物品图标
		if(dwTime > qitem.dwTime)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_items, qitem.iItemLooks,EP_UI);
			if(pTex) g_pGfx->DrawTextureNL(m_iScreenX + i*m_iCellW + 104,m_iScreenY + 135,pTex);
		}
	}
	int x,y;
	//绘制物品栏内物品
	bool bSendOutPet = g_PetData.GetSendOutPetList().size() <= 0?false:true;
	//CTexture* pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14444);
	LPTexture pTex = NULL;
	int iRows = m_iHCells>0?m_iHCells:2;
	int iCols = m_iWCells>0?m_iWCells:5;
	int iMaxPkg = MAX_PET_PACKAGE>0?MAX_PET_PACKAGE:10;

	x = m_iScreenX + m_ptGoodPoint.x - 8;
	y = m_iScreenY + m_ptGoodPoint.y - 8;

	if(MAX_PET_PACKAGE == 5) 
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14496,EP_UI);
		g_pGfx->DrawTextureNL(x,y,pTex);
	}else if(MAX_PET_PACKAGE <= 0)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14497,EP_UI);
		g_pGfx->DrawTextureNL(x,y,pTex);
	}

	g_pFont->DrawText(x,y + 60,"包裹只有在有灵兽跟随的情况下才能使用。",-1);

	if(MAX_PET_PACKAGE > 0)
	{
		for(int nh = 0; nh < iRows; nh++)
		{
			for(int nw = 0; nw < iCols; nw++)
			{
				//物品
				if(nh * iCols + nw >= iMaxPkg) //超过了边界
					continue;

				x = m_iScreenX + m_ptGoodPoint.x + nw * (m_iCellW + m_iCellDisW);
				y = m_iScreenY + m_ptGoodPoint.y + nh * (m_iCellH + m_iCellDisH);

				CGood& temp = SELF.GetPetGood(nh * iCols + nw);

				if(bSendOutPet)
				{
					if(g_pControl) //绘制单个物品
					{
						CGoodGrid::DrawOneGood(x + 18,y + 18,temp);
					}
				}
			}        
		}
	}

	//绘制鼠标移动的快捷消费物品
	if(g_pInput->IsLeftButton() && m_QuickItem.iItemLooks != 0)
	{
		//进行绘制该物品
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_items, m_QuickItem.iItemLooks,EP_UI);
		int iX,iY;
		g_pInput->GetMousePos(iX,iY);
		if(pTex) 
			g_pGfx->DrawTextureNL(iX,iY,pTex);
	}
}

void CPetWnd::DrawFosteragePetInfo()
{
	int petNum = 0;
	int pos = m_pGridFosterage->GetScrollPos();

	for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
	{
		CGood& tmp = SELF.GetPackageGood(i);

		if(tmp.GetID() == 0) continue;

		if(tmp.GetStdMode() == 49 && tmp.GetShape() == 51)
		{
			//看看是不是领养自别人的豹子
			map<DWORD,PET_ADOPT> & MyAdopMap = g_PetData.GetMyAdoptMap();
			map<DWORD,PET_ADOPT>::iterator itr;
			if( (itr = MyAdopMap.find(tmp.GetID())) == MyAdopMap.end())
			{
				petNum ++;
				if(petNum > pos && petNum - pos <= PET_FOSTERAGE_LINE_COUNT)
				{
					m_pFosterBtn[petNum - pos -1]->SetEnable(true);
					m_pFosterBtn[petNum - pos -1]->SetShow(true);
					DrawFosterageLineData(petNum-pos-1,tmp);
				}
			}
		}
	}

	for(int i = petNum-pos; i < PET_FOSTERAGE_LINE_COUNT; i++)
	{
		m_pFosterBtn[i]->SetEnable(false);
		m_pFosterBtn[i]->SetShow(false);
	}

	m_pGridFosterage->SetTotalCount(petNum);
}

void CPetWnd::DrawAdoptPetInfo()
{
	int pos = m_pGridAdopt->GetScrollPos()+1;

	PET_ADOPT_MAP::iterator itr,itrFirst;
	int num = m_pGridAdopt->GetLinesPerPage();
	int n = 0;
	DWORD dwTotalAdoptPetNum = g_PetData.GetAdoptPetNum();
	PET_ADOPT_MAP &MPetAdopt = g_PetData.GetPetAdoptMap();

	for(int i = 0;i < num;i++)
	{
		if( pos+i > dwTotalAdoptPetNum)
		{
			break;
		}

		if((itr=MPetAdopt.find(pos+i)) == MPetAdopt.end())
		{
			//3秒一次
			if(!m_bSending && m_dwTickCount - m_LastSendTick >3000)
			{
				g_pGameControl->SEND_Pet_Adopt_Request(1,MAKELONG(pos,pos+20));//重新请求可以领养的宠物数据,协议头前4字节:起始序号(2字节)终止序号(2字节)

				m_LastSendTick = m_dwTickCount;
				m_bSending = true;
			}

			DrawWaitReading();
			return;
		}
		else
		{
			if(i == 0)
				itrFirst = itr;
			n++;
		}
	}

	m_bSending = false;

	for(int i = 0; i < n; i++)
	{
		m_pAdoptBtn[i]->SetEnable(true);
		m_pAdoptBtn[i]->SetShow(true);
		DrawLineData(i,itrFirst->second);
		itrFirst++;
	}

	for(int i = n; i < PET_ADOPT_LINE_COUNT; i++)
	{
		m_pAdoptBtn[i]->SetEnable(false);
		m_pAdoptBtn[i]->SetShow(false);
	}

	if(m_pGridAdopt->GetScrollPos() != m_oldPos_Adopt)
	{
		m_bSending = false;
		m_oldPos_Adopt = m_pGridAdopt->GetScrollPos();
	}

	g_pFont->DrawText(m_iScreenX+30,m_iScreenY+434,"你每领养一只灵兽，都将对你打怪获得经验有额外加成。",0xFFFFFFFF);
	m_pGridAdopt->SetTotalCount(dwTotalAdoptPetNum);
}

void CPetWnd::DrawWaitReading()
{
	string str;

	if(m_dwTickCount - m_LastSendTick > 8000) //8秒显示繁忙
	{
		g_pFont->DrawText(140+m_iScreenX ,350+m_iScreenY,"查询繁忙,请稍候再试",COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_MIDDLE);
		return;
	}
	if(m_nWaitReading >= 0 && m_nWaitReading<20)
	{
		str = "数据读取中";
		m_nWaitReading++;
	}
	else if(m_nWaitReading >= 20 && m_nWaitReading<40)
	{
		str = "数据读取中。。";
		m_nWaitReading++;
	}
	else if(m_nWaitReading >= 40 && m_nWaitReading<60)
	{
		str = "数据读取中。。。。";
		m_nWaitReading++;
	}
	else if(m_nWaitReading >= 60 && m_nWaitReading<80)
	{
		str = "数据读取中。。。。。";
		m_nWaitReading++;
	}
	else if(m_nWaitReading>=80)
	{
		str = "数据读取中。。。。。";
		m_nWaitReading = 0;
	}

	if(str.size() != 0)
	{
		g_pFont->DrawText(140+m_iScreenX ,350+m_iScreenY,str.c_str(),COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Bold);
	}
}


void CPetWnd::DrawEndAdoptPetInfo()
{
	map<DWORD,PET_ADOPT>::iterator itr;
	int num = m_pGridEndAdopt->GetLinesPerPage();
	int n = 0;

	map<DWORD,PET_ADOPT> &MPetAdopt = g_PetData.GetMyAdoptMap();

	itr = MPetAdopt.begin();
	for(int i = 0; i < num && itr != MPetAdopt.end(); i++,itr++)
	{
		m_pEndAdoptBtn[i]->SetEnable(true);
		m_pEndAdoptBtn[i]->SetShow(true);
		DrawEndAdoptLineData(i,itr->second);

		n++;
	}

	for(int i = n; i < PET_ADOPT_LINE_COUNT; i++)
	{
		m_pEndAdoptBtn[i]->SetEnable(false);
		m_pEndAdoptBtn[i]->SetShow(false);
	}

	m_pGridEndAdopt->SetTotalCount(MPetAdopt.size());
}

void CPetWnd::UpdateAdoptPetInfo()
{
	int pos = m_pGridAdopt->GetScrollPos() + 1;
	//重新请求可以领养的宠物数据,协议头前4字节:起始序号(2字节)终止序号(2字节)
	g_pGameControl->SEND_Pet_Adopt_Request(1,MAKELONG(pos,pos+20));
}

void CPetWnd::DrawLineData(int i,PET_ADOPT &petData)
{
	char strtemp[24];
	int pos = m_pGridAdopt->GetScrollPos()+1;

	m_pGridAdopt->DrawGrid(i,0,itoa(pos + i,strtemp,10));

	if(strlen(petData.szPetName) == 0)
	{
		std::string strPetName = g_PetData.GetPetName(petData.byPetType);
		m_pGridAdopt->DrawGrid(i,1,strPetName.c_str(),true);
	}
	else
		m_pGridAdopt->DrawGrid(i,1,petData.szPetName,true);

	sprintf(strtemp,"%d",petData.byPetLevel);

	m_pGridAdopt->DrawGrid(i,2,strtemp,true);

	int iDays = g_Timer.GetServerHours(petData.tLastEatTime)/24;
	if(iDays >= 4)
		sprintf(strtemp,"死亡");
	else if(iDays == 3)
		sprintf(strtemp,"饥饿");
	else if(iDays >= 2)
		sprintf(strtemp,"微饿");
	else
		sprintf(strtemp,"饱");

	m_pGridAdopt->DrawGrid(i,3,strtemp,true);

	m_pGridAdopt->DrawGrid(i,4,petData.szJYPlayerName,true);
}

void CPetWnd::DrawEndAdoptLineData(int i,PET_ADOPT &petData)
{
	char strtemp[24];
	int pos = m_pGridEndAdopt->GetScrollPos()+1;

	m_pGridEndAdopt->DrawGrid(i,0,itoa(pos + i,strtemp,10));

	if(strlen(petData.szPetName) == 0)
	{
		std::string strPetName = g_PetData.GetPetName(petData.byPetType);
		m_pGridEndAdopt->DrawGrid(i,1,strPetName.c_str(),true);
	}
	else
		m_pGridEndAdopt->DrawGrid(i,1,petData.szPetName,true);

	sprintf(strtemp,"%d",petData.byPetLevel);

	m_pGridEndAdopt->DrawGrid(i,2,strtemp,true);

	int iDays = g_Timer.GetServerHours(petData.tLastEatTime)/24;
	if(iDays >= 4)
		sprintf(strtemp,"死亡");
	else if(iDays == 3)
		sprintf(strtemp,"饥饿");
	else if(iDays >= 2)
		sprintf(strtemp,"微饿");
	else
		sprintf(strtemp,"饱");

	m_pGridEndAdopt->DrawGrid(i,3,strtemp,true);
	m_pGridEndAdopt->DrawGrid(i,4,petData.szJYPlayerName,true);
}

void CPetWnd::DrawPetAdoptWnd()
{
	DrawAdoptPetInfo();
	DrawEndAdoptPetInfo();
}

void CPetWnd::DrawPetFosterWnd()
{
	DrawFosteragePetInfo();
	DrawCarryBackInfo();
}

void CPetWnd::UpdatePetList()
{
	m_vectorPet.clear();
	map<DWORD,PET_SENDOUT>& mappet = g_PetData.GetSendOutPetList();
	map<DWORD,PET_SENDOUT>::iterator it,itEnd;
	it = mappet.begin();
	for(int i = 0;i< mappet.size();i++)
	{
		m_vectorPet.push_back(it->first);
		it++;
	}

	m_bIniHorseSendout = (g_PetData.GetMyHorse().bStatus != 0);
	if(m_bIniHorseSendout) m_vectorPet.push_back(0x00FFFFFF);

}

void CPetWnd::OnClickMenuButton(int iPos)
{
	m_pMenu->Move(m_pFunc[iPos]->GetX(),m_pFunc[iPos]->GetY() + m_pFunc[iPos]->GetHeight() - 116);

	if(m_iPos == iPos)
	{
		m_pMenu->SetShow(!m_pMenu->IsShow());
	}
	else
	{
		m_pMenu->SetShow(true);
	}

	m_pArrow[0]->ReloadTex(16788,16789,16789);
	m_pArrow[1]->ReloadTex(16788,16789,16789);
	m_pArrow[2]->ReloadTex(16788,16789,16789);

	if(!m_pMenu->IsShow())
	{
		m_pMenu->SetEnable(false);		
		m_iPos = -1;
	}
	else
	{
		m_pMenu->SetEnable(true);
		m_pArrow[iPos]->ReloadTex(16790,16790,16790);
		m_iPos = iPos;
	}		
}

void CPetWnd::OnExplore(int i)
{
	DWORD dwID = m_dwPetID[i];
	map<DWORD,PET_SENDOUT>& mappet = g_PetData.GetSendOutPetList();
	map<DWORD,PET_SENDOUT>::iterator it = mappet.find(dwID);
	if(it != mappet.end())
	{
		int iType = it->second.byPetType;

		if(it->second.szReserved[0] == 0)	//未释放,领奖
		{
			g_pGameControl->Send_Pet_Explore(dwID,2);			
		}
		else if(it->second.szReserved[0] == 1)	//跟随,探宝
		{	
			__time32_t t_time;
			_time32(&t_time);
			t_time += g_dwServerTime;

			int iDay = int(t_time - it->second.tLastEatTime)/(24 * 3600);
			if(iDay == 3)
			{
				if(iType == 24)
				{
					g_MsgBoxMgr.PopSimpleAlert("你的赤翎凤目前处在饥饿状态，无法进行探宝。");
				}
				else if(iType == 25)
				{
					g_MsgBoxMgr.PopSimpleAlert("你的傲天凤目前处在饥饿状态，无法进行探宝。");
				}
				else if(iType == 26)
				{
					g_MsgBoxMgr.PopSimpleAlert("你的炽天凤凰目前处在饥饿状态，无法进行探宝。");					
				}
			}
			else
			{
				if(iType == 24)
				{
					g_MsgBoxMgr.PopSimpleComfirm("你是否想让赤翎凤进行探宝。探宝时，赤翎凤将会获得许多奇珍异宝。",MSG_CTRL_TAN_BAO,dwID);
				}
				else if(iType == 25)
				{
					g_MsgBoxMgr.PopSimpleComfirm("你是否想让傲天凤进行探宝。探宝时，傲天凤将会获得许多奇珍异宝。",MSG_CTRL_TAN_BAO,dwID);
				}
				else if(iType == 26)
				{
					g_MsgBoxMgr.PopSimpleComfirm("你是否想让炽天凤凰进行探宝。探宝时，炽天凤凰将会获得许多奇珍异宝。",MSG_CTRL_TAN_BAO,dwID);
				}
			}
		}
		else if(it->second.szReserved[0] == 9)	//探宝,召回
		{	
			if(iType == 24)
			{
				g_MsgBoxMgr.PopSimpleComfirm("你的赤翎凤正在探宝呢，你是否需要召回它呢？",MSG_CTRL_ZHAO_HUAN,dwID);
			}
			else if(iType == 25)
			{
				g_MsgBoxMgr.PopSimpleComfirm("你的傲天凤正在探宝呢，你是否需要召回它呢？",MSG_CTRL_ZHAO_HUAN,dwID);
			}
			else if(iType == 26)
			{
				g_MsgBoxMgr.PopSimpleComfirm("你的炽天凤凰正在探宝呢，你是否需要召回它呢？",MSG_CTRL_ZHAO_HUAN,dwID);
			}
		}
	}
	else 
	{
		//未释放,领奖
		g_pGameControl->Send_Pet_Explore(dwID,2);
	}
}