#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"

class CHeartKnackWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CHeartKnackWnd)
public:
	CHeartKnackWnd(void);
	~CHeartKnackWnd(void);
public:
	virtual void	OnCreate();	
	virtual void	Draw();
	virtual bool	Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool	OnMouseMove(int iX,int iY);
	virtual bool    OnLeftButtonUp(int iX,int iY);

protected:
	void ShowHelp();

	CCtrlButton* m_pLeftBtn;
	CCtrlButton* m_pRightBtn;
	CCtrlButton* m_pBtn[4];
	CCtrlButton* m_pHelpBtn;     //帮助
	CCtrlButton* m_pCloseHelpBtn;//关闭帮助按钮
	
	bool         m_bShowHelp;
	CTagText	 m_npcText;
	CMarkViewer* m_pMarkViewer;  //数据显示框 
};
