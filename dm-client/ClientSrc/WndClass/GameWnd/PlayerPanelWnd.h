#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CPlayerPanelWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CPlayerPanelWnd)

public:
	CPlayerPanelWnd(void);
	~CPlayerPanelWnd(void);

	virtual void OnCreate();
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw(void);


protected:
	DWORD m_dwHPAlarmRand;//血量小于20%时音效的Rand

	void UpdateLocalState(void);
	bool IsInExp(int ix,int iy);//ix,iy是否在经验条位置
	bool IsInWeight(int ix,int iy);//ix,iy是否在负重条位置
	int InAlarmTip(int iX, int iY);
	bool IsInHP(int ix,int iy);//ix,iy是否在HP位置
	bool IsInMP(int ix,int iy);//ix,iy是否在MP位置
	bool IsInFace(int ix,int iy);//ix,iy是否在头像位置


};
