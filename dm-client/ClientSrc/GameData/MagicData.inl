inline void CMagicData::SetShortCutKeyEx(DWORD key)  { m_dwShortCutKeyEx = key; }
inline DWORD CMagicData::GetShortCutKeyEx()          { return m_dwShortCutKeyEx;}
inline void CMagicData::SetShortCutKey(char key)	 { m_cShortCutKey = key; }
inline char CMagicData::GetShortCutKey()			 { return m_cShortCutKey;}
inline void CMagicData::SetMagicLevel(char level)	 { m_iMagicLevel = level;}
inline char CMagicData::GetMagicLevel()				 { return m_iMagicLevel; }
inline void CMagicData::SetNewSpell(WORD wNewSpell)  { m_wNewSpell = wNewSpell;}
inline WORD CMagicData::GetNewSpell()				 { return m_wNewSpell;}
inline void	CMagicData::SetNewDefSpell(BYTE byNewDefSpell){m_byNewDefSpell = byNewDefSpell;}
inline BYTE CMagicData::GetNewDefSpell()			 {return m_byNewDefSpell;}
inline void CMagicData::SetSkillValue(WORD value)	 { m_wSkillValue = value;}
inline WORD CMagicData::GetSkillValue()				 { return m_wSkillValue; }
inline void CMagicData::SetMagicID(WORD id)			 { m_wMagicID = id;  }
inline WORD CMagicData::GetMagicID()				 { return m_wMagicID; }
inline void CMagicData::SetMagicName(const char * strName)		 { m_strName = strName; }
inline const char * CMagicData::GetMagicName()		 { return m_strName.c_str();}	
inline void CMagicData::SetMagicType(char type)	 { m_MagicType = type; }
inline char CMagicData::GetMagicType()				 { return m_MagicType; }
inline void CMagicData::SetEffect(char eff)			 { m_Eff = eff; }
inline char CMagicData::GetEffect()					 { return m_Eff;}
inline void CMagicData::SetSpell(WORD spell)		 { m_wSpell = spell; }
inline WORD CMagicData::GetSpell()					 { return m_wSpell; }
inline void CMagicData::SetPower(WORD power)		 { m_wPower = power;}
inline WORD CMagicData::GetPower()					 { return m_wPower; }
inline void CMagicData::SetNeedLevel(int i,char level)	 { m_NeedLevel[i] = level;}
inline char CMagicData::GetNeedLevel(int i)				 { return m_NeedLevel[i]; }
inline void CMagicData::SetTrain(int i, DWORD train) { m_Train[i] = train; }
inline DWORD CMagicData::GetTrain(int i)			 { return m_Train[i]; }
inline void CMagicData::SetJob(char job)			 { m_iJob = job; }
inline char CMagicData::GetJob()					 { return m_iJob;}
inline void CMagicData::SetDefence(char def)		{ m_Def = def;}
inline char CMagicData::GetDefence()					{ return m_Def;}
inline void CMagicData::SetDefencePower(char defPower)	{ m_DefPower = defPower;}
inline char CMagicData::GetDefencePower()				{ return m_DefPower;}
inline void CMagicData::SetMaxPower(char maxPower)		{ m_MaxPower = maxPower; }
inline char CMagicData::GetMaxPower()					{ return m_MaxPower;}
inline void CMagicData::SetDefenceMaxPower(char power)	{ m_DefMaxPower = power; }
inline char CMagicData::GetDefenceMaxPower()			{ return m_DefMaxPower; }
inline WORD CMagicData::GetMagicDelay()					{ return m_nDelay; }
inline void CMagicData::SetMagicDelay(WORD m)			{ m_nDelay = m;}
inline void CMagicData::SetColor(DWORD color)			{ m_color = color;}
inline DWORD CMagicData::GetColor()						{ return m_color; }

inline void CMagicData::SetDisable(bool b){m_bDisable = b;}
inline bool CMagicData::IsDisable(){return m_bDisable;}
inline DWORD CMagicData::GetFlashTexID(){return m_dwFlashTexID;}
inline void CMagicData::SetFlashTexID(DWORD dwID){m_dwFlashTexID = dwID;}
