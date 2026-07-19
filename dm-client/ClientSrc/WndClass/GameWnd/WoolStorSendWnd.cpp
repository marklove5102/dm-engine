
#include "woolstorsendwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/WooolStoreData.h"

INIT_WND_POSX(CWoolStorSendWnd,POS_AUTO,POS_AUTO)

CWoolStorSendWnd::CWoolStorSendWnd(void)
{
	m_pOK = NULL ;
	m_pCancel = NULL ;
	m_pEditRecvUser = NULL ;
	m_pSendText = NULL;

	m_iIndex = 10420;
}

CWoolStorSendWnd::~CWoolStorSendWnd(void)
{
}

bool CWoolStorSendWnd::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	if(dwMsg == MSG_CTRL_WOOOLSTORE_SEND_FOUCUS)
	{
		m_pEditRecvUser->SetFocus();
		return true;
	}
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{

		if(pControl == m_pOK)
		{
			SetMode(false);

		    g_WooolStoreMgr.GetSendData()->m_strTerminalUser= m_pEditRecvUser->GetText();
			if( g_WooolStoreMgr.GetSendData()->m_strTerminalUser.length()>=4)
			{
				g_WooolStoreMgr.GetSendData()->m_strSendWords = m_pSendText->GetText();
				g_pControl->Msg(MSG_CTRL_WOOOLSTORE_SEND_OK_WND,OPER_CREATE);//弹出赠送的确定框
				CloseWindow();//关闭当前窗口
			}
			else
			{
				SetMode(false);

				g_MsgBoxMgr.PopSimpleAlert("该用户不存在，请注意赠送对象名字的有效性！");
				CloseWindow();//关闭当前窗口
				return true;
			}
			return true;
		}
		else if(pControl == m_pCancel)
		{
			SetMode(false);
			CloseWindow();//关闭当前窗口
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CWoolStorSendWnd::OnCreate(void)
{
	SetMode(true);
	//确定按钮
	m_pOK = new CCtrlButton();
	AddControl(m_pOK);
	m_pOK->CreateEx(this,30,135,90,91,92);
	m_pOK->SetText("确  定");

	//取消按钮
	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,125,135,90,91,92);
	m_pCancel->SetText("取  消");

	//接收人编辑框
	m_pEditRecvUser = new CCtrlEdit();
	AddControl(m_pEditRecvUser);
	m_pEditRecvUser->Create(this,FONTSIZE_MIDDLE,0xFFFFFFFF,21,38,117,14);
	m_pEditRecvUser->SetFont(FONT_YAHEI,FONTSIZE_DEFAULT);
	m_pEditRecvUser->SetMaxLength(14);

	// 详细说明框
	m_pSendText = new CCtrlMultiEdit(52,26,2,true,14);
	AddControl(m_pSendText);
	m_pSendText->Create(this,FONTSIZE_MIDDLE,19,82,196,40);
	m_pSendText->SetFont(FONT_YAHEI,FONTSIZE_DEFAULT);

	SetCloseButton(207,1,80);
}

void CWoolStorSendWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 18,m_iScreenY + 18,"收礼人",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + 18,m_iScreenY + 61,"留言框",0xffffff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
}
