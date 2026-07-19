#pragma once
#pragma warning(disable:4522) // disable: warning C4522: “CGood” : 指定了多个赋值运算符

#include <string>

typedef unsigned long DWORD;
typedef unsigned short WORD;

class CGoodPetInfo //宠物的附加属性
{
public:
	CGoodPetInfo();
	~CGoodPetInfo() {};

public:
	inline DWORD		        GetID()		    { return m_dwItemID;}
	inline WORD                 GetHuanHua()    { return m_nHuanhua;}
	inline unsigned char		GetAP()		    { return m_byAP;	}
	inline unsigned int		    GetHP()		    { return m_nHP;	    }
	inline unsigned int  		GetMP()		    { return m_nMP;	    }
	inline unsigned char		GetAC()			{ return m_nAC;		}
	inline unsigned char		GetAC2()		{ return m_nAC2;	}
	inline unsigned char		GetMAC()		{ return m_nMAC;	}
	inline unsigned char		GetMAC2()		{ return m_nMAC2;	}
	inline unsigned char		GetDC()			{ return m_nDC;		}
	inline unsigned char		GetDC2()		{ return m_nDC2;	}
	inline unsigned char		GetMC()			{ return m_nMC;		}
	inline unsigned char		GetMC2()		{ return m_nMC2;	}
	inline unsigned char		GetSC()			{ return m_nSC;		}
	inline unsigned char		GetSC2()		{ return m_nSC2;	}
	inline unsigned char		GetHit()		{ return m_nHit;	}
	inline unsigned char		GetMHit()		{ return m_nMHit;	}
	inline unsigned char		GetPoisonDef()	{ return m_nPoisonDef;}
	inline unsigned char		GetMDef()		{ return m_nMDef;	}
	inline unsigned char		GetLuck()	    { return m_nLuck;   }
	inline unsigned char		GetSpeed()		{ return m_nSpeed;	}
	inline unsigned char        GetPoisonHit()  { return m_nPoisonHit;}
	inline unsigned char		GetDef()		{ return m_nDef;}
	inline unsigned char		GetDunWu()		{ return m_nDunWu;}
	inline int					GetHurt1()		{ return m_iHurt1;}
	inline int					GetHurt2()		{ return m_iHurt2;}
	inline unsigned char        GetType()       { return m_nType;}
	inline unsigned char        GetLevel()      { return m_nLevel;}
		
	inline void	SetID(unsigned char dwID)		{ m_dwItemID= dwID;}
	inline void SetHuanHua(WORD nHuanHua)       { m_nHuanhua= nHuanHua;}
	inline void	SetAP(unsigned char byAP)		{ m_byAP	= byAP;}
	inline void	SetHP(unsigned char nHP)		{ m_nHP		= nHP; }
	inline void	SetMP(unsigned char nMP)		{ m_nMP 	= nMP; }
	inline void	SetAC(unsigned char nAC)		{ m_nAC		= nAC; }
	inline void	SetAC2(unsigned char nAC2)		{ m_nAC2	= nAC2;}
	inline void	SetMAC(unsigned char nMAC)		{ m_nMAC	= nMAC; }
	inline void	SetMAC2(unsigned char nMAC2)	{ m_nMAC2	= nMAC2;}
	inline void	SetDC(unsigned char nDC)		{ m_nDC		= nDC;  }
	inline void	SetDC2(unsigned char nDC2)		{ m_nDC2 = nDC2; }
	inline void	SetMC(unsigned char nMC)		{ m_nMC  = nMC;  }
	inline void	SetMC2(unsigned char nMC2)		{ m_nMC2 = nMC2; }
	inline void	SetSC(unsigned char nSC)		{ m_nSC  = nSC;  }
	inline void	SetSC2(unsigned char nSC2)		{ m_nSC2 = nSC2; }

	inline void	SetHit(unsigned char nHit)		{ m_nHit = nHit; }
	inline void	SetMHit(unsigned char nMHit)	{ m_nMHit = nMHit;}
	inline void	SetPoisonDef(unsigned char nPoisonDef)	{ m_nPoisonDef  = nPoisonDef;  }
	inline void	SetMDef(unsigned char nMDef)	{ m_nMC2 = nMDef;}
	inline void	SetLuck(unsigned char nLuck)	{ m_nSC  = nLuck;}
	inline void	SetSpeed(unsigned char nSpeed)	{ m_nSpeed = nSpeed; }
	inline void SetPoisonHit(unsigned char nPoisonHit){ m_nPoisonHit = nPoisonHit;}
	inline void SetDef(unsigned char nDef)		{m_nDef = nDef;}
	inline void SetDunWu(unsigned char nDunWu)	{m_nDunWu = nDunWu;}

	void SetValue(const char *str,const int iLen);
	void operator = (CGoodPetInfo& GoodPet);
	void operator = (const CGoodPetInfo& GoodPet);
	void clear();
		

private:
	DWORD                   m_dwItemID; // 物品指针ID
	WORD                    m_nHuanhua; // 幻化次数        
	unsigned char           m_byAP;     // 资质
	unsigned int			m_nHP;		// 生命值
	unsigned int			m_nMP;		// 魔法值
	unsigned char			m_nMAC;		// 魔法防御下限
	unsigned char			m_nMAC2;	// 魔法防御上限
	unsigned char			m_nMC;		// 魔法攻击下限
	unsigned char			m_nMC2;		// 魔法攻击上限
	unsigned char			m_nSC;		// 道法攻击下限
	unsigned char			m_nSC2;		// 道法攻击上限
	unsigned char			m_nAC;		// 物理防御下限
	unsigned char			m_nAC2;		// 物理防御上限
	unsigned char			m_nDC;		// 物理攻击下限
	unsigned char			m_nDC2;		// 物理攻击上限
    unsigned char			m_nHit;		// 命中
	unsigned char			m_nMHit;	// 魔法命中
	unsigned char			m_nPoisonDef;// 中毒躲避
	unsigned char			m_nMDef;	// 魔法躲避
	unsigned char			m_nLuck;	// 幸运
	unsigned char			m_nSpeed;	// 灵兽移动速度
	unsigned char           m_nPoisonHit;// 中毒命中
	unsigned char           m_nDef;		//物理躲避
	unsigned char           m_nDunWu;    //顿悟
	int						m_iHurt1;	//伤害1索引
	int						m_iHurt2;	//伤害2索引
	unsigned char           m_nType;
	unsigned char           m_nLevel;
};

