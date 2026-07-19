//#include "AntiWaiGua.h"
//#include "Global/md5.h"
//#include <TlHelp32.h>
//#include "Psapi.h"
//#include <fstream>
//
//#pragma comment(lib,"Psapi.Lib")
//
//bool CAntiWaiGua::s_bWaiGua = false;
//
//CAntiWaiGua::CAntiWaiGua(void):
//m_dwWaiGuaThreadId(0),
//m_bThreadRunning(false),
//m_hWaiGuaThread(NULL),
//m_ProcNum(0)
//{
//	memset(m_GameMD5,0,sizeof(m_GameMD5));
//}
//
//CAntiWaiGua::~CAntiWaiGua(void)
//{
//	if(m_hWaiGuaThread)
//	{
//		WaitForSingleObject(m_hWaiGuaThread,INFINITE);
//		CloseHandle(m_hWaiGuaThread);
//	}
//}
//
//void CAntiWaiGua::FindClientsNumber(const char* gamemd5)
//{
//	if(m_bThreadRunning)
//		return;
//
//	strcpy(m_GameMD5,gamemd5);
//
//	if (m_hWaiGuaThread != NULL)
//	{
//		CloseHandle(m_hWaiGuaThread);
//		m_hWaiGuaThread = 0;
//	}
//
//	m_hWaiGuaThread = CreateThread(NULL,0,CAntiWaiGua::AntiWaiGuaThread,(DWORD *)this,0,&m_dwWaiGuaThreadId);
//}
//
//void CAntiWaiGua::BeginThread()
//{
//	m_bThreadRunning = true;
//}
//
//void CAntiWaiGua::ExitThread()
//{
//	if (m_ProcNum > 5)
//	{
//		s_bWaiGua = true;
//	}
//
//	m_bThreadRunning = false;
//}
//void CAntiWaiGua::ClearState()
//{ 
//	s_bWaiGua = false;
//
//	if (m_bThreadRunning)
//	{
//		TerminateThread(m_hWaiGuaThread,0);
//		m_bThreadRunning = false;
//
//		CloseHandle(m_hWaiGuaThread);
//		m_hWaiGuaThread = 0;
//	}
//	m_ProcNum = 0;
//}
//
//void CAntiWaiGua::ExeFind()
//{
//	GetPID();
//
//	m_ProcNum = 0;
//
//	for (std::vector<DWORD>::iterator itr = m_vecProcessIDs.begin(); itr != m_vecProcessIDs.end(); ++itr)
//	{
//		char szProcessName[MAX_PATH] = {'\0'};
//
//		GetProcFilePath(*itr,szProcessName);
//		
//		char szMD5[33] = {0};
//		MD5_CTX CMD5;
//		CMD5.MD5File32(szMD5,szProcessName);
//
//		if (strlen(szMD5) > 0 && strcmp(szMD5,m_GameMD5) == 0)
//		{
//			m_ProcNum++;
//		}
//
//	}
//	
//}
//
//DWORD WINAPI CAntiWaiGua::AntiWaiGuaThread(LPVOID param)
//{
//	int ClientsNumber = 0;
//
//	CAntiWaiGua* agent = (CAntiWaiGua*)param;
//
//	if (agent)
//	{
//		agent->BeginThread();
//
//		agent->ExeFind();
//
//		agent->ExitThread();
//	}
//
//	return 0;
//}
//
//void CAntiWaiGua::GetPID()
//{     
//	m_vecProcessIDs.clear();
//
//	HANDLE hHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//
//	if (INVALID_HANDLE_VALUE !=hHandle)
//	{
//		PROCESSENTRY32 stEntry;
//
//		stEntry.dwSize = sizeof(PROCESSENTRY32);
//
//		if(Process32First(hHandle, &stEntry))
//		{
//			do
//			{
//				m_vecProcessIDs.push_back(stEntry.th32ProcessID);
//			}while(Process32Next(hHandle, &stEntry));
//		}
//		CloseHandle(hHandle);
//	}
//}
//void CAntiWaiGua::GetProcFilePath(DWORD processID,char* FileName)
//{
//	char szProcessName[MAX_PATH] = "unknown";   
//
//	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, processID);   
//
//	if( hProcess)   
//	{   
//		HMODULE hMod;   
//		DWORD cbNeeded;   
//
//		if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))   
//		{   
//			GetModuleFileNameEx(hProcess, hMod, szProcessName, sizeof(szProcessName));   
//		}   
//	}   
//
//	CloseHandle( hProcess);   
//
//	strcpy(FileName,szProcessName);
//}
//
//CAntiWaiGua g_AntiWaiGua;