#pragma once

#include "Global/Global.h"

class CSDSamplerMgr
{
public:
	CSDSamplerMgr(void);
	~CSDSamplerMgr(void);


	bool SendGameVersionSample();
	bool SendPlayerInfoSample();
	bool SendSocietyInfoSample();
	bool SendPlayerActionSample();
	bool SendClientInfoSample();
	bool SendQualitySample();

};


extern CSDSamplerMgr g_SdSamplerMgr;