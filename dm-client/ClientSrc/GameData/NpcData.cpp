#include "npcdata.h"
#include "GameData.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

CNpcData  g_NPC;

const WORD	COMPOUND_GEM_STDMODE = 61;//合成宝石stdmode
const WORD	TIME_ITEM_STDMODE = 38;//存在时间(创建时间、有效期)的物品的stdmode
const WORD	SETEXT_ITEM_STDMODE = 9;//附加属性记录特殊信息的stdmode

CNpcData::CNpcData(void)
{
	m_SaleObjectValue = 0xffffffff;
	m_SaleGold =0;
	m_bStart = true;
	m_bSecondBuy = false;
	m_bMakePoison = false;
	m_bIsRepairWindow = false;
	m_iSecBuyStart = 0;
	m_wStorageMaxCount = 0;
    m_bExchangeFromQuickNpcWnd = false;

	m_nNpcID	= 0;
	m_bChangeLogo = false;
	m_dwForgeExp =-1;
	m_dwPackGoodID = 0;
	m_dwSuoPackGoodId = 0;
	m_iBoxOpenCycle = 0;

	m_iSubmitGoodType = 0;
	m_SubmitGood.SetID(0);
	m_strSubmitGoodMsg1.clear();
	m_strSubmitGoodMsg2.clear();

	m_byPrayTreeLevel = 0xff;

	m_bIsCondensing = false;
    m_DamageGood.SetID(0);
	m_ShenGongFangGoods.clear();

	m_strSelFriendName.clear();
	m_kHorsemanshipWatchInfo.clear();
	m_kOtherHorsemanshipWatchInfo.clear();
}

CNpcData::~CNpcData(void)
{
}

////////////////////////////////////////////////////////////////////////////////
//清除所有数据
void CNpcData::Clear()
{
    m_SaleObjectValue = 0xffffffff;
	m_SaleGold =0;
	m_bStart = true;
	m_bSecondBuy = false;
	m_bMakePoison = false;
	m_nNpcID	= 0;
	m_wStorageMaxCount = 0;

	m_StorageData.clear();
	m_name.clear();
	m_vecSellData.clear();
	m_vecQuickData.clear();
	m_vecPTData.clear();
	m_vecStrData.clear();
	m_vecLeavWords.clear();

	m_nNpcID	=0;
	m_Good.SetID(0);
	m_dwForgeExp =-1;
	m_bExchangeFromQuickNpcWnd = false;

	for(int i=0;i<MAX_COMMON_GOOD_COUNT;i++)
	{
		m_GemGood[i].SetID(0);
		m_EquLvlUpGood[i].SetID(0);
	}
	m_bIsCondensing = false;
	m_bIsLUpintEqu = false;

	m_iCondenseFrame = -1;
	m_iLUpFrame = -1;
	m_iBoxType = 0;
	m_iBoxOpenState = -1;
	
	m_dwSuoPackGoodId = 0;
	m_strCommand.clear();

	for(int i=0;i<16;i++)
	{
		m_OreGood[i].SetID(0);
	}


	m_SubmitGood.SetID(0);
	m_iSubmitGoodType = 0;
	m_strSubmitGoodMsg1.clear();
	m_strSubmitGoodMsg2.clear();
 
	m_vecActData.clear();

	m_stWuxingResult.Clear();
	m_ShenGongFangGoods.clear();

	m_GoodChar.clear();
	m_GoodChange.clear();

	m_VipTradeGood.Clear();

	m_strSelFriendName.clear();
	m_kHorsemanshipWatchInfo.clear();
	m_kOtherHorsemanshipWatchInfo.clear();
}

CGood* CNpcData::FindNpcGood(DWORD id)
{
	if(m_Good.GetID() == id)
		return &m_Good;

	if(m_GoodChar.GetID() == id)
		return &m_GoodChar;

	if(m_GoodChange.GetID() == id)
		return &m_GoodChange;

	if(m_PrayCharm.GetID() == id)
		return &m_PrayCharm;

	int i = 0;
	for(i = 0;i < MAX_COMMON_GOOD_COUNT;i++)
	{
		if (m_GemGood[i].GetID() == id)
			return &m_GemGood[i];
	}

	for(i = 0;i < MAX_ORE_COUNT;i++)
	{
		if (m_OreGood[i].GetID() == id)
			return &m_OreGood[i];
	}

	for(i = 0; i < MAX_LEVELUP_COUNT;i++)
	{
		if(m_EquLvlUpGood[i].GetID() == id)
			return &m_EquLvlUpGood[i];
	}

	return NULL;
}

void CNpcData::BackToPackage()
{
	if(m_Good.GetID()!=0)
	{
		SELF.PackageGood().BackToArray(m_Good);
		m_Good.SetID(0);
		SetSaleGold(0);
	}
}

bool CNpcData::RetToPkgFromOperateGood(DWORD dwID)
{
	CGood* pGood = GetOperateGood(dwID);

	if (pGood && pGood->GetID() != 0)
	{
		SELF.PackageGood().BackToArray(*pGood);
		DelOperateGood(pGood->GetID());

		return true;
	}
		
	return false;
}

VTreeGift* CNpcData::GetGiftsByTreeLevel(int iTreeLevel)
{
	if(iTreeLevel < 1 || iTreeLevel > 3)
		return NULL;

	return &m_VecTreeGift[iTreeLevel - 1];
}