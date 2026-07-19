#include "TradeWnd.h"
#include "Global/GlobalMsg.h"
#include "GameData/Good.h"
#include "GameData/GameData.h"
#include "Baseclass/Control/ParserTip.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TalkMgr.h"
#include "GameData/TradeData.h"

INIT_WND_POSX(CTradeWnd,POS_VARIABLE,POS_VARIABLE)

CTradeWnd::CTradeWnd()
{
	m_iCellsW = 5;
	m_iCellsH = 2;
	m_iCellW  = 39; 
	m_iCellH  = 39;
	m_iCellDisW = 0;
	m_iCellDisH = 0;
	m_ptMyHeadPoint.x =32;
	m_ptMyHeadPoint.y = 67;
	m_ptPeerHeadPoint.x =292;
	m_ptPeerHeadPoint.y =67;
	m_bTrade = false;								//是否按下交易按钮

	m_iIndex = 100;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 10;
	m_iOffY = 10;


	//重新打开包裹窗口
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_CREATE);
	g_pControl->SetWindowPos(400,180,"PackageWnd");

    g_pControl->PopupWindow(MSG_CTRL_LUXNPCWND,OPER_CLOSE);
    g_pControl->PopupWindow(MSG_CTRL_NPCWND,OPER_CLOSE);
    g_pControl->PopupWindow(MSG_CTRL_NPCBOARDWND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_NPCRECRUIT,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_RELATION_WND,OPER_CUSTOM);
	g_pControl->PopupWindow(MSG_CTRL_ADVENTURE_WND, OPER_CLOSE);

	CSimpleCharacterNode *pChar = g_pGameData->FindSimpleCharacter(g_TradeData.GetAnotherName());
	if(pChar)
	{
		DWORD dwRelation = pChar->GetRelationType();
		if( (dwRelation & RT_WIFE) != 0)
			m_strRealtion = "<妻>";
		else if( (dwRelation & RT_HUSBAND) != 0)
			m_strRealtion = "<夫>";
		else if( (dwRelation & RT_PRENTICE) != 0)
			m_strRealtion = "<徒>";
		else if( (dwRelation & RT_MASTER) != 0)
			m_strRealtion = "<师> ";
		else if( (dwRelation & RT_FRIEND) != 0)
			m_strRealtion = "<友>";
		else 
			m_strRealtion = "<生>";
	}

	for(int i = 0; i < MAX_TRADE_OBJECTS; i ++)
	{
		g_TradeData.GetSelfGood(i).SetID(0);
		g_TradeData.GetAnotherGood(i).SetID(0);
	}
}

CTradeWnd::~CTradeWnd(void)
{
	//CMainWnd * pParent = (CMainWnd *)GetParent();
	//pParent->SetTradeDelay(GetTickCount());
}

void CTradeWnd::GetMyGrid(int* iX, int* iY, int ix, int iy)	//取得点在所在自己的表格
{
	int w,h;
	w = ix - m_ptMyHeadPoint.x;
	h = iy - m_ptMyHeadPoint.y;

	if( !(IsInMyGrid(ix,iy))|| (w % (m_iCellW + m_iCellDisW)) > m_iCellW ||
		(h % (m_iCellH + m_iCellDisH)) > m_iCellH)
	{
		*iX = -1;
		*iY = -1;
		return;
	}
	*iX = w / (m_iCellW + m_iCellDisW);
	*iY = h / (m_iCellH + m_iCellDisH);
}


void CTradeWnd::GetPeerGrid(int* iX, int* iY, int ix, int iy)	//取得点所在的对方的表格
{

	int w,h;
	w = ix - m_ptPeerHeadPoint.x;
	h = iy - m_ptPeerHeadPoint.y;

	if( !(IsInPeerGrid(ix,iy))|| (w % (m_iCellW + m_iCellDisW)) > m_iCellW ||
		(h % (m_iCellH + m_iCellDisH)) > m_iCellH)
	{
		*iX = -1;
		*iY = -1;
		return;
	}
	*iX = w / (m_iCellW + m_iCellDisW);
	*iY = h / (m_iCellH + m_iCellDisH);
}

bool CTradeWnd::IsInMyGrid(int ix, int iy)
{
	int w,h;
	w = ix - m_ptMyHeadPoint.x;
	h = iy - m_ptMyHeadPoint.y;

	if(w < 0 || h < 0 || w >= (m_iCellW + m_iCellDisW) * m_iCellsW || 
		h >= (m_iCellH + m_iCellDisH) * m_iCellsH)
	{
		return false;
	}

	return true;
}

bool CTradeWnd::IsInPeerGrid(int ix, int iy)
{
	int w,h;
	w = ix - m_ptPeerHeadPoint.x;
	h = iy - m_ptPeerHeadPoint.y;

	if(w < 0 || h < 0 || w >= (m_iCellW + m_iCellDisW) * m_iCellsW || 
		h >= (m_iCellH + m_iCellDisH) * m_iCellsH)
	{
		return false;
	}

	return true;
}

bool CTradeWnd::IsInMyMoneyGrid(int ix,int iy)
{
	if(ix>43 && ix<72  && iy>111  && iy<139 )
		return true;
	else 
		return false;
}


bool CTradeWnd::IsInMyYuanBaoGrid(int ix,int iy)
{
	if(ix>76 && ix<107  && iy>141  && iy<169 )
		return true;
	else 
		return false;
}


void CTradeWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX+233,m_iScreenY+157,"金币数量",COLOR_TEXT_NORMAL, FONT_YAHEI);
	g_pFont->DrawText(m_iScreenX+233,m_iScreenY+182,"元宝数量",COLOR_TEXT_NORMAL, FONT_YAHEI);
	g_pFont->DrawText(230+m_iScreenX, 11+m_iScreenY, "交   易", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);

	int x,y;
	//绘制自己表格内的物品
	for(int nh = 0; nh < m_iCellsH; nh++)
	{
		for(int nw = 0; nw < m_iCellsW; nw++)
		{	
			CGood& temp = g_TradeData.GetSelfGood(nh * m_iCellsW + nw);
			if(temp.GetID() == 0)
				continue; 
			x = m_iScreenX + m_ptMyHeadPoint.x + nw * (m_iCellW + m_iCellDisW) + 2;
			y = m_iScreenY + m_ptMyHeadPoint.y + nh * (m_iCellH + m_iCellDisH) + 2 ;

			if(g_pControl) //绘制单个物品
			{
				CGoodGrid::DrawOneGood(x+16,y+16,temp);
			}
		}        
	} 
	//绘制对方表格内的物品
	{
		for(int nh = 0; nh < m_iCellsH; nh++)
		{
			for(int nw = 0; nw < m_iCellsW; nw++)
			{	
				CGood temp = g_TradeData.GetAnotherGood(nh * m_iCellsW + nw);
				if(temp.GetID() == 0)
					continue;
				x = m_iScreenX + m_ptPeerHeadPoint.x + nw * (m_iCellW + m_iCellDisW) + 3;
				y = m_iScreenY + m_ptPeerHeadPoint.y + nh * (m_iCellH + m_iCellDisH) + 2;

				if(g_pControl) //绘制单个物品
				{
					CGoodGrid::DrawOneGood(x+16,y+16,temp);
				}
			}
		}
	}


	//显示自己的钱
	char str[64] = {0};
	DWORD lMoney = g_TradeData.GetSelfGolds();
	if(lMoney >= 0)
	{
		ToCommaStr(str,lMoney);
		g_pFont->DrawText(m_iScreenX + 66,m_iScreenY + 158,str,COLOR_TEXT_NORMAL);
	}
	DWORD lYuanBao = g_TradeData.GetSelfYuanBao();
	if(lYuanBao >= 0)
	{
		ToCommaStr(str,lYuanBao);
		g_pFont->DrawText(m_iScreenX + 90,m_iScreenY + 184,str,COLOR_TEXT_NORMAL);
	}
	//显示对方的钱
	lMoney = g_TradeData.GetAnotherGolds();
	if(lMoney >= 0)
	{
		ToCommaStr(str,lMoney);
		g_pFont->DrawText(m_iScreenX + 303,m_iScreenY + 158,str,COLOR_TEXT_NORMAL);
	}
	lYuanBao = g_TradeData.GetAnotherYuanBao();
	if(lYuanBao >= 0)
	{
		ToCommaStr(str,lYuanBao);
		g_pFont->DrawText(m_iScreenX + 303,m_iScreenY + 184,str,COLOR_TEXT_NORMAL);
	}

	//显示名字
	int nameLen1 = strlen(SELF.GetName())*FONTSIZE_DEFAULT;
	int nameLen2 = strlen(g_TradeData.GetAnotherName())*FONTSIZE_DEFAULT;
	g_pFont->DrawText(m_iScreenX + 150 - nameLen1/2,m_iScreenY + 44,SELF.GetName(),COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	
	g_pFont->DrawText(m_iScreenX + 410 - nameLen2/2 - g_pFont->GetLen(m_strRealtion.c_str()),m_iScreenY + 44,m_strRealtion.c_str(),0xFFFF0000,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 416 - nameLen2/2,m_iScreenY + 44,g_TradeData.GetAnotherName(),COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);

} 

bool CTradeWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pTradeButton)
		{
			if( g_pGameData->GetDropMoneyFrom() != EDMYF_NONE || g_pGameData->GetDropYuanBaoFrom() != EDMYF_NONE ||      //鼠标上有物品或者黄金，按下按钮无效
				CGoodGrid::GetDropGoodFrom().DropGood.GetID() != 0)
				return true;

			g_pGameControl->SEND_Trade_Ok();
			m_bTrade = true;
			g_pAudio->Play(EAT_OTHER,17,g_pAudio->GetRand()++);
			return true;
		}
		else if(pControl == m_pCancelButton)
		{
			OnClickCloseButton();
			return true;
		}

	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


void CTradeWnd::OnCreate()
{
	m_pTradeButton = new CCtrlButton();
	AddControl(m_pTradeButton);
	m_pTradeButton->CreateEx(this,228,80,95,96,97);
	m_pTradeButton->SetText("交易");

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,228,116,95,96,97);
	m_pCancelButton->SetText("取消");

	g_pGameControl->SEND_Request_Server_Time();

	SetCloseButton(481,5,80);
}

void CTradeWnd::OnClickCloseButton()
{
	g_pAudio->Play(EAT_OTHER,1,g_pAudio->GetRand()++);

	//鼠标上有物品或者黄金，按下按钮无效
	if( g_pGameData->GetDropMoneyFrom() != EDMYF_NONE || g_pGameData->GetDropYuanBaoFrom() != EDMYF_NONE ||	CGoodGrid::GetDropGoodFrom().DropGood.GetID() != 0)
		return;

	CloseWindow();
	g_pGameControl->SEND_Trade_End();
}

bool CTradeWnd::OnLeftButtonDown(int iX, int iY)
{
	if(IsInMyGrid(iX,iY) || IsInPeerGrid(iX,iY)||IsInMyMoneyGrid(iX,iY) || IsInMyYuanBaoGrid(iX,iY))		//点击中表格及黄金不纪录拖动
		return true;
	else
		return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}


bool CTradeWnd::OnLeftButtonUp(int iX, int iY)
{
	int X,Y;
	GetMyGrid(&X,&Y,iX,iY);
	bool bPeerOK  = g_TradeData.GetTradeFlag();
	//表格位置正确
	if(X != -1 && Y != -1)
	{
		if( g_pGameData->GetDropMoneyFrom() == EDMYF_PACKAG || g_pGameData->GetDropYuanBaoFrom() == EDMYF_PACKAG )				//鼠标上的黄金来自包裹
		{
			if(m_bTrade||bPeerOK)						//已经按下交易按钮，放黄金无效				
			{
				g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
				g_pGameData->SetDropYuanBaoFrom(EDMYF_NONE);
				return true;
			}

			if(g_pGameData->GetDropMoneyFrom() == EDMYF_PACKAG)
			{
				g_MsgBoxMgr.PopEditBox("你想支付多少金币?",MSG_CTRL_TRADEGOLDBOX,0,NULL,true);
			}
			else
			{
				g_MsgBoxMgr.PopEditBox("你想支付多少元宝?",MSG_CTRL_TRADEYUANBAOBOX,0,NULL,true);
			}

			g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
			g_pGameData->SetDropYuanBaoFrom(EDMYF_NONE);
			return true;
		}


		stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd != Package_Wnd 
			&& GoodFrom.eFromWnd != Trade_Wnd)
			return true;

		//卸载装备
		if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd == Package_Wnd)
		{
			if(!m_bTrade && !bPeerOK)		 							
			{
				if(GoodFrom.DropGood.IsBind()) //绑定物品不能交易
				{
					////放回包裹
					//for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
					//{	
					//	if(SELF.GetPackageGood(i).GetID() != 0)
					//		continue;
					//	SELF.GetPackageGood(i) = GoodFrom.DropGood;
					//	break;
					//}
					SELF.PackageGood().BackToArray(GoodFrom.DropGood);
					GoodFrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(GoodFrom);
					g_TalkMgr.PushSysTalk("绑定的物品不能交易");

					return true;
				}

				//放物品音乐////////////////////////////
				int i = GoodFrom.DropGood.GetStdMode()+1;
				g_pAudio->Play(EAT_GOODS,i,g_pAudio->GetRand()++);

				if(!g_pGameControl->SEND_Trade_Object_Tackin(GoodFrom.DropGood))
				{
					SELF.PackageGood().BackToArray(GoodFrom.DropGood);
					GoodFrom.DropGood.SetID(0);
					CGoodGrid::SetDropGoodFrom(GoodFrom);
				}
			}
			else
			{
				//已经按下交易按钮，放物品无效					
				for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
				{	
					if(SELF.GetPackageGood(i).GetID() != 0)
						continue;
					SELF.GetPackageGood(i) = GoodFrom.DropGood;
					break;
				}
			}

			GoodFrom.DropGood.SetID(0);
			CGoodGrid::SetDropGoodFrom(GoodFrom);
			return true;
		}
		if(m_bTrade||bPeerOK)
			return true;

		CGood temp = g_TradeData.GetSelfGood(Y * m_iCellsW + X);
		if(temp.GetID() != 0 )//&& GoodFrom.DropGood.GetID() != 0 )    //
			return true; 

		g_TradeData.GetSelfGood(Y * m_iCellsW + X) = GoodFrom.DropGood;
		GoodFrom.DropGood = temp;
		GoodFrom.eFromWnd = Trade_Wnd;
		GoodFrom.iWndPos = Y * m_iCellsW + X;
// 		g_pGameControl->SEND_Trade_Object_Tackout(GoodFrom.DropGood);
		CGoodGrid::SetDropGoodFrom(GoodFrom);
		return true;
	}
	if(IsInMyMoneyGrid(iX,iY))  
	{
		if( CGoodGrid::GetDropGoodFrom().DropGood.GetID() > 0)
			return true;
		if( g_pGameData->GetDropMoneyFrom() == EDMYF_PACKAG)	//来自包裹的黄金
		{
			if(m_bTrade || bPeerOK)					//已按下交易按钮
			{
				g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
				return true;
			}
			g_MsgBoxMgr.PopEditBox("你想支付多少金币?",MSG_CTRL_TRADEGOLDBOX,0,NULL,true);
			g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
			return true;
		}
		else
		{
			if(g_TradeData.GetSelfGolds() ==0 || m_bTrade ||bPeerOK )
				return true;
			else if (g_pGameData->GetDropMoneyFrom() == EDMYF_TRADEWND)    //黄金来自交易窗口，又释放到交易窗口
			{
				g_pGameData->SetDropYuanBaoFrom(EDMYF_NONE);
				g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
			}
			else
			{
				g_pGameData->SetDropYuanBaoFrom(EDMYF_NONE);
				g_pGameData->SetDropMoneyFrom(EDMYF_TRADEWND);
			}
		}
		return true;
	}
	else if(IsInMyYuanBaoGrid(iX,iY))  
	{
		if( CGoodGrid::GetDropGoodFrom().DropGood.GetID() > 0)
			return true;
		if( g_pGameData->GetDropYuanBaoFrom() == EDMYF_PACKAG)	//来自包裹的元宝
		{
			if(m_bTrade || bPeerOK)					//已按下交易按钮
			{
				g_pGameData->SetDropYuanBaoFrom(EDMYF_NONE);
				return true;
			}
			g_MsgBoxMgr.PopEditBox("你想支付多少元宝?",MSG_CTRL_TRADEYUANBAOBOX,0,NULL,true);
			g_pGameData->SetDropYuanBaoFrom(EDMYF_NONE);
			return true;
		}
		else
		{
			if(g_TradeData.GetSelfYuanBao() == 0 || m_bTrade ||bPeerOK )
				return true;
			else if (g_pGameData->GetDropYuanBaoFrom() == EDMYF_TRADEWND)    //元宝来自交易窗口，又释放到交易窗口
			{
				g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
				g_pGameData->SetDropYuanBaoFrom(EDMYF_NONE);
			}
			else
			{
				g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
				g_pGameData->SetDropYuanBaoFrom(EDMYF_TRADEWND);
			}
		}
		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CTradeWnd::OnMouseMove(int iX, int iY)
{
	int X,Y;
	CGood temp;

	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();

	GetMyGrid(&X,&Y,iX,iY);

	if(X != -1 && Y != -1)
	{
		temp =g_TradeData.GetSelfGood(Y * m_iCellsW + X);
	}
	else
	{
		GetPeerGrid(&X,&Y,iX,iY);
		if(X != -1 && Y != -1)
		{
			temp =g_TradeData.GetAnotherGood(Y * m_iCellsW + X);
		}
		else
		{
			temp.SetID(0);
		}
	}
	if(X != -1 && Y != -1 && temp.GetID()!=0)
	{
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
		temp.SetID(0);
		pTip->SetShow(false);

	}

	//pParent->SetFocusGood(temp);
	return CCtrlWindowX::OnMouseMove(iX,iY);
}
