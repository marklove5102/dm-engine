#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CKfzFlow : public CCtrlWindowX
{
	DECLARE_WND_POSX(CKfzFlow)

public:
	CKfzFlow(void);
	~CKfzFlow(void);
public:
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual void OnKillFocus();


};
