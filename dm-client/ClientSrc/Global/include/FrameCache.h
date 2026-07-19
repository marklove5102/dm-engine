#pragma once

#include "Global/Globaldefine.h"

#define	MIN_POOL_SRC	1
#define COUNT			(*pdwCurCount)

enum GetSrcType
{
	NoCurCount		= -1,		// 没有计数指针
	NotEnoughSrc		,		// 池不够大
	BaseSrc				,		// 使用了基本池了
	NormalSrc			,		// 一般的淘汰池
	HitSuccess			,		// 已经在缓冲池中
	FreeSrc				,		// 使用空闲的池
};

// 帧缓冲
template <class _Src>
class CFrameCache
{
public:
	class CUser;
	class SrcList;

	typedef CUser	*_PUser;
	typedef _Src	*_PSrc;

	// 使用资源类
	class CUser
	{
		friend class CFrameCache<_Src>;
	protected:
		SrcList *pList;
	public:
		CUser(void):pList(NULL)
		{
		}
		~CUser(void)
		{
		}
		inline _PSrc GetSrc(void)
		{
			return pList ? pList->pSrc : NULL;
		}
	};

	// 池中每个节点
	class SrcList
	{
		friend class CFrameCache<_Src>;
		friend class CUser;
	protected:
		_PSrc	pSrc;		// 资源
		_PUser	pUser;		// 使用者
		DWORD	dwCount;	// 节点计数

		SrcList *Prev;		// 前节点
		SrcList *Next;		// 后节点
	public:
		SrcList():pSrc(NULL),pUser(NULL),dwCount(0),Prev(NULL),Next(NULL){}
	};

private:
	DWORD*		pdwCurCount;	// 计数
	DWORD		dwSrcs;			// 资源数量

	// 一个基本节点和一个池
	SrcList		BaseNode;		// 基本节点
	SrcList		*pNormalHead;	// 缓冲池头节点
	SrcList		*pNormalEnd;	// 缓冲池尾节点
private:
	// 移动节点到头或尾
	void MoveNode(SrcList *pList,BOOL bHead = TRUE)
	{
		// 特殊处理头尾的情况
		if(pList == pNormalHead)
		{
			if(bHead)
				return;
		}
		if(pList == pNormalEnd)
		{
			if(!bHead)
				return;
		}

		// 断开
		if(pList->Prev)
			pList->Prev->Next = pList->Next;
		else
			pNormalHead = pList->Next;

		if(pList->Next)
			pList->Next->Prev = pList->Prev;
		else
			pNormalEnd = pList->Prev;

		if(bHead)		// 放置到头部
		{
			pList->Prev			= NULL;
			pList->Next			= pNormalHead;
			pNormalHead->Prev	= pList;
			pNormalHead			= pList;
		}
		else			// 放置到尾部
		{
			pList->Prev			= pNormalEnd;
			pList->Next			= NULL;
			pNormalEnd->Next	= pList;
			pNormalEnd			= pList;
		}
	}

public:
	CFrameCache(void)
	{
		pNormalHead	= NULL;
		pNormalEnd	= NULL;

		pdwCurCount	= NULL;
		dwSrcs		= 0;
	}

	~CFrameCache(void)
	{
	}

	// 获取资源个数
	int GetSrcNum(void)
	{
		return dwSrcs;
	}

	BOOL HasFree(void)
	{
		if(pNormalEnd && pNormalEnd->pUser == NULL)
			return TRUE;
		else
			return FALSE;
	}

	// 增加计数
	void SetCount(DWORD *pdw)
	{
		pdwCurCount = pdw;
	}

	// 判断最后一个资源是否在当前帧
	BOOL GetEndIsInCur()
	{
		if (pNormalEnd && ((COUNT <= 1) || pNormalEnd->dwCount >= (COUNT)))
		{
			return TRUE;
		}

		return FALSE;
	}

	// 添加一资源
	BOOL AddSrc(_PSrc pSrc)
	{
		dwSrcs++;

		// 首先需要满足基本节点
		if(BaseNode.pSrc == NULL)
		{
			memset(&BaseNode,0,sizeof(SrcList));

			BaseNode.pSrc = pSrc;
			return TRUE;
		}

		// 其他节点加入链表尾部
		SrcList *pList = new SrcList();
		if(!pList)
			return FALSE;

		pList->pSrc = pSrc;

		if(pNormalHead == NULL)	// 链表还是空的
			pNormalHead = pNormalEnd = pList;
		else					// 链表中已经有节点了
		{
			pList->Prev			= pNormalEnd;
			pList->Next			= NULL;
			pNormalEnd->Next	= pList;

			pNormalEnd			= pList;
		}
		return TRUE;
	}

	// 释放所有资源
	void ReleaseAllSrc(void)
	{
		// 释放基本节点资源
		if(BaseNode.pUser)
		{
			BaseNode.pUser->pList	= NULL;
			BaseNode.pUser			= NULL;
		}
		SAFE_RELEASE(BaseNode.pSrc);
		memset(&BaseNode,0,sizeof(SrcList));

		// 释放链表中的资源
		SrcList *pList = NULL;
		while(pNormalHead)
		{
			pList = pNormalHead->Next;

			if(pNormalHead->pUser)
			{
				pNormalHead->pUser->pList	= NULL;
				pNormalHead->pUser			= NULL;
			}
			SAFE_RELEASE(pNormalHead->pSrc);
			SAFE_DELETE(pNormalHead);

			pNormalHead = pList;
		}

		// 设置为初始状态
		dwSrcs		= 0;
		pNormalHead = NULL;
		pNormalEnd	= NULL;
	}

	// 清除所有资源
	void DeleteAllSrc()
	{
		// 清除基本节点资源
		if(BaseNode.pUser)
		{
			BaseNode.pUser->pList	= NULL;
			BaseNode.pUser			= NULL;
		}
		SAFE_DELETE(BaseNode.pSrc);
		memset(&BaseNode,0,sizeof(SrcList));

		// 清除链表中的资源
		SrcList *pList = NULL;
		while(pNormalHead)
		{
			pList = pNormalHead->Next;

			if(pNormalHead->pUser)
			{
				pNormalHead->pUser->pList	= NULL;
				pNormalHead->pUser			= NULL;
			}
			SAFE_DELETE(pNormalHead->pSrc);
			SAFE_DELETE(pNormalHead);

			pNormalHead = pList;
		}

		// 设置为初始状态
		dwSrcs		= 0;
		pNormalHead = NULL;
		pNormalEnd	= NULL;
	}

	void MoveFront(SrcList *pList)
	{
		if (pList)
		{
			if (pList != pNormalHead && pList != &BaseNode)
			{
				// 断开
				pList->Prev->Next = pList->Next;

				if(pList->Next)
					pList->Next->Prev = pList->Prev;
				else
					pNormalEnd = pList->Prev;

				//放到最前
				pList->Prev			= NULL;
				pList->Next			= pNormalHead;
				pNormalHead->Prev	= pList;
				pNormalHead			= pList;
			}
			pList->dwCount = COUNT;
		}		
	}

	// 从池中获取一资源
	GetSrcType GetSrc(_PUser pUser)
	{
		if(!pdwCurCount)											// 还没有计数指针
			return NoCurCount;

		GetSrcType	eRet;
		if(dwSrcs < MIN_POOL_SRC)									// 缓冲池太小
			eRet = NotEnoughSrc;
		else if(pUser->pList)										// 已经在缓冲池中
		{
			eRet = HitSuccess;
			pUser->pList->dwCount = COUNT;
			if(pUser->pList != pNormalHead && pUser->pList != &BaseNode)
				MoveNode(pUser->pList,TRUE);
		}
		else if(pNormalEnd && (COUNT <= 1 || pNormalEnd->dwCount < (COUNT - 1)))	// 从池中最后一资源
		{
			if(pNormalEnd->pUser)
			{
				eRet = NormalSrc;
				pNormalEnd->pUser->pList = NULL;
			}
			else
				eRet = FreeSrc;

			pNormalEnd->pUser		= pUser;
			pNormalEnd->dwCount		= COUNT;
			pUser->pList			= pNormalEnd;

			MoveNode(pNormalEnd,TRUE);
		}
		else														// 使用基本池
		{
			eRet = BaseSrc;
			if(BaseNode.pUser)
				BaseNode.pUser->pList = NULL;

			BaseNode.pUser		= pUser;
			BaseNode.dwCount	= COUNT;
			pUser->pList		= &BaseNode;
		}
		return eRet;
	}

	// 释放pUser所占用的资源
	void Release(_PUser pUser)
	{
		if(pUser->pList == NULL)
			return;
		if(pUser->pList == &BaseNode)
		{
			BaseNode.pUser		= NULL;
			BaseNode.dwCount	= 0;
		}
		else
		{
			pUser->pList->pUser		= NULL;
			pUser->pList->dwCount	= 0;
			MoveNode(pUser->pList,FALSE);
		}

		pUser->pList		= NULL;
	}
};