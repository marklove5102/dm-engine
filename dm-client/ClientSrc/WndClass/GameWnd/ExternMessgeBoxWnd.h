#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"

class CExternMessgeBoxWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CExternMessgeBoxWnd)

public:
	CExternMessgeBoxWnd(void);
	~CExternMessgeBoxWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate(void);
	virtual bool OnWheel(int iWheel);

	void ShowText(const char * sMsg);
	void SetLineLen(int iLen);

protected:
	bool IsInNameArea(int iX,int iY);

protected:
	int m_type;
	int m_iLineLen;
	char* m_sMsg;
	char  name[128];
	
	void DrawText(void);

	CCtrlButton * m_pOK;          //Œ““™¡Ù—‘
	CCtrlButton * m_pCancel;      //¿Îø™
	CCtrlMultiEdit * m_pLeavWords;//¡Ù—‘øÚ
    CCtrlMultiEdit * m_pInfo;

	CCtrlButton * m_pUpButton_1;
	CCtrlButton * m_pDownButton_1;
	CCtrlScroll * m_pScroll_1;

	CCtrlButton * m_pUpButton_2;
	CCtrlButton * m_pDownButton_2;
	CCtrlScroll * m_pScroll_2;

	CCtrlButton * m_pGetFlag;
};
