#include "autoleavewordswnd.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"

INIT_WND_POSX(CAutoLeaveWordsWnd,POS_VARIABLE,POS_VARIABLE)


CAutoLeaveWordsWnd::CAutoLeaveWordsWnd(void)
{
	m_bModel	= false;	
	//m_pAutoLeaveWords = NULL;
	m_pOk = NULL;
	m_pCancel = NULL;
	
	m_iIndex = 155;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 0;

	if(g_pGfx->GetWidth() > 800)
		m_iOffY = 262;
	else
		m_iOffY = 146;
}

CAutoLeaveWordsWnd::~CAutoLeaveWordsWnd(void)
{
}

void CAutoLeaveWordsWnd::OnCreate(void)
{
	SetCloseButton(307,20);

	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,74,162,14503,14504,14505);

	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,198,162,14515,14516,14517);	

	//m_pAutoLeaveWords = new CCtrlRadio();
	//AddControl(m_pAutoLeaveWords);
	//m_pAutoLeaveWords->Create(this,32,175,0,175);
}


bool CAutoLeaveWordsWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pOk)
		{ 	
			std::string temp = "";
			temp = temp + "##" + g_OtherData.GetOffLinePrivate().strCharName + " ";
			temp = temp + g_OtherData.GetOffLinePrivate().strMessage;
			StringUtil::AutoCut(temp,MAX_NOTE_BYTES);		
			g_pGameControl->SEND_Message_Send(temp.c_str(),temp.length());
			
			g_pControl->PopupWindow(MSG_CTRL_AUTOLEAVEWORD_WND,OPER_CLOSE);
			return true;
		}
		else if(pControl == m_pCancel)
		{
			g_pControl->PopupWindow(MSG_CTRL_AUTOLEAVEWORD_WND,OPER_CLOSE);
			return true;
		}
		//else if(pControl == m_pAutoLeaveWords)
		//{
		//	g_OtherData.SetAutoLeaveWord(m_pAutoLeaveWords->IsChecked());			
		//	return true;
		//}
		break;
	default:
		break;
	}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CAutoLeaveWordsWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	string temp3;
	temp3 = g_OtherData.GetOffLinePrivate().strMessage;

	g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 57,"是否将“",-1);
	if(temp3.length() > 42)
	{
		StringUtil::AutoCut(temp3,MAX_NOTE_BYTES);
		string temp4 = temp3;
		StringUtil::AutoCut(temp4,42);
		g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 73,temp4.c_str(),0xFFFFFFFF);
		g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 89,temp3.c_str() + temp4.length(),0xFFFFFFFF);

		g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 105,"”留言？",0xFFFFFFFF);
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 73,temp3.c_str(),0xFFFFFFFF);
		g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 89,"”留言？",0xFFFFFFFF);
	}

}