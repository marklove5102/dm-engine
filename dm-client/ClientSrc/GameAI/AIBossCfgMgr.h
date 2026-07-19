#pragma once

#include "Global/Global.h"

struct CBossClass
{
	CBossClass()
	{
		bAlarm = false;
		iType  = 0;
	}
	string strName;
	bool   bAlarm;
	int    iType;
};

class CAIBossCfgMgr
{
public:
	CAIBossCfgMgr(void);
	~CAIBossCfgMgr(void);

	bool LoadBossConfig();

	//怪物列表
	bool IsBoss(const char* name);
	void RemoveBoss(const char* name);
	bool IsBlack(const char* name);
	bool AddBlackName(char* str);
	bool AddChouDi(char* name);
	bool RemoveBlackName(const char* str);

	void FlipAlarm(int i);
	void FlipBossType(int i);
	int  InsertMonster(const char* strName,int pos = -1);
	int  RemoveMonster(int pos);
	void WriteConfigFile();
	void ChangeMosterState(CBossClass* pBig,bool bAdd);
	CBossClass* GetBossClass(int i) const;
	int  GetBossClassCount() const { return (int)m_vecOldBig.size(); }

private:

	//BOSS列表
	typedef vector<CBossClass*> VBossClass;
	typedef map<string,string>  NameMap;  
	VBossClass m_vecOldBig;
	NameMap    m_mapBoss;
	NameMap    m_mapBlack;
	string m_strBossFile;
};

extern CAIBossCfgMgr g_BossCfgMgr;
