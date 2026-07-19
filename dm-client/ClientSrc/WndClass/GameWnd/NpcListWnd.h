#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"

class CNpcListWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcListWnd)

public:
	CNpcListWnd(void);
public:
	~CNpcListWnd(void);

public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnWheel(int iWheel);
private:
	int     m_iType;
	CCtrlScroll* m_pScroll;
	static const int     MAX_ROWS;

};
