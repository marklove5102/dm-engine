#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/Good.h"

class CBlessZhongZhouWnd : public CCtrlWindowX
{

	DECLARE_WND_POSX(CBlessZhongZhouWnd)

public:
	CBlessZhongZhouWnd(void);
	~CBlessZhongZhouWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);
	virtual void OnClickCloseButton();
	virtual bool OnMouseMove(int iX,int iY);

	int GetMaterialType(const char* pName);
	void SetButtonsTextByMtrl(int iMtrlType);

protected:
	CCtrlButton* m_pOK;
	CCtrlButton* m_pCancel;
	CCtrlButton* m_pChars[4];
	CCtrlButton* m_pCharButton;
	CCtrlButton* m_pChangeButton;
	int m_iSelected;
	RECT rcChar, rcChange;
	int m_iType;
	const static char* m_pMtrlList[10];
};
