#pragma once

#include "Global/Global.h"
#include "GameData/SimpleCharacter.h"

class CAIAutoFightMgr
{
public:
	CAIAutoFightMgr(void);
	~CAIAutoFightMgr(void);

	void DoLoop();
	bool DealAutoFight();

	void SetReadyDFire(int i){m_iReadyDoubleFire = i;}
	int  GetReadyDFike(){ return m_iReadyDoubleFire; }

	void SetAutoFire(int i){ m_iAutoFire = i; }
	int  GetAutoFire()  { return m_iAutoFire; }
	bool AutoWildCollide(); //自动野蛮冲撞
	bool AutoLionCall();//自动狮子吼
	bool AutoDisputeFire();//自动抗拒
	bool AutoMagicAttack();  //自动魔法攻击
	bool HasBiggerMask(CSimpleCharacter* pChar);

	//战士
	bool OpenSword_Fire(); //使用烈火系列，要开开关的
	bool OpenHuShen_JinGang(); //自动护身和金钢
	bool OpenAttackSkill();
	bool DoMagicImmAfterAttack();//在攻击后首先使用魔法

	//法师
	bool OpenMagicSkill();


	//道士
	bool IsEquipDaoFu();
	bool IsHaveDaoFu();
	bool OpenDaoSkill();

private:
	void AutoTianGuan();	
	
	bool AutoSkill();  //自动练功

	int m_iAutoFire;         //自动烈火状态准备
	int m_iReadyDoubleFire;  //准备好了的双烈火的次数
};

extern CAIAutoFightMgr g_AutoFightMgr;
