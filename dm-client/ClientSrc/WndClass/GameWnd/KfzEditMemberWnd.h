#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "GameData/OtherData.h"


#define KFZ_EDITMEMBER_LINE_COUNT_LEFT 18
#define KFZ_EDITMEMBER_LINE_COUNT_RIGHT 18



class CKfzEditMemberWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CKfzEditMemberWnd)

public:
	CKfzEditMemberWnd(void);
	~CKfzEditMemberWnd(void);

public:
	virtual void Draw(void);
	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnWheel(int iWheel);	

protected:
	CCtrlGrid *m_pGrid_Left,*m_pGrid_Right;//列表控件
	CCtrlButton * m_pAdd,*m_pDel; //添加,删除按钮
	CCtrlButton * m_pConfirm,*m_pCancel; //确定,取消按钮


	vector<_KfzMember> m_VMemberList;//行会成员列表


	void OnGetMemberList();//请求的行会成员列表来了
	void OnGetNewMemberList();//请的行会成员列表来了
	void OnAddMember();//添加成员
	void OnDelMember();//删除成员
	void OnOk();//确定
};