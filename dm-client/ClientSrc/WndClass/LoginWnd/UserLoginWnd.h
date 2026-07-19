#pragma once

#include "BaseClass/Control/CtrlWindowS.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "BaseClass/Control/CtrlButton.h"

class CUserLoginWnd : public CCtrlWindowS
{
	DECLARE_WND_POS(CUserLoginWnd)
public:
	CUserLoginWnd(void);
	~CUserLoginWnd(void);

	enum 
	{
		PAGE_PTACCOUNT = 0,
		PAGE_GAMEACCOUNT,
	};

	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual void Draw();

	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);	
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool Move(int iX,int iY);

	bool   SendLoginData(const char* user = NULL,const char* password = NULL);
	bool   SendUserPwd(const char * str);

protected:

	bool m_isVaildShow;

	CCtrlEdit   * m_pEditUserID;            // 用户id
	CCtrlEdit   * m_pEditPassword;	        // 用户密码
	CCtrlButton * m_pSubmit;				// 提交按钮	
	CCtrlButton * m_pCancel;				// 取消按钮	
	CCtrlButton * m_pNewUser;				// 新用户按钮
	CCtrlButton * m_pChangePassword;        // 修改密码按钮
	CCtrlButton * m_pHelp;		// 帮助按钮
	CCtrlButton * m_pBindDynamicPassword;   // 绑定密宝
	CCtrlButton * m_pTakeSafeCard;
	bool		  m_bAdjustWnd;
	bool          m_bPreDownLoadTex;

	void   SetBlankUserEdit();
	void   CaculateKeyBoardWndPos(int& nX, int& nY);
};
