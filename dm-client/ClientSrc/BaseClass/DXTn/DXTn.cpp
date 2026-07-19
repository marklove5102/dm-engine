
#include <windows.h>
#include "DXTn.h"
#include "Global/include/ATI_Compress.h"

#ifdef _DEBUG
#pragma comment(lib, "ATI_Compress_MTD_VC8.lib")
#else
#pragma comment(lib, "ATI_Compress_MT_VC8.lib")
#endif

// 纹理格式类型
enum TexPixelType
{
	TPT_A8R8G8B8 = 0,
	TPT_A4R4G4B4	,
	TPT_A0R5G6B5	,
	TPT_A8L8		,
	TPT_DXT1		,
	TPT_DXT3		,
	TPT_DXT5		,

	TPT_TYPES		,
};

// A8R8G8B8格式
union A8R8G8B8
{
	struct
	{
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE a;
	};
	DWORD	col;
};

// A4R4G4B4格式
union A4R4G4B4
{
	struct
	{
		WORD b:4;
		WORD g:4;
		WORD r:4;
		WORD a:4;
	};
	WORD	col;
};

// A0R5G6B5格式
union A0R5G6B5
{
	struct
	{
		WORD b:5;
		WORD g:6;
		WORD r:5;
	};
	WORD	col;
};

// DXTn的颜色索引
struct DXTn_IDX
{
	BYTE idx0:2;
	BYTE idx1:2;
	BYTE idx2:2;
	BYTE idx3:2;
};

// DXT1的内存单元格式
struct DXT1_ELE
{
	A0R5G6B5	RGB0;
	A0R5G6B5	RGB1;
	DXTn_IDX	Idx[4];
};

// DXT3的alpha信息
struct ALPHA_DXT3
{
	WORD alpha0:4;
	WORD alpha1:4;
	WORD alpha2:4;
	WORD alpha3:4;
};

// DXT3的内存单元格式
struct DXT3_ELE
{
	ALPHA_DXT3	Alpha[4];
	A0R5G6B5	RGB0;
	A0R5G6B5	RGB1;
	DXTn_IDX	Idx[4];
};

#define		MAX_BLOCK_SIZE		(256 * 256 * 4)

// 颜色索引
A8R8G8B8	RGB32[4];
A4R4G4B4	RGB16[4];
BYTE		buf0[MAX_BLOCK_SIZE];
BYTE		buf1[MAX_BLOCK_SIZE];
BYTE        buf2[MAX_BLOCK_SIZE];

//-------------------------------------------------------------------------------------------------------------------
//目标A4R4G4B4
void IN2_BLOCK_DXT1_A4R4G4B4(A4R4G4B4 *pDst,int pitchs,DXT1_ELE *pSrc)
{
	RGB16[0].a = RGB16[1].a = RGB16[2].a = RGB16[3].a = 0x0F;

	RGB16[0].r = (pSrc->RGB0.r)>>1;
	RGB16[0].g = (pSrc->RGB0.g)>>2;
	RGB16[0].b = (pSrc->RGB0.b)>>1;

	RGB16[1].r = (pSrc->RGB1.r)>>1;
	RGB16[1].g = (pSrc->RGB1.g)>>2;
	RGB16[1].b = (pSrc->RGB1.b)>>1;

	if(pSrc->RGB0.col > pSrc->RGB1.col)	// 没alpha
	{
		RGB16[2].r = ((2 * pSrc->RGB0.r + pSrc->RGB1.r) / 3)>>1;
		RGB16[2].g = ((2 * pSrc->RGB0.g + pSrc->RGB1.g) / 3)>>2;
		RGB16[2].b = ((2 * pSrc->RGB0.b + pSrc->RGB1.b) / 3)>>1;

		RGB16[3].r = ((2 * pSrc->RGB1.r + pSrc->RGB0.r) / 3)>>1;
		RGB16[3].g = ((2 * pSrc->RGB1.g + pSrc->RGB0.g) / 3)>>2;
		RGB16[3].b = ((2 * pSrc->RGB1.b + pSrc->RGB0.b) / 3)>>1;
	}
	else								// 带有1bit的alpha
	{
		RGB16[2].r = ((pSrc->RGB0.r + pSrc->RGB1.r) / 2)>>1;
		RGB16[2].g = ((pSrc->RGB0.g + pSrc->RGB1.g) / 2)>>2;
		RGB16[2].b = ((pSrc->RGB0.b + pSrc->RGB1.b) / 2)>>1;

		RGB16[3].a = RGB16[3].r = RGB16[3].g = RGB16[3].b = 0;
	}

	for(int i = 0; i < 4; i++)
	{
		pDst[0]	= RGB16[pSrc->Idx[i].idx0];
		pDst[1]	= RGB16[pSrc->Idx[i].idx1];
		pDst[2]	= RGB16[pSrc->Idx[i].idx2];
		pDst[3]	= RGB16[pSrc->Idx[i].idx3];

		pDst = (A4R4G4B4 *)(((BYTE *)pDst) + pitchs);
	}
}

void IN2_BLOCK_DXT3_A4R4G4B4(A4R4G4B4 *pDst,int pitchs,DXT3_ELE *pSrc)
{
	RGB16[0].a = RGB16[1].a = RGB16[2].a = RGB16[3].a = 0;

	RGB16[0].r = (pSrc->RGB0.r)>>1;
	RGB16[0].g = (pSrc->RGB0.g)>>2;
	RGB16[0].b = (pSrc->RGB0.b)>>1;

	RGB16[1].r = (pSrc->RGB1.r)>>1;
	RGB16[1].g = (pSrc->RGB1.g)>>2;
	RGB16[1].b = (pSrc->RGB1.b)>>1;

	RGB16[2].r = ((2 * pSrc->RGB0.r + pSrc->RGB1.r) / 3)>>1;
	RGB16[2].g = ((2 * pSrc->RGB0.g + pSrc->RGB1.g) / 3)>>2;
	RGB16[2].b = ((2 * pSrc->RGB0.b + pSrc->RGB1.b) / 3)>>1;

	RGB16[3].r = ((2 * pSrc->RGB1.r + pSrc->RGB0.r) / 3)>>1;
	RGB16[3].g = ((2 * pSrc->RGB1.g + pSrc->RGB0.g) / 3)>>2;
	RGB16[3].b = ((2 * pSrc->RGB1.b + pSrc->RGB0.b) / 3)>>1;

	for(int i = 0; i < 4; i++)
	{
		pDst[0]	= RGB16[pSrc->Idx[i].idx0];
		pDst[1]	= RGB16[pSrc->Idx[i].idx1];
		pDst[2]	= RGB16[pSrc->Idx[i].idx2];
		pDst[3]	= RGB16[pSrc->Idx[i].idx3];

		pDst[0].a = pSrc->Alpha[i].alpha0;
		pDst[1].a = pSrc->Alpha[i].alpha1;
		pDst[2].a = pSrc->Alpha[i].alpha2;
		pDst[3].a = pSrc->Alpha[i].alpha3;

		pDst = (A4R4G4B4 *)(((BYTE *)pDst) + pitchs);
	}
}

BOOL IN1_DXT1_A4R4G4B4(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	A4R4G4B4*	dst		= NULL;
	DXT1_ELE*	src		= (DXT1_ELE*)pSrc;
	for(int j = 0; j < h1; j++)
	{
		if(j < h0 && (j % 4) == 0)
		{
			for(int i = 0; i < w0; i += 4)
			{
				dst	= (A4R4G4B4*)(pDst + j * pitchs1 + i * sizeof(A4R4G4B4));
				IN2_BLOCK_DXT1_A4R4G4B4(dst,pitchs1,src);
				src++;
			}
			if(pitchs1 > (int)(w0 * sizeof(A4R4G4B4)))
			{
				for(int i = 0;i < 4;i++)
					memset(pDst + (j + i) * pitchs1 + w0 * sizeof(A4R4G4B4),0,pitchs1 - w0 * sizeof(A4R4G4B4));
			}
		}
		else if(j >= h0)
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

BOOL IN1_DXT3_A4R4G4B4(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	A4R4G4B4*	dst		= NULL;
	DXT3_ELE*	src		= (DXT3_ELE*)pSrc;
	for(int j = 0; j < h1; j++)
	{
		if(j < h0 && (j % 4) == 0)
		{
			for(int i = 0; i < w0; i += 4)
			{
				dst	= (A4R4G4B4*)(pDst + j * pitchs1 + i * sizeof(A4R4G4B4));
				IN2_BLOCK_DXT3_A4R4G4B4(dst,pitchs1,src);
				src++;
			}
			if(pitchs1 > (int)(w0 * sizeof(A4R4G4B4)))
			{
				for(int i = 0;i < 4;i++)
					memset(pDst + (j + i) * pitchs1 + w0 * sizeof(A4R4G4B4),0,pitchs1 - w0 * sizeof(A4R4G4B4));
			}
		}
		else if(j >= h0)
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

BOOL IN1_A4R4G4B4_A4R4G4B4(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int pitchs0 = w0 * sizeof(A4R4G4B4);
	for(int j = 0; j < h1;j++)
	{
		if(j < h0)
		{
			memcpy(pDst + j * pitchs1,pSrc + j * pitchs0,pitchs0);
			if(pitchs1 > pitchs0)
				memset(pDst + j * pitchs1 + pitchs0,0,pitchs1 - pitchs0);
		}
		else
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

BOOL IN1_A8R8G8B8_A4R4G4B4(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int pitchs0		= w0 * sizeof(A8R8G8B8);
	int pitchs0_	= w0 * sizeof(A4R4G4B4);
	for(int j = 0; j < h1;j++)
	{
		if(j < h0)
		{
			A8R8G8B8 *src = (A8R8G8B8 *)(pSrc + j * pitchs0);
			A4R4G4B4 *dst = (A4R4G4B4 *)(pDst + j * pitchs1);
			for(int i = 0;i < w0;i++)
			{
				dst->a = src->a>>4;
				dst->r = src->r>>4;
				dst->g = src->g>>4;
				dst->b = src->b>>4;
				src++;
				dst++;
			}
			if(pitchs1 > pitchs0_)
				memset(pDst + j * pitchs1 + pitchs0_,0,pitchs1 - pitchs0_);
		}
		else
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
//目标A8R8G8B8
void IN2_BLOCK_DXT1_A8R8G8B8(A8R8G8B8 *pDst,int pitchs,DXT1_ELE *pSrc)
{
	RGB32[0].a = RGB32[1].a = RGB32[2].a = RGB32[3].a = 0xFF;

	RGB32[0].r = (pSrc->RGB0.r)<<3;
	RGB32[0].g = (pSrc->RGB0.g)<<2;
	RGB32[0].b = (pSrc->RGB0.b)<<3;

	RGB32[1].r = (pSrc->RGB1.r)<<3;
	RGB32[1].g = (pSrc->RGB1.g)<<2;
	RGB32[1].b = (pSrc->RGB1.b)<<3;

	if(pSrc->RGB0.col > pSrc->RGB1.col)	// 没alpha
	{
		RGB32[2].r = ((2 * pSrc->RGB0.r + pSrc->RGB1.r) / 3)<<3;
		RGB32[2].g = ((2 * pSrc->RGB0.g + pSrc->RGB1.g) / 3)<<2;
		RGB32[2].b = ((2 * pSrc->RGB0.b + pSrc->RGB1.b) / 3)<<3;

		RGB32[3].r = ((2 * pSrc->RGB1.r + pSrc->RGB0.r) / 3)<<3;
		RGB32[3].g = ((2 * pSrc->RGB1.g + pSrc->RGB0.g) / 3)<<2;
		RGB32[3].b = ((2 * pSrc->RGB1.b + pSrc->RGB0.b) / 3)<<3;
	}
	else								// 带有1bit的alpha
	{
		RGB32[2].r = ((pSrc->RGB0.r + pSrc->RGB1.r) / 2)<<3;
		RGB32[2].g = ((pSrc->RGB0.g + pSrc->RGB1.g) / 2)<<2;
		RGB32[2].b = ((pSrc->RGB0.b + pSrc->RGB1.b) / 2)<<3;

		RGB32[3].a = RGB32[3].r = RGB32[3].g = RGB32[3].b = 0;
	}

	for(int i = 0; i < 4; i++)
	{
		pDst[0]	= RGB32[pSrc->Idx[i].idx0];
		pDst[1]	= RGB32[pSrc->Idx[i].idx1];
		pDst[2]	= RGB32[pSrc->Idx[i].idx2];
		pDst[3]	= RGB32[pSrc->Idx[i].idx3];

		pDst = (A8R8G8B8 *)(((BYTE *)pDst) + pitchs);
	}
}

void IN2_BLOCK_DXT3_A8R8G8B8(A8R8G8B8 *pDst,int pitchs,DXT3_ELE *pSrc)
{
	RGB32[0].a = RGB32[1].a = RGB32[2].a = RGB32[3].a = 0;

	RGB32[0].r = (pSrc->RGB0.r)<<3;
	RGB32[0].g = (pSrc->RGB0.g)<<2;
	RGB32[0].b = (pSrc->RGB0.b)<<3;

	RGB32[1].r = (pSrc->RGB1.r)<<3;
	RGB32[1].g = (pSrc->RGB1.g)<<2;
	RGB32[1].b = (pSrc->RGB1.b)<<3;

	RGB32[2].r = ((2 * pSrc->RGB0.r + pSrc->RGB1.r) / 3)<<3;
	RGB32[2].g = ((2 * pSrc->RGB0.g + pSrc->RGB1.g) / 3)<<2;
	RGB32[2].b = ((2 * pSrc->RGB0.b + pSrc->RGB1.b) / 3)<<3;

	RGB32[3].r = ((2 * pSrc->RGB1.r + pSrc->RGB0.r) / 3)<<3;
	RGB32[3].g = ((2 * pSrc->RGB1.g + pSrc->RGB0.g) / 3)<<2;
	RGB32[3].b = ((2 * pSrc->RGB1.b + pSrc->RGB0.b) / 3)<<3;

	for(int i = 0; i < 4; i++)
	{
		pDst[0]	= RGB32[pSrc->Idx[i].idx0];
		pDst[1]	= RGB32[pSrc->Idx[i].idx1];
		pDst[2]	= RGB32[pSrc->Idx[i].idx2];
		pDst[3]	= RGB32[pSrc->Idx[i].idx3];

		pDst[0].a = pSrc->Alpha[i].alpha0 << 4;
		pDst[1].a = pSrc->Alpha[i].alpha1 << 4;
		pDst[2].a = pSrc->Alpha[i].alpha2 << 4;
		pDst[3].a = pSrc->Alpha[i].alpha3 << 4;

		pDst = (A8R8G8B8 *)(((BYTE *)pDst) + pitchs);
	}
}

BOOL IN1_DXT1_A8R8G8B8(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)//Need
{
	A8R8G8B8*	dst		= NULL;
	DXT1_ELE*	src		= (DXT1_ELE*)pSrc;
	for(int j = 0; j < h1; j++)
	{
		if(j < h0 && (j % 4) == 0)
		{
			for(int i = 0; i < w0; i += 4)
			{
				dst	= (A8R8G8B8*)(pDst + j * pitchs1 + i * sizeof(A8R8G8B8));
				IN2_BLOCK_DXT1_A8R8G8B8(dst,pitchs1,src);
				src++;
			}
			if(pitchs1 > (int)(w0 * sizeof(A8R8G8B8)))
			{
				for(int i = 0;i < 4;i++)
					memset(pDst + (j + i) * pitchs1 + w0 * sizeof(A8R8G8B8),0,pitchs1 - w0 * sizeof(A8R8G8B8));
			}
		}
		else if(j >= h0)
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

BOOL IN1_DXT3_A8R8G8B8(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)//Need
{
	A8R8G8B8*	dst		= NULL;
	DXT3_ELE*	src		= (DXT3_ELE*)pSrc;
	for(int j = 0; j < h1; j++)
	{
		if(j < h0 && (j % 4) == 0)
		{
			for(int i = 0; i < w0; i += 4)
			{
				dst	= (A8R8G8B8*)(pDst + j * pitchs1 + i * sizeof(A8R8G8B8));
				IN2_BLOCK_DXT3_A8R8G8B8(dst,pitchs1,src);
				src++;
			}
			if(pitchs1 > (int)(w0 * sizeof(A8R8G8B8)))
			{
				for(int i = 0;i < 4;i++)
					memset(pDst + (j + i) * pitchs1 + w0 * sizeof(A8R8G8B8),0,pitchs1 - w0 * sizeof(A8R8G8B8));
			}
		}
		else if(j >= h0)
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

BOOL IN1_A4R4G4B4_A8R8G8B8(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int pitchs0		= w0 * sizeof(A4R4G4B4);
	int pitchs0_	= w0 * sizeof(A8R8G8B8);
	for(int j = 0; j < h1;j++)
	{
		if(j < h0)
		{
			A4R4G4B4 *src = (A4R4G4B4 *)(pSrc + j * pitchs0);
			A8R8G8B8 *dst = (A8R8G8B8 *)(pDst + j * pitchs1);
			for(int i = 0;i < w0;i++)
			{
				dst->a = src->a<<4;
				dst->r = src->r<<4;
				dst->g = src->g<<4;
				dst->b = src->b<<4;
				src++;
				dst++;
			}
			if(pitchs1 > pitchs0_)
				memset(pDst + j * pitchs1 + pitchs0_,0,pitchs1 - pitchs0_);
		}
		else
			memset(pDst + j * pitchs1,0,pitchs1);
	}

	return TRUE;
}

BOOL IN1_A8R8G8B8_A8R8G8B8(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int pitchs0 = w0 * sizeof(A8R8G8B8);
	for(int j = 0; j < h1;j++)
	{
		if(j < h0)
		{
			memcpy(pDst + j * pitchs1,pSrc + j * pitchs0,pitchs0);
			if(pitchs1 > pitchs0)
				memset(pDst + j * pitchs1 + pitchs0,0,pitchs1 - pitchs0);
		}
		else
			memset(pDst + j * pitchs1,0,pitchs1);
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------------------
//目标DXT3
void IN2_BLOCK_DXT1_DXT3(DXT3_ELE *pDst,int pitchs,DXT1_ELE *pSrc)
{
	pDst->RGB0 = pSrc->RGB0;
	pDst->RGB1 = pSrc->RGB1;
	memcpy(pDst->Idx,pSrc->Idx,4);
	memset(pDst->Alpha,0xFF,8);

	if(pSrc->RGB0.col <= pSrc->RGB1.col)
	{
		for(int i = 0; i < 4; i++)
		{
			if(pSrc->Idx[i].idx0 == 3)
				pDst->Alpha[i].alpha0 = 0;
			if(pSrc->Idx[i].idx1 == 3)
				pDst->Alpha[i].alpha1 = 0;
			if(pSrc->Idx[i].idx2 == 3)
				pDst->Alpha[i].alpha2 = 0;
			if(pSrc->Idx[i].idx3 == 3)
				pDst->Alpha[i].alpha3 = 0;
		}
	}
}

BOOL IN1_DXT1_DXT3(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int			pitchs0 = (w0 / 4) * sizeof(DXT3_ELE);
	DXT3_ELE*	dst		= NULL;
	DXT1_ELE*	src		= (DXT1_ELE*)pSrc;
	for(int j = 0;j < (h1 / 4);j++)
	{
		if(j < (h0 / 4))
		{
			for(int i = 0;i < (w0 / 4);i++)
			{
				dst	= (DXT3_ELE*)(pDst + j * pitchs1 + i * sizeof(DXT3_ELE));
				IN2_BLOCK_DXT1_DXT3(dst,pitchs1,src);
				src++;
			}
			if(pitchs1 > pitchs0)
				memset(pDst + j * pitchs1 + pitchs0,0,pitchs1 - pitchs0);
		}
		else
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

BOOL IN1_DXT3_DXT3(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int pitchs0 = (w0 / 4) * sizeof(DXT3_ELE);
	for(int j = 0; j < (h1 / 4);j++)
	{
		if(j < (h0 / 4))
		{
			memcpy(pDst + j * pitchs1,pSrc + j * pitchs0,pitchs0);
			if(pitchs1 > pitchs0)
				memset(pDst + j * pitchs1 + pitchs0,0,pitchs1 - pitchs0);
		}
		else
			memset(pDst + j * pitchs1,0,pitchs1);
	}
	return TRUE;
}

BOOL IN1_A8R8G8B8_DXT3(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int		w		= ((w0 + 3) / 4) * 4;
	int		h		= ((h0 + 3) / 4) * 4;
	BYTE*	psrc	= pSrc;

	if(w != w0 || h != h0)
	{
		DWORD *p0 = (DWORD *)pSrc;
		DWORD *p1 = (DWORD *)buf0;
		for(int j = 0;j < h;j++)
		{
			for(int i = 0;i < w;i++)
			{
				if(i < w0 && j < h0)
					p1[j * w + i] = p0[j * w0 + i];
				else
					p1[j * w + i] = 0;
			}
		}
		psrc = buf0;
	}

	// 开始转换A8R8G8B8为DXTn格式
	ATI_TC_Texture srcTex,dstTex;

	srcTex.dwSize		= sizeof(ATI_TC_Texture);
	srcTex.dwWidth		= w;
	srcTex.dwHeight		= h;
	srcTex.format		= ATI_TC_FORMAT_ARGB_8888;
	srcTex.dwPitch		= w * 4;
	srcTex.dwDataSize	= ATI_TC_CalculateBufferSize(&srcTex);
	srcTex.pData		= psrc;

	dstTex.dwSize		= sizeof(ATI_TC_Texture);
	dstTex.dwWidth		= w;
	dstTex.dwHeight		= h;
	dstTex.format		= ATI_TC_FORMAT_DXT3;
	dstTex.dwPitch		= 0;
	dstTex.dwDataSize	= ATI_TC_CalculateBufferSize(&dstTex);
	dstTex.pData		= buf1;

	if(ATI_TC_ConvertTexture(&srcTex,&dstTex,NULL,NULL,NULL,NULL) != ATI_TC_OK)
		return FALSE;

	return IN1_DXT3_DXT3(pDst,pitchs1,h1,buf1,w,h);
}

BOOL IN1_A4R4G4B4_DXT3(BYTE *pDst,int pitchs1,int h1,BYTE *pSrc,int w0,int h0)
{
	int pitch0 = w0 * sizeof(A8R8G8B8);
	if(!IN1_A4R4G4B4_A8R8G8B8(buf2,pitch0,h0,pSrc,w0,h0))
		return FALSE;

	if(!IN1_A8R8G8B8_DXT3(pDst,pitchs1,h1,buf2,w0,h0))
		return FALSE;

	return TRUE;
}

// 目前目标只有DXT3和A4R4G4B4两种格式
BOOL Convert(int iDstType,BYTE *pDst,int pitchs1,int h1,int iSrcType,BYTE *pSrc,int w0,int h0)
{
	switch(iDstType)
	{
	case TPT_DXT3:
		{
			switch(iSrcType)
			{
			case TPT_DXT1:
				return IN1_DXT1_DXT3(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_DXT3:
				return IN1_DXT3_DXT3(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_A4R4G4B4:
				return IN1_A4R4G4B4_DXT3(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_A8R8G8B8:
				return IN1_A8R8G8B8_DXT3(pDst,pitchs1,h1,pSrc,w0,h0);
			}
			break;
		}
	case TPT_A4R4G4B4:
		{
			switch(iSrcType)
			{
			case TPT_DXT1:
				return IN1_DXT1_A4R4G4B4(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_DXT3:
				return IN1_DXT3_A4R4G4B4(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_A4R4G4B4:
				return IN1_A4R4G4B4_A4R4G4B4(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_A8R8G8B8:
				return IN1_A8R8G8B8_A4R4G4B4(pDst,pitchs1,h1,pSrc,w0,h0);
			}
			break;
		}
	case TPT_A8R8G8B8:
		{
			switch(iSrcType)
			{
			case TPT_DXT1:
				return IN1_DXT1_A8R8G8B8(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_DXT3:
				return IN1_DXT3_A8R8G8B8(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_A4R4G4B4:
				return IN1_A4R4G4B4_A8R8G8B8(pDst,pitchs1,h1,pSrc,w0,h0);
			case TPT_A8R8G8B8:
				return IN1_A8R8G8B8_A8R8G8B8(pDst,pitchs1,h1,pSrc,w0,h0);
			}
			break;
		}
	default:
		break;
	}

	// 清空
	if(iDstType >= TPT_DXT1 && iDstType <= TPT_DXT5)
		h1 /= 4;
	for(int j = 0;j < h1;j++)
		memset(pDst + j * pitchs1,0,pitchs1);

	return FALSE;
}

BOOL Combine_DXT3_DXT1(BYTE *pDXT3,BYTE *pDXT1,int w,int h)
{
	if(!pDXT3 || !pDXT1 || w % 4 || h % 4)
		return FALSE;

	DXT1_ELE *pSrc = (DXT1_ELE *)pDXT1;
	DXT3_ELE *pDst = (DXT3_ELE *)pDXT3;

	for(int j = 0;j < h / 4;j++)
	{
		for(int i = 0;i < w / 4;i++)
		{
			memcpy(pDst->Idx,pSrc->Idx,4);
			pDst->RGB0 = pSrc->RGB0;
			pDst->RGB1 = pSrc->RGB1;

			pSrc++;
			pDst++;
		}
	}
	return TRUE;
}

DWORD CalculateBufferSize(int iType,int w,int h)
{
	if(w <= 0 || h <= 0)
		return 0;

	switch(iType)
	{
	case TPT_A4R4G4B4:
		return (w*h*2);
	case TPT_A8R8G8B8:
		return (w*h*4);
	case TPT_DXT1:
	case TPT_DXT3:
		{
			ATI_TC_Texture srcTex;
			srcTex.dwSize		= sizeof(ATI_TC_Texture);
			srcTex.dwWidth		= w;
			srcTex.dwHeight		= h;
			if(iType == TPT_DXT1)
				srcTex.format	= ATI_TC_FORMAT_DXT1;
			else
				srcTex.format   = ATI_TC_FORMAT_DXT3;
			srcTex.dwPitch		= 0;
			srcTex.dwDataSize	= ATI_TC_CalculateBufferSize(&srcTex);
			return srcTex.dwDataSize;
		}
	}
	return 0;
}

BOOL Apply_S3TC(BYTE* pDst,BYTE* pSrc,int iSrcBytes,int w)
{
	__int64 iMask			= 0x00F000F000F000F0;
	__int64 iScaleOneThird	= 0x5556555655565556;
	__int64 i45				= 0x0008000800080008;
	__int64 iMask1			= 0x000000000000000F;
	__int64 iMask2			= 0x00000000000000F0;
	__int64 iMask3			= 0x0000000000000F00;
	__int64 iMask4			= 0x000000000000F000;
	__int64 iMask5			= 0x00000000FF000000;

	DWORD dwRGB[4] = {0};
	int iX = 0,iY = 0;
	int iWidth = min(256,w);
	int iW = iWidth / 4;
	int i = 0;

	while(i< iSrcBytes)
	{
		_asm
		{
			//dw = 0xFF000000;
			//memcpy(&dw,p+a,3);
			//a += 3;
			mov			eax, pSrc
			add			eax, i
			lea			ebx, i
			add			dword ptr [ebx], 3
			movd		mm0, [eax]			// rgb0						
			por			mm0, iMask5
		
			pxor		mm7, mm7			// 临时变量
			punpcklbw	mm0, mm7
			movq		mm1, mm0			// rgb1

			// 得到RGB0
			pand		mm0, iMask

			// 得到RGB1
			pslld		mm1, 4
			pand		mm1, iMask

			// 得到RGB2
			movq		mm2, mm0
			pslld		mm2, 1				// rgb0 * 2
			paddw		mm2, mm1			// rgb0*2 + rgb1
			pmulhw		mm2, iScaleOneThird	// (rgb0*2 + rgb1)/3
			paddusb		mm2, i45			// 四舍五入

			// 得到RGB3
			movq		mm3, mm1
			pslld		mm3, 1				// rgb1 * 2
			paddw		mm3, mm0			// rgb1*2 + rgb0
			pmulhw		mm3, iScaleOneThird	// (rgb1*2 + rgb0)/3
			paddusb		mm3, i45			// 四舍五入

			// 压缩成一个word -- rgb0
			movq		mm6, mm0
			movq		mm5, mm0
			psrld		mm0, 4				// 1
			pand		mm0, iMask1
			psrld		mm6, 16				// 2
			pand		mm6, iMask2
			por			mm0, mm6
			psrlq		mm5, 28				// 3
			pand		mm5, iMask3
			por			mm0, mm5
			por			mm0, iMask4

			// 压缩成一个word -- rgb1
			movq		mm6, mm1
			movq		mm5, mm1
			psrld		mm1, 4				// 1
			pand		mm1, iMask1
			psrld		mm6, 16				// 2
			pand		mm6, iMask2
			por			mm1, mm6
			psrlq		mm5, 28				// 3
			pand		mm5, iMask3
			por			mm1, mm5
			por			mm1, iMask4

			// 压缩成一个word -- rgb2
			movq		mm6, mm2
			movq		mm5, mm2
			psrld		mm2, 4				// 1
			pand		mm2, iMask1
			psrld		mm6, 16				// 2
			pand		mm6, iMask2
			por			mm2, mm6
			psrlq		mm5, 28				// 3
			pand		mm5, iMask3
			por			mm2, mm5
			por			mm2, iMask4

			// 压缩成一个word -- rgb3
			movq		mm6, mm3
			movq		mm5, mm3
			psrld		mm3, 4				// 1
			pand		mm3, iMask1
			psrld		mm6, 16				// 2
			pand		mm6, iMask2
			por			mm3, mm6
			psrlq		mm5, 28				// 3
			pand		mm5, iMask3
			por			mm3, mm5
			por			mm3, iMask4

			// 输出
			lea			eax, dwRGB
			movd		[eax], mm0
			add			eax, 4
			movd		[eax], mm1
			add			eax, 4
			movd		[eax], mm2
			add			eax, 4
			movd		[eax], mm3
			sub			eax, 12

			//k = (y1*4)*w20 + (x1*4);
			//for(j=0;j<4;j++,a++,k+=w20)
			//{
			//	pBuf[k+0] = dwRGB[ (p[a]>>0) & 0x03 ];
			//	pBuf[k+1] = dwRGB[ (p[a]>>2) & 0x03 ];
			//	pBuf[k+2] = dwRGB[ (p[a]>>4) & 0x03 ];
			//	pBuf[k+3] = dwRGB[ (p[a]>>6) & 0x03 ];
			//}
			// 相素赋值
			mov			ebx, iY
			imul		ebx, iWidth
			add			ebx, iX
			shl			ebx, 3				// k = ((y1*4)*w20 + (x1*4)) * 2
			add			ebx, pDst		// ebx 为pBuf[k+n]的地址
			mov			ecx, pSrc
			add			ecx, i

			// 第1行
			mov			edx, [ecx]
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 2
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 4
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 6
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, iWidth
			sub			edx, 4
			shl			edx, 1
			add			ebx, edx
			add			ecx, 1

			// 第2行
			mov			edx, [ecx]
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 2
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 4
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 6
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, iWidth
			sub			edx, 4
			shl			edx, 1
			add			ebx, edx
			add			ecx, 1

			// 第3行
			mov			edx, [ecx]
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 2
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 4
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 6
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, iWidth
			sub			edx, 4
			shl			edx, 1
			add			ebx, edx
			add			ecx, 1

			// 第4行
			mov			edx, [ecx]
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 2
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 4
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
			add			ebx, 2

			mov			edx, [ecx]
			shr			edx, 6
			and			edx, 3
			mov			edx, [eax+edx*4]
			mov			word ptr [ebx], dx
		}
		_asm emms;

		i += 4;

		iX++;
		if(iX >= iW)
		{
			iX = 0;
			iY++;
		}
	}
	return TRUE;
}

BOOL Apply_RLE(BYTE* dst,BYTE* src,int iSrcBytes)
{
	int iSrc = 0;
	while (iSrc< iSrcBytes)
	{
		BYTE cNum = *src++;
		iSrc++;
		if ( cNum & 0x80)
		{
			for (int i=0;i<cNum-0x80;i++)
			{
				*dst++=*src++;
				*dst++=*src++;
				iSrc+=2;
			}
		}
		else
		{
			BYTE cVal[2];
			cVal[0]=*src++;
			cVal[1]=*src++;
			iSrc+=2;
			for (int i=0;i<cNum;i++)
			{
				*dst++=cVal[0];
				*dst++=cVal[1];
			}
		}
	}
	return TRUE;
}

BOOL Apply_Alpha(WORD* pDst,BYTE* src,int iSrcBytes)
{
	DWORD* dst = (DWORD*)pDst;
	int iSrc = 0;
	DWORD dwMask = 0;

	while(iSrc < iSrcBytes)
	{
		BYTE cNum = *src++;
		iSrc++;
		if(cNum > 0x80)
		{
			int iNum = cNum-0x80;
			for(int i = 0;i < iNum;i++)
			{
				cNum = *src++;
				iSrc++;
				dwMask = (((DWORD)cNum)<<8) | (((DWORD)cNum)<<28) | 0x0FFF0FFF;
				*dst++ &= dwMask;
			}
		}
		else
		{
			int iNum = cNum;
			cNum = *src++;
			iSrc++;
			dwMask = (((DWORD)cNum)<<8) | (((DWORD)cNum)<<28) | 0x0FFF0FFF;

			for(int i = 0;i < iNum;i++)
			{
				*dst++ &= dwMask;
			}
		}
	}
	return TRUE;
}
