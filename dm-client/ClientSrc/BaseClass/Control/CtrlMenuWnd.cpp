#include "CtrlMenuWnd.h"
#include "GameData/GameData.h"
#include "Baseclass/Control/ParserTip.h"

CCtrlMenuWnd::CCtrlMenuWnd()
{
	m_iControlMode = CTRL_MODE_POPMENU;
	m_fItemHeight = 20.0f;
	m_iMouseOnItemIdx = -1;
	m_pCaller = NULL;
}

CCtrlMenuWnd::~CCtrlMenuWnd(void)
{
	m_pCaller = NULL;
	m_VMenuItem.clear();
	m_SPopMenuData.clear();
}

bool CCtrlMenuWnd::Create(CControl* pParent)
{
	int iX = 0,iY = 0,iW = 0,iH = 0;
	m_fItemHeight = m_SPopMenuData.fItemHeight;

	LPTexture pTex = NULL;
	if(m_SPopMenuData.wBackTex)
	{
		pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,m_SPopMenuData.wBackTex,EP_UI);
		if(pTex)
		{
			iW = pTex->GetWidth0();
			iH = pTex->GetHeight0();
		}
	}
	
	if(NULL == pTex)
	{
		int iMaxLen = 0,iLen = 0;
		size_t size = m_VMenuItem.size();
		for(size_t i = 0; i < size; i++)
		{
			iLen = m_VMenuItem[i].strText.length();
			if(iLen > iMaxLen)
				iMaxLen = iLen;
		}

		int iFontHeight = m_SPopMenuData.iFontSize;
		int iFontWidth = iFontHeight/2;

		iW = m_SPopMenuData.iW;
		iH = m_SPopMenuData.iH;
		if(iW <= 0)
			iW = iFontWidth * iMaxLen + m_SPopMenuData.iLeftOffX + m_SPopMenuData.iRightOffX;
		if(iH <= 0)
			iH = (int)( m_fItemHeight*size) + m_SPopMenuData.iTopOffY + m_SPopMenuData.iBottomOffY;
	}

	m_iItemWidht = iW;

	g_pInput->GetMousePos(iX,iY);// 得到鼠标坐标	
	if(iX + iW > g_pGfx->GetWidth())
		iX = iX - iW;

	if(iY + iH > g_pGfx->GetHeight())
		iY = iY - iH;

	if(m_SPopMenuData.iX > -100 )//如果菜单数据中有设置以设置的为准
		iX = m_SPopMenuData.iX;
	if(m_SPopMenuData.iY > -100 )//如果菜单数据中有设置以设置的为准
		iY = m_SPopMenuData.iY;


	return CControl::Create(pParent,iX,iY,iW,iH,CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX, m_SPopMenuData.dwBackColor,m_SPopMenuData.wBackTex>0?MAKELONG(m_SPopMenuData.wBackTex,PACKAGE_INTERFACE):0);
}

bool CCtrlMenuWnd::OnMouseMove(int iX, int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->SetShow(false);
	pTip->Clear();

	m_iMouseOnItemIdx = (int)(iY / m_fItemHeight);

	if(m_iMouseOnItemIdx >= 0 && m_iMouseOnItemIdx < m_VMenuItem.size() && m_VMenuItem[m_iMouseOnItemIdx].strTips.length() > 0)
	{
		pTip->SetText(m_VMenuItem[m_iMouseOnItemIdx].strTips.c_str());
		pTip->SetShow(true);
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
	}

	return true;
}

void CCtrlMenuWnd::Draw()
{
	CCtrlWindow::Draw();

	if(m_dwBackTexID == 0)
	{
		g_pGfx->DrawRectangleWithFrame(0xFF8C7C40,m_SPopMenuData.dwBackColor,m_iScreenX,m_iScreenY,m_iScreenX + m_iWidth ,m_iScreenY + m_iHeight,1);
	}

	VPopMenuItem::iterator itr;
	int iFontHeight = m_SPopMenuData.iFontSize;

	if(m_iMouseOnItemIdx >= 0 && m_iMouseOnItemIdx < m_VMenuItem.size() && 
		m_VMenuItem[m_iMouseOnItemIdx].dwItemId != POP_MENU_ITEM_NONE && m_VMenuItem[m_iMouseOnItemIdx].bEnable)
	{
		int iTop = m_iScreenY+m_SPopMenuData.iTopOffY+(int)(m_iMouseOnItemIdx*m_fItemHeight);
		g_pGfx->DrawFillRect(m_iScreenX + m_SPopMenuData.iSelBackColorLeftOffX,iTop - 2,m_iItemWidht - m_SPopMenuData.iSelBackColorLeftOffX - m_SPopMenuData.iSelBackColorRigthOffX,iFontHeight + 4,m_SPopMenuData.dwSelectColor);
	}

	int iNum = 0;
	for(itr = m_VMenuItem.begin(); itr != m_VMenuItem.end(); itr ++,iNum ++)
	{
		if(itr->bEnable)
		{
			DWORD dwColor = m_SPopMenuData.dwItemTextColor;
			if (iNum == m_iMouseOnItemIdx && m_SPopMenuData.dwHoverTextColor!=0)
				dwColor = m_SPopMenuData.dwHoverTextColor;
			if(itr->dwItemId == POP_MENU_ITEM_NONE)
				dwColor = 0xFFFFFFFF;

			g_pFont->DrawText(m_iScreenX+m_SPopMenuData.iLeftOffX,m_iScreenY+m_SPopMenuData.iTopOffY+(int)(iNum*m_fItemHeight),itr->strText.c_str(),dwColor,m_SPopMenuData.iFontType,m_SPopMenuData.iFontSize);
		}
		else
			g_pFont->DrawText(m_iScreenX+m_SPopMenuData.iLeftOffX,m_iScreenY+m_SPopMenuData.iTopOffY+(int)(iNum*m_fItemHeight),itr->strText.c_str(),m_SPopMenuData.dwItemDisableTextColor,m_SPopMenuData.iFontType,m_SPopMenuData.iFontSize);

	}

}

bool CCtrlMenuWnd::OnLeftButtonUp(int iX, int iY)
{
	m_bClick = false;
	int iClickItemIdx = (int)(iY / m_fItemHeight);
	if(m_pCaller && iClickItemIdx >= 0 && iClickItemIdx < m_VMenuItem.size() 
		&& m_VMenuItem[iClickItemIdx].bEnable && m_VMenuItem[iClickItemIdx].dwItemId != POP_MENU_ITEM_NONE)
	{
		m_pCaller->Msg(MSG_CTRL_POP_MENU, m_VMenuItem[iClickItemIdx].dwItemId);
		CloseMenu();
		return true;
	}

	return CCtrlWindow::OnLeftButtonUp(iX, iY);
}

void CCtrlMenuWnd::OnKillFocus()
{
	CloseMenu();//该控件不能失去焦点，一失去就关闭
}

void S_POP_MENU_DATA::clear()
{
	dwSelectColor = 0xFF2F8405;
	dwItemTextColor = 0xFFFFFF00;
	dwItemDisableTextColor = 0xFFA6A5A3;
	dwHoverTextColor = 0;
	wBackTex = 0;
	dwBackColor = 0x88000000;
	iX = -101;
	iY = -101;
	iLeftOffX = 3;
	iTopOffY = 5;
	iRightOffX = 3;
	iBottomOffY = 5;
	fItemHeight = 17;
	iFontType = FONT_DEFAULT;
	iFontSize = FONTSIZE_DEFAULT;
	iW = 0;
	iH = 0;
	iSelBackColorLeftOffX = 1;
	iSelBackColorRigthOffX = 1;
}

void CCtrlMenuWnd::Clear()
{
	m_SPopMenuData.clear();
	m_VMenuItem.clear();
}

void CCtrlMenuWnd::ShowMenu()
{
	Create(g_pControl);
	m_bShow = true;
	m_bEnable = true;
	SetFocus();
}

void CCtrlMenuWnd::CloseMenu()
{
	m_bShow = false;
	m_bEnable = false;
	//右键点击玩家头像,弹出右键菜单,然后再用左键或右键点击某个菜单项不放,此时选中的人跳地图或下线,人物头像消失后会导致鼠标无任怎么点都不响应.
	//因为这个时候焦点在此控件，然而此控件的m_bClick=true,所以无法让其它控件获得焦点，因此加上下面的代码
	m_bFocus = false;
	m_bClick = false;
	m_bRBClick = false;
	m_pCaller = NULL;
	if(g_pControl->GetFocusCtrl() == this)
	{
		//g_pControl->SetFocus();//不能这样,否则会死循环
		g_pControl->SetFocusCtrl(g_pControl);
	}
}

bool CCtrlMenuWnd::AddMenuItem(const POP_MENU_ITEM &sMenuItem)
{
	if(sMenuItem.dwItemId < 0 )
		return false;

	m_VMenuItem.push_back(sMenuItem);
	return true;
}

bool CCtrlMenuWnd::AddMenuItem(DWORD dwItemId,string strText,string strTips,bool bEnable)
{
	if(dwItemId < 0 )
		return false;

	POP_MENU_ITEM sMenuItem;

	sMenuItem.dwItemId = dwItemId;
	sMenuItem.strText = strText;
	sMenuItem.strTips = strTips;
	sMenuItem.bEnable = bEnable;

	AddMenuItem(sMenuItem);
	return true;
}

bool CCtrlMenuWnd::DelMenuItem(DWORD dwItemID)
{
	VPopMenuItem::iterator iter;
	for(iter = m_VMenuItem.begin(); iter != m_VMenuItem.end(); iter++)
	{
		if(iter->dwItemId == dwItemID)
		{
			m_VMenuItem.erase(iter);
			return true;
		}
	}

	return false;
}



bool CCtrlMenuWnd::SetMenuItemEnable(DWORD dwItemID,bool bEnable)
{
	VPopMenuItem::iterator iter;
	for(iter = m_VMenuItem.begin(); iter != m_VMenuItem.end(); iter++)
	{
		if(iter->dwItemId == dwItemID)
		{
			iter->bEnable = bEnable;
			return true;
		}
	}

	return false;
}

bool CCtrlMenuWnd::SetMenuItem(DWORD dwItemId, std::string strText, std::string strTips, bool bEnable)
{
	VPopMenuItem::iterator iter;
	for(iter = m_VMenuItem.begin(); iter != m_VMenuItem.end(); iter++)
	{
		if(iter->dwItemId == dwItemId)
		{			
			iter->strText = strText;
			iter->strTips = strTips;
			iter->bEnable = bEnable;
			return true;
		}
	}
	return false;
}

CCtrlMenuButtonWnd::CCtrlMenuButtonWnd()
{

}

CCtrlMenuButtonWnd::~CCtrlMenuButtonWnd()
{

}

void CCtrlMenuButtonWnd::Clear()
{
	CControl * p = m_pControls;
	CControl * pTemp = m_pControls;

	while( p )
	{
		pTemp = p->GetControlNext();
		RemoveControl(&p);		
		p = pTemp;
	}

	m_VMenuButtonItem.clear();
}

void CCtrlMenuButtonWnd::AddMenuButtonItem(DWORD dwItemId,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex)
{
	POP_MENUBUTTON_ITEM sMenuButtonItem;

	sMenuButtonItem.dwItemId = dwItemId;
	sMenuButtonItem.pbutton = new CCtrlButton;
	sMenuButtonItem.pbutton->CreateEx(this,0,0,wTex,wMTex,wCTex,wDTex);
	this->AddControl(sMenuButtonItem.pbutton);

	m_VMenuButtonItem.push_back(sMenuButtonItem);

	int iW = 0,iH = 0;
	VPopMenuButtonItem::iterator iter;
	for(iter = m_VMenuButtonItem.begin(); iter != m_VMenuButtonItem.end(); iter++)
	{		
		if(iter->pbutton->GetWidth() > iW)
		{			
			iW = iter->pbutton->GetWidth();						
		}
		iter->pbutton->Move(0,iH);
		iH += iter->pbutton->GetHeight();
	}

	ReSize(iW,iH);
}

bool CCtrlMenuButtonWnd::SetMenuButtonItem(DWORD dwItemId,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex,bool bEnable)
{
	VPopMenuButtonItem::iterator iter;
	for(iter = m_VMenuButtonItem.begin(); iter != m_VMenuButtonItem.end(); iter++)
	{
		if(iter->dwItemId == dwItemId)
		{			
			iter->pbutton->ReloadTex(wTex,wMTex,wCTex,wDTex);
			iter->pbutton->SetEnable(bEnable);
			return true;
		}
	}

	return false;
}

bool CCtrlMenuButtonWnd::SetMenuButtonItem(DWORD dwItemId,bool bEnable)
{
	VPopMenuButtonItem::iterator iter;
	for(iter = m_VMenuButtonItem.begin(); iter != m_VMenuButtonItem.end(); iter++)
	{
		if(iter->dwItemId == dwItemId)
		{			
			iter->pbutton->SetEnable(bEnable);
			return true;
		}
	}

	return false;
}

bool CCtrlMenuButtonWnd::Create(CControl* pParent,int iX,int iY)
{
	return CControl::Create(pParent,iX,iY,0,0);
}

bool CCtrlMenuButtonWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		VPopMenuButtonItem::iterator iter;
		for(iter = m_VMenuButtonItem.begin(); iter != m_VMenuButtonItem.end(); iter++)
		{	
			if(iter->pbutton == pControl)
			{
				m_pParent->Msg(MSG_CTRL_POP_MENU, iter->dwItemId);
				SetShow(false);
				SetEnable(false);
			}
		}

		return true;
	}

	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}