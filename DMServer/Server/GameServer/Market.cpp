#include "StdAfx.h"
#include ".\market.h"
#include ".\submarket.h"
#include ".\humanplayer.h"

CMarket::CMarket(UINT id)
{
	m_nSubMarketCount = 0;
	this->m_Id = id;
}

CMarket::~CMarket(VOID)
{
}

CSubMarket* CMarket::AddSubMarket(UINT nId, const char* pszName)
{
	if (this->m_nSubMarketCount >= 32)
		return nullptr;
	std::array<char, 256> szFilename{};

	this->m_pSubMarketArray[this->m_nSubMarketCount] = std::make_unique<CSubMarket>(this->m_Id, nId, pszName);

	snprintf(szFilename.data(), szFilename.size(), ".\\Data\\Market\\%02u%02u.txt", this->m_Id, nId);
	if (!this->m_pSubMarketArray[this->m_nSubMarketCount]->LoadSubMarket(szFilename.data()))
	{
		this->m_pSubMarketArray[this->m_nSubMarketCount].reset();
		return nullptr;
	}
	return this->m_pSubMarketArray[this->m_nSubMarketCount++].get();
}

CSubMarket* CMarket::GetSubMarket(UINT nId)
{
	for (UINT n = 0; n < this->m_nSubMarketCount; n++)
	{
		if (this->m_pSubMarketArray[n] && this->m_pSubMarketArray[n]->GetId() == nId)
			return this->m_pSubMarketArray[n].get();
	}
	return nullptr;
}

VOID CMarket::SendSubMarket(CHumanPlayer* pPlayer)
{
	xPacketPool::ScopedPacket packet(65536);
	snprintf((char*)packet->getbuf(), 65536, "%02u&", m_Id);
	packet->addsize((int)strlen(packet->getbuf()));
	for (UINT n = 0; n < this->m_nSubMarketCount; n++)
	{
		if (this->m_pSubMarketArray[n])
		{
			snprintf((char*)packet->getfreebuf(), 65536 - packet->getsize(), "%02u|%s&", this->m_pSubMarketArray[n]->GetId(), this->m_pSubMarketArray[n]->GetName());
			packet->addsize((int)strlen(packet->getfreebuf()));
		}
	}
	pPlayer->SendMsg(pPlayer->GetId(), 0x1000, 2, 0, 0, (LPVOID)packet->getbuf(), packet->getsize());
	if (this->m_nSubMarketCount > 0 && m_pSubMarketArray[0])
		m_pSubMarketArray[0]->QueryItems(pPlayer, GetId());
}