#include "msgboxmgr.h"
#include "baseclass/control/ctrlmsgbox.h"
#include <algorithm>
#include "GameData.h"

DTI_IMPLEMENT(CMsgBoxMgr, CCtrlBaseTag)
CMsgBoxMgr::CMsgBoxMgr(void)
:m_pMsgBoxInstance(NULL)
{
	Clear();
}

CMsgBoxMgr::~CMsgBoxMgr(void)
{
}

void CMsgBoxMgr::Clear()
{
	m_vMsgBuf.clear();
	SetInstance(NULL);
	ClearAllExistMsgBox();
	m_vMsgBoxList.clear();
}

void CMsgBoxMgr::PushMsg(const MsgBoxInfo& msg)
{
	m_vMsgBuf.push_back(msg);
	m_vMsgBuf.back().m_dwStartTick = GetTickCount();
	g_pControl->Msg(MSG_POPUP_MSG_BOX, OPER_CREATE,(CControl*)msg.m_stcOkMsg.dwMsg);
}

bool CMsgBoxMgr::PopMsg(MsgBoxInfo& rsvMsg)
{
	if(m_vMsgBuf.empty()) return false;
	rsvMsg = m_vMsgBuf.front();
	m_vMsgBuf.pop_front();
	return true;
}

void CMsgBoxMgr::AddMsgBoxTrack(CCtrlMsgBox* p)
{
	if(find(m_vMsgBoxList.begin(), m_vMsgBoxList.end(), p) == m_vMsgBoxList.end())
	{
		m_vMsgBoxList.push_front(p);
	}
}

void CMsgBoxMgr::RemoveMsgBoxTrack(CCtrlMsgBox* p)
{
	MSGBOXLIST::iterator itor = find(m_vMsgBoxList.begin(), m_vMsgBoxList.end(), p);
	if(itor != m_vMsgBoxList.end())
	{
		m_vMsgBoxList.erase(itor);
	}
}

void CMsgBoxMgr::ClearAllExistMsgBox()
{
	MSGBOXLIST tempList;
	for(MSGBOXLIST::iterator itor = m_vMsgBoxList.begin(); itor != m_vMsgBoxList.end(); ++itor)
	{
		tempList.push_back(*itor);
	}
	for(MSGBOXLIST::iterator itor = tempList.begin(); itor != tempList.end(); ++itor)
	{
		CCtrlMsgBox* p = (*itor);
		g_pControl->Msg(MSG_POPUP_MSG_BOX, OPER_CLOSE, p);
	}
}

bool CMsgBoxMgr::ClearExistMsgBox(const string &strMainString)
{
	for(MSGBOXLIST::iterator itor = m_vMsgBoxList.begin(); itor != m_vMsgBoxList.end(); ++itor)
	{
		CCtrlMsgBox* p = (*itor);
		if(p && p->GetInfo().m_strMainString == strMainString)
		{
			g_pControl->Msg(MSG_POPUP_MSG_BOX, OPER_CLOSE, p);
			return true;
		}
	}

	return false;
}

CCtrlMsgBox* CMsgBoxMgr::GetLastMsgBox()
{
	//if(!m_vMsgBoxList.empty())
	//{
	//	return m_vMsgBoxList.front();
	//}
	MSGBOXLIST::iterator itor = m_vMsgBoxList.begin();
	if(itor != m_vMsgBoxList.end())
	{
		itor++;
		if(itor != m_vMsgBoxList.end())
		{
			return *itor;
		}
	}
	return NULL;
}

void CMsgBoxMgr::PopSimpleComfirm(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	MsgBoxInfo msg;
	if(pStr)
	{
		msg.m_strMainString = pStr;
	}
	msg.m_stcOkMsg.dwMsg = dwMsg;
	msg.m_stcOkMsg.dwData = dwData;
	msg.m_stcOkMsg.pControl = pControl;
	msg.m_dwProperty = MSGBOX_NEED_CANCLE_BT;
	PushMsg(msg);
}

void CMsgBoxMgr::PopOkCancelBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl,
					DWORD dwCancelMsg, DWORD dwCancelData, CControl* pCancelControl,bool bModel)
{
	MsgBoxInfo msg;
	if(pStr)
	{
		msg.m_strMainString = pStr;
	}
	msg.m_stcOkMsg.dwMsg = dwMsg;
	msg.m_stcOkMsg.dwData = dwData;
	msg.m_stcOkMsg.pControl = pControl;
	msg.m_stcCancelMsg.dwMsg = dwCancelMsg;
	msg.m_stcCancelMsg.dwData = dwCancelData;
	msg.m_stcCancelMsg.pControl = pCancelControl;
	msg.m_dwProperty = MSGBOX_NEED_CANCLE_BT;
	PushMsg(msg);
}

void CMsgBoxMgr::PopSimpleAlert(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl)
{
	MsgBoxInfo msg;
	if(pStr)
	{
		msg.m_strMainString = pStr;
	}
	msg.m_stcOkMsg.dwMsg = dwMsg;
	msg.m_stcOkMsg.dwData = dwData;
	msg.m_stcOkMsg.pControl = pControl;
	PushMsg(msg);
}


void CMsgBoxMgr::PopTagAlert(const char* pStr,DWORD dwMsg,DWORD dwData, CControl* pControl)
{
	MsgBoxInfo msg;
	if(pStr)
	{
		msg.m_strMainString = pStr;
	}
	msg.m_stcOkMsg.dwMsg = dwMsg;
	msg.m_stcOkMsg.dwData = dwData;
	msg.m_stcOkMsg.pControl = pControl;
	msg.m_dwProperty = MSGBOX_NEED_TAG;
	PushMsg(msg);
}

void CMsgBoxMgr::PopTagOkCancelBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl,
								DWORD dwCancelMsg, DWORD dwCancelData, CControl* pCancelControl,bool bModel)
{
	MsgBoxInfo msg;
	if(pStr)
	{
		msg.m_strMainString = pStr;
	}
	msg.m_stcOkMsg.dwMsg = dwMsg;
	msg.m_stcOkMsg.dwData = dwData;
	msg.m_stcOkMsg.pControl = pControl;
	msg.m_stcCancelMsg.dwMsg = dwCancelMsg;
	msg.m_stcCancelMsg.dwData = dwCancelData;
	msg.m_stcCancelMsg.pControl = pCancelControl;
	msg.m_dwProperty = MSGBOX_NEED_CANCLE_BT | MSGBOX_NEED_TAG;
	PushMsg(msg);
}

void CMsgBoxMgr::PopCheckedBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl, 
		const char* pCheck, DWORD dwCancelMsg, DWORD dwCancelData, CControl* pCancelControl)
{
	MsgBoxInfo msg;
	if(pStr)
	{
		msg.m_strMainString = pStr;
	}
	msg.m_stcOkMsg.dwMsg = dwMsg;
	msg.m_stcOkMsg.dwData = dwData;
	msg.m_stcOkMsg.pControl = pControl;
	msg.m_stcCancelMsg.dwMsg = dwCancelMsg;
	msg.m_stcCancelMsg.dwData = dwCancelData;
	msg.m_stcCancelMsg.pControl = pCancelControl;
	if(pCheck)
	{
		msg.m_strCheckString = pCheck;
	}
	msg.m_dwProperty = MSGBOX_NEED_CANCLE_BT | MSGBOX_NEED_CHECK;
	PushMsg(msg);
}

void CMsgBoxMgr::PopEditBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl,
		bool bDigital, int iMaxLen, DWORD dwCancelMsg, DWORD dwCancelData, CControl* pCancelControl,bool bModel,const char* pStrDefaultEditContent,bool bForbidDirtyWord)
{
	MsgBoxInfo msg;
	if(pStr)
	{
		msg.m_strMainString = pStr;
	}
	msg.m_stcOkMsg.dwMsg = dwMsg;
	msg.m_stcOkMsg.dwData = dwData;
	msg.m_stcOkMsg.pControl = pControl;
	msg.m_stcCancelMsg.dwMsg = dwCancelMsg;
	msg.m_stcCancelMsg.dwData = dwCancelData;
	msg.m_stcCancelMsg.pControl = pCancelControl;
	msg.m_dwProperty |= MSGBOX_NEED_EDIT | MSGBOX_NEED_CANCLE_BT;
	msg.m_bModel = bModel;
	msg.m_bForbidDirtyWord = bForbidDirtyWord;

	if(pStrDefaultEditContent && strlen(pStrDefaultEditContent) > 0)
	{
		msg.m_strEditString = pStrDefaultEditContent;
	}
	else
	{
		msg.m_strEditString.clear();
	}

	if(iMaxLen)
	{
		msg.m_dwProperty |= MSGBOX_NEED_LIMITED_EDIT;
		msg.m_dwMsgLength = iMaxLen;
	}
	if(bDigital)
	{
		msg.m_dwProperty |= MSGBOX_NEED_DIGITAL_EDIT;
	}
	PushMsg(msg);
}

CCtrlMsgBox* CMsgBoxMgr::FindMsgBox(DWORD dwMsg)
{
	for(MSGBOXLIST::iterator itor = m_vMsgBoxList.begin(); itor != m_vMsgBoxList.end(); ++itor)
	{
		CCtrlMsgBox* p = (*itor);
		if(p == NULL)
			continue;

		if(p->GetInfo().m_stcOkMsg.dwMsg == dwMsg)
			return p;
	}
	return NULL;
}

CMsgBoxMgr	g_MsgBoxMgr;
