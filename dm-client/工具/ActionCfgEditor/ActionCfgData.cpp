#include "ActionCfgData.h"
#include <fstream>
#include <stdexcept>
#include <cstring>

void ActionCfgData::Load(const std::string& path)
{
	std::ifstream ifs(path, std::ios::binary);
	if (!ifs)
		throw std::runtime_error("无法打开文件: " + path);

	DrawOrders.clear();
	Monsters.clear();
	Wings.clear();
	WingEffects.clear();

	// ===== 区段1: 绘制顺序表 =====
	ifs.read(reinterpret_cast<char*>(m_header1.data()), 32);
	ifs.read(reinterpret_cast<char*>(&m_ver1), 4);
	int32_t count = 0;
	ifs.read(reinterpret_cast<char*>(&count), 4);
	ifs.read(reinterpret_cast<char*>(&m_tmp1_1), 4);
	ifs.read(reinterpret_cast<char*>(&m_tmp1_2), 4);

	for (int32_t ii = 0; ii < count; ++ii)
	{
		SDrawOrderEntry entry;
		ifs.read(reinterpret_cast<char*>(&entry.Action), 4);
		ifs.read(reinterpret_cast<char*>(&entry.Frame), 4);

		if (entry.Frame > 0)
		{
			int32_t total = 8 * entry.Frame;
			entry.OrderData.resize(total * DP_DrawPartMaxNum);
			for (int32_t k = 0; k < total; ++k)
			{
				ifs.read(reinterpret_cast<char*>(&entry.OrderData[k * DP_DrawPartMaxNum]), DP_DrawPartMaxNum);
			}
		}
		DrawOrders.push_back(std::move(entry));
	}

	// ===== 区段2: 怪物动作表 =====
	ifs.read(reinterpret_cast<char*>(m_header2.data()), 32);
	ifs.read(reinterpret_cast<char*>(&m_ver2), 4);
	count = 0;
	ifs.read(reinterpret_cast<char*>(&count), 4);
	ifs.read(reinterpret_cast<char*>(&m_tmp2_1), 4);
	ifs.read(reinterpret_cast<char*>(&m_tmp2_2), 4);

	for (int32_t i = 0; i < count; ++i)
	{
		int32_t iActionNum = 0;
		ifs.read(reinterpret_cast<char*>(&iActionNum), 4);
		MonsterEntry monster;
		for (int32_t j = 0; j < iActionNum; ++j)
		{
			MonsterActionEntry act;
			ifs.read(reinterpret_cast<char*>(&act.Action), 4);
			ifs.read(reinterpret_cast<char*>(&act.StartIndex), 4);
			ifs.read(reinterpret_cast<char*>(&act.Mmx), 4);
			ifs.read(reinterpret_cast<char*>(&act.Realx), 4);
			ifs.read(reinterpret_cast<char*>(&act.Speedx), 4);
			monster.Actions.push_back(act);
		}
		Monsters.push_back(std::move(monster));
	}

	// ===== 区段3: 翅膀 & 翅膀特效 =====
	ifs.read(reinterpret_cast<char*>(m_header3.data()), 32);
	ifs.read(reinterpret_cast<char*>(&m_ver3), 4);
	int32_t wingcount = 0, wingeffcount = 0;
	ifs.read(reinterpret_cast<char*>(&wingcount), 4);
	ifs.read(reinterpret_cast<char*>(&wingeffcount), 4);
	ifs.read(reinterpret_cast<char*>(&m_tmp3), 4);

	auto readWingEntries = [&ifs](int32_t entryCount, std::vector<WingEntry>& dest)
	{
		for (int32_t ii = 0; ii < entryCount; ++ii)
		{
			int32_t iActionNum = 0;
			ifs.read(reinterpret_cast<char*>(&iActionNum), 4);
			WingEntry wing;
			for (int32_t i = 0; i < iActionNum; ++i)
			{
				WingActionEntry act;
				ifs.read(reinterpret_cast<char*>(&act.Action), 4);
				ifs.read(reinterpret_cast<char*>(&act.Package), 4);
				ifs.read(reinterpret_cast<char*>(&act.StartIndex), 4);
				ifs.read(reinterpret_cast<char*>(&act.FrameLeft), 4);
				ifs.read(reinterpret_cast<char*>(&act.FrameRight), 4);
				ifs.read(reinterpret_cast<char*>(&act.FrameShadow), 4);
				wing.Actions.push_back(act);
			}
			dest.push_back(std::move(wing));
		}
	};

	readWingEntries(wingcount, Wings);
	readWingEntries(wingeffcount, WingEffects);

	if (!ifs)
		throw std::runtime_error("文件读取失败，数据可能不完整: " + path);
}

void ActionCfgData::Save(const std::string& path) const
{
	std::ofstream ofs(path, std::ios::binary);
	if (!ofs)
		throw std::runtime_error("无法写入文件: " + path);

	// ===== 区段1: 绘制顺序表 =====
	ofs.write(reinterpret_cast<const char*>(m_header1.data()), 32);
	ofs.write(reinterpret_cast<const char*>(&m_ver1), 4);
	int32_t count = static_cast<int32_t>(DrawOrders.size());
	ofs.write(reinterpret_cast<const char*>(&count), 4);
	ofs.write(reinterpret_cast<const char*>(&m_tmp1_1), 4);
	ofs.write(reinterpret_cast<const char*>(&m_tmp1_2), 4);

	for (const auto& entry : DrawOrders)
	{
		ofs.write(reinterpret_cast<const char*>(&entry.Action), 4);
		ofs.write(reinterpret_cast<const char*>(&entry.Frame), 4);
		if (entry.Frame > 0 && !entry.OrderData.empty())
		{
			int32_t total = 8 * entry.Frame;
			for (int32_t k = 0; k < total; ++k)
			{
				ofs.write(reinterpret_cast<const char*>(&entry.OrderData[k * DP_DrawPartMaxNum]), DP_DrawPartMaxNum);
			}
		}
	}

	// ===== 区段2: 怪物动作表 =====
	ofs.write(reinterpret_cast<const char*>(m_header2.data()), 32);
	ofs.write(reinterpret_cast<const char*>(&m_ver2), 4);
	count = static_cast<int32_t>(Monsters.size());
	ofs.write(reinterpret_cast<const char*>(&count), 4);
	ofs.write(reinterpret_cast<const char*>(&m_tmp2_1), 4);
	ofs.write(reinterpret_cast<const char*>(&m_tmp2_2), 4);

	for (const auto& monster : Monsters)
	{
		int32_t iActionNum = static_cast<int32_t>(monster.Actions.size());
		ofs.write(reinterpret_cast<const char*>(&iActionNum), 4);
		for (const auto& act : monster.Actions)
		{
			ofs.write(reinterpret_cast<const char*>(&act.Action), 4);
			ofs.write(reinterpret_cast<const char*>(&act.StartIndex), 4);
			ofs.write(reinterpret_cast<const char*>(&act.Mmx), 4);
			ofs.write(reinterpret_cast<const char*>(&act.Realx), 4);
			ofs.write(reinterpret_cast<const char*>(&act.Speedx), 4);
		}
	}

	// ===== 区段3: 翅膀 & 翅膀特效 =====
	ofs.write(reinterpret_cast<const char*>(m_header3.data()), 32);
	ofs.write(reinterpret_cast<const char*>(&m_ver3), 4);
	int32_t wingcount = static_cast<int32_t>(Wings.size());
	int32_t wingeffcount = static_cast<int32_t>(WingEffects.size());
	ofs.write(reinterpret_cast<const char*>(&wingcount), 4);
	ofs.write(reinterpret_cast<const char*>(&wingeffcount), 4);
	ofs.write(reinterpret_cast<const char*>(&m_tmp3), 4);

	auto writeWingEntries = [&ofs](const std::vector<WingEntry>& entries)
	{
		for (const auto& wing : entries)
		{
			int32_t iActionNum = static_cast<int32_t>(wing.Actions.size());
			ofs.write(reinterpret_cast<const char*>(&iActionNum), 4);
			for (const auto& act : wing.Actions)
			{
				ofs.write(reinterpret_cast<const char*>(&act.Action), 4);
				ofs.write(reinterpret_cast<const char*>(&act.Package), 4);
				ofs.write(reinterpret_cast<const char*>(&act.StartIndex), 4);
				ofs.write(reinterpret_cast<const char*>(&act.FrameLeft), 4);
				ofs.write(reinterpret_cast<const char*>(&act.FrameRight), 4);
				ofs.write(reinterpret_cast<const char*>(&act.FrameShadow), 4);
			}
		}
	};

	writeWingEntries(Wings);
	writeWingEntries(WingEffects);
}
