#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"

//快捷消费提醒框
class CInstanceZonePopupWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CInstanceZonePopupWnd)

public:
	CInstanceZonePopupWnd();
	~CInstanceZonePopupWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual bool OnMouseMove(int iX,int iY);

protected:
	int m_iType;    //1为发奖窗口，2为询问窗口

	CMarkViewer *m_pMarkViewer;

	CCtrlButton* m_pOkBtn;

	CCtrlButton* m_pTuTeng;
	CCtrlButton* m_pJinBi;
	CCtrlButton* m_pYuanBao;	
	CCtrlButton* m_pBaiHu;
	CCtrlButton* m_pJinLong;

	POINT m_ptCardPos[5];
	int m_iResult;
	CTagText m_TagText;

};