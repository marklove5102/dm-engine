#pragma once

#define SDAPI __stdcall

#include "RSSDKdefine.h"

class IRSSDK
{
public:
	virtual bool SDAPI SendQuestion(int gametype, int areacode, const char* szIDType, const char* szUserID, 
				const char* szQuestion, int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI JudgeAnswer(int gametype, int areacode, const char* szQuestionID, const char* szPoint,
								int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual int SDAPI SetNetParam(char* szHostName, char* szServerPort = NULL, 
								char* szKey = NULL, int nTimeOut = 2) = 0;
	virtual bool SDAPI SendQuestionEx(int gametype, int areacode, const char* szIDType, const char* szUserID, 
		const char* szQuestion,	int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI SendQuestionNew(int gametype, int areacode, const char* szIDType, const char* szUserID, 
		const char* szQuestion,	const char* szUserLogIdx, int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI RecvAnswer(int gametype, int areacode, const char* szUserLogIdx, const char* szAnsID,
		int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
	virtual bool SDAPI HotQuestionTopN(int gametype, int areacode, const char* szIDType, const char* szUserID,
		int &nRetCommand, char* szRetContent, int &nRetContentLen) = 0;
};

extern IRSSDK * SDAPI CreateRSClient(void);