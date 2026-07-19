#pragma once

#include "Global/Global.h"
#include "GameData/Good.h"

class CAIGoodMgr
{
public:
	CAIGoodMgr(void);
	~CAIGoodMgr(void);

	bool FilterUseGood(CGood& tmp);
	bool IsLingfu(CGood& tmp);
	bool IsLinkGood(CGood& tmp);
	UINT GetObjectAttr(CGood& tmp);
	UINT GetObjectAttrByLooks(WORD wGoodLooks);
	bool IsNotAuth(CGood& tmp);      //是否未鉴定物品
	bool IsBackHome(CGood& tmp);
	bool IsReliveStone(CGood& tmp);
	bool IsReliveSymbol(CGood& tmp);
	bool IsRiderClothes(CGood& tmp);
	bool IsCanLock(CGood& tmp);
	bool IsCanLettering(CGood& tmp);
	bool IsLeopardStone(CGood& tmp);
	bool IsCanUseByDClick(CGood& tmp);    //道具栏中是否可以双击使用
	bool IsCanContain(CGood& tmp,int pos);
	bool IsCanKeZi(CGood & temp);  //是否可刻字
	bool IsFitBelt(CGood & temp); //是否可以加入快捷栏

	bool IsValidPoisonCurse(CGood& tmp,bool bNeedGreen);
	bool IsValidPoisonCurse(CGood& tmp,bool bNeedGreen,bool bNeedPoison);
	bool IsValidDaoFu(CGood& tmp);
	bool IsMiXianLuYing(CGood& tmp);
	bool IsLingShouXiangQuan(CGood& tmp);
	bool IsCanZhang(CGood& tmp);		//是否是骑站残章技能书
	bool IsYuanShi(CGood& tmp);
	bool IsJiGuanYaoShi(CGood& tmp);
	int GetYuanShiLooks(int iShape,int iLevel);		//通过原石的looks获得
	int GetCanDigHoles(CGood& tmp);
	int GetDigHole(CGood& tmp);
	int GetLevel(CGood& tmp);
	bool IsCanDigHoleEquip(CGood& tmp);
	bool IsEquip(CGood& tmp);//是否装备
	bool IsCanLevelUpEquip(CGood& tmp);

	bool IsEquipWeapon(CGood& tmp);//武器
	bool IsEquipCloth(CGood& tmp);//衣服
	//bool IsEquipTneupChart(CGood& tmp);//群英阵谱
	bool IsEquipHelmet(CGood& tmp);//头盔
	bool IsEquipNecklace(CGood& tmp);//项链
	bool IsEquipMedal(CGood& tmp);//勋章
	bool IsEquipBelt(CGood& tmp);//腰带
	bool IsEquipBoot(CGood& tmp);//靴子
	bool IsEquipGem(CGood& tmp);//宝石
	bool IsEquipBangle(CGood& tmp);//手镯
	bool IsEquipRing(CGood& tmp);//戒指
	bool IsEquipShield(CGood& tmp);//盾牌
	int  GetEquipPos(CGood& tmp,bool bLeft = true);//获得这个物品可以装备到装备栏的位置
	bool IsEquipSoulCrystal(CGood& tmp);//是不是器魂结晶

	bool IsFuShi(CGood& tmp);//是不是符石
	bool IsLingFuShi(CGood &tmp);//是不是灵符石
	int GetFuShiAddAttack(int iKind,int iLevel);	
	int GetFuShiAddDefense(int iKind,int iLevel);
	int GetFuShiAddHP(int iKind,int iLevel);
	DWORD GetSoulLevelUpExp(CGood& tmp,int &iMaxAddAtrrItems,int iLevel = -1);//iLevel取哪一个等级的升级经验，如果为-1表示取当前等级的
	string GetSoulEquipAttr(CGood& tmp,int iLevel);
	bool LoadSoulLevelUpExp();

	string GetWingNameByLevel(int level);
	DWORD GetWingColorByLevel(int level);

	DWORD GetAddTogetherEffTexID(CGood& src,CGood& dest);//如果可以叠加在一起,返回叠加时的特效id,否则返回0

	bool IsWenPei(CGood& tmp);
	bool IsSubWenPei(CGood& tmp);

	int CompareInPackage(CGood* first, CGood* second);		//包裹排序使用
	bool IsShengLing(CGood& tmp);//是不是圣灵
	bool IsJingShengLing(CGood& tmp);//是不是纯净圣灵


	//法宝
	bool IsFaBao(CGood &tmp);
	float  GetFaBaoQiHeDu(CGood &tmp);//契合度
	int  GetFabaoWuXing(CGood &tmp);//五行
	int  GetFabaoPinJie(CGood &tmp);//品阶
	int  GetFabaoMinAttack(CGood &tmp);//最小攻击力
	int  GetFabaoMaxAttack(CGood &tmp);//最小攻击力
	int  GetFaBaoMinZhong(CGood &tmp);//命中
	int  GetFaBaoBaoJi(CGood &tmp);//暴击
	int  GetFaBaoXingYun(CGood &tmp);//幸运
	int  GetFaBaoPoFang(CGood &tmp);//破防
	int  GetFaBaoWuXingAddAttack(CGood &tmp);//五行攻击加成
	int  GetFaBaoWuXingReduceDefence(CGood &tmp);//五行防御减免
	int  GetFaBaoLingGenAddLowAttack(CGood &tmp);//法宝灵根增加的攻击下限加成
	int  GetFaBaoLingGenAddHighAttack(CGood &tmp);//法宝灵根增加的攻击下限加成
	int  GetFaBaoNeedFireLevel(CGood &tmp);//法宝需要人物三昧真火等级
	int  GetFaBaoLucky(CGood &tmp);//法宝幸运

	//end法宝

	bool IsLianHuLu(CGood &tmp);//炼葫芦
	//int TianDiRenByShape(int shape);

	bool IsChiLianHuFu(CGood &tmp);//赤金护符

	//骑术修炼书
	bool IsQiShuXiuLianBook(CGood &tmp);

	//天佑中州
	bool IsTianYouZhongZhou(CGood &tmp);

	//豹魔石
	bool IsPetStone(CGood &tmp);

protected:
	struct SoulLevelUpExp 
	{
		SoulLevelUpExp()
		{
			for (int i = 0; i < MAX_EQUIP_SOUL_LEVEL; i++)
			{
				dwExp[i] = 0;
			}
			iAddAtrrItems = 0;
		}

		int iAddAtrrItems;//最多可以激活多少个属性
		DWORD dwExp[MAX_EQUIP_SOUL_LEVEL];//第1,2,3级升级所需经验
	};

	struct SoulEquipAttr 
	{
		SoulEquipAttr()
		{
			for (int i = 0; i < MAX_EQUIP_SOUL_LEVEL; ++i)
			{
				addAttr[i].clear();
			}
		}

		string addAttr[MAX_EQUIP_SOUL_LEVEL];//每一级不同的属性加成
	};

	map<int,SoulLevelUpExp> m_MSoulLevelUpExp;
	map<string,SoulEquipAttr> m_MSoulEquipAttr;

};

extern CAIGoodMgr g_AIGoodMgr;
