#include "ReceiveMsgWnd.h"

#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "GameClient/SDOAInterface.h"


INIT_WND_POSX(CReceiveMsgWnd,POS_VARIABLE,POS_VARIABLE)

CReceiveMsgWnd::CReceiveMsgWnd()
{
	m_nCurLeaveWord = -1;

	m_iIndex = 15945;
	m_iPages = 1;

	m_iAlignType = XAL_CENTER;
	m_iOffX	= -74;
	m_iOffY	= -44;

	//m_iVersion = 2;
	//AddTabPage(0,0,MAKELONG(15945,PACKAGE_INTERFACE),35,36,120,121,122,123,"接收消息",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	//AddTabPage(0,0,MAKELONG(15945,PACKAGE_INTERFACE),107,36,120,121,122,123,"网络电台",NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);

}

CReceiveMsgWnd::~CReceiveMsgWnd(void)
{
	if(m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size() && m_nCurLeaveWord >= 0)
		g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded = true;
}

void CReceiveMsgWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	SetCloseButton(405,4,80);

	std::vector<_LeaveWords>& VLeaveWords = g_OtherData.GetLeaveWordsVector();

	//_LeaveWords a;
	//a.bIsReaded = false;
	//a.strCharName = "何国辉何国辉何";
	//a.strDate = "2009-10-10";
	//a.strMessage = "dfffffffffffffff";
	//a.strTime = "12:33:23";
	//VLeaveWords.push_back(a);


	m_pMultiEdit = new CCtrlMultiEdit(2800,56,50,false);
	AddControl(m_pMultiEdit);
	m_pMultiEdit->Create(this,FONTSIZE_SMALL,30,69,376,208);
	m_pMultiEdit->AddScrollEx(361,0,18,202,true,1);
	m_pMultiEdit->SetDisplayRows(10);

	if(VLeaveWords.size() > 0)
		m_nCurLeaveWord = 0;
	else
		m_nCurLeaveWord = -1;

	if(m_nCurLeaveWord == 0)
	{
		for(;m_nCurLeaveWord < VLeaveWords.size() - 1;m_nCurLeaveWord++)
		{
			if(!VLeaveWords.at(m_nCurLeaveWord).bIsReaded)
				break;
		}

		if(m_nCurLeaveWord == VLeaveWords.size() - 1 && VLeaveWords.at(m_nCurLeaveWord).bIsReaded)
			m_nCurLeaveWord = 0;

		m_pMultiEdit->SetText(VLeaveWords.at(m_nCurLeaveWord).strMessage.c_str());
	}

	m_pPre = new CCtrlButton();
	m_pPre->CreateEx(this,37,286,15621,15622,15623,15624);
	AddControl(m_pPre);
	m_pPre->CreateEx(this, 37,286, 95, 96, 97, 98);
	m_pPre->SetText("上一条", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pNext = new CCtrlButton();
	AddControl(m_pNext);
	m_pNext->CreateEx(this, 348,286, 95, 96, 97, 98);
	m_pNext->SetText("下一条", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pDelete = new CCtrlButton();
	AddControl(m_pDelete);
	m_pDelete->CreateEx(this, 27,317, 90, 91, 92, 93);
	m_pDelete->SetText("删除", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,FONT_YAHEI);

	m_pClearAll = new CCtrlButton();
	AddControl(m_pClearAll);
	m_pClearAll->CreateEx(this, 337,317, 90, 91, 92, 93);
	m_pClearAll->SetText("清空", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,FONT_YAHEI);
}

void CReceiveMsgWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 219,m_iScreenY + 10,"消  息",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
	std::vector<_LeaveWords>& VLeaveWords = g_OtherData.GetLeaveWordsVector();
	if(m_nCurLeaveWord >= 0 && m_nCurLeaveWord < VLeaveWords.size())
	{

		char temp[256] = {0};	
		// 是否以读标示
		if(VLeaveWords.at(m_nCurLeaveWord).bIsReaded )
			strcpy(temp,"已读");
		else
			strcpy(temp,"未读");
		g_pFont->DrawText(m_iScreenX + 187,m_iScreenY + 289,temp,COLOR_TEXT_NORMAL,FONT_YAHEI);
		// 当前位置
		sprintf(temp,"%d/%d",m_nCurLeaveWord+1,VLeaveWords.size());
		g_pFont->DrawText(m_iScreenX + 213,m_iScreenY + 289,temp,COLOR_TEXT_NORMAL,FONT_YAHEI);




		// 留言时间
		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 40,"时间:",COLOR_TEXT_NORMAL,FONT_YAHEI);
		sprintf(temp,"%s %s",VLeaveWords.at(m_nCurLeaveWord).strDate.c_str(),VLeaveWords.at(m_nCurLeaveWord).strTime.c_str());
		g_pFont->DrawText(m_iScreenX + 115,m_iScreenY + 40,temp,COLOR_TEXT_NORMAL,FONT_YAHEI);
		// 发送人
		g_pFont->DrawText(m_iScreenX + 235,m_iScreenY + 40,"发送人:",COLOR_TEXT_NORMAL,FONT_YAHEI);
		strcpy(temp, g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strCharName.c_str());
		g_pFont->DrawText(m_iScreenX + 280,m_iScreenY + 40,temp,COLOR_TEXT_NORMAL,FONT_YAHEI);

		m_pDelete->SetEnable(true);
		m_pClearAll->SetEnable(true);

		if(m_nCurLeaveWord > 0)
			m_pPre->SetEnable(true);
		else
			m_pPre->SetEnable(false);

		if(m_nCurLeaveWord < VLeaveWords.size() - 1)
			m_pNext->SetEnable(true);
		else
			m_pNext->SetEnable(false);
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 324,m_iScreenY + 37,"无留言 0/0",0xFFFFFFFF,FONT_YAHEI);
		m_pPre->SetEnable(false);
		m_pNext->SetEnable(false);
		m_pDelete->SetEnable(false);
		m_pClearAll->SetEnable(false);
	}
}

bool CReceiveMsgWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
#ifdef _IGW
			if(g_pSDOAInterface && g_pSDOAInterface->OpenWidgetA("Radio") == IGW_OK)
			{
				int i = 3; 
			}
			else
			{
				//g_MsgBoxMgr.PopSimpleAlert("无法打开电台");
				g_pControl->PopupWindow(MSG_CTRL_RADIO_WND,OPER_CREATE);
			}
#endif

			CloseWindow();

			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK :
		if(pControl == m_pPre)
		{
			if( m_nCurLeaveWord > 0 )
			{
				g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded = true;
				m_nCurLeaveWord--;
				m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMessage.c_str());
			}
			return true;
		}
		else if(pControl == m_pNext)
		{		
			if(m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size() - 1)
			{
				g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded = true;
				m_nCurLeaveWord++;	
				m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMessage.c_str());
			}
			return true;
		}
		else if(pControl == m_pClearAll)
		{
			if(g_OtherData.GetLeaveWordsVector().size() > 0)
			{
				g_MsgBoxMgr.PopSimpleComfirm("确定清空所有留言么？",MSG_CTRL_CLEARALL_LEAVEWORD,0);
			}
			return true;
		}
		else if(pControl == m_pDelete)
		{
			if(g_OtherData.GetLeaveWordsVector().size() > 0)
			{
				if(m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size() && m_nCurLeaveWord >= 0)
				{
					std::vector<_LeaveWords>::iterator pos;
					pos = find( g_OtherData.GetLeaveWordsVector().begin(),
						g_OtherData.GetLeaveWordsVector().end(),
						g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord));

					g_OtherData.GetLeaveWordsVector().erase(pos);
				}
				if(m_nCurLeaveWord == g_OtherData.GetLeaveWordsVector().size())
					m_nCurLeaveWord--;

				if( m_nCurLeaveWord >= 0 )
				{
					m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMessage.c_str());
				}
				else
					m_pMultiEdit->SetText("   ");
			}
			return true;
		}
		break;

	case MSG_CTRL_RECEIVE_MSG_WND:
		if(dwData == 1)
		{
			g_OtherData.GetLeaveWordsVector().clear();
			SetCurLeaveWord(-1);
			return true;
		}
		break;

	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CReceiveMsgWnd::SetCurLeaveWord(int nCurLeaveWord)
{
	m_nCurLeaveWord = nCurLeaveWord;
	if(nCurLeaveWord == -1)
	{
		m_pMultiEdit->SetText(" ");
	}
}