inline void			CCharacterAttr::SetID(DWORD nID){ m_nID = nID; }
inline void			CCharacterAttr::SetTitle(const char * strTitle){ m_strTitle = strTitle; }
inline void			CCharacterAttr::SetDir(BYTE nDir)	{ if(nDir >=0 && nDir < 8) m_nDirection = nDir;}
inline void			CCharacterAttr::SetFlags(int nFlags)			{ m_nFlags = nFlags;}
inline void			CCharacterAttr::SetHP(WORD nLife)				{ m_nHP = nLife;}
inline void			CCharacterAttr::SetHPMax(WORD nLifeMax)			{ m_nHPMax = nLifeMax;}
inline void			CCharacterAttr::SetMP(WORD nMana)				{ m_nMP = nMana;}
inline void			CCharacterAttr::SetMPMax(WORD nManaMax)			{ m_nMPMax = nManaMax;}
inline void			CCharacterAttr::SetXY(int x,int y)              { m_iX = x; m_iY = y; }
inline void         CCharacterAttr::SetScrXY(int x,int y)           { m_iScrX = x; m_iScrY = y; }
inline void         CCharacterAttr::SetTopXY(int x,int y)           { m_iTopX = x; m_iTopY = y; }
inline void         CCharacterAttr::SetTexWH(int w,int h)           { m_iTexW = w; m_iTexH = h; }
inline void         CCharacterAttr::SetTexOffXY(int x,int y)        { m_iTexOffX = x; m_iTexOffY = y; }
inline void			CCharacterAttr::SetNameColor(char cColor)		{ m_cNameColor = cColor;}
inline void			CCharacterAttr::SetOffset(int x, int y)			{ m_iOffX = x; m_iOffY = y;}
inline void			CCharacterAttr::SetDrawOffset(int x, int y)			{ m_iDrawOffX = x; m_iDrawOffY = y;}
inline void	        CCharacterAttr::SetUsingMagicType(int iType)    { m_iUsingMagicType = iType;}
inline void	        CCharacterAttr::AddUsingMagicType(int iType)    { m_iUsingMagicType |= iType;}
inline void	        CCharacterAttr::RemoveUsingMagicType(int iType)    { m_iUsingMagicType &= ~iType;}

inline bool         CCharacterAttr::HasMagicState(int iState)       { return (m_iMagicState & iState) != 0; }
inline void         CCharacterAttr::AddMagicState(int iState)       { m_iMagicState |= iState; }
inline void         CCharacterAttr::RemoveMagicState(int iState)    { m_iMagicState &= ~iState; } 

//动作相关
inline SAction&		CCharacterAttr::GetAction()						{ return m_ActionNow; }
inline SNextAction * CCharacterAttr::GetNextAction()				{ return m_pNextBegin; }
inline SNextAction& CCharacterAttr::GetOldNextAction()              { return m_OldNextAction; }

inline void CCharacterAttr::SetTuTengState(DWORD dwState){m_dwTuTengState = dwState;}
inline DWORD CCharacterAttr::GetTuTengState(){return m_dwTuTengState;}

inline void CCharacterAttr::SetFaBaoType(int dwType ,int iLevel,BYTE byAnimation){m_sFaBao.SetCurFaBaoType(dwType, iLevel,byAnimation);}
inline void CCharacterAttr::SetLeftFaBaoType(int dwType){m_sFaBao.dwLeftFaBaoType = dwType;}
inline void CCharacterAttr::SetRightFaBaoType(int dwType){m_sFaBao.dwRightFaBaoType = dwType;}
inline void	CCharacterAttr::UseFaZhen(bool use){ m_sFaBao.bFaZhen = use;}
inline bool	CCharacterAttr::IsUsingFaZhen(){ return m_sFaBao.bFaZhen;}
inline FABAOStruct* CCharacterAttr::GetFaBaoType(){return &m_sFaBao;}
inline void CCharacterAttr::HideFaBao(int dwType){m_sFaBao.iHide += (dwType?dwType:-1);}
inline void    CCharacterAttr::SetSanWeiFireLevel(BYTE byLevel){m_sFaBao.bySanWeiFireLevel = byLevel;}
inline BYTE    CCharacterAttr::GetSanWeiFireLevel(){return m_sFaBao.bySanWeiFireLevel;}

inline E_NEXT_STATE	CCharacterAttr::HasNextAction()
{
	if(m_pNextBegin != NULL && m_pNextEnd != NULL)
		return ENS_SERVER_ACTION;
	else
		return ENS_NO_ACTION;
}
inline tex_show_t*  CCharacterAttr::GetTexFront(){   return m_pTexFront; }
inline tex_show_t*  CCharacterAttr::GetTexBack(){  return m_pTexBack; }
inline IMagicCase*  CCharacterAttr::GetMagicCase(){  return m_pMagicCase; }

inline int			CCharacterAttr::IsSpeedup()					{ return m_iSpeedup; }
inline void			CCharacterAttr::SetSpeedup(int i)			{ m_iSpeedup = i; }

inline void			CCharacterAttr::SetDeal(bool b)					{ m_bDeal = b; }
inline void			CCharacterAttr::SetWait(bool b)					{ m_bWait = b; }
inline WORD         CCharacterAttr::GetStatus()                     { return m_wStatus;  }
inline void         CCharacterAttr::RemoveStatus(WORD state)			{ m_wStatus &= ~state;  }
inline bool         CCharacterAttr::HasStatus(WORD state)           { return (m_wStatus & state) != 0; }

inline void         CCharacterAttr::SetExtraState(WORD state)       { m_ExtraState = state;}
inline void         CCharacterAttr::RemoveExtraState(WORD state)    { m_ExtraState &= ~state;}
inline void			CCharacterAttr::SetDead(bool b)					{ m_bDead = b; }

inline void			CCharacterAttr::GetXY(int &x,int &y)			{ x=m_iX; y=m_iY; }
inline int          CCharacterAttr::GetX()                          { return m_iX; }
inline int          CCharacterAttr::GetY()                          { return m_iY; }
inline void			CCharacterAttr::GetRealXY(int &x,int &y)		{ x=m_iRealX; y=m_iRealY; }
inline void			CCharacterAttr::GetScrXY(int &x,int &y)		    { x=m_iScrX; y=m_iScrY; }
inline void			CCharacterAttr::GetTopXY(int &x,int &y)		    { x=m_iTopX; y=m_iTopY; }
inline void         CCharacterAttr::GetTexWH(int &w,int &h)         { w = m_iTexW; h = m_iTexH;}
inline void         CCharacterAttr::GetTexOffXY(int &x,int &y)      { x = m_iTexOffX; y = m_iTexOffY; }
inline int          CCharacterAttr::GetTexW()                       {return m_iTexW;}
inline int          CCharacterAttr::GetTexH()                       {return m_iTexH;}

inline void         CCharacterAttr::GetPlayerXY(int& x,int & y)
{
	x = m_iX + m_iOffX / 64;
	y = m_iY + m_iOffY / 32;
}

inline DWORD        CCharacterAttr::GetID()						    { return m_nID; }
inline const char * CCharacterAttr::GetName()						{ return m_strName.c_str();}
inline const char * CCharacterAttr::GetTitle()						{ return m_strTitle.c_str();}
inline LOOKS&	    CCharacterAttr::GetLooks()					    { return m_Looks;}
inline __int64        CCharacterAttr::GetLooksValue()                 { return m_Looks.Player.iLooks; }
inline char			CCharacterAttr::GetDir()						{ return m_nDirection;}
inline int			CCharacterAttr::GetFlags()						{ return m_nFlags;}
inline WORD			CCharacterAttr::GetHP()							{ return m_nHP;}
inline WORD			CCharacterAttr::GetHPMax()						{ return m_nHPMax;}
inline WORD			CCharacterAttr::GetMP()							{ return m_nMP;}
inline WORD			CCharacterAttr::GetMPMax()						{ return m_nMPMax;}
inline BYTE		    CCharacterAttr::GetNameColor()					{ return m_cNameColor;}
inline void 		CCharacterAttr::GetOffset(int & x, int & y)		{ x = m_iOffX; y = m_iOffY;}
inline void 		CCharacterAttr::GetDrawOffset(int & x, int & y)		{ x = m_iDrawOffX; y = m_iDrawOffY;}
inline int			CCharacterAttr::GetMagicState()					{ return m_iMagicState;}	
inline int          CCharacterAttr::GetUsingMagicType()             { return m_iUsingMagicType;}

inline VString&		CCharacterAttr::GetShowName()			{ return m_showName; }

inline void			CCharacterAttr::SetLevel(WORD level){  m_iLevel = level;  }
inline WORD			CCharacterAttr::GetLevel()       {    return m_iLevel;    }

inline bool			CCharacterAttr::IsDeal()						{ return m_bDeal; }
inline bool			CCharacterAttr::IsWait()						{ return m_bWait; }
inline WORD         CCharacterAttr::GetExtraState()                 { return m_ExtraState;}
inline bool			CCharacterAttr::IsDead()						{ return m_bDead; }

inline void         CCharacterAttr::SetSex(int i) 
{ 
	if (m_Looks.Player.byType == CHARACTER_MONSTER)
	{
		if (i & 0x20) SetCamp(1);
		else if (i & 0x40) SetCamp( 2);
		else SetCamp(0);
	}

	m_iSex = i & 0x01;
}

inline int          CCharacterAttr::GetSex() { return m_iSex; }
inline bool			CCharacterAttr::IsMale(){return ( m_iSex == 0 );}

inline int			CCharacterAttr::GetCareer()		{ return m_nCareer;}
inline void			CCharacterAttr::SetCareer(int nCareer)	{ m_nCareer = nCareer;}

inline int			CCharacterAttr::GetRaceType() { return m_Looks.Player.byType; }
inline int			CCharacterAttr::GetRaceNo()
{
	if(GetRaceType() == CHARACTER_HUMAN)return 0;
	else if(GetRaceType() == CHARACTER_NPC)return m_Looks.Char.wShape;
	return m_Looks.Char.wShape;
}
inline DWORD		CCharacterAttr::GetNpcActionTm(){return m_dwNpcActionTm;}
inline void			CCharacterAttr::SetNpcActionTm(DWORD dwTm){m_dwNpcActionTm = dwTm;}
inline BYTE			CCharacterAttr::GetTalkFontType()	{ return m_cTalkFontType; }
inline int          CCharacterAttr::GetFontSize()       { return m_iFontSize;  }
inline DWORD		CCharacterAttr::GetTalkColor()		{ return m_dwTalkColor; }
inline VString&     CCharacterAttr::GetTalk()			{ return m_VTalk; }
inline DWORD        CCharacterAttr::GetTalkShowTime()		{return m_dwTalkShowTime; }

inline void			CCharacterAttr::SetStopGo(bool b)				{ m_bStopGo = b; }
inline bool			CCharacterAttr::IsStopGo()						{ return m_bStopGo; }

inline void			CCharacterAttr::SetAttackSpeed(int i)			{ m_iAttackSpeed = i; }
inline int			CCharacterAttr::GetAttackSpeed()				{ return m_iAttackSpeed; }
inline void			CCharacterAttr::SetShowHP(UCHAR c)				{ m_cShowHP = c; }
inline UCHAR		CCharacterAttr::GetShowHP()						{ return m_cShowHP; }
inline LOOKS&		CCharacterAttr::GetBianShenLooks()		        { return m_BianshenLooks;}
inline __int64        CCharacterAttr::GetBianShenLooksValue()         { return m_BianshenLooks.Player.iLooks; }
inline bool			CCharacterAttr::IsBianShen()                    { return m_BianshenLooks.Player.iLooks != 0;  }
inline WORD         CCharacterAttr::GetBody()          { return m_Looks.Player.wBody;}

inline void			CCharacterAttr::SetLastTex(PlayerTex* pLastTex)
{
	if(pLastTex)
	{
		memcpy(&m_LastTex,pLastTex,sizeof(PlayerTex));
	}
}

inline PlayerTex*		CCharacterAttr::GetLastTex(){  return &m_LastTex; }

//摆摊
inline bool	 CCharacterAttr::GetBoothState() { return m_bBoothState; }
inline void	 CCharacterAttr::SetBoothState(bool b) { m_bBoothState = b; }
inline BoothInfo_t&	 CCharacterAttr::GetBoothInfo() { return m_BoothInfo; }

//社会关系
inline DWORD    CCharacterAttr::GetRelationType() {  return m_dwRelationType;  }
inline void     CCharacterAttr::AddRelationType(DWORD type) {  m_dwRelationType |= type; }
inline void     CCharacterAttr::RemoveRelationType(DWORD type){  m_dwRelationType &= ~type; }
inline bool     CCharacterAttr::IsGrouped(){ return (m_dwRelationType & RT_GROUP) != 0 ;}//是否组队
inline bool     CCharacterAttr::IsJieYi(){ return (m_dwRelationType & RT_JIEYI) != 0 ;}//是否结义


inline void     CCharacterAttr::SetMaskName(int level,const char* maskname) { m_iMaskLevel = level, m_strMaskName.assign(maskname); }
inline const char*  CCharacterAttr::GetMaskName()  { return m_strMaskName.c_str();  }
inline int     CCharacterAttr::GetMaskLevel()  {   return m_iMaskLevel; }
inline DWORD    CCharacterAttr::GetReserveData(int i){return m_dwReserveData[i];}
inline void		CCharacterAttr::AddReserveData(int i){	m_dwReserveData[i]++;}
inline void		CCharacterAttr::DecReserveData(int i){	m_dwReserveData[i]--;}

inline void CCharacterAttr::SetShield(int iShieldShape)
{
	if (iShieldShape >= 1 && iShieldShape <= 16)
	{
		m_iPutOnShield = (iShieldShape - 1) / 4 + 1;
		m_iPutOnShieldLevel = (iShieldShape - 1) % 4;
	}
	else if (iShieldShape >= 17 && iShieldShape <= 19)//血月虎王盾
	{
		m_iPutOnShield = 5;
		m_iPutOnShieldLevel = (iShieldShape - 17) + 1;
	}
	else
	{
		m_iPutOnShield = 0;
		m_iPutOnShieldLevel = 0;
	}
}


inline WORD CCharacterAttr::GetGreateMagicState(){return m_wGreateMagicState;}
inline void CCharacterAttr::AddGreateMagicState(eGREATEMAGIC e){m_wGreateMagicState |= e;}
inline void	CCharacterAttr::RemoveGreateMagicState(eGREATEMAGIC e){m_wGreateMagicState &= (~e);}

inline BYTE   CCharacterAttr::GetVipCardType()			{ return m_byVipCardType; }
inline void   CCharacterAttr::SetVipCardType(BYTE val)	{ m_byVipCardType = val; }
inline BYTE   CCharacterAttr::GetVipTradeLevel()		{ return m_byVipTradeLevel; }
inline void   CCharacterAttr::SetVipTradeLevel(BYTE val){ m_byVipTradeLevel = val; }
inline void	  CCharacterAttr::Set12GongTitle(BYTE val)	{ m_by12GongTitle = val; }
inline BYTE   CCharacterAttr::Get12GongTitle()			{ return m_by12GongTitle; }
inline bool   CCharacterAttr::IsCanDig()				{ return m_bCanDig; }
inline void   CCharacterAttr::SetCanDig(bool val)		{ m_bCanDig = val; }
inline void   CCharacterAttr::SetCamp(BYTE byValue)		{ m_byCamp = byValue;}
inline BYTE   CCharacterAttr::GetCamp()					{ return m_byCamp; }
// inline void   CCharacterAttr::SetTrusteeshipFlag(BYTE byFlag){m_byTrusteeshipFlag = byFlag;}
// inline BYTE   CCharacterAttr::GetTrusteeshipFlag(){return m_byTrusteeshipFlag;}

inline int    CCharacterAttr::GetCharHolyWingLevel(){return m_iHolyWingLevel;}
inline int    CCharacterAttr::GetCharHolyWingStrongLevel(){return m_iHolyWingStrongLevel;}
inline void   CCharacterAttr::SetCharHolyWingLevel(int val){m_iHolyWingLevel = val;}
inline void   CCharacterAttr::SetCharHolyWingStrongLevel(int val){m_iHolyWingStrongLevel = val;}
