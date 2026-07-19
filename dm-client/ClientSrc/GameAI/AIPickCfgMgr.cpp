#include "AIPickCfgMgr.h"
#include "AIConfigMgr.h"
#include "AIAutoPickMgr.h"
#include "Global/DebugTry.h"

CAIPickCfgMgr g_PickCfgMgr;

CAIPickCfgMgr::CAIPickCfgMgr(void)
{
	m_mapClass.clear();
	m_vecClass.clear();
}

CAIPickCfgMgr::~CAIPickCfgMgr(void)
{
	Clear();
}

void CAIPickCfgMgr::Clear()
{
	while(!m_mapClass.empty())
	{
		MItemClass::iterator it = m_mapClass.begin();
		CItemClass* pItemClass = it->second;
		SAFE_DELETE(pItemClass);
		m_mapClass.erase(it);
	}
	m_vecClass.clear();
}

//导入文件
bool CAIPickCfgMgr::LoadConfig()
{
	char szPath[1024] = {0};
	sprintf(szPath,"%s\\config\\%s\\BestItem.ini",GetGameDataDir(),g_AICfgMgr.GetIDPath());
	m_strItemFile.assign(szPath);

	FILE* fp = fopen(m_strItemFile.c_str(),"rt");
	if(!fp)
		return false;

	Clear();

	char szLine[1024] = {0};

	while (!feof(fp))
	{
		memset(szLine,0,1024);
		if(NULL == fgets(szLine,1024,fp))
			break;

		char* szName = strtok(szLine,"=");
		if(!szName)
			continue;

		char* szParam = strtok(NULL,"\r\n");
		if(!szParam)
			continue;

		BOOL bBest,bExpensive,bPick,bShow;
		sscanf(szParam,"%d,%d,%d,%d",&bBest,&bExpensive,&bPick,&bShow);

		//如果已经记录进去了
		string strName = szName;
		if(strName.empty())
			continue;

		MItemClass::iterator it = m_mapClass.find(strName);
		if(it != m_mapClass.end())
			continue;

		CItemClass* pItemClass = NULL;
		TRY_BEGIN
			pItemClass = new CItemClass();
		TRY_END_DO_JUMP(
			pItemClass = NULL;
		    SaveException(__FILE__,__FUNCTION__,__LINE__,pSec,strAddMsg.c_str());
		    break;
	    )

		if(pItemClass)
		{
			pItemClass->strName = strName;
			pItemClass->bBest = bBest?true:false;
			pItemClass->bExpensive = bExpensive?true:false;
			pItemClass->bPick = bPick?true:false;
			pItemClass->bShow = bShow?true:false;
			m_mapClass[strName]=pItemClass;
			m_vecClass.push_back(pItemClass);
		}
	}
	fclose(fp);
	return true;
}

////////////////////////////////////////////////////////////////////////
//根据名字获得物品颜色
bool CAIPickCfgMgr::GetItemColor(const char* name,DWORD& color)
{
	DWORD dwType = GetShowType(name);
	return GetItemColor(dwType,color);
}

bool CAIPickCfgMgr::GetItemColor(DWORD dwType,DWORD& color)
{
	if(dwType & ITEMSHOWTYPE_BEST)
	{
		color = g_AICfgMgr.GetBestItemColor();;
		return true;
	}
	else if(dwType & ITEMSHOWTYPE_EXPENSIVE)
	{
		color = g_AICfgMgr.GetExpensiveItemColor();
		return true;
	}
	else if(dwType & ITEMSHOWTYPE_SHOW)
	{
		if(dwType & ITEMSHOWTYPE_PICK)
			color = g_AICfgMgr.GetPickItemColor();
		else
			color = g_AICfgMgr.GetShowItemColor();
		return true;
	}
	else
	{
		color = 0xFFFFFFFF;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
//是否需要经过就捡的物品
bool CAIPickCfgMgr::IsAutoPick(const char* name)
{
	DWORD dwType = GetShowType(name);
	return IsAutoPick(dwType);
}

bool CAIPickCfgMgr::IsAutoPick(DWORD dwType)
{
	if((dwType & ITEMSHOWTYPE_BEST)||
		(dwType & ITEMSHOWTYPE_EXPENSIVE)||
		(dwType & ITEMSHOWTYPE_PICK))
		return true;
	else
		return false;
	//未知物品都是要能自动捡的，因为可能是新加的物品
	//垃圾物品也要添加到列表中，并设置不自动捡
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
//是否需要飞去捡的物品
bool CAIPickCfgMgr::IsGoPick(const char* name)   
{
	DWORD dwType = GetShowType(name);
	return IsGoPick(dwType);
}

bool CAIPickCfgMgr::IsGoPick(DWORD dwType)
{
	if(dwType & ITEMSHOWTYPE_BEST)
		return true;
	else
		return false;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
bool CAIPickCfgMgr::IsBest(const char* name)     //是否极品
{
	DWORD dwType = GetShowType(name);
	return IsBest(dwType);
}

bool CAIPickCfgMgr::IsBest(DWORD dwType)
{
	if(dwType & ITEMSHOWTYPE_BEST)
		return true;
	else
		return false;
	return false;
}

bool CAIPickCfgMgr::IsPetPick(const char* name)
{
	DWORD dwType = GetShowType(name);
	return IsPetPick(dwType);
}

bool CAIPickCfgMgr::IsPetPick(DWORD dwType)
{
	if((dwType & ITEMSHOWTYPE_BEST) || (dwType & ITEMSHOWTYPE_EXPENSIVE))
		return true;
	else
		return false;

	return false;
}

DWORD  CAIPickCfgMgr::GetShowType(const char* name)     //返回要显示的类型
{
	MItemClass::iterator it = m_mapClass.find(name);
	if(it == m_mapClass.end())
		return ITEMSHOWTYPE_SHOW|ITEMSHOWTYPE_PICK|ITEMSHOWTYPE_NOFOUND;

	CItemClass* pItemClass = it->second;
	if(NULL == pItemClass)
		return ITEMSHOWTYPE_SHOW|ITEMSHOWTYPE_PICK;

	if(pItemClass->strName.compare(name)!=0)
		return ITEMSHOWTYPE_SHOW|ITEMSHOWTYPE_PICK;

	DWORD dwType = ITEMSHOWTYPE_HIDE;

	if(pItemClass->bBest)
		dwType|=ITEMSHOWTYPE_BEST;

	if(pItemClass->bExpensive)
		dwType|=ITEMSHOWTYPE_EXPENSIVE;

	if(pItemClass->bPick)
		dwType|=ITEMSHOWTYPE_PICK;

	if(pItemClass->bShow)
		dwType|=ITEMSHOWTYPE_SHOW;

	return dwType;
}

CItemClass* CAIPickCfgMgr::GetItemClass(int i) const
{
	if(i < 0 || i >= (int)m_vecClass.size())
		return NULL;

	return m_vecClass[i];
}

void CAIPickCfgMgr::FlipBest(int i)
{
	CItemClass* pClass = GetItemClass(i);
	if(pClass != NULL)
	{
		pClass->bBest = !pClass->bBest;
		WriteConfigFile();
	}
}

void CAIPickCfgMgr::FlipExpensive(int i)
{
	CItemClass* pClass = GetItemClass(i);
	if(pClass != NULL)
	{
		pClass->bExpensive = !pClass->bExpensive;
		WriteConfigFile();
	}
}

void CAIPickCfgMgr::FlipPick(int i)
{
	CItemClass* pClass = GetItemClass(i);
	if(pClass != NULL)
	{
		pClass->bPick = !pClass->bPick;
		WriteConfigFile();
	}
}

void CAIPickCfgMgr::FlipShow(int i)
{
	CItemClass* pClass = GetItemClass(i);
	if(pClass != NULL)
	{
		pClass->bShow = !pClass->bShow;
		WriteConfigFile();
	}
}

int CAIPickCfgMgr::InsertItemClass(const char* strName,int pos)
{
	if(strName == NULL || strlen(strName) == 0)
		return -1;

	MItemClass::iterator it = m_mapClass.find(strName);
	if(it != m_mapClass.end())
	{
		for(int i = 0;i < (int)m_vecClass.size();i++)
		{
			if(m_vecClass[i]->strName.compare(strName) == 0)
				return i;
		}
		return -1;
	}

	CItemClass* pClass = new CItemClass();
	pClass->strName = strName;
	pClass->bBest = true;
	pClass->bExpensive = true;
	pClass->bPick = true;
	pClass->bShow = true;
	m_mapClass[pClass->strName] = pClass;

	m_vecClass.push_back(pClass);

	WriteConfigFile();

	return ((int)m_vecClass.size() - 1);
}

int CAIPickCfgMgr::RemoveItemClass(int pos)
{
	if(pos < 0 || pos >= (int)m_vecClass.size())
		return -1;

	CItemClass* pClass  = m_vecClass[pos];
	if(NULL == pClass)
		return -1;

	const char* strName = pClass->strName.c_str();

	MItemClass::iterator it = m_mapClass.find(strName);
	if(it != m_mapClass.end())
		m_mapClass.erase(it);

	VItemClass::iterator iv = m_vecClass.begin() + pos;
	m_vecClass.erase(iv);

	SAFE_DELETE(pClass);

	WriteConfigFile();

	return -1;
}

void CAIPickCfgMgr::WriteConfigFile()
{
	g_AutoPickMgr.LoadBestItems();

	FILE* fp = fopen(m_strItemFile.c_str(),"wt");
	if(!fp)
		return;

	VItemClass::iterator it;
	for(it = m_vecClass.begin();it != m_vecClass.end();it++)
	{
		CItemClass* pClass = *it;
		if(pClass == NULL)
			continue;

		fprintf(fp,"%s=%d,%d,%d,%d\n",pClass->strName.c_str(),pClass->bBest,
			pClass->bExpensive,pClass->bPick,pClass->bShow);
	}
	fclose(fp);
}