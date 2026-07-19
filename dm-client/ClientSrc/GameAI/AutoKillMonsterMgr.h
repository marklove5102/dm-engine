#pragma once
#include "Global/Global.h"
#include "GameData/SimpleCharacter.h"

enum AutoKMPetType
{
	AKM_PetSkull,						//普通骷髅
	AKM_PetCorpus,						//赶尸
	AKM_PetSupernalBeast,				//神兽
	AKM_PetSuperSkull,					//强化骷髅	
	AKM_PetNum,							//宠物类型数
};

class CAutoKillMonsterMgr
{
public:
	CAutoKillMonsterMgr(void);
	~CAutoKillMonsterMgr(void);

	void SetEnabled(bool val);

	bool DoLoop();
	void Clear();

	void AddMonster(CSimpleCharacterNode* pNode);
	void AddMonster(DWORD dwMonsterID);
	void DelMonster(DWORD dwMonsterID);
	bool HasAutoKillMonster();

	void ReAddMonsters();// 刷新怪物列表

	void SetSelfAttackID(DWORD val);
	DWORD GetSelfAttackID()			{ return m_dwSelfAttackID; }

	void OnPetStateChange();
	void OnMagicEffectived(WORD wMagicID);
	void DealWithSelfHurted(DWORD id);

protected:
	bool AIAttackPolicy(CSimpleCharacterNode* pNode);		//攻击
	bool AIFighterAttackPolicy(CSimpleCharacterNode* pNode);//战士攻击策略
	bool AIRabbiAttackPolicy(CSimpleCharacterNode* pNode);	//法师攻击策略
	bool AITaoistAttackPolicy(CSimpleCharacterNode* pNode);	//道士攻击策略

	bool AITaoistAttack(int iMagicID,DWORD dwID,bool bGreen);

	bool AIDefendPolicy();
	bool AIFighterDefendPolicy();
	bool AIRabbiDefendPolicy();
	bool AITaoistDefendPolicy();

	void _AddMonster(DWORD id, bool checkexist = true);
	void _AddMonster(CSimpleCharacterNode* pNode, bool checkexist = true);

	bool IsCanAttack(CSimpleCharacterNode* pChar);
	bool HasAttackMe(DWORD dwID);

	CSimpleCharacterNode* GetCanAttackTarget();	
	CSimpleCharacterNode* GetNearestVaildMonster();

	bool CanReach2(CSimpleCharacterNode* pNode);	
	void Reach(CSimpleCharacterNode* pNode);
	
	void SortMonster();

	bool AIRandAutoWalk();

	bool CanUseMagic(int wMagicID);
	bool AttackUseMagic(int iMagicID,DWORD dwTargetID);

	void ReLoadAllowKillMonsters();

	list<DWORD>		m_vMonsters;
	bool			m_isMonsterSorted;

	bool			m_bAllowKillMonsterEnabled;
	std::map<string, char> m_allowKillMonsters;

	DWORD			m_dwSelfAttackID;	  //自己攻击目标
	int				m_iSelfX, m_iSelfY;

	int				aPet[AKM_PetNum];//召唤出来的骷髅数量
	int				m_iPosionTimes;//对同一目标使用毒的次数,对同一目标使用次数不超过三次,如果目标改变或是目标被毒上时清0

	//战士	
	bool			m_bAutoSteelProtect;	//金刚
	bool			m_bAutoProtectSkin;		//护身	
	bool			m_bAutoWildCollideKill;	//野蛮	
	bool			m_bDestroyShield;		//持续破盾
	bool			m_bDestroyShell;		//持续破击	
	bool			m_bAutoFire;			//烈火剑法
	bool			m_bAutoDoubleFire;		//自动双烈火	

	//法师
	bool			m_bAutoMagicProtect;	//魔法盾
	bool			m_bAutoDispute;			//抗拒
	bool			m_bAutoDragonLight;		//狂龙
	bool			m_bAutoThunder;			//雷电术/冰箭术
	bool			m_bAutoFireBall;		//小火球

	//道士
	bool			m_bAutoProtectGhost;	//幽灵盾
	bool			m_bAutoProtectArmor;	//神圣战甲术
	bool			m_bAutoLionCall;		//自动狮子吼
	bool			m_bAutoFireJuju;		//幽冥火咒
	bool			m_bAutoProtectSymbol;	//灵魂道符
	bool			m_bAutoPoison;			//自动施毒/诅咒

	bool			m_bAutoSuperFireBall;	//强化小火球
	int             m_iLastAutoWalkX,m_iLastAutoWalkY;//自动寻路的坐标,打完了怪物后还应该往这个方向防护,不然有可能会出现来回反复

};

extern CAutoKillMonsterMgr g_AutoKillMonsterMgr;