#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlLabel.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TaskData.h"


// 五色日常任务
class CTaskWuSeDailyWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTaskWuSeDailyWnd)

public:
	CTaskWuSeDailyWnd(void);
	~CTaskWuSeDailyWnd(void);

public:

	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

	virtual void OnClickCloseButton();
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);

protected:
	CCtrlButton* m_pBtnFlush;		// 立即刷新
	CCtrlButton* m_pBtnTaskAuto;	// 自动完成

	CCtrlLabel* m_pLabTaskName;

	DWORD m_colors[5];
	CCtrlButton * m_pHole[5];
	POINT m_ptHole[5];

	_WuSeTaskStruct m_task;

	int         m_iEnterTime;

	CMarkViewer * m_pMarkViewerUp;
	CMarkViewer * m_pMarkViewerDown;

	CTagText	m_TagUp;
	CTagText	m_TagDwon;
};
