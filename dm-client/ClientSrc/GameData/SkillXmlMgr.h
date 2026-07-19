/*#pragma once
#include <map>
#include <string>
#include "Global/StringUtil.h"

class CSkillXmlMgr
{
	typedef std::map<string,VString> MSkill;

public:
	CSkillXmlMgr();
	~CSkillXmlMgr();
	bool LoadFile(const char*);
	VString* GetSkillDesc(const char* name);
protected:
	MSkill  m_Skills;
};

extern CSkillXmlMgr g_SkillXmlMgr;
*/