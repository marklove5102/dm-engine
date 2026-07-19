#pragma once
#include "Global/Global.h"
#include <wininet.h>

enum SERVER_NO
{
	INET_ACTIVITY = 0	,
	INET_MAIN,
	INET_INVEST,
	INET_REPLY,
	INET_RADIO,
	INET_LOGO,
};

class CInternet
{
	struct Inet_Param
	{
		string strUrl;
		string strFile;
		string strCNT;
		int iServer;
		int iPort;

		Inet_Param()
		{
			strUrl.clear();
			strFile.clear();
			strCNT.clear();
			iServer = 0;
			iPort = 80;
		}
	};
public:
	CInternet(void);
	~CInternet(void);

	static BOOL InternetOpen();
	static BOOL OpenUrl(const char * lpszUrl,unsigned long dwFlags);

    static BOOL InternetConnect(const char * strServerName);
	static BOOL OpenRequest(const char * strVerb,const char * strObjName,const char * strReferer,unsigned long dwFlags);

	// 这个函数是上面的封装，调用的时候调这个就可以了。
	BOOL   InternetGet(const char * strUrl,int iServer);

    static HINTERNET	hRootHandle;
    static HINTERNET	hOpenUrlHandle;
	static HINTERNET	hRequest;

	Inet_Param	param;
	HANDLE		m_hThread;	

	static HANDLE m_hEvent;
	static int	m_iNumThreadsRunning;

	static char* m_BufArray; //缓冲数组

	static CRITICAL_SECTION m_sec;
	static BOOL				m_bRun;

	BOOL SendHttpData(LPCTSTR str);
	BOOL PostData(const char* URL,const char* FName,LPCTSTR str,int port = 80);

	static DWORD WINAPI ReadThread(LPVOID param);
	static DWORD WINAPI PostThread(LPVOID param); //Post发送线程
};

extern CInternet  g_HttpMgr;