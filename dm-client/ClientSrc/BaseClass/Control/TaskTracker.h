#pragma once
#include "Control.h"
#include "MarkViewer.h"
#include "GameData/TagText.h"
#include "Global/StdHeaders.h"
#include "BaseClass/Control/CtrlButton.h"

#define MAX_TASKTRACK_COUNT 5
struct TaskTracker
{
	WORD	 wTaskID;
	CTagText tagText;
	CCtrlButton* Button;
	CMarkViewer* TrackView;
	TaskTracker()
	{
		wTaskID = 0;
		tagText.Clear();
		Button = NULL;
		TrackView = NULL;
	}
};
typedef  std::vector<TaskTracker*> VTASKTRACKER;

class CTaskTracker : public CCtrlWindow
{
	DTI_DECLARE(CTaskTracker, CControl)
public:
	CTaskTracker(void);
	~CTaskTracker(void);

	virtual void Draw();
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);
public:
	void AddTaskTracker(WORD wID);
	void UpdateTaskTracker(WORD wID);
	void DelTaskTracker(WORD wID);
	void ClearTaskTracker();
	TaskTracker* FindTaskTracker(WORD wID);
	int GetTaskTrackerPosY(WORD wID);
	int GetTaskTrackerHeight(WORD wID);
	int GetHeightEx();
	WORD FindTaskID(CControl* pControl);
	TaskTracker* GetClickedTracked(int iX,int iY);
	int GetTaskTrackedNum(){return m_pVTrackViewer.size();}
	_Tag *FindFirstCommand();
protected:
	VTASKTRACKER	m_pVTrackViewer;
};
