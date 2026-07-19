#include "replywnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "Global/Interface/StreamInterface.h"
#include "GameData/OtherData.h"
#include "BaseClass/Misc/Internet.h"

INIT_WND_POSX(CReplyWnd,POS_VARIABLE,POS_VARIABLE)

// 回复
CReplyWnd::CReplyWnd()
{
	m_idx = sm_dwWindowType;
	m_iControlMode  = CTRL_MODE_MULIEDIT;

	m_iIndex = 197;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 200;
	m_iOffY = 100;

}

CReplyWnd::~CReplyWnd(void)
{
}

void CReplyWnd::OnCreate()
{

	m_pButtonOK = new CCtrlButton(); 
	AddControl(m_pButtonOK);
	m_pButtonOK->CreateEx(this,197,151,0,6,7);

	m_pButtonCancel = new CCtrlButton(); 
	AddControl(m_pButtonCancel);
	m_pButtonCancel->Create(this,254,151,0,8,9);

	m_pButtonPhrase = new CCtrlButton(); 
	AddControl(m_pButtonPhrase);
	m_pButtonPhrase->Create(this,21,151,0,481,482);

	m_pMultiEdit = new CCtrlMultiEdit(MAX_NOTE_BYTES,44,8);
	AddControl(m_pMultiEdit);
	m_pMultiEdit->Create(this,14,24,36,276,200);
	m_pMultiEdit->SetFocus();
}

bool CReplyWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK :
		if(pControl == m_pButtonPhrase)
		{
			g_pControl->Msg(MSG_CTRL_PHRASE_WND,0);
		}
		else if(pControl == m_pButtonOK)
		{
			if(m_idx >= 0 && m_idx < g_OtherData.GetLeaveWordsVector().size())
			{
				char temp[1024] = {0};
				sprintf(temp,"##%s %s",g_OtherData.GetLeaveWordsVector().at(m_idx).strCharName.c_str(),m_pMultiEdit->GetText());
				g_pGameControl->SEND_Message_Send(temp,strlen(temp));
			}
			g_pControl->Msg(MSG_CTRL_REPLY_WND,0);
		}
		else if(pControl == m_pButtonCancel)
		{
			g_pControl->Msg(MSG_CTRL_REPLY_WND,0);
		}
		else
		{
			break;
		}
		return true;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CReplyWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_idx >= 0 && m_idx < g_OtherData.GetLeaveWordsVector().size())
	{
		char temp[256] = {0};
		sprintf(temp,"您回复 %s 的留言",g_OtherData.GetLeaveWordsVector().at(m_idx).strCharName.c_str());
		g_pFont->DrawText(m_iX + 20,m_iY + 18,temp,0xFFFFFFFF);
	}

}


//////////////////////////////////////////////////////////////////////////////////////
// 转发
INIT_WND_POSX(CForwardWnd,POS_VARIABLE,POS_VARIABLE)

CForwardWnd::CForwardWnd()
{
	m_idx = sm_dwWindowType;
	m_iControlMode  = CTRL_MODE_MULIEDIT;

	m_iIndex = 198;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 250;
	m_iOffY = 100;

}

CForwardWnd::~CForwardWnd()
{
}

void CForwardWnd::OnCreate()
{
	m_pButtonOK = new CCtrlButton(); 
	AddControl(m_pButtonOK);
	m_pButtonOK->Create(this,197,158,0,6,7);

	m_pButtonCancel = new CCtrlButton(); 
	AddControl(m_pButtonCancel);
	m_pButtonCancel->Create(this,254,158,0,8,9);

	m_pButtonPhrase = new CCtrlButton(); 
	AddControl(m_pButtonPhrase);
	m_pButtonPhrase->Create(this,21,158,0,481,482);

	m_pMultiEdit = new CCtrlMultiEdit(MAX_NOTE_BYTES,44,10);
	AddControl(m_pMultiEdit);
	m_pMultiEdit->Create(this,14,24,45,276,200);
	if(m_idx >= 0 && m_idx < g_OtherData.GetLeaveWordsVector().size())
	{
		m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_idx).strMessage.c_str());
	}

	m_pEdit = new CCtrlEdit();
	AddControl(m_pEdit);
	m_pEdit->Create(this,FONTSIZE_DEFAULT,-1,0xFFFFFFFF,128,24,114,20);
	m_pEdit->SetMaxLength(14);
	m_pEdit->SetFocus();
}

bool CForwardWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK :
		if(pControl == m_pButtonPhrase)
		{
			g_pControl->Msg(MSG_CTRL_PHRASE_WND,0);
		}
		else if(pControl == m_pButtonOK)
		{
			char temp[1024] = {0};
			sprintf(temp,"##%s %s",m_pEdit->GetText(),m_pMultiEdit->GetText());
			g_pGameControl->SEND_Message_Send(temp,strlen(temp));
			g_pControl->Msg(MSG_CTRL_FORWARD_WND,0);
		}
		else if(pControl == m_pButtonCancel)
		{
			g_pControl->Msg(MSG_CTRL_FORWARD_WND,0);
		}
		return true;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CForwardWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
// 短语
INIT_WND_POSX(CPhraseWnd,POS_VARIABLE,POS_VARIABLE)

CPhraseWnd::CPhraseWnd()
{
	m_iType = 0;
	m_idx = -1;

	m_iCx = 32;
	m_iCy = 40;

	g_OtherData.GetNoteBody().clear();
	g_OtherData.GetNoteTitle().clear();

	string strUrl = g_pStreamMgr->GetWebsite("Note","");
	g_HttpMgr.InternetGet(strUrl.c_str(),7);
	SELF.SetReserveData(plyPhrase,1);

	m_iIndex = 199;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 250;
	m_iOffY = 100;

}

CPhraseWnd::~CPhraseWnd()
{
}

void CPhraseWnd::OnCreate()
{
	SetCloseButton(290,14);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,288,67,22,134,483);


	m_pMultiEdit = new CCtrlMultiEdit(12288,42,10);
	AddControl(m_pMultiEdit);
	m_pMultiEdit->Create(this,14,22,66,260,130);
	m_pMultiEdit->SetEditable(false);
}

bool CPhraseWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{

	switch(dwMsg)
	{
	case MSG_INPUT_WHEEL:
		{
			m_pScroll->Msg(dwMsg,dwData);
			return true;
		}
	case MSG_CTRL_NOTE_MSG:
		{
			if(g_OtherData.GetNoteTitle().size() > 0)
			{
				std::string temp;

				for(int i = g_OtherData.GetNoteTitle()[m_iType].iBegin;i <= g_OtherData.GetNoteTitle()[m_iType].iEnd; i++)
				{
					char sss[32] = {0};
					sprintf(sss,"#%3d",i);
					if(sss[1] == 32)
						sss[1] = '0';
					if(sss[2] == 32)
						sss[2] = '0';
					temp += sss;
					temp += "  ";
					temp += g_OtherData.GetNoteBody()[sss].c_str();
					sss[0] = 13;
					sss[1] = 10;
					sss[2] = 0;
					temp += sss;
					temp += sss;
				}
				m_pMultiEdit->SetText(temp.c_str());
				m_pScroll->SetStep(1);
				m_pScroll->SetRange(m_pMultiEdit->GetLines());
				m_pScroll->SetPos(0);
			}
			return true;
		}
	case MSG_INPUT_LEFTBT_DCLICK:
		{
			if(pControl == m_pMultiEdit)
			{
				int iY = (int)dwData;
				if(iY < 4)
					iY = 4;
				for(int i = m_pScroll->GetPos() + iY / 12; i >= 0; i--)
				{
					char sss[512] = {0};
					m_pMultiEdit->GetLineText(sss,i);
					if(sss[0] == '#')
					{
						char str[4] = {0};
						memcpy(str,sss+1,3);
						g_pControl->Msg(MSG_CTRL_NOTE_CHANGED,atoi(str));
						break;
					}
				}
				return true;
			}
			break;
		}
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CPhraseWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(SELF.GetReserveData(plyPhrase))			// 正在读取或者读取失败
	{
		m_pMultiEdit->SetText(g_OtherData.GetNoteBody()["#000"].c_str());
	}
	else															// 读取成功，显示title
	{
		m_pMultiEdit->SetDisLine(m_pScroll->GetPos());
		// title
		DWORD dwColor;
		for(size_t i = 0; i < g_OtherData.GetNoteTitle().size(); i++)
		{
			if(i == m_iType)
				dwColor = 0xFFFF0000;
			else
				dwColor = 0xFFFFFF00;
			g_pFont->DrawText(m_iX + m_iCx + i * 72,m_iY + m_iCy,g_OtherData.GetNoteTitle()[i].sTitle.c_str(),dwColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_UnderLine);
		}
	}

}
bool CPhraseWnd::OnLeftButtonDown(int iX, int iY)
{
	if(iY >= m_iCy && iY < m_iCy + 14 && iX >= m_iCx && g_OtherData.GetNoteTitle().size() > 0)
	{
		for(size_t i = 0; i < g_OtherData.GetNoteTitle().size(); i++)
		{
			if(iX >= (m_iCx + i * 72) && iX < (m_iCx + i * 72 + strlen(g_OtherData.GetNoteTitle()[i].sTitle.c_str()) * 6))
			{
				m_iType = i;
				m_idx = g_OtherData.GetNoteTitle()[i].iBegin;
				Msg(MSG_CTRL_NOTE_MSG,0,NULL);
				return true;
			}
		}
	}
	return CCtrlWindowX::OnLeftButtonDown(iX, iY);
}

void CReplyWnd::SetEditText(const char * sText)
{
	m_pMultiEdit->SetText(sText);
}

void CForwardWnd::SetEditText(const char * sText)
{
	m_pMultiEdit->SetText(sText);
}
