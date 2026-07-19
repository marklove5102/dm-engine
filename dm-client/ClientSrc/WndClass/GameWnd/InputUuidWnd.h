#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"

//#include "WndClass/LoginWnd/SmltKeyBoard.h"

class CInputUuidWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CInputUuidWnd)

public:
	CInputUuidWnd(void);
	~CInputUuidWnd(void);
public:

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();

	virtual void OnCreate(void);
	void GetEditText(char * sEditText);
	bool SetFocus(void);
	void SendDPWD();

	void DrawText(void);
	void ShowText(const char * sMsg);
	virtual bool OnLeftButtonDown(int iX,int iY);

protected:
    int m_iType;		//0为秘宝验证，1为安全卡
	int m_iOpenPlace;   //0:登录过程弹出的密保输入框,1:游戏中gs要求弹出的      

	DWORD	m_dwMsg;
	DWORD	m_dwData;

	CCtrlButton *	 m_aDigitalBtn[10];
	CCtrlButton *    m_pOKButton;
	CCtrlButton *    m_pCancelButton;
	CCtrlEdit *		 m_pEdit;
	CCtrlEdit *		 m_pEdit1;
	CCtrlEdit *		 m_pEdit2;
	CCtrlEdit *		 m_pCurEdit;		//焦点编辑框
	int				 m_aiDitital[10];
	char*			 m_sMsg;
	int				 m_iLineLen;
	int				 m_aiRowNum[3];
	int				 m_aiColNum[3];
	char			 m_PosArray[3][8];	
	int				 m_iFocusPos;		//焦点编辑框的位置

	//bool			 m_bSmallMatrix;		//大小安全卡的标志
};
