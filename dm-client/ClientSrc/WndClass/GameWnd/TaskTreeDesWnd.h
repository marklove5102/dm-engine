#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlButton.h"

class CTaskTreeDesWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTaskTreeDesWnd)
public:
	CTaskTreeDesWnd(void);
	~CTaskTreeDesWnd(void);

	virtual void Draw(void);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
public:
	void SetDesText(string& str);
protected:
	CMarkViewer* m_pMVTaskDes;
	CTagText	 m_TagText;
	CCtrlButton* m_pOK;
};
