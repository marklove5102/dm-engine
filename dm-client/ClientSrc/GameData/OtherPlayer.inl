inline bool COtherPlayer::IsMale()						{ return m_bIsMale;}

inline const char * COtherPlayer::GetName()		{ return m_strName.c_str();}
inline const char * COtherPlayer::GetGuildName(){ return m_strGuildName.c_str();}
inline const char * COtherPlayer::GetTitle()	{ return m_strTitle.c_str();}
inline LOOKS &      COtherPlayer::GetLooks()	{ return m_looks;}
inline DWORD        COtherPlayer::GetNameColor(){ return m_NameColor;}

inline CGood & COtherPlayer::GetEquipGood(int i){ return m_equipment[i];}

inline bool    COtherPlayer::IsGettingGuildLogo()		 { return m_bGettingTex;}
inline void    COtherPlayer::SetGettingGuildLogo(bool b) { m_bGettingTex = b;}
inline DWORD   COtherPlayer::GetLogoTexTime()		 { return m_dwGettingTexTime;}
inline void    COtherPlayer::SetLogoTexTime(DWORD dwTime) { m_dwGettingTexTime = dwTime;}

inline void    COtherPlayer::SetWing(bool b){ m_bWing = b;}
inline bool    COtherPlayer::HaveWing()     { return m_bWing;}

inline BYTE    COtherPlayer::GetYuanShenInfo()          { return m_nYuanShenInfo;}
inline bool    COtherPlayer::IsYuanShenMale()           { return (m_iYuanShenSex != 2);}
inline int     COtherPlayer::GetYSShapeType()           { return m_iYSShapeType;}
inline void    COtherPlayer::SetYSShapeType(int i)      {m_iYSShapeType = i;}

inline string& COtherPlayer::GetIndividualStr(){ return m_strIndividualStr;}

inline bool    COtherPlayer::GetLogo()      { return m_bLogo;}
inline void    COtherPlayer::SetLogo(bool b){m_bLogo = b;}