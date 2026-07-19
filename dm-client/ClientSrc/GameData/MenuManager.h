#pragma once

#include "Global/Global.h"

#pragma warning(disable : 4996)

class CControl;

struct MenuItem
{
	DWORD		m_dwMsg;
	DWORD		m_dwData;
	CControl*	m_pControl;
	char		m_szItemName[20];
	const char*	m_szTip;

	MenuItem(DWORD dwMsg, DWORD dwData, CControl* pControl, const char* szName, const char* pTip=NULL)
	{
		m_dwMsg = dwMsg;
		m_dwData = dwData;
		m_pControl = pControl;
		strcpy(m_szItemName, szName);
		m_szTip	= pTip;
	}
	MenuItem()
	{
		m_dwMsg = 0;
		m_dwData = 0;
		m_pControl = NULL;
		memset(m_szItemName, 0, sizeof(m_szItemName));
		m_szTip	= NULL;
	}
};

class CMenuManager
{
public:
	typedef enum eEDIT_POP_TYPE
	{
		POP_CLOSE = 0,
		POP_OPEN
	} EDIT_POP_TYPE;

	typedef enum ePOP_CALL_BY
	{
		POP_UNDEFINED	= 0,
		POP_BY_IM_FRIEND,
	} POP_CALL_BY;

public:
	CMenuManager();
	void StartPushItem();
	void EndPushItem();
	void PushItem(MenuItem& rfItem);
	bool StartAppendItem();
	int GetSize();
	inline int	GetCounter() {return m_iCounter;}
	bool GetItem(int iIndex, MenuItem& rfGoal);
	void Clear();
	inline void SetPopPos(POINT pt) { m_ptPopPos = pt;}
	inline POINT GetPopPos() { return m_ptPopPos;}
	inline bool GetReverse() { return m_bReverse;}
	inline void SetReverse(bool b) {m_bReverse = b;}
	inline void SetNeedFocusOn(bool b) { m_bNeedFocusOn = b;}
	inline bool DoesNeedFocusOn() { return m_bNeedFocusOn;}
	inline void SetCaller(POP_CALL_BY e) { m_eCaller = e;}
	inline POP_CALL_BY GetCaller() const {return m_eCaller;}

private:
	int		m_iCounter;
	POINT	m_ptPopPos;
	bool	m_bReverse;
	bool	m_bNeedFocusOn;
	POP_CALL_BY	m_eCaller;
};