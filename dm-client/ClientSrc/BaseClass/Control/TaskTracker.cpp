#include "TaskTracker.h"
#include "GameData/TaskData.h"
DTI_IMPLEMENT(CTaskTracker, CControlContainer)

CTaskTracker::CTaskTracker(void)
{
}

CTaskTracker::~CTaskTracker(void)
{
	ClearTaskTracker();
}

void CTaskTracker::Draw()
{
	//先绘制一个框
	int iHeigt = GetHeightEx() + 4;
	if (iHeigt > 0)
	{
		g_pGfx->DrawRect(GetScreenX() - 1 + m_iOffX,GetScreenY() - 3,205,iHeigt + 1,0xFFDEAA52);
		g_pGfx->DrawFillRect(GetScreenX() + m_iOffX,GetScreenY() - 2,203,iHeigt - 1,0xC83f2319);
	}
	
	CControlContainer::Draw();
}

bool CTaskTracker::OnLeftButtonDown(int iX,int iY)
{

	return CControlContainer::OnLeftButtonDown(iX,iY);
}

bool CTaskTracker::OnLeftButtonUp(int iX,int iY)
{
	TaskTracker* pTracked = GetClickedTracked(iX,iY);
	if (m_pParent) m_pParent->Msg(MSG_CTRL_UPDATE_TRACK_INFO,MAKELONG(0,3),(CControl*)pTracked);

	return CControlContainer::OnLeftButtonUp(iX,iY);
}

TaskTracker* CTaskTracker::GetClickedTracked(int iX,int iY)
{
	int iRX,iRY,iW,iH;
	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();
	while(it != m_pVTrackViewer.end())
	{	
		iRX = (*it)->TrackView->GetScreenX() - (m_iScreenX + (*it)->TrackView->GetOffX());
		iRY = (*it)->TrackView->GetScreenY() - m_iScreenY;
		iW = (*it)->TrackView->GetWidth();
		iH = (*it)->TrackView->GetHeight();
		
		if (iX >= iRX && iX <= iRX + iW + 30 + 80 &&
			iY >= iRY && iY <= iRY + iH )
		{//选中
			return (*it);
		}

		it++; 
	}

	return NULL;
}

bool CTaskTracker::OnMouseMove(int iX,int iY)
{
	return CControlContainer::OnMouseMove(iX,iY);
}

bool CTaskTracker::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	WORD wID = 0;
	switch (dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		wID = FindTaskID(pControl);
		DelTaskTracker(wID);
		m_pParent->Msg(MSG_CTRL_UPDATE_TRACK_INFO,MAKELONG(wID,2));
		break;
	case MSG_CTRL_POPUP_ARROWTIP:
		if (dwData == EP_First_AutoFindPath)//在第一个命令处显示点击此处自动寻路
		{
			_Tag *pTag = FindFirstCommand();
			if (pTag)
			{
				this->Draw();//先绘制一次,得到pTag命令的行的坐标
				AddArrowTip(EP_First_AutoFindPath,pTag->iDrawX + 6 - m_iScreenX,pTag->iDrawY + 6 - m_iScreenY);
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

bool CTaskTracker::OnKeyDown(WORD wState,UCHAR cKey)
{
	return CControlContainer::OnKeyDown(wState,cKey);
}

void CTaskTracker::AddTaskTracker(WORD wID)
{
	int iCurSize = m_pVTrackViewer.size();
	if (iCurSize >= MAX_TASKTRACK_COUNT) return;

	_Task* pTask = g_TaskData.FindTask(wID,true);
	if (!pTask) pTask = g_TaskData.FindTask(wID,false);

	if (!pTask) return;
	//新增加一个
	int iPosY = GetTaskTrackerPosY(wID);

	TaskTracker* pTracker = new TaskTracker;
	pTracker->wTaskID = wID;
	pTracker->Button = new CCtrlButton;
	pTracker->Button->CreateEx(this,m_iOffX,iPosY,17663,17663,17663);
	AddControl(pTracker->Button);
	char ctemp[256] = {0};
	sprintf(ctemp,"<color=0xFFDEAA52 %s>\\",pTask->strTaskName.c_str());
	string str = ctemp;
	str += pTask->strTaskAim;
	pTracker->tagText.Parse(str,0,"\\",30,0xFFFFFFFF);
	int iRows = pTracker->tagText.GetRow();
	if (iRows > 3) iRows = 3;

	pTracker->TrackView = new CMarkViewer(15,3);
	pTracker->TrackView->Create(this,20 + m_iOffX,iPosY,16 * FONTSIZE_SMALL/2,FONTSIZE_SMALL*iRows + 2);
	AddControl(pTracker->TrackView);
	pTracker->TrackView->SetFont(FONT_YAHEI);
	pTracker->TrackView->SetTagText(&(pTracker->tagText));

	m_pVTrackViewer.push_back(pTracker);
}

void CTaskTracker::UpdateTaskTracker(WORD wID)
{
	int iCurSize = m_pVTrackViewer.size();
	//if (iCurSize >= MAX_TASKTRACK_COUNT) return;

	_Task* pTask = g_TaskData.FindTask(wID,true);
	if (!pTask) pTask = g_TaskData.FindTask(wID,false);

	if (!pTask) return;
	
	TaskTracker* pTracker = FindTaskTracker(wID);
	if (!pTracker) return;

	int iHeight = GetTaskTrackerHeight(wID);
	int iPosY = GetTaskTrackerPosY(wID);

	char ctemp[256] = {0};
	sprintf(ctemp,"<color=0xFFDEAA52 %s>\\",pTask->strTaskName.c_str());
	string str = ctemp;
	str += pTask->strTaskAim;
	pTracker->tagText.Parse(str,0,"\\",30,0xFFFFFFFF);
	int iRows = pTracker->tagText.GetRow();
	if (iRows > 3) iRows = 3;

	RemoveControl((CControl**)&(pTracker->TrackView));
	pTracker->TrackView = NULL;
	//SAFE_DELETE(pTracker->TrackView)
	pTracker->TrackView = new CMarkViewer(15,3);
	pTracker->TrackView->Create(this,20,iPosY,16 * FONTSIZE_SMALL/2,FONTSIZE_SMALL*iRows + 2);
	AddControl(pTracker->TrackView);
	pTracker->TrackView->SetFont(FONT_YAHEI);
	pTracker->TrackView->SetTagText(&(pTracker->tagText));

	int iTemp = pTracker->TrackView->GetHeight() - iHeight;
	if (iTemp != 0)
	{		
		bool bFound = false;
		VTASKTRACKER::iterator itResult = m_pVTrackViewer.end();
		VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

		while (it != m_pVTrackViewer.end())
		{
			if ((*it)->wTaskID == wID)
				bFound = true;
			else if (bFound && (*it)->wTaskID != wID)
			{
				(*it)->Button->Move((*it)->Button->GetX(),(*it)->Button->GetY() + iTemp);
				(*it)->TrackView->Move(20,(*it)->TrackView->GetY() + iTemp);
			}

			it++;
		}
	}
}

void CTaskTracker::DelTaskTracker(WORD wID)
{
	VTASKTRACKER::iterator itResult = m_pVTrackViewer.end();
	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

	bool bFound = false;int iHeight = 0;
	while (it != m_pVTrackViewer.end())
	{
		if ((*it)->wTaskID == wID)
		{
			bFound = true;
			itResult = it;
			iHeight = (*it)->TrackView->GetHeight() + 5;

		}
		else 
		{
			if (bFound && (*it)->wTaskID != wID)
			{
				(*it)->Button->Move(0,(*it)->Button->GetY() - iHeight);
				(*it)->TrackView->Move(20,(*it)->TrackView->GetY() - iHeight);
			}

		}

		it++;
	}

	if (itResult != m_pVTrackViewer.end())
	{
		RemoveControl((CControl**)&(*itResult)->Button);
		RemoveControl((CControl**)&(*itResult)->TrackView);
		SAFE_DELETE(*itResult);
		m_pVTrackViewer.erase(itResult);
	}
}

void CTaskTracker::ClearTaskTracker()
{
	VTASKTRACKER::iterator itResult = m_pVTrackViewer.end();
	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

	bool bFound = false;int iHeight = 0;
	while (it != m_pVTrackViewer.end())
	{
		DelTaskTracker((*it)->wTaskID);
		it = m_pVTrackViewer.begin();
	}
}

TaskTracker* CTaskTracker::FindTaskTracker(WORD wID)
{
	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

	while (it != m_pVTrackViewer.end())
	{
		if ((*it)->wTaskID == wID) return (*it);

		it++;
	}

	return NULL;
}

int CTaskTracker::GetTaskTrackerPosY(WORD wID)
{
	int iPosY = 0;
	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

	while (it != m_pVTrackViewer.end())
	{
		if ((*it)->wTaskID == wID) return iPosY;

		if ((*it)->TrackView) iPosY += (*it)->TrackView->GetHeight() + 5;

		it++;
	}

	return iPosY;
}

int CTaskTracker::GetTaskTrackerHeight(WORD wID)
{
	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

	while (it != m_pVTrackViewer.end())
	{
		if ((*it)->wTaskID == wID) return (*it)->TrackView->GetHeight();

		it++;
	}

	return 0;
}

int CTaskTracker::GetHeightEx()
{	
	int iHeight = 0;
	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

	while (it != m_pVTrackViewer.end())
	{
		iHeight += (*it)->TrackView->GetHeight() + 5;

		it++;
	}

	return iHeight;
}

WORD CTaskTracker::FindTaskID(CControl* pControl)
{
	if (pControl == NULL) return 0;

	VTASKTRACKER::iterator it = m_pVTrackViewer.begin();

	while (it != m_pVTrackViewer.end())
	{
		if ((*it)->Button == pControl) return (*it)->wTaskID;

		it++;
	}

	return 0;
}

_Tag * CTaskTracker::FindFirstCommand()
{
	for(VTASKTRACKER::iterator it = m_pVTrackViewer.begin(); it != m_pVTrackViewer.end(); it++)
	{
		if (!(*it))
		{
			continue;
		}
		CTagText &tagText = (*it)->tagText;

		int iRows = tagText.GetRow();
		for (int i = 0; i < iRows; i++)
		{
			int iCols = tagText.GetColumn(i);
			for (int j = 0; j < iCols; j++)
			{
				_Tag* pTag = tagText.GetAt(i,j);
				if (pTag && pTag->comm.size() > 0)
				{
					return pTag;
				}
			}
		}
	}

	return NULL;
}