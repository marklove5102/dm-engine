#include "Registry.h"
#include "Global/Global.h"
#include "tchar.h"
#include "Global/Interface/StreamInterface.h"

#include <time.h>


//注册表项目检查
static char* KEY_COMMON_PATH = "SoftWare\\snda";
static char* COMMON_PATH = "C:\\Program Files\\Common Files\\snda shared";

static char* KEY_PATH = "SoftWare\\snda\\ws";



bool ReadRegString(char* szKeyName,char* szValue,char* szKeyPath,HKEY hOpenKey)
{
	bool bRet = false;

	HKEY hKey;
	if (ERROR_SUCCESS == RegOpenKey(hOpenKey,szKeyPath,&hKey))
	{
		TCHAR szBuffer[1024] = {0};

		DWORD dwSize = 1024;
		DWORD dwType = 0;
		if (ERROR_SUCCESS == RegQueryValueEx(hKey, szKeyName,0,&dwType,(BYTE*)szBuffer,&dwSize))
		{
			if(dwType == REG_SZ && dwSize > 0)
			{
				strcpy(szValue,szBuffer);
				bRet = true;
			}
		}
		RegCloseKey(hKey);
	}
	return bRet;
}

bool WriteRegString(char* szKeyName,char* szValue,char* szKeyPath,HKEY hOpenKey)
{
	HKEY hKey;
	if (ERROR_SUCCESS != RegCreateKey(hOpenKey,szKeyPath,&hKey))
		return false;

	TCHAR* szBuffer = szValue;

	DWORD cbValue = ((DWORD)_tcslen(szBuffer)+1) * sizeof(TCHAR);
	RegSetValueEx(hKey, szKeyName, 0, REG_SZ, (BYTE*)szBuffer, cbValue);

	RegCloseKey(hKey);

	return true;
}

bool EnumRegString(int iIndex,char* szValue,char* szKeyPath,HKEY hOpenKey)
{
	bool bRet = false;

	HKEY hkRoot = NULL,hSubKey = NULL; //定义注册表根关键字及子关键字
	char ValueName[MAX_PATH] = {0};
	unsigned char DataValue[MAX_PATH] = {0};
	unsigned long cbValueName = MAX_PATH;
	unsigned long cbDataValue = MAX_PATH;

	DWORD dwType;
	//打开注册表根关键字
	if(RegOpenKey(hOpenKey,NULL,&hkRoot)==ERROR_SUCCESS)
	{
		//打开子关键字
		if(RegOpenKeyEx(hkRoot,
			szKeyPath,
			iIndex,
			KEY_ALL_ACCESS,
			&hSubKey)==ERROR_SUCCESS)
		{
			//读取注册表，获取默认浏览器的命令行     
			RegEnumValue(hSubKey, 
				0,
				ValueName,
				&cbValueName,
				NULL,
				&dwType,
				DataValue,
				&cbDataValue);

			strcpy(szValue,(char *)DataValue);
			bRet = true;
			//关闭注册表
			RegCloseKey(hSubKey);
		}

		RegCloseKey(hkRoot);
	}

	return bRet;
}


void WriteCommonPath()
{
	char szCommonPath[256]={0};
	bool bRet=false;
    bRet = ReadRegString("CommonPath",szCommonPath,KEY_COMMON_PATH);
	if(!bRet || strcmp(szCommonPath,"snda shared")<=0)
	{
		WriteRegString("CommonPath",COMMON_PATH,KEY_COMMON_PATH);
	}

}

void RegistryAsRun()
{
	WriteCommonPath();

	TCHAR szModulePath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szModulePath,MAX_PATH);

	WriteRegString("MainProg",szModulePath); //可执行程序路径

	TCHAR szDriver[_MAX_DRIVE] = {0};
	TCHAR szDir[_MAX_DIR] = {0};
	TCHAR szFileName[_MAX_FNAME] = {0};
	TCHAR szExt[_MAX_EXT] = {0};

	TCHAR szPath[MAX_PATH] = {0};
	_fullpath(szPath,"../",MAX_PATH);
	WriteRegString("Path",szPath);          //传世路径

	_splitpath(szPath,szDriver,szDir,szFileName,szExt);

	TCHAR szLoader[MAX_PATH] = {0};
	_makepath(szLoader,szDriver,szDir,"ws","exe");
	WriteRegString("Loader",szLoader);     //引导程序路径

	string URL = g_pStreamMgr->GetWebsite("Website");
	if(URL.empty())
		URL = "http://ws.sdo.com";
	WriteRegString("HomePage",(char*)URL.c_str()); //主页

	TCHAR szVersionPath[MAX_PATH];
	_makepath(szVersionPath,szDriver,szDir,"Version","dat");

	char szCurrentVersionName[256] = {0};
	::GetPrivateProfileString("info","Version","",szCurrentVersionName,256,szVersionPath);

	char* szL = strchr(szCurrentVersionName,'\"');
	if(szL != NULL)
	{
		char* szR = strchr(szL + 1,'\"');
		if(szR != NULL) *szR = '\0';
		WriteRegString("CurrentVersionName",szL + 1);  //版本名字
	}

	WriteRegString("CurrentVersion",g_strVersion);  //版本号

	//char szDiscription[256] = {0};
	//g_pStream->LoadWebSiteConfigString("Description",szDiscription,256,"");
	//WriteRegString("Description",szDiscription);// 当前版本的描述

	char szLastRunTime[256] = {0};
	time_t tNow;
	time(&tNow);
	strftime(szLastRunTime,256,"%Y-%m-%d %H:%M:%S",localtime(&tNow));
	WriteRegString("LastRunTime",szLastRunTime);      //最近运行时间
}
