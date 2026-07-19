#pragma once

#include "AIMedicaDefine.h"
#include "Global/Global.h"
#include "Global/StringUtil.h"
#include "GameData/WooolStoreData.h"

class CAIConfigMgr
{
public:
	CAIConfigMgr(void);
	~CAIConfigMgr(void);

	bool  IsAbsoluteLock()  {   return m_bIsAbsoluteLock;  }
	bool  IsMessageAwoke()  {   return m_bMessageAwoke;	   }
	bool  IsKeepOutInfo()	{	return m_bKeepOutInfo;	   }
	bool  IsMountain()      {   return m_bIsMountain;      }
	bool  IsBury()          {   return m_bIsBury;          }
	bool  IsShowExtendShortCut() { return m_bShowExtendShortCut; }
	bool  IsGoPickup()      {   return m_bIsGoPickup;      }
	bool  IsAutoFindWay()   {	return m_bIsAutoFindWay;   } 
	bool  IsAutoPickup()    {   return m_bIsAutoPickup;    }
	bool  IsStarObject()    {   return m_bIsStarObject;    }
	bool  IsShowObjectName(){   return m_bIsShowObjectName;}
	bool  IsShowPlayerName(){   return m_bIsShowPlayerName;}
	bool  IsShowArrow()     {   return m_bIsShowArrow;     }
	bool  IsDurationPrompt(){   return m_bIsDurationPrompt;}
	bool  IsBossPrompt()    {   return m_bIsBossPrompt;    }
	bool  IsObjectPrompt()  {   return m_bIsObjectPrompt;  }
	bool  IsTeamColor()     {   return m_bIsTeamColor;     }
	bool  IsAttackColor()   {   return m_bIsAttackColor;   }
	bool  IsCloseTeam()     {   return m_bIsCloseTeam;     }
	bool  IsDismount()      {   return m_bIsDismount;      }
	bool  IsBlackFilter()   {   return m_bIsBlackFilter;   }
	bool  IsShowBestItem()  {   return m_bIsShowBestItem;  }
	bool  IsAutoMagicAttack(){  return m_bIsAutoMagicAttack;}
	bool  IsRunAttack()     {   return m_bIsRunAttack;     }
	bool  IsPKAlarm()       {   return m_bIsPKAlarm;       }
	bool  IsQuickOutGame()  {   return m_bIsQuickOutGame;  }
	bool  IsShowNpcPriority()  {   return m_bShowNpcPriority;  }
	bool  IsShowNewHandPrompt(){   return m_bShowNewHandPrompt;}
	bool  IsShowHpChangeEffect(){   return m_bShowHpChangeEffect;}
	bool  IsAcceptFrientAndTrade(){   return m_bAcceptFrientAndTrade;}
	//bool  IsAutoFeed()      {   return m_bAutoFeed;  }
	bool  IsAutoTeamVoice() {   return m_bAutoTeamVoice;  }
	bool  IsAutoShowActLog(){   return m_bAutoShowActLog; }

	bool  IsPkAllAttack()	{   return m_bPkAllAttack;	   }
	bool  IsAllowFly()      {   return m_bIsAllowFly;      }
	bool  IsShowSysInfo()	{   return m_bShowSysInfo;	   }
	bool  IsRunNotStop()    {   return m_bRunNotStop;      }
	int   GetHuiChengShiPosition() { return m_nHuiChengShiPosition; }

	bool  IsAutoQuit()      {   return m_bAutoQuit;        }
	int   GetAutoQuitLimit(){   return m_iAutoQuitLimit;   }
	int   GetAutoAction()   {   return m_iAutoAction;      }
	int   GetAutoActionLimit(){ return m_iAutoActionLimit; }

	void  SetAutoQuit(bool b);
	void  SetAutoQuitLimit(int i);
	void  SetAutoAction(int i);
	void  SetAutoActionLimit(int i);


	string& GetAutoSay()    {   return m_auto_say;         }
	bool  IsAutoSay()       {   return m_bIsAutoSay;     }
	int   GetAutoSayTimer() {   return m_iAutoSayTimer;    }
	bool  IsShowMagicTime() {   return m_bShowMagicTime;   }
	bool  IsRecordMsg()     {   return m_bRecordMsg;       }
	bool  IsAutoReply()     {   return m_bAutoReply;       }
	string& GetReplyMsg()   {   return m_reply_msg;        }
	bool  IsAutoMogong()    {   return m_bAutoMogong;      }
	bool  IsPetPickup()     {   return m_bIsPetPickup;     }




	bool  IsAutoSteelProtect(); //自动金刚
	bool  IsAutoProtectSkin() ; //自动护身真气
	bool  IsAutoFire(); //自动烈火
	bool  IsAutoSuperAttack(); //自动强化攻杀
	bool  IsAutoWildCollide();//自动突斩
	bool  IsDirWild();//定向突斩
	bool  IsDoubleFire();//自动双烈火
	bool  IsDestroyShield();//持续破盾
	bool  IsDestroyShell();//持续破击
	bool  IsAutoProtectGhost();//持续幽灵盾
	bool  IsAutoProtectArmor();//持续神圣战甲术
	bool  IsAutoLionCall(); //自动狮子吼
	bool  IsSmartLionCall();//定向狮子吼
	bool  IsAutoMagicProtect();//持续魔法盾
	bool  IsAutoDodgeSkill();//持续风影盾
	bool  IsAutoDispute();//自动抗拒






	bool  IsAntiAttack()    {   return m_bIsAntiAttack;    } //自动反击
	bool  IsAutoSkill()     {   return m_bIsAutoSkill;     } //自动练功
	int   GetAutoSkillTime(){   return m_iAutoSkillTime;   }
	char  GetAutoSkillKey() {   return m_cAutoSkillKey;    }

	//内部用到的状态设置//////////////////////////////////////////////////////////////////
	bool  IsAutoWildRun()     { return m_bAutoWildRun;     } //被野蛮突斩打中，自动逃离
	int   GetAutoWildDelay()  { return m_iAutoWildDelay;   } //被野蛮突斩打中后延迟
	bool  IsNoDrawChar()      { return m_bNoDrawChar;      }

	void  SetShowBestItem(bool b){  m_bIsShowBestItem = b; }
	void  SetAutoMagicAttack(bool b){  m_bIsAutoMagicAttack = b; }
	void  SetRunAttack(bool b)      {  m_bIsRunAttack = b; }
	void  SetAbsoluteLock(bool b)   {  m_bIsAbsoluteLock = b; }
	void  SetNoDrawChar(bool b)     {  m_bNoDrawChar = b; }

	//基本页签部分设置//////////////////////////////////////////////////////////////////
	void  SetShowObjectName(bool b);
	void  SetShowPlayerName(bool b);
	void  SetShowArrow(bool b);
	void  SetShowSysInfo(bool b);

	void  SetQuickOutGame(bool b);
	void  SetShowNpcPriority(bool b);
	void  SetShowNewHandPrompt(bool b);
	void  SetShowHpChangeEffect(bool b);
	void  SetAcceptFrientAndTrade(bool b);
	//void  SetAutoFeed(bool b);
	void  SetAutoTeamVoice(bool b);
	void  SetCloseTeam(bool b);
	void  SetDurationPrompt(bool b);
	void  SetTeamColor(bool b);
	void  SetDismount(bool b);
	void  SetAutoShowActLog(bool b);


	void  SetAutoFindWay(bool b);
	void  SetIsGoPickup(bool b);
	void  SetAutoPickup(bool b);
	void  SetShowMagicTime(bool b);
	void  SetBury(bool b);
	void  SetShowExtendShortCut(bool b);
	void  SetMountain(bool b);
	void  SetStarObject(bool b);
	void  SetAutoMogong(bool b);
	void  SetAllowFly(bool b);
	void  SetRunNotStop(bool b);      //跑不停

	//战斗页签部分////////////////////////////////////////////////////////////////
	void  SetAutoSteelProtect(bool v);
	void  SetAutoProtectSkin(bool v);
	void  SetAutoFire(bool v);
	void  SetAutoSuperAttack(bool b);

	void  SetAutoWildCollide(bool v);
	void  SetDirWild(bool v);
	void  SetDoubleFire(bool v);
	void  SetDestroyShell(bool v);
	void  SetDestroyShield(bool v);
	void  SetAutoProtectGhost(bool v);
	void  SetAutoProtectArmor(bool v);

	void  SetAutoLionCall(bool v);
	void  SetSmartLionCall(bool v);
	void  SetAutoMagicProtect(bool v);
	void  SetAutoDodgeSkill(bool v);
	void  SetAutoDispute(bool v);

	void  SetAntiAttack(bool v);
	void  SetAutoSkill(bool v);
	void  SetAutoSkillKey(char key);
	void  SetAutoSkillTime(int iTime);

	void  SetAutoKillShow(bool v);
	bool  IsAutoKillShow()	{ return m_bAutoKillShow; }

	//怪物页签
	void  SetBossPrompt(bool b);
	void  SetObjectPrompt(bool b);
	void  SetPkAllAttack(bool b);
	void  SetBlackFilter(bool b);
	void  SetPKAlarm(bool b);

	//聊天
	void  SetAutoSay(bool b);
	void  SetAutoSayMsg(const char* str);
	void  SetAutoReply(bool b);
	void  SetAutoReplyMsg(const char* str);
	void  SetAutoSayTimer(int i);
	void  SetRecordMsg(bool b);
	void  SetMessageAwoke(bool b);
	void  SetKeepOutInfo(bool b);

	bool  IsUsePTSuperHP()   {   return  m_bUsePTSuperHP; }
	bool  IsUsePTSuperMP()   {   return  m_bUsePTSuperMP; }
	void  SetUsePTSuperHP(bool b);
	void  SetUsePTSuperMP(bool b);
	
	bool  IsRejectBadItemWhenPickBestItem() const { return m_bRejectBadItemWhenPickBestItem; }
	void  SetRejectBadItemWhenPickBestItem(bool val);

	//特殊物品颜色//////////////////////////////////////////////////////////////////////
	DWORD GetBestItemColor(){   return m_dwBestItemColor; }
	DWORD GetExpensiveItemColor(){  return m_dwExpensiveItemColor; }
	DWORD GetPickItemColor(){   return m_dwPickItemColor;  }
	DWORD GetShowItemColor(){   return m_dwShowItemColor;  }

	void  SetBestItemColor(DWORD dwColor);
	void  SetExpensiveItemColor(DWORD dwColor);
	void  SetPickItemColor(DWORD dwColor);
	void  SetShowItemColor(DWORD dwColor);

	//时间//////////////////////////////////////////////////////////////////////////////
	int   GetDurationPromptTime(){   return m_iDurationPromptTime; }
	int   GetBossPromptTime(){   return m_iBossPromptTime; }
	int   GetObjectPromptTime(){   return m_iObjectPromptTime; }

	void  LoadConfig();

	string  GetReadme(int i);
	int     GetReadmeCount()  const {  return (int)m_readme.size(); }

	//快捷键///////////////////////////////////////////////////////////////////////////
	bool  SaveShortCutConfig();
	string GetShortCutKeyExStr(DWORD dwKey);

	void     AddPrivateMsg(const char* strMsg,int iLen,bool bNeedClearWords = true);
	void     ClearPrivateMsg();

	const char* GetIDPath(){ return m_id.c_str(); }
	const char* GetConfigPath(){return m_strPath.c_str();}
	const char* GetConfigDir(){return m_strSelfConfigDir.c_str();}

	void     SetQuickItem(int i,CQuickItem& Good);
	void     LoadConfigBeforeEnter();//进入游戏之间要读入的配置

	bool     LoadShortcutConfig();

	bool LoadFireLianHuaHelpCfg();
	string GetFireLianHuaHelpByName(const string& name);

	bool LoadQiShuHelpCfg();
	string GetQiShuHelpByName(const string& name);

private: 
	void     SetConfigPath();
	bool     LoadOtherConfig();
	bool     LoadReadmeConfig();
	bool     LoadMessageConfig();
	bool     LoadQuickItemConfig();
	bool	 LoadHelpCfg(const char* path, map<std::string, std::string>& mapHelps);
	string	 GetHelpByName(map<std::string, std::string>& mapHelps, const string& name);
	//---------------------------------------------------------------------------------
	char*    ReadConfigString(const char* strKey,const char* strDefault="");
	int      ReadConfigInt(const char * strKey,int iDefault=0);
	int      ReadConfigTime(const char* strKey,int iDefault=0);
	bool     ReadConfigBoolean(const char* strKey,bool bDefault=false);
	DWORD    ReadConfigColor(const char* strKey,DWORD dwDefaultColor = 0xFFFFFFFF);
	int      ReadConfigSection(const char* strSection,vector<string>& strArray);

	void     WriteConfigBoolean(const char* strKey,bool b);
	void     WriteConfigString(const char* strKey,const char* str);
	void     WriteConfigInt(const char* strKey,int i);
	void     WriteConfigColor(const char* strKey,DWORD dwColor);

	string   m_strPath;
	string   m_strSelfConfigDir;
	string   m_strKeyPath;
	string   m_strRecordPath;

	bool	 m_bKeepOutInfo;
	bool	 m_bMessageAwoke;
	bool     m_bIsAutoFindWay;
	bool     m_bIsAbsoluteLock;
	bool     m_bIsMountain;
	bool     m_bIsBury;
	bool     m_bIsGoPickup;
	bool     m_bIsAutoPickup;
	bool     m_bIsStarObject;
	bool     m_bIsShowObjectName;
	bool     m_bIsShowPlayerName;
	bool     m_bIsTeamColor;
	bool     m_bIsAttackColor;
	bool     m_bIsShowArrow;
	bool     m_bIsDurationPrompt;
	bool     m_bIsBossPrompt;
	bool     m_bIsObjectPrompt;
	bool     m_bIsCloseTeam;
	bool     m_bIsDismount;
	bool     m_bIsBlackFilter;
	bool     m_bIsShowBestItem;
	bool     m_bIsAutoMagicAttack;
	bool     m_bIsRunAttack;
	bool     m_bIsAntiAttack;
	bool     m_bIsAutoSkill;
	char     m_cAutoSkillKey;
	bool     m_bIsPKAlarm;
	bool     m_bIsQuickOutGame;
	bool     m_bShowNpcPriority;
	bool	 m_bShowNewHandPrompt;
	bool	 m_bShowHpChangeEffect;
	bool	 m_bAcceptFrientAndTrade;
	bool	 m_bPkAllAttack;
	bool     m_bIsAllowFly;
	bool	 m_bShowSysInfo;
	bool     m_bShowExtendShortCut;

	//道士自动治疗的功能，因为一直没有开，不需要了
	int		 m_nHuiChengShiPosition;
	bool     m_bRunNotStop;
	bool     m_bIsPetPickup;
	bool     m_bNoDrawChar; //不绘制人物

	//自动小退
	bool     m_bAutoQuit;
	int      m_iAutoQuitLimit;
	int      m_iAutoAction;
	int      m_iAutoActionLimit;
	string   m_auto_say;
	bool     m_bIsAutoSay;
	int      m_iAutoSayTimer;
	bool     m_bShowMagicTime;
	bool     m_bRecordMsg;
	bool     m_bAutoReply;
	string   m_reply_msg;
	bool     m_bAutoMogong;

	//战士
	bool     m_bAutoSteelProtect;
	bool     m_bAutoProtectSkin;
	bool     m_bAutoFire;
	bool     m_bAutoSuperAttack;//是否自动使用强化攻杀
	int      m_iAutoWildDelay;
	bool     m_bAutoWildCollide;
	bool     m_bDirWild;
	bool     m_bAutoWildRun;
	bool     m_bDoubleFire;
	bool     m_bDestroyShield;
	bool     m_bDestroyShell;
	//道士
	bool     m_bAutoProtectGhost;
	bool     m_bAutoProtectArmor;
	bool     m_bAutoLionCall;
	bool     m_bSmartLionCall;
	//法师
	bool     m_bAutoMagicProtect;
	bool     m_bAutoDodgeSkill;
	bool     m_bAutoDispute;

	//特殊物品颜色
	DWORD    m_dwBestItemColor;
	DWORD    m_dwExpensiveItemColor;
	DWORD    m_dwPickItemColor;
	DWORD    m_dwShowItemColor;

	//时间
	int      m_iDurationPromptTime;
	int      m_iBossPromptTime;
	int      m_iObjectPromptTime;
	int      m_iAutoSkillTime;

	bool     m_bUsePTSuperHP;
	bool     m_bUsePTSuperMP;

	//bool     m_bAutoFeed;
	bool     m_bAutoTeamVoice;
	bool     m_bRejectBadItemWhenPickBestItem;
	//复杂的文件，通过此类做代理
	vector<string>   m_readme;

	string   m_id;
	VString  m_files;

	bool     m_bAutoShowActLog;

	bool	 m_bAutoKillShow;//自动打怪

	//真火炼化帮助说明
	map<std::string, std::string> m_kFireLianHuaHelps;

	//骑术帮助说明
	map<std::string, std::string> m_kQiShuHelps;
};

extern CAIConfigMgr g_AICfgMgr;