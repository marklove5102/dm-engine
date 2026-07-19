inline VCharInfo& COtherData::GetCharList() { return m_VCharList;  }
inline void    COtherData::SetCharNo(int i)  { m_iCharNo = i;  }
inline int     COtherData::GetCharNo()      { return m_iCharNo; }
inline VCharInfo& COtherData::GetDelCharList(){ return m_VDelCharList; }
inline bool    COtherData::IsNewUser(){   return m_bNewUser;  }
inline bool    COtherData::IsAutoEnter() { return m_bAutoEnter; }
inline void    COtherData::SetAutoEnter(bool b){  m_bAutoEnter = b; }
inline bool    COtherData::IsReceivedCharList(){ return m_bReceivedCharList; }
inline void    COtherData::SetReceivedCharList(bool b){ m_bReceivedCharList = b; }
inline DWORD   COtherData::GetExpUpTime(){  return m_dwExpUpTime; }
inline void    COtherData::SetExpUpTime(DWORD dwTime){ m_dwExpUpTime = dwTime; }
inline DWORD   COtherData::GetMapInTime(){ return m_dwMapInTime; }
inline void    COtherData::SetMapInTime(DWORD dwTime){ m_dwMapInTime = dwTime; }
inline bool    COtherData::IsDay(){ return m_bDay; }
inline void    COtherData::SetDay(bool bDay){ m_bDay = bDay; }
inline COtherPlayer& COtherData::OtherPlayer(){ return m_OtherPlayer;  }


inline VOtherMsg& COtherData::GetOtherMsg(){return m_VOtherMsg;} 
inline VOtherMsg& COtherData::GetOtherMsg2(){return m_VOtherMsg2;}
inline VOtherMsg& COtherData::GetAnotherMsg(){return m_VAnotherMsg;} 
inline VCustomPiaohongMsg& COtherData::GetCustomPiaohong(){return m_VCustomMsg;} 

inline VRelationStruct& COtherData::GetRelationVector(){return m_vecRelation;}
inline VCommonConfirm&	COtherData::GetCommonConfirmVector()	{ return m_VCommonConfirm; }
inline VNoteTitle &      COtherData::GetNoteTitle(void){return m_NoteTitle;}
inline VLeaveWords &     COtherData::GetLeaveWordsVector()	{ return m_vecLeaveWords; }
inline MStringPair & COtherData::GetNoteBody(void){return m_NoteBody;}
inline bool	COtherData::IsAutoLeaveWord()							{ return m_bAutoLeaveWord; }
inline void	COtherData::SetAutoLeaveWord(bool bAutoLeaveWord)		{ m_bAutoLeaveWord = bAutoLeaveWord;}
inline _OffLinePrivate &	COtherData::GetOffLinePrivate() {return m_sOffLinePrivate;}
inline int		COtherData::GetDelFriendNo()		{return m_iDelFriend;}
inline void	COtherData::SetDelFriendNo(int i)	{m_iDelFriend = i;}
inline int		COtherData::GetFindPathReason(){ return m_iFindPathReason; }
inline void	COtherData::SetFindPathReason(int iReason){ m_iFindPathReason = iReason;}
inline void	COtherData::GetFindPathClickNpcXY(int &iX,int &iY){iX = m_iFindPathClickNpcX;iY = m_iFindPathClickNpcY;}
inline void	COtherData::SetFindPathClickNpcXY(int iX,int iY){m_iFindPathClickNpcX = iX ;m_iFindPathClickNpcY = iY;}
inline DWORD	COtherData::GetEnterGameTime(){return m_dwEnterGameTime;}
inline void	COtherData::SetEnterGameTime(DWORD dwTime){m_dwEnterGameTime = dwTime;}
inline DWORD	COtherData::GetLastMovePosTime(){return m_dwLastMovePosTime;}
inline void	COtherData::SetLastMovePosTime(DWORD dwTime){m_dwLastMovePosTime = dwTime;}
inline DWORD COtherData::GetCenserStartTime(){return m_dwCenserStartTime;}
inline DWORD COtherData::GetCenserDurTime(){return m_dwCenserDurTime;}
inline void COtherData::SetCenserDurTime(DWORD dwDurTime){m_dwCenserDurTime = dwDurTime;}
inline bool COtherData::IsCenserStart(){return m_bCenserStart;}
inline void COtherData::SetCenserStart(bool bStart){m_bCenserStart = bStart;}
inline const char* COtherData::GetDeleteMember(){return m_strDeleteMember.c_str();}
inline void  COtherData::SetDeleteMember(const char* strName){m_strDeleteMember = strName;}
inline DWORD COtherData::GetSetMsgStartTime() { return m_dwSetMsgStartTime; }
inline void  COtherData::SetSetMsgStartTime(DWORD dwTime) { m_dwSetMsgStartTime = dwTime; }
inline string& COtherData::GetSetMsg() { return m_strSetMsg; }
inline VTimeOut& COtherData::GetTimeOut(){return m_TimeOut;}
inline void COtherData::SetReadUsbEkeyDelayStartTime(DWORD dwTime){m_dwReadUsbEkeyDelayStartTime = dwTime;}
inline DWORD COtherData::GetReadUsbEkeyDelayStartTime(){return m_dwReadUsbEkeyDelayStartTime;}
inline bool COtherData::IsNeedFlashProduceTab(){ return m_bNeedFlashProduceTab; }
inline void COtherData::SetNeedFlashProduceTab(bool val) { m_bNeedFlashProduceTab = val; }
inline int COtherData::GetFlashRelationTabType(){ return m_iFlashRelationTabType; }
inline void COtherData::SetFlashRelationTabType(int val) { m_iFlashRelationTabType = val; }

inline VQunYingReportStruct& COtherData::GetQunYingReports() { return m_VQunYingReports; }

inline void COtherData::SetFireArtTime(DWORD time)	{ m_dwFireArtTime = time; }
inline DWORD COtherData::GetFireArtTime()			{ return m_dwFireArtTime; }
inline void COtherData::SetFireArtStartTime(DWORD time) { m_dwFireArtStartTime = time; }
inline DWORD COtherData::GetFireArtStartTime()			{ return m_dwFireArtStartTime; }

inline S_VipTrade & COtherData::GetVipTradeStruct(){return m_SVipTrade;}



