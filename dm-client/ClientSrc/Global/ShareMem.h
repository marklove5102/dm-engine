#pragma once

#include "Global/Globaldefine.h"

class CShareMem
{
private:
	BOOL		m_bOK;
	HANDLE		m_hFileMap;
public:
	CShareMem(void);
	~CShareMem(void);
	BOOL GetShare(PVOID pVoid, int iSize);
	BOOL SetShare(PVOID pVoid, int iSize);
};

extern CShareMem	g_ShareMem;