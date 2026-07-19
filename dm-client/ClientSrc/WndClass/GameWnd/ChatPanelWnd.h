#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMenuButton.h"


class CChatPanelWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CChatPanelWnd)

public:
	CChatPanelWnd(void);
	~CChatPanelWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

protected:
	void OnChangeChannel(CControl * pControl);//点了改变频道按钮

	CCtrlButton*	m_pCommandBtn;
	CCtrlButton*	m_pChangeChannelBtn;
	CCtrlButton*	m_pChangeSizeBtn;
	
	CCtrlRadio*	m_pChannelPhyle;
	CCtrlRadio*	m_pChannelNormal;
	CCtrlRadio*	m_pChannelPrivate;	
	CCtrlRadio*	m_pChannelPublic;
	CCtrlRadio*	m_pChannelTroop;
	CCtrlRadio*	m_pChannelGuild;

	struct stTalkFilter
	{
		stTalkFilter(const char* s,DWORD flt)
		{
			str.assign(s);
			filter = flt;
		}
		string str;
		DWORD  filter;
	};
	vector<stTalkFilter> m_vTalkFilter;
};
