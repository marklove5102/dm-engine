#pragma once
#include "Global/Global.h"
#include "Global/StdHeaders.h"

struct sWenPeiInfo
{
	sWenPeiInfo()
	{
		looks = 0;
		for (int i = 0; i < 3; ++i)
		{
			rate[i] = 0;
		}
		shengwang = 0;
	}

	int looks;	//配件looks
	int rate[3];		//分解精华的概率
	int shengwang;		//分解花费的声望
	std::string name;	//配件名称
	std::string strType;
	std::string strValue;	
};

struct sWenPeiTask
{
	sWenPeiTask()
	{
		looks = 0;
		clear();
	}

	void clear()
	{
		iMaxNum = 1;
		iFinishNum = 0;
		taskstate = 0;
	}
	
	int looks;			//配件looks
	std::string name;	//配件名称
	std::string content[2];//配件任务Tips说明	
	int taskstate;		//任务状态,0:未接,1:已接,2:完成,可以领取纹配
	//任务进度
	int iMaxNum;
	int iFinishNum;		
};

typedef map<int,sWenPeiInfo> MWenPeiCfg;
typedef vector<sWenPeiTask> VWenPeiTask;


//活跃度
struct S_ActivityRate
{
	S_ActivityRate()
	{
		dwID = 0;
	}

	DWORD dwID;//编号
	string strTime;//活动时间
	string strName;//活动名称
	string strNPCName;//NPC
	string strNeedLevel;//等级限制
	string strAward;//奖励的活跃度
	string strNpcCommand;//NPC自动寻路
	string strTips;//说明文字
};
typedef vector<S_ActivityRate> VActivityRate;


class CXmlCfg
{
public:
	CXmlCfg(void);
	~CXmlCfg(void);

public:
	void    Clear();
	void ReloadXmgCfg();
	sWenPeiInfo* GetWenPeiCfg(int looks);
	VWenPeiTask& GetWenPeiTasks(int i);
	sWenPeiTask* GetWenPeiTask(int looks);

	VActivityRate& GetActivityRateVector(){return m_VActivityRate;}


protected:	
	bool 	ReLoadVActivityRateCfg();
	bool 	ReLoadWenPeiCfg();

	MWenPeiCfg m_kWenPeiCfgs;
	VWenPeiTask m_kWenPeiTasks[4];
	VActivityRate m_VActivityRate;
};

extern CXmlCfg g_XmlCfg;