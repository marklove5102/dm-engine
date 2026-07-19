#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "Global/StdHeaders.h"

#define USE_OBJECT_LINE_COUNT 9


class CUseObjectWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CUseObjectWnd)

public:
	CUseObjectWnd(void);
	~CUseObjectWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

	void SetLines(const char * pChar);

protected:

	CCtrlButton *  m_pSubmitBtn; //确认按钮
	CCtrlEdit *	   m_pEdit;	
	CCtrlGrid *    m_pGrid;//列表控件

	int            m_iObjectType;//使用或设置那种物品:1:飞来神石,2:师徒神镯,3：结义圣链，4:回城神石、回城神石(大) 5:新手怪物召唤卡
	DWORD          m_dwObjID;//物品ID
	vector<string> m_VLine;//目标名称列表
	int			   m_iMonsterNum;	//记录在Player所在地图的monster个数

	void DrawMsg();//绘制说明文字
	void DrawLineData();//绘制列表框内容
	void CreateFLSSList();//飞来神石目标名列表
	void CreateSTSZList();//师徒神镯目标名列表		
	void CreateJYXLList();//结义项链目标名列表
	void CreateHCSSList();//回城神石目标名列表
	void CreateList(DWORD dwRelation);//根据关系创建名称列表
	void OnSubmit();//点了确认按钮	
};

