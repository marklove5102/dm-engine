#include "woolstorbuycount.h"
#include "woolstorsendwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/MacroDefine.h"
#include "GameData/WooolStoreData.h"

INIT_WND_POSX(CWoolStorBuyCount,POS_AUTO,POS_AUTO)

CWoolStorBuyCount::CWoolStorBuyCount(void)
{
	m_pOK = NULL ;
	m_pCancel = NULL ;
	m_pCountSelect = NULL;
    m_strConfirm.clear();
    m_strMoneyCount.clear();
	m_bModel = true;

	//m_pRDBuyUseBindYuanBoa = NULL;
	m_iIndex = 40;
	
	if (sm_dwWindowType == 1)
		m_bBind = true;
	else
		m_bBind = false;
}

CWoolStorBuyCount::~CWoolStorBuyCount(void)
{
}

void CWoolStorBuyCount::OnDraw(void)
{
    char strTemp[128]="";
	sprintf(strTemp,"您确定要购买%s吗?请选择购买数量,默认为1件",g_WooolStoreMgr.GetBuyData()->m_strItemName.c_str());

	g_pFont->DrawText(m_iScreenX+ 45,m_iScreenY + 32,strTemp,COLOR_TEXT_NORMAL, FONT_YAHEI);
	int iCount = atoi(g_WooolStoreMgr.GetBuyData()->m_strItemCount.c_str());
	int iPrice = atoi(g_WooolStoreMgr.GetBuyData()->m_strItemPrice.c_str());

	if(iCount>=1 && iPrice>=1)
	{
		if(iCount>9 )
			iCount = 9;
		iPrice *= iCount;
	}


	if(m_bBind)
	{
		sprintf(strTemp,"共%d绑定元宝",iPrice);
	}
	else
	{
		sprintf(strTemp,"共%d元宝",iPrice);
	}

	g_pFont->DrawText(m_iScreenX+ 160,m_iScreenY + 70,strTemp,0xffE6C800,FONT_YAHEI,FONTSIZE_MIDDLE);
}

bool CWoolStorBuyCount::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{

		if(pControl == m_pOK)
		{
			SetMode(false);

			int iCount = atoi(g_WooolStoreMgr.GetBuyData()->m_strItemCount.c_str());
			int iPrice = atoi(g_WooolStoreMgr.GetBuyData()->m_strItemPrice.c_str());

			if(iCount>=1 && iPrice>=1)
			{
				if(iCount>9 )
					iCount = 9;
				iPrice *= iCount;
			}


			//如果是虚拟道具
			if( atoi(g_WooolStoreMgr.GetBuyData()->m_strItemReal.c_str())==0)
			{
				if ( iCount > GetLeftGrid())
				{
					g_MsgBoxMgr.PopSimpleAlert(STRING_PACKAGE_FULL);
					CloseWindow();
					return true;
				}
				else if(m_bBind && SELF.GetBindYuanBao() < iPrice)
				{
					g_MsgBoxMgr.PopTagAlert("对不起,您的绑定元宝不足!");
					CloseWindow();
					return true;
				}


				g_pControl->MsgToWnd(MSG_CTRL_WOOOL_STORE_WND,MSG_CTRL_WOOOLSTORE_BUYCONFIRM,atoi(g_WooolStoreMgr.GetBuyData()->m_strItemCount.c_str()),(CControl*)m_bBind);
			}
			//如果是实物道具
			else if( atoi(g_WooolStoreMgr.GetBuyData()->m_strItemReal.c_str()) ==1 )
			{
				if (SELF.GetYuanBao() < iPrice)
				{
					g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
					CloseWindow();
					return true;
				}
			    g_pControl->PopupWindow(MSG_CTRL_WOOOLSTORE_USERINFO_WND,OPER_CREATE);
			}
			CloseWindow();
			return true;
		}
		else if(pControl == m_pCancel)
		{
			SetMode(false);
			CloseWindow();
			return true;
		}
	}
	if(dwMsg == MSG_CTRL_MENU_SELCHANGED)
	{
		g_WooolStoreMgr.GetBuyData()->m_strItemCount = m_pCountSelect->GetText();
		return true;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CWoolStorBuyCount::OnCreate(void)
{
	SetMode(true);
	//确定按钮
	m_pOK = new CCtrlButton();
	if(m_pOK)
	{
		AddControl(m_pOK);
		m_pOK->CreateEx(this,100,128,90,91,92);
		m_pOK->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		m_pOK->SetTips("点击后使用元宝或绑定元宝购买");
	}
	//取消按钮
	m_pCancel = new CCtrlButton();
	if(m_pCancel)
	{
		AddControl(m_pCancel);
		m_pCancel->CreateEx(this, 220,128,90,91,92);
		m_pCancel->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	}

	//m_pRDBuyUseBindYuanBoa = new CCtrlRadio();
	//AddControl(m_pRDBuyUseBindYuanBoa);
	//m_pRDBuyUseBindYuanBoa->Create(this,100,100,125,126,127,128,110,20);
	//m_pRDBuyUseBindYuanBoa->SetText("使用绑定元宝购买",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);


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
		m_pCountSelect->AddString("7");
		m_pCountSelect->AddString("8");
		m_pCountSelect->AddString("9");
		m_pCountSelect->SetCurSel(0);
	}
}


int CWoolStorBuyCount::GetLeftGrid()
{
	int iReturn  = 0;
	for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6; ++i)
	{
		if(SELF.GetPackageGood(i).GetID() == 0)
			++iReturn;
	}
	return iReturn;
}