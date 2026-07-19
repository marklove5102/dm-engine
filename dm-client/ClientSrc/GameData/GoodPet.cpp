#include "GoodPet.h"

typedef unsigned short WORD;
/******************************************************************
/*
/* CGoodPetInfo 宠物的附加属性
/*
/******************************************************************/
CGoodPetInfo::CGoodPetInfo()
{
	clear();
}


void CGoodPetInfo::operator = (const CGoodPetInfo& GoodPet)
{
	m_dwItemID      = GoodPet.m_dwItemID;
	m_nHuanhua      = GoodPet.m_nHuanhua;
	m_byAP          = GoodPet.m_byAP;
	m_nHP			= GoodPet.m_nHP;
	m_nMP			= GoodPet.m_nMP;
	m_nDC			= GoodPet.m_nDC;
	m_nDC2			= GoodPet.m_nDC2 ;
	m_nMC			= GoodPet.m_nMC ;
	m_nMC2			= GoodPet.m_nMC2 ;
	m_nAC			= GoodPet.m_nAC ;
	m_nAC2			= GoodPet.m_nAC2 ;
	m_nMAC			= GoodPet.m_nMAC ;
	m_nMAC2			= GoodPet.m_nMAC2 ;
	m_nSC			= GoodPet.m_nSC ;
	m_nSC2			= GoodPet.m_nSC2 ;
	m_nHit		    = GoodPet.m_nHit ;
	m_nMHit		    = GoodPet.m_nMHit ;
	m_nPoisonDef	= GoodPet.m_nPoisonDef ;
	m_nMDef		    = GoodPet.m_nMDef ;
	m_nLuck		    = GoodPet.m_nLuck ;
	m_nSpeed        = GoodPet.m_nSpeed;
	m_nPoisonHit    = GoodPet.m_nPoisonHit;
	m_nDef          = GoodPet.m_nDef;
	m_nDunWu        = GoodPet.m_nDunWu;
	m_nType         = GoodPet.m_nType;
	m_nLevel        = GoodPet.m_nLevel;
}

void CGoodPetInfo::operator = (CGoodPetInfo& GoodPet)
{
	m_dwItemID      = GoodPet.m_dwItemID;
    m_nHuanhua      = GoodPet.m_nHuanhua;
	m_byAP          = GoodPet.m_byAP;
	m_nHP			= GoodPet.m_nHP;
	m_nMP			= GoodPet.m_nMP;
	m_nDC			= GoodPet.m_nDC;
	m_nDC2			= GoodPet.m_nDC2 ;
	m_nMC			= GoodPet.m_nMC ;
	m_nMC2			= GoodPet.m_nMC2 ;
	m_nAC			= GoodPet.m_nAC ;
	m_nAC2			= GoodPet.m_nAC2 ;
	m_nMAC			= GoodPet.m_nMAC ;
	m_nMAC2			= GoodPet.m_nMAC2 ;
	m_nSC			= GoodPet.m_nSC ;
	m_nSC2			= GoodPet.m_nSC2 ;
	m_nHit		    = GoodPet.m_nHit ;
	m_nMHit		    = GoodPet.m_nMHit ;
	m_nPoisonDef	= GoodPet.m_nPoisonDef ;
	m_nMDef		    = GoodPet.m_nMDef ;
	m_nLuck		    = GoodPet.m_nLuck ;
	m_nSpeed        = GoodPet.m_nSpeed;
	m_nPoisonHit    = GoodPet.m_nPoisonHit;
	m_nDef          = GoodPet.m_nDef;
	m_nDunWu        = GoodPet.m_nDunWu;
	m_nType         = GoodPet.m_nType;
	m_nLevel        = GoodPet.m_nLevel;
}

void CGoodPetInfo::SetValue(const char *str,const int iLen)
{
	if(iLen < 22)
	{
		clear();
		return;
	}
	clear();
	
	//物品ID
	m_dwItemID		= *((DWORD*)str);
	m_nHuanhua      = *((WORD*)(str+4));
	m_byAP          = str[6];
	m_nHP           = *((WORD*)(str+7));
    m_nMP           = ((unsigned char)(str[9]));
	m_nPoisonDef    = ((unsigned char)str[10])&0x1F;
	m_nMAC          = ((unsigned char)str[11])&0x1F;
	m_nMC           = ((unsigned char)str[11])>>5;
	m_nMAC2         = ((unsigned char)str[12])&0x1F;
	m_nMC2          = ((unsigned char)str[12])>>5;
	m_nMHit         = ((unsigned char)str[13])&0x1F;
	m_nSC           = ((unsigned char)str[13])>>5;
	m_nDef          = ((unsigned char)str[14])&0x1F;
	m_nHit          = ((unsigned char)str[14])>>5;
	m_nAC           = ((unsigned char)str[15])&0x1F;
	m_nDC           = ((unsigned char)str[15])>>5;
	m_nAC2          = ((unsigned char)str[16])&0x1F;
	m_nDC2          = ((unsigned char)str[16])>>5;
	m_nMDef         = ((unsigned char)str[17])&0x1F;
	m_nSC2          = ((unsigned char)str[17])>>5;
	m_nPoisonHit    = ((unsigned char)str[18])&0x1F;
	m_nLuck         = ((unsigned char)str[18]>>5)&0x03;
	m_nSpeed        = ((unsigned char)str[18])>>7;
	m_nDunWu        = (unsigned char)str[19];
	//[协议要修改]
	//m_nWXHurt1      = ((unsigned char)str[20])&0x3F;
	//m_nWXHurt2      = ((unsigned char)str[21])&0x3F;
	
	//WORD i = (((unsigned char)str[20])>>6);
	//switch(i)
	//{
	//case 0:
	//	m_strWXHurtName1 = "金";
	//	break;
	//case 1:
	//	m_strWXHurtName1 = "木";
	//	break;
	//case 2:
	//	m_strWXHurtName1 = "土";
	//	break;
	//case 3:
	//	m_strWXHurtName1 = "水";
	//	break;
	//}
	//m_iHurt1 = i;

	//i = (((unsigned char)str[21])>>6);
	//switch(i)
	//{
	//case 0:
	//	m_strWXHurtName2 = "火";
	//	m_iHurt2 = 4;
	//	break;
	//case 1:
	//	m_strWXHurtName2 = "木";
	//	m_iHurt2 = 1;
	//	break;
	//case 2:
	//	m_strWXHurtName2 = "土";
	//	m_iHurt2 = 2;
	//	break;
	//case 3:
	//	m_strWXHurtName2 = "水";
	//	m_iHurt2 = 3;
	//	break;
	//}

	m_nType = (unsigned char)str[22];
	m_nLevel = (unsigned char)str[23];
}

void CGoodPetInfo::clear()
{
	m_dwItemID      = 0;
	m_nHuanhua      = 0;
	m_byAP          = 0;
	m_nHP			= 0;
	m_nMP			= 0;
	m_nDC			= 0;
	m_nDC2			= 0;
	m_nMC			= 0;
	m_nMC2			= 0;
	m_nAC			= 0;
	m_nAC2			= 0;
	m_nMAC			= 0;
	m_nMAC2			= 0;
	m_nSC			= 0;
	m_nSC2			= 0;
	m_nHit		    = 0;
	m_nMHit		    = 0;
	m_nPoisonDef	= 0;
	m_nMDef		    = 0;
	m_nLuck		    = 0;
	m_nSpeed        = 0;
	m_nPoisonHit    = 0;
	m_nDef          = 0;
	m_nDunWu        = 0;
	m_nType         = 0;
	m_nLevel        = 0;
}
