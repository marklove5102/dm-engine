#include "StdAfx.h"
#include "humanplayer.h"
#include "ExchangeObjectMgr.h"
#include "ExchangeObj.h"
#include "itemmanager.h"
#include "logicmap.h"
#include "gsclientobj.h"

BOOL CHumanPlayer::Trade(CHumanPlayer* pPlayer)
{
	BOOL IsFasst = FALSE;
	if (pPlayer == nullptr)
	{
		if (m_pMap == nullptr) return FALSE;
		int x = getX();
		int y = getY();
		GETNEXTPOS(x, y, GetDirection());
		pPlayer = (CHumanPlayer*)m_pMap->FindObject(x, y, OBJ_PLAYER);
		if (pPlayer == nullptr)
		{
			SaySystem("对面没有人, 无法交易!");
			return FALSE;
		}
		int tx = pPlayer->getX();
		int ty = pPlayer->getY();
		GETNEXTPOS(tx, ty, pPlayer->GetDirection());
		if (tx != getX() || ty != getY())
		{
			SaySystem("对方没有面对着你, 无法交易!");
			return FALSE;
		}
	}
	else
		IsFasst = TRUE;
	m_pExchangeObj = CExchangeObjectMgr::GetInstance()->BeginExchange(this, pPlayer);
	if (m_pExchangeObj)
	{
		m_pExchangeObj->SetFastExchange(IsFasst);
		pPlayer->m_pExchangeObj = m_pExchangeObj;
	}
	return m_pExchangeObj != nullptr;
}

VOID CHumanPlayer::Send_Exchg_OtherAddItem(CHumanPlayer* pOther, ITEM& item)
{
	SendMsg(pOther->GetId(), SM_OTHERPUTTRADEITEM, 0, 0, 0, &item, sizeof(ITEMCLIENT));
}

VOID CHumanPlayer::Send_Exchg_OtherAddMoney(CHumanPlayer* pOther, money_type type, DWORD dwCount)
{
	SendMsg(dwCount, SM_OTHERPUTTRADEGOLD, 0, 0, (WORD)type, nullptr);
}

BOOL CHumanPlayer::PutTradeItem(DWORD dwMakeIndex)
{
	if (m_pExchangeObj == nullptr)return FALSE;
	ITEM* pItem = this->m_ItemBox.FindItem(dwMakeIndex);
	if (pItem == nullptr)return FALSE;
	if (CItemManager::GetInstance()->ItemLimited(*pItem, IL_NOEXCHANGE))
	{
		SaySystem("该物品不能交易");
		return FALSE;
	}
	if (m_pExchangeObj->PutItem(this, *pItem))
	{
		m_ItemBox.RemoveItem(pItem->dwMakeIndex);
		return TRUE;
	}
	return FALSE;
}

BOOL CHumanPlayer::PutTradeMoney(money_type type, DWORD dwCount)
{
	if (m_pExchangeObj == nullptr)return FALSE;
	DWORD dwTradeGold = m_pExchangeObj->GetGold(this, type);
	if (dwTradeGold == dwCount)return TRUE;
	if (dwTradeGold < dwCount)
	{
		if (!CostMoney(type, dwCount - dwTradeGold, FALSE))
		{
			SaySystem("您没有那么多钱可以支付!");
			return FALSE;
		}
	}
	else
	{
		if (!AddMoney(type, dwTradeGold - dwCount, FALSE))
		{
			SaySystem("您身上的钱太多, 无法容纳更多的钱!");
			return FALSE;
		}
	}
	if (!m_pExchangeObj->PutMoney(this, type, dwCount))
	{
		AddMoney(type, dwCount);
		return FALSE;
	}
	return TRUE;
}

BOOL CHumanPlayer::CancelTrade()
{
	if (m_pExchangeObj == nullptr)return FALSE;
	return m_pExchangeObj->End(this, ET_CANCEL);
}

BOOL CHumanPlayer::ConfirmTrade()
{
	if (m_pExchangeObj == nullptr)return FALSE;
	return m_pExchangeObj->End(this, ET_CONFIRM);
}

BOOL CHumanPlayer::CheckTradeOtherSideOk(CHumanPlayer* p)
{
	int x = getX();
	int y = getY();
	GETNEXTPOS(x, y, GetDirection());
	int tx = p->getX();
	int ty = p->getY();
	if (x != tx || y != ty)return FALSE;
	GETNEXTPOS(tx, ty, p->GetDirection());
	if (getX() != tx || getY() != ty)return FALSE;
	return TRUE;
}