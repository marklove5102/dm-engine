#include "RangeXmlMgr.h"

CRangeXmlMgr g_RangeXmlMgr;

CRangeXmlMgr::CRangeXmlMgr(void)
{
	LoadRangeHelp();
}

CRangeXmlMgr::~CRangeXmlMgr(void)
{
}

bool CRangeXmlMgr::LoadRangeHelp()
{
	char szBuffer[2048] = {0};
	char szPath[1024] ={0};
	sprintf(szPath,"%s\\config\\rangehelp.txt",GetGameDataDir());
	FILE* fp = fopen(szPath,"rt");
	if(NULL == fp)
		return false;

	tagRangeHelp help;
	bool bReadRange = false;

	while(!feof(fp))
	{
		ZeroMemory(szBuffer,2000);
		if(NULL == fgets(szBuffer,2000,fp))
			break;

		char* szLine = strtok(szBuffer,"\r\n");
		if(NULL == szLine) continue;

		string str = szLine;
		if(str.size() <= 0) continue;

		if(str.size()>12 && str.substr(0,12) == "<RANGE TYPE=")
		{
			int iEndPos = str.find_first_of('>',12);
			if(iEndPos<0) iEndPos = str.size()-1;
			help.strName = str.substr(12,iEndPos-12);
			if(help.strName.empty())
				bReadRange = false;
			else
				bReadRange = true;
		}
		else if(str.size()>=8 && str.substr(0,8) == "</RANGE>")
		{
			if(bReadRange)
			{
				m_vecRange.push_back(help);
				help.strDesc.clear();
			}
		}
		else
		{
			if(bReadRange)
			{
				help.strDesc.push_back(str);
			}
		}
	}

	fclose(fp);
	return true;
}

vector<string>* CRangeXmlMgr::GetRangeDesc(const char* szName)
{
	if(NULL == szName || strlen(szName) == 0)
		return false;

	for(int i=0;i<(int)m_vecRange.size();i++)
	{
		if(m_vecRange[i].strName.compare(szName) == 0)
			return &m_vecRange[i].strDesc;
	}
	return NULL;
}