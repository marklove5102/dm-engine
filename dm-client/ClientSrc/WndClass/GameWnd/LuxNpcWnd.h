#pragma once

#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlWindowX.h"

class CLuxNpcWnd : public CCtrlWindowX
{
    DECLARE_WND_POSX(CLuxNpcWnd)
public:
	CLuxNpcWnd(void);
	~CLuxNpcWnd(void);
    virtual void OnCreate();
    virtual void Draw(void);
    virtual void OnDraw();
    virtual void OnClickCloseButton();
    virtual bool OnLeftButtonDown(int iX, int iY);
    virtual bool OnLeftButtonUp(int iX, int iY);
    virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);	
   	virtual bool OnMouseMove(int iX, int iY);

private:
    CMarkViewer* m_pMarkViewer;
    int m_iType;
	int m_iPercent;

    CCtrlButton* m_pGoWxxgBtn;    
    CCtrlButton* m_pGetPrizeBtn;
    CCtrlButton* m_pQuitBtn;
    CCtrlButton* m_pReplayBtn;

    WORD m_iStartFrame;
    WORD m_iTotalFrame;
    WORD m_vPauseFrame[5];
    WORD m_wCurFrames;
    WORD m_wCriticalFrames;
    WORD m_iCount;
};
