#include "stdafx.h"
#include "Stream.h"

//文件管理类//
#define MAX_STREAM_LEN   256

char* CStreamManager::GetConfigStr(const char* strKey,const char* strDefault)
{
	static char szReturn[MAX_STREAM_LEN];
	int r = GetPrivateProfileString(SECTION_NAME,strKey,strDefault,
		szReturn,MAX_STREAM_LEN,m_szConfigFile);
	return szReturn;
}

int CStreamManager::GetConfigInt(const char* strKey,int iDefault)
{
	int iRet = (int)GetPrivateProfileInt(SECTION_NAME,strKey,iDefault,m_szConfigFile);
	return iRet;
}

void CStreamManager::SetConfigStr(const char* strKey,const char * strValue)
{
	WritePrivateProfileString(SECTION_NAME,strKey,strValue,m_szConfigFile);
}

void CStreamManager::SetConfigInt(const char* strKey,int iValue)
{
	char str[32] = {0};
	sprintf(str,"%d",iValue);
	WritePrivateProfileString(SECTION_NAME,strKey,str,m_szConfigFile);
}

char* CStreamManager::GetOtherConfigStr(const char* strKey,const char* strDefault)
{
	static char szReturn[MAX_STREAM_LEN];
	int r = GetPrivateProfileString(SECTION_NAME,strKey,strDefault,
		szReturn,MAX_STREAM_LEN,m_szOtherConfigFile);
	return szReturn;
}

int CStreamManager::GetOtherConfigInt(const char* strKey,int iDefault)
{
	int iRet = (int)GetPrivateProfileInt(SECTION_NAME,strKey,iDefault,m_szOtherConfigFile);
	return iRet;
}

void CStreamManager::SetOtherConfigStr(const char* strKey,const char * strValue)
{
	WritePrivateProfileString(SECTION_NAME,strKey,strValue,m_szOtherConfigFile);
}

void CStreamManager::SetOtherConfigInt(const char* strKey,int iValue)
{
	char str[32] = {0};
	sprintf(str,"%d",iValue);
	WritePrivateProfileString(SECTION_NAME,strKey,str,m_szOtherConfigFile);
}

char* CStreamManager::GetGameStr(const char* strKey,const char* strDefault)
{
	static char szReturn[MAX_STREAM_LEN];
	int r = GetPrivateProfileString(SECTION_NAME,strKey,strDefault,
		szReturn,MAX_STREAM_LEN,m_szGameFile);
	return szReturn;
}

void CStreamManager::SetGameStr(const char* strKey,const char* strValue)
{
	WritePrivateProfileString(SECTION_NAME,strKey,strValue,m_szGameFile);
}

int CStreamManager::GetGameInt(const char* strKey,int iDefault)
{
	int iRet = (int)GetPrivateProfileInt(SECTION_NAME,strKey,iDefault,m_szGameFile);
	return iRet;
}

void CStreamManager::SetGameInt(const char* strKey,int iValue)
{
	char str[32] = {0};
	sprintf(str,"%d",iValue);
	WritePrivateProfileString(SECTION_NAME,strKey,str,m_szGameFile);
}

char* CStreamManager::GetWebsite(const char* strKey,const char* strDefault)
{
	static char szReturn[MAX_STREAM_LEN];
	int r = GetPrivateProfileString(SECTION_NAME,strKey,strDefault,
		szReturn,MAX_STREAM_LEN,m_szWebsiteFile);
	return szReturn;
}

void  CStreamManager::SetSectionPath(const char* szSection,const char* path)
{
	strcpy(m_szINIFile,path);
	strcpy(SECTION_INI,szSection);
}

char* CStreamManager::GetINIStr(const char* strKey,const char* strDefault)
{
	static char szReturn[MAX_STREAM_LEN];
	int r = GetPrivateProfileString(SECTION_INI,strKey,strDefault,
		szReturn,MAX_STREAM_LEN,m_szINIFile);
	return szReturn;
}

int   CStreamManager::GetINIInt(const char* strKey,int iDefault)
{
	int iRet = (int)GetPrivateProfileInt(SECTION_INI,strKey,iDefault,m_szINIFile);
	return iRet;
}