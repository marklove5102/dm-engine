#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"

#define WOOLSTORY_PAGES  3

class CWooolStoryWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CWooolStoryWnd)

public:
	CWooolStoryWnd(void);
	~CWooolStoryWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void ResetControlPos();

protected:
	CCtrlButton * m_pNextPageBtn;
	CCtrlButton * m_pPrePageBtn;
	CCtrlButton * m_pCloseBtn;

	int m_iTexLeft;//左边起画位置
	int m_iMaxWidth;//最大宽度
	int m_iCurPage;//当前显示页
};
