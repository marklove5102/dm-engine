#include "ColorModel.h"
#include <math.h>


CColorModel::CColorModel()
{
	m_fH	= 0.0f;
	m_fS	= 0.0f;
	m_fV	= 0.0f;
	m_cR	= 0;
	m_cG	= 0;
	m_cB	= 0;
	m_cA	= 0;

	m_wReservedColor	= 0;
	DWORD	dwReserve = 0xFF656662;
	for(int i = 0, j = 0, k = 0; i < 4; ++i, j += 8, k+=4)
	{
		BYTE byChannel = (BYTE)((dwReserve & (0xFF<<j))>>j);
		WORD wChannel = (byChannel>>4)<<k;
        m_wReservedColor |= wChannel;
	}
}

CColorModel::~CColorModel()
{
}

void CColorModel::RGBFromColor16(WORD wColor)
{
	m_cA	= (wColor & 0xF000) >> 8;
	m_cR	= (wColor & 0x0F00) >> 4;
	m_cG	= (wColor & 0x00F0);
	m_cB	= (wColor & 0x000F) << 4;
}

WORD CColorModel::RGBToColor16()
{
	WORD	wResult = 0;
	wResult	|= m_cR >> 4;
	wResult	|= m_cA & 0xF0;
	wResult	= wResult << 8;
	wResult	|= m_cB >> 4;
	wResult	|= (m_cG & 0xF0);
	return wResult;
}

void CColorModel::DWORD2RGB(DWORD dwColor)
{
	m_cA	= (UCHAR)((dwColor>>24) & 0x000000FF);
	m_cR	= (UCHAR)((dwColor>>16) & 0x000000FF);
	m_cG	= (UCHAR)((dwColor>>8)	& 0x000000FF);
	m_cB	= (UCHAR)(	dwColor		& 0x000000FF);
}

DWORD CColorModel::RGB2DWORD()
{
	DWORD dwResult = 0;
	dwResult |= m_cB;
	dwResult |= m_cG << 8;
	dwResult |= m_cR << 16;
	dwResult |= m_cA << 24;
	return dwResult;
}

void CColorModel::RGB2HSV()
{
	UCHAR	cMax = max(max(m_cB, m_cG),m_cR);
	UCHAR	cMin = min(min(m_cB, m_cG),m_cR);
	UCHAR	cDelta	= cMax - cMin;
	if(cMax == 0)
	{
		m_fH	= 0.0f;
		m_fS	= 0.0f;
		m_fV	= 0.0f;
		return;
	}
	m_fV	= (float)cMax/255;
	m_fS	= (float)cDelta/cMax;
	if(cDelta == 0)
	{
		m_fH = 0;
		return;
	}
	if(cMax == m_cR)
	{
		m_fH = 60.0f *(m_cG - m_cB) / cDelta;
	}
	else if(cMax == m_cG)
	{
		m_fH = 120.0f + 60.0f *(m_cB - m_cR) / cDelta;
	}
	else
	{
		m_fH = 240.0f + 60.0f *(m_cR - m_cG) / cDelta;
	}
	if(m_fH < 0)
	{
		m_fH += 360.0f;
	}
}

void CColorModel::HSV2RGB()
{
	while(m_fH< .0f)
	{m_fH+=360;}
	while(m_fH>= 360.0f)
	{m_fH-=360;}

	float	fTemp = m_fH / 60;
	int		iTemp = (int)fTemp;
	fTemp -= iTemp;

	float fTP = m_fV * ( 1 - m_fS);
	float fTQ = m_fV * ( 1 - m_fS*fTemp);
	float fTT = m_fV * ( 1 - m_fS * ( 1- fTemp));

	float fR = 0.0f, fG = 0.0f, fB = 0.0f;

	switch(iTemp)
	{
	case 0: fR = m_fV;	fG = fTT;	fB	= fTP;	break;
	case 1: fR = fTQ;	fG = m_fV;	fB	= fTP;	break;
	case 2: fR = fTP;	fG = m_fV;	fB	= fTT;	break;
	case 3: fR = fTP;	fG = fTQ;	fB	= m_fV;	break;
	case 4: fR = fTT;	fG = fTP;	fB	= m_fV;	break;
	case 5: fR = m_fV;	fG = fTP;	fB	= fTQ;	break;
	default:
		break;
	}
	m_cR = (UCHAR)(255 * fR);
	m_cG = (UCHAR)(255 * fG);
	m_cB = (UCHAR)(255 * fB);
}

void CColorModel::OffsetHue(float fOff)
{
	m_fH += fOff;
	while(m_fH< .0f)
	{m_fH+=360;}
	while(m_fH>= 360.0f)
	{m_fH-=360;}
}

void CColorModel::OffsetSat(float fOff)
{
	m_fS += fOff;
	m_fS = max(0, min(m_fS, 1.0f));
}

void CColorModel::OffsetBri(float fOff)
{
	//gamma adjust
	////////////////
	//double dinvgamma = 1/gamma;
	//double dMax = pow(255.0, dinvgamma) / 255.0;
	//BYTE cTable[256]; //<nipper>
	//for (int i=0;i<256;i++)	{
	//	cTable[i] = (BYTE)max(0,min(255,(int)( pow((double)i, dinvgamma) / dMax)));
	//}
	////////////////
	if(fOff > -1e-6f && fOff < 1e-6f) return;
	fOff += 1.0f;
	float fInv = 1 / fOff;
	float fAdjust = powf(m_fV, fInv);
	m_fV = max(0, min(fAdjust, 1.0f));

	//linear adjust
	//m_fV += fOff;
	//m_fV = max(0, min(m_fV, 1.0f));
}

 WORD CColorModel::OffsetColor16(WORD wColor, float fOffHue, float fOffSat, float fOffBri)
{
	if(IsLockColor(wColor))
	{
		return wColor;
	}

	RGBFromColor16(wColor);
	RGB2HSV();

	OffsetHue(fOffHue);
	OffsetSat(fOffSat);
	OffsetBri(fOffBri);

	HSV2RGB();
	return RGBToColor16();
}


BOOL CColorModel::IsLockColor(WORD wColor)
{
	BYTE byLast = wColor & 0x000F;
	for(int i = 1, j = 4; i < 3; ++i, j += 4)
	{
		BYTE byChannel = (BYTE)((wColor & (0x0F<<j))>>j);
		char cDelta = (char)byChannel - byLast;

		if(cDelta != 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}


DWORD ColorMultiply(DWORD dwColor1,DWORD dwColor2)
{
	const __int64 i64 = 0x0001000100010001;
	_asm
	{
		movd		mm1, dword ptr [dwColor1] 
		movd		mm2, dword ptr [dwColor2] 
		pxor		mm0, mm0
			punpcklbw	mm1, mm0
			punpcklbw	mm2, mm0
			paddw		mm2, i64
			pmullw		mm1, mm2
			psrlw		mm1, 8
			packuswb	mm1, mm0
			movd		eax, mm1
			emms
	}
}