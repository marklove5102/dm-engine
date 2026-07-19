#include "KfzFlow.h"

INIT_WND_POSX(CKfzFlow,POS_AUTO,POS_AUTO)

CKfzFlow::CKfzFlow(void)
{
	m_iIndex = 16049 + sm_dwWindowType;
	m_iPages = 1;
}

CKfzFlow::~CKfzFlow(void)
{
}

bool CKfzFlow::OnLeftButtonUp(int iX, int iY)
{
	CloseWindow();
	return true;
}

void CKfzFlow::OnKillFocus()
{
	CloseWindow();
}
