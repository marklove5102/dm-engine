#include "CharacterAttr.h"
#include "GameData/GameGlobal.h"
#include "GameData/GameData.h"
#include "GameData/MagicCtrlMgr.h"
#include "Global/Interface/GraphicInterface.h"
#include "PetData.h"
#include "Global/Interface/FontInterface.h"
#include "GameControl/GameControl.h"
#include "GameData/ConfigData.h"

CCharacterAttr::CCharacterAttr(void)
{
	m_pNextBegin = m_pNextEnd = NULL;
	m_pMagicCase = NULL;
	m_pTexFront = m_pTexBack = NULL;
	m_cNameColor = 255;
	m_dwNpcActionTm = GetTickCount();
	m_byNumberLiveToken = 0;				//初始化生存令牌数量0
	m_byBufferTypeQQ = 0;					//圈圈特效
	clear();
}

CCharacterAttr::~CCharacterAttr(void)
{
	ClearAllNext();
	ClearMagicCase();
	ClrTexFront();
	ClrTexBack();
}

void CCharacterAttr::clear()
{
	m_nID = 0;
	m_iX = 0;
	m_iY = 0;
	m_iOffX = 0;
	m_iOffY = 0;
	m_iDrawOffX = 0;
	m_iDrawOffY = 0;

	m_bDead	= false;
	m_bWait = false;

	m_iRealX = 0;
	m_iRealY = 0;
	m_pNextBegin = m_pNextEnd = NULL;

	m_iMagicState = 0;
	m_iSpeedup = 0; 
	m_bDeal = false;
	m_wStatus = 0;
	m_ExtraState = 0;

	memset(&m_Looks,0,sizeof(LOOKS));
	memset(&m_BianshenLooks,0,sizeof(LOOKS));

	m_nHP	= 0;
	m_nHPMax = 0;
	m_nMP   = 0;
	m_nMPMax = 0;
	m_nFlags = 0;
	m_bStopGo = false;
	m_iAttackSpeed = 0;
	m_iSex = 0;
	m_nCareer = 0;
	m_nDirection = 0;
	m_cShowHP = 0;
	m_iUsingMagicType = 0;

	m_iNumForZhongLeiHit = 0;
	m_cTalkFontType		= 1;
	m_dwTalkColor		= COLOR_DEFAULT;
	m_dwTalkShowTime    = 0;
	m_iFontSize         = FONTSIZE_DEFAULT;

	m_iScrX = m_iScrY = 0;

	m_iTopX = m_iTopY = -10000;
	m_iTexW = m_iTexH = 0;
	m_iTexOffX = m_iTexOffY = 0;

	m_bBoothState = false;

	m_BoothInfo.dwFlagColor = COLOR_DEFAULT;
	m_BoothInfo.iBoothType  = 0;
	m_BoothInfo.iFlagType   = 0;

	m_dwRelationType = 0;

	m_iMaskLevel = 0;
	m_strMaskName.clear();
	m_dwBubbleUpLastTime = 0;
	m_strTitle.clear();
	m_byVipCardType = 0;
	m_byVipTradeLevel = 0;
	m_by12GongTitle = 0;

	memset(m_dwReserveData,0,plyNums*sizeof(DWORD));

	ClearBubbleUp();
	ClearAllNext();
	ClearMagicCase();
	ClrTexFront();
	ClrTexBack();

    m_bFightOnLeopard = false;
    m_iRunStepOnLeopard = 3;

	m_iPutOnShield = 0;
	m_iGuildFlag = 0;
	m_iPutOnShieldLevel = 0;
	m_bCanDig = false;
	m_byCamp = 0;
	m_iUsingMagicType = 0;
// 	m_byTrusteeshipFlag = 0;
	m_iHolyWingLevel = -1;
	m_iHolyWingStrongLevel = -1;
	m_wGreateMagicState = 0;
	m_dwTuTengState = 0;
	memset(&m_sFaBao,0,sizeof(FABAOStruct));
	memset(m_czMagicSlogan,0,64);
	m_dwMagicSloganEndTime = 0;
	m_dwLianJiNumber = 0;
	m_dwLIanJiShowEndTime = 0;
	m_dwLianJiZhuangJiTime = 0;
	m_dwRealLianJiNumber = 0;
	m_dwLastLianJiStartTime = 0;

	m_IsInDaoZunJiangLinState = false;
	m_IsIn8DunState = false;
	m_b8DunShan = false;
	m_8DunCurProgress = 0;
	m_8DunMaxProgress = -1;

	m_IsInShadowKill8 = false;
	m_byShadowKill8Dir = 0;
	m_iShadowStepFrame = 10;
	m_iLeftShadowNum = 0;
	m_iRightShadowNum = 0;
	m_iMaxShadowNum = 0;
}

void CCharacterAttr::SetReserveData(int i,UINT dwData, WORD magID)
{
// 	if (g_TrusteeshipData.IsTeamTrusteeship() && 
// 		g_TrusteeshipData.IsCaptain())
// 	{
// 		if (i == plyAttackLockID ||
// 			i == plyMagicLockID)
// 		{
// 			if (m_dwReserveData[i] != dwData && dwData != 0)
// 			{
// 				if (magID != MAGICID_DESTROY_POISON/*解毒术*/)
// 				{
// 					g_pGameControl->Send_TrusteeshipHeadAttackTarget(dwData);
// 				}
// 			}
// 		}
// 	}

	m_dwReserveData[i] = dwData;
}

void CCharacterAttr::SetName(const char * strName)	
{
	//m_showName.clear();
	//VString & VName = StringUtil::split(strName,"\\");
	//for(int i = 0; i < VName.size(); i++)
	//	m_showName.push_back(VName[i]); 

	m_showName = StringUtil::split(strName,"\\");

	if(m_showName.size() > 0)
	{
		m_strName = m_showName[0];
	}else{
		//直接使用
		m_strName = strName;
	}
}

//设置话
void CCharacterAttr::SetTalk(const char *str,DWORD dwColor,BYTE cFontType,BYTE cFontSize)
{
	m_VTalk.clear();

	CutByUnicode(str,m_VTalk,MAXHEADWIDTH);

	m_cTalkFontType = cFontType;
	m_dwTalkColor	= dwColor;
	m_iFontSize = cFontSize;
	m_dwTalkShowTime  = GetTickCount();
}

int	CCharacterAttr::GetAttackActionNum()
{
	int j = 0;
	if(m_ActionNow.wAction == ACTION_ATTACKED)
		j++;

	SNextAction* pNext = m_pNextBegin;
	while(pNext)
	{
		if(pNext->wAction == ACTION_ATTACKED)
			j++;
		pNext = pNext->pNext;
	}
	return j;
}

int CCharacterAttr::GetNextActionNum()
{
	int j = 0;
	SNextAction* pNext = m_pNextBegin;
	while(pNext)
	{
		j++;
		pNext = pNext->pNext;
	}
	return j;
}

void CCharacterAttr::SetBianShenLooks(__int64 l)
{
	AssignLooks(m_BianshenLooks,l);
	InitAction(ACTION_STAND);
}

void CCharacterAttr::SetLooks(__int64 looks)
{
	AssignLooks(m_Looks,looks);
}

SNextAction* CCharacterAttr::PushSNextAction(int iAction,char dir,int x,int y)
{
	if (!g_Config.IsEnablePushAction())
	{
		return NULL;
	}

	SNextAction *p = NEW_NEXTACTION();
	if(p == NULL)
		return NULL;

	p->wAction = iAction;
	p->pNext = NULL;
	p->cDir = dir;
	p->iAimX = x;
	p->iAimY = y;

	if(m_pNextBegin == NULL || m_pNextEnd == NULL)
	{
		m_pNextBegin = m_pNextEnd = p;
		return p;
	}
	m_pNextEnd->pNext	= p;		// 添加倒最后
	m_pNextEnd			= p;		// 移动指针到最后

	return p;
}

SNextAction* CCharacterAttr::InsertSNextAction(int iAction,char dir,int iAimX,int iAimY,int idx)
{
	if (!g_Config.IsEnablePushAction())
	{
		return NULL;
	}

	SNextAction *p = NEW_NEXTACTION();
	if(p == NULL)
		return NULL;

	p->wAction = iAction;
	p->pNext = NULL;
	p->cDir = dir;
	p->iAimX = iAimX;
	p->iAimY = iAimY;

	if(m_pNextBegin == NULL || m_pNextEnd == NULL)
	{
		m_pNextBegin = m_pNextEnd = p;
		return p;
	}

	SNextAction *pTemp = m_pNextBegin;
	for(int i = 0; i < idx; i++)
	{
		if(pTemp)
			pTemp = pTemp->pNext;
		else
			break;
	}

	if(!pTemp)
	{
		m_pNextEnd->pNext	= p;		// 添加倒最后
		m_pNextEnd			= p;		// 移动指针到最后
	}
	else
	{
		p->pNext = pTemp->pNext;
		pTemp->pNext	= p;		// 添加倒最后
	}

	return p;
}

bool CCharacterAttr::PopSNextAction(SNextAction* pNextAction)
{
	if(pNextAction == NULL) //仅仅判断
	{
		if(m_pNextBegin)
			return true;

		return false;
	}
	if(m_pNextBegin == NULL || m_pNextEnd == NULL) //队列里面没有任何动作
	{
		//根据人物身上的状态产生持续动作
		return false;
	}
	else
	{
		memcpy(pNextAction,m_pNextBegin,sizeof(SNextAction));
		pNextAction->uFlag |= SERVER_ACTION;

		SNextAction * pNext = m_pNextBegin;

		if(m_pNextBegin == m_pNextEnd)						// 动作链表里面只有一个动作
			m_pNextBegin = m_pNextEnd = NULL;
		else
			m_pNextBegin = m_pNextBegin->pNext;				// 动作链表里面有多个动作

		//SAFE_DELETE(pNext);
		SNextAction::DEL_SNextAction(pNext);
	}
	return true;
}

void CCharacterAttr::ClearAllNext()
{
	SNextAction *pNext;
	while(m_pNextBegin)
	{
		pNext = m_pNextBegin->pNext;
		m_pNextBegin->ClearTarget();
		//SAFE_DELETE(m_pNextBegin);
		SNextAction::DEL_SNextAction(m_pNextBegin);
		m_pNextBegin = pNext;
	}
	m_pNextBegin = m_pNextEnd = NULL;
}

// 运行完所有队列中的动作
void CCharacterAttr::FinishAllNext(BOOL bNow)
{
	//动作的效果要保留的！！！
	ClearAllNext();
}

//初始化当前动作
void CCharacterAttr::InitAction(int iAction)
{
	ACTION_PIC_TYPE atcType = APT_NORMAL;
	if(this->GetFightOnLeopard())
		atcType = APT_RIDE_ATTACK_LEAPOARD;
	else if(this->IsOnLepoard())
		atcType = APT_RIDE_LEPOARD;
	else if(this->IsOnHorse())
		atcType = APT_RIDE_HORSE;
	
	if (GetRaceType() == CHARACTER_NPC)
	{
		//是NPC
		if (GetTickCount() - m_dwNpcActionTm >= 1 * 1000 * 5)//一分钟
		{
			m_ActionNow.SetNpcRandStand(GetRaceNo());
			m_dwNpcActionTm = GetTickCount();
		}
	}

	int iDrawAction = g_ActionInf.ConvertAction(iAction,atcType);

	stActionPic* ActionPic = g_ActionInf.GetActionPic(iDrawAction,GetRaceType(),GetRaceNo());
	if(ActionPic == NULL)
		return;

	int iOldAction = m_ActionNow.wAction;

	m_ActionNow.iFrameSpeed = ActionPic->iFrameSpeed;
	m_ActionNow.iFrameCount = ActionPic->iFrameReal;

	if(GetRaceType() == CHARACTER_HUMAN)
	{
		if(iAction == ACTION_STAND)
		{
			m_ActionNow.iFrameCount = 6;
		}
		else if(iAction == ACTION_ATTACK1)
		{
			int i = GetAttackSpeed();
			if( i < -6 ) i = -6;
			if( i >  6 ) i =  6;

			m_ActionNow.iFrameSpeed = ((27 - i*2) / 6) * 2;
			if(m_ActionNow.iFrameSpeed > 6)
				m_ActionNow.iFrameSpeed = 6;
			m_ActionNow.iFrameCount = 6;
		}

		//变身以后
		if(IsBianShen())
		{
			stActionPic* BianShenPic = g_ActionInf.GetActionPic(iAction,m_BianshenLooks.Char.byType,m_BianshenLooks.Char.wShape);
			if(BianShenPic == NULL)
				return;

			iDrawAction = iAction;

			if(BianShenPic->iFrameReal != 0)
			{
				m_ActionNow.iFrameSpeed = (m_ActionNow.iFrameSpeed * m_ActionNow.iFrameCount) / BianShenPic->iFrameReal;
				m_ActionNow.iFrameCount = BianShenPic->iFrameReal;
			}
		}
	}

	m_ActionNow.wAction = iAction;
	m_ActionNow.wDrawAction = iDrawAction;

	//原来和现在都还是STAND动作则不打断前面的,否则从头开始
	if(iOldAction != ACTION_STAND || iAction != ACTION_STAND || m_ActionNow.iFrameNow >= m_ActionNow.iFrameCount - 1)
	{
		m_ActionNow.tFrameStart = 0;
		m_ActionNow.iFrameNow = 0;
		m_ActionNow.iDealTimesCurFrame = 0;
	}

	m_ActionNow.iFrameSpeed /= (m_iSpeedup + 1); //加速度
    if(m_ActionNow.iFrameSpeed <= 0)
		m_ActionNow.iFrameSpeed = 1;

	m_ActionNow.tFrameStart = g_pGfx->GetFrameCount();

	m_ActionNow.cDir = 0;
	m_ActionNow.iAimX = m_ActionNow.iAimY = 0;
	m_ActionNow.iMoveX = m_ActionNow.iMoveY = 0;
	m_ActionNow.uFlag = 0;
	m_ActionNow.uData = 0;
	m_ActionNow.iData = 0;
	m_ActionNow.uTarget = 0;
	m_ActionNow.ClearTarget();
	m_ActionNow.dwColor = 0;
	m_bStopGo = false;
}

void CCharacterAttr::InitAction(SNextAction* pNext)
{
	if(pNext == NULL || pNext->wAction == ACTION_STAND)
	{
		InitAction(ACTION_STAND);
		return;
	}

	InitAction(pNext->wAction);

	if(pNext->wDrawAction != ACTION_NULL)
	{
		m_ActionNow .wDrawAction = pNext->wDrawAction; 
	}

	m_ActionNow.cDir = pNext->cDir;
	m_nDirection     = pNext->cDir;
	m_ActionNow.uFlag = pNext->uFlag;
	m_ActionNow.uData = pNext->uData;
	m_ActionNow.iData = pNext->iData;
	m_ActionNow.uTarget = pNext->uTarget;
	m_ActionNow.pTarget = pNext->pTarget;
	pNext->pTarget = NULL;
	m_ActionNow.dwColor = pNext->dwColor;

	if(pNext->iLooks != 0)
		SetLooks(pNext->iLooks);

	//计算目标和偏移
	m_ActionNow.iAimX = pNext->iAimX;
	m_ActionNow.iAimY = pNext->iAimY;

	SetOffset(0,0);
}

void CCharacterAttr::SetRealXY(int x,int y)			
{
	m_iRealX = x; 
	m_iRealY = y;

	if (!g_Config.IsEnablePushAction())
	{
		SetXY(m_iRealX,m_iRealY);
		SetOffset(0,0);
		if (SELF.GetID() != m_nID)
		{
			g_pGameData->AddSimpleCharacter(x,y,m_nID);
		}
	}
}

void CCharacterAttr::AddAttackedTarget(DWORD id)
{

}

//人物附加图片效果
void CCharacterAttr::PushTexFront	(tex_show_t *p)
{
	push_tex_store(&m_pTexFront,p);
}
void CCharacterAttr::PushTexBack	(tex_show_t *p)
{
	push_tex_store(&m_pTexBack,p);
}
void CCharacterAttr::ClrTexFront()
{
	clear_tex_store(&m_pTexFront);
}
void CCharacterAttr::ClrTexBack()
{
	clear_tex_store(&m_pTexBack);
}

void CCharacterAttr::SetMagicCase(IMagicCase* pMagicCase)
{
	ClearMagicCase();
	m_pMagicCase = pMagicCase;
}

void CCharacterAttr::ClearMagicCase()
{
	if(m_pMagicCase)
	{
		m_pMagicCase->ShutDown(this);
		delete m_pMagicCase;
	}
	m_pMagicCase = NULL;
}

bool CCharacterAttr::IsOnHorse()
{
	if(GetRaceType() != CHARACTER_HUMAN)
		return false;

	return m_Looks.Player.wHorse != 0;
}

bool CCharacterAttr::IsOnLepoard()
{
	if(GetRaceType() != CHARACTER_HUMAN)
		return false;

	return IsLeopard(m_Looks.Player.wHorse);
}

bool CCharacterAttr::IsOnBigLepoard()
{
	if(GetRaceType() != CHARACTER_HUMAN)
		return false;

	return IsBigLeopard(m_Looks.Player.wHorse);
}

bool CCharacterAttr::IsOnPhenix()
{
	if(GetRaceType() != CHARACTER_HUMAN)
		return false;

	return IsPhenix(m_Looks.Player.wHorse);
}

bool CCharacterAttr::IsOnBigPhenix()
{
	if(GetRaceType() != CHARACTER_HUMAN)
		return false;

	return IsBigPhenix(m_Looks.Player.wHorse);
}

bool CCharacterAttr::IsDemonMonster()
{
	if(m_Looks.Player.byType != CHARACTER_HUMAN)
		return false;

	if(m_Looks.Player.bySubType == CHARACTER_DEMON)
		return true;

	return false;
}

BubbleUpStruct & CCharacterAttr::NewBubbleUp()
{
	for(int i = 0; i < 8; i++)
	{
		if(m_BubbleUp[i].byType == 0)
			return m_BubbleUp[i];
	}

	m_BubbleUp[0].byType = 0;
	return m_BubbleUp[0];
}

void CCharacterAttr::ClearBubbleUp()
{
	for(int i = 0; i < 8; i++)
		m_BubbleUp[i].byType = 0;
}

BubbleUpStruct::BubbleUpStruct()
{
	dwStartTime = 0;
	dwValue = 0;
	byType = 0;
	dwLastTime = 3000;
}

int CCharacterAttr::CalStepCount(WORD wAction)
{
	int iStep = 1;

	if(wAction == ACTION_RUN)// || wAction == ACTION_ATTACK1)
	{
		if(m_Looks.Player.wHorse > 0)
			iStep = m_iRunStepOnLeopard;
		else
			iStep = 2;
	}

	return iStep;
}

void CCharacterAttr::SetStatus(WORD wState)
{
	WORD wOldState = m_wStatus; //原来的状态
	UINT id = GetID(); 

	if( wState & CS_LULL){
		output_debug("CCharacterAttr::SetStatus CS_LULL 石化!!! \n");
	}

	if(wState & CS_PROTECTSKIN)// 护身真气的出现
	{
		DWORD dwColor = GetReserveData(pubHushenColor);
		if(dwColor == 0)
			dwColor = 0xFFFFFFFF;

		WORD wMagicID = MAGICID_PROTECT_SKIN;
		if (m_wGreateMagicState & EG_PROTECTSKIN)
		{
			//BYTE byFlyType = ((m_wGreateMagicState & 0x8000) >> 15) + 1;//m_wGreateMagicState最高一个bit表示是神还是魔
			wMagicID = GetGreateMagicID(wMagicID,1,4,IsInDaoZunJiangLinState(),GetFightOnLeopard());
		}
		Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(wMagicID,id);
		if (pShow)	
			pShow->dwColor = dwColor;
		else		
			g_pMagicCtrl->CreateMagic(wMagicID,EMP_MAGIC_SERVER_ADVISE,id,id,-1,dwColor);
	}
	else if((wOldState & CS_PROTECTSKIN) && !(wState & CS_PROTECTSKIN))	// 护身真气的消失(需要延时设置标示位)
	{
		WORD wMagicID = MAGICID_PROTECT_SKIN;
		if (m_wGreateMagicState & EG_PROTECTSKIN)
		{
			//BYTE byFlyType = ((m_wGreateMagicState & 0x8000) >> 15) + 1;//m_wGreateMagicState最高一个bit表示是神还是魔
			wMagicID = GetGreateMagicID(wMagicID,1,4,IsInDaoZunJiangLinState(),GetFightOnLeopard());
		}
		g_pMagicCtrl->FinishMagic(g_pMagicCtrl->FindMagicByAll(wMagicID,id));

		wMagicID = MAGICID_PROTECT_SKIN_END;
		if (m_wGreateMagicState & EG_PROTECTSKIN)
		{
			//BYTE byFlyType = ((m_wGreateMagicState & 0x8000) >> 15) + 1;//m_wGreateMagicState最高一个bit表示是神还是魔
			wMagicID = GetGreateMagicID(wMagicID,1,4,IsInDaoZunJiangLinState(),GetFightOnLeopard());
		}

		g_pMagicCtrl->CreateMagic(wMagicID,EMP_MAGIC_SERVER_ADVISE,id,id);
	}

	//风火轮
	if(!(wOldState & CS_ROTATEFIRE) && (wState & CS_ROTATEFIRE))
	{
		WORD wMagicID = MAGICID_ROTATE_FIRE;
		if (m_wGreateMagicState & EG_ROTATE_FIRE)
		{
			//BYTE byFlyType = ((m_wGreateMagicState & 0x8000) >> 15) + 1;//m_wGreateMagicState最高一个bit表示是神还是魔
			wMagicID = GetGreateMagicID(wMagicID,1,4,IsInDaoZunJiangLinState(),GetFightOnLeopard());
		}
		if (!g_pMagicCtrl->FindMagicByAll(wMagicID,id))	
			g_pMagicCtrl->CreateMagic(wMagicID,EMP_MAGIC_SERVER_ADVISE,id,id);
	}
	else if((wOldState & CS_ROTATEFIRE) && !(wState & CS_ROTATEFIRE))
	{
		WORD wMagicID = MAGICID_ROTATE_FIRE;
		if (m_wGreateMagicState & EG_ROTATE_FIRE)
		{
			//BYTE byFlyType = ((m_wGreateMagicState & 0x8000) >> 15) + 1;//m_wGreateMagicState最高一个bit表示是神还是魔
			wMagicID = GetGreateMagicID(wMagicID,1,4,IsInDaoZunJiangLinState(),GetFightOnLeopard());
		}
		g_pMagicCtrl->FinishMagic(g_pMagicCtrl->FindMagicByAll(wMagicID,id));
	}

	if(wState & CS_SOULWALL)
	{	
		SetReserveData(pubSoulWallState,3);		//灵魂之墙正常
		SetReserveData(pubSoulWallFrame,0);		

		if (m_wGreateMagicState & EG_MAGICID_SOULWALL)
		{
			//BYTE byFlyType = ((m_wGreateMagicState & 0x8000) >> 15) + 1;//m_wGreateMagicState最高一个bit表示是神还是魔
			//if(byFlyType == 1)
			//{
			SetReserveData(pubSoulWallState,GetReserveData(pubSoulWallState) + 4);
			//}
			//else if(byFlyType == 2)
			//{
			//SetReserveData(pubSoulWallState,GetReserveData(pubSoulWallState) + 8);
			//}
		}
	}
	else if(wOldState & CS_SOULWALL)
	{
		SetReserveData(pubSoulWallState,2);		//灵魂之墙正常
		SetReserveData(pubSoulWallFrame,0);		

		if (m_wGreateMagicState & EG_MAGICID_SOULWALL)
		{
			//BYTE byFlyType = ((m_wGreateMagicState & 0x8000) >> 15) + 1;//m_wGreateMagicState最高一个bit表示是神还是魔
			//if(byFlyType == 1)
			//{
			SetReserveData(pubSoulWallState,GetReserveData(pubSoulWallState) + 4);
			//}
			//else if(byFlyType == 2)
			//{
			//SetReserveData(pubSoulWallState,GetReserveData(pubSoulWallState) + 8);
			//}
		}

		wState |= CS_SOULWALL;		//画完灵魂墙消失画面再取消状态
	}

	m_wStatus = wState;
}

void CCharacterAttr::SetGreateMagicState(WORD state)
{
	UINT id = GetID(); 
	//心灵护体
	if(state & EG_KULONGQIULONG)
	{
		WORD wMagicID = MAGICID_KULOUQIULONG;
		Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(wMagicID,id);
		if (!pShow)	
			g_pMagicCtrl->CreateMagic(wMagicID,EMP_MAGIC_SERVER_ADVISE,id,id,-1,-1);		
	}
	else if((m_wGreateMagicState & EG_KULONGQIULONG) && !(state & EG_KULONGQIULONG))	// 心灵护体的消失(需要延时设置标示位)
	{
		Magic_Show_s* ms = NULL;
		vector<Magic_Show_s*> VMs;
		while (ms = g_pMagicCtrl->FindMagicByAll(MAGICID_KULOUQIULONG,id,-1,-1,-1,ms))
		{
			VMs.push_back(ms);
		}

		for (size_t i = 0; i < VMs.size(); i++)
		{
			g_pMagicCtrl->CreateMagic(VMs[i]);//放到m_ShowList头部
			g_pMagicCtrl->FinishMagic(VMs[i]);
		}
		//Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(MAGICID_COLDSTROM,SELF.GetID());
		//g_pMagicCtrl->FinishMagic(pShow);

	}

	//法诀击中燃烧
	if(state & EG_FANZHEN_BURNNING)
	{
		WORD wMagicID = MAGICID_FAJUE_BURNNING;
		Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(wMagicID,id);
		if (!pShow)	
			g_pMagicCtrl->CreateMagic(wMagicID,EMP_MAGIC_SERVER_ADVISE,id,id,-1,-1);		
	}
	else if((m_wGreateMagicState & EG_FANZHEN_BURNNING) && !(state & EG_FANZHEN_BURNNING))	
	{
		Magic_Show_s* ms = NULL;
		vector<Magic_Show_s*> VMs;
		while (ms = g_pMagicCtrl->FindMagicByAll(MAGICID_FAJUE_BURNNING,id,-1,-1,-1,ms))
		{
			VMs.push_back(ms);
		}

		for (size_t i = 0; i < VMs.size(); i++)
		{
			g_pMagicCtrl->CreateMagic(VMs[i]);//放到m_ShowList头部
			g_pMagicCtrl->FinishMagic(VMs[i]);
		}
	}

	m_wGreateMagicState = state;
}

bool CCharacterAttr::IsSinging()
{
	return (m_ExtraState & ES_COOLICESTORM) == ES_COOLICESTORM;
}

void CCharacterAttr::SetDaoZunJiangLinState(bool val)
{
	UINT id = GetID(); 
	if (val)
	{
		WORD wMagicID = MAGICID_DAOZUN_JIANGLIN;
		Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(wMagicID,id);
		if (!pShow)	
			g_pMagicCtrl->CreateMagic(wMagicID,EMP_MAGIC_SERVER_ADVISE,id,id);
	}
	else
	{
		g_pMagicCtrl->FinishMagic(g_pMagicCtrl->FindMagicByAll(MAGICID_DAOZUN_JIANGLIN,id));
	}

	m_IsInDaoZunJiangLinState = val;
}

void CCharacterAttr::Set8DunState(bool val)
{
	UINT id = GetID(); 
	if (val)
	{
		WORD wMagicID = MAGICID_FIRE_SHIELD;
		Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(wMagicID,id);
		if (!pShow)	
			g_pMagicCtrl->CreateMagic(wMagicID,EMP_MAGIC_SERVER_ADVISE,id,id);
	}
	else
	{
		g_pMagicCtrl->FinishMagic(g_pMagicCtrl->FindMagicByAll(MAGICID_FIRE_SHIELD,id));
	}

	m_IsIn8DunState = val;
}

void CCharacterAttr::Set8DunShan(bool val)	
{
	if (!m_b8DunShan && val)
	{
		g_pMagicCtrl->CreateMagic(MAGICID_FIRE_SHIELD_HE,EMP_MAGIC_SERVER_ADVISE,GetID(),GetID());
	}
	m_b8DunShan = val; 
}

void CCharacterAttr::InitShadowNum(int iShadowNum)
{
	if (iShadowNum == 3)
	{
		m_iLeftShadowNum = 2;
		m_iRightShadowNum = 1;
	}
	else if (iShadowNum == 7)
	{
		m_iLeftShadowNum = 3;
		m_iRightShadowNum = 4;
	}
	else if (iShadowNum > 0 && iShadowNum <= 7)
	{
		m_iRightShadowNum = iShadowNum / 2;
		m_iLeftShadowNum = iShadowNum - m_iRightShadowNum;
	}
	else
	{
		m_iLeftShadowNum = 0;
		m_iRightShadowNum = 0;
	}
	m_iMaxShadowNum = (std::max)(m_iLeftShadowNum,m_iRightShadowNum);	
}