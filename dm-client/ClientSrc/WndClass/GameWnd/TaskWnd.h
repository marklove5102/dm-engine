#pragma once


#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/TagText.h"
#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlTreeView.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "GameData/TaskData.h"
class CTaskWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTaskWnd)

public:
	CTaskWnd(void);
	~CTaskWnd(void);
public:

	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnWheel(int iWheel);
protected:
	CCtrlTreeView*	m_pTreeCtrl;			//树控件

	CMarkViewer*	m_pMKVDes;				//背景介绍	

	CCtrlButton*	m_pBtnAllTsk;			//所有

	CCtrlButton*	m_pBtnAvaTsk;//可接任务
	CCtrlGrid*		m_pGrid;//可接任务列表
	/*CMarkViewer*	m_pMVNPC;
	CTagText		m_TagNPC;*/
	
	//CCtrlButton*	m_pBranchTsk;			//支线
	//CCtrlButton*	m_pTreeTsk;				//任务树

	CCtrlButton*	m_paBtn[8];				//按钮
	
	CCtrlButton*    m_pAbandonButton;       //放弃任务

	CTagText        m_TaskDes;              //任务描述
	CCtrlScroll*	m_pScrollBar;

	DWORD			m_dwPic;
	int             m_iStartPage;           //窗口打开时显示的那个页签，/0:主线任务，1：支线任务:2是任务树
	CCtrlGrid*		m_pGridTaskTree;
	int				m_iLastPos;
	int				m_iSelectedTask;
	int				m_iSelectDetail;
	int				m_iLastSelTaskTree;

	int				m_iSelectedAvaTask; //  [4/19/2010 dujun]
	bool			m_bShowDelButton;//只有支线任务才能被删除 [4/23/2010 dujun]

	static WORD		m_wSTOnConstruct;
protected:
	void ConstructTaskTree(int iCurPage);   //0是全部，1是主线，2是支线，3是任务树
	void SwitchToPage(int iPage,bool bForced = false);           //页签切换
	void UpdateTask(_Task* pTask);
	//void DrawTaskTree();
	void DrawTask();
	void DrawAvaTask();// 绘制可接任务 [4/15/2010 dujun]

	//void OnSelectTaskTree();
	//void RefreshTaskTree(bool bForced = false);

	_Task* GetTask(CTreeViewNode* pTreeNode);
	void DealAbandonTask();
	void DealSelectedUpdate(CTreeViewNode* pTreeNode);
	void BuildTree(CTreeViewNode* pRoot,MTaskList& TaskList);

	bool IsShowAband();//检查当前选中的时候是支线任务
	void MarkTask(MTaskList& mList);//加上主/支标记
};
