#pragma once
#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlWindowX.h"

class CNpcWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CNpcWnd)
public:
	CNpcWnd(void);
	~CNpcWnd(void);
    virtual void OnCreate();
    virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnRightButtonDown(int iX,int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnClickCloseButton();
	virtual void Draw(void);
	virtual bool OnWheel(int iWheel);

public:
	static bool  m_sbHeaderNpc;//是否带头像的NPC
private:
	void MoveChildWindow();

    CMarkViewer* m_pMarkViewer;
	CCtrlButton * m_pRainbowButton;//彩虹帮助按钮

	int     m_iChildX,m_iChildY;
	VString m_VChildName;
	DWORD m_dwLastClickCmdTime;//上一次点击命令的时间,两次点击之前不能太快,否则服务器可能还没有回应过来又点了一次命令
	int m_iNpcHeadTex;

	DWORD m_dwAppr;//指定头像的npc对话框

};
