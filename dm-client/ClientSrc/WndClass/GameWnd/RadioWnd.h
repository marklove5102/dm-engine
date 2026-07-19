#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/MarkViewer.h"

class CRadioWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CRadioWnd)

public:
	CRadioWnd();
	~CRadioWnd(void);
public:

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual void OnMove();
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual void OnSetFocus();
	virtual void OnKillFocus();

	void SetCurLeaveWord(int nCurLeaveWord);
	void SaveSoundOnOff();
private:
	DWORD m_dwStartLoadRadioTime;

	CCtrlButton* m_pCloseRadioBtn;//关闭
	CMarkViewer* m_pContainer;
	int m_iWidgetX,m_iWidgetY;

	CCtrlRadio*          m_pRDSoundOn;//开启游戏音效
	CCtrlRadio*          m_pRDSoundOff;//关闭游戏音效

};