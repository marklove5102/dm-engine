#include "SealBookWnd.h"
#include "sealwnd.h"
#include "Global/Global.h"
#include "Global/GlobalMsg.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/TalkMgr.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/ItemCfgMgr.h"
#include "GameData/MsgBoxMgr.h"

INIT_WND_POSX(CSealBookWnd,POS_AUTO,POS_AUTO)

CSealBookWnd::CSealBookWnd(void)
{
	m_iType = 0;
	m_dwCurrentBookID = 0;

	ZeroMemory(m_szName,32);

	m_ptCardPos[0].x = 90;
	m_ptCardPos[0].y = 80;

	m_ptCardPos[1].x = 215;
	m_ptCardPos[1].y = 80;

	m_ptCardPos[4].x = 155;
	m_ptCardPos[4].y = 176;

	m_ptCardPos[3].x = 90;
	m_ptCardPos[3].y = 271;

	m_ptCardPos[2].x = 215;
	m_ptCardPos[2].y = 271;

	m_ptCardPos[5].x = m_ptCardPos[0].x + 293;
	m_ptCardPos[5].y = m_ptCardPos[0].y;

	m_ptCardPos[6].x = m_ptCardPos[1].x + 293;
	m_ptCardPos[6].y = m_ptCardPos[1].y;

	m_ptCardPos[7].x = m_ptCardPos[2].x + 293;
	m_ptCardPos[7].y = m_ptCardPos[2].y;

	m_ptCardPos[8].x = m_ptCardPos[3].x + 293;
	m_ptCardPos[8].y = m_ptCardPos[3].y;

	m_ptCardPos[9].x = m_ptCardPos[4].x + 293;
	m_ptCardPos[9].y = m_ptCardPos[4].y;

	m_iIndex = 15175;
	m_iPages = 1;
	
}

CSealBookWnd::~CSealBookWnd(void)
{
}

void CSealBookWnd::OnCreate()
{
	m_iSelectedNum = 0;
	SetMask(15175,691,422);
	SetCloseButton(605,11);	
}

void CSealBookWnd::GetBookInfo(CGood* pGood)
{
	if(pGood == NULL)
	{
		return;
	}

	if(pGood->GetID() != 0 && pGood->GetStdMode() == 49 && pGood->GetShape() == 56)		//封元册
	{
        m_iType = pGood->GetAC();
		m_dwCurrentBookID = pGood->GetID();

		if(m_iType == 0)
		{
			if(pGood->GetMAC() == 1)
			{
				strcpy(m_szName,"玄华封元册");
			}
			else
			{
				strcpy(m_szName,"  封元册");
			}
		}
		
		char szKey[16] = "";
		char szName[64] = "";
		
		m_vecCardNames.clear();
		for(int i = 1;i <= 10;i++)		//获得所有的卡片名字
		{
			sprintf(szKey,"%d",m_iType * 20 + i);
			g_ItemCfgMgr.ParseCardInfo("CardName",szKey,szName,NULL); 
			m_vecCardNames.push_back(szName);
		}
	}	
}

int  CSealBookWnd::GetSelectedTNum(int iPos)
{
	unsigned char name[10] = {0};

	CGood* pGood = SELF.PackageGood().FindGood(m_dwCurrentBookID);    

	if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() == m_dwCurrentBookID && pGood == NULL )
	{
		pGood = &(CGoodGrid::GetDropGoodFrom().DropGood);
	}

	if(pGood && pGood->GetID() != 0 && pGood->GetStdMode() == 49 && pGood->GetShape() == 56)
	{
		*(WORD*)(name) = pGood->GetDemonDark1();
		*(WORD*)(name+2) = pGood->GetDemonDark2();
		*(WORD*)(name+4) = pGood->GetDemonDark3();

		for(int ii = 0;ii < 4;ii++)
		{
			name[6+ii] = pGood->GetResvEx(ii);
		}
	}

	if(iPos >0 && iPos < 11)
		return name[iPos - 1];
	else
		 return 0;
}

bool CSealBookWnd::OnLeftButtonDClick(int iX,int iY)
{
	if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE || g_pGameData->GetDropYuanBaoFrom() != EDMYF_NONE)
		return true; 

	stGoodFrom stDropGood = CGoodGrid::GetDropGoodFrom();
	if(stDropGood.DropGood.GetID() != 0 && stDropGood.eFromWnd == SealBook_Wnd)
	{
		stDropGood.DropGood.SetID(0);
		CGoodGrid::SetDropGoodFrom(stDropGood);

		GetSelectedPos(iX,iY);
		if(m_iSelectedNum == 0)
		{
			return true;
		}

		if(GetSelectedTNum(m_iSelectedNum) > 0)
		{
			g_pGameControl->SEND_Seal_Info(m_iSelectedNum,2,m_dwCurrentBookID);
		}
	}
	
	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

bool CSealBookWnd::OnLeftButtonUp(int iX,int iY)
{
	stGoodFrom stDropGood = CGoodGrid::GetDropGoodFrom();

	if(stDropGood.DropGood.GetID() != 0 && stDropGood.eFromWnd != SealBook_Wnd)
	{
		if(stDropGood.eFromWnd == Package_Wnd)
		{

			CGood * pGood = SELF.PackageGood().FindGood(m_dwCurrentBookID);
			
			if(pGood == NULL)
			{
				return true;
			}

			if(stDropGood.DropGood.GetID() != 0 && stDropGood.DropGood.GetStdMode() == 49 && stDropGood.DropGood.GetShape() == 55)
			{
                if(stDropGood.DropGood.GetMAC() == 1 && pGood->GetMAC() == 0)
				{
                    m_bClick = false;
                    g_MsgBoxMgr.PopSimpleAlert("封元册只能存放普通封元印");
					g_TalkMgr.PushSysTalk("封元册只能存放普通封元印");
					return true;
				}

				if(stDropGood.DropGood.GetMAC() == 0 && pGood->GetMAC() == 1)
				{
                    m_bClick = false;
                    g_MsgBoxMgr.PopSimpleAlert("玄华封元册只能存放玄华封元印");
					g_TalkMgr.PushSysTalk("玄华封元册只能存放玄华封元印");
					return true;
				}

				if(pGood->GetDura() == 0)
				{
					if(pGood->GetMAC() == 0)
					{
						g_TalkMgr.PushSysTalk("封元册持久度为0，无法继续存放");
						return true;
					}

					if(pGood->GetMAC() == 1)
					{
						g_TalkMgr.PushSysTalk("玄华封元册持久度为0，无法继续存放");
						return true;
					}
				}

				g_pGameControl->SEND_Seal_Info(stDropGood.DropGood.GetID(),1,m_dwCurrentBookID);
				CGood * pGood = SELF.GetUsingTemp().Add(stDropGood.DropGood);
				if(pGood) 
					pGood->SetPos(0);
				stDropGood.DropGood.SetID(0);
				CGoodGrid::SetDropGoodFrom(stDropGood);

//				g_pAudio->Play( 9000027,g_pAudio->GetRand()++); //暂时注释，待修改
			}
		}

		return true;
	}

	if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE || g_pGameData->GetDropYuanBaoFrom() != EDMYF_NONE)
		return true;

	bool bClear = false;
	int iOldPos = m_iSelectedNum;
	GetSelectedPos(iX,iY);
	if(m_iSelectedNum == 0) 
		bClear = true;

	bClear = (GetSelectedTNum(m_iSelectedNum)== 0) || bClear;

	if((stDropGood.DropGood.GetID() != 0 && stDropGood.eFromWnd == SealBook_Wnd) || bClear)
	{
		if(m_iSelectedNum == iOldPos || bClear)
		{
			m_iSelectedNum = 0;
			stGoodFrom stgood;
			stgood.DropGood.SetID(0);
			CGoodGrid::SetDropGoodFrom(stgood);
			return true;
		}
	}
    
	stGoodFrom stgood;
	stgood.DropGood.SetID(m_dwCurrentBookID);
	stgood.DropGood.SetAC(m_iSelectedNum);
	stgood.DropGood.SetStdMode(49);
	stgood.DropGood.SetShape(55);
	stgood.DropGood.SetLooks(5035);
	stgood.eFromWnd = SealBook_Wnd;
	CGoodGrid::SetDropGoodFrom(stgood);
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

void CSealBookWnd::GetSelectedPos(int iX,int iY)
{
	m_iSelectedNum = 0;
	for(int i = 0;i < 10;i++)
	{
		if(iX > m_ptCardPos[i].x && iX < m_ptCardPos[i].x + 60 && iY > m_ptCardPos[i].y && iY < m_ptCardPos[i].y + 82)
		{
			m_iSelectedNum = i + 1;
			break;
		}
	}
}

bool CSealBookWnd::OnRightButtonUp(int iX,int iY)
{
	stGoodFrom stDropGood = CGoodGrid::GetDropGoodFrom();

	if(stDropGood.DropGood.GetID() != 0)
	{
		return true;
	}

	if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE || g_pGameData->GetDropYuanBaoFrom() != EDMYF_NONE)
		return true;

	//查找相应的位置
	CGood * pGood = SELF.PackageGood().FindGood(m_dwCurrentBookID);
	
	if(pGood == NULL) return true;

	m_iSelectedNum = 0;
	if(pGood->GetStdMode() == 49 && pGood->GetShape() == 56)
	{
		GetSelectedPos(iX,iY);
		
		if(m_iSelectedNum == 0) return true;
		if(GetSelectedTNum(m_iSelectedNum) == 0) return true;
		DWORD dwValue = 0;
		if(pGood->GetMAC() == 0)
		{
			dwValue = MAKELONG(pGood->GetAC(),m_iSelectedNum);
		}
		else
		{
			dwValue = MAKELONG(pGood->GetAC(),m_iSelectedNum + 10);
		}
		m_bRBClick = false;
		g_pControl->PopupWindow(MSG_CTRL_SEAL_WND,OPER_CREATE);
        g_pControl->Msg(MSG_CTRL_SEAL_WND,2,(CControl*)dwValue);
		return true;
	}

	return CCtrlWindowX::OnRightButtonUp(iX,iY);
}

bool CSealBookWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_SEALBOOK_WND:
		if(dwData == 1)
		{
            CGood* pGood = (CGood*)pControl;
			GetBookInfo(pGood);			
			return true;
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CSealBookWnd::DwawSingleCard(int iX,int iY,int iIndex,int iCount,bool IsFlash)
{
	//画底框
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15177,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(iX,iY,pTex);
	}

	//画怪物
	if(iCount > 0)
	{
		if(IsFlash)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15180,EP_UI);
			if(pTex)
			{
				g_pGfx->DrawTextureNL(iX - 99,iY - 87,pTex);
			}
		}

		if(iIndex>= 0&& iIndex <= 9)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15400 + iIndex,EP_UI);
		}

        
		if(pTex)
		{
			g_pGfx->DrawTextureNL(iX,iY,pTex);

			if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() != 0 && CGoodGrid::GetDropGoodFrom().eFromWnd == SealBook_Wnd)
			{
				if(m_iSelectedNum == (iIndex + 1))
				{
					g_pGfx->SetRenderMode(RM_ADD1);				
					g_pGfx->DrawTextureNL(iX,iY,pTex,0x71FFFFFF);
					g_pGfx->SetRenderMode();
				}
			}
			else
			{
				int x,y;
				g_pInput->GetMousePos(x,y);
				x -= m_iScreenX;
				y -= m_iScreenY;

				if(x > m_ptCardPos[iIndex].x && x < m_ptCardPos[iIndex].x + 60 && y > m_ptCardPos[iIndex].y && y < m_ptCardPos[iIndex].y + 82)
				{
					g_pGfx->SetRenderMode(RM_ADD1);				
					g_pGfx->DrawTextureNL(iX,iY,pTex,0x71FFFFFF);
					g_pGfx->SetRenderMode();
				}
			}			
		}		
	}
	else
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15300 + iIndex,EP_UI);
		if(pTex)
		{
			g_pGfx->DrawTextureNL(iX,iY,pTex);
		}
	}
	
	//画边角
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15176,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(iX - 5,iY + 63,pTex);
	}

	//显示名字
	char szName[32] = "";
	if(iCount > 1)
	{
		sprintf(szName,"%s*%d",m_vecCardNames[iIndex].c_str(),iCount);
	}
	else
	{
		sprintf(szName,"%s",m_vecCardNames[iIndex].c_str());
	}

	if(m_iType == 0)
	{
		g_pFont->DrawText(iX + 30 - strlen(szName) * 3,iY + 84,szName,0xff100900);
	}
}

void CSealBookWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_iType == 0)
	{
		g_pFont->DrawText(m_iScreenX + 445,m_iScreenY + 48,"铁血魔城",0xff382610,FONT_LISHU,18);
	}

	g_pFont->DrawText(m_iScreenX + 140,m_iScreenY + 48,m_szName,0xff382610,FONT_LISHU,18);

	CGood * pGood = SELF.PackageGood().FindGood(m_dwCurrentBookID);
	unsigned char name[10] = {0};

	if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() == m_dwCurrentBookID && pGood == NULL )
	{
		pGood = &(CGoodGrid::GetDropGoodFrom().DropGood);
	}

	bool bFlash = false;
	if(pGood != NULL && pGood->GetMAC() == 1)
	{
		bFlash = true;
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15178,EP_UI);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + 220,m_iScreenY + 85,pTex);
			g_pGfx->SetRenderMode();
		}
	}

	if(pGood && pGood->GetID() != 0 && pGood->GetStdMode() == 49 && pGood->GetShape() == 56)
	{
		*(WORD*)(name) = pGood->GetDemonDark1();
		*(WORD*)(name+2) = pGood->GetDemonDark2();
		*(WORD*)(name+4) = pGood->GetDemonDark3();

		for(int ii = 0;ii < 4;ii++)
		{
			name[6+ii] = pGood->GetResvEx(ii);
		}
	}

	for(int i = 0;i < m_vecCardNames.size();i++)
	{
		DwawSingleCard(m_iScreenX + m_ptCardPos[i].x,m_iScreenY + m_ptCardPos[i].y,i,name[i],bFlash);
	}
}

bool CSealBookWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();	
	pTip->SetShow(false);

	int i = 0;
	for(;i < 10;i++)
	{
		if(iX > m_ptCardPos[i].x && iX < m_ptCardPos[i].x + 60 && iY > m_ptCardPos[i].y && iY < m_ptCardPos[i].y + 82 && GetSelectedTNum(i + 1))
		{
			pTip->Clear();
			pTip->AddText("右键查看，双击或拖动到包裹中取出。",0xFFFFCF63);
			int x = m_iScreenX + iX + 10;
	        int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
		    pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

