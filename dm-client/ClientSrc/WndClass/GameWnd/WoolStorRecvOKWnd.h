#pragma once

#include "BaseClass\Control\CtrlWindowX.h"
#include "BaseClass\Control\CtrlButton.h"
#include "BaseClass/Control/CtrlMultiEdit.h"

class CWoolStorRecvOKWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CWoolStorRecvOKWnd)
public:
	CWoolStorRecvOKWnd(void);
	~CWoolStorRecvOKWnd(void);

	virtual void Draw(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	//void DrawText2(int iX,int iY);
protected:
	std::string			 m_strRecvWords;	  //留言
	std::string          m_strItemName;       //物品名称
	std::string          m_strOriginUser;     //发送人
	CCtrlButton*         m_pOK;
	CCtrlMultiEdit*      m_pSendText;
	int                  m_iPos;//接收礼物的缓冲位置
};
