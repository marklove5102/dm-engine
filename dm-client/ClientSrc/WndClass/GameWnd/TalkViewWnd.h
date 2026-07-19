#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/GlyphEdit.h"
#include "BaseClass/Control/TalkViewer.h"

class CTalkViewWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTalkViewWnd)

public:
	CTalkViewWnd(void);
public:
	~CTalkViewWnd(void);

	virtual void OnCreate(void);
	virtual void Draw();
	//virtual void OnDraw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool IsInControl(int iX,int iY);
	virtual bool OnWheel(int iWheel);
	//virtual bool OnMouseMove(int iX, int iY);
	//virtual bool OnLeftButtonUp(int iX,int iY);
	//virtual bool OnLeftButtonDClick(int iX,int iY);
	//virtual void ResetControlPos();

	bool IsFakeFocus();
	void ChangeTalkAutoWheel();
	CGlyphEdit*  GetEditCtrl(){ return m_pPanelEdit; }

protected:
	CTalkViewer*   m_pTalkViewer;
	CGlyphEdit*    m_pPanelEdit; //¡ƒÃÏ ‰»ÎøÚ


	//void SetControlState();
	bool OnClickTalkView();

};
