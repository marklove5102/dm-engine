#ifndef MUSICINTERFACE_H
#define MUSICINTERFACE_H

#include "Global/Globaldefine.h"

#pragma warning (disable :4267)// conversion from 'size_t' to 'int'

/*
**背景音乐接口
**--硬设备及混音、小型WAV文件包裹、cache
**
*/
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

	enum E_AUDIO_STATE
	{
		EAS_PLAYING = 0,
		EAS_PAUSED,
		EAS_STOPED,
	};
	extern int g_Audio_State;					//Open 

	//设备管理
	int		musicInit(HWND hwnd,char *szFilePath);
	int		musicShutDown();

	//播放控制
	void	setAudioState(E_AUDIO_STATE eState);
	bool	musicValidate(char *cMusicName);	//check file is exsiting
	void	musicPlay(char *cMusicName,int ms,long lVolume=-1,int loops = 0x7fffffff);	//play with fading time如果lVolume <= 0 则不重新设置音量，保持原来的
	bool	musicPlaying(char *cMusicName);		//playing?

	void	musicPause(char *cMusicName);		//Pause
	bool	musicPaused(char *cMusicName);		//paused?
	void	musicResume(char *cMusicName);		//resume

	void	musicStop(char *cMusicName,int ms);	//stop with fading time
	bool	musicStoped(char *cMusicName);		//stoped?
	void	musicClose(char *cMusicName,int ms);//close the music source
	void	musicStopAll(int ms);	
	void	musicRestartAll(int ms)	;
	void	musicVolume(long lVol);
	void	soundVolume(long lVol);
	void    soundPlay(char *soundname, signed short angle = 0, unsigned char distance = 0, int loops = 0, int ticks = 0);

	void	musicPauseAll();	
	void	musicResumeAll();

	void	musicSwitch(char *cMusicNameFirst,char *cMusicNameSecond,int ms);	//stop the first,play the second

	/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif