#pragma once
#include "BaseClass/Control/CtrlWindowX.h"


class CBindPTWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBindPTWnd)

public:
	CBindPTWnd(void);
	~CBindPTWnd(void);

	virtual void OnCreate();
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

protected:
	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pZhuButton;

	CCtrlEdit   * m_pEditUserID;            // 用户id
	CCtrlEdit   * m_pEditPassword;	        // 用户密码
};
