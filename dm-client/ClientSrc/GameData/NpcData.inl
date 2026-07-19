inline void  CNpcData::SetID(DWORD id){ m_nNpcID = id;}
inline DWORD CNpcData::GetID()        { return m_nNpcID;}

inline void        CNpcData::SetName(const char * str){ m_name = str; }
inline const char* CNpcData::GetName()                { return m_name.c_str();}
inline void        CNpcData::SetTrayTitle(char *str)  { m_sTrayTitle = str;}
inline const char* CNpcData::GetTrayTitle()           { return m_sTrayTitle.c_str();}

//物品的买/卖/存放
inline void  CNpcData::SetSaleObjectValue(DWORD v){ m_SaleObjectValue = v; }
inline DWORD CNpcData::GetSaleObjectValue()		  { return m_SaleObjectValue; }
inline void  CNpcData::SetStorageMaxCount(WORD wCount){m_wStorageMaxCount = wCount;}
inline int   CNpcData::GetStorageMaxCount(){return m_wStorageMaxCount;}
inline void  CNpcData::SetExchangeFromQuickNpcWnd(bool b)	{m_bExchangeFromQuickNpcWnd = b;}
inline bool  CNpcData::IsExchangeFromQuickNpcWnd()	    {return m_bExchangeFromQuickNpcWnd;}
inline vector<_SellData> &  CNpcData::GetVectorSellData(){ return m_vecSellData; }
inline vector<_SellData> &  CNpcData::GetVectorPTData()  { return m_vecPTData;   }
inline vector<CGood> &      CNpcData::GetNpcQuickVector(){ return m_vecQuickData; }
inline SVipTradGood &      CNpcData::GetVipTradeGood(){ return m_VipTradeGood; }

inline CGood &              CNpcData::GetGood()          { return m_Good;}
inline void                 CNpcData::SetSaleGold(DWORD iMoney)
{   
    m_SaleGold = iMoney;
    if(iMoney==0)
        m_bStart= false;
}
inline DWORD CNpcData::GetSaleGold()				    { return m_SaleGold;}
inline bool  CNpcData::GetStart()						{ return m_bStart;}
inline void  CNpcData::SetStart(bool start)				{ m_bStart = start;}
inline vector<_StorageData> & CNpcData::GetStorageData(){ return m_StorageData;}
inline void CNpcData::SetSecondBuy(bool b)				{ m_bSecondBuy = b;}
inline bool CNpcData::GetSecondBuy()					{ return m_bSecondBuy;}
inline void CNpcData::SetPoisonFlag(bool b)				{ m_bMakePoison = b;}
inline bool CNpcData::GetPoisonFlag()					{ return m_bMakePoison;}
inline vector<CGood> & CNpcData::GetGoodList()			{ return m_GoodList;}
inline bool CNpcData::IsRepairWindow()					{ return m_bIsRepairWindow;}
inline void CNpcData::SetRepairWindow(bool b)			{ m_bIsRepairWindow = b;}
inline int  CNpcData::GetSecBuyStartPos()				{ return m_iSecBuyStart; }
inline void CNpcData::SetSecBuyStartPos(int start)		{ m_iSecBuyStart = start;}

inline VString& CNpcData::GetLeavWords()                { return m_vecLeavWords;}

inline void  CNpcData::SetChangeLogo(bool b)			{ m_bChangeLogo = b;}
inline bool  CNpcData::GetChangeLogo()					{ return m_bChangeLogo;}
inline void  CNpcData::SetForgeExp(DWORD dw)			{ m_dwForgeExp =dw;}
inline DWORD CNpcData::GetForgeExp()					{ return m_dwForgeExp;}

//黑暗残留换取装备升级
inline CGood& CNpcData::GetEquLUpGood(int i)            { return m_EquLvlUpGood[i];}
inline bool   CNpcData::IsLUpintEqu()                   { return m_bIsLUpintEqu;}
inline void   CNpcData::SetIsLUpintEqu(bool b)          { m_bIsLUpintEqu = b;}
inline int    CNpcData::GetLUpFrame()                   { return m_iLUpFrame;}
inline void   CNpcData::SetLUpFrame(int i)              { m_iLUpFrame = i;}

//宝石的凝练和合成
inline CGood& CNpcData::GetOreGood(int i)      { return m_OreGood[i];}
inline CGood& CNpcData::GetGemGood(int i)      { return m_GemGood[i];}
inline bool   CNpcData::IsCondensing()         { return m_bIsCondensing;}
inline void   CNpcData::SetIsCondensing(bool b){ m_bIsCondensing = b;}
inline int    CNpcData::GetCondenseFrame()     { return m_iCondenseFrame;}
inline void   CNpcData::SetCondenseFrame(int i){ m_iCondenseFrame = i;}

//宝箱的开启
inline _TreasureData& CNpcData::GetBoxGood(int iPos){return m_BoxGood[iPos];}
inline int CNpcData::GetBoxOpenState(){return m_iBoxOpenState;}
inline void CNpcData::SetBoxOpenState(int i){m_iBoxOpenState = i;}
inline int CNpcData::GetBoxType(){return m_iBoxType;}
inline void CNpcData::SetBoxType(int i){ m_iBoxType = i;}
inline int CNpcData::GetBoxOpenCycle(){return m_iBoxOpenCycle;}
inline void CNpcData::SetBoxOpenCycle(int i){m_iBoxOpenCycle = i;}

//设定装包裹的物品
inline DWORD CNpcData::GetPackGoodID()          { return m_dwPackGoodID;}
inline void  CNpcData::SetPackGoodID(DWORD dwID){ m_dwPackGoodID = dwID;}

//锁包裹的物品
inline DWORD CNpcData::GetSuoPackGoodID()          { return m_dwSuoPackGoodId;}
inline void  CNpcData::SetSuoPackGoodID(DWORD dwID){ m_dwSuoPackGoodId = dwID;}

//英雄榜
inline VString& CNpcData::GetVectorStrData()       { return m_vecStrData;}
inline int      CNpcData::GetVectorStrCount()      { return m_vecStrData.size();}
inline string&  CNpcData::GetTitleStrData()        { return m_strTitleData;}
inline void     CNpcData::SetTitleStrData(const char* str){ m_strTitleData = str;}

inline void         CNpcData::SetLastCommand(string& str){ m_strCommand=str;}
inline const char * CNpcData::GetLastCommand()           { return m_strCommand.c_str();}


//物品提交界面
inline CGood& CNpcData::GetSubmitGood()             { return m_SubmitGood;}
inline void   CNpcData::SetSubmitGoodType(int iType){ m_iSubmitGoodType = iType;}
inline int    CNpcData::GetSubmitGoodType()         { return m_iSubmitGoodType;}

inline void   CNpcData::SetSubmitGoodMsg1(const char* str){ m_strSubmitGoodMsg1 = str;}
inline void   CNpcData::SetSubmitGoodMsg2(const char* str){ m_strSubmitGoodMsg2 = str;}
inline const char* CNpcData::GetSubmitGoodMsg1(){ return m_strSubmitGoodMsg1.c_str();}
inline const char* CNpcData::GetSubmitGoodMsg2(){ return m_strSubmitGoodMsg2.c_str();}

//新年树由高到低分为三层
inline void  CNpcData::SetPrayTreeLevel(BYTE byLevel){ m_byPrayTreeLevel = byLevel;}
inline BYTE  CNpcData::GetPrayTreeLevel(){return m_byPrayTreeLevel;}
inline string& CNpcData::GetPrayTreePrize(){return m_strPrize;}
inline CGood& CNpcData::GetPrayCharm(){ return m_PrayCharm; }

inline vector<_ACTIVITY>& CNpcData::GetVectorActData()        { return m_vecActData;	}//每周任务


inline bool CNpcData::DelOperateGood(DWORD dwID)
{
	MOperateGood::iterator it = m_mOperateGood.find(dwID);
	if (it != m_mOperateGood.end())
	{
		m_mOperateGood.erase(it);

		return true;
	}

	return false;
}

inline void CNpcData::AddOperateGood(CGood& good)
{
	if(good.GetID() != 0)
		m_mOperateGood[good.GetID()] = good;
}

inline CGood* CNpcData::GetOperateGood(DWORD dwID)
{
	MOperateGood::iterator it = m_mOperateGood.find(dwID);
	if (it != m_mOperateGood.end())
	{
		return &(it->second);
	}

	return NULL;
}

inline CGood& CNpcData::GetCharGood()
{
	return m_GoodChar;
}

inline CGood& CNpcData::GetCharChangeGood()
{
	return m_GoodChange;
}

inline CGood& CNpcData::GetZhenPuGood()
{
	return m_GoodZhenPu;
}

inline CGood& CNpcData::GetWuSeGood()
{
	return m_GoodWuSe;
}


inline _EquipSoulInfo& CNpcData::GetEquipSoulInfo()
{
	return m_EquipSoulInfo;
}

