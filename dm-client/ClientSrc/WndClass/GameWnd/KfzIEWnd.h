#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

class CKfzIEWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CKfzIEWnd)

public:
	CKfzIEWnd(void);
	~CKfzIEWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnMove();
	virtual void OnSetFocus();
	virtual void OnKillFocus();

protected:
	int m_iType;//1:排名,2:对阵图,3:仙灵大陆排名
	DWORD m_dwStartLoadPageTime;

};