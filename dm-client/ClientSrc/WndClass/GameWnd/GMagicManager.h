#pragma once

#include "GSceneDefine.h"
#include "GameData/MagicDefine.h"

class CMagicManager
{
	friend class CSceneManager;
public:
	CMagicManager(void);
	~CMagicManager(void);

	void    DealMagic();
	bool	ShowMagic(Magic_Show_s *ms);				//显示魔法
	bool    ShowSpecialMagic(Magic_Show_s *show); //	显示特殊魔法
	void	RefleshFlyObject(Magic_Show_s *ms,float iSpeed = 1.,bool bUseTargetTile = false);//飞行物体的位置刷新
	bool	WillBlast(Magic_Show_s *ms);											//判断是否要爆炸
	bool	WillHitOn(Magic_Show_s *ms,UINT uTargetID);								//判断是否要击中
	void	DrawSwearLight(int iX,int iY,CSimpleCharacter* pChar);		//绘制刀光
	void    DrawHumanEff(int iX,int iY,CSimpleCharacter* pChar);            //绘制人物身上的持续特效
	void	DrawSwearLightEx(int iX,int iY,BYTE cDir,CSimpleCharacter* pChar);		//绘制刀光

	void	DrawMagicNode(Magic_Node_s* pNode, int x, int y, BYTE cDir, int iFrameNow, DWORD dwColor);

    bool    AddMagicTarget(DWORD dwTargetID,int iType);//根据dwTargetID查找对应的魔法，找到加入到消失或者死亡目标列队
	bool    AddMagicTarget(Magic_Show_s* ms,DWORD dwTargetID,int iType = 0);//在ms中加入攻击目标dwTargetID
	bool    AddMagicTarget(DWORD dwTargetID,WORD wMagicID = 0,DWORD dwOwnerID = -1);//根据wMagicID或dwOwnerID查找对应的魔法，找到加入到攻击目标列队wMagicID = 0表示忽略魔法ID,dwOwnerID == -1表示忽略dwOwnerID

private:
	tex_show_t* CreateTexShow();
	void	FinishMagic(Magic_Show_s *ms);
	Magic_Show_s* CreateMagic(Magic_Show_s* ms);
	void	GetXYFromID(DWORD dwID,int &X,int &Y,int &ox,int &oy);			// 根据人物的ID取得砖块坐标和偏移坐标
	void	PushTexStore(Magic_Show_s *ms,tex_show_t *p_tex,int offy);
	bool	DealMagicFlying(Magic_Show_s *ms,int &iDelayFinish);
	bool    IsHumanByID(UINT uID);
    void    DealCycleFlower(Magic_Show_s* ms);//16方向向四周散开的特效
	void    DealDropGoodEffect(Magic_Show_s* ms);//丟物品到地上
	void    DealFestalSkyrocket(Magic_Show_s* ms);//处理节日焰火
	void    DealStrewSkyrocket(Magic_Show_s* ms); //处理背景焰火
	void    DealSkyrocket(Magic_Show_s* ms);      //处理焰火粒子
	void    DrawCenserEffect(Magic_Show_s* ms);//处理香燃烧的效果
	void    DealSparkle(Magic_Show_s* ms);        //处理小火球后面的火花
	void    CreateSparkle(Magic_Show_s* ms);      //创建小火球后面的火花
	void    DealGradualAppear(Magic_Show_s* ms);  //处理渐隐特效魔法
	void    DealNewMagicUnit(Magic_Show_s* ms);//处理创建新魔法单元,根据当前魔法的属性创建新的魔法,然后结束当前魔法
    void    DealOpenNewWnd(Magic_Show_s* ms);   //处理创建新窗口单元,根据当前魔法的属性创建新的魔法,然后结束当前魔法
    void    DealGenerateRandomNum(Magic_Show_s* ms);
	void    DealSuperFireBall(Magic_Show_s* ms);//处理强化小火球飞行
	void    DealSuperPoison(Magic_Show_s* ms);  //处理强化施毒术飞行
	void    DealYanHuaTong(Magic_Show_s* ms);   //处理烟花筒特效
	void    DealThrowStone(Magic_Show_s* ms);   //处理投石巨魔投石特效
	void    DealExchangeID(Magic_Show_s* ms);					//targetID和OwnerID互换
	void    DealMagicDulingbo(Magic_Show_s* ms);   //毒凌波特效
	void	DealSpirtMonsterMsg(Magic_Show_s* ms);		//灵兽捕捉成功与失败消息
	void    DealMagicHide(Magic_Show_s* ms);//magic owner或magei taget隐身
	void    DealMagicShow(Magic_Show_s* ms);//magic owner或magei taget现身
	void    SetMagicState(Magic_Show_s* ms);//设置magic owner或magei taget的魔法状态,有些地方要根据这个状态来判断能否移动,做其它动作等
	void    DealOwnerFly(Magic_Show_s* ms);
	void    DealHeiYanMoRefraction(Magic_Show_s* ms);
	void    DealFlyTowerCycle(Magic_Show_s* ms);
	void    DealPopupPaoPao(Magic_Show_s* ms);
	void    DealDaoFighterPoison(Magic_Show_s* ms);//火毒攻心剑
	void	DealAlternativeMagic(Magic_Show_s* ms);//多态魔法
	void    DealSanWeiUp(Magic_Show_s* ms);//三味真火云彩,角色向上漂
	void	SetFaBaoState(Magic_Show_s* ms);//改变法宝状态
	void	DealSendMessage(Magic_Show_s* ms);//向服务器发消息
	void	DealShadowKill8Start(Magic_Show_s* ms);//八方分影斩
	void	DealFireShield(Magic_Show_s* ms);//强袭烈焰盾
	void	DealLianJiZhuangji(Magic_Show_s* ms);//连击字撞击事件
	void	DealFaJueState(Magic_Show_s* ms);//法诀攻击

	//void	TextOnHead(Magic_Show_s* ms);
	void	GetDevByMagicDir(int iMagicID,int iDir,int& ix,int& iy);

	//魔法特效
	tex_show_t*		m_pMagicFloor;						//地表魔法暂存
	tex_show_t*		m_pMagicSky;						//空中魔法暂存

	int		m_iTileStartX,m_iTileStartY;			// 场景起始Tile坐标
	int     m_iDrawOffX,m_iDrawOffY;                // 画场景时的偏移
	int     m_iScrWidth;
	int     m_iScrHeight;
	DWORD   m_dwTickCount;                          // 当前计时器

};

extern CMagicManager* g_pMagicMgr;
