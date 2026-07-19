#pragma once

//用于显示和绘制场景和角色等
#include "GameMap/GameMap.h"
#include "GameData/UnionStruct.h"
#include "GameData/GameData.h"
#include "SceneDefine.h"


//场景管理器
class CSceneManager
{
	friend class CMagicManager;
public:
	typedef bool (CSceneManager::*SceneIterator)(SMapTile* pTile,int iX,int iY,int sx,int sy);
	typedef bool (CSceneManager::*BGSceneIterator)(SBgTile* pTile,int sx,int sy);

	CSceneManager(void);
	~CSceneManager(void);

	void Draw();

private:
	int		m_iTileWidth,m_iTileHeight;				// 场景的Tile尺寸
	int		m_iTileStartX,m_iTileStartY;			// 场景中心点Tile坐标
	int     m_iDrawOffX,m_iDrawOffY;                // 画场景时的偏移
	int     m_iScrWidth,m_iScrHeight;               // 屏幕的像素高度和宽度,为了使用方便暂存
	int     m_iMapTileWidth,m_iMapTileHeight;       // 地图的Tile尺寸,为了使用方便暂存= g_pGameMap->GetWidth();g_pGameMap->GetHeight()

	int		m_iPlayerX,m_iPlayerY;					// 主角实际所在地单元格
	int		m_iOffX,   m_iOffY;						// 主角的偏移,为了使用方便暂存
	int     m_iScrX,   m_iScrY;                     // 主角的屏幕坐标
	int	    m_iMouseX,m_iMouseY;					// 当前鼠标所在点的坐标

	DrawCharInf		m_DrawInf;				        // 绘制looks类型的参数
	DWORD   m_dwTickCount;                          // 当前计时器
	SMapTile* m_sLastMap;							// 上一帧绘制的地图
	int m_iLastX,m_iLastY;							// 上一帧绘制的位置
	vector<stLineMagic> m_vLastObjects;             // 屏幕下面的高建筑

	LightColorData	m_vLightColor[LIGHT_MAX_COLOR];

	//排序的绘制列表
	DrawSceneArray*  m_vDrawScene[MAX_LAYERS];
	DrawCharArray*   m_vDrawChar[MAX_LAYERS];
	BYTE             m_byMaxLayerNo;

private:
	// 处理地表，物体的纹理预先读
	void DealCache();

	//绘制前的参数准备
	void PrepareDraw();

	//绘制排序
	void BeginSorter();
	void DrawSorter();
	//加入图像元素，屏幕格子坐标，屏幕像素坐标
	void AddSorterScene(BYTE byLayer,int sx,int sy, DWORD texId);
	//加入对象元素，屏幕格子坐标，屏幕像素坐标
	void AddSorterChar(BYTE byLayer,int sx,int sy, CSimpleCharacterNode* pChar);
	void ClearSorter();

	//画背景层
	void BGStartAndOffset(BGLayerData& bgLayer);
	bool BGLayerTileIterator(SBgTile* pTile,int sx,int sy);
	bool BGLayerSmTileIterator(SBgTile* pTile,int sx,int sy);
	bool BGLayerObjectIterator(SBgTile* pTile,int sx,int sy);
	void DrawBackground();

	//画地面的大地表、小地表、单格的物体等
	bool GroundIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool GroundCorpusIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool GroundGoodIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool GroundEffIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	void DrawGround();

	//画地面上的人物，物体
	void DrawObjectTexture(int sx,int sy,DWORD texId);
	bool OvergroundIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool OverObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	void DrawOverground();

	//绘制最上面
	bool OverlayNameIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	void DrawOverlay(); 

	void IterateByXY(int x0,int y0,int x1,int y1,SceneIterator itr);
	void IterateBGByXY(BGLayerData& bgLayer,int x0,int y0,int x1,int y1,BGSceneIterator itr);

	//画阴影特效
	void DrawShadow(int iPosX,int iPosY,int iTileX,int iTileY);

	//用于画魔法产生的阴影特效
	void DrawShadow(int cX,int cY,int iPosX,int iPosY,int iTileX,int iTileY);

	//画LOOKS
	BOOL DrawLooks(int iX,int iY,DrawCharInf *pDrawInf);

	//画人物
	void DrawSimpleCharacter(int iX,int iY,CSimpleCharacterNode* pChar,BOOL bCorpus = FALSE);
	void DrawHuman(int iX,int iY,CSimpleCharacter * pChar = NULL);// 显示人
	void DrawMonster(int iX,int iY,CSimpleCharacter * pChar);	  // 显示怪物
	void DrawNpc(int iX,int iY,CSimpleCharacter * pChar);		  // 显示NPC
	void DrawTalk(int iX,int iY,CSimpleCharacter* pChar = NULL);         // 显示人物头上的话
	void DrawHPBar(int iX,int iY,CSimpleCharacter* pChar);        // 显示血条
    void DrawBooth(int iX,int iY,CSimpleCharacter* pChar = NULL);        // 显示摊位
	void DrawBoothName(int iX,int iY,CSimpleCharacter* pChar = NULL);    // 显示摊位名字

	//画物品
	void DrawSimpleGood(int iX,int iY,CSimpleGood* pGood);

	//绘制魔法
	void DrawTexShow(tex_show_t  *p_head);
};

extern CSceneManager*  g_pSceneMgr;  //魔法管理器指针

