#pragma once
#include "GameData.h"
struct PHYLEMEMBER
{
	BYTE byType;
	std::string strName;
	PHYLEMEMBER()
	{
		byType = 0;//0是成员，1是宗主
		strName = "";
	}
};
typedef std::map<std::string,PHYLEMEMBER> GuildPhyleEle;

class CPhyleData
{
public:
	CPhyleData(void);
	~CPhyleData(void);
	struct sPhyleElement
	{
		std::string strName;//成员姓名
		BYTE	byPhyleType[4];//[0]表示宗派 0是元宗派，1是神宗，2是神宗
		bool bOnline;//是否在线
	};
	struct sPhyleMem 
	{
		DWORD dwBourn;//境界
		std::vector<sPhyleElement> vPhyleMem;
	};
public:
	bool AddPhyleMem(std::string& strName,std::string& strMem,BYTE byType,bool bOnline);
	bool DeletePhyleMem(std::string& strName,std::string& strMem);
	bool DeletePhyle(std::string& strName);
	std::map<std::string,sPhyleMem>& GetPhyle(){return m_mapGuildPhyle;}
	GuildPhyleEle& GetGuildPhyleMap(){ return m_mapGuildMemberPhyle;}
	bool IsPhyleMaster(std::string& strName);
protected:
	std::map<std::string,sPhyleMem> m_mapGuildPhyle;//行会宗派
	GuildPhyleEle	m_mapGuildMemberPhyle;
};
extern CPhyleData g_PhyleData;