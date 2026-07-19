#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"

class CReceiveMsgWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CReceiveMsgWnd)

public:
	CReceiveMsgWnd(void);
	~CReceiveMsgWnd(void);

public:

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();

	void SetCurLeaveWord(int nCurLeaveWord);
private:
	CCtrlMultiEdit		*m_pMultiEdit;

	//接受消息
	int m_nCurLeaveWord;
	CCtrlButton * m_pPre;
	CCtrlButton * m_pNext;
	CCtrlButton * m_pDelete;
	CCtrlButton * m_pClearAll;
};
