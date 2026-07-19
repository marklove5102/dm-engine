#include "petdata.h"

CPetData  g_PetData;

CPetData::CPetData(void)
{
	Clear();
}

CPetData::~CPetData(void)
{
}

void CPetData::Clear()
{
	m_iTotalAdoptPetNum = 9888888;
	m_iTotalCarryBackPetNum = 0;

	m_SendOutPet.dwPetItemID = 0;
	m_MPetAdopt.clear();
	m_MSendOutPet.clear();
	m_MPetCarryBack.clear();
	m_MMyAdoptPet.clear();
	m_sHorse.bStatus = 0;

}

std::string CPetData::GetPetName(BYTE byPetType,BYTE byPetColor)
{
	std::string strPetName;
	switch(byPetType)
	{
	case 0:
		strPetName = "´ÔÁÖ±ª";	
		break;
	case 1:
		strPetName = "Ì¤ÔÆ±ª";	
		break;
	case 2:
		strPetName = "Ñ¸ÌìÀ×±ª";	
		break;
	case 8:
		strPetName = "ÌìÊ¨ÊÞ";	
		break;
	case 9:
		strPetName = "ÕðÌìÊ¨";	
		break;
	case 10:
		strPetName = "Õ½Ìì¿ñÊ¨";	
		break;
	case 16:
		strPetName = "÷è÷ëÊÞ";	
		break;
	case 17:
		strPetName = "Ê¥÷è÷ë";	
		break;
	case 18:
		strPetName = "ÐþÌì÷è÷ë";	
		break;
	case 24:
		strPetName = "³àôá·ï";
		break;
	case 25:
		strPetName = "°ÁÌì·ï";
		break;
	case 26:
		strPetName = "³ãÌì·ï»Ë";	
		break;
	default:
		strPetName = "";
		break;
	}

	return strPetName;
}

BOOL CPetData::HasPetFollowStatus()
{
	if (m_MSendOutPet.size() <= 0) return FALSE;

	PET_SENDOUT_MAP::iterator it = m_MSendOutPet.begin();
	while (it != m_MSendOutPet.end())
	{
		if (it->second.szReserved[0] == 1)
		{
			return TRUE;
		}

		it++;
	}

	return FALSE;
}

//WORD GetRidePetType( __int64 dwLooks )
//{
//	DWORD tmp = (WORD)((dwLooks >> 8) & 0x000000000000FFFF);
//
//	if(tmp >= 124)
//	{
//		if (tmp >= 225 && tmp <= 237 || (tmp >= 248 && tmp <= 253) || (tmp >= 238 && tmp <= 243))
//		{
//			return (BYTE)(tmp - 224);					//24ÎªÕðÌì¿ñÊ¨
//		}
//		else if(tmp >= 124 && tmp <= 224)				//·ï»Ë
//		{
//			return (BYTE)(tmp - 124 + 30);				//Ò»×ª·ï»Ë30 31 32  ¶þ×ª·ï»Ë33 34 35
//		}			
//	}	
//
//	return 0;
//}

bool IsBigLeopard( WORD wHorse )
{
	//if((byHorse >= 9 && byHorse <= 11) || (byHorse == 28 || byHorse == 29 || byHorse == 24) || (byHorse >= 17 && byHorse <= 19) || (byHorse >= 33 && byHorse <= 35))
	//	return true;

	//return false;
	//Ã»ÓÐÐ¡±ª×Ó,Ð¡Ê¨×ÓµÈ²»ÄÜÆïµÄÁéÊÞÁË
	return IsLeopard(wHorse);
}

bool IsLeopard( WORD wHorse )
{
	//if((wHorse >= 6 && wHorse <= 11) || (wHorse >= 24 && wHorse <= 29) || (wHorse >= 14 && wHorse <= 19) || (wHorse >= 30 && wHorse <= 35))
	//	return true;

	if((wHorse >= 166 && wHorse <= 168) //±ª×Ó
		|| (wHorse >= 185 && wHorse <= 187)//Ê¨×Ó
		|| (wHorse >= 39 && wHorse <= 41) //÷è÷ë
		|| (wHorse >= 301 && wHorse <= 303)//·ï»Ë
		|| (wHorse == 373)
		|| (wHorse >= 452 && wHorse <= 477) )//ÀÏ»¢
		return true;

	return false;
}

bool IsBigPhenix( WORD wHorse )
{
	//if(wHorse >= 33 && wHorse <= 35)
	//	return  true;

	return IsPhenix(wHorse);
}

bool IsPhenix( WORD wHorse )
{
	if((wHorse >= 301 && wHorse <= 302)
		|| (wHorse >= 462 && wHorse <= 464)
		|| (wHorse >= 475 && wHorse <= 477))
		return  true;

	return false;
}

void GetArmCloseIDByPetType( int& iArmId,int& iCloseId,WORD wHorse,int iSex )
{
	iArmId = iCloseId = 0;

	if(iSex == 0)
	{
		iCloseId = 896;
	}
	else
	{
		iCloseId = 898;
	}

	if (wHorse >= 166 && wHorse <= 168)//±ª×Ó
	{
		iArmId = 887;
	}	
	else if (wHorse >= 185 && wHorse <= 187)//Ê¨×Ó
	{
		iArmId = 890; 
	}
	else if (wHorse >= 39 && wHorse <= 41)//÷è÷ë
	{
		iArmId = 893;
	}
	else if (wHorse == 301)//»Æ·ï»Ë
	{
		iArmId = 919;

		if(iSex == 0)		//ÓùÇÝÌìÒÂ×ø×Ë
		{
			iCloseId = 923;		
		}
		else
		{
			iCloseId = 932;
		}
	}
	else if(wHorse == 302)		//×Ï·ï»Ë
	{
		iArmId = 928;

		if(iSex == 0)		//ÓùÇÝÌìÒÂ×ø×Ë
		{
			iCloseId = 923;
		}
		else
		{
			iCloseId = 932;
		}
	}	

	//iArmId = iCloseId = 0;

	//if(iSex == 0)
	//{
	//	iCloseId = 896;
	//}
	//else
	//{
	//	iCloseId = 898;
	//}

	//if(wHorse >= 6 && wHorse <= 11) //±ª×Ó
	//{
	//	iArmId = 887;
	//}
	//else if(wHorse >= 24 && wHorse <= 29) //Ê¨
	//{
	//	iArmId = 890;                    
	//}
	//else if(wHorse >= 14 && wHorse <= 19)		//÷è÷ë
	//{
	//	iArmId = 893;
	//}	
	//else if(wHorse == 33)		//»Æ·ï»Ë
	//{
	//	iArmId = 919;

	//	if(iSex == 0)		//ÓùÇÝÌìÒÂ×ø×Ë
	//	{
	//		iCloseId = 923;		
	//	}
	//	else
	//	{
	//		iCloseId = 932;
	//	}
	//}
	//else if(wHorse == 34)		//×Ï·ï»Ë
	//{
	//	iArmId = 928;

	//	if(iSex == 0)		//ÓùÇÝÌìÒÂ×ø×Ë
	//	{
	//		iCloseId = 923;
	//	}
	//	else
	//	{
	//		iCloseId = 932;
	//	}
	//}      
}
