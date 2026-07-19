#pragma once

#include "Global/Global.h"
#include "Global/StdHeaders.h"

struct WorldmapInfo_s
{
	WorldmapInfo_s()
	{
		strMapFile.clear();
		strMapName.clear();
	}
	string strMapFile;
	string strMapName;
};

struct NpcInfo_s
{
	NpcInfo_s()
	{
		strNpcName.clear();
		iX = iY = -1;
		iExNpcType = 0;
	}
	string strNpcName;
	int  iX;
	int  iY;
	string strDesc; //描述
	int iExNpcType;//额外NPC类型,1:行会塔,2:飞升台-神,3:飞升台-魔,4动态场景
};

struct MonsterInfo_s
{
	MonsterInfo_s()
	{
		strName.clear();
		strLevel.clear();
		bYouHuo = false;
	}

	string strName;//名字
	string strLevel;//等级
	bool  bYouHuo;//能否诱惑
};

struct JumplumpInfo_s
{
	JumplumpInfo_s()
	{
		strDestMapFile.clear();
		szTipsName.clear();
		szShowDestName.clear();
		iSrcX = iSrcY = -1;
		iDestX = iSrcY = -1;
		iType = 0;
		bShowBtn = false;
		bShowJump = true;
	}
	string strDestMapFile,szTipsName;
	int  iSrcX,iSrcY;
	int  iDestX,iDestY;
	int  iType;//类型,0:普通,1:跳转到地表地图
	bool bShowBtn;//是否显按钮
	bool bShowJump;//是否显示跳转点
	string szShowDestName;//显示用的跳转目标,有的时候按钮上显示的和要去的地方不一样,比如走到NPC前显示去另一张地图
};

struct LocalmapInfo_s
{
	LocalmapInfo_s()
	{
		strMapFile.clear();
		strMapName.clear();
		wMinMapID = 0;
		iWidth = 0;
		iHeight = 0;
	}
	string strMapFile;
	string strMapName;
	WORD wMinMapID;
	int  iWidth;
	int  iHeight;
	vector<NpcInfo_s> VNpc;
	vector<JumplumpInfo_s> VJump;
};

//保存小地图跳转信息等的类
class CMinMap
{
public:
	typedef std::map<std::string, LocalmapInfo_s*> MLocalmapInfo_s;
	typedef std::vector<WorldmapInfo_s*> VWorldmapInfo_s;
	typedef std::map<std::string,vector<NpcInfo_s>> MExLocalNpcInfo;//额外的NPC数据
	typedef std::map<std::string, vector<MonsterInfo_s>> MLocalMonsterInfo_s;

public:
	CMinMap();
	~CMinMap();

	bool Load();
	void Clear();

	VWorldmapInfo_s& GetWorldMap(){ return m_WorldMap; }
	LocalmapInfo_s*  GetLocalMap(const char* szMapFile);
	vector<MonsterInfo_s>* GetLocalMonster(const char* szMapFile);

	WORD GetMinMapID(const char* szMapFile);
	const char* MapFile2Name(const char* szMapFile);
	MExLocalNpcInfo& GetExLocalNpc(){ return m_ExLocalNpc;}
	//显示突出显示某个特定点
	bool  GetFindPoint(int& x,int& y)
	{
		x = m_iFindPointX;
		y = m_iFindPointY;
		return m_bIsFindPoint;
	}
	void  SetFindPoint(int x,int y)
	{
		m_iFindPointX = x;
		m_iFindPointY = y;
		m_bIsFindPoint = true;
	}
	void  ClearFindPoint(){	m_bIsFindPoint = false;	}

private:
	VWorldmapInfo_s    m_WorldMap;
	MLocalmapInfo_s    m_LocalMap;
	MExLocalNpcInfo	   m_ExLocalNpc;
	MLocalMonsterInfo_s m_LocalMonster;

	bool  m_bIsFindPoint;
	int   m_iFindPointX,m_iFindPointY;
};

extern CMinMap* g_pMinMap;