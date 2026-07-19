#include "BoothGuideWnd.h"
#include "GameData/OtherData.h"
#include "GameControl\GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/MapArray.h"
#include "BaseClass/Control/ParserTip.h"

#define PERSONS_LINE_COUNT 17

INIT_WND_POSX(CBoothGuideWnd,POS_AUTO,POS_AUTO)


//用来排序的比较函数
bool SBoothsGreater ( S_Booth &elem1, S_Booth &elem2 )
{
	return elem1.strName < elem2.strName;
}

CBoothGuideWnd::CBoothGuideWnd(void)
{
	m_iIndex = 22266;
	m_iPages = 1;
	m_dwLastRefrashData = GetTickCount();
}

CBoothGuideWnd::~CBoothGuideWnd(void)
{
}

void CBoothGuideWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	m_pOpenBtn = new CCtrlButton();
	AddControl(m_pOpenBtn);
	m_pOpenBtn->CreateEx(this,50,411,95,96,97,98);
	m_pOpenBtn->SetText("打开", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	//加为好友
	m_pCancelBtn = new CCtrlButton();
	AddControl(m_pCancelBtn);
	m_pCancelBtn->CreateEx(this,189,411,95,96,97,98);
	m_pCancelBtn->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	SetCloseButton(251,4,80);	


	//列表控件
	m_pBoothGrid = new CCtrlGrid();
	AddControl(m_pBoothGrid);
	m_pBoothGrid->Create(this,20,41,274,396,PERSONS_LINE_COUNT,21.0f);
	m_pBoothGrid->AddScrollEx(231,0,20,355);
	m_pBoothGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

	m_pBoothGrid->SetLinesPerPage(PERSONS_LINE_COUNT);
	m_pBoothGrid->PushColumn(118); 
	m_pBoothGrid->SetDrawOffXY(0,2);
	
	CreateBoothList();

}

void CBoothGuideWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 138,m_iScreenY + 10,"附近摊位",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);

	DWORD dwCount = GetTickCount();

	if(dwCount - m_dwLastRefrashData > 1000) //每1s更新一下数据
	{
		CreateBoothList();
		m_dwLastRefrashData = dwCount;
	}


	size_t personNum = m_VBooths.size();
	int pos = m_pBoothGrid->GetScrollPos();
	int iLine = 0;

	for(size_t i = 0; iLine < PERSONS_LINE_COUNT && i + pos < personNum; i ++,iLine ++)
	{
		m_pBoothGrid->DrawGrid(iLine,0,m_VBooths[iLine+pos].strName.c_str(),false,0xFFFF8000);
	}
}

bool CBoothGuideWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_GRID_DBCLICK:
		{
			OnOpenBooth();
			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOpenBtn)
			{
				OnOpenBooth();
				return true;
			}			
			else if(pControl == m_pCancelBtn)
			{
				CloseWindow();
				return true;
			}
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}



bool CBoothGuideWnd::OnWheel(int iWheel)
{
	return m_pBoothGrid->OnWheel(iWheel);
}

bool CBoothGuideWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();

	int iHotLine = m_pBoothGrid->GetHotLine();	

	if(m_pBoothGrid->IsMouseOn(m_iScreenX + iX,m_iScreenY + iY) && iHotLine >= 0 && iHotLine < m_VBooths.size())
	{
		pTip->Clear();
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);

		pTip->AddText("双击打开该玩家摊位",0xFFFFFF00);
		pTip->AddText(m_VBooths[iHotLine].strBoothName.c_str(),-1,15 * FONTSIZE_DEFAULT / 2);
		pTip->SetShow(true);

		return true;
	}

	pTip->SetShow(false);

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void CBoothGuideWnd::CreateBoothList()
{
	int iSelLine = -1;
	int iDisLine = 0;
	
	if (m_pBoothGrid)
	{
		iSelLine = m_pBoothGrid->GetSelLine();
		iDisLine = m_pBoothGrid->GetDisLine();
	}

	m_VBooths.clear();

	MiniChar& _chars = MapArray.GetMiniChar();
	MiniChar::iterator it;

	int iSelfX = 0,iSelfY = 0,iX = 0,iY = 0;
	SELF.GetXY(iSelfX,iSelfY);

	S_Booth booth;
	for(it = _chars.begin(); it != _chars.end(); it++)
	{
		CSimpleCharacter* pChar = it->second;
		if(NULL == pChar || !pChar->IsHuman() || !pChar->GetBoothState()) 
			continue;

		pChar->GetXY(iX,iY);

		if (abs(iX - iSelfX) > 5 || abs(iY - iSelfY) > 5)
		{
			continue;
		}

		booth.strName = pChar->GetName();
		booth.strBoothName = pChar->GetBoothInfo().strBoothName;
		booth.dwID = pChar->GetID();

		m_VBooths.push_back(booth);

		//for test
		//for (int i = 0; i < 100; i++)
		//{
		//	m_VBooths.push_back(booth);
		//}
		//end for test
	}

	sort(m_VBooths.begin(), m_VBooths.end(), SBoothsGreater);

	if (iDisLine >= m_VBooths.size())
	{
		iDisLine = 0;
	}

	if (m_pBoothGrid)
	{
		m_pBoothGrid->SetTotalCount((int)(m_VBooths.size()));
		m_pBoothGrid->SetDisLine(iDisLine);
		m_pBoothGrid->SetSelLine(iSelLine);
	}

}

void CBoothGuideWnd::OnOpenBooth()
{
	int iSelLine = m_pBoothGrid->GetSelLine();
	if( iSelLine >= 0 && iSelLine < m_VBooths.size())
	{
		g_pGameControl->SEND_Exchange_Goin(m_VBooths[iSelLine].dwID);
	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("请先选中一个玩家。");
	}
}
