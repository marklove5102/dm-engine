#pragma once

#include "GameDefine.h"
#include "Character.h"
#include "ItemCfgMgr.h"
#include "MapArray.h"
#include "Global/GlobalMsg.h"
#include "Global/Interface/FontInterface.h"
#include "ObjectLink/StringLine.h"
#include "BaseClass/Misc/Net.h"


#define SELF       (*g_pSelf)
#define MapArray   SELF.GetMapArray()
#define ORIGINALSELF  (g_pGameData->Self())

struct stBoxInfo
{
	bool bOpen;
	CGoodArray freegoods;	//灵犀存放的物品
	CGoodArray paygoods;	//灵犀存放的物品
	int iFreeGoodCount;//免费存放的物品数
	int iFreeCapacity;//免费格子能够容纳的物品数
	int iPayGoodCount;//收费存放的物品数
	int iPayCapacity;//收费格子能够容纳的物品数
	DWORD dwTime;//能够维持到什么时候,0表示已强化过不需要灵力维护了
	char szExpireTime[256];//能够维持到什么时候
	int iLingLi;//冲入的灵力数	

	stBoxInfo():
	freegoods(0),paygoods(0)
	{
		clear();
	}

	void clear()	
	{
		bOpen = false;
		iFreeGoodCount = 0;
		iPayGoodCount = 0;
		iFreeCapacity = 0;	
		iPayCapacity = 0;	
		dwTime = -1;
		memset(szExpireTime,0,256);
		freegoods.ReSize(0);
		paygoods.ReSize(0);
	}

	bool IsExpired()
	{
		if(dwTime == 0)
			return false;
		else
			return (g_Timer.GetCurTime() + g_dwServerTime) > dwTime;
	}
};
struct SDynRecover
{
	DWORD dwID;
	WORD wStartX,wStartY;
	DWORD dwTime;
	SDynRecover()
	{
		dwID = 0;
		wStartX = 0;
		wStartY = 0;
		dwTime = 0;
	}
};
typedef std::vector<SDynRecover> VSDynRecover;
struct DynMiniMapObj
{
	WORD  wID;
	WORD  wTex;
	WORD  wX;
	WORD  wY;
	WORD  wMX;
	WORD  wMY;
	bool  bShow;
	std::string strDes;
	DynMiniMapObj()
	{
		wID = 0;
		wTex = 0;
		wX = 0;
		wY = 0;
		wMX = 0;
		wMY = 0;
		bShow = false;
		strDes.clear();
	}
};
typedef std::map<std::string,std::vector<DynMiniMapObj>> MDynMiniMapObj;

struct Elixir
{
	DWORD dwLeftTm;//剩余多少时间(分钟)
	DWORD dwExp[4];//一颗丹多少经验
	WORD  wHour[4];//一颗丹多少小时
	WORD  wPrice[4];//价格(小中大特)
	WORD  wTimes[4];//倍率
	//（倍率*一颗丹多少经验*多少个丹 = 时间/一颗丹多少小时）
	Elixir()
	{
		dwLeftTm = 0;
		for (int i = 0;i<4;i++)
		{
			dwExp[i] = 0;
			wHour[i] = 0;
			wPrice[i] = 0;
		}
	}
};

struct PromptInfo
{
	DWORD dwType;//类型
	DWORD dwID;//ID
	DWORD dwTime;//持续时间
	DWORD dwTmStart;//开始时间
	WORD  wLooks;//图片
	Game_Message_e eMsgWnd;
	std::string str;//说明
	PromptInfo(){
		dwType = 0;
		dwID = 0;
		dwTime = 60000;//时间为一分钟
		dwTmStart = 0;
		wLooks = 0;
		eMsgWnd = MSG_GAMEWNDS_MESSAGE_BEGIN;
	}
};
typedef std::vector<PromptInfo> VPROMPTINFO;

enum PROMPTTYPE
{
	PROMPT_NORMAL = 0x0001,
	PROMPT_URGENT = 0x0002,
};

struct CareForInfo
{
	DWORD			dwType;//PROMPTTYPE
	DWORD			dwID;
	std::string		strTile;
	std::string		strContent;
	CareForInfo()
	{
		dwType = PROMPT_NORMAL;
		dwID   = 0;
		strTile.clear();
		strContent.clear();
	}
};
typedef std::vector<CareForInfo> VCAREFORINFO;

struct HorseRunToDead
{
	DWORD		dwColor;//颜色
	DWORD		eFontType;
	int			iFontSize;
	WORD		wLastTimes;
	string		strText;
	
	HorseRunToDead()
	{
		dwColor = COLOR_TEXT_MAIN_TITLE;
		eFontType = FONT_YAHEI;
		iFontSize = 12;
		wLastTimes = 1;
	}

	HorseRunToDead(DWORD Color,DWORD eType,int fontsize,WORD wTimes,string& str)
	{
		dwColor = Color;
		eFontType = eType;
		iFontSize = fontsize;
		wLastTimes = wTimes;
		strText = str;
	}
};
typedef std::vector<HorseRunToDead> VHORSERUNTODEAD;

struct SkillShortCut
{
	SkillShortCut()
	{
		iShortCut = -1;
		iShortCutEx = -1;
		iMagicID = 0;
		iConSkillID = 0;
		pConSkill = 0;
	}
	
	int iShortCut;
	int iShortCutEx;
	int iMagicID;
	int iConSkillID;
	ConSkillData* pConSkill;
};



class CGameData
{
public:
	CGameData(void);
	~CGameData(void);

public: //基本数据
	CCharacter&		Self();

	DWORD           GetMirColor(BYTE tByte);

	void SetMouseType(int iType);
	int GetMouseType();
	DWORD GetMouseTexID();
	int GetMouseTexRenderMode() { return m_mouseRM; }
   //纪录丢弃元宝和金钱的来路
    DWORD		    GetDropMoneyFrom();
    DWORD           GetDropYuanBaoFrom();
    void	        SetDropMoneyFrom(int lMoney);    
    void	        SetDropYuanBaoFrom(int lYuanBao);

	//使用葫芦快捷键时记录位置
	void SetUseHuLuGoodPos(int pos) { m_iUseHuLuGoodPos = pos; }
	int GetUseHuLuGoodPos()			{ return m_iUseHuLuGoodPos; }

	//组队相关
	ListString& GetTroopMemberList();
	void	EnableTroop(bool b);
	void	AddTroopMember(const char * name);
	void	DelAllTroopMember();
	bool	IsAllowTroop();
	int		GetTroopMembers();
	void	DelTroopMember(const char * str); 
	BOOL    FindTroopMember(char* name);
	const char * GetMemberName(int i);
	std::vector<DWORD> &    GetGroupPos();


public: //操作

	void Clear();
	void SafeClear(); //安全复活
	void JumpClear(); //调地图时
	void ChangeMapClear(const char *strNewMapName);//地图发生改变时调用

	std::deque<CStringLine*>& GetHistoryLines(){ return m_HistoryLines; }

	//对人物和角色的操作
	CSimpleGoodNode *  AddSimpleGood(int x,int y,DWORD nID);
	CSimpleCharacterNode * AddSimpleCharacter(int x,int y,DWORD nID);
	CSimpleGoodNode * FindSimpleGood(DWORD nID);
	CSimpleCharacterNode * FindSimpleCharacter(DWORD nID);
	CSimpleCharacterNode * FindSimpleCharacter(const char* name);
	CSimpleCharacter*   FindCharacterByID(DWORD nID);
	void			  DeleteCharacter(DWORD nID);
	CGood*			  SearchGoodByID(DWORD id,int &iFromWnd); //在全局范围内
	bool              TakeoutGoodByID(DWORD id); //在全局范围内取走物品
	CGood*            SearchOtherGoodByID(DWORD id);

	//物品扩展字符串列表
	void InsertExternString(DWORD id,const string& str);
	string GetExternString(DWORD id);
	void DelExternString(DWORD id);

	const char*  GetInvestURL();//获得调查问卷地址
	void         SetInvestURL(const char* str);//调查问卷地址赋值

	//角色所处阶段
	BYTE GetCharPhase(){return m_bPhases;}
	void SetCharPhase(BYTE b){m_bPhases = b;}
	void SetSuperArmCondenseType(int iType){ m_iSuperArmCondenseType = iType;}
	int  GetSuperArmCondenseType(){ return m_iSuperArmCondenseType;}

	void EnableSelfCursor(bool b);
	bool IsSelfCursor()  { return m_bSelfCursor; }	
	bool IsHardCursor()  { return m_bHardCursor; }	
	void SetHardCursor(bool b)  { m_bHardCursor = b; }	

	stBoxInfo& GetBoxInfo(){return m_BoxInfo;}
	std::map<DWORD,std::string>& GetSkillMapList(){ return m_mapSkill;}
	VSDynRecover& GetVDynRecover(){return m_VDynRecover;}
	SDynRecover* GetDynRecover(DWORD dwID,WORD wStartX,WORD wStartY);
	MDynMiniMapObj& GetDynMinMapObj(){return m_MDynMiniMapObj;}
	void SetDynMiniMapObj(char* szMapName,WORD wIndex,bool bShow);
	void ResetDynMiniMapObj(char* szMapName);
	void LoadDynMiniMapData();
	Elixir& GetElixir(){return m_Elixir;}
	VPROMPTINFO& GetPromptInfo(){ return m_VPromptInfo;}
	PromptInfo* GetPromptInfoFirst(){  return (m_VPromptInfo.size() > 0)?&(m_VPromptInfo[0]):NULL;}
	void DelPromptInfo(DWORD dwID = 0xFFFFFFFF);
	VCAREFORINFO& GetCareForInfo(){return m_VCareForInfo;}
	CareForInfo* GetFirstCareForInfo(){ return (m_VCareForInfo.size() > 0)?&(m_VCareForInfo[0]):NULL;}
	bool HasCareForInfo(){return (m_VCareForInfo.size() > 0);}
	void DelCareForInfo(DWORD dwID = 0xFFFFFFFF);
	void ClearCareForInfo();
	void AddCareForInfo(CareForInfo& info);
	void AddCareForInfo(_sPromptInfo* pPromptInfo);
	const BYTE* GetStatusInfo(){return m_byNewHandStatus;}
	bool HasStatus(int i);
	void SetStatusInfo(BYTE* pStatus);
	void SetStatusInfo(int i);
	void SetPaoPaoStatus(unsigned __int64 _int64Status){m_int64PaoPaoStatus = _int64Status;}
	bool HasPaoPaoStatus(ePaoPaoPromptStatus i);
	unsigned __int64 GetPaoPaoStatus(){return m_int64PaoPaoStatus;}
	void SetPaoPaoStatus_Closed(unsigned __int64 _int64Status){m_int64PaoPaoStatus_Closed = _int64Status;}
	unsigned __int64 GetPaoPaoStatus_Closed(){return m_int64PaoPaoStatus_Closed;}
	bool HasPaoPaoStatus_Closed(ePaoPaoPromptStatus i);
	void AddPaoPaoStatus_Closed(ePaoPaoPromptStatus i);
	void AddHorseRunToDead(HorseRunToDead& sHRTD){ m_VHorseRunToDead.push_back(sHRTD);}
	void RemoveHorseRunToDead();
	bool HasHorseRunToDead(){return (m_VHorseRunToDead.size() > 0);}
	HorseRunToDead* GetFirstHorseRunToDead();
	bool GetShowWuXing(){return m_bShowWuXing;}
	void SetShowWuXing(bool b){m_bShowWuXing = b;}
	SkillShortCut & GetSkillShortCut(){return m_SkillShortCut;}
	void SetSkillShortCut(int iMagicID,int iConSkillID,int iShortCut,int iShortCutEx);

	inline eSendTalkChannel GetSendTalkChannelType() const { return m_eSendTalkChannelType; }
	inline void SetSendTalkChannelType(eSendTalkChannel val) { m_eSendTalkChannelType = val; }

	bool IsAutoKillMonster()				{ return m_IsAutoKillMonster; }
	void SetAutoKillMonster(bool val)		{ m_IsAutoKillMonster = val; }

	DWORD GetSingingTimeStart(){ return m_dwSingingTimeStart;}
	void SetSingingTimeStart(DWORD time){ m_dwSingingTimeStart = time;}

	int GetSingingTechID(){ return m_nSingingTechID;}
	void SetSingingTechID(int techid){ m_nSingingTechID = techid;}

	void SetNoticeIEURL(const std::string& str){ m_strNoticeIEURL = str; }
	const string& GetNoticeIEURL()	{ return m_strNoticeIEURL; }
	void SetNoticeIEState(int i)	{ m_iNoticeIEState = i; }
	int GetNoticeIEState()			{ return m_iNoticeIEState; }
	void SetNoticeFrame(int i)		{ m_iNoticeFrame = i;}
	int GetNoticeFrame()			{ return m_iNoticeFrame; }

	bool GetShowFireLianHuaHelp(){return m_bShowFireLianHuaHelp;}
	void SetShowFireLianHuaHelp(bool b){m_bShowFireLianHuaHelp = b;}

	inline string GetLoginCheckMask(){return m_strCheckMask;}
	inline void   SetLoginCheckMask(const char* pszCheckMask){m_strCheckMask = pszCheckMask;}

protected:
	void IniSkillData();
	
private:
	CCharacter				m_self;

	//任务和商城
	DWORD	                m_dwMirColor[256];
	void                    InitMirColor(void);

	bool                    m_bSelfCursor;
	bool                    m_bHardCursor;

	int                     m_iMouseType;//鼠标类型，E_MouseType中的值之一，
	DWORD                   m_dwMouseTexID;//鼠标纹理ID
	int						m_mouseRM;
    int   	                m_iDropMoneyFrom;	//鼠标上金钱的来路,E_DropMoneyYuanBaoFrom之一
    int	                    m_iDropYuanBaoFrom; //鼠标上元宝的来路,E_DropMoneyYuanBaoFrom之一
	int						m_iSuperArmCondenseType;
	bool					m_bTroop;//是否组队

	ListString				m_vecTroopMember;//组队成员列表
	IDStringMap             m_mapExternString;
	std::vector<DWORD>  	m_MemberPos;
	std::string             m_strInvestURL;          //调查问卷地址URL
	BYTE		m_bPhases;//角色所处阶段0为新手期，1为真元期，2为结丹期，3为元婴期，4为元神期，5为分神期

	Elixir		m_Elixir;
	stBoxInfo m_BoxInfo;
	std::map<DWORD,std::string> m_mapSkill;
	VSDynRecover	m_VDynRecover;
	MDynMiniMapObj	m_MDynMiniMapObj;
	VPROMPTINFO		m_VPromptInfo;
	VHORSERUNTODEAD	m_VHorseRunToDead;

	//
	VCAREFORINFO	m_VCareForInfo;
	BYTE			m_byNewHandStatus[16];
	//unsigned __int64 m_int64Status;
	unsigned __int64 m_int64PaoPaoStatus;
	unsigned __int64 m_int64PaoPaoStatus_Closed;

	bool m_bShowWuXing;

	SkillShortCut m_SkillShortCut;
	eSendTalkChannel     m_eSendTalkChannelType;//发送聊天时用的频道
	std::deque<CStringLine*> m_HistoryLines; //记录保存地址
	
	DWORD m_dwSingingTimeStart;	//吟唱技能开始时间
	int	m_nSingingTechID;		//吟唱技能id

	bool m_IsAutoKillMonster;

	string m_strNoticeIEURL;
	int m_iNoticeIEState;
	int m_iNoticeFrame;

	bool m_bShowFireLianHuaHelp;

	int m_iUseHuLuGoodPos;//使用葫芦快捷键时记录位置

	string m_strCheckMask;

};


bool Init_GameData();
bool Release_GameData();

#include "GameData.inl"
