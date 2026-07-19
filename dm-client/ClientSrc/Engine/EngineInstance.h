#pragma once

#include "Global/Interface/EngineInterface.h"

class CEngineInstance:public CEngineInterface
{
public:
	CEngineInstance(void);
public:
	~CEngineInstance(void);


	CAudioInterface * GetAudioInterface();
	CLightInterface * GetLightInterface();
	CFontInterface * GetFontInterface();
	CGraphicInterface * GetGraphicInterface();
	CTexManagerInterface * GetTexMgrInterface();
	CWeatherInterface * GetWeatherInterface();
	CStreamManagerInterface* GetStreamManagerInterface();
};
