#pragma once

#include "Global/Global.h"
#include "GameDefine.h"
#include <vector>
#include "OtherPlayer.h"
#include <algorithm>
#include "GameData/MagicDefine.h"
#include "GameData/GameData.h"
#include <string>
#include "Global/MathUtil.h"


#ifndef ROLENAME_MAX_LEN
#define ROLENAME_MAX_LEN  15
#endif

#ifndef TIME_STRING_LEN
#define TIME_STRING_LEN  20
#endif

struct _QunYingReportStruct
{
	_QunYingReportStruct()
	{
		strName = "";
		iKills = 0;
		iExp = 0;
		iPkKills = 0;
		iDeaths = 0;
	}

	std::string strName;// 曾加入群英小队队长名称
	DWORD iKills;			// 杀怪数量
	DWORD iExp;			// 获得经验值数量
	DWORD iPkKills;		// PK斩敌数
	DWORD iDeaths;		// 阵亡次数
};

struct _RelationStruct
{
	_RelationStruct()
	{
		strName = "";
		iMuoQi = 0;
		iRelType = 0;
		iJob = 0;
		iLevel = 0;
		iOnline = 0;
		strGuild = "";
		//好友名次排行趋势
		cQushi = 0;
		byPhyleType = 0;
		byJinJie = 0;
		bHasXuanTieFlag = false;//是否收到玄铁令牌

		iCurLive = 0;
		iYesterLive = 0;

	}

	void   AddRelationType(DWORD type)   {  iRelType |= type; }
	void   RemoveRelationType(DWORD type){  iRelType &= ~type; }

	std::string strName;
	DWORD		iMuoQi;	// 默契度
	DWORD		iRelType;
	int			iJob;
	int			iLevel;
	int			iOnline;
	std::string strGuild;
	BYTE		byPhyleType;
	BYTE		byJinJie;
	//好友名次排行趋势
	char					cQushi;
	bool                 bHasXuanTieFlag;//是否收到玄铁令牌

	DWORD			iCurLive;
	DWORD			iYesterLive;
};

struct _MazeZone
{
	BYTE byMap;//索引
	BYTE byType;//类型(地图0x01，小宝0x02，大宝0x04)
	_MazeZone()
	{
		byMap = 0;
		byType = 0;
	}
};

struct _MazeMap
{
	BYTE byPlayerIndex;
	_MazeZone maze[49];
	void clear()
	{
		byPlayerIndex = 0;
		memset(maze,0,49*sizeof(_MazeZone));
	}
};

struct _FireStruct
{
	unsigned long 		dwID;
	int 				iType;
	char				cFrame;
	DWORD				color;
	int x,y;
};

struct S_CommonConfirm
{
	S_CommonConfirm()
	{
		iType = 0;
		dwLastTime = 0;
		dwRequestPersonID = 0;
	}

	int          iType;//1为确认是否接受其它人的拜师，2为是否接受其它人的收徒，3为是否接受交易,4是否接受加为好友，5是否接受加入行会，6是否接受官职任命
	DWORD        dwLastTime;//倒计时的时间，毫秒，0为不自动关闭
	DWORD        dwRequestPersonID;//请求我回应也就是发这条消息的玩家的ID
	std::string  strMsg;//确认提示文字
};

//留言数据
struct _LeaveWords
{
	inline bool operator ==(const _LeaveWords & t)
	{
		if(strCharName == t.strCharName && strDate==t.strDate 
			&& strTime==t.strTime && strPic ==t.strPic && strMusic==t.strMusic)
			return true;
		else
			return false;
	}
	std::string			strCharName;
	std::string			strDate;
	std::string			strTime;
	std::string			strMessage;
	std::string			strPic;
	std::string			strMusic;
	bool				bIsReaded;
};

struct NoteTitle
{
	std::string			sTitle;
	int					iBegin;
	int					iEnd;
};

struct _CharInfo
{
	_CharInfo()
	{
		memset(szRoleName,0,ROLENAME_MAX_LEN);
		byJob = 0;
		byHair = 0;
		wLevel = 0;
		bySex = 0;
		memset(szDeleteTime,0,TIME_STRING_LEN);
		byDelete = 0;
	}

	char             szRoleName[ROLENAME_MAX_LEN];
	unsigned char    byJob;
	unsigned char    byHair;
	unsigned short   wLevel;
	unsigned char    bySex;
	char             szDeleteTime[TIME_STRING_LEN];
	unsigned char    byDelete;
};

struct _OffLinePrivate
{
	std::string			strCharName;
	std::string			strMessage;
	bool				bLeaveWords;
};

//飘红
struct _OtherMsg 
{
	string strMsg;
	WORD wColor;
};


struct WUXINGFLAG{
	DWORD dwFlagInMonstID;//
	DWORD dwLeftTime;//以秒为单位，下轮逃跑时间
	DWORD dwStartTime;//下轮逃跑的开始时间
	DWORD dwGuarderNum;//守卫上限
    DWORD dwGuardChangeTm;//守卫数目改变的时间
	DWORD dwEscapeMon;//逃跑的怪物数目
    DWORD dwEscapeMonTm;//逃跑的怪物数目改变的时间
    DWORD dwCurrentRound;//当前回合数
    DWORD dwRoundChangeTm;//回合数改变时间
	bool  bUseFlag;//使用旗子
	bool  bFinishFlag;//旗子主角使用完成
	bool  bMonsterStart;//怪物起手是否完成	
	//bool  bFirstRound;//五行玄关是否是第一轮怪物
};

//跨服战成员结构
struct _KfzMember
{
	_KfzMember(string name = "",int career = -1,int sex = -1,int level = -1,BYTE state = 0)
	{
		strName = name;
		iCareer = career;
		iSex = sex;
		iLevel = level;
		byState = state;
	}

	string strName;
	int iCareer;
	int iSex;
	int iLevel;
	BYTE byState;//0未激活,1:激活中,2:已激活,3:已记录
};

struct _KfzGuild
{
	string strGuildName;//行会名称
	vector<_KfzMember> VKfzMemberList; //本服参加跨服战的成员列表
};
//读秒倒计数结构
struct _TimeOut
{
	_TimeOut()
	{
		SetValue("",0,0,0,0,0);
	}

	_TimeOut(string szMsg,DWORD starttime,DWORD lasttime,int type,bool bSecond,int lastmusicsecond = 0,DWORD monsterID = 0)
	{
		strMsg = szMsg;
		dwStartTime = starttime;
		dwLastTime = lasttime;
		iType = type;
		bShowSecond = bSecond;
		iLastMusicSecond = lastmusicsecond;
		dwMonsterID = monsterID;
	}

	void SetValue(string szMsg,DWORD starttime,DWORD lasttime,int type,bool bSecond,DWORD color,int lastmusicsecond = 0,DWORD monsterID = 0)
	{
		strMsg = szMsg;
		dwStartTime = starttime;
		dwLastTime = lasttime;
		iType = type;
		bShowSecond = bSecond;
		iLastMusicSecond = lastmusicsecond;
		dwMonsterID = monsterID;
	}
	DWORD dwStartTime;//倒计时开始时间,单位毫秒
	DWORD dwLastTime;//倒计时持续时间,单位毫秒
	string strMsg;//要显示的内容,"%s"是用来显示时间的
	bool bShowSecond;//true以秒为单位,否则以分钟为单位
	int iType;//倒计数的类型,不同的类型倒计时结束可能要做不同的事情,1:五行玄关防守倒计数,2:跨服战开始倒计时,10001:进入本地服务器KFZZB地图和跨服战服务器KFZZB001~~030地图后，在屏幕左上方提示玩家：离下一轮比赛开始还有XX分钟，请速至跨服战区的跨服战使者处发起挑战，角逐最强行会！
	//1-20000客户端自己用,1-10000:图形倒计时(大图),10001-20000,普通数字倒计时,后面服务器用20001-30000:图形倒计时(大图),30001-40000普通倒计时,要显示内容,30010:提示下次领奖时间
	//40001-50000:图形(小图)加普通文字显示
	int iLastMusicSecond;   //读秒音效
	DWORD dwMonsterID;		//id = 0 代表玩家
};
//铁血战歌及rts副本数据结构
struct sTXZG
{
	sTXZG()
	{
		clear();
	}

	void clear()
	{
		byType = 0;
		bStarted = false;
		wCurRes_Red = 0;
		wCurRes_Blue = 0;
		wContribute = 0;	
		bySide = 0;
		wGetExp = 0;
		wContributeExp = 0;
		byVictorySide = 0;
		wKillMonsterNum_Red = 0;
		wKillMonsterNum_Blue = 0;
		byResSide = 0;
	}

	BYTE byType;//副本类型,1:铁血战歌，2:rts副本
	bool bStarted;//是否开始
	WORD wCurRes_Red;//红方当前资源
	WORD wCurRes_Blue;//蓝方当前资源
	WORD wContribute;//贡献度
	BYTE bySide;//自己在那一方,0:红方,1:蓝方

	//rts中用到的数据
	WORD wKillMonsterNum_Red;//红方中立怪物击杀数
	WORD wKillMonsterNum_Blue;//蓝方中立怪物击杀数
	BYTE byResSide;//公共资源归属，0：红方 1：蓝方

	//结果
	BYTE byVictorySide;//胜利方,0:红方,1:蓝方
	WORD wGetExp;//获得的经验值
	WORD wContributeExp;//个人贡献所得经验值

};

//世界杯数据结构
struct sTSJB
{
	sTSJB()
	{
		clear();
	}

	void clear()
	{
		bStarted = false;
		FirstTeamScore = 0;	
		SecondTeamScore = 0;
	}
	bool bStarted;			//是否开始
	BYTE byMyTeam;			//自己属于哪个队,0-1队,1-2队
	WORD FirstTeamScore;	//1队分数
	WORD SecondTeamScore;	//2队分数
};

//功勋值升级相关数据
struct MeritoriousnessUpdate 
{
	DWORD dwNextLevel;		//升到下一级所需功勋值
	DWORD dwMostPerHour;	//每小时获得功勋值上限
	DWORD dwMeriToExp;		//功勋兑换经验比例
	DWORD dwPresToExp;		//声望兑换经验比例
};

#pragma pack(push,1)
struct SanWeiZhenHuoInfo//三味真火详细信息
{
	//offset = 4, 2字节表示消息号0x0A21
	BYTE	byFirst;						//offset = 6, 1字节表示是第一次领悟后发送的详细信息
	BYTE	byFireLevel;					//offset = 7, 1字节表示三昧真火等级
	WORD	wFireMulty;						//offset = 8, 2字节表示三昧真火的重数
	WORD	wReserved;						//保留,跳过头部
	DWORD	dwFireExp;						//offset = 12, 4字节表示当前三昧真火的真火值
	DWORD	dwFireExpMax;					//offset = 16, 4字节表示当前最大的真火值
	DWORD	dwHPAdd;						//offset = 20, 4字节表示当前三昧真火产生的HP加成
	DWORD	dwMPAdd;						//offset = 24, 4字节表示当前三昧真火产生的MP加成
	DWORD	dwAttackAdd;					//offset = 28, 4字节表示当前三昧真火产生的物攻加成
	DWORD	dwMagicAttackAdd;				//offset = 32, 4字节表示当前三昧真火产生的魔攻加成
	DWORD	dwDaoShuAttackAdd;				//offset = 36, 4字节表示当前三昧真火产生的道术加成
	DWORD	dwDefendAdd;					//offset = 40, 4字节表示当前三昧真火产生的物防加成
	DWORD	dwMagicDefendAdd;				//offset = 44, 4字节表示当前三昧真火产生的魔防加成
	DWORD	dwHitAdd;						//offset = 48, 4字节表示当前三昧真火产生的物理命中加成
	DWORD	dwMagicHitAdd;					//offset = 52, 4字节表示当前三昧真火产生的魔法命中加成
	DWORD	dwCruelAttackAdd;				//offset = 56, 4字节表示当前三昧真火产生的暴击加成
	DWORD	dwReboundHurtAdd;				//offset = 60, 4字节表示当前三昧真火产生的伤害反弹加成
	DWORD	dwAbsordBloodAdd;				//offset = 64, 4字节表示当前三昧真火产生的吸血加成
	DWORD	dwDestroyDefendAdd;				//offset = 68, 4字节表示当前三昧真火产生的破防加成
	DWORD	dwResistAdd;					//offset = 72, 4字节表示当前三昧真火产生的抵抗加成
	DWORD	dwAttackAvoidAdd;				//offset = 76, 4字节表示当前三昧真火产生的物理躲避加成
	DWORD	dwMagicAttackAvoidAdd;			//offset = 80, 4字节表示当前三昧真火产生的魔法躲避加成
	DWORD	dwLuckyAdd;						//offset = 84, 4字节表示当前三昧真火产生的幸运加成
	DWORD	dwNextHPAdd;					//offset = 84 + 4, 4字节表示下一等级三昧真火产生的HP加成
	DWORD	dwNextMPAdd;					//offset = 84 + 8, 4字节表示下一等级三昧真火产生的MP加成
	DWORD	dwNextAttackAdd;				//offset = 84 + 12, 4字节表示下一等级三昧真火产生的物攻加成
	DWORD	dwNextMagicAttackAdd;			//offset = 84 + 16, 4字节表示下一等级三昧真火产生的魔攻加成
	DWORD	dwNextDaoShuAttackAdd;			//offset = 84 + 20, 4字节表示下一等级三昧真火产生的道术加成
	DWORD	dwNextDefendAdd;				//offset = 84 + 24, 4字节表示下一等级三昧真火产生的物防加成
	DWORD	dwNextMagicDefendAdd;			//offset = 84 + 28, 4字节表示下一等级三昧真火产生的魔防加成
	DWORD	dwNextHitAdd;					//offset = 84 + 32, 4字节表示下一等级三昧真火产生的物理命中加成
	DWORD	dwNextMagicHitAdd;				//offset = 84 + 36, 4字节表示下一等级三昧真火产生的魔法命中加成
	DWORD	dwNextCruelAttackAdd;			//offset = 84 + 40, 4字节表示下一等级三昧真火产生的暴击加成
	DWORD	dwNextReboundHurtAdd;			//offset = 84 + 44, 4字节表示下一等级三昧真火产生的伤害反弹加成
	DWORD	dwNextAbsordBloodAdd;			//offset = 84 + 48, 4字节表示下一等级三昧真火产生的吸血加成
	DWORD	dwNextDestroyDefendAdd;			//offset = 84 + 52, 4字节表示下一等级三昧真火产生的破防加成
	DWORD	dwNextResistAdd;				//offset = 84 + 56, 4字节表示下一等级三昧真火产生的抵抗加成
	DWORD	dwNextAttackAvoidAdd;			//offset = 84 + 60, 4字节表示下一等级三昧真火产生的物理躲避加成
	DWORD	dwNextMagicAttackAvoidAdd;		//offset = 84 + 64, 4字节表示下一等级三昧真火产生的魔法躲避加成
	DWORD	dwNextLuckyAdd;					//offset = 84 + 68, 4字节表示下一等级三昧真火产生的幸运加成
	
};
#pragma  pack(pop)
//自定义飘红数据结构
struct CustomPiaohongMsg
{
	BYTE bNeedbk;			//是否需要描边
	BYTE byFontType;		//字体
	BYTE byFontSize;		//字大小
	BYTE byFontColor;		//字体颜色
	BYTE byBkColor;			//描边颜色
	string Message;			//飘红内容
};

// 纹佩部件
struct SubWenPei
{
	int id;
	int looks;
	int nums;
};

// 太极幻境奇遇物品奖励
struct QiYuRotItem
{	
	char szName[24];
	int looks;
};

//vip商行结构
struct S_VipTrade
{	
	BYTE  byEffected;   //是否有效,否则为过期
	BYTE  byVipType;    //Vip类型，1:体验,2:包月，3包季，4包年……
	DWORD tEndTime;     //到期时间  
	BYTE  byVipLevel;   //Vip等级
	int   iVipExp;      //Vip经验，累加后的结果
	bool  bAutoExtern;   //自动续费
	BYTE  byAddExpOneDay;//每天登录增加多少积分
	BYTE  byBoxOpened;//今天的5个宝箱是否开启, 每一个bit表示一个宝箱


	bool bNeedOpenWndWhenReceiveData;//收到协议后是否要打开窗口
};


//12宫布兵
struct S_BingBaseInfo
{
	string strName;//名字
	string strDesp;//描述	
};

#pragma pack(push, 1)
struct S_BingInfo
{
	BYTE byType;//种类
	char  szName[20];//名字
	BYTE Num;//剩余数量
	DWORD money;//花费荣誉点
	WORD ac1;//攻击下限
	WORD ac2;//攻击上限
	WORD df;//防御
	WORD hp;//攻击上限
	WORD wLooksShape;
};

struct S_BingNet
{
	S_BingNet()
	{
		wID = 0;
		byType = 0;
		byInOut = 0;
		iX = iY = 0;
		iRealX = iRealY = 0;
	}
	WORD wID;//兵卒编号
	BYTE byType;//种类	
	BYTE byInOut;//殿内外，0外，1内
	WORD  iX;
	WORD  iY;
	WORD  iRealX;
	WORD  iRealY;
};
#pragma pack(pop)

//12宫布兵
struct S_Bing
{
	S_Bing()
	:iCurNum(0)	
	,iStandContiFrame(200)
	,iRunContiFrame(200)
	,iRunFrameCount(0)
	,iStandFrameCount(0)
	,iAttackFrameCount(0)
	,iAttackFrameStart(0)
	,iFrameNow(0)
	,iAction(0)
	{}
	S_BingInfo info;
	int iCurNum; //当前个数

	int iAction; //动作	
	int iFrameNow;

	int iStandContiFrame; //站立持续时间
	int iStandFrameCount;

	int iRunContiFrame; //run持续时间
	int iRunFrameCount;

	int iAttackFrameCount;
	int iAttackFrameStart;
};

//异火战系统消息
struct S_YiHuoZhanMsg
{
	S_YiHuoZhanMsg()
	:iShowTime(0)
	,iStartTime(0)
	{
		memset(szType,0,sizeof(szType));
		memset(szBtn1,0,sizeof(szBtn1));
		memset(szBtn2,0,sizeof(szBtn2));
	}
	char szType[10];
	char szBtn1[10];
	char szBtn2[10];
	string strMsg;
	DWORD iShowTime;
	DWORD iStartTime;
};

enum stAlarmState
{
	DEADLY = 0,
	VERY_SLOW,
	SLOW,
	NORMAL,
	GOOD,
	VERY_GOOD,

	STATE_NUM
};



struct stAlarm
{
	int	    iState;
	int		x,y;
	char	sAlarmName[64];
	char	sAlarmTip[STATE_NUM * 64];
};
#pragma pack(push,1)
//骑术主界面
struct HorsemanshipProtocalSkill
{
	char	szSkillName[16];//技能名称
	DWORD	dwSkillID;		//技能ID
	BYTE	byLevel;		//技能等级
	BYTE	byPassiveOrNot;	//被动技能	
};

struct HorsemanshipTotalEffect{
	int hp;			//HP
	int mp;			//MP
	int dc;			//物理攻击
	int mc;			//魔法攻击
	int sc;			//道术攻击
	int ac;			//物理防御
	int mac;		//魔法防御
	int rate;		//物理命中
	int avoid;		//物理躲避
	int mrate;		//魔法命中
	int mavoid;		//魔法躲避
	int brust;		//暴击
	int absorbedHP;	//吸血
	int rebound;	//反弹
	int realDef;	//抵抗
	int ignoredDef;	//破防	
}; // struct HorsemanshipTotalEffect 
#pragma pack(pop)
struct HorsemanshipProtocalInfo
{
	HorsemanshipProtocalInfo():Grade(0),Exp(0),MaxExp(0){}

	//基础信息	
	int Grade;
	DWORD Exp;
	DWORD MaxExp;
	//骑术收益
	HorsemanshipTotalEffect effect;
	//骑术技能
	//int count;
	std::vector<HorsemanshipProtocalSkill> vecSkill;

	void clear()
	{
		Grade = 0;
		Exp = 0;
		MaxExp = 0;
		memset(&effect, 0, sizeof(HorsemanshipTotalEffect));
		vecSkill.clear();
	}
};



typedef std::vector<_QunYingReportStruct> VQunYingReportStruct;
typedef std::vector<_RelationStruct> VRelationStruct;
typedef std::vector<S_CommonConfirm> VCommonConfirm;
typedef std::vector<NoteTitle>       VNoteTitle;
typedef std::vector<_LeaveWords>     VLeaveWords;
typedef std::map<std::string,std::string> MStringPair;
typedef vector<_CharInfo>    VCharInfo;
typedef vector<_OtherMsg>    VOtherMsg;
typedef vector<_TimeOut>    VTimeOut;
typedef vector<CustomPiaohongMsg> VCustomPiaohongMsg;
typedef std::vector<SubWenPei> VSubWenPei;
typedef std::vector<QiYuRotItem> VQiYuRotItem;
typedef vector<S_Bing>	BingList;



class COtherData
{
public:

	COtherData(void);
	~COtherData(void);

	void    Clear();
	void    JumpClear();
	DWORD   GetHairColor(int i);
	DWORD   GetBodyColor(int i,int iSex);
	VCharInfo& GetCharList();
	const  char* GetCharName();
	void    SetCharNo(int i);
	int     GetCharNo();
	void    CopyToSelf();
	VCharInfo& GetDelCharList();
	bool    IsNewUser();
	bool    IsAutoEnter() ;
	void    SetAutoEnter(bool b);

	bool    IsReceivedCharList();
	void    SetReceivedCharList(bool b);

	DWORD   GetExpUpTime();
	void    SetExpUpTime(DWORD dwTime);
	DWORD   GetMapInTime();
	void    SetMapInTime(DWORD dwTime);

	bool   IsDay();
	void   SetDay(bool bDay);
	COtherPlayer& OtherPlayer();

	int	   GetJieYiExp()	{ return m_iExp100; }
	void   SetJieYiExp(int val)    { m_iExp100 = val; }

	const std::string& GetJieYiLeader()				{ return m_strJieYiLeader; }
	void   SetJieYiLeader(const std::string& val)   { m_strJieYiLeader = val; }

	VRelationStruct& GetRelationVector();
	VCommonConfirm&  GetCommonConfirmVector();
	VNoteTitle &    GetNoteTitle();
	VLeaveWords &    GetLeaveWordsVector();
	MStringPair &    GetNoteBody();

	VOtherMsg& GetOtherMsg();  //左侧到右侧飘红
	VOtherMsg& GetOtherMsg2();  //闪现飘红
	VOtherMsg& GetAnotherMsg(); //右侧到左侧飘红
	VCustomPiaohongMsg& GetCustomPiaohong();
	void SetTelStr(const char* str,WORD color); //设置千里传音
	const char * GetTelStr(WORD& color);  //得到千里传音
	DWORD GetTelTime(){ return m_dwTelTime;}

	bool IsAutoLeaveWord();
	void SetAutoLeaveWord(bool bAutoLeaveWord);
	_OffLinePrivate & GetOffLinePrivate();
	int	GetDelFriendNo();
	void  SetDelFriendNo(int i);
	int GetFindPathReason();
	void SetFindPathReason(int iReason);
	void GetFindPathClickNpcXY(int &iX,int &iY);
	void SetFindPathClickNpcXY(int iX,int iY);
	DWORD GetEnterGameTime();
	void SetEnterGameTime(DWORD dwTime);
	DWORD GetLastMovePosTime();
	void SetLastMovePosTime(DWORD dwTime);
	DWORD	GetCenserStartTime();
	void	SetCenserStartTime(DWORD dwStartTime);
	DWORD   GetCenserDurTime();
	void    SetCenserDurTime(DWORD dwDurTime);
	bool    IsCenserStart();
	void    SetCenserStart(bool bStart);
	const char* GetDeleteMember();
	void  SetDeleteMember(const char* strName);

	void ParseNote(std::string& str);

	void SetUpdateMedalExp(int i,DWORD dwData){ m_dwUpdateMedalExp[i] = dwData;}
	DWORD GetUpdateMedalExp(int i){ return m_dwUpdateMedalExp[i];}
	WUXINGFLAG& GetWuXingFlag(){return m_sWuXingFlag;}
	bool GetSecondKillStatus(){return m_bShowSecondKill;}
	void SetSecondKillStatus(bool b){m_bShowSecondKill = b;}

	DWORD GetSetMsgStartTime();
	void  SetSetMsgStartTime(DWORD dwTime);
	string& GetSetMsg();
	_MazeMap& GetMazeMap(){ return m_MazeMap;}

    const vector<DWORD>& GetSummonPoints(){ return m_vecSummonPoint;}
    bool IsSummonPoint(DWORD pos){ return find(m_vecSummonPoint.begin(),m_vecSummonPoint.end(),pos) != m_vecSummonPoint.end();}
	
	bool IsRtsSummonPoint(int x,int y);

	Magic_Show_s** GetFireShowList(int x,int y);
	tex_show_t*  GetFireTexShow(){return m_TexShow;}
	void SetFireTexShow(tex_show_t* show){m_TexShow = show;}
	void ClearFireTexShow();

	bool ClearFireShowList(int x,int y);
	void ClearAllFireShowList();

	void SetRadioOpen(bool b){ m_bRadioOpen = b;}
	bool GetRadioOpen(){return m_bRadioOpen;}

	_KfzGuild & GetKfzGuildData(){return m_KfzGuildData;}
	VTimeOut & GetTimeOut();
	bool AddTimeOut(_TimeOut &timeout);

	void SetChangeLogo(bool b)				{m_bChangeLogo = b;}
	bool IsChangeLogo()					    {return m_bChangeLogo;}

	void SetReadUsbEkeyDelayStartTime(DWORD dwTime);
	DWORD GetReadUsbEkeyDelayStartTime();

	void SetPhyleName(string& strName){m_strPhyleName = strName;}
	string& GetPhyleName(){return m_strPhyleName;}
	void SetPhyleMaster(string& strMaster){m_strPhyleMaster = strMaster;}
	string& GetPhyleMaster(){ return m_strPhyleMaster;}
	BYTE GetPhyleType(){return m_byPhyleType;}
	void SetPhyleType(BYTE byType){ m_byPhyleType = byType;}
	bool IsPhyleMaster(){return (m_strPhyleMaster.compare(const_cast<char*>(SELF.GetName())) == 0);}

	void GetLogoTex(const char * pBuf);

	DWORD GetExploreID(){return m_dwExploreID;}
	void SetExploreID(DWORD id){m_dwExploreID = id;}

	inline BYTE GetCreditAlertState(){return m_byCreditAlertState;}
	inline void SetCreditAlertState(BYTE byValue){m_byCreditAlertState = byValue;}

	void       SetReLivePrice(int i){ m_iReLivePrice = i; }
	int       GetReLivePrice(){ return m_iReLivePrice; }

	int GetDelCharSelectIdx() const { return m_iDelCharSelectIdx; }
	void SetDelCharSelectIdx(int val) { m_iDelCharSelectIdx = val; }
	int GetUnDelCharSelectIdx() const { return m_iUnDelCharSelectIdx; }
	void SetUnDelCharSelectIdx(int val) { m_iUnDelCharSelectIdx = val; }
	sTXZG &GetTXZG() { return m_sTXZG; }
	sTSJB &GetTSJB() { return m_sTSJB; }
	MeritoriousnessUpdate& GetMeritoriousnessUpdate(){ return m_MeritoriousnessUpdate;} 
	SanWeiZhenHuoInfo& GetSanWeiZhenHuoInfo(){ return m_SanWeiZhenHuoInfo;}
	void SetMeritoriousnessUpdate(DWORD dwNextLevel,DWORD dwMostPerHour,DWORD dwMeriToExp,DWORD dwPresToExp);
	bool IsNeedFlashProduceTab();
	void SetNeedFlashProduceTab(bool val);
	int  GetFlashRelationTabType();
	void SetFlashRelationTabType(int val);

	bool HaveRequestQunYingReport() const { return m_bHaveRequestQunYingReport; }
	void SetHaveRequestQunYingReport(bool val) { m_bHaveRequestQunYingReport = val; }
	VQunYingReportStruct& GetQunYingReports();
	DWORD GetHunDunNvWangLock() const { return m_dwHunDunNvWangLock; }
	void SetHunDunNvWangLock(DWORD val) { m_dwHunDunNvWangLock = val; }
	void SetLockHunDunNvWangLockID(DWORD val){m_dwLockHunDunNvWangLockID = val;}
	DWORD GetLockHunDunNvWangLockID(){return m_dwLockHunDunNvWangLockID;}

	//i:0=边纹，1=底纹，2=主纹，3=辅纹
	VSubWenPei& GetSubWenPei(int i);

	//按绘制顺序 i:0=边纹，1=底纹，2=辅纹，3=主纹
	int GetSubWenPeiDraw(int i);

	DWORD GetLastHeartBeat() const { return m_dwLastHeartBeat; }
	void SetLastHeartBeat(DWORD val) { m_dwLastHeartBeat = val; }

	bool IsCanClickQiYu();
	void SetHaveQiYu(bool val)	{ m_bHaveQiYu = val;}
	bool IsHaveQiYu()			{ return m_bHaveQiYu; }
	void SetQiYuState(int i);
	int GetQiYuState()			{ return m_iQiYuState; }
	void SetQiYuFrame(int i)	{ m_iQiYuFrame = i;}
	int GetQiYuFrame()			{ return m_iQiYuFrame; }
	void SetQiYuShow(bool val)	{ m_bQiYuShow = val;}
	bool GetQiYuShow()			{ return m_bQiYuShow; }

	VQiYuRotItem& GetQiYuRotItems()			{ return m_vQiYuItems; }


	void SetFireArtTime(DWORD time);
	DWORD GetFireArtTime();
	void SetFireArtStartTime(DWORD time);
	DWORD GetFireArtStartTime();

	S_VipTrade &GetVipTradeStruct();
	
	BingList& GetBings()				{ return m_vBings; }
	S_Bing* GetBingByType(BYTE byType);

	S_YiHuoZhanMsg& GetYiHuoZhanMsg()	{ return m_YiHuoMsg; }
	map<int,string> &GetArrorTipsMap() { return m_MArrorTips; }

	void InitAlarmState(void);//初始化性能显示信息

	DWORD	GetAlarmColor(int i){return m_dwAlarmColor[i];}//由外部保证参数的正确
	stAlarm	&GetAlarmState(int i){return m_Alarm[i];}//由外部保证参数的正确
	void UpdateLocalState(void);
	void SetAlarmState(int iAlarm, int iState);

	HorsemanshipProtocalInfo& GetHorseManShipInfo(){ return m_sHorseManShip; }

private:
	DWORD	m_dwHairColor[MAX_HAIR_COLOR];
	DWORD	m_dwBodyColor[2][MAX_BODY_COLOR];
	DWORD   m_dwEnterGameTime;//进入游戏的时间
	DWORD   m_dwLastMovePosTime;//是近一次随机,瞬移或换地图,用来控件物品出现是否放物品跳动特效

	VCharInfo m_VDelCharList; //已删除角色信息
	VCharInfo m_VCharList; //选中的角色信息
	int       m_iDelCharSelectIdx;//删除角色时选中的角色编号
	int       m_iUnDelCharSelectIdx;//恢复角色时选中的角色编号

	VOtherMsg m_VOtherMsg;  //左侧到右侧飘红
	VOtherMsg m_VOtherMsg2;  //闪现飘红
	VOtherMsg m_VAnotherMsg; //右侧到左侧飘红
	VCustomPiaohongMsg m_VCustomMsg;	 //自定义飘红

	int     m_iCharNo;     //当前选中的角色编号
	bool    m_bRadioOpen;  //

	bool    m_bNewUser;
	bool    m_bAutoEnter;  //自动进入游戏
	bool    m_bReceivedCharList;
	bool    m_bAutoLeaveWord;//发消息时不在线是否自动留言

	DWORD   m_dwExpUpTime;
	DWORD   m_dwMapInTime;
	bool	m_bDay;//白天还是晚上
	string   m_strTel; //千里传音内容
	WORD     m_wTelColor;//千里传音颜色
	DWORD    m_dwTelTime;//千里传音时间
	DWORD    m_dwReadUsbEkeyDelayStartTime;

	COtherPlayer	m_OtherPlayer;
	VCommonConfirm	m_VCommonConfirm;//所有需要回答的请求
	_OffLinePrivate m_sOffLinePrivate;
	VLeaveWords		m_vecLeaveWords;   //所有未读的留言
	VNoteTitle		m_NoteTitle;
	MStringPair		m_NoteBody;
	VRelationStruct m_vecRelation;//关系系统

	int				m_iExp100;//结义经验加成比例
	std::string		m_strJieYiLeader;

	int	  m_iDelFriend;//删除的好友序号
	int   m_iFindPathReason;//自动寻路的原因，0为普通寻路,1为自动寻路，走到目标点后自动点击m_iFindPathClickNpcX,m_iFindPathClickNpcY处的NPC
	int   m_iFindPathClickNpcX;
	int   m_iFindPathClickNpcY;//寻路结束后如果要点NPC，该NPC的位置
	DWORD m_dwCenserStartTime;//香炉特效的开始时间,0表示没有开始
	DWORD m_dwCenserDurTime;//香炉特效的持续时间
	bool  m_bCenserStart;//是否香炉特效
	string m_strDeleteMember;//删除队友操作时记下被删除对象的名字
	string m_strSetMsg;
	DWORD  m_dwSetMsgStartTime;
	DWORD  m_dwUpdateMedalExp[8];
	WUXINGFLAG m_sWuXingFlag;//五行旗
	bool	  m_bShowSecondKill;//秒杀槽    
    vector<DWORD> m_vecSummonPoint; //五行玄关召唤点坐标
	_MazeMap      m_MazeMap;
	Magic_Show_s* m_FireMagic[MAPARR_WIDTH][MAPARR_WIDTH];
	tex_show_t*  m_TexShow;
	_KfzGuild m_KfzGuildData; //本服参加跨服战的行会信息
	VTimeOut m_TimeOut;//读秒倒计时结构

	bool   m_bChangeLogo;		//行会会长是否改变Logo
	string m_strPhyleName;		//宗派名字
	string m_strPhyleMaster;	//宗族族长名字
	BYTE	m_byPhyleType;//宗派修神还是修魔

	DWORD m_dwExploreID;	//探宝凤凰的ID
	BYTE m_byCreditAlertState;//0:不提醒,1:已经点了开始按,2:点了开始并且收到了信用信用,此时判断igw登录状态,登录了而且有欠款就需要提醒还款
	
	int          m_iReLivePrice;//还魂神符价格
	sTXZG        m_sTXZG;//铁血战副本数据
	sTSJB		 m_sTSJB;//世界杯数据
	vector<SPoint> m_VRtsSummonPoint;
	MeritoriousnessUpdate  m_MeritoriousnessUpdate;
	bool  m_bNeedFlashProduceTab;//是否要闪烁生产技能页签
	int   m_iFlashRelationTabType;//社会关系页签闪烁类型

	bool m_bHaveRequestQunYingReport;
	VQunYingReportStruct m_VQunYingReports; // 群英战报

	DWORD m_dwLastHeartBeat;//上一次心跳时间

	VSubWenPei m_vSubWenPei[4];
	map<int,string> m_MArrorTips;//泡泡内容

	bool m_bHaveQiYu;
	int m_iQiYuState; // 0:奇遇触发 1:奇遇未完成 2:奇遇完成，等待滚动奖励
	int m_iQiYuFrame;
	bool m_bQiYuShow;

	VQiYuRotItem m_vQiYuItems;	
	SanWeiZhenHuoInfo	m_SanWeiZhenHuoInfo;


	// 真火炼化冷却时间
	DWORD m_dwFireArtTime;
	DWORD m_dwFireArtStartTime;


	S_VipTrade  m_SVipTrade;

	//12宫布兵
	BingList			m_vBings;		//兵	

	//异火战系统消息
	S_YiHuoZhanMsg m_YiHuoMsg;

	//性能状态
	DWORD	m_dwAlarmColor[STATE_NUM];
	stAlarm	m_Alarm[3];

	DWORD m_dwLockHunDunNvWangLockID;//被锁住的混沌女王的id
	DWORD m_dwHunDunNvWangLock;//混沌女王锁链状态

	HorsemanshipProtocalInfo m_sHorseManShip;
};

#include "OtherData.inl"

extern COtherData g_OtherData;
