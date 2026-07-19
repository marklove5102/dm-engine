#include "StdAfx.h"
#include "goodinfo.h"
#include "time.h"
//#include "../GameControl/GameMessage.h"
#include "GameData/MagicDefine.h"
#include <bitset>

CGoodInfo g_GoodInfo;  //物品解析的类

char* g_strFlyMask[MAX_FLY_LEVEL]={
	"   --",
	"天人一重",
	"天人二重",
	"天人三重",
	"天人四重",
	"天人五重",
	"天人六重",
	"天人七重",
	"天人八重",
	"天人九重",
	"天人十重",
	"天人十一重",
	"天人十二重",
	"天人十三重",
	"天人十四重",
	"天人十五重",
	"天人十六重",
	"天人十七重",
	"天人十八重",
	"天人十九重",
	"天人二十重",
	"天人二十一重",
	"天人二十二重",
	"天人二十三重",
	"天人二十四重",
	"天人二十五重",
	"天人二十六重",
	"天人二十七重",
	"天人二十八重",
	"天人二十九重",
	"天人三十重",
	"天人三十一重",
	"天人三十二重",
	"天人三十三重",
	"天人三十四重",
	"天人三十五重",
	"天人三十六重",
	"天人三十七重",
	"天人三十八重",
	"天人三十九重",
	"天人四十重",
	"天人四十一重",
	"天人四十二重",
	"天人四十三重",
	"天人四十四重",
	"天人四十五重",
	"天人四十六重",
	"天人四十七重",
	"天人四十八重",
	"天人四十九重",
	"天人五十重",
	"天人五十一重",
	"天人五十二重",
	"天人五十三重",
	"天人五十四重",
	"天人五十五重",
	"天人五十六重",
	"天人五十七重",
	"天人五十八重",
	"天人五十九重",
	"天人六十重",
	"天人六十一重",
	"天人六十二重",
	"天人六十三重",
	"天人六十四重",
	"天人六十五重",
	"天人六十六重",
	"天人六十七重",
	"天人六十八重",
	"天人六十九重",
	"天人七十重",
	"天人七十一重",
	"天人七十二重",
	"天人七十三重",
	"天人七十四重",
	"天人七十五重",
	"天人七十六重",
	"天人七十七重",
	"天人七十八重",
	"天人七十九重",
	"天人八十重",
	"天人八十一重",
	"天人八十二重",
	"天人八十三重",
	"天人八十四重",
	"天人八十五重",
	"天人八十六重",
	"天人八十七重",
	"天人八十八重",
	"天人八十九重",
	"天人九十重",
	"天人九十一重",
	"天人九十二重",
	"天人九十三重",
	"天人九十四重",
	"天人九十五重",
	"天人九十六重",
	"天人九十七重",
	"天人九十八重",
	"天人九十九重",
	"天人一零零重",
	"天人一零一重",
	"天人一零二重",
	"天人一零三重",
	"天人一零四重",
	"天人一零五重",
	"天人一零六重",
	"天人一零七重",
	"天人一零八重",
	"天人一零九重",
	"天人一一零重",
	"天人一一一重",
	"天人一一二重",
	"天人一一三重",
	"天人一一四重",
	"天人一一五重",
	"天人一一六重",
	"天人一一七重",
	"天人一一八重",
	"天人一一九重",
	"天人一二零重",
	"天人一二一重",
	"天人一二二重",
	"天人一二三重",
	"天人一二四重",
	"天人一二五重",
	"天人一二六重",
	"天人一二七重",
	"天人一二八重",
	"天人一二九重"
};

CGoodInfo::CGoodInfo(void)
{
	m_strTipText.clear();
	g_server_time = 0 ;


	m_mapSkill[MAGICID_BLOOD_SHADE] = "血影刀法";
	m_mapSkill[MAGICID_REMAIN_SHADE] = "残影刀法";
	m_mapSkill[MAGICID_ATTACK_KILL] = "攻杀剑法";
	m_mapSkill[MAGICID_ATTACK_STICK] = "刺杀剑术";
	m_mapSkill[MAGICID_THUNDER_FIRE] = "雷霆剑法";
	m_mapSkill[MAGICID_ATTACK_FIRE] = "烈火剑法";
	m_mapSkill[MAGICID_WHOLE_MOON] = "抱月刀";
	m_mapSkill[MAGICID_ATTACK_MOON] = "半月弯刀";

	m_mapSkill[MAGICID_PROTECT_SYMBOL] = "灵魂道符";
	m_mapSkill[MAGICID_FIRE_JUJU] = "幽冥火咒";
	m_mapSkill[MAGICID_POISON_MAGIC] = "施毒术";
	m_mapSkill[MAGICID_CURSE] = "诅咒术";

	m_mapSkill[MAGICID_FIREBALL] = "小火球";
	m_mapSkill[MAGICID_HELL_FIRE] = "地狱火焰";
	m_mapSkill[MAGICID_THUNDER] = "雷电术";
	m_mapSkill[MAGICID_ATTACK_STICK] = "刺杀剑术";
	m_mapSkill[MAGICID_ICE_ARROW] = "冰箭术";
	m_mapSkill[MAGICID_ROTATE_FIRE] = "风火轮";
	m_mapSkill[MAGICID_ADV_FIREBALL] = "火炎刀";
	m_mapSkill[MAGICID_BLOW_FIRE] = "爆烈火焰";
	m_mapSkill[MAGICID_FIRE_WALL] = "火墙";
	m_mapSkill[MAGICID_ICE_DRAG] = "冰龙破";
	m_mapSkill[MAGICID_TRACE_THUNDER] = "疾光电影";
	m_mapSkill[MAGICID_ICE_ARRAY] = "玄冰刃";
	m_mapSkill[MAGICID_HELL_THUNDER] = "地狱雷光";
	m_mapSkill[MAGICID_ICE_STORM] = "冰咆哮";
	m_mapSkill[MAGICID_MULTI_THUDER] = "五雷轰";
	m_mapSkill[MAGICID_ICE_WHIRLWIND] = "冰旋风";
	m_mapSkill[MAGICID_DRAGON_LIGHT] = "狂龙紫电";
	m_mapSkill[MAGICID_FIRE_RAIN] = "流星火雨";
	m_mapSkill[MAGICID_PROTECT_SKIN] = "护身真气";
	m_mapSkill[MAGICID_HEAL] = "治疗术";

	m_mapSkill[MAGICID_AOFENGCUT_ATTACK] = "翱风斩";
	m_mapSkill[MAGICID_ZHONGLEI_ATTACK] = "纵雷诀";
	m_mapSkill[MAGICID_DULINGBO_ATTACK] = "毒凌波";
}

CGoodInfo::~CGoodInfo(void)
{
}

void CGoodInfo::AddText(std::string addtext,DWORD color)
{
	m_strTipText = m_strTipText + addtext + "^$^" ;
}

char* CGoodInfo::GetDateTime(const char* fmt,DWORD dwTime)
{
	static char str[128] = {0};

	tm*		when;
	__time32_t now = dwTime;
	when = _localtime32(&now);
	if (when)
	{
		strftime(str,128,fmt,when);
	}

	return str;
}

bool CGoodInfo::DumpGood(const char * strMsg , char * strResult , int &len)
{
	FocusGood.clear();
	FocusGood.SetGoodInfo(strMsg ,68);

	return DumpInfoFromCGood(FocusGood , strResult , len) ;
}

bool CGoodInfo::DumpInfoFromCGood(CGood& FocusGood , char * strResult , int &len)
{
	try
	{
		int rlen ;

		GetGoodInfo(FocusGood) ;
		rlen = (int) (m_strTipText.length());
		if( len < rlen)
		{
			len = rlen ;
			return false ;
		}

		len = rlen ;
		memcpy(strResult , m_strTipText.c_str() ,rlen ) ;
	}
	catch(...)
	{
		len = 0 ;
		return false ;
	}

	return true ;
}

void CGoodInfo::GetGoodInfo(CGood& FocusGood)
{

	int iTemp;
	char str[256] = {0};
	m_strTipText.clear();
	m_strTipText = "";

	//第一行
	//物品名称
	int iStdMode = FocusGood.GetStdMode();
	int iShape = FocusGood.GetShape();


	if(iStdMode == 46 && (iShape == 7 || iShape == 8))
	{
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);
		if(FocusGood.GetDura() == 0)
		{
			if(FocusGood.GetRecordTime() == 0x3fffffff)
			{
				this->AddText("完整的藏宝图");
			}
			else
			{
				this->AddText("残缺的藏宝图");
			}

			bitset<32> b((unsigned long long)FocusGood.GetRecordTime());
			int iCount = b.count();			

			if(iShape == 7)
			{
				sprintf(str,"内涵%d片银月碎片",iCount);
				this->AddText(str);
				this->AddText("双击后，拼搜出完整的银月藏宝图就可召唤神圣之门进入神秘的银月地图中",TIPS_KHAKI);
			}
			else
			{
				sprintf(str,"内涵%d片炙日碎片",iCount);
				this->AddText(str);
				this->AddText("双击后，拼搜出完整的炙日藏宝图就可召唤神圣之门进入神秘的炙日地图中",TIPS_KHAKI);
			}
		}
		else
		{
			sprintf(str,"%d号碎片",FocusGood.GetDura());
			this->AddText(str);

			if(iShape == 7)
			{
				this->AddText("可拖入到银月藏宝图中，拼搜银月藏宝图",TIPS_KHAKI);
			}
			else
			{
				this->AddText("可拖入到炙日藏宝图中，拼搜炙日藏宝图",TIPS_KHAKI);
			}
		}
		return;
	}

	if(iStdMode == 3 && iShape == 212) //真灵玉
	{
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);
		sprintf(str,"使用 %d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
		this->AddText(str,TIPS_GOLDEN);
		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,TIPS_WHITE);
		return;
	}

	if(iStdMode ==  3 && iShape == 213)	//真灵玉符
	{
		char* level[4] ={"大师","宗师","权威","王者"};
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);

		char name[16] = {0};
		*(WORD*)(name) = FocusGood.GetDemonDark1();
		*(WORD*)(name+2) = FocusGood.GetDemonDark2();
		*(WORD*)(name+4) = FocusGood.GetDemonDark3();

		for(int ii = 0;ii < 8;ii++)
		{
			name[6+ii] = FocusGood.GetResvEx(ii);
		}

		WORD iMagicID = MAKEWORD(FocusGood.GetAC(),FocusGood.GetAC2());
		std::map <DWORD, std::string> :: const_iterator it;

		it = m_mapSkill.find(iMagicID);
		if(it != m_mapSkill.end())
		{
			if(FocusGood.GetMAC() >= 4 && FocusGood.GetMAC() <= 7)
			{
				sprintf(str,"由%s制作的封印%s级%s玉符",name,level[FocusGood.GetMAC() - 4],it->second.c_str());
				this->AddText(str,TIPS_GOLDEN);
			}
		}
		sprintf(str,"使用 %d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
		this->AddText(str,TIPS_GOLDEN);
		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,TIPS_WHITE);

		return;
	}

	if (iStdMode == 90 && iShape >= 1 && iShape <= 4)
	{
		//碎末
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);
		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,TIPS_WHITE);
		sprintf(str,"数量%d",FocusGood.GetDura());
		this->AddText(str,TIPS_WHITE);
		return;
	}
	if (iStdMode == 90 && iShape == 5)
	{//行会资源包
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);
		if(FocusGood.IsBind()) // 已绑定
		{
			this->AddText("绑定",TIPS_RED);
		}
		this->AddText("内含：",TIPS_RED);
		sprintf(str,"%d点矿石资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点灵石资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点金条资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点宝石资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点木材资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);
		return;
	}

	//iStdMode 46,shape 4,5的为活动道具,shape 5,6的为能合并的道具,5为能合并活动道具
	if(iStdMode == 46 && (iShape == 4 || iShape == 5 || iShape == 6))
	{
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);
		if(FocusGood.IsBind()) // 已绑定
		{
			this->AddText("绑定",TIPS_RED);
		}		

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,TIPS_WHITE);

		if(iShape != 4)
		{
			sprintf(str,"%s%d个",FocusGood.GetName(),FocusGood.GetDura());
			this->AddText(str,TIPS_GOLDEN);
		}

		if(iShape != 6)
		{
			this->AddText("活动任务类道具，在中州杂货商处可以被回收",TIPS_KHAKI);
		}

		return;
	}	
	if(iStdMode == 49 && iShape == 81)
	{
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);

		if(FocusGood.IsBind()) // 已绑定
		{
			this->AddText("绑定",TIPS_RED);
		}	

		DWORD m = FocusGood.GetRecordTime();
		BYTE   cUnit        = FocusGood.GetDC();

		if( m != 0 )
		{
			char* btmp = NULL;
			if(cUnit<=3)
				btmp = GetDateTime("%Y年%m月%d日",m);
			else
				btmp = GetDateTime("%Y年%m月%d日 %H:%M:%S",m);

			this->AddText("获得时间:",TIPS_GOLDEN);
			this->AddText(btmp,TIPS_GOLDEN);
		}

		char szUnit[32] = {0};
		switch(cUnit)
		{
		case 1:
			strcpy(szUnit,"年");
			break;
		case 2:
			strcpy(szUnit,"个月");
			break;
		case 3:
			strcpy(szUnit,"天");
			break;
		case 4:
			strcpy(szUnit,"小时");
			break;
		case 5:
			strcpy(szUnit,"分");
			break;
		case 6:
			strcpy(szUnit,"秒");
			break;
		default:
			strcpy(szUnit,"天");
			break;
		}
		sprintf(str,"有效期限:%d%s",FocusGood.GetOriDuraMax(),szUnit);
		this->AddText(str,TIPS_GOLDEN);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,TIPS_WHITE);

		return;
	}


	if(iStdMode == 12)		//行会盾牌
	{
		this->AddText("行会盾牌",TIPS_GOLDEN);

		int iLevel = HIBYTE(LOWORD(FocusGood.GetFlag()));		
		int ipercent = HIBYTE(LOWORD(FocusGood.GetOriDuraMax()));

		sprintf(str,"品级: %d",iLevel);
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"持久力%d/%d",(FocusGood.GetDura()+500)/1000,(FocusGood.GetDuraMax()+500)/1000);
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"防御%d-%d",FocusGood.GetAC(),FocusGood.GetAC2());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"魔防%d-%d",FocusGood.GetMAC(),FocusGood.GetMAC2());
		this->AddText(str,TIPS_WHITE);

		if(iShape == 1)
		{
			sprintf(str,"攻击%d-%d",FocusGood.GetDC(),FocusGood.GetDC2());					
		}
		else if(iShape == 2)
		{
			sprintf(str,"魔法%d-%d",FocusGood.GetMC(),FocusGood.GetMC2());
		}
		else if(iShape == 3)
		{
			sprintf(str,"道术%d-%d",FocusGood.GetSC(),FocusGood.GetSC2());
		}
		this->AddText(str,TIPS_WHITE);			

		sprintf(str,"有一定几率免除%d%%伤害",ipercent);
		this->AddText(str,TIPS_GREEN);

		return;
	}

	if (iStdMode == 90 && iShape == 5)
	{//行会资源包
		this->AddText(FocusGood.GetName(),TIPS_GOLDEN);
		this->AddText("内含：",TIPS_RED);
		sprintf(str,"%d点矿石资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点灵石资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点金条资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点宝石资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%d点木材资源",FocusGood.GetDuraMax());
		this->AddText(str,TIPS_WHITE);
		return;
	}

	//第一行
	//物品名称
	if((iStdMode == 43) && (iShape == 1))	//碎片
	{
		int iLooks = FocusGood.GetLooks();

		if (iLooks >= 1229 && iLooks <= 1231)//蟠龙碎片
		{
			if(iLooks == 1229)
			{
				strcpy(str,"蟠龙傲天");
			}
			else if(iLooks == 1230)
			{
				strcpy(str,"蟠龙幻天");
			}
			else if(iLooks == 1231)
			{
				strcpy(str,"蟠龙玄天");
			}

			char* name[6] = {"头盔","项链","手镯","戒指","靴子","腰带"};

			if(FocusGood.GetAC2() >= 1 && FocusGood.GetAC2() <= 6)
			{
				strcat(str,name[FocusGood.GetAC2() - 1]);
			}

			string name1 = str;

			strcat(str,"碎片");

			this->AddText(str,TIPS_GOLDEN);

			name1 = "集齐四个之后即可重铸" + name1;
			this->AddText(name1.c_str(),TIPS_KHAKI);
			return;
		}
		else if (iLooks >= 1254 && iLooks <= 1256)//圣王碎片
		{
			if(iLooks == 1254)
			{
				strcpy(str,"圣天");
			}
			else if(iLooks == 1255)
			{
				strcpy(str,"圣魔");
			}
			else if(iLooks == 1256)
			{
				strcpy(str,"圣道");
			}

			char* name[6] = {"头盔","项链","手镯","戒指","靴子","腰带"};
			int iType = FocusGood.GetAC2() % 10;

			if(iType >= 1 && iType <= 6)
			{
				strcat(str,name[iType - 1]);
			}



			char* levle[3] = {"上品","中品","下品"};
			int iLevel = FocusGood.GetAC2() / 10;
			if(iLevel >= 0 && iLevel <= 2)
			{
				strcat(str,"(");
				strcat(str,levle[iLevel]);
				strcat(str,")");
			}

			string name1 = str;
			strcat(str,"碎片");

			this->AddText(str,TIPS_GOLDEN);

			name1 = "集齐四个之后即可重铸" + name1;
			this->AddText(name1.c_str(),TIPS_KHAKI);
			return;
		}	
	}

	if((iStdMode == 37) && ( (iShape == 222) || (iShape == 223) || (iShape == 224) ))	//残章
	{
		string name = FocusGood.GetName();
		this->AddText(name.c_str(),TIPS_GOLDEN);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"%s%d个",name.c_str(),FocusGood.GetDura());
		this->AddText(str,TIPS_GOLDEN);	

		sprintf(str,"收集一定数量的残章，重叠后可在灵兽骑将处合成各级%s技能书",name.substr(0,6).c_str());
		this->AddText(str,TIPS_KHAKI);

		return;
	}

	//归元丹，归元神丹，补元旦
	if((iStdMode == 0) && (iShape == 2))
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);

		if(FocusGood.GetDuraMax() > 1)
		{
			sprintf(str,"使用 %d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
			this->AddText(str,0xFFFFFF00);
		}

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);

		return;
	}

	if((iStdMode == 37 && iShape == 214) || stricmp(FocusGood.GetName(),"神秘符石") == 0)
	{
		DWORD dwNum = 0;
		BYTE* pByte = (BYTE*)&dwNum;
		*pByte			= FocusGood.GetAC();		
		*(pByte + 1)	= FocusGood.GetAC2();
		*(pByte + 2)	= FocusGood.GetMAC();
		*(pByte + 3)	= FocusGood.GetMAC2();
		sprintf(str,"%s×%d块",FocusGood.GetName(),dwNum);
		this->AddText(str,0xFFFFFF00);
		this->AddText("可以用于锻造各种顶级武器",0xFFFFFF00);
		return;
	}
	//强化技能经验丹
	if((iStdMode == 0) && (iShape == 5))
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"蕴涵强化技能经验%d点",FocusGood.GetMC());
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);

		this->AddText("在本体或元神包裹中双击可直接增加强化技能经验",0xFFFFFF00);

		return;
	}
	//精英勋章
	if(iStdMode == 30 && iShape ==0 && FocusGood.GetLooks() == 8020)
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,TIPS_GOLDEN);

		int iTemp = FocusGood.GetDuraMax()+500;
		if(iTemp/1000 > 0)
		{
			sprintf(str,"持久力%d/%d",(FocusGood.GetDura()+500)/1000,iTemp/1000);
			this->AddText(str,TIPS_WHITE);
		}

		int iTotalDC = FocusGood.GetDC();
		int iTotalDC2 = FocusGood.GetDC2();
		DWORD dwColor = TIPS_WHITE;
		if(iTotalDC != 0 || iTotalDC2 != 0 )
		{
			sprintf(str,"攻击%d-%d",iTotalDC,iTotalDC2);
			this->AddText(str,dwColor);
		}

		int iTotalMC = FocusGood.GetMC();
		int iTotalMC2 = FocusGood.GetMC2();
		if(iTotalMC != 0 || iTotalMC2 != 0 )
		{
			sprintf(str,"魔法%d-%d",iTotalMC,iTotalMC2);
			this->AddText(str,dwColor);
		}

		int iTotalSC = FocusGood.GetSC();
		int iTotalSC2 = FocusGood.GetSC2();
		if(iTotalSC != 0 || iTotalSC2 != 0 )
		{
			sprintf(str,"道术%d-%d",iTotalSC,iTotalSC2);
			this->AddText(str,dwColor);
		}

		int iTotalAC = FocusGood.GetAC();
		int iTotalAC2 = FocusGood.GetAC2();
		if(iTotalAC != 0 ||  iTotalAC2 != 0 )
		{
			sprintf(str,"防御%d-%d",iTotalAC,iTotalAC2);
			this->AddText(str,dwColor);
		}

		int iTotalMAC = FocusGood.GetMAC();
		int iTotalMAC2 = FocusGood.GetMAC2();
		if(iTotalMAC != 0 || iTotalMAC2 != 0 )
		{
			sprintf(str,"魔防%d-%d",iTotalMAC,iTotalMAC2);
			this->AddText(str,dwColor);
		}

		return;
	}

	//圣灵精华
	if(iStdMode == 37 && iShape == 212)
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);

		//if(FocusGood.IsBind()) // 已绑定
		//{
		//	this->AddText("绑定",TIPS_RED);
		//}

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFFFF);

		sprintf(str,"圣殿精华%d个",FocusGood.GetDura());
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"%s","双击操作可激活武器上的隐藏技能属性，将其拖动到宝石上可将宝石变成圣殿宝石");
		this->AddText(str,0xFFFFCF63);
		return;
	}

	//灵符袋
	if((iStdMode == 3) && (iShape == 205))
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,TIPS_GOLDEN);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,TIPS_WHITE);

		sprintf(str,"内有灵符%d个",FocusGood.GetDura());
		this->AddText(str,TIPS_GOLDEN);		
		return;
	}

	//祝福油罐
	if((iStdMode == 37) && (iShape == 210))
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"%d/%d",FocusGood.GetAC(),FocusGood.GetDuraMax());
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);

		return;
	}
	//封元册
	if((iStdMode == 49) && (iShape == 56))
	{
		if(FocusGood.GetAC() == 0)
		{
			if(FocusGood.GetMAC() == 1)
			{
				sprintf(str,"%s","玄华封元册(铁血魔城)");
			}
			else
			{
				sprintf(str,"%s","封元册（铁血魔城）");
			}

			this->AddText(str,0xFFFFFF00);
		}
		else
		{
			return;
		}

		sprintf(str,"残余封印力量%d",FocusGood.GetDura());
		this->AddText(str,0xFFFFFFFF);

		if(FocusGood.GetMAC() == 1)
		{
			sprintf(str,"%s","用于存取玄华封元印，双击查看");
		}
		else
		{
			sprintf(str,"%s","用于存取封元印，双击查看");
		}

		this->AddText(str,0xFFFFCF63);

		return;
	}

	//天关幻镜宝石
	if((iStdMode == 37) && (iShape == 209))
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);

		time_t m = 0;
		*((BYTE *)&m)		= FocusGood.GetDC();		
		*((BYTE *)&m + 1)	= FocusGood.GetDC2();
		*((BYTE *)&m + 2)	= FocusGood.GetMC();
		*((BYTE *)&m + 3)	= FocusGood.GetMC2();

		if( m != 0 )
		{
			time_t t_time = 0;
			time(&t_time);
			t_time += g_server_time;
			int iHours = (int)( (m - t_time)/ (3600));//m为截止时间，最长可以使用时间


			if(iHours > 120 && iHours < 168)
				this->AddText("宝石晶莹而光华四射",0xFFFFFF00);
			else if(iHours > 72 && iHours <= 120)
				this->AddText("宝石出现了细微裂纹",0xFFFFFF00);
			else if(iHours > 25 && iHours <= 72)
				this->AddText("宝石的四周已布满了细纹",0xFFFF0000);
			else if( iHours >= 0 && iHours <= 24)
				this->AddText("宝石即将风化碎裂，请尽快使用",0xFFFF0000);
			else
				this->AddText("长期暴露在空气中，宝石完全腐蚀了",0xFFFF0000);


		}

		if(FocusGood.GetAC() == 1)
		{	
			this->AddText("海蓝色的宝石，能使天关幻镜映射出大量奖励",0xFFFFFF00);
			this->AddText("如在神威狱中使用更能获得额外的奖励",0xFFFFFF00);
		}
		else if(FocusGood.GetAC() == 2)
		{
			this->AddText("血红的钻石，能使天关幻镜映射出海量奖励",0xFFFFFF00);
			this->AddText("如在神威狱中使用更能获得意想不到的奖励",0xFFFFFF00);
		}
		else if(FocusGood.GetAC() == 3)
		{
			this->AddText("金玉镶嵌的宝石，能使天关幻镜映射出至高奖励",0xFFFFFF00);
			this->AddText("如在神威狱中使用更能获得为之惊叹的奖励",0xFFFFFF00);
		}


		return;
	}    
	//天关幻镜
	if((iStdMode == 46) && (iShape == 3))
	{
		if(FocusGood.GetAC() == 1)
			sprintf(str,"%s","蓝宝幻镜");
		else if(FocusGood.GetAC() == 2 )
			sprintf(str,"%s","红钻幻镜");
		else if(FocusGood.GetAC() == 3 )
			sprintf(str,"%s","金玉幻镜");
		else
			sprintf(str,"%s",FocusGood.GetName());

		this->AddText(str,0xFFFFFF00);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"拥有灵力数:%d",FocusGood.GetDura());
		this->AddText(str,0xFFFFFF00);

		if(FocusGood.GetMAC() == 0)
		{
			this->AddText("已破碎",0xFFFFFF00);
		}


		this->AddText("注入灵符灵力后可以获取海量经验和各类宝物",0xFFFFFF00);

		if(FocusGood.GetAC() == 1)
		{
			this->AddText("海蓝色的宝石使天关幻镜映射出大量奖励",0xFFFFFF00);
			this->AddText("如在神威狱中使用更能获得额外的奖励",0xFFFFFF00);
		}
		else if(FocusGood.GetAC() == 2 )
		{
			this->AddText("血红的钻石使天关幻镜映射出海量奖励",0xFFFFFF00);
			this->AddText("如在神威狱中使用更能获得意想不到的奖励",0xFFFFFF00);
		}
		else if(FocusGood.GetAC() == 3 )
		{
			this->AddText("金玉镶嵌的宝石使天关幻镜映射出至高奖励",0xFFFFFF00);
			this->AddText("如在神威狱中使用更能获得为之惊叹的奖励",0xFFFFFF00);
		}


		return;
	}
	//神威狱金币
	if((iStdMode == 9) && (iShape == 2))
	{
		sprintf(str,"神威狱金币×%d枚",FocusGood.GetDura());
		this->AddText(str,0xFFFFFF00);
		this->AddText("可以用于获取神威狱累计大奖",0xFFFFFF00);

		return;
	}	
	//邪心乾坤咒
	if((iStdMode == 46) && (iShape == 2))
	{
		char name[16] = {0};
		*(WORD*)(name) = FocusGood.GetDemonDark1();
		*(WORD*)(name+2) = FocusGood.GetDemonDark2();
		*(WORD*)(name+4) = FocusGood.GetDemonDark3();

		for(int ii = 0;ii < 8;ii++)
		{
			name[6+ii] = FocusGood.GetResvEx(ii);
		}

		if(strlen(name) > 0)
			sprintf(str,"%s(%s)",FocusGood.GetName(),name);
		else
			strcpy(str,FocusGood.GetName());

		this->AddText(str,0xFFFFFF00);

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);

		return;
	}

	//祈愿符
	if(iStdMode == 37 && iShape == 216)
	{		
		this->AddText(FocusGood.GetName(),0xFFFFFF00);
		char name[16] = {0};
		*(WORD*)(name) = FocusGood.GetDemonDark1();
		*(WORD*)(name+2) = FocusGood.GetDemonDark2();
		*(WORD*)(name+4) = FocusGood.GetDemonDark3();

		for(int ii = 0;ii < 8;ii++)
		{
			name[6+ii] = FocusGood.GetResvEx(ii);
		}

		if(strlen(name) > 0)
			sprintf(str,"捆扎%s",name);

		this->AddText(str,0xFFFFFF00);

		if(stricmp(FocusGood.GetName(),"祈愿符(捆)") == 0)
		{
			sprintf(str,"持久%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
			this->AddText(str,0xFFFFFFFF);
		}

		sprintf(str,"重量%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFFFF);
		return;
	}

	if(iStdMode == 49 && iShape == 10)
	{
		BYTE a0 = FocusGood.GetMAC2();
		BYTE a1 = FocusGood.GetMAC();
		BYTE a2 = FocusGood.GetAC2();
		BYTE a3 = FocusGood.GetAC();

		DWORD m = a0 << 24;
		m += a1 << 16;
		m += a2 << 8;
		m += a3;

		if(m > 0)
			sprintf(str,"%s(%d)",FocusGood.GetName(),m);
		else
			sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);
		return ;
	}
	if((iStdMode == 49) && (iShape == 11))
	{
		time_t m = 0;
		*((BYTE *)&m)		= FocusGood.GetAC();		
		*((BYTE *)&m + 1)	= FocusGood.GetAC2();
		*((BYTE *)&m + 2)	= FocusGood.GetMAC();
		*((BYTE *)&m + 3)	= FocusGood.GetMAC2();

		if( m != 0 )
		{
			tm * btime = localtime(&m);
			char btmp[256];
			strftime(btmp,256,"%Y年%m月%d日 %H:%M",btime);


			*((BYTE *)&m)		= FocusGood.GetDC();
			*((BYTE *)&m + 1)	= FocusGood.GetDC2();
			*((BYTE *)&m + 2)	= FocusGood.GetMC();
			*((BYTE *)&m + 3)	= FocusGood.GetMC2();

			tm * etime = localtime(&m);

			char etmp[256];
			strftime(etmp,256,"%Y年%m月%d日 %H:%M",etime);
			sprintf(str,"%s(%s - %s)",FocusGood.GetName(),btmp,etmp);
			this->AddText(str,0xFFFFFF00);
			return;
		}
	}
	if(iStdMode ==  49 && iShape == 24)		//至尊宝盒
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,TIPS_GOLDEN);


		//bool bUsed = true;
		//DWORD m = FocusGood.GetRecordTime();
		//if(m == 0)
		//{
		//	this->AddText("未使用",TIPS_GOLDEN);
		//	bUsed = false;
		//}
		//else
		//{
		//	this->AddText("已使用",TIPS_GOLDEN);
		//}

		

		this->AddText("重量1",TIPS_WHITE);

		
		//sprintf(str,"初始灵力值%d",((WORD)FocusGood.GetSC2() << 8) | (FocusGood.GetSC()));
		//this->AddText(str,TIPS_GOLDEN);
		//this->AddText("双击后使用，可以同时使用多个灵犀宝盒",TIPS_KHAKI);
		
		return;
	}
	if((iStdMode == 49) && (iShape >= 12 && iShape <=50 ))
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);
		time_t m = 0;
		*((BYTE *)&m)		= FocusGood.GetAC();		
		*((BYTE *)&m + 1)	= FocusGood.GetAC2();
		*((BYTE *)&m + 2)	= FocusGood.GetMAC();
		*((BYTE *)&m + 3)	= FocusGood.GetMAC2();

		if( m != 0 )
		{
			tm * btime = localtime(&m);
			char btmp[256];
			strftime(btmp,256,"%Y年%m月%d日 ",btime);

			sprintf(str,"%s开始使用，期限%d天",btmp,FocusGood.GetDuraMax());

			this->AddText("已使用",0xFFFFFF00);
			this->AddText(str,0xFFFFFF00);
			return;
		}
		else
		{
			sprintf(str,"未使用(%d天)",FocusGood.GetDuraMax());
			this->AddText(str,0xFFFFFF00);

		}
		this->AddText("重量1",0xFFFFFF00);
		return;
	}

	if(iStdMode == 49 && iShape == 64)//一种统一卡,包括天和小时
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);
		//if(FocusGood.IsBind()) // 已绑定
		//{
		//	this->AddText("绑定",TIPS_RED);
		//}

		time_t m = 0;
		*((BYTE *)&m)		= FocusGood.GetAC();		
		*((BYTE *)&m + 1)	= FocusGood.GetAC2();
		*((BYTE *)&m + 2)	= FocusGood.GetMAC();
		*((BYTE *)&m + 3)	= FocusGood.GetMAC2();

		bool bFlagHour = (FocusGood.GetDC() != 0);

		char cTemp[100]={0};
		double  fCoubleNum = FocusGood.GetDC2()/10.0 + 1;

		if(FocusGood.GetDC2()%10 == 0)
		{
			sprintf(cTemp,"%d倍经验卡",(int)fCoubleNum);
		}
		else
		{
			sprintf(cTemp,"%2.1f倍经验卡",fCoubleNum);
		}			

		this->AddText(cTemp,0xFFFFFF00);

		if( m != 0)
		{
			tm * btime = localtime(&m);
			char btmp[256];
			strftime(btmp,256,"%Y年%m月%d日 %H:%M",btime);

			if(bFlagHour)
				sprintf(str,"%s开始使用，期限%d小时",btmp,FocusGood.GetOriDuraMax());
			else
				sprintf(str,"%s开始使用，期限%d天",btmp,FocusGood.GetOriDuraMax());

			this->AddText("已使用",0xFFFFFF00);
			this->AddText(str,0xFFFFFF00);
			if(FocusGood.GetMC()==0)
			{
				this->AddText("所有地图有效",0xFFFFFF00);
			}else if(FocusGood.GetMC() == 1)
			{
				this->AddText("只在炼狱地图有效",0xFFFFFF00);
			}
			return;
		}
		else
		{
			if(bFlagHour)
				sprintf(str,"未使用(%d小时)",FocusGood.GetOriDuraMax());
			else
				sprintf(str,"未使用(%d天)",FocusGood.GetOriDuraMax());
			this->AddText(str,0xFFFFFF00);

			if(FocusGood.GetMC()==0)
			{
				this->AddText("所有地图有效",0xFFFFFF00);
			}else if(FocusGood.GetMC() == 1)
			{
				this->AddText("只在炼狱地图有效",0xFFFFFF00);
			}
		}
		this->AddText("重量1",0xFFFFFFFF);

		return;
	}

	//小时卡
	if((iStdMode == 49) && (iShape >= 61 && iShape <= 80 ))
	{
		sprintf(str,"%s",FocusGood.GetName());
		this->AddText(str,0xFFFFFF00);
		time_t m = 0;
		*((BYTE *)&m)		= FocusGood.GetAC();		
		*((BYTE *)&m + 1)	= FocusGood.GetAC2();
		*((BYTE *)&m + 2)	= FocusGood.GetMAC();
		*((BYTE *)&m + 3)	= FocusGood.GetMAC2();

		if( m != 0 )
		{
			tm * btime = localtime(&m);
			char btmp[256];
			strftime(btmp,256,"%Y年%m月%d日 %H:%M",btime);

			sprintf(str,"%s开始使用，期限%d小时",btmp,FocusGood.GetDuraMax());

			this->AddText("已使用",0xFFFFFF00);
			this->AddText(str,0xFFFFFF00);
			return;
		}
		else
		{
			sprintf(str,"未使用(%d小时)",FocusGood.GetDuraMax());
			this->AddText(str,0xFFFFFF00);

		}
		this->AddText("重量1",0xFFFFFF00);
		return;
	}

	//豹魔石
	if(iStdMode == 49 && iShape == 51)
	{
		char str[256] = "";
		switch((FocusGood.GetSC() & 0xF8) >> 3)	//bit[0~2] 转生次数0，1，2，3，4.。。bit[3~7] 宠物类型 0-豹子 1-狮子 2-麒麟
		{
		case 0:		
			sprintf(str,"%s","豹魔石");
			break;
		case 1:
			sprintf(str,"%s","狮魔石");
			break;
		case 2:
			sprintf(str,"%s","麒麟石");
			break;
		case 3:
			sprintf(str,"%s","凤凰石");				
			break;
		default:
			sprintf(str,"%s","豹魔石");
		}

		

		this->AddText(str,0xFFFFFF00);

		////自定义名字
		if(FocusGood.GetExternString() != "")
		{
			this->AddText(FocusGood.GetExternString().c_str(),0xFF00FF00);
		}

		std::string strPetName = FocusGood.GetPetName();
		sprintf(str,"%d级%s",FocusGood.GetDC(), strPetName.c_str());

		//喂食时间
		DWORD m = FocusGood.GetRecordTime();

		if( m != 0 )
		{
			__time32_t t_time;
			_time32(&t_time);
			t_time += g_server_time;
			int iHours = (int)((t_time - m) / 3600);
			int iDays = iHours / 24;
			if(iDays >= 4)
				strcat(str,"(死亡)");
			else if(iDays == 3)
				strcat(str,"(饥饿)");
			else if(iDays >= 2)
				strcat(str,"(微饿)");
			else
				strcat(str,"(饱)");
		}
		this->AddText(str,0xFFFFFF00);

		
		WORD wExp = 0;
		*((BYTE *)&wExp)     = FocusGood.GetMC();
		*((BYTE *)&wExp + 1) = FocusGood.GetMC2();
		sprintf(str,"经验值%d",wExp);
		this->AddText(str,0xFFFFFF00);
		

		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);
		if(FocusGood.GetSC2() & 0x20 )
			this->AddText("已用手机帐号开通",0xFFFF0000);
		//if(FocusGood.GetDC2()>>4 == 2)
		//	this->AddText("仅限沙城城主使用",0xFFFF0000);
		return;
	}

	if(iStdMode == 37 && (iShape == 203 || iShape == 205) ) //乾坤锁he乾坤箱
	{
		std::string name;
		name.assign(FocusGood.GetName(),6);
		this->AddText(name.c_str(),0xFFFFFF00);

		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFFFF);
		sprintf(str,"持久:%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
		this->AddText(str,0xFFFFFFFF);

		if(iShape == 203)
		{ 
			int hour = FocusGood.GetAC2()*256 + FocusGood.GetAC() ;
			if(hour>0)
			{
				sprintf(str,"锁定有效时间：在线累积%d小时",hour);
				this->AddText(str,0xFF00FF00);
			}

		}

		return;
	}
	if(iShape == 204 && iStdMode == 37 )  //显示乾坤封印剩余时间
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);
		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFFFF);
		ZeroMemory(str,256);

		int hour = FocusGood.GetDura();
		sprintf(str,"剩余有效封印时间：%d小时",hour);
		this->AddText(str,0xFF00FF00);
		return;
	}

	if(iStdMode == 3 && iShape == 25)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);
		sprintf(str,"魔力:%d/%d",FocusGood.GetAC2(),FocusGood.GetAC());
		this->AddText(str,0xFFFFFF00);
		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFFFF);
		return;
	}
	if(iStdMode == 55 && iShape == 1)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);
		if(FocusGood.GetDura() == 2)
		{
			this->AddText("(未记录)",0xFFFFFFFF);
			this->AddText("双击即可记录你对你的爱人的浓情蜜语",0xFFFFFFFF);
		}
		else
		{
			this->AddText("(已记录)",0xFFFFFFFF);
			this->AddText("将此道具交给爱情神树，即可让全区",0xFFFFFFFF);
			this->AddText("全服玩家看到你的爱情宣言",0xFFFFFFFF);
		}
		return;
	}
	//计次数的PT道具，如千里传音卷轴
	//熔炼宝箱
	if(iStdMode == 37 || 
		(iStdMode == 3 && (iShape == 201  || iShape == 203  || iShape == 29
		||	iShape == 18 || iShape == 19 || iShape == 20 || iShape == 24 || iShape == 27))
		||(iStdMode == 36 && iShape == 199 )
		)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);

		if(iStdMode == 3 && (iShape==27 || iShape == 29))
		{
			sprintf(str,"增加镇魔值:%d",FocusGood.GetAC());
			this->AddText(str,0xFFFFFF00);
		}

		sprintf(str,"可使用次数:%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
		if(iStdMode == 3 && iShape==24)
		{
			sprintf(str,"持久:%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
			this->AddText(str,0xFFFFFF00);
		}
		else
		{
			this->AddText(str,0xFFFFFF00);
		}

		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);
		if(iStdMode == 37 && iShape == 40)
		{
			sprintf(str,"开启后%d天有效",FocusGood.GetAC());
			this->AddText(str,0xFFFFFF00);
		}
		if(iStdMode == 37 && iShape == 20)
		{
			sprintf(str,"幸运+1");
			this->AddText(str,0xFFFFFF00);

		}

		if(iStdMode == 37 && iShape == 220)
		{
			if(strcmp(FocusGood.GetName(),"分红经验书") == 0)
			{
				this->AddText("双击分红经验书即可获得经验",0xFFFFCF63);

			}else if(strcmp(FocusGood.GetName(),"分红经验天书") == 0)
			{
				this->AddText("双击分红经验天书即可获得经验",0xFFFFCF63);
			}
		}

		return;
	}
	if(iStdMode == 44 && (iShape == 4|| iShape ==3) )
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);
		sprintf(str,"可使用次数:%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
		this->AddText(str,0xFFFFFFFF);
		//if(iShape ==3)
		//	sprintf(str,"%s","可提升幸运概率15％-20％");
		//if(iShape ==4)
		//	sprintf(str,"%s","可提升幸运概率5％-10％");
		//this->AddText(str);
		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,-1);
		return;
	}

	//传承珠，有显示经验的新物品
	if(iStdMode == 9 && iShape == 1)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);

		//经验值
		DWORD dwExp;
		*((BYTE *)&dwExp)		= FocusGood.GetAC();		
		*((BYTE *)&dwExp + 1)	= FocusGood.GetAC2();
		*((BYTE *)&dwExp + 2)	= FocusGood.GetMAC();
		*((BYTE *)&dwExp + 3)	= FocusGood.GetMAC2();

		sprintf(str,"目前累积的经验为%u",dwExp);
		this->AddText(str,0xFFFFFF00);

		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);
		return;
	}

	//超级技能项链
	if(iStdMode == 18 || iStdMode == 29)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);

		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);

		iTemp = FocusGood.GetDuraMax()+500;
		if(iTemp/1000 > 0)
		{
			sprintf(str,"持久力:%d/%d",(FocusGood.GetDura()+500)/1000,iTemp/1000);
			this->AddText(str,0xFFFFFF00);
		}

		time_t m = 0;
		*((BYTE *)&m)		= FocusGood.GetAC();		
		*((BYTE *)&m + 1)	= FocusGood.GetAC2();
		*((BYTE *)&m + 2)	= FocusGood.GetMAC();
		*((BYTE *)&m + 3)	= FocusGood.GetMAC2();

		if( m != 0 )
		{
			tm* btime = localtime(&m);
			char btmp[256];
			strftime(btmp,256,"%Y年%m月%d日",btime);

			sprintf(str,"%s开始使用，期限%d天",btmp,FocusGood.GetDuraMax()/1000);
			this->AddText(str,0xFFFFFF00);
		}
		return;
	}

	//元神的提示信息
	if(iStdMode == 70)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);
		if(iShape == 0)
		{
			sprintf(str,"天魂灵力:%d",FocusGood.GetAC());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"地魂灵力:%d",FocusGood.GetAC2());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"命魂灵力:%d",FocusGood.GetMAC());
			this->AddText(str,0xFFFFFFFF);
		}
		else if(iShape == 1)
		{
			sprintf(str,"天冲灵力:%d",FocusGood.GetMAC2());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"灵慧灵力:%d",FocusGood.GetDC());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"力魄灵力:%d",FocusGood.GetDC2());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"气魄灵力:%d",FocusGood.GetMC());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"中枢灵力:%d",FocusGood.GetMC2());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"精魂灵力:%d",FocusGood.GetSC());
			this->AddText(str,0xFFFFFFFF);

			sprintf(str,"英魄灵力:%d",FocusGood.GetSC2());
			this->AddText(str,0xFFFFFFFF);
		}
		else if(iShape == 2)
		{
			if(FocusGood.GetAC() == 0)
				strcpy(str,"职业:战士");
			else if(FocusGood.GetAC() == 1)
				strcpy(str,"职业:法师");
			else
				strcpy(str,"职业:道士");

			this->AddText(str,0xFFFFFF00);
		}
		return;
	}

	//三魂七魄的石头
	if(iStdMode == 71)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);

		int iDura = (FocusGood.GetDura() + 500) / 1000;
		switch(iShape)
		{
		case 1:
			sprintf(str,"蕴含天魂灵力:%d",iDura);
			break;
		case 2:
			sprintf(str,"蕴含地魂灵力:%d",iDura);
			break;
		case 3:
			sprintf(str,"蕴含命魂灵力:%d",iDura);
			break;
		case 4:
			sprintf(str,"蕴含天冲灵力:%d",iDura);
			break;
		case 5:
			sprintf(str,"蕴含灵慧灵力:%d",iDura);
			break;
		case 6:
			sprintf(str,"蕴含力魄灵力:%d",iDura);
			break;
		case 7:
			sprintf(str,"蕴含精魄灵力:%d",iDura);
			break;
		case 8:
			sprintf(str,"蕴含英魄灵力:%d",iDura);
			break;
		case 9:
			sprintf(str,"蕴含气魄灵力:%d",iDura);
			break;
		case 10:
			sprintf(str,"蕴含中枢灵力:%d",iDura);
			break;
		default:
			break;
		}
		if(strlen(str) > 0)
			this->AddText(str,0xFFFFFFFF);

		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFFFF);
		return;
	}

	//PT设置了开启时间和期限的物品，如盛大宝卡
	if(iStdMode == 38)
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);
		time_t m = 0;
		*((BYTE *)&m)		= FocusGood.GetAC();		
		*((BYTE *)&m + 1)	= FocusGood.GetAC2();
		*((BYTE *)&m + 2)	= FocusGood.GetMAC();
		*((BYTE *)&m + 3)	= FocusGood.GetMAC2();
		BYTE   cUnit        = FocusGood.GetDC();

		if( m != 0 )
		{
			tm* btime = localtime(&m);
			char btmp[256];
			if(cUnit<=3)
				strftime(btmp,256,"%Y年%m月%d日",btime);
			else
				strftime(btmp,256,"%Y年%m月%d日 %H:%M:%S",btime);
			sprintf(str,"开启时间:%s(已开启)",btmp);
			this->AddText(str,0xFFFFFF00);
		}
		else
		{
			sprintf(str,"(未开启)");
			this->AddText(str,0xFFFFFF00);

		}
		char szUnit[32] = {0};
		switch(cUnit)
		{
		case 1:
			strcpy(szUnit,"年");
			break;
		case 2:
			strcpy(szUnit,"个月");
			break;
		case 3:
			strcpy(szUnit,"天");
			break;
		case 4:
			strcpy(szUnit,"小时");
			break;
		case 5:
			strcpy(szUnit,"分");
			break;
		case 6:
			strcpy(szUnit,"秒");
			break;
		default:
			strcpy(szUnit,"天");
			break;
		}
		sprintf(str,"有效期限:%d%s",FocusGood.GetDuraMax(),szUnit);
		this->AddText(str,0xFFFFFF00);
		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);
		return;
	}

	//回城石
	if (iStdMode == 3 && (iShape == 15 || iShape == 17))
	{
		this->AddText(FocusGood.GetName(),0xFFFFFF00);

		if(iShape == 17)
		{
			sprintf(str,"可使用次数:%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
			this->AddText(str,0xFFFFFF00);
		}

		char m[8] = {0};
		m[0] = FocusGood.GetDC();
		m[1] = FocusGood.GetDC2();
		m[2] = FocusGood.GetMC();
		m[3] = FocusGood.GetMC2();
		m[4] = FocusGood.GetSC();
		m[5] = FocusGood.GetSC2();

		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,0xFFFFFF00);
		return;
	}

	unsigned long ii = FocusGood.GetFlag();
	char * p =(char *) & ii;
	if( (iStdMode == 5 || iStdMode == 6 || iStdMode == 10 || iStdMode == 11) && ( ( p[2]&0x01) == 1) )
	{
		if((iStdMode == 5 || iStdMode == 6) && (FocusGood.GetAttachSkill() != 0 && FocusGood.GetAttachSkillLevel() != 0))
		{
			sprintf(str,"(*)%s(圣殿)",FocusGood.GetName());
		}
		else sprintf(str,"(*)%s",FocusGood.GetName());
	}
	else if(iStdMode == 4 && iShape >= 16 && iShape <= 18)//大师级技能书,db里是"大师小火球" 要显示成 "小火球(大师级)"
	{
		char strTemp[12] = {0};
		memcpy(strTemp,FocusGood.GetName(),4);
		memcpy(strTemp + 4,"级",2);
		sprintf(str,"%s(%s)",FocusGood.GetName() + 4,strTemp);
	}
	else
	{
		if((iStdMode == 5 || iStdMode == 6 ) &&( FocusGood.GetAttachSkill() != 0 && FocusGood.GetAttachSkillLevel() != 0))
			sprintf(str,"%s(圣殿)",FocusGood.GetName());
		else sprintf(str,"%s",FocusGood.GetName());
	}

	this->AddText(str,0xFFFFFF00);

	//1.907新增钻石刀刻字
	if(FocusGood.GetExternString()!="")
	{
		this->AddText(FocusGood.GetExternString().c_str(),0xFF00FF00);
	}

	if( iStdMode == 33 )
	{
		char chHorseName[11] = {0};
		chHorseName[0] = FocusGood.GetAC();
		chHorseName[1] = FocusGood.GetAC2();
		chHorseName[2] = FocusGood.GetMAC();
		chHorseName[3] = FocusGood.GetMAC2();
		chHorseName[4] = FocusGood.GetDC();
		chHorseName[5] = FocusGood.GetDC2();
		chHorseName[6] = FocusGood.GetMC();
		chHorseName[7] = FocusGood.GetMC2();
		chHorseName[8] = FocusGood.GetSC();
		chHorseName[9] = FocusGood.GetSC2();
		this->AddText(chHorseName,0xFFFFFF00);
	}
	//马票的出场时间
	if(iStdMode == 49 && iShape >= 1 && iShape <= 4  )
	{
		char p[4] = {0};
		p[0] = FocusGood.GetAC();
		p[1] = FocusGood.GetAC2();
		p[2] = FocusGood.GetMAC();
		p[3] = FocusGood.GetMAC2();
		if(*((DWORD *)p))
		{
			tm *pTime = localtime((const time_t *)p);

			p[0] = FocusGood.GetDC();
			p[1] = FocusGood.GetDC2();
			p[2] = FocusGood.GetMC();
			p[3] = FocusGood.GetMC2();

			sprintf(str,"%d-%d-%d的第%d场比赛",pTime->tm_year + 1900,pTime->tm_mon + 1,pTime->tm_mday,*((DWORD *)p));
			this->AddText(str,0xFFFFFFFF);        
		}
	}

	if( iStdMode == 0)
	{
		//深海灵礁，治疗神水
		if(iShape == 200 || iShape == 201)
		{
			sprintf(str,"可使用次数:%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
			this->AddText(str,0xFFFFFF00);
		}

		if (FocusGood.GetAC2() == 100)
		{
			sprintf(str,"魔元力 + %d",FocusGood.GetAC());
			this->AddText(str,TIPS_GREEN);
		}
		else if (FocusGood.GetAC2() == 101)
		{
			sprintf(str,"神元力 + %d",FocusGood.GetAC());
			this->AddText(str,TIPS_GREEN);
		}
		else if(FocusGood.GetAC() != 0)
		{
			sprintf(str," HP + %d",FocusGood.GetAC());
			this->AddText(str,-1);
		}

		if(FocusGood.GetMAC() != 0)
		{
			sprintf(str," MP + %d",FocusGood.GetMAC());
			this->AddText(str,-1);
		}
	}
	//物品重量
	if(FocusGood.GetWeight() > 0)
	{
		sprintf(str,"重量:%d",FocusGood.GetWeight());
		this->AddText(str,-1);
	}

	if ( iStdMode == 40) // 
	{
		sprintf(str,"品质%d/%d",(FocusGood.GetDura() + 500)/1000,(FocusGood.GetDuraMax() + 500) /1000);
		this->AddText(str,-1);
	}
	else if( (iStdMode == 43 && iShape != 1 && iShape != 11) || iStdMode == 48 ) 
	{
		sprintf(str,"品级%d",(FocusGood.GetDura()) / 10000 + 1);
		this->AddText(str,-1);
	}
	else if( iStdMode == 5 || iStdMode == 6 ||
		iStdMode == 10 || iStdMode == 11 ||
		iStdMode == 15 || iStdMode == 19 ||
		iStdMode == 20 || iStdMode == 21 ||
		iStdMode == 22 || iStdMode == 23 ||
		iStdMode == 24 || iStdMode == 25 ||
		iStdMode == 26 || iStdMode == 30 ||
		iStdMode == 33 || iStdMode == 34 ||
		iStdMode == 81 || iStdMode == 58)
	{
		if( (iStdMode == 25 || iStdMode == 34) && 
			(iShape == 5 || iShape == 1 || iShape == 2) )
		{
			if(FocusGood.GetAC() == 1)
				sprintf(str,"使用 :%d/%d",FocusGood.GetDura(),FocusGood.GetDuraMax());
			else
				sprintf(str,"使用 :%d/%d",FocusGood.GetDura()/100,FocusGood.GetDuraMax()/100);
			this->AddText(str,-1);
		}
		else
		{
			iTemp = FocusGood.GetDuraMax()+500;
			if(iTemp/1000 > 0)
			{
				sprintf(str,"持久力:%d/%d",(FocusGood.GetDura()+500)/1000,iTemp/1000);
				this->AddText(str,-1);
			}
		}
	}
	else if(iStdMode == 2)
	{
		sprintf(str,"使用 :%d/%d",FocusGood.GetDura()/100,FocusGood.GetDuraMax()/100);
		this->AddText(str,-1);
	}

	if( iShape == 130  ||  iShape == 131 ) // 神秘系列
	{
		this->AddText("????",-1);
		return;
	}


	//第二行
	//攻击力
	if( iStdMode == 4)
	{
		if(iShape == 0)
			this->AddText("武术秘笈",-1);
		else if( iShape == 1)
			this->AddText("魔法书",-1);
		else if( iShape == 2)
			this->AddText("道士秘笈",-1);
		else if(iShape >= 4 && iShape <= 9)//组合秘笈
			this->AddText("组合魔法秘笈",-1);
		else if(iShape == 13)
			this->AddText("战士骑战技能书",-1);
		else if(iShape == 14)
			this->AddText("法师骑战技能书",-1);
		else if(iShape == 15)
			this->AddText("道士骑战技能书",-1);
	}
	else if (iStdMode == 8) //元神的书
	{
		if(iShape == 0)
			this->AddText("战士元神秘笈",-1);
		else if( iShape == 1)
			this->AddText("法师元神秘笈",-1);
		else if( iShape == 2)
			this->AddText("道士元神秘笈",-1);
		else if(iShape >= 4 && iShape <= 9) //组合秘笈
			this->AddText("组合魔法秘笈",-1);
	}
	else if ( iStdMode == 10 || iStdMode == 11 ||
		iStdMode == 15 || iStdMode == 26 ||
		iStdMode == 30 || iStdMode == 22 ||
		iStdMode == 81 || iStdMode == 58 ||
		iStdMode == 59 || iStdMode == 60 ||
		iStdMode == 61) 
	{
		if(iShape == 1 && iStdMode == 30)
		{
			sprintf(str,"极品爆出率:+%d",FocusGood.GetSC());
			this->AddText(str,-1);
			return;
		}
		if(iShape == 2 && iStdMode == 30)
		{
			sprintf(str,"极品爆出率:+%d",FocusGood.GetDC());
			this->AddText(str,-1);
			return;
		}
		if(iShape == 3 && iStdMode == 30)
		{
			sprintf(str,"极品爆出率:+%d",FocusGood.GetMC());
			this->AddText(str,-1);
			return;
		}

		//原始的属性要变色
		int iTotalAC = FocusGood.GetAC();
		int iTotalAC2 = FocusGood.GetAC2();
		DWORD dwColor = 0xFFFFFFFF;

		if(iTotalAC != 0 ||  iTotalAC2 != 0 )
		{
			sprintf(str,"防御%d-%d",iTotalAC,iTotalAC2);
			this->AddText(str,dwColor);
		}

		//原始的属性要变色
		int iTotalMAC = FocusGood.GetMAC();
		int iTotalMAC2 = FocusGood.GetMAC2();
		dwColor = 0xFFFFFFFF;

		if(iTotalMAC != 0 || iTotalMAC2 != 0 )
		{
			sprintf(str,"魔防%d-%d",iTotalMAC,iTotalMAC2);
			this->AddText(str,dwColor);
		}
	}

	if( iStdMode == 19 )
	{
		if(FocusGood.GetAC2() != 0 )
		{
			sprintf(str,"魔法躲避:+%d%%",FocusGood.GetAC2() * 10);
			this->AddText(str,-1);
		}
		if(FocusGood.GetMAC2() != 0)
		{
			sprintf(str,"幸运:+%d",FocusGood.GetMAC2());
			this->AddText(str,-1);
		}
		if(FocusGood.GetMAC() != 0)
		{
			sprintf(str,"诅咒:+%d",FocusGood.GetMAC());
			this->AddText(str,-1);
		}
	}
	else if( iStdMode == 20 || iStdMode == 24 )
	{
		if( FocusGood.GetAC2() != 0)
		{
			sprintf(str,"命中:+%d",FocusGood.GetAC2()); 
			this->AddText(str,-1);
		}
		if( FocusGood.GetMAC2() != 0)
		{
			sprintf(str,"躲避:+%d",FocusGood.GetMAC2());
			this->AddText(str,-1);
		}
	}
	else if( iStdMode == 21 )
	{
		if( FocusGood.GetAC2() != 0)
		{
			sprintf(str,"体力恢复:+%d",FocusGood.GetAC2());
			this->AddText(str,-1);
		}
		if( FocusGood.GetMAC2() != 0)
		{
			sprintf(str,"魔法恢复:+%d",FocusGood.GetMAC2());
			this->AddText(str,-1);
		}
		if( FocusGood.GetAC() != 0)
		{
			sprintf(str,"攻击速度:+%d",FocusGood.GetAC());
			this->AddText(str,-1);
		}
		if( FocusGood.GetMAC() != 0)
		{
			sprintf(str,"攻击速度:-%d",FocusGood.GetMAC());
			this->AddText(str,-1);
		}
	}
	else if( iStdMode == 23)
	{
		if( FocusGood.GetAC2() != 0)
		{
			sprintf(str,"毒物躲避:+%d%%",FocusGood.GetAC2() * 10);
			this->AddText(str,-1);
		}
		if( FocusGood.GetMAC2() != 0)
		{
			sprintf(str,"中毒恢复:+%d%%",FocusGood.GetMAC2() * 10);
			this->AddText(str,-1);
		}
		if( FocusGood.GetAC() != 0)
		{
			sprintf(str,"攻击速度:+%d",FocusGood.GetAC());
			this->AddText(str,-1);
		}
		if( FocusGood.GetMAC() != 0)
		{
			sprintf(str,"攻击速度:-%d",FocusGood.GetMAC());
			this->AddText(str,-1);
		}
	}


	if( iStdMode == 5 || iStdMode == 6 ||
		iStdMode == 10 || iStdMode == 11 ||
		iStdMode == 15 || iStdMode == 19 ||
		iStdMode == 20 || iStdMode == 21 ||
		iStdMode == 22 || iStdMode == 23 ||
		iStdMode == 24 || iStdMode == 26 ||
		iStdMode == 30 ||
		iStdMode == 81 || iStdMode == 58 ||
		iStdMode == 59 || iStdMode == 60 ||
		iStdMode == 61) 
	{
		//原始的属性要变色
		int iTotalDC = FocusGood.GetDC();
		int iTotalDC2 = FocusGood.GetDC2();

		DWORD dwColor = 0xFFFFFFFF;
		if(iTotalDC != 0 || iTotalDC2 != 0 )
		{
			sprintf(str,"攻击%d-%d",iTotalDC,iTotalDC2);
			this->AddText(str,dwColor);
		}

		//原始的属性要变色
		int iTotalMC = FocusGood.GetMC();
		int iTotalMC2 = FocusGood.GetMC2();

		dwColor = 0xFFFFFFFF;
		if(iTotalMC != 0 || iTotalMC2 != 0 )
		{
			sprintf(str,"魔法%d-%d",iTotalMC,iTotalMC2);
			this->AddText(str,dwColor);
		}

		//原始的属性要变色
		int iTotalSC = FocusGood.GetSC();

		//天地宝石特殊性
		if(iStdMode == 61)
			iTotalSC = (FocusGood.GetSC() & 0x1f);

		int iTotalSC2 = FocusGood.GetSC2();

		dwColor = 0xFFFFFFFF;

		if(iTotalSC != 0 || iTotalSC2 != 0 )
		{
			sprintf(str,"道术%d-%d",iTotalSC,iTotalSC2);
			this->AddText(str,dwColor);
		}

		if( iStdMode == 5 || iStdMode == 6 )
		{
			unsigned long nTemp = FocusGood.GetFlag();
			char * pp = (char *)&nTemp;

			if( p[1] < 0 )
			{
				sprintf(str,"神圣:+%d",-pp[1]);
				this->AddText(str,-1);
			}
			else if( p[1] > 0 )
			{
				sprintf(str,"强度:+%d",pp[1]);
				this->AddText(str,-1);
			}
		}
	}

	//增加技能属性
	ParseAttackSkillInfo(FocusGood);
	//第三行
	//使用该物品所需的等级
	if( iStdMode == 5 || iStdMode == 6 ) 
	{
		if(FocusGood.GetAC2()!= 0)
		{
			sprintf(str,"命中:+%d",FocusGood.GetAC2());
			this->AddText(str,-1);
		}
		if(FocusGood.GetMAC2()!= 0)
		{
			if(FocusGood.GetMAC2() < 10)
			{
				sprintf(str,"攻击速度:-%d",FocusGood.GetMAC2());
				this->AddText(str,-1);
			}
			else
			{
				sprintf(str,"攻击速度:+%d",(unsigned char)(FocusGood.GetMAC2() - 10));
				this->AddText(str,-1);
			}
		}
		if(FocusGood.GetAC()!= 0)
		{
			sprintf(str,"幸运:+%d",FocusGood.GetAC());
			this->AddText(str,-1);
		}

		if(FocusGood.GetMAC()!= 0)
		{
			sprintf(str,"诅咒:+%d",FocusGood.GetMAC());
			this->AddText(str,-1);
		}
	}
	if( iStdMode == 4  || iStdMode == 5  || 
		iStdMode == 10 || iStdMode == 11 ||
		iStdMode == 15 || iStdMode == 19 ||
		iStdMode == 20 || iStdMode == 21 ||
		iStdMode == 22 || iStdMode == 23 ||
		iStdMode == 24 || iStdMode == 26 ||
		iStdMode == 6  || iStdMode == 8  ||
		iStdMode == 32 || iStdMode == 33 ||
		iStdMode == 81 || iStdMode == 58) 
	{
		if( (FocusGood.GetNeed() == 0 && FocusGood.GetNeedLevel() != 0)	
			|| (FocusGood.GetNeed() == 7 && FocusGood.GetNeedLevel() != 0) ||
			iStdMode == 4 || iStdMode == 8)
		{
			if(iStdMode == 4 || iStdMode == 8)
			{

				if(iShape >= 4 && iShape <= 9) //组合技能
				{
					if(FocusGood.GetNeedLevel() > 71 && FocusGood.GetNeedLevel() < 71 + 127)
						sprintf(str,"需要分身境界飞升%s",g_strFlyMask[FocusGood.GetNeedLevel()-71] + 4);
					else
						sprintf(str,"需要分身等级%d",FocusGood.GetNeedLevel());

					this->AddText(str,TIPS_GREEN);

					if(FocusGood.GetDuraMax() > 71 && FocusGood.GetDuraMax() < 71 + 127)
						sprintf(str,"需要主体境界%s",g_strFlyMask[FocusGood.GetDuraMax()-71]);
					else
						sprintf(str,"需要主体等级%d",FocusGood.GetDuraMax());

					this->AddText(str,TIPS_GREEN);
				}
				else if(iStdMode == 4 && (iShape >= 10 && iShape <= 12))//强化攻杀、火球、施毒
				{
					if(strnicmp(FocusGood.GetName(),"本体",4) == 0)
					{
						sprintf(str,"%s%d重",FocusGood.GetName(),FocusGood.GetAC());
						this->AddText(str,TIPS_GOLDEN);

						if(FocusGood.GetDuraMax() > 71 && FocusGood.GetDuraMax() < 71 + 127)
							sprintf(str,"需要本体境界%s",g_strFlyMask[FocusGood.GetDuraMax()-71]);
						else
							sprintf(str,"需要本体等级%d",FocusGood.GetDuraMax());

						this->AddText(str,TIPS_GREEN);
						this->AddText("本体包裹中双击直接学习",TIPS_KHAKI);		
					}
					else
					{
						if(FocusGood.GetDuraMax() > 71 && FocusGood.GetDuraMax() < 71 + 127)
							sprintf(str,"需要分身境界飞升%s",g_strFlyMask[FocusGood.GetDuraMax()-71] + 4);
						else
							sprintf(str,"需要元神等级%d",FocusGood.GetDuraMax());

						this->AddText(str,TIPS_GREEN);
					}
				}
				else if(iStdMode == 4 && (iShape >= 13 && iShape <= 15))		//骑站技能书
				{
					if(FocusGood.GetNeedLevel() > 71 && FocusGood.GetNeedLevel() < 71 + 127)
						sprintf(str,"需要主体境界%s",g_strFlyMask[FocusGood.GetNeedLevel()-71]);
					else
						sprintf(str,"需要主体等级%d",FocusGood.GetNeedLevel());

					this->AddText(str,TIPS_GREEN);
					sprintf(str,"需要灵兽等级%d",FocusGood.GetDuraMax());
					this->AddText(str,TIPS_GREEN);
				}
				else if(iStdMode == 4 && (iShape >= 16 && iShape <= 18))		//大师级技能
				{ 
					int iNeedLevel = max(FocusGood.GetDuraMax(),FocusGood.GetDarkNeedLevel());
					if(iNeedLevel > 71 && iNeedLevel < 71 + 127)
					{
						sprintf(str,"需要境界:%s",g_strFlyMask[iNeedLevel-71]);
					}
					else
						sprintf(str,"需要等级:%d",iNeedLevel);

					this->AddText(str,TIPS_GREEN);

					if (FocusGood.GetMAC2() == 17)
					{
						this->AddText("天人修神秘籍",TIPS_RED);
					}
					else if(FocusGood.GetMAC2() == 18)
					{
						this->AddText("天人修魔秘籍",TIPS_RED);
					}
				}
				else
				{ 
					int iNeedLevel = max(FocusGood.GetDuraMax(),FocusGood.GetDarkNeedLevel());
					if(iNeedLevel > 71 && iNeedLevel < 71 + 127)
					{
						sprintf(str,"需要境界:%s",g_strFlyMask[iNeedLevel-71]);
					}
					else
						sprintf(str,"需要等级:%d",iNeedLevel);

					this->AddText(str,TIPS_GREEN);
				}
			}
			else
			{
				int  iNeedLevel = 0;
				iNeedLevel = max(FocusGood.GetNeedLevel(),FocusGood.GetDarkNeedLevel());
				if(iNeedLevel > 71 && iNeedLevel < 71 + 127)
				{
					sprintf(str,"需要境界:%s",g_strFlyMask[iNeedLevel-71]);
				}
				else
					sprintf(str,"需要等级:%d",iNeedLevel);

				if(iNeedLevel >0)
				{
					this->AddText(str,TIPS_GREEN);
				}

			}
		}
		else if( FocusGood.GetNeed() == 1 && FocusGood.GetNeedLevel() != 0)
		{
			sprintf(str,"需要攻击力%d",FocusGood.GetNeedLevel());

			this->AddText(str,0xFFFFFFFF);
		}
		else if( FocusGood.GetNeed() == 2 && FocusGood.GetNeedLevel() != 0)
		{
			sprintf(str,"需要魔法力%d",FocusGood.GetNeedLevel());

			this->AddText(str,0xFFFFFFFF);
		}
		else if( FocusGood.GetNeed() == 3 && FocusGood.GetNeedLevel() != 0)
		{
			sprintf(str,"需要道术%d",FocusGood.GetNeedLevel());

			this->AddText(str,0xFFFFFFFF);
		}
	}

	if(IsCanLockStdMode(FocusGood) && FocusGood.GetFlag2() > 0)  //显示物品锁定剩余时间
	{
		int hour = FocusGood.GetFlag2();
		sprintf(str,"锁定有效时间还剩约:%d小时",hour);
		this->AddText(str,0xFFFFFF00);
		return;
	}
	//if(FocusGood.GetNeed() == 7) //只有元神的东东need属性为7
	//{
	//	this->AddText("元神装备",0xFFFFFF00);
	//	if(FocusGood.GetNeedMerLevel()>=1 && FocusGood.GetNeedMerLevel()<=3)
	//	{
	//		if(FocusGood.GetNeedMerLevel()==1)
	//		{
	//			this->AddText("需要培元境界",0xFFFFFF00);
	//		}
	//		else if(FocusGood.GetNeedMerLevel()==2)
	//		{
	//			this->AddText("需要明心境界",0xFFFFFF00);
	//		}
	//		else if(FocusGood.GetNeedMerLevel()==3)
	//		{
	//			this->AddText("需要大悟境界",0xFFFFFF00);
	//		}
	//	}
	//}

	if(iStdMode == 59 && iShape >= 10)
	{
		//圣殿宝石
		this->AddText("双击操作可提升武器上的隐藏技能属性的威力",0xFFFFFF00);
	}

	if (iStdMode == 90 && (iShape>= 1 && iShape <= 4))//矿石碎沫等
	{
		sprintf(str,"持久%d",FocusGood.GetDura());
		this->AddText(str,TIPS_WHITE);
	}

  return ;

}

void CGoodInfo::ParseAttackSkillInfo(CGood& FocusGood)
{
	int iSkill = FocusGood.GetAttachSkill();

	if(!iSkill) return;
	char ctemp[128] ={0};
	int iPercent = FocusGood.GetAttachSkillLevel() * 3;
	switch(iSkill)
	{
	case MAGICID_BLOOD_SHADE:
		{
			sprintf(ctemp,"增加血影刀法威力:%d%%",iPercent);
		}
		break;
	case MAGICID_REMAIN_SHADE:
		{
			sprintf(ctemp,"增加残影刀法威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ATTACK_KILL:
		{
			//攻杀剑法
			sprintf(ctemp,"增加攻杀剑法威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ATTACK_STICK:
		{
			//刺杀剑术
			sprintf(ctemp,"增加刺杀剑术威力:%d%%",iPercent);
		}
		break;
	case MAGICID_THUNDER_FIRE:
		{
			//雷霆剑法
			sprintf(ctemp,"增加雷霆剑法威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ATTACK_FIRE:
		{
			//烈火剑法
			sprintf(ctemp,"增加烈火剑法威力:%d%%",iPercent);
		}
		break;
	case MAGICID_WHOLE_MOON:
		{
			//抱月刀
			sprintf(ctemp,"增加抱月刀威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ATTACK_MOON:
		{
			//半月弯刀
			sprintf(ctemp,"增加半月弯刀威力:%d%%",iPercent);
		}
		break;
		//道士
	case MAGICID_PROTECT_SYMBOL:
		{
			//灵魂道符
			sprintf(ctemp,"增加灵魂道符威力:%d%%",iPercent);
		}
		break;
	case MAGICID_FIRE_JUJU:
		{
			//幽冥火咒
			sprintf(ctemp,"增加幽冥火咒威力:%d%%",iPercent);
		}
		break;
	case MAGICID_POISON_MAGIC:
		{
			//施毒术
			sprintf(ctemp,"增加施毒术威力:%d%%",iPercent);
		}
		break;
	case MAGICID_CURSE:
		{
			//诅咒术
			sprintf(ctemp,"增加诅咒术威力:%d%%",iPercent);
		}
		break;
		//法师
	case MAGICID_FIREBALL:
		{
			//小火球
			sprintf(ctemp,"增加小火球威力:%d%%",iPercent);
		}
		break;
	case MAGICID_HELL_FIRE:
		{
			//地狱火焰
			sprintf(ctemp,"增加地狱火焰威力:%d%%",iPercent);
		}
		break;
	case MAGICID_THUNDER:
		{
			//雷电术
			sprintf(ctemp,"增加雷电术威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ICE_ARROW:
		{
			//冰箭术
			sprintf(ctemp,"增加冰箭术威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ROTATE_FIRE:
		{
			//风火轮
			sprintf(ctemp,"增加风火轮威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ADV_FIREBALL:
		{
			//火炎刀
			sprintf(ctemp,"增加火炎刀威力:%d%%",iPercent);
		}
		break;
	case MAGICID_BLOW_FIRE:
		{
			//爆烈火焰
			sprintf(ctemp,"增加爆烈火焰威力:%d%%",iPercent);
		}
		break;
	case MAGICID_FIRE_WALL:
		{
			//火墙
			sprintf(ctemp,"增加火墙威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ICE_DRAG:
		{
			//冰龙破
			sprintf(ctemp,"增加冰龙破威力:%d%%",iPercent);
		}
		break;
	case MAGICID_TRACE_THUNDER:
		{
			//疾光电影
			sprintf(ctemp,"增加疾光电影威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ICE_ARRAY:
		{
			//玄冰刃
			sprintf(ctemp,"增加玄冰刃威力:%d%%",iPercent);
		}
		break;
	case MAGICID_HELL_THUNDER:
		{
			//地狱雷光
			sprintf(ctemp,"增加地狱雷光威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ICE_STORM:
		{
			//冰咆哮
			sprintf(ctemp,"增加冰咆哮威力:%d%%",iPercent);
		}
		break;
	case MAGICID_MULTI_THUDER:
		{
			//五雷轰
			sprintf(ctemp,"增加五雷轰威力:%d%%",iPercent);
		}
		break;
	case MAGICID_ICE_WHIRLWIND:
		{
			//冰旋风
			sprintf(ctemp,"增加冰旋风威力:%d%%",iPercent);
		}
		break;
	case MAGICID_DRAGON_LIGHT:
		{
			//狂龙紫电
			sprintf(ctemp,"增加狂龙紫电威力:%d%%",iPercent);
		}
		break;
	case MAGICID_FIRE_RAIN:
		{
			//流星火雨
			sprintf(ctemp,"增加流星火雨威力:%d%%",iPercent);
		}
		break;
	}

	if(strlen(ctemp) > 0) this->AddText(ctemp,0xFFFFFF00);
}

bool  CGoodInfo::IsCanLockStdMode(CGood & temp)
{
	if(
		temp.GetStdMode() == 18 || //[20051128][超级技能项链]
		temp.GetStdMode() == 29 || //灵狗神佑
		temp.GetStdMode() == 33//马牌
		|| temp.GetStdMode() == 49//马票
		|| (temp.GetStdMode() == 82 ||	temp.GetStdMode() == 83)	//鉴定类物品未鉴定
		|| temp.GetStdMode() == COMPOUND_GEM_STDMODE	//合成宝石
		|| ( temp.GetStdMode()==3 && (	temp.GetStdMode()==15 || 	temp.GetStdMode()==17)    )//回城石
		|| temp.GetStdMode() == TIME_ITEM_STDMODE	//存在时间(创建时间、有效期)的物品
		|| temp.GetStdMode() == SETEXT_ITEM_STDMODE
		|| ( temp.GetStdMode()==3 && temp.GetStdMode()==18 )	//还魂石
		|| temp.GetStdMode() == 70			//[元神]
		|| temp.GetStdMode() == 72			//[附加属性保存的信息（前10个字节）需要显示的特殊道具：天缘玉佩，师徒心锁]
		|| ( temp.GetStdMode() == 37 && temp.GetStdMode() == 11 || temp.GetStdMode() == 49 && temp.GetStdMode() == 22  )//特殊衣服(御兽天袍/御兽天衣)
		)

		return false;

	if (temp.GetStdMode() != 4	&& temp.GetStdMode() != 5	&&
		temp.GetStdMode() != 6	&& temp.GetStdMode() != 10	&&
		temp.GetStdMode() != 11	&& temp.GetStdMode() != 15	&&
		temp.GetStdMode() != 26	&& temp.GetStdMode() != 58	&&
		temp.GetStdMode() != 59	&& temp.GetStdMode() != 30	&&
		temp.GetStdMode() != 61	&& temp.GetStdMode() != 70	&&
		!(temp.GetStdMode() > 18 && temp.GetStdMode() < 25)	&&
		!(temp.GetStdMode() > 80 && temp.GetStdMode() < 84))
	{
		return false;
	}

	return true;
}
