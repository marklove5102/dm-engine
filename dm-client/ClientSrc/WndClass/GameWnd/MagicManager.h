#pragma once

#include "SceneDefine.h"
#include "GameData/MagicDefine.h"

class CMagicManager
{
	friend class CSceneManager;
public:
	CMagicManager(void);
	~CMagicManager(void);

	void    DealMagic();
	bool	ShowMagic(Magic_Show_s *ms);				//显示魔法
	void	RefleshFlyObject(Magic_Show_s *ms,float iSpeed = 1.,bool bUseTargetTile = false);//飞行物体的位置刷新
	bool	WillBlast(Magic_Show_s *ms);											//判断是否要爆炸
	bool	WillHitOn(Magic_Show_s *ms,UINT uTargetID);								//判断是否要击中
	void	DrawSwearLight(int iX,int iY,CSimpleCharacterNode* pChar);		//绘制刀光

private:
	tex_show_t* CreateTexShow();
	void	FinishMagic(Magic_Show_s *ms);
	Magic_Show_s* CreateMagic(Magic_Show_s* ms);
	void	GetXYFromID(DWORD dwID,int &X,int &Y,int &ox,int &oy);			// 根据人物的ID取得砖块坐标和偏移坐标
	void	PushTexStore(Magic_Show_s *ms,tex_show_t *p_tex,int offy);
	bool	DealMagicFlying(Magic_Show_s *ms,int &iDelayFinish);
	bool    IsHumanByID(UINT uID);

	//魔法特效
	tex_show_t*		m_pMagicFloor;						//地表魔法暂存
	tex_show_t*		m_pMagicSky;						//空中魔法暂存

	int		m_iTileStartX,m_iTileStartY;			// 场景中心点Tile坐标
	int     m_iDrawOffX,m_iDrawOffY;                // 画场景时的偏移
	int     m_iScrWidth;
	int     m_iScrHeight;
	DWORD   m_dwTickCount;                          // 当前计时器
};

extern CMagicManager* g_pMagicMgr;