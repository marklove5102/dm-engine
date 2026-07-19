#pragma once
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"
#include "BaseClass/Control/CtrlTreeView.h"

class CMemoireWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CMemoireWnd)
public:
	CMemoireWnd(void);
	~CMemoireWnd(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);
	void OnSetFocus();
	void OnKillFocus();
protected:
	void BuildTree();
protected:
	CCtrlTreeView*  m_pPromptTree;
	CMarkViewer*	m_pContent;
	CTagText		m_TagText;

	//  [2/2/2010 dujun]
	DWORD m_selectedNode;
};
