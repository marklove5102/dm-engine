
inline CCharacter&		CGameData::Self(){return m_self;}
inline DWORD			CGameData::GetMirColor(BYTE tByte)	{ return m_dwMirColor[tByte];		}

inline int CGameData::GetMouseType(){return m_iMouseType;}
inline DWORD CGameData::GetMouseTexID(){return m_dwMouseTexID;}
//纪录鼠标上元宝和金钱来路
inline DWORD CGameData::GetDropMoneyFrom(void){ return m_iDropMoneyFrom; }
inline DWORD CGameData::GetDropYuanBaoFrom(void){ return m_iDropYuanBaoFrom; }
//组队相关
inline ListString& CGameData::GetTroopMemberList(){ return m_vecTroopMember; }
inline void	CGameData::EnableTroop(bool b){ m_bTroop = b;}
inline void	CGameData::AddTroopMember(const char * name){ m_vecTroopMember.push_back(name); }
inline void	CGameData::DelAllTroopMember(){ m_vecTroopMember.clear(); }
inline bool	CGameData::IsAllowTroop(){ return m_bTroop; }
inline int	CGameData::GetTroopMembers(){ return m_vecTroopMember.size(); }
inline void CGameData::DelTroopMember(const char * str){m_vecTroopMember.remove(str);}
inline std::vector<DWORD> &    CGameData::GetGroupPos()	{return m_MemberPos;}

inline const char* CGameData::GetInvestURL(){ return m_strInvestURL.c_str(); }//获得调查问卷地址
inline void  CGameData::SetInvestURL(const char* str){ m_strInvestURL = str; }//调查问卷地址赋值

//-----------------------------------
inline CSimpleGoodNode *  CGameData::AddSimpleGood(int x,int y,DWORD nID)
{
	return MapArray.AddGood(x,y,nID);
}

inline CSimpleCharacterNode * CGameData::AddSimpleCharacter(int x,int y,DWORD nID)
{
	return MapArray.AddCharacter(x,y,nID);
}

inline CSimpleGoodNode* CGameData::FindSimpleGood(DWORD nID)
{
	return MapArray.FindSimpleGood(nID);
}

inline CSimpleCharacterNode* CGameData::FindSimpleCharacter(DWORD nID)
{
	return MapArray.FindSimpleCharacter(nID);
}

inline CSimpleCharacterNode* CGameData::FindSimpleCharacter(const char* name)
{
	return MapArray.FindSimpleCharacter(name);
}

inline void CGameData::DeleteCharacter(DWORD nID)
{
	return MapArray.DeleteCharacter(nID);
}

inline CSimpleCharacter* CGameData::FindCharacterByID(DWORD nID)
{
	if(nID == SELF.GetID())
		return &SELF;

	return MapArray.FindSimpleCharacter(nID);
}

inline bool CGameData::HasStatus(int i)
{
	if (i < 0 || i > 128) return false;

	return ((m_byNewHandStatus[i/8] & (1 << (i%8))) != 0);
}

inline bool CGameData::HasPaoPaoStatus(ePaoPaoPromptStatus i)
{
	return (((m_int64PaoPaoStatus >> i) & 1) != 0);
}

inline bool CGameData::HasPaoPaoStatus_Closed(ePaoPaoPromptStatus i)
{
	return (((m_int64PaoPaoStatus_Closed >> i) & 1) != 0);
}

inline void CGameData::AddPaoPaoStatus_Closed(ePaoPaoPromptStatus i)
{
	if (i < 0 || i > 63) 
		return;

	m_int64PaoPaoStatus_Closed |= ((unsigned __int64)1 << i);
}

inline void CGameData::RemoveHorseRunToDead()
{
	VHORSERUNTODEAD::iterator it = m_VHorseRunToDead.begin();
	if (it != m_VHorseRunToDead.end())
	{
		m_VHorseRunToDead.erase(it);
	}
}

inline HorseRunToDead* CGameData::GetFirstHorseRunToDead()
{
	VHORSERUNTODEAD::iterator it = m_VHorseRunToDead.begin();
	if (it != m_VHorseRunToDead.end())
	{
		return &(*it);
	}

	return NULL;
}

inline void CGameData::SetStatusInfo(BYTE* pStatus)
{
	if (!pStatus) return;
	memcpy(m_byNewHandStatus,pStatus,16);
}

inline void CGameData::SetStatusInfo(int i)
{
	if (i< 0 || i>128) return;
	
	m_byNewHandStatus[i/8] |= 1 << (i%8);
}
