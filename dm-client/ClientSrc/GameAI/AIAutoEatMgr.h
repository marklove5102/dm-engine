#pragma once

#include "AIMedicaDefine.h"
#include "GameData/Good.h"

#define MAX_PANEL_GOOD_NUM  6

//托盘物品使用
struct GoodInf_t
{
	UINT uType;
	DWORD dwLastTime; //上次使用的时间
};

class CAIAutoEatMgr
{
public:
	CAIAutoEatMgr(void);
	~CAIAutoEatMgr(void);

	bool LoadConfig();

	void AutoPlace();
	void AddHP();
	void AddMP();

	void UseSuccess(CGood& tmp);
	void UseFailed(CGood& tmp);
	void BadBood(DWORD dwID);//出现假物品,如果
	bool IsMedical(CGood& tmp);

	void WriteMedicaConfig(int i);
	bool IsUseHPMedica(int i);
	bool IsUseMPMedica(int i);
	bool IsCanAddToPanel(WORD wGoodLooks,int iPanelPos);//新来的物品能不能放到腰带的第iPanelPos个位置中，比如刚才使用了腰带上的物品，应该自动放一个同类的下来，假如在放下来之前包裹来了其它非同类物品应该不要占据这个位置,另外上一次使用的持久大于1的物品要求放回原来的位置，也不应该被其它物品占用它的位置
	int  FindPanelEmptyPos(WORD wGoodLooks);//wGoodLooks这个物品能否放到腰带上某个位置
	WOOOLMEDICAEAT *GetHPMPMedicaInfo(int i);
	void SetUseHPMedia(int i,bool b);
	void SetUseMPMedia(int i,bool b);
	void SetHPMediaLimit(int i,int hp);
	void SetMPMediaLimit(int i,int mp);
	void SetHPMediaDelay(int i,int delay);
	void SetMPMediaDelay(int i,int delay);
	
	//void AutoFeed();
	//void SetastAutoFeedTime(DWORD dwTime){m_dwLastAutoFeedTime = dwTime;}
	void SetGoodInf(int i,UINT uType = 0);

private:
	int  CountItemNumber(const char* name);
	int  CountBlankNumber();
	int  FindPackageItem(const char* name);
	BOOL AutoUnbindGoods(const char* name,DWORD& time);

	char* ReadConfigString(const char* strKey,const char* strDefault="");
	void  WriteConfigString(const char* strKey,const char* str);

	GoodInf_t           m_GoodUseInf[MAX_PANEL_GOOD_NUM];

	//喝药相关
	DWORD				m_hp_nexttime;
	DWORD				m_mp_nexttime;
	DWORD				m_AutoUse_itemID;
	DWORD				m_AutoUse_MpItemID;
	DWORD				m_AutoUse_PackItemID;
	DWORD               m_dwAutoUnbindTime; //自动解包时间
	DWORD               m_dwLastUseSuperRed;//上一次使用超级天山雪莲
	DWORD               m_dwLastUseSuperBlue;//上一次使用超级深海灵礁


	WOOOLMEDICAEAT		m_woool_hpmp[MAX_MEDICA_NUMBER];
	string              m_strPath;

	//DWORD               m_dwLastAutoFeedTime; //最后一次自动喂食的时间
};

extern CAIAutoEatMgr g_AutoEatMgr;
