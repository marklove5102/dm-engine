#pragma once

#include "CtrlWindowS.h"
#include "CtrlRadio.h"
#include "CtrlEdit.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"

typedef struct tagMsgBoxInfo
{
	CControl::MSG_STRUCT	m_stcOkMsg;
	CControl::MSG_STRUCT	m_stcCancelMsg;
	DWORD	m_dwProperty;
	DWORD	m_dwShowTick;
	DWORD	m_dwStartTick;
	DWORD	m_dwMsgLength;
	string	m_strMainString;
	string	m_strCheckString;
	string	m_strEditString;//输入框默认内容
	string	m_strServerString;
	bool    m_bModel;//是否模式框
	bool    m_bForbidDirtyWord;//是否禁止屏蔽关键字

	tagMsgBoxInfo();
	void operator=(const tagMsgBoxInfo& rf);
} MsgBoxInfo;

typedef enum eMSGBOX_PROPERTY
{
	MSGBOX_NEED_NONE			= 0,
	MSGBOX_NEED_TIME_LINE		= 1<<0,  //
	MSGBOX_NEED_EDIT			= 1<<1,  //需要edit框
	MSGBOX_NEED_CHECK			= 1<<2,  //有check框
	MSGBOX_NEED_LIMITED_EDIT	= 1<<3,  //edit框有最大字符数限制
	MSGBOX_NEED_DIGITAL_EDIT	= 1<<4,  //edit只能输入数字
	MSGBOX_NEED_CANCLE_BT		= 1<<5,  //需要取消按钮
	MSGBOX_SERVER_MSG			= 1<<6,  //服务器端触发的,确定或取消自动完成和服务器之间的交互
	MSGBOX_NEED_TAG             = 1<<8   //npcText
}MSGBOX_PROPERTY;

class CCtrlMsgBox :	public CCtrlWindowS
{
	DECLARE_WND_POS(CCtrlMsgBox)
public:
	CCtrlMsgBox(void);
	virtual ~CCtrlMsgBox(void);
	virtual void OnCreate();
	virtual void OnClickOkButton();
	virtual void OnClickCancelBt();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl=NULL);
	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	virtual void OnClickCloseButton();
	virtual void OnDraw();
	virtual bool SetFocus(void);
	virtual bool OnLeftButtonUp(int iX, int iY);

	MsgBoxInfo& GetInfo(){ return m_Info; }

	bool IsChecked() const;
	int GetNum() const;
	const char* GetInputString() const;
	DWORD GetProperty() const;
protected:
	CCtrlButton*	m_pOKButton;
	CCtrlButton*	m_pCancelBt;
	CCtrlRadio*	    m_pCheck;
	CCtrlEdit*		m_pEdit;
	MsgBoxInfo		m_Info;
	RECT			m_rcTextPos;
	CMarkViewer*    m_pMarkViewer;
    CTagText        m_npcText;

};
