#pragma once

//用于显示和绘制场景和角色等
#include "GameMap/GameMap.h"
#include "GameData/UnionStruct.h"
#include "GameData/GameData.h"
#include "GSceneDefine.h"

struct SDrawTex
{
	SDrawTex ()
	{
		pTex = NULL;
		renderMode = RM_ALPHA;
	}

	SDrawTex (LPTexture tex,RenderMode rm = RM_ALPHA)
	{
		pTex = tex;
		renderMode = rm;
	}

	LPTexture pTex;
	RenderMode renderMode;
};


//场景管理器
class CSceneManager
{
	friend class CMagicManager;
public:
	typedef bool (CSceneManager::*SceneIterator)(SMapTile* pTile,int iX,int iY,int sx,int sy);

	CSceneManager(void);
	~CSceneManager(void);

	//绘制前的参数准备
	void PrepareDraw();
	//绘制
	void Draw(bool bNeedRt);

	//
	void GetTileStartXY(int& iTileX,int& iTileY){ iTileX = m_iTileStartX;iTileY = m_iTileStartY;}
	void GetDrawOffXY(int& iDrawOffX,int& iDrawOffY){ iDrawOffX = m_iDrawOffX;iDrawOffY = m_iDrawOffY;}

private:
	int		m_iTileWidth,m_iTileHeight;				// 场景的Tile尺寸
	int		m_iHalfTileWidth,m_iHalfTileHeight;		// 场景的Tile尺寸/2
	int		m_iTileStartX,m_iTileStartY;			// 场景左上角Tile坐标
	int     m_iDrawOffX,m_iDrawOffY;                // 画场景时的偏移
	int     m_iScrWidth,m_iScrHeight;               // 屏幕的像素高度和宽度,为了使用方便暂存
	int     m_iMapTileWidth,m_iMapTileHeight;       // 地图的Tile尺寸,为了使用方便暂存= g_pGameMap->GetWidth();g_pGameMap->GetHeight()

	int		m_iPlayerX,m_iPlayerY;					// 主角实际所在地单元格
	int		m_iOffX,   m_iOffY;						// 主角的偏移,为了使用方便暂存
	int     m_iScrX,   m_iScrY;                     // 主角的屏幕坐标(像素)
	int	    m_iMouseX,m_iMouseY;					// 当前鼠标所在点的坐标
	int     m_iShakeX, m_iShakeY;                   // 地震时移动的偏移

	int     m_iInDownloadFiles;                     // 正在下载的文件个数

	DrawCharInf		m_DrawInf;				        // 绘制looks类型的参数
	DrawBoothInf    m_DrawBooth;                    // 绘制摊位的参数
	DWORD   m_dwFrameCount;                          // 当前计时器
	SMapTile* m_sLastMap;							// 上一帧绘制的地图
	int m_iLastX,m_iLastY;							// 上一帧绘制的位置
	DWORD  m_dwLastCheckDownLoadListTime;           // 上一次检测下载列表的时间

	LightColorData	m_vLightColor[LIGHT_MAX_COLOR];

	//排序的绘制列表
	DrawCharArray    m_vDrawCharTop;   //顶层人物绘制
	bool             m_bMouseInNpc;  //是否有NPC在鼠标中,如果有优先选择NPC,这时即使有其它更上层的人物也在鼠标中也忽略
	bool             m_bNeedRt;      //是否需要输出，鼠标在游戏窗口内
	DWORD            m_dwDemonSmokeTime;//附魔冒烟时间控制

	RenderMode m_TexPartRenderMode[DP_DrawPartMaxNum];

private:
	// 处理地表，物体的纹理预先读
	void DealCache();


	//绘制排序
	void DrawSorterTop();
	void DrawSorterEx(int x0,int y0,int x1,int y1);

	void DrawAlpha();

	//画背景层
	void BGStartAndOffset(BGLayerData& bgLayer);
	bool BGLayerTileIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool BGLayerSmTileIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool BGLayerObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	void DrawBackground();

	//画地面的大地表、小地表、单格的物体等
	bool GroundIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool SMTileIterater(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool NearGroundObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);//单格子物体或有贴地属性的物体
	bool UnderObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);//物体之后的一层场景
	bool OverObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	//bool GroundOverEffIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);//绘制地表魔法之上的场景
	bool GroundBoothIterator(SMapTile* pTile,int iX,int iY,int sx,int sy); //所有的0号摊位包括主角的和豹子
	bool GroundCorpusIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool GroundGoodIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool GroundEffIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	void DrawGround();

	//画地面上的人物，物体
	void DrawObjectTexture(int sx,int sy,LPTexture pTex,DWORD dwColor = 0xFFFFFFFF);
	bool OvergroundIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool ObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	void DrawOverground();

	//绘制最上面
	bool OverlayGoodNameIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool OverlayCharNameIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool OverlayNPCNameIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	bool OverlayCharHPIterator(SMapTile* pTile,int iX,int iY,int sx,int sy);
	void DrawOverlay(); 
	void DrawLianJiNumber();	//显示连击数字
	bool DrawCharName(int sx,int sy,CSimpleCharacter* pChar);
	bool DrawCharHP(int sx,int sy,CSimpleCharacter* pChar);

	void IterateByXY(int x0,int y0,int x1,int y1,SceneIterator itr);
	void IterateBGByXY(BGLayerData& bgLayer,int x0,int y0,int x1,int y1,SceneIterator itr);

	//画阴影特效
	void DrawShadow(int iPosX,int iPosY,int iTileX,int iTileY);

	//用于画魔法产生的阴影特效
	void DrawShadow(int cX,int cY,int iPosX,int iPosY,int iTileX,int iTileY);

	//！！注意下列函数的iX,iY都是表示的屏幕坐标，即所在单元格左上角的屏幕坐标
	//画LOOKS
	BOOL DrawLooks(int iX,int iY,DrawCharInf *pDrawInf);

	//画人物
	void DrawSimpleCharacterNode(int iX,int iY,CSimpleCharacterNode* pChar,BOOL bCorpus = FALSE);
	void DrawSimpleCharacter(int iX,int iY,CSimpleCharacter* pChar,BOOL bCorpus = FALSE);
	void DrawHuman(int iX,int iY,CSimpleCharacter * pChar = NULL,BOOL bNotAlpha = TRUE);// 显示人
	void DrawMonster(int iX,int iY,CSimpleCharacter * pChar,BOOL bNotAlpha = TRUE);	  // 显示怪物,pChar为NULL时表示画自己变身后的效果
	void DrawNpc(int iX,int iY,CSimpleCharacter * pChar,BOOL bNotAlpha = TRUE);		  // 显示NPC
	void DrawSeniorBooth(int iX,int iY,DrawBoothInf* pBoothInf,BOOL bFirst);  // 绘制高级的旗帜和摊位

	//利用TOPXY属性绘制
	void DrawTalk(CSimpleCharacter* pChar = NULL);         // 显示人物头上的话
	void DrawBoothName(CSimpleCharacter* pChar = NULL);    // 显示摊位名字

	//画物品
	void DrawSimpleGood(int iX,int iY,CSimpleGood* pGood);

	//绘制魔法
	void DrawTexShow(tex_show_t  *p_head);

	//绘制野蛮冲撞
	void DrawWildCollide(int iX, int iY);

	void DrawSavageCollide(int iX,int iY,BYTE byType);
	
	//绘制突斩刀光
    void DrawSuddenKill(int iX, int iY, SAction* pAction, CSimpleCharacter* pChar);

	//绘制残影刀法
	void DrawRemainShade(int iX, int iY);
	//绘制怪物灵魂墙
	void DrawSoulWall(int iX,int iY,CSimpleCharacter * pChar);
	//画黄榜大旗,黄榜大大旗,华盖天罗旗,玄天功德牌上面的文字
	void DrawFlagText(int x,int y,CSimpleCharacter * pChar);
	////画经验冒泡
	//void DrawBubbleUp(CSimpleCharacter * pChar);
	//绘制纵雷诀残影
	void DrawZhongLeiShadow(int x,int y);
	void GetZhongLeiDev(int iDir,int& iDevX,int& iDevY);//根据方向获得纵雷诀偏移值
	
	//强袭烈焰盾攻击
	void DrawFireShieldAttack(int x,int y,CSimpleCharacter * pChar);
	//八方分影斩
	void DrawShadowKill8(int x,int y,CSimpleCharacter * pChar);
};

extern CSceneManager*  g_pSceneMgr;  //魔法管理器指针

