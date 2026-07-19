#pragma once

#include "Global/Global.h"

struct WOOOLMEDICA
{
	WOOOLMEDICA()
	{
		hp_limit = 0;
		hp_delay = 0;
		hp_bused = 0;
		mp_limit = 0;
		mp_delay = 0;
		mp_bused = 0;
	}
	WOOOLMEDICA(const WOOOLMEDICA& m)
	{
		hp_delay	= m.hp_delay;
		hp_limit	= m.hp_limit;
		hp_bused	= m.hp_bused;
		mp_delay	= m.mp_delay;
		mp_limit	= m.mp_limit;
		mp_bused	= m.mp_bused;
	}
	void Clear()
	{
		hp_limit = 0;
		hp_delay = 0;
		hp_bused = 0;
		mp_limit = 0;
		mp_delay = 0;
		mp_bused = 0;
	}
	int         hp_delay;		//补红的延迟
	int         hp_limit;		//补红的下限
	int         hp_bused;		//是否使用
	int         mp_delay;		//补蓝的延迟
	int			mp_limit;		//补蓝的下限
	int			mp_bused;		//是否使用
};

struct WOOOLMEDICAEAT : public WOOOLMEDICA
{
	void Init(const char* szHPName,const char* szHPPack,const char* szMPName,const char* szMPPack)
	{
		sHPName.assign(szHPName);
		sHPPack.assign(szHPPack);
		sMPName.assign(szMPName);
		sMPPack.assign(szMPPack);

		dwUseHPPackTime = 0;
		dwUseMPPackTime = 0;
		dwLastHPTime = 0;
		dwLastMPTime = 0;

		Clear();
	}
	string sHPName;
	string sHPPack;
	string sMPName;
	string sMPPack;

	DWORD  dwUseHPPackTime;
	DWORD  dwUseMPPackTime;
	DWORD  dwLastHPTime;
	DWORD  dwLastMPTime;
};

#define MAX_MEDICA_NUMBER            9
