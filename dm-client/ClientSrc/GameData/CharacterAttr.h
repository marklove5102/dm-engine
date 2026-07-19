#pragma once

#include "Global/Global.h"
#include "GameData/GameDefine.h"
#include "ActionStruct.h"
#include "UnionStruct.h"
#include "ReserveData.h"
#include "Global/StringUtil.h"

struct BoothInfo_t
{
	string		 strBoothName; //摊位名
	int          iBoothType;   //摊位类型
	DWORD        dwFlagColor;  //旗子的颜色
	int          iFlagType;	   //旗子类型
};

struct BubbleUpStruct
{
	BubbleUpStruct();

	BYTE  byType;    //冒的类型,0:该项未用，1为经验，2为天魔宝藏地图的黑暗属性增加
	DWORD dwStartTime;//开始的时间
	DWORD dwValue;//冒的值
	DWORD dwLastTime;//持续时间
};

enum E_UsingMagicType
{
	UMT_NORMAL     = 0,   //正常
	UMT_NOACTION   = 1,   //不能移动及做其它动作
};

struct FABAOStruct
{
	int   dwCurFaBaoType;			//当前法宝类型
	int   dwLastFaBaoType;			//之前法宝类型
	int   iFrameTrans;				//状态转换帧数:7-12处于上一个法宝的收起阶段 1-6处于当前法宝的出现阶段 0普通阶段 
	int	  iFrameStand;				//待机动作,4帧循环
	int	  iState;					//0-待机 1-走动
	int   iHide;					//0--显示,1--隐藏
	int	  iFaBaoLevel;				//法宝等级
	DWORD dwLastUpdateTime;			//控制出现消失播放速度
	BYTE  bySanWeiFireLevel;        //三味真火等级
	int	  dwLeftFaBaoType;			//左法宝类型
	int   dwRightFaBaoType;			//右法宝类型
	bool  bFaZhen;					//使用法阵技能中--1

	//-1--收起当前法宝 
	void SetCurFaBaoType(int newFaBao ,int FaBaoLevel,BYTE byAnimation = 0)
	{
		if (!byAnimation)
		{
			dwLastFaBaoType = dwCurFaBaoType;
			dwCurFaBaoType = newFaBao;
			iFaBaoLevel = FaBaoLevel;
			return;
		}

		if (dwCurFaBaoType > 0)
		{
			dwLastFaBaoType = dwCurFaBaoType;
			dwCurFaBaoType = newFaBao;
			iFrameTrans = 12;
		} 
		else
		{
			dwLastFaBaoType = dwCurFaBaoType;
			dwCurFaBaoType = newFaBao;
			iFrameTrans = 6;
		}
		iFaBaoLevel = FaBaoLevel;
		
	}

	void Update()
	{
		if (iFrameTrans > 0 && GetTickCount() > dwLastUpdateTime + 100)
		{
			iFrameTrans--;
			dwLastUpdateTime = GetTickCount();
		}
		else
		{
			if (iState == 0)
			{
				iFrameStand++;
				iFrameStand %= 4;
			}
		}
	}

	//state: 2-收起出现 0-待机 1-移动
	void GetCurShowInfo(int& FaBaoType, int& Frame, int& State)
	{
		if (iFrameTrans > 6)//处于前一个法宝收起阶段
		{
			FaBaoType = dwLastFaBaoType;
			Frame = iFrameTrans - 7;
			State = 2;

		}
		else if (iFrameTrans > 0)//出于当前法宝出现阶段
		{
			FaBaoType = dwCurFaBaoType;
			Frame = 6 - iFrameTrans;
			State = 2;
		}
		else 
		{
			FaBaoType = dwCurFaBaoType;
			Frame = iFrameStand;
			State = 0;
		}

	}
	
	void SetState(int state)
	{
		if (state != 0)
		{
			iFrameStand = 0;
		}
		iState = state;
	}

};

class CCharacterAttr
{
public:
	CCharacterAttr(void);
	~CCharacterAttr(void);

	//基本属性
	void    clear();
	void	SetID(DWORD nID);
	void	SetTitle(const char * strTitle);
	void    SetName(const char * strName);

	void	SetLooks(__int64 looks);
	void	SetDir(BYTE nDir);
	void	SetFlags(int nFlags);
	virtual void	SetHP(WORD nLife);
	virtual void	SetHPMax(WORD nLifeMax);
	virtual void	SetMP(WORD nMana);
	virtual void	SetMPMax(WORD nManaMax);
	void	SetXY(int x,int y);
	void	SetRealXY(int x,int y);
	void    SetScrXY(int x,int y);
	void    SetTopXY(int x,int y);
	void    SetTexWH(int w,int h);
	void    SetTexOffXY(int w,int h);
	void	SetNameColor(char cColor);
	void	SetOffset(int x, int y);
	void	SetDrawOffset(int x, int y);

	void	GetXY(int &x,int &y);
	int     GetX();
	int     GetY(); 
	void	GetRealXY(int &x,int &y);
	void    GetScrXY(int &x,int &y);
	void    GetTopXY(int &x,int & y);
	void    GetTexWH(int &w,int & h);
	void    GetTexOffXY(int &w,int & h);
	int     GetTexW();
	int     GetTexH();
	void    GetPlayerXY(int& x,int & y); //显示位置对应的格子

	DWORD   GetID();
	const char * GetName();
	const char * GetTitle();
	LOOKS&	GetLooks();
	__int64   GetLooksValue();
	char	GetDir();
	int		GetFlags();
	virtual WORD	GetHP();
	virtual WORD	GetHPMax();
	virtual WORD	GetMP();
	virtual WORD	GetMPMax();
	BYTE	GetNameColor();
	void 	GetOffset(int & x, int & y);
	void 	GetDrawOffset(int & x, int & y);
	int		GetMagicState();
	int     GetUsingMagicType();

	VString& GetShowName();

	bool	IsDeal();
	bool	IsWait();
	WORD	GetStatus();
	WORD    GetExtraState();
	bool	IsDead();

	void    SetSex(int i);
	int     GetSex();
	bool	IsMale();

	void	SetAttackSpeed(int i);
	int		GetAttackSpeed();
	void	SetShowHP(UCHAR c);
	UCHAR	GetShowHP();

	int     GetCareer();
	void    SetCareer(int nCareer);

	virtual void     SetLevel(WORD level);
	virtual WORD	 GetLevel();

	void    SetLastTex(PlayerTex* pLastTex);
	PlayerTex* GetLastTex();

	//人物状态
	bool    HasMagicState(int iState);
	void    AddMagicState(int iState);
	void    RemoveMagicState(int iState);

	void	SetDeal(bool b);
	void	SetWait(bool b);
	void	SetStatus(WORD state);
	void	SetUsingMagicType(int iType);
	void	AddUsingMagicType(int iType);
	void	 RemoveUsingMagicType(int iType);
	void    RemoveStatus(WORD state);
	bool    HasStatus(WORD state);
	void    SetExtraState(WORD state);
	void    RemoveExtraState(WORD state);
	void    SetExtraState2(DWORD dwStatus);
	void	SetDead(bool b);
	void	SetHurtedTime(DWORD dwHurtedTime);

	LOOKS&	GetBianShenLooks();
	__int64   GetBianShenLooksValue();
	void	SetBianShenLooks(__int64 l);
	bool	IsBianShen();

	//角色类型
	int		GetRaceType();
	int		GetRaceNo();
	bool	IsOnHorse();        //判断是否在骑马或骑豹子
	bool    IsOnLepoard();      //判断是否在骑豹子
	bool    IsOnBigLepoard();
	bool    IsOnPhenix();
	bool    IsOnBigPhenix();
	bool    IsDemonMonster();//是不是假心魔

	//魔法特效相关
	void		PushTexFront(tex_show_t *p);
	void		PushTexBack(tex_show_t *p);
	void		ClrTexFront();
	void		ClrTexBack();
	tex_show_t* GetTexFront();
	tex_show_t* GetTexBack();

	void		ClearMagicCase();
	void		SetMagicCase(IMagicCase* pMagicCase);
	IMagicCase* GetMagicCase();


	//动作相关
	SAction& GetAction();
	SNextAction& GetOldNextAction();
	SNextAction* GetNextAction();
	E_NEXT_STATE HasNextAction();
	bool    PopSNextAction(SNextAction* pNextAction);
	SNextAction* PushSNextAction(int iAction,char dir = DIR_NO,int x = 0,int y = 0);
	SNextAction* InsertSNextAction(int iAction,char dir,int iAimX,int iAimY,int idx = 0);
	void    ClearAllNext();
	void    FinishAllNext(BOOL bNow = TRUE);
	void    InitAction(int iAction);
	void    InitAction(SNextAction* pNext);
	void    AddAttackedTarget(DWORD id);
	int	    GetAttackActionNum();
	int     GetNextActionNum();

	int		IsSpeedup(); //加速的倍数
	void	SetSpeedup(int i);

	void	SetStopGo(bool b);
	bool	IsStopGo();

	//摆摊
	bool	GetBoothState();
	void	SetBoothState(bool b);
	BoothInfo_t& GetBoothInfo();

	//社会关系
	DWORD   GetRelationType();
	void    AddRelationType(DWORD type);
	void    RemoveRelationType(DWORD type);
	bool    IsGrouped();
	bool    IsJieYi();

	void    SetMaskName(int level,const char* maskname);
	const char*  GetMaskName();
	int     GetMaskLevel();
	
	WORD    GetBody();

	DWORD   GetReserveData(int i);
	void	SetReserveData(int i,UINT dwData, WORD magID = 0);
	void	AddReserveData(int i);
	void	DecReserveData(int i);

	//聊天
	void	 SetTalk(const char *str,DWORD dwColor,BYTE cFontType,BYTE cFontSize);
	VString& GetTalk();
	BYTE	 GetTalkFontType();
	int      GetFontSize();
	DWORD	 GetTalkColor();
	DWORD	 GetTalkShowTime();

	//头顶经验
	BubbleUpStruct & GetBubbleUp(int i){return m_BubbleUp[i];}
	BubbleUpStruct & NewBubbleUp();
	void ClearBubbleUp();
	DWORD  GetLastBubbleUpTime(){return m_dwBubbleUpLastTime;}
	void   SetLastBubbleUpTime(DWORD dwTime){m_dwBubbleUpLastTime = dwTime;}

	virtual int   CalStepCount(WORD wAction);

    void    SetFightOnLeopard(bool b)       {m_bFightOnLeopard = b;}
    bool    GetFightOnLeopard()             {return m_bFightOnLeopard;}

	//iShieldShape,战士:1,2,3,4;法师:5,6,7,8;道士:9,10,11,12,虎王盾:13,14,15,16,血月虎王盾:17,18,19
	void    SetShield(int iShieldShape);
	int     GetShieldType()					{return m_iPutOnShield;}
	int     GetShieldLevel()				{return m_iPutOnShieldLevel;}

    int     GetRunStepOnLeopard()           {return m_iRunStepOnLeopard;}
    void    SetRunStepOnLeopard(int iStep)  {m_iRunStepOnLeopard = iStep;}
	int     GetNumForZhongLeiHit()			{return m_iNumForZhongLeiHit;}
	void	SetNumForZhongLeiHit(int iNum)	{m_iNumForZhongLeiHit = iNum;}

	WORD    GetGreateMagicState();
	void    SetGreateMagicState(WORD state);
	void    AddGreateMagicState(eGREATEMAGIC e);
	void	RemoveGreateMagicState(eGREATEMAGIC e);

	virtual BYTE    GetVipCardType();
	virtual void    SetVipCardType(BYTE val);
	virtual BYTE    GetVipTradeLevel();
	virtual void    SetVipTradeLevel(BYTE val);

	void Set12GongTitle(BYTE val);
	BYTE Get12GongTitle();

	bool    IsCanDig();
	void    SetCanDig(bool val);

	DWORD   GetNpcActionTm();
	void    SetNpcActionTm(DWORD dwTm);
	void    SetCamp(BYTE byValue);
	BYTE    GetCamp();
// 	void    SetTrusteeshipFlag(BYTE byFlag);
// 	BYTE    GetTrusteeshipFlag();

	int     GetCharHolyWingLevel();
	int     GetCharHolyWingStrongLevel();
	void    SetCharHolyWingLevel(int val);
	void    SetCharHolyWingStrongLevel(int val);

	BYTE	GetLiveTokenNumber()			{return m_byNumberLiveToken;}
	void	SetLiveTokenNumber(BYTE number)	{m_byNumberLiveToken = number;}

	BYTE	GetTypeBufferQQ()			{return m_byBufferTypeQQ;}
	void	SetTypeBufferQQ(BYTE type)	{m_byBufferTypeQQ = type;}

	bool	IsSinging();
	void    SetTuTengState(DWORD dwState);
	DWORD   GetTuTengState();
	void    SetFaBaoType(int dwState, int ilevel = 0,BYTE byAnimation = 0);
	void	SetLeftFaBaoType(int dwState);
	void	SetRightFaBaoType(int dwState);
	void	UseFaZhen(bool use);
	bool	IsUsingFaZhen();
	FABAOStruct*   GetFaBaoType();
	void    HideFaBao(int dwState);
	void    SetSanWeiFireLevel(BYTE byLevel);
	BYTE    GetSanWeiFireLevel();
	DWORD	GetMagicSloganEndTime(){return m_dwMagicSloganEndTime;}
	void    SetMagicSloganEndTime(DWORD end){ m_dwMagicSloganEndTime = end;}
	void	SetMagicSlogan(const char* slogan){memset(m_czMagicSlogan,0,64); memcpy(m_czMagicSlogan,slogan,strlen(slogan));}
	char*	GetMagicSlogan(){ return m_czMagicSlogan;}

	void	SetLianJiNumber(DWORD number){ m_dwLianJiNumber = number;}
	DWORD	GetLianJiNumber(){return m_dwLianJiNumber;}

	DWORD	GetLianJiShowEndTime(){return m_dwLIanJiShowEndTime;}
	void	SetLianJiShowEndTime(DWORD end){m_dwLIanJiShowEndTime = end;}

	DWORD	GetLianJiZhuangJiTime(){return m_dwLianJiZhuangJiTime;}
	void	SetLianJiZhuangJiTime(DWORD end){m_dwLianJiZhuangJiTime = end;}

	void	SetRealLianJiNumber(DWORD number){ m_dwRealLianJiNumber = number;}
	DWORD	GetRealLianJiNumber(){return m_dwRealLianJiNumber;}

	void	SetLastLianJiStartTime(DWORD number){ m_dwLastLianJiStartTime = number;}
	DWORD	GetLastLianJiStartTime(){return m_dwLastLianJiStartTime;}

	bool    IsInDaoZunJiangLinState() { return m_IsInDaoZunJiangLinState; }
	void    SetDaoZunJiangLinState(bool val);

	bool    IsIn8DunState()			{ return m_IsIn8DunState; }
	void    Set8DunState(bool val);
	void	Set8DunShan(bool val);
	bool	Is8DunShan()			{ return m_b8DunShan; }
	void	Set8DunCurProgress(int val)	{ m_8DunCurProgress = val; }
	int		Get8DunCurProgress()		{ return m_8DunCurProgress; }
	void	Set8DunMaxProgress(int val)	{ m_8DunMaxProgress = val; }
	int		Get8DunMaxProgress()		{ return m_8DunMaxProgress; }

	void	SetInShadowKill8(bool val)	{ m_IsInShadowKill8 = val; }
	bool	IsInShadowKill8()			{ return m_IsInShadowKill8; }
	void	SetShadowKill8Dir(BYTE dir) { m_byShadowKill8Dir = dir; }
	BYTE	GetShadowKill8Dir()			{ return m_byShadowKill8Dir; }
	void	InitShadowNum(int iShadowNum);
	int		GetLeftShadowNum()			{ return m_iLeftShadowNum; }
	int		GetRightShadowNum()			{ return m_iRightShadowNum; }
	int		GetMaxShadowNum()			{ return m_iMaxShadowNum; }
	int		GetShadowStepFrame()		{ return m_iShadowStepFrame; }

protected:
	DWORD		m_nID;		// ID

	string		m_strName;	// 角色名
	string		m_strTitle;	// 头衔
	VString		m_showName;

	LOOKS	    m_Looks;	// 外观
	char		m_nDirection;// 方向
	int			m_nFlags;
	int			m_nHP;	// 生命
	int			m_nHPMax;	// 最大生命值
	int			m_nMP;	// 生命
	int			m_nMPMax;	// 最大生命值
	int			m_iX;
	int			m_iY;
	BYTE 		m_cNameColor;
	int			m_iOffX,m_iOffY;   //偏移
	int			m_iRealX,m_iRealY; //实际坐标
	int         m_iScrX,m_iScrY;   //屏幕坐标
	int			m_iDrawOffX,m_iDrawOffY;   //偏移


	int			m_iAttackSpeed;
	int			m_iSex;					// 男为0,女为1
	BYTE		m_cShowHP;				// 血条长度
	int 		m_nCareer;      //职业
	WORD        m_iLevel;  //怪物等级  

	//人物状态
	bool        m_bBoothState;        //是否摆摊
	BoothInfo_t m_BoothInfo;          //摊位类型 
	DWORD       m_dwRelationType;     //同玩家的关系

	PlayerTex   m_LastTex;

	int  		m_iTopX,m_iTopY;
	int         m_iTexW,m_iTexH;
    int         m_iTexOffX,m_iTexOffY;
	//动作
	SAction		m_ActionNow;
	SAction     m_ActionOld;

	SNextAction * m_pNextBegin;
	SNextAction * m_pNextEnd;
	SNextAction m_OldNextAction;


	//魔法相关
	tex_show_t* m_pTexFront;	// 图片暂存链表-身前
	tex_show_t* m_pTexBack;	// 图片暂存链表-身后
	IMagicCase* m_pMagicCase;

	int 		m_iSpeedup;  //加速的倍数
	int			m_iMagicState;			// 0:基本剑法 1:攻杀 2:刺杀 3:半月 4:烈火
	int         m_iUsingMagicType;      //正在使用的魔法类型,0:正常,1,不能移动及做其它动作
	LOOKS		m_BianshenLooks;

	bool		m_bDeal;
	bool		m_bWait;
	WORD		m_wStatus;
	WORD        m_ExtraState;
	bool		m_bDead;
	VString	    m_VTalk;
	BYTE        m_cTalkFontType;
	DWORD       m_dwTalkColor;
	int         m_iFontSize;

	bool		m_bStopGo;

	int         m_iMaskLevel;
	string      m_strMaskName;
	DWORD       m_dwTalkShowTime;      //头顶说话保留时间
	DWORD		m_dwNpcActionTm;
 
	DWORD       m_dwReserveData[plyNums];  //保留数据

	BubbleUpStruct m_BubbleUp[8];//有些地方获得经验要求从头上冒出来，可以同时多个在冒,如果byType为0则表示该项没用
	DWORD m_dwBubbleUpLastTime;//该人物最后一次冒泡的时间

    bool         m_bFightOnLeopard;	//骑战
    int          m_iRunStepOnLeopard;//骑宠物所跑的格子数
	int			 m_iNumForZhongLeiHit;

	int         m_iPutOnShield;	//佩戴盾牌
	int         m_iPutOnShieldLevel;//盾牌等级
	int         m_iGuildFlag;	//行会旗帜
	BYTE        m_byVipCardType;        //0:不是vip,1:银卡,2:金卡
	BYTE        m_byVipTradeLevel;        //vip等级,0表示不是vip会员
	BYTE		m_by12GongTitle;		//12宫称号，0无，1-12，
	bool        m_bCanDig;//能不能挖的
	BYTE		m_byCamp;	//阵营0x20(红方) 0x40(蓝方)
// 	BYTE        m_byTrusteeshipFlag;//是否托管,非零表示已经托管,1表示是已阵营,2表示非已方阵营
	
	int         m_iHolyWingLevel;//神巽当前等级 < 0表示没有觉醒,//1.28版修改为翅膀颜色，1：红色，2：蓝色，3：金色
	int         m_iHolyWingStrongLevel;//神巽强化等级,也就是镶边类型,镶边颜色根据这个判断 < 0表示没有觉醒,//1.28版修改为翅膀镶边颜色，1：红色，2：蓝色，3：金色
	BYTE		m_byNumberLiveToken;//生存令牌数量
	BYTE		m_byBufferTypeQQ;//圈圈的类型
	WORD        m_wGreateMagicState;//群英传里的bodycolor3,传是是:这个玩家使用的所使用的技能是否为大师级,如护身真气,金刚护体等,从外面走到他面前时要知道这个状态,最高一个bit表示是神还是魔(如果是大师级的话)

	DWORD       m_dwTuTengState;//图腾效果
	FABAOStruct m_sFaBao;
	char		m_czMagicSlogan[64];//施法口号,显示在人头顶
	DWORD       m_dwMagicSloganEndTime;//施法口号结束时间

	DWORD		m_dwLianJiNumber;	//连击数
	DWORD		m_dwLIanJiShowEndTime;	//连击数显示结束时间
	DWORD		m_dwLianJiZhuangJiTime;	//连击数撞击时间
	DWORD		m_dwRealLianJiNumber;	//协议收到的真实连击数
	DWORD		m_dwLastLianJiStartTime;	//解决:前一次连击数过多,后一次协议来时,上一次效果没播完,导致数字回退

	bool		m_IsInDaoZunJiangLinState;//是否有无量五行诀
	bool		m_IsIn8DunState;//是否有强袭烈焰盾
	bool		m_b8DunShan;//强袭烈焰盾闪烁
	int			m_8DunCurProgress;//强袭烈焰盾当前进度
	int			m_8DunMaxProgress;//强袭烈焰盾最大进度

	bool		m_IsInShadowKill8;//八方分影斩
	BYTE		m_byShadowKill8Dir;//八方分影斩人物方向
	int			m_iShadowStepFrame;
	int			m_iLeftShadowNum;
	int			m_iRightShadowNum;
	int			m_iMaxShadowNum;
};

#include "CharacterAttr.inl"
