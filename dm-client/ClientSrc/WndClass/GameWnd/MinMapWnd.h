#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "GameMap/MinMap.h"
#include "GameMap/GameMap.h"
#include "Global/MathUtil.h"
#include <vector>
#include <string>
#include <deque>

#define MAX_MAPFILE_BACK_COUNT  30

class CMinMapWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CMinMapWnd)

public:
	CMinMapWnd(void);
	~CMinMapWnd(void);

	virtual void Draw(void);
	virtual void OnDraw();
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnMove();
	virtual void OnSwitchToTop();
	virtual void ResetWidthAndHeight(int iW,int iH);

private:
	SRect    m_rcArea;
	CCtrlMenuButton*  m_pWorldButton;    //世界地图
	CCtrlMenuButton*  m_pJumpButton;     //跳转点
	CCtrlButton*  m_pBackButton;//返回上一张地图
	//CCtrlButton*  m_pPersonGuide;    //人物指南

	CCtrlRadio* m_pShowFriends;
	char m_currentMinMapName[64];
	bool m_bMenuDown;

private:
	void SetInfoTip(int iRealX,int iRealY,int iDelta);
	void OnUpdateInterface(const char* szMapFile); //更新界面
	bool IsInArea(int iX,int iY);
	bool IsInMapArea(int iX,int iY);
	void ReflashJumBtn();

	//地图信息
	string m_strMapName;
	string m_strMapFile;
	bool   m_bBlock;
	//地图
	LocalmapInfo_s* m_pAreaMap;
	JumplumpInfo_s* m_pJumpPoint;  //选中的跳转点
	NpcInfo_s*  m_pNpc;           //选中的NPC

	int  m_iMinMapX,m_iMinMapY; //小地图相对于框的坐标点
	int  m_iMinMapWidth,m_iMinMapHeight;

	float m_fRateX;  //水平方向缩放比例
	float m_fRateY;  //垂直方向缩放比例

	//LPTexture m_pMapTex;
	bool  m_bFriend;
	bool  m_bEntireWorldMap;
	deque<string> m_strMapFileStack;

	CControl * m_pPersonGuideWnd;
	vector<CCtrlButton*> m_VJumpBtn;//跳转按钮

	WORD  m_wMinMapID;
	bool  m_bNeedReflashedMapInfo;//是否刷新地图信息,开始图没有读到,后来下载下来了,要刷新
};
