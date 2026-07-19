
#pragma once

BOOL Convert(int iDstType,BYTE *pDst,int pitchs1,int h1,int iSrcType,BYTE *pSrc,int w0,int h0);
BOOL Combine_DXT3_DXT1(BYTE *pDXT3,BYTE *pDXT1,int w,int h);
DWORD CalculateBufferSize(int iType,int w,int h);
BOOL Apply_S3TC(BYTE* pDst,BYTE* pSrc,int iSrcBytes,int w);
BOOL Apply_RLE(BYTE* pDst,BYTE* pSrc,int iSrcBytes);
BOOL Apply_Alpha(WORD* pDst,BYTE* pSrc,int iSrcBytes);