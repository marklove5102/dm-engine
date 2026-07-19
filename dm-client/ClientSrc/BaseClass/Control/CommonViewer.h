#pragma once

#include "Control.h"
#include "GameData/GameGlobal.h"

//游戏内数据的公共组件
class CCommonViewer : public CControl
{
	DTI_DECLARE(CCommonViewer, CControl)
public:
	enum eCOMMON_VIEWER
	{
		CV_NONE = 0,
		CV_PANEL_HPMP,
		CV_ALARM_LIGHT,
		CV_PANEL_HPMP_NEW,
		CV_ALARM_LIGHT_NEW,
	};
	CCommonViewer(void);
	~CCommonViewer(void);

	virtual bool Create(CControl * pParent,int iX,int iY,eCOMMON_VIEWER t);
	virtual bool CreateXML(CControl* pParent,CXmlControl* ctrl,eCOMMON_VIEWER t);
	virtual void OnCreate();
	virtual void OnDraw();

	void    SetType(eCOMMON_VIEWER t);
	void    DrawHPMP();//显示血量蓝量数字
protected:
	void    DrawPanelLevel();
	void    DrawPanelHPMP();
	void    DrawAlarmLight();
	

	eCOMMON_VIEWER m_ViewerType; //观察器类型
	DWORD m_dwHPAlarmRand;//血量小于20%时音效的Rand
};
