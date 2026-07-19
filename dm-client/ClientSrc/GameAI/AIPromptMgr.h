#pragma once

#include "Global/Global.h"
#include "Global/StringUtil.h"
#include "GameData/SimpleCharacter.h"
#include <time.h>

typedef list<DWORD> IDList;

class CAIPromptMgr
{
public:
	struct CBossPrompt
	{
		CBossPrompt()
		{
			dwBossID = 0;
			tBossAppearTime = 0;
			tBossDeadTime = 0;
			tBossCorpusTime = 0;
		}
		DWORD          dwBossID;
		string         szBossName;
		string         szMapName;
		__time32_t         tBossAppearTime;
		__time32_t         tBossDeadTime;
		__time32_t         tBossCorpusTime;
	};

	CAIPromptMgr(void);
	~CAIPromptMgr(void);

	string& get_str(int i);
	void  set_str(const char* str);
	string& GetMerStr();
	void  SetMerStr(const char* str);

	void  DoLoop();
	void  FormatBossReminder(VString& vecBR,bool bScreen = true);
	void  DrawPrompt();
	void  ExtraDraw();

	void  DoCharacterAppear(CSimpleCharacter* pChar);
	void  DoCharacterDisappear(DWORD id);
	void  DoCharacterDead(DWORD id);
	void  DoCharacterCorpus(CSimpleCharacter* pChar);
	void  DoCharacterBone(CSimpleCharacter* pChar);
	void  DoCharacterToPet(DWORD id);
	void  SetAttackerID(DWORD id);

	//BOSSÃ·–—∂‡“≥œ‘ æ
	int   GetBossTotalPages();
	void  NextBossPage();
	void  PriorBossPage();

	void Clear();
	void  ReadPKFile();
	void  ReadBossDeathFile();
	IDList& GetPromptList(){ return m_listBoss; }
private:
	CSimpleCharacter* GetNearestChar();
	void  CheckDura();
	void  DisplayChar(CSimpleCharacter* pChar);
	void  NotifyTroopMembers(CSimpleCharacter* pChar);
	void  WriteToLog(CSimpleCharacter* pChar,BOOL bAppear = TRUE);
	
	void  WriteBossDeathFile();	
	void  WritePKFile();

	VString        m_vecPrompt;
	string         m_strStr;
	string         m_strBossDeathPath;
	string         m_strMerStr;
	string         m_strPKPath;

	typedef list<CBossPrompt*> BossPromptList;

	IDList         m_listBoss;
	BossPromptList m_listBossReminder;
	ListString     m_listPKer;

	DWORD          m_dwLastCheckTime;
	DWORD          m_dwLastBossTime;
	DWORD          m_dwAttackerID;
	int            m_iBossReminderPage;
	int            m_iBossRowCount;
	DWORD          m_dwLastMSTime;
};

extern CAIPromptMgr g_AIPromptMgr;