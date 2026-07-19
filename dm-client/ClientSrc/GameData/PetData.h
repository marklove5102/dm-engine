#pragma once

#include "Global/Global.h"
#include "GoodPet.h"

#include <map>

struct PET_ADOPT   //宠物领养领回数据结构
{
	DWORD  dwPetItemID;//豹魔石流水号
	char   szPetName[15];//豹子名,（兽名魔咒命名的）
	BYTE   byPetType;//豹子类型(0丛林豹，1踏云豹)
	BYTE   byPetLevel;//豹子等级
	BYTE   byStatus;//状态,1寄养中未领养，2领养中
	DWORD  tLastEatTime;//豹子最后喂食时间
	char szJYPlayerName[15];//寄养者角色名
	char szLYPlayerName[15];//领养者角色名
	char szReserved[8];
};

typedef std::map<DWORD,PET_ADOPT> PET_ADOPT_MAP;

struct PET_HORSE
{
	PET_HORSE()
	{
		wAppr = 0;
		bStatus = 0;
		byHPPercent = 0;
	}

	WORD wAppr;
	BYTE   byHPPercent;//血条百分比
	BYTE bStatus;
};

struct PET_SENDOUT	    //放出来的豹子的数据结构
{
	DWORD  dwPetItemID;//兽魔石ID
	char   szPetName[15];//宠物名,（兽名魔咒命名的）
	BYTE   byPetType;//宠物类型,bit[0~2] 转生次数0，1，2，3，4.。。bit[3~7] 宠物类型 0-豹子 1-狮子 2-麒麟
	BYTE   byPetLevel;//宠物等级
	WORD   wPetExp;//宠物经验
	BYTE   byPetColor;//宠物颜色,0普通，1白，2黑
	__time32_t tLastEatTime;//宠物最后喂食时间
	BYTE   byFullTime;//宠物饱食最长时间(天)
	DWORD  dwMaxSorbExp;//宠物吸收的经验上限
	DWORD  dwSorbExp;//宠物吸收的经验
	BYTE   byHPPercent;//血条百分比	
	DWORD  dwTodayReleasedExp;//玩家今天吸收了多少经验
	DWORD  dwMaxCheapExpPerDay;//低价经验，每天最多1000万
	DWORD  dwCheapExpPerFullDay;//每天饱食度，可以吸取的低价经验28万
	DWORD  dwExpensiveExpPerFullDay;//每天饱食度，可以吸取的高价经验20万 
	char   szReserved[8];//szReserved[0] ==0未释放 ==1释放出来跟随 ==2本体骑乘，==3摆摊，==4 下马，==5 改变颜色，==6本体骑战，==7元神骑乘，==8元神骑战 ==9凤凰探宝
};

struct PET_STATUS
{
	BYTE cPetLevel;
	char cPetName[15];
	WORD wHp,wHpMax;
	BYTE cDc,cDcMax;
	BYTE cAc,cAcMax;
};

typedef std::vector<PET_STATUS> PET_STATUS_VECTOR;
typedef std::map<DWORD,PET_SENDOUT> PET_SENDOUT_MAP;

class CPetData
{
public:
	CPetData(void);
	~CPetData(void);

public:
	int GetAdoptPetNum();
	void SetAdoptPetNum(DWORD dwNum);
	int GetCarrayBackPetNum();
	void SetCarrayBackPetNum(DWORD dwNum);
	PET_SENDOUT &GetSendOutPet();
	PET_ADOPT_MAP &GetPetAdoptMap();
	PET_ADOPT_MAP &GetPetCarryBackMap();
	PET_ADOPT_MAP &GetMyAdoptMap();
	PET_SENDOUT_MAP &GetSendOutPetList();
	PET_HORSE &GetMyHorse();
	PET_STATUS_VECTOR &GetPetStatus();
	DWORD GetFeedID(){return m_dwFeedID;}
	void  SetFeedID(DWORD dwID){ m_dwFeedID = dwID;}
	void Clear();
	map<DWORD,CGoodPetInfo> &GetGoodPetInfoMap(){return m_MGoodPetInfo;}
    std::string GetPetName(BYTE byPetType,BYTE byPetColor = -1);
	BOOL HasPetFollowStatus();
protected:
	PET_ADOPT_MAP m_MPetAdopt;//宠物领养数据 WORD为名次即排序序号
	PET_ADOPT_MAP m_MPetCarryBack;//宠物取回数据 WORD为名次即排序序号
	PET_ADOPT_MAP m_MMyAdoptPet;//自己所领养到的宠物
	DWORD m_iTotalAdoptPetNum;//服务器总共可以领养的宠物数
	DWORD m_iTotalCarryBackPetNum;//自己寄养了的可以领回的宠物数
	PET_SENDOUT m_SendOutPet;//已经放出来的豹子，只有一只
	PET_SENDOUT_MAP m_MSendOutPet;//释放出来的宠物,可以是多只。
	PET_HORSE m_sHorse;		//释放出来的马.
	PET_STATUS_VECTOR m_vPetStatus;  //宠物状态

	DWORD m_dwFeedID;//喂食的宠物ID
    map<DWORD, CGoodPetInfo> m_MGoodPetInfo; //宠物的附加属性
};

extern CPetData  g_PetData;
//WORD GetRidePetType( __int64 dwLooks );
bool IsBigLeopard( WORD wHorse );
bool IsLeopard( WORD wHorse );
bool IsBigPhenix( WORD wHorse );
bool IsPhenix( WORD wHorse );
void GetArmCloseIDByPetType(int& iArmID,int& iCloseID,WORD wHorse,int iSex);

#include "PetData.inl"