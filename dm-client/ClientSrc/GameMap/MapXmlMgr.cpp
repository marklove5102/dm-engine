#include "MapXmlMgr.h"

CMapXmlMgr g_MapXmlMgr;

BEGIN_VAR(CMapXmlEntry,CXmlObj)
INIT_VAR(string,Src,"")
INIT_VAR(string,Dst,"")
INIT_VAR(string,MinMap,"")
END_VAR(CMapXmlEntry,CXmlObj)

BEGIN_VAR(CMapXmlMgr,CXmlObj)
END_VAR(CMapXmlMgr,CXmlObj)

CMapXmlMgr::CMapXmlMgr(void)
{
}

CMapXmlMgr::~CMapXmlMgr(void)
{
}

bool CMapXmlMgr::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	if(stricmp(pValue,"map") == 0)
	{
		CMapXmlEntry entry;
		entry.Create();
		m_map.push_back(entry);
		return true;
	}
	return false;
}

const char * CMapXmlMgr::GetRealMap(const char * strMapName)
{
	for(int i = 0;i < m_map.size(); i++)
	{
		string src = m_map[i].GetSrc();
		int pos = src.find_first_of('*');
		if(pos >= 0) //找到通配符'*'
		{
			string tmp = src.substr(0,pos);
			if(!tmp.empty())
			{
				if(strnicmp(tmp.c_str(),strMapName,tmp.size()) == 0)
					return m_map[i].GetDst().c_str();
			}
			else
			{
				return strMapName;
			}
		}
		else
		{
			if(stricmp(src.c_str(),strMapName) == 0)
				return m_map[i].GetDst().c_str();
		}
	}
	return strMapName;
}

const char * CMapXmlMgr::GetLocalMap(const char * strMapName)
{
	for(int i = 0;i < m_map.size(); i++)
	{
		string src = m_map[i].GetSrc();
		int pos = src.find_first_of('*');
		if(pos >= 0) //找到通配符'*'
		{
			string tmp = src.substr(0,pos);
			if(!tmp.empty())
			{
				if(strnicmp(tmp.c_str(),strMapName,tmp.size()) == 0)
					return m_map[i].GetMinMap().c_str();
			}
			else
			{
				return "";
			}
		}
		else
		{
			if(stricmp(src.c_str(),strMapName) == 0)
				return m_map[i].GetMinMap().c_str();
		}
	}
	return "";
}