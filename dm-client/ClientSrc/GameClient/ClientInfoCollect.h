#pragma once
#include "Global/Global.h"


typedef int (__stdcall *lpReportClientQos)(unsigned int gameid, unsigned int areaid, unsigned int groupid, unsigned int grade, const char* userID, unsigned int clientType,unsigned int invokeType);
typedef int (__stdcall *lpClientQosInfoUpdate)(unsigned int gameid, unsigned int areaid, unsigned int groupid, unsigned int grade, const char* userid);

class CClientInfoCollect
{
public:
	CClientInfoCollect(void);
	~CClientInfoCollect(void);
public:
	int ReportClientQos(unsigned int gameid, unsigned int areaid, unsigned int groupid, unsigned int grade, const char* userID);
	int ClientQosInfoUpdate(unsigned int gameid, unsigned int areaid, unsigned int groupid, unsigned int grade, const char* userid);
protected:
	HINSTANCE m_hInstance;
	lpReportClientQos		m_pfReportClientQos;
	lpClientQosInfoUpdate	m_pfClientQosInfoUpdate;
	bool m_bSend;
};
