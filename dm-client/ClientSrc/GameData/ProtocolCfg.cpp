#include "ProtocolCfg.h"
#include "Global/StringUtil.h"
#include "BaseClass/TiXml/tinyxml.h"

CProtocolsCfg g_ProtosCfg;

CProtocolsCfg::CProtocolsCfg(void)
{
	ReLoadProtocolCfg();
}

CProtocolsCfg::~CProtocolsCfg(void)
{
}

sProtocolCfg * CProtocolsCfg::GetProtoCfg(WORD id)
{
	MProtosCfg::iterator itr = m_MProtosCfg.find(id);
	if (itr != m_MProtosCfg.end())
	{
		return &(itr->second);
	}
	
	return NULL;	
}

bool CProtocolsCfg::ReLoadProtocolCfg()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\protocolcfg.xml",GetGameDataDir());
	return ReLoadProtosCfg(path, m_MProtosCfg);
}

bool CProtocolsCfg::ReLoadProtosCfg(const char* szXmlPath, MProtosCfg &mProtosCfg)
{
	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(szXmlPath)) //¶ÁÈ¡Ê§°Ü
		return false;

	mProtosCfg.clear();

	TiXmlElement* rootNode = xmlDoc.RootElement();
	TiXmlElement* node = rootNode->FirstChildElement();

	int pos = 0;
	int len = 0;
	std::string strtype;
	std::string strvar;

	for(;node;node = node->NextSiblingElement())
	{
		sProtocolCfg info;

		for(TiXmlAttribute* attr = node->FirstAttribute();attr;attr = attr->Next())
		{
			if(stricmp(attr->Name(),"id") == 0)
				info.id = (WORD)strtoul(attr->Value(),0,16);
			else if(stricmp(attr->Name(),"length") == 0)
				info.length = attr->IntValue();
		}

		TiXmlElement* child = node->FirstChildElement();
		for(;child;child = child->NextSiblingElement())
		{
			for(TiXmlAttribute* attr = child->FirstAttribute();attr;attr = attr->Next())
			{				
				if(stricmp(attr->Name(),"type") == 0)
					strtype = attr->Value();
				else if(stricmp(attr->Name(),"var") == 0)
					strvar = attr->Value();
				else if(stricmp(attr->Name(),"pos") == 0)
					pos = attr->IntValue();
				else if(stricmp(attr->Name(),"length") == 0)
					pos = attr->IntValue();
			}

			info.data.push_back(sProtocolData(strtype, strvar, pos, len));
		}

		mProtosCfg[info.id] = info;
	}
	return true;
}