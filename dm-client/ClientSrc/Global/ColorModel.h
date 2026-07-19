#pragma once

#include "Global/Globaldefine.h"

class CColorModel
{
public:
	CColorModel();
	~CColorModel(void);
	DWORD	RGB2DWORD();
	void	DWORD2RGB(DWORD dwColor);
	void	RGB2HSV();
	void	HSV2RGB();
	void	RGBFromColor16(WORD wColor);
	WORD	RGBToColor16();
	void	OffsetHue(float fOff);
	void	OffsetSat(float fOff);
	void	OffsetBri(float	fOff);
	WORD	OffsetColor16(WORD wColor, float fOffHue, float fOffSat = 0, float fOffBri = 0);
protected:
	float	m_fH;	//[0,	360]
	float	m_fS;	//[0,	1]
	float	m_fV;	//[0,	1]
	UCHAR	m_cR;	//[0,	255]
	UCHAR	m_cG;	//[0,	255]
	UCHAR	m_cB;	//[0,	255]
	UCHAR	m_cA;	//[0,	255]

	WORD	m_wReservedColor;
	BOOL	IsLockColor(WORD wColor);
};

DWORD	ColorMultiply(DWORD dwColor1,DWORD dwColor2);	// 混合两种颜色

