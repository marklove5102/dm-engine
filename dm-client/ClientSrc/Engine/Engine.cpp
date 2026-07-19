// Engine.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "math.h"
#include "Global/Interface/Callbackinterface.h"
#include "Global/Interface/AudioInterface.h"
#include "FontD3D.h"
#include "GraphicD3D.h"
#include "TexManager.h"
#include "Weather.h"
#include "Light.h"
#include "BaseClass/Audio/Audio.h"
#include "EngineInstance.h"
#include "Stream.h"


#pragma comment(lib,"ijl15l.lib")


#define	    PI					3.14159f
float	    g_fSin[360];
float       g_fCos[360];

CCallBackInterface * g_pCallBack = NULL;


#ifdef _MANAGED
#pragma managed(push, off)
#endif


CEngineInstance *g_pEngine = NULL;
CTexManager * g_pETexMgr = NULL;
CGraphicD3D * g_pEGfx = NULL;
CFontD3D * g_pEFont = NULL;
CLight * g_pELight = NULL;;
CWeather * g_pEWeather = NULL;
CAudio *g_pEAudio	= NULL;
CStreamManager* g_pEStreamMgr = NULL;



void output_debug(char *format,...)
{
	char str[8 * 1024] = {0};
	va_list argptr;

	va_start(argptr,format);
	vsprintf(str,format,argptr);
	va_end(argptr);

	OutputDebugString(str);
}

//像vector取下标[]越界等不会抛出任何异常,直接crash,但可以_set_invalid_parameter_handler,自行处理参数非法的情况
void myInvalidParameterHandler(const wchar_t* expression,const wchar_t* function,const wchar_t* file,unsigned int line,uintptr_t pReserved)
{
	/////////////////
	if(expression && function && file)
	{
		wchar_t wstr[512] = {0};
		char tmp[1024] = {0};
		swprintf(wstr,L"Invalid parameter detected in function %s.File: %s Line: %d,Expression: %s", function, file, line,expression);
		int iLen = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wstr,wcslen(wstr),tmp,1023,NULL,NULL);
		throw exception(tmp);
	}
	else
	{
		throw exception("invalid_parameter_handler:参数非法");
	}
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		output_debug("EngineLoad");
		// 预生成360度的con和sin值
		for(int i = 0;i < 360;i++)
		{
			g_fCos[i] = (float)cos(i*PI/180);
			g_fSin[i] = (float)sin(i*PI/180);
		}

		_set_invalid_parameter_handler(myInvalidParameterHandler);

	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		SAFE_DELETE(g_pEWeather);
		SAFE_DELETE(g_pELight);		
		SAFE_DELETE(g_pEFont);
		SAFE_DELETE(g_pEGfx);
		SAFE_DELETE(g_pETexMgr);
		SAFE_DELETE(g_pEAudio);	 
		SAFE_DELETE(g_pEStreamMgr);	 

		SAFE_DELETE(g_pEngine);	 
		g_pCallBack = NULL;
	}

	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

CEngineInterface* CreateInstance(CCallBackInterface * pCallBack)
{
	output_debug("CreateInstance_Start");
	g_pCallBack = pCallBack;
	if (g_pEngine == NULL)
	{
		g_pEngine = new CEngineInstance();

		if (g_pEStreamMgr == NULL)
		{
			g_pEStreamMgr = new CStreamManager();
		}
		if(g_pETexMgr == NULL)
		{
			g_pETexMgr = new CTexManager();
		}
		if(g_pEGfx == NULL)
		{
			g_pEGfx = new CGraphicD3D();
		}
		if(g_pEFont == NULL)
		{
			g_pEFont = new CFontD3D();
		}
		if(g_pELight == NULL)
		{
			g_pELight = new CLight();
		}
		if(g_pEWeather == NULL)
		{
			g_pEWeather = new CWeather();
		}
		if(g_pEAudio == NULL)
		{
			g_pEAudio = new CAudio();
		}
	}
	output_debug("CreateInstance_End");

	return g_pEngine;
}

void ReleaseEngine()
{
	SAFE_DELETE(g_pEWeather);
	SAFE_DELETE(g_pELight);		
	SAFE_DELETE(g_pEFont);
	SAFE_DELETE(g_pEGfx);
	SAFE_DELETE(g_pETexMgr);
	SAFE_DELETE(g_pEAudio);	 
	SAFE_DELETE(g_pEStreamMgr);	 

	SAFE_DELETE(g_pEngine);	 
	g_pCallBack = NULL;
}

