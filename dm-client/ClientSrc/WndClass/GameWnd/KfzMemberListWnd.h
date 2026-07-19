#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "GameData/OtherData.h"


#define PERSONS_KFZ_LINE_COUNT 10

//跨服战行会成员列表
class CKfzMemberListWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CKfzMemberListWnd)

public:
	CKfzMemberListWnd(void);
	~CKfzMemberListWnd(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnWheel(int iWheel);	

protected:
	CCtrlGrid *   m_pGrid;//列表控件
	CCtrlButton * m_pClose; //离开按钮

};