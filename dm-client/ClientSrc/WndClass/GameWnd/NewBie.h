#pragma once


#include "BaseClass/Misc/Internet.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"

class CNewBie : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNewBie)

public:
	CNewBie(void);
	~CNewBie(void);
private:
	static CInternet m_inter;
	CMarkViewer *m_pMarkViewer;
public:
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
};
