#pragma once

#include "Global/Global.h"
#include "GameData/SimpleCharacter.h"
#include "GameData/PetData.h"

#ifndef g_AIMgr
#define g_AIMgr g_pSelf->AiMgr()
#endif

enum ePLAYER_TIME
{
	plyNextMagicTime = 0,
	plyNextAttackTime,
	plyNextMoveTime,
	plyLastMagicTime,
	plyLastAttackTime,
	plyLastMoveTime,
	plyLastVerifyAction,				// 最近一次需要验证的动作
	plyLastVerifyActionTime,			// 最近一次需要验证的动作发出的时间
	plyLastFailTime,					// 最近一次fail的时间
	plyLastFlyTime,                     // 最近一次飞行时间
	plyLastFireTime,                    // 最近一次放烈火时间
	plyLastDWTime,                      // 最近一次击破时间
	plyLastCollide,						// 最近使用冲撞的时间
	plyLastDisputeTime,                 // 最近一次使用抗拒的时间
	plyLastLionCall,                    // 最近一次使用狮子吼的时间
	plyLastProtectGhost,                // 最近一次使用幽灵盾
	plyLastProtectArmor,                // 最近一次使用神圣战甲术
	plyLastSAttackKill,                 // 最近一次使用强化攻杀
	plyLastAutoSkillTime,               // 最近一次自动练功的时间
	plyLastMagicProtect,                // 最近一次使用魔法盾的时间
	plyLastDodgeProtect,                // 最近一次使用风影盾的时间
	plyLastStellProtect,                // 最近一次使用金刚护体的时间
	plyLastProtectSkin,                 // 最近一次使用护身真气的时间

	plyLastRequireTradeTime,            //最后一次请求交易时间
	plyLastRequireFriendTime,           //最后一次请求交友时间
	plyLastTakeinGuildTime,             //最后一次请求加入行会时间
	plyLastTakeinPrenticeTime,          //最后一次请求收徒时间
	plyLastTakeinMasterTime,            //最后一次请求拜师时间
	plyStartShakeTime,                  //最后一次震动的时间
	plyEndShakeTime,                    //震动的持续时间

	plyStartGhostTime,                  //抗魔法力开始时间
	plyDuraGhostTime,                   //搞魔法力持续时间

	plyStartArmorTime,                  //防御力增加开始时间
	plyDuraArmorTime,                   //防御力增加持续时间

	plyMibaoLateTime,                   //输入密宝消息的延迟时间,为0表示没有绑定密保或是过了密保输入时段或是已经输入了
	plyMibaoTotalTime,                  //plyMibaoLateTime + 多少秒之内输入
	plyVerifyLateTime,                  //输入验证码消息的延迟时间
	plyVerifyTotalTime,                 //plyVerifyLateTime + 多少秒之内输入
	plyRideFightTime,					//骑战时间间隔
	plyLastZhiYanTime,                  //上一次使用炙炎技能的时间
	plyLastTaoZhuangTime,               //上一次使用套装技能的时间
	plyLastSuperFireBall,               //上一次使用强化小火球
	plyLastColdstrom,					//上一次使用寒冰风暴

	plyTimeNums,                        // 时间的个数
};


// 连击技能中 技能的最大数量
#define M_MAXCONSKILLSUBNUM 3

struct ConSubSkill
{	
	ConSubSkill()
	{
		wMagicID = 0;
		nBuffID = 0;
	}

	WORD wMagicID;
	string nName;
	int nBuffID;
	string nInfo;
};

struct ConSkillData
{
	ConSkillData()
	{
		iConSkillID = 0;
		nCareer = 0;
		nMagicCount = 0;
		iCurrentIndex = 0;
		dwKey = 0;
		bActive = false;
	}

	void SetToFirst()
	{
		iCurrentIndex = 0;
	}

	WORD GetFirstMagicID()
	{
		return wMagicIDs[0].wMagicID;
	}

	ConSubSkill* GetCurrentConSubSkill()
	{
		if (iCurrentIndex >= 0 && iCurrentIndex < M_MAXCONSKILLSUBNUM)
		{
			return &wMagicIDs[iCurrentIndex];
		}
		return NULL;
	}

	WORD GetCurrentMagicID()
	{
		if (iCurrentIndex >= 0 && iCurrentIndex < M_MAXCONSKILLSUBNUM)
		{
			return wMagicIDs[iCurrentIndex].wMagicID;
		}		
		return 0;
	}	

	void ChangeToNextConSubSkill()
	{
		iCurrentIndex++;
		if (iCurrentIndex >= nMagicCount || iCurrentIndex >= M_MAXCONSKILLSUBNUM)
		{
			iCurrentIndex = 0;
		}		
	}

	bool IsOnEndMagic()
	{
		return (iCurrentIndex == nMagicCount - 1);		
	}

	int iConSkillID; // > 0
	int nCareer;//职业	

	int nMagicCount;
	ConSubSkill wMagicIDs[M_MAXCONSKILLSUBNUM];	// 技能连击列表	
	int iCurrentIndex;

	bool bActive;
	DWORD dwKey; //快捷键
};


struct ConSkillBuff
{
	ConSkillBuff()
	{
		nBuffID = 0;
		nTotalTime = 6000;
		nIcon = 0;

		bActive = false;		
		nStartTime = 0;
		nNowTime = 0;
		pConSkill = 0;
		wMagicID = 0;
	}

	void Deactive()
	{
		if (bActive)
		{
			bActive = false;
			pConSkill = 0;
		}
	}

	void Deactive(bool bSetToFirst)
	{
		if (bActive)
		{
			bActive = false;
			if (pConSkill)
			{
				if (bSetToFirst || pConSkill->IsOnEndMagic())
				{
					pConSkill->SetToFirst();
				}
			}

			pConSkill = 0;
		}
	}

	int		nBuffID;	// >= 0
	string	nName;		// buff名称
	int		nTotalTime;	// 总时间	
	int		nIcon;		// 图标	

	bool	bActive;
	DWORD	nStartTime;	// 开始时间
	DWORD	nNowTime;
	ConSkillData* pConSkill;	// 连击技能
	WORD	wMagicID;
};


typedef std::map<int, ConSkillData*> ConSkillMap;


class CAIMgr
{
public:
	enum eAI_ACTION
	{
		AI_ACTION_NONE = 0,
		AI_ACTION_MOVE,
		AI_ACTION_ATTACK,
		AI_ACTION_MAGIC,
		AI_ACTION_TURN,
		AI_ACTION_NUMS
	};

	CAIMgr(void);
	~CAIMgr(void);

	void  Clear();

	//技能辅助
	bool  IsLittleFighter();
	bool  IsFlyMagic(WORD wMagicID);
	bool  IsSuperMagic(WORD wMagicID);
	bool  IsRideFightMagic(WORD iMagicID);
	bool  IsTaoZhuangMagic(WORD iMagicID);

	bool  CanUseMagic(int wMagicID,bool bAlert = true);
	bool  IsCanFly(WORD& wMagicID);
	bool  IsCanMove();
	bool  IsCanAttack();
	bool  NeedFailDelay();

	//bool  CanDig(CSimpleCharacterNode * pChar);
	bool  HaveTrainEquip();
	bool  HaveChopEquip();

	//控制时间
	void  UpdateAttackTime(BOOL bRET = FALSE);
	void  UpdateMagicTime(WORD wMagicID = 0,BOOL bRET = FALSE);
	void  UpdateRideFightTime();
	void  UpdateMoveTime(BOOL bRET = FALSE);

	void  DelayAttackTime();
	void  DelayMoveTime();
	void  DelayMagicTime(WORD wMagicID);

	DWORD GetReserveTime(int i){ return m_dwReserveTime[i]; }
	void  SetReserveTime(int i,DWORD dwTime){ m_dwReserveTime[i] = dwTime; }

	//AI相关
	void SetLastRepeatConSkill(int iConSkillID) { m_dwCurRepeatConSkill = iConSkillID; }
	int  GetLastRepeatConSkill()				{ return m_dwCurRepeatConSkill; }
	void  SetLastPressConSkill(int iConSkillID);
	int  GetLastPressConSkill()					{ return m_dwLastPressConSkill; }

	void SetShortCutKeyMagicPos(int i)	{ m_kSetKeyMagicPos = i; }
	int GetShortCutKeyMagicPos()		{ return m_kSetKeyMagicPos; }
	void SetShortCutKeyConSkillID(int i)	{ m_iSetKeyConSkillID = i; }
	int GetShortCutKeyConSkillID()		{ return m_iSetKeyConSkillID; }

	void InitConSkill();
	void ClearAllConSkill();
	ConSkillMap& GetConSkills()					{ return m_kConSkills; }
	ConSkillData* FindConSkillByKey(DWORD dwKey);
	ConSkillData* FindConSkillByID(int id);	
	void SetAllConSkillToFirst();
	void UpdateConSkillActive();

	std::vector<ConSkillBuff>& GetConSkillBuffs();
	void UpdateConSkillBuff();	
	void AddConSubSkillBuff(WORD wMagicID, int nTotalTime);
	void RemoveConSubSkillBuff(WORD wMagicID, bool bSetToFirst);
	void ClearAllSkillBuff();
	void QuestConSubSkillBuffUpdate(int nTotalTime, int index, ConSkillData* pConSkill, WORD wMagicID);

	void  SetLastRepeatMagic(WORD wMagicID);
	WORD  GetLastRepeatMagic(){ return m_wCurRepeatMagicID; }
	void  SetLastPressMagic(WORD wMagicID);
	WORD  GetLastPressMagic(){return m_wLastPressMagicID;}
	void  SetLastDealPressMagic(WORD wMagicID);
	eAI_ACTION GetAI_Action(){ return m_eAIAction; }
	WORD  GetLastMagic(){ return m_wCurMagicID; }
	int   GetSurroundHumanCount(int iRange);  //周围怪物数目
	void  ChangeMagicState(WORD wMagicID,BOOL bAdd = TRUE);

	bool  AutoPlaceEquip(int iMagicID);
	//bool  AutoPlaceEquip(int iMagicID,bool bGreen,bool bAutoUnArm = true);
	bool  HasMagicEquip(int iMagicID,bool bGreen,bool bBothRedAndGreen = false);//bBothRedAndGreen为true时只要有红或绿都可以,此时忽略bGreen

	void  StartShake(DWORD dwDuraTime);
	void  MasterMoved();

	void PlayerRidePet(PET_SENDOUT_MAP::iterator it);
	void PlayerStartUseCloth(int iPos);
private:

	WORD   m_wCurRepeatMagicID;
	WORD   m_wLastPressMagicID;//最后一次按下却没有使用成功的魔法快捷键
	WORD   m_wLastDealPressMagicID;//最后一处理成功了m_wLastPressMagicID
	DWORD  m_dwLastDealPressMagicTime;//最后一处理成功了m_wLastDealPressMagicID的时间
	WORD   m_wCurMagicID; //当前正使用的魔法ID
	DWORD  m_dwReserveTime[plyTimeNums];
	eAI_ACTION m_eAIAction;
	bool   m_bNeedGreen; //需要上绿毒

	int	m_dwCurRepeatConSkill;
	int	m_dwLastPressConSkill;
	int	m_dwLastDealPressConSkill;	

	std::vector<ConSkillBuff> m_kConSkillBuffs;

	ConSkillMap m_kConSkills;
	int m_kSetKeyMagicPos;//设置普通技能快捷键的位置;
	int m_iSetKeyConSkillID;//设置普通技能快捷键的位置;
};

