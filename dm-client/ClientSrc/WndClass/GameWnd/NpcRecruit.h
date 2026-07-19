#pragma once
#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlWindowX.h"

#define NPCRECRUIT_MAX_ROWS 13
class CNpcRecruit:public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcRecruit)

public:
	CNpcRecruit(void);
	~CNpcRecruit(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual void Draw();

private:
	CMarkViewer* m_pMarkViewer;
	CCtrlButton * m_pCommitBtn[NPCRECRUIT_MAX_ROWS];
	CCtrlButton * m_pPrePage;
	CCtrlButton * m_pNextPage;
	CCtrlButton * m_pApply;

	int m_iPage,m_iSubPage;//当前页及子页
};