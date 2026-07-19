#include "ActionStruct.h"
#include "GameData.h"
#include "GameAI/AIConfigMgr.h"

IMPLEMENT_POOL(STargetData,128,32);		// 攻击目标缓冲池
IMPLEMENT_POOL(SNextAction,64,32);		// 动作缓冲池
IMPLEMENT_POOL(tex_show_t,128,32);		// 动作缓冲池

//------------------------------------------------------------------
//目标数据结构
STargetData::STargetData()
{
	dwID = 0;
	pNext = NULL;
}

void STargetData::InitData(void * pBuf)
{
}

//------------------------------------------------------------------
//当前动作
SAction::SAction()
{
	wAction = ACTION_STAND;
	wDrawAction = ACTION_STAND;
	iFrameSpeed = 1;
	iFrameCount = 1;
	tFrameStart = 0;
	iFrameNow = 0;
	iDealTimesCurFrame = 0;

	cDir = 0;
	iAimX = iAimY = 0;
	iMoveX = iMoveY = 0;
	uFlag = 0;
	uData = 0;
	iData = 0;
	pTarget = NULL;
	uTarget = 0;
	dwColor = 0xFFFFFFFF;
}

bool SAction::IsEnd(DWORD tTime)
{
	if(iFrameSpeed == 0 || iFrameCount == 0)
		return true;

	 if(tFrameStart == 0)
		 return false;

	if((tTime - tFrameStart) >= iFrameSpeed * iFrameCount)
		return true;

	return false;
}

int SAction::GetFrame(DWORD tTime) //计算当前帧
{
	if(iFrameCount == 0)
		return 0;

	if(iFrameSpeed == 0)
		return iFrameCount - 1;

	DWORD tPass = 0;
	if(tFrameStart != 0)
		tPass = tTime - tFrameStart;
	else
		tFrameStart = tTime;

	iFrameNow = tPass / iFrameSpeed;
	iDealTimesCurFrame = tPass % iFrameSpeed;

	if(iFrameNow >= iFrameCount)
	{
		if((tTime - tFrameStart) >= iFrameSpeed * iFrameCount)
		{
			if(wAction == ACTION_ATTACK1)
				iFrameNow = 0;
			else
				iFrameNow = iFrameCount - 1;

			iDealTimesCurFrame = 0;
		}
		else//iDealTimesCurFrame == 0的时候,也就是iFrameNow 第一次等于iFrameCount的时候,还是属于这个会动作的最后一帧
		{
			iFrameNow = iFrameCount - 1;
		}
	}
	else if(wAction == ACTION_ATTACK1 && iFrameNow == iFrameCount - 1)//如果是攻击的最后一帧，去除MAGIC_ACTION，防止后面继续绘制刀光
	{
		uFlag &= ~MAGIC_ACTION;
	}

	return iFrameNow;
}

float SAction::GetRate(DWORD tTime)
{
	if(iFrameCount == 0)
		return 0.0f;

	if(iFrameSpeed == 0)
		return 1.0f;

	DWORD tPass = 0;
	if(tFrameStart != 0)
		tPass = tTime - tFrameStart;
	else
		tFrameStart = tTime;

	float fFrameRate = (float)tPass / (iFrameSpeed * iFrameCount);

	if(fFrameRate > 1.0f)
		fFrameRate = 1.0f;

	return fFrameRate;
}

void SAction::SetNpcRandStand(WORD wRaceNum)
{
	if(!IsStand()) return;

	int iAction = GetMultiActionNpcAction(wRaceNum);
	if(iAction == ACTION_STAND2)
	{
		iFrameCount = 8;
		iFrameSpeed = 8;
		wAction = iAction;
	}
	else if (iAction == ACTION_STAND3)
	{
		iFrameCount = 12;
		iFrameSpeed = 8;
		wAction = iAction;
	}
}

int SAction::GetMultiActionNpcAction(WORD wID)
{
	switch (wID)
	{
	case 1:
	case 5:
	case 6:
	case 7:
	case 10:
	case 12:
	case 191:
		return ACTION_STAND2;
		break;
	case 248:
		return ACTION_STAND3;
		break;
	default:
		return ACTION_STAND;
		break;
	}

	return false;
}

bool SAction::IsStand()
{
	if(wAction == ACTION_STAND || wAction == ACTION_STAND2 || wAction == ACTION_STAND3)
		return true;
	return false;
}

bool SAction::IsMove()
{
	if(wAction == ACTION_WALK || wAction == ACTION_RUN)
		return true;

	return false;
}

bool SAction::IsDeath()
{
	if(wAction == ACTION_DEATH)
		return true;
	return false;
}

bool SAction::IsAttack()
{
	if(wAction == ACTION_ATTACK1 || 
		(wAction >= ACTION_ATTACK2 && wAction <= ACTION_ATTACK6))
		return true;

	return false;
}

void SAction::ClearTarget()
{
	STargetData* pTargetNow = NULL;

	while(pTarget)
	{
		pTargetNow = pTarget->pNext;
		DEL_TARGETDATA(pTarget);
		pTarget = pTargetNow;
	}
}

void SAction::AddTarget(DWORD id)
{
	STargetData* pCur = NEW_TARGETDATA();
	pCur->dwID = id;

	if(pTarget)
		pCur->pNext = pTarget;

	pTarget = pCur;
}

//-----------------------------------------------------------------------------------
//下一步动作
SNextAction::SNextAction()
{
	pTarget = NULL;
	pNext = NULL;
	Clear();
}

void SNextAction::Clear()
{
	wAction = ACTION_STAND;
	wDrawAction = ACTION_NULL;
	cDir = 0;
	iLooks = 0;
	iAimX = iAimY = 0;
	uFlag = 0;
	uData = 0;
	iData = 0;
	iData2 = 0;
	uTarget = 0;
	ClearTarget();
	dwColor = 0;
}

void SNextAction::ClearTarget()
{
	STargetData * pTargetNow;
	while(pTarget)
	{
		pTargetNow = pTarget->pNext;
		SAFE_DELETE(pTarget);
		pTarget = pTargetNow;
	}
}

bool SNextAction::IsStand()
{
	if(wAction == ACTION_STAND || wAction == ACTION_STAND2 || wAction == ACTION_STAND3)
		return true;
	return false;
}

bool SNextAction::IsMove()
{
	if(wAction == ACTION_WALK || wAction == ACTION_RUN)
		return true;

	return false;
}

bool SNextAction::IsAttack()
{
	if(wAction == ACTION_ATTACK1 || 
		(wAction >= ACTION_ATTACK2 && wAction <= ACTION_ATTACK6))
		return true;

	return false;
}

void SNextAction::AddTarget(DWORD id)
{
	STargetData* pCur = NEW_TARGETDATA();
	pCur->dwID = id;

	if(pTarget)
		pCur = pTarget->pNext;

	pTarget = pCur;
}

/////////////////////////////////////////////////////////////////////////////////
void TargetDataClear(STargetData *&dst)
{
	STargetData *p = dst;
	STargetData *pNext = NULL;
	while(p)
	{
		pNext = p->pNext;
		DEL_TARGETDATA(p);
		p = pNext;
	}
	dst = NULL;

}

// 被攻击动作效果
BOOL AddAttacked(DWORD id,DWORD oid,int iData)
{
	DWORD dwCount = GetTickCount();
	SNextAction* pNext = NULL;

	if(id == SELF.GetID())
	{
		if(g_AICfgMgr.IsMountain())     //稳如泰山
			return FALSE;

		if(SELF.IsDead() || dwCount - SELF.GetReserveData(pubLastHurtedTime) < 400) //短时间内不重复
			return FALSE;

		SELF.SetReserveData(pubLastHurtedTime,dwCount);

		if(pNext = SELF.PushSNextAction(ACTION_ATTACKED))
			pNext->uData = oid;
	}
	else
	{
		CSimpleCharacterNode *pChar = g_pGameData->FindSimpleCharacter(id);
		if(pChar)
		{

			//忽略受伤动作的怪物
			switch (pChar->GetRaceNo())
			{
			case 388:
			case 389:
				return FALSE;
			}

			if(pChar->IsDead() || dwCount - pChar->GetReserveData(pubLastHurtedTime) < 1080)
				return FALSE;

			pChar->SetReserveData(pubLastHurtedTime,dwCount);
			if(pNext = pChar->PushSNextAction(ACTION_ATTACKED))
				pNext->uData = oid;
		}
	}
	return TRUE;
}

BOOL EffectAttacked(STargetData*&pTarget)
{
	STargetData *pSrcTarget = pTarget;		//保存pTarget，用于删除！
	if(pSrcTarget == NULL)
		return FALSE;

	int iAttacked = 0;
	for(;pSrcTarget;pSrcTarget = pSrcTarget->pNext)
	{
		iAttacked++;

		// 被攻击动作
		//if(pSrcTarget->dwID == SELF.GetID())
		//{
		//	SELF.PushSNextAction(ACTION_ATTACKED);
		//}
		//else
		//{
		//	CSimpleCharacterNode *pChar = g_pGameData->FindSimpleCharacter(pSrcTarget->dwID);
		//	if(pChar)
		//	{
		//		pChar->PushSNextAction(ACTION_ATTACKED);
		//	}
		//}
        if(pSrcTarget->iType == 0)
        {
            AddAttacked(pSrcTarget->dwID,pSrcTarget->dwOID);
        }
        else if(pSrcTarget->iType == 1)     //人物消失
        {
            MapArray.DeleteCharacter(pSrcTarget->dwID);     
        }
        else if(pSrcTarget->iType == 2)     //人物死亡
        {

        }
	}
	TargetDataClear(pTarget);

	return TRUE;
}

//push p_tex to the head of p_head
//2004-8-26
void push_tex_store(tex_show_t	**p_head,tex_show_t	*p_tex)
{
	if (p_head && p_tex)
	{
		p_tex->next = *p_head;
		*p_head = p_tex;
	}
};

//clear p_head
//2004-8-26
void clear_tex_store(tex_show_t	**p_head)						
{
	if (p_head )
	{
		tex_show_t	*p,*p1 = *p_head;
		while (p1)
		{
			p = p1->next;
			DEL_TEX_SHOW(p1);
			p1 = p;
		}
		*p_head =NULL;
	}
};