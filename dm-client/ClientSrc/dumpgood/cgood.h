///////////////////////////////////////////////////////////////////////
//文件名：   Good.h
//版权：上海盛大网络有限公司版权所有
//作者：	许猛
//创建日期：2003-1-23
//版本：	1.0
//文件说明：本文件是类CGood的头文件（包含实现）
//			CGood用来记录游戏中物品的详细信息。
//			由于每个玩家有一个唯一ID，所以CGood
//			重载了==，用ID来判断是否相等。
//
//注：		函数SetName在内存不足的时候会抛出异常。
///////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

class CGood
{
public:
	CGood()
	{
		clear();
	}
	void clear()
	{
		m_nID		= 0;
		m_stdMode	= 0;
		m_nShape	= 0;
		m_nWeight	= 0;
		m_nLooks	= 0;
		m_nDura		= 0;
		m_nDuraMax	= 0;
		m_nAC		= 0;
		m_nAC2		= 0;
		m_nMAC		= 0;
		m_nMAC2		= 0;
		m_nDC		= 0;
		m_nDC2		= 0;
		m_nMC		= 0;
		m_nMC2		= 0;
		m_nSC		= 0;
		m_nSC2		= 0;
		m_nNeed		= 0;
		m_nNeedLevel= 0;
		m_nPrice	= 0;
		m_lFlag		= 0;
		m_nFlag1	= 0;
		m_nFlag2	= 0;

		m_nDemonDark1 = 0;
		m_nDemonDark2 = 0;
		m_nDemonDark3 = 0;

		memset(m_cResvEx,0,10);

		m_strName.clear();
		m_ExternString.clear();
	}
	//by json 读取物品
	bool SetGoodInfo(const char* buf,int iLen)
	{
		if(iLen < 68)
		{
			clear();
			return false;
		}

		if(buf[0] < 0 || buf[0] > 14)
		{
			clear();
			return false;
		}
		//by json 传世物品结构
/*
TStdItem = packed record              //OK
Name: string[14];					
StdMode: Byte;				15
Shape: Byte;					16
Weight: Byte;				17
AniCount: Byte;				18
Source: Shortint;			19 (8字节) 27
Reserved: Byte;				28
NeedIdentify: Byte;		29
Looks: Word;				30
DuraMax: Word;
AC: Word;
MAC: Word;
DC: Word;
MC: Word;
SC: Word;
Need: Byte;
NeedLevel: byte;
b1: byte;                           //仙官装备所加属性类别  0：HP，1：DC，2：AC，3：MAC
b2: byte;                           //未使用
Price: Integer;
MakeIndex: Integer;
Dura: Word;
DuraMax: Word;
DarkProperty: TDarkProperty;
HYSSInfo: THYSSInfo;
wlen: Word;                         //固定为$28
memo: array[0..24] of Byte;
btY: Byte;
btLoYaoLi: Byte;
btHiYaoLi: Byte;
end;*/

		m_strName.assign(buf+1,buf[0]);
		m_stdMode = buf[15];
		m_nShape  = buf[16];
		m_nWeight = buf[17];
		m_lFlag   = *((DWORD *)(buf + 18));

		//by json


		//m_nLooks = *((WORD*)(buf + 22));
		m_nLooks = *((WORD*)(buf + 30));
		m_nOriDuraMax = *((WORD*)(buf + 24));

		m_nAC = buf[26];
		m_nAC2 = buf[27];
		m_nMAC = buf[28];
		m_nMAC2 = buf[29];
		m_nDC = buf[30];
		m_nDC2 = buf[31];
		m_nMC = buf[32];
		m_nMC2 = buf[33];
		m_nSC = buf[34];
		m_nSC2 = buf[35];
		m_nNeed = buf[36];
		m_nNeedLevel = buf[37];

		m_nFlag1 = *((WORD*)(buf+38));
		m_nPrice = *((WORD*)(buf+40));
		m_nFlag2 = *((WORD*)(buf+42));

		m_nID  = *((DWORD*)(buf + 44));
		m_nDura = *((WORD*)(buf + 48));
		m_nDuraMax = *((WORD*)(buf + 50));

		m_nDemonDark1 = *((WORD*)(buf + 52));
		m_nDemonDark2 = *((WORD*)(buf + 54));
		m_nDemonDark3 = *((WORD*)(buf + 56));

		memcpy(m_cResvEx,buf + 58,10);

		return true;

	}

	inline std::string	GetExternString(){return m_ExternString ;}
	inline unsigned long		GetID()			{ return m_nID; }
	inline const char * GetName()				
	{
		return m_strName.c_str(); 
	}

	inline int				GetStdMode()	{ return m_stdMode; }
	inline unsigned char	GetShape()		{ return m_nShape;	}
	inline unsigned char	GetWeight()		{ return m_nWeight;	}
	inline int				GetLooks()		{ return m_nLooks;	}
	inline unsigned int		GetDura()		{ return m_nDura;	}
	inline unsigned int		GetDuraMax()	{ return m_nDuraMax;}
	inline unsigned int		GetOriDuraMax()	{ return m_nOriDuraMax;}
	inline unsigned char	GetAC()			{ return m_nAC;		}
	inline unsigned char	GetAC2()		{ return m_nAC2;	}
	inline unsigned char	GetMAC()		{ return m_nMAC;	}
	inline unsigned char	GetMAC2()		{ return m_nMAC2;	}
	inline unsigned char	GetDC()			{ return m_nDC;		}
	inline unsigned char	GetDC2()		{ return m_nDC2;	}
	inline unsigned char	GetMC()			{ return m_nMC;		}
	inline unsigned char	GetMC2()		{ return m_nMC2;	}
	inline unsigned char	GetSC()			{ return m_nSC;		}
	inline unsigned char	GetSC2()		{ return m_nSC2;	}
	inline unsigned int		GetNeed()		{ return m_nNeed;	}
	inline unsigned int		GetNeedLevel()	{ return m_nNeedLevel; }
	inline unsigned int		GetPrice()		{ return m_nPrice;	}
	inline unsigned long	GetFlag()		{ return m_lFlag;	}
	inline int				GetFlag1()		{ return m_nFlag1;	}
	inline int				GetFlag2()		{ return m_nFlag2;	} 

	inline int     GetDemonDark1(){return m_nDemonDark1;}
	inline int     GetDemonDark2(){return m_nDemonDark2;}
	inline int     GetDemonDark3(){return m_nDemonDark3;}


	unsigned char&   GetResvEx(int i){return m_cResvEx[i];}
	unsigned char  GetDarkNeedLevel(){	return   m_cResvEx[5];}

	inline unsigned char GetAttachSkill() { return m_cResvEx[8];     }
	inline void          SetAttachSkill(unsigned char uAttachSkill){ m_cResvEx[8] = uAttachSkill;}
	inline unsigned char GetAttachSkillLevel()   { return m_cResvEx[9];}
	inline void          SetAttackSkillLevel(unsigned char uSkillLevel){ m_cResvEx[9] = uSkillLevel;}

	DWORD GetRecordTime(bool bFront = true)
	{
		DWORD m;

		if(bFront)
		{
			*((BYTE *)&m)		= m_nAC;		
			*((BYTE *)&m + 1)	= m_nAC2;
			*((BYTE *)&m + 2)	= m_nMAC;
			*((BYTE *)&m + 3)	= m_nMAC2;
		}
		else
		{
			*((BYTE *)&m)		= m_nDC;		
			*((BYTE *)&m + 1)	= m_nDC2;
			*((BYTE *)&m + 2)	= m_nMC;
			*((BYTE *)&m + 3)	= m_nMC2;
		}
		return m;
	}

	std::string GetPetName()
	{
		std::string strPetName;
		switch(m_nSC)
		{
		case 0:
			strPetName = "丛林豹";	
			break;
		case 1:
			strPetName = "踏云豹";	
			break;
		case 2:
			strPetName = "迅天雷豹";	
			break;
		case 8:
			strPetName = "天狮兽";	
			break;
		case 9:
			strPetName = "震天狮";	
			break;
		case 10:
			strPetName = "战天狂狮";	
			break;
		case 16:
			strPetName = "麒麟兽";	
			break;
		case 17:
			strPetName = "圣麒麟";	
			break;
		case 18:
			strPetName = "玄天麒麟";	
			break;
		default:
			strPetName = "";
			break;
		}

		return strPetName;
	}

	bool IsBind()
	{
		//特殊物品，该字段被使用了
		if(m_stdMode == 46 && m_nShape == 2) //邪心乾坤咒
			return false;

		if(m_stdMode == 3 && m_nShape == 213) //真灵玉符无需绑定，该字段被占用
			return false;

		if((m_nDemonDark3 & 0x0001) != 0)
			return true;
		else
			return false;
	}



private:
	unsigned long	m_nID;
	std::string 	m_strName; // string m_strName;
	int				m_stdMode;
	unsigned char	m_nShape;	// 形状
	unsigned int	m_nWeight;	// 重量
	int				m_nLooks;	// 外观
	unsigned int	m_nDura;	// 持久力
	unsigned int	m_nDuraMax; // 持久力上限
	unsigned int	m_nOriDuraMax; // 原有持久力上限
	unsigned char	m_nAC;		// 
	unsigned char	m_nAC2;
	unsigned char	m_nMAC;
	unsigned char	m_nMAC2;
	unsigned char	m_nDC;
	unsigned char	m_nDC2;
	unsigned char	m_nMC;
	unsigned char	m_nMC2;
	unsigned char	m_nSC;
	unsigned char	m_nSC2;
	unsigned int	m_nNeed;		// 
	unsigned int	m_nNeedLevel;	// 需要等级
	unsigned int	m_nPrice;
	unsigned long	m_lFlag;
	int				m_nFlag1;
	int				m_nFlag2;
	//1.907增加商城道具属性
	std::string		m_ExternString; //扩展文字

	int             m_nDemonDark1;//附魔残留信息
	int             m_nDemonDark2;//附魔残留信息
	int             m_nDemonDark3;//附魔残留信息

	unsigned char   m_cResvEx[10];

};
