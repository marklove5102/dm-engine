#define ENABLE_SOUND
#define _sqrt_(a) sqrt((double)(a))

#include <math.h>
#include <io.h>
#include "audio.h"
#include "dsutil.h"
#include "Global/PerfCheck.h"
#include "Global/DebugTry.h"
#include "Audio.inl"
#include "MusicInterface.h"

#pragma comment(lib,"MusicInterface.lib")

 
UINT		a_uPeriodMin			= 0;
int g_Audio_Rain = 0;
int g_Audio_Blood = 0;
UINT g_nRand = 3;

const int g_Audio_Allow_Mem  = MAX_AUDIO_NUM;	//当前最大允许cache个数
const int g_Audio_Allow_Play = MAX_AUDIO_NUM - 25;	//当前音效最大允许同时播放个数

//SetPan ：The returned value is measured in hundredths of a decibel (dB), in the range of DSBPAN_LEFT to DSBPAN_RIGHT. These values are currently defined in Dsound.h as –10,000 and 10,000 respectively. The value DSBPAN_LEFT means the right channel is attenuated by 100 dB. The value DSBPAN_RIGHT means the left channel is attenuated by 100 dB. The neutral value is DSBPAN_CENTER, defined as zero. This value of 0 in the plPan parameter means that both channels are at full volume (they are attenuated by 0 decibels). At any setting other than DSBPAN_CENTER, one of the channels is at full volume and the other is attenuated.
//A pan of –2173 means that the left channel is at full volume and the right channel is attenuated by 21.73 dB. Similarly, a pan of 870 means that the left channel is attenuated by 8.7 dB and the right channel is at full volume. A pan of DSBPAN_LEFT means that the right channel is silent and the sound is all the way to the left, while a pan of DSBPAN_RIGHT means that the left channel is silent and the sound is all the way to the right.
//The pan control acts cumulatively with the volume control.
//SetVolume ：The volume is specified in hundredths of decibels (dB). Allowable values are between DSBVOLUME_MAX (no attenuation) and DSBVOLUME_MIN (silence). These values are currently defined in Dsound.h as 0 and –10,000 respectively. The value DSBVOLUME_MAX represents the original, unadjusted volume of the stream. The value DSBVOLUME_MIN indicates an audio volume attenuated by 100 dB, which, for all practical purposes, is silence. Currently DirectSound does not support amplification.

int g_Audio_Vol_Gain = -2000;
int g_Audio_Pan_Gain = -5000;
//音量分级表（for(int i=1;i<=100;++i)	g_fLog[i] = (2.0-log10(i))）
//定义应用音量表（1，100），对应于dsound音量表（-10000，0）
//
//double	g_fLog[101] = {2.000000000000,	
double	g_fLog[101] = {4.000000000000,	
2.000000000000,1.698970004336,1.522878745280,1.397940008672,1.301029995664,1.221848749616,1.154901959986,1.096910013008,1.045757490561,1.000000000000,
0.958607314842,0.920818753952,0.886056647693,0.853871964322,0.823908740944,0.795880017344,0.769551078622,0.744727494897,0.721246399047,0.698970004336,
0.677780705266,0.657577319178,0.638272163982,0.619788758288,0.602059991328,0.585026652029,0.568636235841,0.552841968658,0.537602002101,0.522878745280,
0.508638306166,0.494850021680,0.481486060122,0.468521082958,0.455931955650,0.443697499233,0.431798275933,0.420216403383,0.408935392974,0.397940008672,
0.387216143280,0.376750709602,0.366531544420,0.356547323514,0.346787486225,0.337242168318,0.327902142064,0.318758762624,0.309803919971,0.301029995664,
0.292429823902,0.283996656365,0.275724130399,0.267606240177,0.259637310506,0.251811972994,0.244125144328,0.236572006437,0.229147988358,0.221848749616,
0.214670164989,0.207608310502,0.200659450546,0.193820026016,0.187086643357,0.180456064458,0.173925197299,0.167491087294,0.161150909263,0.154901959986,
0.148741651281,0.142667503569,0.136677139880,0.130768280269,0.124938736608,0.119186407719,0.113509274828,0.107905397310,0.102372908710,0.096910013008,
0.091514981121,0.086186147616,0.080921907624,0.075720713938,0.070581074286,0.065501548756,0.060480747381,0.055517327850,0.050609993355,0.045757490561,
0.040958607679,0.036212172654,0.031517051446,0.026872146400,0.022276394711,0.017728766960,0.013228265734,0.008773924308,0.004364805402,0.000000000000
};

//音量衰减表
//总音量根据距离衰减
//pan,仅考虑第4象限,右手迪卡尔坐标系;其余采用对称算法.
//地图范围(1024 * 768) = Tiles(16,24)
//long	g_lPanTable[8][12] = {
//	{};
//}

#define CHECK_STR_RETURN(a) { if(!a || !a[0])return 0;};
/*
 * 用于4位字节编码；
 * 高位在后
 */
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
		((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
		((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif


//added 2004-5-9
inline bool __string_equal(char des[],char src[])
{
	return (strcmp(des,src)==0);
};

inline void __string_clone(char des[],char src[])
{
	strcpy(des,src);
};

inline bool __string_not_null(char des[])
{
	return (des && des[0]!='\0');
};

inline bool __string_null(char des[])
{
	return (!des || (des[0]=='\0'));
};

// some special char should be trimed,including tab,space ,enter ,etc
inline int __get_audio_name(char *des,char *src,int n)
{
	while(*src == ' ' || *src == '\t' )src++; //trim the left space

	int i = 0;
	for(;i < n && src[i] && src[i] !=';'  && src[i] !='*' && src[i] !='\t' ; ++ i)
	{
		des[i] = src[i];
	}
	int j = i - 1;
	for(;j >=0 && des[j] == ' '; -- j) //trim the right space
	{
		des[j] = '\0';
	}
	return (j+1);
};
//ext func for complex config file!
//返回个数
//char C_HANDLE_BUF[512];	//the max handle buf;
inline int __get_audio_name_table(sAudioName_t *table,int index,int items,char *src,int n)
{
	CHECK_STR_RETURN(src);

	while( *src == '\t' )src++; //trim the left \t , we leave the space ,just for safe

	char *cSep =  strchr(src,';');
	if(cSep)
		cSep[0] = '\0';
	char seps[]   = ",\t\n";
	char *token;

	int i = 0;
	token = strtok( src, seps );
	while( token != NULL )
	{
		__get_audio_name(table[index*items + i].cName,token,n);
		if(++i >= items)	break;
		token = strtok( NULL, seps );
	}
	return (i);
};


struct sAudioType_t
{
	char		cType[16];	//类型名称
	int			iItems;		//控制重复\同性\随机个数，对于不同类型有不同的随机范围，该参数需要预先设置!!!
	int			iPriority;	//默认的优先级别
	int			iVol;		//默认的音量
};

sAudioType_t sAudioType[AUDIO_TYPES_COUNT] =	//强制数量同步
{
	{"music",		1,1,0},
	{"control",		1,1,0},
	{"player",		1,1,0},//this is for all player??
	{"monster",		1,2,0},
	{"combine",		1,2,0},
	{"npc",			1,2,0},
	{"magic",		1,1,0},//modify:2004-9-28
	{"good",		1,3,0},
	{"weapon",		1,3,0},//randon
	{"envionment",	1,1,0},//
	{"other",		1,2,0},
	{"bkmusic",		1,2,0}
};

//[二进制]music file config file head
struct sAudioConfigHead_t
{
	unsigned long lName;
	unsigned long lVer;
	char cDes[16];
};

#define CFG_FILE_ID (MAKEFOURCC('C','F','G',' '))

//对应类型的配置文件
struct sAudioConfig_t
{
	char		cType[16];	//类型名称
	long		iItems;		//控制重复\同性\随机个数，对于不同类型有不同的随机范围，该参数需要预先设置!!!
	long		iCount;		//最大类型索引，ex:对于monster这样的类型，取决于最大的monster id!
							//计算名称时，iCount * iItems + 索引偏移

	sAudioName_t *sName;	//基于索引的文件名列表，不同类型的索引取决于“音效配置文件规范”

	sAudioConfig_t():iCount(0),iItems(1),sName(0)
	{
		cType[0] = '\0';
	};

	//根据类型读取配制；
	sAudioConfig_t(char *cThisType ,long iThisItems,char *strMusicFile):iCount(0),iItems(iThisItems)
	{
		//strncpy(cType,cThisType,AUDIO_NAMES_LENGTH);
		Load(cThisType,strMusicFile);
	};
	~sAudioConfig_t()
	{
		Clear();
	};

	// 初始化，参数：cThisType-类型名称；iThisItems-重复个数；strMusicFile-配置文件名。
	bool  Init(char *cThisType ,long iThisItems,char *strMusicFile)
	{
		iItems = iThisItems;
		return Load(cThisType,strMusicFile);
	}

	// 根据focc代码初始化类型
	bool InitType(long focc)
	{
		for(int i = 0;i<AUDIO_TYPES_COUNT;++i)
		{
			const char *c = &sAudioType[i].cType[0];
			if(focc == MAKEFOURCC(c[0],c[1],c[2],c[3]))
			{
				strcpy(cType,c);
				return true;
			}
		}
		return false;
	};

	// 清空
	void     Clear()						
	{
		SAFE_DELETE_ARRAY(sName);		//2-d array
		iCount = 0;
		iItems = 1;
		cType[0] = '\0';
	};

	//根据类型[cTypeParam]和配置文件[strMusicFile]加载音效索引
	//strMusicFile must be the full path of the "ini" file
	bool	Load(char *cTypeParam,char *strMusicFile)	
	{
		CHECK_STR_RETURN(cTypeParam);
		CHECK_STR_RETURN(strMusicFile);

		memcpy(cType,cTypeParam,sizeof(cType)); //

		UINT	p=0;
		int		size;
		char	*pChar;
		DWORD	dwBufLen;
		int		iMax =0;
		int     iMaxTemp = 0;

#define MAX_BUFFER_SIZE 32000

		char	buffer[MAX_BUFFER_SIZE]={0}; 
		
		dwBufLen = GetPrivateProfileSection(cType,buffer,MAX_BUFFER_SIZE,strMusicFile);

		//获取最大索引号
		while(p<dwBufLen-1 && dwBufLen )
		{
			size	= (strlen(buffer+p)+1);
			pChar	= strchr(buffer+p,'=');
			if(!pChar)
				break;
			*pChar	= 0;
			iMaxTemp = atol(buffer+p);
			if(iMaxTemp>iMax)
				iMax = iMaxTemp;
			*pChar	= '=';
			p += size;
		}

		//新建
		iCount = iMax+1;
		sName  = (sAudioName_t *)new sAudioName_t[iCount * iItems];	//转化2-d 表为array!!!
		memset(sName,0,sizeof(sAudioName_t)*(iCount * iItems));

		//dump 所有音效资源名称
		p=0;
		iMax=0;
		while(p < dwBufLen-1 && dwBufLen)
		{
			size = (strlen(buffer+p)+1);
			pChar = strchr(buffer+p,'=');
			if(!pChar)
				break;
			*pChar = 0;
			pChar+=1;

			//取得音频文件表
			 __get_audio_name_table(sName,atol(buffer+p),iItems,pChar,AUDIO_NAMES_LENGTH);
			p += size;
		}
		return true;
	};

	long GetLength()
	{
		long lLen = 0;
		lLen = sizeof(cType) + sizeof(iItems) + sizeof(iCount) +  sizeof(sAudioName_t) * iItems * iCount;
		return lLen;
	}

	//加载二进制格式
	//根据关键四字码读取每一节
	//每一节的数据格式为：
	//FOCC;			//4 bytes,the key FOCC;
	//int lLen;		//the length of all the section ;
	//char cName[16];//the full section name 
	//int  iItems;	//the split items;
	//int  iCounts; //the subfields counts;
	//sAudioName_t array;//

	//返回读取字节，为0，错误
	long LoadBinary(char *pFileBuf,long len)
	{
		long lReadBytes = 0;

		long lTemp = 0;

		lTemp = *((long *)(pFileBuf+lReadBytes));lReadBytes+=sizeof(lTemp); //focc

		lTemp = *((long *)(pFileBuf+lReadBytes));lReadBytes+=sizeof(lTemp); //len

		if(lTemp !=len )	//is this possible!notify the main loop to exit!maybe the data error!
			return 0;

		if(lTemp < (sizeof(lTemp) +sizeof(lTemp) +sizeof(cType) + sizeof(iItems) + sizeof(iCount)))//ensure length enougth!
			return 0;


		memcpy(cType,pFileBuf+lReadBytes,sizeof(cType));lReadBytes+=sizeof(cType); //len

		iItems = *((long *)(pFileBuf+lReadBytes));lReadBytes+=sizeof(iItems); //
		iCount = *((long *)(pFileBuf+lReadBytes));lReadBytes+=sizeof(iCount); //

		if(iItems>0 && iCount > 0)//this is absolutely right!
		{
			sName  = (sAudioName_t *)new sAudioName_t[iCount * iItems];	//转化2-d 表为array!!!
			memcpy(sName,(pFileBuf+lReadBytes),sizeof(sAudioName_t)*(iCount * iItems));
			lReadBytes+=sizeof(sAudioName_t) * iItems * iCount;
		}

		return lReadBytes;
	}
};

//配置文件管理者
struct  sAudioConfigManager_t
{
	int	nTypes ;
	sAudioConfig_t *pAudioConfig;

	sAudioConfigManager_t():nTypes(0),pAudioConfig(0)
	{
	};
	~sAudioConfigManager_t()
	{
		Clear();
	};

	// 清空
	void    Clear()
	{
		SAFE_DELETE_ARRAY(pAudioConfig);
		nTypes = 0;
	};

	// 加载ini配置[文本方式]；
	bool	LoadIni(char *strMusicFile);
	//加载cfg配置[二进制方式]
	bool	LoadCfg(char *strFileName);
	// 根据类型和索引得到播放的文件名
	bool GetName(int iType,UINT iIndex,char cName[],int *iPriority=NULL,int *iVol=NULL);
	char *GetName(int iType,UINT iIndex,int *iPriority=NULL,int *iVol=NULL);

	// 随机得到一个播放名称
	char *GetNameRandon(int iType,UINT iIndex = 0,int *iPriority=NULL,int *iVol=NULL);
};


//加载所有的配置项信息
bool sAudioConfigManager_t::LoadIni(char *strFileName)	
{
	TRY_BEGIN
	CHECK_STR_RETURN(strFileName);	
	Clear();
	pAudioConfig = new sAudioConfig_t[AUDIO_TYPES_COUNT];
	for(int i = 0;i<AUDIO_TYPES_COUNT;++i)
	{
		pAudioConfig[i].Init(sAudioType[i].cType,sAudioType[i].iItems,strFileName);
	}
	return true;
	TRY_END_NOTHROW
	return false;
};

//加载music二进制配置文件;2005-1-6
bool sAudioConfigManager_t::LoadCfg(char *strFileName)
{
	TRY_BEGIN
	CHECK_STR_RETURN(strFileName);	
	Clear();

	char *pFileBuf	= 0;
	long lFileLen	= 0;
	if( (lFileLen = LoadFile(strFileName,&pFileBuf,false)) == 0)
		return false;

	while(lFileLen>sizeof(sAudioConfigHead_t))
	{
		sAudioConfigHead_t cfg;
		memcpy(&cfg,pFileBuf,sizeof(sAudioConfigHead_t));

		unsigned long id = CFG_FILE_ID;
		if(cfg.lName != id)
		{
#ifdef _DEBUG
			OutputDebugString("ERROR:not music cfg file!\n");
#endif
			break;
		}

		//定位到数据区
		char *buffer = pFileBuf+sizeof(sAudioConfigHead_t);
		lFileLen-=sizeof(sAudioConfigHead_t);

		char *cp = buffer;
		long lReadBytes = 0;
		pAudioConfig = new sAudioConfig_t[AUDIO_TYPES_COUNT];
		for(int i = 0;i<AUDIO_TYPES_COUNT;++i)
		{
			cp+=lReadBytes;

			long lFocc,lTemp = 0;
			lFocc = *((long *)(cp+0));; //focc
			lTemp = *((long *)(cp+4));; //len

			if(lTemp == 0)
			{
#ifdef _DEBUG
				OutputDebugString("ERROR:section length!\n");
#endif
				break;
			}
			//类型匹配和初始化
			pAudioConfig[i].InitType(lFocc);

			//加载数据
			long lTest = lTemp;
			if(lTest == pAudioConfig[i].LoadBinary(cp,lTemp))
			{
			}
			else 
			{
#ifdef _DEBUG
				OutputDebugString("ERROR:loading section!\n");
#endif
			}
			lReadBytes=lTemp;
		}
		break;
	}

//	PkgFreeBuffer(pFileBuf);
	if(pFileBuf)
		delete [] pFileBuf;

	return true;
	TRY_END_NOTHROW
	return false;
}

//根据类型和索引得到一个具体的音乐文件名称、默认的优先级和音量
//
bool sAudioConfigManager_t::GetName(int iType,UINT iIndex,char cName[],int *iPriority,int *iVol)
{
	if(pAudioConfig && iType < AUDIO_TYPES_COUNT && iType>=0 && cName)
	{
		if(iIndex < (pAudioConfig[iType].iCount * sAudioType[iType].iItems) && iIndex >=0)
		{
			if(iPriority)
			{
				*iPriority = sAudioType[iType].iPriority;
			}
			if(iVol)
			{
				*iVol = sAudioType[iType].iVol;
			}
			__string_clone(cName,pAudioConfig[iType].sName[iIndex].cName);
			return true;
		}
	}
	return false ;
};

//根据类型和索引得到一个具体的音乐文件名称、默认的优先级和音量
//
char *sAudioConfigManager_t::GetName(int iType,UINT iIndex,int *iPriority,int *iVol)
{
	if(pAudioConfig && iType < AUDIO_TYPES_COUNT && iType>=0 )
	{
		if(iIndex < (pAudioConfig[iType].iCount * sAudioType[iType].iItems) && iIndex >=0)
		{
			if(iPriority)
			{
				*iPriority = sAudioType[iType].iPriority;
			}
			if(iVol)
			{
				*iVol = sAudioType[iType].iVol;
			}
			return pAudioConfig[iType].sName[iIndex].cName;
		}
	}
	return 0 ;
};

//随机得到某个类型中的一个音效、默认的优先级和音量
char *sAudioConfigManager_t::GetNameRandon(int iType,UINT iIndex,int *iPriority,int *iVol)
{
	if(pAudioConfig && iType < AUDIO_TYPES_COUNT && iType>=0 )
	{
		UINT iIndexRandon = ::GetTickCount()%(pAudioConfig[iType].iCount* sAudioType[iType].iItems);

		if(iIndexRandon == 0)
			++iIndexRandon;
		if(iIndexRandon < (pAudioConfig[iType].iCount * sAudioType[iType].iItems) && iIndexRandon >=0)
		{
			if(iPriority)
			{
				*iPriority = sAudioType[iType].iPriority;
			}
			if(iVol)
			{
				*iVol = sAudioType[iType].iVol;
			}
			return pAudioConfig[iType].sName[iIndexRandon].cName;
		}
	}
	return 0 ;
};

///////////////////////////////////////////////////////////////////////////////////
//

CAudio::CAudio(void)
{
	m_sAudio		= NULL;
	m_nMaxStaticMem = 4 * 1024 * 1024; 
	m_nMaxStreamingMem = 40 * 1024 * 1024;
	m_nCurStaticMem = 0;
	m_nCurStreamingMem = 0;
	m_pSoundManager = NULL;
	m_pAudioConfigManager = NULL;

	m_sAudio = NULL;
	m_sSound = NULL;
	m_pStaticStart = NULL;
	m_pStaticEnd = NULL; 
	m_pStaticStopStart = NULL;
	m_pStaticStopEnd = NULL;
	m_nVolumeSound_Last = 80;
	m_nSqrtVolumeSound=	(long)(g_Audio_Vol_Gain* g_fLog[m_nVolumeSound_Last]);

	m_pLoader =NULL ;      
	m_pPerformance = NULL ; 
	m_pSegment = NULL  ;    
	m_pMusicFadeIn = NULL;
	m_dwMidiFileName = 0;
	m_pDSB = NULL;
	m_p3DAudioPath = NULL;

	m_bOK = false;
	m_bMidiOK = false;
	m_b3DMidi = false;
	m_bSoundOK = true;
	m_nVolumeSound = 80;
	m_nVolumeMusic = 80;
	m_nCurSoundCount =0;

#ifdef ENABLE_SOUND 
	m_bOK = Initialize();
#endif

	if( strcmp(g_pEStreamMgr->GetConfigStr("PlayMidi","yes"),"yes") == 0 )
	{
		TRY_BEGIN
			m_bMidiOK = InitMidi();
		TRY_END_DO(m_bMidiOK = false;)
	}



	// 设置时间精度
	TIMECAPS tcap;
	if(timeGetDevCaps(&tcap,sizeof(tcap)) != TIMERR_NOERROR)
		tcap.wPeriodMin = 1;
	a_uPeriodMin = max(tcap.wPeriodMin,1);
	timeBeginPeriod(a_uPeriodMin);
}

CAudio::~CAudio(void)
{
TRY_BEGIN
    StopAllMusic();
	musicShutDown();
TRY_END

	SAudio *pAudio = m_pStaticStart;
	//SAFE_DELETE(g_Midi);
	while(pAudio)
	{
		DeleteAudio(pAudio);	//删除正在播放的资源buf
		pAudio = m_pStaticStart;
	}

	SAFE_DELETE_ARRAY(m_sSound);	//删除Sound 容器	
	SAFE_DELETE_ARRAY(m_sAudio);	//删除sound adaptor

	SAFE_DELETE(m_pSoundManager); 

    // Close down DirectMusic after releasing the DirectSound buffers
	SAFE_RELEASE(m_p3DAudioPath);
 	SAFE_RELEASE(m_pDSB);
	SAFE_RELEASE(m_pLoader);
	SAFE_RELEASE(m_pSegment);


	if(m_pPerformance)
		m_pPerformance->CloseDown();
	SAFE_RELEASE(m_pPerformance);
	CoUninitialize();

#ifdef ENABLE_SOUND 
	SAFE_DELETE(m_pAudioConfigManager); 
//	PkgShutDown();
#endif
	timeEndPeriod(a_uPeriodMin);
}

bool CAudio::Initialize()
{
	if(!g_pCallBack)
		return false;

	//加载音效配置文件
	m_pAudioConfigManager = new sAudioConfigManager_t;


	//先尝试从二进制配置文件中加载，失败了再从ini文件中加载
	char strMusicFile[260]="";
	sprintf(strMusicFile,"%s/config/music.cfg",g_pCallBack->GetDataDir());

	if(!m_pAudioConfigManager->LoadCfg(strMusicFile))
	{
		sprintf(strMusicFile,"%s/config/music.ini",g_pCallBack->GetDataDir());
		m_pAudioConfigManager->LoadIni(strMusicFile);
	}

	//初始化directsound
    if(!ReCreateSoundManager())
    {
        return false; 
    }

	//初始化音效缓存
	m_sAudio = new SAudio[MAX_AUDIO_NUM];	memset(m_sAudio,0,MAX_AUDIO_NUM* sizeof(SAudio));
	m_sSound = new CSound[MAX_AUDIO_NUM];
	
	for(int i=0; i<g_Audio_Allow_Mem; ++i)
	{
		m_sAudio[i].nRand = 0xFFFFFFFF;
		m_sAudio[i].pSound = (void *)(m_sSound+i);
	}

	//////////初始化音乐模块，创建soft-mixer播放线程
	string strPath = "../Data";
	if (g_pCallBack)
	{
		strPath = g_pCallBack->GetDataDir();
	}
	strPath += "\\music\\ogg\\";

	if( musicInit(g_pCallBack->GetWndHandle(),(char*)(strPath.c_str())) != 0)///////////
		m_bMusicOK = TRUE;
	else
		m_bMusicOK = FALSE;

	//设置默认的音量
	SetAllVol();

	return true;
}

bool CAudio::ReCreateSoundManager()
{
	if(!g_pCallBack)
		return false;

	SAudio *pAudio = m_pStaticStart;
	//SAFE_DELETE(g_Midi);
	while(pAudio)
	{
		DeleteAudio(pAudio);	//删除正在播放的资源buf
		pAudio = m_pStaticStart;
	}

	HRESULT hr;

	//设置音效工作路径
	char strMusicPath[260]="music\\default\\";

	SAFE_DELETE(m_pSoundManager);
	m_pSoundManager = new CSoundManager();
	if( FAILED( hr = m_pSoundManager->Initialize( g_pCallBack->GetWndHandle(), DSSCL_PRIORITY,strMusicPath) ) )
	{
		return false; 
	}

	return true;
}

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {

	//puts("in filter.");

	if (code == EXCEPTION_ACCESS_VIOLATION) {

		//puts("caught AV as expected.");
		return EXCEPTION_EXECUTE_HANDLER;
	}

	else {

		//puts("didn't catch AV, unexpected.");
		return EXCEPTION_CONTINUE_SEARCH;
	};

}

bool CAudio::InitMidi()
{
	// Create loader object
	bool b=true;
	HRESULT hr= E_FAIL;
	//__try
	//{
	//	//ReadMidiConfig();

	//	m_b3DMidi = (g_pEStreamMgr->GetConfigInt("3DMIDI",1) == 1)?true:false;

	//	CoInitialize(NULL);
	//	if(FAILED(hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
	//		IID_IDirectMusicLoader8, (void**)&m_pLoader )))
	//		__leave;

	//	// Create performance object
	//	if(FAILED(hr = CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
	//		IID_IDirectMusicPerformance8, (void**)&m_pPerformance )))
	//		__leave;	
	//	// This initializes both DirectMusic and DirectSound and 
	//	// sets up the synthesizer. 
	//	if(FAILED(hr = m_pPerformance->InitAudio( NULL, NULL, NULL, 
	//		DMUS_APATH_DYNAMIC_STEREO, 64,
	//		DMUS_AUDIOF_ALL, NULL )))
	//		__leave;

	//	CHAR strPath[MAX_PATH];
	//	sprintf(strPath,"%s/music/midi",g_pCallBack->GetDataDir());

	//	// Tell DirectMusic where the default search path is
	//	WCHAR wstrSearchPath[MAX_PATH];
	//	MultiByteToWideChar( CP_ACP, 0, strPath, -1, 
	//		wstrSearchPath, MAX_PATH );

	//	if(FAILED (hr = m_pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, 
	//		wstrSearchPath, FALSE )))
	//		__leave;
	//}
	//__finally
	//{
	//	if(FAILED(hr))
	//	{
	//		if(m_pLoader)
	//		{
	//			b=false;
	//			SAFE_RELEASE(m_pLoader);
	//		}
	//		if(m_pPerformance)
	//		{
	//			b=false;
	//			SAFE_RELEASE(m_pPerformance);
	//		}
	//	}
	//}
	__try
	{
		//ReadMidiConfig();
		m_b3DMidi = (g_pEStreamMgr->GetConfigInt("3DMIDI",1) == 1)?true:false;

		CoInitialize(NULL);
		hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC,IID_IDirectMusicLoader8, (void**)&m_pLoader);

		// Create performance object
		if(SUCCEEDED(hr))
			hr = CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC,IID_IDirectMusicPerformance8, (void**)&m_pPerformance );

		// This initializes both DirectMusic and DirectSound and 
		// sets up the synthesizer. 
		if(SUCCEEDED(hr))
			hr = m_pPerformance->InitAudio( NULL, NULL, NULL,DMUS_APATH_DYNAMIC_STEREO, 64,DMUS_AUDIOF_ALL, NULL);

		if(SUCCEEDED(hr))
		{
			CHAR strPath[MAX_PATH];
			sprintf(strPath,"%s/music/midi",g_pCallBack->GetDataDir());

			// Tell DirectMusic where the default search path is
			WCHAR wstrSearchPath[MAX_PATH];
			MultiByteToWideChar( CP_ACP, 0, strPath, -1, 
				wstrSearchPath, MAX_PATH );

			hr = m_pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes,wstrSearchPath, FALSE );
		}
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
		//MessageBox(NULL,"1","",MB_OK);
		int i = 3;
	}

	if(FAILED(hr))
	{
		b=false;
		if(m_pLoader)
		{
			SAFE_RELEASE(m_pLoader);
		}
		if(m_pPerformance)
		{
			SAFE_RELEASE(m_pPerformance);
		}
	}

	return b;
}

//fade out
//DWORD dwArray[10]={0}; 
//fade out step :40ms
//
void CAudio::TimerHandler(void)
{
	TRY_BEGIN
	LpSAudio pAudio = m_pStaticStopStart;
	LpSAudio preAudio = NULL;

	static int TimeCount = 0;
	TimeCount++;
	if(TimeCount>=200) //每100帧图像cut一次
	{
		CutAudio();
		TimeCount = 0;
	}

	//处理音效
	while(pAudio)
	{
		long lVol = ((CSound *)(pAudio->pSound))->DecVol();
		if(lVol <= -6000)
		{
			((CSound *)(pAudio->pSound))->Stop();
			if(!preAudio)
			{
				m_pStaticStopStart = pAudio->pNextStop;
				pAudio->pNextStop = NULL;
				pAudio->bLock = false;
				pAudio = m_pStaticStopStart;
			}
			else
			{
				preAudio->pNextStop = pAudio->pNextStop;
				pAudio->pNextStop = NULL;
				pAudio->bLock = false;
				pAudio = preAudio->pNextStop;
			}		
		}
		else
		{
			preAudio = pAudio;
			pAudio = pAudio->pNextStop;
		}
	}
	TRY_END_NOTHROW
}

//查找未播放的同名样本.
LpSAudio CAudio::GetAudioToStart(int iType,UINT iIndex,int iTx,int iTy)
{
	LpSAudio  pAudio=NULL,pStartAudio=NULL,pEndAudio=NULL;

	pAudio = m_pStaticStart;
	pStartAudio = m_pStaticStart;
	pEndAudio = m_pStaticEnd;

	pAudio = m_pStaticStart;
	pStartAudio = m_pStaticStart;
	pEndAudio = m_pStaticEnd;

    while(pAudio)
	{
		if(pAudio->pNextAudio == pAudio)
		{
			pAudio->pNextAudio = NULL;
			return NULL;
		}
		if(pAudio->pNextAudio == pStartAudio)
		{
			pAudio->pNextAudio = NULL;
			return NULL;
		}

		if(pAudio->iType == iType && pAudio->iIndex == iIndex)
		{

			//环境音效必须附加判断
			if(EAT_ENVIROMENT == iType )
			{
				//只要在内存中，直接返回
				if( pAudio->iTx == iTx && pAudio->iTy == iTy )
					break;
			}
			else
			{	
				//如果在播放，重新增加；如果不在播放（处于停止态），置前。
				if(0x01 == ((CSound *)(pAudio->pSound))->IsSoundPlaying())//不在播放
					break;
			}
		}
		pAudio = pAudio->pNextAudio;
	}

	if(pAudio) 
	{
		if(pAudio == pStartAudio)
		{
		}
		else if(pAudio == pEndAudio)
		{
			pEndAudio = pEndAudio->pPrevAudio;
			pEndAudio->pNextAudio = NULL;
			pAudio->pNextAudio = pStartAudio;			
			pAudio->pPrevAudio = NULL;
			pStartAudio->pPrevAudio = pAudio;
			pStartAudio = pAudio;				
		}
		else
		{
			pAudio->pPrevAudio->pNextAudio = pAudio->pNextAudio;
			pAudio->pNextAudio->pPrevAudio = pAudio->pPrevAudio;
			pAudio->pNextAudio = pStartAudio;
			pAudio->pPrevAudio = NULL;
			pStartAudio->pPrevAudio = pAudio;
			pStartAudio = pAudio;
		}

		m_pStaticStart = pStartAudio;
		m_pStaticEnd = pEndAudio;

		if(!pAudio->pSound)
		{
			DeleteAudio(pAudio);
			return NULL;
		}
		return pAudio;
	}
	return NULL;
}

//删除
bool CAudio::DeleteAudio(LpSAudio pAudio)
{
	if(!pAudio)
		return false;
	CSound * pStatic = NULL;

	if( pAudio->pPrevAudio )
		pAudio->pPrevAudio->pNextAudio = pAudio->pNextAudio;
	else
	{
		m_pStaticStart = pAudio->pNextAudio;
	}

	
	if(pAudio->pNextAudio)
		pAudio->pNextAudio->pPrevAudio = pAudio->pPrevAudio;
	else
	{
		m_pStaticEnd = pAudio->pPrevAudio;
	}
	
	if(pAudio->pSound)
	{
		pStatic = (CSound *)pAudio->pSound;
		pStatic->Stop();
		m_nCurStaticMem -= pStatic->GetBufferSize();
		pStatic->Free();
	}

	void * p = pAudio->pSound;
	ZeroMemory(pAudio, sizeof(SAudio));
	pAudio->pSound = p;
	pAudio->nRand = 0xFFFFFFFF;
	
	return true;
}


char *CAudio::GetName(int iType,UINT iIndex,int *iPriority ,int *iVol)
{
	if(!m_pAudioConfigManager)
		return NULL;
	return m_pAudioConfigManager->GetName(iType,iIndex,iPriority,iVol);
};
char *CAudio::GetNameRandon(int iType,UINT iIndex,int *iPriority,int *iVol)
{
	if(!m_pAudioConfigManager)
		return NULL;

	return m_pAudioConfigManager->GetNameRandon(iType,iIndex,iPriority ,iVol);
};

//处理环境音效
void CAudio::ParseEnviroment(CAudioEnviroment &ae)
{
	int iSize = ae.m_vItems.size();
	for(int i = 0;i<iSize;++i)
	{
		sAudioEnviroment_t &item = ae.m_vItems[i];
		PlayEx(EAT_ENVIROMENT,item.iEnvType, ++g_nRand,ae.iSelfX,item.iTx,ae.iSelfY,item.iTy,false,0,true);
	}
};

//增加样本到cache中
//如果同类型,同属性的样本存在,那么应该禁止播放
//
bool CAudio::AddAudio(int iType,UINT iIndex,bool bPlayLoop, UINT nRand ,int iTx ,int iTy)
{
	if(!m_pSoundManager)
		return false;

	//每个样本都有默认的优先级和音量;iVol应该为衰减级别,以获得真实比例音量
	int iPriority,iVol;
	char *pName = GetName(iType,iIndex,&iPriority ,&iVol);
	if(NULL == pName || pName[0]=='\0')
		return false ;

	LpSAudio pAudio = NULL;


	if(m_nCurSoundCount + 1 > g_Audio_Allow_Play )
	{
		CutAudio();//整理暂停的样本,应当进入未播放队列.
	}

	HRESULT		hr;
	int index	=0;
	pAudio = GetFreeAudio(index)?m_sAudio+index:NULL;
	if( !pAudio )			
	{	
		//实在找不到,使用优先级淘汰算法.
		pAudio = WashAudio( iType, iIndex);		
		if(!pAudio )
			return false;
	}

	pAudio->bySounType = EST_WAVE;
	pAudio->bIsPlaying = false;
	pAudio->iIndex = iIndex;
	pAudio->iType  = iType;
	pAudio->iPriority = iPriority;

	pAudio->iTx = iTx;//
	pAudio->iTy = iTy;

	pAudio->nRand  = nRand;
	pAudio->bLoop = bPlayLoop;
	pAudio->pNextAudio = NULL;
	pAudio->pPrevAudio = NULL;
	
	char strFileName[256]; 	
	sprintf(strFileName,"%s.wav",pName);

	if(strlen(strFileName)>254)//this is impossible!
	{
		pAudio->nRand = 0xFFFFFFFF;
		return false;
	}
	
	DWORD dwCreationFlags = DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME;
       


	if( FAILED( hr =m_pSoundManager->Create((CSound *)(pAudio->pSound),strFileName,dwCreationFlags) ))
	{
		//当资源不存在时，将产生该错误；
		// Not a critical failure, so just update the status
		((CSound*)(pAudio->pSound))->Free();
		pAudio->nRand = 0xFFFFFFFF;

		return false;//S_FALSE; 
	}

	m_nCurStaticMem += ((CSound *)(pAudio->pSound))->GetBufferSize();



	if(m_pStaticStart)
	{
		pAudio->pNextAudio = m_pStaticStart;
		m_pStaticStart->pPrevAudio = pAudio;
		m_pStaticStart = pAudio;	
	}
	else
	{
		m_pStaticStart = pAudio;	
		m_pStaticEnd = pAudio;		
	}

	pAudio = m_pStaticEnd;
	LpSAudio temp;

	while(m_nCurStaticMem > m_nMaxStaticMem && pAudio && pAudio!=m_pStaticStart)
	{
		if((pAudio->pSound) && ((CSound *)(pAudio->pSound))->IsSoundPlaying() ==0xFFFFFFFF)
		{
			pAudio = pAudio->pPrevAudio;
			continue;
		}
		if(pAudio->bLock)
		{
			pAudio = pAudio->pPrevAudio;
			continue;
		}
		temp = pAudio;
		pAudio = pAudio->pPrevAudio;
		DeleteAudio(temp);
	}

	m_nCurSoundCount ++;

	return true;
}

//根据位置衰减音量
//输入参数：listener positon（selfX，selfY）；sound source positon（otherX，otherY）,units:map tiles 
//			其中（0< otherX - selfX < 9）（0< |otherY - selfY| < 13）
//			
bool CAudio::AttenuationVol(int selfX,int otherX,int selfY,int otherY,long &vol,long & pan)
{TRY_BEGIN
	
	//
	pan = (long)_sqrt_(labs(otherX - selfX))*(otherX - selfX) * (labs(otherX - selfX)<4?220:140);
	if(pan > 9000)
		pan = 9000;
	else if(pan < -9000)
		pan= -9000;

	float disY = (float)(((labs(otherY-selfY) >= 12)?1:0.666)*labs(otherY - selfY)); 
	int distance = (int)(labs(otherX - selfX)*1.0>disY ?labs(otherX - selfX):disY);
	
	long lVolume = m_nVolumeSound_Last - 8 * distance;
	if(lVolume<0.1)
		lVolume = 0;//lVolume = 3;

	if(m_nVolumeSound_Last>=60 && lVolume < 9)
		lVolume = 9;
	
	vol = (long)(g_Audio_Vol_Gain* g_fLog[lVolume]);
	if(vol <-9000)
		vol = -8000;
	else if(vol>0)
		vol = m_nSqrtVolumeSound;
	return true;

TRY_END_RETURN(false)}

//
//输入参数：对应音源的衰减限度和当前tile位置；
//输出参数：pan && vol
bool CAudio::AttenuationVolEnv(int iType,UINT iIndex,int selfX,int otherX,int selfY,int otherY,long &vol,long & pan)
{
	//
	char *pLimit = GetName(iType,iIndex+1);//定义索引后一位为环境音效的limit；对于小音源（火盆），limit比较小；对于大音源（瀑布），limit较大；
	if(NULL == pLimit)
		return false;
	int iLimit = atoi(pLimit);//音源的衰减限度，单位：tile坐标

	////////////////////////////
	if(iLimit < 1 || iLimit > 12 )/////////////////当前仅支持1-12级内的衰减
		return false;

	int  ax = 0;
	int  dx = otherX - selfX;
	int  dy = otherY - selfY;
	if(dx < 0)
	{
		dx =-dx;
		ax = 1;
	}
	else if(dx == 0 && dy == 0)
	{
		vol = m_nVolumeSound_Last;
		pan = 0;
		return true;
	}

	if((dx) >  iLimit || (dy) >  iLimit )
		return false;

	int nVolIndex =(int) _sqrt_(dx * dx + dy * dy);		//绝对距离，限定为iLimit以内
	if(nVolIndex > iLimit)
		return false;

	int nPanIndex = (iLimit - dx) * AUDIO_MAX_VOLUME / iLimit;	
	if(nPanIndex > AUDIO_MAX_VOLUME)			
		nPanIndex = AUDIO_MAX_VOLUME;

	pan = (long)(g_fLog[nPanIndex ]*g_Audio_Pan_Gain);
	if(ax)
		pan = -pan;


	nVolIndex = (iLimit-nVolIndex) * AUDIO_MAX_VOLUME / iLimit;	//反函数，距离越远，音量越小

	if(nVolIndex < 0)
		nVolIndex = 0;

	if(nVolIndex > AUDIO_MAX_VOLUME)
		nVolIndex = AUDIO_MAX_VOLUME;

	vol = (long)(g_Audio_Vol_Gain* g_fLog[nVolIndex]);
	return true;
}

//计算新的方位和偏移
void AngleDistance(int selfX,int otherX,int selfY,int otherY,short &angle,unsigned char &distance)
{
	long xx = (otherX - selfX);
	long yy = (selfY - otherY);
	distance = (unsigned char) (_sqrt_(xx*xx + yy*yy) * 255/16);
	if(xx == 0)
	{
		if(yy > 0)angle = 0;
		else angle = 180;
	}
	else
	{
		double s = 90 - atan(fabs((double)yy/xx)) *180 / 3.1415926;
		if (xx > 0) {
			if (yy<0) {//四
				s = 180-s;
			}
		}
		else//第二。三
		{
			if (yy>0) {
				s = 360-s;
			}
			else
			{
				s = 180+s;
			}
		}
		if(s < 0)s+=360;
		else if(s > 360)s-=360;
		angle = (short)s;
	}
}

void CAudio::PlayEx(int iType,UINT iIndex, UINT nRand,int selfX,int otherX,int selfY,int otherY, bool bSelf,int bRand,bool bPlayLoop)
{
	TRY_BEGIN
	if(!m_bOK || !m_bSoundOK)
		return;

	int iPriority,iVol;
	char *pName = GetName(iType,iIndex,&iPriority ,&iVol);
	if(NULL == pName || pName[0]=='\0')
		return ;

	long lVol, lPan;

	//环境音效的特殊衰减，根据特定的环境衰减因子
	if(EAT_ENVIROMENT == iType)
	{
		if(!AttenuationVolEnv(iType,iIndex, selfX,otherX,selfY,otherY,lVol,lPan))	//计算衰减音量；根据位置
			return;
	}
	else 
	{
		if(!AttenuationVol(selfX,otherX,selfY,otherY,lVol,lPan))	//计算衰减音量；根据位置
			return;
	}	

	LpSAudio pAudio = NULL;
	pAudio = GetAudioToStart(iType, iIndex,otherX,otherY);//判断是否加载sound source!
	CSound * pStatic = NULL;

	//内存中存在同id样本
	if(pAudio)
	{
		if(!pAudio->pSound) return;	//不可能出现！

		pStatic = (CSound *)pAudio->pSound;
		DWORD dwPlay  = pStatic->IsSoundPlaying();

		if(dwPlay == 0)					
		{
			//产生错误,加载wave失败；仅当资源不存在时，可能发生；故直接返回
			DeleteAudio(pAudio);
			return;
		}
		else if(dwPlay == 0xFFFFFFFF)
		{
			//OutputDebugString("Play");
			//同id的样本正在播放,处理其音量和pan;返回
			pStatic->SetPan(lPan);
			pStatic->SetVol(lVol);
			return;
		}


		pAudio->nRand = nRand;
		if(bPlayLoop)
		{
			pStatic->Play(0,DSBPLAY_LOOPING ,lVol,-1,lPan);//DSBPLAY_LOOPING
			pAudio->bLoop =true;
			//Sleep(1000);
		}
		else
		{
			pStatic->Play(0,0 ,lVol,-1,lPan);//
			pAudio->bLoop = false;
		}
	}
	else
	{
        if(!AddAudio(iType,iIndex,bPlayLoop, nRand,otherX,otherY))
			return;


		pAudio = m_pStaticStart;		//add后，总是位于开始位置
		pStatic = (CSound * )pAudio->pSound;

		if( !pAudio ||!pAudio->pSound) 
			return;

		if(bPlayLoop)
		{
			pStatic->Play(0,DSBPLAY_LOOPING ,lVol,-1,lPan);//DSBPLAY_LOOPING
			pAudio->bLoop =true;
		}
		else
		{
			pStatic->Play(0,0 ,lVol,-1,lPan);//
			pAudio->bLoop = false;
		}
	}
	TRY_END_NOTHROW
}

void CAudio::Play(int iType,UINT iIndex,UINT nRand,bool bPlayLoop,bool bLimitPlayNum, int nMaxNum,UINT nType)
{TRY_BEGIN
	if(!m_bOK || !m_bSoundOK)
		return;

	//每个样本都有默认的优先级和音量;iVol应该为衰减级别,以获得真实比例音量
	int iPriority,iVol;
	char *pName = GetName(iType,iIndex,&iPriority ,&iVol);
	if(NULL == pName || pName[0]=='\0')
		return ;

	LpSAudio pAudio = NULL;
	pAudio = GetAudioToStart(iType, iIndex);//得到空闲的buffer(已经有了wave)
	
	CSound * pStatic = NULL;
	if(pAudio)
	{
		if(!pAudio->pSound) return;			//do u delete this ??

		pStatic = (CSound *)pAudio->pSound;
		DWORD dwPlay  = pStatic->IsSoundPlaying();

		if(dwPlay == 0)					//释放pStatic;
		{
			DeleteAudio(pAudio);//释放pStatic;
			return;
		}
		if(dwPlay == 0xFFFFFFFF)
		{
			//正在播放
			//OutputDebugString("Playing！！");
			return;
		}

		pAudio->nRand = nRand;
		if(bPlayLoop)
		{
			pStatic->Play(0,DSBPLAY_LOOPING ,m_nSqrtVolumeSound,-1,0);//DSBPLAY_LOOPING
			pAudio->bLoop =true;
			//Sleep(1000);
		}
		else
		{
			pStatic->Play(0,0 ,m_nSqrtVolumeSound,-1,0);//
			pAudio->bLoop = false;
		}

	}
	else//paudio==NULL
	{
        if(!AddAudio(iType, iIndex,bPlayLoop, nRand))
			return;

		pAudio = m_pStaticStart;
		pStatic = (CSound * )pAudio->pSound;
	
		if( !pAudio ||!pAudio->pSound) 
			return;

		if(bPlayLoop)
		{
			pStatic->Play(0,DSBPLAY_LOOPING ,m_nSqrtVolumeSound,-1,0);//DSBPLAY_LOOPING
			pAudio->bLoop =true;
		}
		else
		{
			pStatic->Play(0,0 ,m_nSqrtVolumeSound,-1,0);//
			pAudio->bLoop = false;
		}
	}
TRY_END_NOTHROW}

void CAudio::PlayMidi(DWORD dwFileName, bool bRepeat,int nSourceX,int nSourceY,int nListenerX,int nListenerY)
{
	PERF_STARTCHECK(20);
	if(!m_bMidiOK || dwFileName== 0)
		return;

	if(m_pSegment && dwFileName != m_dwMidiFileName)
	{
		StopMidi(m_dwMidiFileName);

		if( m_pLoader )
		{
			m_pLoader->ReleaseObjectByUnknown( m_pSegment );
		}
		SAFE_RELEASE(m_p3DAudioPath);
		SAFE_RELEASE(m_pSegment);
		SAFE_RELEASE(m_pDSB);
	}

	HRESULT hr = S_OK;
	//IDirectMusicAudioPath8* p3DAudioPath = NULL;
	//IDirectSound3DBuffer8* pDSB = NULL;
	__try
	{
		char strFileName[MAX_PATH] = {0};
		sprintf(strFileName,"%d.mid",dwFileName);
		WCHAR wstrFileName[MAX_PATH];
		MultiByteToWideChar( CP_ACP, 0, strFileName, -1, wstrFileName, MAX_PATH );

		if(dwFileName != m_dwMidiFileName)
		{
			if( m_pLoader )
				m_pLoader->CollectGarbage();

			if( FAILED( hr = m_pLoader->LoadObjectFromFile( CLSID_DirectMusicSegment,
				IID_IDirectMusicSegment8,
				wstrFileName,
				(LPVOID*) &m_pSegment ) ) )
				__leave;

			// Download the segment's instruments to the synthesizer
			if(m_pSegment)
				m_pSegment->Download( m_pPerformance );

			if(FAILED (hr = m_pPerformance->CreateStandardAudioPath( DMUS_APATH_DYNAMIC_3D, 
				64, TRUE, &m_p3DAudioPath )))
				__leave;


			if(m_b3DMidi)
			{
				if(FAILED(hr = m_p3DAudioPath->GetObjectInPath( DMUS_PCHANNEL_ALL, DMUS_PATH_BUFFER, 0, 
					GUID_NULL, 0, IID_IDirectSound3DBuffer, 
					(LPVOID*) &m_pDSB )))
					__leave;
			}
		}

		if(bRepeat && m_pSegment)
			m_pSegment->SetRepeats( DMUS_SEG_REPEAT_INFINITE );
		// Play segment on the 3D audiopath
		if(m_pPerformance)
			m_pPerformance->PlaySegmentEx( m_pSegment, NULL, NULL, 0, 0, NULL, NULL, m_p3DAudioPath );

		// Set the position of sound a little to the left
		if(m_b3DMidi) 
		{
			if(m_pDSB)
				m_pDSB->SetPosition( (nSourceX-nListenerX)/48.0f, (nSourceY-nListenerY)/24.0f, 0.0f, DS3D_IMMEDIATE );
		}
		m_dwMidiFileName = dwFileName;
		// Cleanup all interfaces
	}
	__finally
	{
		if(FAILED(hr))
		{
			if(m_pSegment)
				SAFE_RELEASE(m_pSegment);
			if(m_p3DAudioPath)
				SAFE_RELEASE(m_p3DAudioPath);
			m_bMidiOK = false;
		}
	}
	PERF_ENDCHECK(20);
}

void CAudio::StopMidi(DWORD dwFileName)
{
	if(!m_bMidiOK)
		return;

	TRY_BEGIN
		if(dwFileName ==  m_dwMidiFileName || dwFileName==0)
		{
			if( m_pSegment == NULL || m_pPerformance == NULL )
				return ;
			m_pPerformance->Stop( m_pSegment, NULL, 0, DMUS_SEGF_BEAT );
		}
	TRY_END_DO(m_bMidiOK = false;)
}

void CAudio::SetPositionMidi(DWORD dwFileName,int nBufferX,int nBufferY,int nListenerX,int nListenerY)
{
	if(!m_bMidiOK)
		return;
	if(m_b3DMidi)
	{
		TRY_BEGIN
			if(m_pDSB && dwFileName == m_dwMidiFileName )
				m_pDSB->SetPosition((D3DVALUE)((nBufferX-nListenerX)/48.0), (D3DVALUE)((nBufferY - nListenerY)/24.0f),0.0f,DS3D_IMMEDIATE);
		TRY_END_DO(m_bMidiOK = false;)
	}
}

void CAudio::SetPan(int iType,UINT iIndex,UINT nRand,int selfX, int otherX,int selfY,int otherY)
{
TRY_BEGIN
	LpSAudio temp;   
	temp = GetAudio(iType, iIndex,nRand);

	if(temp)
		SetPan(temp,selfX,otherX,selfY,otherY);
	
TRY_END_NOTHROW
}

void CAudio::SetPan(LpSAudio temp,int selfX, int otherX,int selfY,int otherY)
{
	LONG lVol;

	LONG lPan = (long)_sqrt_(labs(otherX - selfX))*(otherX - selfX) *  (labs(otherX - selfX)<4?220:140) ;

	float disY  = (float)(0.666* labs(otherY - selfY));
	long distance = (long)(labs(otherX - selfX)*1.0 > disY?labs(otherX - selfX):disY);
	
	long lVolume = m_nVolumeSound_Last - 8 * distance;
	
	if(lVolume<0.1)
		lVolume = 3;
	if(m_nVolumeSound_Last>=60 && lVolume < 7)
		lVolume = 7;
	
	lVol = (long)(g_Audio_Vol_Gain*  g_fLog[lVolume]);//log10(100.0/lVolume);
	
	if(lVol <-9000)
		lVol = -8000;
	else if(lVol>0)
		lVol = m_nSqrtVolumeSound;

	//////////
	if(lPan>9000)
		lPan = 9000;
	else if(lPan<-9000)
		lPan= -9000;

	if(temp && temp->pSound)
	{
		((CSound *)(temp->pSound))->SetPan(lPan);
		((CSound *)(temp->pSound))->SetVol(lVol);
	}
}

LpSAudio CAudio::GetAudio(int iType,UINT iIndex,UINT nRand)
{TRY_BEGIN

	LpSAudio temp;
	temp = m_pStaticStart;

    while(temp)
	{
		if((temp->iIndex == iIndex && temp->iType==iType) && (nRand == 0||temp->nRand == nRand))
			break;
		temp = temp->pNextAudio;
	}
 
	if(temp && !temp->pSound)
	{
		DeleteAudio(temp);
		temp = NULL;
	}
	return temp;
TRY_END_NOTHROW
return NULL;
}

//淘汰一个低优先级别的并且正在播放的样本；

//当前的播放数大于允许播放个数时，必须强制wash
		//如果声音正在播放，并且其优先级低于增加音源的优先级，则清除该声音
		//目前定义了3级，1为最高，2次之，3最低

//优先删除不在播放的样本，不考虑其优先级别
LpSAudio CAudio::WashAudio(int iType,UINT iIndex)		
{
	int iPriority,iVol ;
	char *pName = GetName(iType,iIndex,&iPriority ,&iVol);
	if(!pName || !pName[0])
		return NULL ;
	//wash one 
	LpSAudio pAudioWash = NULL,pAudioTemp;

	pAudioTemp = m_pStaticStart;

	while(pAudioTemp)
	{
		if(pAudioTemp->pSound)	//永远成立！！！
		{
			DWORD dwPlaying = ((CSound *)(pAudioTemp->pSound))->IsSoundPlaying();
			if(dwPlaying ==0xFFFFFFFF)	//仅处理播放态样本,查找最小优先级样本
			{
				if(pAudioWash == NULL)	//查找第一个
				{
					pAudioWash = pAudioTemp;
				}
				else
				{
					if(pAudioWash->iPriority < pAudioTemp->iPriority)
					{
						pAudioWash = pAudioTemp;
					}
				}
			}
			else //不播放,不管其优先级而直接删除，这仍然会有可能导致
			{
				pAudioWash = pAudioTemp;
				DeleteAudio(pAudioWash);
				return pAudioWash;
			}
		}
		pAudioTemp = pAudioTemp->pNextAudio;
	}
	
	//仅当优先级小于当前的播放样本才可以删除.
	if(pAudioWash && pAudioWash->iPriority >= iPriority)
	{			
		DeleteAudio(pAudioWash);
		return pAudioWash;
	}
	return NULL;
}

//获得空余的内存cache
bool CAudio::GetFreeAudio(int & index)
{
	for(int i=0; i<g_Audio_Allow_Mem ;++i)
	{
		if(m_sAudio[i].nRand == 0xFFFFFFFF)//cache空闲标记
		{
			index = i;
			return true;
		}
	}

	return false;
}

void CAudio::Stop(int iType,UINT iIndex,UINT nRand)
{TRY_BEGIN
	LpSAudio temp;
	temp = GetAudio( iType,iIndex,nRand);

	if(temp && temp->pSound &&( !(temp->bLock))&& ((CSound *)(temp->pSound))->IsSoundPlaying() == 0xFFFFFFFF)
	{
		temp->bLock = true;
		if(nRand==0)
			temp->nRand = 1;
		if(!m_pStaticStopStart)
		{	
			m_pStaticStopStart = temp;
			m_pStaticStopEnd = temp;
		}
		else
		{
			temp->pNextStop = m_pStaticStopStart;
			m_pStaticStopStart = temp;
		}
	}
TRY_END_NOTHROW}

void CAudio::StopAll()
{
	TRY_BEGIN;

	LpSAudio pAudio;
	pAudio = m_pStaticStart;
	while(pAudio)
	{
		if(pAudio->pSound)
			Stop(pAudio->iType,pAudio->iIndex,0);
			//((CSound *)(pAudio->pSound))->Stop();
		pAudio = pAudio->pNextAudio;
	}

TRY_END_NOTHROW
}

//继续
void CAudio::ResumeAll()
{
	TRY_BEGIN;
	musicResumeAll();
	g_Audio_State = EAS_PLAYING;
	TRY_END_NOTHROW
}

//暂停
void CAudio::PauseAll()
{
	TRY_BEGIN;
	musicPauseAll();
	setAudioState(EAS_PAUSED);
	TRY_END_NOTHROW
}

void CAudio::RestartAll()
{
	TRY_BEGIN;
	musicRestartAll(0);
	setAudioState(EAS_PLAYING);

	LpSAudio pAudio;
	pAudio = m_pStaticStart;
	while(pAudio)
	{
		if(pAudio->pSound)
		{
			((CSound *)(pAudio->pSound))->Restart(pAudio->bLoop?1:0,0);
			pAudio->bIsPlaying = false;
		}
		pAudio = pAudio->pNextAudio;
	}

	TRY_END_NOTHROW
}

void CAudio::SetAllVol()
{
	TRY_BEGIN;
	//设置音乐音量
	if(0<= m_nVolumeMusic &&  m_nVolumeMusic<=100)//SDL_MIX_MAXVOLUME
		musicVolume(m_nVolumeMusic);

	if( 0<= m_nVolumeSound &&  m_nVolumeSound<=100)//SDL_MIX_MAXVOLUME
		soundVolume((long)(m_nVolumeSound * 1.28));
	else
		return;

	long lVolume = m_nVolumeSound;
	
	if(lVolume != m_nVolumeSound_Last) //只在音量发生变化时候作一次开方
	{
		m_nVolumeSound_Last = lVolume;
		m_nSqrtVolumeSound = (long)(g_Audio_Vol_Gain * g_fLog[lVolume]);//log10(100.0/lVolume);
	}

	LpSAudio pAudio;
	pAudio = m_pStaticStart;	
	while(pAudio)
	{
		if(pAudio->pSound && ((CSound *)(pAudio->pSound))->IsSoundPlaying() ==0xFFFFFFFF)
			
			((CSound*)(pAudio->pSound))->SetVol(m_nSqrtVolumeSound);
		pAudio = pAudio->pNextAudio;
	}

TRY_END_NOTHROW
}

//删除未播放的多余样本
void CAudio::CutAudio()
{
	LpSAudio pAudio,temp;

	m_nCurSoundCount = 0;
	pAudio = m_pStaticStart;

	int nLoop=0;
	while(pAudio)		//播放队列;最近不使用原则.
	{
		++nLoop;
		if(pAudio->pSound && ((CSound *)(pAudio->pSound))->IsSoundPlaying() ==0xFFFFFFFF)
		{
			pAudio = pAudio->pNextAudio;
			m_nCurSoundCount ++;
		}
		else
		{
			//如果不在播放，并且内存中超过30个样本，删除多余的样本
			temp = pAudio;
			pAudio = pAudio->pNextAudio;
			if(nLoop>=g_Audio_Allow_Play)
			{
				DeleteAudio(temp);
			}
		}
	}	
}

void CAudio::SetVolumeSound(long l)// 0 ~~100
{ 
	m_nVolumeSound = l;

	if(m_nVolumeSound < 0)
		m_nVolumeSound = 0;
	else if(m_nVolumeSound > 100)
		m_nVolumeSound = 100;

	SetAllVol();
} 

void CAudio::SetVolumeMusic(long l)// 0 ~~100
{  
	m_nVolumeMusic = l; 

	if(m_nVolumeMusic < 0)
		m_nVolumeMusic = 0;
	else if(m_nVolumeMusic > 100)
		m_nVolumeMusic = 100;
TRY_BEGIN
	musicVolume(m_nVolumeMusic);
TRY_END
} 

inline UINT& CAudio::GetRand()
{
	return g_nRand;
}

int& CAudio::GetAudioRain()
{
	return g_Audio_Rain;
}

int& CAudio::GetAudioBlood()
{
	return g_Audio_Blood;
}

//playing?
bool	CAudio::MusicPlaying(char *cMusicName)		
{TRY_BEGIN
return musicPlaying(cMusicName)	;
TRY_END_RETURN(false)
}

//Pause
void	CAudio::MusicPause(char *cMusicName)	
{TRY_BEGIN
musicPause(cMusicName);
TRY_END_NOTHROW}

//paused?
bool	CAudio::MusicPaused(char *cMusicName)	
{TRY_BEGIN
return musicPaused(cMusicName);
TRY_END_RETURN(false)}

//resume
void	CAudio::MusicResume(char *cMusicName)	
{TRY_BEGIN
musicResume(cMusicName);
TRY_END_NOTHROW}

//暂停所有
void	CAudio::MusicPauseAll()
{TRY_BEGIN
musicPauseAll();
TRY_END_NOTHROW}

//继续所有
void	CAudio::MusicResumeAll()
{TRY_BEGIN
musicResumeAll();
TRY_END_NOTHROW}

//stop with fading time
void	CAudio::MusicStop(char *cMusicName,int ms)	
{TRY_BEGIN
musicStop(cMusicName, ms)	;
TRY_END_NOTHROW}

//just stop,should we stop the device ??
//for and the speed,we should do;
//but ,
void	CAudio::MusicStopAll(int ms)	
{TRY_BEGIN
musicStopAll(ms);
TRY_END_NOTHROW}

void	CAudio::MusicRestartAll(int ms)	
{TRY_BEGIN
if(!m_bMusicOK)	return ;
musicRestartAll( ms)	;
TRY_END_NOTHROW}

//stoped?
bool	CAudio::MusicStoped(char *cMusicName)		
{TRY_BEGIN
return musicStoped(cMusicName);
TRY_END_RETURN(true)}

//close the music source
void	CAudio::MusicClose(char *cMusicName,int ms)
{TRY_BEGIN
musicClose(cMusicName, ms);
TRY_END_NOTHROW}

//stop the first,play the second
void	CAudio::MusicSwitch(char *cMusicNameFirst,char *cMusicNameSecond,int ms)
{TRY_BEGIN
if(!m_bMusicOK)	return ;
musicSwitch(cMusicNameFirst,cMusicNameSecond, ms);
TRY_END_NOTHROW}

bool	CAudio::MusicValidate(char *cMusicName)	//check file is exsiting
{TRY_BEGIN
return musicValidate(cMusicName);
TRY_END_RETURN(0);}


void CAudio::PlayMusic(int iType,UINT iIndex,bool bPlayLoop)
{
	if(!m_bMusicOK || !m_bBkSoundOK)
	{
		return;
	}

	int iPriority,iVol;
	char *pName = GetName(iType,iIndex,&iPriority ,&iVol);
	if(NULL == pName || pName[0]=='\0')
		return ;

	StopAllMusic();//目前只有一个背景,所以播放一个新的音乐之前,先关闭之前的

	long lVolume = m_nVolumeMusic;
	if( 0<= lVolume &&  lVolume<=100  )//SDL_MIX_MAXVOLUME
	{
		lVolume = long(lVolume * 1.28 * 0.9);
	}
	else 
		lVolume = 0;

	char strFileName[MAX_PATH] = {0};
	sprintf(strFileName,"%s.ogg",pName);
TRY_BEGIN
	musicPlay(strFileName, 0 ,lVolume,bPlayLoop?0x7fffffff:1);
TRY_END
}

void CAudio::StopMusic(int iType,UINT iIndex)
{
	if(!m_bMusicOK || !m_bBkSoundOK)
	{
		return;
	}

	int iPriority,iVol;
	char *pName = GetName(iType,iIndex,&iPriority ,&iVol);
	if(NULL == pName || pName[0]=='\0')
		return ;
TRY_BEGIN
	musicStop(pName,0);
TRY_END
}

bool CAudio::IsMusicPlaying(int iType,UINT iIndex)
{
	if(!m_bMusicOK || !m_bBkSoundOK)
	{
		return false;
	}

	int iPriority,iVol;
	char *pName = GetName(iType,iIndex,&iPriority ,&iVol);
	if(NULL == pName || pName[0]=='\0')
		return false;
	
	return MusicPlaying(pName);
}

void CAudio::StopAllMusic()
{
TRY_BEGIN
	musicStopAll(0);
TRY_END
}
