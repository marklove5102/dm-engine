////////////////////////////////////////////////////////////////////////////////////////
// 主角和其他角色dwReserveData用途

#pragma once

// 主角和其他角色的公用部分
enum PUBLIC_RESERVE
{
	// 注意添加在pubNums以前
	pubFrameCount	= 0,				// 帧计数（从角色产生出来draw开始的计数）
	pubAppTime,							// 角色出现的时间

	// 角色说话相关
	pubBoolSay,							// 是否显示说话
	pubSayTime,							// 最近一次说话的时间
	pubTalkColor,						// 说话的颜色
	pubTalkWidth,						// 说话框的宽度
	pubTalkHeight,						// 说话框的高度

	pubActions,							// 队列中的动作个数
	pubDisableDraw,						// 禁止显示
	pubAttackedCount,					// 被攻击计数器
	pubHuanCaiCloth,                    // 是否装备了幻彩套装

	pubTeleport,
	pubGradualAppear,
	pubGradualTime,
	pubRunTransiteState,
	pubStandTrans,
	pubHasWings,

	pubStampFrame,                      // 突显玩家自己或是身上有特殊道具标示(活动专用)的帧计数
	pubHideMonster,

	pubHushenColor,
	pubHushenHP,

	pubHurtedTime,
	pubLastHurtedTime,
	pubLastCloseId,

	pubLastEffDodgeTime,                //风影盾特效时间
	pubSteelAttackedEff,                //金刚受攻击特效帧控制
	pubSoulWallState,                   //怪物灵魂墙状态1：开始，2结束，3：正常，4被攻击
	pubSoulWallFrame,                   //怪物灵魂墙绘制帧计数
	pubSoulWallColor,                   //怪物灵魂墙绘制颜色
	pubRingOfJD,						//禁地魔王状态变量
	pubBoneState,						//标记为尸骨
	pubChristmasTreePos,                //圣诞树位置
	plyZhenYuanStart,                   //真元攻击的开始时间
	plyZhenYuanConTm,                   //真元攻击的持续时间
	pubShadowKill8FrameStart,           //八方分影斩开始帧
	// 添加在上面
	pubNums,							// 公用的个数
};

// 主角自身使用部分
enum PLAYER_RESERVE
{
	// 注意添加在plyNums以前
	plyEnableFriend	= pubNums,			// 是否允许添加好友
	plyFilterMessage,					// 消息过滤标识
	plyCantUserObj,						// 是否可以使用物品

	plyAttackType,                      //主角攻击模式,取PLAYER_ATTACK_TYPE中值
	plyMagicLockID,                     //魔法攻击锁定ID
	plyAttackLockID,                    //物理攻击锁定ID
	plyClickCharID,                     //点中的角色id,脚下有光圈

	plyUnReadMsg,						// 当前屏蔽的频道有新消息
	plyOOKickOut,						// 版本过低被踢出来
	plyWGInfo,							// 客户端外挂使用状态 0:没用外挂 1:智能打怪 2:梦幻小秘 ...
	plyRungateLastAskTime,				//上一次请求RunGate时间
	plySendRungateNums,                 // 发送给Rungate的协议数目
	plyJumpMap,                         //是否在跳地图
	plyDBClickOtherQuipPos,             //双击了其它玩家纸娃娃界面的那个位置的装备
	plyPhrase,                          //读取网页中的短语，非0表示正在读取或者读取失败
	plyRedGemCount,                     //玩家的红宝石数量
	plyBlueGemCount,                    //玩家的蓝宝石数量
	plyGreenGemCount,                   //玩家的绿宝石数量
	plyDrinkSuperRed,                   //是否喝了超超红
	plyDrinkSuperBlue,                  //是否喝了超超蓝
	plyLastInviteAddGuild,              //上一次请求别人加入行会的时间
	plyLastInviteAddFriend,             //上一次请求别人加为好友的时间
	plyLastInviteTeacher,               //上一次请求拜师的时间
	plyLastInviteStudent,               //上一次请求招徒的时间
	plyLastInviteTrade,                 //上一次请求交易的时间
	plyLastMerStatus,                   //元神的最后的模式
	plyShenYouStartTime,                //神佑开始时间
	plyShenYouLastTime,                 //神佑剩余时间
	plyLockMonsterTime,                 //上一次收到困魔咒效果的的时间
	plyFireWallTime,                    //上一次收到火墙效果的的时间
	plyLastTurnTime,                    //上一次转方向的时间
	plyLastUseObjectTime,               //上一次使用物品的时间
	plyLastExchangeTrusteeshipObjectTime,//上一次放置或取出拖管包裹物品的时间,
	plyLastRejectObjectTime,            //上一次使用物品的时间
	plyRungateNotAnswer,                //RunGate没有响应
	plySuddenKillFrame,					//记录突斩播放帧数
	plyWaitServerTimes,                 //等待gs回应的次数
	plyNoWaitServerTimes,               //不需要等待的次数,也就是及时响应的次数
	// 添加在上面
	plyNums,							// 玩家自身使用的个数
};

//主角的攻击模式
enum PLAYER_ATTACK_TYPE
{
	ATTACK_TYPE_SE    = 0, //善恶攻击
	ATTACK_TYPE_ALL      , //全体攻击
	ATTACK_TYPE_PEACE    , //和平攻击
	ATTACK_TYPE_GROUP    , //编组攻击
	ATTACK_TYPE_GUILD    , //行会攻击
	ATTACK_TYPE_ST       , //师徒攻击
	ATTACK_TYPE_WH       , //夫妻攻击
};