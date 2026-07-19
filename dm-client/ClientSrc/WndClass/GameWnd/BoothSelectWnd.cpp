#include "BoothSelectWnd.h"
#include "GameData/GameData.h"
#include "GameData/BoothData.h"

INIT_WND_POSX(CBoothSelectWnd,POS_AUTO,POS_AUTO)

//开始摆摊的时候 选择是豹子摆摊 还是玩家摆摊
CBoothSelectWnd::CBoothSelectWnd(void)
{
	m_pPlayerBoothButton = NULL;
	//m_pUploadButton = NULL;

	m_iIndex = 103;
	m_iPages = 1;
}

CBoothSelectWnd::~CBoothSelectWnd(void)
{

}

void CBoothSelectWnd::OnCreate()
{
	//玩家摆摊按钮
	m_pPlayerBoothButton = new CCtrlButton();
	AddControl(m_pPlayerBoothButton);
	m_pPlayerBoothButton->CreateEx(this, 64,127, 95, 96, 97, 98);
	m_pPlayerBoothButton->SetText("亲自摆摊", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	//m_pUploadButton = new CCtrlButton();
	//AddControl(m_pUploadButton);
	//m_pUploadButton->CreateEx(this, 256,127, 95, 96, 97, 98);
	//m_pUploadButton->SetText("拍卖行", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);


	SetCloseButton(377,10);
}

void CBoothSelectWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//g_pFont->DrawText(m_iScreenX+49,m_iScreenY+46,"温馨提示：您还可以将物品提交到拍卖行进行无限时出售。",0xFFFFFFFF);
}

bool CBoothSelectWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pPlayerBoothButton)
		{
			CloseWindow();
			g_pControl->PopupWindow(MSG_CTRL_BOOTH_WND,OPER_CREATE);
			return true;
		}
		//else if(pControl == m_pUploadButton)
		//{
		//	g_pControl->PopupWindow(MSG_CTRL_SELECT_BOOTH_CONFIRM,OPER_CLOSE);
		//	g_pControl->PopupWindow(MSG_CTRL_UPLOAD_WND,OPER_CREATE);
		//	return true;
		//}
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}