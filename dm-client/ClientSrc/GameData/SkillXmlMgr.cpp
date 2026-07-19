/*#include "SkillXmlMgr.h"
#include "BaseClass\TiXml\tinyxml.h"

CSkillXmlMgr g_SkillXmlMgr;

CSkillXmlMgr::CSkillXmlMgr(void)
{
	m_Skills.clear();

	char path[256];
	sprintf(path,"%s\\config\\SkillHelp.xml",GetGameDataDir());
	g_SkillXmlMgr.LoadFile(path);
}

CSkillXmlMgr::~CSkillXmlMgr(void)
{
	m_Skills.clear();
}

bool CSkillXmlMgr::LoadFile(const char* path)
{
	m_Skills.clear();
	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(path))
		return false;

	TiXmlElement* root = xmlDoc.RootElement();
	if (!root)
		return false;

	TiXmlElement* node = root->FirstChildElement();

	for(;node;node = node->NextSiblingElement())
	{
		if(stricmp(node->Value(),"Skill") == 0)
		{
			string name;

			for(TiXmlAttribute* attr = node->FirstAttribute();attr;attr = attr->Next())
			{
				if(stricmp(attr->Name(),"name") == 0)
					name = attr->Value();
			}

			VString vTips;
			TiXmlNode* child = 0;
			while(child = node->IterateChildren(child))
			{
				if(child->Type() == TiXmlNode::TEXT)
				{
					vTips = StringUtil::split(child->ToText()->Value(),"\n");
				}
			}
			m_Skills[name] = vTips;
		}
	}



	//以下代码把原来的tips转换成新的格式
	//string str;
	//FILE *fp = fopen("c:\\skillhelp.xml","wb");
	//fwrite("<?xml version=\"1.0\" encoding=\"gb2312\" standalone=\"yes\" ?>\r\n",strlen("<?xml version=\"1.0\" encoding=\"gb2312\" standalone=\"yes\" ?>\r\n"),1,fp);
	//fwrite("<SkillHelper Ver=\"1\">\r\n",strlen("<SkillHelper Ver=\"1\">\r\n"),1,fp);
	//
	//
	//for (MSkill::iterator itr = m_Skills.begin();itr != m_Skills.end();itr++)
	//{
	//	str = "    <Skill name = \"";
	//	str += itr->first + "\">\r\n";
	//	fwrite(str.c_str(),str.length(),1,fp);
	//
	//	VString &strAddTips = itr->second;
	//	for (int i = 0; i < strAddTips.size(); i++)
	//	{
	//		str = "        <line>\r\n";
	//		fwrite(str.c_str(),str.length(),1,fp);
	//		str = "            <col color = \"0xFFFFFFFF\" content = \"";
	//		str += strAddTips[i] + "\"/>\r\n";
	//		fwrite(str.c_str(),str.length(),1,fp);
	//		str = "        </line>\r\n";
	//		fwrite(str.c_str(),str.length(),1,fp);
	//	}
	//
	//	str ="    </Skill>\r\n";
	//	fwrite(str.c_str(),str.length(),1,fp);
	//}
	//
	//str ="</SkillHelper>\r\n";
	//fwrite(str.c_str(),str.length(),1,fp);
	//fclose(fp);

	return true;
}

VString* CSkillXmlMgr::GetSkillDesc(const char* name)
{
	MSkill::iterator itr =  m_Skills.find(name);

	if (itr != m_Skills.end())
		return &(itr->second);

	return NULL;
}
*/