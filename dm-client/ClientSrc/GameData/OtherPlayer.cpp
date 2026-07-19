#include "otherplayer.h"
#include "GameData.h"
#include "PetData.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

COtherPlayer::COtherPlayer(void)
{
	Clear();
	m_bGettingTex = false;
	m_strIndividualStr.clear();
}

COtherPlayer::~COtherPlayer(void)
{
}

void COtherPlayer::Clear()
{
	m_bWing = false;
	m_bLogo = false;
	m_bGuildTowerSwitch = false;
	m_dwLogoID  = 0;
	m_NameColor = 0;
	m_strName.clear();
	m_strGuildName.clear();
	m_strTitle.clear();
	m_strPhyleName.clear();

	for(int i = 0; i < MAX_EQUIPMENT; i++)
	{
		m_equipment[i].SetID(0);
	}
	m_nYuanShenInfo = 0;
	m_iYuanShenSex  = 0;
    m_bFightOnLeopard = false;
	m_byJob = 0;
	m_bPutOnShield = 0;
	m_iGuildTowerLevel = 0;
	m_byGuildPhyle = 0;
	memset(&m_GuildBuffer,0,sizeof(m_GuildBuffer));
	m_iVipTradeLevel = 0;
	m_iFaBaoType = 0;
	m_iFaBaoLevel = 0;
// 	m_iHolyWingLevel = -1;
// 	m_iHolyWingStrongLevel = -1;
	m_iFaBaoTypeLeft = 0;
	m_iFaBaoTypeRight = 0;
	m_iFaBaoLevel = 0;
}

bool COtherPlayer::FromBuffer(const char* buf,int iLen)
{
	Clear();

	m_dwPlayerID = Conv_DWORD(buf);
	m_dwLogoID = BYTE(buf[6]);//Conv_DWORD(buf+6);
	BYTE byGoodLen = BYTE(buf[7]);//单个物品的长度
	if (byGoodLen == 0)
	{
		byGoodLen = CGood::PKLength();
	}

	SetLooks(Conv_INT64(buf + 12));
	m_nYuanShenInfo = buf[10];
	m_iYuanShenSex = buf[11];

	m_strName.clear();
	if (buf[20] > 0)
	{
		m_strName.assign(buf + 21,buf[20]);
	}

	CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(m_strName.c_str());
	if(pChar != NULL)
	{
		if(pChar->GetMaskLevel() > 0 && strlen(pChar->GetMaskName()) > 0)
		{
			SetWing(true);
		}
		else
		{
			SetWing(false);
		}

		m_iFaBaoType = pChar->GetFaBaoType()->dwCurFaBaoType;
		m_iFaBaoLevel = pChar->GetFaBaoType()->iFaBaoLevel;
		m_iFaBaoTypeLeft = pChar->GetFaBaoType()->dwLeftFaBaoType;
		m_iFaBaoTypeRight = pChar->GetFaBaoType()->dwRightFaBaoType;
	}

	m_NameColor = g_pGameData->GetMirColor(buf[36]);

	m_strGuildName.clear();
	if (buf[40] > 0)
	{
		m_strGuildName.assign(buf+41,buf[40]);
	}

	m_strTitle.clear();
	if (buf[55] > 0)
	{
		m_strTitle.assign(buf + 56,buf[55]);
	}

	if(buf[71])
		m_bIsMale = false;
	else
		m_bIsMale = true;

	char* ch = (char*)(buf + 72);
	for(int i = 0; i <  MAX_EQUIPMENT && ch - buf < iLen ; i++)
	{
		if (i == 16)
		{
			m_equipment[ITEM_POS_WING].FromBuffer(ch,false,byGoodLen);
		}
		else if (i == 17)
		{
			m_equipment[ITEM_POS_WENPEI].FromBuffer(ch,false,byGoodLen);
		}
		else
		{
			m_equipment[i].FromBuffer(ch,false,byGoodLen);
		}
		
		m_equipment[i].AddItemState(InOtherPlayer);
		ch += byGoodLen;
	}

	if(m_equipment[ITEM_POS_SHIELD].GetID() != 0)
	{
		m_bPutOnShield = true;
	}
	else
	{
		m_bPutOnShield = false;
	}

	m_strIndividualStr.clear();
	if(ch[0] > 0 && ch - buf < iLen)
	{
		m_strIndividualStr.assign(ch+1,ch[0]);
		//后面没法再扩展数据了,为了保证老客户端,在个性化签名后面扩展后继数据
		char szExtBuf[10] = {0};
		int iPos = m_strIndividualStr.find('\0');
		if (iPos >= 0 && iPos < m_strIndividualStr.size())
		{
			memcpy(szExtBuf,m_strIndividualStr.c_str() + iPos + 1,min(10,m_strIndividualStr.size() - iPos - 1));
			if (iPos == 0)
			{
				m_strIndividualStr.clear();
			}
			else
			{
				m_strIndividualStr = m_strIndividualStr.substr(0,iPos);
			}
		}

		m_iVipTradeLevel = szExtBuf[0];		
		//BYTE byWingColor = szExtBuf[1];
		//SetCharHolyWingStrongLevel( (int)(byWingColor & 0xF) );
		//SetCharHolyWingLevel( (int)((byWingColor >> 4) & 0xF) );
		//
		//m_iFaZhenLevel = szExtBuf[2];
	}

    ch += (ch[0] + 1);

	m_byJob = ch[0];
	ch += 1;

    if(ch - buf < iLen)	//如果有数据的话就判断骑战状态
    {
        if(ch[0] != 0)
        {
            m_bFightOnLeopard = true;
        }
    }
    else
    {
        m_bFightOnLeopard = false;
    }

	ch += 1;
	if(ch - buf < iLen)		//如果有数据的话就判断行会附加状态
	{
		m_iGuildTowerLevel  = ch[0];
		m_bGuildTowerSwitch = (ch[1] == 1);
		if(m_dwLogoID == 0 && m_iGuildTowerLevel != 0)
		{
			m_dwLogoID = 1;
		}

		ch += 2;
		//本体Buffer加成
		m_GuildBuffer.m_byAttackFgh[0] = ch[0];
		m_GuildBuffer.m_byAttackFgh[1] = ch[1];	
		m_GuildBuffer.m_byAttackRab[0] = ch[2];
		m_GuildBuffer.m_byAttackRab[1] = ch[3];
		m_GuildBuffer.m_byAttackDao[0] = ch[4];
		m_GuildBuffer.m_byAttackDao[1] = ch[5];
		m_GuildBuffer.m_byPhyDef[0]	   = ch[6];
		m_GuildBuffer.m_byPhyDef[1]	   = ch[7];
		m_GuildBuffer.m_byMagicDef[0]  = ch[8];	
		m_GuildBuffer.m_byMagicDef[1]  = ch[9];
		m_GuildBuffer.m_byMagicHit	   = ch[10];		
		m_GuildBuffer.m_byPhyHit	   = ch[11];			
		m_GuildBuffer.m_byMagicAvoid   = ch[12];		
		m_GuildBuffer.m_byPhyAvoid	   = ch[13];
// 		//元神Buffer加成
// 		m_GuildBuffer.m_byMAttackFgh[0] = ch[14];
// 		m_GuildBuffer.m_byMAttackFgh[1] = ch[15];	
// 		m_GuildBuffer.m_byMAttackRab[0] = ch[16];
// 		m_GuildBuffer.m_byMAttackRab[1] = ch[17];
// 		m_GuildBuffer.m_byMAttackDao[0] = ch[18];
// 		m_GuildBuffer.m_byMAttackDao[1] = ch[19];
// 		m_GuildBuffer.m_byMPhyDef[0]    = ch[20];
// 		m_GuildBuffer.m_byMPhyDef[1]    = ch[21];
// 		m_GuildBuffer.m_byMMagicDef[0]  = ch[22];
// 		m_GuildBuffer.m_byMMagicDef[1]  = ch[23];
// 		m_GuildBuffer.m_byMMagicHit	    = ch[24];		
// 		m_GuildBuffer.m_byMPhyHit		= ch[25];		
// 		m_GuildBuffer.m_byMMagicAvoid	= ch[26];	
// 		m_GuildBuffer.m_byMPhyAvoid	    = ch[27];

		m_GuildBuffer.m_byJinDefend = ch[14];
		m_GuildBuffer.m_byMuDefend = ch[15];	
		m_GuildBuffer.m_byTuDefend = ch[16];
		m_GuildBuffer.m_byShuiDefend = ch[17];
		m_GuildBuffer.m_byHuoDefend = ch[18];
		m_GuildBuffer.m_wHP = Conv_WORD(ch + 19);
		m_GuildBuffer.m_wMP = Conv_WORD(ch + 21);

// 		m_GuildBuffer.m_byMPhyDef[1]    = ch[21];
// 		m_GuildBuffer.m_byMMagicDef[0]  = ch[22];
// 		m_GuildBuffer.m_byMMagicDef[1]  = ch[23];
// 		m_GuildBuffer.m_byMMagicHit	    = ch[24];		
// 		m_GuildBuffer.m_byMPhyHit		= ch[25];		
// 		m_GuildBuffer.m_byMMagicAvoid	= ch[26];	
// 		m_GuildBuffer.m_byMPhyAvoid	    = ch[27];

		ch += 28;
		m_bSelfGuildTowerSwitch = (*((WORD*)ch) == 1);

		ch += 2;
		m_strOfficer.clear();
		if(ch - buf < iLen)
		{
			m_strOfficer.assign(ch,32);
			ch += 32;
		}

		m_byGuildPhyle = 0;
		if(ch - buf < iLen)//有行会神魔属性加成
		{
			m_byGuildPhyle = ch[0];

			ch += 1;
			//本体Buffer加成
			m_GuildBuffer.m_byAttackFgh_Phyle[0] = ch[0];
			m_GuildBuffer.m_byAttackFgh_Phyle[1] = ch[1];	
			m_GuildBuffer.m_byAttackRab_Phyle[0] = ch[2];
			m_GuildBuffer.m_byAttackRab_Phyle[1] = ch[3];
			m_GuildBuffer.m_byAttackDao_Phyle[0] = ch[4];
			m_GuildBuffer.m_byAttackDao_Phyle[1] = ch[5];
			m_GuildBuffer.m_byPhyDef_Phyle[0]	   = ch[6];
			m_GuildBuffer.m_byPhyDef_Phyle[1]	   = ch[7];
			m_GuildBuffer.m_byMagicDef_Phyle[0]  = ch[8];	
			m_GuildBuffer.m_byMagicDef_Phyle[1]  = ch[9];
			m_GuildBuffer.m_byMagicHit_Phyle	   = ch[10];		
			m_GuildBuffer.m_byPhyHit_Phyle	   = ch[11];			
			m_GuildBuffer.m_byMagicAvoid_Phyle   = ch[12];		
			m_GuildBuffer.m_byPhyAvoid_Phyle	   = ch[13];
			//元神Buffer加成
			m_GuildBuffer.m_byMAttackFgh_Phyle[0] = ch[14];
			m_GuildBuffer.m_byMAttackFgh_Phyle[1] = ch[15];	
			m_GuildBuffer.m_byMAttackRab_Phyle[0] = ch[16];
			m_GuildBuffer.m_byMAttackRab_Phyle[1] = ch[17];
			m_GuildBuffer.m_byMAttackDao_Phyle[0] = ch[18];
			m_GuildBuffer.m_byMAttackDao_Phyle[1] = ch[19];
			m_GuildBuffer.m_byMPhyDef_Phyle[0]    = ch[20];
			m_GuildBuffer.m_byMPhyDef_Phyle[1]    = ch[21];
			m_GuildBuffer.m_byMMagicDef_Phyle[0]  = ch[22];
			m_GuildBuffer.m_byMMagicDef_Phyle[1]  = ch[23];
			m_GuildBuffer.m_byMMagicHit_Phyle	    = ch[24];		
			m_GuildBuffer.m_byMPhyHit_Phyle		= ch[25];		
			m_GuildBuffer.m_byMMagicAvoid_Phyle	= ch[26];	
			m_GuildBuffer.m_byMPhyAvoid_Phyle	    = ch[27];

			ch += 28;
		}


		if(ch - buf < iLen)//有宗族
		{
			m_strPhyleName.assign(ch,25);
			ch += 25;
		}
		else
		{
			m_strPhyleName.clear();
		}
	}

	return true;
}


int COtherPlayer::GetWearGold()
{
	if(!(m_looks.Player.wBody == 19 || m_looks.Player.wBody == 20))
	{
		return -1;
	}

	for(int i = 3;i <= 10;i++)
	{
		if(m_equipment[i].GetID() == 0)
		{
			return -1;
		}
	}

	for(int i = 0;i < 3;i++)
	{
		//3项链4头盔5 6左右手镯7 8左右戒指9鞋子10腰带
		if(m_equipment[3].GetLooks() ==1201 + i * 6
			&& m_equipment[4].GetLooks() ==1200 + i * 6
			&& m_equipment[5].GetLooks() ==1202 + i * 6
			&& m_equipment[6].GetLooks() ==1202 + i * 6
			&& m_equipment[7].GetLooks() ==1203 + i * 6
			&& m_equipment[8].GetLooks() ==1203 + i * 6
			&& m_equipment[9].GetLooks() ==1205 + i * 6
			&& m_equipment[10].GetLooks() ==1204 + i * 6)
		{
			return i;
		}
	}

	return -1;
}

bool COtherPlayer::IsOnLepoard()
{
	return IsLeopard(m_looks.Player.wHorse);
}

bool COtherPlayer::IsOnBigLepoard()
{
	return IsBigLeopard(m_looks.Player.wHorse);
}

DWORD   COtherPlayer::GetGuildLogoID()
{
	if(m_iGuildTowerLevel == 0)
	{
		return 0;
	}

	if(m_dwLogoID > 0 && m_dwLogoID < 5)
	{
		return m_dwLogoID;
	}
	else if(m_iGuildTowerLevel > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void  COtherPlayer::SetLooks(__int64 iLooks)
{
	AssignLooks(m_looks,iLooks);
}
