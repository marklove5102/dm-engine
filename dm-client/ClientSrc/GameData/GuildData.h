#pragma once

#include "Global/Global.h"

enum eGuildOfficer
{
	eOffNone,			//非行会成员
	eOffMember,			//成员
	eOffBase,			//香主
	eOffHallFavor,		//堂主亲信
	eOffHall,			//堂主	
	eOffProtectFavor,	//护法亲信
	eOffProtect,		//护法
	eOffPresbyterFavor,	//长老亲信
	eOffPresbyter,		//长老
	eOffAssChmnFavor,	//副会长亲信
	eOffAssChmn,		//副会长
	eOffChmn,			//会长
};

enum eGuildFunction
{
	eAdmitMember	    = 0x00000001,         //接收成员
	eKickOutMember      = 0x00000002,         //开除成员 
	eDismiss            = 0x00000004,         //罢免官职
	eAppointFavor       = 0x00000008,         //任命亲信
	eHegemonyFgh        = 0x00000010,         //行会争霸
	eGuildAlly          = 0x00000020,         //行会结盟
	eDismissAlly        = 0x00000040,         //解散结盟
	eAttackCity         = 0x00000080,		  //发动攻沙
	eGuildTask   		= 0x00000100,		  //行会任务
	eEditBulletin  		= 0x00000200,		  //编辑公告
	ePayWealth			= 0x00000400,		  //上交资源
	eAssignTael			= 0x00000800,		  //分配银两
	eEditTile			= 0x00001000,		  //编辑封号
	eGuildShutUp		= 0x00004000,		  //行会禁言
	ePersonShutUp		= 0x00008000,		  //个人禁言
};

class CGuildData
{
public:
	struct _Member
	{
		string			num;
		string			title;
		vector<string>  name;
	};

	struct _GuildBuffer
	{
		_GuildBuffer()
		{
			memset(this,0,sizeof(_GuildBuffer));
		}
		//主体Buffer加成
		BYTE                m_byAttackFgh[2];		//战(0是下限,1是上限)
		BYTE                m_byAttackRab[2];		//法
		BYTE                m_byAttackDao[2];		//道
		BYTE				m_byPhyDef[2];			//防上,防下
		BYTE				m_byMagicDef[2];		//魔防上,魔防下
		BYTE				m_byMagicHit;			//魔法命中
		BYTE				m_byPhyHit;				//物理命中
		BYTE				m_byMagicAvoid;			//魔法躲避
		BYTE				m_byPhyAvoid;			//物理躲避
		////元神Buffer加成
		//BYTE              m_byMAttackFgh[2];		//战(0是下限,1是上限)
		//BYTE              m_byMAttackRab[2];		//法
		//BYTE              m_byMAttackDao[2];		//道
		//BYTE				m_byMPhyDef[2];			//防上,防下
		//BYTE				m_byMMagicDef[2];		//魔防上,魔防下
		//BYTE				m_byMMagicHit;			//魔法命中
		//BYTE				m_byMPhyHit;			//物理命中
		//BYTE				m_byMMagicAvoid;		//魔法躲避
		//BYTE				m_byMPhyAvoid;			//物理躲避
		//元神Buffer加成
		BYTE                m_byJinDefend;			//金防
		BYTE                m_byMuDefend;			//木防
		BYTE                m_byTuDefend;			//土防
		BYTE                m_byShuiDefend;		    //水防
		BYTE                m_byHuoDefend;			//火防
		WORD                m_wHP;                  //生命值上限
		WORD                m_wMP;                  //魔法值上限



		//主体Buffer加成
		BYTE                m_byAttackFgh_Phyle[2];		//战(0是下限,1是上限)
		BYTE                m_byAttackRab_Phyle[2];		//法
		BYTE                m_byAttackDao_Phyle[2];		//道
		BYTE				m_byPhyDef_Phyle[2];			//防上,防下
		BYTE				m_byMagicDef_Phyle[2];		//魔防上,魔防下
		BYTE				m_byMagicHit_Phyle;			//魔法命中
		BYTE				m_byPhyHit_Phyle;				//物理命中
		BYTE				m_byMagicAvoid_Phyle;			//魔法躲避
		BYTE				m_byPhyAvoid_Phyle;			//物理躲避
		//元神Buffer加成
		BYTE                m_byMAttackFgh_Phyle[2];		//战(0是下限,1是上限)
		BYTE                m_byMAttackRab_Phyle[2];		//法
		BYTE                m_byMAttackDao_Phyle[2];		//道
		BYTE				m_byMPhyDef_Phyle[2];			//防上,防下
		BYTE				m_byMMagicDef_Phyle[2];		//魔防上,魔防下
		BYTE				m_byMMagicHit_Phyle;			//魔法命中
		BYTE				m_byMPhyHit_Phyle;			//物理命中
		BYTE				m_byMMagicAvoid_Phyle;		//魔法躲避
		BYTE				m_byMPhyAvoid_Phyle;			//物理躲避
	};

	struct _NewMember
	{
		_NewMember()
		{
			bOnline = false;
			bAssist = false;
			strName = "";
			strAssist = "";
		}
		bool bOnline;//成员是否在线
		bool bAssOnline;//助手是否在线
		bool bAssist;//是否有助手
		BYTE byPhyle;//宗派(0无宗派，1是神宗，2是魔宗)
		BYTE byAssPhyle;//助手是否有宗派
		string strName;//成员姓名
		string strAssist;//助手姓名
	};

	struct _GuildStele
	{
		_GuildStele()
		{
			strName = "";
			bPart = false;
		}
		string strName;//姓名
		bool   bPart;//是否参与
	};

	struct sTileNode
	{
		std::string					sTile;	//标题
		WORD						wNum;	//coreNum;
		std::vector<_NewMember>	vMember;//成员（官员和普通成员不同，官员是以*来界定亲信的）
	};
	struct sOfficer
	{
		WORD wNum;//本封号人数
		WORD wTotalNum;//此封号最多人数
		bool bFav;//是否有亲信
		DWORD dwTael;//本封号需要的银两数目
		std::string strTile;//封号名
	};
	struct sOfficeCost
	{
		DWORD dwCost;	//职位消耗
		WORD  wJob;		//职位
		DWORD dwID;		//任命者ID
		string strName;//任命者姓名
	};

	struct sGuildTael
	{
		string strName;//玩家姓名
		DWORD  dwTael;//行会银两
	};

	struct sGuildTower
	{
		DWORD dwTaelPub;//行会公共银两
		WORD  wFlag;	//行会标志
		DWORD dwResource[5];//矿石 木材 宝石 金条 灵石
		DWORD dwResourceMax[5];//矿石 木材 宝石 金条 灵石
	};

	CGuildData(void);
	~CGuildData(void);

	void				SetBulletin(const char * str);
	vector<string> &	GetBulletin();

	void				SetAlly(const char * str);
	vector<string> &	GetAlly();

	void				SetKillAlly(const char * str);
	vector<string> &	GetKillAlly();

	void				SetGuildName(const char * str);
	const char *		GetGuildName();

	vector<_Member> &	GetMember();

	void				SetMemberTemp(const char * str);
	vector<string> &	GetMemberTemp();

	void				SetTempBulletin(const char * str);
	vector<string> &	GetTempBulletin();

	vector<string> &    GetFindStr(){  return m_vecFindStr; }

	//CTalkContent &	    GetChat();
	vector<string> &    GetWeb(){      return m_strWeb;   }

	bool	IsNewGuildData(){ return m_bAdoptNewGuild;}
	void	SetNewGuildData(bool b){ m_bAdoptNewGuild = b;}
	_GuildBuffer& GetGuildBuffer(){ return m_sGuildBuffer;}
	DWORD GetGuildFunc() { return m_dwGuildFunc;}
	void  SetGuildFunc(DWORD dwFunc) { m_dwGuildFunc = dwFunc;}
	int	  GetGuildFlag();
	void  SetGuildFlag(int iFlag);
	DWORD GetGuildTowerLevel() { return m_dwTowerLevel;}
	void  SetGuildTowerLevel(DWORD dwLevel){ m_dwTowerLevel = dwLevel;}
	int	  GetGuildTael() { return m_dwGuildTael;}
	void  SetGuildTael(DWORD dwTael) { m_dwGuildTael = dwTael;}
	bool  IsGuildBufferEff(){ return m_bGuildBuffSwitch;}
	void  SetGuildBufferSwitch(bool b){ m_bGuildBuffSwitch = b; }

	bool  IsSelfGuildBufferEff(){ return m_bSelfGuildBuffSwitch;}
	void  SetSelfGuildBufferSwitch(bool b){ m_bSelfGuildBuffSwitch = b; } 

	std::vector<sTileNode>& GetGuildMemberOfficer(){ return m_vOfficer;}
	std::vector<sTileNode>& GetGuildMemberNormal(){ return m_vMembers;}
	std::map<int ,sOfficer>& GetGuildOfficerMap(){ return m_mapGuildOfficer;}
	std::vector<sGuildTael>& GetGuildOnlineTael(){ return m_vOnlineTael;}
	sOfficeCost&			 GetOfficeCost(){ return m_sOfficeCost;}
	sGuildTower&			 GetGuildTower(){ return m_sGuildTower;}

	void                SetCancelAlly(const char* str){  m_strCancelAlly = str;}
	const char*         GetCancelAlly(){ return m_strCancelAlly.c_str();  }
	void				SetGuildLevel(const char* p){m_sGuildLevel = p;}
	const char*				GetGuildLevel(){return m_sGuildLevel.c_str();}
	void				SetGuildMornitor(const char* p){m_sGuildMornitor = p;}
	const char*				GetGuildMornitor(){return m_sGuildMornitor.c_str();}
	void				SetGuildExp(const char* p){m_sGuildExp = p;}
	const char*				GetGuildExp(){return m_sGuildExp.c_str();}
	const char*			GetGuildDelName(){return m_strDelName.c_str();}
	void				SetGuildDelName(const char* p){m_strDelName = p;}

	bool				IsHeader(bool bIncludeAssit = true);
	void				SetHeader(bool b);
	void                Clear();
	void				SetFirstGuildData(bool b){m_bFirstGuild = b;}
	bool				IsFirstGuildData(){return m_bFirstGuild;}
	void				AddGuildMember(const char* pName,const char* pTitle);
	void				ChangeGuildTitle(const char* pName,const char* pNewTitle,const char* pOldTitle);
	string&				GetChairMan(){return m_guildChairMan;}
	void			    SetChairMan(string strChairMan){ m_guildChairMan = strChairMan;}

	std::vector<_GuildStele>& GetGuildSteleMember() {return m_vGuildSteleMem;}
	std::map<std::string,std::string>& GetOfficerMemberMap(){return  m_mapOfficerMap;}
	std::string			GetOfficerByName(std::string& strName);
	void				SetGuildSteleFghValue(DWORD dwValue) { m_dwGuildSteleFhgValue = dwValue;}
	DWORD				GetGuildSteleFghValue(){return m_dwGuildSteleFhgValue;}
	void				SetGuildSteleHeroNum(WORD wNum) { m_wSteleHeroNum = wNum;}
	WORD				GetGuildSteleHeroNum() {return m_wSteleHeroNum;}
	void				SetGuildSteleLevelNum(DWORD dwNum) { m_dwSteleLevelNum = dwNum;}
	DWORD				GetGuildSteleLevelNum(){return m_dwSteleLevelNum;}
	void				SetGuildPhyleState(BYTE byState){ m_byGuildPhyle = byState;}
	BYTE				GetGuildPhyleState(){return m_byGuildPhyle;}
	void				AddGuildMemeberOfficerMap(WORD wCoreNum,std::string& strName);
private:

	string			m_guildName;
	string			m_guildChairMan;//会长
	vector<string>	m_bulletin;
	vector<_Member> m_member;
	vector<string>	m_strWeb;

	vector<string>	m_ally;
	vector<string>	m_killally;

	vector<string>	m_memberTemp;
	vector<string>	m_TempBulletin;
	vector<string>  m_vecFindStr;    //最近查找的数据
	string          m_strCancelAlly; //要取消结盟的行会
	bool			m_bGuildHeader;
	std::string     m_sGuildLevel;
	std::string     m_sGuildMornitor;
	std::string		m_sGuildExp;
	std::string		m_strDelName;
	bool			m_bFirstGuild;//首次获得行会数据
	bool			m_bAdoptNewGuild;//是否采用新的行会机制

	DWORD			m_dwGuildTael;//自己的行会银两
	DWORD			m_dwGuildFunc;//行会职能
	int				m_iGuildFlag;// 行会旗帜
	DWORD			m_dwTowerLevel;//通灵塔的等级
	BYTE			m_byGuildPhyle;//行会修神还是修魔0是凡人1是修神2是修魔
	//Buffer加成
	_GuildBuffer	m_sGuildBuffer;//行会加成的Buffer
	std::vector<sTileNode>		  m_vOfficer; //当官的
	std::vector<sTileNode>		  m_vMembers; //当小兵的
	std::map<int ,sOfficer>		  m_mapGuildOfficer;//官职映射表
	sOfficeCost					  m_sOfficeCost;//职位消耗
	std::vector<sGuildTael>		  m_vOnlineTael;//本GS在线人员的行会银两信息
	std::map<std::string,std::string> m_mapOfficerMap;//官职映射表
	sGuildTower					  m_sGuildTower;//行会塔信息
	bool						  m_bGuildBuffSwitch;//行会Buffer加成是否有效
	bool                          m_bSelfGuildBuffSwitch;//行会buffBuffer加成对于玩家自己是否有效

	DWORD						  m_dwGuildSteleFhgValue;//战斗值
	WORD						  m_wSteleHeroNum;//最多参与仙灵碑人数
	std::vector<_GuildStele>	  m_vGuildSteleMem;//仙灵碑参与人员
	DWORD						  m_dwSteleLevelNum;//仙级人数(高)和仙级级别(低)
};

extern CGuildData   g_GuildData;