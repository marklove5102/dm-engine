#include "QuickBuyWnd.h"
#include "GameData/WooolStoreData.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include "GameData/MsgBoxMgr.h"
#include <vector>
#include "GameData/WooolStoreData.h"
#include "GameData/MacroDefine.h"

INIT_WND_POSX(CQuickBuyWnd,POS_AUTO,POS_AUTO)

CQuickBuyWnd::CQuickBuyWnd(void)
{
	m_pOK = NULL ;
	m_pCancel = NULL ;
	//m_bAlwaysOnTop = true;

	m_iIndex = 30;
	m_iPages = 1;
	

	//CQuickBuyData& rBuyData = g_WooolStoreMgr.GetQuickBuyData();
	//if(rBuyData.pItem && rBuyData.pItem->strName == "超级冰泉圣水")
	//{
	//	m_iAlignType = XAL_BOTTOMLEFT;
	//	m_iOffX = 5;

	//	if((g_pGfx->GetWidth() > 800))
	//		m_iOffX = - 205;
	//	else
	//		m_iOffY = - 107;
	//}
	//else  //居中
	{
		m_iAlignType = XAL_CENTER;
		m_iOffX = 0;
		m_iOffY = - 50;
	}

}

CQuickBuyWnd::~CQuickBuyWnd(void)
{

}

void CQuickBuyWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	std::vector<std::string>   vDrawStr;
	CutByUnicode((char*)(g_WooolStoreMgr.GetQuickBuyData().strMsg.c_str()),vDrawStr,26);

	for(size_t i = 0,j = 0; i < vDrawStr.size(); i++,j += 14)
	{
		g_pFont->DrawText(m_iScreenX+ 27,m_iScreenY + 17+j,vDrawStr[i].c_str(),COLOR_TEXT_NORMAL, FONT_YAHEI);
	}


}

bool CQuickBuyWnd::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{

	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pOK)
		{
			CQuickItem *pQuickItem = g_WooolStoreMgr.GetQuickBuyData().pItem;
			if(!pQuickItem)
				return true;


			int iCount = 1;//pQuickBuyItem->iBuyCount;
			int iPrice = pQuickItem->iPrice;

			if(iCount>=1 && iPrice>=1)
			{
				if(iCount>9 )
					iCount = 9;
				iPrice *= iCount;
			}

			if ( iCount > GetLeftGrid())
			{
				g_MsgBoxMgr.PopSimpleAlert(STRING_PACKAGE_FULL);
				CloseWindow();
				return true;
			}
			//else if (SELF.GetYuanBao() < iPrice)
			//{
			//	g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
			//	CloseWindow();
			//	return true;
			//}

			pQuickItem->iType = 1;
			bool bUse = g_WooolStoreMgr.GetQuickBuyData().iUseAfterBuyType == 0?false:true;
			g_WooolStoreMgr.BuyQuickItem(*pQuickItem,bUse,g_WooolStoreMgr.GetQuickBuyData().iBuyCount);

			//关闭当前窗口
			CloseWindow();
			return true;
		}
		else if(pControl == m_pCancel)
		{
			//关闭当前窗口
			CloseWindow();
			return true;
		}

	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CQuickBuyWnd::OnCreate(void)
{
	//确定按钮
	m_pOK = new CCtrlButton();
	if(m_pOK)
	{
		AddControl(m_pOK);
		m_pOK->CreateEx(this,30,105,90,91,92);
		m_pOK->SetText("购 买", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	}
	//取消按钮
	m_pCancel = new CCtrlButton();
	if(m_pCancel)
	{
		AddControl(m_pCancel);
		m_pCancel->CreateEx(this,130,105,90,91,92);
		m_pCancel->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	}

	SetCloseButton(220,3);

	m_dwStartTime = GetTickCount();
	SetLifeTime(m_dwStartTime + 10000);
}

int CQuickBuyWnd::GetLeftGrid()
{
	int iReturn  = 0;
	for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6; ++i)
	{
		if(SELF.GetPackageGood(i).GetID() == 0)
			++iReturn;
	}

	return iReturn;
}

bool CQuickBuyWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	if(cKey == VK_RETURN)
	{
		this->Msg(MSG_CTRL_BUTTON_CLICK,0,m_pOK);
		return true;
	}
	return CCtrlWindowX::OnKeyDown(wState,cKey);
}


