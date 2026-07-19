#pragma once
#include "BaseClass/Control/CtrlWindowX.h"


class CHandyMapWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CHandyMapWnd)

public:
	CHandyMapWnd(void);
	~CHandyMapWnd(void);
	static int GetHandyMapStatus(){return m_siHandyMapStatus;}
	static void SetHandyMapStatus(int i){m_siHandyMapStatus = i;}
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual void Draw();
	void UpdatePos();
private:
	void DrawMiniMap();
	void DrawOther();
	bool HasMessage();
protected:
	static int m_siHandyMapStatus;

	CCtrlButton* m_pMinMax;//最大,最小化
	CCtrlButton* m_pUITypeChange;//改变ui
	CCtrlButton* m_pCourseTable;//活动日志
	CCtrlButton* m_pPublicBord;//传世残卷
	CCtrlButton* m_pWorldMap;//世界地图
	CCtrlButton* m_pInfoInform;//查看最新消息
	CCtrlButton* m_pNpcAutoPath;//附近npc
	CCtrlButton* m_pNewHandPrompt;//新手帮助
	CCtrlButton* m_pSystemSetting;//系统设置
	CCtrlButton* m_pWenwen;//系统设置

	//LPTexture   m_pTemplate;
};
