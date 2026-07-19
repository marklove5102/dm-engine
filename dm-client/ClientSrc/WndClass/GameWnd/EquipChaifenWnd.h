#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/good.h"


class CEquipChaifenWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CEquipChaifenWnd)

public:
	CEquipChaifenWnd(void);
	~CEquipChaifenWnd(void);

	virtual void Draw();
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);

protected:
	void BackToPackage();
	CGood* GetGoodOnGrid(int iX,int iY,bool& grid);
	bool IsInGrid(int iX,int iY, int gridx, int gridy);
	bool IsInValidGoodOnGrid(CGood& good);
	void SendEquipSoulMsg();

	CCtrlButton * m_pOkBtn;
	CMarkViewer * m_pMarkViewer;
	CTagText	m_Tag;	

	int m_iFrame;
	bool m_bFirstClick;
};