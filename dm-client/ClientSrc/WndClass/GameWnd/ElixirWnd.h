#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
class CElixirWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CElixirWnd)
public:
	CElixirWnd(void);
	~CElixirWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
private:
	int GetNeedCount(int i);
private:
	CCtrlButton* m_pBtnBuy[4];
	CCtrlButton* m_pBtnAdd[4];
	CCtrlButton* m_pBtnDec[4];

	CCtrlGrid*   m_pGrid;
	int			 m_iCount[4];
};
