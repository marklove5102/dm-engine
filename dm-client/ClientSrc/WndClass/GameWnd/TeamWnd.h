
#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include <vector>

class CTeamWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTeamWnd)

protected:
	//CCtrlButton * m_pUpButton;
	//CCtrlButton * m_pDownButton;

	CCtrlButton * m_pAddButton;
	CCtrlButton * m_pDeleteButton;

	CCtrlGrid	* m_pGroupedGrid;
	CCtrlGrid	* m_pNeedGroupGrid;

	CCtrlButton * m_pAddListenVoiceButton;//2套声音控制按钮
	CCtrlButton * m_pDecListenVoiceButton;
	CCtrlButton * m_pAddSpeakVoiceButton;
	CCtrlButton * m_pDecSpeakVoiceButton;
	CCtrlButton * m_pCloseVoiceButton;
	CCtrlButton * m_pVoiceControlButton;

	CCtrlRadio	* m_pEnableTeamButton,*m_pAutoTeamVoice;
	//CCtrlRadio	*m_pEnableFreeQunying,*m_pEnableRegionQunying;//允许自由和群英模式

	//CCtrlRadio	*m_pTrustepshipOnlyFriend;//只允许好友群英模式
	//CCtrlEdit	* m_pCharID;
	//CCtrlRadio	* m_pEnableVoice; // 允许组队语音

	//LPTexture     m_pVolumeTex;

	int m_nWhichPerson;
	int m_nPage;
	bool m_bInTeamRoom;

	int m_nNumPersons;
	std::vector<std::string> m_vecNearPersons;
	DWORD m_dwLastLeaveOrEnterRoomTime;

//protected:
	//int InWhichArea(int iX,int iY);//决定鼠标在那个区

public:
	CTeamWnd(void);
	~CTeamWnd(void);

	virtual void OnCreate();
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	void SwitchButton(bool bInTeamRoom);
};
