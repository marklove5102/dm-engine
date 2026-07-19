#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Misc/Internet.h"
#include "BaseClass/Control/CtrlMultiEdit.h"


// »Ø¸´
class CReplyWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CReplyWnd)

public:
	CReplyWnd();
	virtual ~CReplyWnd(void);

public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate();

	virtual void Draw();
private:
	int m_idx;
	CCtrlMultiEdit		*m_pMultiEdit;
	CCtrlButton * m_pButtonOK;
	CCtrlButton * m_pButtonCancel;
	CCtrlButton * m_pButtonPhrase;
public:
	void SetEditText(const char * sText);
};

// ×ª·¢
class CForwardWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CForwardWnd)

public:
	CForwardWnd();
	virtual ~CForwardWnd(void);

public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate();
	virtual void Draw();

private:

	int m_idx;
	CCtrlButton	*m_pButtonOK;
	CCtrlButton	*m_pButtonCancel;
	CCtrlButton	*m_pButtonPhrase;
	CCtrlMultiEdit		*m_pMultiEdit;
	CCtrlEdit			*m_pEdit;
public:
	void SetEditText(const char * sText);
};

// ¶ÌÓï
class CPhraseWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CPhraseWnd)

public:
	CPhraseWnd(void);
	virtual ~CPhraseWnd(void);

public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate();
	virtual void Draw();
	virtual bool OnLeftButtonDown(int iX, int iY);

private:
	int				m_iType;
	int				m_idx;
	int				m_iCx,m_iCy;
	CCtrlMultiEdit	*m_pMultiEdit;
	CCtrlScroll		*m_pScroll;
};


