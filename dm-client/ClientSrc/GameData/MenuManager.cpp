#include "MenuManager.h"

CMenuManager::CMenuManager()
{
	m_iCounter = -1;
	m_ptPopPos.x = 0;
	m_ptPopPos.y = 0;
	m_bReverse = false;
	m_bNeedFocusOn = true;
	m_eCaller		= POP_UNDEFINED;
}

void CMenuManager::StartPushItem()
{
	m_eCaller		= POP_UNDEFINED;
	m_iCounter = -1;
	//memcpy(g_cBuf, (void*)m_iCounter, sizeof(m_iCounter));
	*((int*)g_cBuf) = -1;
	m_iCounter++;

	//添加默认关闭按钮
	//MenuItem defaultItem(MSG_CTRL_POP_MENU, CMenuManager::POP_CLOSE, NULL, "关闭");
	//PushItem(defaultItem);
}

void CMenuManager::PushItem(MenuItem& rfItem)
{
	memcpy(g_cBuf + sizeof(m_iCounter) + m_iCounter * sizeof(MenuItem),	(void*)&rfItem, sizeof(MenuItem));
	++m_iCounter;
}

void CMenuManager::EndPushItem()
{
	*((int*)g_cBuf) = m_iCounter;
	m_iCounter = -1;
}

bool CMenuManager::StartAppendItem()
{
	int iOriginalSize = *((int*)g_cBuf);
	if(iOriginalSize > 0)
	{
		m_iCounter = iOriginalSize-1;
		return true;
	}
	return false;
}

int CMenuManager::GetSize()
{
	return *((int*)g_cBuf);
}

bool CMenuManager::GetItem(int iIndex, MenuItem& rfGoal)
{
	if(iIndex >= 0 && iIndex < GetSize())
	{
		rfGoal = (*((MenuItem*)(g_cBuf + sizeof(m_iCounter) + iIndex * sizeof(MenuItem))));
		return true;
	}
	return false;
}

void CMenuManager::Clear()
{
	if(m_iCounter > 0)
	{
		memset(g_cBuf, NULL, sizeof(m_iCounter) + (m_iCounter-1) * sizeof(MenuItem));
	}
	m_bNeedFocusOn = true;
	m_eCaller		= POP_UNDEFINED;
}