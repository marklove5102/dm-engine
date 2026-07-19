#include "maparray.h"
#include "GameMap/GameMap.h"
#include "GameData/GameData.h"


#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif
CMapArray::CMapArray(void)
{
}

CMapArray::~CMapArray(void)
{
	for(int i = 0 ; i < IDMAXNUMBER; i++)
	{
		while(m_arrID[i].m_IDnext != NULL)
		{
			CSimpleCharacterNode * p = m_arrID[i].m_IDnext;
			m_arrID[i].m_IDnext = m_arrID[i].m_IDnext->m_IDnext;
			delete p;
		}
		while(m_arrIDgood[i].m_IDnext != NULL)
		{
			CSimpleGoodNode * p = m_arrIDgood[i].m_IDnext;
			m_arrIDgood[i].m_IDnext = m_arrIDgood[i].m_IDnext->m_IDnext;
			delete p;
		}
	}
	{
		for(int i = 0; i < MAPARR_WIDTH; i++)
		{
			for(int j = 0; j < MAPARR_WIDTH; j++)
			{
				m_arrMap[i][j].m_good.m_IDnext = NULL;
				m_arrMap[i][j].m_character.m_IDnext = NULL;
			}
		}
	}
}

CSimpleGoodNode * CMapArray::AddGood(int x,int y,DWORD nID)
{
	CSimpleGoodNode * pCurr = FindSimpleGood(nID);
	if(pCurr != NULL)
		return pCurr;

	if(x < 0 || y < 0)
		return NULL;

	pCurr = AddSimpleGood(nID);
	hashXYfunc(x,y);

	if( pCurr == NULL)
	{
		return NULL;
	}

	pCurr->m_MapNext = m_arrMap[x][y].m_good.m_MapNext;
	if(m_arrMap[x][y].m_good.m_MapNext != NULL)
		m_arrMap[x][y].m_good.m_MapNext->m_MapPrev = pCurr;
	pCurr->m_MapPrev = &m_arrMap[x][y].m_good;
	m_arrMap[x][y].m_good.m_MapNext = pCurr;
	return pCurr;
}

CSimpleGoodNode * CMapArray::AddSimpleGood(DWORD nID)
{
	CSimpleGoodNode * pTemp1 = FindSimpleGood(nID);
	if(pTemp1 != NULL)
	{
		return pTemp1;
	}
	CSimpleGoodNode * pTemp = new CSimpleGoodNode();

	if(pTemp == NULL)
	{
		// 内存不足
		return NULL;
	}

	pTemp->SetID(nID);

	pTemp->m_IDnext = m_arrIDgood[hashIDfunc(nID)].m_IDnext;
	if(m_arrIDgood[hashIDfunc(nID)].m_IDnext != NULL)
	{
		m_arrIDgood[hashIDfunc(nID)].m_IDnext->m_IDprev = pTemp;
	}

	m_arrIDgood[hashIDfunc(nID)].m_IDnext = pTemp;
	pTemp->m_IDprev = &m_arrIDgood[hashIDfunc(nID)];

	m_MiniGood[nID] = pTemp; //Add by Yuan

	return pTemp;
}
CSimpleCharacterNode * CMapArray::AddSimpleCharacter(DWORD nID)
{
	CSimpleCharacterNode * pTemp1 = FindSimpleCharacter(nID);
	if(pTemp1 != NULL)
	{
		return pTemp1;
	}

	CSimpleCharacterNode * pTemp = new CSimpleCharacterNode();

	if(pTemp == NULL)
	{
		// 内存不足
		return NULL;
	}

	pTemp->SetID(nID);

	pTemp->m_IDnext = m_arrID[hashIDfunc(nID)].m_IDnext;
	if(m_arrID[hashIDfunc(nID)].m_IDnext != NULL)
	{
		m_arrID[hashIDfunc(nID)].m_IDnext->m_IDprev = pTemp;
	}
	m_arrID[hashIDfunc(nID)].m_IDnext = pTemp;
	pTemp->m_IDprev = &m_arrID[hashIDfunc(nID)];

	m_MiniChar[nID] = pTemp; // add by Yuan

	return pTemp;
}

CSimpleCharacterNode * CMapArray::AddCharacter(int x,int y,DWORD nID)
{
	TRY_BEGIN
	if(x < 0 || y < 0 || SELF.GetID() == nID)
		return NULL;

	CSimpleCharacterNode * pCurr = FindSimpleCharacter(nID);
	hashXYfunc(x,y);

	if( pCurr == NULL)  // 查找是否有此id
	{

		// 无此节点
		if((pCurr = AddSimpleCharacter(nID)) == NULL) // 增加一个
		{
			// 内存不足
			return NULL;
		}

	}
	else
	{

		if( pCurr->m_MapPrev )
			pCurr->m_MapPrev->m_MapNext = pCurr->m_MapNext;
		if( pCurr->m_MapNext )
			pCurr->m_MapNext->m_MapPrev = pCurr->m_MapPrev;

	}


	pCurr->m_MapNext = m_arrMap[x][y].m_character.m_MapNext;


	if(m_arrMap[x][y].m_character.m_MapNext != NULL)
		m_arrMap[x][y].m_character.m_MapNext->m_MapPrev = pCurr;


	pCurr->m_MapPrev = &m_arrMap[x][y].m_character;
	m_arrMap[x][y].m_character.m_MapNext = pCurr;


	return pCurr;
	TRY_END_RETURN(NULL);
}
CSimpleCharacterNode * CMapArray::FindSimpleCharacter(const char* name)
{
	int i;
	CSimpleCharacterNode * pCurr;

	if(name == NULL)
		return NULL;
	if(strlen(name) == 0)
		return NULL; 
	for(i=0;i<IDMAXNUMBER;i++)
	{
		pCurr = m_arrID[i].m_IDnext;
		if(pCurr == NULL)
			continue;
		while(pCurr != NULL)
		{
			if(pCurr == NULL)
				break;
			if( strcmp(pCurr->GetName(),name) == 0)
				return pCurr;
			pCurr = pCurr->m_IDnext;
		}
	}


	return NULL;

}

CSimpleCharacterNode * CMapArray::FindSimpleCharacter(int x,int y)
{
	int i;
	int _x,_y;
	CSimpleCharacterNode * pCurr;

	for(i=0;i<IDMAXNUMBER;i++)
	{
		pCurr = m_arrID[i].m_IDnext;
		if(pCurr == NULL)
			continue;
		while(pCurr != NULL)
		{
			if(pCurr == NULL)
				break;
			pCurr->GetXY(_x,_y);
			if(_x == x && _y == y)
				return pCurr;
			pCurr = pCurr->m_IDnext;
		}
	}


	return NULL;


}


CSimpleCharacterNode * CMapArray::FindSimpleCharacter(DWORD nID)
{
	if(nID == 0) return NULL;

	CSimpleCharacterNode * pCurr = m_arrID[hashIDfunc(nID)].m_IDnext;

	while(pCurr!= NULL)
	{

		if(pCurr->GetID() == nID)
		{
			return pCurr;
		}

		pCurr = pCurr->m_IDnext;
	}
	return NULL;
}

CSimpleGoodNode * CMapArray::FindSimpleGood(DWORD nID)
{
	if(nID == 0) return NULL;

	CSimpleGoodNode * pCurr = m_arrIDgood[hashIDfunc(nID)].m_IDnext;
	while(pCurr!= NULL)
	{
		if(pCurr->GetID() == nID)
		{
			return pCurr;
		}
		pCurr = pCurr->m_IDnext;
	}
	return NULL;
}

void CMapArray::DeleteGood(DWORD nID)
{
	CSimpleGoodNode * pTemp = FindSimpleGood(nID);
	if( pTemp != NULL )
	{
		pTemp->m_MapPrev->m_MapNext = pTemp->m_MapNext;// 一定成功

		if(pTemp->m_MapNext != NULL)
			pTemp->m_MapNext->m_MapPrev = pTemp->m_MapPrev; // 不是尾节点

		pTemp->m_IDprev->m_IDnext = pTemp->m_IDnext; // 一定成功

		if(pTemp->m_IDnext != NULL)  // 不是尾节点
			pTemp->m_IDnext->m_IDprev = pTemp->m_IDprev;

		delete pTemp;
		pTemp = NULL;
	}

	// add by Yuan，删除链表对应结点
	MiniGood::iterator itr = m_MiniGood.find(nID);
	if(itr != m_MiniGood.end())
		m_MiniGood.erase(itr);
}

void CMapArray::DeleteCharacter(DWORD nID)
{
	CSimpleCharacterNode * pTemp = FindSimpleCharacter(nID);
	if(pTemp != NULL)
	{
		pTemp->m_MapPrev->m_MapNext = pTemp->m_MapNext;// 一定成功

		if(pTemp->m_MapNext != NULL)
			pTemp->m_MapNext->m_MapPrev = pTemp->m_MapPrev; // 不是尾节点

		pTemp->m_IDprev->m_IDnext = pTemp->m_IDnext; // 一定成功

		if(pTemp->m_IDnext != NULL)  // 不是尾节点
			pTemp->m_IDnext->m_IDprev = pTemp->m_IDprev;

		delete pTemp;
		pTemp = NULL;

		// add by Yuan，删除链表对应结点
		MiniChar::iterator itr = m_MiniChar.find(nID);
		if(itr != m_MiniChar.end())
			m_MiniChar.erase(itr);
		////

	}
}

void CMapArray::DeleteAllXYGoods(int x,int y)
{
	if(x < 0 || y < 0)
		return;

	hashXYfunc(x,y);
	CSimpleGoodNode * pTemp = m_arrMap[x][y].m_good.m_MapNext;

	while(pTemp != NULL)
	{
		pTemp->m_IDprev->m_IDnext = pTemp->m_IDnext;
		if(pTemp->m_IDnext != NULL)
			pTemp->m_IDnext->m_IDprev = pTemp->m_IDprev;

		m_arrMap[x][y].m_good.m_MapNext = pTemp->m_MapNext;

		// add by Yuan，删除链表对应结点
		MiniGood::iterator itr = m_MiniGood.find(pTemp->GetID());
		if(itr != m_MiniGood.end())
			m_MiniGood.erase(itr);

		delete pTemp;
		pTemp = m_arrMap[x][y].m_good.m_MapNext;
	}	
}

void CMapArray::DeleteAllXYCharacters(int x,int y)
{
	if(x < 0 || y < 0)
		return;

	hashXYfunc(x,y);
	CSimpleCharacterNode * pTemp = m_arrMap[x][y].m_character.m_MapNext;

	while(pTemp != NULL)
	{
		pTemp->m_IDprev->m_IDnext = pTemp->m_IDnext;
		if(pTemp->m_IDnext != NULL)
			pTemp->m_IDnext->m_IDprev = pTemp->m_IDprev;

		m_arrMap[x][y].m_character.m_MapNext = pTemp->m_MapNext;

		// add by Yuan，删除链表对应结点
		MiniChar::iterator itr = m_MiniChar.find(pTemp->GetID());
		if(itr != m_MiniChar.end())
			m_MiniChar.erase(itr);
		////

		delete pTemp;
		pTemp = m_arrMap[x][y].m_character.m_MapNext;
	}
}

CSimpleGoodNode * CMapArray::GetSimpleGoodHead(int x,int y)
{
	if(x < 0 || y < 0)
		return NULL;

	hashXYfunc(x,y);
	return m_arrMap[x][y].m_good.m_MapNext;
}
CSimpleCharacterNode * CMapArray::GetSimpleCharacterHead(int x,int y)
{
	if(x < 0 || y < 0)
		return NULL;

	hashXYfunc(x,y);
	return m_arrMap[x][y].m_character.m_MapNext;
}

void CMapArray::Clear()
{
	for(int i = 0; i < MAPARR_WIDTH; i++)
	{
		for(int j = 0; j < MAPARR_WIDTH; j++)
		{
			this->DeleteAllXYCharacters(i,j);
			this->DeleteAllXYGoods(i,j);
		}

	}
	m_MiniChar.clear();
}

void CMapArray::ResetAllCharacters()
{
	for(int i = 0; i < MAPARR_WIDTH; i++)
	{
		for(int j = 0; j < MAPARR_WIDTH; j++)
		{
			hashXYfunc(i,j);
			CSimpleCharacterNode * pTemp = m_arrMap[i][j].m_character.m_MapNext;

			while(pTemp != NULL)
			{
				int x,y;
				pTemp->GetRealXY(x,y);
				pTemp->SetXY(x,y);
				pTemp->SetOffset(0,0);
				pTemp->ClearAllNext();
				//切换微操时gs是先发身边的怪物信息再发微操成功,所以不判断的话收到MSG_Monster_Corpus后这里会重新把death动作变成了stand动作
				//if (pTemp->GetAction().wAction != ACTION_DEATH)
				//{
				//	pTemp->InitAction(ACTION_STAND);
				//}
				if(pTemp->IsDead())
				{
					pTemp->InitAction(ACTION_DEATH);
					SAction& actionNow = pTemp->GetAction();
					actionNow.tFrameStart = 1;//停到最后一帧
				}
				else
				{
					pTemp->InitAction(ACTION_STAND);
				}

				pTemp = pTemp->m_MapNext;
			}
		}
	}
}