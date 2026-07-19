#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "TodayActivityLocalWnd.h"
#include "TodayActivityIEWnd.h"

class CTodayActivityWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTodayActivityWnd)

public:
	CTodayActivityWnd(void);
public:
	~CTodayActivityWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);	
	virtual void ResetControlPos();


protected:
	CTodayActivityLocalWnd *m_pActivityLocalWnd;
	CTodayActivityIEWnd *m_pActivityIEWnd;
	CCtrlButton * m_pStartButton;


	DWORD         m_dwEnterTime;   //进入游戏的时间
	VString       m_VMessages;
	DWORD         m_dwTime;
	int           m_iHorseDrawX;
	int           m_iDrawLine;

	void SetControlState();
};
