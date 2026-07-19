#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/MarkViewer.h"

class CExtractAndUpgradeYuanshiWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CExtractAndUpgradeYuanshiWnd)
public:
	CExtractAndUpgradeYuanshiWnd(void);
	virtual void OnCreate();
	virtual void Draw(void);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	~CExtractAndUpgradeYuanshiWnd(void);
private:
	CCtrlButton * m_pOkBtn;
	int m_iFrame;
	int m_iResultSuccessEffFrame;
	int m_iResultFailEffFrame;

	bool m_bFirstClick;
	string m_strSkillLevel;

	CMarkViewer * m_pMarkViewer;
};
