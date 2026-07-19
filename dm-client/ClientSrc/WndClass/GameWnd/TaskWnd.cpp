#include "taskwnd.h"
#include "GameControl/GameControl.h"
#include "GameMap/GameMap.h"
#include "Baseclass/Control/ParserTip.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TaskData.h"
#include "GameData/ItemCfgMgr.h"
#include "GameData/GameData.h"
#include "GameWnd.h"
#include "TaskTreeDesWnd.h"
#include "GameData/MapFinder.h"

#pragma warning(disable:4311)
#define MAX_TASK_ROWS  7
#define MAX_DETAIL_ROWS 9

INIT_WND_POSX(CTaskWnd,POS_AUTO,POS_AUTO)
WORD CTaskWnd::m_wSTOnConstruct = 0;
CTaskWnd::CTaskWnd(void)
{
	m_pTreeCtrl = NULL;
	m_pScrollBar = NULL;
	m_pMKVDes = NULL;
	m_pAbandonButton = NULL;
	m_dwPic			 = 0;
	m_iLastSelTaskTree = -1;
	m_TaskDes.Clear();
	m_iStartPage = 0;
	if (sm_dwWindowType != 0)
		m_wSTOnConstruct = LOWORD(sm_dwWindowType);

	m_pGrid = NULL;
	m_bShowDelButton = false;

	m_iStartPage = HIWORD(sm_dwWindowType);
	if (m_iStartPage == 0) m_iStartPage = -1;
	
	//if (m_iStartPage == 2) 
	//{
	//	if (m_wSTOnConstruct == 1)
	//		m_iSelectedTask = 0xFFFFFFFF;
	//	else if (m_wSTOnConstruct == 2)
	//		m_iSelectedTask = 0xFFFFFFFE;
	//}

	m_iIndex = 17601;

	for (int i = 0;i<8;i++)
	{
		m_paBtn[i] = NULL;
	}
}

CTaskWnd::~CTaskWnd(void)
{
	g_pControl->PopupWindow(MSG_CTRL_TASKTREEDES_WND,OPER_CLOSE);
}

void CTaskWnd::OnCreate()
{
	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_FLASH_BTN,1003);//停止按钮闪烁
	//g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstGetNewTask_PaoPao);

	SwitchToPage(m_iStartPage,true);

	if(!g_pGameData->HasPaoPaoStatus(EP_FirstOpenTaskWnd_PaoPao))
	{
		AddArrowTip(EP_FirstOpenTaskWnd_PaoPao,360,280,XAL_TOPLEFT,false,XAL_TOPRIGHT);
	}
}

void CTaskWnd::Draw(void)
{

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
/*	if (m_iStartPage == 2)
		DrawTaskTree();
	else */if(m_iStartPage == 0)
		DrawTask();
	//  [4/15/2010 dujun]
	else if(m_iStartPage == 1)
		DrawAvaTask();

	//绘制附加信息
	g_pFont->DrawText(m_iScreenX + 282,m_iScreenY + 9,"任 务",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
	if(m_iStartPage == 0)
		g_pFont->DrawText(m_iScreenX + 381,m_iScreenY + 91,"任务详细",0xffe0b060,FONT_YAHEI,FONTSIZE_BIG);

	char ctemp[128] = {0};
	sprintf(ctemp,"已接任务数：%d/%d",g_TaskData.GetRevTaskNum(),g_TaskData.GetMaxTaskNum());
	g_pFont->DrawText(m_iScreenX + 205,m_iScreenY + 35,ctemp,COLOR_TEXT_NORMAL);

	sprintf(ctemp,"已完成任务数：%d",g_TaskData.GetFinishNum());
	g_pFont->DrawText(m_iScreenX + 323,m_iScreenY + 35,ctemp,COLOR_TEXT_NORMAL);
}

void CTaskWnd::DrawAvaTask()
{
	g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 80,"任务名称",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 193,m_iScreenY + 80,"可完成次数",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 300,m_iScreenY + 80,"所需等级",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 455,m_iScreenY + 80,"任务发布者",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);

	if(m_pGrid)
	{
		int iPos = m_pGrid->GetScrollPos();
		int iDrawLines = 15;
		VTaskAvaList& vAvaTask = g_PromptInfoMgr.GetAvaTaskList();
		if(iDrawLines > vAvaTask.size() - iPos)
			iDrawLines = vAvaTask.size() - iPos;
		
		for(int i = iPos; i<iPos + iDrawLines; i++)
		{
			TaskAva& temTask =vAvaTask.at(i);
			
			char tem[64] = {0};
			
			m_pGrid->DrawGrid(i - iPos,0,temTask.strTaskName.c_str(),true,0xff5AA0A0);
			m_pGrid->DrawGrid(i - iPos,1,temTask.strTaskDetail.c_str(),true,0xFFFF8000);
			sprintf(tem,"≥%d级",temTask.wLevel);
			m_pGrid->DrawGrid(i - iPos,2,tem,true,0xff5AA0A0);
			m_pGrid->DrawGrid(i - iPos,3,temTask.strNPC.c_str(),true,0xFF87C23A);
			
		}
	}

	g_pFont->DrawText(m_iScreenX + 365,m_iScreenY + 418,"双击任务，可自动寻路至相关NPC处",0xffffee55,FONT_YAHEI,FONTSIZE_MIDDLE);

}

// void CTaskWnd::DrawTaskTree()
// {
// 	//绘制任务树
// 	VTaskTree& vTaskTree = g_PromptInfoMgr.GetTaskTree();
// 	if (vTaskTree.size() <= 0) return;
// 
// 	int iPos = m_pGridTaskTree->GetScrollPos();
// 	int iDrawLines = 13;
// 	if (iDrawLines > vTaskTree.size() - iPos) iDrawLines = vTaskTree.size() - iPos;
// 
// 	for (int i = iPos;i<iPos + iDrawLines;i++)
// 	{
// 		TaskTree& tr = vTaskTree.at(i);
// 		m_pGridTaskTree->DrawGrid(i - iPos,0,tr.strTheme.c_str());
// 	}
// 
// 	int iCurSeleLine = m_pGridTaskTree->GetSelLine();
// 	if (iCurSeleLine < 0) iCurSeleLine = m_iLastSelTaskTree;
// 	else m_iLastSelTaskTree = iCurSeleLine;
// 
// 	if (iCurSeleLine < 0) return;
// 
// 	if (vTaskTree.at(iCurSeleLine).bSequence)
// 	{
// 		for(int i = 0;i<8;i++)
// 		{
// 			if (i % 2 == 0)
// 			{
// 				if ( i + 2 <= 6)
// 				{
// 					if (m_paBtn[i+2]->IsShow())
// 					{
// 						//draw
// 						LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17673);
// 						int iX = m_paBtn[i]->GetScreenX() + 48;
// 						int iY = m_paBtn[i]->GetScreenY() + 60;
// 
// 						if (pTex) g_pGfx->DrawTextureNL(iX,iY,pTex);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if (m_paBtn[i]->IsShow())
// 				{
// 					//draw
// 					LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17674);
// 					int iX = m_paBtn[i]->GetScreenX() - 24;
// 					int iY = m_paBtn[i]->GetScreenY() + 20;
// 
// 					if (pTex) g_pGfx->DrawTextureNL(iX,iY,pTex);
// 				}
// 			}
// 		}
// 	}
// 
// 	RefreshTaskTree();
// }

// void CTaskWnd::RefreshTaskTree(bool bForced)
// {
// 	VTaskTree& vTaskTree = g_PromptInfoMgr.GetTaskTree();
// 	
// 	if (m_iSelectedTask >= 0 && m_iSelectedTask < vTaskTree.size())
// 	{
// 		int iDrawLines = 4;
// 		TaskTree& tr = vTaskTree.at(m_iSelectedTask);
// 		int iPos = m_pScrollBar->GetPos();
// 		if (m_iLastPos == iPos && !bForced)
// 			return;
// 
// 		m_iLastPos = iPos;
// 
// 		for (int i = 0;i<8;i++)
// 		{
// 			(m_paBtn[i])->SetShow(false);
// 		}
// 
// 		if (iDrawLines > tr.vTask.size() - iPos) iDrawLines = tr.vTask.size() - iPos;
// 		for (int i = iPos;i < iDrawLines + iPos;i++)
// 		{
// 			TaskTreeNode& tn = tr.vTask.at(i);
// 			
// 			if (!tn.strSimDes.empty())
// 			{ 
// 				(m_paBtn[(i - iPos) * 2])->SetEnable(true);
// 				(m_paBtn[(i - iPos) * 2])->SetShow(true);
// 				(m_paBtn[(i - iPos) * 2])->SetText(tn.strSimDes.c_str(),COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
// 			}
// 
// 			if (!tn.strSubSimDes.empty())
// 			{
// 				(m_paBtn[(i - iPos) * 2 + 1])->SetEnable(true);
// 				(m_paBtn[(i - iPos) * 2 + 1])->SetShow(true);
// 				(m_paBtn[(i - iPos) * 2 + 1])->SetText(tn.strSubSimDes.c_str(),COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
// 			}
// 		}	
// 	}
// }

void CTaskWnd::DrawTask()
{
	//绘制任务	
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,LOWORD(m_dwPic),EP_UI);
	if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 245,m_iScreenY +111,pTex);

	//  [4/23/2010 dujun]
	m_pAbandonButton->SetShow(m_bShowDelButton);
	m_pAbandonButton->SetEnable(m_bShowDelButton);
}

void CTaskWnd::DealSelectedUpdate(CTreeViewNode* pTreeNode)
{
	if (!pTreeNode) return;

	_Task* pTask = GetTask(pTreeNode);
	if (pTask)
	{
		pTreeNode->SetChecked(pTask->bTrack == TRUE?true:false);
		m_TaskDes.Clear();
		m_wSTOnConstruct = pTask->wTaskID;
		m_TaskDes.Parse(pTask->strTaskDesc);
		m_dwPic = pTask->dwTaskPic;
		m_pMKVDes->SetTagText(&m_TaskDes);
		//如果是支线任务  [4/23/2010 dujun]
		if(!pTask->bMain)
			m_bShowDelButton = true;
	}
}

_Task* CTaskWnd::GetTask(CTreeViewNode* pTreeNode)
{
	if (!pTreeNode || !pTreeNode->IsLeaf()) return NULL;

	WORD wSmItemID = (WORD)pTreeNode->GetID();
	WORD wBigItemID = (WORD)pTreeNode->GetParent()->GetID();
	_Task* pTask = g_TaskData.FindTask(wBigItemID,wSmItemID,FALSE);
	if (!pTask) pTask = g_TaskData.FindTask(wBigItemID,wSmItemID,TRUE);

	return pTask;
}

void CTaskWnd::DealAbandonTask()
{
	if (m_iStartPage != 0/*2*/) return;

	char tempMsg[128] = {0};
	CTreeViewNode* pTreeNode = m_pTreeCtrl->GetSelectedNode();
	_Task* pTask = GetTask(pTreeNode);
	if (pTask && !pTask->bMain)
	{
		sprintf(tempMsg,"您真的要放弃“%s”任务吗？",pTask->strTaskName.c_str());

		g_TaskData.SetAbandonTaskID(pTask->wTaskID);
		g_MsgBoxMgr.PopSimpleComfirm(tempMsg,MSG_CTRL_ABANDON_TASK,0);
	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("请先选中你要删除的支线任务！");
	}

}

bool CTaskWnd::IsShowAband()
{
	if(m_iStartPage == 0)
	{
		CTreeViewNode* tem = m_pTreeCtrl->GetSelectedNode();
		if(tem)
		{
			_Task* pTask = GetTask(tem);
			if (pTask)
			{
				if(!pTask->bMain)
					return true;
			}
		}	
	}

	return false;
}

bool CTaskWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_UPDATE_WND:
		{
			DealSelectedUpdate((CTreeViewNode*)pControl);
			return true;
		}
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pAbandonButton)
		{
			DealAbandonTask();
			return true;
		}
		else if (pControl == m_pBtnAllTsk)
		{
			if (m_iStartPage != 0) SwitchToPage(0);
			return true;
		}
		else if(pControl == m_pBtnAvaTsk)
		{
			if (m_iStartPage != 1) SwitchToPage(1);
			return true;
		}
		/*else if (pControl == m_pThemeTsk)
		{
			if (m_iStartPage != 1) SwitchToPage(1);
			return true;
		}
		else if (pControl == m_pBranchTsk)
		{
			if (m_iStartPage != 2) SwitchToPage(2);
			return true;
		}*/
		//else if (pControl == m_pTreeTsk)
		//{
		//	if (m_iStartPage != 2) SwitchToPage(2);
		//	return true;
		//}
		else
		{
			for (int i = 0;i<8;i++)
			{
				if(pControl == m_paBtn[i] && pControl->IsShow())
				{
					int iRow = i / 2;
					bool bSub = (i - iRow * 2 != 0);
					int iPos = m_pScrollBar->GetPos();
					iPos += iRow;
					VTaskTree& vTaskTree = g_PromptInfoMgr.GetTaskTree();
					if (m_iSelectedTask >= 0 && m_iSelectedTask < vTaskTree.size())
					{
						TaskTree& tr = vTaskTree.at(m_iSelectedTask);
						
						if (iPos >= 0 && iPos < tr.vTask.size())
						{
							TaskTreeNode& tn = tr.vTask.at(iPos);
							string str = tn.strDes;
							if (bSub) str = tn.strSubDes;

							g_pControl->Msg(MSG_CTRL_TASKTREEDES_WND,OPER_CREATE);
							int iX,iY;
							g_pInput->GetMousePos(iX,iY);							
							CTaskTreeDesWnd* pWnd = (CTaskTreeDesWnd*)(g_pControl->FindWindowByName("TaskTreeDesWnd"));
							
							if (pWnd) 
							{
								pWnd->SetDesText(str);
								pWnd->Move(iX,iY);
							}
						}
					}
					break;
				}
			}
		}
		break;
	case MSG_CTRL_TASK_WND:
		{//Data:高位为任务主ID，低位为任务次ID，pControl为主线支线(2为主线，3为支线增加或删除任务)
			//  [4/23/2010 dujun]
			m_bShowDelButton = IsShowAband();
			
			if (dwData == 0xFFFFFFFF)
			{
				DealSelectedUpdate((CTreeViewNode*)pControl);
			}
			else if (dwData == 0xFFFFFFFE)
			{
				if(!g_pControl->Msg(MSG_CTRL_UPDATE_TRACK_INFO,MAKELONG(0,5),pControl)) 
					return true;

				CTreeViewNode* pTreeNode = (CTreeViewNode*)pControl;
				if (!pTreeNode) break;

				pTreeNode->OnTracked();
				WORD wThemeID = HIWORD(pTreeNode->GetID());
				WORD wTaskID  = LOWORD(pTreeNode->GetID());

				_Task* pTask = g_TaskData.FindTask(wThemeID,wTaskID,true);
				if (!pTask)	pTask = g_TaskData.FindTask(wThemeID,wTaskID,false);

				if (!pTask) break;
				pTask->bTrack = pTreeNode->IsChecked();
				if (pTreeNode->IsChecked())
				{
					DealSelectedUpdate((CTreeViewNode*)pControl);
				}

				DWORD TaskData = pTask->bTrack?MAKELONG(pTask->wTaskID,0):MAKELONG(pTask->wTaskID,1);
				g_pControl->Msg(MSG_CTRL_UPDATE_TRACK_INFO,TaskData);
			}
			else if (dwData == 0xFFFFFFFD)
			{
				if (m_iCurPage == 0 || m_iCurPage == 1)
				{
					//增加追踪标志
					CTreeViewNode* pTreeNode = m_pTreeCtrl->GetRootNode();
					if (pTreeNode)
					{
						DWORD dwID = (DWORD)pControl;
						CTreeViewNode* pNode = pTreeNode->FindNode(dwID);
						if (pNode) pNode->SetChecked(true);
					}
				}
			}
			else
			{
				if(m_pTreeCtrl)//  [4/26/2010 dujun]
				{
					CTreeViewNode* pTreeNode = m_pTreeCtrl->GetRootNode();
					if(pTreeNode) DealSelectedUpdate(pTreeNode->FindNode(dwData));	
				}
				
				int iPage = reinterpret_cast<int>(pControl) - 1;
				if (iPage == 1 || iPage == 2) SwitchToPage(0,true);//不管是主线还是支线,现在都在所有任务里面显示
			}
		}
		break;
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			if(pControl != m_pGrid) //  [4/19/2010 dujun]
			{
				VTaskTree& vTaskTree = g_PromptInfoMgr.GetTaskTree();
				if (m_pGridTaskTree == pControl)
				{
					m_iSelectedTask = LOWORD(dwData);
					m_wSTOnConstruct = m_iSelectedTask;
					if (m_iSelectedTask < vTaskTree.size())
					{
						TaskTree& tr = vTaskTree.at(m_iSelectedTask);
						
						m_pScrollBar->SetRange(tr.vTask.size());
						m_pScrollBar->SetPos(0);
						//RefreshTaskTree(true);
					}				 
				}
				else 
				{
					m_iSelectDetail = LOWORD(dwData);
				}

			}
			
		}
		break;
	//  [4/19/2010 dujun]双击列就自动寻路过去
	case MSG_CTRL_GRID_DBCLICK:
		{
			if(pControl == m_pGrid)
			{
				string strCommand1;
				strCommand1.clear();
				VTaskAvaList& vTask = g_PromptInfoMgr.GetAvaTaskList();
				m_iSelectedAvaTask = LOWORD(dwData);

				if (m_iSelectedAvaTask < vTask.size())
				{
					TaskAva& temp = vTask.at(m_iSelectedAvaTask);
					strCommand1 = temp.strPath;
					m_bClick = false;
					g_pGameControl->DealGotoCommand(strCommand1);	
				}		
			}

		}
		break;
	default:
		break;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CTaskWnd::OnLeftButtonDown(int iX, int iY)
{
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CTaskWnd::OnLeftButtonUp(int iX, int iY)
{
	string strCommand;
	if (m_pMKVDes)
		strCommand = m_pMKVDes->GetCommand();

	/*if(strCommand.empty() && m_pMVNPC)
		strCommand = m_pMVNPC->GetCommand();*/

	//if (strCommand.empty() && m_pMVNPC) strCommand = m_pMVNPC->GetCommand();

	else strCommand.clear();

	m_bClick = false;//否则后面弹出的模式框获得不到焦点
	bool bRet = g_pGameControl->DealGotoCommand(strCommand);	
	if (bRet) return true;

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CTaskWnd::OnLeftButtonDClick(int iX, int iY)
{
	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

bool CTaskWnd::OnMouseMove(int iX,int iY)
{
	if(m_pTreeCtrl)	m_pTreeCtrl->SetMouseXY(iX + m_iScreenX,iY + m_iScreenY);		
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CTaskWnd::OnWheel(int iWheel)
{
	int iMouseX, iMouseY;
	g_pControl->GetMouseXY(iMouseX, iMouseY);

	if (iMouseX < m_iScreenX + 230)
	{
		if(m_pTreeCtrl)
			return m_pTreeCtrl->OnWheel(iWheel);
	}
	else
	{
		if (m_pScrollBar && m_iStartPage != 1)//  [4/26/2010 dujun]
			return m_pScrollBar->OnWheel(iWheel);

		if (m_pMKVDes)
			return m_pMKVDes->OnWheel(iWheel);
	}

	return CCtrlWindowX::OnWheel(iWheel);
}

void CTaskWnd::SwitchToPage(int iPage,bool bForced)
{
	g_pControl->PopupWindow(MSG_CTRL_TASKTREEDES_WND,OPER_CLOSE);

	if(iPage == m_iStartPage && !bForced) return; 
	m_iStartPage = iPage;

	if (m_iStartPage < 0 || m_iStartPage > 2/*3*/)
		m_iStartPage = 0;

	RemoveBodyChildControl();

	m_pTreeCtrl = NULL;
	m_pMKVDes = NULL;
	m_pBtnAllTsk = NULL;
	m_pBtnAvaTsk = NULL;

	m_pGrid = NULL;
	m_pScrollBar = NULL;

	//m_pTreeTsk = NULL;
	m_pAbandonButton = NULL;
	m_TaskDes.Clear();
	m_dwPic = 0;	

	//创建共用按钮
	SetCloseButton(584,3,80);

	m_pBtnAllTsk = new CCtrlButton;
	if(m_iStartPage != 0)
		m_pBtnAllTsk->CreateEx(this,22,55,17616,17617,17618);
	else m_pBtnAllTsk->CreateEx(this,22,55,17618,17617,17616);
	m_pBtnAllTsk->SetText("所有",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
	AddControl(m_pBtnAllTsk);

	//  [4/14/2010 dujun]
	m_pBtnAvaTsk = new CCtrlButton;
	if(m_iStartPage != 1)
		m_pBtnAvaTsk->CreateEx(this,73,55,17616,17617,17618);
	else
		m_pBtnAvaTsk->CreateEx(this,73,55,17618,17617,17616);
	m_pBtnAvaTsk->SetText("可接任务",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
	AddControl(m_pBtnAvaTsk);

	//m_pTreeTsk = new CCtrlButton;
	//if(m_iStartPage != 2/*3*/)
	//	m_pTreeTsk->CreateEx(this,126,55,17616,17617,17618);
	//else m_pTreeTsk->CreateEx(this,126,55,17618,17617,17616);
	//m_pTreeTsk->SetText("任务树",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);
	//AddControl(m_pTreeTsk);


	if (m_iStartPage == 0 /*|| m_iStartPage == 1 || m_iStartPage == 2*/)
	{
		m_pMKVDes = new CMarkViewer(16,10,0,2,true);
		AddControl(m_pMKVDes);
		m_pMKVDes->Create(this,249,268,336,168);
		m_pMKVDes->SetTagText(&m_TaskDes);

		//MarkTask();
		MarkTask(g_TaskData.GetMainTaskList());
		MarkTask(g_TaskData.GetBranchTaskList());

		ConstructTaskTree(m_iStartPage);

		if (m_iStartPage == 0 /*2*/)
		{
			m_pAbandonButton = new CCtrlButton;
			m_pAbandonButton->CreateEx(this,74,420,90,91,92);
			AddControl(m_pAbandonButton);
			m_pAbandonButton->SetText("放弃任务",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

			m_bShowDelButton = IsShowAband();
			m_pAbandonButton->SetEnable(m_bShowDelButton);
			m_pAbandonButton->SetShow(m_bShowDelButton);

			/*m_bShowDelButton = false;
			m_pAbandonButton->SetShow(m_bShowDelButton);
			m_pAbandonButton->SetEnable(m_bShowDelButton);*/
		}

		m_iIndex = 17601;
		SetBackTexByID(0xFFFFFFFF,MAKELONG(m_iIndex,PACKAGE_INTERFACE));

	}

	//可接任务
	else if(m_iStartPage == 1)
	{
		g_PromptInfoMgr.UpdateAvaTask(SELF.GetLevel());
		VTaskAvaList& taskList = g_PromptInfoMgr.GetAvaTaskList();
	
		m_iLastPos = -1;
		m_pGrid = new CCtrlGrid;
		//m_pGrid->Create(this,20,89,602,450,15,20);
		m_pGrid->CreateEx(this,20,89,602,415,15,20,17637);
		AddControl(m_pGrid);
		m_pGrid->SetTextColor(0xffb48c5a,0xffffe650);
		//m_pGrid->SetSelTextColor(0xff87c23a);
		m_pGrid->SetTotalCount(g_PromptInfoMgr.GetAvaTaskList().size());
		m_pGrid->SetFont(FONT_YAHEI);

		m_pGrid->PushColumn(150);
		m_pGrid->PushColumn(100);
		m_pGrid->PushColumn(100);
		m_pGrid->PushColumn(220);

		if(m_iSelectedAvaTask<0)
			m_iSelectedAvaTask = 0;
		m_pGrid->SetSelLine(m_iSelectedAvaTask);

		m_iIndex = 21640;
		SetBackTexByID(0xFFFFFFFF,MAKELONG(m_iIndex,PACKAGE_INTERFACE));
	}
// 
// 	else if (m_iStartPage == 2)
// 	{
// 		m_iLastPos = -1;
// 		m_pGridTaskTree = new CCtrlGrid;
// 		m_pGridTaskTree->CreateEx(this,20,83,231,407,15,20,17665);
// 		AddControl(m_pGridTaskTree);
// 		m_pGridTaskTree->SetDrawOffXY(0,5);
// 		m_pGridTaskTree->PushColumn(220);
// 
// 		VTaskTree& vTaskTree = g_PromptInfoMgr.GetTaskTree();
// 		m_pGridTaskTree->SetTotalCount(vTaskTree.size());
// 
// 		string str = "";
// 		if (m_iSelectedTask == 0xFFFFFFFF)
// 		{
// 			str = "军机令-活动任务";
// 			//m_iSelectDetail = 52;
// 		}
// 		else if (m_iSelectedTask == 0xFFFFFFFE)
// 		{			
// 			str = "长是人千里-活动任务";
// 			//m_iSelectDetail = 60;
// 		}
// 		
// 		int iSelLine = g_PromptInfoMgr.FindTaskTree(str);
// 
// 		if (iSelLine < 0) iSelLine = m_iSelectedTask;
// 		if (iSelLine < 0) iSelLine = m_iSelectedTask = 0;
// 		m_pGridTaskTree->SetSelLine(iSelLine);
// 
// 		m_pScrollBar = new CCtrlScroll;
// 		m_pScrollBar->CreateEx(this,572,110,16,324);
// 		AddControl(m_pScrollBar);
// 		m_pScrollBar->SetPos(0);
// 
// 		for (int i = 0;i < 4;i++)
// 		{
// 			m_paBtn[i * 2 ] = new CCtrlButton;
// 			(m_paBtn[i * 2])->CreateEx(this,280,120 + i * 85,17670,17671,17672);
// 			AddControl(m_paBtn[i * 2]);
// 			(m_paBtn[i * 2])->SetShow(false);
// 
// 			m_paBtn[i * 2 + 1] = new CCtrlButton;
// 			(m_paBtn[i * 2 + 1])->CreateEx(this,427,120 + i * 85,17670,17671,17672);
// 			AddControl(m_paBtn[i * 2 + 1]);
// 			(m_paBtn[i * 2 + 1])->SetShow(false);
// 		}
// 
// 		if (vTaskTree.size() > 0)
// 		{
// 			m_iSelectedTask = iSelLine;
// 			m_iSelectDetail = 0;
// 
// 			OnSelectTaskTree();
// 			RefreshTaskTree(true);
// 		}
// 		else
// 		{
// 			m_iSelectedTask = -1;
// 			m_iSelectDetail = -1;
// 		}
// 
// 		m_iIndex = 17600;
// 		SetBackTexByID(0xFFFFFFFF,MAKELONG(m_iIndex,PACKAGE_INTERFACE));
// 	}
}

//void CTaskWnd::OnSelectTaskTree()
//{
//	VTaskTree& vTaskTree = g_PromptInfoMgr.GetTaskTree();
//
//	if (m_iSelectedTask < vTaskTree.size())
//	{
//		TaskTree& tr = vTaskTree.at(m_iSelectedTask);
//		m_pScrollBar->SetRange(tr.vTask.size());
//	}
//}

void CTaskWnd::UpdateTask(_Task* pTask)
{
	if (!pTask) return;

	m_TaskDes.Clear();
	m_wSTOnConstruct = pTask->wTaskID;
	m_TaskDes.Parse(pTask->strTaskDesc);
}

void CTaskWnd::BuildTree(CTreeViewNode* pRoot,MTaskList& TaskList)
{
	if (!pRoot) return;

	MTaskList::iterator it = TaskList.begin();
	
	if (it != TaskList.end() && m_wSTOnConstruct == 0)
	{
		_ThemeTask& ThemeTask = it->second;
		VTask::iterator itTask = ThemeTask.vTask.begin();

		if (itTask != ThemeTask.vTask.end())
			m_wSTOnConstruct = itTask->wTaskID;
	}

	while (it != TaskList.end())
	{
		_ThemeTask& ThemeTask = it->second;
		if (ThemeTask.wThemeID != 0)
		{
								
			CTreeViewNode* pNode = new CTreeViewNode(pRoot,m_pTreeCtrl,ThemeTask.strTaskTile.c_str(),ThemeTask.wThemeID,17654,17658);
			pNode->SetUnBind();

			VTask::iterator itTask = ThemeTask.vTask.begin();

			//char buff[128] = {0};

			while (itTask != ThemeTask.vTask.end())
			{
				DWORD dwID = MAKELONG(itTask->wTaskID,ThemeTask.wThemeID);
				CTreeViewNode* pNodeLeaf = new CTreeViewNode(pNode,m_pTreeCtrl,itTask->strTaskName.c_str(),dwID,17662,17663,true,false,false);
				if (itTask->bTrack) 
					pNodeLeaf->SetChecked(true);

				pNodeLeaf->GetButton()->SetTips("选中后追踪");

				pNode->InsertChildNode(pNodeLeaf);
				if (m_wSTOnConstruct == itTask->wTaskID)
				{
					pNodeLeaf->SetSelected(true);
					m_pTreeCtrl->SetSelectedNode(pNodeLeaf);
					DealSelectedUpdate(pNodeLeaf);
				}

				itTask++;
			}

			pRoot->InsertChildNode(pNode);
		}
		it++;
	}
}

void CTaskWnd::MarkTask(MTaskList& mList)
{
	if (mList.size() > 0)
	{
		MTaskList::iterator it = mList.begin();
		while (it != mList.end())
		{
			_ThemeTask& ThemeTask = it->second;
			if (ThemeTask.wThemeID != 0)
			{
				VTask::iterator itTask = ThemeTask.vTask.begin();

				char buff[128] = {0};
				while (itTask != ThemeTask.vTask.end())
				{
					//标记是主线还是支线
					//  [4/23/2010 dujun]
					if(itTask->bMain)
					{
						if((itTask->strTaskName.find("(主)",0)) == string::npos)
						{
							sprintf(buff,"(主)%s",itTask->strTaskName.c_str());
							itTask->strTaskName = buff;
						}
					}
					else
					{
						if((itTask->strTaskName.find("(支)",0)) == string::npos)
						{
							sprintf(buff,"(支)%s",itTask->strTaskName.c_str());
							itTask->strTaskName = buff;
						}
					}

					itTask++;
				}
			}
			it++;
		}
	}
}

void CTaskWnd::ConstructTaskTree(int iCurPage)
{
	m_pTreeCtrl = new CCtrlTreeView();
	m_pTreeCtrl->Create(this,20,83,231,407,15);
	m_pTreeCtrl->SetSelectedBar(17665);
	AddControl(m_pTreeCtrl);

	CTreeViewNode* pRootNode = new CTreeViewNode(NULL,m_pTreeCtrl,NULL,0,0,0,false);
	pRootNode->SetUnBind();
	m_pTreeCtrl->SetRootNode(pRootNode);

	bool bFind = (g_TaskData.FindTask(m_wSTOnConstruct,true) != NULL);//(g_TaskData.FindThemeTask(m_wSTOnConstruct,true) != NULL);
	if (!bFind) bFind = (g_TaskData.FindTask(m_wSTOnConstruct,false) != NULL);

	if (!bFind) m_wSTOnConstruct = 0;

	if (iCurPage == 0)
	{//全部
		MTaskList& TaskListMain = g_TaskData.GetMainTaskList();
		if (TaskListMain.size() > 0)
			BuildTree(pRootNode,TaskListMain);

		MTaskList& TaskListBranch = g_TaskData.GetBranchTaskList();
		if (TaskListBranch.size() > 0)
			BuildTree(pRootNode,TaskListBranch);

	}

	m_pTreeCtrl->RefreshControl();
}