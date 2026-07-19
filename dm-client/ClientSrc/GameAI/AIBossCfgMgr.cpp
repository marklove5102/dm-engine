#include "AIBossCfgMgr.h"
#include "AIConfigMgr.h"
#include "GameData/TalkMgr.h"

CAIBossCfgMgr g_BossCfgMgr;

CAIBossCfgMgr::CAIBossCfgMgr(void)
{
}

CAIBossCfgMgr::~CAIBossCfgMgr(void)
{
	while(!m_vecOldBig.empty())
	{
		CBossClass* pBig = m_vecOldBig.back();
		SAFE_DELETE(pBig);
		m_vecOldBig.pop_back();
	}
}


bool CAIBossCfgMgr::LoadBossConfig()
{
	char temp[1024] = {0};
	sprintf(temp,"%s\\config\\%s\\Boss.ini",GetGameDataDir(),g_AICfgMgr.GetIDPath());
	m_strBossFile.assign(temp);

	FILE* fp = fopen(m_strBossFile.c_str(),"rt");
	if(!fp)
		return false;

	char szLine[1024] = {0};
	while(!m_vecOldBig.empty())
	{
		CBossClass* pBig = m_vecOldBig.back();
		SAFE_DELETE(pBig);
		m_vecOldBig.pop_back();
	}

	m_mapBoss.clear();
	m_mapBlack.clear();

	while(!feof(fp))
	{
		memset(szLine,0,1024);
		if(NULL == fgets(szLine,1024,fp))
			break;

		//trim(szLine);

		if(strlen(szLine) == 0)
			continue;

		if(szLine[0]==',' || szLine[0]=='#')
			continue;

		string str;
		str.assign(szLine);
		int iPos = str.find_first_of('=');
		int iCommaPos = -1;
		if(iPos>0)
			iCommaPos = str.find_first_of(',',iPos);

		if(iPos>0 && iCommaPos>iPos+1)
		{
			string strOn = str.substr(iPos+1,iCommaPos-iPos-1);

			if(strOn.size() <= 0)
				continue;

			int iOn = atoi(strOn.c_str());
			string name = str.substr(0,iPos);

			if(name.size() == 0)  continue;

			string strType = str.substr(iCommaPos+1,str.size()-iCommaPos-1);
			int iType = atoi(strType.c_str());

			//ADD TO VECTOR
			CBossClass* pBig = new CBossClass();
			pBig->strName = name;
			if(iOn > 0)
				pBig->bAlarm = true;
			else
				pBig->bAlarm = false;

			if(iType < 0 || iType >= 3) iType = 0;
			pBig->iType = iType;
			m_vecOldBig.push_back(pBig);

			if(iOn > 0)
			{
				if(iType == 0)
					m_mapBoss[name] = name;
				else if(iType == 2)
					m_mapBlack[name] = name;
			}
		}
	}
	fclose(fp);
	return true;
}

void CAIBossCfgMgr::RemoveBoss(const char* name)
{
	if(NULL == name) return;

	string strName = name;
	if(strName.empty())	return;

	NameMap::iterator it = m_mapBoss.find(strName);
	if(it != m_mapBoss.end())
	{
		m_mapBoss.erase(it);
	}
}

bool CAIBossCfgMgr::IsBoss(const char* name)
{
	if(NULL == name)
		return false;

	string strName = name;
	if(strName.empty())
		return false;

	NameMap::iterator it = m_mapBoss.find(strName);
	if(it != m_mapBoss.end())
	{
		string& str = it->second;
		if (str.compare(strName) == 0)
			return true;
		else
			return false;
	}

	return false;
}

bool CAIBossCfgMgr::IsBlack(const char* name)
{
	if(NULL == name)
		return false;

	string strName = name;
	if(strName.empty())
		return false;

	NameMap::iterator it = m_mapBlack.find(strName);

	if(it != m_mapBlack.end())
	{
		string& str = it->second;
		if(str.compare(strName) == 0)
			return true;
		else
			return false;
	}

	return false;
}

bool CAIBossCfgMgr::AddBlackName(char* name)
{
	if(NULL == name || strlen(name) == 0)
		return false;

	//int  iLen = strlen(name);
	//while(iLen>0)
	//{
	//	if(name[iLen-1] == ' ')
	//		iLen--;
	//	else
	//		break;
	//}

	//if(iLen == 0)
	//	return false;

	//string str = name;
	//str = str.substr(0,iLen);

	char tmpName[256] = {0};
	strcpy(tmpName,name);
	StringUtil::trim(tmpName);

	char temp[1024] = {0};
	sprintf(temp,"已将%s加入黑名单",tmpName);
	g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);

	NameMap::iterator it = m_mapBlack.find(tmpName);
	if(it != m_mapBlack.end())
		return true;

	m_mapBlack[tmpName] = tmpName;

	//ADD TO VECTOR
	CBossClass* pBig = new CBossClass();
	pBig->strName = tmpName;
	pBig->bAlarm = true;
	pBig->iType = 2;
	m_vecOldBig.push_back(pBig);

	WriteConfigFile();

	return true;
}

bool CAIBossCfgMgr::AddChouDi(char* name)
{
	if(NULL == name || strlen(name) == 0)
		return false;

	int  iLen = strlen(name);
	while(iLen>0)
	{
		if(name[iLen-1] == ' ')
			iLen--;
		else
			break;
	}

	if(iLen == 0)
		return false;

	string str = name;
	str = str.substr(0,iLen);

	//ADD TO VECTOR
	CBossClass* pBig = new CBossClass();
	pBig->strName = name;
	pBig->bAlarm = true;
	pBig->iType = 1;
	m_vecOldBig.push_back(pBig);

	WriteConfigFile();

	return true;
}

void CAIBossCfgMgr::WriteConfigFile()
{
	FILE* fp = NULL;
	fp = fopen(m_strBossFile.c_str(),"wt");
	if(fp == NULL)
		return;

	VBossClass::iterator it;
	for(it = m_vecOldBig.begin();it != m_vecOldBig.end();it++)
	{
		CBossClass* pBig = *it;
		if(pBig)
		{
			fprintf(fp,"%s=%d,%d\n",pBig->strName.c_str(),pBig->bAlarm?1:0,pBig->iType);
		}
	}
	fclose(fp);
}

CBossClass* CAIBossCfgMgr::GetBossClass(int i) const
{
	if(i < 0 || i>= (int)m_vecOldBig.size())
		return NULL;

	return m_vecOldBig[i];
}

void CAIBossCfgMgr::ChangeMosterState(CBossClass* pBig,bool bAdd)
{
	if(NULL == pBig)
		return;

	if(pBig->iType == 0)
	{
		NameMap::iterator it = m_mapBoss.find(pBig->strName);

		if(bAdd)
		{
			if(it == m_mapBoss.end() && !pBig->strName.empty())
				m_mapBoss[pBig->strName] = pBig->strName;
		}
		else
		{
			if(it != m_mapBoss.end())
				m_mapBoss.erase(it);
		}
	}
	else if(pBig->iType == 2)
	{
		NameMap::iterator it = m_mapBlack.find(pBig->strName);

		if(bAdd)
		{
			if(it == m_mapBlack.end() && !pBig->strName.empty())
				m_mapBlack[pBig->strName] = pBig->strName;
		}
		else
		{
			if(it != m_mapBlack.end())
				m_mapBlack.erase(it);
		}
	}
}

void CAIBossCfgMgr::FlipAlarm(int i)
{
	CBossClass* pBig = GetBossClass(i);
	if(pBig)
	{
		pBig->bAlarm = !pBig->bAlarm;
		ChangeMosterState(pBig,false); //删除怪物
		
		if (pBig->bAlarm)
			m_mapBoss[pBig->strName] = pBig->strName;

		WriteConfigFile();
	}
}

void CAIBossCfgMgr::FlipBossType(int i)
{
	CBossClass* pBig = GetBossClass(i);
	if(pBig)
	{
		pBig->iType %= 3;
		ChangeMosterState(pBig,false); //删除老的

		if(pBig->iType == 0)
			pBig->iType = 2;
		else
			pBig->iType = 0;

		ChangeMosterState(pBig,true);  //添加新的

		WriteConfigFile();
	}
}

int  CAIBossCfgMgr::InsertMonster(const char* strName,int pos)
{
	if(strName == NULL || strlen(strName) == 0)
		return -1;

	//已经存在了
	for(int i = 0;i < (int)m_vecOldBig.size();i++)
	{
		if(m_vecOldBig[i]->strName.compare(strName) == 0)
			return i;
	}

	CBossClass* pClass = new CBossClass();
	pClass->strName = strName;
	pClass->bAlarm = true;
	pClass->iType  = 0;

	ChangeMosterState(pClass,true);

	m_vecOldBig.push_back(pClass);
	WriteConfigFile();

	return ((int)m_vecOldBig.size() - 1);
}

bool CAIBossCfgMgr::RemoveBlackName(const char* str)
{
	CBossClass* pBoss = NULL;
	if(!str || strlen(str) <= 0) return false;	

	int iSize = GetBossClassCount();
	for(int i = 0;i< iSize;i++)
	{
		pBoss = GetBossClass(i);

		if(pBoss->iType == 2 && pBoss->strName.compare(str)== 0)
		{
			RemoveMonster(i);
			NameMap::iterator it = m_mapBlack.find(str);
			if(it != m_mapBlack.end())
			{
				m_mapBlack.erase(it);
				return true;
			}
			break;
		}
	}
	return false;
}

int CAIBossCfgMgr::RemoveMonster(int pos)
{
	if(pos < 0 || pos >= (int)m_vecOldBig.size())
		return -1;

	CBossClass* pClass  = m_vecOldBig[pos];
	if(NULL == pClass)
		return -1;

	VBossClass::iterator iv = m_vecOldBig.begin() + pos;
	RemoveBoss(pClass->strName.c_str());
	m_vecOldBig.erase(iv);

	SAFE_DELETE(pClass);

	WriteConfigFile();	
	return -1;
}