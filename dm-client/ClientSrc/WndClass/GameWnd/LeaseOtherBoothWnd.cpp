#include "LeaseOtherBoothWnd.h"
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

INIT_WND_POSX(CLeaseOtherBoothWnd,POS_VARIABLE,POS_VARIABLE)

CLeaseOtherBoothWnd::CLeaseOtherBoothWnd(void)
{
	//m_iBoothType = (int)sm_dwWindowType;//包裹格子数类别

	m_iWCells = 6;
	m_iHCells = 5;
	m_iCellW  = 37;
	m_iCellH  = 37;
	m_iCellDisW = 2;
	m_iCellDisH = 2;
	m_bDClicked = false;
	m_ptGoodPoint.x = 27;
	m_ptGoodPoint.y = 74;
	m_pBuyButton = NULL;

	m_iIndex = 4611;
	m_iPages = 1;
	m_dwLastSayTime = 0;

	m_iAlignType = XAL_TOPLEFT;

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

CLeaseOtherBoothWnd::~CLeaseOtherBoothWnd(void)
{
	g_TalkMgr.Clear(TALKTYPE_BOOTH);
}

void CLeaseOtherBoothWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//太远了，关闭窗口
	if(!g_pGameData->FindSimpleCharacter(g_OtherBoothData.GetOtherPlayerID()))
	{
		CloseWindow();
		return;
	}

	g_pFont->DrawText(120+m_iScreenX, 11+m_iScreenY, "商 铺", COLOR_TEXT_NORMAL, FONT_YAHEI, FONTSIZE_BIG);

	g_pFont->DrawText(45 + m_iScreenX, 46 + m_iScreenY, "当前所有人:", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
	g_pFont->DrawText(m_iScreenX + 135,m_iScreenY + 46,g_OtherBoothData.GetBoothName(), COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);

	char tempstr[256]={0};
	sprintf(tempstr,"距离租赁到期还剩%u天！",g_OtherBoothData.GetDaysRemain());
	g_pFont->DrawText(40 + m_iScreenX, 275 + m_iScreenY, tempstr, COLOR_TEXT_SBU_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);

	unsigned long nID = g_OtherBoothData.GetOtherPlayerID();

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

}

bool CLeaseOtherBoothWnd::Create(CControl* pParent,int iCurPage)
{
	if (g_Login.GetLoginInExpType() == 1)
	{
		g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_RECREATE);
		return false;
	}

	return CCtrlWindowX::Create(pParent, iCurPage);
}

void CLeaseOtherBoothWnd::OnCreate()
{
	//复用全局变量g_otherboothData
	//与otherboothwnd是互斥的,应在打开之前,关闭它
	{
		g_pControl->PopupWindow(MSG_CTRL_BOOTH_WND,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_OTHER_BOOTH,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_CLOSE);
	}
	
	g_TalkMgr.GetBuyTalk().clear();
	m_dwLastBuyGoodID = 0;
	m_dwLastClickBuyTime = 0;

	SetCloseButton(251,4,80);
	//购买
	m_pBuyButton = new CCtrlButton();
	AddControl(m_pBuyButton);
	m_pBuyButton->CreateEx(this,201,274,95,96,97);
	m_pBuyButton->SetText("购 买");

	//聊天输入框
	m_pChatEdit = new CCtrlEdit();
	AddControl(m_pChatEdit);
	m_pChatEdit->CreateEx(this,31,393,162,23,0,39,39,39,39);
	m_pChatEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pChatEdit->SetMaxLength(100);
	
	m_pTalkViewer = new CTalkViewer();
	AddControl(m_pTalkViewer);
	m_pTalkViewer->Create(this,35,304,213,81);	
	m_pTalkViewer->AddScrollEx(200, 0, 17, 81);
	m_pTalkViewer->SetTalkType(&g_TalkMgr.GetBoothTalk());
}
bool CLeaseOtherBoothWnd::OnLeftButtonDown(int iX, int iY)
{
	if(IsInGoodGrid(iX,iY))		//点击中表格不纪录拖动
		return true;

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}
bool CLeaseOtherBoothWnd::OnLeftButtonUp(int iX, int iY)
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
bool CLeaseOtherBoothWnd::OnMouseMove(int iX, int iY)
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
bool CLeaseOtherBoothWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{

	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pBuyButton)
		{
			if (g_Login.GetLoginInExpType() == 1)
			{
				g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_RECREATE);
				return true;
			}

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
				g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CREATE,1);
			}
			return true;
		}
		break;
	case MSG_CTRL_EDIT_ENTRY:
		{
			char chatline[256] = {0};
			strcpy(chatline,m_pChatEdit->GetText());
			StringUtil::trim(chatline);
			WORD strLen = strlen(chatline);

			if ( strLen <= 0)
				return true;

			unsigned long nPlayerID = g_OtherBoothData.GetOtherPlayerID();
			
			DWORD dwCount = GetTickCount();
			if((dwCount-m_dwLastSayTime) > 10000)
			{
				g_pGameControl->Send_LeaseBooth_LeaveMessage(nPlayerID,strLen,chatline);
				m_dwLastSayTime = dwCount;
				g_TalkMgr.PushDataTalk(TALKTYPE_BOOTH,chatline,strlen(chatline),TALKCOLOR_WHITE);
				m_pChatEdit->Clear();
			}
			return true;
		}
		break;
	case MSG_CTRL_OTHER_BOOTH:
		{
			if(dwData == 1)
			{
				m_dwLastBuyGoodID = g_OtherBoothData.GetReadyBuy().GetID();
				g_OtherBoothData.GetReadyBuy().SetID(0);
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
bool CLeaseOtherBoothWnd::OnLeftButtonDClick(int iX,int iY)
{
	m_bDClicked = true;
	if (IsInGoodGrid(iX,iY))
	{
		m_bDClicked = false;
		return CLeaseOtherBoothWnd::OnLeftButtonUp(iX,iY);
	}

	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

//取得点所在的格子,(ix,iy)是相对本窗口的坐标
void CLeaseOtherBoothWnd::GetGoodGrid(int* iX, int* iY, int ix, int iy)
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
bool CLeaseOtherBoothWnd::IsInGoodGrid(int ix, int iy)
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