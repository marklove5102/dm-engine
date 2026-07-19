#pragma once

#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"


class CShortCutWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CShortCutWnd)

public:
	CShortCutWnd(void);
	~CShortCutWnd(void);

public:
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

private:
	CCtrlRadio* m_pKeyButton[9];
	CCtrlButton * m_pOk;
	CCtrlButton * m_pCancel;
	int m_nPos;
	int m_nKey;
	int m_kSetShortCutKeyType;//1为普通技能，2为快捷键，3为法决

	int m_nConSkillID;

};
