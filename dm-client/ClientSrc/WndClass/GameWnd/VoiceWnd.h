#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "BaseClass/Control/TalkViewer.h"

#include "GameClient/VoiceProxy.h"

#ifdef _CHAT

class CVoiceWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVoiceWnd)
public:
	CVoiceWnd(void);
	~CVoiceWnd(void);

	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual void OnCreate();
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();

private:

	CCtrlButton * m_pCreatButton;
	CCtrlButton * m_pEnterButton;
	CCtrlButton * m_pSetupButton;
	CCtrlButton * m_pSearchButton;
	CCtrlButton * m_pReturnButton;
	//CCtrlEdit   * m_pCharID;
	CCtrlGrid   * m_pGrid;

	DWORD m_dwClickReturnButtonTime;
	int m_iType;
};

class CVoiceChatWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVoiceChatWnd)
public:
	CVoiceChatWnd(void);
	~CVoiceChatWnd(void);

	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

	void ClearEditContent();



protected:
	CCtrlButton * m_pLeaveButton;//离开按钮
	CCtrlButton * m_pForbidButton;//禁言按钮
	CCtrlButton * m_pConfigButton;//设置拉按钮
	CCtrlButton * m_pSendButton;//发送按钮
	CCtrlButton * m_pAddVoiceButton;//2套声音控制按钮
	CCtrlButton * m_pDecVoiceButton;
	CCtrlButton * m_pAddMicButton;
	CCtrlButton * m_pDecMicButton;

	CCtrlEdit   * m_pSendEdit;//发送信息
	CCtrlGrid   * m_pGrid;
	CTalkViewer * m_pTalkViewer;

	void	OnRightBtnClick(int iRow);
	void	OnChangePasword();
	void    OnKickOut();
	void	OnForbidOne();
	void	OnAllowOne();
	void	OnReceiveOne();
	void	OnRejectOne();
	void	OnReceiveAll();
	void	OnRejectAll();
	void	OnForbidAll();
	void	OnAllowAll();
	void	OnAllowOneHearMe();
	void	OnRejectOneHearMe();
};


class CVoiceCfgWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVoiceCfgWnd)
public:
	CVoiceCfgWnd(void);
	~CVoiceCfgWnd(void);

	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);

protected:
	CCtrlButton  * m_pConfirmButton;
	CCtrlButton  * m_pCancelButton;
	CCtrlRadio	 * m_pButtonActivate;
	CCtrlRadio	 * m_pVoiceActivate;
	CCtrlRadio	 * m_pMicEnhance;
	int m_ArrowPosition;
};


class CVoiceAddWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVoiceAddWnd)
public:
	CVoiceAddWnd(void);
	~CVoiceAddWnd(void);

	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);


protected:
	CCtrlButton * m_pConfirmButton;//确认按钮
	CCtrlButton * m_pCancelButton;//取消按钮
	CCtrlMenuButton * m_pGetDownButton;//下拉按钮
	CCtrlEdit * m_pEditName; //频道名称                 
	CCtrlEdit * m_pEditPassword;//设置秘密
	CCtrlEdit * m_pEditConfirmPassword; //确认秘密                    
	int m_iType;
};

class CVoiceSearchWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVoiceSearchWnd)
public:
	CVoiceSearchWnd(void);
	~CVoiceSearchWnd(void);

	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw();


protected:
	CCtrlButton * m_pSearchButton;

	CCtrlMenuButton    * m_pNameDownButton;
	CCtrlMenuButton	   * m_pAreaDownButton;
	CCtrlMenuButton    * m_pGroupDownButton;

	void  ChangeAreaButtonStatus(int flag = 0);
	void  ChangeGroupButtonStatus(int flag = 0);
};

class CVoicePasswordWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CVoicePasswordWnd)
public:
	CVoicePasswordWnd(void);
	~CVoicePasswordWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw();
	void   SetType(int iType){m_iType = iType;}
protected:
	CCtrlButton * m_pConfirmButton;
	CCtrlButton * m_pCancelButton;
	CCtrlEdit	* m_pPasswordEdit;
	int m_nRoomNum;
	int m_iType;
};

#endif

