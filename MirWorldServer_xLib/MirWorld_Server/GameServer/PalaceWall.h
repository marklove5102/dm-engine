#pragma once
#include "monsterex.h"

class CPalaceWall : public CMonsterEx
{
public:
	CPalaceWall(VOID);
	virtual ~CPalaceWall(VOID);
	BOOL Init(const char* pszName, int mapid, int x, int y, WORD wHp);
	VOID OnDeath(DWORD dwKiller) override;
	VOID OnDamage(CAliveObject* pAttacker, int nDamage, damage_type type) override;
	VOID Repair();
	VOID OnEnterMap(CLogicMap* pMap) override;
	VOID CleanAroundBlock();
	VOID SetAroundBlock();
};
