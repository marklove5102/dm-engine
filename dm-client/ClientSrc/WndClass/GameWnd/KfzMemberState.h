#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"


class CKfzMemberState : public CCtrlWindowX
{
	DECLARE_WND_POSX(CKfzMemberState)

public:
	CKfzMemberState(void);
	~CKfzMemberState(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnWheel(int iWheel);	
protected:
	CCtrlGrid *m_pGrid;//列表控件
	CCtrlButton * m_pOkBtn,*m_pCancelBtn; //按钮

	struct _KfzMemberState
	{
		string strName;//本体名字
		BYTE byState;//本体状态
		string strMerName;//元神名字
		BYTE byMerState;//元神状态
	};
	vector<_KfzMemberState> m_VMemberStateList;//行会成员状态列表

	void OnGetMemberStateList(const char *msg,int iLen);

};
