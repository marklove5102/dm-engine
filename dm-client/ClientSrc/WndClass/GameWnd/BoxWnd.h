#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CBoxWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBoxWnd)
public:
	CBoxWnd(void);
	~CBoxWnd(void);
	virtual void OnCreate();
	virtual void Draw();
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual bool OnMouseMove(int iX,int iY);

	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	int GetGrid(int iX,int iY);
	static void SetLastAccessTime(DWORD t){sm_LastAccessTime = t;}
	static DWORD GetLastAccessTime(){return sm_LastAccessTime;}
	int GetGridOrLevel(int i,int iType);
private:
	static DWORD sm_LastAccessTime;

	CCtrlButton * m_pOpenBox;
};
