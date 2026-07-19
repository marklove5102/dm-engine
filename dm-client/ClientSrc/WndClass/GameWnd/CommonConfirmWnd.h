#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"


class CCommonConfirmWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CCommonConfirmWnd)

public:
	CCommonConfirmWnd(void);
	~CCommonConfirmWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);


protected:
	CCtrlButton * m_pBtnOk; //确定
	CCtrlButton * m_pBtnCancel; //取消
	CMarkViewer * m_pMarkViewer;
	CTagText m_TagText;

};
