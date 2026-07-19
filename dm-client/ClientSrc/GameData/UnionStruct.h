#pragma once
#include "GameDefine.h"

struct FABAOStruct;

struct MonsterTex
{
	MonsterTex()
	{
		dwShapeTex = 0;
		dwArmTex = 0;
		dwHead = 0;
		dwHeadArm = 0;
		dwMonsterRight = 0;
		dwMonsterLeft = 0;
		dwMonsterEff = 0;
	}
	DWORD   dwShapeTex;
	DWORD   dwArmTex;
	DWORD   dwHead;//宠物头
	DWORD   dwHeadArm;//宠物头盔甲
	DWORD   dwMonsterLeft;	//怪物左边部分
	DWORD   dwMonsterRight;	//怪物右边部分
	DWORD   dwMonsterEff;
	DWORD	dwFaBao;		//怪物法宝
	DWORD	dwFaBaoEff;		//怪物法宝特效
	DWORD	dwMonsterLeftArm;
	DWORD	dwMonsterRightArm;
};

struct PlayerTex
{
	PlayerTex()
	{
		dwClothTex = 0;
		dwPatternTex = 0;
		dwHairTex = 0;
		dwWeaponTex = 0;
		dwWeaponEffTex = 0;
		dwHairTex = 0;
		dwShieldTex = 0;
		dwShieldEffTex = 0;


		dwWingShadowTex = 0;
		dwWingLeftTex = 0;
		dwWingRightTex = 0;
		dwWingShadowEffTex = 0;
		dwWingLeftEffTex = 0;
		dwWingRightEffTex = 0;

		dwMShapeTex = 0;
		dwMArmTex = 0;
		dwMHead = 0;
		dwMHeadArm = 0;
		dwMonsterRight = 0;
		dwMonsterLeft = 0;
		dwClothEffTex = 0;

	}
	DWORD   dwWingShadowTex;
	DWORD   dwWingShadowEffTex;
	DWORD   dwWingLeftTex;
	DWORD   dwWingLeftEffTex;
	DWORD   dwWingRightTex;
	DWORD   dwWingRightEffTex;
	DWORD	dwWeaponTex;
	DWORD   dwWeaponEffTex;
	DWORD   dwShieldTex;
	DWORD   dwShieldEffTex;


	DWORD   dwMShapeTex;
	DWORD   dwMArmTex;
	DWORD   dwMHead;//宠物头
	DWORD   dwMHeadArm;//宠物头盔甲
	DWORD   dwMonsterLeft;	//怪物左边部分
	DWORD   dwMonsterRight;	//怪物右边部分


	DWORD	dwClothTex;
	DWORD	dwPatternTex;
	DWORD	dwHairTex;
	DWORD   dwClothEffTex;

	DWORD	dwFaBao;			//法宝
	DWORD	dwFaBaoEff;			//法宝特效

	DWORD	dwMonsterLeftArm;
	DWORD	dwMonsterRightArm;
};

union LOOKS
{
	//0:类型 玩家0 怪1 npc心魔元神不变
	//1~2:附加类型:骑宠的appr,wHorse
	//3: weapon
	//4:hair
	//5~6:body
	//7:bodycolor
	struct
	{
		__int64 iLooks;
		BYTE  byType;
		WORD  wHorse;
		BYTE  byWeapon;
		BYTE  byHairType;
		BYTE  byHairColor;
		WORD  wBody;
		BYTE  byColor;
		BYTE  bySubType;//byType类里的子类，比如心魔的byType和普通人的byType都是CHARACTER_HUMAN
	} Player;
	//0: 类型 玩家0 怪1 npc心魔元神不变
	//1:未用
	//2,3:怪物编号appr(wShape)
	struct
	{
		__int64 iLooks;
		BYTE  byType;
		BYTE  byReserve1;
		WORD  wShape;
		WORD  wReserve;
	} Char;
};

enum DrawInfFlag
{
	DRAWINFO_FLAG_SELF = 0x000001,
};

struct DrawCharInf
{
	// 输入
	BOOL	bNeedRt;		// 是否需要输出
	LOOKS *	pLooks;			// 外观

	DWORD	dwColor;		// 底色
	BOOL    bAlpha;         // 是否alpha

	WORD    wBody;          // 衣服
	BYTE    byWing;         // 翅膀
	BYTE    byWingEff;      // 翅膀特效
	BYTE    byArm;          // 豹子Arm

	DWORD	dwFlag;			// 标记
	int		iDir;			// 方向
	int		iSex;			// 性别
	int		iJob;			// 职业
	int		iAction;		// 动作
	int		iDrawAction;	// 绘制动作 如wAction=ACTION_STAND时drawaction可以为ACTION_HSATAND,ACTION_LSTAND
	int		iFrame;			// 当前帧
	int     iActionFrames;  // 动作总帧数
	float	fScale;			// 缩放大小

	int     iPutOnShield;   //是否有盾牌,以及盾牌的种类
	//int     iGuildFlag;		//行会旗帜，用于绘制行会盾牌特效
	int     iShieldLevel;	//盾牌等级，用于绘制盾牌特效

	BOOL	bNotAlpha;		// 是否Alpha方式绘制
	UINT	uData;			// 其他
	int iScaleDrawOffX,iScaleDrawOffY;//用来表示缩放之后应该向左上角偏移多少了

	PlayerTex *pLast;			// 上一次帧信息

	// 输出
	int		iMouseIn;		// 鼠标点中信息
	FABAOStruct*	pFaBao;	//法宝类型
};

//摊位的绘制信息
struct DrawBoothInf
{
	DrawBoothInf()
	{
		iDir = DIR_UP;
		bBoothFirst = TRUE;
		iBoothType = 0;
		iOffX = iOffY = 0;
		iFlagOffX = iFlagOffY = 0;
		bFlagFirst  = FALSE;
		iFlagType   = 0;
		dwFlagColor = 0xFFFFFFFF;
	}
	int    iDir;
	int    iBoothType;
	BOOL   bBoothFirst;
	int    iOffX,iOffY;
	BOOL   bFlagFirst;
	int    iFlagOffX,iFlagOffY;
	int    iFlagType;
	DWORD  dwFlagColor;
};