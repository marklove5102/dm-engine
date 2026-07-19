#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMenuButton.h"

enum E_VIP_SHOWPAGE
{
	EVS_BUY = 0,           //购买vip会员页签
	EVS_WATCH_PRIVILEGE,   //查看会面特权
	EVS_VIPMEMBER_PAGE,    //会员首页
	EVS_UPGRADE_EXTERN,    //会员升级继费
};

class CVipCardWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVipCardWnd)

public:
	CVipCardWnd(void);
	~CVipCardWnd(void);

public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual void OnMove();

protected:
	CCtrlButton*    m_pBuyBtn,*m_pUpgradeBtn,*m_pExternBtn;
	CCtrlButton*    m_pBuyTabBtn,*m_pWatchPrivilegeTabBtn,*m_pMemberTabBtn,*m_pUpgradeExternTabBtn;
	CCtrlRadio*     m_pAutoExtern;//自动续费
	CCtrlRadio*     m_pTiyan,*m_pMonth,*m_pQuater,*m_pYear;




	int m_iLevelExp[7];
	string m_strVipTypToName[5];
	int m_iShowPage;
	bool m_bWillOverdue;//是否快过期




	void SetControlState();

};
