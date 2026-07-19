#pragma once
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"
class CFairyGuide : public CCtrlWindowX
{
	DECLARE_WND_POSX(CFairyGuide)
public:
	CFairyGuide(void);
	~CFairyGuide(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnWheel(int iWheel);

protected:
	void InitialFairyGuide();
protected:
	CCtrlButton*  m_pPreBtn;
	CCtrlButton*  m_pNextBtn;

	string			m_strTile;
	CMarkViewer*	m_pContent;
	CTagText		m_TagText;
	int             m_iCurLine;//当前显示那一条
};
