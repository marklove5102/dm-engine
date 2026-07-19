#pragma once

#include "BaseClass/Control/CtrlWindowX.h"

class CFaBaoShowWnd :
	public CCtrlWindowX
{

	DECLARE_WND_POSX(CFaBaoShowWnd)

public:
	CFaBaoShowWnd(void);
	~CFaBaoShowWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);

	CCtrlButton*  m_pLianHua;				//炼化
	//CCtrlEdit*    m_pEdFaZhenName;			//法阵名字
	//CCtrlButton*  m_pFaZhenShortKey;		//法阵技能快捷键

	static int GetFaBaoEffType(int FaBaoType);
	//static int TianDiRen(int FaBaoType);			//法宝的天地人属性
	int	m_iBlankColor;							//辅法宝格子颜色,0--无颜色 1--左红色 2--左绿色 3--右红色 4--右绿色
	bool m_bHasFaZhen;//因为小客户端资源没有法阵的图,但是小客户端又是在1.28版本上合的,所以程序特殊处理一下,低于1.28版本的小客户端不显示法阵相关信息

	//void RefreshButtonState();
};
