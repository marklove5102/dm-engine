#pragma once

#include "../../Global/Globaldefine.h"



//除环境音效外,同name/id播放都不允许重复
enum	E_AUDIO_TYPES
{
	EAT_MUSIC	= 0	,
	EAT_CONTROL		, //priority = 1
	EAT_PLAYER		, //           1
	EAT_MONSTER		, //           2
	EAT_COMBINE		, //
	EAT_NPC			, //           2
	EAT_MAGIC		, //           1
	EAT_GOODS		, //           3
	EAT_WEAPON		, //           3
	EAT_ENVIROMENT	, //           1,环境音效
	EAT_OTHER		, //           2,包括附加音源
	EAT_BKMUSIC     , //           地图背景音乐
	AUDIO_TYPES_COUNT 
};

typedef enum 
{
	AUDIO_NO_FADING,
	AUDIO_FADING_OUT,
	AUDIO_FADING_IN
} Audio_Fading;

enum	//E_MUSIC_PLAYER_ITEMS
{
	E_MUSIC_PLAYER_WALK	= 1	,//走路1
	E_MUSIC_PLAYER_RUN		,//跑步2
	E_MUSIC_PLAYER_ATTACK	,//攻击3
	E_MUSIC_PLAYER_MAGIC	,//施法4
	E_MUSIC_PLAYER_GET		,//获取5
	E_MUSIC_PLAYER_HURT		,//受伤6
	E_MUSIC_PLAYER_LIE		,//倒地7
	E_MUSIC_PLAYER_RIDE		,//骑乘8
	E_MUSIC_PLAYER_HELLO	,//招呼9
	E_MUSIC_PLAYER_BYE		,//再见10
	E_MUSIC_PLAYER_SIT		,//坐下11
	E_MUSIC_PLAYER_VICTORY	,//胜利12
	E_MUSIC_PLAYER_CRY		,//哭泣13
	E_MUSIC_PLAYER_RAGE		,//愤怒14
	E_MUSIC_PLAYER_DIE		,//装死15
	E_MUSIC_PLAYER_WEAK		,//虚弱16
	E_MUSIC_PLAYER_RUSH		,//冲锋17
	E_MUSIC_PLAYER_LOVE		,//爱情18

	E_MUSIC_PLAYER_SHOCK_MALE	,//休克男19
	E_MUSIC_PLAYER_SHOCK_FEMALE	,//休克女20

	E_MUSIC_PLAYER_HURT_FEMALE	,//受伤女21
	E_MUSIC_PLAYER_LIE_FEMALE	,//倒地女22

	E_MUSIC_PLAYER_ITEMS 
};
enum	//E_MUSIC_MONSTER_ITEMS
{
	E_MUSIC_MONSTER_APPEAR	= 0	,//出现
	E_MUSIC_MONSTER_ATTACK		,//攻击
	E_MUSIC_MONSTER_ATTACKED	,//被攻击
	E_MUSIC_MONSTER_DIE			,//死亡
	E_MUSIC_MONSTER_RUN			,//跑步
	E_MUSIC_MONSTER_WALK		,//走
	E_MUSIC_MONSTER_TRAINING	,//训练
	E_MUSIC_MONSTER_ITEMS 
};

enum	//E_MUSIC_COMBINE_ITEMS
{
	E_MUSIC_COMBINE_APPEAR	= 0	,//出现
	E_MUSIC_COMBINE_ATTACK		,//攻击
	E_MUSIC_COMBINE_ATTACKED	,//被攻击
	E_MUSIC_COMBINE_DIE			,//死亡
	E_MUSIC_COMBINE_RUN			,//跑步
	E_MUSIC_COMBINE_WALK		,//走
	E_MUSIC_COMBINE_TRAINING	,//训练
	E_MUSIC_COMBINE_ITEMS 
};

enum	//E_MUSIC_MAGIC_ITEMS
{
	E_MUSIC_MAGIC_BEGIN		= 0	,//起手
	E_MUSIC_MAGIC_FLY			,//飞行
	E_MUSIC_MAGIC_FINISH		,//结束
	E_MUSIC_MAGIC_SPECIAL		,//特效
	E_MUSIC_MAGIC_ITEMS 
};

enum	//E_MUSIC_MAGIC_ITEMS
{
	E_MUSIC_NPC_APPEAR		= 0	,//出现
	E_MUSIC_NPC_TURN			,//转方向
	E_MUSIC_NPC_ITEMS 
};

enum	//E_MUSIC_CONTROL_ITEMS
{
	MUSIC_CONTROL_NULL		= 0	,//

	MUSIC_CONTROL_DLJM_BTN   		,//1登陆界面按键
	MUSIC_CONTROL_DLJM_PSLCT   		,//2登陆界面角色选择框选中
	MUSIC_CONTROL_DLJM_SLCT   		,//3登陆界面选择
	MUSIC_CONTROL_GNJM_BTN   		,//4功能界面功能按键
	MUSIC_CONTROL_GNJM_JMBTN   		,//5功能界面界面按键	  
	MUSIC_CONTROL_GNJM_ALM   		,//6功能界面警告	
	MUSIC_CONTROL_GNJM_KJL	   		,//7功能界面快捷栏按钮按键 
	MUSIC_CONTROL_GNJM_SLCT   		,//8功能界面选择按键 
	MUSIC_CONTROL_GNJM_LM	   		,//9功能界面最大最小化   
	MUSIC_CONTROL_GNJM_XXLM    		,//10功能界面消息栏最大最小化
	MUSIC_CONTROL_COIN  			,//11功能界面剩余钱币按键   
	MUSIC_CONTROL_DHK_HTML  		,//12对话框界面超级链接点击       
	MUSIC_CONTROL_DHK_SLCT  		,//13对话框界面选择物品点击 
	MUSIC_CONTROL_DITU_MAX  		,//14地图界面最大化及缩略图按键   
	MUSIC_CONTROL_DITU_LM			,//15地图界面大地图缩放按键
	MUSIC_CONTROL_TISHI				,//16小提示按钮的音效（tips menu）

	MUSIC_CONTROL_DAOJISHI			,//17
	MUSIC_CONTROL_KAIZHAN			,//18

	MUSIC_CONTROL_ITEMS			 //	 
};

//环境音效类型
enum 
{
	AUDIO_ENV_HUOPENG = 1	,//
	AUDIO_ENV_PUBU			,//
	AUDIO_ENV_ITEMS			 //

};

struct sAudioEnviroment_t
{
	sAudioEnviroment_t(int ix,int iy,int iType):iTx(ix),iTy(iy),iEnvType(iType)
	{
	}
	int		iEnvType;
	int		iTx,iTy;		// 环境音效标识符,同一幅地图不存在同iTx和iTy的环境音效；
};

enum E_SoundType
{
	EST_WAVE = 0,
	EST_OGG,
};

//音效播放链表
typedef struct _SAudio{     

	void *		pSound;
	BYTE		bySounType;     //E_SoundType
	bool		bLock;			// 静音时加锁
	bool		bIsPlaying;		// 记录pause时，是否在播放
	bool		bLoop;        	// 当前文件和当前播放方式有几个
	_SAudio*	pPrevAudio;		// 音乐链表的下一个元素，如果无下一个元素，则置为NULL
	_SAudio*	pNextAudio;		// 音乐链表的上一个元素，如果无上一个元素，则置为NULL
	_SAudio*	pNextStop;
	UINT		nRand;			// 第二个作用时表示声音播放完毕

	int			iType;			// 类型，用来识别类型，作为附加判断
	int			iIndex;			// 音乐文件索引，对应文件由配置文件获得

	Audio_Fading	fading;			//fading 类型			
	int				fade_step;		//当前fade到的step
	int				fade_steps;		//总共需要fade的step个数
	//		int				audio_active;	//当前播放状态
	//		int				audio_stopped;	//是否停止
	//		int				audio_loops;	//循环个数，0为非循环(do not set for negative!!!)

	int			iPriority;		// 优先级标识,高优先级替换低优先级，除环境音效外，采用cut；
	int			iTx,iTy;		// 环境音效标识符,同一幅地图不存在同iTx和iTy的环境音效；
	// 如果指定为环境音效，必须设定该项
	//int count;
}SAudio, *LpSAudio;

class  CAudioInterface 
{ 
public:
	virtual ~CAudioInterface(void){}
	
	virtual UINT& GetRand() = 0;
	virtual int& GetAudioRain() = 0;
	virtual int& GetAudioBlood() = 0;

	virtual void TimerHandler() = 0;

	virtual void PlayMusic(int iType,UINT iIndex,bool bPlayLoop) = 0;
	virtual void StopMusic(int iType,UINT iIndex) = 0;
	virtual bool IsMusicPlaying(int iType,UINT iIndex) = 0;
	virtual void StopAllMusic() = 0;

	//iType:E_AUDIO_TYPES之一，iIndex:配置文件中该TYPE下的id
	virtual void Play(int iType,UINT iIndex, UINT nRand,bool bPlayLoop=false,bool bLimitPlayNum=true, int nMaxNum=6,UINT nType=0) = 0;
	//播放的时候,需要根据类型和索引自动识别环境音效并作相应的特殊处理
	//播放环境音效，otherx/y表示音源位置，
	virtual void PlayEx(int iType,UINT iIndex, UINT nRand,int selfX,int otherX,int selfY,int otherY,bool bSelf=false,int bRand=0,bool bPlayLoop=false) = 0;

	//环境音效在draw的时候，自动判断；如果位置太远，将自动停止，但不立刻卸载出内存；也就是说，环境音效的停止不经由stop显示停止；除非在stopall的时候。
	//timerhander的时候，应该对所有的环境音效进行附加处理；
	virtual void SetPan(LpSAudio temp,int selfX, int otherX,int selfY,int otherY) = 0;
	virtual void SetPan(int iType,UINT iIndex,UINT nRand,int selfX, int otherX,int selfY,int otherY) = 0;
	virtual void Stop(int iType,UINT iIndex,UINT nRand) = 0;

	virtual void PlayMidi(DWORD dwFileName, bool bRepeat,int nSourceX=0,int nSourceY=0,int nListenerX=0,int nListenerY=0) = 0;
	virtual void StopMidi(DWORD dwFileName) = 0;
	virtual void SetPositionMidi(DWORD dwFileName,int nBufferX,int nBufferY,int nListenerX,int nListenerY) = 0;

	virtual void ResumeAll() = 0;
	virtual void PauseAll() = 0;

	virtual void RestartAll() = 0;
	virtual void StopAll() = 0;
	virtual void SetAllVol() = 0; 
	virtual void CutAudio() = 0;
	virtual void EnableMidi(bool b) = 0;
	virtual void EnableSound(bool b) = 0;
	virtual void EnableBkSound(bool b) = 0;

	virtual void SetVolumeSound(long l) = 0;
	virtual long GetVolumeSound() = 0;
	virtual void SetVolumeMusic(long l) = 0;
	virtual long GetVolumeMusic() = 0;

	virtual bool ReCreateSoundManager() = 0;
};

extern CAudioInterface	* g_pAudio;
