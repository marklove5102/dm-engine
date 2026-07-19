#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

//心魔宝宝属性列表
#define MAX_DEMON_PET_NUM   5

class CPetInfoWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CPetInfoWnd)

public:
	CPetInfoWnd();
	~CPetInfoWnd(void);

public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnLeftButtonUp(int iX,int iY);

private:
	CCtrlButton*  m_pNextPage;
	CCtrlButton*  m_pPriorPage;

	int     m_iLineIndex;
};
