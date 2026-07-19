inline DWORD        CGoodDemonInfo::GetID()         { return dwItemID;}
inline DWORD        CGoodDemonInfo::GetRemainTime() { return dwRemainTime;}
inline const char*  CGoodDemonInfo::GetName()       { return szDemonName;}
inline BYTE         CGoodDemonInfo::GetSex()        { return bySex;}
inline BYTE         CGoodDemonInfo::GetDemonJob()   { return byJob;}
inline BYTE         CGoodDemonInfo::GetLevel()      { return byJingJie;}
inline BYTE		    CGoodDemonInfo::GetAC()			{ return m_nAC;}
inline BYTE		    CGoodDemonInfo::GetAC2()		{ return m_nAC2;}
inline BYTE		    CGoodDemonInfo::GetMAC()		{ return m_nMAC;}
inline BYTE		    CGoodDemonInfo::GetMAC2()		{ return m_nMAC2;}
inline BYTE		    CGoodDemonInfo::GetDC()			{ return m_nDC;}
inline BYTE		    CGoodDemonInfo::GetDC2()		{ return m_nDC2;}
inline BYTE		    CGoodDemonInfo::GetMC()			{ return m_nMC;}
inline BYTE		    CGoodDemonInfo::GetMC2()		{ return m_nMC2;}
inline BYTE		    CGoodDemonInfo::GetSC()			{ return m_nSC;}
inline BYTE		    CGoodDemonInfo::GetSC2()		{ return m_nSC2;}
inline int          CGoodDemonInfo::GetDarkDC()     { return m_iDarkDC;}
inline int          CGoodDemonInfo::GetDarkMC()     { return m_iDarkMC;}
inline int          CGoodDemonInfo::GetDarkAC()     { return m_iDarkAC;}
inline int          CGoodDemonInfo::GetDarkMAC()    { return m_iDarkMAC;}
inline int          CGoodDemonInfo::GetDarkSC()     { return m_iDarkSC;}
inline int          CGoodDemonInfo::GetDemonPower() { return m_iDemonPower;}
inline BYTE         CGoodDemonInfo::GetItemType()   { return m_nItemType;}
inline BYTE         CGoodDemonInfo::GetResv()       { return m_nResv;}

//Set...
inline void         CGoodDemonInfo::SetID(DWORD id)         { dwItemID = id;}
inline void         CGoodDemonInfo::SetRemainTime(DWORD t)  { dwRemainTime = t;}
inline void         CGoodDemonInfo::SetName(const char* str){ memcpy(szDemonName,str,max(15,strlen(str)));}
inline void	        CGoodDemonInfo::SetSex(BYTE nSex)		{ bySex = nSex;}
inline void	        CGoodDemonInfo::SetDemonJob(BYTE nJob)	{ byJob = nJob;}
inline void	        CGoodDemonInfo::SetLevel(BYTE nLevel)	{ byJingJie = nLevel;}
inline void	        CGoodDemonInfo::SetAC(BYTE nAC)		    { m_nAC = nAC;}
inline void	        CGoodDemonInfo::SetAC2(BYTE nAC2)		{ m_nAC2 = nAC2;}
inline void	        CGoodDemonInfo::SetMAC(BYTE nMAC)		{ m_nMAC = nMAC;}
inline void	        CGoodDemonInfo::SetMAC2(BYTE nMAC2)	    { m_nMAC2 = nMAC2;}
inline void	        CGoodDemonInfo::SetDC(BYTE nDC)		    { m_nDC	= nDC;}
inline void	        CGoodDemonInfo::SetDC2(BYTE nDC2)		{ m_nDC2 = nDC2;}
inline void	        CGoodDemonInfo::SetMC(BYTE nMC)		    { m_nMC = nMC;}
inline void	        CGoodDemonInfo::SetMC2(BYTE nMC2)		{ m_nMC2 = nMC2;}
inline void	        CGoodDemonInfo::SetSC(BYTE nSC)		    { m_nSC = nSC;}
inline void	        CGoodDemonInfo::SetSC2(BYTE nSC2)		{ m_nSC2 = nSC2;}
inline void	        CGoodDemonInfo::SetDarkAC(int iDarkAC)	{ m_iDarkAC = iDarkAC;}
inline void	        CGoodDemonInfo::SetDarkMAC(int iDarkMAC){ m_iDarkMAC = iDarkMAC;}
inline void	        CGoodDemonInfo::SetDarkDC(int iDarkDC)	{ m_iDarkDC	= iDarkDC;}
inline void	        CGoodDemonInfo::SetDarkMC(int iDarkMC)	{ m_iDarkMC = iDarkMC;}
inline void	        CGoodDemonInfo::SetDarkSC(int iDarkSC)	{ m_iDarkSC = iDarkSC;}
inline void         CGoodDemonInfo::SetDemonPower(int i)    { m_iDemonPower = i;}
inline void         CGoodDemonInfo::SetItemType(BYTE nType) { m_nItemType = nType;}
inline void         CGoodDemonInfo::SetResv(BYTE nResv)     { m_nResv = nResv;}