#include "stdafx.h"
#include "FengHaoGrowManager.h"
#include "humanplayer.h"
#include "itemmanager.h"
#include "scriptshell.h"
#include "scriptview.h"
#include "logicmap.h"
#include "timesystem.h"
#include "GameWorld.h"

VOID CHumanPlayer::SendUpdateItem(ITEM& item)
{
	ItemToClient(item);
	ITEMCLIENT clientItem;
	memcpy(&clientItem, &item, sizeof(ITEMCLIENT));
	SendMsg(GetId(), 0xcb, 0, 0, 1, &clientItem, sizeof(ITEMCLIENT));
}

VOID CHumanPlayer::SendWeaponBroken()
{
	SaySystem("您的武器已破碎!");
	SendAroundMsg(GetId(), 0x44e, 0, 0, 0);
	SendMsg(GetId(), 0x44e, 0, 0, 0);
}

VOID CHumanPlayer::SendAddCommunity(WORD wType, const char* pszName)
{
	SendMsg(0, 0x1c2, wType, 0, 0, (LPVOID)pszName);
}

VOID CHumanPlayer::SendDeleteCommunity(WORD wType, const char* pszName)
{
	SendMsg(0, 0x1c3, wType, 0, 0, (LPVOID)pszName);
}

VOID CHumanPlayer::SendSetPetBag(UINT nSize)
{
	SendMsg(GetId(), 0x9601, 0, 0, nSize);
}

VOID CHumanPlayer::SendPetBag(ITEMCLIENT* pItems, UINT nCount)
{
	if (nCount > 0 && pItems != nullptr)
		SendMsg(GetId(), 0x9602, 0, 0, nCount, (LPVOID)pItems, sizeof(ITEMCLIENT) * nCount);
	else
	{
		nCount = 0;
		SendMsg(GetId(), 0x9602, 0, 0, nCount);
	}
}

VOID CHumanPlayer::SendWeightChanged()
{
	SendMsg(GetPropValue(PI_CURBAGWEIGHT), SM_WEIGHTCHANGED, GetPropValue(PI_CURBODYWEIGHT), GetPropValue(PI_CURHANDWEIGHT), 0, 0, 0);
}

VOID CHumanPlayer::SendGoldChanged(DWORD dwChanged)
{
	SendMsg(m_Humandesc.dbinfo.dwGold, SM_SETGOLD, 0, 0, 0, 0, 0);
}

VOID CHumanPlayer::SendEatOk()
{
	SendMsg(0, SM_EAT_OK, 0, 0, 0, 0, 0);
	SendWeightChanged();
}

VOID CHumanPlayer::SendEatFail()
{
	SendMsg(0, SM_EAT_FAIL, 0, 0, 0, 0, 0);
}

VOID CHumanPlayer::SendScrollText(const char* pszText)
{
	SendMsg(0, SM_SCROLLTEXT, 0, 0, 0, (LPVOID)pszText);
}

VOID CHumanPlayer::SendClientKeyConfig()
{
	xPacketPool::ScopedPacket packet;
	int nValue = 0x64;
	packet->push(&nValue, 4);
	ClientKeyState* clientKeyConfig = CGameWorld::GetInstance()->GetClientKeyConfig();
	packet->push(clientKeyConfig, sizeof(ClientKeyState) * 100);
	SendMsg(GetId(), 0x97a, 0, 0, 0, (LPVOID)packet->getbuf(), packet->getsize());
}

VOID CHumanPlayer::SendClientPluginInfo()
{
	xPacketPool::ScopedPacket packet;
	//位运算，开启客户端插件功能：
	//开特权大包裹
	//内挂持续使用
	//挂机绑金上限
	//时长充值按钮
	//玄武炉无限制
	//开启物品来源
	//开启无限刀
	//整理包裹触发
	int nValue = 255;
	packet->push(&nValue, 4);
	//插入包裹名称
	const char* sBagName = "VIP包裹";
	packet->push((LPVOID)sBagName, 15);
	packet->push(1);
	//插入充值网址
	const char* sPayWeb = "https://www.jiangjiali.com";
	packet->push((LPVOID)sPayWeb, 254);
	packet->push(1);
	SendMsg(GetId(), 0xa02, 0, 10086, 0, (LPVOID)packet->getbuf(), packet->getsize());
}

VOID CHumanPlayer::Sendfirstdlg(const char* pszString)
{
	xPacketPool::ScopedPacket packet;
	//packet->push((LPVOID)&m_Humandesc.dbinfo.nGameTime, 4);
	//packet->push((LPVOID)&m_Humandesc.dbinfo.wLevel, 2);
	packet->push(pszString);
	SendMsg(GetId(), SM_FIRSTDIALOG, 257, 1101, 1, (LPVOID)packet->getbuf(), packet->getsize());
	//SendMsg(m_Humandesc.dbinfo.dwYuanbao, 0xe679, 0, 0, 0); // 发送元宝数量
}

VOID CHumanPlayer::SendOpenGameTimeInfo()
{
	xPacketPool::ScopedPacket packet;
	const char* s1C = "GameTimeMgr";
	packet->push(s1C);
	packet->push(1);
	int nValue = 0x01;
	packet->push((LPVOID)&nValue, 4);
	packet->push(1);
	nValue = 0x02;
	packet->push((LPVOID)&nValue, 4);
	packet->push((LPVOID)&m_Humandesc.dbinfo.nGameTime, 4);
	packet->push(16);
	nValue = 0x02;
	packet->push((LPVOID)&nValue, 4);
	packet->push((LPVOID)&m_Humandesc.dbinfo.nGameTime, 4);
	packet->push(12);
	SendMsg(GetId(), 0xa02, 0, 0, 0, (LPVOID)packet->getbuf(), packet->getsize());
}

VOID CHumanPlayer::SendClientfunction()
{
	xPacketPool::ScopedPacket packet;
	DWORD nValue = 0x00;
	packet->push((LPVOID)&nValue, 4);
	nValue = 0x00;
	packet->push((LPVOID)&nValue, 4);
	WORD LoParam1{}, HiParam1{};
	LoParam1 += 1 << 6; // 新邮件
	//LoParam1 += 1 << 7; // 开启2.4内容, 包括友好度、结婚、结义
	LoParam1 += 1 << 10; // 2019版活跃度
	LoParam1 += 1 << 13; // 资源服务器重新连接
	BYTE LoParam2{}, HiParam2{};
	LoParam2 += 1 << 0; // 豹子摆摊功能
	LoParam2 += 1 << 2; // 新彩虹精灵
	LoParam2 += 1 << 4; // 快捷消费使用功能
	HiParam2 += 1 << 1; // 主线任务删除按钮
	HiParam2 += 1 << 2; // 开启IGW
	HiParam2 += 1 << 7; // 安全区穿人
	BYTE LoParam3{}, HiParam3{};
	LoParam3 += 1 << 1; // 元宝拍卖行和现金拍卖行是否合并
	LoParam3 += 1 << 3; // 打开宝石系统
	LoParam3 += 1 << 5; // 关闭彩虹网弹框
	HiParam3 += 1 << 6; // 开启自定义快捷键功能
	SendMsg(static_cast<DWORD>(MAKELONG(LoParam1, HiParam1)), 0x330, MAKEWORD(LoParam2, HiParam2), MAKEWORD(LoParam3, HiParam3),
		0, (LPVOID)packet->getbuf(), packet->getsize());
}

VOID CHumanPlayer::SendZhenBao(DWORD dwZhenBaoExp, DWORD dwZhenBaoExpMax, DWORD dwZhenBaoStar)
{
	xPacketPool::ScopedPacket packet;
	packet->push("ZhenBaoExp");
	packet->push(1);
	packet->push((LPVOID)&dwZhenBaoExp, 8);
	SendMsg(GetId(), 0x9af, 0, 0, 0, (LPVOID)packet->getbuf(), packet->getsize());

	if (dwZhenBaoExpMax != -1 && dwZhenBaoExpMax != m_dwZhenBaoExpMax)
	{
		m_dwZhenBaoExpMax = dwZhenBaoExpMax;
		packet->clear();
		packet->push("ZhenBaoExpMax");
		packet->push(1);
		packet->push((LPVOID)&dwZhenBaoExpMax, 8);
		SendMsg(GetId(), 0x9af, 0, 0, 0, (LPVOID)packet->getbuf(), packet->getsize());
	}
	if (dwZhenBaoStar != -1 && dwZhenBaoStar != m_dwZhenBaoStar)
	{
		m_dwZhenBaoStar = dwZhenBaoStar;
		packet->clear();
		packet->push("ZhenBaoStar");
		packet->push(1);
		packet->push((LPVOID)&dwZhenBaoStar, 8);
		SendMsg(GetId(), 0x9af, 0, 0, 0, (LPVOID)packet->getbuf(), packet->getsize());
	}
}

VOID CHumanPlayer::SendJingLiZhi(DWORD wStamina)
{
	m_wStamina = static_cast<WORD>(wStamina);
	xPacketPool::ScopedPacket packet;
	packet->push("jinglizhi");
	packet->push(1);
	packet->push((LPVOID)&wStamina, 8);
	SendMsg(GetId(), 0x9af, 0, 0, 0, (LPVOID)packet->getbuf(), packet->getsize());
}

VOID CHumanPlayer::SendDuraChanged(int pos, WORD wCurDura, WORD wMaxDura)
{
	SendMsg(wCurDura, 0x282, pos, wMaxDura, 0);
}

VOID CHumanPlayer::SendGroupMode()
{
	WORD wFlag = 0;
	if (m_Humandesc.dbinfo.dwFlag[0] & 0x80000000)
		wFlag = 1;
	SendMsg(0, SM_GROUPMODE, wFlag, 0, 0);
}

VOID CHumanPlayer::SendTitleChanged()
{
	if (m_szCurrentTitle[0] != 0)
	{
		WORD wFlag = 8 | ((m_iCurrentTitleIndex + 1) << 8);
		SendAroundMsg(GetId(), 0x532c, wFlag, 0, 0, (LPVOID)m_szCurrentTitle.data());
		SendMsg(GetId(), 0x532c, wFlag, 0, 0, (LPVOID)m_szCurrentTitle.data());
	}
}

VOID CHumanPlayer::SendMagicList()
{
	int count = 0;
	MAGIC g_tmpMagicBuffer[27];//最多技能的是法师, 27个.
	for (auto& up : m_vMagic)
	{
		if (CMagicManager::GetInstance()->CreateMagic((UINT)up->magic.wId, g_tmpMagicBuffer[count]))
		{
			g_tmpMagicBuffer[count].cKey = static_cast<char>(up->magic.btKey);
			g_tmpMagicBuffer[count].btLevel = up->magic.btLevel;
			g_tmpMagicBuffer[count].iCurExp = static_cast<WORD>(up->magic.dwCurTrain);
			count++;
		}
	}
	SendMsg(0, 0xd3, 0, 0, 0, (LPVOID)g_tmpMagicBuffer, static_cast<WORD>(sizeof(MAGIC) * count));
}

VOID CHumanPlayer::SendTakeBagItem(ITEM* pItem)
{
	SendMsg(GetId(), 0xca, 0, 0, 1, (LPVOID)pItem, sizeof(ITEMCLIENT));
}

VOID CHumanPlayer::SendMoneyChanged(money_type type)
{
	WORD wMsg = (type == MT_GOLD ? SM_GOLDCHANGED : SM_SETSUPERGOLD);
	SendMsg(GetMoney(type), wMsg, 0, 0, 0);
}

VOID CHumanPlayer::SendMagicExpChg(USERMAGIC* pMagic)
{
	SendMsg(pMagic->magic.wId, SM_SPELLEXPCHG, pMagic->magic.btLevel, pMagic->magic.dwCurTrain & 0xffff,
		(pMagic->magic.dwCurTrain & 0xffff0000) >> 16, nullptr);
}

VOID CHumanPlayer::SendCloseScriptPage(UINT nId)
{
	SendMsg(nId, 0x284, 0, 0, 0, nullptr);
}

VOID CHumanPlayer::SendPage(CScriptShell* pShell, CScriptView* pView)
{
	UINT id = pShell == nullptr ? 0xffffffff : pShell->GetTitleId();
	SendMsg(id, 0x283, static_cast<WORD>(pView->GetParam()), 0, 1, (LPVOID)pView->getPacket().getbuf(), pView->getPacket().getsize());
}

VOID CHumanPlayer::SendClosePage(CScriptShell* pShell)
{
	SendCloseScriptPage(pShell == nullptr ? 0xffffffff : pShell->GetTitleId());
}

VOID CHumanPlayer::PostMsg(const char* pszMsg, int length)
{
	if (m_pClientObj == nullptr || pszMsg == nullptr) return;
	if (length == 0) length = (int)strlen(pszMsg);
	m_pClientObj->PostMsg(pszMsg, length);
}

VOID CHumanPlayer::SendTimeWeatherChanged()
{
	if (m_pMap == nullptr) return;
	WORD wTime = 0xffff;
	if (!m_pMap->IsFlagSeted(MF_DAY) && !m_pMap->IsFlagSeted(MF_NIGHT))
		wTime = CTimeSystem::GetInstance()->GetCurrentlyTime();
	WORD wWeather = m_pMap->GetWeather().wWeatherIndex;
	WORD wFlag = m_pMap->GetWeather().wFlag;
	DWORD dwWeatherColor = m_pMap->GetWeather().dwWeatherColor;
	if (!m_pMap->IsFlagSeted(MF_WEATHER))
	{
		wWeather = 0;
		dwWeatherColor = 0xffffffff;
	}
	SendMsg(m_pMap->GetWeather().dwBGColor, SM_SETGAMEDATETIME, wTime, wWeather, wFlag, &dwWeatherColor, sizeof(DWORD));
}

static thread_local std::array<DBITEM, 100> s_dbPacketDst{};
static thread_local std::array<ITEM, 100> s_dbPacketItems{};
static thread_local std::array<BAGITEMPOS, 100> s_dbPacketPos{};
VOID CHumanPlayer::GetDBInfoPacket(xPacket& packet)
{
	CHARDBINFO info;
	int length = 0;
	packet.clear();
	// 角色属性数据
	if (GetDBInfo(info))
	{
		if (!IsDeath())
		{
			if (info.hp == 0)
				info.hp = info.maxhp;
		}
		else
			info.hp = 0;
		length = EncodeMsg((char*)packet.getfreebuf(), 0, DM_PUTCHARDBINFO, 0, 0, 0, &info, sizeof(info));
		packet.addsize(length);
	}
	// 社交数据
	if (IsSystemFlagSeted(SF_COMMUNITYLOADED))
	{
		char szCommunityText[4096];
		length = GetCommunityInfo(szCommunityText, 4096);
		length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATECOMMUNITY, 0, 0, 0, (LPVOID)szCommunityText);
		packet.addsize(length);
	}
	// 已学习技能数据
	if (this->m_fMagicLoaded)
	{
		std::array<MAGICDB, 255> array{};
		int	count = 0;
		for (auto& up : m_vMagic)
		{
			if (count < 255)
				array[count++] = up->magic;
		}
		if (count > 0)
		{
			length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATEMAGIC, 0, 0, count, (LPVOID)array.data(), sizeof(MAGICDB) * count);
			packet.addsize(length);
		}
	}
	// 任务信息
	length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATETASKINFO, 0, 0, 0, (LPVOID)&m_TaskInfo, sizeof(m_TaskInfo));
	packet.addsize(length);
	// 时长封号信息
	length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATEFENGHAO, 0, 0, 0, (LPVOID)&m_FenghaoInfo, sizeof(m_FenghaoInfo));
	packet.addsize(length);
	// 背包数据
	int count = 0;
	count = m_ItemBox.GetItems(s_dbPacketItems.data(), 100);
	int updatecount = 0;
	int uposcount = 0;
	for (int i = 0; i < count; i++)
	{
		if (s_dbPacketItems[i].dwMakeIndex & 0x80000000)
		{
			if (CItemManager::GetInstance()->ItemLimited(s_dbPacketItems[i], IL_NOUPDATETODB))continue;
			s_dbPacketDst[updatecount].item = s_dbPacketItems[i];
			s_dbPacketDst[updatecount].btFlag = 0;
			s_dbPacketDst[updatecount].pos = static_cast<WORD>(s_dbPacketItems[i].dwParam[2]);
			updatecount++;
		}
		else
		{
			BYTE btFlag = static_cast<BYTE>(s_dbPacketItems[i].baseitem.wFeature & 0x00f0);
			if (btFlag != 0)
			{
				s_dbPacketDst[updatecount].item = s_dbPacketItems[i];
				s_dbPacketDst[updatecount].btFlag = btFlag;
				s_dbPacketDst[updatecount].pos = static_cast<WORD>(s_dbPacketItems[i].dwParam[2]);
				updatecount++;
			}
			else
			{
				s_dbPacketPos[uposcount].ItemId = s_dbPacketItems[i].dwMakeIndex;
				s_dbPacketPos[uposcount].wPos = static_cast<WORD>(s_dbPacketItems[i].dwParam[2]);
				uposcount++;
			}
		}
	}
	if (updatecount > 0)
	{
		length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATEITEMS, updatecount, IDF_BAG, 0, (LPVOID)s_dbPacketDst.data(), sizeof(DBITEM) * updatecount);
		packet.addsize(length);
	}
	// 背包的其他物品的位置要更新
	if (uposcount > 0)
	{
		length = EncodeMsg((char*)packet.getfreebuf(), 0, DM_UPDATEITEMPOSEX, uposcount, IDF_BAG, 0, (LPVOID)s_dbPacketPos.data(), sizeof(BAGITEMPOS) * uposcount);
		packet.addsize(length);
	}
	// 仓库物品数据
	count = m_ItemBank.GetItems(s_dbPacketItems.data(), 100);
	updatecount = 0;
	for (int i = 0; i < count; i++)
	{
		if (s_dbPacketItems[i].dwMakeIndex & 0x80000000)
		{
			if (CItemManager::GetInstance()->ItemLimited(s_dbPacketItems[i], IL_NOUPDATETODB))continue;
			s_dbPacketDst[updatecount].item = s_dbPacketItems[i];
			s_dbPacketDst[updatecount].btFlag = 0;
			s_dbPacketDst[updatecount].pos = i;
			updatecount++;
		}
		else
		{
			BYTE btFlag = (s_dbPacketItems[i].baseitem.wFeature & 0x00f0);
			if (btFlag != 0)
			{
				s_dbPacketDst[updatecount].item = s_dbPacketItems[i];
				s_dbPacketDst[updatecount].btFlag = btFlag;
				s_dbPacketDst[updatecount].pos = i;
				updatecount++;
			}
		}
	}
	if (updatecount > 0)
	{
		length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATEITEMS, updatecount, IDF_BANK, 0, (LPVOID)s_dbPacketDst.data(), sizeof(DBITEM) * updatecount);
		packet.addsize(length);
	}
	// 宠物背包数据
	count = m_ItemPetBag.GetItems(s_dbPacketItems.data(), 100);
	updatecount = 0;
	for (int i = 0; i < count; i++)
	{
		if (s_dbPacketItems[i].dwMakeIndex & 0x80000000)
		{
			if (CItemManager::GetInstance()->ItemLimited(s_dbPacketItems[i], IL_NOUPDATETODB))continue;
			s_dbPacketDst[updatecount].item = s_dbPacketItems[i];
			s_dbPacketDst[updatecount].btFlag = 0;
			s_dbPacketDst[updatecount].pos = i;
			updatecount++;
		}
		else
		{
			BYTE btFlag = (s_dbPacketItems[i].baseitem.wFeature & 0x00f0);
			if (btFlag != 0)
			{
				s_dbPacketDst[updatecount].item = s_dbPacketItems[i];
				s_dbPacketDst[updatecount].btFlag = btFlag;
				s_dbPacketDst[updatecount].pos = i;
				updatecount++;
			}
		}
	}
	if (updatecount > 0)
	{
		length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATEITEMS, updatecount, IDF_PETBANK, 0, (LPVOID)s_dbPacketDst.data(), sizeof(DBITEM) * updatecount);
		packet.addsize(length);
	}
	// 装备数据
	ITEM* pEquipment = nullptr;
	updatecount = 0;
	for (int i = 0; i < _U_MAX; i++)
	{
		pEquipment = m_Equipments.GetEquipment(i);
		if (pEquipment)
		{
			if (pEquipment->dwMakeIndex & 0x80000000)
			{
				if (CItemManager::GetInstance()->ItemLimited(*pEquipment, IL_NOUPDATETODB))continue;
				s_dbPacketDst[updatecount].item = *pEquipment;
				s_dbPacketDst[updatecount].btFlag = 0;
				s_dbPacketDst[updatecount].pos = i;
				updatecount++;
			}
			else
			{
				BYTE btFlag = (pEquipment->baseitem.wFeature & 0x00f0);
				if (btFlag != 0)
				{
					s_dbPacketDst[updatecount].item = *pEquipment;
					s_dbPacketDst[updatecount].btFlag = btFlag;
					s_dbPacketDst[updatecount].pos = i;
					updatecount++;
				}
			}
		}
	}
	if (updatecount > 0)
	{
		length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATEITEMS, updatecount, IDF_EQUIPMENT, 0, (LPVOID)s_dbPacketDst.data(), sizeof(DBITEM) * updatecount);
		packet.addsize(length);
	}
	// 存储升级临时武器的物品
	if ((this->m_UpgradeItem.dwMakeIndex & 0x80000000) && !CItemManager::GetInstance()->ItemLimited(this->m_UpgradeItem, IL_NOUPDATETODB))
	{
		updatecount = 0;
		s_dbPacketDst[updatecount].item = m_UpgradeItem;
		s_dbPacketDst[updatecount].btFlag = 0;
		s_dbPacketDst[updatecount].pos = 0;
		updatecount++;
		length = EncodeMsg((char*)packet.getfreebuf(), info.dwDBId, DM_UPDATEITEMS, updatecount, IDF_UPGRADE, 0, (LPVOID)s_dbPacketDst.data(), sizeof(DBITEM) * updatecount);
		packet.addsize(length);
	}
}