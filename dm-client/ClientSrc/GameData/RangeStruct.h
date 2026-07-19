#pragma once

#include "Global/Global.h"

struct ALL_RANGE	//等级总排名 职业排名 每等级排名
{
	WORD place;			//名次
	char name[14];	//名字
	BYTE  level;			//等级
	BYTE  work;			//职业(0,1,2)
	BYTE  sex;			//性别(0,1)
	char guild[30];	//行会
	BYTE  exp;	//当前经验百分数
	BYTE  merwork;	//元神职业(0,1,2)
	BYTE  merlevel;	//元神等级
	BYTE  inmerwork;//内敛元神职业(0,1,2)
	BYTE  inmerlevel;//内敛元神等级
	BYTE  byflylevel;//主体飞升境界
	BYTE  bymerflylevel;//主体飞升境界
	BYTE  byinmerflylevel;//主体飞升境界

};

struct TEACHER_RANGE
{
	WORD place;			//名次
	char name[14];	//名字
	WORD tudiNum;			//出师土地数
	BYTE  level;			//等级
	BYTE  work;			//职业(0,1,2)
	BYTE  sex;			//性别(0,1)
	char guild[30];	//行会
	WORD repute;	//声望值
	BYTE  nowtudiNum;	//当前徒弟数 
	BYTE  byflylevel;   //飞升境界
};

struct PET_RANGE	//宠物豹排名
{
	WORD place;			//名次
	char name[14];		//名字
	BYTE level;			//等级(0-7，8-15,16以上)
	WORD exp;			//经验
	char Fenhao[14];	//封号
	char mainName[14];	//主体名字
	BYTE work;			//主体职业
	BYTE type;			//灵兽种类0为豹子，1为狮子,2为骑令
};


struct ZHUZAO_RANGE	//铸造排名
{
	WORD place;			//名次
	char name[14];	//名字
	WORD exp;			//积分
	BYTE level;			//等级
	BYTE work;			//职业(0,1,2)
	BYTE sex;			//性别(0,1)
	BYTE  byflylevel;   //飞升类型,0:未飞升,1:神,2:魔
};

struct GUILD_RANGE	//行会排名
{
	WORD place;			//名次
	char name[30];	//行会名字
	WORD num46;			//46等级以上人数
	WORD numShadow;//元神人数
	WORD num;			//总人数
	DWORD exp;			//行会经验
	BYTE towerLevel;	//行会通灵塔等级
};

struct MIEMO_RANGE	//灭魔排名
{
	WORD place;			//名次
	WORD cishu;			//灭魔次数
	char name[14];		//名字
	BYTE level;			//等级
	BYTE work;			//职业(0,1,2)
	BYTE sex;			//性别(0,1)
	char guild[30];		//行会
	BYTE  byflylevel;   //飞升境界
};

struct ARENA_RANGE	//英雄擂台排名
{
	WORD place;			//名次
	WORD exp;			//积分
	char name[14];	//名字
	BYTE level;			//等级
	BYTE work;			//职业(0,1,2)
	BYTE sex;			//性别(0,1)
	WORD	cishu;			//场次
	DWORD	money;			//奖金
	char Fenhao[14];		//封号
	BYTE  byflylevel;//飞升境界
};

struct RS_RANGE	//恶魔城排名
{
	WORD place;			//名次
	char name[14];	//名字
	BYTE level;			//等级
	BYTE	work;			//职业(0,1,2)
	DWORD	haoshi;		//耗时
	BYTE  byflylevel;//飞升境界
};

struct WUGUAN_RANGE //武馆
{
	WORD place;			//名次
	char name[14];	//名字
	BYTE work;			//职业(0,1,2)
	WORD exp;			//积分
	char Fenhao[14];	//封号

};

struct PHYLE_RANGE		//宗派
{
	WORD place;			//名次
	char name[16];		//宗派名字
	WORD SeizeNPCNum;	//抢夺npc人数
	WORD MemberNum;		//人数
	WORD FlyMemberNum;	//飞升人数
};

struct PRODUCE_SKILL_RANGE 
{
	WORD place;			    //名次
	char cPlayerName[14];	//玩家名
	BYTE byLevel;	     	//等级
	BYTE byWork;			//职业(0,1,2)
	BYTE bySex;				//性别(0,1)
	BYTE bySkillID;			//技能ID
	BYTE bySkillLevel;		//技能等级
	BYTE byExp;			    //当前经验百分数
	char cGuild[14];		//行会
};

struct RTS_RANGE 
{
	WORD place;				//名次
	char cPlayerName[14];	//玩家名
	BYTE byLevel;	     	//等级
	BYTE byWork;			//职业(0,1,2)
	BYTE bySex;				//性别(0,1)
	int  iWinNum;            //胜利次数
	char cGuild[14];		 //行会
};

struct RTS_FIRE//真火排行榜 
{
	WORD place;			//名次
	char name[14];      //名字
	BYTE work;			//职业(0,1,2)
	BYTE sex;           //性别
	WORD wSamadhiFireLevel; //真火等级
	BYTE bySamadhiFire; //真火值,百分数
	char guild[30];	    //行会名字
};

//神翼等级榜
struct PINNA_RANGE
{
	WORD place;			//名次
	char name[14];      //名字
	BYTE work;			//职业(0,1,2)
	BYTE sex;           //性别
	WORD wPinnaLevel;   //神翼等级
	char guild[30];	    //行会名字
};

//骑术排行榜
struct HORSEMANSHIP_RANGE
{
	WORD place;			//名次
	char name[14];      //名字
	BYTE work;			//职业(0,1,2)
	BYTE sex;           //性别
	WORD wHorsemanshipLevel; //骑术等级
	BYTE byHorsemanship; //骑术经验百分比
	char guild[30];	    //行会名字
};


////////////////////////////////////////////////
//排名类型
enum RANGE_TYPE
{

	//RT_KFZ = 32,      //天下霸者榜
};

//排名数据结构
struct STRUCT_RANGE_DADA
{
	WORD	version;//版本
	void *  vData;	//排名数据
};

#define RANGE_DADA  std::map<WORD,void*>	//key=位置
#define LP_RANGE_DADA  std::map<WORD,void*>*
#define ALL_RANGE_DADA std::map<WORD,LP_RANGE_DADA>
extern  std::map<WORD,LP_RANGE_DADA>		m_mapRangeData;	//key大类+小类,void* 代表STRUCT_RANGE_DADA

extern  std::map<WORD,LP_RANGE_DADA>		m_mapMyRangeData;
extern  std::map<WORD,char>				m_mapMyRangeQushi;	//个人排名趋势
extern  std::map<WORD,WORD>				m_mapTotalNum;	//排名数
extern  std::map<WORD,WORD>				m_mapRangePos;	//上一次的滚柱及小类、pos位置,key=类型
extern	WORD m_old_range;	//旧的排名
extern  WORD  g_iSetRangeType;//打开后要求设定的排名类别
extern  WORD  g_iSetSmallRangeType;//打开后要求设定的排名子类别
extern  WORD g_Range[32];	//排名类型
extern  WORD g_RangeNum;
extern  WORD g_RangeVesion[32]; //每个类型version
extern  bool g_HaveSBK;  //拥有沙城
extern  std::map<WORD,WORD>				m_HesternalPlace; //昨日名次
extern	string		 g_strFindRange;  //需要查找好友名次的名字


/////////////////////////////////////////////////
int NewStruct(WORD type,void	**RangeData);
void DeleteStruct(WORD type,void	*RangeData);
bool FindData(WORD type,WORD startPlace ,void **RangeData,LP_RANGE_DADA *lpRangMap);
void ClearAllRangeData();
void ClearMyRangeData();