#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/NpcData.h"


class CGemCondenseWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CGemCondenseWnd)

public:
	CGemCondenseWnd(void);
	~CGemCondenseWnd(void);
public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

	int  GetType() {return m_iType;}
	void SetFuShi(bool b){m_bIsFuShi = b;}
	void BackToPackage();
private:
	bool    IsYuanShiSameType(CGood& tmp);
	int     GetGemIndex(int iX,int iY);
	RECT    m_rcGem[MAX_GEM_COUNT+1];

	CCtrlButton*  m_pStartButton; //开始凝练按钮
	int        m_iType;    //类型
	bool       m_bIsCondensing; //正在凝练
	bool	   m_bIsFuShi;		//符石锻造超级武器
	int        m_iMouseOnGem;   //鼠标在此上面的索引
	int        m_iMouseType;    //鼠标在此上面的类型
	int        m_iResult;		//灵犀宝盒强化结果，1成功2失败
};

