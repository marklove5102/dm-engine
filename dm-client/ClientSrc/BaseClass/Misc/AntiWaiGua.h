// #pragma once
// 
// #include "Global/Global.h"
// 
// class CAntiWaiGua
// {
// public:
// 	CAntiWaiGua(void);
// 	~CAntiWaiGua(void);
// 
// 	void FindClientsNumber(const char* gamemd5);
// 	void BeginThread();
// 	void ExitThread();
// 	void ExeFind();
// 	void GetPID();
// 	void GetProcFilePath(DWORD processID,char* FileName);
// 	bool IsWaiGua(){return s_bWaiGua || (m_ProcNum > 5);}
// 	void ClearState();
// 
// 	static DWORD WINAPI AntiWaiGuaThread(LPVOID param);
// 
// 	static bool s_bWaiGua;
// protected:
// 	int m_ProcNum;
// 	bool m_bThreadRunning;
// 	std::vector<DWORD> m_vecProcessIDs;
// 	char m_GameMD5[33];
// 
// 	HANDLE m_hWaiGuaThread;
// 	DWORD m_dwWaiGuaThreadId;
// 
// };
// 
// extern CAntiWaiGua g_AntiWaiGua;
