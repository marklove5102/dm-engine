#pragma once

#include "Global/Globaldefine.h"
#include <basetsd.h>

class CTimer
{
	struct _STIMER
	{
		DWORD dwMsgWnd;
		WORD  wTimerID;
		DWORD dwCurTm;
		DWORD dwTime;
		WORD  wLastTimes;
		_STIMER()
		{
			dwCurTm = GetTickCount();
			dwMsgWnd = 0;
			wTimerID = 0;
			dwTime = 0;
			wLastTimes = 1;
		}

		_STIMER(DWORD msgwnd,WORD tmID,DWORD tm,WORD wLTs = 1)
		{
			dwCurTm = GetTickCount();
			dwMsgWnd = msgwnd;
			wTimerID = tmID;
			dwTime = tm;
			wLastTimes = wLTs;
		}
	};
	typedef std::map<WORD,_STIMER> MTimer;
private:
	DWORD mStartTick;
	LONGLONG mLastTime;
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mFrequency;

	DWORD mProcMask;
	DWORD mSysMask;

	HANDLE mThread;

	DWORD mQueryCount;

	DWORD mTickCount;
	MTimer mTimer;
public:
	CTimer();
	~CTimer();

	// overrides standard methods
	bool SetTimer(DWORD dwWnd,WORD wTimerID,DWORD dwTime,WORD wLastTms = 1);
	void KillTimer(WORD wID);
	void OnIdle();
	void Reset();
	DWORD GetMilliseconds();
	DWORD GetMicroseconds();

	UINT64 _GetCPUCount(void);
	DWORD  GetCurTime(); //time_t格式的当前时间
	char*  GetDateTime();
	char*  GetDateTime(const char* fmt);
	char*  GetDateTime(const char* fmt,DWORD dwTime);
	int    GetServerHours(__time32_t dwTime);
	DWORD  StrToTime(const char* str);

};

extern CTimer g_Timer;
