#pragma once

#include "Global/Global.h"
#include "GameData/SimpleCharacter.h"
#include "GSceneDefine.h"
#include "GameAI/PathFinder.h"

//游戏场景内处理鼠标和键盘相关事件
//比单纯的控件事件抽象要高一些，直接影响游戏数据

class CGood;

class CGameManager
{
public:
	CGameManager(void);
	~CGameManager(void);

	bool OnLeftButtonDown(int iX,int iY);
	bool OnLeftButtonUp(int iX,int iY);
	bool OnLeftButtonDClick(int iX,int iY);
	bool OnRightButtonDown(int iX,int iY);
	bool OnRightButtonUp(int iX,int iY);
	bool OnRightButtonDClick(int iX,int iY);
	bool OnMiddleButtonDown(int iX,int iY);
	bool OnMouseMove(int iX,int iY);

	void Update(bool bIsFocus);
	void DealPlayer();
	void DealCharacter();
	void PlayerDoNextStep();
	void CharDoNextStep(CSimpleCharacterNode *pChar);

	void SetMouseTile(int iX,int iY);
	void GetMouseTile(int& x,int& y);
	int  GetPlayerDir(int iX,int iY);
	char GetGoDir2(int x,int y,char cDir,int step,BOOL bAttack,BOOL &bNeedRun);
	bool IsCanGo(int x,int y,char cDir,int step = 1);
	bool PlayerCanRun();

	void ForceAttack(UINT uID);
	void GotoMousePos(BOOL bNeedRun,BOOL bNeedCheckTurn = FALSE); //走到鼠标点击的地方bNeedCheckTurn为TRUE时如果离目标很近只转方向
	bool OnClickChar(UINT uID);
	bool OnRClickChar(UINT uID);
	bool OnKeyDown(WORD wState,UCHAR cKey);
	bool OnKeyUp(WORD wState,UCHAR cKey);
	bool AttackUseMagic(int iMagicID,DWORD dwTargetID = 0,bool bAlert = true); //对指定目标使用技能
	bool AI_AutoWalk(int iX,int iY); //自动走到目的地
	bool DealAutoRun();
	void OnRankUpLevel(int iOldLevel,int iNewLevel);
	bool PlayerMoveImmediate(SNextAction& Next,int iSelfX,int iSelfY);

private:
	// 寻路移动相关,路径是否为全通路,iIntegrity
	bool PlayerMove(SNextAction &Next,int iSelfX,int iSelfY,int iStep = 1,BOOL bAttack = FALSE,int *iIntegrity = 0,int FailNoIntegrity = 0);
	bool PlayerAttack(SNextAction& Next,int iSelfX,int iSelfY);
	bool PlayerMagic(SNextAction& Next,int iSelfX,int iSelfY);
	void AutoNextAction(); //根据鼠标等状态生成新动作
	void UpdateAlarm(bool bWaiting);
	void SetCharBlock(CSimpleCharacter* pChar);
	void OnClickMonster(CSimpleCharacter *pChar);
	void OnClickNpc(CSimpleCharacter* pChar);
	void AddCharacterBlock();

	void DealPlayerActionStand();				//非托管模式
// 	void DealPlayerActionStandTrust();			//托管模式
	void DealPlayerActionRunning();				//动作正在执行中
	void DealPlayerActionEnd();					//非托管模式
// 	void DealPlayerActionEndTrust();			//托管模式


	DWORD m_dwFrameCount;  //暂存记录时间
	bool  m_bMouseInGameWnd;
	bool  m_bMouseDClick;
	bool  m_bCannotMove;
	bool  m_bLeftDown,m_bRightDown;
	int	  m_iMouseX,m_iMouseY;	// 当前鼠标所在点的坐标
	int	  m_iPlayerX,m_iPlayerY;// 主角实际所在地单元格
	int   m_iMouseTileX,m_iMouseTileY;

	stFollowChar	m_FollowChar;
	CPathFinder     m_PathFinder;
	DWORD m_dwAutoDigID;//自动挖心魔的心魔ID
	DWORD m_dwActionStartTime;
};

extern CGameManager* g_pGameMgr;
