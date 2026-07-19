#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include <string>
#include <vector>

class CNoticeHelpWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNoticeHelpWnd)

public:
	CNoticeHelpWnd(void);
	~CNoticeHelpWnd(void);
public:
	//÷ÿ‘ÿ∏∏¿‡∫Ø ˝
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
private:
	std::vector<std::string>  vecStr;
	std::string          str;
};
