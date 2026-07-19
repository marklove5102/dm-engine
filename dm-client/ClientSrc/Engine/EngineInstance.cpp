#include "StdAfx.h"
#include "EngineInstance.h"
#include "FontD3D.h"
#include "GraphicD3D.h"
#include "TexManager.h"
#include "Weather.h"
#include "Light.h"
#include "BaseClass/Audio/Audio.h"
#include "Stream.h"

CEngineInstance::CEngineInstance(void)
{
}

CEngineInstance::~CEngineInstance(void)
{
}

CAudioInterface * CEngineInstance::GetAudioInterface()
{
	return g_pEAudio;
}

CLightInterface * CEngineInstance::GetLightInterface()
{
	return g_pELight;
}

CFontInterface * CEngineInstance::GetFontInterface()
{
	return g_pEFont;
}

CGraphicInterface * CEngineInstance::GetGraphicInterface()
{
	return g_pEGfx;
}

CTexManagerInterface * CEngineInstance::GetTexMgrInterface()
{
	return g_pETexMgr;
}

CWeatherInterface * CEngineInstance::GetWeatherInterface()
{
	return g_pEWeather;
}

CStreamManagerInterface* CEngineInstance::GetStreamManagerInterface()
{
	return g_pEStreamMgr;
}