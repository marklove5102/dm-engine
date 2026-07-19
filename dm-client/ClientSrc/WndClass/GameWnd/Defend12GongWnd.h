#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "Global/MathUtil.h"
#include "GameMap/MinMap.h"
#include "GameMap/GameMap.h"
#include "GameData/OtherData.h"
#include <string>
#include <vector>


#define D12BING_LINE_COUNT 5

class CDefend12GongWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CDefend12GongWnd)

public:
	CDefend12GongWnd(void);
	~CDefend12GongWnd(void);
public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnDraw();
	virtual void OnCreate();
	virtual bool OnWheel(int iWheel);
	virtual bool IsInControl(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickRow();

private:
	void DrawBingLineData();
	void DrawBingTips();
	void UpdateBings();


	CCtrlButton * m_pClose2;	//关闭

	CCtrlGrid *   m_pBingGrid;	//兵种列表控件

	CCtrlScroll * m_pLeftScroll;

	CCtrlButton * m_pBtnAdd[4];

	int iFrameCount;
	
};
