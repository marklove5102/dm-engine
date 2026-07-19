
inline void	CGood::SetName(const char * name)   {memset(m_ItemPacket.szName,0,24); strcpy_s(m_ItemPacket.szName,23,name);}
inline void	CGood::SetStdMode(int stdmode)		{ m_ItemPacket.byStdMode = stdmode; }
inline void	CGood::SetShape(BYTE nShape)	{ m_ItemPacket.byShape = nShape; }
inline void	CGood::SetWeight(int nWeight)			{ m_ItemPacket.byWeight = nWeight;}
inline void	CGood::SetLooks(int nLooks)			{ m_ItemPacket.wLooks  = nLooks; }
inline void	CGood::SetDura(WORD nDura)		{ m_ItemPacket.wItemDura = nDura; }
inline void	CGood::SetDuraMax(WORD nDuraMax){ m_ItemPacket.wMaxItemDura = nDuraMax; }
inline void	CGood::SetOriDuraMax(WORD nDuraMax){ m_ItemPacket.wDuraMax = nDuraMax; }
inline void	CGood::SetAC(BYTE nAC)		{ m_ItemPacket.byAC1 = nAC; }
inline void	CGood::SetAC2(BYTE nAC2)		{ m_ItemPacket.byAC2 = nAC2;}
inline void	CGood::SetMAC(BYTE nMAC)		{ m_ItemPacket.byMAC1 = nMAC; }
inline void	CGood::SetMAC2(BYTE nMAC2)	{ m_ItemPacket.byMAC2 = nMAC2;}
inline void	CGood::SetDC(BYTE nDC)		{ m_ItemPacket.byDC1 = nDC;  }
inline void	CGood::SetDC2(BYTE nDC2)		{ m_ItemPacket.byDC2 = nDC2; }
inline void	CGood::SetMC(BYTE nMC)		{ m_ItemPacket.byMC1 = nMC;  }
inline void	CGood::SetMC2(BYTE nMC2)		{ m_ItemPacket.byMC2 = nMC2; }
inline void	CGood::SetSC(BYTE nSC)		{ m_ItemPacket.bySC1 = nSC;  }
inline void	CGood::SetSC2(BYTE nSC2)		{ m_ItemPacket.bySC2 = nSC2; }
inline void	CGood::SetNeed(BYTE nNeed)		{ m_ItemPacket.byNeed = nNeed;}
inline void	CGood::SetNeedLevel(BYTE nNeedLevel){ m_ItemPacket.byNeedLevel = nNeedLevel;}
inline void	CGood::SetPrice(DWORD nPrice)	{ m_ItemPacket.dwPrice = nPrice; }
inline void CGood::SetFlag(DWORD flag)		{ /*m_ItemPacket.dwFlag  = flag;*/ }
inline void	CGood::SetFlag1(WORD nFlag1)			{ /*m_ItemPacket.wFlags1 = nFlag1; */}
inline void	CGood::SetFlag2(int nFlag2)			{ m_nFlag2 = nFlag2; }
inline void CGood::SetAttachSkill(BYTE uAttachSkill)   { m_ItemPacket.szReserved[8] = uAttachSkill;}
inline void CGood::SetAttackSkillLevel(BYTE uSkillLevel) { m_ItemPacket.szReserved[9] = uSkillLevel;}

inline void CGood::SetDemondark1(WORD wDemonDark1)   {m_ItemPacket.wEvilAttr[0] = wDemonDark1;}
inline void CGood::SetDemondark2(WORD wDemonDark2)   {m_ItemPacket.wEvilAttr[1] = wDemonDark2;}
inline void CGood::SetDemondark3(WORD wDemonDark3)   {m_ItemPacket.wEvilAttr[2] = wDemonDark3;}
inline void CGood::SetResvEx(int i , BYTE& c)       {m_ItemPacket.szReserved[i] = c;}
inline void CGood::SetPayType(BYTE pay){ m_nPayType = pay; }

//1.907商城道具 
inline void	CGood::SetExternString(string text){ m_ExternString = text ;}

inline void  CGood::SetPos(int pos){ m_iPos = pos; }

inline DWORD	CGood::GetID()			{ return m_ItemPacket.dwUniqueID; }
inline const char * CGood::GetName(){	return m_ItemPacket.szName; 	}
inline int		CGood::GetStdMode()	{ return m_ItemPacket.byStdMode; }
inline BYTE CGood::GetShape()			{ return m_ItemPacket.byShape;	}
inline BYTE		CGood::GetWeight()		{ return m_ItemPacket.byWeight;	}
inline WORD	CGood::GetDura()		{ return m_ItemPacket.wItemDura;	}
inline WORD	CGood::GetDuraMax()	{ return m_ItemPacket.wMaxItemDura;}
inline WORD	CGood::GetOriDuraMax()	{ return m_ItemPacket.wDuraMax;}
inline BYTE		CGood::GetAC()		{ return m_ItemPacket.byAC1;		}
inline BYTE		CGood::GetAC2()		{ return m_ItemPacket.byAC2;	}
inline BYTE		CGood::GetMAC()		{ return m_ItemPacket.byMAC1;	}
inline BYTE		CGood::GetMAC2()	{ return m_ItemPacket.byMAC2;	}
inline BYTE		CGood::GetDC()		{ return m_ItemPacket.byDC1;		}
inline BYTE		CGood::GetDC2()		{ return m_ItemPacket.byDC2;	}
inline BYTE		CGood::GetMC()		{ return m_ItemPacket.byMC1;		}
inline BYTE		CGood::GetMC2()		{ return m_ItemPacket.byMC2;	}
inline BYTE		CGood::GetSC()		{ return m_ItemPacket.bySC1;		}
inline BYTE		CGood::GetSC2()		{ return m_ItemPacket.bySC2;	}
inline BYTE  	CGood::GetNeed()		{ return m_ItemPacket.byNeed;	}
inline BYTE 	CGood::GetNeedLevel()	{ return m_ItemPacket.byNeedLevel; }
inline DWORD	CGood::GetPrice()		{ return m_ItemPacket.dwPrice;	}
inline DWORD	CGood::GetFlag()		{ /*return m_ItemPacket.dwFlag;*/ return 0;	}
inline WORD		CGood::GetFlag1()		{ /*return m_ItemPacket.wFlags1;*/ return 0;	}
inline int		CGood::GetFlag2()		{ return m_nFlag2;	} 
inline BYTE     CGood::GetPayType(){ return m_nPayType; } //支付方式金币还是元宝
inline BYTE     CGood::GetAttachSkill() { return m_ItemPacket.szReserved[8];     }
inline BYTE     CGood::GetAttachSkillLevel()   { return m_ItemPacket.szReserved[9];}

inline string	CGood::GetExternString(){return m_ExternString ;}

//心魔增加的物品属性
inline WORD  CGood::GetDemonDark1() { return   m_ItemPacket.wEvilAttr[0];}  //黑暗属性1
inline WORD  CGood::GetDemonDark2() { return   m_ItemPacket.wEvilAttr[1];}  //黑暗属性2
inline WORD  CGood::GetDemonDark3() { return   m_ItemPacket.wEvilAttr[2];}  //黑暗属性3

inline BYTE&  CGood::GetResvEx(int i){return m_ItemPacket.szReserved[i];}
inline BYTE*  CGood::GetResvExBuf(){return m_ItemPacket.szReserved;}

inline BYTE&  CGood::GetResvEx3(int i){return m_ItemPacket.szExtproperty3[i];}
inline BYTE*  CGood::GetResvEx3Buf(){return m_ItemPacket.szExtproperty3;}

inline DWORD CGood::GetItemState(){  return m_dwState;	}
inline void  CGood::AddItemState(GoodState dwState){ m_dwState |= dwState; }
inline void  CGood::RemoveItemState(GoodState dwState){ m_dwState &= ~dwState; }

inline int   CGood::GetPos(){ return m_iPos; }
inline GoodAddEffect& CGood::GetGoodAddEffect(){return m_AddEffect;}

inline void  CGood::SetFromWnd(int iWnd){m_iFromWnd = iWnd;}
inline int   CGood::GetFromWnd(){return m_iFromWnd;}
inline void  CGood::SetToWnd(int iWnd){m_iToWnd = iWnd;}
inline int   CGood::GetToWnd(){return m_iToWnd;}
inline BYTE CGood::GetLevelUpTimes(){ /*return (BYTE((m_ItemPacket.dwFlag & 0x00FF0000) >> 16) & 0x0F);*/ return 0x0F; }//m_lflag第三个字节的低四位
inline BYTE CGood::GetStarCount(){ /*return BYTE((m_ItemPacket.dwFlag & 0x00FF0000) >> 20);*/ return 0x0F; }//m_lflag第三个字节的高四位

inline bool CGood::IsUpgrading(){return (m_ItemPacket.wEvilAttr[0] & 0x8000) != 0;}//最高一个bit表示是否正在升级中
inline DWORD CGood::GetFlashTexID(){return m_dwFlashTexID;}
inline void CGood::SetFlashTexID(DWORD dwID){m_dwFlashTexID = dwID;}
inline DWORD CGood::GetArrowTipWnd() { return m_dwArrowTipWnd; }
inline void CGood::SetArrowTipWnd(DWORD val) { m_dwArrowTipWnd = val; }
inline int CGood::GetArrowTipMsgID() { return m_iArrowTipMsgID; }
inline void CGood::SetArrowTipMsgID(int val) { m_iArrowTipMsgID = val; }
inline BYTE     CGood::GetSoulLevel() { return m_ItemPacket.byWakeUpLevel; }
inline void     CGood::SetSoulLevel(BYTE val) { m_ItemPacket.byWakeUpLevel = val; }
inline DWORD    CGood::GetSoleExp() { return m_ItemPacket.dwTotalExp; }
inline void     CGood::SetSoleExp(DWORD val) { m_ItemPacket.dwTotalExp = val; }
inline bool     CGood::HaveWakeUpSole(){return (m_ItemPacket.byWakeUpLevel > 0 || m_ItemPacket.dwTotalExp > 0) && !(GetStdMode() == 104 && GetShape() != 1);}//是否魂器觉醒

inline BYTE    CGood::GetHolyWingAttrType(int i){return (PSHolyWing(&m_ItemPacket))->byHolyWingAttrType[i];}
inline WORD    CGood::GetHolyWingAttrValue(int i){	return (PSHolyWing(&m_ItemPacket))->wHolyWingAttrValue[i];}
inline WORD    CGood::GetHolyWingAttrExp(int i)
{
	if (m_ItemPacket.dwUniqueID == 0)
	{
		return 0;
	}
	return (PSHolyWing(&m_ItemPacket))->wHolyWingPartExp[i];
}
inline int    CGood::GetHolyWingLevel()
{
	if (m_ItemPacket.dwUniqueID == 0)
	{
		return -1;
	}
	return (PSHolyWing(&m_ItemPacket))->byHolyWingLevel;
}
inline WORD    CGood::GetHolyWingLevelUpNeedLevel(){return (PSHolyWing(&m_ItemPacket))->wHolyWingLevelUpNeedLevel;}
inline int     CGood::GetHolyWingStrongLevel()
{
	if (m_ItemPacket.dwUniqueID == 0)
	{
		return -1;
	}

	return (PSHolyWing(&m_ItemPacket))->byHolyWingStrongLevel;
}
inline DWORD   CGood::GetHolyWingPartLevelUpExpMax(){ return (PSHolyWing(&m_ItemPacket))->dwHolyWingPartLevelUpExpMax; }
inline WORD    CGood::GetHolyWingAddHp(){return (PSHolyWing(&m_ItemPacket))->wHolyWingAddHp;}
inline WORD    CGood::GetHolyWingAddMp(){return (PSHolyWing(&m_ItemPacket))->wHolyWingAddMp;}
inline WORD    CGood::GetHolyWingAttack(){return (PSHolyWing(&m_ItemPacket))->wHolyWingAttack;}
inline WORD    CGood::GetHolyWingDefend(){return (PSHolyWing(&m_ItemPacket))->wHolyWingDefend;}
inline WORD    CGood::GetHolyWingMagicHit(){return (PSHolyWing(&m_ItemPacket))->wHolyWingMagicHit;}
inline WORD    CGood::GetHolyWingCruelAttack(){return (PSHolyWing(&m_ItemPacket))->wHolyWingCruelAttack;}
inline WORD    CGood::GetHolyWingReboundHurt(){return (PSHolyWing(&m_ItemPacket))->wHolyWingReboundHurt;}
inline WORD    CGood::GetHolyWingAbsordBlood(){return (PSHolyWing(&m_ItemPacket))->wHolyWingAbsordBlood;}
inline WORD    CGood::GetHolyWingMagicDefend(){return (PSHolyWing(&m_ItemPacket))->wHolyWingMagicDefend;}
inline WORD    CGood::GetHolyWingAttackAvoid(){return (PSHolyWing(&m_ItemPacket))->wHolyWingAttackAvoid;}
inline WORD	   CGood::GetHolyWingPoFang(){return (PSHolyWing(&m_ItemPacket))->wHolyWingPoFang;}
inline WORD    CGood::GetHolyWingDiKang(){return (PSHolyWing(&m_ItemPacket))->wHolyWingDiKang;}
inline WORD    CGood::GetHolyWingMagicAvoid(){return (PSHolyWing(&m_ItemPacket))->wHolyWingMagicAvoid;}

inline BYTE    CGood::GetBlessAC()		{ return m_ItemPacket.byDC1; }
inline BYTE    CGood::GetBlessMC()		{ return m_ItemPacket.byDC2; }
inline BYTE    CGood::GetBlessDC()		{ return m_ItemPacket.byMC1; }
inline BYTE    CGood::GetBlessAddHP()	{ return m_ItemPacket.byMC2; }
inline BYTE    CGood::GetBlessDefend()	{ return m_ItemPacket.bySC1; }
inline BYTE    CGood::GetBlessMDefend()	{ return m_ItemPacket.szReserved[0]; }
inline BYTE    CGood::GetBlessAttackAvoid()	{ return m_ItemPacket.szReserved[1]; }
inline BYTE    CGood::GetBlessAttackHit()	{ return m_ItemPacket.szReserved[2]; }
inline BYTE    CGood::GetBlessMagicHit()	{ return m_ItemPacket.szReserved[3]; }
inline BYTE    CGood::GetBlessMagicAvoid()	{ return m_ItemPacket.szReserved[4]; }

inline int	   CGood::GetSubWenPeiType() { return m_ItemPacket.wLooks; }
inline int   CGood::GetWenPeiSubCount()  { return 4; }
inline int	 CGood::GetWenPeiExtAttr(int i) 
{
	if (i >= 0 && i <= 9)
	{
		return *((BYTE*)((&m_ItemPacket.byAC1) + i));
	}
	else if (i >= 10 && i <= 12)
	{
		return m_ItemPacket.szReserved[i-10];
	}
	return 0;
}

inline int CGood::GetWenPeiGuDian()
{
	return m_ItemPacket.wEvilAttr[0];
}

inline WORD CGood::GetAddHP()
{
	return *((WORD*)(&m_ItemPacket.szReserved[5]));
}

inline int CGood::GetChiLianBaoji()
{
	return m_ItemPacket.szExtproperty3[9];
}

inline int CGood::GetChiLianFanTan()
{
	return m_ItemPacket.szExtproperty3[10];
}
inline int CGood::GetChiLianXiXie()
{
	return m_ItemPacket.szExtproperty3[11];
}
inline int CGood::GetChiLianJueFang()
{
	return m_ItemPacket.szExtproperty3[12];
}
inline int CGood::GetChiLianPoFang()
{
	return m_ItemPacket.szExtproperty3[13];
}
