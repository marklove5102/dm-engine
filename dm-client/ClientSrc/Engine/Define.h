#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define D3D_PI			3.1415926538f
#define NML_POOL_SIZE	(256 * 256 * 20 + 16)


// Êµ¼ÊµÄ»º³å³Ø
struct PoolData;
struct PixelTable;

extern PoolData     s_OldPool[];
extern PoolData		s_TexPool[];
extern PoolData		s_FontPool[];
extern PixelTable	s_Table[];
extern BYTE			s_Buf[];