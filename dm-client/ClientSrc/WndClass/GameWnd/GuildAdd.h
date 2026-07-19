#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include <string>

class CGuildAdd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CGuildAdd)

public:
	CGuildAdd();

	virtual bool Create(CControl * pParent,int iX,int iY);
	virtual void Draw(void);
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual void OnClickCloseButton();

private:
	void OnOk();
	void OnCancel();

private:
	std::string m_txt;
	std::string m_txt2;
	CCtrlButton * m_pOk;
	CCtrlButton * m_pCancel;
	DWORD m_begin_time;
	bool   m_bFriend;
};