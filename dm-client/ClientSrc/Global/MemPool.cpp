#include <stdio.h>
#include "Global/Globaldefine.h"

#include "mempool.h"

#ifdef _DEBUG
#include <stdio.h>
#include "new.h"
#define new DEBUG_NEW
#endif

CClassCache::CClassCache(UINT uEleSize,UINT uBaseEles,UINT uAddEles,char *sName)
{
	m_uEleSize	= uEleSize < sizeof(void*) ? sizeof(void*) : uEleSize;
	m_uBaseEles = uBaseEles;
	m_uAddEles	= uAddEles;

	if(sName && strlen(sName) < 256)
		strcpy(m_sPoolName,sName);
	else
		m_sPoolName[0] = 0;

	Init();
	ExtendPool();
}


CClassCache::~CClassCache(void)
{
#if 0
	if(m_uAllocEles != 0)
	{
		char fn[256];
		sprintf(fn,"..\\pool_%s.txt",m_sPoolName);
		FILE *fp = fopen(fn,"a");

		//日期
		if(fp)
		{
			fprintf(fp, "日期:%s\n", g_Timer.GetDateTime());
		}

		//类型
		char sss[128] = {0};
		sprintf(sss,"%s pool memLeak detected:Elements(%d) TotalSize(%dbytes)!\n",m_sPoolName,m_uAllocEles,m_uAllocEles * m_uEleSize);
		OutputDebugString(sss);

		if(fp)
		{
			fprintf(fp, "%s\n", sss );
		}
		
		//查找所有的空闲位置,标记
		void *pTempFree = m_pFreeFirst;
		while(pTempFree)
		{
			void *p = pTempFree;
			pTempFree = *((void **)p);
			
			memset(p,0xBB,4);			//
		}

		int iCounts = 1;
		//遍历所有的分配表
		HeadList *pNext;
		for( HeadList *pCur = m_HeadList.pNextHead; pCur != &m_HeadList; pCur = pNext )
		{
			unsigned char *p = (unsigned char *)pCur->pData;
			UINT uEles	= (pCur->uSize - sizeof(HeadList) + 4) /m_uEleSize;

			//遍历所有的元素
			for(UINT i = 0; i < uEles; i++,p += m_uEleSize)
			{
				if(*((DWORD *)p) != 0xBBBBBBBB)
				{
					if(fp)
					{
						fprintf(fp, "索引[%d] :\n", iCounts );
						fprintf(fp, "内存块：\n");
						unsigned char *c = p;
						for(unsigned int i = 1;i<m_uEleSize+1;++i)
						{
							fprintf(fp, "%02X ", c[i-1] );
							//if(!(i%8))
							//	fprintf(fp, "    ");
							if(!(i%16))
								fprintf(fp, "\n");
						}
						fprintf(fp, "\n");
					}
				}
			}

			pNext = pCur->pNextHead;
		}

		if(fp)
		{
			fprintf(fp, "\n");
			fclose(fp);
		}
	}
#endif // _DEBUG
	Clear();
}

void CClassCache::Init(void)
{
	m_uAllocEles	= 0;
	m_uAllocMaxEles	= 0;
	m_pFreeFirst	= NULL;

	m_HeadList.pNextHead = m_HeadList.pPrevHead = &m_HeadList;
}

void * CClassCache::NewEle(void)
{
	if(m_pFreeFirst == NULL && !ExtendPool())
		return NULL;

	m_uAllocEles++;
	if(m_uAllocMaxEles < m_uAllocEles)
		m_uAllocMaxEles = m_uAllocEles;

	void *p = m_pFreeFirst;
	m_pFreeFirst = *((void **)m_pFreeFirst);

	return p;
}

void CClassCache::DelEle(void *pe)
{
	if(pe == NULL && m_uAllocEles == 0)
		return;

#ifdef _DEBUG
	bool bOK = false;
	for(HeadList *pHead = m_HeadList.pNextHead; pHead != &m_HeadList; pHead = pHead->pNextHead)
	{
		if((char *)pe >= pHead->pData && (char *)pe < pHead->pData + pHead->uSize)
		{
			if(((char *)pe - pHead->pData) % m_uEleSize == 0)
				bOK = true;
			break;
		}
	}
	if(!bOK)
	{
		OutputDebugString("Delete error\n");
		return;
	}
	memset(pe,0xBB,m_uEleSize);
#endif // _DEBUG

	m_uAllocEles--;

	*((void **)pe)	= m_pFreeFirst;
	m_pFreeFirst		= pe;
}

bool CClassCache::ExtendPool(void)
{
	UINT uEles	= (m_HeadList.pNextHead == &m_HeadList) ? m_uBaseEles : m_uAddEles;
	UINT uSize	= uEles * m_uEleSize + sizeof(HeadList) - 4;

	HeadList *pHeadList = (HeadList *)(new char[uSize]);
	if(pHeadList == NULL)
		return false;

	pHeadList->uSize = uSize;

	pHeadList->pNextHead = &m_HeadList;
	pHeadList->pPrevHead = pHeadList->pNextHead->pPrevHead;
	pHeadList->pNextHead->pPrevHead = pHeadList->pPrevHead->pNextHead = pHeadList;

	m_pFreeFirst = pHeadList->pData;

	char *p = pHeadList->pData;
	for(UINT i = 0; i < uEles - 1; i++,p += m_uEleSize)
		*((void **)p) = (void *)(p + m_uEleSize);
	*((void **)p) = NULL;

	//{
	//	char sss[128] = {0};
	//	sprintf(sss,"Extend eles:%d\n",uEles);
	//	OutputDebugString(sss);
	//}
	return true;
}

UINT CClassCache::GetAllocSize(void)
{
	return m_uAllocEles;
}

UINT CClassCache::GetMaxAllocSize(void)
{
	return m_uAllocMaxEles;
}

void CClassCache::Clear(void)
{
	HeadList *pNext;
	for( HeadList *pCur = m_HeadList.pNextHead; pCur != &m_HeadList; pCur = pNext )
	{
		pNext = pCur->pNextHead;
		delete (char *)pCur;
	}
	Init();
}