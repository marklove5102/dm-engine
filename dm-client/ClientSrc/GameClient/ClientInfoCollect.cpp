#include "ClientInfoCollect.h"

CClientInfoCollect::CClientInfoCollect(void)
{
	//在构造函数中进行初始化
	m_bSend = false;
	m_pfReportClientQos = NULL;
	m_pfClientQosInfoUpdate = NULL;

	m_hInstance = ::LoadLibrary("cliqos.dll");
	if(m_hInstance)
	{
		m_pfReportClientQos = (lpReportClientQos)::GetProcAddress(m_hInstance,"ReportClientQos");
		m_pfClientQosInfoUpdate = (lpClientQosInfoUpdate)::GetProcAddress(m_hInstance,"ClientQosInfoUpdate");	//更换玩家信息的函数
	}
}

CClientInfoCollect::~CClientInfoCollect(void)
{
	if (m_hInstance)
		FreeLibrary(m_hInstance);
}

int CClientInfoCollect::ReportClientQos(unsigned int gameid, unsigned int areaid, unsigned int groupid, unsigned int grade, const char* userID)
{
	if (!m_bSend)
	{
		if (m_pfReportClientQos)
		{
			m_bSend = true;
			return m_pfReportClientQos(gameid,areaid,groupid,grade,userID,1,1);
		}
	}
	else
	{
		if (m_pfClientQosInfoUpdate)
		{
			return m_pfClientQosInfoUpdate(gameid,areaid,groupid,grade,userID);
		}		
	}

	return -1;
}

int CClientInfoCollect::ClientQosInfoUpdate(unsigned int gameid, unsigned int areaid, unsigned int groupid, unsigned int grade, const char* userid)
{
	if (m_pfClientQosInfoUpdate)
	{
		return m_pfClientQosInfoUpdate(gameid,areaid,groupid,grade,userid);
	}

	return -1;
}