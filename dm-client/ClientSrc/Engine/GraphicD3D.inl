
inline DWORD& CGraphicD3D::GetAlloacMem()
{
	return m_dwAlloacMem;
}

inline DWORD& CGraphicD3D::GetShouldMem()
{
	return m_dwShouldMem;
}

inline DWORD CGraphicD3D::GetFPS(void)
{
	return m_dwRealFPS;
}

inline DWORD CGraphicD3D::GetTotalFPS(void)
{
	return m_dwTotalRealFps;
}

inline DWORD CGraphicD3D::GetFrameCount(void)
{
	return m_dwFrameCount;	
}

inline void CGraphicD3D::SetFrameCount(DWORD dwCount)
{
	m_dwFrameCount = dwCount;	
}

inline DWORD CGraphicD3D::_GetTimeCount(void)
{
	return m_iNextTime;
}

inline DisplayMode CGraphicD3D::GetDisMode(void)
{
	return m_eDisMode;
}



inline void CGraphicD3D::SetWidth(int i)
{
	m_iWidth = i;
}

inline void CGraphicD3D::SetHeight(int i)
{
	m_iHeight = i;
}

inline int CGraphicD3D::GetWidth(void)
{
	return m_iWidth;
}

inline int CGraphicD3D::GetHeight(void)
{
	return m_iHeight;
}

inline void CGraphicD3D::GetDisSize(int &x,int &y)
{
	x = m_iWidth;
	y = m_iHeight;
}

inline BOOL CGraphicD3D::IsBig()
{
	return m_iWidth > 800;
}