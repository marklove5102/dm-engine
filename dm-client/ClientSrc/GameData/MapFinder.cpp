#include "MapFinder.h"
#include "Global/Global.h"
#include "BaseClass/TiXml/tinyxml.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"

CMapFinder g_MapFinder;


CMapFinder::CMapFinder()
{
	LoadMapJumpXML();
	m_bHaveFindPath = false;
}

CMapFinder::~CMapFinder()
{
	StopSearch();
}

void CMapFinder::LoadMapJumpXML()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\MapJump.xml",GetGameDataDir());

	m_mapJumpInfo.clear();
	m_VPath.clear();
	m_VQueue.clear();

	TiXmlDocument* pTiXmlDoc = new TiXmlDocument(path);
	pTiXmlDoc->LoadFile();

	TiXmlElement* pRoot = pTiXmlDoc->RootElement();

	if (pRoot != NULL)
	{
		TiXmlElement* pMap = pRoot->FirstChildElement();
		while (pMap != NULL)
		{//Map节点

			string strMapId = pMap->Attribute("id");
			StringUtil::toLowerCase(strMapId);

			TiXmlElement* pJump = pMap->FirstChildElement();
			while(pJump != NULL)
			{//Jump节点

				int srcx = atoi(pJump->Attribute("srcx"));
				int srcy = atoi(pJump->Attribute("srcy"));
				int type = atoi(pJump->Attribute("type"));

				TiXmlElement* pPoint = pJump->FirstChildElement();
				while (pPoint != NULL)
				{//Point节点

					string dstid = pPoint->Attribute("dstid");
					StringUtil::toLowerCase(dstid);
					int dstx          = atoi(pPoint->Attribute("dstx"));
					int dsty          = atoi(pPoint->Attribute("dsty"));

					JumpPoint jp;
					jp.wSrcX = srcx;
					jp.wSrcY = srcy;
					jp.wDesX = dstx;
					jp.wDesY = dsty;
					jp.strDesMN = dstid;
					jp.strSrcMN = strMapId;
					jp.bNpc  = (type == 1);

					if (type == 1)
					{
						TiXmlElement* pCommand = pPoint->FirstChildElement();
						while(pCommand != NULL)
						{
							string strCommand = pCommand->Attribute("Com");
							jp.vNpcCom.push_back(strCommand);
							pCommand = pCommand->NextSiblingElement();
						}
					}

					m_mapJumpInfo[strMapId].push_back(jp);

					pPoint = pPoint->NextSiblingElement();
				}

				pJump = pJump->NextSiblingElement();
			}

			pMap = pMap->NextSiblingElement();
		}
	}

	delete pTiXmlDoc;

}

bool CMapFinder::DoNextStep()
{
	VPJumpPoint::iterator it = m_VPath.begin();

	if (it != m_VPath.end())
	{
		g_pGameControl->DealGotoCommand(*it);
	}
	else
	{
		SetWalking(false);
	}

	return true;
}

void CMapFinder::SetWalking(bool b)
{
	m_bWalking = b;
	if (!m_bWalking)
	{
		g_OtherData.SetFindPathReason(0);
	}
}

void CMapFinder::StopSearch()
{
	m_bHaveFindPath = false;
	m_dwVisistedNodeCount = 0;
	m_VPath.clear();
	m_VQueue.clear();

	m_wDesX = 0;
	m_wDesY = 0;
	m_strDesMap = "";
	m_bNpc = false;
	m_bWalking = false;
}

bool CMapFinder::AutoLookforPath(const char* pszSrcName,WORD wSrcX,WORD wSrcY,const char* pszDesName,WORD wDesX,WORD wDesY,bool bNpc)
{
	if (!pszDesName || !pszSrcName) 
		return false;

	StopSearch();

	m_bNpc = bNpc;
	m_wDesX = wDesX;
	m_wDesY = wDesY;

	m_strDesMap = pszDesName;
	string str = pszSrcName;

	StringUtil::toLowerCase(str);
	StringUtil::toLowerCase(m_strDesMap);

	if (str == m_strDesMap)
	{
		JumpPoint jp;
		jp.strSrcMN = str;
		jp.strDesMN = pszDesName;
		jp.bNpc = bNpc;
		jp.wSrcX = wDesX;
		jp.wSrcY = wDesY;
		g_pGameControl->DealGotoCommand(&jp);
		return true;
	}

	if(!FindPath(str.c_str()))
		return false;

	SetWalking(m_VPath.size() > 0);
	if (m_bWalking)
	{
		DoNextStep();
	}
	return m_bWalking;
}

//加入已经搜索队列
void CMapFinder::EnQueue(JumpPoint* pNode,JumpPoint* pParent)
{
	if (m_bHaveFindPath || !pNode || pNode->dwSearchFlag == m_dwSearchFlag)//pNode->dwSearchFlag == m_dwSearchFlag:这个结点这次寻路有访问过
	{
		return;
	}

	pNode->pParent = pParent;
	pNode->dwSearchFlag = m_dwSearchFlag;//表示这个结点这次寻路已经访问过了
	m_dwVisistedNodeCount ++;

	if (pNode->strDesMN.compare(m_strDesMap.c_str()) == 0)//找到目标
	{
		JumpPoint* pCurNode = pNode;
		static JumpPoint LastPoint;//最后跳到了目标地图后再自动寻路到目标地图指定坐标点

		LastPoint.bNpc = m_bNpc;
		LastPoint.strDesMN = m_strDesMap;
		LastPoint.strSrcMN = m_strDesMap;
		LastPoint.wSrcX = m_wDesX;
		LastPoint.wSrcY = m_wDesY;

		m_VPath.push_back(&LastPoint);
		while(pCurNode)   
		{
			pCurNode->bHaveSendNpcCom = false;
		    m_VPath.insert(m_VPath.begin(),pCurNode);
			pCurNode = pCurNode->pParent;
		}

		m_bHaveFindPath = true; 
		return;
	}

	m_VQueue.push_back(pNode);
}

bool CMapFinder::FindPath(const char* pszSrcName)
{
	MapJumpInfo::iterator it = m_mapJumpInfo.find(pszSrcName);
	if(it == m_mapJumpInfo.end())//这张地图没有任何出口
		return false;

	JumpPoint* pCurNode = NULL;
	m_bHaveFindPath = false;
	m_dwVisistedNodeCount = 0;
	m_VPath.clear();
	m_VQueue.clear();
	
	m_dwSearchFlag ++;
	//先把这张地图的所有出口加入队列
	VJumpPoint &vJumPonit = it->second;
	for (size_t i = 0; i < vJumPonit.size(); i++)
	{
		EnQueue(&(vJumPonit[i]),NULL);
	}	

	while(!m_VQueue.empty() && !m_bHaveFindPath && m_dwVisistedNodeCount < 1000)//最大访问1000个结点
	{
		pCurNode = m_VQueue.front();
		m_VQueue.erase(m_VQueue.begin());

		//遍历pCurNode的所有出口
		MapJumpInfo::iterator itSub = m_mapJumpInfo.find(pCurNode->strDesMN);
		if(itSub != m_mapJumpInfo.end())
		{
			VJumpPoint &vSubJumPonit = itSub->second;
			for (size_t i = 0; i < vSubJumPonit.size(); i++)
			{
				EnQueue(&(vSubJumPonit[i]),pCurNode);
			}
		}
	}

	if(m_bHaveFindPath)
	{ 
		return true;
	}

	return false;
}
