#pragma once

#include "BaseClass/Graphic/TextureD3D.h"
#include "GameData/SimpleCharacter.h"

#define MAX_LINE_MAGIC				24
#define MAX_MAGIC_TYPE				60			// 最多魔法类型
#define TEXCACHE_TILEX				6
#define TEXCACHE_TILEY				8

#define DEFAULT_COLOR_STONE			0xFFFFFF80
#define DEFAULT_COLOR_BLACK         0xFF808080

#define ACTIONTYPE_NONE				0
#define ACTIONTYPE_STAND			1
#define ACTIONTYPE_ATTACK			2
#define ACTIONTYPE_MAGIC			3
#define ACTIONTYPE_OTHER			4

#define ALARM_DIS_TIME				300000		

#define LIGHT_MAX_COLOR				16			// 光影颜色的种类数
#define MAX_TALK_TEXTURES			10			// 对话框最大低图

enum TimeCheck_e
{
	LAST_MAGIC = 0,
	LAST_FIRE,
	LAST_COMBINE,
	LAST_STRENGTHEN,
	LAST_CONJURE,
	LAST_BUTTON_DOWN,
	LAST_F_KEY_DOWN,

	TIME_CHECK_NUM
};

struct stFocusType
{
	BYTE	cType;
	WORD	wDeepness;
};


struct LightColorData
{
	int		iX,iY;
	DWORD	dwColor;

	void Init(DWORD color,int x = 0,int y = 0)
	{
		iX		= x;
		iY		= y;
		dwColor	= color;
	}
};

struct stLineMagic
{
	stLineMagic(int x,int y,LPTexture pTex,DWORD color = 0xFFFFFFFF,DWORD dwBlend = 0,BOOL _bMapping= 0):
		ix(x),
		iy(y),
		pMagicTex(pTex),
		dwColor(color),
		dwBlendType(dwBlend),
		bMapping(_bMapping)
	{
	}
	int			ix,iy;
	LPTexture	pMagicTex;
	DWORD		dwColor;
	DWORD		dwBlendType;
	BOOL		bMapping;	//是否绕Y轴旋转。
};

typedef vector<stLineMagic> LineMagicArray;

struct stFollowChar
{
	BOOL					bFollow;
	BOOL					bSwitch;
	BOOL					bUpdate;
	char					sName[128];
	DWORD					dwID;
	CSimpleCharacterNode *	pChar;

	stFollowChar()
	{
		SetChar();
	}

	void SetChar(CSimpleCharacterNode *	p = NULL)
	{
		if(p)
		{
			bFollow		= TRUE;
			bSwitch		= TRUE;
			bUpdate		= FALSE;
			dwID		= p->GetID();
			pChar		= p;
			strcpy(sName,p->GetName());
		}
		else
		{
			bFollow		= FALSE;
			bSwitch		= FALSE;
			bUpdate		= FALSE;
			dwID		= 0;
			sName[0]	= 0;
			pChar		= NULL;
		}
	}
};

//物体层绘制顺序列表

#define MAX_LAYERS  255

struct stDrawScene
{
	stDrawScene(int x,int y, DWORD id)
	{
		sx = x;
		sy = y;
		texId = id;
	}
	int sx,sy;
	DWORD texId;
};

struct stDrawChar
{
	stDrawChar(int x,int y, CSimpleCharacterNode* p)
	{
		sx = x;
		sy = y;
		pChar = p;
	}
	int sx,sy;
	CSimpleCharacterNode* pChar;
};

typedef vector<stDrawScene> DrawSceneArray;
typedef vector<stDrawChar>  DrawCharArray;

