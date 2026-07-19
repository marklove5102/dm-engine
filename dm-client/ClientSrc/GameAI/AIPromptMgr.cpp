#include "AIPromptMgr.h"

#include "BaseClass/Control/Control.h"
#include "AIConfigMgr.h"
#include "AIBossCfgMgr.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include "GameData/TalkMgr.h"
#include "GameData/OtherData.h"
#include "GameControl/GameControl.h"
#include "GameMap/GameMap.h"
#include "Global/Timer.h"
#include <time.h>

CAIPromptMgr g_AIPromptMgr;

CAIPromptMgr::CAIPromptMgr(void)
{
	m_dwLastBossTime = 0;
	m_dwLastCheckTime = 0;
	m_dwAttackerID = 0;
	m_iBossReminderPage = 0;
	m_iBossRowCount = 10;
	m_dwLastMSTime = 0;
}

CAIPromptMgr::~CAIPromptMgr(void)
{
	m_vecPrompt.clear();
	m_listBoss.clear();
	m_listPKer.clear();

	while(!m_listBossReminder.empty())
	{
		CBossPrompt* pBR = m_listBossReminder.front();
		SAFE_DELETE(pBR);
		m_listBossReminder.pop_front();
	}
}

void CAIPromptMgr::Clear()
{
	m_iBossReminderPage = 0;
	m_iBossRowCount = 10;
	m_listBoss.clear();
	m_vecPrompt.clear();

	m_dwLastMSTime = 0;
}

string& CAIPromptMgr::GetMerStr()
{
	return m_strMerStr;
}

void CAIPromptMgr::SetMerStr(const char* str)
{
	m_strMerStr.assign(str);
}

string& CAIPromptMgr::get_str(int i)
{
	if(!m_strStr.empty())
	{
		if(i<=0)
			return m_strStr;
		else
			return m_vecPrompt[i-1];
	}
	else
	{
		return m_vecPrompt[i];
	}
}

void CAIPromptMgr::set_str(const char* str)
{
	if(str!=NULL)
		m_strStr.assign(str);
	else
		m_strStr.clear();
}

void CAIPromptMgr::DoLoop()
{
	DWORD dwCount = GetTickCount();

	if(g_AICfgMgr.IsDurationPrompt())
	{
		if(dwCount > m_dwLastCheckTime+g_AICfgMgr.GetDurationPromptTime())
		{
			CheckDura();
			m_dwLastCheckTime = dwCount;
		}
	}

	if(g_AICfgMgr.IsBossPrompt())
	{
		if(dwCount > m_dwLastBossTime+g_AICfgMgr.GetBossPromptTime())
		{
			CSimpleCharacter* pChar = GetNearestChar();
			if(pChar)
			{
				DisplayChar(pChar);
			}
			m_dwLastBossTime = dwCount;
		}
	}
}

void CAIPromptMgr::CheckDura()
{
	m_vecPrompt.clear();

	char temp[512] = {0};

	int iSize = SELF.EquipGood().Size();
	for(int i = 0;i < iSize;i++)
	{
		if (i == ITEM_POS_WING || i == ITEM_POS_WENPEI)
		{
			continue;
		}
		CGood& tGood = SELF.GetEquipGood(i);

		if(tGood.GetID() >0 && ((tGood.GetDura()+500) / 1000)<2)
		{
			if(tGood.GetStdMode() != 25 && tGood.GetStdMode() != 34	&&
				tGood.GetStdMode() != 59 && tGood.GetStdMode() != 60 &&
				tGood.GetStdMode() != 61 && tGood.GetStdMode() != 13)
			{
				sprintf(temp,"您的%s持久太低了",tGood.GetName());
				if (i == ITEM_POS_WEAPON)
				{
					strcat(temp,",请前往铁匠处修理");
				}
				else if (i == ITEM_POS_NECKLACE || i == ITEM_POS_RIGHT_BANGLE || i == ITEM_POS_LEFT_BANGLE || i == ITEM_POS_RIGHT_RING || i == ITEM_POS_LEFT_RING)
				{
					strcat(temp,",请前往首饰商人处修理");
				}
				else if (i == ITEM_POS_CLOTH || i == ITEM_POS_HELMET || i == ITEM_POS_CLOTH_BOOT || i == ITEM_POS_BELT)
				{
					strcat(temp,",请前往服装店处修理");
				}

				m_vecPrompt.push_back(temp);
			}
		}
	}
}

CSimpleCharacter* CAIPromptMgr::GetNearestChar()
{
	//Boss
	if(m_listBoss.size()>0)
	{
		unsigned long tempid = m_listBoss.front();
		m_listBoss.pop_front();

		CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(tempid);
		if(NULL == pChar)
			return NULL;

		if(pChar->IsPet() || pChar->IsDead())
			return NULL;

		m_listBoss.push_back(tempid);

		return pChar;
	}
	return NULL;
}

void CAIPromptMgr::DisplayChar(CSimpleCharacter* pChar)
{
	if(!pChar)
		return;

	int iSelfX,iSelfY;
	int iX = 0,iY = 0;

	SELF.GetXY(iSelfX,iSelfY);
	pChar->GetXY(iX,iY);

	DWORD dwSelfID = SELF.GetID();
	const char* szDir = GetToDir(iSelfX,iSelfY,iX,iY);

	if (g_BossCfgMgr.IsBoss(pChar->GetName()))
	{	
		char temp[256]={0};
		sprintf(temp,"发现怪物：%s  方向： %s  坐标：(%d,%d)",pChar->GetName(),szDir,iX,iY);

		g_TalkMgr.PushShoutTalk(temp,strlen(temp),0xFCFF,dwSelfID,0xFF,FONT_DEFAULT,FONTSIZE_DEFAULT,TALKTYPE_SYSTEM);
	}
}

void CAIPromptMgr::NotifyTroopMembers(CSimpleCharacter* pChar)
{
	if(!pChar)
		return;

	int iX = 0,iY = 0;
	pChar->GetXY(iX,iY);

	string str;
	str.assign("!!");
	char temp[1024]={0};
	//sprintf(temp,"发现[%s]  地图：%s  坐标：(%d,%d)",pChar->GetName(),g_pGameData->GetMapName(),iX,iY);
	sprintf(temp,"%s 出现在 %s(%d,%d)",pChar->GetName(),g_pGameMap->GetMapTitle(),iX,iY);
	str.append(temp);

	g_pGameControl->SEND_Message_Send(str.c_str(),str.size());
}

void CAIPromptMgr::WriteToLog(CSimpleCharacter* pChar,BOOL bAppear)
{
	if(!pChar)
		return;

	CBossPrompt* pBossReminder = NULL;
	BossPromptList::iterator it;

	for(it=m_listBossReminder.begin();it!=m_listBossReminder.end();it++)
	{
		CBossPrompt* pBR = *it;
		if(pBR == NULL) continue;

		if(pBR->dwBossID == pChar->GetID() || (pBR->szBossName.compare(pChar->GetName()) == 0 &&
			pBR->szMapName.compare(g_pGameMap->GetMapTitle()) == 0))
		{
			pBossReminder = pBR;
			break;
		}
	}

	if(NULL == pBossReminder)
	{
		pBossReminder = new CBossPrompt();
		m_listBossReminder.push_back(pBossReminder);
		pBossReminder->dwBossID = pChar->GetID();
		pBossReminder->szBossName.assign(pChar->GetName());
		pBossReminder->szMapName.assign(g_pGameMap->GetMapTitle());
	}

	if(bAppear)
	{
		if(!pChar->IsDead())
			_time32(&pBossReminder->tBossAppearTime);
		else
			_time32(&pBossReminder->tBossCorpusTime);
	}
	else
	{
		_time32(&pBossReminder->tBossDeadTime);
	}
	WriteBossDeathFile();
}

int CAIPromptMgr::GetBossTotalPages()
{
	int iTotalPage = (m_listBossReminder.size() + m_iBossRowCount -1) / m_iBossRowCount;
	if(iTotalPage == 0) iTotalPage = 1;
	return iTotalPage;
}

void CAIPromptMgr::NextBossPage()
{
	int iTotalPages = GetBossTotalPages();
	m_iBossReminderPage++;
	m_iBossReminderPage %= iTotalPages;
}

void CAIPromptMgr::PriorBossPage()
{
	int iTotalPages = GetBossTotalPages();
	if(m_iBossReminderPage > 0)
		m_iBossReminderPage--;
	else
		m_iBossReminderPage = iTotalPages - 1;

	m_iBossReminderPage %= iTotalPages;
}

void CAIPromptMgr::FormatBossReminder(VString& vecBR,bool bScreen)
{
	vecBR.clear();
	vecBR.push_back("   Boss名称        精确死亡时间          最新出现时间              发现尸体时间              位置");

	int iTotalPages = GetBossTotalPages();
	m_iBossReminderPage %= iTotalPages;

	int iRowCount = 0;
	BossPromptList::iterator it;
	for(it=m_listBossReminder.begin();it!=m_listBossReminder.end();it++)
	{
		iRowCount++;

		CBossPrompt* pBR = *it;
		if(NULL == pBR)
			continue;

		if(bScreen)
		{
			if(iRowCount <= m_iBossReminderPage * m_iBossRowCount)
				continue;

			if(iRowCount > (m_iBossReminderPage + 1) * m_iBossRowCount)
				break;
		}

		string str;
		char temp[128] = {0};
		sprintf(temp,"%s",pBR->szBossName.c_str());
		str.append(temp);
		for(int i=pBR->szBossName.size();i<14;i++)
			str.append(" ");

		if(pBR->tBossDeadTime>0)
		{
			if(bScreen)
			{
				sprintf(temp," %s",_ctime32(&pBR->tBossDeadTime));
				temp[strlen(temp)-1] = ' ';
			}
			else
			{
				tm * pTm = _localtime32(&pBR->tBossDeadTime);
				if (pTm)
				{
					strftime(temp,100,"|%Y/%m/%d %H:%M:%S      ",pTm);
				}
			}
		}
		else
		{
			if(bScreen)
				sprintf(temp,"                          ");
			else
				sprintf(temp,"|                         ");
		}
		str.append(temp);

		if(pBR->tBossAppearTime>0)
		{
			if(bScreen)
			{
				sprintf(temp," %s",_ctime32(&pBR->tBossAppearTime));
				temp[strlen(temp)-1] = ' ';
			}
			else
			{
				tm * pTm = _localtime32(&pBR->tBossAppearTime);
				if (pTm)
				{
					strftime(temp,100,"|%Y/%m/%d %H:%M:%S      ",pTm);
				}
			}
		}
		else
		{
			if(bScreen)
				sprintf(temp,"                          ");
			else
				sprintf(temp,"|                         ");
		}
		str.append(temp);

		if(pBR->tBossCorpusTime>0)
		{
			if(bScreen)
			{
				sprintf(temp," %s",_ctime32(&pBR->tBossCorpusTime));
				temp[strlen(temp)-1] = ' ';
			}
			else
			{
				tm *pTm = _localtime32(&pBR->tBossCorpusTime);
				if (pTm)
				{
					strftime(temp,100,"|%Y/%m/%d %H:%M:%S      ",pTm);
				}
			}
		}
		else
		{
			if(bScreen)
				sprintf(temp,"                          ");
			else
				sprintf(temp,"|                         ");
		}
		str.append(temp);

		if(bScreen)
			sprintf(temp," %s",pBR->szMapName.c_str());
		else
			sprintf(temp,"|%s",pBR->szMapName.c_str());
		str.append(temp);

		vecBR.push_back(str);
	}
}

void CAIPromptMgr::WriteBossDeathFile()
{
	char temp[1024]={0};
	sprintf(temp,"%s\\config\\%s\\BossDeathTime.txt",GetGameDataDir(),g_AICfgMgr.GetIDPath());
	m_strBossDeathPath.assign(temp);

	FILE* fp = fopen(m_strBossDeathPath.c_str(),"wt");
	if(!fp)
		return;

	vector<string> vecBR;

	FormatBossReminder(vecBR,false);
	for(int i=0;i<vecBR.size();i++)
		fprintf(fp,"%s\n",vecBR[i].c_str());

	fclose(fp);
}

void CAIPromptMgr::ReadBossDeathFile()
{
	char temp[1024]={0};
	sprintf(temp,"%s\\config\\%s\\BossDeathTime.txt",GetGameDataDir(),g_AICfgMgr.GetIDPath());
	m_strBossDeathPath.assign(temp);

	FILE* fp = fopen(m_strBossDeathPath.c_str(),"rt");
	if(!fp)
		return;

	while(!m_listBossReminder.empty())
	{
		CBossPrompt* pBR = m_listBossReminder.front();
		SAFE_DELETE(pBR);
		m_listBossReminder.pop_front();
	}

	m_listBossReminder.clear();

	char szLine[1024]={0};
	if(NULL == fgets(szLine,1024,fp))
		return;

	while(!feof(fp))
	{
		ZeroMemory(szLine,1000);
		if(NULL == fgets(szLine,1000,fp))
			break;

		if(strlen(szLine) == 0)
			continue;

		char* szBossName = strtok(szLine,"|\n");
		if(NULL == szBossName)
			continue;

		char* szBossDeadTime = strtok(NULL,"|\n");
		if(NULL == szBossDeadTime)
			continue;

		char* szBossAppearTime = strtok(NULL,"|\n");
		if(NULL == szBossAppearTime)
			continue;

		char* szBossCorpusTime = strtok(NULL,"|\n");
		if(NULL == szBossCorpusTime)
			continue;

		char* szMapName = strtok(NULL," |\n");
		if(NULL == szMapName)
			continue;

		szBossName = strtok(szBossName," ");
		if(NULL == szBossName)
			continue;

		if(!g_BossCfgMgr.IsBoss(szBossName))
			continue;

		CBossPrompt* pBR = new CBossPrompt();
		pBR->dwBossID = 0;
		pBR->szBossName.assign(szBossName);
		pBR->tBossDeadTime = g_Timer.StrToTime(szBossDeadTime);
		pBR->tBossAppearTime = g_Timer.StrToTime(szBossAppearTime);
		pBR->tBossCorpusTime = g_Timer.StrToTime(szBossCorpusTime);
		pBR->szMapName.assign(szMapName);

		m_listBossReminder.push_back(pBR);
	}
}

void CAIPromptMgr::DoCharacterAppear(CSimpleCharacter* pChar)
{
	if(pChar == NULL)
		return;

	if(pChar->IsMonster())
	{
		//组合技能，神之召唤出来的BOSS
		if(pChar->GetReserveData(pubGradualAppear))
			return;

		if(!pChar->IsDead() && !pChar->IsPet() && g_AICfgMgr.IsBossPrompt() &&  g_BossCfgMgr.IsBoss(pChar->GetName()))
		{
			DisplayChar(pChar);
			m_dwLastBossTime = GetTickCount();
			m_listBoss.push_back(pChar->GetID());
			WriteToLog(pChar);

			//组队时通知队友
			if(g_pGameData->GetTroopMembers() > 0)
			{
				NotifyTroopMembers(pChar);
			}
		}
		else if(pChar->IsDead() && g_BossCfgMgr.IsBoss(pChar->GetName()))
		{
			WriteToLog(pChar);
		}
	}
	else if(pChar->IsHuman())
	{
		//确认这个和自己的关系
		//先看看是不是队友关系
		if(g_pGameData->GetTroopMembers()>0)
		{
			ListString& _members = g_pGameData->GetTroopMemberList();
			ListString::iterator it;
			for(it=_members.begin();it!=_members.end();it++)
			{
				string& str = *it;
				if(str.compare(pChar->GetName()) == 0)
				{
					pChar->AddRelationType(RT_GROUP);
					break;
				}
			}
		}
		//再看看有没有其它关系
		for(int i = 0;i< g_OtherData.GetRelationVector().size();i++)
		{
			DWORD dwMyRelationType = g_OtherData.GetRelationVector()[i].iRelType;
			if(stricmp(g_OtherData.GetRelationVector()[i].strName.c_str(),pChar->GetName()) == 0)
			{
				if( (dwMyRelationType & RT_WIFE) != 0 )//夫妻
					pChar->AddRelationType(RT_WIFE);
				else if( (dwMyRelationType & RT_HUSBAND) != 0 )
					pChar->AddRelationType(RT_HUSBAND);

				if((dwMyRelationType & RT_PRENTICE) != 0)//徒弟
					pChar->AddRelationType(RT_PRENTICE);
				else if((dwMyRelationType & RT_MASTER) != 0)//师傅
					pChar->AddRelationType(RT_MASTER);

				if((dwMyRelationType & RT_FRIEND) != 0)//好友
					pChar->AddRelationType(RT_FRIEND);

				if((dwMyRelationType & RT_JIEYI) != 0)//结义好友
					pChar->AddRelationType(RT_JIEYI);		

				break;
			}
		}
		//再看看是不是同一个行会
		if(strlen(pChar->GetGuildName()) > 0 && strcmp( SELF.GetTitle(), pChar->GetGuildName()) == 0)
		{
			pChar->AddRelationType(RT_GUILD);
		}
	}
	//else if (pChar->IsNpc())
	//{
	//	SAction& saction = pChar->GetAction();
	//	saction.SetNpcRandStand(pChar->GetRaceNo());
	//}
}

void CAIPromptMgr::DoCharacterDisappear(DWORD id)
{
	IDList::iterator it = m_listBoss.begin();
	while(it!=m_listBoss.end())
	{
		DWORD tempid = *it;
		if(tempid == id)
		{
			it = m_listBoss.erase(it);
			continue;
		}
		it++;
	}
}

void CAIPromptMgr::DoCharacterDead(DWORD id)
{
	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(id);
	if(pChar == NULL)
		return;

	IDList::iterator it = m_listBoss.begin();

	while(it!=m_listBoss.end())
	{
		DWORD tempid = *it;
		if(tempid == id)
		{
			it = m_listBoss.erase(it);
			WriteToLog(pChar,FALSE);
			continue;
		}
		it++;
	}

	//队友死亡提醒
	if(pChar->IsHuman() && (pChar->GetRelationType() & RT_GROUP) != 0)
	{
		int iX = 0,iY = 0;
		int iSelfX,iSelfY = 0;
		pChar->GetXY(iX,iY);
		SELF.GetXY(iSelfX,iSelfY);
		
		DWORD dwSelfID = SELF.GetID();
		const char* szDir = GetToDir(iSelfX,iSelfY,iX,iY);

		char temp[256]={0};
		sprintf(temp,"队友死亡：%s  方向： %s  坐标：(%d,%d)",pChar->GetName(),szDir,iX,iY);

		g_TalkMgr.PushShoutTalk(temp,strlen(temp),TALKCOLOR_RED,dwSelfID,0xFF,FONT_DEFAULT,FONTSIZE_DEFAULT,TALKTYPE_SYSTEM);
	}
}

void CAIPromptMgr::DoCharacterCorpus(CSimpleCharacter* pChar)
{
	if(pChar == NULL)
		return;

	if(pChar->IsMonster())
	{
		if(pChar->IsDead() && g_BossCfgMgr.IsBoss(pChar->GetName()))
		{
			WriteToLog(pChar);
		}
	}
}

void CAIPromptMgr::DoCharacterBone(CSimpleCharacter* pChar)
{
	if(pChar == NULL)
		return;

	if(pChar->IsMonster())
	{
		if(pChar->IsDead() && g_BossCfgMgr.IsBoss(pChar->GetName()))
		{
			WriteToLog(pChar);
		}
	}
}

void CAIPromptMgr::DoCharacterToPet(DWORD id)
{
	IDList::iterator it = m_listBoss.begin();
	while(it!=m_listBoss.end())
	{
		DWORD tempid = *it;
		if(tempid == id)
		{
			it = m_listBoss.erase(it);
			continue;
		}
		it++;
	}
}

void CAIPromptMgr::SetAttackerID(DWORD id)
{
	m_dwAttackerID = id;

	if(m_dwAttackerID == 0)
		return;

	if(g_AICfgMgr.IsPkAllAttack())
	{
		CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwAttackerID);

		//心魔不改变攻击模式，因为在服务器心魔是怪物
		if(pChar && !pChar->IsDead() && pChar->IsHuman()) 
		{
			//改变攻击模式
			if(SELF.GetReserveData(plyAttackType) != ATTACK_TYPE_ALL)
			{
				char strTemp[56]={0};
				sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_ALL);
				g_pGameControl->SEND_Guild_Ally(strTemp);
			}
		}
	}

	if(g_AICfgMgr.IsPKAlarm())
	{
		CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(m_dwAttackerID);

		//心魔不改变攻击模式，因为在服务器心魔是怪物
		if(pChar && !pChar->IsDead() && pChar->IsHuman()) 
		{
			string szPKer = pChar->GetName();
			ListString::iterator it;
			for(it = m_listPKer.begin();it!=m_listPKer.end();it++)
			{
				if(szPKer.compare(*it) == 0) //已经在列表中，需要先删除之
				{
					m_listPKer.erase(it);
					break;
				}
			}

			if(m_listPKer.size() > 15)
				m_listPKer.pop_back();

			m_listPKer.push_front(szPKer);

			WritePKFile();
		}
	}
	if(g_AICfgMgr.IsAntiAttack())
	{
		CSimpleCharacter* pChar= g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyAttackLockID));

		if(pChar == NULL || pChar->IsDead())
		{
			pChar = g_pGameData->FindSimpleCharacter(m_dwAttackerID);
			if(pChar && !pChar->IsDead())
			{
				SELF.SetReserveData(plyAttackLockID,m_dwAttackerID);
			}
		}
	}
}

void CAIPromptMgr::ReadPKFile()
{
	char path[1024]={0};
	sprintf(path,"%s\\config\\%s\\pk.txt",GetGameDataDir(),g_AICfgMgr.GetIDPath());
	m_strPKPath.assign(path);

	FILE* fp = fopen(m_strPKPath.c_str(),"rt");
	if(!fp)
		return;

	m_listPKer.clear();

	char szBuffer[1024]={0};
	while(!feof(fp))
	{
		memset(szBuffer,0,1024);
		if(NULL == fgets(szBuffer,1000,fp))
			break;

		char* szLine = strtok(szBuffer,"\r\n");
		if(NULL == szLine)
			continue;
		if(strlen(szLine) == 0)
			continue;

		m_listPKer.push_back(szLine);
	}

	fclose(fp);
}

//写PK文件
void CAIPromptMgr::WritePKFile()
{
	char path[1024]={0};
	sprintf(path,"%s\\config\\%s\\pk.txt",GetGameDataDir(),g_AICfgMgr.GetIDPath());
	m_strPKPath.assign(path);

	FILE* fp = fopen(m_strPKPath.c_str(),"wt");
	if(!fp)
		return;

	ListString::iterator it;
	for(it = m_listPKer.begin();it!=m_listPKer.end();it++)
	{
		string str = *it;
		fprintf(fp,"%s\n",str.c_str());
	}
	fclose(fp);
}

void CAIPromptMgr::DrawPrompt()
{
	//重要提醒
	int iReminderX = g_pGfx->GetWidth();
	int iReminderY = 180;

	int iFontSize = FONTSIZE_DEFAULT;

	if(!m_strStr.empty())
	{
		g_pFont->DrawText(iReminderX- m_strStr.length()/2 * iFontSize - iFontSize / 2 ,iReminderY,m_strStr.c_str(),0xFFFF8000,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iReminderY += iFontSize + 2;
	}

	int iReminderCount = m_vecPrompt.size();
	for(int i = 0; i< iReminderCount; i++)
	{
		if(!(m_vecPrompt[i].empty()))
		{
			g_pFont->DrawText(iReminderX- m_vecPrompt[i].length()/2 * iFontSize - iFontSize / 2 ,iReminderY,m_vecPrompt[i].c_str(),0xFFFF8000,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
			iReminderY += iFontSize + 2;
		}
	}
}

void CAIPromptMgr::ExtraDraw()
{
	if(!g_pInput->IsEnable())
		return;

	CControl* pCtrl = g_pControl->GetFocusCtrl();

	char str_buf[1024] = {0};
	int _iFontSize = FONTSIZE_DEFAULT + 2;

	if(g_pInput->IsKeyDown(VK_DELETE) && !g_pInput->IsCtrl() && !g_pInput->IsAlt()
		&& !g_pControl->IsPanelFakeFocus() && (pCtrl == NULL || !pCtrl->IsNeedKeyInput()))
	{
		if(g_pGfx->GetHeight() > 600)
			m_iBossRowCount = 30;
		else
			m_iBossRowCount = 20;

		int _iBossReminderX = 25;
		int _iBossReminderY = 60;

		VString vecBR;
		FormatBossReminder(vecBR);

		//当前页和总页数
		sprintf(str_buf,"%3d/%3d",m_iBossReminderPage,GetBossTotalPages());
		g_pFont->DrawText(600,45,str_buf,0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);

		//画框
		DWORD  dwTableColor = 0xFFFFFFFF;
		int _iBossWidth = 700;
		int _iBossHeight = _iFontSize* vecBR.size();
		int _iBossX = _iBossReminderX-5;
		int _iBossY = _iBossReminderY-1;

		g_pGfx->DrawFillRect(_iBossX,_iBossY,_iBossWidth+1,_iBossHeight+1,0x600000FF);
		g_pGfx->DrawLine(_iBossY,_iBossX,_iBossX+_iBossWidth,dwTableColor); //水平线
		g_pGfx->DrawLine(_iBossX,_iBossY,_iBossY+_iBossHeight,dwTableColor,FALSE); //垂直线
		g_pGfx->DrawLine(_iBossX+_iBossWidth,_iBossY,_iBossY+_iBossHeight,dwTableColor,FALSE);
		g_pGfx->DrawLine(_iBossX+92,_iBossY,_iBossY+_iBossHeight,dwTableColor,FALSE);
		g_pGfx->DrawLine(_iBossX+249,_iBossY,_iBossY+_iBossHeight,dwTableColor,FALSE);
		g_pGfx->DrawLine(_iBossX+403,_iBossY,_iBossY+_iBossHeight,dwTableColor,FALSE);
		g_pGfx->DrawLine(_iBossX+560,_iBossY,_iBossY+_iBossHeight,dwTableColor,FALSE);

		int  iBR = 0;
		for(iBR = 0;iBR<vecBR.size();iBR++)
		{
			_iBossY+=_iFontSize;
			g_pGfx->DrawLine(_iBossY,_iBossX,_iBossX+_iBossWidth,dwTableColor); //水平线
		}

		_iBossX = _iBossReminderX;
		_iBossY = _iBossReminderY;
		for(iBR = 0;iBR<vecBR.size();iBR++)
		{
			g_pFont->DrawText(_iBossX,_iBossY,vecBR[iBR].c_str(),0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
			_iBossY+=_iFontSize;
		}
	}

	if(g_pInput->IsKeyDown(VK_PRIOR))
	{
		int _iAttackerX = g_pGfx->GetWidth() - 100;
		int _iAttackerY = 185;

		ListString::iterator it;

		string str = "PK者：";
		g_pFont->DrawText(_iAttackerX,_iAttackerY,str.c_str(),0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		_iAttackerY+=16;

		for(it = m_listPKer.begin();it != m_listPKer.end(); it++)
		{
			str = *it;
			g_pFont->DrawText(_iAttackerX,_iAttackerY,str.c_str(),0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
			_iAttackerY+=16;
		}
	}
}