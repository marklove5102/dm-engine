#include "Compr.h"

#include "zlib.h"

// Ñ¹Ëõº¯Êý
int Compress(BYTE *pDest,DWORD *pDestSize,BYTE *pSrc,DWORD dwSrcSize,eCompressLevel lvl)
{
	int level = Z_NO_COMPRESSION;
	switch(lvl)
	{
	case CL_RLE_COMPRESS:
		level = Z_RLE;
		break;
	case CL_BEST_SPEED:
		level = Z_BEST_SPEED;
		break;
	case CL_BEST_COMPRESS:
		level = Z_BEST_COMPRESSION;
		break;
	case CL_DEFAULT_COMPRESS:
		level = Z_DEFAULT_COMPRESSION;
		break;
	}

	return compress2(pDest,pDestSize,pSrc,dwSrcSize,level);
}

// ½âÑ¹Ëõº¯Êý
int Uncompress(BYTE *pDest,DWORD *pDestSize,BYTE *pSrc,DWORD dwSrcSize)
{
	return uncompress(pDest,pDestSize,pSrc,dwSrcSize);
}