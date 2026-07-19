#pragma once

class CLightInterface
{
public:	
	virtual ~CLightInterface(){}
	virtual void	SetEnable(BOOL bEnable) = 0;
	virtual BOOL	IsEnable(void) = 0;
	virtual void	SetLightColor(DWORD dwColor) = 0;
	virtual DWORD	GetLightColor(void) = 0;

	virtual void	CreateLight(void) = 0;
	virtual void	DrawLight(int iX,int iY,DWORD dwID,float fScaleX,float fScaleY,DWORD dwColor) = 0;
	virtual void    DrawBLight(int iX,int iY,DWORD dwID,float fScaleX,float fScaleY,DWORD dwColor,float fRotZ) = 0;

	virtual void	RenderLight(void) = 0;
};

extern CLightInterface *     g_pLight;
