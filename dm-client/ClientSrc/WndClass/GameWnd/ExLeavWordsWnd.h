#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"

class CExLeavWordsWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CExLeavWordsWnd)

public:
	CExLeavWordsWnd(void);
	~CExLeavWordsWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate(void);

protected:
	int m_type;
	CCtrlButton *   m_pOK;
	CCtrlButton *   m_pCancel;
	CCtrlMultiEdit* m_pInfo;
};
