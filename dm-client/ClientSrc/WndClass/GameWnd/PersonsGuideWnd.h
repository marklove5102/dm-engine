#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include <vector>

using namespace std;

#define PERSONS_LINE_COUNT 5
#define MONSTERS_LINE_COUNT 4

struct S_Persons
{
	string strName;//名字
	DWORD dwID;//人物ID
};

struct S_Npc
{
	string strName;//名字
	string strDesp;//描述
	POINT  pt;//坐标
};

struct S_Monsters
{
	string strName;//名字
	string strLevel;//等级
	bool  bYouHuo;//能否诱惑
};


class CPersonsGuideWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CPersonsGuideWnd)

public:
	CPersonsGuideWnd(void);
	~CPersonsGuideWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnWheel(int iWheel);	
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual void OnClickRow();
	virtual bool OnChar(UCHAR cChar);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);	
	virtual void OnMove();
	virtual void OnSwitchToTop();
	virtual void ResetWidthAndHeight(int iW,int iH);


protected:
	//static int m_iLastStayPage;//

	//CCtrlGrid *   m_pGrid;//NPC列表控件
	CCtrlGrid *   m_pNPCGrid;//NPC列表控件
	CCtrlGrid *   m_pPlayerGrid;//Player列表控件
	CCtrlGrid *   m_pMonsterGrid;//monster列表控件

	CCtrlButton * m_pSociety1; //人脉
	//CCtrlButton * m_pTeam; //组队
	//CCtrlButton * m_pTrade; //交易
	//CCtrlButton * m_pWatch; //查看
	//CCtrlButton * m_pChatSecret; //密聊
	CCtrlButton * m_pAddFriend; //加为好友
	//CCtrlButton * m_pSociety2; //人脉

	//int m_iCurPage;//当前是那一页
	DWORD m_dwLastRefrashPersonsData;

	vector<S_Persons>     m_VPersons;//人物数据表,根据名字排序
	vector<S_Npc>         m_VNpc;//NPC数据表
	vector<int>           m_VOldPersonsID;//旧的人物ＩＤ表

	vector<S_Monsters>    m_vMonsters;

	void DrawPersonsLineData();//绘制人物数据
	void DrawNpcLineData();//绘制NPc数据
	void DrawMonsterLineData();
	void OnMoveToPlace();//移动
	void OnTeam();//组队
	void OnTrade();//交易
	void OnWatchEquip();//查看装备
	void OnChatSecret();//密聊
	void OnFriend();//加为好友
	void OnTakeInGuild();//邀请加入行会
	void OnTakeinPrentice();//招收徒弟
	void OnWatchBooth();//查看摊位
	void OnCopyName();//复制人名
	void OnTakeInMaster();//拜师
	void OnTakeInPhyle();
// 	void OnTneup(DWORD dwData);
	void CreatePersonsMap();//创建视野范围里的人物列表m_VPersons
	bool IsDataValid();//看看执行操作前是否数据正确

	int getFirstChar(BYTE * strName);//获得NPC或者玩家的名字的第一个字母，如果是中文字名字那么获得第一个汉字的拼音首字母(对于一级汉字有效)
	void RefrashPersonsData();

};
