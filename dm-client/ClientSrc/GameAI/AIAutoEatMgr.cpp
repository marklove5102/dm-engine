#include "AIAutoEatMgr.h"
#include "AIConfigMgr.h"
#include "GameData/GameData.h"
#include "GameData/TalkMgr.h"
#include "GameControl/GameControl.h"
#include "GameData/BoothData.h"
#include "AIGoodMgr.h"
#include "GameData/PetData.h"
#include "GameData/BoothData.h"

CAIAutoEatMgr g_AutoEatMgr;

static char* MEDICA_NAME[] = 
{
	"LittleWater",//金创药(小量)
	"MiddleWater",//金创药(中量)
	"BigWater",//金创药(大量)
	"SmartWater",//特级金创药
	"BetterWater",//强效金创药
	"SunWater",//太阳神水
	"SuperSunWater",//强效太阳神水
	"HealWater",//极效太阳神水
	"PTHealWater",//治疗药水

};

CAIAutoEatMgr::CAIAutoEatMgr(void)
{
	m_hp_nexttime = 0;
	m_mp_nexttime = 0;
	m_AutoUse_PackItemID = 0;
	m_AutoUse_itemID = 0;
	m_AutoUse_MpItemID = 0;

	m_dwAutoUnbindTime = 0;
	//m_dwLastAutoFeedTime = 0;
	m_dwLastUseSuperRed = 0;
	m_dwLastUseSuperBlue = 0;
}

CAIAutoEatMgr::~CAIAutoEatMgr(void)
{
}

char* CAIAutoEatMgr::ReadConfigString(const char * strKey,const char * strDefault)
{
	DWORD dwSize = 1024;
	static char szReturn[1024] = {0};
	ZeroMemory(szReturn,1024);
	GetPrivateProfileString("Config",strKey,strDefault,szReturn,dwSize,m_strPath.c_str());
	return szReturn;
}

void CAIAutoEatMgr::WriteConfigString(const char* strKey,const char* str)
{
	WritePrivateProfileString("Config",strKey,str,m_strPath.c_str());
}

bool CAIAutoEatMgr::LoadConfig()
{
	//清掉上次使用信息
	memset(m_GoodUseInf,0,sizeof(GoodInf_t)*MAX_PANEL_GOOD_NUM);

	//喝药辅助工具
	m_strPath = StringUtil::format("%s\\config\\%s\\Eat.ini",GetGameDataDir(),g_AICfgMgr.GetIDPath());

	m_hp_nexttime = 0;
	m_mp_nexttime = 0;

	//初始化药的状态
	m_woool_hpmp[0].Init("金创药(小量)","金创药（小）包","魔法药(小量)","魔法药（小）包");
	m_woool_hpmp[1].Init("金创药(中量)","金创药（中）包","魔法药(中量)","魔法药（中）包");
	m_woool_hpmp[2].Init("金创药(大量)","超级金创药","魔法药(大量)","超级魔法药");
	m_woool_hpmp[3].Init("特级金创药","特级金创药包","特级魔法药","特级魔法药包");
	m_woool_hpmp[4].Init("强效金创药","强效金创药包","强效魔法药","强效魔法药包");
	m_woool_hpmp[5].Init("太阳神水","太阳水药袋","太阳神水","太阳水药袋");
	m_woool_hpmp[6].Init("强效太阳神水","强效太阳水药袋","强效太阳神水","强效太阳水药袋");
	m_woool_hpmp[7].Init("极效太阳神水","极效太阳水药袋","极效太阳神水","极效太阳水药袋");

	m_woool_hpmp[8].Init("治疗神水","","治疗神水","");

	//喝药的情况
	for(int i = 0;i < MAX_MEDICA_NUMBER;i++)
	{
		WOOOLMEDICAEAT& eat = m_woool_hpmp[i]; 
		eat.Clear();
		eat.hp_bused = 1;
		eat.mp_bused = 1;
		if(i < 5)
		{
			eat.hp_delay = 3000;
			eat.mp_delay = 3000;
		}
		else
		{
			eat.hp_delay = 2000;
			eat.mp_delay = 2000;
		}

		char* strWater = ReadConfigString(MEDICA_NAME[i],"");
		if(strlen(strWater) > 0)
		{
			sscanf(strWater,"%d,%d,%d,%d,%d,%d",&eat.hp_limit,&eat.hp_delay,
				&eat.hp_bused,&eat.mp_limit,
				&eat.mp_delay,&eat.mp_bused);
		}
	}

	return true;
}

void CAIAutoEatMgr::WriteMedicaConfig(int i)
{
	WOOOLMEDICAEAT& eat = m_woool_hpmp[i];

	char temp_str[256] = {0};

	sprintf(temp_str,"%d,%d,%d,%d,%d,%d",eat.hp_limit,eat.hp_delay,
		eat.hp_bused,eat.mp_limit,
		eat.mp_delay,eat.mp_bused);

	WriteConfigString(MEDICA_NAME[i],temp_str);
}

bool CAIAutoEatMgr::IsUseHPMedica(int i)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return false;

	if(m_woool_hpmp[i].hp_bused)
		return true;

	return false;
}

bool CAIAutoEatMgr::IsUseMPMedica(int i)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return false;

	if(m_woool_hpmp[i].mp_bused)
		return true;

	return false;
}

WOOOLMEDICAEAT *CAIAutoEatMgr::GetHPMPMedicaInfo(int i)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return NULL;

	return &(m_woool_hpmp[i]);
}

void CAIAutoEatMgr::SetUseHPMedia(int i,bool b)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return;

	m_woool_hpmp[i].hp_bused = b?1:0;
	WriteMedicaConfig(i);
}

void CAIAutoEatMgr::SetUseMPMedia(int i,bool b)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return;

	m_woool_hpmp[i].mp_bused = b?1:0;
	WriteMedicaConfig(i);
}

void CAIAutoEatMgr::SetHPMediaLimit(int i,int hp)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return;

	m_woool_hpmp[i].hp_limit = hp;
	WriteMedicaConfig(i);
}

void CAIAutoEatMgr::SetHPMediaDelay(int i,int delay)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return;

	if(i< 5 && delay < 500) return;
	if(i >= 5 && delay < 50)  return;

	m_woool_hpmp[i].hp_delay = delay;
	WriteMedicaConfig(i);
}

void CAIAutoEatMgr::SetMPMediaLimit(int i,int mp)
{
	if(i < 0 || i >=  MAX_MEDICA_NUMBER)
		return;

	m_woool_hpmp[i].mp_limit = mp;
	WriteMedicaConfig(i);
}

void CAIAutoEatMgr::SetMPMediaDelay(int i,int delay)
{
	if(i< 5 && delay < 500) return;
	if(i >= 5 && delay < 50)  return;

	m_woool_hpmp[i].mp_delay = delay;
	WriteMedicaConfig(i);
}

int CAIAutoEatMgr::CountItemNumber(const char* name)
{
	if(strlen(name) == 0)
		return 0;

	int iCount  = 0;
	int iSize = SELF.PackageGood().Size();

	for(int ii = 0;ii < iSize;ii++)
	{
		CGood& temp = SELF.GetPackageGood(ii);
		if(temp.GetID() == 0)
			continue;

		if(stricmp(name,temp.GetName()) == 0)
			iCount++;
	}
	return iCount;
}

int CAIAutoEatMgr::FindPackageItem(const char* name)
{
	if(strlen(name) == 0)
		return -1;

	int iPos = SELF.PackageGood().ReverseFindGoodByName(name,MAX_PACKAGE_ELEMENT - 7);
	if(iPos < 0)
	{
		iPos = SELF.PackageGood().FindGoodByName(name,MAX_PACKAGE_ELEMENT - 6);
	}

	return iPos;
}

int CAIAutoEatMgr::CountBlankNumber()
{
	return SELF.PackageGood().GetBlankNumber();
}

BOOL CAIAutoEatMgr::AutoUnbindGoods(const char* name,DWORD& time)
{
	if(strlen(name) == 0)
		return FALSE;

	char temp[256];
	int iItem = FindPackageItem(name);
	if(iItem < 0)
	{
		sprintf(temp,"包裹里已经没有[%s]",name);
		g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
		return FALSE;
	}

	DWORD dwCount = GetTickCount();
	if(dwCount - time < 1000)
		return FALSE;

	int blank = CountBlankNumber();
	if(blank >= 5 && (m_AutoUse_PackItemID == 0 || dwCount - time > 3000))//> 3s就认为是异常,为了不至于导致一直无法解包的情况,这里应该清除m_AutoUse_PackItemID
	{
		DWORD dwID = SELF.GetPackageGood(iItem).GetID();
		if(g_pGameControl->SEND_Use_Object(iItem))
		{
			m_AutoUse_PackItemID = dwID;
			time = dwCount;
			return TRUE;                                                                                   
		}
	}
	else
	{
		if(blank < 5)
		{
			static DWORD dwLastPromt = 0;
			if(GetTickCount() - dwLastPromt > 10000)
			{
				g_TalkMgr.PushSysTalk("包裹里空位不足，无法解包！",TALKCOLOR_PINK);
				dwLastPromt = dwCount;
				return FALSE;
			}
		}
	}
	return FALSE;
}

// 加血的策略
void CAIAutoEatMgr::AddHP()					// 封装加红策略
{
	int hp = SELF.GetHP();
	int hp_max = SELF.GetHPMax();
	DWORD dwCount = GetTickCount();
	static DWORD dwLastPrompt = 0;
	char temp[256] = {0};

// 	//自己在托管模式下,而且不是队长,或者队长微操队员,或者离线托管的队员,如果群英包裹里有药品,先用群英包裹里的
// 	if (g_TrusteeshipData.IsTrusteeship() && (!g_TrusteeshipData.IsCaptain() || g_pSelf != &ORIGINALSELF))
// 	{
// 		TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 
// 		if (pSelf)
// 		{
// 			for(int i = MAX_MEDICA_NUMBER - 1; i >= 0; i--)
// 			{
// 				if(i < 5 && dwCount <= m_hp_nexttime) //还没有到允许的喝药时间,5以上的物品不受m_hp_nexttime限制
// 					break;
// 
// 				WOOOLMEDICAEAT& eat = m_woool_hpmp[i];
// 				WOOOLMEDICA& trusteeshipEatCfg = pSelf->eatConfig.medica[i];
// 
// 				if(trusteeshipEatCfg.hp_limit == 0 || trusteeshipEatCfg.hp_delay == 0 || eat.sHPName.empty())
// 					continue;
// 
// 				if(trusteeshipEatCfg.hp_bused && trusteeshipEatCfg.hp_limit < hp_max && hp < trusteeshipEatCfg.hp_limit)
// 				{
// 					int iItem = pSelf->package.FindGoodByName(eat.sHPName.c_str());
// 					bool bSelfPackage = false;//是否喝自己包裹里的,否则喝群英包裹里的
// 					if (iItem < 0)
// 					{
// 						iItem = FindPackageItem(eat.sHPName.c_str());
// 						bSelfPackage = true;
// 					}
// 
// 					//没有单个的,喝带持久的包
// 					if (iItem < 0)
// 					{
// 						iItem = pSelf->package.FindGoodByName(eat.sHPPack.c_str());
// 						bSelfPackage = false;
// 					}
// 
// 					if (iItem < 0)
// 					{
// 						iItem = FindPackageItem(eat.sHPPack.c_str());
// 						bSelfPackage = true;
// 					}
// 
// 					if(iItem >= 0)
// 					{
// 						if (dwCount > trusteeshipEatCfg.hp_delay + eat.dwLastHPTime)
// 						{
// 							eat.dwLastHPTime = dwCount; //每一种药品都要计算时间
// 							DWORD dwID = 0;
// 							bool bRtn = false;
// 							if (bSelfPackage)
// 							{
// 								dwID = SELF.GetPackageGood(iItem).GetID();
// 								bRtn = g_pGameControl->SEND_Use_Object(iItem);
// 							}
// 							else
// 							{
// 								dwID = pSelf->package.Get(iItem).GetID();
// 								bRtn = g_pGameControl->SEND_Trusteeship_Use_Object(iItem);
// 							}
// 
// 							if (bRtn)
// 							{
// 								m_AutoUse_itemID = dwID;
// 								if(i < 5)// < 5的慢性药品不要同时喝,>5的速效药品允许同时喝
// 								{
// 									m_hp_nexttime = dwCount + trusteeshipEatCfg.hp_delay;
// 									break;
// 								}
// 							}
// 						}
// 					}
// 					else
// 					{
// 						if(dwCount - dwLastPrompt > 10000)
// 						{
// 							sprintf(temp,"包裹里已经没有[%s]",eat.sHPName.c_str());
// 							g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
// 							dwLastPrompt = dwCount;
// 						}
// 					}
// 				}
// 			}
// 
// 			// 使用pt道具，如天山雪莲
// 			if(pSelf->eatConfig.bUsePTSuperHP && dwCount - m_dwLastUseSuperRed > 2000 && SELF.GetReserveData(plyDrinkSuperRed) == 0)
// 			{
// 				int iItem = pSelf->package.FindGoodByName("天山雪莲");
// 				if(iItem < 0)
// 					iItem = pSelf->package.FindGoodByName("超级天山雪莲");
// 				if(iItem < 0)
// 					iItem = pSelf->package.FindGoodByName("群英天山雪莲");
// 				
// 				bool bSelfPackage = false;//是否喝自己包裹里的,否则喝群英包裹里的
// 				if(iItem < 0)
// 				{
// 					bSelfPackage = true;
// 					iItem = FindPackageItem("天山雪莲");
// 				}
// 				if(iItem < 0)
// 					iItem = FindPackageItem("超级天山雪莲");
// 				if(iItem < 0)
// 					iItem = FindPackageItem("群英天山雪莲");
// 
// 				if(iItem >= 0)
// 				{
// 					m_dwLastUseSuperRed = dwCount;
// 					DWORD dwID = 0;
// 					bool bRtn = false;
// 					if (bSelfPackage)
// 					{
// 						dwID = SELF.GetPackageGood(iItem).GetID();
// 						bRtn = g_pGameControl->SEND_Use_Object(iItem);
// 					}
// 					else
// 					{
// 						dwID = pSelf->package.Get(iItem).GetID();
// 						bRtn = g_pGameControl->SEND_Trusteeship_Use_Object(iItem);
// 					}
// 
// 					if (bRtn)
// 					{
// 						m_AutoUse_itemID = dwID;
// 					}
// 				}
// 			}
// 
// 		}
// 
// 		return;
// 	}


	//非托管模式下
	for(int i = MAX_MEDICA_NUMBER - 1; i >= 0; i--)
	{
		if(i < 5 && dwCount <= m_hp_nexttime) //还没有到允许的喝药时间,5以上的物品不受m_hp_nexttime限制
			break;

		WOOOLMEDICAEAT& eat = m_woool_hpmp[i];

		if(eat.hp_limit == 0 || eat.hp_delay == 0 || eat.sHPName.empty())
			continue;

		if(eat.hp_bused && eat.hp_limit < hp_max && hp < eat.hp_limit)
		{
			int iItem = FindPackageItem(eat.sHPName.c_str());
			//没有单个的,喝带持久的包
			if (iItem < 0)
			{
				iItem = FindPackageItem(eat.sHPPack.c_str());
			}

			if(iItem >= 0)
			{
				if(dwCount > eat.hp_delay + eat.dwLastHPTime)
				{
					eat.dwLastHPTime = dwCount; //每一种药品都要计算时间
					DWORD dwID = SELF.GetPackageGood(iItem).GetID();
					if(g_pGameControl->SEND_Use_Object(iItem))
					{
						m_AutoUse_itemID = dwID;

						if(i < 5)// < 5的慢性药品不要同时喝,>=5的速效药品允许同时喝
						{
							m_hp_nexttime = dwCount+eat.hp_delay;
							break;
						}
					}
				}
			}
			else//没
			{
				if(dwCount - dwLastPrompt > 10000)
				{
					sprintf(temp,"包裹里已经没有[%s]",eat.sHPName.c_str());
					g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
					dwLastPrompt = dwCount;
				}
				continue;
			}
		}
	}

	// 使用pt道具，如天山雪莲
	if(g_AICfgMgr.IsUsePTSuperHP() && dwCount - m_dwLastUseSuperRed > 2000 && SELF.GetReserveData(plyDrinkSuperRed) == 0)
	{
		int iItem = FindPackageItem("天山雪莲");
		if(iItem < 0)
			iItem = FindPackageItem("超级天山雪莲");
		//if(iItem < 0)
		//	iItem = FindPackageItem("群英天山雪莲");

		if(iItem >= 0)
		{
			m_dwLastUseSuperRed = dwCount;
			DWORD dwID = SELF.GetPackageGood(iItem).GetID();
			if(g_pGameControl->SEND_Use_Object(iItem))
			{
				m_AutoUse_itemID = dwID;
			}
		}
	}
}

void CAIAutoEatMgr::AddMP()					// 封装加蓝策略
{
	int mp = SELF.GetMP(); 
	int mp_max = SELF.GetMPMax(); 
	DWORD dwCount = GetTickCount();
	static DWORD dwLastPrompt = 0;
	char temp[256] = {0};

// 	//自己在托管模式下,而且不是队长,或者队长微操队员,或者离线托管的队员,如果群英包裹里有药品,先用群英包裹里的
// 	if (g_TrusteeshipData.IsTrusteeship() && (!g_TrusteeshipData.IsCaptain() || g_pSelf != &ORIGINALSELF))
// 	{
// 		TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 
// 		if (pSelf)
// 		{
// 			for(int i = MAX_MEDICA_NUMBER - 1; i >= 0; i--)
// 			{
// 				if(i < 5 && dwCount <= m_mp_nexttime) //还没有到允许的喝药时间,5以上的物品不受m_mp_nexttime限制
// 					return;
// 
// 				WOOOLMEDICAEAT& eat = m_woool_hpmp[i]; 
// 				WOOOLMEDICA& trusteeshipEatCfg = pSelf->eatConfig.medica[i];
// 
// 				if(trusteeshipEatCfg.mp_limit == 0 || trusteeshipEatCfg.mp_delay == 0 || eat.sMPName.empty())
// 					continue;
// 
// 				if(trusteeshipEatCfg.mp_bused && trusteeshipEatCfg.mp_limit < mp_max && mp < trusteeshipEatCfg.mp_limit)
// 				{
// 					int iItem = pSelf->package.FindGoodByName(eat.sMPName.c_str());
// 					bool bSelfPackage = false;//是否喝自己包裹里的,否则喝群英包裹里的
// 					if (iItem < 0)
// 					{
// 						iItem = FindPackageItem(eat.sMPName.c_str());
// 						bSelfPackage = true;
// 					}
// 					//没有单个的,喝带持久的包
// 					if (iItem < 0)
// 					{
// 						iItem = pSelf->package.FindGoodByName(eat.sMPPack.c_str());
// 						bSelfPackage = false;
// 					}
// 
// 					if (iItem < 0)
// 					{
// 						iItem = FindPackageItem(eat.sMPPack.c_str());
// 						bSelfPackage = true;
// 					}
// 
// 					if(iItem >= 0)
// 					{
// 						if (dwCount > trusteeshipEatCfg.mp_delay + eat.dwLastMPTime)
// 						{
// 							eat.dwLastMPTime = dwCount; //每一种药品都要计算时间
// 
// 							DWORD dwID = 0;
// 							bool bRtn = false;
// 							if (bSelfPackage)
// 							{
// 								dwID = SELF.GetPackageGood(iItem).GetID();
// 								bRtn = g_pGameControl->SEND_Use_Object(iItem);
// 							}
// 							else
// 							{
// 								dwID = pSelf->package.Get(iItem).GetID();
// 								bRtn = g_pGameControl->SEND_Trusteeship_Use_Object(iItem);
// 							}
// 
// 							if(bRtn)
// 							{
// 								m_AutoUse_MpItemID = dwID;
// 								if(i < 5)// < 5的慢性药品不要同时喝,>4的速效药品允许同时喝
// 								{
// 									m_mp_nexttime = dwCount+trusteeshipEatCfg.mp_delay; 
// 									break;
// 								}
// 							}
// 						}
// 					}
// 					else
// 					{
// 						if(dwCount - dwLastPrompt > 10000)
// 						{
// 							sprintf(temp,"包裹里已经没有[%s]",eat.sMPName.c_str());
// 							g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
// 							dwLastPrompt = dwCount;
// 						}
// 					}
// 				}
// 			}
// 
// 			// 使用pt道具，如深海灵礁
// 			if(pSelf->eatConfig.bUsePTSuperMP && dwCount - m_dwLastUseSuperBlue > 2000 && SELF.GetReserveData(plyDrinkSuperBlue) == 0)
// 			{
// 				int iItem = pSelf->package.FindGoodByName("深海灵礁");
// 				if(iItem < 0)
// 					iItem = pSelf->package.FindGoodByName("超级深海灵礁");
// 				if(iItem < 0)
// 					iItem = pSelf->package.FindGoodByName("群英深海灵礁");
// 
// 				bool bSelfPackage = false;//是否喝自己包裹里的,否则喝群英包裹里的
// 				if(iItem < 0)
// 				{
// 					bSelfPackage = true;
// 					iItem = FindPackageItem("深海灵礁");
// 				}
// 				if(iItem < 0)
// 					iItem = FindPackageItem("超级深海灵礁");
// 				if(iItem < 0)
// 					iItem = FindPackageItem("群英深海灵礁");
// 
// 
// 				if(iItem >= 0)
// 				{
// 					m_dwLastUseSuperBlue = dwCount;
// 					DWORD dwID = 0;
// 
// 					bool bRtn = false;
// 					if (bSelfPackage)
// 					{
// 						dwID = SELF.GetPackageGood(iItem).GetID();
// 						bRtn = g_pGameControl->SEND_Use_Object(iItem);
// 					}
// 					else
// 					{
// 						dwID = pSelf->package.Get(iItem).GetID();
// 						bRtn = g_pGameControl->SEND_Trusteeship_Use_Object(iItem);
// 					}
// 
// 					if(bRtn)
// 					{
// 						m_AutoUse_MpItemID = dwID;
// 					}
// 				}
// 			}
// 		}
// 
// 		return;
// 	}


	//非托管模式下
	for(int i = MAX_MEDICA_NUMBER - 1; i >= 0; i--)
	{
		if(i < 5 && dwCount <= m_mp_nexttime) //还没有到允许的喝药时间,4以上的物品不受m_mp_nexttime限制
			return;

		WOOOLMEDICAEAT& eat = m_woool_hpmp[i]; 

		if(eat.mp_limit == 0 || eat.mp_delay == 0 || eat.sMPName.empty())
			continue;

		if(eat.mp_bused && eat.mp_limit < mp_max && mp < eat.mp_limit)
		{
			int iItem = FindPackageItem(eat.sMPName.c_str());
			//没有单个的,喝带持久的包
			if (iItem < 0)
			{
				iItem = FindPackageItem(eat.sMPPack.c_str());
			}

			if(iItem >= 0)
			{
				if(dwCount > eat.mp_delay + eat.dwLastMPTime)
				{
					eat.dwLastMPTime = dwCount; //每一种药品都要计算时间

					DWORD dwID = SELF.GetPackageGood(iItem).GetID();
					if(g_pGameControl->SEND_Use_Object(iItem))
					{
						m_AutoUse_MpItemID = dwID;

						if(i < 5)// < 5的慢性药品不要同时喝,>=5的速效药品允许同时喝
						{
							m_mp_nexttime = dwCount+eat.mp_delay; 
							break;
						}
					}

				}
			}
			else//没
			{
				if(dwCount - dwLastPrompt > 10000)
				{
					sprintf(temp,"包裹里已经没有[%s]",eat.sMPName.c_str());
					g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
					dwLastPrompt = dwCount;
				}
				continue;
			}
		}
	}

	// 使用pt道具，如深海灵礁
	if(g_AICfgMgr.IsUsePTSuperMP() && dwCount - m_dwLastUseSuperBlue > 2000 && SELF.GetReserveData(plyDrinkSuperBlue) == 0)
	{
		int iItem = FindPackageItem("深海灵礁");
		if(iItem < 0)
			iItem = FindPackageItem("超级深海灵礁");
		//if(iItem < 0)
		//	iItem = FindPackageItem("群英深海灵礁");

		if(iItem >= 0)
		{
			m_dwLastUseSuperBlue = dwCount;
			DWORD dwID = SELF.GetPackageGood(iItem).GetID();
			if(g_pGameControl->SEND_Use_Object(iItem))
			{
				m_AutoUse_MpItemID = dwID;
			}
		}
	}
}

void CAIAutoEatMgr::UseSuccess(CGood& tmp)
{
	DWORD dwID = tmp.GetID();
	if(dwID == 0)
		return;

	//设置上次使用物品成功信息
	int pos = tmp.GetPos() - MAX_PACKAGE_NUM;
	SetGoodInf(pos,g_AIGoodMgr.GetObjectAttr(tmp));

	if(dwID != m_AutoUse_PackItemID && dwID != m_AutoUse_itemID && dwID != m_AutoUse_MpItemID)
		return;

	//提示信息
	char temp[256];
	if(dwID == m_AutoUse_PackItemID)
	{
		int item_number = CountItemNumber(tmp.GetName());
		sprintf(temp,"自动解包[%s],包裹里剩余[%d]个",tmp.GetName(),item_number);
		g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
		m_AutoUse_PackItemID = 0;
	}
	else
	{
		int item_number = CountItemNumber(tmp.GetName());
// 		if (g_TrusteeshipData.IsTrusteeship() && (!g_TrusteeshipData.IsCaptain() || g_TrusteeshipData.GetMicroControlPos() >= 0 || g_TrusteeshipData.GetCaptionSubstitutePos() >= 0))
// 		{
// 			TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 			if (pSelf)
// 				item_number += pSelf->package.GetNumber(-1,-1,tmp.GetName());
// 		}

		if (item_number >= 0)
		{
			sprintf(temp,"自动使用[%s],包裹里剩余[%d]个",tmp.GetName(),item_number); 
			g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
		}
		if(dwID == m_AutoUse_itemID)
			m_AutoUse_itemID = 0;
		else
			m_AutoUse_MpItemID = 0;

		m_AutoUse_PackItemID = 0;//清除包
	}
}

void CAIAutoEatMgr::BadBood(DWORD dwID)
{
	if(dwID == m_AutoUse_PackItemID)
	{
		m_AutoUse_PackItemID = 0;
	}
	else if(dwID == m_AutoUse_itemID)
	{
		m_AutoUse_itemID = 0;
	}
	else if(dwID == m_AutoUse_MpItemID)
	{
		m_AutoUse_MpItemID = 0;	
	}
}

bool CAIAutoEatMgr::IsMedical(CGood& tmp)
{
	if(m_AutoUse_itemID > 0 || m_AutoUse_MpItemID > 0)
	{
		if(tmp.GetStdMode() == 0 && (tmp.GetShape() == 0 || tmp.GetShape() == 1))
			return true;
	}
	return false;
}

void CAIAutoEatMgr::UseFailed(CGood& tmp)
{
	DWORD dwID = tmp.GetID();
	if(dwID == 0)
		return;
	if(dwID != m_AutoUse_PackItemID && dwID != m_AutoUse_itemID && dwID != m_AutoUse_MpItemID)
		return;

	char temp[256];
	if(dwID == m_AutoUse_PackItemID)
	{
		if(strlen(tmp.GetName()) > 0)
		{
			sprintf(temp,"自动解包[%s]失败",tmp.GetName());
			g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
		}
		m_AutoUse_PackItemID = 0;
	}
	else
	{
		if(strlen(tmp.GetName()) > 0)
		{
			//雪莲类物品,按持久使用的药品包[药品(批量)]
			if(tmp.GetStdMode() == 0 && (tmp.GetShape() == 200 || tmp.GetShape() == 201 || tmp.GetShape() == 202))
			{
				int iUseTimes = tmp.GetDura()-1;
				if(iUseTimes < 0) iUseTimes = 0;
				int item_number = CountItemNumber(tmp.GetName());
				if(iUseTimes == 0)//像超级天山雪莲，最一次被使用的时候iUseTimes = tmp.GetDura()-1　＝　0;的时候，马上后面协议来了这个物品就会被收掉
					item_number -= 1;

// 				if (g_TrusteeshipData.IsTrusteeship() && (!g_TrusteeshipData.IsCaptain() || g_TrusteeshipData.GetMicroControlPos() >= 0 || g_TrusteeshipData.GetCaptionSubstitutePos() >= 0))
// 				{
// 					TneupMember* pSelf = g_TrusteeshipData.GetSelf();
// 					if (pSelf)
// 						item_number += pSelf->package.GetNumber(-1,-1,tmp.GetName());
// 				}

				if (item_number >= 0)
				{
					sprintf(temp,"自动使用[%s],剩余[%d]次,包裹里剩余[%d]个",tmp.GetName(),iUseTimes,item_number); 
					g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
				}
			}
			else
			{
				sprintf(temp,"自动使用[%s]失败",tmp.GetName()); 
				g_TalkMgr.PushSysTalk(temp,TALKCOLOR_PINK);
			}				
		}		

		if(dwID == m_AutoUse_itemID)
			m_AutoUse_itemID = 0;
		else
			m_AutoUse_MpItemID = 0;			
	}
}

void CAIAutoEatMgr::SetGoodInf(int i,UINT uType)
{
	if(i < 0 || i >= MAX_PANEL_GOOD_NUM)
		return;

	m_GoodUseInf[i].uType = uType;
	if(uType == 0)
		m_GoodUseInf[i].dwLastTime = 0;
	else
		m_GoodUseInf[i].dwLastTime = GetTickCount();
}

void CAIAutoEatMgr::AutoPlace()
{
	DWORD dwCount = GetTickCount();
	if(m_AutoUse_PackItemID > 0 && dwCount - m_dwAutoUnbindTime < 3000) //// 正在解包，还没有消息
		return;

	if(SELF.GetUsingTemp().GetBlankNumber() <= 0 && dwCount - SELF.GetReserveData(plyLastUseObjectTime) < 3000) //正在使用物品
		return;

	for(int i = 0; i < MAX_PANEL_GOOD_NUM; i++) //托盘
	{
		if(m_GoodUseInf[i].uType == 0 || dwCount - m_GoodUseInf[i].dwLastTime < 500)
			continue;

		if(SELF.GetPackageGood(i+MAX_PACKAGE_NUM).GetID() != 0) //已经存在物品了
		{
			SetGoodInf(i,0);
			continue;
		}

		int iFindPos = -1;
		for(int ii = MAX_PACKAGE_NUM - 1;ii >= 0; ii--)
		{
			CGood& tmp = SELF.GetPackageGood(ii);
			if(tmp.GetID() == 0 || (SELF.GetBoothState() && (g_BoothData.IsInBooth(tmp.GetID(),tmp.GetName()) || g_OtherBoothData.IsInBooth(tmp.GetID(),tmp.GetName()))))
				continue;

			UINT uType = g_AIGoodMgr.GetObjectAttr(tmp);
			if((uType & 0xFFFF00) == (m_GoodUseInf[i].uType & 0xFFFF00))
			{
				if(iFindPos < 0)
					iFindPos = ii;

				if(uType == m_GoodUseInf[i].uType)
				{
					iFindPos = ii;
					break;
				}
			}
		}

		if(iFindPos >= 0) //找到了对应的药品
		{
			CGood& SrcGood = SELF.GetPackageGood(iFindPos);
			SELF.GetPackageGood(i+MAX_PACKAGE_NUM).Assign(SrcGood);
			SrcGood.SetID(0);
		}
		else //找对应的打包物品进行解包
		{
			for(int ii = MAX_PACKAGE_NUM - 1;ii >= 0; ii--)
			{
				CGood& tmp = SELF.GetPackageGood(ii);
				if(tmp.GetID() == 0 || (SELF.GetBoothState() && (g_BoothData.IsInBooth(tmp.GetID(),tmp.GetName()) || g_OtherBoothData.IsInBooth(tmp.GetID(),tmp.GetName()))))
					continue;

				UINT uType = g_AIGoodMgr.GetObjectAttr(tmp);
				if(((uType & 0x00FF00) == (m_GoodUseInf[i].uType & 0x00FF00)) && ((uType &0xFF0000) == 0x010000))
				{
					if(iFindPos < 0)
						iFindPos = ii;

					if((uType & 0x00FFFF) == (m_GoodUseInf[i].uType & 0x00FFFF))
					{
						iFindPos = ii;
						break;
					}
				}
			}

			if(iFindPos >= 0)
			{
				int blank = CountBlankNumber();
				if(blank >= 5)
				{
					if((m_AutoUse_PackItemID == 0 || dwCount - m_dwAutoUnbindTime > 3000) && dwCount - m_dwAutoUnbindTime > 1000)//> 3s就认为是异常,为了不至于导致一直无法解包的情况,这里应该清除m_AutoUse_PackItemID
					{
						DWORD dwID = SELF.GetPackageGood(iFindPos).GetID();
						if(g_pGameControl->SEND_Use_Object(iFindPos))
						{
							m_AutoUse_PackItemID = dwID;
							m_dwAutoUnbindTime = dwCount;
						}
					}
					else//如果已经有一个开始解包了，或是时间还没到，下一个等这个解完了再处理,
					{
						continue; //继续,其它的物品继续往腰带上放，但不解包,也不清除,不能执行SetGoodInf(i,0);
					}
				}
				else if(blank < 5)
				{
					static DWORD dwLastPromt = 0;
					if(GetTickCount() - dwLastPromt > 10000)
					{
						g_TalkMgr.PushSysTalk("包裹里空位不足，无法解包！",TALKCOLOR_PINK);
						dwLastPromt = dwCount;
					}
				}
			}
			else
			{
				switch(m_GoodUseInf[i].uType)
				{
				case 0x000101:
				case 0x000102:
				case 0x000103:
				case 0x000104:
					g_TalkMgr.PushSysTalk("您包裹内的金创药已经用完了!",TALKCOLOR_PINK);
					break;
				case 0x000201:
				case 0x000202:
				case 0x000203:
				case 0x000204:
					g_TalkMgr.PushSysTalk("您包裹内的魔法药已经用完了!",TALKCOLOR_PINK);
					break;
				}
			}
		}
		SetGoodInf(i,0);
	}
}

////灵兽微俄时自动喂食兽粮
//void CAIAutoEatMgr::AutoFeed()
//{
//	if(g_AICfgMgr.IsAutoFeed() && GetTickCount() - m_dwLastAutoFeedTime > 5000)
//	{
//		m_dwLastAutoFeedTime = GetTickCount();
//
//		map<DWORD,PET_SENDOUT>::iterator it;
//		it = g_PetData.GetSendOutPetList().begin();
//		for( ; it != g_PetData.GetSendOutPetList().end(); it++)
//		{
//			PET_SENDOUT &SendOutPet = it->second;
//
//			if(SendOutPet.dwPetItemID == 0)
//				continue;
//
//			//判断灵兽是否处于微饿状态
//			__time32_t t_time;
//			_time32(&t_time);
//			t_time += g_dwServerTime;
//
//			if(t_time - SendOutPet.tLastEatTime < 48*3600)
//				continue;
//
//			int iPos = 0;
//			DWORD dwGoodID = 0;
//
//			//先寻找极品
//			for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
//			{
//				CGood& tmp = SELF.GetPackageGood(i);
//				if(tmp.GetID() == 0)
//					continue;
//
//				if(stricmp("极品兽粮",tmp.GetName()) == 0 || stricmp("极品豹粮",tmp.GetName()) == 0)
//				{
//					dwGoodID = tmp.GetID();
//					iPos = i;
//					break;
//				}
//			}
//
//			//再寻找精品
//			if (dwGoodID == 0)
//			{
//				for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
//				{
//					CGood& tmp = SELF.GetPackageGood(i);
//					if(tmp.GetID() == 0)
//						continue;
//
//					if(stricmp("精制兽粮",tmp.GetName()) == 0 || stricmp("精制豹粮",tmp.GetName()) == 0)
//					{
//						dwGoodID = tmp.GetID();
//						iPos = i;
//						break;
//					}
//				}
//			}
//
//			//寻找一般兽粮
//			if (dwGoodID == 0)
//			{
//				for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
//				{
//					CGood& tmp = SELF.GetPackageGood(i);
//					if(tmp.GetID() == 0)
//						continue;
//
//					if(stricmp("兽粮",tmp.GetName()) == 0 || stricmp("豹粮",tmp.GetName()) == 0)
//					{
//						dwGoodID = tmp.GetID();
//						iPos = i;
//						break;
//					}
//				}
//			}
//
//			//寻找豹粮（捆）
//			if (dwGoodID == 0)
//			{
//				for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
//				{
//					CGood& tmp = SELF.GetPackageGood(i);
//
//					if(tmp.GetID() == 0)
//						continue;
//
//					if((stricmp("兽粮(捆)",tmp.GetName()) == 0 || stricmp("豹粮(捆)",tmp.GetName()) == 0) && tmp.GetDura() > 0) 
//					{
//						dwGoodID = tmp.GetID();
//						iPos = i;
//						break;
//					}
//				}
//			}
//
//			if(dwGoodID > 0)
//			{			
//				CGood *pGood = SELF.GetUsingTemp().Add(SELF.GetPackageGood(iPos));
//				if(pGood)
//				{
//					pGood->SetPos(iPos);
//					SELF.GetPackageGood(iPos).SetID(0);
//					g_pGameControl->SEND_Feed_Pet_From_Interface(SendOutPet.dwPetItemID,dwGoodID);
//				}
//			}				
//		}
//	}
//}

bool CAIAutoEatMgr::IsCanAddToPanel(WORD wGoodLooks,int iPanelPos)
{
	if(iPanelPos < 0 || iPanelPos >= MAX_PANEL_GOOD_NUM)
		return true;

	//先看看是不是有一个正在使用并且持久大于1的物品正好在这个位置，如果是不能占据，因为使用后这个物品要求放回这个位置
	CGoodArray& goodArray = SELF.GetUsingTemp();
	int iPackagePos = iPanelPos + (MAX_PACKAGE_ELEMENT - MAX_PANEL_GOOD_NUM);
	for(int i = 0; i < goodArray.Size(); i++)
	{
		CGood& tmp = goodArray.Get(i);
		if(tmp.GetID() > 0 && tmp.GetDura() > 1 && tmp.GetPos() == iPackagePos)
		{
			return false;
		}
	}
	//再看看这个位置是否要求自动把同类物品放到这个位置,如果是而新来的物品又和它不同类那么这个位置不能放
	if(m_GoodUseInf[iPanelPos].uType == 0)
		return true;

	UINT uType = g_AIGoodMgr.GetObjectAttrByLooks(wGoodLooks);

	//if(uType != m_GoodUseInf[iPanelPos].uType)
	if((uType & 0xFFFF00) != (m_GoodUseInf[iPanelPos].uType & 0xFFFF00))//不是相近类别
		return false;

	return true;
}

int CAIAutoEatMgr::FindPanelEmptyPos(WORD wGoodLooks)
{
	if(wGoodLooks <= 0)
		return -1;

	for(int i = MAX_PACKAGE_ELEMENT - 6,iPanelPos = 0;i < MAX_PACKAGE_ELEMENT; i++,iPanelPos++)
	{
		if(SELF.GetPackageGood(i).GetID() == 0 && IsCanAddToPanel(wGoodLooks,iPanelPos))
		{
			return i;
		}
	}

	return -1;
}


