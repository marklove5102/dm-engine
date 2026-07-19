#include "YiHuoZhanMsgWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"
#include "Global/GlobalMsg.h"

INIT_WND_POSX(CYiHuoZhanMsgWnd,POS_VARIABLE,POS_VARIABLE)

CYiHuoZhanMsgWnd::CYiHuoZhanMsgWnd(void)
{
	m_iIndex = 23028;
	m_iAlignType = XAL_LEFT;
}

CYiHuoZhanMsgWnd::~CYiHuoZhanMsgWnd(void)
{
}

void CYiHuoZhanMsgWnd::OnCreate()
{
	m_pSubmit = new CCtrlButton();
	m_pSubmit->CreateEx(this,44,105,95,96,97,98);
	m_pSubmit->SetText("");
	AddControl(m_pSubmit);

	m_pClose = new CCtrlButton();
	m_pClose->CreateEx(this,133,105,95,96,97,98);
	m_pClose->SetText("¹Ø ±Õ");
	AddControl(m_pClose);

	m_pMarkViewer = new CMarkViewer(16,4);
	m_pMarkViewer->Create(this,14,10,210,90);
	AddControl(m_pMarkViewer);	
	m_pMarkViewer->SetTagText(&g_TagTextMgr.GetYiHuoZhanMsgText());
	m_pMarkViewer->SetFont(FONT_YAHEI,FONTSIZE_MIDDLE);
}

void CYiHuoZhanMsgWnd::Draw()
{
	CCtrlWindowX::Draw();

	S_YiHuoZhanMsg& yihuomsg = g_OtherData.GetYiHuoZhanMsg();
	if (GetTickCount() - yihuomsg.iStartTime > yihuomsg.iShowTime)
	{
		CloseWindow();
		return;
	}	
}

bool CYiHuoZhanMsgWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_YIHUOZHANMSG_WND:
		{
			if (dwData == 10)
			{
				S_YiHuoZhanMsg& yihuomsg = g_OtherData.GetYiHuoZhanMsg();

				m_pSubmit->SetText(yihuomsg.szBtn1);
				m_pClose->SetText(yihuomsg.szBtn2);
				g_TagTextMgr.GetYiHuoZhanMsgText().Clear();
				g_TagTextMgr.GetYiHuoZhanMsgText().Parse(yihuomsg.strMsg,0,"\\");
				return true;
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pSubmit)
			{				
				g_pGameControl->SEND_YiHuoZhanMsg(0);
				OnClickCloseButton();
				return true;
			}
			else if (pControl == m_pClose)
			{
				OnClickCloseButton();
				return true;
			}
		}
		break;	
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}