#pragma once


#define INITGUID
#include "../../Global/Interface/AudioInterface.h"

#include <dmusicc.h>
#include <dmusici.h>
#include <cguid.h>
#include <vector>

#define MAX_AUDIO_NUM 60


#ifndef AUDIO_NAMES_LENGTH
#define AUDIO_NAMES_LENGTH	15
#endif

class CSoundManager;
class CSound;  
class CStreamingSound;

extern int g_Audio_Rain;
extern int g_Audio_Blood;
extern UINT g_nRand;
//extern UINT *g_Midi;

///////////////////////////////////////////////////////////////////config file 

#ifndef AUDIO_NAMES_LENGTH
#define AUDIO_NAMES_LENGTH	63
#endif

#ifndef AUDIO_MAX_VOLUME
#define AUDIO_MAX_VOLUME	100
#endif



class  CAudioEnviroment
{
	friend class CAudio;
public:
	CAudioEnviroment()
	{

	}
	~CAudioEnviroment()
	{
		clear();
	}

	void add(int iEnvType,int iTx,int iTy)
	{
		m_vItems.push_back(sAudioEnviroment_t(iTx,iTy,iEnvType));
	}
	void clear()
	{
		m_vItems.clear();
	}
	void SetSelfPos(int ix,int iy)
	{
		iSelfX = ix;
		iSelfY = iy;
	}
	int GetSize()
	{
		return (int)m_vItems.size();
	}
protected:
	int  iSelfX,iSelfY;
	std::vector<sAudioEnviroment_t> m_vItems;		//存储每次标绘时获得的所有地表环境音效项
};

struct sAudioName_t
{
	char cName[AUDIO_NAMES_LENGTH+1];
};

struct sAudioConfigManager_t;			//配置文件管理



//音效和背景音乐播放的类
class  CAudio :public CAudioInterface 
{ 
public:
	CAudio(void);
	~CAudio(void);
public:
	void TimerHandler();
	
	UINT& GetRand();
	int& GetAudioRain();
	int& GetAudioBlood();

	//根据给定的类型和索引获得名字，以及该名字所代表的优先级
	char *GetName(int iType,UINT iIndex,int *iPriority = NULL,int *iVol=NULL);
	char *GetNameRandon(int iType,UINT iIndex,int *iPriority = NULL,int *iVol=NULL);

	void ParseEnviroment(CAudioEnviroment &ae);			//处理环境音效

	//iType:E_AUDIO_TYPES之一，iIndex:配置文件中该TYPE下的id
	void Play(int iType,UINT iIndex, UINT nRand, bool bPlayLoop=false,bool bLimitPlayNum=true, int nMaxNum=6,UINT nType=0);

	//播放的时候,需要根据类型和索引自动识别环境音效并作相应的特殊处理
	//播放环境音效，otherx/y表示音源位置，
	void PlayEx(int iType,UINT iIndex, UINT nRand,int selfX,int otherX,int selfY,int otherY,bool bSelf=false,int bRand=0,bool bPlayLoop=false);

	//环境音效在draw的时候，自动判断；如果位置太远，将自动停止，但不立刻卸载出内存；也就是说，环境音效的停止不经由stop显示停止；除非在stopall的时候。
	//timerhander的时候，应该对所有的环境音效进行附加处理；
	void SetPan(LpSAudio temp,int selfX, int otherX,int selfY,int otherY);
	void SetPan(int iType,UINT iIndex,UINT nRand,int selfX, int otherX,int selfY,int otherY);
	void Stop(int iType,UINT iIndex,UINT nRand);

	void PlayMidi(DWORD dwFileName, bool bRepeat,int nSourceX=0,int nSourceY=0,int nListenerX=0,int nListenerY=0);
	void StopMidi(DWORD dwFileName);
	void SetPositionMidi(DWORD dwFileName,int nBufferX,int nBufferY,int nListenerX,int nListenerY);

	void ResumeAll();
	void PauseAll();

	void RestartAll();
	void StopAll();
	void SetAllVol(); 
	void CutAudio();
	void EnableMidi(bool b) { m_bMidiOK = b; }
	void EnableSound(bool b){ m_bSoundOK = b; }
	void EnableBkSound(bool b){m_bBkSoundOK = b;}

	void SetVolumeSound(long l);
	long GetVolumeSound() {    return m_nVolumeSound; }
	void SetVolumeMusic(long l);
	long GetVolumeMusic() {    return m_nVolumeMusic; }

	bool ReCreateSoundManager();

	//背景音乐相关的函数
	void PlayMusic(int iType,UINT iIndex,bool bPlayLoop);
	void StopMusic(int iType,UINT iIndex);
	bool IsMusicPlaying(int iType,UINT iIndex);
	void StopAllMusic();

	bool	MusicValidate(char *cMusicName);	//check file is exsiting
	//音乐文件播放控制
	void	MusicPlay(char *cMusicName,int ms);	//play with fading time 无限次循环播放
	bool	MusicPlaying(char *cMusicName);		//playing?

	void	MusicPause(char *cMusicName);	//Pause
	bool	MusicPaused(char *cMusicName);	//paused?
	void	MusicResume(char *cMusicName);	//resume

	void	MusicStop(char *cMusicName,int ms);	//stop with fading time
	bool	MusicStoped(char *cMusicName);		//stoped?
	void	MusicClose(char *cMusicName,int ms);//close the music source
	void	MusicStopAll(int ms);	
	void	MusicRestartAll(int ms)	;

	void	MusicPauseAll();	
	void	MusicResumeAll();

	void	MusicSwitch(char *cMusicNameFirst,char *cMusicNameSecond,int ms);	//stop the first,play the second




protected:
	IDirectMusicLoader8*      m_pLoader  ;      
	IDirectMusicPerformance8* m_pPerformance  ; 
	IDirectMusicSegment8*     m_pSegment   ;
	IDirectSound3DBuffer8*		m_pDSB;
	IDirectMusicAudioPath8*  m_p3DAudioPath;
	bool m_bMidiOK;
	bool m_b3DMidi;

	bool m_bStopOK;
	HANDLE m_hExit ;
	HANDLE m_hThead;
	HANDLE m_hTimer;
	LARGE_INTEGER m_liDueTime;
	
	bool m_bOK;
	UINT m_nMaxStaticMem;
	UINT m_nMaxStreamingMem;
	UINT m_nCurStaticMem;
	UINT m_nCurStreamingMem;

	CSoundManager* m_pSoundManager;

	SAudio * m_pStaticStart;
	SAudio * m_pStaticEnd;
	SAudio * m_pStaticStopStart;
	SAudio * m_pStaticStopEnd;
	
	SAudio * m_sAudio;
	CSound * m_sSound;

	SAudio * m_pMusicFadeIn;
	//bool m_bStreamingOK;
	
	long m_nVolumeSound_Last;
	long m_nSqrtVolumeSound;

	DWORD  m_dwMidiFileName;


	//配置文件管理
	sAudioConfigManager_t	*m_pAudioConfigManager;

	long   m_nVolumeSound,m_nVolumeMusic;
	bool   m_bSoundOK;
	bool   m_bMusicOK;
	bool   m_bBkSoundOK;

	int    m_nCurSoundCount;

	//内部函数
protected :
	//增加音效到播放引擎中
	bool AddAudio(int iType,UINT iIndex,bool bPlayLoop, UINT nRand,int iTx = 0,int iTy = 0);

	//检测当前标示(iType,iIndex)的音效是否正在播放
	LpSAudio GetAudioToStart(int iType,UINT iIndex,int iTx = 0,int iTy = 0);

	bool DeleteAudio(LpSAudio pAudio);
	bool Initialize();
	bool InitMidi();
	//bool NewStreamingThread();
	inline void UINTToString(char cFileName[], char *strFileName);

	//
	LpSAudio GetAudio(int iType,UINT iIndex,UINT nRand = 0);
	LpSAudio WashAudio(int iType,UINT iIndex);		//淘汰一个低优先级别的播放样本
	bool GetFreeAudio(int & index);
	bool AttenuationVol(int selfX,int otherX,int selfY,int otherY,long &vol,long & pan);
	bool AttenuationVolEnv(int iType,UINT iIndex,int selfX,int otherX,int selfY,int otherY,long &vol,long & pan);
};

extern CAudio *g_pEAudio;

