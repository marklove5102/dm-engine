#include "sharemem.h"

#define		SHARE_FILE_NAME		"WOLWS_SHARE_MEM"
#define		SHARE_SIZE			1024


CShareMem g_ShareMem;							// 进程共享数据

CShareMem::CShareMem(void)
{
	m_hFileMap = OpenFileMapping(FILE_MAP_READ|FILE_MAP_WRITE,FALSE,SHARE_FILE_NAME);
	if(m_hFileMap)
	{
		m_bOK = TRUE;
		return;
	}
	m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,SHARE_SIZE,SHARE_FILE_NAME);
	if(!m_hFileMap || GetLastError() == ERROR_ALREADY_EXISTS)
        m_bOK = FALSE;
	else
		m_bOK = TRUE;

	char str[SHARE_SIZE] = {0};
	SetShare(str,SHARE_SIZE);
}

CShareMem::~CShareMem(void)
{
	if(m_hFileMap)
		CloseHandle(m_hFileMap);
}

BOOL CShareMem::GetShare(PVOID pVoid, int iSize)
{
	if(!m_bOK || !m_hFileMap || !pVoid)
		return FALSE;

	memset(pVoid,0,iSize);

	if(iSize > SHARE_SIZE)
		iSize = SHARE_SIZE;

	PVOID pData = MapViewOfFile(m_hFileMap,FILE_MAP_READ,0,0,0);
	if(!pData)
		return FALSE;
	memcpy(pVoid,pData,iSize);
	UnmapViewOfFile(pData);

	return TRUE;
}

BOOL CShareMem::SetShare(PVOID pVoid, int iSize)
{
	if(!m_bOK || !m_hFileMap || !pVoid)
		return FALSE;

	if(iSize > SHARE_SIZE)
		iSize = SHARE_SIZE;

	PVOID pData = MapViewOfFile(m_hFileMap,FILE_MAP_WRITE,0,0,0);
	if(!pData)
		return FALSE;
	memcpy(pData,pVoid,iSize);
	UnmapViewOfFile(pData);

	return TRUE;
}