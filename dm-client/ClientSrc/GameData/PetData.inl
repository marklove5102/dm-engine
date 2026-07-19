inline int  CPetData::GetAdoptPetNum()                { return m_iTotalAdoptPetNum;}
inline void CPetData::SetAdoptPetNum(DWORD dwNum)     { m_iTotalAdoptPetNum = dwNum;}
inline int  CPetData::GetCarrayBackPetNum()           { return m_iTotalCarryBackPetNum;}
inline void CPetData::SetCarrayBackPetNum(DWORD dwNum){ m_iTotalCarryBackPetNum = dwNum;}

inline PET_SENDOUT &   CPetData::GetSendOutPet()     { return m_SendOutPet;}
inline PET_ADOPT_MAP & CPetData::GetPetAdoptMap()    { return m_MPetAdopt;}
inline PET_ADOPT_MAP & CPetData::GetPetCarryBackMap(){ return m_MPetCarryBack;}
inline PET_ADOPT_MAP & CPetData::GetMyAdoptMap()     { return m_MMyAdoptPet;}
inline PET_SENDOUT_MAP & CPetData::GetSendOutPetList()     { return m_MSendOutPet;}
inline PET_HORSE & CPetData::GetMyHorse(){return m_sHorse;}
inline PET_STATUS_VECTOR & CPetData::GetPetStatus(){return m_vPetStatus;}

