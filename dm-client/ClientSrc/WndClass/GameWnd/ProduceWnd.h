#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/MarkViewer.h"
class CProduceWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CProduceWnd)
public:
	CProduceWnd(void);
	~CProduceWnd(void);
	typedef std::map<string,int> MAPINTSTR;
public:
	virtual void OnCreate();
	virtual void Draw(void);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
protected:
	void UpdateDescription(CGood& good);
protected:
	CGood		m_Good;
	CTagText	m_Tag;
	CCtrlButton * m_pOkBtn;
	int m_iProbabity;
	int m_iType;//¥∞ø⁄¿‡–Õ
	int m_iFrame;
	int m_iResultSuccessEffFrame;
	int m_iResultFailEffFrame;

	bool m_bFirstClick;
	bool m_bStartAin;
	bool m_bFine;
	int  m_iFlag;
	bool m_bDrawSucess;
	bool m_bDrawFoundSucess;
	bool m_bDrawFoundFail;

	CMarkViewer * m_pMarkViewer;
};
