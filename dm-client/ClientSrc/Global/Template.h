#pragma once

template<class T>
class ListNode : public T
{
public:
	ListNode<T>()
	{
		m_IDnext = NULL;
		m_IDprev = NULL;
		m_MapNext = NULL;
		m_MapPrev = NULL;
	}
	int GetNodeCount()
	{
		int count = 0;
		ListNode<T>* p = this;
		while(p)
		{
			count++;
			p = p->m_MapNext;
		}
		return count;
	}
	bool HasID(unsigned long id)
	{
		ListNode<T>* p = this;
		while(p)
		{
			if(p->GetID() == id)
				return true;

			p = p->m_MapNext;
		}
		return false;
	}
public:
	ListNode<T> * m_IDnext;
	ListNode<T> * m_IDprev;
	ListNode<T> * m_MapNext;
	ListNode<T> * m_MapPrev;
};