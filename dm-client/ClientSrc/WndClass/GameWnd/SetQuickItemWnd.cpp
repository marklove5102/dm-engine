#include "setquickitemwnd.h"
#include "GameData/WooolStoreData.h"
#include "GameAI/AIConfigMgr.h"

INIT_WND_POSX(CSetQuickItemWnd,POS_VARIABLE,POS_VARIABLE)

CSetQuickItemWnd::CSetQuickItemWnd(void)
{
	m_pCancel = NULL;
	m_bModel = true;


	m_iIndex = 40;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOP;
	m_iOffX = 0;
	m_iOffY = 100;
}

CSetQuickItemWnd::~CSetQuickItemWnd(void)
{
}

void CSetQuickItemWnd::OnCreate()
{
	//	创建按钮
	m_pConfirm = new CCtrlButton();
	AddControl(m_pConfirm);
	m_pConfirm->CreateEx(this,100,128,90,91,92);
	m_pConfirm->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);


	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,220,128,90,91,92);
	m_pCancel->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pCountSelect = new CCtrlMenuButton();
	if (m_pCountSelect)
	{
		AddControl(m_pCountSelect);
		m_pCountSelect->Create(this, 100, 70, 55, 120);
		m_pCountSelect->SetMenuTextColor(0xFFFFFFFF);
		m_pCountSelect->SetFont(FONT_YAHEI);
		m_pCountSelect->AddString("1");
		m_pCountSelect->AddString("2");
		m_pCountSelect->AddString("3");
		m_pCountSelect->AddString("4");
		m_pCountSelect->AddString("5");
		m_pCountSelect->AddString("6");
		m_pCountSelect->SetCurSel(0);
	}
}

void CSetQuickItemWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	char pBuff[1024]= "你想把此道具设置到你的商城快捷栏中吗？";
	g_pFont->DrawText(m_iScreenX+94,m_iScreenY+30,pBuff,COLOR_TEXT_NORMAL,FONT_YAHEI);
	sprintf(pBuff,"将此物品设置到快捷栏第几栏?");
	g_pFont->DrawText(m_iScreenX+94,m_iScreenY+50,pBuff,COLOR_TEXT_NORMAL,FONT_YAHEI);

}


bool CSetQuickItemWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if( m_pConfirm && dwMsg == MSG_CTRL_BUTTON_CLICK && pControl==(CControl *)m_pConfirm )
	{
		OnOK();
		return true;
	}
	else if( m_pCancel && dwMsg == MSG_CTRL_BUTTON_CLICK && pControl==(CControl *)m_pCancel )
	{		
		OnCancel();
		return true;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CSetQuickItemWnd::OnOK()
{
	int iIndex = m_pCountSelect->GetCurSel();

	//保存数据
	if(iIndex < 0 || iIndex >= QUICKITEM_COUNT)
		return;

	CQuickItem& qitem = g_WooolStoreMgr.GetQuickItem(iIndex);
	qitem = g_WooolStoreMgr.GetTmpQuickItem();

	for(int i = 0;i<QUICKITEM_COUNT;i++)
	{
		CQuickItem tempqitem = g_WooolStoreMgr.GetQuickItem(i);
		//g_pAssConfig->SetQuickItem(i,tempqitem);
		g_AICfgMgr.SetQuickItem(i,tempqitem);
	}

	CloseWindow();
}

void CSetQuickItemWnd::OnCancel()
{
	CloseWindow();
}
