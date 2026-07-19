inline void  CSimpleCharacter::SetMonsterArmLevel(int i)       {m_iMonstArmLevel = i;}
inline int   CSimpleCharacter::GetMonsterArmLevel()                              {return m_iMonstArmLevel;}

//±ª×ÓÁôÑÔ
inline string& CSimpleCharacter::GetPetLeaveWords()       {return m_strLeaveWords;}
inline void  CSimpleCharacter::SetPetLeaveWords(const char * str){ m_strLeaveWords.clear(); m_strLeaveWords = str;}
inline DWORD CSimpleCharacter::GetPetLeaveWordsTime()          {return m_dwPetLeaveWordsTime;}
inline void  CSimpleCharacter::SetPetLeaveWordsTime(DWORD dw)  {m_dwPetLeaveWordsTime = dw;}

inline int CSimpleCharacter::GetSilkwormFrame()                {return m_iSilkwormFrame;   }
inline void  CSimpleCharacter::SetSilkwormFrame(int i)         {m_iSilkwormFrame = i;  }

inline void  CSimpleCharacter::SetGuildName(const char * strName){m_strGuildName = strName;}
inline const char * CSimpleCharacter::GetGuildName(){ return m_strGuildName.c_str();}
