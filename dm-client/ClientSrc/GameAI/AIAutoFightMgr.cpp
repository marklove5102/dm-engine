#include "AIAutoFightMgr.h"
#include "AIConfigMgr.h"
#include "AIMgr.h"
#include "AIPromptMgr.h"
#include "AIAutoMgr.h"
#include "GameData/GameData.h"
#include "GameData/MagicDefine.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameData/GameGlobal.h"
#include "GameControl/GameControl.h"
#include "GameData/TalkMgr.h"
#include "GameData/OtherData.h"
#include "GameMap/GameMap.h"
#include "Global/Interface/GraphicInterface.h"
#include "WndClass/GameWnd/GSceneManager.h"

CAIAutoFightMgr g_AutoFightMgr;

CAIAutoFightMgr::CAIAutoFightMgr(void)
{
	m_iAutoFire = 0;
	m_iReadyDoubleFire = 0;
}

CAIAutoFightMgr::~CAIAutoFightMgr(void)
{
}

void CAIAutoFightMgr::DoLoop()
{
	//如果在延时输入密保期间不能攻击
	if(g_AIMgr.GetReserveTime(plyMibaoLateTime) != 0 || g_OtherData.GetEnterGameTime() == 0)
		return;

	if(SELF.IsDead() || SELF.GetBoothState() || SELF.IsBianShen() || (SELF.IsOnHorse() && !SELF.GetFightOnLeopard()))
		return;

	if(GetTickCount() - g_OtherData.GetLastMovePosTime() < 300)//跨GS后立即发开启护身真气之类的魔法,然后立即移动,服务器会不响应,因此就一直waitserver,导致10秒内不能动
		return;

    if(strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0)    //五行玄关中不能释放技能
        return;


	switch(SELF.GetCareer())
	{
	case JOB_ZHANSHI:
		OpenSword_Fire();
		break;
	case JOB_FASHI:
		break;
	case JOB_DAO:
		break;
	}
}

bool CAIAutoFightMgr::DealAutoFight()
{
// 	//自己被队长微操，并且这个客户端不是队长的,什么也不做;如果是对长的,要处理被操控队员,自己及离线托管对队的AI,,这里的self是被处理对象,在CGameManager::Update处理指定对象时会赋值
// 	if (SELF.IsMicroControled() && !g_TrusteeshipData.IsCaptain())
// 	{
// 		return false;
// 	}

	//如果在延时输入密保期间不能攻击
	if(g_AIMgr.GetReserveTime(plyMibaoLateTime) != 0 || !g_AIAutoMgr.IsEnalbeWaiGua() || g_OtherData.GetEnterGameTime() == 0)
		return false;

	if(GetTickCount() - g_OtherData.GetLastMovePosTime() < 300)//跨GS后立即发开启护身真气之类的魔法,然后立即移动,服务器会不响应,因此就一直waitserver,导致10秒内不能动
		return true;//不做DealAutoFight,也不能动,所以返回true

	if(SELF.IsDead() || SELF.GetBoothState() || SELF.IsBianShen() || (SELF.IsOnHorse() && !SELF.GetFightOnLeopard()))
		return false;

	if(strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0)    //五行玄关中不能释放技能
		return false;

	switch(SELF.GetCareer())
	{
	case JOB_ZHANSHI:
		if(SELF.GetExtraState() & ES_KUANGNUWIND)//狂怒旋风不能使用魔法
			return false;

		if(DoMagicImmAfterAttack())
			return true;
		if(OpenHuShen_JinGang())
			return true;
		if(AutoWildCollide())
			return true;
		OpenSword_Fire();//开关魔法不要返回true，后面可以继续做动作
		if(OpenAttackSkill())
			return true;
		break;
	case JOB_FASHI:
		if(OpenMagicSkill())
			return true;
		if(AutoDisputeFire())
			return true;
		break;
	case JOB_DAO:
		if(OpenDaoSkill())
			return true;
		if(AutoLionCall())
			return true;
		break;
	}

	if(AutoSkill())
		return true;

	return false;
}


bool CAIAutoFightMgr::OpenSword_Fire() //打开烈火剑法等
{
	int iMagicID = 0;
	DWORD dwCount = GetTickCount();

	bool bUseFire = false;

// 	if (g_TrusteeshipData.IsTrusteeship())
// 	{
// 		TneupMember *pSelf = g_TrusteeshipData.GetSelf();
// 		if (pSelf && (pSelf->fightConfig.bAutoFire || pSelf->fightConfig.bAutoDoubleFire))
// 		{
// 			bUseFire = true;
// 		}
// 	}
// 	else 
	if(g_AICfgMgr.IsAutoFire() || g_AICfgMgr.IsDoubleFire())
	{
		bUseFire = true;
	}

	if(SELF.GetExtraState() & ES_KUANGNUWIND)//狂怒旋风不能使用魔法
		return false;

	if(bUseFire)
	{
		if(SELF.GetBoothState() || SELF.IsBianShen())
			return false;

		// 是否使用了高级剑法攻击
		if(SELF.HasMagicState(MS_ATTACKKILL))		// 攻杀 剑术 
			return false;
		if(SELF.HasMagicState(MS_REMAINSHADE))		// 残影
			return false;
		if(SELF.HasMagicState(MS_BLOODSHADE))		// 血影
			return false;

		if( dwCount < g_AIMgr.GetReserveTime(plyLastStellProtect) + 1400)// 刚使用过金刚			
			return false;
		if( dwCount < g_AIMgr.GetReserveTime(plyLastProtectSkin) + 1200)//刚使用过护身			 
			return false;

		if(dwCount < g_AIMgr.GetReserveTime(plyLastCollide) + 1700)// 突斩中
			return false;

		if(SELF.IsLearnMagic(MAGICID_ATTACK_FIRE))
		{
			if(!SELF.HasMagicState(MS_ATTACKFIRE)) //还没有开启烈火剑法
			{
				if(dwCount - g_AIMgr.GetReserveTime(plyLastFireTime) > 10000)
				{
					iMagicID = MAGICID_ATTACK_FIRE;

					if(SELF.GetNeedMP(iMagicID) >= SELF.GetMP())
					{
						g_AIPromptMgr.set_str("魔法值不够了！");
						return false;
					}
					g_AIPromptMgr.set_str("");

					g_pGameControl->SEND_Player_Attack_Magic(0,(WORD)(SELF.GetDir()),0,iMagicID);
					return true;
				}
			}
			else if(m_iAutoFire == 1 && dwCount - g_AIMgr.GetReserveTime(plyLastFireTime) > 10000)
			{
				m_iAutoFire = 2;

				if(g_AICfgMgr.IsDoubleFire())	//双烈火
				{
					g_TalkMgr.PushSysTalk("[双烈火准备就绪]",TALKCOLOR_PINK);
					m_iReadyDoubleFire = 2;
				}
				else
				{
					m_iReadyDoubleFire = 0;
				}
			}
		}
		else if(SELF.IsLearnMagic(MAGICID_THUNDER_FIRE))
		{
			if(!SELF.HasMagicState(MS_THUNDERFIRE)) //还没有开启雷霆
			{
				if(dwCount - g_AIMgr.GetReserveTime(plyLastFireTime) > 8100)
				{
					iMagicID = MAGICID_THUNDER_FIRE;

					if(SELF.GetNeedMP(iMagicID) >= SELF.GetMP())
					{
						g_AIPromptMgr.set_str("魔法值不够了！");
						return false;
					}
					g_pGameControl->SEND_Player_Attack_Magic(0,(WORD)(SELF.GetDir()),0,iMagicID);
					return true;
				}
			}
			else//已经开启了雷霆的状态 
			{
				if(m_iAutoFire == 1 && dwCount - g_AIMgr.GetReserveTime(plyLastFireTime) > 7200) //已经开启了雷霆
				{
					m_iAutoFire = 2;

					if(g_AICfgMgr.IsDoubleFire()) 	//双雷霆
					{
						g_TalkMgr.PushSysTalk("[双雷霆准备就绪]",TALKCOLOR_PINK);
						m_iReadyDoubleFire = 2;
					}
					else
					{
						m_iReadyDoubleFire = 0;
					}
				}
			}
		}
	}

	return false;
}

bool CAIAutoFightMgr::AutoMagicAttack()
{
	if (SELF.IsDead())
		return false;

	//如果在延时输入密保期间不能攻击
	if(g_AIMgr.GetReserveTime(plyMibaoLateTime) != 0 || !g_AIAutoMgr.IsEnalbeWaiGua() || g_OtherData.GetEnterGameTime() == 0)
		return false;

	if(GetTickCount() - g_OtherData.GetLastMovePosTime() < 300)//跨GS后立即发开启护身真气之类的魔法,然后立即移动,服务器会不响应,因此就一直waitserver,导致10秒内不能动
		return true;//不做DealAutoFight,也不能动,所以返回true

	if(SELF.IsDead() || SELF.GetBoothState() || SELF.IsBianShen() || (SELF.IsOnHorse() && !SELF.GetFightOnLeopard()))
		return false;

	if(strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0)    //五行玄关中不能释放技能
		return false;

	//连续魔法攻击，仅对法师和道士适用
	if(g_AICfgMgr.IsAutoMagicAttack() && SELF.GetCareer() != JOB_ZHANSHI)
	{
		// 1.先计算Press的魔法 (获得PressConSkill，如果没有则获得PressMagic)
		// 2.如果没有Press的魔法，则获得Repeat的魔法 (先获得RepeatConSkill，如果没有则获得RepeatMagic)
		// 3.如果使用的是ConSkill成功，处理ConSkill,
		WORD  wMagicID = 0;
		ConSkillData* pConSkillData = 0;
		bool bPressMagic = false;

		int iPressConSkill = g_AIMgr.GetLastPressConSkill();
		ConSkillData* pPressConSkillData = 0;
		if (iPressConSkill != 0)
		{
			pPressConSkillData = g_AIMgr.FindConSkillByID(iPressConSkill);
			if (pPressConSkillData)
			{
				wMagicID = pPressConSkillData->GetCurrentMagicID();
				if (wMagicID > 0)
				{
					pConSkillData = pPressConSkillData;
					bPressMagic = true;
				}

			}
		}
		else if(g_AIMgr.GetLastPressMagic() != 0)
		{
			wMagicID = g_AIMgr.GetLastPressMagic();
			bPressMagic = true;
		}

		if (wMagicID <= 0)
		{
			int iRepeatConSkill = g_AIMgr.GetLastRepeatConSkill();
			ConSkillData* pRepeatConSkillData = 0;
			if (iRepeatConSkill != 0)
			{
				pRepeatConSkillData = g_AIMgr.FindConSkillByID(iRepeatConSkill);
				if (pRepeatConSkillData)
				{					
					wMagicID = pRepeatConSkillData->GetCurrentMagicID();
					if (wMagicID > 0)
					{
						pConSkillData = pRepeatConSkillData;
					}
				}
			}
			else
			{
				wMagicID = g_AIMgr.GetLastRepeatMagic();
			}
		}

		if(wMagicID > 0 && g_AIMgr.CanUseMagic(wMagicID,false))
		{
			DWORD dwLockID = SELF.GetReserveData(plyMagicLockID);
			CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(dwLockID);

			if(pChar && !pChar->IsDead() && !pChar->IsNpc())
			{
				CMagicData* pMagic = SELF.FindMagic(wMagicID);
				if(pMagic)
				{
					if (bPressMagic || pMagic->HasAttr(MATTR_REPEAT) || pConSkillData != 0)
					{
						g_AICfgMgr.SetAbsoluteLock(true);
						int cx,cy;
						pChar->GetRealXY(cx,cy);
						int iSelfX,iSelfY;
						SELF.GetRealXY(iSelfX,iSelfY);

						if(abs(iSelfX-cx) <= 8 && abs(iSelfY-cy) <= 10)
						{
							if(g_pGameMgr->AttackUseMagic(wMagicID))
							{
								if (pConSkillData)
								{
									g_AIMgr.SetLastRepeatConSkill(pConSkillData->iConSkillID);
									g_AIMgr.SetLastRepeatMagic(0);
								}
								else
								{
									g_AIMgr.SetLastRepeatConSkill(0);
									g_AIMgr.SetLastRepeatMagic(wMagicID);
								}

								if(bPressMagic)
								{
									g_AIMgr.SetLastDealPressMagic(wMagicID);
								}
								return true;
							}
						}
						g_AICfgMgr.SetAbsoluteLock(false);
					}

				}
			}
		}
	}


	return false;
}

bool CAIAutoFightMgr::AutoSkill()
{
	//自动练功
	if(g_AICfgMgr.IsAutoSkill() && g_AICfgMgr.GetAutoSkillKey() != 0 && g_AICfgMgr.GetAutoSkillTime() > 0)
	{
		DWORD dwTime = GetTickCount();
		if(dwTime > g_AIMgr.GetReserveTime(plyNextMagicTime) && dwTime > (g_AIMgr.GetReserveTime(plyLastAutoSkillTime) + g_AICfgMgr.GetAutoSkillTime()))
		{
			g_AIMgr.SetReserveTime(plyLastAutoSkillTime,dwTime);
			WORD wMagicID = SELF.MagicArray().FindByShortcut(g_AICfgMgr.GetAutoSkillKey());
			if(g_pGameMgr->AttackUseMagic(wMagicID))
			{
				g_AICfgMgr.SetAbsoluteLock(true);
				g_AICfgMgr.SetAbsoluteLock(false);
				return true;
			}
		}
	}

	return false;
}

bool CAIAutoFightMgr::OpenHuShen_JinGang()
{
	if(SELF.GetBoothState() == true || SELF.IsBianShen())
		return false;

	if((SELF.GetStatus() & CS_PROTECTSKIN)) //护身真气开启中
		return false;

	if(SELF.GetExtraState() & ES_STEELPROTECT)//金刚开启中
		return false;

	if(GetTickCount() - g_AIMgr.GetReserveTime(plyLastCollide) <= 3000)// 突斩中
		return false;

	if(g_AICfgMgr.IsAutoSteelProtect() && SELF.IsLearnMagic(MAGICID_STEEL_PROTECT)) //自动金钢&有金刚护体技能
	{
		if (g_AIMgr.GetReserveTime(plyLastStellProtect) == 0 || GetTickCount() - g_AIMgr.GetReserveTime(plyLastStellProtect) > 1400)
		{
			if(g_AIMgr.CanUseMagic(MAGICID_STEEL_PROTECT,!g_pGameData->IsAutoKillMonster()))
			{   
				int iX,iY;
				int iDir = SELF.GetDir();
				SELF.GetRealXY(iX,iY);
				g_pGameControl->SEND_Player_Attack_Magic(0,iX,iY,MAGICID_STEEL_PROTECT);
				Sleep(100);
				g_pGameControl->SEND_Player_Turn(char(iDir));
				SELF.InitAction(ACTION_STAND);
				return true;
			}
		}	
	}
	else if(g_AICfgMgr.IsAutoProtectSkin() && SELF.IsLearnMagic(MAGICID_PROTECT_SKIN))//护身真气
	{
		if (g_AIMgr.GetReserveTime(plyLastProtectSkin) == 0 || GetTickCount() - g_AIMgr.GetReserveTime(plyLastProtectSkin) > 1200)
		{
			if(g_AIMgr.CanUseMagic(MAGICID_PROTECT_SKIN,!g_pGameData->IsAutoKillMonster()))
			{
				int iX,iY;
				SELF.GetRealXY(iX,iY);
				g_pGameControl->SEND_Player_Attack_Magic(0,iX,iY,MAGICID_PROTECT_SKIN);
				SELF.InitAction(ACTION_STAND);
				return true;
			}
		}	
	}
	return false;
}

bool CAIAutoFightMgr::OpenAttackSkill()
{
	//状态开启一次就不用再开启了，也许跳地图后需要重新开启
	if(g_AICfgMgr.IsDestroyShield() && SELF.IsLearnMagic(MAGICID_DESTROY_SHIELD)) //持续破盾
	{
		if(!SELF.HasMagicState(MS_DESTROYSHIELD))
		{
			if(g_pGameMgr->AttackUseMagic(MAGICID_DESTROY_SHIELD,0,!g_pGameData->IsAutoKillMonster()))
				return true;
		}
	}

	if(g_AICfgMgr.IsDestroyShell() && SELF.IsLearnMagic(MAGICID_DESTROY_SHELL))//持续破击
	{
		if(!SELF.HasMagicState(MS_DESTROYSHELL))
		{
			if(g_pGameMgr->AttackUseMagic(MAGICID_DESTROY_SHELL,0,!g_pGameData->IsAutoKillMonster()))
				return true;
		}
	}
	return false;
}

bool CAIAutoFightMgr::DoMagicImmAfterAttack()
{
	WORD wMagicID = 0;
	
	if(g_AIMgr.GetLastPressMagic() != 0)
	{
		wMagicID = g_AIMgr.GetLastPressMagic();		
	}

	if(g_AIMgr.CanUseMagic(wMagicID,false))
	{
		DWORD dwLockID = SELF.GetReserveData(plyMagicLockID);

		if(dwLockID == 0)
			dwLockID = SELF.GetReserveData(plyAttackLockID);
		
		CMagicData* pMagic = SELF.FindMagic(wMagicID);
		if(pMagic )
		{
			g_AICfgMgr.SetAbsoluteLock(true);

			if(g_pGameMgr->AttackUseMagic(wMagicID,dwLockID))
			{
				g_AIMgr.SetLastDealPressMagic(wMagicID);
				return true;
			}

			g_AICfgMgr.SetAbsoluteLock(false);
		}				   
		
	}
	
	return false;
}

bool CAIAutoFightMgr::OpenMagicSkill()
{
	if(g_AICfgMgr.IsAutoMagicProtect() && SELF.IsLearnMagic(MAGICID_MAGIC_PROTECT)) //自动魔法盾
	{
		if((SELF.GetStatus()& CS_MAGICPROTECT) == 0 && g_AIMgr.CanUseMagic(MAGICID_MAGIC_PROTECT,!g_pGameData->IsAutoKillMonster())) 
		{
			int iRealX,iRealY;
			SELF.GetRealXY(iRealX,iRealY);
			g_pGameControl->SEND_Player_Attack_Magic(0,iRealX,iRealY,MAGICID_MAGIC_PROTECT);
			SELF.InitAction(ACTION_STAND);
			return true;
		}
	}

	if(g_AICfgMgr.IsAutoDodgeSkill() && SELF.IsLearnMagic(MAGICID_DODGE_SKILL)) //自动风影盾
	{
		if((SELF.GetExtraState() & ES_DODGEPROTECT) == 0) 
		{
			if(g_pGameMgr->AttackUseMagic(MAGICID_DODGE_SKILL,SELF.GetID(),!g_pGameData->IsAutoKillMonster()))
				return true;
		}
	}

	return false;
}

bool CAIAutoFightMgr::AutoDisputeFire()
{
	if (SELF.IsDead())
		return false;

	if (!g_pGameData->IsAutoKillMonster() && ATTACK_TYPE_PEACE == SELF.GetReserveData(plyAttackType))
		return false;

	//自动抗拒
	if(g_AICfgMgr.IsAutoDispute() && SELF.IsLearnMagic(MAGICID_DISPUTE_FIRE)) //自动抗拒
	{
		if(g_AIMgr.CanUseMagic(MAGICID_DISPUTE_FIRE,!g_pGameData->IsAutoKillMonster())) 
		{
			//攻击目标
			CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyMagicLockID));
			if(pChar == NULL)
				pChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyAttackLockID));

			if(pChar != NULL  && !pChar->IsDead())
			{
				if (g_pGameData->IsAutoKillMonster() || pChar->IsHuman())
				{
					if(!HasBiggerMask(pChar)) // 对方等级不比自己低
						return false;

					int cx,cy;
					pChar->GetRealXY(cx,cy);
					int iSelfX,iSelfY;
					SELF.GetRealXY(iSelfX,iSelfY);

					if(_closePoint(iSelfX,iSelfY,cx,cy,1))
					{
						if(g_pGameMgr->AttackUseMagic(MAGICID_DISPUTE_FIRE,SELF.GetID(),!g_pGameData->IsAutoKillMonster()))
							return true;
					}
				}				
			}
		}
	}
	return false;
}

bool CAIAutoFightMgr::IsEquipDaoFu()
{
	int pos = SELF.EquipGood().FindGoodByStdmode(34,5);
	return (pos >= 0);
}

bool CAIAutoFightMgr::IsHaveDaoFu()
{
	int pos = SELF.EquipGood().FindGoodByStdmode(34,5);
	if(pos >= 0)
		return true;

	pos = SELF.PackageGood().FindGoodByStdmode(34,5);
	return (pos>=0);
}

bool CAIAutoFightMgr::OpenDaoSkill()
{
	if(SELF.GetMagic(0).GetMagicID() != 0 && !IsHaveDaoFu())//至少学了一个技能后才提示
	{
		g_AIPromptMgr.set_str("您没有道符了");
		return false;
	}

	g_AIPromptMgr.set_str(NULL); //有道符

	if(g_AICfgMgr.IsAutoProtectArmor()) //持续神圣战甲术
	{
		if (g_AIMgr.GetReserveTime(plyLastProtectArmor) == 0 || GetTickCount() - g_AIMgr.GetReserveTime(plyLastProtectArmor) > 1300)
		{
			//判断是否已经开启。。
			if(SELF.IsLearnMagic(MAGICID_PROTECT_ARMOR) && g_AIMgr.GetReserveTime(plyStartArmorTime) == 0)
			{
				if(g_pGameMgr->AttackUseMagic(MAGICID_PROTECT_ARMOR,SELF.GetID(),!g_pGameData->IsAutoKillMonster())) //给自己使用
					return true;
			}
		}	
	}

	if(g_AICfgMgr.IsAutoProtectGhost()) //幽灵盾
	{
		if (g_AIMgr.GetReserveTime(plyLastProtectGhost) == 0 || 
			GetTickCount() - g_AIMgr.GetReserveTime(plyLastProtectGhost) > SELF.GetMagicDelayTime(plyLastProtectGhost) + g_AIMgr.GetReserveTime(plyDuraGhostTime))
		{
			//判断是否已经开启。。
			if(SELF.IsLearnMagic(MAGICID_PROTECT_GHOST) && g_AIMgr.GetReserveTime(plyStartGhostTime) == 0)
			{
				if(g_pGameMgr->AttackUseMagic(MAGICID_PROTECT_GHOST,SELF.GetID(),!g_pGameData->IsAutoKillMonster())) //给自己使用
					return true;
			}
		}	
	}
	return false;
}

bool CAIAutoFightMgr::AutoLionCall()
{
	if (!g_pGameData->IsAutoKillMonster() && ATTACK_TYPE_PEACE == SELF.GetReserveData(plyAttackType))
		return false;

	if(!IsHaveDaoFu())
		return false;

	if(!g_AICfgMgr.IsAutoLionCall() || !SELF.IsLearnMagic(MAGICID_LION_CALL) || GetTickCount() - g_AIMgr.GetReserveTime(plyLastLionCall) <  1500)
		return false;

	if(g_AIMgr.CanUseMagic(MAGICID_LION_CALL,!g_pGameData->IsAutoKillMonster())) 
	{
		//攻击目标
		CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyMagicLockID));
		if(pChar == NULL)
			pChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyAttackLockID));

		if(pChar != NULL  && !pChar->IsDead())
		{
			if (g_pGameData->IsAutoKillMonster() || pChar->IsHuman())
			{
				if(!HasBiggerMask(pChar)) // 对方等级不比自己低
					return false;

				int cx,cy;
				pChar->GetRealXY(cx,cy);
				int iSelfX,iSelfY;
				SELF.GetRealXY(iSelfX,iSelfY);

				if(_closePoint(iSelfX,iSelfY,cx,cy,1))
				{
					int iDir = GetDir8(cx,cy,iSelfX,iSelfY);
					g_pGameControl->SEND_Player_Turn(char(iDir));
					SELF.SetDir(iDir);

					if(g_pGameMgr->AttackUseMagic(MAGICID_LION_CALL,pChar->GetID(),!g_pGameData->IsAutoKillMonster()))
					{
						SELF.InitAction(ACTION_STAND);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool CAIAutoFightMgr::HasBiggerMask(CSimpleCharacter* pChar)
{
	if(pChar == NULL)
		return true;

	int iOtherLevel = pChar->GetMaskLevel();
	int iMyLevel    = SELF.GetMaskLevel();

	if(iMyLevel <= 0) // 自己未出封号
	{
		if(iOtherLevel <= 0 ) // 攻击目标未出封号,只能去撞了
			return true;
		else // 攻击目标已经出封号了,撞不动啊,砍他
			return false;
	}
	else  // 出了封号
	{
		if(iOtherLevel <= 0 ) // 攻击目标未出封号
			return true;
		else if(iMyLevel == iOtherLevel ) //封号相等比三味真火等级
		{
			if(SELF.GetSanWeiFireLevel() > pChar->GetSanWeiFireLevel())
				return true;
		}
		else if(iMyLevel < iOtherLevel ) //封号比对方低
			return false;
		else
			return true;
	}
	return false;
}

bool CAIAutoFightMgr::AutoWildCollide() //自动野蛮冲撞
{
	if (SELF.IsDead())
		return false;

	if (!g_pGameData->IsAutoKillMonster() && ATTACK_TYPE_PEACE == SELF.GetReserveData(plyAttackType))
		return false;

	if(!g_AICfgMgr.IsAutoWildCollide() || GetTickCount() - g_AIMgr.GetReserveTime(plyLastCollide) <=  3000 - SELF.GetAttackSpeed() * 80) //自动抗拒
		return false;

	int wMagicID = 0;
	if(SELF.IsLearnMagic(MAGICID_SUDDEN_KILL)) //学了突斩
		wMagicID = MAGICID_SUDDEN_KILL;
	else if(SELF.IsLearnMagic(MAGICID_WILD_COLLIDE)) //学了野蛮
		wMagicID = MAGICID_WILD_COLLIDE;
	else //两个都没有学习
		return false;

	DWORD dwCount = GetTickCount();

	if( dwCount < g_AIMgr.GetReserveTime(plyLastStellProtect) + 1400)// 刚使用过金刚			
		return false;
	if( dwCount < g_AIMgr.GetReserveTime(plyLastProtectSkin) + 1200)//刚使用过护身			 
		return false;

	if(dwCount - g_AIMgr.GetReserveTime(plyLastFireTime) < 1080)
		return false;

	if(g_AIMgr.CanUseMagic(wMagicID,!g_pGameData->IsAutoKillMonster())) 
	{
		//攻击目标
		//CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyMagicLockID));
		//if(pChar == NULL)

		CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyAttackLockID));

		if(pChar != NULL  && !pChar->IsDead())
		{
			if(!HasBiggerMask(pChar)) // 对方等级不比自己低
				return false;

			int cx,cy;
			pChar->GetRealXY(cx,cy);
			int iSelfX,iSelfY;
			SELF.GetRealXY(iSelfX,iSelfY);

			if(_closePoint(iSelfX,iSelfY,cx,cy,1))
			{
				int iDir = GetDir8(cx,cy,iSelfX,iSelfY);
				if(iDir != SELF.GetDir())
				{
					g_pGameControl->SEND_Player_Turn(char(iDir));
					SELF.SetDir(iDir);
				}

				//g_pGameMgr->AttackUseMagic(wMagicID,pChar->GetID());
				//output_debug("cx:%d,cy:%d,selfx:%d,selfy:%d\r\n",cx,cy,iSelfX,iSelfY);

				g_pGameControl->SEND_Player_Attack_Magic(0,WORD(iDir),0,wMagicID);
				SELF.InitAction(ACTION_STAND);


				return true;
			}
		}
	}

	return false;
}
