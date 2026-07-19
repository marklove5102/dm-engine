#pragma once

#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"
#include <string>
#include <vector>


#define BUTTONCOUNT 40
#define SKILL_NUM_PAGE 8

#define M_CONSKILL_NUM_PAGE 2

class CSkillWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CSkillWnd)

public:
	CSkillWnd();
	~CSkillWnd(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual void Draw();
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnWheel(int iWheel);

	virtual void SwitchToPage(int iPage, bool force = false);

protected:
	struct _SkillInfo
	{
		_SkillInfo()
		{
			dwMagicID = 0;
			iPageNumber = -1;
			bActive = false;
		}
		string szName;
		DWORD  dwMagicID;
		int    iPageNumber;
		bool   bActive; // 连续技能时使用,骑术技能时true为主动技能，false为被动技能
		string szInfo;
		//vector<string> vecDesc;
	};

	int           m_iPageType;//0:普通,1:生产技能,2:连续技能,3:骑术技能

	CCtrlButton * m_pTabPage1;
	CCtrlButton * m_pTabPage2;
	CCtrlButton * m_pTabPage3;
	CCtrlButton * m_pTabPage4;
	CCtrlScroll *m_pLeftScroll,*m_pRightScroll;

	

	//左侧技能按钮
	CCtrlButton * m_pLeftSkillButton[BUTTONCOUNT];
	_SkillInfo           m_LeftSkill[BUTTONCOUNT];  //技能属性
	int                  m_iTotalSkillCount;        //当前职业魔法总数

	//右侧技能按钮
	CCtrlButton * m_pRightSkillButton[BUTTONCOUNT];
	CCtrlButton * m_pAddExpButton[BUTTONCOUNT];
	std::vector<int>   m_iRightSkillIndexs;

	int					m_iRightConSkill[M_CONSKILL_NUM_PAGE];


	CTagText	  m_kHelpText;
	CMarkViewer*  m_pMarkViewer;

	static std::string m_str[8];
	static std::string m_strSuper[6];

	void RefreshLeftSkillButtonState();
	void SetRightBtnTips(int i,int mid);
	void SetAllRightBtnTips();
	void SetRightPageNum();
	void UpdateConSkillState();
	void UpdateSkill();

};
