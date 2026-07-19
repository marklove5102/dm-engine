#pragma once

#include "BaseClass\Control\CtrlTag.h"
#include "BaseClass\Control\CtrlMsgBox.h"
#include "TagText.h"

class CCtrlMsgBox;

class CMsgBoxMgr :	public CCtrlBaseTag
{
DTI_DECLARE(CMsgBoxMgr, CCtrlBaseTag)
public:
	CMsgBoxMgr(void);
	virtual ~CMsgBoxMgr(void);
	void Clear();
	void PushMsg(const MsgBoxInfo& msg);
	bool PopMsg(MsgBoxInfo& rsvMsg);
	DWORD GetMsgNum() const { return m_vMsgBuf.size();}
	void SetInstance(CCtrlMsgBox* p) { m_pMsgBoxInstance = p;}
	CCtrlMsgBox* GetInstance() { return m_pMsgBoxInstance;}
	void AddMsgBoxTrack(CCtrlMsgBox* p);
	void RemoveMsgBoxTrack(CCtrlMsgBox* p);
	void ClearAllExistMsgBox();
	bool ClearExistMsgBox(const string &strMainString);//根据显示内容清除存在的msgbox
	int  GetCurMsgBoxListSize() const { return m_vMsgBoxList.size();}
	CCtrlMsgBox* GetLastMsgBox();
	CTagText& GetTag(); //标记文本

	CCtrlMsgBox* FindMsgBox(DWORD dwMsg = 0);
	void PopSimpleComfirm(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl = NULL);
	void PopSimpleAlert(const char* pStr, DWORD dwMsg = 0, DWORD dwData = 0, CControl* pControl = NULL);
	void PopCheckedBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl, 
		const char* pCheck, DWORD dwCancelMsg=0, DWORD dwCancelData=0, CControl* pCancelControl=0);
	void PopEditBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl = NULL,
		bool bDigital=false, int iMaxLen=0, DWORD dwCancelMsg=0, DWORD dwCancelData=0, CControl* pCancelControl=0,bool bModel = true,const char* pStrDefaultEditContent = NULL,bool bForbidDirtyWord = false);
	void PopOkCancelBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl = NULL,
		DWORD dwCancelMsg=0, DWORD dwCancelData=0, CControl* pCancelControl=0,bool bModel = true);
	void PopTagOkCancelBox(const char* pStr, DWORD dwMsg, DWORD dwData, CControl* pControl = NULL,
		DWORD dwCancelMsg=0, DWORD dwCancelData=0, CControl* pCancelControl=0,bool bModel = true);
	void PopTagAlert(const char* pStr,DWORD dwMsg = 0,DWORD dwData = 0, CControl* pControl = NULL);
protected:
	typedef std::deque<MsgBoxInfo>	MSGBUFTYPE;
	MSGBUFTYPE	m_vMsgBuf;
	CCtrlMsgBox*	m_pMsgBoxInstance;
	typedef std::list<CCtrlMsgBox*>	MSGBOXLIST;
	MSGBOXLIST	m_vMsgBoxList;
	CTagText    m_vTag; //标记文本
};

extern CMsgBoxMgr	g_MsgBoxMgr;