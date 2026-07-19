#pragma once

#include "Global/Global.h"
#include "Global/StdHeaders.h"

//物品特效配置信息的基本结构
//关键字为 物品的looks编号
struct SItemEffect
{
	int iTexturePackage;     //图包的位置
	int iTextureID;//图片位置
	int iRenderMode;         //绘制模式
	int iOffsetX;            //图片的偏移位X
	int iOffsetY;			 //图片的偏移位置Y
}; 

//特效相关的配置
struct SItem2MagicEffect
{
	int iMagicID;
	int iAlpha; //绘制方式
	int iFx,iFy;
	int iUsedXY;
};

typedef map<string,string,std::less<string> > StringMap;

class CItemCfgMgr
{
public:
	CItemCfgMgr();
	~CItemCfgMgr();	
	string ParseGoodsTips(const char*pkey);//解析一条物品的说明信息
    bool   ParseItemEffect(const char* psect,const char*pkey , SItemEffect &sInfo);//解析一条物品的特效信息
	bool   ParseItem2Magic(const char* psect, const char* pkey, SItem2MagicEffect& sInfo);
	bool   ParseItemFeed(const char* psect,const char* pkey,unsigned char& byValue);
	bool   ParseCardInfo(const char* psect,const char* pkey,const char* pName,const char* pDesc);
	int	   ParseRefFoundInfo(const char* psect,const char* pkey,int iLevel);
	void   ParseInfo(const char* psect,const char* pkey,string& strDes);
	void   ReLoad();
	StringMap &GetMap(){return m_mEffectCfg;}


private:
	StringMap m_mEffectCfg;
};

extern CItemCfgMgr g_ItemCfgMgr;


struct _sPromptInfo
{
	bool  bEmergent;
	DWORD dwID;
	string strTile;
	string strDes;
};

struct SPromptInfo
{
	string strSec;
	DWORD dwSecID;
	std::vector<_sPromptInfo> vPromptInfo;
};
typedef std::vector<SPromptInfo> VSPromptInfo;

struct TaskTreeNode
{
	string strSimDes;
	string strDes;

	string strSubSimDes;
	string strSubDes;
};

struct TaskTree
{
	bool   bSequence;
	string strTheme;
	vector<TaskTreeNode> vTask;
	TaskTree()
	{
		bSequence = false;
		strTheme = "";
		vTask.clear();
	}
};
typedef vector<TaskTree> VTaskTree;

struct _ActivityInfo
{
	DWORD dwID;
	string strTime;
	string strName;
	string strActType;
	string strConfine;
	string strAward;
	string strNPCCont;
	string strNPCName;
	string strFinishNum;
};
struct ThemeInfo
{
	WORD wID;
	string strSec;
	string strTheme;
};

struct TimerActivityInfo
{
	bool bRuned;
	DWORD dwID;
	float fStartTm;
	float fEndTm;
	string strTile;
	string strContent;
	TimerActivityInfo()
	{
		bRuned = false;
		dwID = 0;
		fStartTm = 0.0;
		fEndTm = 0.0;
		strTile.clear();
		strContent.clear();
	}
};
typedef std::vector<TimerActivityInfo> VTimerActivityInfo;

//  [4/15/2010 dujun]可接任务
struct TaskAva
{
	WORD wTaskID;
	string strTaskName;
	string strTaskDetail;//任务描述
	WORD wLevel; //任务推荐等级
	string strNPC;
	string strPath;//寻路的command
	TaskAva()
	{
		wTaskID = 0;
		strTaskName.clear();
		strTaskDetail.clear();
		wLevel = 0;
		strNPC.clear();
		strPath.clear();
	}
};
typedef vector<TaskAva> VTaskAvaList;


class CPromptInfoMgr
{
public:
	CPromptInfoMgr();
	~CPromptInfoMgr();	

	void LoadTaskTree();
	VTaskTree& GetTaskTree(){return m_vTaskTree;}
	void LoadPromptInfo();
	VSPromptInfo& GetPromptInfo(){return m_vPromptInfo;}
	void SetSelectedTaskTree(TaskTree* pTR){ m_pSelectedTR = pTR;}
	TaskTree* GetSelectedTaskTree(){return m_pSelectedTR;}
	int  FindTaskTree(string& str);

	void SetSelectedTaskNode(TaskTreeNode* pTN){ m_pSelectedNode = pTN;}
	TaskTreeNode* GetSelectedTaskNode(){return m_pSelectedNode;}
	_sPromptInfo* FindPromptInfo(DWORD dwID);
	_sPromptInfo* FindNewHandProInfo(DWORD dwID);
	void GetActivityInfo(_ActivityInfo** pArray);
	_ActivityInfo* GetActivityInfo(DWORD dwID);
	std::vector<_ActivityInfo>& GetActivityInfo(){return m_vActivityInfo;}
	VTimerActivityInfo& GetTimerActivityInfo(){return m_vTimerActInfo;}
	ThemeInfo* GetThemeInfo(WORD wID);
	ThemeInfo* GetGameThemeInfo(WORD wID);

	//  [4/15/2010 dujun]
	void LoadAvaTask();
	void ParseAvaTask(string&str,TaskAva& task);
	void UpdateAvaTask(int iLevel);//根据当前等级判断哪些任务可接
	VTaskAvaList& GetAvaTaskList() {return m_vAvaTask;}

protected:
	bool ParseTaskTree(string&strSect,string& strKey,string& strDes,string& str);
	void ParseTaskTree(string& str,string& strSimDes,string& strDes,string& strSubSimDes,string& strSubDes);
	void ParseTaskTreeDetail(string& str,string& strSimDes,string& strDes);
	bool ParsePromptInfo(string& strSect,string& strKey,string& strDes,DWORD& dwID,string& str);
protected:
	TaskTree*	  m_pSelectedTR;//当前选取中的任务树
	TaskTreeNode* m_pSelectedNode;//当前选中的任务树结点

	VTaskTree	 m_vTaskTree;
	VSPromptInfo m_vPromptInfo;
	VTimerActivityInfo m_vTimerActInfo;
	std::vector<_sPromptInfo> m_vNewHandInfo;
	std::vector<_ActivityInfo> m_vActivityInfo;
	std::map<WORD,ThemeInfo> m_mThemeInfo;
	std::map<WORD,ThemeInfo> m_mGameThemeInfo;

	VTaskAvaList m_oriAvaTask; //[4/15/2010 dujun] 可接的任务列表
	VTaskAvaList m_vAvaTask;
};
extern CPromptInfoMgr g_PromptInfoMgr;