#include "PhyleData.h"
CPhyleData g_PhyleData;
CPhyleData::CPhyleData(void)
{
}

CPhyleData::~CPhyleData(void)
{
}

bool CPhyleData::IsPhyleMaster(std::string& strName)
{
	GuildPhyleEle::iterator it = m_mapGuildMemberPhyle.find(strName);
	if (it != m_mapGuildMemberPhyle.end())
	{
		return (it->second.byType == 1);
	}
	return false;
}

bool CPhyleData::AddPhyleMem(std::string& strName,std::string& strMem,BYTE byType,bool bOnline)
{
	if (strName.size() <= 0 || strMem.size() <= 0)
		return false;

	std::vector<sPhyleElement>& vMem = m_mapGuildPhyle[strName].vPhyleMem;
	bool bFound = false;
	std::vector<sPhyleElement>::iterator it = vMem.begin();
	while (it != vMem.end())
	{
		if(it->strName.compare(strMem.c_str()) == 0 )
		{
			bFound = true;
			break;
		}
		it++;
	}

	if (!bFound)
	{
		sPhyleElement ele;
		ele.strName = strMem;
		ele.byPhyleType[0] = byType;
		ele.bOnline = bOnline;
		vMem.push_back(ele);
		return true;
	}

	return false;
}

bool CPhyleData::DeletePhyleMem(std::string& strName,std::string& strMem)
{
	if (strName.size() <= 0 || strMem.size() <= 0)
		return false;

	std::map<std::string,sPhyleMem>::iterator it = m_mapGuildPhyle.find(strName);
	if (it != m_mapGuildPhyle.end())
	{
		std::vector<sPhyleElement>& vMem = it->second.vPhyleMem;

		bool bFound = false;
		std::vector<sPhyleElement>::iterator itMem = vMem.begin();
		while (itMem != vMem.end())
		{
			if(itMem->strName.compare(strMem.c_str()) == 0 )
			{
				bFound = true;
				vMem.erase(itMem);
				break;
			}
			itMem++;
		}

		if (vMem.size() <= 0)
			m_mapGuildPhyle.erase(it);

		return bFound;
	}

	return  false;
}

bool CPhyleData::DeletePhyle(std::string& strName)
{
	if (strName.size() <= 0)
		return false;

	std::map<std::string,sPhyleMem>::iterator it = m_mapGuildPhyle.find(strName);
	if (it != m_mapGuildPhyle.end())
	{
		m_mapGuildPhyle.erase(it);
		return true;
	}

	return false;
}