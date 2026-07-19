#pragma once
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"


class CBoothSelectWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBoothSelectWnd)

public:
	CBoothSelectWnd(void);
	~CBoothSelectWnd(void);

public:
	//÷ÿ‘ÿ∏∏¿‡∫Ø ˝
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

protected:
	CCtrlButton* m_pPlayerBoothButton;
	//CCtrlButton* m_pUploadButton;
};
