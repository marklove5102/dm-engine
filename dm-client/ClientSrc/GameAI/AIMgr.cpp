#include "AIMgr.h"
#include "GameData/MagicDefine.h"
#include "GameData/GameData.h"
#include "AIGoodMgr.h"
#include "GameControl/GameControl.h"
#include "AIPromptMgr.h"
#include "GameData/TalkMgr.h"
#include "WndClass/GameWnd/GameManager.h"
#include "BaseClass/Control/Control.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/WooolStoreData.h"
#include "GameData/MagicCtrlMgr.h"


CAIMgr::CAIMgr(void)
{
	Clear();

	InitConSkill();
}

CAIMgr::~CAIMgr(void)
{
	ClearAllConSkill();
}

void CAIMgr::Clear()
{
	m_wCurRepeatMagicID = 0;
	m_wLastPressMagicID = 0;
	m_wLastDealPressMagicID = 0;
	m_dwLastDealPressMagicTime = 0;
	m_wCurMagicID = 0;
	memset(m_dwReserveTime,0,plyTimeNums*sizeof(DWORD));
	m_eAIAction = AI_ACTION_NONE;

	m_bNeedGreen = false;

	m_kSetKeyMagicPos = 0;
	m_iSetKeyConSkillID = 0;

	m_dwCurRepeatConSkill = 0;
	m_dwLastPressConSkill = 0;
	m_dwLastDealPressConSkill = 0;

	ClearAllSkillBuff();
}


bool CAIMgr::IsLittleFighter()
{
	if(SELF.GetCareer() != JOB_ZHANSHI || SELF.GetLevel() > 27)
		return false;

	return true;
}

bool CAIMgr::IsFlyMagic(WORD wMagicID)
{
	if(wMagicID == MAGICID_ENCHANTER_WILD 
		|| wMagicID == MAGICID_ESCAPE_QUICK 
		|| wMagicID == MAGICID_SHIFT_TRANSPORT)
		return true;

	return false;
}
bool CAIMgr::IsSuperMagic(WORD wMagicID)
{
	if(wMagicID >= MAGICID_SUPER_FIREBALL  && wMagicID <= MAGICID_SUPER_CURSE)
		return true;
	return false;
}
bool CAIMgr::IsRideFightMagic(WORD iMagicID)
{
	if(iMagicID >= MAGICID_AOFENGCUT_ATTACK && iMagicID <= MAGICID_DULINGBO_ATTACK)
		return true;

	return false;
}

bool CAIMgr::NeedFailDelay()
{
	if(m_dwReserveTime[plyLastFailTime] == 0)
		return false;

	if(GetTickCount() < m_dwReserveTime[plyLastFailTime] + 200)
		return true;

	return false;
}

void CAIMgr::UpdateAttackTime(BOOL bRET)
{
	m_eAIAction = AI_ACTION_ATTACK;
	m_dwReserveTime[plyLastAttackTime] = GetTickCount();
	m_dwReserveTime[plyNextAttackTime] = m_dwReserveTime[plyLastAttackTime] + 1080 - SELF.GetAttackSpeed() * 80;

	//只有要更新的时间比原来的时候后才设置，不然会导致原来限制的发包提前，就会出现FAIL包
	DWORD dwCount = m_dwReserveTime[plyLastAttackTime] + 550;
	if(dwCount > m_dwReserveTime[plyNextMoveTime])
		m_dwReserveTime[plyNextMoveTime] = dwCount;

	//攻击之后的魔法动作不用限制，攻击动作做完后立即可以使用魔法
	//dwCount = m_dwReserveTime[plyLastAttackTime] + 1080;
	//if(dwCount > m_dwReserveTime[plyNextMagicTime])
	//	m_dwReserveTime[plyNextMagicTime] = dwCount;
}

void CAIMgr::UpdateMoveTime(BOOL bRET)
{
	m_eAIAction = AI_ACTION_MOVE;

	m_dwReserveTime[plyLastMoveTime] = GetTickCount();
	m_dwReserveTime[plyNextMoveTime] = m_dwReserveTime[plyLastMoveTime] + 600;// 640;

	//只有要更新的时间比原来的时候后才设置，不然会导致原来限制的发包提前，就会出现FAIL包
	DWORD dwCount = m_dwReserveTime[plyLastMoveTime] + 640;
	if(dwCount > m_dwReserveTime[plyNextAttackTime])
		m_dwReserveTime[plyNextAttackTime] = dwCount;

	dwCount = m_dwReserveTime[plyLastMoveTime] + 620;
	if(dwCount > m_dwReserveTime[plyNextMagicTime])
		m_dwReserveTime[plyNextMagicTime] = dwCount;
}

void  CAIMgr::UpdateRideFightTime()
{
	m_dwReserveTime[plyRideFightTime] = GetTickCount();
}

void CAIMgr::UpdateMagicTime(WORD wMagicID,BOOL bRET)
{
	DWORD dwMagicDelay = 960;//两个魔法动作的间隔
	DWORD dwMoveDelay = 550;
	DWORD dwAttackDelay = 500;
	bool  bSwitch = false;

	if(wMagicID == 0)
		wMagicID = m_wCurMagicID;

	//if (wMagicID == MAGICID_FAZHEN_CARRYON)//法阵的延迟特殊处理
	//{
	//	m_dwReserveTime[plyNextMagicTime] = GetTickCount() + 1800;
	//	return ;
	//}
	CMagicData* pMagic =  SELF.FindMagic(wMagicID);
	if(pMagic == NULL)
		return;

	//if(pMagic->HasAttr(MATTR_REPEAT))
	//	m_wCurRepeatMagicID = wMagicID;

	dwMagicDelay = pMagic->GetMagicDelay();


	switch(wMagicID)
	{
	case MAGICID_FIREBALL: 		// 攻击性魔法,使用magicdata中的delay
	case MAGICID_SUPER_FIREBALL:
	case MAGICID_ADV_FIREBALL:
	case MAGICID_POISON_MAGIC:
	case MAGICID_SUPER_POISON_MAGIC:
	case MAGICID_HELL_FIRE:
	case MAGICID_TRACE_THUNDER:
	case MAGICID_THUNDER:
	case MAGICID_PROTECT_SYMBOL:
	case MAGICID_FIRE_WALL:
	case MAGICID_BLOW_FIRE:
	case MAGICID_HELL_THUNDER:
	case MAGICID_GOD_SAY:
	case MAGICID_ICE_STORM:
	case MAGICID_CURSE:
	case MAGICID_SUPER_CURSE:
	case MAGICID_ICE_ARROW:
	case MAGICID_ICE_DRAG:
	case MAGICID_ICE_ARRAY:
	case MAGICID_MULTI_THUDER:
	case MAGICID_ICE_WHIRLWIND:
	case MAGICID_FIRE_JUJU:
	case MAGICID_DRAGON_LIGHT:
	case MAGICID_FIRE_SHIELD_ATTACK:
		dwAttackDelay = 720;
		break;
	case MAGICID_LION_CALL:
	case MAGICID_SUDDEN_KILL:
	case MAGICID_WILD_COLLIDE:
		dwAttackDelay = 850;
		break;

	case MAGICID_ATTACK_STICK:  // 开关性魔法不做限制
	case MAGICID_ATTACK_MOON:
	case MAGICID_ATTACK_FIRE:
	case MAGICID_WHOLE_MOON:
	case MAGICID_THUNDER_FIRE:
	case MAGICID_DESTROY_SHELL:
	case MAGICID_DESTROY_SHIELD:
	case MAGICID_SUPER_ATTACK_KILL:
		dwMoveDelay = 20;
		dwAttackDelay = 20;
		bSwitch = true;
		break;
	case MAGICID_PROTECT_SKIN:
	case MAGICID_STEEL_PROTECT:
	case MAGICID_MAGIC_PROTECT:
	case MAGICID_DODGE_SKILL:
	case MAGICID_DAOZUN_JIANGLIN:
		dwAttackDelay = 720;
		break;
	case MAGICID_ENCHANTER_WILD:
	case MAGICID_ESCAPE_QUICK:
	case MAGICID_SHIFT_TRANSPORT:
		dwAttackDelay = 1800;
		break;
	case MAGICID_SHADOWKILL8:		
		dwAttackDelay = 850;
		break;
	default:  // 治疗性魔法
		break;
	}

	DWORD dwCount = GetTickCount();
	m_wCurMagicID = wMagicID;
	m_eAIAction = AI_ACTION_MAGIC;
	m_dwReserveTime[plyLastMagicTime] = dwCount;

	switch(wMagicID)
	{
	case MAGICID_DISPUTE_FIRE:
		m_dwReserveTime[plyLastDisputeTime] = dwCount;
		break;
	case MAGICID_ATTACK_FIRE:
	case MAGICID_THUNDER_FIRE:
		m_dwReserveTime[plyLastFireTime] = dwCount;
		break;
	case MAGICID_DESTROY_WEAPON:
		m_dwReserveTime[plyLastDWTime] = dwCount;
		break;
	case MAGICID_ENCHANTER_WILD:
	case MAGICID_ESCAPE_QUICK:
	case MAGICID_SHIFT_TRANSPORT:
		m_dwReserveTime[plyLastFlyTime] = dwCount;
		break;
	case MAGICID_PROTECT_GHOST:
		m_dwReserveTime[plyLastProtectGhost] = dwCount;
		break;
	case MAGICID_PROTECT_ARMOR:
		m_dwReserveTime[plyLastProtectArmor] = dwCount;
		break;
	case MAGICID_LION_CALL:
		m_dwReserveTime[plyLastLionCall] = dwCount;
		break;
	case MAGICID_WILD_COLLIDE:
	case MAGICID_SUDDEN_KILL:
		m_dwReserveTime[plyLastCollide] = dwCount;
		break;
	case MAGICID_SUPER_ATTACK_KILL:
		m_dwReserveTime[plyLastSAttackKill] = dwCount;
		break;
	case MAGICID_MAGIC_PROTECT:
		m_dwReserveTime[plyLastMagicProtect] = dwCount;
		break;
	case MAGICID_DODGE_SKILL:
		m_dwReserveTime[plyLastDodgeProtect] = dwCount;
		break;
	case MAGICID_STEEL_PROTECT:
		m_dwReserveTime[plyLastStellProtect] = dwCount;
		break;
	case MAGICID_PROTECT_SKIN:
		m_dwReserveTime[plyLastProtectSkin] = dwCount;
		break;
	case MAGICID_ZHI_YAN:
		m_dwReserveTime[plyLastZhiYanTime] = dwCount;
		break;
	case MAGICID_SUPER_FIREBALL:
		m_dwReserveTime[plyLastSuperFireBall] = dwCount;
		break;
	case MAGICID_SWT:
	case MAGICID_LSJ:
	case MAGICID_JYL:
		m_dwReserveTime[plyLastTaoZhuangTime] = dwCount;
		break;
	case MAGICID_COLDSTROM:
		m_dwReserveTime[plyLastColdstrom] = dwCount;
		break;
	}

	m_dwReserveTime[plyNextMagicTime] = m_dwReserveTime[plyLastMagicTime] + dwMagicDelay;
	if(bSwitch)
	{
		SELF.SetWaitServer(false,true);
	}
	else
	{
		//只有要更新的时间比原来的时候后才设置，不然会导致原来限制的发包提前，就会出现FAIL包
		dwCount = m_dwReserveTime[plyLastMagicTime] + dwMoveDelay;
		if(dwCount > m_dwReserveTime[plyNextMoveTime])
			m_dwReserveTime[plyNextMoveTime] = dwCount;
	}

	dwCount = m_dwReserveTime[plyLastMagicTime] + dwAttackDelay;
	if(dwCount > m_dwReserveTime[plyNextAttackTime])
		m_dwReserveTime[plyNextAttackTime] = dwCount;


}

void CAIMgr::DelayAttackTime()
{
	DWORD dwCount = GetTickCount();

	if(m_dwReserveTime[plyNextAttackTime] < dwCount)
		m_dwReserveTime[plyLastAttackTime] = dwCount;
	else
		m_dwReserveTime[plyLastAttackTime] = m_dwReserveTime[plyNextAttackTime];

	UpdateAttackTime(TRUE);
}

void CAIMgr::DelayMoveTime()
{
	DWORD dwCount = GetTickCount();

	if(m_dwReserveTime[plyNextMoveTime] < dwCount)
		m_dwReserveTime[plyLastMoveTime] = dwCount;
	else
		m_dwReserveTime[plyLastMoveTime] = m_dwReserveTime[plyNextMoveTime];


	//UpdateMoveTime(TRUE);
	m_dwReserveTime[plyNextMoveTime] = m_dwReserveTime[plyLastMoveTime] + 640 - 20 + 200;
	//只有要更新的时间比原来的时候后才设置，不然会导致原来限制的发包提前，就会出现FAIL包
	if(m_dwReserveTime[plyNextMoveTime] > m_dwReserveTime[plyNextAttackTime])
		m_dwReserveTime[plyNextAttackTime] = m_dwReserveTime[plyNextMoveTime];

	dwCount = m_dwReserveTime[plyLastMoveTime] + 500;
	if(dwCount > m_dwReserveTime[plyNextMagicTime])
		m_dwReserveTime[plyNextMagicTime] = dwCount;

}

void CAIMgr::DelayMagicTime(WORD wMagicID)
{
	DWORD dwCount = GetTickCount();

	if(m_dwReserveTime[plyNextMagicTime] < dwCount)
		m_dwReserveTime[plyLastMagicTime] = dwCount;
	else
		m_dwReserveTime[plyLastMagicTime] = m_dwReserveTime[plyNextMagicTime];

	UpdateMagicTime(wMagicID,TRUE);
}

void  CAIMgr::SetLastDealPressMagic(WORD wMagicID)
{
	m_wLastDealPressMagicID = wMagicID;
	m_dwLastDealPressMagicTime = GetTickCount();	
}

void  CAIMgr::SetLastRepeatMagic(WORD wMagicID)
{
	if (wMagicID == 0)
	{
		m_wCurRepeatMagicID = 0;
	}
	else
	{
		CMagicData* pMagic = SELF.FindMagic(wMagicID);
		if(pMagic && pMagic->HasAttr(MATTR_REPEAT))
			m_wCurRepeatMagicID = wMagicID;	
	}
}

void  CAIMgr::SetLastPressMagic(WORD wMagicID)
{
	if(wMagicID != 0)
	{
		//if(m_wCurRepeatMagicID == 0)
		//	return;		

		DWORD tCount = GetTickCount();
		if(wMagicID == m_wCurMagicID)
		{
			if(tCount < m_dwReserveTime[plyNextMagicTime])
				return;
		}
		//如果离上一次使用的时间太近不记录
		CMagicData* pMagic =  SELF.FindMagic(m_wLastDealPressMagicID);
		if(pMagic)
		{
			if(tCount - m_dwLastDealPressMagicTime < pMagic->GetMagicDelay())
				return;
		}
		//如果这个魔法的魔法效果还没有结束也不记录,如蝙蝠
		if(g_pMagicCtrl->FindMagicByAll(wMagicID,SELF.GetID()))
			return;
	}

	m_wLastPressMagicID = wMagicID;
}

bool CAIMgr::CanUseMagic(int wMagicID,bool bAlert)
{
	if(wMagicID == 0)
		return false;

	DWORD tCount = GetTickCount();
	if(SELF.IsWaitServer() || tCount < m_dwReserveTime[plyNextMagicTime])
        return false;	

	//由于下面的原因失败不再尝试执行PressMagic,否则会出一直提示元气值不足之类的提示
	g_AIMgr.SetLastPressMagic(0);
	g_AIMgr.SetLastPressConSkill(0);

	if(SELF.IsOnHorse() && !SELF.GetFightOnLeopard())
		return false;

	CMagicData* pMagic = SELF.FindMagic(wMagicID);
	if((pMagic == NULL || pMagic->IsDisable())/* && wMagicID != MAGICID_FAZHEN_CARRYON*/)
	{
		return false;
	}

	if(/*wMagicID != MAGICID_FAZHEN_CARRYON && */pMagic->GetNeedManaValue() >= SELF.GetMP())
	{
		//魔法值不够了！！！！
		g_AIPromptMgr.set_str("魔法值不够了！");
		return false;
	}

	//强袭烈焰盾攻击 需要处于激活闪烁状态
	if (wMagicID == MAGICID_FIRE_SHIELD_ATTACK && !SELF.Is8DunShan())
	{
		g_AIPromptMgr.set_str("强袭烈焰盾还未激活！");
		return false;
	}

	if (/*g_dwServerVersion >= 1070 && */!HasMagicEquip(wMagicID,false,true))
	{
		return false;
	}


	g_AIPromptMgr.set_str("");

	if(IsRideFightMagic(wMagicID))
	{
		if(tCount < m_dwReserveTime[plyRideFightTime] + 30000)
		{
			if (bAlert)
			{
				g_TalkMgr.PushSysTalk("您的技能还未冷却，请等待一段时间。");
			}
			return false;
		}

		//if((SELF.GetSkillPower() * 2) < SELF.GetSkillPowerMax())//元气值满50%就可以使用组合技能
		//{
		//	if (bAlert)
		//	{
		//		g_TalkMgr.PushSysTalk("元气未满，还不能施展强大的骑战技能！");
		//	}
		//	return false;
		//}

		if((SELF.GetExtraState() & ES_FROST) || SELF.HasStatus(CS_LULL))
		{
			if (bAlert)
			{
				g_TalkMgr.PushSysTalk("被麻痹或冰冻时不能骑战技能。");
			}
			return false;
		}

		return true;
	}

	switch(wMagicID)
	{
	//强化小火球,50级以后每5级时间减5秒,还要打了一个标记以后才有效,所以客户端就不限制了
	//case MAGICID_SUPER_FIREBALL:
	//	if(tCount < m_dwReserveTime[plyLastSuperFireBall] + 15000)//15秒只能使用一次
	//	{
	//		if (bAlert)
	//		{
	//			char strTemp[128]={0};
	//			sprintf(strTemp,"无法连续使用该技能，请%d秒后再使用。",(m_dwReserveTime[plyLastSuperFireBall] + 15000 + 999 - tCount) / 1000);
	//			g_TalkMgr.PushSysTalk(strTemp);
	//		}
	//		return false;
	//	}
	//	break;
	case MAGICID_SWT:
	case MAGICID_LSJ:
	case MAGICID_JYL:
		if(tCount < m_dwReserveTime[plyLastTaoZhuangTime] + 15000)//15秒只能使用一次
		{
			if (bAlert)
			{
				char strTemp[128]={0};
				sprintf(strTemp,"无法连续使用该技能，请%d秒后再使用。",(m_dwReserveTime[plyLastTaoZhuangTime] + 15000 + 999 - tCount) / 1000);
				g_TalkMgr.PushSysTalk(strTemp);
			}
			return false;
		}
		break;
	case MAGICID_COLDSTROM:
		if(tCount < m_dwReserveTime[plyLastColdstrom] + 30000)//30秒只能使用一次
		{
			if (bAlert)
			{
				char strTemp[128]={0};
				sprintf(strTemp,"无法连续使用该技能，请%d秒后再使用。",(m_dwReserveTime[plyLastColdstrom] + 30000 + 999 - tCount) / 1000);
				g_TalkMgr.PushSysTalk(strTemp);
			}
			return false;
		}
		break;	
	case MAGICID_ZHI_YAN:
		{
			DWORD dwTime = 30000;
			if (SELF.GetCareer() != 0)
			{
				dwTime = 60000;
			}

			if(tCount < m_dwReserveTime[plyLastZhiYanTime] + dwTime)//180秒只能使用一次
			{
				if (bAlert)
				{
					char strTemp[128]={0};
					sprintf(strTemp,"无法连续使用该技能，请%d秒后再使用。",(m_dwReserveTime[plyLastZhiYanTime] + dwTime + 999 - tCount) / 1000);
					g_TalkMgr.PushSysTalk(strTemp);
				}
				return false;
			}

			int iTileX,iTileY;
			int iSelfX,iSelfY;
			SELF.GetRealXY(iSelfX,iSelfY);
			CSimpleCharacterNode *pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
			if(pChar && !pChar->IsDead() && pChar->GetRaceType() != CHARACTER_NPC)
				pChar->GetRealXY(iTileX,iTileY);
			else
				g_pGameMgr->GetMouseTile(iTileX,iTileY);

			//if(abs(iTileX - iSelfX) > 1 || abs(iTileY - iSelfY) > 1)
			//{
			//	g_TalkMgr.PushSysTalk("超过攻击范围，技能使用失败。");
			//	return false;
			//}
		}
		break;

	default:
		break;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////
//能用某个魔法否？
bool CAIMgr::IsCanFly(WORD& wMagicID)
{
	DWORD dwCount = GetTickCount();

	if((SELF.IsOnHorse() && !SELF.GetFightOnLeopard()) || SELF.IsDead())
		return false;

	//if(SELF.IsFlying()) //已经在飞了
	//	return false;

	if(SELF.IsBianShen()) //正变身着呢
		return false;

	wMagicID = 0; //瞬移技能
	if(SELF.IsLearnMagic(MAGICID_ENCHANTER_WILD))
		wMagicID = MAGICID_ENCHANTER_WILD;
	else if(SELF.IsLearnMagic(MAGICID_ESCAPE_QUICK))
		wMagicID = MAGICID_ESCAPE_QUICK;
	else if(SELF.IsLearnMagic(MAGICID_SHIFT_TRANSPORT))
		wMagicID = MAGICID_SHIFT_TRANSPORT;

	if(wMagicID == 0)
		return false;

	if(!CanUseMagic(wMagicID)) //时间没有到等
	{
		return false;
	}

	//移形换影70级之后每升5级，移形换影的使用CD减少5秒，直至最终无CD。并且在无CD时，表现去除最后还原的特效。还要打了一个标记以后才有效,所以客户端就不限制了
	//DWORD dwShiftTransportTime = 60000;
	//if (SELF.GetLevel() > 70 && g_dwServerVersion >= 1160)//移形换影70级之后每升5级，移形换影的使用CD减少5秒，直至最终无CD。并且在无CD时，表现去除最后还原的特效。
	//{
	//	DWORD dwTime = (SELF.GetLevel() - 70) / 5 * 5;
	//	if (dwTime >= dwShiftTransportTime)
	//	{
	//		dwShiftTransportTime = 0;
	//	}
	//	else
	//	{
	//		dwShiftTransportTime -= dwTime;
	//	}
	//}

	//if(wMagicID == MAGICID_SHIFT_TRANSPORT	&& dwCount < m_dwReserveTime[plyLastFlyTime] + dwShiftTransportTime)
	//	return false;

	return true;
}


bool CAIMgr::IsCanMove()
{
	if(GetTickCount() < m_dwReserveTime[plyNextMoveTime])
		return false;
	if(SELF.IsWaitServer())
		return false;

	return true;
}

bool CAIMgr::IsCanAttack()
{
	if(SELF.IsWaitServer() || GetTickCount() < m_dwReserveTime[plyNextAttackTime])
		return false;

	return true;
}

// bool CAIMgr::CanDig(CSimpleCharacterNode * pChar)
// {
// 	if(pChar == NULL || !pChar->IsDead())
// 		return false;
// 
// 	if(!pChar->IsMonster())
// 		return false;
// 
// 	switch(pChar->GetRaceNo())
// 	{
// 	case 1:
// 	case 2:
// 	case 22:
// 	case 24:
// 	case 31:
// 	case 33:
// 	case 36:
// 	case 37:
// 	case 121:
// 	case 131:
// 	case 180:
// 	case 181:
// 	case 191:
// 	case 179:
// 	case 243:
// 	case 244:
// 	case 245:
// 	case 58://宝箱怪
// 		return true;
// 	}
// 	return false;
// }

bool CAIMgr::HaveChopEquip()
{
	CGood& tmp = SELF.GetEquipGood(ITEM_POS_WEAPON);

	if(tmp.GetID() != 0 && tmp.GetStdMode() == 6 && tmp.GetShape() == 19)
		return true;

	return false;
}

bool CAIMgr::HaveTrainEquip()
{
	CGood& tmp = SELF.GetEquipGood(ITEM_POS_WEAPON);

	if(tmp.GetID() != 0 && tmp.GetStdMode() == 5 && tmp.GetShape() == 40)
		return true;

	return false;
}

int CAIMgr::GetSurroundHumanCount(int iRange)
{
	if(iRange < 0)
		return 0;

	if(iRange > 12) iRange = 12;

	int iMonsterNum = 0; //计算周围的怪物数
	int iRealX,iRealY;
	SELF.GetRealXY(iRealX,iRealY);

	for(int iY = iRealY - iRange;iY <= iRealY + iRange;iY++)
	{
		for(int iX = iRealX - iRange;iX <= iRealX + iRange;iX++)
		{
			CSimpleCharacter* pChar = MapArray.GetSimpleCharacterHead(iX,iY);
			if(pChar == NULL || pChar->IsDead())
				continue;

			if(!pChar->IsHuman())
				iMonsterNum++;
		}
	}
	return iMonsterNum;
}

void CAIMgr::ChangeMagicState(WORD wMagicID,BOOL bAdd)
{
	CMagicData* pMagic = SELF.FindMagic(wMagicID);
	if(pMagic == NULL)
		return;

	int iMState = 0;
	switch(wMagicID)
	{
	case MAGICID_ATTACK_STICK:				// 刺杀剑术默认开启
		iMState = MS_ATTACKSTICK;
		break;
	case MAGICID_SUPER_ATTACK_KILL:
		iMState = MS_SATTACKKILL;
		break;
	case MAGICID_DESTROY_WEAPON:
		iMState = MS_DESTROYWEAPON;
		break;
	}

	if(bAdd)
		SELF.AddMagicState(iMState);
	else
		SELF.RemoveMagicState(iMState);
}

void CAIMgr::StartShake(DWORD dwDuraTime)
{
	m_dwReserveTime[plyStartShakeTime] =  GetTickCount();
	m_dwReserveTime[plyEndShakeTime] =  m_dwReserveTime[plyStartShakeTime] + dwDuraTime;
}

bool CAIMgr::AutoPlaceEquip(int iMagicID)
{
	//自动上毒
	switch(iMagicID)
	{
	case MAGICID_POISON_MAGIC:
	case MAGICID_SUPER_POISON_MAGIC:
	case MAGICID_CURSE:
	case MAGICID_SUPER_CURSE://强化诅咒术
		{
			int iPos = ITEM_POS_RIGHT_BANGLE;  //
			CGood tmp = SELF.GetEquipGood(iPos); //

			//是否装备了正确的毒药
			if(g_AIGoodMgr.IsValidPoisonCurse(tmp,m_bNeedGreen))
			{
				m_bNeedGreen = !m_bNeedGreen;
				return true;
			}

			if(SELF.GetLevel() >= MAX_LEVEL)
			{
				iPos = ITEM_POS_OTHERS;
				tmp = SELF.GetEquipGood(iPos);

				if(g_AIGoodMgr.IsValidPoisonCurse(tmp,m_bNeedGreen))
				{
					m_bNeedGreen = !m_bNeedGreen;
					return true;
				}
			}

			//判断:是需要毒呢,还是需要人偶???
			bool bNeedPoison = false;
			if(iMagicID == MAGICID_POISON_MAGIC || iMagicID == MAGICID_SUPER_POISON_MAGIC)
				bNeedPoison = true;
			else
				bNeedPoison = false;

			bool bFlag = false;
			for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
			{
				CGood& temp1 = SELF.GetPackageGood(i);

				if(g_AIGoodMgr.IsValidPoisonCurse(temp1,m_bNeedGreen,bNeedPoison))
				{
					temp1.SetPos(i);
					g_pGameControl->SEND_Package_Object_Arm(temp1,iPos);
					bFlag = true;
					break;
				}
			}

			if(!bFlag) //没有替换类型的毒
			{
				//没有替换类型的毒、原来的毒不动
				if(g_AIGoodMgr.IsValidPoisonCurse(tmp,!m_bNeedGreen,bNeedPoison)) 
					return true;

				for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
				{
					CGood& temp1 = SELF.GetPackageGood(i);

					if(g_AIGoodMgr.IsValidPoisonCurse(temp1,!m_bNeedGreen,bNeedPoison))
					{
						temp1.SetPos(i);
						g_pGameControl->SEND_Package_Object_Arm(temp1,iPos);
						bFlag = true;
						break;
					}
				}

				if(!bFlag) //将原来的东西卸下来
				{
					g_pGameControl->SEND_Package_Object_Unarm(tmp,iPos);
				}
			}
			m_bNeedGreen = !m_bNeedGreen;
			return bFlag;
		}
		break;
	//自动上符
	case MAGICID_PROTECT_SYMBOL:
	case MAGICID_PROTECT_GHOST:
	case MAGICID_PROTECT_ARMOR:
	case MAGICID_LOCK_MONSTER:
	case MAGICID_CALL_MONSTER:
	case MAGICID_HIDE_SKILL:
	case MAGICID_HIDE_ALL:
	case MAGICID_TEAM_HEAL:
	case MAGICID_CALL_JOSS:
	case MAGICID_CONTROL_CORPSE:
	case MAGICID_REPLACE_MAGIC:
	case MAGICID_SOUL_WALL:
	case MAGICID_ENCALL_MONSTER:
	case MAGICID_GODLIGHT_SKILL:
	case MAGICID_FIRE_JUJU:
	case MAGICID_DESTROY_POISON:
		{
			int iPos = ITEM_POS_RIGHT_BANGLE;
			CGood tmp = SELF.GetEquipGood(iPos);

			if(g_AIGoodMgr.IsValidDaoFu(tmp))
				return true;

			if(SELF.GetLevel() >= MAX_LEVEL)
			{
				iPos = ITEM_POS_OTHERS;
				tmp = SELF.GetEquipGood(iPos);

				if(g_AIGoodMgr.IsValidDaoFu(tmp))
					return true;
			}

			bool bFlag = false;
			for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
			{
				CGood& temp1 = SELF.GetPackageGood(i);
				if(g_AIGoodMgr.IsValidDaoFu(temp1))
				{
					temp1.SetPos(i);
					g_pGameControl->SEND_Package_Object_Arm(temp1,iPos);
					bFlag = true;
					break;
				}
			}
			if(!bFlag)
			{
				g_pGameControl->SEND_Package_Object_Unarm(tmp,iPos);
			}
			return bFlag;
		}
		break;
	}
	return false;
}

/*bool  CAIMgr::AutoPlaceEquip(int iMagicID,bool bGreen,bool bAutoUnArm)
{
	//自动上毒
	switch(iMagicID)
	{
	case MAGICID_POISON_MAGIC:
	case MAGICID_CURSE:
		{
			int iPos = ITEM_POS_RIGHT_BANGLE;
			CGood tmp = SELF.GetEquipGood(iPos);

			//是否装备了正确的毒药
			if(g_AIGoodMgr.IsValidPoisonCurse(tmp,bGreen))
				return true;

			if(SELF.GetLevel() >= MAX_LEVEL)
			{
				iPos = ITEM_POS_OTHERS;
				tmp = SELF.GetEquipGood(iPos);

				if(g_AIGoodMgr.IsValidPoisonCurse(tmp,bGreen))
					return true;
			}

			//判断:是需要毒呢,还是需要人偶
			bool bNeedPoison = false;
			if(iMagicID == MAGICID_POISON_MAGIC)
				 bNeedPoison = true;
			else bNeedPoison = false;

			bool bFlag = false;
			for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
			{
				CGood& temp1 = SELF.GetPackageGood(i);

				if(g_AIGoodMgr.IsValidPoisonCurse(temp1,bGreen,bNeedPoison))
				{
					temp1.SetPos(i);
					g_pGameControl->SEND_Package_Object_Arm(temp1,iPos);
					bFlag = true;
					break;
				}
			}

			if(!bFlag) //没有替换类型的毒
			{
				//没有替换类型的毒、原来的毒不动
				if(g_AIGoodMgr.IsValidPoisonCurse(tmp,!bGreen,bNeedPoison)) 
					return true;

				for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
				{
					CGood& temp1 = SELF.GetPackageGood(i);

					if(g_AIGoodMgr.IsValidPoisonCurse(temp1,!bGreen,bNeedPoison))
					{
						temp1.SetPos(i);
						g_pGameControl->SEND_Package_Object_Arm(temp1,iPos);
						bFlag = true;
						break;
					}
				}

				if(!bFlag && bAutoUnArm) g_pGameControl->SEND_Package_Object_Unarm(tmp,iPos); //将原来的东西卸下来
			}
			return bFlag;
		}
		break;
		//自动上符
	case MAGICID_PROTECT_SYMBOL:
	case MAGICID_PROTECT_GHOST:
	case MAGICID_PROTECT_ARMOR:
	case MAGICID_LOCK_MONSTER:
	case MAGICID_CALL_MONSTER:
	case MAGICID_HIDE_SKILL:
	case MAGICID_HIDE_ALL:
	case MAGICID_TEAM_HEAL:
	case MAGICID_CALL_JOSS:
	case MAGICID_CONTROL_CORPSE:
	case MAGICID_REPLACE_MAGIC:
	case MAGICID_SOUL_WALL:
	case MAGICID_ENCALL_MONSTER:
	case MAGICID_GODLIGHT_SKILL:
	case MAGICID_FIRE_JUJU:
	case MAGICID_DESTROY_POISON:
		{
			int iPos = ITEM_POS_RIGHT_BANGLE;
			CGood tmp = SELF.GetEquipGood(iPos);

			if(g_AIGoodMgr.IsValidDaoFu(tmp))
				return true;

			if(SELF.GetLevel() >= MAX_LEVEL)
			{
				iPos = ITEM_POS_OTHERS;
				tmp = SELF.GetEquipGood(iPos);

				if(g_AIGoodMgr.IsValidDaoFu(tmp))
					return true;
			}

			bool bFlag = false;
			for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
			{
				CGood& temp1 = SELF.GetPackageGood(i);
				if(g_AIGoodMgr.IsValidDaoFu(temp1))
				{
					temp1.SetPos(i);
					g_pGameControl->SEND_Package_Object_Arm(temp1,iPos);
					bFlag = true;
					break;
				}
			}
			if(!bFlag)
			{
				g_pGameControl->SEND_Package_Object_Unarm(tmp,iPos);
			}
			return bFlag;
		}
		break;
	}
	
	return false;
}
*/
bool CAIMgr::HasMagicEquip(int iMagicID,bool bGreen,bool bBothRedAndGreen)
{
	//毒
	switch(iMagicID)
	{
	case MAGICID_POISON_MAGIC:
	case MAGICID_SUPER_POISON_MAGIC: //强化放毒术
	case MAGICID_CURSE:
	case MAGICID_SUPER_CURSE://强化诅咒术
		{
			//判断:是需要毒呢,还是需要人偶???
			bool bNeedPoison = false;
			if(iMagicID == MAGICID_POISON_MAGIC|| iMagicID == MAGICID_SUPER_POISON_MAGIC)
				bNeedPoison = true;
			else
				bNeedPoison = false;

			int iPos = ITEM_POS_RIGHT_BANGLE;  //
			CGood &tmp = SELF.GetEquipGood(iPos); //

			//是否装备毒药
			if (bBothRedAndGreen)
			{
				if(g_AIGoodMgr.IsValidPoisonCurse(tmp,true,bNeedPoison))
				{
					return true;
				}
				if(g_AIGoodMgr.IsValidPoisonCurse(tmp,false,bNeedPoison))
				{
					return true;
				}
			}
			else
			{
				if(g_AIGoodMgr.IsValidPoisonCurse(tmp,bGreen,bNeedPoison))
				{
					return true;
				}
			}

			if(SELF.GetLevel() >= MAX_LEVEL)
			{
				iPos = ITEM_POS_OTHERS;
				CGood &tmp1 = SELF.GetEquipGood(iPos);

				if (bBothRedAndGreen)
				{
					if(g_AIGoodMgr.IsValidPoisonCurse(tmp1,true,bNeedPoison))
					{
						return true;
					}
					if(g_AIGoodMgr.IsValidPoisonCurse(tmp1,false,bNeedPoison))
					{
						return true;
					}
				}
				else
				{
					if(g_AIGoodMgr.IsValidPoisonCurse(tmp1,bGreen,bNeedPoison))
					{
						return true;
					}
				}
			}

			for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
			{
				CGood& temp1 = SELF.GetPackageGood(i);

				if (bBothRedAndGreen)
				{
					if(g_AIGoodMgr.IsValidPoisonCurse(temp1,true,bNeedPoison))
					{
						return true;
					}
					if(g_AIGoodMgr.IsValidPoisonCurse(temp1,false,bNeedPoison))
					{
						return true;
					}
				}
				else
				{
					if(g_AIGoodMgr.IsValidPoisonCurse(temp1,bGreen,bNeedPoison))
					{
						return true;
					}
				}
			}

			return false;
		}
		break;
		//自动上符
	case MAGICID_PROTECT_SYMBOL:
	case MAGICID_PROTECT_GHOST:
	case MAGICID_PROTECT_ARMOR:
	case MAGICID_LOCK_MONSTER:
	case MAGICID_CALL_MONSTER:
	case MAGICID_HIDE_SKILL:
	case MAGICID_HIDE_ALL:
	case MAGICID_TEAM_HEAL:
	case MAGICID_CALL_JOSS:
	case MAGICID_CONTROL_CORPSE:
	case MAGICID_REPLACE_MAGIC:
	case MAGICID_SOUL_WALL:
	case MAGICID_ENCALL_MONSTER:
	case MAGICID_GODLIGHT_SKILL:
	case MAGICID_FIRE_JUJU:
	case MAGICID_DESTROY_POISON:
		{
			int iPos = ITEM_POS_RIGHT_BANGLE;
			CGood &tmp = SELF.GetEquipGood(iPos);

			if(g_AIGoodMgr.IsValidDaoFu(tmp))
				return true;

			if(SELF.GetLevel() >= MAX_LEVEL)
			{
				iPos = ITEM_POS_OTHERS;
				CGood &tmp1 = SELF.GetEquipGood(iPos);

				if(g_AIGoodMgr.IsValidDaoFu(tmp1))
					return true;
			}

			for(int i = 0; i < MAX_PACKAGE_ELEMENT - 6;i++)
			{
				CGood& temp1 = SELF.GetPackageGood(i);
				if(g_AIGoodMgr.IsValidDaoFu(temp1))
				{
					return true;
				}
			}

			return false;
		}
		break;
	}
	//其它的不限
	return true;
}

void CAIMgr::MasterMoved()
{
	SELF.GetMapPathFinder().SetOnRoute(false);

	//if(g_pMerSys)//主角移动
	//{
	//	g_pMerSys->MasterMove(); 
	//}
}

void CAIMgr::PlayerRidePet(PET_SENDOUT_MAP::iterator it)
{
	PET_SENDOUT &SendOutPet = it->second;

	if(SendOutPet.dwPetItemID == 0 || (SendOutPet.byPetType&0x07) == 0)//要骑的不是转生后的宠物
	{
		g_pGameControl->SEND_Player_Ride_Horse();
		return;
	}

	int iType = 0;
	if((SendOutPet.byPetType&0x07) > 0) 
	{
		iType = 1;
	}
	////如果已经穿了御兽天袍/御兽天衣，则直接骑上
	//if(SELF.GetBody() == 16)//无双寄宠不用指定衣服
	{
		g_pGameControl->SEND_Player_Ride_Horse(iType,SendOutPet.dwPetItemID);
		return;
	}

	const char * strCloth[] =
	{
		"御兽天袍",
		"御兽天衣"
	};

	int iClothIdx = SELF.IsMale()?0:1;

	//没穿看包裹里有没有，先查带期限的，再查次数的
	DWORD dwGoodID = 0;
	int iPos = 0;

	for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
	{
		CGood &tmpGood = SELF.GetPackageGood(i);

		if( strcmp(tmpGood.GetName(),strCloth[iClothIdx]) == 0 ) 
		{
			if(tmpGood.GetID() != 0 && tmpGood.GetStdMode() == 49) //带期限的御兽天袍/御兽天衣
			{
				dwGoodID = tmpGood.GetID();
				iPos = i;
				break;
			}
			else if(tmpGood.GetID() != 0 && tmpGood.GetStdMode() == 37 && tmpGood.GetDura() > 0)  //带次数限制的御兽天袍/御兽天衣，且可使用次数大于0
			{
				dwGoodID = tmpGood.GetID();
				iPos = i;
				continue;
			}
			else
				continue;				
		}
	}

	if(dwGoodID == 0)//没有衣服，快捷消费
	{
		const char * strPtCloth[] =
		{
			"御兽天袍(包月)",
			"御兽天衣(包月)"
		};

		g_WooolStoreMgr.SetQuickBuyItem(strPtCloth[iClothIdx]);
		CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

		if(QuickBuyData.pItem)
		{
			char cTemp[128]={0};
			sprintf(cTemp,"需要御兽天袍/御兽天衣才能骑乘灵兽，要马上购买%s并穿上吗？单价%d元宝。",strCloth[iClothIdx],QuickBuyData.pItem->iPrice);
			QuickBuyData.iUseAfterBuyType = 4;
			QuickBuyData.strMsg = cTemp;
			QuickBuyData.iType = 1;
			g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);
		}
		else
		{
			char cTemp[128]={0};
			sprintf(cTemp,"您需要穿上%s才能骑乘灵兽，请到商城购买%s后再骑乘。",strCloth[iClothIdx],strCloth[iClothIdx]);
			g_MsgBoxMgr.PopSimpleAlert(cTemp);
		}

		return;
	}
	else//找到了给他穿上
	{
		CGood &tmpGood = SELF.GetPackageGood(iPos);
		if(tmpGood.GetStdMode() == 49 && tmpGood.GetRecordTime() == 0)//还没有开启
		{
			char str[256];

			sprintf(str,"需要%s才能骑乘灵兽,你的%s还没有开启，是否现在开启？",strCloth[iClothIdx],tmpGood.GetName());
			g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_START_USE_YSTPY,iPos);
			return;
		}
		else if(tmpGood.GetStdMode() == 37)
		{
			char str[256];
			sprintf(str,"骑乘灵兽后将减少%s可使用次数，是否继续？",tmpGood.GetName());
			g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_START_USE_YSTPY,iPos);
			return;
		}

		SELF.SetReserveData(pubLastCloseId,tmpGood.GetID());
		g_pGameControl->SEND_Use_Object(iPos);
		g_pGameControl->SEND_Player_Ride_Horse(iType,SendOutPet.dwPetItemID);
	}	
}

void CAIMgr::PlayerStartUseCloth(int iPos)
{
	if(iPos < 0 || iPos >= MAX_PACKAGE_ELEMENT)
		return;
	
	SELF.SetReserveData(pubLastCloseId,SELF.GetPackageGood(iPos).GetID());
	g_pGameControl->SEND_Use_Object(iPos);	
	g_pGameControl->SEND_Player_Ride_Horse(1,0);
}

bool CAIMgr::IsTaoZhuangMagic( WORD iMagicID )
{
	if(iMagicID >= MAGICID_SWT && iMagicID <= MAGICID_JYL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CAIMgr::SetLastPressConSkill(int iConSkillID)
{
	if(iConSkillID != 0)
	{
		if(m_wCurRepeatMagicID == 0 && m_dwCurRepeatConSkill == 0)
			return;

		DWORD tCount = GetTickCount();

		//如果离上一次使用的时间太近不记录
		CMagicData* pMagic =  SELF.FindMagic(m_wLastDealPressMagicID);
		if(pMagic)
		{
			if(tCount - m_dwLastDealPressMagicTime < pMagic->GetMagicDelay())
				return;
		}
	}

	m_dwLastPressConSkill = iConSkillID;
}

ConSkillData* CAIMgr::FindConSkillByKey(DWORD dwKey)
{
	ConSkillMap::iterator i = m_kConSkills.begin();
	for (;i != m_kConSkills.end();++i)
	{
		if ( i->second->dwKey == dwKey )
		{
			return i->second;
		}
	}

	return 0;
}

ConSkillData* CAIMgr::FindConSkillByID(int id)
{
	if (id == 0)
		return 0;

	ConSkillMap::iterator i = m_kConSkills.find(id);
	if (i != m_kConSkills.end())
	{
		return i->second;		
	}
	return 0;
}

void CAIMgr::SetAllConSkillToFirst()
{
	ConSkillMap::iterator i = m_kConSkills.begin();
	for (;i != m_kConSkills.end();++i)
	{
		i->second->SetToFirst();		
	}
}

void CAIMgr::AddConSubSkillBuff(WORD wMagicID, int time)
{
	if (wMagicID != 0 && time > 0)
	{
		ConSkillMap::iterator i = m_kConSkills.begin();
		for (;i != m_kConSkills.end();++i)
		{
			ConSkillData* pConSkillData2 = i->second;
			if (pConSkillData2 == 0 || !pConSkillData2->bActive)
				continue;

			for (int j = 0; j < pConSkillData2->nMagicCount; ++j)
			{
				if (pConSkillData2->wMagicIDs[j].wMagicID == wMagicID)
				{
					if (pConSkillData2)
					{
						pConSkillData2->iCurrentIndex = j;
						pConSkillData2->ChangeToNextConSubSkill();

						ConSubSkill* pConSubSkill = pConSkillData2->GetCurrentConSubSkill();
						if (pConSubSkill && pConSubSkill->nBuffID >= 0)
						{
							QuestConSubSkillBuffUpdate(time, pConSubSkill->nBuffID, pConSkillData2, wMagicID);
						}
					}

					return;
				}
			}
		}
	}	
}

void CAIMgr::RemoveConSubSkillBuff(WORD wMagicID, bool bSetToFirst)
{
	for (int i = 0; i < m_kConSkillBuffs.size(); ++i)
	{
		ConSkillBuff& lConSkillBuff = m_kConSkillBuffs[i];
		if (lConSkillBuff.bActive)
		{
			if (lConSkillBuff.wMagicID == wMagicID)
			{
				lConSkillBuff.Deactive(bSetToFirst);
			}
		}
	}
}

void CAIMgr::QuestConSubSkillBuffUpdate(int nTotalTime, int index, ConSkillData* pConSkill, WORD wMagicID)
{
	if (index < 0)
		return;
	if (index >= m_kConSkillBuffs.size())
		return;

	m_kConSkillBuffs[index].bActive = true;
	m_kConSkillBuffs[index].nTotalTime = nTotalTime;
	m_kConSkillBuffs[index].nStartTime = GetTickCount();
	m_kConSkillBuffs[index].pConSkill = pConSkill;
	m_kConSkillBuffs[index].wMagicID = wMagicID;
}

std::vector<ConSkillBuff>& CAIMgr::GetConSkillBuffs()
{	
	return m_kConSkillBuffs;
}

void CAIMgr::UpdateConSkillBuff()
{
	DWORD dwNowTime = GetTickCount();

	for (int i = 0; i < m_kConSkillBuffs.size(); ++i)
	{
		ConSkillBuff& lConSkillBuff = m_kConSkillBuffs[i];

		if (lConSkillBuff.nTotalTime <= 0)
		{
			lConSkillBuff.Deactive();
			continue;
		}
		if (lConSkillBuff.bActive)
		{
			lConSkillBuff.nNowTime = dwNowTime;
			if (dwNowTime - lConSkillBuff.nStartTime >= lConSkillBuff.nTotalTime)
			{
				lConSkillBuff.Deactive(true);
			}
		}
	}	
}


void CAIMgr::ClearAllConSkill()
{
	ConSkillMap::iterator i = m_kConSkills.begin();
	for (;i != m_kConSkills.end();++i)
	{
		delete i->second;		
	}
	m_kConSkills.clear();
}

void CAIMgr::UpdateConSkillActive()
{
	for(ConSkillMap::iterator i=m_kConSkills.begin();i!=m_kConSkills.end();++i)
	{
		ConSkillData* lConSkillData = i->second;
		if (lConSkillData == 0)
			continue;

		lConSkillData->bActive = true;

		for (int isub = 0; isub < lConSkillData->nMagicCount;++isub)
		{
			ConSubSkill& kConSubSkill = lConSkillData->wMagicIDs[isub];
			CMagicData *pMagic = SELF.FindMagic(kConSubSkill.wMagicID);
			if (pMagic == 0 || pMagic->GetMagicLevel() < 4)
			{
				lConSkillData->dwKey = 0;
				lConSkillData->bActive = false;
				continue;
			}
		}
	}
}


void CAIMgr::ClearAllSkillBuff()
{
	for (int i = 0; i < m_kConSkillBuffs.size(); ++i)
	{
		ConSkillBuff& lConSkillBuff = m_kConSkillBuffs[i];
		lConSkillBuff.Deactive();
	}
	SetAllConSkillToFirst();
}


void CAIMgr::InitConSkill()
{
	m_kConSkillBuffs.clear();	
	ClearAllConSkill();


	m_kConSkillBuffs.resize(5);

	m_kConSkillBuffs[0].bActive = false;
	m_kConSkillBuffs[0].nBuffID = 0;
	m_kConSkillBuffs[0].nIcon = 308;
	m_kConSkillBuffs[0].nName = "使用火炎刀造成的伤害随技能等级的提升而提高，持续6秒\n(仅对大师级或大师级以上火炎刀有效)";
	m_kConSkillBuffs[0].nTotalTime = 6000;
	m_kConSkillBuffs[0].nStartTime = 0;
	m_kConSkillBuffs[0].pConSkill = 0;


	m_kConSkillBuffs[1].bActive = false;
	m_kConSkillBuffs[1].nBuffID = 1;
	m_kConSkillBuffs[1].nIcon = 344;
	m_kConSkillBuffs[1].nName = "下一次爆裂火焰造成的伤害100%触发暴击效果\n(仅大师级或大师级以上爆裂火焰有效)";
	m_kConSkillBuffs[1].nTotalTime = 6000;
	m_kConSkillBuffs[1].nStartTime = 0;
	m_kConSkillBuffs[1].pConSkill = 0;


	m_kConSkillBuffs[2].bActive = false;
	m_kConSkillBuffs[2].nBuffID = 2;
	m_kConSkillBuffs[2].nIcon = 352;
	m_kConSkillBuffs[2].nName = "野蛮冲撞强化\n(仅大师级或大师级以上野蛮冲撞有效)";
	m_kConSkillBuffs[2].nTotalTime = 6000;
	m_kConSkillBuffs[2].nStartTime = 0;
	m_kConSkillBuffs[2].pConSkill = 0;

	m_kConSkillBuffs[3].bActive = false;
	m_kConSkillBuffs[3].nBuffID = 3;
	m_kConSkillBuffs[3].nIcon = 310;
	m_kConSkillBuffs[3].nName = "毒爆\n(仅大师级或大师级以上施毒术有效)";
	m_kConSkillBuffs[3].nTotalTime = 6000;
	m_kConSkillBuffs[3].nStartTime = 0;
	m_kConSkillBuffs[3].pConSkill = 0;

	m_kConSkillBuffs[4].bActive = false;
	m_kConSkillBuffs[4].nBuffID = 4;
	m_kConSkillBuffs[4].nIcon = 388;
	m_kConSkillBuffs[4].nName = "毒爆\n(仅大师级或大师级以上诅咒术有效)";
	m_kConSkillBuffs[4].nTotalTime = 6000;
	m_kConSkillBuffs[4].nStartTime = 0;
	m_kConSkillBuffs[4].pConSkill = 0;

	//法师连击技能
	{		
		ConSkillData* pSkillData = new ConSkillData();
		pSkillData->iConSkillID = 1;
		pSkillData->nCareer = JOB_FASHI;
		pSkillData->dwKey = 0;
		pSkillData->nMagicCount = 3;
		pSkillData->wMagicIDs[0].wMagicID = MAGICID_FIREBALL;
		pSkillData->wMagicIDs[0].nName = "小火球";
		pSkillData->wMagicIDs[0].nBuffID = -1;
		pSkillData->wMagicIDs[0].nInfo = "当前技能等级为大师级或大师级以上，成功施放小火球，可以触发火炎刀加强效果，在该效果持续阶段火炎刀伤害得到加强，加强的效果和小火球的等级相关。";
		pSkillData->wMagicIDs[1].wMagicID = MAGICID_ADV_FIREBALL;
		pSkillData->wMagicIDs[1].nName = "火炎刀";
		pSkillData->wMagicIDs[1].nBuffID = 0;
		pSkillData->wMagicIDs[1].nInfo = "当前技能等级为大师级或大师级以上，在火炎刀加强效果下，施法火炎刀，不仅可以增强火炎刀的伤害效果，还可能触发强化爆裂火焰效果，触发概率和火炎刀技能等级相关。";
		pSkillData->wMagicIDs[2].wMagicID = MAGICID_BLOW_FIRE;
		pSkillData->wMagicIDs[2].nName = "爆裂火焰";
		pSkillData->wMagicIDs[2].nBuffID = 1;
		pSkillData->wMagicIDs[2].nInfo = "当前技能等级为大师级或大师级以上，在强化爆裂火焰效果下，本次爆裂火炎必然暴击，同时强化爆裂火焰效果消失。";
		m_kConSkills[pSkillData->iConSkillID] = pSkillData;	
	}


	//战士连击技能
	{		
		ConSkillData* pSkillData = new ConSkillData();
		pSkillData->iConSkillID = 2;
		pSkillData->nCareer = JOB_ZHANSHI;
		pSkillData->dwKey = 0;
		pSkillData->nMagicCount = 2;
		pSkillData->wMagicIDs[0].wMagicID = MAGICID_PROTECT_SKIN;
		pSkillData->wMagicIDs[0].nName = "护身真气";
		pSkillData->wMagicIDs[0].nBuffID = -1;
		pSkillData->wMagicIDs[0].nInfo = "当前技能等级为大师级或大师级以上，施法护身真气，可以获得额外的一个护体效果，在该效果下使用野蛮冲撞会产生一个回血效果。";
		pSkillData->wMagicIDs[1].wMagicID = MAGICID_WILD_COLLIDE;
		pSkillData->wMagicIDs[1].nName = "野蛮冲撞";
		pSkillData->wMagicIDs[1].nBuffID = 2;
		pSkillData->wMagicIDs[1].nInfo = "当前技能等级为大师级或大师级以上，护体效果下，施法野蛮冲撞，玩家可以获得一个百分比加血效果，效果强弱由玩家的护身真气和野蛮冲撞的技能等级相关，同时护体效果消失。";
		m_kConSkills[pSkillData->iConSkillID] = pSkillData;	
	}

	//道士连击技能
	{		
		ConSkillData* pSkillData = new ConSkillData();
		pSkillData->iConSkillID = 3;
		pSkillData->nCareer = JOB_DAO;
		pSkillData->dwKey = 0;
		pSkillData->nMagicCount = 2;
		pSkillData->wMagicIDs[0].wMagicID = MAGICID_PROTECT_SYMBOL;
		pSkillData->wMagicIDs[0].nName = "灵魂道符";
		pSkillData->wMagicIDs[0].nBuffID = -1;
		pSkillData->wMagicIDs[0].nInfo = "当前技能等级为大师级或大师级以上，施法灵魂道符，有可能触发毒爆的效果，触发的概率和灵魂道符的等级相关。";
		pSkillData->wMagicIDs[1].wMagicID = MAGICID_POISON_MAGIC;
		pSkillData->wMagicIDs[1].nName = "施毒术";
		pSkillData->wMagicIDs[1].nBuffID = 3;
		pSkillData->wMagicIDs[1].nInfo = "当前技能等级为大师级或大师级以上，毒爆效果下施毒术伤害急剧提升，另外当玩家身上有中毒效果时伤害还会额外加深，同时毒爆效果消失。";
		m_kConSkills[pSkillData->iConSkillID] = pSkillData;	
	}
	{		
		ConSkillData* pSkillData = new ConSkillData();
		pSkillData->iConSkillID = 4;
		pSkillData->nCareer = JOB_DAO;
		pSkillData->dwKey = 0;
		pSkillData->nMagicCount = 2;
		pSkillData->wMagicIDs[0].wMagicID = MAGICID_PROTECT_SYMBOL;
		pSkillData->wMagicIDs[0].nName = "灵魂道符";
		pSkillData->wMagicIDs[0].nBuffID = -1;
		pSkillData->wMagicIDs[0].nInfo = "当前技能等级为大师级或大师级以上，施法灵魂道符，有可能触发毒爆的效果，触发的概率和灵魂道符的等级相关。";
		pSkillData->wMagicIDs[1].wMagicID = MAGICID_CURSE;
		pSkillData->wMagicIDs[1].nName = "诅咒术";
		pSkillData->wMagicIDs[1].nBuffID = 4;
		pSkillData->wMagicIDs[1].nInfo = "当前技能等级为大师级或大师级以上，毒爆效果下诅咒术伤害急剧提升，另外当玩家身上有诅咒效果时伤害还会额外加深，同时毒爆效果消失。";
		m_kConSkills[pSkillData->iConSkillID] = pSkillData;	
	}
}
