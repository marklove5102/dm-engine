#include "AIGoodMgr.h"
#include "GameData/GameData.h"
#include "GameMap/GameMap.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/Control.h"
#include "GameData/TalkMgr.h"
#include <bitset>
#include "BaseClass/TiXml/tinyxml.h"

CAIGoodMgr g_AIGoodMgr;

CAIGoodMgr::CAIGoodMgr(void)
{
	//LoadSoulLevelUpExp();	
}

CAIGoodMgr::~CAIGoodMgr(void)
{
}

bool CAIGoodMgr::LoadSoulLevelUpExp()
{
	char path[MAX_PATH] = {0};
    sprintf(path,"%s\\config\\SoulLevelUpExpCfg.xml",GetGameDataDir());

	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(path)) //读取失败
		return false;

	char str[64] = {0};

	m_MSoulLevelUpExp.clear();

	TiXmlElement* rootNode = xmlDoc.RootElement();
	TiXmlElement* node = rootNode->FirstChildElement();


	for(;node;node = node->NextSiblingElement())
	{
		if(stricmp(node->Value(),"LevelUpExp") == 0)
		{
			TiXmlElement* child = node->FirstChildElement();
			for(;child;child = child->NextSiblingElement())
			{
				if(stricmp(child->Value(),"level") == 0)
				{
					SoulLevelUpExp expCfg;
					char   *stopstring = NULL;
					int iLevel = 0;
					int iAddAtrrItems = 0;
					int iSoulLevel = -1;
					for(TiXmlAttribute* attr = child->FirstAttribute();attr;attr = attr->Next())
					{
						if(stricmp(attr->Name(),"value") == 0)
						{
							iLevel = attr->IntValue();
						}
						else 
						{
							for (int i = 0; i < MAX_EQUIP_SOUL_LEVEL; i++)
							{
								sprintf(str,"exp%d",i+1);
								if(stricmp(attr->Name(),str) == 0)
								{
									expCfg.dwExp[i] = strtoul(attr->Value(),&stopstring,10);
									iAddAtrrItems ++;
									break;
								}
							}
						}
					}

					if (iLevel > 0)
					{
						expCfg.iAddAtrrItems = iAddAtrrItems;
						m_MSoulLevelUpExp[iLevel] = expCfg;
					}
				}
			}
		}
		else if(stricmp(node->Value(),"Equip") == 0)
		{

			TiXmlElement* child = node->FirstChildElement();
			for(;child;child = child->NextSiblingElement())
			{
				SoulEquipAttr sEattr;
				if(stricmp(child->Value(),"goods") == 0)
				{
					string szName = "";
					for(TiXmlAttribute* attr = child->FirstAttribute();attr;attr = attr->Next())
					{
						if(stricmp(attr->Name(),"name") == 0)
						{
							szName = attr->Value();
						}
						else 
						{
							for (int i = 0; i < MAX_EQUIP_SOUL_LEVEL; i++)
							{
								sprintf(str,"level%d",i+1);
								if(stricmp(attr->Name(),str) == 0)
								{
									sEattr.addAttr[i] = attr->Value();
									break;
								}
							}
						}
					}

					if (!szName.empty())
					{
						m_MSoulEquipAttr[szName] = sEattr;
					}
				}
			}
		}




	}

	return true;
}

DWORD CAIGoodMgr::GetSoulLevelUpExp(CGood& tmp,int &iMaxAddAtrrItems,int iLevel)
{
	int iEquipLevel = GetLevel(tmp);
	if(iLevel < 0)
		iLevel = tmp.GetSoulLevel();

	iMaxAddAtrrItems = 0;
	if (iLevel < 0 || iLevel > MAX_EQUIP_SOUL_LEVEL)
	{
		return 0;
	}

	map<int,SoulLevelUpExp>::iterator itr = m_MSoulLevelUpExp.find(iEquipLevel);
	if (itr != m_MSoulLevelUpExp.end())
	{
		iMaxAddAtrrItems = itr->second.iAddAtrrItems;
		if (iLevel + 1 > iMaxAddAtrrItems)//这个装备最多只能升iMaxAddAtrrItems级,每升一级有一项属性增加
		{
			return 0;
		}

		return itr->second.dwExp[iLevel];
	}

	return 0;
}

string CAIGoodMgr::GetSoulEquipAttr(CGood& tmp,int iLevel)
{
	map<string,SoulEquipAttr>::iterator itr = m_MSoulEquipAttr.find(tmp.GetName());
	if (itr != m_MSoulEquipAttr.end() && iLevel < MAX_EQUIP_SOUL_LEVEL)
	{
		return itr->second.addAttr[iLevel];
	}

	return "";
}

UINT CAIGoodMgr::GetObjectAttr(CGood& tmp)
{
	UINT uType = 0;

	if(tmp.GetID() == 0)
		return 0;

	return GetObjectAttrByLooks(tmp.GetLooks());
}

UINT CAIGoodMgr::GetObjectAttrByLooks(WORD wGoodLooks)
{
	UINT uType = 0;

	switch(wGoodLooks)
	{
	case 168:	// 红(小)
		uType = 0x000101;
		break;
	case 169:	// 红(中)
		uType = 0x000102;
		break;
	case 170:	// 红(强)
		uType = 0x000103;
		break;
	case 733:	// 特级金创药
		uType = 0x000104;
		break;
	case 1325:	// 强效金创药
		uType = 0x000105;
		break;
	case 175:	// 蓝(小)
		uType = 0x000201;
		break;
	case 176:	// 蓝(中)
		uType = 0x000202;
		break;
	case 177:	// 蓝(强)
		uType = 0x000203;
		break;
	case 734:	// 特级魔法药(强)
		uType = 0x000204;
		break;
	case 1326:	// 强效魔法药
		uType = 0x000205;
		break;
	case 180:	// 太阳水
		uType = 0x000301;
		break;
	case 223:	// 强效太阳水
		uType = 0x000302;
		break;
	case 174:	// 疗伤药
		uType = 0x000401;
		break;
	case 244:	// 鹿茸
		uType = 0x000501;
		break;
	case 185:	// 随机传送卷
		uType = 0x000701;
		break;
	case 184:	// 地牢逃脱卷
		uType = 0x000601;
		break;
	case 183:	// 回城卷
		uType = 0x000801;
		break;
	case 179:	// 祝福油
		uType = 0x000901;
		break;
	case 186:	// 行会回城卷
		uType = 0x000A01;
		break;
	case 737:   // 灵符
		uType = 0x000B01;
		break;
	case 891:   // 灵符（捆）
		uType = 0x000B02;
		break;
	case 981:
		uType = 0x000C01;
		break;
	case 980:
	case 982:
		uType = 0x000C02;
		break;
	case 930:   //归元丹,归元神丹
		uType = 0x000D01;
		break;
	case 1220:	// 灵兽项圈
		uType = 0x000E01;
		break;
	case 1221:	// 灵兽项圈捆
		uType = 0x000E02;
		break;
	case 751:   //强化技能经验丹
		uType = 0x000F01;
		break;
	case 1227:   // 迷仙路引
		uType = 0x001001;
		break;
	case 856:   // 治疗神水
		uType = 0x001101;
		break;
	case 793:   // 超级天山雪莲
		uType = 0x001201;
		break;
	case 794:   // 超级深海灵礁
		uType = 0x001301;
		break;
	case 1513:   //普通妖翎羽
	case 1514:   //完美妖翎羽
		uType = 0x001401;
		break;
	case 1511:   //轰天雷
	case 1512:   //霸极轰天雷
		uType = 0x001501;
		break;

	case 979:
	case 235:	// 金创药（小）包
		uType = 0x000111;
		break;
	case 237:	// 金创药（中）包
		uType = 0x000112;
		break;
	case 171:	// 超级金创药
		uType = 0x000113;
		break;
	case 731:	// 特级金创药包
		uType = 0x000114;
		break;
	case 1327:	// 强效金创药包
		uType = 0x000115;
		break;
	case 236:	// 魔法药（小）包
		uType = 0x000211;
		break;
	case 238:	// 魔法药（中）包
		uType = 0x000212;
		break;
	case 178:	// 超级魔法药
		uType = 0x000213;
		break;
	case 732:	// 特级魔法药包
		uType = 0x000214;
		break;
	case 1328:	// 强效魔法药包
		uType = 0x000215;
		break;
	case 825:   // 灵符包
		uType = 0x010B01;
		break;
	case 241:	// 随机传送卷包
		uType = 0x010701;
		break;
	case 240:   // 地牢逃脱卷包
		uType = 0x010601;
		break;
	case 5250:   // 炼火葫芦
		uType = 0x010801;
		break;
	default:	// unknown
		uType = 0;
		break;
	}
	return uType;
}


bool CAIGoodMgr::IsNotAuth(CGood& tmp) //没有鉴定的靴子和腰带等
{
	return (tmp.GetStdMode() == 82 || tmp.GetStdMode() == 83);
}

bool CAIGoodMgr::IsBackHome(CGood& tmp)	//是否是回城石
{
	return (tmp.GetStdMode() ==3 && (tmp.GetShape() ==15 || tmp.GetShape() ==17));
}

bool CAIGoodMgr::IsReliveStone(CGood& tmp)	//是否是还魂石
{
	return (tmp.GetStdMode() ==3 && tmp.GetShape() == 18);
}

bool CAIGoodMgr::IsReliveSymbol(CGood& tmp) //是否还魂神符
{
	return (tmp.GetStdMode() ==3 && tmp.GetShape() == 18);
}

bool CAIGoodMgr::IsRiderClothes(CGood& tmp)
{
	return ((tmp.GetStdMode() == 37 && tmp.GetShape() == 11) || (tmp.GetStdMode() == 49 && tmp.GetShape() == 22));
}

bool CAIGoodMgr::IsLeopardStone(CGood& tmp)
{
	return (tmp.GetStdMode() == 49 && tmp.GetShape() == 51);
}

bool CAIGoodMgr::IsCanLock(CGood& tmp)
{
	int iStdMode = tmp.GetStdMode();

	if(iStdMode == 18 || //[20051128][超级技能项链]
		iStdMode == 29 || //灵狗神佑
		iStdMode == 33//马牌
		|| iStdMode == 49//马票
		|| IsNotAuth(tmp)	//鉴定类物品未鉴定
		|| iStdMode == 61	//合成宝石
		|| IsBackHome(tmp)	//回城石
		|| iStdMode == 38	//存在时间(创建时间、有效期)的物品
		|| iStdMode == 9
		|| IsReliveStone(tmp)	//还魂石
		|| iStdMode == 70	//[元神]
		|| iStdMode == 72	//[附加属性保存的信息（前10个字节）需要显示的特殊道具：天缘玉佩，师徒心锁]
		|| IsRiderClothes(tmp))//特殊衣服(御兽天袍/御兽天衣)
	{
		return false;
	}

	if (iStdMode != 4	&& iStdMode != 5	&&
		iStdMode != 6	&& iStdMode != 10	&&
		iStdMode != 11	&& iStdMode != 15	&&
		iStdMode != 26	&& iStdMode != 58	&&
		iStdMode != 59	&& iStdMode != 30	&&
		iStdMode != 61	&& iStdMode != 70	&&
		!(iStdMode > 18 && iStdMode < 25)	&&
		!(iStdMode > 80 && iStdMode < 84))
	{
		return false;
	}

	return true;
}

bool CAIGoodMgr::IsCanLettering(CGood& tmp)  //是否可刻字
{
	int iStdMode = tmp.GetStdMode();

	if(iStdMode == 5 || iStdMode == 6	||//武器
		iStdMode == 10 ||	iStdMode == 11 ||//衣服
		iStdMode == 15 ||//头盔
		iStdMode == 26 ||//手套手镯护腕
		iStdMode == 30 ||//勋章、神饰、灵配
		(iStdMode == 33 && tmp.GetShape() == 4) ||//黄金宝马
		iStdMode == 58 ||//腰带
		iStdMode == 59 || iStdMode == 61 || //宝石、天地宝石
		(iStdMode > 18 && iStdMode < 25) || //项链、戒指、手镯
		(iStdMode > 80 && iStdMode < 84)) //靴子、靴子(未鉴定)、腰带(未鉴定)
	{
		return true;
	}
	return false;
}

bool CAIGoodMgr::IsCanUseByDClick(CGood& tmp)
{
	int iStdMode = tmp.GetStdMode();
	int iShape = tmp.GetShape();

	if(tmp.GetID() != 0 && (iStdMode < 7 || iStdMode == 53
		|| iStdMode == 31 || iStdMode == 56 || iStdMode == 32
		|| iStdMode == 35 || iStdMode == 36 || iStdMode == 49
		|| iStdMode == 55 || iStdMode == 82 || iStdMode == 83
		|| iStdMode == 71 || iStdMode == 70 || iStdMode == 29
		|| iStdMode == 37 || iStdMode == 46 || iStdMode == 15//使用药品传送卷类等物品
		|| iStdMode == 18 || iStdMode == 19 || iStdMode == 20//补充头盔，项链，勋章，腰带，靴子，宝石，手镯，戒指
		|| iStdMode == 21 || iStdMode == 30 || iStdMode == 33
		|| iStdMode == 38 || iStdMode == 81 || iStdMode == 59
		|| iStdMode == 60 || iStdMode == 61 || iStdMode == 24
		|| iStdMode == 26 || iStdMode == 22 || iStdMode == 23
		//|| iStdMode == 14//群英阵谱
		|| (iStdMode == 38 && iShape == 3) //离火令
		|| iStdMode == 15//使用药品传送卷类等物品
		|| iStdMode == 18 || iStdMode == 19 || iStdMode == 20//补充头盔，项链，勋章，靴子，宝石，手镯，戒指
		|| iStdMode == 21 || iStdMode == 30 || iStdMode == 33
		|| iStdMode == 38 || iStdMode == 81 || iStdMode == 59
		|| iStdMode == 60 || iStdMode == 61 || iStdMode == 24
		|| iStdMode == 26 || iStdMode == 22 || iStdMode == 23
		|| iStdMode == 10 || iStdMode == 11 || iStdMode == 58//衣服，腰带
		|| (iStdMode == 46 && iShape == 6)//赠送元宝，活动道具等
		|| iStdMode == 96//妖翎羽
		|| (iStdMode == 98 && iShape == 0)//经验宝玉
		|| (iStdMode == 57 && iShape >= 6 && iShape <= 10)//神佑
		|| (iStdMode == 100 && iShape >= 1 && iShape <= 4)//纹佩部件
		|| (iStdMode == 99 && iShape == 0)//火凝丹
		|| iStdMode == 66//妖胎
		|| (iStdMode == 69 && iShape == 0)//骑兽笼
		|| iStdMode == 67//骑术经验书
		|| iStdMode == 109//骑术行动卡
		))					
	{
		return true;
	}

	return false;
}

bool CAIGoodMgr::IsLingfu(CGood& tmp)
{
	int iShape = tmp.GetShape();
	if(tmp.GetStdMode() == 3 && (iShape == 202 || iShape == 203 || iShape == 205 || iShape == 27 || iShape == 29))
		return true;

	return false;
}

bool CAIGoodMgr::IsLinkGood(CGood& tmp)
{
	int iStdMode = tmp.GetStdMode();
	int iShape = tmp.GetShape();

	if(iStdMode == 37 && iShape == 202)
	{
		return true;
	}//乾坤锁、钻石刀、命运石/银钥匙/金钥匙
	else if(iStdMode == 37 && (iShape == 203 || iShape == 206 ||iShape == 207 || iShape == 208 || iShape == 209) )
	{
		return true;
	}
	else if((iStdMode == 53 && iShape == 0)|| (iStdMode == 49 && iShape == 52))
	{
		return true;
	}
	return false;
}

bool CAIGoodMgr::FilterUseGood(CGood& tmp)
{
	std::string  strName = "灵符(捆)";
	bool bUseLingfu = false,bInMap = false;
	if(strName.compare(tmp.GetName()) == 0)
	{
		bUseLingfu = true;
	}
	else
	{
		strName = "灵符";
		if(strName.compare(tmp.GetName()) == 0) bUseLingfu = true;	
        else 
        {
            strName = "灵符(袋)";
            if(strName.compare(tmp.GetName()) == 0) bUseLingfu = true;
        }
	}

	if(bUseLingfu)
	{		
		//是否在天关，恶魔城，神威狱中
		if(strnicmp(g_pGameMap->GetMapName(),"SWY",3) == 0)			bInMap = true;
		else if(strnicmp(g_pGameMap->GetMapName(),"RTG",3) == 0)	bInMap = true;
		else if(strnicmp(g_pGameMap->GetMapName(),"NTG",3) == 0)	bInMap = true;
	}

	if(bUseLingfu && !bInMap)
	{
		g_TalkMgr.PushSysTalk("灵符只能在天关、恶魔城、神威狱中使用!");
		return true;
	}


	if(strcmp(tmp.GetName(),"飞来神石") == 0)
	{
	}
	else if(strcmp(tmp.GetName(),"回城神石") == 0 || strcmp(tmp.GetName(),"回城神石(大)") == 0)
	{
		char m[8] = {0};
		m[0] = tmp.GetDC();
		m[1] = tmp.GetDC2();
		m[2] = tmp.GetMC();
		m[3] = tmp.GetMC2();
		m[4] = tmp.GetSC();
		m[5] = tmp.GetSC2();
	}
	else if(tmp.GetStdMode() == 3 && tmp.GetShape() == 28)//用还魂神符复活别人
	{
		if(SELF.IsDead())
		{
			g_pGameControl->SEND_Use_ReliveItem(SELF.GetID(),tmp.GetID());
		}
		else
		{
			CSimpleCharacterNode* p = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
			if(p && p->IsDead() )
			{
				g_pGameControl->SEND_Use_ReliveItem(p->GetID(),tmp.GetID());
			}
		}

		return true;
	}	

	return false;
}


bool CAIGoodMgr::IsValidPoisonCurse(CGood& tmp,bool bNeedGreen)
{
	if(tmp.GetID() == 0)
		return false;

	if(tmp.GetStdMode() != 25)
		return false;

	if(bNeedGreen)
	{
		if(tmp.GetShape() == 1)
			return true;
	}
	else
	{
		if(tmp.GetShape() == 2)
			return true;
	}
	return false;
}

bool CAIGoodMgr::IsValidPoisonCurse(CGood& tmp,bool bNeedGreen,bool bNeedPoison)
{
	if(!IsValidPoisonCurse(tmp,bNeedGreen))
		return false;

	int iLooks = tmp.GetLooks();
	if(bNeedPoison)
	{
		if(iLooks < 270 || iLooks == 834 || iLooks == 835)
			return true;
	}
	else
	{
		if(iLooks >= 270 && iLooks <= 275)
			return true;
	}
	return false;
}

bool CAIGoodMgr::IsValidDaoFu(CGood& tmp)
{
	if(tmp.GetID() == 0)
		return false;

	if((tmp.GetStdMode() == 34 || tmp.GetStdMode() == 25) && tmp.GetShape() == 5)
		return true;

	return false;
}


bool CAIGoodMgr::IsCanContain(CGood& tmp,int pos)
{
	int mode = tmp.GetStdMode();
	switch(pos)
	{
	case ITEM_POS_CLOTH:		// 衣服
		if(mode == 10 || mode == 11)
			return true;
		break;
	case ITEM_POS_WEAPON:		// 武器
		if(mode == 5 || mode == 6)
			return true;
		break;
	case ITEM_POS_MEDAL:		// 勋章 
		if(mode == 30 || mode == 33 || mode == 29)
			return true;
		break;
	case ITEM_POS_NECKLACE:		// 项链
		if(mode == 18 || mode == 19 || mode == 20 || mode == 21)
			return true;
		break;
	case ITEM_POS_HELMET:		// 头盔
		if(mode == 15)
			return true;
		break;
	case ITEM_POS_RIGHT_BANGLE:		// 手镯,毒粉
		if(mode == 24 || mode == 25 || mode == 26 || mode == 34)
			return true;
		break;
	case ITEM_POS_LEFT_BANGLE:		// 手镯
		if(mode == 24 || mode == 26)
			return true;
		break;
	case ITEM_POS_RIGHT_RING:		// 戒指
	case ITEM_POS_LEFT_RING:
		if(mode == 22 || mode == 23)
			return true;
		break;

	case ITEM_POS_CLOTH_BOOT:  // 鞋子
		if(SELF.GetLevel() < 22)
			return false;

		if(mode == 81)
			return true;
		break;
	case ITEM_POS_BELT: // 腰带 

		if(SELF.GetLevel() < 22)
			return false;

		if(mode == 58)
			return true;
		break;
	case ITEM_POS_GEM: // 宝石
		if(SELF.GetLevel() < 22)
			return false;

		if(mode == 59 || mode == 60 || mode == 61) 
			return true;
		break;
	case ITEM_POS_OTHERS: // 杂物
		if(SELF.GetLevel() < 22)
			return false;

		if(mode == 25 || mode == 34 || mode == 33 || mode == 29 || mode == 73)
			return true;
		break;
	case ITEM_POS_SHIELD:
		if(mode == 12)
			return true;
		break;
	//case ITEM_TNEUPCHART://群英阵谱
	//	if (mode == 14)
	//		return true;
	//	break;
	case ITEM_POS_WENPEI:
		if (IsWenPei(tmp))
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool CAIGoodMgr::IsCanKeZi(CGood & temp)  //是否可刻字
{
	if(temp.GetStdMode() == 5 ||	temp.GetStdMode() == 6	||//武器
		temp.GetStdMode() == 10 ||	temp.GetStdMode() == 11 ||//衣服
		temp.GetStdMode() == 15 ||//头盔
		temp.GetStdMode() == 26 ||//手套手镯护腕
		temp.GetStdMode() == 30 ||//勋章、神饰、灵配
		(temp.GetStdMode() == 33 && temp.GetShape() == 4) ||//黄金宝马
		temp.GetStdMode() == 58 ||//腰带
		temp.GetStdMode() == 59 || temp.GetStdMode() == 61 || //宝石、天地宝石
		(temp.GetStdMode() > 18 && temp.GetStdMode() < 25) || //项链、戒指、手镯
		(temp.GetStdMode() > 80 && temp.GetStdMode() < 84) //靴子、靴子(未鉴定)、腰带(未鉴定)
		)
		return true;

	return false;
}

bool CAIGoodMgr::IsMiXianLuYing(CGood& tmp)
{
	if(tmp.GetStdMode() == 3 && tmp.GetShape() == 209)
		return true;

	return false;
}

bool CAIGoodMgr::IsLingShouXiangQuan(CGood& tmp)
{
	if(tmp.GetStdMode() == 3 && (tmp.GetShape() == 206 || tmp.GetShape() == 207))
		return true;

	return false;
}

bool CAIGoodMgr::IsCanZhang(CGood& tmp)
{
	if((tmp.GetStdMode() == 37) && ( (tmp.GetShape() == 222) || (tmp.GetShape() == 223) || (tmp.GetShape() == 224) ))
		return true;

	return false;
}

bool CAIGoodMgr::IsFitBelt(CGood & temp)
{
	if(temp.GetStdMode() < 4)
	{
		return true;
	}

	if(temp.GetStdMode() == 96)//妖翎羽
	{
		return true;
	}
	else if(temp.GetStdMode() == 49 && temp.GetShape() == 83)	//图腾
	{
		return true;
	}
	if(temp.GetStdMode() == 46 && temp.GetShape() == 13)	//修罗圣火
	{
		return true;
	}

	//else if(temp.GetStdMode() == 52 && temp.GetShape() == 157)	//图腾
	//{
	//	return true;
	//}

	return false;
}

int CAIGoodMgr::GetYuanShiLooks( int iShape,int iLevel )
{	
	if(iLevel == 0)
	{
		iLevel = 1;
	}

	if(iLevel >= 6)
	{
		return 5114 + (iShape - 225);
	}
	else
	{
		return 5114 + (iShape - 225) + iLevel * 10;
	}
}

bool CAIGoodMgr::IsYuanShi( CGood& tmp )
{
	if(tmp.GetStdMode() == 37 && tmp.GetShape() >= 225 && tmp.GetShape() <= 234)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CAIGoodMgr::GetLevel( CGood& tmp )
{
	return HIBYTE(tmp.GetDemonDark3());
}

int CAIGoodMgr::GetCanDigHoles( CGood& tmp )
{
	return (tmp.GetDemonDark3() & 0x00F0) >> 4;//5-8表示洞的个数
}

int CAIGoodMgr::GetDigHole( CGood& tmp )
{
	//前面5个孔的类型放在LOWORD(FocusGood.GetDemonDark1());
	WORD wHoles = LOWORD(tmp.GetDemonDark1());
	int iHoleNum = 0;

	for(int i = 0;i < 5;i++)
	{
		if(wHoles & 0x7)
		{
			iHoleNum++;
		}

		wHoles >>= 3;
	}
	//后面5个也的类型放在GetResvEx3(0),GetResvEx3(1)
	wHoles = (WORD(tmp.GetResvEx3(0))) | (WORD(tmp.GetResvEx3(1)) << 8);
	for(int i = 5;i < 10;i++)
	{
		if(wHoles & 0x7)
		{
			iHoleNum++;
		}

		wHoles >>= 3;
	}		

	return iHoleNum;
}

bool CAIGoodMgr::IsCanDigHoleEquip( CGood& tmp )
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();

	if (iStdMode == 10 || iStdMode == 11 || iStdMode == 5 || iStdMode == 6 ||
		iStdMode == 18 || iStdMode == 20 || iStdMode == 21 ||
		iStdMode == 15 || iStdMode == 24 || iStdMode == 26 || iStdMode == 22 ||
		iStdMode == 23 || iStdMode == 81 || iStdMode == 58 ||
		iStdMode == 59 || iStdMode == 60 || iStdMode == 61 ||
		iStdMode == 30 || iStdMode == 33 || iStdMode == 19 ||//勋章
		iStdMode == 12//盾牌
		)
	{
		return true;
	}

	return false;
}
//是否装备
bool CAIGoodMgr::IsEquip(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	int iShape = tmp.GetShape();

	if (iStdMode == 10 || iStdMode == 11 || iStdMode == 5 || iStdMode == 6 ||
		iStdMode == 18 || iStdMode == 19 || iStdMode == 20 || iStdMode == 21 ||
		iStdMode == 15 || iStdMode == 24 || iStdMode == 26 || iStdMode == 22 ||
		iStdMode == 23 || iStdMode == 81 || iStdMode == 58 ||
		iStdMode == 30 || iStdMode == 33 || iStdMode == 29 ||
		iStdMode == 59 /*|| iStdMode == 60*/ || iStdMode == 61 || iStdMode == 12 || 
		(iStdMode == 104 && iShape != 1)
		)
	{
		return true;
	}

	return false;
}
//武器
bool CAIGoodMgr::IsEquipWeapon(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 5 || iStdMode == 6)
	{
		return true;
	}

	return false;
}
//衣服
bool CAIGoodMgr::IsEquipCloth(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 10 || iStdMode == 11)
	{
		return true;
	}

	return false;
}
////群英阵谱
//bool CAIGoodMgr::IsEquipTneupChart(CGood& tmp)
//{
// 	if (tmp.GetID() == 0)
// 	{
// 		return false;
// 	}
//	int iStdMode = tmp.GetStdMode();
//	if(iStdMode == 14)
//	{
//		return true;
//	}
//
//	return false;
//}
//头盔
bool CAIGoodMgr::IsEquipHelmet(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 15)
	{
		return true;
	}

	return false;
}
//项链
bool CAIGoodMgr::IsEquipNecklace(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 18 || iStdMode == 19 || iStdMode == 20 || iStdMode == 21)
	{
		return true;
	}

	return false;
}
//勋章
bool CAIGoodMgr::IsEquipMedal(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 30 || iStdMode == 33 || iStdMode == 19)
	{
		return true;
	}

	return false;
}
//腰带
bool CAIGoodMgr::IsEquipBelt(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 58)
	{
		return true;
	}

	return false;
}
//靴子
bool CAIGoodMgr::IsEquipBoot(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 81)
	{
		return true;
	}

	return false;
}
//宝石
bool CAIGoodMgr::IsEquipGem(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 59 /*|| iStdMode == 60*/ || iStdMode == 61)
	{
		return true;
	}

	return false;
}
//手镯
bool CAIGoodMgr::IsEquipBangle(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 24 || iStdMode == 26)
	{
		return true;
	}

	return false;
}
//戒指
bool CAIGoodMgr::IsEquipRing(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 22 || iStdMode == 23)
	{
		return true;
	}

	return false;
}
//盾牌
bool CAIGoodMgr::IsEquipShield(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}
	int iStdMode = tmp.GetStdMode();
	if(iStdMode == 12)
	{
		return true;
	}

	return false;
}
//获得这个物品可以装备到装备栏的位置
int  CAIGoodMgr::GetEquipPos(CGood& tmp,bool bLeft)
{
	if(g_AIGoodMgr.IsEquipWeapon(tmp))//刀剑等装备物品
	{
		return ITEM_POS_WEAPON;
	}
	else if(g_AIGoodMgr.IsEquipCloth(tmp))//衣服
	{
		return ITEM_POS_CLOTH;
	}
	//else if(g_AIGoodMgr.IsEquipTneupChart(tmp))//群英阵谱
	//{
	//	return ITEM_TNEUPCHART;
	//}
	else if(g_AIGoodMgr.IsEquipHelmet(tmp))//头盔
	{
		return ITEM_POS_HELMET;
	}
	else if(g_AIGoodMgr.IsEquipNecklace(tmp))//项链
	{
		return ITEM_POS_NECKLACE;
	}
	else if(g_AIGoodMgr.IsEquipMedal(tmp))//勋章
	{
		return ITEM_POS_MEDAL;
	}
	else if(g_AIGoodMgr.IsEquipBelt(tmp))//腰带
	{
		return ITEM_POS_BELT;
	}
	else if(g_AIGoodMgr.IsEquipBoot(tmp))//靴子
	{
		return ITEM_POS_CLOTH_BOOT;
	}
	else if(g_AIGoodMgr.IsEquipGem(tmp) )//宝石
	{
		return ITEM_POS_GEM;
	}
	else if(g_AIGoodMgr.IsEquipBangle(tmp))//手镯
	{
		if (bLeft)
		{
			return ITEM_POS_LEFT_BANGLE;
		}
		else
		{
			return ITEM_POS_RIGHT_BANGLE;
		}
	}
	else if(g_AIGoodMgr.IsEquipRing(tmp))//戒指
	{
		if (bLeft)
		{
			return ITEM_POS_LEFT_RING;
		}
		else
		{
			return ITEM_POS_RIGHT_RING;
		}
	}
	else if (g_AIGoodMgr.IsEquipShield(tmp))//盾牌
	{
		return ITEM_POS_SHIELD;
	}
	else if (g_AIGoodMgr.IsWenPei(tmp))
	{
		return ITEM_POS_WENPEI;
	}
	else if (g_AIGoodMgr.IsFaBao(tmp))
	{
		return ITEM_POS_FABAO;
	}

	return -1;
}
//是不是器魂结晶
bool CAIGoodMgr::IsEquipSoulCrystal(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	// 判断物品是否是器魂结晶
	//所有结晶，stdmode = 93
	//	shape
	//	头盔 1
	//	项链 2
	//	手镯 3
	//	戒指 4
	//	衣服 5
	//	腰带 6
	//	靴子 7
	//	武器 8
	//	完美 0

	return (tmp.GetStdMode() == 93 && tmp.GetShape() <= 8);

}

bool CAIGoodMgr::IsCanLevelUpEquip(CGood& tmp)
{
	if (!IsEquip(tmp))
	{
		return false;
	}

	if (GetLevel(tmp) == 0)
	{
		return false;
	}

	return true;
}

bool CAIGoodMgr::IsFuShi(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	int iShape = tmp.GetShape();

	if(iStdMode == 89 && iShape >= 0 && iShape <= 30)
	{
		return true;
	}

	return false;
}

bool CAIGoodMgr::IsLingFuShi(CGood &tmp)
{
	if (!IsFuShi(tmp))
	{
		return false;
	}

	if ((tmp.GetShape() % 6) == 0 || (tmp.GetShape() % 6) == 3)
	{
		return true;
	}

	return false;
}

int CAIGoodMgr::GetFuShiAddAttack( int iKind,int iLevel )
{
	if(iLevel < 1 || iLevel > 10)
	{
		return 0;
	}		

	//int iAddAttackNormal[6] = {1,3,5,7,9,11};
	int iAddAttackNormal[10] = {1,2,3,5,7,10,14,18,22,26};

	//if(iKind == 1)	//尖符石
	{
		return iAddAttackNormal[iLevel - 1];
	}

	//int iAddAttackPure[6] = {2,4,6,8,10,12};

	//if(iKind == 4)	//纯净尖符石
	//{
	//	return iAddAttackPure[iLevel - 1];
	//}

	return 0;
}

int CAIGoodMgr::GetFuShiAddDefense( int iKind,int iLevel )
{
	if(iLevel < 1 || iLevel > 10)
	{
		return 0;
	}		

	int iAddAttackNormal[10] = {2,4,6,8,10,12,14,16,18,20};

	//if(iKind == 2)	//钢符石
	{
		return iAddAttackNormal[iLevel - 1];
	}

	//int iAddAttackPure[10] = {2,4,6,8,10,12,14,16,18,20};

	//if(iKind == 5)	//纯净钢符石
	//{
	//	return iAddAttackPure[iLevel - 1];
	//}

	return 0;
}

int CAIGoodMgr::GetFuShiAddHP( int iKind,int iLevel )
{
	if(iLevel < 1 || iLevel > 10)
	{
		return 0;
	}	

	int iAddHP[10] = {2,4,6,10,14,20,26,32,40,50};	

	//if(iKind == 5)	//纯净钢符石
	{
		return iAddHP[iLevel - 1];
	}

	return 0;
}

bool CAIGoodMgr::IsJiGuanYaoShi( CGood& tmp )
{
	if(tmp.GetStdMode() == 3 && tmp.GetShape() == 214)
		return true;

	return false;
}

string CAIGoodMgr::GetWingNameByLevel(int level)
{
	static const string s_strWingName[6] = {
		"白羽之翼",	
		"蓝魔之翼",
		"弑神之翼",
		"逆影之翼",
		"破灭之翼",
		"王者之翼",
	};

	int iWingIndex = level / 10;

	if (iWingIndex >= 0 && iWingIndex < 6 )
	{
		return s_strWingName[iWingIndex];
	}
	return "";
}

DWORD CAIGoodMgr::GetWingColorByLevel(int level)
{
	static const DWORD s_wingColor[6] = {
		0xFFFFFFFF,	
		0xFF00B0F0,
		0xFFA933BD,
		0xFFFF8000,
		0xFFFF0000,
		0xFFFFFF00,		
	};

	int iWingIndex = level / 10;

	if (iWingIndex >= 0 && iWingIndex < 6 )
	{
		return s_wingColor[iWingIndex];
	}
	return -1;
}

DWORD CAIGoodMgr::GetAddTogetherEffTexID(CGood& src,CGood& dest)
{
	int iStdMode = src.GetStdMode();
	int iShape = src.GetShape();

	//规定能合并的道具,活动道具及封元神力果,器魂结晶,矿石,行会宝石,非自动叠加可手动叠加收集类道具(11,12)
	if( ( (iStdMode == 46 && (iShape == 5 || iShape == 6 || iShape == 10 || iShape == 11 || iShape == 12 || iShape == 13)) 
		|| (iStdMode == 3 && iShape == 215)//封元神力果 
		|| (iStdMode == 93 && iShape <= 8)//器魂结晶
		|| (iStdMode == 96)//妖翎羽
		|| (iStdMode == 43 && iShape == 0) || iStdMode == 48 || iStdMode == 63//矿石
		|| (iStdMode == 100 && iShape >= 1 && iShape <= 4)//纹佩配件
		|| (iStdMode == 102 && (iShape >= 1 && iShape <= 3))//纹佩精华
		|| iStdMode == 103//特殊收集类道具
		|| (iStdMode == 37 && iShape == 212)//圣殿精华
		)
		&& dest.GetStdMode() == iStdMode
		&& dest.GetShape() == iShape
		&& strcmp(src.GetName(),dest.GetName()) == 0)
	{
		return MAKELONG(1070,PACKAGE_stateitem);
	}

	if(g_AIGoodMgr.IsYuanShi(src) && g_AIGoodMgr.IsYuanShi(dest) && src.GetAC() == dest.GetAC() && iShape == dest.GetShape())
	{
		return MAKELONG(1070,PACKAGE_stateitem);
	}

	return 0;
}

bool CAIGoodMgr::IsWenPei( CGood& tmp )
{
	return (tmp.GetID() > 0 && tmp.GetStdMode() == 101);
}

bool CAIGoodMgr::IsSubWenPei(CGood& tmp)
{
	if (tmp.GetID() == 0)
		return false;

	int iStdMode = tmp.GetStdMode();
	int iShape = tmp.GetShape();

	return (iStdMode == 100 && iShape >= 1 && iShape <= 4);
}

bool CAIGoodMgr::IsShengLing(CGood& tmp)
{
	if (tmp.GetID() == 0)
	{
		return false;
	}

	int iStdMode = tmp.GetStdMode();
	int iShape = tmp.GetShape();

	if(iStdMode == 89 && iShape >= 31 && iShape <= 50)
	{
		return true;
	}

	return false;
}

bool CAIGoodMgr::IsJingShengLing(CGood& tmp)
{
	if(((tmp.GetShape() - 31) % 4) >= 2)
	{
		return true;
	}

	return false;
}

bool CAIGoodMgr::IsFaBao(CGood &tmp)
{
	return (tmp.GetID() > 0 && tmp.GetStdMode() == 104 && tmp.GetShape() != 1);
}

float  CAIGoodMgr::GetFaBaoQiHeDu(CGood &tmp)
{
	float fRtn = tmp.GetAC2() + 0.10f * tmp.GetResvEx3(14);
	return fRtn;
}

int  CAIGoodMgr::GetFabaoWuXing(CGood &tmp)
{
	return ((tmp.GetAC() & 0xF0) >> 4);
}

int  CAIGoodMgr::GetFabaoPinJie(CGood &tmp)
{
	return tmp.GetAC() & 0x0F;
}

int  CAIGoodMgr::GetFabaoMinAttack(CGood &tmp)
{
	return tmp.GetDC();
}

int  CAIGoodMgr::GetFabaoMaxAttack(CGood &tmp)
{
	return ((WORD)(tmp.GetDC2()) | (((WORD)tmp.GetResvEx3(15)) << 8));
}

int  CAIGoodMgr::GetFaBaoMinZhong(CGood &tmp)
{
	return tmp.GetMAC2();
}

int  CAIGoodMgr::GetFaBaoXingYun(CGood &tmp)
{
	return tmp.GetMC();
}

int  CAIGoodMgr::GetFaBaoBaoJi(CGood &tmp)
{
	return tmp.GetMC2();
}

int  CAIGoodMgr::GetFaBaoPoFang(CGood &tmp)
{
	return tmp.GetSC();
}

int  CAIGoodMgr::GetFaBaoWuXingAddAttack(CGood &tmp)
{
	return tmp.GetSC2();
}

int  CAIGoodMgr::GetFaBaoWuXingReduceDefence(CGood &tmp)
{
	return tmp.GetNeed();
}

int  CAIGoodMgr::GetFaBaoLingGenAddLowAttack(CGood &tmp)
{
	return (tmp.GetNeedLevel() & 0x0F);
}

int  CAIGoodMgr::GetFaBaoLingGenAddHighAttack(CGood &tmp)
{
	return ((tmp.GetNeedLevel() & 0xF0) >> 4);
}

int  CAIGoodMgr::GetFaBaoNeedFireLevel(CGood &tmp)
{
	return tmp.GetResvEx3(13);
}

int  CAIGoodMgr::GetFaBaoLucky(CGood &tmp)
{
	return tmp.GetMC();

}

/*
	药品>器魂>符石>装备
*/
int CAIGoodMgr::CompareInPackage(CGood* first, CGood* second)
{
	if (!first || !second)
	{
		return -1;
	}

	int firstType = 0;
	int secondType = 0;

	if (first->GetStdMode() <= 3)
	{
		firstType = 5;
	}
	else if (IsEquipSoulCrystal(*first))
	{
		firstType = 4;
	}
	else if (IsFuShi(*first))
	{
		firstType = 3;
	}
	else if (IsEquip(*first))
	{
		firstType = 2;
	}
	else
		firstType = 1;


	if (second->GetStdMode() <= 3)
	{
		secondType = 5;
	}
	else if (IsEquipSoulCrystal(*second))
	{
		secondType = 4;
	}
	else if (IsFuShi(*second))
	{
		secondType = 3;
	}
	else if (IsEquip(*second))
	{
		secondType = 2;
	}
	else
		secondType = 1;

	if (firstType > secondType)
	{
		return	1;
	}
	else if (secondType > firstType)
	{
		return -1;
	}
	
	return strcmp(second->GetName(),first->GetName());
	
}


bool CAIGoodMgr::IsLianHuLu(CGood &tmp)
{
	return (tmp.GetID() > 0 && tmp.GetStdMode() == 3 && tmp.GetShape() == 40);
}

//int CAIGoodMgr::TianDiRenByShape(int shape)
//{
//	switch (shape)
//	{
//	case 2:
//	case 5:
//	case 8:
//	case 11:
//		return 1;//天
//	case 3:
//	case 6:
//	case 9:
//	case 12:
//		return 2;//地
//	case 4:
//	case 7:
//	case 10:
//	case 13:
//		return 3;//人
//	}
//
//	return 0;
//}

bool CAIGoodMgr::IsChiLianHuFu(CGood &tmp)
{
	return (tmp.GetID() > 0 && tmp.GetStdMode() == 73);
}

bool CAIGoodMgr::IsQiShuXiuLianBook(CGood &tmp)
{
	return (tmp.GetID() > 0 && tmp.GetStdMode() == 46 && tmp.GetShape() == 1);
}

bool CAIGoodMgr::IsTianYouZhongZhou(CGood &tmp)
{
	return (tmp.GetID() > 0 && tmp.GetStdMode() == 69 && tmp.GetShape() >= 1 && tmp.GetShape() <= 4);
}

bool CAIGoodMgr::IsPetStone(CGood &tmp)
{
	return (tmp.GetID() > 0 && tmp.GetStdMode() == 69 && tmp.GetShape() == 0);
}
