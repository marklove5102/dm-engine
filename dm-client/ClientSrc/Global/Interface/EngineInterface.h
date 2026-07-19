#pragma once

#include "../Global/Globaldefine.h"

class CAudioInterface;
class CLightInterface;
class CFontInterface;
class CGraphicInterface;
class CTexManagerInterface;
class CWeatherInterface;
class CStreamManagerInterface;

class CEngineInterface
{
public:
	virtual ~CEngineInterface(){}

    virtual CAudioInterface * GetAudioInterface() = 0;
	virtual CLightInterface * GetLightInterface() = 0;
	virtual CFontInterface * GetFontInterface() = 0;
	virtual CGraphicInterface * GetGraphicInterface() = 0;
	virtual CTexManagerInterface * GetTexMgrInterface() = 0;
	virtual CWeatherInterface * GetWeatherInterface() = 0;
	virtual CStreamManagerInterface* GetStreamManagerInterface() = 0;
};
