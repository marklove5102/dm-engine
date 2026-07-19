#pragma once
#include "Global/StringUtil.h"

struct JumpPoint
{
	JumpPoint()
	{
		bNpc = false;
		wSrcX = 0;
		wSrcY = 0;
		wDesX = 0;
		wDesY = 0;
		dwSearchFlag = 0;
		pParent = NULL;
		bHaveSendNpcCom = false;
	}

	bool bNpc;//是否npc跨地图寻路
	bool bHaveSendNpcCom;//这个结点是否已经发送过npc跳转命令,搜索的时候会初始成false,发完以后会置true,不然有可能会多次发送
	string strSrcMN;//源地图名
	WORD wSrcX;//源坐标点x
	WORD wSrcY;//源坐标点y
	
	string strDesMN;//目标地图名
	WORD wDesX;//目标坐标x
	WORD wDesY;//目标坐标y
	VString vNpcCom;//从源地图到目标地图的npc命令
	DWORD  dwSearchFlag;//访问过的标记,每次搜索以后加一,用来标记某个结点这一次寻跳是否访问过
	JumpPoint *pParent;//用来反向找到全路径
};

typedef std::vector<JumpPoint> VJumpPoint;
typedef std::vector<JumpPoint*> VPJumpPoint;
typedef std::map<string,VJumpPoint> MapJumpInfo;

class CMapFinder
{
public:
	CMapFinder();
	~CMapFinder();
public:
	void StopSearch();
	bool AutoLookforPath(const char* pszSrcName,WORD wSrcX,WORD wSrcY,const char* pszDesName,WORD wDesX,WORD wDesY,bool bNpc = false);
	bool DoNextStep();
	VPJumpPoint& GetPath(){return m_VPath;}
	bool IsWalking(){return m_bWalking;}
	void SetWalking(bool b);
	void LoadMapJumpXML();

	MapJumpInfo &GetMapJumInfo(){return m_mapJumpInfo;}

private:
	bool FindPath(const char* pszSrcName);
private:
	MapJumpInfo	m_mapJumpInfo;
	VPJumpPoint m_VPath;//保存最终路径
	VPJumpPoint m_VQueue;//搜索时的临时队列
	WORD	   m_wDesX;
	WORD	   m_wDesY;
	string	   m_strDesMap;
	bool	   m_bNpc;
	bool	 m_bWalking;
	bool     m_bHaveFindPath;//是否已经找到路径的标记
	DWORD    m_dwSearchFlag;//访问过的标记,每次搜索以后加一,用来标记某个结点这一次寻跳是否访问过
	DWORD    m_dwVisistedNodeCount;//已经访问过的结点个数

	void EnQueue(JumpPoint* pNode,JumpPoint* pParent);//加入已经搜索队列
};

extern CMapFinder g_MapFinder;