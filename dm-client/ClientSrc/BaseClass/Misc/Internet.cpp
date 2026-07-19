#include "Global/Global.h"
#include "Global/Interface/StreamInterface.h"
#include "internet.h"
#include "global/GlobalMsg.h"
#include "Global/DebugTry.h"

#include <process.h>

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

#define  MAX_INET_READ_BUFFER  1024 * 32

CInternet  g_HttpMgr;


HINTERNET  CInternet::hRootHandle = NULL;
HINTERNET  CInternet::hOpenUrlHandle = NULL;
HINTERNET  CInternet::hRequest = NULL;
CRITICAL_SECTION CInternet::m_sec;
BOOL       CInternet::m_bRun = FALSE;
char*      CInternet::m_BufArray = NULL;

HANDLE		CInternet::m_hEvent = NULL;
int			CInternet::m_iNumThreadsRunning = 0;

CInternet::CInternet(void)
{
	InitializeCriticalSection(&m_sec);
	m_hEvent = CreateEvent(NULL,0,0,NULL);

	m_bRun	= FALSE;
	hRootHandle			= NULL;
	hOpenUrlHandle		= NULL;
	hRequest			= NULL;
	m_hThread			= NULL;

	m_BufArray          = new char[MAX_INET_READ_BUFFER];
	memset(m_BufArray,0,MAX_INET_READ_BUFFER);

	InternetOpen();
}

CInternet::~CInternet(void)
{
	bool done = false;
	while (!done)
	{
		Sleep(50);

		if (m_iNumThreadsRunning == 0)
			done = true;
	}


	SAFE_DELETE_ARRAY(m_BufArray);

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hOpenUrlHandle);
	InternetCloseHandle(hRootHandle);

	DeleteCriticalSection(&m_sec);
	CloseHandle(m_hEvent);
}

BOOL CInternet::InternetOpen()
{
	hRootHandle = ::InternetOpen(NULL,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	if(hRootHandle == NULL)
		return false;

	return true;
}

BOOL CInternet::OpenUrl(const char * lpszUrl,DWORD dwFlags)
{
    if(hRootHandle == NULL)
		return false;
	hOpenUrlHandle = ::InternetOpenUrl(hRootHandle,lpszUrl,NULL,0,dwFlags,NULL);
	if(hOpenUrlHandle == NULL)
		return false;

    DWORD dwSize = 4;
	DWORD code;
    DWORD ss = 0;    	
	if(!HttpQueryInfo(hOpenUrlHandle,HTTP_QUERY_FLAG_NUMBER|HTTP_QUERY_STATUS_CODE,&code,&dwSize,&ss))
		return false;

	if(code != HTTP_STATUS_OK)
		return false;

	return true;
}

BOOL CInternet::InternetConnect(const char * strServerName)
{
	hOpenUrlHandle = ::InternetConnect(hRootHandle,strServerName,INTERNET_DEFAULT_HTTP_PORT,
		NULL,NULL,INTERNET_SERVICE_HTTP,0,NULL);
	if(hOpenUrlHandle == NULL)
		return false;
	return true;
}

BOOL CInternet::OpenRequest(const char * strVerb,const char * strObjName,const char * strReferer,DWORD dwFlags)
{
	hRequest = ::HttpOpenRequest(hOpenUrlHandle,strVerb,strObjName,NULL,strReferer,NULL,dwFlags,NULL);
	if(hRequest == NULL)
		return false;
	return true;
}

BOOL CInternet::InternetGet(const char * strUrl,int iServer)
{	
	param.strUrl	= string(strUrl);
	//param.strUrl    = "http://127.0.0.1/bangzhu.txt";
	param.iServer	= iServer;
	DWORD id = 0;
    m_hThread = CreateThread(NULL,0,ReadThread,this,NULL,&id);
	WaitForSingleObject(m_hEvent, INFINITE);
	return TRUE;
}

DWORD WINAPI CInternet::ReadThread(LPVOID param)
{
	CSeException::Install_Handler();
	Inet_Param p = *(Inet_Param*)(param);

	InterlockedIncrement((LONG*)&m_iNumThreadsRunning);

	SetEvent(m_hEvent);

	EnterCriticalSection(&m_sec);
	int iRet = 0;

	TRY_BEGIN
		if(!InternetOpen())
		{
			SendMessage(g_hWnd,WM_INTERNET_ERROR,p.iServer,0);
			iRet =  1;
			goto endlbl;
		}
		if(!OpenUrl(p.strUrl.c_str(),0))
		{
			SendMessage(g_hWnd,WM_INTERNET_ERROR,p.iServer,0);
			iRet =  2;
			goto endlbl;
		}

		ULONG nLen = 0;
		if(!::InternetReadFile(hOpenUrlHandle,m_BufArray,MAX_INET_READ_BUFFER,&nLen))
		{
			SendMessage(g_hWnd,WM_INTERNET_ERROR,p.iServer,0);
			iRet =  3;
			goto endlbl;
		}

		if(nLen == MAX_INET_READ_BUFFER)
			m_BufArray[nLen - 1] = 0;
		else
			m_BufArray[nLen] = 0;

		SendMessage(g_hWnd,WM_ADDMSG,(WPARAM)m_BufArray,p.iServer);
	TRY_END
endlbl:
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hOpenUrlHandle);
		InternetCloseHandle(hRootHandle);
		LeaveCriticalSection(&m_sec);
	}


	InterlockedDecrement((LONG*)&m_iNumThreadsRunning);
	return iRet;
}



BOOL CInternet::SendHttpData(LPCTSTR str)
{
	string URL = g_pStreamMgr->GetGameStr("FeedBack");
	if(URL.size() < 5)
		return FALSE;

	string FileName = g_pStreamMgr->GetGameStr("FDFileName");
	if(FileName.size() < 5)
		return FALSE;

	PostData(URL.c_str(),FileName.c_str(),str);

	return TRUE;
}

BOOL CInternet::PostData(const char* URL,const char* FName,LPCTSTR str,int port)
{
	param.strUrl	= URL;
	param.strFile   = FName;
	param.strCNT    = str;
	param.iServer	= 0;
	param.iPort		= port;	
	DWORD id;
	m_hThread = CreateThread(NULL,0,PostThread,this,NULL,&id);
	WaitForSingleObject(m_hEvent, INFINITE);

	return TRUE;
}

DWORD WINAPI CInternet::PostThread(LPVOID param)
{
	CSeException::Install_Handler();
	Inet_Param p = *(Inet_Param*)(param);

	InterlockedIncrement((LONG*)&m_iNumThreadsRunning);

	SetEvent(m_hEvent);

	EnterCriticalSection(&m_sec);

	HINTERNET hInter1 = NULL;
	HINTERNET hInter2 = NULL;
	HINTERNET hInter3 = NULL;
	int iRet = 0;

	TRY_BEGIN
		hInter1 = ::InternetOpen("",0 ,NULL,NULL,0);
		if(!hInter1) 
		{
			iRet = 1;
			goto enddl;
		}

		hInter2 = ::InternetConnect(hInter1,p.strUrl.c_str(),p.iPort,NULL,NULL,INTERNET_SERVICE_HTTP,0,1);
		if(!hInter2)
		{
			iRet = 2;
			goto enddl;
		}

		hInter3 = ::HttpOpenRequest(hInter2,"POST", p.strFile.c_str(),"HTTP/1.0", NULL, NULL,INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE,1);
		if(!hInter3)
		{
			iRet = 3;
			goto enddl;
		}

		strcpy(m_BufArray,p.strCNT.c_str());

		const char *strHeaders = "Accept:*/*\r\n""Content-Type:application/x-www-form-urlencoded\r\n";
		if(!::HttpSendRequest(hInter3,strHeaders,strlen(strHeaders),m_BufArray,strlen(m_BufArray))) 
		{			
			iRet = 4;
			goto enddl;
		}
	TRY_END
enddl:
	{
		InternetCloseHandle(hInter3);
		InternetCloseHandle(hInter2);
		InternetCloseHandle(hInter1);

		LeaveCriticalSection(&m_sec);		
	}


	InterlockedDecrement((LONG*)&m_iNumThreadsRunning);

	return iRet;
}
