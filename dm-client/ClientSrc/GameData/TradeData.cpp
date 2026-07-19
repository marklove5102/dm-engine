#include "tradedata.h"
#include "GameData.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

CTradeData g_TradeData;

CTradeData::CTradeData(void)
{
	Clear();
}

CTradeData::~CTradeData(void)
{
}
void CTradeData::Clear()
{
	for(int i = 0; i < MAX_TRADE_OBJECTS; i ++)
	{
        m_AnotherGoods[i].SetID(0);
		m_SelfGoods[i].SetID(0);
	}
	m_dwSelfYuanBao = 0;
	m_dwAnotherYuanBao = 0;
	m_dwSelfGolds = 0;
	m_dwAnotherGolds = 0;
	m_tempGood.clear();
	m_bIsTrade = false;
}

void CTradeData::BackToPackage()
{
	// 交易取消, 设置自己包裹中的金币
	SELF.SetGold(SELF.GetGold() 
		+ g_TradeData.GetSelfGolds());

	// 交易取消, 设置自己包裹中的元宝
	SELF.SetYuanBao(SELF.GetYuanBao() 
		+ g_TradeData.GetSelfYuanBao());

	m_dwSelfGolds = 0;
	m_dwAnotherGolds = 0;

	m_dwSelfYuanBao = 0;
	m_dwAnotherYuanBao = 0;

	for(int i = 0; i < MAX_TRADE_OBJECTS; i ++)
	{
		SELF.PackageGood().BackToArray(m_SelfGoods[i]);
		m_SelfGoods[i].SetID(0);
		m_AnotherGoods[i].SetID(0);
	}
}
	