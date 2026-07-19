#include "FindGood.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

CFindGood g_FindGood;

CFindGood::CFindGood(void)
{
	m_VDisplay.clear();
}

CFindGood::~CFindGood(void)
{
}

void CFindGood::AddMessage(const char* str,int type)
{
	if(m_VDisplay.size() != 0 && type > 2)
	{
		if(m_VDisplay.back().str.compare(str) == 0) //相同的信息不再提醒
			return;
	}

	if(m_VDisplay.size() > 10)
		m_VDisplay.erase(m_VDisplay.begin());

	m_VDisplay.push_back(_FindData(str,type));
}