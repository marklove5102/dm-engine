#pragma once

#include "GraphicD3D.h"
#include <vector>
#include "Global/Interface/LightInterface.h"

#define MAX_LIGHTS	256

class CLight : public CLightInterface
{
	struct stLightMap
	{
		stLightMap()
		{
			iX = 0;
			iY = 0;
			dwID = 0;
			fScaleX = 1.0f;
			fScaleY = 1.0f;
			dwColor = 0xFFFFFFFF;
			fRotZ = 0.0f;
			eMode = RM_ADD2;
			bCenter = false;
		}

		int		iX;
		int		iY;
		DWORD	dwID;
		float	fScaleX;
		float	fScaleY;
		DWORD	dwColor;
		float   fRotZ;
		RenderMode eMode;
		bool    bCenter;
	};

private:
	friend class CGraphicD3D;
	BOOL		m_bEnable;
	DWORD		m_dwSetColor;
	A8R8G8B8	m_dwLightColor;

	float		m_fScaleX;
	float		m_fScaleY;
	int			m_iLightCount;
	stLightMap	m_vLightMap[MAX_LIGHTS];

public:
	CLight(void);
	~CLight(void);

	void	SetEnable(BOOL bEnable);
	BOOL	IsEnable(void);
	void	SetLightColor(DWORD dwColor);
	DWORD	GetLightColor(void);

	void	CreateLight(void);
	void	DrawLight(int iX,int iY,DWORD dwID,float fScaleX,float fScaleY,DWORD dwColor);
	void    DrawBLight(int iX,int iY,DWORD dwID,float fScaleX,float fScaleY,DWORD dwColor,float fRotZ);

	void	RenderLight(void);
};

extern CLight* g_pELight;