#pragma once

typedef unsigned int	UINT;

#define DEFAULT_ELES	16

#include <new>

class CClassCache
{
public:
	CClassCache(UINT uEleSize,UINT uBaseEles = DEFAULT_ELES,UINT uAddEles = DEFAULT_ELES,char *sName = 0);
	~CClassCache(void);

	void *	NewEle(void);			// 申请一个元素
	void	DelEle(void *pEle);		// 释放一个元素
	UINT	GetAllocSize(void);
	UINT	GetMaxAllocSize(void);
	void	Clear(void);

private:
	class HeadList
	{
	public:
		HeadList *	pPrevHead;
		HeadList *	pNextHead;
		UINT		uSize;
		char		pData[4];	// 为了字节对齐
	};
	char	m_sPoolName[256];	// 缓冲池名称
	UINT	m_uEleSize;			// 每个元素的分配的大小
	UINT	m_uBaseEles;		// 最大的分配的元素个数
	UINT	m_uAddEles;			// 第一池用完后增加的步长

	UINT	m_uAllocEles;		// 已经被用掉了元素个数
	UINT	m_uAllocMaxEles;	// 最大被分配元素的个数

	void  *	m_pFreeFirst;		// 空闲指针链表

	HeadList m_HeadList;		// 链表指针

	bool	ExtendPool(void);	//
	void	Init(void);
};

#define DECLARE_POOL(ClassName)	\
	private:\
		static CClassCache s_MemPool;\
	public:\
		static ClassName * 	NEW_##ClassName(void);\
		static void			DEL_##ClassName(ClassName * pDelClass);\
		static void			CLR_##ClassName();


#define IMPLEMENT_POOL(ClassName,BaseEles,AddEles)\
	CClassCache	ClassName::s_MemPool(sizeof(##ClassName),BaseEles,AddEles,#ClassName);\
	ClassName * ClassName::NEW_##ClassName(void)\
	{\
		ClassName * pNewClass = (ClassName *)(s_MemPool.NewEle());\
		if(pNewClass)\
			new (pNewClass) ClassName;\
		return pNewClass;\
	}\
	void ClassName::DEL_##ClassName(ClassName* pDelClass)\
	{\
		if(pDelClass)\
		{\
			pDelClass->~##ClassName();\
			s_MemPool.DelEle(pDelClass);\
		}\
	}\
	void ClassName::CLR_##ClassName()\
	{\
		s_MemPool.Clear();\
	}