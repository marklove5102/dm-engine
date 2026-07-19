#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"

class CSanWeiFireWnd :
	public CCtrlWindowX
{

	DECLARE_WND_POSX(CSanWeiFireWnd)

public:
	CSanWeiFireWnd(void);
	~CSanWeiFireWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool IsInControl(int iX,int iY);

	void ShowHelp();


	CCtrlButton*  m_pShowHelp;
	CTagText	  m_kHelpText;
	CMarkViewer*  m_pMarkViewer;

	CCtrlButton*  m_pLianHua;				//Á¶»¯
	static char*  s_vParaName[17];
	static char*  s_vJingJieName[9];
};
