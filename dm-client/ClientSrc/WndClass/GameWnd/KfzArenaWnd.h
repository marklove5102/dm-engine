#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include <vector>

#define KFZ_LEITAI_LINE_COUNT 12

class CKfzArenaWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CKfzArenaWnd)

public:
	CKfzArenaWnd(void);
	~CKfzArenaWnd(void);

public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

protected:
	struct _KfzLeiTai
	{
		int iNo; //擂台号
		string strGuildName;//发起行会
		string strAreaServer;//来自什么区服
		DWORD  dwDeposit;//押金
		int iLeaverMins;//剩余时间,多少分种
		string strMapName;//地图编号
	};

	CCtrlGrid *m_pGrid;//列表控件
	CCtrlButton * m_pPreBtn,*m_pNextBtn; //添加,删除按钮
	int m_iTotalNum,m_iStart,m_iEnd;
	std::vector<_KfzLeiTai> m_VKfzLeiTai;


	void OnGetMemberList(const char* msg);

};
