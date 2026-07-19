#pragma once

#include "GameDefine.h"
#include "UnionStruct.h"
#include "Global/Global.h"


enum ACTION_PIC_TYPE
{
	APT_NORMAL            = 0,      //正常的动作
	APT_RIDE_HORSE           ,      //骑马
	APT_RIDE_LEPOARD         ,      //骑豹子
	APT_RIDE_ATTACK_LEAPOARD ,      //骑了2转可以骑战的豹子
};

enum MONSTER_ARM_TYPE
{
	MAT_ZHUAN_0 = 0,
	MAT_ZHUAN_1,
	MAT_ZHUAN_2,
	MAT_ZHUAN_3,

	MAT_FIRST_BAO,	//天下第一豹
	MAT_RIDE_BODY,	//骑乘凤凰身体
	MAT_BODY = 10,
	MAT_HEAD,
	MAT_RIDE_LEFT,	//骑乘凤凰左侧
	MAT_RIDE_RIGHT,	//骑乘凤凰右侧

	MAT_ARM_TYPE_NUMBER,
};

enum PACKAGE_TYPE
{
	PICPACKAGE_NULL		 = 0,
	PICPACKAGE_HUMAN     = 1,
	PICPACKAGE_HAIR			,
	PICPACKAGE_PATTERN		,
	PICPACKAGE_MONSTER		,
	PICPACKAGE_WEAPON		,
	PICPACKAGE_WEAPONEFF	,
	PICPACKAGE_FABAO		,
	PICPACKAGE_FABAOEFF		,
	PICPACKAGE_MONSTER_ARM,
	PICPACKAGE_NUM,
};
// 动作图片信息
struct stActionPic
{
	stActionPic()
	{
		iStartIndex = 0;
		iFrameMax = 0;
		iFrameReal = 0;
		iFrameSpeed = 0;
		iInterval = 0;
	}
	stActionPic(int start,int mmx,int real,int speed, int interval = 0)
	{
		iStartIndex = start;
		iFrameMax = mmx;
		iFrameReal = real;
		iFrameSpeed = speed;
		iInterval = interval;
	}
	int	iStartIndex;				// 图片起始帧索引
	int iFrameMax;					// 最大帧数
	int	iFrameReal;					// 实际帧数
	int iFrameSpeed;                // 播放速度
	int iInterval;					// 间隔
};

struct stWingPic
{
	stWingPic()
	{
		iPackage = 0;
		iStartIndex = 0;
		iFrameLeft = 0;
		iFrameRight = 0;
		iFrameShadow = 0;
	}
	stWingPic(int package,int start,int left,int right,int shadow)
	{
		iPackage = package;
		iStartIndex = start;
		iFrameLeft = left;
		iFrameRight = right;
		iFrameShadow = shadow;
	}
	int iPackage;                   // 图包编号0:MAGIC1,1:MAGIC2
	int	iStartIndex;				// 图片起始帧索引
	int iFrameLeft;					// 左边翅膀测试
	int	iFrameRight;				// 右边翅膀测试
	int iFrameShadow;               // 播放速度
};

enum DrawPart
{
	DP_WingShadow = 0,
	DP_WingShadowEff,
	DP_WingLeft,
	DP_WingLeftEff,
	DP_WingRight,
	DP_WingRightEff,
	DP_Weapon,
	DP_WeaponEff,
	DP_Shield,
	DP_ShieldEff,
	DP_MonsterBody,
	DP_MonsterArm,
	DP_MonsterHead,
	DP_MonsterHeadArm,
	DP_MonsterLeft,
	DP_MonsterRight,
	DP_Cloth,
	DP_Pattern,
	DP_Hair,	
	DP_ClothEff,
	DP_FaBao,						//法宝
	DP_FaBaoEFF,					//法宝特效
	DP_MonsterLeftArm,
	DP_MonsterRightArm,

	DP_DrawPartEffectNum,//最多有效层


	DP_DrawPartMaxNum = 50,//最大可层数,配置文件里每个动作,方向的每一帧有DP_DrawPartMaxNum项顺序
};

//单个动作单个方向单帧的绘制顺序
struct SDrawOrder_Dir_Frame
{
	SDrawOrder_Dir_Frame()
	{
		memset(byOrder,255,DP_DrawPartMaxNum);
	}

	BYTE byOrder[DP_DrawPartMaxNum];

};

//单个动作的绘制顺序
struct SDrawOrder
{
	SDrawOrder()
	{
		iFrames = 0;
		pData = NULL;
	}
	SDrawOrder(int frames,SDrawOrder_Dir_Frame *data)
	{
		iFrames = frames;
		pData = data;
	}

	int iFrames;//帧数
	SDrawOrder_Dir_Frame *pData;//8个方向,每个方向帧数都为iFrames,8*iFrames项
};

typedef map<int,SDrawOrder> DrawOrderMap;


class CActionInf
{
public:
	CActionInf(void);
	~CActionInf(void);

	typedef map<int,BYTE*> ArrayMap;
	typedef map<int,stWingPic> WingMap;
	typedef map<int,stActionPic> ActionMap;
	PlayerTex	 GetPlayerTex(DrawCharInf* pDrawInf);
	MonsterTex   GetMonsterTex(DrawCharInf* pDrawInf); //获得怪物图片
	MonsterTex   GetMonsterTex(int iMonster,int iAction,int iFrame,char cDir,BYTE byArm,BYTE byReborn = 0,bool bFightOnLeopard = false,FABAOStruct* pFaBao = NULL);
	DWORD        GetNPCTex(DrawCharInf* pDrawInf);
	DWORD        GetNPCTex(int iNPC,int iAction,int iFrame,char cDir);
	BOOL         IsRideLepoard(DrawCharInf* pDrawInf);
	void         GetWingsTex(DrawCharInf* pDrawInf,PlayerTex& tex);
	static int	 GetDrawMonsterAppr(int iMonsterAppr,BYTE& reborn);

	BOOL         IsWeaponFirst(DrawCharInf* pDrawInf,BOOL& bShieldFirst);
	BOOL         IsWingsFirst(DrawCharInf* pDrawInf,BOOL bLeft);
	void		 CacheHumanLooks(const PlayerTex &TEX,int iFrameNow,int iFrameCount,eReadPrior ePrior);
	void		 CacheMonsterLooks(const MonsterTex &MTEX,int iFrameNow,int iFrameCount,eReadPrior ePrior);

	void         GetBoothInfo(int iDir,DrawBoothInf* pDrawInf);
	int          GetPackage(int iType,int iIndex,int iStartIndex,int iBody = 0);
	int			 ConvertAction(int iAction,int iType);
	stActionPic* GetActionPic(int iAction,int iRaceType,int iRaceNo,int iType = APT_NORMAL);

	void         clear();
	SDrawOrder_Dir_Frame * Get_Draw_Order_Dir_Frame(int iAction,int iDir,int iFrame);
	void			  LoadDATFile();
private:
	ActionMap	       m_vPlayer;	  // 人物图片信息
	ActionMap          m_vNpc;         // NPC图片信息
	ActionMap          m_vMonster;     // 默认的图片信息
	//ActionMap		   m_vMonsterArm;	//怪物装备图片信息
	ActionMap		   m_vFaBao;	//怪物装备图片信息
	vector<ActionMap>  m_Monster;  // 特殊怪物图片
	vector<WingMap>    m_Wing;         // 翅膀信息
	vector<WingMap>    m_WingEff;      // 翅膀特效
	std::map<DWORD,DWORD> m_mapMonsterArm;			//怪物盔甲映射

	int               GetRideAction(int iAction,int iDrawAction);
	int				  GetWingsAction(DrawCharInf* pDrawInf,bool bEffected = false);

	DrawOrderMap      m_MDrawOrder;  // 绘制顺序表
	WORD m_PackageIndex[PICPACKAGE_NUM][4];
};

extern CActionInf g_ActionInf;
