#include "SimpleGood.h"

CSimpleGood::CSimpleGood()
{
	m_nID = 0;
	m_nLooks  = 0;
	m_dwStar = rand() * 100/RAND_MAX;
	m_iX = m_iY = 0;
	m_dwShowType = 0;
}


CSimpleGood::~CSimpleGood()
{

}

void CSimpleGood::SetID(DWORD nID)
{
	m_nID = nID;
	if(nID == 0)
	{
		m_strName.clear();
		m_nLooks = 0;
		m_dwShowType = 0;
	}
}

void CSimpleGood::FromBuffer(const char* buf,int iLen)
{
	m_iX	= Conv_WORD(buf + 6);
	m_iY	= Conv_WORD(buf+ 8);
	m_nLooks= Conv_WORD(buf + 10);

	m_strName.assign(buf+12,iLen-12);
}

