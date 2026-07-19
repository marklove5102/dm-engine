#pragma once

#include "Global/Global.h"
#include "ReliveWnd.h"

class CReliveWithYuanBaoWnd: public CReliveWnd
{
	DECLARE_WND_POSX(CReliveWithYuanBaoWnd)
public:
	CReliveWithYuanBaoWnd(void);
	~CReliveWithYuanBaoWnd(void);

	virtual void OnCreate(void);
	virtual void Draw(void);
	virtual void OnClickCloseButton();
protected:
	virtual void OnReliveImm();
};
