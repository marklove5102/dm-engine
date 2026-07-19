#include "QuickItemFashionalWnd.h"
#include "GameData/WooolStoreData.h"
#include "GameAI/AIConfigMgr.h"
#include "GameData/ConfigData.h"
#include "Baseclass/Control/ParserTip.h"

const POINT CQuickItemFashionalWnd::m_ptPos[6] = {
	{ 3, 30 },
	{ 3, 65 },
	{ 3, 100 },
	{ 3, 135 },
	{ 3, 170 },
	{ 3, 205 },
};


INIT_WND_POSX(CQuickItemFashionalWnd,POS_VARIABLE,POS_VARIABLE)

CQuickItemFashionalWnd::CQuickItemFashionalWnd(void)
{	
	m_bNoMove = true;
	m_bNoChangeLevel = true;
	m_bNeedSavePos  = false;
	m_bDisableEscape = true;

	m_iIndex = 14735;
	m_iPages = 1;

	m_iAlignType = XAL_RIGHT;
	m_iOffX = -10;
	m_iOffY = 10;
}

CQuickItemFashionalWnd::~CQuickItemFashionalWnd(void)
{
}

void CQuickItemFashionalWnd::OnCreate()
{
	
}

void CQuickItemFashionalWnd::Draw(void)
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//绘制快捷消费栏
	DWORD dwTime = GetTickCount();
	for(int i = 0;i<QUICKITEM_COUNT;++i)
	{
		CQuickItem& qitem = g_WooolStoreMgr.GetQuickItem(i);
		// 绘制物品图标
		if(dwTime > qitem.dwTime)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_items, qitem.iItemLooks,EP_UI);
			if(pTex)
				DrawTexture(m_ptPos[i].x,m_ptPos[i].y,pTex);
		}
	}
}


bool CQuickItemFashionalWnd::OnLeftButtonDClick(int iX,int iY)
{
	int index;
	if(IsInStoreGrid(iX,iY,index))
	{
		m_bClick = false;

		if (index < 0 || index >= QUICKITEM_COUNT)
		{
			return true;
		}

		CQuickItem& qitem = g_WooolStoreMgr.GetQuickItem(index);
		if(qitem.strName.empty() || qitem.iItemLooks == 0)
			return true;

		if(g_Config.GetCheckQuick())
			g_pControl->PopupWindow( MSG_CTRL_CHECK_QUICK_WND,OPER_CREATE,index);
		else
			g_WooolStoreMgr.UseQuickItem(index);

		return true;
	}
	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

bool CQuickItemFashionalWnd::OnMouseMove(int iX, int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();
	pTip->SetShow(false);

	int index;
	if (IsInStoreGrid(iX,iY,index))
	{
		//快速消费提示信息
		CQuickItem& qitem = g_WooolStoreMgr.GetQuickItem(index);
		pTip->Clear();
		if(qitem.strName.size() >0)
		{
			pTip->AddText(qitem.strName.c_str(),0xFFFFFF00);
			pTip->AddText("双击购买使用");
			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
		}		
		return true;
	}	

	return false;
}

bool CQuickItemFashionalWnd::IsInStoreGrid(int ix,int iy,int &iIndex)
{
	for (int i = 0; i < 6; ++i)
	{
		if(ix > m_ptPos[i].x && ix < m_ptPos[i].x + 32 && iy > m_ptPos[i].y && iy < m_ptPos[i].y + 32)
		{
			iIndex = i;
			return true;
		}
	}

	return false;
}