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

enum eCompressResult
{
	CR_OK = 0,
	CR_STREAM_END  =  1,
	CR_NEED_DICT   =  2,
	CR_ERRNO       = (-1),
	CR_STREAM_ERROR= (-2),
	CR_DATA_ERROR  = (-3),
	CR_MEM_ERROR   = (-4),
	CR_BUF_ERROR   = (-5),
	CR_VERSION_ERROR= (-6),
};

// 压缩函数
int Compress(BYTE *pDest,DWORD *pDestSize,BYTE *pSrc,DWORD dwSrcSize,eCompressLevel lvl);

// 解压缩函数
int Uncompress(BYTE *pDest,DWORD *pDestSize,BYTE *pSrc,DWORD dwSrcSize);