#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <array>

/// 绘制部件最大数量(对应传奇世界客户端DP_DrawPartMaxNum)
constexpr int DP_DrawPartMaxNum = 50;

/// 绘制顺序条目——定义某个动作某个帧数下、8方向×帧的部件绘制顺序
struct SDrawOrderEntry
{
	int32_t Action = 0;                      // 动作编号
	int32_t Frame = 0;                       // 每方向的帧数
	std::vector<uint8_t> OrderData;          // 扁平存储: (方向*Frame+帧)*50 + 部件索引

	/// 获取(d方向, f帧, p部件)处的绘制层索引
	uint8_t Get(int d, int f, int p) const noexcept
	{
		return OrderData[(d * Frame + f) * DP_DrawPartMaxNum + p];
	}

	/// 设置(d方向, f帧, p部件)处的绘制层索引
	void Set(int d, int f, int p, uint8_t val) noexcept
	{
		OrderData[(d * Frame + f) * DP_DrawPartMaxNum + p] = val;
	}

	/// 确保OrderData内存大小匹配当前Frame
	void ResizeData()
	{
		if (Frame > 0)
			OrderData.resize(8 * Frame * DP_DrawPartMaxNum, 0);
		else
			OrderData.clear();
	}
};

/// 怪物动作条目
struct MonsterActionEntry
{
	int32_t Action = 0;      // 动作编号
	int32_t StartIndex = 0;  // 起始图片索引
	int32_t Mmx = 0;         // 最大帧数(FrameMax)
	int32_t Realx = 0;       // 实际帧数(FrameReal)
	int32_t Speedx = 0;      // 播放速度
};

/// 怪物条目——含一组动作配置
struct MonsterEntry
{
	std::vector<MonsterActionEntry> Actions;
};

/// 翅膀/翅膀特效动作条目
struct WingActionEntry
{
	int32_t Action = 0;       // 动作编号
	int32_t Package = 0;      // 资源包编号
	int32_t StartIndex = 0;   // 起始图片索引
	int32_t FrameLeft = 0;    // 左翅帧数
	int32_t FrameRight = 0;   // 右翅帧数
	int32_t FrameShadow = 0;  // 影子帧数
};

/// 翅膀/翅膀特效条目——含一组动作配置
struct WingEntry
{
	std::vector<WingActionEntry> Actions;
};

/// ActionCfg.dat 完整数据容器
/// 文件格式分为3个区段:
///   1. 绘制顺序表(DrawOrder)
///   2. 怪物动作表(Monsters)
///   3. 翅膀&翅膀特效表(Wings + WingEffects)
class ActionCfgData
{
public:
	std::vector<SDrawOrderEntry> DrawOrders;
	std::vector<MonsterEntry> Monsters;
	std::vector<WingEntry> Wings;
	std::vector<WingEntry> WingEffects;

	/// 从文件加载
	void Load(const std::string& path);

	/// 保存到文件
	void Save(const std::string& path) const;

private:
	// 区段1: DrawOrder
	std::array<uint8_t, 32> m_header1{};
	uint32_t m_ver1 = 0;
	int32_t m_tmp1_1 = 0, m_tmp1_2 = 0;

	// 区段2: Monsters
	std::array<uint8_t, 32> m_header2{};
	uint32_t m_ver2 = 0;
	int32_t m_tmp2_1 = 0, m_tmp2_2 = 0;

	// 区段3: Wings + WingEffects
	std::array<uint8_t, 32> m_header3{};
	uint32_t m_ver3 = 0;
	int32_t m_tmp3 = 0;
};
