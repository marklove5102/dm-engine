#pragma once
#include "BaseClass/Control/CtrlWindowX.h"


class CComposeEquipWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CComposeEquipWnd)

public:
	CComposeEquipWnd(void);
	~CComposeEquipWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);

protected:

	CCtrlButton* m_pCancle;	
	CCtrlButton* m_pOK;

	int m_iType;

	int m_iMsg;	

	int m_iUpFrame;
	int m_iRotFrame;
	int m_GoodFrame;

	DWORD m_dwLianHunGoodid;
	WORD  m_wShowIconLooks;
	std::string m_LianHunName;
	int m_iHunLevel;
};
