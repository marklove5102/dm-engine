

#include "woolstorsendokwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/WooolStoreData.h"

INIT_WND_POSX(CWoolStorSendOKWnd,POS_AUTO,POS_AUTO)

CWoolStorSendOKWnd::CWoolStorSendOKWnd(void)
{
	m_pOK = NULL ;
	m_pCancel = NULL ;
	m_iIndex = 10421;
}

CWoolStorSendOKWnd::~CWoolStorSendOKWnd(void)
{
}

void CWoolStorSendOKWnd::OnDraw(void)
{
	m_strPrice = g_WooolStoreMgr.GetSendData()->m_strItemPrice;
	m_strTerminalUser = g_WooolStoreMgr.GetSendData()->m_strTerminalUser;

	g_pFont->DrawText(m_iScreenX+ 88,m_iScreenY + 36,"您确定要把这份道具送给",0xffE6C800,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 88,m_iScreenY + 56,m_strTerminalUser.c_str(),-1,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX+ 88 + m_strTerminalUser.length() * 8,m_iScreenY + 56,"作礼物吗?",0xffE6C800,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	g_pFont->DrawText(m_iScreenX + 23,m_iScreenY + 85,m_strPrice.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + 45,m_iScreenY + 85,"元宝",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20903,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 20, m_iScreenY + 25, pTex);

	LPTexture pItemTex = g_pTexMgr->GetTexImm( PACKAGE_ptitems,atoi(g_WooolStoreMgr.GetSendData()->m_strItemLooks.c_str()),EP_UI );
	g_pGfx->DrawTextureNL(m_iScreenX + 22, m_iScreenY + 30, pItemTex); 
}

bool CWoolStorSendOKWnd::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pOK)//确定赠送
		{
			SetMode(false);
			//send message here 
			m_strSend = g_WooolStoreMgr.GetSendData()->m_strItemID;
			m_strSend.append(1,'\0');
			m_strSend += g_WooolStoreMgr.GetSendData()->m_strTerminalUser;
			m_strSend.append(1,'\0');
			m_strSend += g_WooolStoreMgr.GetSendData()->m_strSendWords;
			m_strSend.append(1,'\0');
			//char temp[256]="";
			//sprintf(temp,"%s,%d",m_strSend.c_str(),m_strSend.size());
			g_pGameControl->SEND_PT_Store_Info(7, m_strSend.c_str(), m_strSend.size());//向服务端发送消息确定赠送
			g_WooolStoreMgr.GetSendData()->clear();
			CloseWindow();//关闭当前窗口  
			
			return true;
		}
		else if(pControl == m_pCancel)
		{
			SetMode(false);

			g_WooolStoreMgr.GetSendData()->clear();
			CloseWindow();//关闭当前窗口
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CWoolStorSendOKWnd::OnCreate(void)
{

	SetMode(true);
	//确定按钮
	m_pOK = new CCtrlButton();
	AddControl(m_pOK);
	m_pOK->CreateEx(this,30,115,165,166,167);
	m_pOK->SetText("确  定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_MIDDLE);
	
	//取消按钮
	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,135,115,165,166,167);
	m_pCancel->SetText("取  消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_MIDDLE);
	
}

