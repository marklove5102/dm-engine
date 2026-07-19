#pragma once

#include "Global/Global.h"
#include "GameData/GameDefine.h"
#include "good.h"
#include "GameData/GuildData.h"

struct stDemonPackage 
{
	string name;
	DWORD looks;
};


class COtherPlayer
{
public:
	COtherPlayer(void);
	~COtherPlayer(void);

	bool IsMale();
	const char * GetName();
	const char * GetGuildName();
	const char * GetTitle();
	void  SetLooks(__int64 iLooks);
	LOOKS & GetLooks();
	DWORD GetNameColor();
	
	CGood &	 GetEquipGood(int i);
	void	Clear();
	DWORD   GetGuildLogoID();
	bool    GettingGuildLogo();
	bool    IsGettingGuildLogo();
	void    SetGettingGuildLogo(bool b);
	DWORD   GetLogoTexTime();
	void    SetLogoTexTime(DWORD dwTime);

	void    SetWing(bool b);
	bool    HaveWing();

	bool    IsGuildTowerSwitch(){ return m_bGuildTowerSwitch;}//获得行会塔是否开启
	bool    IsSelfGuildBufferEff(){ return m_bSelfGuildTowerSwitch;}

	BYTE    GetYuanShenInfo();
	bool    IsYuanShenMale();
	int     GetYSShapeType();
	void    SetYSShapeType(int i);

	string& GetIndividualStr();

	bool   FromBuffer(const char* buf,int iLen);

	bool   GetLogo();
	void   SetLogo(bool b);

    bool GetFightOnLeopard(){return m_bFightOnLeopard;}
    void SetFightOnLeopard(bool b){m_bFightOnLeopard = b;}

	bool GetPutOnShield(){return m_bPutOnShield;}
	void SetPutOnShield(bool b){m_bPutOnShield = b;}

	BYTE GetCareer(){return m_byJob;}
	void SetCareer(BYTE job){m_byJob = job;}
	DWORD GetID(){return m_dwPlayerID;}
	bool IsOnLepoard();
	bool IsOnBigLepoard();

	int     GetWearGold();		//穿戴蟠龙金甲及全套金甲首饰,0为穿戴傲天套装，1为穿戴幻天套装，2为穿戴玄天套装,-1为未穿戴

	int   GetGuildTowerLevel(){return m_iGuildTowerLevel;}
	CGuildData::_GuildBuffer& GetGuildBuffer(){return m_GuildBuffer;}

	void SetGuildPhyleState(BYTE byState){ m_byGuildPhyle = byState;}
	BYTE GetGuildPhyleState(){return m_byGuildPhyle;}

	const char * GetOfficer(){return m_strOfficer.c_str();}

	stDemonPackage* GetDemonPackage(int iIndex)
	{
		if(iIndex >= 0 && iIndex < 16)
			return &m_DemonPackages[iIndex];
		else
			return NULL;
	}

	const char* GetPhyleName(){return m_strPhyleName.c_str();}

	//int GetCharHolyWingLevel()			{ return m_iHolyWingLevel; }
	//int GetCharHolyWingStrongLevel()	{ return m_iHolyWingStrongLevel; }
	//void SetCharHolyWingLevel(int val)			{ m_iHolyWingLevel = val; }
	//void SetCharHolyWingStrongLevel(int val)	{ m_iHolyWingStrongLevel = val; }

	inline int    GetHolyWingLevel(){return m_equipment[ITEM_POS_WING].GetHolyWingLevel();}
	inline int    GetHolyWingStrongLevel(){return m_equipment[ITEM_POS_WING].GetHolyWingStrongLevel();}
	inline int    GetVipTradeLevel(){return m_iVipTradeLevel;}

	inline int    GetFaBaoLevel(){return m_iFaBaoLevel;}
	inline int    GetFaBaoType(){return m_iFaBaoType;}

	inline int	  GetFaBaoTypeLeft(){return m_iFaBaoTypeLeft;}
	inline int	  GetFaBaoTypeRight(){return m_iFaBaoTypeRight;}

	inline int    GetFaZhenLevel(){return m_iFaZhenLevel;}

private:
	DWORD               m_dwPlayerID;
	bool                m_bWing;
	LOOKS		        m_looks;
	DWORD		        m_NameColor;
	string				m_strName;
	string				m_strGuildName;
	string				m_strTitle;
	bool				m_bIsMale;
	bool				m_bGettingTex;
	DWORD				m_dwGettingTexTime;
	DWORD				m_dwLogoID;
	CGood				m_equipment[MAX_EQUIPMENT];
	BYTE                m_nYuanShenInfo;
	int                 m_iYuanShenSex;
    int                 m_iYSShapeType;//如果是其它玩家的元神的话,保存元神境界信息
	bool                m_bLogo;
	string				m_strIndividualStr; //个性化设置

    bool				m_bFightOnLeopard;	//骑战
	BYTE                m_byJob;	//职业

	bool                m_bPutOnShield;//是否佩戴盾牌

	int					m_iGuildTowerLevel;
	bool				m_bGuildTowerSwitch;//行会塔是否开启
	bool                m_bSelfGuildTowerSwitch;////行会buffBuffer加成对于玩家自己是否有效

	string              m_strOfficer;	//行会官职
	BYTE                m_byGuildPhyle;//行会修神还是修魔0是凡人1是修神2是修魔

	CGuildData::_GuildBuffer        m_GuildBuffer;
	stDemonPackage m_DemonPackages[16];

	string m_strPhyleName;

	int   m_iVipTradeLevel;

	int   m_iHolyWingLevel;//翅膀颜色，1：红色，2：蓝色，3：金色
	int   m_iHolyWingStrongLevel;//翅膀镶边颜色，1：红色，2：蓝色，3：金色

	int   m_iFaBaoType;
	int   m_iFaBaoLevel;

	int   m_iFaBaoTypeLeft;
	int   m_iFaBaoTypeRight;

	int   m_iFaZhenLevel;
};

#include "OtherPlayer.inl"