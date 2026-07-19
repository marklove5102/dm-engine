#include "AutoKillMonsterMgr.h"
#include "GameData/GameData.h"
#include "GameAI/AIMgr.h"
#include "GameAI/PathFinder.h"
#include "GameMap/GameMap.h"
#include "GameControl/GameControl.h"
#include "GameData/MagicDefine.h"
#include "GameData/GameGlobal.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameAI/AIPromptMgr.h"
#include "GameData/TalkMgr.h"
#include "GameAI/AIAutoFightMgr.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/OtherData.h"
#include "AIConfigMgr.h"
#include "AIAutoPickMgr.h"
#include "BaseClass/Control/Control.h"


CAutoKillMonsterMgr g_AutoKillMonsterMgr;



struct SortByDirectionFunctor
{
	int iX;
	int iY;
	SortByDirectionFunctor(int x, int y):iX(x),iY(y){}
	bool operator()(DWORD left, DWORD right) const
	{
		CSimpleCharacterNode* pLeft = MapArray.FindSimpleCharacter(left);
		CSimpleCharacterNode* pRight = MapArray.FindSimpleCharacter(right);		

		if (pLeft && pRight)
		{
			int iLeftX,iLeftY;
			pLeft->GetRealXY(iLeftX,iLeftY);

			int iRightX,iRightY;
			pRight->GetRealXY(iRightX,iRightY);

			if ((iLeftX - iX)*(iLeftX - iX) + (iLeftY - iY)*(iLeftY - iY) < (iRightX - iX)*(iRightX - iX) + (iRightY - iY)*(iRightY - iY))
			{
				return true;
			}
		}
		
		return false;
	}
};



CAutoKillMonsterMgr::CAutoKillMonsterMgr(void)
{
	Clear();
}

CAutoKillMonsterMgr::~CAutoKillMonsterMgr(void)
{
}

bool CAutoKillMonsterMgr::DoLoop()
{
	if (!g_pGameData->IsAutoKillMonster())
		return false;

	if (SELF.IsDead() || SELF.GetBoothState() || SELF.IsBianShen())
	{
		SetEnabled(false);
		g_pControl->Msg(MSG_CTRL_AUTOKILLBTN,0);
		return false;
	}

	m_isMonsterSorted = false;

	if (g_AutoPickMgr.NeedPickUpGood())
	{
		//当周围没怪的时候，有物品要捡，但捡不到，会一直原地不动
		//CAIAutoPickMgr::AutoMoveFaild()将捡不到的物品删除
		return false;
	}

	SELF.GetRealXY(m_iSelfX, m_iSelfY);

	m_bAutoSteelProtect = g_AICfgMgr.IsAutoSteelProtect();
	m_bAutoProtectSkin = g_AICfgMgr.IsAutoProtectSkin();
	m_bAutoWildCollideKill = g_AICfgMgr.IsAutoWildCollide();
	m_bDestroyShield = g_AICfgMgr.IsDestroyShield();
	m_bDestroyShell = g_AICfgMgr.IsDestroyShell();
	m_bAutoFire = g_AICfgMgr.IsAutoFire();
	m_bAutoDoubleFire = g_AICfgMgr.IsDoubleFire();

	m_bAutoMagicProtect = g_AICfgMgr.IsAutoMagicProtect();
	m_bAutoDispute = g_AICfgMgr.IsAutoDispute();
	m_bAutoDragonLight = true;
	m_bAutoThunder = true;
	m_bAutoFireBall = true;

	m_bAutoProtectGhost = g_AICfgMgr.IsAutoProtectGhost();
	m_bAutoProtectArmor = g_AICfgMgr.IsAutoProtectArmor();
	m_bAutoLionCall = g_AICfgMgr.IsAutoLionCall();
	m_bAutoFireJuju = true;
	m_bAutoProtectSymbol = true;
	m_bAutoPoison = true;
	m_bAutoSuperFireBall = true;

	if (AIDefendPolicy())
		return true;


	CSimpleCharacterNode* pNode = GetCanAttackTarget();
	if(pNode)
	{
		if (SELF.GetMapPathFinder().IsOnRoute())
		{
			SELF.GetMapPathFinder().SetOnRoute(0);
		}

		if (!AIAttackPolicy(pNode))
		{
			Reach(pNode);
		}
		
		return true;
	}

	if (m_vMonsters.empty())
	{
		if (AIRandAutoWalk())
		{
			return true;
		}
	}
	

	return true;
}

void CAutoKillMonsterMgr::Clear()
{
	m_vMonsters.clear();
	m_dwSelfAttackID = 0;

	m_iSelfX = 0;
	m_iSelfY = 0;

	m_iPosionTimes = 0;

	for (int i = 0; i < AKM_PetNum; ++i)
	{
		aPet[i] = 0;
	}

	m_bAllowKillMonsterEnabled = true;
	
	m_bAutoSteelProtect = true;
	m_bAutoProtectSkin = true;
	m_bAutoWildCollideKill = true;
	m_bDestroyShield = true;		
	m_bDestroyShell = true;		
	m_bAutoDoubleFire = true;		
	m_bAutoFire = true;	

	m_bAutoMagicProtect = true;
	m_bAutoDragonLight = true;   
	m_bAutoThunder = true;       
	m_bAutoFireBall = true;      


	m_bAutoProtectGhost = true;
	m_bAutoProtectArmor = true;
	m_bAutoLionCall = true;
	m_bAutoFireJuju = true;      
	m_bAutoProtectSymbol = true; 
	m_bAutoPoison = true;        

	m_bAutoSuperFireBall = true; 
	m_iLastAutoWalkX = m_iLastAutoWalkY = 0;

}

void CAutoKillMonsterMgr::SetEnabled(bool val)
{
	if (g_pGameData->IsAutoKillMonster() == val)
		return;

	g_pGameData->SetAutoKillMonster(val);	

	SELF.GetMapPathFinder().SetOnRoute(0);
	g_MapFinder.SetWalking(false);	
	SELF.GetCNextAction().Clear();

	if (g_pGameData->IsAutoKillMonster())
	{
		g_TalkMgr.PushSysTalk("[自动打怪开启]",TALKCOLOR_GREEN);
		if(SELF.IsOnHorse() && !SELF.GetFightOnLeopard())//自动下马
		{
			g_pGameControl->SEND_Player_Ride_Horse();
		}

		g_AutoPickMgr.SetLimitRange(8);

		if (m_bAllowKillMonsterEnabled)
			ReLoadAllowKillMonsters();

		// 开启的时候将地图中怪加入列表
		ReAddMonsters();

		// 继续攻击正在攻击的目标
		DWORD dwLockID = SELF.GetReserveData(plyMagicLockID);
		if(dwLockID == 0)
			dwLockID = SELF.GetReserveData(plyAttackLockID);
		if (dwLockID != 0)
		{
			CSimpleCharacterNode * pChar = MapArray.FindSimpleCharacter(dwLockID);
			if (pChar && IsCanAttack(pChar))
			{
				SetSelfAttackID(dwLockID);
				_AddMonster(pChar, true);
			}
		}
	}
	else
	{
		g_TalkMgr.PushSysTalk("[自动打怪关闭]",TALKCOLOR_GREEN);

		SetSelfAttackID(0);
		g_AutoPickMgr.SetLimitRange(3);
	}
}

void CAutoKillMonsterMgr::ReLoadAllowKillMonsters()
{
	m_allowKillMonsters.clear();

	string str = g_pStreamMgr->GetConfigStr("AllowKillMonsters","");
	if (str.empty())
		return;

	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(",", start);

		if (pos == start)
		{
			start = pos + 1;
		}
		else if (pos == string::npos)
		{
			m_allowKillMonsters[str.substr(start)] = 0;
			break;
		}
		else
		{
			m_allowKillMonsters[str.substr(start, pos - start)] = 0;
			start = pos + 1;
		}

		start = str.find_first_not_of(",", start);

	} while (pos != string::npos);

}

void CAutoKillMonsterMgr::ReAddMonsters()
{
	m_vMonsters.clear();

	int iSelfX,iSelfY;
	SELF.GetRealXY(iSelfX,iSelfY);

	MiniChar& vChar = MapArray.GetMiniChar();
	MiniChar::iterator itr;
	for(itr = vChar.begin();itr != vChar.end();itr++)
	{
		CSimpleCharacterNode* pNode = itr->second;
		if(pNode)
		{
			int cx,cy;
			pNode->GetRealXY(cx,cy);

			if (_closePoint(iSelfX,iSelfY,cx,cy,1) || pNode->GetHP() == 100)
			{
				_AddMonster(pNode, false);
			}
		}
	}
}

void CAutoKillMonsterMgr::SetSelfAttackID(DWORD val) 
{
	m_dwSelfAttackID = val;
}

void CAutoKillMonsterMgr::_AddMonster(DWORD id, bool checkexist)
{
	if (id == 0)
		return;

	CSimpleCharacterNode* pNode = MapArray.FindSimpleCharacter(id);
	_AddMonster(pNode, checkexist);
}

void CAutoKillMonsterMgr::_AddMonster(CSimpleCharacterNode* pNode, bool checkexist)
{
	if (pNode == 0)
		return;

	if (!IsCanAttack(pNode))
		return;

	if (checkexist && HasAttackMe(pNode->GetID()))
		return;

	m_vMonsters.push_back(pNode->GetID());
}

void CAutoKillMonsterMgr::AddMonster(CSimpleCharacterNode* pNode)
{
	if (!g_pGameData->IsAutoKillMonster())
		return;

	if (pNode->GetHP() < 100)//被别人打过的不打
		return;

	_AddMonster(pNode);
}

void CAutoKillMonsterMgr::AddMonster(DWORD dwMonsterID)
{
	if (!g_pGameData->IsAutoKillMonster())
		return;

	CSimpleCharacterNode* pChar = MapArray.FindSimpleCharacter(dwMonsterID);
	AddMonster(pChar);
}

void CAutoKillMonsterMgr::DelMonster(DWORD dwMonsterID)
{
	m_vMonsters.remove(dwMonsterID);
}

//进攻策略
bool CAutoKillMonsterMgr::AIAttackPolicy(CSimpleCharacterNode* pNode)
{
	if (!pNode) return false;

	switch(SELF.GetCareer())
	{
	case JOB_ZHANSHI:
		return AIFighterAttackPolicy(pNode);
		break;
	case JOB_FASHI:
		return AIRabbiAttackPolicy(pNode);
		break;
	case JOB_DAO:
		return AITaoistAttackPolicy(pNode);
		break;
	}

	return false;
}

bool CAutoKillMonsterMgr::AIFighterAttackPolicy(CSimpleCharacterNode* pNode)
{
	//战士攻击策略
	if(!pNode) return false;

	if(SELF.GetExtraState() & ES_KUANGNUWIND)//狂怒旋风不能使用魔法
		return false;

	SELF.SetReserveData(plyAttackLockID,pNode->GetID());

	int iAimX,iAimY;
	pNode->GetRealXY(iAimX,iAimY);

	if (_closePoint(m_iSelfX,m_iSelfY,iAimX,iAimY,5))
	{
		if(SELF.GetFightOnLeopard() && SELF.IsLearnMagic(MAGICID_AOFENGCUT_ATTACK))
		{
			if(g_AIMgr.CanUseMagic(MAGICID_AOFENGCUT_ATTACK, false))
			{
				g_pGameControl->SEND_Player_Attack_Magic(pNode->GetID(),iAimX,iAimY,MAGICID_AOFENGCUT_ATTACK);
				return true;
			}
		}
	}

	//突斩或野蛮
	if(g_AutoFightMgr.AutoWildCollide())
		return true;

	//打开战士的技能，烈火，雷霆
	g_AutoFightMgr.OpenSword_Fire();
	if(g_AutoFightMgr.OpenAttackSkill())
		return true;
	return false;
}


//法师攻击策略
bool CAutoKillMonsterMgr::AIRabbiAttackPolicy(CSimpleCharacterNode* pNode)
{
	if(!pNode) return false;

	int iAimX,iAimY;
	pNode->GetRealXY(iAimX,iAimY);

	if (abs(m_iSelfX - iAimX) <= 6 && abs(m_iSelfY - iAimY) <= 8)	
	{
		if (SELF.GetFightOnLeopard() && SELF.IsLearnMagic(MAGICID_ZHONGLEI_ATTACK))
		{
			if(GetTickCount() > g_AIMgr.GetReserveTime(plyRideFightTime) + 10000)//暂时设置为10秒
			{
				if (g_pGameMgr->AttackUseMagic(MAGICID_ZHONGLEI_ATTACK,pNode->GetID(),false))
					return true;
			}
		}

		if (m_bAutoSuperFireBall && AttackUseMagic(MAGICID_SUPER_FIREBALL, pNode->GetID()) && g_pGameMap->CanThrough(m_iSelfX,m_iSelfY,iAimX,iAimY) > 0)
			return true;

		if (m_bAutoDragonLight && AttackUseMagic(MAGICID_DRAGON_LIGHT,pNode->GetID()))
			return true;

		if (m_bAutoThunder && AttackUseMagic(MAGICID_THUNDER,pNode->GetID()))
			return true;

		if (m_bAutoThunder && AttackUseMagic(MAGICID_ICE_ARROW,pNode->GetID()))
			return true;

		if (m_bAutoFireBall && AttackUseMagic(MAGICID_FIREBALL,pNode->GetID()) && g_pGameMap->CanThrough(m_iSelfX,m_iSelfY,iAimX,iAimY) > 0)
			return true;
	}

	return false;
}

//道士攻击策略
bool CAutoKillMonsterMgr::AITaoistAttackPolicy(CSimpleCharacterNode* pNode)
{
	if (!pNode) return false;

	bool bGreen = false,bSuccess = false;
	WORD wStatus = pNode->GetStatus();
	int iMagicID = 0,iAimX,iAimY;
	pNode->GetRealXY(iAimX,iAimY);	

	if (abs(m_iSelfX - iAimX) <= 6 && abs(m_iSelfY - iAimY) <= 8)
	{
		if (SELF.GetFightOnLeopard() && SELF.IsLearnMagic(MAGICID_DULINGBO_ATTACK))
		{
			if(_closePoint(m_iSelfX,m_iSelfY,iAimX,iAimY,7))
			{
				if(g_AIMgr.CanUseMagic(MAGICID_DULINGBO_ATTACK, false))
				{
					int iDir = GetDir8(iAimX,iAimY,m_iSelfX,m_iSelfY);
					g_pGameControl->SEND_Player_Turn(char(iDir));
					SELF.SetDir(iDir);

					if(g_pGameMgr->AttackUseMagic(MAGICID_DULINGBO_ATTACK,pNode->GetID(),false))
					{
						SELF.InitAction(ACTION_STAND);
						return true;
					}
				}
			}
		}

		if (m_bAutoPoison && SELF.IsLearnMagic(MAGICID_SUPER_POISON_MAGIC) && m_iPosionTimes < 3)
		{
			if (!(wStatus & CS_RED) || !(wStatus & CS_GREEN))
			{
				bGreen	 = ((wStatus & CS_GREEN) == 0);
				iMagicID = MAGICID_SUPER_POISON_MAGIC;

				if(AITaoistAttack(iMagicID,pNode->GetID(),bGreen))
				{
					m_iPosionTimes ++;
					return true;
				}
			}
			else
			{
				m_iPosionTimes = 0;
			}
		}

		if (m_bAutoPoison && SELF.IsLearnMagic(MAGICID_POISON_MAGIC) && m_iPosionTimes < 3)
		{
			if (!(wStatus & CS_RED) || !(wStatus & CS_GREEN))
			{
				bGreen	 = ((wStatus & CS_GREEN) == 0);
				iMagicID = MAGICID_POISON_MAGIC;

				if(AITaoistAttack(iMagicID,pNode->GetID(),bGreen))
				{
					m_iPosionTimes ++;
					return true;
				}
			}
			else
			{
				m_iPosionTimes = 0;
			}
		}

		if (m_bAutoPoison && SELF.IsLearnMagic(MAGICID_CURSE) && m_iPosionTimes < 3)
		{
			if (!(wStatus & CS_PURPLE) || !(wStatus & CS_BLUE))
			{
				bGreen	 = ((wStatus & CS_BLUE) == 0);
				iMagicID = MAGICID_CURSE;

				if(AITaoistAttack(iMagicID,pNode->GetID(),bGreen))
				{
					m_iPosionTimes ++;
					return true;
				}
			}
			else
			{
				m_iPosionTimes = 0;
			}
		}


		if (m_bAutoFireJuju && SELF.IsLearnMagic(MAGICID_FIRE_JUJU) && g_pGameMap->CanThrough(m_iSelfX,m_iSelfY,iAimX,iAimY) > 0)
			iMagicID = MAGICID_FIRE_JUJU;
		else if (m_bAutoProtectSymbol && SELF.IsLearnMagic(MAGICID_PROTECT_SYMBOL) && g_pGameMap->CanThrough(m_iSelfX,m_iSelfY,iAimX,iAimY) > 0)
			iMagicID = MAGICID_PROTECT_SYMBOL;

		if (AITaoistAttack(iMagicID,pNode->GetID(),bGreen))
			return true;
	}	

	return false;
}

bool CAutoKillMonsterMgr::AITaoistAttack(int iMagicID,DWORD dwID,bool bGreen)
{
	if ((iMagicID != 0) && (dwID != 0))
	{
		if (!g_AIMgr.HasMagicEquip(iMagicID,bGreen))
			return false;		

		if (AttackUseMagic(iMagicID,dwID))
			return true;
	}

	return false;
}



bool CAutoKillMonsterMgr::AIDefendPolicy()
{
	switch(SELF.GetCareer())
	{
	case JOB_ZHANSHI:
		return AIFighterDefendPolicy();
		break;
	case JOB_FASHI:
		return AIRabbiDefendPolicy();
		break;
	case JOB_DAO:
		return AITaoistDefendPolicy();
		break;
	}

	return false;
}

//战士防御策略
bool CAutoKillMonsterMgr::AIFighterDefendPolicy()
{
	if(SELF.GetExtraState() & ES_KUANGNUWIND)//狂怒旋风不能使用魔法
		return false;

	return g_AutoFightMgr.OpenHuShen_JinGang();
}

//法师防御策略
bool CAutoKillMonsterMgr::AIRabbiDefendPolicy()
{
	if (g_AutoFightMgr.OpenMagicSkill())
		return true;

	CSimpleCharacterNode* pNode = GetCanAttackTarget();
	if (pNode)
	{
		SELF.SetReserveData(plyMagicLockID,pNode->GetID());

		if (g_AutoFightMgr.AutoDisputeFire())
			return true;
	}	

	return false;
}
//道士防御策略
bool CAutoKillMonsterMgr::AITaoistDefendPolicy()
{
	if(g_AutoFightMgr.OpenDaoSkill())
		return true;

	CSimpleCharacterNode* pNode = GetCanAttackTarget();	
	if (pNode)
	{
		SELF.SetReserveData(plyMagicLockID,pNode->GetID());

		if(g_AutoFightMgr.AutoLionCall())
			return true;
	}

	WORD wStatus = 0;	

	wStatus = SELF.GetStatus();
	if (((wStatus & CS_RED) || (wStatus & CS_GREEN)))
	{
		if (g_pGameMgr->AttackUseMagic(MAGICID_DESTROY_POISON,SELF.GetID(),false))
			return true;
	}

	if (SELF.IsLearnMagic(MAGICID_ENCALL_MONSTER))
	{		
		if (aPet[AKM_PetSuperSkull] < 2)
		{
			if( g_pGameMgr->AttackUseMagic(MAGICID_ENCALL_MONSTER,0,false))
				return true;			
		}
	}
	else if (SELF.IsLearnMagic(MAGICID_CALL_JOSS))
	{
		if (aPet[AKM_PetSupernalBeast] < 1)
		{
			if( g_pGameMgr->AttackUseMagic(MAGICID_CALL_JOSS,0,false))
				return true;
		}
	}
	else if (SELF.IsLearnMagic(MAGICID_CALL_MONSTER))
	{
		if (aPet[AKM_PetSkull] < 1)
		{
			if( g_pGameMgr->AttackUseMagic(MAGICID_CALL_MONSTER,0,false))
				return true;
		}
	}
	else if (SELF.IsLearnMagic(MAGICID_CONTROL_CORPSE))
	{
		if (aPet[AKM_PetCorpus] < 1)
		{
			CSimpleCharacterNode* pNode = NULL;
			//获得身边死亡的怪
			for (int iY = m_iSelfY - 5;iY < m_iSelfY + 5;iY++)
			{
				for (int iX = m_iSelfX - 5;iX < m_iSelfX + 5;iX++)
				{
					pNode = MapArray.FindSimpleCharacter(iX,iY);
					if (pNode && pNode->IsMonster() && !pNode->IsPet() && pNode->IsDead())
					{
						if( g_pGameMgr->AttackUseMagic(MAGICID_CONTROL_CORPSE,pNode->GetID(),false))
							return true;

						return false;
					}
				}
			}
		}
	}

	return false;
}

void CAutoKillMonsterMgr::SortMonster()
{
	// 每帧只排序一次
	if (!m_isMonsterSorted && m_vMonsters.size() > 1)
	{
		m_vMonsters.sort(SortByDirectionFunctor(m_iSelfX, m_iSelfY));
		m_isMonsterSorted = true;
	}
	
}

CSimpleCharacterNode* CAutoKillMonsterMgr::GetCanAttackTarget()
{
	CSimpleCharacterNode* pNode = MapArray.FindSimpleCharacter(m_dwSelfAttackID);
	if (pNode && !pNode->IsDead())
		return pNode;

	// 获得最近一个可攻击目标
	pNode = GetNearestVaildMonster();
	if (pNode)
		SetSelfAttackID(pNode->GetID());
	else
		SetSelfAttackID(0);

	return pNode;
}

CSimpleCharacterNode* CAutoKillMonsterMgr::GetNearestVaildMonster()
{
	// 根据离玩家自己距离从近到远排序
	SortMonster();

	// 获得最近一个可攻击目标
	int iter = 20;
	CSimpleCharacterNode* pNode = NULL;
	while (!m_vMonsters.empty() && iter > 0)
	{
		pNode = NULL;

		DWORD dwAttack = *m_vMonsters.begin();
		if (dwAttack == 0)
		{
			m_vMonsters.erase(m_vMonsters.begin());
			continue;
		}
		pNode = MapArray.FindSimpleCharacter(dwAttack);
		if (!pNode || !IsCanAttack(pNode) || !CanReach2(pNode))
		{
			m_vMonsters.erase(m_vMonsters.begin());
			pNode = NULL;
			--iter;
			continue;
		}
		return pNode;
	}

	return NULL;
}

bool CAutoKillMonsterMgr::CanReach2(CSimpleCharacterNode* pNode)
{
	// 走2步，如果回到开始位置则失败
	if (!pNode) return false;

	int iSelfX = m_iSelfX,iSelfY = m_iSelfY;

	int itargetx,itargety;
	pNode->GetRealXY(itargetx,itargety);
	
	//计算步数		
	int iStep = SELF.CalStepCount(ACTION_RUN);

	if(!g_pGameMgr->PlayerCanRun())
		iStep = 1;

	//走2步
	for (int i = 0; i < 2; ++i)
	{
		if(_closePoint(iSelfX,iSelfY,itargetx,itargety,1))
			return true;

		int cDir = GetDir8Attack(itargetx,itargety,iSelfX,iSelfY);
		WORD wAction = ACTION_RUN;

		BOOL bNeedRun = false;
		cDir = g_pGameMgr->GetGoDir2(iSelfX,iSelfY,cDir,iStep,false,bNeedRun);
		if(cDir == DIR_NO)
			return false;

		if(bNeedRun)
			wAction = ACTION_RUN;
		else
			wAction = ACTION_WALK;

		int ls = iStep;
		if(!bNeedRun)
			ls = 1;

		//计算目的地
		int dx,dy;
		GetDirStep(cDir,dx,dy);
		iSelfX = iSelfX + ls*dx;
		iSelfY = iSelfY + ls*dy;
	}

	if (m_iSelfX == iSelfX && m_iSelfY == iSelfY)
	{
		return false;
	}

	return true;
}


void CAutoKillMonsterMgr::Reach(CSimpleCharacterNode* pNode)
{
	if (!pNode) return;

	if (CanReach2(pNode))
	{
		SNextAction& Next = SELF.GetCNextAction();
		Next.Clear();
		Next.wAction	= ACTION_ATTACK1;
		Next.uTarget	= pNode->GetID();
		//Next.cDir       = g_pGameMgr->GetPlayerDir(pNode->GetX(),pNode->GetY());
	}
	else
	{
		DelMonster(pNode->GetID());
		if (m_dwSelfAttackID == pNode->GetID())
		{
			SetSelfAttackID(0);
		}
	}
}

bool CAutoKillMonsterMgr::HasAttackMe(DWORD dwID)
{
	if (dwID == 0)
		return false;
	if (std::find(m_vMonsters.begin(), m_vMonsters.end(), dwID) == m_vMonsters.end())
		return false;
	return true;
}

bool CAutoKillMonsterMgr::AIRandAutoWalk()
{
	if (SELF.GetMapPathFinder().IsOnRoute())
		return false;

	if (m_iLastAutoWalkX > 0 && m_iLastAutoWalkY > 0 && abs(m_iSelfX - m_iLastAutoWalkX) >= 6 && abs(m_iSelfY - m_iLastAutoWalkY) >= 6)
	{
		if (g_pGameMgr->AI_AutoWalk(m_iLastAutoWalkX,m_iLastAutoWalkY))
		{
			return true;
		}
	}

	int iter = 20;
	while (iter > 0)
	{
		int iX = rand() % g_pGameMap->GetWidth();
		int iY = rand() % g_pGameMap->GetHeight();
		if(!g_pGameMap->IsBlock(iX,iY))
		{
			if (g_pGameMgr->AI_AutoWalk(iX,iY))
			{
				m_iLastAutoWalkX = iX;
				m_iLastAutoWalkY = iY;
				return true;
			}
		}
		
		--iter;
	}	
	
	return false;	
}


void CAutoKillMonsterMgr::OnPetStateChange()
{
	aPet[AKM_PetSkull] = 0;
	aPet[AKM_PetCorpus] = 0;
	aPet[AKM_PetSupernalBeast] = 0;

	aPet[AKM_PetSuperSkull]--;
	if (aPet[AKM_PetSuperSkull] < 0)
		aPet[AKM_PetSuperSkull] = 0;
}

void CAutoKillMonsterMgr::OnMagicEffectived(WORD wMagicID)
{
	switch (wMagicID)
	{
	case MAGICID_ENCALL_MONSTER:
		aPet[AKM_PetSuperSkull] = 2;
		break;
	case MAGICID_CONTROL_CORPSE:
		aPet[AKM_PetCorpus] = 1;
		break;
	case MAGICID_CALL_JOSS:
		aPet[AKM_PetSupernalBeast] = 1;
		break;
	case MAGICID_CALL_MONSTER:
		aPet[AKM_PetSkull] = 1;
		break;
	default:
		break;
	}
}

bool CAutoKillMonsterMgr::CanUseMagic(int wMagicID)
{
	if (wMagicID == 0)
	{
		return false;
	}

	CMagicData* pMagic = SELF.FindMagic(wMagicID);
	if(pMagic == NULL || pMagic->IsDisable())
	{
		return false;
	}

	if(pMagic->GetNeedManaValue() >= SELF.GetMP())
	{
		//魔法值不够了！！！！		
		return false;
	}

	if (!g_AIMgr.HasMagicEquip(wMagicID,false,true))
	{
		return false;
	}

	return true;
}

bool CAutoKillMonsterMgr::AttackUseMagic(int iMagicID,DWORD dwTargetID)
{
	if (!CanUseMagic(iMagicID))
		return false;

	g_pGameMgr->AttackUseMagic(iMagicID,dwTargetID,false);
	return true;
}

// 受到怪物攻击时
void CAutoKillMonsterMgr::DealWithSelfHurted(DWORD id)
{
	if (!g_pGameData->IsAutoKillMonster())
		return;

	if (id == 0 || m_dwSelfAttackID == id)
		return;

	// 将攻击自己的怪物加入列表
	_AddMonster(id, true);
}

bool CAutoKillMonsterMgr::HasAutoKillMonster()
{
	if (g_pGameData->IsAutoKillMonster() && !m_vMonsters.empty())
	{
		CSimpleCharacterNode *pNode = GetNearestVaildMonster();
		if (pNode)
		{
			int iSelfX,iSelfY;
			SELF.GetRealXY(iSelfX,iSelfY);
			int cx,cy;
			pNode->GetRealXY(cx,cy);

			if (_closePoint(iSelfX,iSelfY,cx,cy,3))
			{
				return true;
			}
		}
	}

	return false;
}


bool CAutoKillMonsterMgr::IsCanAttack(CSimpleCharacterNode* pChar)
{
	if(pChar == NULL || pChar->IsDead() || pChar->IsGrouped() || pChar->IsNpc()) return false;

	if (pChar->IsMonster())
	{
		if(pChar->IsPet())	return false;

		if (SELF.GetCamp() != 0 && SELF.GetCamp() == pChar->GetCamp() )
			return false;

		int iMonster = pChar->GetRaceNo();
		if (iMonster >= 410 && iMonster <= 417)// 圣殿山雕像
			return false;

		if (iMonster >= 260 && iMonster <= 270)// 图腾
			return false;

		string monName = pChar->GetName();

		if (m_bAllowKillMonsterEnabled && !m_allowKillMonsters.empty())
		{
			if (m_allowKillMonsters.find(monName) != m_allowKillMonsters.end())
				return true;
			else
				return false;
		}

		if( monName == "树" || monName == "松树" || monName == "桐树" || 
			monName == "桃树" || monName == "樟树" ||
			monName == "弓箭守卫" || monName == "卫士" || 
			monName == "乌骓" || monName == "雪龙" || 
			monName == "枣红马"|| monName == "常胜玄兵")
		{
			return false;
		}

		return true;
	}

	return false;
}

