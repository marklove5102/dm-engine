#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlGrid.h"

class CDelSkillWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CDelSkillWnd)
public:
	CDelSkillWnd(void);
	~CDelSkillWnd(void);
	virtual void OnCreate();
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

private:
	struct SSkill
	{
		SSkill(string name,WORD id,string level)
		{
			strName = name;
			wID = id;
			strLevel = level;
		}		

		string strName;
		WORD wID;
		string strLevel;
	};

	CCtrlButton *  m_pSubmitBtn; //确认按钮
	CCtrlButton *  m_pCancelBtn; //确认按钮	
	CCtrlGrid *    m_pGrid;//列表控件	
	vector<SSkill> m_VSkill;//技能名称列表
	DWORD m_dwGoodID;
	int m_iSelectedRow;


	void DrawLineData();	
};