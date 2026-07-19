#pragma once

#include "BaseClass/Control/CtrlWindowX.h"


#define MAX_ROWS    10 

class CNameWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CNameWnd)

public:
	CNameWnd();
	~CNameWnd(void);

public:
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);	

private :
	int m_iParentX;
	int m_iParentY;
	int m_iStart;
	int m_iCurrent;//µ±Ç°ÐÐ
	int m_iCount;
	int m_iType;
	BYTE m_byFirstChar;
};
