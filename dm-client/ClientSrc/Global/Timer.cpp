#include "Timer.h"
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string>
#include "GameControl/GameControl.h"
#include "WndClass/MainWnd/MainWnd.h"
#pragma warning(disable : 4996)

CTimer g_Timer;

#define FREQUENCY_RESAMPLE_RATE 200

CTimer::CTimer()
{
	Reset();
	mTickCount = 0;
}

CTimer::~CTimer()
{

}

// 取得精确时间计数
UINT64 CTimer::_GetCPUCount(void)
{
	__asm RDTSC
}

void CTimer::Reset()
{
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mStartTime);
	mStartTick = GetTickCount();
	mLastTime = 0;
	mQueryCount = 0;

	// Save the current process
	HANDLE mProc = GetCurrentProcess();

	// Get the current Affinity
	GetProcessAffinityMask(mProc, &mProcMask, &mSysMask);

	mThread = GetCurrentThread();
}
//-------------------------------------------------------------------------
unsigned long CTimer::GetMilliseconds()
{
	LARGE_INTEGER curTime;

	// Set affinity to the first core
	SetThreadAffinityMask(mThread, 1);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(mThread, mProcMask);

	// Resample the frequency
	mQueryCount++;
	if(mQueryCount == FREQUENCY_RESAMPLE_RATE)
	{
		mQueryCount = 0;
		QueryPerformanceFrequency(&mFrequency);
	}

	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

	// scale by 1000 for milliseconds
	unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - mStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{
		// We must keep the timer running forward :)
		LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
		mStartTime.QuadPart += adjust;
		newTime -= adjust;

		// Re-calculate milliseconds
		newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
	}

	// Record last time for adjust
	mLastTime = newTime;

	return newTicks;
}
//-------------------------------------------------------------------------
unsigned long CTimer::GetMicroseconds()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);
	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

	// get milliseconds to check against GetTickCount
	unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - mStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{
		// We must keep the timer running forward :)
		LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
		mStartTime.QuadPart += adjust;
		newTime -= adjust;
	}

	// Record last time for adjust
	mLastTime = newTime;

	// scale by 1000000 for microseconds
	unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

	return newMicro;
}

DWORD CTimer::GetCurTime()
{
	__time32_t tNow;
	_time32(&tNow);
	return (DWORD)tNow;
}

char* CTimer::GetDateTime()
{
	static char str[128];
	char tmp_d[64];
	char tmp_t[64];

	_tzset();
	_strdate(tmp_d);
	_strtime(tmp_t);
	sprintf(str,"%s\t%s",tmp_d,tmp_t);

	return str;
}

char* CTimer::GetDateTime(const char* fmt)
{
	static char str[128] = {0};
	tm*		when;
	__time32_t	now;
	_time32(&now);
	when = _localtime32(&now);
	if (when)
		strftime(str,128,fmt,when);

	return str;
}

//修正原来时间显示错误的问题，原来跟一个参数的处理一样的
char* CTimer::GetDateTime(const char* fmt,DWORD dwTime)
{
	static char str[128] = {0};

	tm*		when;
	__time32_t now = dwTime;
	when = _localtime32(&now);
	if (when)
		strftime(str,128,fmt,when);

	return str;
}

int CTimer::GetServerHours(__time32_t dwTime)
{
	__time32_t t_time;
	_time32(&t_time);
	t_time += g_dwServerTime;
	int iHours = (int)((t_time - dwTime) / 3600);
	return iHours;
}

DWORD CTimer::StrToTime(const char* s)
{
	char str[64] = {0};
	strcpy(str,s);

	tm t;
	memset(&t,0,sizeof(tm));

	char* szYear = strtok(str," ");
	if(NULL == szYear)
		return 0;

	char* szTime = strtok(NULL," ");

	szYear = strtok(szYear,"/");
	if(!szYear||strlen(szYear)<4) return 0;
	char* szMonth = strtok(NULL,"/");
	if(!szMonth||strlen(szMonth)<1) return 0;
	char* szDay = strtok(NULL,"/");
	if(!szDay||strlen(szDay)<1) return 0;

	t.tm_year = atoi(szYear)-1900;
	t.tm_mon = atoi(szMonth)-1;
	t.tm_mday = atoi(szDay);
	char* szHour = strtok(szTime,":");
	if(szHour)
		t.tm_hour = atoi(szHour);
	else
		t.tm_hour = 0;

	char* szMinu = strtok(NULL,":");
	if(szMinu)
		t.tm_min = atoi(szMinu);
	else
		t.tm_min = 0;

	char* szSec = strtok(NULL," ");
	if(szSec)
		t.tm_sec = atoi(szSec);
	else
		t.tm_sec = 0;

	return (DWORD)mktime(&t);
}

bool CTimer::SetTimer(DWORD dwWnd,WORD wTimerID,DWORD dwTime,WORD wLastTms)
{
	MTimer::iterator it = mTimer.find(wTimerID);
	if (it != mTimer.end()) return false;

	_STIMER tm(dwWnd,wTimerID,dwTime,wLastTms);
	mTimer[wTimerID] = tm;

	return true;
}

void CTimer::KillTimer(WORD wID)
{
	MTimer::iterator it = mTimer.find(wID);

	if (it != mTimer.end())
		mTimer.erase(it);
}

void CTimer::OnIdle()
{
	DWORD dwCurTime = GetTickCount();
	MTimer::iterator it = mTimer.begin();

	while (it != mTimer.end())
	{
		_STIMER& stm = it->second;
		if (dwCurTime - stm.dwCurTm >= stm.dwTime)
		{
			if (stm.wLastTimes != 0xFFFF)
				stm.wLastTimes--;

			g_pControl->Msg(MSG_WND_TIMER,stm.wTimerID);
			stm.dwCurTm = dwCurTime;

			if(stm.wLastTimes == 0) 
			{
				KillTimer(stm.wTimerID);
				return;//有删除,后面的留到下一帧来处理,不然后面的it++会出错
			}

			if (mTimer.size() <= 0) return;

		}

		it++;
	}
}