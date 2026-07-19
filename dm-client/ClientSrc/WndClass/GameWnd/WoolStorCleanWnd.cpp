#include "WoolStorCleanWnd.h"
#include "Global\GlobalMsg.h"
#include "GameControl/GameControl.h"
#include "GameData/WooolStoreData.h"

INIT_WND_POSX(CWoolStorCleanWnd,POS_AUTO,POS_AUTO)

CWoolStorCleanWnd::CWoolStorCleanWnd()
{
	m_pOK = NULL ;
	m_iIndex = 10421;
}

CWoolStorCleanWnd::~CWoolStorCleanWnd(void)
{
}

void CWoolStorCleanWnd::OnDraw(void)
{
	for(int j=0;j<10;j++)
	{
		if(!g_WooolStoreMgr.GetRecvData(j)->m_strItemID.empty())
		{
			m_strItemName = g_WooolStoreMgr.GetRecvData(j)->m_strItemName;
			m_strInform   = "有人送您 ";
			m_strInform  += m_strItemName;
			m_strInform  += " 作为礼物";
			g_pFont->DrawText(m_iScreenX+ 45,m_iScreenY + 32,m_strInform.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX+ 45,m_iScreenY + 48,"请您在包裹中留出足够空位后重新",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX+ 45,m_iScreenY + 64,"进入商城尝试领取",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL,DTF_BlackFrame);
			return;
		}

	}
}

bool CWoolStorCleanWnd::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		SetMode(false);
		if(pControl == m_pOK)
		{
			//这里弹出包裹窗口	
			g_pControl->Msg(MSG_CTRL_PACKAGEWND,OPER_CREATE);//打开包裹窗口
			CloseWindow();//关闭当前窗口
			return true;
		}

	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CWoolStorCleanWnd::OnCreate(void)
{

	SetMode(true);
	//确定按钮
	m_pOK = new CCtrlButton();
	if(m_pOK)
	{
		AddControl(m_pOK);
		m_pOK->CreateEx(this,65,100,21,22,23);
		m_pOK->SetText("我去清理包裹");
	}

}