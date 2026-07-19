#include "MinMap.h"
#include "GameMap.h"
#include "Global/StringUtil.h"
#include "BaseClass/TiXml/tinyxml.h"
#include "MapXmlMgr.h"

//小地图信息
CMinMap* g_pMinMap = NULL;

CMinMap::CMinMap()
{
	m_LocalMap.clear();
	m_WorldMap.clear();
	m_bIsFindPoint = false;
	m_iFindPointX = m_iFindPointY = 0;

	Load();
}

CMinMap::~CMinMap()
{
	Clear();
}

void CMinMap::Clear()
{
	//世界地图
	while(!m_WorldMap.empty())
	{
		WorldmapInfo_s* inf = m_WorldMap.back();
		SAFE_DELETE(inf);
		m_WorldMap.pop_back();
	}

	//本地地图
	while(!m_LocalMap.empty())
	{
		MLocalmapInfo_s::iterator it = m_LocalMap.begin();
		LocalmapInfo_s* pMap = it->second;
		if (pMap)
		{
			pMap->VNpc.clear();
			pMap->VJump.clear();
			SAFE_DELETE(pMap);
		}
		m_LocalMap.erase(it);
	}
}

LocalmapInfo_s* CMinMap::GetLocalMap(const char* szMapFile)
{
	if (NULL == szMapFile || strlen(szMapFile) == 0)
		return NULL;

	string strMapFile = szMapFile;
	StringUtil::toUpperCase(strMapFile);

	MLocalmapInfo_s::iterator it = m_LocalMap.find(strMapFile);
	if(it == m_LocalMap.end())
	{
		strMapFile = g_MapXmlMgr.GetLocalMap(szMapFile); //如果配置了MinMap,直接获得对应的MinMap
		if(strMapFile.empty())
			strMapFile = g_MapXmlMgr.GetRealMap(szMapFile); //转化成存储

		StringUtil::toUpperCase(strMapFile);
		it = m_LocalMap.find(strMapFile);
		if(it == m_LocalMap.end())
		{
			return NULL;
		}
	}

	LocalmapInfo_s* pMap = it->second;
	if(NULL == pMap)
		return NULL;

	//if (0 == pMap->strMapFile.compare(szMapFile))
		return pMap;

	//return NULL;
}

vector<MonsterInfo_s>* CMinMap::GetLocalMonster(const char* szMapFile)
{
	if (NULL == szMapFile || strlen(szMapFile) == 0)
		return NULL;

	string strMapFile = szMapFile;
	StringUtil::toUpperCase(strMapFile);

	MLocalMonsterInfo_s::iterator it = m_LocalMonster.find(strMapFile);
	if(it == m_LocalMonster.end())
	{
		strMapFile = g_MapXmlMgr.GetLocalMap(szMapFile); //如果配置了MinMap,直接获得对应的MinMap
		if(strMapFile.empty())
			strMapFile = g_MapXmlMgr.GetRealMap(szMapFile); //转化成存储

		StringUtil::toUpperCase(strMapFile);
		it = m_LocalMonster.find(strMapFile);
		if(it == m_LocalMonster.end())
		{
			return NULL;
		}
	}
	
	return &(it->second);
}

WORD CMinMap::GetMinMapID(const char* szMapFile)
{
	if (NULL == szMapFile || strlen(szMapFile) == 0)
		return 0;

	LocalmapInfo_s* pMap = GetLocalMap(szMapFile);
	if (NULL == pMap)
		return 0;

	return pMap->wMinMapID;
}

bool CMinMap::Load()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\minmaplist.xml",GetGameDataDir());

	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(path)) //读取失败
		return false;

	Clear();

	TiXmlElement* rootNode = xmlDoc.RootElement();

	TiXmlElement* attrNode = rootNode->FirstChildElement();
	for(;attrNode;attrNode = attrNode->NextSiblingElement())
	{
		if(stricmp(attrNode->Value(),"WorldMap") == 0) //世界地图
		{
			TiXmlElement* node = attrNode->FirstChildElement();
			for(;node;node = node->NextSiblingElement())
			{
				string szMapFile,szMapName;

				for(TiXmlAttribute* attr = node->FirstAttribute();attr;attr = attr->Next())
				{
					if(stricmp(attr->Name(),"id") == 0)
						szMapFile = attr->Value();
					else if(stricmp(attr->Name(),"name") == 0)
						szMapName = attr->Value();
				}

				WorldmapInfo_s* pMap = new WorldmapInfo_s();
				pMap->strMapFile = szMapFile;
				pMap->strMapName = szMapName;
				m_WorldMap.push_back(pMap);
			}
		}
		else if(stricmp(attrNode->Value(),"LocalMap") == 0) //局部地图
		{
			TiXmlElement* node = attrNode->FirstChildElement();
			for(;node;node = node->NextSiblingElement())
			{
				string szMapFile,szMapName;
				int _width = 0,_height = 0;
				int _minmapid = 0;

				for(TiXmlAttribute* attr = node->FirstAttribute();attr;attr = attr->Next())
				{
					if(stricmp(attr->Name(),"id") == 0)
						szMapFile = attr->Value();
					else if(stricmp(attr->Name(),"name") == 0)
						szMapName = attr->Value();
					else if(stricmp(attr->Name(),"minmapid") == 0)
						_minmapid = attr->IntValue();
					else if(stricmp(attr->Name(),"width") == 0)
						_width = attr->IntValue();
					else if(stricmp(attr->Name(),"height") == 0)
						_height = attr->IntValue();
				}

				StringUtil::toUpperCase(szMapFile);
				MLocalmapInfo_s::iterator  it = m_LocalMap.find(szMapFile);
				if(it != m_LocalMap.end()) //还没有插入继续
					continue;

				LocalmapInfo_s* pMap = new LocalmapInfo_s();
				pMap->strMapFile = szMapFile;
				pMap->strMapName = szMapName;
				pMap->wMinMapID = _minmapid;
				pMap->iWidth = _width;
				pMap->iHeight = _height;
				m_LocalMap[szMapFile] = pMap;

				TiXmlElement* child = node->FirstChildElement();
				for(;child;child = child->NextSiblingElement())
				{
					if(stricmp(child->Value(),"Npc") == 0)
					{
						NpcInfo_s npc;

						for(TiXmlAttribute* attr = child->FirstAttribute();attr;attr = attr->Next())
						{
							if(stricmp(attr->Name(),"name") == 0)
								npc.strNpcName = attr->Value();
							else if(stricmp(attr->Name(),"x") == 0)
								npc.iX = attr->IntValue();
							else if(stricmp(attr->Name(),"y") == 0)
								npc.iY = attr->IntValue();
							else if(stricmp(attr->Name(),"desc") == 0)
								npc.strDesc = attr->Value();
						}
						pMap->VNpc.push_back(npc);
					}
					else if(stricmp(child->Value(),"Jump") == 0)
					{
						JumplumpInfo_s jump;

						for(TiXmlAttribute* attr = child->FirstAttribute();attr;attr = attr->Next())
						{
							if(stricmp(attr->Name(),"dstid") == 0)
								jump.strDestMapFile = attr->Value();
							else if(stricmp(attr->Name(),"srcx") == 0)
								jump.iSrcX = attr->IntValue();
							else if(stricmp(attr->Name(),"srcy") == 0)
								jump.iSrcY = attr->IntValue();
							else if(stricmp(attr->Name(),"dstx") == 0)
								jump.iDestX = attr->IntValue();
							else if(stricmp(attr->Name(),"dsty") == 0)
								jump.iDestY = attr->IntValue();
							else if(stricmp(attr->Name(),"type") == 0)
								jump.iType = attr->IntValue();
							else if(stricmp(attr->Name(),"showbutton") == 0)
								jump.bShowBtn = attr->BoolValue();
							else if(stricmp(attr->Name(),"showjump") == 0)
								jump.bShowJump = attr->BoolValue();
							else if(stricmp(attr->Name(),"tips") == 0)
								jump.szTipsName = attr->Value();
							else if(stricmp(attr->Name(),"showdest") == 0)
								jump.szShowDestName = attr->Value();
						}
						pMap->VJump.push_back(jump);
					}
				}
			}
		}
	}

	sprintf(path,"%s\\config\\mapmonster.xml",GetGameDataDir());

	if(!xmlDoc.LoadFile(path)) //读取失败
		return false;

	m_LocalMonster.clear();
	rootNode = xmlDoc.RootElement();

	attrNode = rootNode->FirstChildElement();
	for(;attrNode;attrNode = attrNode->NextSiblingElement())
	{
		if(stricmp(attrNode->Value(),"Map") == 0) //局部地图
		{
			string mapName = attrNode->FirstAttribute()->Value();

			vector<MonsterInfo_s> vecMonster;

			TiXmlElement* node = attrNode->FirstChildElement();		

			for(;node;node = node->NextSiblingElement())
			{	
				MonsterInfo_s monster;

				for(TiXmlAttribute* attr = node->FirstAttribute();attr;attr = attr->Next())
				{	
					if(stricmp(attr->Name(),"name") == 0)
						monster.strName = attr->Value();
					else if(stricmp(attr->Name(),"level") == 0)
						monster.strLevel = attr->Value();
					else if(stricmp(attr->Name(),"youhuo") == 0)
						monster.bYouHuo = (attr->IntValue() == 1 ? true : false);						
				}				
				
				vecMonster.push_back(monster);	
			}					
			
			StringUtil::toUpperCase(mapName);
			MLocalMonsterInfo_s::iterator  it = m_LocalMonster.find(mapName);
			if(it != m_LocalMonster.end()) //还没有插入继续
				continue;
							
			m_LocalMonster[mapName] = vecMonster;				
		}
	}
	return true;
}

const char* CMinMap::MapFile2Name(const char* szMapFile)
{
	if (NULL == szMapFile || strlen(szMapFile) == 0)
		return "";

	LocalmapInfo_s* pMap = GetLocalMap(szMapFile);
	if (NULL == pMap)
		return "";

	return pMap->strMapName.c_str();
}

