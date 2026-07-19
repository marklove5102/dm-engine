#pragma once
#include "BaseClass/Control/CtrlMsgBox.h"
#include "BaseClass/Control/CtrlButton.h"


class CLevelCheck :	public CCtrlMsgBox
{
	DECLARE_WND_POS(CLevelCheck)

public:
	CLevelCheck();
	virtual ~CLevelCheck(void);
	virtual void OnClickOkButton();
	virtual void Draw();

private:
	LPTexture m_pValidate;
};
