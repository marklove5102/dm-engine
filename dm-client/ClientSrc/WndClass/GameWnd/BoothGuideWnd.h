#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include <vector>

using namespace std;


struct S_Booth
{
	string strName;//名字
	string strBoothName;//摊位名称
	DWORD dwID;//人物ID
};


class CBoothGuideWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CBoothGuideWnd)

public:
	CBoothGuideWnd(void);
	~CBoothGuideWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnWheel(int iWheel);	
	virtual bool OnMouseMove(int iX,int iY);


protected:
	CCtrlButton * m_pOpenBtn; //打开摊位
	CCtrlButton * m_pCancelBtn; //关闭摊位


	CCtrlGrid *   m_pBoothGrid;//NPC列表控件
	vector<S_Booth>     m_VBooths;//摊位数据表序
	DWORD   m_dwLastRefrashData;


	void CreateBoothList();
	void OnOpenBooth();



};
