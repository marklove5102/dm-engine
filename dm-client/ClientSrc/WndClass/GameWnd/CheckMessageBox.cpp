#include "CheckMessageBox.h"
#include "GameData/GameData.h"
#include "Global/Interface/StreamInterface.h"
#include "GameData/WooolStoreData.h"
#include "GameData/ConfigData.h"

INIT_WND_POSX(CCheckMessageBox,POS_AUTO,POS_AUTO)

CCheckMessageBox::CCheckMessageBox()
{
	m_pCheckOn = NULL;
	m_bModel = true;

	m_iGoodIndex  = sm_dwWindowType;
    m_iIndex = 40;
	m_iAlignType = XAL_TOPLEFT;
}

CCheckMessageBox::~CCheckMessageBox(void)
{
}

void CCheckMessageBox::OnCreate()
{
	//CCtrlMessageBox::OnCreate();
	//创建按钮
	m_pCheckOn = new CCtrlRadio();
	AddControl(m_pCheckOn);
	m_pCheckOn->Create(this,30,76,125,126,127,128);
	m_pCheckOn->SetChecked(!g_Config.GetCheckQuick());
	//确定按钮
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,90,129,90,91,92);
	m_pOKButton->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//取消按钮
	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,225,129,90,91,92);
	m_pCancelButton->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
}

void CCheckMessageBox::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX+ 70 , m_iScreenY+50 ,"从商城中购买此道具放入您的包裹,您确定吗?",COLOR_TEXT_NORMAL,FONT_YAHEI);
	g_pFont->DrawText(m_iScreenX+ 50 , m_iScreenY+80 ,"以后购买时不再确认",0xFFFFFF00,FONT_YAHEI);
	g_pFont->DrawText(m_iScreenX+ 160 , m_iScreenY+80 ,"（可在辅助工具里重新开启此确认框）",COLOR_TEXT_NORMAL,FONT_YAHEI);

	m_pCheckOn->SetChecked(!g_Config.GetCheckQuick());
}

bool CCheckMessageBox::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pCheckOn)
		{
			g_Config.SetCheckQuick(!m_pCheckOn->IsChecked());
			g_pStreamMgr->SetConfigStr("CheckQuick",m_pCheckOn->IsChecked()?"No":"Yes");
			return true;
		}
		else if(pControl == m_pOKButton)
		{   //快速购买
			g_Config.SetCheckQuick(!m_pCheckOn->IsChecked());
			g_pStreamMgr->SetConfigStr("CheckQuick",m_pCheckOn->IsChecked()?"No":"Yes");

			//这个窗口双击只购买 不使用物品
			if(m_iGoodIndex == 100)
			{
				CQuickItem &quickItem = g_WooolStoreMgr.GetTmpQuickItem();
				if(quickItem.strItemID.length() > 0)
				{
					g_WooolStoreMgr.BuyQuickItem(quickItem,false);
					quickItem.clear();
				}
			}
			else
			{
				g_WooolStoreMgr.UseQuickItem(m_iGoodIndex , false);
			}
			CloseWindow();
			return true;
		}
		else if(pControl == m_pCancelButton)
		{
			if(m_iGoodIndex == 100)
			{
				g_WooolStoreMgr.GetTmpQuickItem().clear();
			}
			//取消快速购买
			CloseWindow();
			return true;
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}