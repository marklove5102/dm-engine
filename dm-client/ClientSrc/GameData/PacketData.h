//********************************** PLAYER_PROPERTY **************************************
// 结构的内部具体成员
 
struct tagPLAYER_PROPERTY
{
	DWORD dwID;				//人物ID
	UINT64 dwDigitID;        //数字ID

	//4
	BYTE		byDirection;		//方向
	BYTE		bySex;				//性别
	BYTE		byLevel;			//人物等级
	BYTE		byDisplayColor;		//名字颜色

	//8
	DWORD		dwStatus;			//人物状态

	//12
	BYTE		byAttackSpeed;		//攻击速度
	BYTE		byWalkSpeed;
	BYTE		bySP;				//用以换取技能点数
	BYTE		byJob;				//职业

	WORD		wCurPosX;		// X坐标
	WORD		wCurPosY;		// Y坐标

	//16
	char sTitle[DATA_LEN_TITLE];		//头衔
	//112
	char sApprMap[DATA_LEN_APPR];		//地图上外观
	char sApprEquip[DATA_LEN_APPR];		//装备上外观

		//128
	DWORD		dwMoney;			//金币
	DWORD		dwTargetID;			//人物锁定的ID

		//136
	WORD		wCurWeight;			//当前负重
	WORD		wMaxWeight;			//最大负重
	WORD		wPetPkgCurWeight;	//宠物包裹当前负重
	WORD		wPetPkgMaxWeight;	//宠物包裹最大负重

		//144
	DWORD		dwCurExp;			//当前经验值
	DWORD		dwMaxExp;			//升级经验值

		//152
	WORD		wCurHP;				//当前Hp
	WORD		wMaxHP;				//Hp上限
	WORD		wCurMP;				//当前Mp
	WORD		wMaxMP;				//Mp上限

		//160
	WORD		wDC1;				//物理攻击下限
	WORD		wDC2;				//物理攻击上限
	WORD		wMC1;				//魔法下限
	WORD		wMC2;				//魔法上限

	WORD		wAC1;				//物理防御下限
	WORD		wAC2;				//物理防御上限
	WORD		wMAC1;				//魔法防御下限
	WORD		wMAC2;				//魔法防御上限

		//176
	BYTE		byDamageAvoid;		//物理躲避
	BYTE		byMagicAvoid;		//魔法躲避
	BYTE		byPrecision;		//准确度

	//180
	char sName[DATA_LEN_NAME];		//名字
	char sGuildTitle[DATA_LEN_GUILDTITLE];//旅团名

		//212
	DWORD		dwSwitch;			//组队、旅团开关等

	//216
	DWORD      dwMonsterID;        //宠物指针
	DWORD      dwPetBackground;    //宠物背景图编号

	//224
	DWORD      dwStatusEx;         //人物状态扩展

	//228
	DWORD		dwDeposit;			//存款

	//232
	BYTE		byGroupFlag;		//组队 低四位：0：普通；1：队员；2：队长 高四位：颜色
	BYTE		byIsFree;
	WORD		wYuanbao;			//元宝

	//236
	DWORD      dwRank;             //榜单信息

	//240
	WORD		wPortraitNo;		//头像编号
	WORD		wPortraitType;		//头像表情

	//244
	DWORD      dwLover;            //行为伙伴

	//248
	DWORD      dwStatusEx2;		//人物状态扩展2

	//252
	BYTE       byDesignation;		//人物称号
	BYTE       byPKStatus;		    //人物pk状态
	short      nValidHoodleNums;	//玩家身上有效弹珠个数

	//256
	DWORD	   dwRepute;			//玩家的声望值

	//260
	short	   nHalfcostSeconds;	//玩家剩余的祝福时间（秒）
	BYTE	   byEnergy;            //精力值
	BYTE	   byTalent;			//天赋

	//264
	DWORD	   dw15ExpTime;         //1.5倍时间

	//268
	BYTE	   byExpMul;            //xx倍经验值
	BYTE	   byReserve;
	WORD	   wReserve;

	//272
};


//********************************** CREATURE_PROPERTY **************************************

struct tagCREATURE_PROPERTY
{
	DWORD		dwID;			//ID

//4
	BYTE		byDirection;		//方向
	BYTE		bySex;				//性别
	BYTE		byLive;				//0：出现；1：死亡出现
	BYTE		byDisplayColor;		//名字显示颜色

//8
	DWORD		dwStatus;			//状态

//12
	BYTE		byAttackSpeed;		//攻击速度
	BYTE       byWalkSpeed;        //移动速度
	BYTE		byScale;            //宠物缩放
	BYTE		byJob;				//职业

//16
	char sTitle[DATA_LEN_TITLE];		//头衔
	char sApprMap[DATA_LEN_APPR];		//地图上外观

//120
	WORD		wCurPosX;			//X坐标
	WORD		wCurPosY;			//Y坐标

//124
	WORD		wHpPercent;		//血量百分比
	WORD		wStatusEx;		//怪物扩展状态 //第0位表示是否飞过去进行物理攻击，例如戟兵卫
                                                       //第1位表示是否圣火怪物 0 不是 1 是
													   //第2位表示是否是boss
													   //第3位表示是否能穿过去 0 不能 1 能
													   //第4位表示是否血条旁边显示等级 0 显示 1 不显示

	BYTE		byGroupFlag;		//组队 低四位：0：普通；1：队员；2：队长 高四位：颜色
	BYTE		byMonsterLevel;     //怪物等级
	BYTE		byEnemy;            //是否是敌人 0不是 1是
	BYTE		byReserved;

//132
	DWORD      dwLover;            //行为伙伴

//136
	DWORD	    dwStatusEx3;       //怪物扩展状态3

//140
};

//********************************** ITEM_PACKET **************************************

struct tagITEM_PACKET
{
	DWORD		dwType;			//类型。0：物品；1：宠物蛋
	DWORD		dwUniqueID;			//物品ID
	DWORD		dwPrice;			//价格

//12
	char szName[DATA_LEN_NAME];		//名称

//28
	WORD		wStdMode;			//StdMode
	WORD		wReserved;			//

//32
	BYTE		byComRate;			//提升的合成概率
	BYTE		byShape;			//Shape
	BYTE		byWeight;			//Weight
	BYTE		byItemLevel;		//物品等级

//36
	WORD		wLooks;				//外观
	WORD		wDuraMax;			//最大持久

//40
	WORD		wDC1;				//物理攻击下限
	WORD		wDC2;				//物理攻击上限
	WORD		wMC1;				//魔法下限
	WORD		wMC2;				//魔法上限
	WORD		wAC1;				//物理防御下限
	WORD		wAC2;				//物理防御上限
	WORD		wMAC1;				//魔法防御下限
	WORD		wMAC2;				//魔法防御上限

//56
	BYTE		byNeed;				//需要的类型
	BYTE		byNeedLevel;		//需要的等级
	BYTE		byPrecision;		//精确
	BYTE		byNameColor;		//名字颜色

//60
	WORD		wItemDura;			//当前持久
	WORD		wMaxItemDura;		//恢复持久的最大值

//64
	WORD		wRecoverHP;		//HP恢复量
	WORD		wRecoverMP;			//MP恢复量
	WORD		wMergeNum;			//药品合并的个数
	BYTE		byDamageAvoid;		//物理躲避
	BYTE		byMagicAvoid;		//魔法躲避

//72
	char szItemExt[DATA_LEN_ITEM_EXT];	//物品特殊附加属性

//92
	DWORD      dwItemFlag;         //物品锁定标志等...

//94
	char szExtName[DATA_LEN_NAME];		//附加名字
	char szSetName[DATA_LEN_NAME];		//套装名
	char szSetExtPro[DATA_LEN_ITEM_SET_EXT];	//套装附加属性

//144
	BYTE		byExgType;			//0 (普通或默多克)；1 梦幻币；2 只接受点券购买 ；4 只是声望
	BYTE		bySCType;			//0：普通；1：新品；2：折扣
	BYTE		byItemFlag;			//0：正常；1：饰品已使用
	BYTE		byRes;				//

//148
	char sReserved[8];

//156
};

//********************************** MAGIC_PACKET **************************************

struct tagMAGIC_PACKET
{
	WORD		wMagicID;			//魔法ID
	BYTE		byMagicLevel;		//魔法等级
	BYTE		byShortCut;			//快捷键

//4
	char szMagicName[DATA_LEN_MAGIC_NAME];//魔法名

//20
	WORD		wMPCost;			//所耗魔法
	WORD		wCooldown;			//连续释放同一魔法的最小时间间隔
	WORD		wInterval;			//连续释放不同魔法的最小时间间隔
	WORD		wFreezetime;		//吟唱类魔法的吟唱时间

//28
	BYTE		byJob;				//职业
	BYTE		byNeedLevel;		//等级
	BYTE		byIsLearned;		//1：已学习；2：可以学习
	BYTE		byReserved;

//32
	char szReserved[8];					//

//40
};

//*******************************************************************************************
//
//#undef	T_BEGIN
//#undef	T_END
//#undef	VAR
//#undef	STR