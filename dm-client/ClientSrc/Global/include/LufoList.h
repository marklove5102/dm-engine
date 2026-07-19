
#pragma once

#include "Global/Globaldefine.h"
#include <map>

template <typename T>
class CLufoList
{
private:
	struct stList
	{
		stList *Prev;
		stList *Next;

		T t;
		stList():Prev(NULL),Next(NULL)
		{
		}
	};
	typedef std::map<T,stList*>	MFindMap;

	MFindMap	m_mFindMap;
	stList*		m_pListHead;
	stList*		m_pListEnd;

private:
	void ListAdd(stList* pList)
	{
		if(!pList)
			return;
		if(m_pListHead == NULL)
			m_pListHead = m_pListEnd = pList;
		else
		{
			pList->Next			= m_pListHead;
			m_pListHead->Prev	= pList;
			m_pListHead			= pList;
		}
	}

	void ListDel(stList* pList)
	{
		if(!pList)
			return;

		if(pList == m_pListHead)
			m_pListHead = pList->Next;
		if(pList == m_pListEnd)
			m_pListEnd = pList->Prev;

		if(pList->Prev)
			pList->Prev->Next = pList->Next;
		if(pList->Next)
			pList->Next->Prev = pList->Prev;
	}

	void ListHead(stList* pList)
	{
		if(!pList || pList == m_pListHead)
			return;

		if(pList == m_pListEnd)
			m_pListEnd = pList->Prev;

		if(pList->Prev)
			pList->Prev->Next = pList->Next;
		if(pList->Next)
			pList->Next->Prev = pList->Prev;

		pList->Prev			= NULL;
		pList->Next			= m_pListHead;
		m_pListHead->Prev	= pList;
		m_pListHead			= pList;
	}

public:
	CLufoList()
	{
		m_pListHead = NULL;
		m_pListEnd	= NULL;
	}

	~CLufoList()
	{
		while(m_pListEnd)
		{
			erase(m_pListEnd->t);
		}
	}

	void push(T t)
	{
		stList* pList = new stList;
		if(pList)
		{
			pList->t = t;
			m_mFindMap[t] = pList;
			ListAdd(pList);
		}
	}

	void pop(void)
	{
		if(m_pListEnd)
			erase(m_pListEnd->t);
	}

	int size(void)
	{
		return (int)m_mFindMap.size();
	}

	void erase(T t)
	{
		MFindMap::iterator itr = m_mFindMap.find(t);
		if(itr == m_mFindMap.end())
			return;
		ListDel(itr->second);
		SAFE_DELETE(itr->second);
		m_mFindMap.erase(itr);
	}

	BOOL find(T t)
	{
		if(m_mFindMap.find(t) == m_mFindMap.end())
			return FALSE;
		return TRUE;
	}

	BOOL reflesh(T t)
	{
		MFindMap::iterator itr = m_mFindMap.find(t);
		if(itr == m_mFindMap.end())
			return FALSE;

		ListHead(itr->second);
		return TRUE;
	}

	T& back(void)
	{
		return m_pListEnd->t;
	}
};