#pragma once


#include "BaseClass/Control/CtrlWindowX.h"
#ifdef _CHAT
#include "GameClient/VoiceAdapter.h"
#endif

class CPersonPanelWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CPersonPanelWnd)

public:
	CPersonPanelWnd(void);
	~CPersonPanelWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnMouseMove(int iX,int iY);

protected:
	DWORD m_dwPersonID;//该窗口显示的人物ＩＤ
	string m_strPersonName;//名字

	void OnAddJieyi();//好友结义
	void OnAddBlack();//加入黑名单
	void OnTeam();//组队
	void OnTrade();//交易
	void OnWatchEquip();//查看装备
	void OnChatSecret();//密聊
	void OnFriend();//加为好友
	void OnTakeInGuild();//邀请加入行会
	void OnTakeinPrentice();//招收徒弟
	void OnWatchBooth();//查看摊位
	void OnTakeInMaster();//拜师
	void OnTakeInPhyle();//邀请加个宗派
// 	void OnTneup(DWORD dwData);

#ifdef _CHAT
	void OnInviteVoice();//邀请语聊
#endif

};
