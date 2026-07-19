#include "TexPool.h"

#define INIT_MEM_POOL(size,base)	{\
										stPool pool;\
										pool.dwEleSize		= size;\
										pool.dwBaseEles		= base;\
										pool.dwCurEles		= 0;\
										pool.dwMaxEles		= 0;\
										pool.dwCurAvlSize	= 0;\
										for(int i = 0;i < base;i++)\
										{\
											pool.vFreePool.push_back(new BYTE[size]);\
										}\
										m_vPools.push_back(pool);\
									}

#define ADD_HIS(now,his,add)		{\
										(now) += (add);\
										if((now) > (his))\
											(his) = (now);\
									}

CTexPool::CTexPool(BOOL bUsePool)
{
	m_bUsePool		= bUsePool;

	m_dwCurAvlSize	= 0;
	m_dwMaxAvlSize	= 0;
	m_dwCurAllSize	= 0;
	m_dwMaxAllSize	= 0;
	m_dwOverSize	= 0;
	m_dwMaxOverSize	= 0;

	InitPool();

	InitializeCriticalSection(&m_CriSect);
}

CTexPool::~CTexPool(void)
{
	ReleaseAll();
	DeleteCriticalSection(&m_CriSect);
}

BOOL CTexPool::InitPool(void)
{
	if(m_bUsePool)
	{
		INIT_MEM_POOL(  500,  64);
		INIT_MEM_POOL( 1000, 400);
		INIT_MEM_POOL( 1500, 400);
		INIT_MEM_POOL( 2000, 400);
		INIT_MEM_POOL( 2500, 400);
		INIT_MEM_POOL( 3000, 400);
		INIT_MEM_POOL( 3500,  64);
		INIT_MEM_POOL( 4000,  64);
		INIT_MEM_POOL( 4500,  64);
		INIT_MEM_POOL( 5000,  64);
		INIT_MEM_POOL( 5500,  64);
		INIT_MEM_POOL( 6000,  64);
		INIT_MEM_POOL( 6500,  64);
		INIT_MEM_POOL( 7000,  64);
		INIT_MEM_POOL( 7500,  64);
		INIT_MEM_POOL( 8000,  64);
		INIT_MEM_POOL( 8500,  64);
		INIT_MEM_POOL( 9000,  64);
		INIT_MEM_POOL( 9500,  64);
		INIT_MEM_POOL(10000,  64);
		INIT_MEM_POOL(10500,  64);
		INIT_MEM_POOL(11000,  64);
		INIT_MEM_POOL(11500,  64);
		INIT_MEM_POOL(12000,  64);
	}
	return TRUE;
}

void CTexPool::ReleaseAll(void)
{
	for(size_t j = 0;j < m_vPools.size();j++)
	{
		stPool& pool = m_vPools[j];
		for(size_t i = 0;i < m_vPools[j].vFreePool.size();i++)
		{
			delete [] (m_vPools[j].vFreePool[i]);
		}
		m_vPools[j].vFreePool.clear();
	}
	m_vPools.clear();
}

BYTE *CTexPool::NewTexMem(DWORD size)
{
	//{
	//	static DWORD vCount[17] = {0};
	//	int idx = (size - 1) / 500;
	//	if(size > 16 * 500)
	//		idx = 16;
	//	vCount[idx]++;
	//}

	if(!m_bUsePool)
		return new BYTE[size];

	stEle	ele;
	BYTE *	p = NULL;

	EnterCriticalSection(&m_CriSect);
	// 临时的分布规则
	{
		int iPool = -1;
		if(size <= m_vPools[m_vPools.size() - 1].dwEleSize)
			iPool = (size - 1) / 500;

		ele.dwSize = size;
		ADD_HIS(m_dwCurAvlSize,m_dwMaxAvlSize,size);
		if(iPool == -1)
		{
			ele.pPool = NULL;
			p = new BYTE [size];
			ADD_HIS(m_dwOverSize,m_dwMaxOverSize,size);
			ADD_HIS(m_dwCurAllSize,m_dwMaxAllSize,size);
		}
		else
		{
			ele.pPool = &m_vPools[iPool];
			if(ele.pPool->vFreePool.size() == 0)
				ele.pPool->vFreePool.push_back(new BYTE[ele.pPool->dwEleSize]);

			ele.pPool->dwCurAvlSize += size;
			p = ele.pPool->vFreePool.back();
			ele.pPool->vFreePool.pop_back();

			ADD_HIS(ele.pPool->dwCurEles,ele.pPool->dwMaxEles,1);
			ADD_HIS(m_dwCurAllSize,m_dwMaxAllSize,ele.pPool->dwEleSize);
		}

		if(p)
			m_mUsedEles[p] = ele;
	}
	LeaveCriticalSection(&m_CriSect);
	return p;
}

void CTexPool::DelTexMen(BYTE *p)
{
	if(!m_bUsePool)
		delete []p;

	EnterCriticalSection(&m_CriSect);

	MPoint::iterator itr = m_mUsedEles.find(p);
	if(itr == m_mUsedEles.end())
	{
		LeaveCriticalSection(&m_CriSect);
		return;
	}

	stEle& ele = itr->second;
	if(ele.pPool)
	{
		ele.pPool->dwCurEles--;
		ele.pPool->dwCurAvlSize -= ele.dwSize;
		ele.pPool->vFreePool.push_back(p);

		m_dwCurAllSize -= ele.pPool->dwEleSize;
	}
	else
	{
		m_dwOverSize	-= ele.dwSize;
		m_dwCurAllSize	-= ele.dwSize;
		delete []p;
	}
	m_dwCurAvlSize	-= ele.dwSize;
	m_mUsedEles.erase(itr);

	LeaveCriticalSection(&m_CriSect);
}

BOOL CTexPool::CleanUp(void)
{
	return FALSE;
}