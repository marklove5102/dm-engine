#include "XmlCfg.h"
#include "Global/StringUtil.h"
#include "BaseClass/TiXml/tinyxml.h"

CXmlCfg g_XmlCfg;

CXmlCfg::CXmlCfg(void)
{

}

CXmlCfg::~CXmlCfg(void)
{

}

void CXmlCfg::ReloadXmgCfg()
{
	ReLoadWenPeiCfg();
	ReLoadVActivityRateCfg();
}

void CXmlCfg::Clear()
{
	for ( int i = 0; i < 4; ++i)
	{
		for (VWenPeiTask::iterator itr = m_kWenPeiTasks[i].begin(); itr != m_kWenPeiTasks[i].end(); ++itr)
		{
			itr->clear();
		}
	}
}

bool CXmlCfg::ReLoadWenPeiCfg()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\WenPeiCfg.xml",GetGameDataDir());

	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(path)) //¶ÁÈ¡Ê§°Ü
		return false;

	m_kWenPeiCfgs.clear();

	for ( int i = 0; i < 4; ++i)
	{
		m_kWenPeiTasks[i].clear();
	}

	TiXmlElement* rootNode = xmlDoc.RootElement();
	TiXmlElement* node = rootNode->FirstChildElement();


	for(;node;node = node->NextSiblingElement())
	{
		if(stricmp(node->Value(),"WenPeis") == 0)
		{
			TiXmlElement* child = node->FirstChildElement();
			for(;child;child = child->NextSiblingElement())
			{
				if(stricmp(child->Value(),"WenPei") == 0)
				{
					sWenPeiInfo info;
					sWenPeiTask wptask;

					bool task = false;

					for(TiXmlAttribute* attr = child->FirstAttribute();attr;attr = attr->Next())
					{
						if(stricmp(attr->Name(),"looks") == 0)
						{
							info.looks = atoi(attr->Value());
						}
						else if(stricmp(attr->Name(),"name") == 0)
						{
							info.name = attr->Value();
						}
						else if(stricmp(attr->Name(),"type") == 0)
						{
							info.strType = attr->Value();
						}
						else if(stricmp(attr->Name(),"value") == 0)
						{
							info.strValue = attr->Value();
						}
						else if(stricmp(attr->Name(),"shengwang") == 0)
						{
							info.shengwang = atoi(attr->Value());
						}
						else if(stricmp(attr->Name(),"rate1") == 0)
						{
							info.rate[0] = atoi(attr->Value());
						}
						else if(stricmp(attr->Name(),"rate2") == 0)
						{
							info.rate[1] = atoi(attr->Value());
						}
						else if(stricmp(attr->Name(),"rate3") == 0)
						{
							info.rate[2] = atoi(attr->Value());
						}
						else if(stricmp(attr->Name(),"task") == 0)
						{
							if (stricmp(attr->Value(), "true") == 0)
							{
								task = true;
							}
						}
						else if(stricmp(attr->Name(),"content") == 0)
						{
							wptask.content[0] = attr->Value();
						}
						else if(stricmp(attr->Name(),"content2") == 0)
						{
							wptask.content[1] = attr->Value();
						}
					}

					if (task)
					{
						wptask.looks = info.looks;
						wptask.name = info.name;						

						if (info.strType == "±ßÎÆ")
						{
							m_kWenPeiTasks[0].push_back(wptask);
						}
						else if (info.strType == "µ×ÎÆ")
						{
							m_kWenPeiTasks[1].push_back(wptask);
						}
						else if (info.strType == "Ö÷ÊÎÎÆ")
						{
							m_kWenPeiTasks[2].push_back(wptask);
						}
						else if (info.strType == "¸¨ÊÎÎÆ")
						{
							m_kWenPeiTasks[3].push_back(wptask);
						}
					}

					m_kWenPeiCfgs[info.looks] = info;
					
				}
			}
		}		
	}

	return true;
}

sWenPeiInfo* CXmlCfg::GetWenPeiCfg(int looks)
{
	if (looks <= 0)
		return NULL;

	MWenPeiCfg::iterator i = m_kWenPeiCfgs.find(looks);
	if (i != m_kWenPeiCfgs.end())
	{
		return &i->second;
	}
	return NULL;
}

VWenPeiTask& CXmlCfg::GetWenPeiTasks(int i) 
{
	if (i >= 0 && i < 4)
	{
		return m_kWenPeiTasks[i]; 
	}

	return m_kWenPeiTasks[0];	
}

sWenPeiTask* CXmlCfg::GetWenPeiTask(int looks)
{
	if (looks <= 0)
		return NULL;

	for ( int i = 0; i < 4; ++i)
	{
		for (VWenPeiTask::iterator it = m_kWenPeiTasks[i].begin(); it != m_kWenPeiTasks[i].end(); ++it)
		{
			if ((*it).looks == looks)
			{
				return &(*it);
			}
		}
	}
	
	return NULL;
}

bool CXmlCfg::ReLoadVActivityRateCfg()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\ActivityRateCfg.xml",GetGameDataDir());

	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(path)) //¶ÁÈ¡Ê§°Ü
		return false;

	m_VActivityRate.clear();

	TiXmlElement* rootNode = xmlDoc.RootElement();
	TiXmlElement* node = rootNode->FirstChildElement();

	for(;node;node = node->NextSiblingElement())
	{
		if(stricmp(node->Value(),"Tasks") == 0)
		{
			TiXmlElement* child = node->FirstChildElement();
			for(;child;child = child->NextSiblingElement())
			{
				if(stricmp(child->Value(),"Task") == 0)
				{
					S_ActivityRate actrate;

					for(TiXmlAttribute* attr = child->FirstAttribute();attr;attr = attr->Next())
					{
						if(stricmp(attr->Name(),"id") == 0)
						{
							actrate.dwID = atoi(attr->Value());
						}
						else if(stricmp(attr->Name(),"time") == 0)
						{
							actrate.strTime = attr->Value();
						}
						else if(stricmp(attr->Name(),"name") == 0)
						{
							actrate.strName = attr->Value();
						}
						else if(stricmp(attr->Name(),"npc") == 0)
						{
							actrate.strNPCName = attr->Value();
						}
						else if(stricmp(attr->Name(),"needlevel") == 0)
						{
							actrate.strNeedLevel = attr->Value();
						}
						else if(stricmp(attr->Name(),"award") == 0)
						{
							actrate.strAward = attr->Value();
						}
						else if(stricmp(attr->Name(),"command") == 0)
						{
							actrate.strNpcCommand = attr->Value();
						}
						else if(stricmp(attr->Name(),"tips") == 0)
						{
							actrate.strTips = attr->Value();
						}
					}

					m_VActivityRate.push_back(actrate);
					
				}
			}
		}		
	}

	return true;
}

