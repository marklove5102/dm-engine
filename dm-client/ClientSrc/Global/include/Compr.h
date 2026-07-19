#pragma once

#include <windows.h>

// 压缩类型
enum eCompressLevel
{
	CL_RLE_COMPRESS		= 0	,	// RLE压缩
	CL_BEST_SPEED			,	// 最快zip压缩
	CL_BEST_COMPRESS		,	// 最好zip压缩
	CL_DEFAULT_COMPRESS		,	// 一般zip压缩
};

// 压缩函数
int Compress(BYTE *pDest,DWORD *pDestSize,BYTE *pSrc,DWORD dwSrcSize,eCompressLevel lvl);

// 解压缩函数
int Uncompress(BYTE *pDest,DWORD *pDestSize,BYTE *pSrc,DWORD dwSrcSize);