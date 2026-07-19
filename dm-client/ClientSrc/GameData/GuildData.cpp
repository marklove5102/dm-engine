#include "guilddata.h"
#include "GameData.h"

CGuildData   g_GuildData;

CGuildData::CGuildData(void)
{
	m_bFirstGuild = true;
	m_dwGuildSteleFhgValue = 0;
	m_wSteleHeroNum = 0;
	m_dwSteleLevelNum = 0;
}

CGuildData::~CGuildData(void)
{
}
void CGuildData::Clear()
{
	m_killally.clear();
	m_ally.clear();
	m_guildName.clear();
	m_guildChairMan.clear();
	m_bulletin.clear();
	m_member.clear();
	m_strWeb.clear();
	//m_chat.Clear();
	m_memberTemp.clear();
	m_TempBulletin.clear();
	m_strWeb.clear();
	m_bGuildHeader = false;
	m_bFirstGuild = true;
	m_vOfficer.clear();
	m_vMembers.clear();
	m_mapGuildOfficer.clear();
	m_vOnlineTael.clear();
	m_bAdoptNewGuild = false;
	m_bGuildBuffSwitch = false;
	m_dwGuildTael = 0;
	m_dwGuildFunc = 0;
	m_iGuildFlag = 0;
	m_dwTowerLevel = 0;
	m_byGuildPhyle = 0;
	memset(&m_sGuildBuffer,0,sizeof(m_sGuildBuffer));
	memset(&m_sGuildTower,0,sizeof(m_sGuildTower));
}
void CGuildData::SetBulletin(const char * str)
{
	m_bulletin.push_back(str); 
}

vector<string> & CGuildData::GetBulletin()
{
	return m_bulletin; 
}

void CGuildData::SetAlly(const char * str)
{
	m_ally.push_back(str);
}
vector<string> & CGuildData::GetAlly()
{
	return m_ally; 
}

void CGuildData::SetKillAlly(const char * str)
{
	m_killally.push_back(str);
}
vector<string> & CGuildData::GetKillAlly()
{
	return m_killally; 
}

void CGuildData::SetGuildName(const char * str)
{
	m_guildName = str; 
}
const char * CGuildData::GetGuildName()
{
	return m_guildName.c_str();
}

vector<CGuildData::_Member> & CGuildData::GetMember()
{
	return m_member; 
}

void CGuildData::SetMemberTemp(const char * str)
{
	m_memberTemp.push_back(str); 
}
vector<string> & CGuildData::GetMemberTemp()
{
	return m_memberTemp; 
}

void CGuildData::SetTempBulletin(const char * str)
{
	m_TempBulletin.push_back(str); 
}
vector<string> & CGuildData::GetTempBulletin()
{
	return m_TempBulletin; 
}

//CTalkContent & CGuildData::GetChat()
//{
//	return m_chat;
//}

bool CGuildData::IsHeader(bool bIncludeAssit)
{
	bool bRet = false;

	std::string strName = SELF.GetName();
	for (int i = 0;i<m_vOfficer.size();i++)
	{
		sTileNode& stilenode = m_vOfficer[i];
		if (stilenode.wNum == 1 || (stilenode.wNum == 2 && bIncludeAssit))//会长副会长专用职位
		{
			for (int j = 0;j<stilenode.vMember.size();j++)
			{
				CGuildData::_NewMember& newMem = stilenode.vMember[j];

				if(strName.compare(newMem.strName.c_str()) ==0)
					return true;					
			}
		}
	}

	//行会数据没有传过来时可据此判断
	std::string str = g_GuildData.GetChairMan();
	if(str.compare(SELF.GetName()) == 0)
		bRet = true;

	return bRet;
}

void CGuildData::SetHeader(bool b)
{
	m_bGuildHeader = b; 
}

void CGuildData::AddGuildMember(const char* pName,const char* pTitle)
{
	bool bRet = false;
	int iSize =  m_member.size();
	for(int i = 0;!bRet && i< iSize;i++)
	{
		if(m_member[i].title.compare(pTitle) != 0)
			continue;

		std::vector<std::string>& strVecName = m_member[i].name;

		int iNum = strVecName.size();
		for(int i = 0;i<iNum;i++)
		{
			if(strcmp(pName,strVecName[i].c_str()) == 0)
				return;
		}

		strVecName.push_back(pName);
		bRet = true;
	}
	if(!bRet)
	{
		CGuildData::_Member member;
		member.title = pTitle;
		member.num="1";
		member.name.push_back(pName);				
		m_member.push_back(member);			
	}
}

void CGuildData::ChangeGuildTitle(const char* pName,const char* pNewTitle,const char* pOldTitle)
{
	int iSize =  m_member.size();

	//改变封号
	//删除封号
	bool bRet = false;
	for(int i = 0;!bRet && i< iSize;i++)
	{
		if(m_member[i].title.compare(pOldTitle) != 0)
			continue;

		std::vector<std::string>& strVecName = m_member[i].name;
		for(int j = 0;j< strVecName.size();j++)
		{
			if(strVecName[j].compare(pName) == 0)
			{
				strVecName.erase(strVecName.begin()+j);
				if(strVecName.size() == 0) m_member.erase(m_member.begin()+i);
				bRet = true;
				break;
			}
		}
	}

	AddGuildMember(pName,pNewTitle);
}

void CGuildData::SetGuildFlag(int iFlag)
{
	m_iGuildFlag = iFlag;
	//SELF.SetGuildFlag(m_iGuildFlag);
}

int CGuildData::GetGuildFlag()
{
	if(m_dwTowerLevel == 0)
	{
		return 0;
	}

	if(m_iGuildFlag > 0 && m_iGuildFlag < 5)
	{
		return m_iGuildFlag;
	}
	else if(m_dwTowerLevel > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void CGuildData::AddGuildMemeberOfficerMap(WORD wCoreNum,std::string& strName)
{
	if(wCoreNum <=0 || wCoreNum > 10) return;

	std::map<int,CGuildData::sOfficer>& mOfficer = g_GuildData.GetGuildOfficerMap();
	std::map<int,CGuildData::sOfficer>::iterator it = mOfficer.find(wCoreNum);

	if (it != mOfficer.end())
	{
		m_mapOfficerMap[strName] = it->second.strTile;
	}
}

std::string CGuildData::GetOfficerByName(std::string& strName)
{
	std::map<std::string,std::string>::iterator it = m_mapOfficerMap.find(strName);
	if (it != m_mapOfficerMap.end())
	{
		return it->second;
	}

	return "";
}