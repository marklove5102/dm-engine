#pragma once
#include "Global/Global.h"
#include "Global/StdHeaders.h"
#include "Global/Interface/FontInterface.h"

struct sProtocolData
{
	sProtocolData(const std::string& atype, const std::string& avar,int apos, int alen)
		:strtype(atype),strvar(avar),pos(apos),len(alen)
	{

	}
	int pos;
	int len;
	std::string strtype;
	std::string strvar;
};

struct sProtocolCfg
{
	sProtocolCfg()
		:id(0),length(12)
	{
	}

	WORD id;
	int length;
	std::vector<sProtocolData> data;
};


typedef map<WORD,sProtocolCfg> MProtosCfg;

class CProtocolsCfg
{
public:
	CProtocolsCfg(void);
	~CProtocolsCfg(void);

public:
	sProtocolCfg * GetProtoCfg(WORD id);
	bool 	ReLoadProtocolCfg();

protected:
	MProtosCfg m_MProtosCfg;

	bool 	ReLoadProtosCfg(const char* szXmlPath, MProtosCfg &mProtosCfg);
};

extern CProtocolsCfg g_ProtosCfg;
