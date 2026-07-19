#include "ExLeavWordsWnd.h"
#include "GameData/NpcData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CExLeavWordsWnd,POS_AUTO,POS_AUTO)

CExLeavWordsWnd::CExLeavWordsWnd(void)
{
	m_type = sm_dwWindowType;
	m_pOK = NULL ;
	m_pCancel = NULL ;	
	m_pInfo = NULL;

	m_iPages = 1;
	
    
	if(m_type == 1)
	    m_iIndex = 1004;
	else if(m_type == 2)
		m_iIndex = 1002;
}

CExLeavWordsWnd::~CExLeavWordsWnd(void)
{
}

void CExLeavWordsWnd::OnCreate()
{
    if(m_type == 1)
	{

		//确定按钮
		m_pOK = new CCtrlButton();
		if(m_pOK)
		{
			AddControl(m_pOK);
			m_pOK->CreateEx(this,75, 110,90,91,92);
			m_pOK->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		}
		//取消按钮
		m_pCancel = new CCtrlButton();
		if(m_pCancel)
		{
			AddControl(m_pCancel);
			m_pCancel->CreateEx(this,200, 110,90,91,92);
			m_pCancel->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

		}
		m_pInfo = new CCtrlMultiEdit(52,40,3,true);
		AddControl(m_pInfo);
		m_pInfo->Create(this,14,45,53,253,40);
		m_pInfo->SetText("");
		m_pInfo->SetFont(FONT_YAHEI);
		m_pInfo->SetColor(COLOR_TEXT_NORMAL);
	}
	else
	{
		SetMask(1002);

		//确定按钮
		m_pOK = new CCtrlButton();
		if(m_pOK)
		{
			AddControl(m_pOK);
			m_pOK->CreateEx(this,95,141,4843,4844,4845);
		}
		//取消按钮
		m_pCancel = new CCtrlButton();
		if(m_pCancel)
		{
			AddControl(m_pCancel);
			m_pCancel->CreateEx(this, 225,141,4846,4847,4848);
		}
		m_pInfo = new CCtrlMultiEdit(52,40,3,true);
		AddControl(m_pInfo);
		m_pInfo->Create(this,14,62,83,246,45);
		m_pInfo->SetText("");
	}
}

void CExLeavWordsWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_type == 1)
	    g_pFont->DrawText(m_iScreenX + 30,m_iScreenY + 28,"请在下框中输入留言（留言字数请控制在26字以内）：",0xFFFFFF00,FONT_YAHEI);
	else
		g_pFont->DrawText(m_iScreenX + 50,m_iScreenY + 60,"请在下框中输入留言（留言字数请控制在26字以内）：",0xFFFFFF00);
}

bool CExLeavWordsWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pOK)
		{
			//验证有效性
			//提交消息到gs
			//关闭当前窗口
			if(	strlen(m_pInfo->GetText())>52)
			{
				g_MsgBoxMgr.PopSimpleAlert("留言长度超过上限。");
				return true;
		    }
			
			g_pGameControl->SEND_LeavWords(g_NPC.GetID(),m_pInfo->GetText(),m_type);
			g_pControl->PopupWindow(MSG_CTRL_EX_LEAVWORDS_WND, OPER_CLOSE);
			return true;
		}
		else if(pControl == m_pCancel)
		{
			//关闭当前窗口
			g_pControl->PopupWindow(MSG_CTRL_EX_LEAVWORDS_WND, OPER_CLOSE);
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}