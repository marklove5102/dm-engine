#include "FreeType.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Global/include/LufoList.h"
#include <map>
#include <string>
#include <math.h>

#pragma warning(disable : 4996)

#ifdef _DEBUG
#pragma comment(lib,"freetype239MT.lib")
#else
#pragma comment(lib,"freetype239MT.lib")
#endif

#define MAXCHACHESIZE         300
#define SAFE_DELETE(p)        { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)  { if(p) { delete [] (p); (p)=NULL;		} }		// 安全的删除new []出来的数组

struct Font_s
{
	Font_s()
	{
		face = 0;
		cache.clear();
		sFontName.clear();
        iAscender = 12;
        iDescender = 0;
		iWidth = 12;
        iHeight = 12;
		iCurFontSize = 0;
	}

	~Font_s()
	{
		clear();
	}

	void clear()
	{
		face = NULL;
		sFontName.clear();

		while(!cache.empty())
		{
			MCache::iterator it = cache.begin();
			SAFE_DELETE_ARRAY(it->second->pBuf);
			SAFE_DELETE(it->second);
			cache.erase(it);
		}
	}

	FT_Face     face;
	std::string   sFontName;
	int iCurFontSize;
	int iAscender;
	int iDescender;
	int iWidth;
	int iHeight;

	typedef std::map<DWORD,BitmapInfo*> MCache;
	MCache cache;
	CLufoList<DWORD> Lufo;
};

FT_Library  library = NULL;
FT_Error    error = 0 ;

CFreeType::CFreeType(void)
{
	error = FT_Init_FreeType( &library );
	if( error != 0)
	{
        //initFreeType Failed
	}
	m_iCurFont = 0;
} 


CFreeType::~CFreeType(void)
{
	//清空缓冲区中动态申请的内存
	for(size_t ii = 0; ii < m_Fonts.size();ii++)
	{
		Font_s* font = (Font_s*)m_Fonts.at(ii);
		if(font)
		{
			font->clear();
			SAFE_DELETE(font);
		}
	}
	m_Fonts.clear();

	if(library)
		FT_Done_FreeType(library);

}

int	CFreeType::GetFont(void)
{
	return m_iCurFont;
}

bool CFreeType::SetFont(int iFont)
{
	if(iFont < 0 || iFont >= (int)m_Fonts.size())
		return false;

    m_iCurFont = iFont;
	return true;
}

bool CFreeType::SetCurFontSize(int iFontSize,int *pCharWidth,int *pCharHeight,int *pBaseLine)
{
	if (m_iCurFont >= m_Fonts.size())
	{
		return false;
	}

	Font_s* font = (Font_s*)m_Fonts.at(m_iCurFont);
	if(font == NULL || font->face == NULL)
		return false;

	if (font->iCurFontSize == iFontSize)
	{
		if (pCharWidth && pCharHeight && pBaseLine)
		{
			*pCharWidth = font->iWidth;
			*pCharHeight = font->iHeight;
			*pBaseLine = -font->iDescender;
		}

		return true;
	}

	//设置字体大小
	if (FT_IS_SCALABLE(font->face) ) 
	{
		if(FT_Set_Pixel_Sizes(font->face, iFontSize, iFontSize) != 0)
			return false;
	}
	else
	{
		if ( iFontSize >= font->face->num_fixed_sizes )
			iFontSize = font->face->num_fixed_sizes - 1;
		if(FT_Set_Pixel_Sizes( font->face,font->face->available_sizes[iFontSize].height,
			font->face->available_sizes[iFontSize].width ) == 0)
		{
			return false;
		}
	}

 	//float m_fAscender = (float)font->face->ascender * font->face->size->metrics.y_scale * float(1.0/64.0) * (1.0f/65536.0f);
 	//float m_fDescender = (float)font->face->descender * font->face->size->metrics.y_scale * float(1.0/64.0) * (1.0f/65536.0f);
 	//float m_fHeight = (float)font->face->height * font->face->size->metrics.y_scale * float(1.0/64.0) * (1.0f/65536.0f);
	//float f1 = ceil(m_fAscender);
	//float f = -floor(-m_fDescender);


	//font->iAscender = font->face->size->metrics.ascender >> 6;
	//font->iDescender = font->face->size->metrics.descender >> 6;

	font->iAscender = (int)(ceil((float)font->face->ascender * font->face->size->metrics.y_scale * float(1.0/64.0) * (1.0f/65536.0f)));
	font->iDescender = -(int)(floor(-1.0f * (float)font->face->descender * font->face->size->metrics.y_scale * float(1.0/64.0) * (1.0f/65536.0f)));

	font->iHeight = font->face->size->metrics.height >> 6;
	font->iWidth = font->face->size->metrics.max_advance >> 6;
	font->iCurFontSize = iFontSize;

	if (pCharWidth && pCharHeight && pBaseLine)
	{
		*pCharWidth = font->iWidth;
		*pCharHeight = font->iHeight;
		*pBaseLine = -font->iDescender;
	}

	return true;
}

bool CFreeType::AddFont(const char* path)
{
	Font_s*  font = NULL;
	try
	{
		font = new Font_s();
	}
	catch(...)
	{
		font = NULL;
		return false;
	}

	font->sFontName.assign(path);

	error = FT_New_Face(library,path,0,&font->face);
	if ( error == FT_Err_Unknown_File_Format )
	{
		//非法字体
		SAFE_DELETE(font);
		return false;
	}
	else if ( error )
	{
		//其它错误
		SAFE_DELETE(font);
		return false;
	}

	m_Fonts.push_back(font);
	return true;
}


bool CFreeType::GetWord(BitmapInfo **pBI,WCHAR wChar)
{
	if (m_iCurFont >= m_Fonts.size())
	{
		return false;
	}

	Font_s* font = (Font_s*)m_Fonts.at(m_iCurFont);
	if(font == NULL || font->face == NULL)
		return FALSE;

	//先查看cache有没有iSize字号的wChar
	DWORD dwSizeWchar = MAKELONG(font->iCurFontSize,wChar);

	Font_s::MCache::iterator it;
	it = font->cache.find(dwSizeWchar);

	if(it != font->cache.end())
	{
		*pBI = it->second;
		//最近使用的放到List最前，用于淘汰最久没使用的cache
		font->Lufo.reflesh(dwSizeWchar);

		return TRUE;
	}


	//构造点阵
	FT_GlyphSlot glyph;

	if(FT_Load_Char(font->face,wChar,FT_LOAD_DEFAULT) != 0)
		return FALSE;

	glyph = font->face->glyph;

	if(FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL ) != 0)
		return FALSE;

	FT_Bitmap* src = &glyph->bitmap;

	BitmapInfo *pBitmapInfo = new BitmapInfo;

	pBitmapInfo->x = glyph->bitmap_left;
	//pBitmapInfo->y = glyph->bitmap_top;
	//pBitmapInfo->y = height - glyph->bitmap_top - (int)(float(-font->face->descender)/(font->face->ascender - font->face->descender)*height + 0.50f);
	pBitmapInfo->y = font->iAscender - glyph->bitmap_top;

	//int glyph_w = int (ceil (glyph->metrics.width * float(1.0/64.0)));

	pBitmapInfo->w = glyph->bitmap.width;
	pBitmapInfo->h = glyph->bitmap.rows;
	pBitmapInfo->bMono = (glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)?TRUE:FALSE;
	pBitmapInfo->pitch = glyph->bitmap.pitch < 0?-glyph->bitmap.pitch:glyph->bitmap.pitch;

	pBitmapInfo->pBuf = new BYTE[pBitmapInfo->pitch*pBitmapInfo->h];

	//复制点阵数据	
	for ( int i = 0; i < pBitmapInfo->h; i++)
	{
		int soffset = i * src->pitch;
		int doffset = i * pBitmapInfo->pitch;
		if (glyph->bitmap.pitch < 0)
		{
			soffset = (- src->pitch * src->rows) - src->pitch * i;
		}

		memcpy(pBitmapInfo->pBuf+doffset,src->buffer+soffset, src->pitch);
	}
		
	*pBI = pBitmapInfo;

	//加入缓冲区
	font->cache[dwSizeWchar] = pBitmapInfo;
	font->Lufo.push(dwSizeWchar);

	//如果超出最大缓冲，则淘汰最久未用的
	if(font->cache.size() > MAXCHACHESIZE)
	{
		for(int i = 0; i < MAXCHACHESIZE / 10; i++)//淘汰最久没有使用的30项
		{
			if(font->Lufo.size() <= 0)
				break;

			dwSizeWchar = font->Lufo.back();

			Font_s::MCache::iterator it = font->cache.find(dwSizeWchar);
			if(it != font->cache.end() )
			{
				SAFE_DELETE_ARRAY(it->second->pBuf);
				SAFE_DELETE(it->second);
				font->cache.erase(it);
			} 

			font->Lufo.pop();
		}
	}
	return TRUE;

}