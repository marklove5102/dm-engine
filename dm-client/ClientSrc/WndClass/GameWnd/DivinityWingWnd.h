#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/Good.h"

// 神翼
class CDivinityWingWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CDivinityWingWnd)

public:
	CDivinityWingWnd(void);
	~CDivinityWingWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);
	virtual bool OnMouseMove(int iX, int iY);

protected:
	CCtrlButton* m_pUp; // 升级按钮
	CCtrlRadio* m_pRadioBtn[4];
	CCtrlButton* m_pSkillButton;
	
	static const std::string m_strWingSkillLevel[];
	static const std::string m_strWingNeedLevel[];
	
	static const string m_WingAttrTypes[3][14];
	static const std::string m_WingAttrTypes1[];
	static const std::string m_WingAttrTypes2[];
	static const POINT m_ptLevelPos[];

	CTagText	 m_npcText;
	CMarkViewer* m_pMarkViewer;  //数据显示框 
};
