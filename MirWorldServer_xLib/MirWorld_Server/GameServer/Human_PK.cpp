#include "StdAfx.h"
#include "humanplayer.h"
#include "gameworld.h"
#include "sandcity.h"
#include "LogicMap.h"
#include "guildex.h"
#include "gsclientobj.h"

// ==================== PK系统 / 红名 / 安全区 ====================

BOOL CHumanPlayer::NoLawProtect()
{
	if (this->IsStatusSet(SI_ITEMTRACED))return TRUE;
	if (m_bJustPk || GetPkValue() >= CGameWorld::GetInstance()->GetVar(EVI_REDPKPOINT))
		return TRUE;
	return FALSE;
}

VOID CHumanPlayer::CheckPk(CAliveObject* pTarget)
{
	if (pTarget == this)return;
	if (CSandCity::GetInstance()->IsWarStarted() && InWarArea())
		return;
	if (pTarget->GetType() == OBJ_PLAYER && GetGuild() && GetGuild()->IsKillGuildMember((CHumanPlayer*)pTarget))
		return;
	//战斗地图
	if (m_pMap && m_pMap->IsFlagSeted(MF_FIGHTMAP))
		return;
	//如果目标不是玩家或者受保护
	if (pTarget->GetType() != OBJ_PLAYER || ((CHumanPlayer*)pTarget)->NoLawProtect())
		return;
	if (m_bJustPk)
		m_tmrJustPk.Savetime();
	else if (GetPkValue() < CGameWorld::GetInstance()->GetVar(EVI_REDPKPOINT))
	{
		m_bJustPk = TRUE;
		m_tmrJustPk.Savetime();
		SendChangeName();
	}
}

VOID CHumanPlayer::OnEnterCityArea()
{
	if (m_pMap != nullptr)
		m_pMap->CheckEnterCity(this);
	if (GetGuild() && GetGuild()->GetKillGuildCount() > 0)
		this->ReviewAroundNameColor();
}

VOID CHumanPlayer::OnLeaveCityArea()
{
	if (GetGuild() && GetGuild()->GetKillGuildCount() > 0)
		this->ReviewAroundNameColor();
}

constexpr BYTE NCOLOR_GRAY = 61;
constexpr BYTE NCOLOR_RED = 58;
constexpr BYTE NCOLOR_YELLOW = 251;
constexpr BYTE NCOLOR_NORMAL = 255;
BYTE CHumanPlayer::GetNameColor(CMapObject* pViewer)
{
	if (CSandCity::GetInstance()->IsWarStarted() && InWarArea() && pViewer != nullptr && pViewer->GetType() == OBJ_PLAYER)
	{
		CGuildEx* pGuild = GetGuild();
		CGuildEx* pViewerGuild = ((CHumanPlayer*)pViewer)->GetGuild();
		BYTE btNormal = static_cast<BYTE>(CGameWorld::GetInstance()->GetVar(EVI_WARNORMALCOLOR));
		BYTE btEnemy = static_cast<BYTE>(CGameWorld::GetInstance()->GetVar(EVI_WARENEMYCOLOR));
		BYTE btAlly = static_cast<BYTE>(CGameWorld::GetInstance()->GetVar(EVI_WARALLYCOLOR));
		if (pViewerGuild == nullptr) // 普通人看来
		{
			return btNormal;
		}
		else if (pViewerGuild == CSandCity::GetInstance()->GetOwnerGuild()) // 沙城人来看
		{
			if (pGuild == nullptr)return btNormal;
			if (pGuild->IsAttackSabuk())return btEnemy;
			if (pGuild == pViewerGuild)return btAlly;
			return btNormal;
		}
		else if (pViewerGuild->IsAttackSabuk())
		{
			if (pGuild == nullptr)return btNormal;
			if (pGuild->IsAttackSabuk())return btAlly;
			if (pGuild == CSandCity::GetInstance()->GetOwnerGuild())return btEnemy;
			return btNormal;
		}
		else
			return btNormal;
	}
	else if (GetGuild() != nullptr &&
		pViewer != nullptr && pViewer->GetType() == OBJ_PLAYER && !((CHumanPlayer*)pViewer)->InCityArea() &&
		((CHumanPlayer*)pViewer)->GetGuild() && ((CHumanPlayer*)pViewer)->GetGuild()->GetKillGuildCount() > 0)
	{
		if (((CHumanPlayer*)pViewer)->GetGuild()->IsAllyGuild(GetGuild()) || ((CHumanPlayer*)pViewer)->GetGuild() == GetGuild())
			return 0xfc;
		else if (((CHumanPlayer*)pViewer)->GetGuild()->IsKillGuild(GetGuild()))
			return 0x45;
	}
	if (IsSystemFlagSeted(SF_SPECIALNAMECOLOR))
		return (BYTE)m_SystemFlag.GetParam(SF_SPECIALNAMECOLOR);
	if (GetPkValue() >= CGameWorld::GetInstance()->GetVar(EVI_REDPKPOINT))
		return NCOLOR_RED;
	if (m_bJustPk)
		return NCOLOR_GRAY;
	if (GetPkValue() >= CGameWorld::GetInstance()->GetVar(EVI_YELLOWPKPOINT))
		return NCOLOR_YELLOW;
	return NCOLOR_NORMAL;
}

VOID CHumanPlayer::AddPkPoint(DWORD btPoint)
{
	BYTE btColor = GetNameColor(this);
	this->m_dwPkValue += btPoint;
	if (GetPkValue() >= CGameWorld::GetInstance()->GetVar(EVI_REDPKPOINT))
	{
		if (m_bJustPk)
			m_bJustPk = FALSE;
	}
	else if (!m_bJustPk)
		m_bJustPk = TRUE;
	if (m_bJustPk)
		m_tmrJustPk.Savetime();
	BYTE btColor2 = GetNameColor(this);
	if (btColor != btColor2)
		SendChangeName();
}

VOID CHumanPlayer::DecPkPoint(DWORD btPoint)
{
	BYTE btColor = GetNameColor(this);
	if (this->m_dwPkValue < btPoint)
		this->m_dwPkValue = 0;
	else
		this->m_dwPkValue -= btPoint;
	BYTE btColor2 = GetNameColor(this);
	if (btColor != btColor2)
		SendChangeName();
}

VOID CHumanPlayer::OnEnterSafeArea()
{
	if (CGameWorld::GetInstance()->GetVar(EVI_ENABLESAFEAREANOTICE))
		SaySystem("您进入了安全区～");
}

VOID CHumanPlayer::OnLeaveSafeArea()
{
	if (!IsDeath() && CGameWorld::GetInstance()->GetVar(EVI_ENABLESAFEAREANOTICE))
		SaySystem("您离开了安全区～");
}

VOID CHumanPlayer::OnEnterWarArea()
{
	if (CSandCity::GetInstance()->IsWarStarted())
	{
		SaySystem("您进入了攻城区域～");
		UpdateViewName();
	}
}

VOID CHumanPlayer::OnLeaveWarArea()
{
	if (!IsDeath() && CSandCity::GetInstance()->IsWarStarted())
	{
		SaySystem("您离开了攻城区域～");
		UpdateViewName();
	}
}
