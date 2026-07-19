#include "PetPackageWnd.h"

#include "GameData/GameData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameControl/GameControl.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/TalkMgr.h"

INIT_WND_POSX(CPetPackageWnd,POS_VARIABLE,POS_VARIABLE)

CPetPackageWnd::CPetPackageWnd()
{
	m_iType = MAX_PET_PACKAGE / 5;
	m_iWCells = 5;
	m_iHCells = m_iType;
	m_iCellWidth  = 39;
	m_iCellHeight  = 35;
	m_iCellDisW = 0;
	m_iCellDisH = 0;
	m_bDClicked = false;

	if(m_iType == 1)
		m_iIndex = 4857;
	else
		m_iIndex = 4858;

	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 60;
	m_iOffY = 30;	
}

CPetPackageWnd::~CPetPackageWnd(void)
{

}

bool CPetPackageWnd::Create(CControl* pParent,int iCurPage)
{
	if(m_iType == 0)
	{
		//没有宠物包裹
		if(SELF.GetMyPetID() > 0)
		{
			g_TalkMgr.PushSysTalk("您需要放出1级以上的灵兽才能使用灵兽包裹。");
		}
		else
		{
			g_TalkMgr.PushSysTalk("你不能打开灵兽包裹，请放出灵兽之后再使用灵兽包裹。");
		}

		return false;
	}

	return CCtrlWindowX::Create(pParent,iCurPage);
}

void CPetPackageWnd::OnCreate()
{
	g_pControl->SetWindowPos((g_pGfx->GetWidth() > 800)?500:340,50,"PackageWnd");
	g_pControl->PopupWindow(MSG_CTRL_PACKAGEWND,OPER_CREATE);

	m_pGoodGrid = new CGoodGrid(m_iWCells,m_iHCells,m_iCellWidth,m_iCellHeight,m_iCellDisW,m_iCellDisH);
	AddControl(m_pGoodGrid);
	m_pGoodGrid->SetGridType(GOODGRID_TYPE_PETPACKAGE);
	m_pGoodGrid->Create(this,42,43);

	SetCloseButton(240,20);
	
	if(m_iType == 1)
		SetMask(4857);
	else
		SetMask(4858);
}

void CPetPackageWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

}

bool CPetPackageWnd::OnLeftButtonDown(int iX, int iY)
{
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CPetPackageWnd::OnLeftButtonUp(int iX, int iY)
{
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CPetPackageWnd::OnMouseMove(int iX, int iY)
{
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

