#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

enum EBlessCompoundType
{
	EBCT_SXRH = 1,	// 属性融合
	EBCT_GNRH,		// 功能融合
	EBCT_SYHF,		// 神佑恢复
	EBCT_JLL,		// 纹佩精炼炉
	EBCT_BJRH,		// 纹佩部件融合
};

// 神佑融合
class CBlessCompoundWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CBlessCompoundWnd)

public:
	CBlessCompoundWnd(void);
	~CBlessCompoundWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);

protected:
	void ShowBlessTips(const char* str, int val);

	CCtrlButton* m_pCancle;	
	CCtrlButton* m_pOK;

	int m_iType;

	int m_iMsg;
	int m_iSuccessFrame;
	int m_iFailFrame;

	int m_iOneLineShowNum;
	int m_iShowStartY;
	char m_str[256];
	
	int m_iRotFrame;
};
