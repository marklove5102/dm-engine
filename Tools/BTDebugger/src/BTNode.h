#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <map>

// ============================================================================
// 行为树节点类型枚举
// 与 GameServer/BotBehaviorTree.cpp 的工厂表完全对齐（共 54 种）
//   复合 7 / 装饰 7 / 条件 17 / 动作 23
// ============================================================================
enum class BTNodeType
{
	// 复合节点（7）
	SEQUENCE, SELECTOR, PARALLEL, RANDOM, PROBABILITY,
	MEM_SEQUENCE, MEM_SELECTOR,
	// 装饰节点（7）
	INVERTER, DECORATOR_REPEAT, DECORATOR_TIMEOUT, DECORATOR_COOLDOWN,
	DECORATOR_PERIODIC, SUCCEEDER, FAILER,
	// 条件节点 / 动作节点
	CONDITION, ACTION
};

// 节点分类
enum class BTNodeCategory
{
	COMPOSITE, DECORATOR, CONDITION, ACTION
};

// 执行结果（与 GameServer BTResult 对齐，IDLE 为调试器内部初始态）
enum class BTResult
{
	SUCCESS, FAILURE, RUNNING, IDLE
};

// ============================================================================
// 条件子类型（17）- 对齐 GameServer BotBehaviorCondition.h
// ============================================================================
enum class ConditionType
{
	NONE,
	LOW_HP, LOW_MP, HAS_TARGET, IN_SAFE_AREA, BAG_FULL,
	HAS_ITEM, SKILL_READY, IS_DEAD, TARGET_DISTANCE, TARGET_TYPE,
	HAS_BUFF, HAS_NEARBY_PLAYER, MONSTER_COUNT, HAS_POTION,
	HAS_DROPPED_ITEM, HP_RANGE, TIME_OF_DAY
};

// ============================================================================
// 动作子类型（23）- 对齐 GameServer BotBehaviorAction.h 工厂表
// ============================================================================
enum class ActionType
{
	NONE,
	USE_POTION, USE_ITEM, CHANGE_ATTACK_MODE, ATTACK,
	MOVE_TO_TARGET, PATROL, PICKUP_ITEM, FLEE, REST, REVIVE,
	CHAT, USE_SKILL, SAY, RECALL, DELAY, ATTACK_DIR, SPELL_CAST,
	DROP_ITEM, EQUIP_ITEM, SUMMON_PET, FOLLOW, GROUP, MINE
};

// ============================================================================
// 行为树节点
// ============================================================================
struct BTNode
{
	std::wstring id;
	BTNodeType type = BTNodeType::ACTION;
	ConditionType conditionType = ConditionType::NONE;
	ActionType actionType = ActionType::NONE;
	std::wstring name;
	std::map<std::wstring, std::wstring> params;
	std::vector<std::shared_ptr<BTNode>> children;
	BTNode* parent = nullptr;
	int depth = 0;
	bool collapsed = false;

	// 执行状态
	BTResult lastResult = BTResult::IDLE;
	bool isActive = false;
	int execOrder = -1;
};

// 执行日志条目
struct LogEntry
{
	std::wstring nodeId;
	std::wstring nodeName;
	BTResult result;
	int depth;
	BTNodeType type;
};

// ============================================================================
// 执行上下文（模拟 GameServer CBotContext/CBotPlayer 的查询接口）
// 调试器通过该结构模拟机器人所处游戏状态，用于驱动条件节点判定
// ============================================================================
struct ExecutionContext
{
	// 自身状态
	int hpPercent = 80;          // HP 百分比 0-100
	int mpPercent = 70;          // MP 百分比 0-100
	bool isDead = false;         // 是否死亡
	bool inSafeArea = false;     // 是否在安全区
	bool bagFull = false;        // 背包是否已满

	// 目标相关
	bool hasTarget = true;       // 是否有攻击目标
	int targetDistance = 3;      // 与目标距离（格）
	int targetType = 0;          // 目标类型 0=怪物 1=玩家 2=宠物

	// Buff/状态（按 buffId 检测，0-5 见 ConditionHasBuff 注释）
	bool hasBuff = false;        // 是否持有指定 Buff（简化为单一开关）

	// 周围环境
	bool hasNearbyPlayer = false;// 附近是否有玩家
	int monsterCount = 2;        // 周围怪物数量
	bool hasDroppedItem = false; // 地上是否有掉落物
	int timeOfDay = 0;           // 游戏内时间 0=白天 1=夜晚 2=黄昏 3=黎明

	// 物品/技能
	bool hasItem = true;         // 是否持有指定物品
	bool hasPotionHP = true;     // 是否有 HP 药水
	bool hasPotionMP = true;     // 是否有 MP 药水
	bool skillReady = true;      // 技能是否就绪

	// 时间轴（ms），用于 DecoratorTimeout/Cooldown/Periodic 的模拟
	DWORD frameTime = 0;         // 当前帧时间
};

// 节点类型颜色 (RGB 格式)
inline COLORREF GetNodeColor(BTNodeType type)
{
	switch (type)
	{
	case BTNodeType::SEQUENCE:
	case BTNodeType::SELECTOR:
	case BTNodeType::PARALLEL:
	case BTNodeType::RANDOM:
	case BTNodeType::PROBABILITY:
	case BTNodeType::MEM_SEQUENCE:
	case BTNodeType::MEM_SELECTOR:
		return RGB(0x00, 0xAA, 0xFF);   // 复合节点 - 金色
	case BTNodeType::INVERTER:
	case BTNodeType::DECORATOR_REPEAT:
	case BTNodeType::DECORATOR_TIMEOUT:
	case BTNodeType::DECORATOR_COOLDOWN:
	case BTNodeType::DECORATOR_PERIODIC:
	case BTNodeType::SUCCEEDER:
	case BTNodeType::FAILER:
		return RGB(0xFF, 0x66, 0xCC);   // 装饰节点 - 紫色
	case BTNodeType::CONDITION:
		return RGB(0xFF, 0xCC, 0x00);   // 条件节点 - 青色
	case BTNodeType::ACTION:
		return RGB(0x44, 0x88, 0xFF);   // 动作节点 - 橙色
	}
	return RGB(0x88, 0x88, 0x88);
}

inline COLORREF GetResultColor(BTResult result)
{
	switch (result)
	{
	case BTResult::SUCCESS: return RGB(0x88, 0xFF, 0x00);
	case BTResult::FAILURE: return RGB(0x66, 0x44, 0xFF);
	case BTResult::RUNNING: return RGB(0xFF, 0x88, 0x44);
	case BTResult::IDLE:    return RGB(0x66, 0x55, 0x55);
	}
	return RGB(0x88, 0x88, 0x88);
}

inline BTNodeCategory GetNodeCategory(BTNodeType type)
{
	switch (type)
	{
	case BTNodeType::SEQUENCE:
	case BTNodeType::SELECTOR:
	case BTNodeType::PARALLEL:
	case BTNodeType::RANDOM:
	case BTNodeType::PROBABILITY:
	case BTNodeType::MEM_SEQUENCE:
	case BTNodeType::MEM_SELECTOR:
		return BTNodeCategory::COMPOSITE;
	case BTNodeType::INVERTER:
	case BTNodeType::DECORATOR_REPEAT:
	case BTNodeType::DECORATOR_TIMEOUT:
	case BTNodeType::DECORATOR_COOLDOWN:
	case BTNodeType::DECORATOR_PERIODIC:
	case BTNodeType::SUCCEEDER:
	case BTNodeType::FAILER:
		return BTNodeCategory::DECORATOR;
	case BTNodeType::CONDITION:
		return BTNodeCategory::CONDITION;
	case BTNodeType::ACTION:
		return BTNodeCategory::ACTION;
	}
	return BTNodeCategory::ACTION;
}

inline const wchar_t* GetNodeTypeName(BTNodeType type)
{
	switch (type)
	{
	case BTNodeType::SEQUENCE:          return L"序列(Sequence)";
	case BTNodeType::SELECTOR:          return L"选择(Selector)";
	case BTNodeType::PARALLEL:          return L"并行(Parallel)";
	case BTNodeType::RANDOM:            return L"随机(Random)";
	case BTNodeType::PROBABILITY:       return L"概率(Probability)";
	case BTNodeType::MEM_SEQUENCE:      return L"记忆序列(MemSequence)";
	case BTNodeType::MEM_SELECTOR:      return L"记忆选择(MemSelector)";
	case BTNodeType::INVERTER:          return L"反转(Inverter)";
	case BTNodeType::DECORATOR_REPEAT:  return L"重复(DecoratorRepeat)";
	case BTNodeType::DECORATOR_TIMEOUT: return L"超时(DecoratorTimeout)";
	case BTNodeType::DECORATOR_COOLDOWN:return L"冷却(DecoratorCooldown)";
	case BTNodeType::DECORATOR_PERIODIC:return L"周期(DecoratorPeriodic)";
	case BTNodeType::SUCCEEDER:         return L"强制成功(Succeeder)";
	case BTNodeType::FAILER:            return L"强制失败(Failer)";
	case BTNodeType::CONDITION:         return L"条件(Condition)";
	case BTNodeType::ACTION:            return L"动作(Action)";
	}
	return L"未知";
}

// 显示具体子类型名称（如 "条件 - 低血(LowHP)"）
inline std::wstring GetNodeTypeDetail(BTNodeType type, ConditionType ct, ActionType at)
{
	switch (type)
	{
	case BTNodeType::CONDITION:
		switch (ct)
		{
		case ConditionType::LOW_HP:           return L"条件 - 低血(LowHP)";
		case ConditionType::LOW_MP:           return L"条件 - 低蓝(LowMP)";
		case ConditionType::HAS_TARGET:       return L"条件 - 有目标(HasTarget)";
		case ConditionType::IN_SAFE_AREA:     return L"条件 - 安全区(InSafeArea)";
		case ConditionType::BAG_FULL:         return L"条件 - 背包满(BagFull)";
		case ConditionType::HAS_ITEM:         return L"条件 - 持有物品(HasItem)";
		case ConditionType::SKILL_READY:      return L"条件 - 技能就绪(SkillReady)";
		case ConditionType::IS_DEAD:          return L"条件 - 已死亡(IsDead)";
		case ConditionType::TARGET_DISTANCE:  return L"条件 - 目标距离(TargetDistance)";
		case ConditionType::TARGET_TYPE:      return L"条件 - 目标类型(TargetType)";
		case ConditionType::HAS_BUFF:         return L"条件 - 有Buff(HasBuff)";
		case ConditionType::HAS_NEARBY_PLAYER:return L"条件 - 附近玩家(HasNearbyPlayer)";
		case ConditionType::MONSTER_COUNT:    return L"条件 - 怪物数量(MonsterCount)";
		case ConditionType::HAS_POTION:       return L"条件 - 有药水(HasPotion)";
		case ConditionType::HAS_DROPPED_ITEM: return L"条件 - 掉落物(HasDroppedItem)";
		case ConditionType::HP_RANGE:         return L"条件 - 血量范围(HPRange)";
		case ConditionType::TIME_OF_DAY:      return L"条件 - 时间段(TimeOfDay)";
		default: return L"条件(Condition)";
		}
	case BTNodeType::ACTION:
		switch (at)
		{
		case ActionType::USE_POTION:          return L"动作 - 喝药(UsePotion)";
		case ActionType::USE_ITEM:            return L"动作 - 使用道具(UseItem)";
		case ActionType::CHANGE_ATTACK_MODE:  return L"动作 - 切攻击模式(ChangeAttackMode)";
		case ActionType::ATTACK:              return L"动作 - 攻击(Attack)";
		case ActionType::MOVE_TO_TARGET:      return L"动作 - 移向目标(MoveToTarget)";
		case ActionType::PATROL:              return L"动作 - 巡逻(Patrol)";
		case ActionType::PICKUP_ITEM:         return L"动作 - 拾取(PickupItem)";
		case ActionType::FLEE:                return L"动作 - 逃跑(Flee)";
		case ActionType::REST:                return L"动作 - 休息(Rest)";
		case ActionType::REVIVE:              return L"动作 - 复活(Revive)";
		case ActionType::CHAT:                return L"动作 - 聊天(Chat)";
		case ActionType::USE_SKILL:           return L"动作 - 使用技能(UseSkill)";
		case ActionType::SAY:                 return L"动作 - 说话(Say)";
		case ActionType::RECALL:              return L"动作 - 回城(Recall)";
		case ActionType::DELAY:               return L"动作 - 延迟(Delay)";
		case ActionType::ATTACK_DIR:          return L"动作 - 定向攻击(AttackDir)";
		case ActionType::SPELL_CAST:          return L"动作 - 精确施法(SpellCast)";
		case ActionType::DROP_ITEM:           return L"动作 - 丢弃(DropItem)";
		case ActionType::EQUIP_ITEM:          return L"动作 - 穿戴(EquipItem)";
		case ActionType::SUMMON_PET:          return L"动作 - 召唤宠物(SummonPet)";
		case ActionType::FOLLOW:              return L"动作 - 跟随(Follow)";
		case ActionType::GROUP:               return L"动作 - 组队(Group)";
		case ActionType::MINE:                return L"动作 - 挖矿(Mine)";
		default: return L"动作(Action)";
		}
	default:
		return GetNodeTypeName(type);
	}
}

inline const wchar_t* GetResultName(BTResult r)
{
	switch (r)
	{
	case BTResult::SUCCESS: return L"成功";
	case BTResult::FAILURE: return L"失败";
	case BTResult::RUNNING: return L"执行";
	case BTResult::IDLE:    return L"空闲";
	}
	return L"";
}

// ============================================================================
// 从 XML 标签名解析节点类型（标签名与 GameServer CreateNodeByTypeName 完全一致）
// ============================================================================
inline BTNodeType ParseNodeType(const std::wstring& tagName)
{
	if (tagName == L"Sequence")          return BTNodeType::SEQUENCE;
	if (tagName == L"Selector")          return BTNodeType::SELECTOR;
	if (tagName == L"Parallel")          return BTNodeType::PARALLEL;
	if (tagName == L"Random")            return BTNodeType::RANDOM;
	if (tagName == L"Probability")       return BTNodeType::PROBABILITY;
	if (tagName == L"MemSequence")       return BTNodeType::MEM_SEQUENCE;
	if (tagName == L"MemSelector")       return BTNodeType::MEM_SELECTOR;
	if (tagName == L"Inverter")          return BTNodeType::INVERTER;
	if (tagName == L"DecoratorRepeat")   return BTNodeType::DECORATOR_REPEAT;
	if (tagName == L"DecoratorTimeout")  return BTNodeType::DECORATOR_TIMEOUT;
	if (tagName == L"DecoratorCooldown") return BTNodeType::DECORATOR_COOLDOWN;
	if (tagName == L"DecoratorPeriodic") return BTNodeType::DECORATOR_PERIODIC;
	if (tagName == L"Succeeder")         return BTNodeType::SUCCEEDER;
	if (tagName == L"Failer")            return BTNodeType::FAILER;
	if (tagName.find(L"Condition") == 0) return BTNodeType::CONDITION;
	if (tagName.find(L"Action") == 0)    return BTNodeType::ACTION;
	return BTNodeType::ACTION;
}

inline ConditionType ParseConditionType(const std::wstring& tagName)
{
	if (tagName == L"ConditionLowHP")          return ConditionType::LOW_HP;
	if (tagName == L"ConditionLowMP")          return ConditionType::LOW_MP;
	if (tagName == L"ConditionHasTarget")      return ConditionType::HAS_TARGET;
	if (tagName == L"ConditionInSafeArea")     return ConditionType::IN_SAFE_AREA;
	if (tagName == L"ConditionBagFull")        return ConditionType::BAG_FULL;
	if (tagName == L"ConditionHasItem")        return ConditionType::HAS_ITEM;
	if (tagName == L"ConditionSkillReady")     return ConditionType::SKILL_READY;
	if (tagName == L"ConditionIsDead")         return ConditionType::IS_DEAD;
	if (tagName == L"ConditionTargetDistance") return ConditionType::TARGET_DISTANCE;
	if (tagName == L"ConditionTargetType")     return ConditionType::TARGET_TYPE;
	if (tagName == L"ConditionHasBuff")        return ConditionType::HAS_BUFF;
	if (tagName == L"ConditionHasNearbyPlayer")return ConditionType::HAS_NEARBY_PLAYER;
	if (tagName == L"ConditionMonsterCount")   return ConditionType::MONSTER_COUNT;
	if (tagName == L"ConditionHasPotion")      return ConditionType::HAS_POTION;
	if (tagName == L"ConditionHasDroppedItem") return ConditionType::HAS_DROPPED_ITEM;
	if (tagName == L"ConditionHPRange")        return ConditionType::HP_RANGE;
	if (tagName == L"ConditionTimeOfDay")      return ConditionType::TIME_OF_DAY;
	return ConditionType::NONE;
}

inline ActionType ParseActionType(const std::wstring& tagName)
{
	if (tagName == L"ActionUsePotion")         return ActionType::USE_POTION;
	if (tagName == L"ActionUseItem")           return ActionType::USE_ITEM;
	if (tagName == L"ActionChangeAttackMode")  return ActionType::CHANGE_ATTACK_MODE;
	if (tagName == L"ActionAttack")            return ActionType::ATTACK;
	if (tagName == L"ActionMoveToTarget")      return ActionType::MOVE_TO_TARGET;
	if (tagName == L"ActionPatrol")            return ActionType::PATROL;
	if (tagName == L"ActionPickupItem")        return ActionType::PICKUP_ITEM;
	if (tagName == L"ActionFlee")              return ActionType::FLEE;
	if (tagName == L"ActionRest")              return ActionType::REST;
	if (tagName == L"ActionRevive")            return ActionType::REVIVE;
	if (tagName == L"ActionChat")              return ActionType::CHAT;
	if (tagName == L"ActionUseSkill")          return ActionType::USE_SKILL;
	if (tagName == L"ActionSay")               return ActionType::SAY;
	if (tagName == L"ActionRecall")            return ActionType::RECALL;
	if (tagName == L"ActionDelay")             return ActionType::DELAY;
	if (tagName == L"ActionAttackDir")         return ActionType::ATTACK_DIR;
	if (tagName == L"ActionSpellCast")         return ActionType::SPELL_CAST;
	if (tagName == L"ActionDropItem")          return ActionType::DROP_ITEM;
	if (tagName == L"ActionEquipItem")         return ActionType::EQUIP_ITEM;
	if (tagName == L"ActionSummonPet")         return ActionType::SUMMON_PET;
	if (tagName == L"ActionFollow")            return ActionType::FOLLOW;
	if (tagName == L"ActionGroup")             return ActionType::GROUP;
	if (tagName == L"ActionMine")              return ActionType::MINE;
	return ActionType::NONE;
}

// ============================================================================
// 获取节点类型的默认参数
// 参数名严格对齐 GameServer BotBehaviorTree.cpp 的 QueryIntAttribute/Attribute 调用
// ============================================================================
inline std::map<std::wstring, std::wstring> GetDefaultParams(BTNodeType type, ConditionType ct, ActionType at)
{
	std::map<std::wstring, std::wstring> p;
	switch (type)
	{
	case BTNodeType::PROBABILITY:        p[L"chance"] = L"50"; break;
	case BTNodeType::DECORATOR_REPEAT:   p[L"count"] = L"3"; break;
	case BTNodeType::DECORATOR_TIMEOUT:  p[L"ms"] = L"5000"; break;
	case BTNodeType::DECORATOR_COOLDOWN: p[L"ms"] = L"3000"; break;
	case BTNodeType::DECORATOR_PERIODIC: p[L"ms"] = L"30000"; break;
	case BTNodeType::CONDITION:
		switch (ct)
		{
		case ConditionType::LOW_HP:           p[L"percent"] = L"30"; break;
		case ConditionType::LOW_MP:           p[L"percent"] = L"20"; break;
		case ConditionType::HAS_ITEM:         p[L"itemName"] = L"回城卷"; break;
		case ConditionType::SKILL_READY:      p[L"magicId"] = L"0"; break;
		case ConditionType::TARGET_DISTANCE:  p[L"minDist"] = L"0"; p[L"maxDist"] = L"16"; break;
		case ConditionType::TARGET_TYPE:      p[L"targetType"] = L"0"; break;
		case ConditionType::HAS_BUFF:         p[L"buffId"] = L"0"; break;
		case ConditionType::HAS_NEARBY_PLAYER:p[L"range"] = L"16"; break;
		case ConditionType::MONSTER_COUNT:    p[L"range"] = L"10"; p[L"mode"] = L"0"; p[L"count"] = L"3"; break;
		case ConditionType::HAS_POTION:       p[L"hpType"] = L"1"; break;
		case ConditionType::HAS_DROPPED_ITEM: p[L"range"] = L"5"; break;
		case ConditionType::HP_RANGE:         p[L"minPercent"] = L"0"; p[L"maxPercent"] = L"100"; break;
		case ConditionType::TIME_OF_DAY:      p[L"period"] = L"0"; break;
		default: break;
		}
		break;
	case BTNodeType::ACTION:
		switch (at)
		{
		case ActionType::USE_POTION:         p[L"hpType"] = L"1"; break;
		case ActionType::USE_ITEM:           p[L"itemName"] = L"回城卷"; break;
		case ActionType::CHANGE_ATTACK_MODE: p[L"attackMode"] = L"1"; break;
		case ActionType::REST:               p[L"duration"] = L"5000"; break;
		case ActionType::REVIVE:             p[L"hpPercent"] = L"100"; p[L"teleportHome"] = L"1"; p[L"delay"] = L"0"; break;
		case ActionType::USE_SKILL:          p[L"magicId"] = L"0"; break;
		case ActionType::SAY:                p[L"message"] = L"Hello"; break;
		case ActionType::RECALL:             p[L"type"] = L"0"; break;
		case ActionType::DELAY:              p[L"minMs"] = L"200"; p[L"maxMs"] = L"1000"; break;
		case ActionType::ATTACK_DIR:         p[L"dir"] = L"0"; break;
		case ActionType::SPELL_CAST:         p[L"magicId"] = L"0"; p[L"targetX"] = L"0"; p[L"targetY"] = L"0"; p[L"targetId"] = L"0"; break;
		case ActionType::DROP_ITEM:          p[L"itemName"] = L"物品"; p[L"count"] = L"1"; break;
		case ActionType::EQUIP_ITEM:         p[L"itemName"] = L"装备"; break;
		case ActionType::SUMMON_PET:         p[L"petName"] = L"骷髅"; p[L"count"] = L"1"; break;
		case ActionType::FOLLOW:             p[L"targetName"] = L""; p[L"followRange"] = L"3"; break;
		case ActionType::GROUP:              p[L"playerName"] = L""; p[L"action"] = L"0"; break;
		case ActionType::MINE:               p[L"duration"] = L"10000"; break;
		default: break;
		}
		break;
	default: break;
	}
	return p;
}

// 反向：从节点类型/条件类型/动作类型获取 XML 标签名
inline std::wstring GetTagName(BTNodeType type, ConditionType ctype, ActionType atype)
{
	switch (type)
	{
	case BTNodeType::SEQUENCE:          return L"Sequence";
	case BTNodeType::SELECTOR:          return L"Selector";
	case BTNodeType::PARALLEL:          return L"Parallel";
	case BTNodeType::RANDOM:            return L"Random";
	case BTNodeType::PROBABILITY:       return L"Probability";
	case BTNodeType::MEM_SEQUENCE:      return L"MemSequence";
	case BTNodeType::MEM_SELECTOR:      return L"MemSelector";
	case BTNodeType::INVERTER:          return L"Inverter";
	case BTNodeType::DECORATOR_REPEAT:  return L"DecoratorRepeat";
	case BTNodeType::DECORATOR_TIMEOUT: return L"DecoratorTimeout";
	case BTNodeType::DECORATOR_COOLDOWN:return L"DecoratorCooldown";
	case BTNodeType::DECORATOR_PERIODIC:return L"DecoratorPeriodic";
	case BTNodeType::SUCCEEDER:         return L"Succeeder";
	case BTNodeType::FAILER:            return L"Failer";
	case BTNodeType::CONDITION:
		switch (ctype)
		{
		case ConditionType::LOW_HP:           return L"ConditionLowHP";
		case ConditionType::LOW_MP:           return L"ConditionLowMP";
		case ConditionType::HAS_TARGET:       return L"ConditionHasTarget";
		case ConditionType::IN_SAFE_AREA:     return L"ConditionInSafeArea";
		case ConditionType::BAG_FULL:         return L"ConditionBagFull";
		case ConditionType::HAS_ITEM:         return L"ConditionHasItem";
		case ConditionType::SKILL_READY:      return L"ConditionSkillReady";
		case ConditionType::IS_DEAD:          return L"ConditionIsDead";
		case ConditionType::TARGET_DISTANCE:  return L"ConditionTargetDistance";
		case ConditionType::TARGET_TYPE:      return L"ConditionTargetType";
		case ConditionType::HAS_BUFF:         return L"ConditionHasBuff";
		case ConditionType::HAS_NEARBY_PLAYER:return L"ConditionHasNearbyPlayer";
		case ConditionType::MONSTER_COUNT:    return L"ConditionMonsterCount";
		case ConditionType::HAS_POTION:       return L"ConditionHasPotion";
		case ConditionType::HAS_DROPPED_ITEM: return L"ConditionHasDroppedItem";
		case ConditionType::HP_RANGE:         return L"ConditionHPRange";
		case ConditionType::TIME_OF_DAY:      return L"ConditionTimeOfDay";
		default: return L"Condition";
		}
	case BTNodeType::ACTION:
		switch (atype)
		{
		case ActionType::USE_POTION:         return L"ActionUsePotion";
		case ActionType::USE_ITEM:           return L"ActionUseItem";
		case ActionType::CHANGE_ATTACK_MODE: return L"ActionChangeAttackMode";
		case ActionType::ATTACK:             return L"ActionAttack";
		case ActionType::MOVE_TO_TARGET:     return L"ActionMoveToTarget";
		case ActionType::PATROL:             return L"ActionPatrol";
		case ActionType::PICKUP_ITEM:        return L"ActionPickupItem";
		case ActionType::FLEE:               return L"ActionFlee";
		case ActionType::REST:               return L"ActionRest";
		case ActionType::REVIVE:             return L"ActionRevive";
		case ActionType::CHAT:               return L"ActionChat";
		case ActionType::USE_SKILL:          return L"ActionUseSkill";
		case ActionType::SAY:                return L"ActionSay";
		case ActionType::RECALL:             return L"ActionRecall";
		case ActionType::DELAY:              return L"ActionDelay";
		case ActionType::ATTACK_DIR:         return L"ActionAttackDir";
		case ActionType::SPELL_CAST:         return L"ActionSpellCast";
		case ActionType::DROP_ITEM:          return L"ActionDropItem";
		case ActionType::EQUIP_ITEM:         return L"ActionEquipItem";
		case ActionType::SUMMON_PET:         return L"ActionSummonPet";
		case ActionType::FOLLOW:             return L"ActionFollow";
		case ActionType::GROUP:              return L"ActionGroup";
		case ActionType::MINE:               return L"ActionMine";
		default: return L"Action";
		}
	}
	return L"Action";
}

// ============================================================================
// 节点目录项 - 用于"添加子节点/更改类型"对话框与属性面板
// 完整列出 GameServer 支持的全部 54 种节点，保证调试器可编辑与服务端一致
// ============================================================================
struct BTNodeCatalogEntry
{
	BTNodeType type;
	ConditionType ct;   // 仅 CONDITION 有效
	ActionType at;      // 仅 ACTION 有效
	const wchar_t* displayName;
};

inline const std::vector<BTNodeCatalogEntry>& GetNodeCatalog()
{
	static const std::vector<BTNodeCatalogEntry> kCatalog = {
		// 复合节点（7）
		{ BTNodeType::SEQUENCE,           ConditionType::NONE, ActionType::NONE, L"Sequence (序列)" },
		{ BTNodeType::SELECTOR,           ConditionType::NONE, ActionType::NONE, L"Selector (选择)" },
		{ BTNodeType::PARALLEL,           ConditionType::NONE, ActionType::NONE, L"Parallel (并行)" },
		{ BTNodeType::RANDOM,             ConditionType::NONE, ActionType::NONE, L"Random (随机)" },
		{ BTNodeType::PROBABILITY,        ConditionType::NONE, ActionType::NONE, L"Probability (概率)" },
		{ BTNodeType::MEM_SEQUENCE,       ConditionType::NONE, ActionType::NONE, L"MemSequence (记忆序列)" },
		{ BTNodeType::MEM_SELECTOR,       ConditionType::NONE, ActionType::NONE, L"MemSelector (记忆选择)" },
		// 装饰节点（7）
		{ BTNodeType::INVERTER,           ConditionType::NONE, ActionType::NONE, L"Inverter (反转)" },
		{ BTNodeType::DECORATOR_REPEAT,   ConditionType::NONE, ActionType::NONE, L"DecoratorRepeat (重复)" },
		{ BTNodeType::DECORATOR_TIMEOUT,  ConditionType::NONE, ActionType::NONE, L"DecoratorTimeout (超时)" },
		{ BTNodeType::DECORATOR_COOLDOWN, ConditionType::NONE, ActionType::NONE, L"DecoratorCooldown (冷却)" },
		{ BTNodeType::DECORATOR_PERIODIC, ConditionType::NONE, ActionType::NONE, L"DecoratorPeriodic (周期)" },
		{ BTNodeType::SUCCEEDER,          ConditionType::NONE, ActionType::NONE, L"Succeeder (强制成功)" },
		{ BTNodeType::FAILER,             ConditionType::NONE, ActionType::NONE, L"Failer (强制失败)" },
		// 条件节点（17）
		{ BTNodeType::CONDITION, ConditionType::LOW_HP,            ActionType::NONE, L"ConditionLowHP (低血)" },
		{ BTNodeType::CONDITION, ConditionType::LOW_MP,            ActionType::NONE, L"ConditionLowMP (低蓝)" },
		{ BTNodeType::CONDITION, ConditionType::HAS_TARGET,        ActionType::NONE, L"ConditionHasTarget (有目标)" },
		{ BTNodeType::CONDITION, ConditionType::IN_SAFE_AREA,      ActionType::NONE, L"ConditionInSafeArea (安全区)" },
		{ BTNodeType::CONDITION, ConditionType::BAG_FULL,          ActionType::NONE, L"ConditionBagFull (背包满)" },
		{ BTNodeType::CONDITION, ConditionType::HAS_ITEM,          ActionType::NONE, L"ConditionHasItem (持有物品)" },
		{ BTNodeType::CONDITION, ConditionType::SKILL_READY,       ActionType::NONE, L"ConditionSkillReady (技能就绪)" },
		{ BTNodeType::CONDITION, ConditionType::IS_DEAD,           ActionType::NONE, L"ConditionIsDead (已死亡)" },
		{ BTNodeType::CONDITION, ConditionType::TARGET_DISTANCE,   ActionType::NONE, L"ConditionTargetDistance (目标距离)" },
		{ BTNodeType::CONDITION, ConditionType::TARGET_TYPE,       ActionType::NONE, L"ConditionTargetType (目标类型)" },
		{ BTNodeType::CONDITION, ConditionType::HAS_BUFF,          ActionType::NONE, L"ConditionHasBuff (有Buff)" },
		{ BTNodeType::CONDITION, ConditionType::HAS_NEARBY_PLAYER, ActionType::NONE, L"ConditionHasNearbyPlayer (附近玩家)" },
		{ BTNodeType::CONDITION, ConditionType::MONSTER_COUNT,     ActionType::NONE, L"ConditionMonsterCount (怪物数量)" },
		{ BTNodeType::CONDITION, ConditionType::HAS_POTION,        ActionType::NONE, L"ConditionHasPotion (有药水)" },
		{ BTNodeType::CONDITION, ConditionType::HAS_DROPPED_ITEM,  ActionType::NONE, L"ConditionHasDroppedItem (掉落物)" },
		{ BTNodeType::CONDITION, ConditionType::HP_RANGE,          ActionType::NONE, L"ConditionHPRange (血量范围)" },
		{ BTNodeType::CONDITION, ConditionType::TIME_OF_DAY,       ActionType::NONE, L"ConditionTimeOfDay (时间段)" },
		// 动作节点（23）
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::USE_POTION,         L"ActionUsePotion (喝药)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::USE_ITEM,           L"ActionUseItem (使用道具)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::CHANGE_ATTACK_MODE, L"ActionChangeAttackMode (切攻击模式)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::ATTACK,             L"ActionAttack (攻击)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::MOVE_TO_TARGET,     L"ActionMoveToTarget (移向目标)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::PATROL,             L"ActionPatrol (巡逻)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::PICKUP_ITEM,        L"ActionPickupItem (拾取)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::FLEE,               L"ActionFlee (逃跑)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::REST,               L"ActionRest (休息)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::REVIVE,             L"ActionRevive (复活)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::CHAT,               L"ActionChat (聊天)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::USE_SKILL,          L"ActionUseSkill (使用技能)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::SAY,                L"ActionSay (说话)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::RECALL,             L"ActionRecall (回城)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::DELAY,              L"ActionDelay (延迟)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::ATTACK_DIR,         L"ActionAttackDir (定向攻击)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::SPELL_CAST,         L"ActionSpellCast (精确施法)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::DROP_ITEM,          L"ActionDropItem (丢弃)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::EQUIP_ITEM,         L"ActionEquipItem (穿戴)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::SUMMON_PET,         L"ActionSummonPet (召唤宠物)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::FOLLOW,             L"ActionFollow (跟随)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::GROUP,              L"ActionGroup (组队)" },
		{ BTNodeType::ACTION, ConditionType::NONE, ActionType::MINE,               L"ActionMine (挖矿)" },
	};
	return kCatalog;
}

// 根据节点三元组在目录中查找索引（用于对话框预选）
inline int FindCatalogIndex(BTNodeType type, ConditionType ct, ActionType at)
{
	const auto& cat = GetNodeCatalog();
	for (size_t i = 0; i < cat.size(); i++)
	{
		if (cat[i].type == type && cat[i].ct == ct && cat[i].at == at)
			return (int)i;
	}
	return -1;
}
