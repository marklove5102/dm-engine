
inline DWORD CTextureD3D::GetOption(void)
{
	return m_dwOption;
}

inline void	CTextureD3D::SetOption(DWORD dwOption)
{
	m_dwOption = dwOption;
}

inline DWORD CTextureD3D::GetFrames(void)
{
	return m_dwFrames;
}

inline DWORD CTextureD3D::GetSpeed(void)
{
	return m_dwSpeed;
}

inline void CTextureD3D::SetSpeed(DWORD dwSpeed)
{
	m_dwSpeed = dwSpeed;
}

inline DWORD CTextureD3D::GetID(void)
{ 
	return m_dwID;
}

inline void CTextureD3D::SetID(DWORD dwID)
{ 
	m_dwID = dwID;
}

inline DWORD CTextureD3D::GetFrameNow(void)
{
	return m_dwFrameNow;
}

inline BOOL CTextureD3D::IsOption(eOptionFlag eOF)
{
	return (m_dwOption & eOF) ? TRUE : FALSE;
}

inline void CTextureD3D::EnableSysAnim(BOOL b)
{
	m_bSysAnim = b;
}

inline int CTextureD3D::GetWidth0(void)
{
	return m_pCurFrame ? m_pCurFrame->wWidth0 : 0;
}

inline int CTextureD3D::GetHeight0(void)
{ 
	return m_pCurFrame ? m_pCurFrame->wHeight0 : 0;
}

inline int CTextureD3D::GetWidth(void)
{
	return m_pCurFrame ? ((m_pCurFrame->wWidth > m_pCurFrame->wWidth0) ? m_pCurFrame->wWidth0 : m_pCurFrame->wWidth): 0;
}

inline int CTextureD3D::GetHeight(void)
{ 
	return m_pCurFrame ? ((m_pCurFrame->wHeight > m_pCurFrame->wHeight0) ? m_pCurFrame->wHeight0 : m_pCurFrame->wHeight): 0;
}

inline int CTextureD3D::GetOffX(void)
{
	return m_pCurFrame ? m_pCurFrame->wOffX : 0;
}

inline int CTextureD3D::GetOffY(void)
{
	return m_pCurFrame ? m_pCurFrame->wOffY : 0;
}

inline int CTextureD3D::GetCenterX(void)
{
	return m_pCurFrame ? m_pCurFrame->wCenterX : 0;
}

inline int CTextureD3D::GetCenterY(void)
{
	return m_pCurFrame ? m_pCurFrame->wCenterY : 0;
}

inline void CTextureD3D::SetOff(int iOffX,int iOffY)
{
	if(m_pCurFrame)
	{
		m_pCurFrame->wOffX = iOffX;
		m_pCurFrame->wOffY = iOffY;
	}
}

inline void	CTextureD3D::SetCenter(int iCenterX,int iCenterY)
{
	if(m_pCurFrame)
	{
		m_pCurFrame->wCenterX = iCenterX;
		m_pCurFrame->wCenterY = iCenterY;
	}
}

inline CTexFrame* CTextureD3D::GetCurFrame()
{
	return m_pCurFrame;
}

inline VTexFrame& CTextureD3D::GetVFrame()
{
	return m_vFrame;
}

inline void CTextureD3D::ClearFrame()
{
	m_vFrame.clear();
}

inline void	CTextureD3D::SetPixelSize(DWORD dwSize)
{
	m_dwPixelSize = dwSize;
}

inline DWORD CTextureD3D::GetPixelSize()
{
	return m_dwPixelSize;
}
