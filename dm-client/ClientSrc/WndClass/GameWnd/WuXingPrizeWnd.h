#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

//快捷消费提醒框
class CWuXingPrizeWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CWuXingPrizeWnd)

public:
	CWuXingPrizeWnd();
	~CWuXingPrizeWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual bool OnMouseMove(int iX,int iY);

protected:

	CCtrlButton* m_pOKButton;
	CCtrlButton* m_pChangeLiHuoLing;
    CCtrlButton* m_pCancelButton;
    int m_iType;    //1为发奖窗口，2为询问窗口,3铁血战歌副本结果窗口
	bool m_bIsJinKa;
};
