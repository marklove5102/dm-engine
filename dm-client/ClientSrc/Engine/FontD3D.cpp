#include "stdafx.h"
#include "TextureD3D.h"
#include "GraphicD3D.h"
#include "TexManager.h"
#include "FontD3D.h"
#include "Global/Interface/CallBackInterface.h"
#include "../FreeTypeDLL/FreeType.h"

#include "Crc32.h"
#include "Global/DebugTry.h"

#define	FONT_PERPIXEL			4		// A8R8G8B8格式的字体纹理
#define POOL_SIZE_16			(128 * 16 * FONT_PERPIXEL)
#define POOL_SIZE_32			(128 * 32 * FONT_PERPIXEL)
#define POOL_SIZE_64			(128 * 64 * FONT_PERPIXEL)
#define POOL_SIZE_256			(256 * 256 * FONT_PERPIXEL)

CFreeType g_pFreeType;



template<typename T> FORCEINLINE T Bound(T x, T m, T M) { return (x < m) ? m : ((x > M) ? M : x); }

//字体配置管理,可配置轮廓,对比度等
class CFontConfig
{
public:
	CFontConfig(){}
	~CFontConfig(){}

	void Init(float weight = 1.5f,float contrast = 1.5f,float gamma = 1.4f,int mode = 0);
	inline BYTE GetAlpha(BYTE byAlpha);


private:
	inline int Conv1(BYTE n) {return tbl1[n];}
	inline BYTE Conv2(int n) {return tbl2[n / (BASE * BASE / (sizeof tbl2 - 1))];}
	inline int ConvAlpha(int alpha) {return alphatbl[alpha];}
	inline BYTE Rconv1(int n);



protected:
	int alphatbl[256];
	int tbl1[257];
	BYTE tbl2[256 * 16 + 1];

	int tunetbl[256];
	int BASE;


	int alpha0;//如果下一次要求计算的alpha==alpha0直接返回c0就可以了
	BYTE c0;//上一次计算的值,如果下一次要求计算的alpha==alpha0直接返回就可以了
	int temp_fg;//为了速度,暂存的conv1(0xFF)
};

static CFontConfig s_AlphaBlendTable;

BYTE CFontConfig::Rconv1(int n)
{
	int pos = 0x80;
	int i = pos >> 1;
	while (i > 0) {
		if (n >= tbl1[pos]) {
			pos += i;
		} else {
			pos -= i;
		}
		i >>= 1;
	}
	if (n >= tbl1[pos]) {
		++pos;
	}
	return (BYTE)(pos - 1);
}

void CFontConfig::Init(float weight,float contrast,float gamma,int mode)
{
	//const float weight = 1.5f;//pSettings->RenderWeight();越大越粗越黑
	//const float contrast = 1.5f;//pSettings->Contrast();对比度,越大越锐,越小越发虚
	//const float gamma = 1.4f;//pSettings->GammaValue();
	//const int mode = 0;//pSettings->GammaMode();-1:禁止,0:使用gamma,1:SRGB

	int i;
	float temp, alpha;

	BASE = 0x4000;

	for (i = 0; i < 256; ++i) 
	{
		temp = pow((1.0f / 255.0f) * i, 1.0f / weight);

		if  (temp < 0.5f)
		{
			alpha = pow(temp * 2, contrast) / 2.0f;
		} 
		else 
		{
			alpha = 1.0f - pow((1.0f - temp) * 2, contrast) / 2.0f;
		}
		alphatbl[i] = (int)(alpha * BASE);

		if (mode < 0) 
		{
			temp = (1.0f / 255.0f) * i;
		} 
		else
		{
			if (mode == 1) 
			{
				if (i <= 10) 
				{
					temp = (float)i / (12.92f * 255.0f);
				} 
				else 
				{
					temp = pow(((1.0f / 255.0f) * i + 0.055f) / 1.055f, 2.4f);
				}
			} 
			else if (mode == 2) 
			{
				if (i <= 10) 
				{
					temp = ((float)i / (12.92f * 255.0f) + (float)i / 255.0f) / 2;
				} 
				else 
				{
					temp = (pow(((1.0f / 255.0f) * i + 0.055f) / 1.055f, 2.4f) + (float)i / 255.0f) / 2;
				}
			} 
			else 
			{
				temp = pow((1.0f / 255.0f) * i, gamma);
			}
		}
		tbl1[i] = (int)(temp * BASE);
	}

	tbl1[i] = BASE;

	for (i = 0; i <= sizeof tbl2 - 1; ++i) 
	{
		tbl2[i] = Rconv1(i * (BASE / (sizeof tbl2 - 1)));
	}



	int  nTuneTable[256];
	int col;
	double tmp, p;
	int v = 0;//0-12

	p = (double)v;
	p = 1 - (p / (p + 10.0));
	for(i = 0;i < 256;i++)
	{
		tmp = (double)i / 255.0;
		tmp = pow(tmp, p);
		col = 255 - (int)(tmp * 255.0 + 0.5);
		nTuneTable[255 - i] = col;
	}


	for (i = 0; i < 256; ++i)
	{
		tunetbl[i] = alphatbl[Bound(nTuneTable[i], 0, 255)];
	}

	temp_fg = Conv1(0xFF);

}


FORCEINLINE BYTE CFontConfig::GetAlpha(BYTE alpha)
{
	if (alpha0 == alpha) return c0;
	int temp_alpha = tunetbl[alpha];
	if (temp_alpha <= 0) return 0;
	if (temp_alpha >= BASE) return 0xFF;
	int temp_bg = Conv1(0);
	int temp = temp_bg * (BASE - temp_alpha) + temp_fg * temp_alpha;

	alpha0 = alpha;
	c0 = Conv2(temp);
	return c0;
}







CFontD3D::CFontD3D(void):m_FixPool16(POOL_SIZE_16,96),m_FixPool32(POOL_SIZE_32,32),m_FixPool64(POOL_SIZE_64,16),m_FixPool256(POOL_SIZE_256,4)
{
	m_pListHead	= NULL;
	m_pListEnd	= NULL;
	m_iCurFontType = FONT_DEFAULT;

	//m_pFreeType = GetFreeTypeInstance();
	m_pFreeType = &g_pFreeType;

	s_AlphaBlendTable.Init();

	//为GDI字体准备
	m_iLastGUIFont = FONT_DEFAULT;
	m_iLastFontSize = 16;
	m_hFont = NULL;
	m_iFace = 0;
	m_hDC = CreateCompatibleDC(NULL);
	SetTextColor(m_hDC,RGB(255,255,255));
	SetBkColor(m_hDC,0);
	SetTextAlign(m_hDC,TA_TOP);

	ZeroMemory(&m_sBmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	m_sBmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_sBmi.bmiHeader.biPlanes		= 1;
	m_sBmi.bmiHeader.biBitCount		= 32;
	m_sBmi.bmiHeader.biCompression	= BI_RGB;

	for(int i = FONT_GDI_XINSONG;i < FONT_NUM;i ++)
	{
		m_sFontLib[i].height	= 16;
		m_sFontLib[i].width		= m_sFontLib[i].height / 2;
		ZeroMemory(&m_sFontLib[i].sLogFont,sizeof(m_sFontLib[i].sLogFont));
		m_sFontLib[i].sLogFont.lfHeight			= - m_sFontLib[i].height;
		m_sFontLib[i].sLogFont.lfWeight			= FW_NORMAL;
		m_sFontLib[i].sLogFont.lfCharSet		= DEFAULT_CHARSET;
		m_sFontLib[i].sLogFont.lfPitchAndFamily	= FIXED_PITCH;
	}

	strcpy(m_sFontLib[FONT_GDI_XINSONG].sLogFont.lfFaceName,	"新宋体");
	strcpy(m_sFontLib[FONT_GDI_KAITI].sLogFont.lfFaceName,		"楷体_GB2312");
	strcpy(m_sFontLib[FONT_GDI_HEITI].sLogFont.lfFaceName,		"黑体");
	strcpy(m_sFontLib[FONT_GDI_FANGSONG].sLogFont.lfFaceName,	"仿宋_GB2312");
	strcpy(m_sFontLib[FONT_GDI_SONG_XIE].sLogFont.lfFaceName,	"新宋体");
	strcpy(m_sFontLib[FONT_GDI_SONG_CU].sLogFont.lfFaceName,	"新宋体");
	strcpy(m_sFontLib[FONT_GDI_YOUYUAN].sLogFont.lfFaceName,	"幼圆");
	strcpy(m_sFontLib[FONT_GDI_LISHU].sLogFont.lfFaceName,		"隶书");
	strcpy(m_sFontLib[FONT_GDI_SHUTI].sLogFont.lfFaceName,		"方正舒体");
	strcpy(m_sFontLib[FONT_GDI_TAOTI].sLogFont.lfFaceName,		"方正姚体");
	strcpy(m_sFontLib[FONT_GDI_CAIYUN].sLogFont.lfFaceName,		"华文彩云");
	strcpy(m_sFontLib[FONT_GDI_XIHEI].sLogFont.lfFaceName,		"华文细黑");
	strcpy(m_sFontLib[FONT_GDI_XINWEI].sLogFont.lfFaceName,		"华文新魏");
	strcpy(m_sFontLib[FONT_GDI_HANGKAI].sLogFont.lfFaceName,	"华文行楷");
	strcpy(m_sFontLib[FONT_GDI_ZHONGSONG].sLogFont.lfFaceName,	"华文中宋");

	//两种特殊字体处理
	m_sFontLib[FONT_GDI_SONG_XIE].sLogFont.lfItalic = true;
	m_sFontLib[FONT_GDI_SONG_CU].sLogFont.lfWeight	= FW_BOLD;

	//End 为GDI字体准备




	char path[128] = {0};

	//载入12像素ASCII点阵
	sprintf(path,"%s/font/asc.fnt",g_pCallBack->GetDataDir());
	FILE *fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vAsc12,FONT_SONGTI_12_ASC_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vAsc12,0,FONT_SONGTI_12_ASC_SIZE);
	}

	//12像素GBK汉字字符
	sprintf(path,"%s\\font\\gbk12.fnt",g_pCallBack->GetDataDir());
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vFnt12,FONT_SONGTI_12_FNT_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vFnt12,0,FONT_SONGTI_12_FNT_SIZE);
	}

	// 14像素ASCII字符
	sprintf(path,"%s\\font\\ASC14.fnt",g_pCallBack->GetDataDir());
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vAsc14,FONT_SONGTI_14_ASC_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vAsc14,0,FONT_SONGTI_14_ASC_SIZE);
	}

	// 14像素汉字字符
	sprintf(path,"%s\\font\\gbk14.fnt",g_pCallBack->GetDataDir());
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vFnt14,FONT_SONGTI_14_FNT_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vFnt14,0,FONT_SONGTI_14_FNT_SIZE);
	}

	// 16像素ASCII字符
	sprintf(path,"%s\\font\\asc16.fnt",g_pCallBack->GetDataDir());
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vAsc16,FONT_SONGTI_16_ASC_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vAsc16,0,FONT_SONGTI_16_ASC_SIZE);
	}

	// 16像素汉字字符
	sprintf(path,"%s\\font\\gbk16.fnt",g_pCallBack->GetDataDir());
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vFnt16,FONT_SONGTI_16_FNT_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vFnt16,0,FONT_SONGTI_16_FNT_SIZE);
	}

	//载入笑脸符号
	sprintf(path,"%s/font/face.fnt",g_pCallBack->GetDataDir());
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(&m_iFace,sizeof(WORD),1,fp);
		if(m_iFace > 100) //异常数据
		{
			m_iFace = 0;
			m_vFace = NULL;
			fclose(fp);
		}
		int iSize = m_iFace * 12 * 12;
		m_vFace = new WORD[iSize];
		memset(m_vFace,0,sizeof(WORD)*iSize);
		fread(m_vFace,sizeof(WORD),iSize,fp);
		fclose(fp);
	}
	else
	{
		m_iFace = 0;
		m_vFace = NULL;
	}

	//载入TrueType字体
	const char* FONT_STR[] =
	{
		"simsun.ttc",
		"simli.ttf",
		"MSYH.TTF",
	};

	char szWinDir[256] = {0};
	GetWindowsDirectory(szWinDir,256);

	char szFontPath[256] = {0};
	char szDestPath[256] = {0};
	for (int i = 0; i < 2; i++)
	{
		sprintf(szFontPath,"%s\\Fonts\\%s",szWinDir,FONT_STR[i]);
		sprintf(szDestPath,"../Data/font/%s",FONT_STR[i]);
		CopyFile(szFontPath,szDestPath,true);
	}


	for(int i = 0;i< 3;i++)
	{
		sprintf(path,"%s\\font\\%s",g_pCallBack->GetDataDir(),FONT_STR[i]);
		if(!m_pFreeType->AddFont(path))
		{
			if (i > 0)
			{
				sprintf(path,"%s\\font\\%s",g_pCallBack->GetDataDir(),FONT_STR[0]);
				m_pFreeType->AddFont(path);
			}
		}
	}
}


CFontD3D::~CFontD3D(void)
{
	SAFE_DELETE_ARRAY(m_vFace);
	ListClear();
	// GDI 清除
	if(m_hFont)
		DeleteObject((HGDIOBJ)m_hFont);

	DeleteDC(m_hDC);	
}

bool CFontD3D::SetFont(int iFont)
{
	if(iFont < 0 || iFont >= FONT_NUM || iFont == FONT_FREETYPE_NUM)
	{
		return false;
	}

	if (m_iCurFontType == iFont)
	{
		return true;
	}

	if(iFont >= FONT_SONGTI && iFont < FONT_FREETYPE_NUM)
	{
		if (!m_pFreeType->SetFont(iFont))
		{
			m_iCurFontType = FONT_DEFAULT;
			return false;
		}		
	}
	m_iCurFontType = iFont; 

	return true;
}

CFontD3D::stList* CFontD3D::ListAdd(stTextID& textID)
{
	// 使用淘汰
	ListWash(textID.dwCount,400);

	stList *pList = new stList;
	if(pList)
	{
		memcpy(pList,&textID,sizeof(stTextID));

		if(m_pListHead == NULL)
			m_pListHead = m_pListEnd = pList;
		else
		{
			pList->Next			= m_pListHead;
			m_pListHead->Prev	= pList;

			m_pListHead			= pList;
		}
	}
	return pList;
}

CFontD3D::stList* CFontD3D::ListFind(stTextID& textID)
{
	stList* pList = m_pListHead;
	while(pList)
	{
		if(memcmp(pList,&textID,28) == 0)
			return pList;
		pList = pList->Next;
	}
	return pList;
}

void CFontD3D::ListDel(stList *pList)
{
	if(pList == m_pListHead)
		m_pListHead = pList->Next;
	if(pList == m_pListEnd)
		m_pListEnd = pList->Prev;

	if(pList->Prev)
		pList->Prev->Next = pList->Next;
	if(pList->Next)
		pList->Next->Prev = pList->Prev;

	if(pList->pTex)
		pList->pTex->ReleaseFontTex();
	SAFE_DELETE(pList->pTex);

	SAFE_DELETE(pList);
}


void CFontD3D::ListMoveHead(stList *pList)
{
	pList->dwCount = g_pEGfx->GetFrameCount();

	if(pList == m_pListHead)
		return;

	if(pList == m_pListEnd)
		m_pListEnd = pList->Prev;

	if(pList->Prev)
		pList->Prev->Next = pList->Next;
	if(pList->Next)
		pList->Next->Prev = pList->Prev;

	pList->Prev			= NULL;
	pList->Next			= m_pListHead;
	m_pListHead->Prev	= pList;
	m_pListHead			= pList;
}

void CFontD3D::ListWash(DWORD dwCount,DWORD dwDelta)
{
	if(m_pListEnd && m_pListEnd->dwCount > 0 && (int)(dwCount - m_pListEnd->dwCount) >= (int)dwDelta)
		ListDel(m_pListEnd);
}

void CFontD3D::ListClear(void)
{
	stList*	pTemp = NULL;		
	stList* pList = m_pListHead;
	while(pList)
	{
		pTemp = pList->Next;
		if(pList->pTex)
			pList->pTex->ReleaseFontTex();
		SAFE_DELETE(pList->pTex);
		SAFE_DELETE(pList);
		pList = pTemp;
	}

	m_pListHead = m_pListEnd = NULL;
}

BOOL CFontD3D::GetWordPixel(stTextID& textID,WORD wTemp,int& iw,DWORD* pdwBuf,DWORD dwColor)
{
	int iCharWidth = textID.iFontSize;
	if(wTemp < 128)
		iCharWidth = textID.iFontSize/2;

	BitmapInfo	*pBitmapinfo = NULL;


	TRY_BEGIN;
	if(m_pFreeType->GetWord(&pBitmapinfo,wTemp) == FALSE || pBitmapinfo == NULL)
	{
		iw += textID.iFontSize;
		return FALSE;
	}
	TRY_END;

	int iBitmapX = pBitmapinfo->x;
	int iBitmapY = pBitmapinfo->y;

	if(iBitmapX + pBitmapinfo->w > iCharWidth)//iCharWidth单个字符的宽度
		iBitmapX = iCharWidth - pBitmapinfo->w;
	if(iBitmapX < 0)
		iBitmapX = 0;

	if(iBitmapY + pBitmapinfo->h > textID.ih)
		iBitmapY = textID.ih - pBitmapinfo->h;
	if(iBitmapY < 0)
		iBitmapY = 0;


	int x,y;
	BYTE byAlpha;
	DWORD dwAlpha;


	for(int h = 0;h < pBitmapinfo->h;h++)
	{
		for(int w = 0;w < pBitmapinfo->w;w++)
		{
			if(pBitmapinfo->bMono)
			{
				if(pBitmapinfo->pBuf[h * pBitmapinfo->pitch + w / 8] & (1<<~((w&0x00000007)|0xFFFFFFF8)))
					byAlpha = 0xFF;
				else
					byAlpha = 0x00;
			}
			else
			{
				byAlpha = pBitmapinfo->pBuf[h * pBitmapinfo->pitch + w];
			}

			x = iw + iBitmapX + w;
			y = iBitmapY + h + 1;//+1:为了和点阵字体保持一致,第一行留空
			if(x < textID.iw && y < textID.ih && y * textID.iw + x >= 0)
			{
				dwAlpha = s_AlphaBlendTable.GetAlpha(byAlpha);//byAlpha;
				dwAlpha = (dwAlpha << 24) | 0x00FFFFFF;
				pdwBuf[y * textID.iw + x] = dwAlpha & dwColor;
			}
		}
	}

	//iw += pBitmapinfo->x + pBitmapinfo->w;
	iw += iCharWidth;

	return TRUE;
}

BOOL CFontD3D::GetTextTex(stTextID& textID,LPCTSTR str)
{
	int iFont = textID.iFontCRC;
	if(iFont == FONT_SONGTI && (textID.iFontSize == FONTSIZE_DEFAULT || textID.iFontSize == FONTSIZE_MIDDLE || textID.iFontSize == FONTSIZE_BIG))
		return GetTextTexFNT(textID,str);
	else if(iFont >= FONT_SONGTI && iFont < FONT_FREETYPE_NUM)
		return GetTextTexFreeType(textID,str);
	else if(iFont >= FONT_GDI_XINSONG && iFont < FONT_NUM)
		return GetTextTexGDI(textID,str);

	return FALSE;
}

BOOL CFontD3D::GetTextTexFNT(stTextID& textID,LPCTSTR str)
{
	TRY_BEGIN;

	BYTE c1,c2;
	int len = (int)strlen(str);

	textID.dwCount	= g_pEGfx->GetFrameCount();
	textID.iw		= GetLen(str,NULL,textID.iFontSize) + 1;//+ 2;
	textID.ih		= textID.iFontSize + 1;

	int    cw       = textID.iFontSize / 2;   //单个字符的宽度
	int    ch       = textID.iFontSize;       //单个字符的高度
	int  clen		= cw * ch;                //ASCII码字符所占字节数
	int  chzlen		= ((cw*2+7) / 8) * ch;    //汉字所占字节数

	if(textID.iw * 64 * FONT_PERPIXEL > NML_POOL_SIZE)
		return FALSE;

	memset(s_Buf,0,textID.iw * 64 * FONT_PERPIXEL);
	DWORD *pdwBuf = (DWORD *)s_Buf;

	// 填充A4R4G4B4像素数据
	int		iw = 1,iCount = 0;
	DWORD	dwColor = textID.dwColor;
	for(int i = 0;i < len;i++)
	{
		c1 = (BYTE)str[i];
		c2 = (BYTE)str[i + 1];
		
		if(c1== 0xF8 && c2>=0xA1 && c2<=0xFE) //笑脸符号
		{
			if(m_vFace != NULL && m_iFace > 0)
			{
				c2 -= 0xA1;
				if( c2 >= m_iFace)
					c2 = m_iFace-1;

				WORD* pwFace = &m_vFace[c2*144];
				DWORD dwFace = 0;

				for(int h = 0;h < 12;h++)
				{
					for(int w = 0;w < 12;w++)
					{
						dwFace = *pwFace;
						if(dwFace & 0xF000)
						{
							pdwBuf[h * textID.iw + iw + w] = 0xFF000000 | ((dwFace & 0x00000F00) << 12) | ((dwFace & 0x000000F0) << 8) | ((dwFace & 0x0000000F) << 4);
						}
						pwFace++;
					}
				}
			}
			iw += 12;
			i++;
		}
		else if( c1>=0x81 && c2>=0x40 && c1<=0xFE && c2<=0xFE )// 中文
		{
			BYTE* pChar = &m_vFnt12[((c1-129)*191+(c2-64))*chzlen];
			if(textID.iFontSize == FONTSIZE_MIDDLE)
			{
				pChar = &m_vFnt14[((c1-129)*191+(c2-64))*chzlen];
			}
			else if (textID.iFontSize == FONTSIZE_BIG)
			{
				pChar = &m_vFnt16[((c1-129)*191+(c2-64))*chzlen];
			}

			for(int y = 1;y < ch;y++)
			{
				int j = y*textID.iw + 1 + i*cw;

				for(int ii = 0;ii< 8;ii++)
				{
					BYTE cMask = (0x80 >> ii);
					if(*pChar & cMask)
						pdwBuf[j+ ii]=dwColor;
				}
				pChar++;

				for(int ii = 8;ii< cw * 2;ii++)
				{
					BYTE cMask = (0x80 >> (ii - 8));
					if(*pChar & cMask)
						pdwBuf[j+ ii]=dwColor;
				}
				pChar++;
			}

			iw += textID.iFontSize;
			i++;
		}
		else// ASCII
		{		
			if( c1<32 || c1>126 ) c1 = 32;

			if(textID.iFontSize == FONTSIZE_DEFAULT)
			{
				int count = 9 * (c1 - 32) * 8;
				for(int h = 0;h < 12;h++)
				{
					for(int w = 0;w < 6;w++)
					{
						if(m_vAsc12[count/8] & 1<<(7 - (count % 8)))
							pdwBuf[h * textID.iw + iw + w] = dwColor;
						count++;
					}
				}
			}
			else
			{
				WORD* pAsc = &m_vAsc14[(c1-32)*clen];
				if (textID.iFontSize == FONTSIZE_BIG)
				{
					pAsc = &m_vAsc16[(c1-32)*clen];
				}

				// 英文字符，每个点都是16bit为A4R4G4B4格式，可以直接memcpy
				for(int h = 1;h < ch;h++) 
				{
					int j = h*textID.iw + 1 + i*cw;
					for(int w = 0;w < cw;w++)
					{
						if(pAsc[w])
							pdwBuf[j+w]=dwColor;
					}
					pAsc += cw;
				}
			}

			iw += cw;
		}
	}

	// 下划线
	if(textID.dwFlag & DTF_UnderLine)
	{
		for(int i = 0;i < textID.iw;i++)
			pdwBuf[(textID.ih - 1) * textID.iw + i] = dwColor;
	}

	// 粗体
	if(textID.dwFlag & DTF_Bold)
	{
		DWORD temp[2048];
		for(int j = 0;j < textID.ih;j++)
		{
			memcpy(temp,&pdwBuf[j * textID.iw],textID.iw * FONT_PERPIXEL);
			for(int i = 1;i < textID.iw;i++)
			{
				if(!(pdwBuf[j * textID.iw + i] & 0xFF000000) && (pdwBuf[j * textID.iw + i - 1] & 0xFF000000))
					temp[i] = pdwBuf[j * textID.iw + i - 1];
			}
			memcpy(&pdwBuf[j * textID.iw],temp,textID.iw * FONT_PERPIXEL);
		}
	}

	// 黑边框
	if((textID.dwFlag & DTF_BlackFrame) && textID.dwFrameColor !=  textID.dwColor)
	{
		DWORD dwFrameColor = textID.dwFrameColor;
		for(int j = 0;j < textID.ih;j++)
		{
			for(int i = 0;i < textID.iw;i++)
			{
				if((pdwBuf[j * textID.iw + i] & 0xFF000000) && (pdwBuf[j * textID.iw + i] | 0xFF000000) == (dwColor | 0xFF000000))
				{
					if(j > 0 && !(pdwBuf[(j - 1) * textID.iw + i] & 0xFF000000))
						pdwBuf[(j - 1) * textID.iw + i] = dwFrameColor;

					if(i > 0 && !(pdwBuf[j * textID.iw + (i - 1)] & 0xFF000000))
						pdwBuf[j * textID.iw + (i - 1)] = dwFrameColor;

					if(i < (textID.iw - 1) && !(pdwBuf[j * textID.iw + (i + 1)] & 0xFF000000))
						pdwBuf[j * textID.iw + (i + 1)] = dwFrameColor;

					if(j < (textID.ih - 1) && !(pdwBuf[(j + 1) * textID.iw + i] & 0xFF000000))
						pdwBuf[(j + 1) * textID.iw + i] = dwFrameColor;
				}
			}
		}
	}

	// 背景色
	if(textID.dwBkColor)
	{
		DWORD dwBackColor = textID.dwBkColor;
		for(int j = 0;j < textID.ih;j++)
		{
			for(int i = 0;i < textID.iw;i++)
			{
				if(!(pdwBuf[j * textID.iw + i] & 0xFF000000))
					pdwBuf[j * textID.iw + i] = dwBackColor;
			}
		}
	}

	textID.pTex = new CTextureD3D;
	if(!textID.pTex)
	{
		return FALSE;
	}

	if(textID.pTex->CreateFontTex(textID.iw,textID.ih,s_Buf) == FALSE)
	{
		SAFE_DELETE(textID.pTex);
		return FALSE;
	}

	TRY_END;

	return TRUE;
}

BOOL CFontD3D::GetTextTexFreeType(stTextID& textID,LPCTSTR str)
{
	TRY_BEGIN;

	BYTE c1,c2;
	int len = (int)strlen(str);

	int iW = textID.iFontSize,iH = textID.iFontSize;
	m_pFreeType->SetCurFontSize(textID.iFontSize,&iW,&iH,&(textID.iBaseLine));

	textID.dwCount	= g_pEGfx->GetFrameCount();
	textID.iw		= GetLen(str,NULL,textID.iFontSize) + 2;
	textID.ih		= iH + 1;


	if(textID.iw * textID.ih * FONT_PERPIXEL > NML_POOL_SIZE)
		return FALSE;

	memset(s_Buf,0,textID.iw * textID.ih * FONT_PERPIXEL);
	DWORD *pdwBuf = (DWORD *)s_Buf;


	// 填充A4R4G4B4像素数据
	WORD	wTemp;
	int		iw = 1,iCount = 0;
	DWORD	dwColor = textID.dwColor;
	for(int i = 0;i < len;i++)
	{
		c1 = (BYTE)str[i];
		c2 = (BYTE)str[i + 1];

	    if(c1== 0xF8 && c2>=0xA1 && c2<=0xFE) //笑脸符号
		{
			if(m_vFace != NULL && m_iFace > 0)
			{
				c2 -= 0xA1;
				if( c2 >= m_iFace)
					c2 = m_iFace-1;

				WORD* pwFace = &m_vFace[c2*144];
				DWORD dwFace = 0;

				for(int h = 0;h < 12;h++)
				{
					for(int w = 0;w < 12;w++)
					{
						dwFace = *pwFace;
						if(dwFace & 0xF000)
						{
							pdwBuf[h * textID.iw + iw + w] = 0xFF000000 | ((dwFace & 0x00000F00) << 12) | ((dwFace & 0x000000F0) << 8) | ((dwFace & 0x0000000F) << 4);
						}
						pwFace++;
					}
				}
			}
			iw += 12;
			i++;
		}
		else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
		{
			MultiByteToWideChar(CP_ACP,0,str + i,2,(LPWSTR)&wTemp,1);
			GetWordPixel(textID,wTemp,iw,pdwBuf,dwColor);
			i++;
		}
		else
		{
			if(c1 > 32 && c1 < 128)
			{
				//int iOldFont = m_iCurFontType;
				//if (m_iCurFontType != FONT_SONGTI)
				//{
				//	SetFont(FONT_SONGTI);
				//	m_pFreeType->SetCurFontSize(textID.iFontSize);
				//}				
				
				MultiByteToWideChar(CP_ACP,0,str + i,1,(LPWSTR)&wTemp,1);
				GetWordPixel(textID,wTemp,iw,pdwBuf,dwColor);

				//if (m_iCurFontType != iOldFont)
				//{
				//	SetFont(iOldFont);
				//	m_pFreeType->SetCurFontSize(textID.iFontSize);
				//}
			}
			else
			{
				iw += textID.iFontSize / 2;
			}
		}
	}

	// 下划线
	if(textID.dwFlag & DTF_UnderLine)
	{
		for(int i = 0;i < textID.iw;i++)
			pdwBuf[(textID.ih - 1) * textID.iw + i] = dwColor;
	}

	// 粗体
	if(textID.dwFlag & DTF_Bold)
	{
		DWORD temp[2048];
		for(int j = 0;j < textID.ih;j++)
		{
			memcpy(temp,&pdwBuf[j * textID.iw],textID.iw * FONT_PERPIXEL);
			for(int i = 1;i < textID.iw;i++)
			{
				if(!(pdwBuf[j * textID.iw + i] & 0xFF000000) && (pdwBuf[j * textID.iw + i - 1] & 0xFF000000))
					temp[i] = pdwBuf[j * textID.iw + i - 1];
			}
			memcpy(&pdwBuf[j * textID.iw],temp,textID.iw * FONT_PERPIXEL);
		}
	}

	// 黑边框
	if((textID.dwFlag & DTF_BlackFrame) && textID.dwFrameColor !=  textID.dwColor)
	{
		DWORD dwFrameColor = textID.dwFrameColor;
		for(int j = 0;j < textID.ih;j++)
		{
			for(int i = 0;i < textID.iw;i++)
			{
				if((pdwBuf[j * textID.iw + i] & 0xFF000000) && (pdwBuf[j * textID.iw + i] | 0xFF000000) == (dwColor | 0xFF000000))
				{
					if(j > 0 && !(pdwBuf[(j - 1) * textID.iw + i] & 0xFF000000))
						pdwBuf[(j - 1) * textID.iw + i] = dwFrameColor;

					if(i > 0 && !(pdwBuf[j * textID.iw + (i - 1)] & 0xFF000000))
						pdwBuf[j * textID.iw + (i - 1)] = dwFrameColor;

					if(i < (textID.iw - 1) && !(pdwBuf[j * textID.iw + (i + 1)] & 0xFF000000))
						pdwBuf[j * textID.iw + (i + 1)] = dwFrameColor;

					if(j < (textID.ih - 1) && !(pdwBuf[(j + 1) * textID.iw + i] & 0xFF000000))
						pdwBuf[(j + 1) * textID.iw + i] = dwFrameColor;
				}
			}
		}
	}

	// 背景色
	if(textID.dwBkColor)
	{
		DWORD dwBackColor = textID.dwBkColor;
		for(int j = 0;j < textID.ih;j++)
		{
			for(int i = 0;i < textID.iw;i++)
			{
				//if(!(pdwBuf[j * textID.iw + i] & 0xFF000000))
				if((pdwBuf[j * textID.iw + i] & 0xFF000000) < 0x70000000)
					pdwBuf[j * textID.iw + i] = dwBackColor;
			}
		}
	}

	textID.pTex = new CTextureD3D;
	if(!textID.pTex)
	{
		return FALSE;
	}

	if(textID.pTex->CreateFontTex(textID.iw,textID.ih,s_Buf) == FALSE)
	{
		SAFE_DELETE(textID.pTex);
		return FALSE;
	}

	TRY_END;

	return TRUE;
}

// 使用GDI函数来显示文字，效率比较差，但是可以使用任意的windows字体
// 虽然理论上用GDI字体速度应该比用点阵字库慢，但是实际测试发现速度差别很小，这说明Cache结构做的是成功的，在极端情况下，速度能提高九倍左右!
BOOL CFontD3D::GetTextTexGDI(stTextID& textID,LPCTSTR str)
{
	int i,j,y,x,w,h;
	DWORD *pBuf, *pBits;
	DWORD dwColor,dwBackColor;
	HBITMAP hBmp;
	RECT rc;
	bool bNewBuf;

	int iLen = (int)strlen(str);

	textID.dwCount	= g_pEGfx->GetFrameCount();

	// 当前字体是否改变，如果改变要产生新的字体
	if(textID.iFontCRC != m_iLastGUIFont || textID.iFontSize != m_iLastFontSize)
	{
		if(m_hFont) DeleteObject((HGDIOBJ)m_hFont);
		m_sFontLib[textID.iFontCRC].sLogFont.lfHeight = -textID.iFontSize;
		m_hFont = CreateFontIndirect( &(m_sFontLib[textID.iFontCRC].sLogFont) );
		m_iLastGUIFont = textID.iFontCRC;
		m_iLastFontSize = textID.iFontSize;
		if(m_hFont == NULL)
			return GetTextTex(textID,str);
	}
	SelectObject(m_hDC,m_hFont);


	//当前字体为非固定长度，必须用GDI函数来得到此行文字的宽度
	SIZE sSize;
	GetTextExtentPoint(m_hDC,str,iLen,&sSize);
	w = sSize.cx;
	w = min(w,1024);
	if( w % 2 ) w++;
	h	= textID.iFontSize;
	dwColor		= textID.dwColor;
	dwBackColor	= textID.dwBkColor;

	int iNeedBufLen = (w + 2) * h * 2;
	if(iNeedBufLen <= NML_POOL_SIZE)
	{
		memset(s_Buf,0,iNeedBufLen);
		pBuf = (DWORD *)s_Buf;
		bNewBuf = false;
	}
	else
	{
		pBuf = new DWORD[(w+2)*h];
		if(!pBuf)
			return FALSE;

		bNewBuf = true;
	}

	// 创建HBITMAP
	m_sBmi.bmiHeader.biWidth	= w;
	m_sBmi.bmiHeader.biHeight	= h;
	hBmp = CreateDIBSection(m_hDC, &m_sBmi, DIB_RGB_COLORS,(void **)&pBits, NULL, 0);
	SelectObject(m_hDC,hBmp);

	// 输出文字
	rc.left	  = 0;
	rc.top	  = 0;
	rc.right  = w - 1;
	rc.bottom = h - 1;
	FillRect(m_hDC,&rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
	TextOut(m_hDC,0,0,str,iLen);

	// 多出来的边框清零
	w += 2;
	j = (h-1) * w;
	for(x = 0;x < w; x++)
	{
		pBuf[x]		= dwBackColor;
		pBuf[j+x]	= dwBackColor;
	}
	for(y = 0;y < h;y ++)
	{
		pBuf[y*w]		= dwBackColor;
		pBuf[y*w+(w-1)]	= dwBackColor;
	}
	w-=2;

	// 把文字从HBITMAP复制到CTexture中
	for(y=0;y<h;y++) 
	{
		i = (h-y-1) * w;
		for(x=0;x<w;x++)
		{
			if( pBits[i+x] ) 
				pBuf[y*(w+2)+(x+1)] = dwColor; 
			else 
				pBuf[y*(w+2)+(x+1)] = dwBackColor;
		}
	}
	w += 2;

	// 加边框
	if(textID.dwFlag & DTF_BlackFrame)
	{
		DWORD dwFrameColor = textID.dwFrameColor;
		for(y=1;y<(h-1);y++)
		{
			j = y*w;
			for(x=1;x<(w-1);x++)
			{
				if( pBuf[j+x] == dwColor )
				{
					if( pBuf[j+x-w] == dwBackColor )
						pBuf[j+x-w] = dwFrameColor;
					if( pBuf[j+x+w] == dwBackColor )
						pBuf[j+x+w] = dwFrameColor;
					if( pBuf[j+x-1] == dwBackColor )
						pBuf[j+x-1] = dwFrameColor;
					if( pBuf[j+x+1] == dwBackColor )
						pBuf[j+x+1] = dwFrameColor;
				}
			}
		}
	}


	textID.pTex = new CTextureD3D;
	if(!textID.pTex)
	{
		return FALSE;
	}

	textID.iw = w;
	textID.ih = h;

	if(textID.pTex->CreateFontTex(w,h,s_Buf) == FALSE)
	{
		SAFE_DELETE(textID.pTex);
		return FALSE;
	}

	DeleteObject(hBmp);
	if(bNewBuf) 
		SAFE_DELETE_ARRAY(pBuf);

	return true;
}


// 取得当前字体下字符串的宽度
int CFontD3D::GetLen(LPCTSTR p0,LPCTSTR p1,int iFontSize)
{
	BYTE c1,c2;
	WORD wTemp = 0;
	BitmapInfo	*pBitmapinfo = NULL;

	try
	{
		//int i = 0;
		//while(*p0)
		//{
		//	if(p1 && p0 >= p1)
		//		break;

		//	c1 = (BYTE)*p0;
		//	c2 = (BYTE)*(p0 + 1);

		//	if(c1 == DICON_BEGIN &&  c2 < 200)
		//	{
		//		i += 16;
		//		p0++;
		//	}
		//	else if(c1== 0xF8 && c2>=0xA1 && c2<=0xFE)
		//	{
		//		i += 12;
		//		p0++;
		//	}
		//	else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
		//	{
		//		MultiByteToWideChar(CP_ACP,0,p0,2,(LPWSTR)&wTemp,1);

		//		m_FreeType.GetWord(&pBitmapinfo,wTemp,iFontSize);
		//		if(pBitmapinfo)
		//			i += pBitmapinfo->x + pBitmapinfo->w;
		//		else
		//			i += iFontSize;
		//		p0++;
		//	}
		//	else 
		//	{
		//		if(c1 > 32 && c1 < 128)
		//		{
		//			if(iFontSize == 12)
		//				i += 6;
		//			else
		//			{
		//				MultiByteToWideChar(CP_ACP,0,p0,1,(LPWSTR)&wTemp,1);
		//				m_FreeType.GetWord(&pBitmapinfo,wTemp,iFontSize);
		//				if(pBitmapinfo)
		//					i += pBitmapinfo->x + pBitmapinfo->w;
		//				else
		//					i += iFontSize / 2;
		//			}

		//		}
		//		else
		//			i += iFontSize / 2;
		//	}
		//	p0++;
		//}
		//return i;


		int i = 0;
		while(*p0)
		{
			if(p1 && p0 >= p1)
				break;

			c1 = (BYTE)*p0;
			c2 = (BYTE)*(p0 + 1);

			if(c1==0xF8 && c2>=0xA1 && c2<=0xFE )
			{
				i += 12;
				p0++;
			}
			else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
			{
				i += iFontSize;
				p0++;
			}
			else 
			{
				i += iFontSize / 2;
			}
			p0++;
		}

		return i;
	}
	catch(...)
	{
		return 0;
	}
}

// 一般的文字绘制
void CFontD3D::DrawText(int x,int y,LPCTSTR str,DWORD dwColor,int iFontType,int iFontSize,DWORD dwFlag,DWORD dwBkColor,DWORD dwFrameColor)
{
	if(g_pEGfx->IsNoDraw() || !str || iFontType < 0 || iFontSize > 128 || iFontSize <= 0 || iFontSize % 2)
		return;

	if(dwFlag & DTF_Center)
	{
		x -= GetLen(str,NULL,iFontSize) / 2;
	} 

	if(!SetFont(iFontType))
	{
		if (iFontType < FONT_FREETYPE_NUM && iFontSize > FONTSIZE_BIG)//设置失败默认用宋体点阵,最大16号
		{
			iFontSize = FONTSIZE_BIG;
		}
	}


	stTextID text;

	text.iFontCRC	= m_iCurFontType;//m_FreeType.GetFont();
	text.iTextCRC	= g_CRC32.Get_CRC((BYTE *)str,(ULONG)strlen(str));
	text.dwColor	= dwColor;
	text.dwFrameColor = dwFrameColor;
	text.iFontSize	= iFontSize;
	text.dwFlag		= dwFlag;
	text.dwBkColor	= dwBkColor;

	stList *pList = ListFind(text);
	if(pList)
		ListMoveHead(pList);
	else if(GetTextTex(text,str))
		pList = ListAdd(text);

	if(pList && pList->pTex)
	{
		dwColor |= 0x00FFFFFF;

		if(dwFlag & DTF_Shadow)//带阴影
			g_pEGfx->DrawTextureNL(x + 1,y + 1 - pList->iBaseLine,pList->pTex,0xFF000000);

		g_pEGfx->DrawTextureNL(x,y - pList->iBaseLine,pList->pTex,dwColor);
	}
}

void CFontD3D::DrawTextEx(int x,int y,int iDLX,int iDRX,LPCTSTR str,DWORD dwColor,int iFontType,int iFontSize,DWORD dwFlag,DWORD dwBkColor,DWORD dwFrameColor)
{
	if(g_pEGfx->IsNoDraw() || !str || iFontType < 0 || iFontSize > 64 || iFontSize <= 0 || iFontSize % 2)
		return;

	//if (strlen(str) * iFontSize/2 + x < iDLX ||
	//	x > iDRX)
		//return;

	if(!SetFont(iFontType))
	{
		if (iFontSize > FONTSIZE_BIG)//设置失败默认用宋体点阵,最大16号
		{
			iFontSize = FONTSIZE_BIG;
		}
	}

	stTextID text;

	text.iFontCRC	= m_iCurFontType;
	text.iTextCRC	= g_CRC32.Get_CRC((BYTE *)str,(ULONG)strlen(str));
	text.dwColor	= dwColor;
	text.dwFrameColor = dwFrameColor;
	text.iFontSize	= iFontSize;
	text.dwFlag		= dwFlag;
	text.dwBkColor	= dwBkColor;

	stList *pList = ListFind(text);
	if(pList)
		ListMoveHead(pList);
	else if(GetTextTex(text,str))
		pList = ListAdd(text);

	if(pList && pList->pTex)
	{
		dwColor |= 0x00FFFFFF;
		int iLeft = iDLX - x,iTop = 0,iRight = iDRX - x,iBottom = y + iFontSize;

		if(dwFlag & DTF_Shadow)//带阴影
			g_pEGfx->DrawPartTexture(x + 1,y + 1 - pList->iBaseLine,pList->pTex,iLeft,iTop,iRight,iBottom,0xFF000000);

		g_pEGfx->DrawPartTexture(x,y - pList->iBaseLine,pList->pTex,iLeft,iTop,iRight,iBottom,dwColor);
	}
}

// 绘制部分文字
void CFontD3D::DrawTextPart(int x,int y,LPCTSTR str,DWORD dwColor,int iOffH,int iFontType,int iFontSize,DWORD dwFlag,DWORD dwBkColor,DWORD dwFrameColor)
{
	if(g_pEGfx->IsNoDraw() || !str || iFontSize > 64 || iFontSize % 2)
		return;

	if(dwFlag & DTF_Center)
	{
		x -= GetLen(str,NULL,iFontSize) / 2;
	}

	if(!SetFont(iFontType))
	{
		if (iFontSize > FONTSIZE_BIG)//设置失败默认用宋体点阵,最大16号
		{
			iFontSize = FONTSIZE_BIG;
		}
	}

	stTextID text;

	text.iFontCRC	= m_iCurFontType;//m_FreeType.GetFont();
	text.iTextCRC	= g_CRC32.Get_CRC((BYTE *)str,(ULONG)strlen(str));
	text.dwColor	= dwColor;
	text.dwFrameColor = dwFrameColor;
	text.iFontSize	= iFontSize;
	text.dwFlag		= dwFlag;
	text.dwBkColor	= dwBkColor;

	stList *pList = ListFind(text);
	if(pList)
		ListMoveHead(pList);
	else if(GetTextTex(text,str))
		pList = ListAdd(text);

	if(pList && pList->pTex)
	{
		RECT	rc;
		int		iMaxH = (iFontSize < 16) ? 16 : iFontSize;

		rc.left		= 0;
		rc.right	= pList->iw;

		if(iOffH >= iMaxH || iOffH <= -iMaxH)
			iOffH = 0;

		if(iOffH >= 0)
		{
			rc.top		= iOffH + (iFontSize - iMaxH) / 2;
			rc.bottom	= pList->ih;
		}
		else
		{
			rc.top		= 0;
			rc.bottom	= iMaxH + iOffH + (iFontSize - iMaxH) / 2;
		}

		if(rc.top < 0)
			rc.top = 0;
		if(rc.bottom > pList->ih)
			rc.bottom = pList->ih;

		dwColor |= 0x00FFFFFF;
		g_pEGfx->DrawTextureFX(x,y - pList->iBaseLine,pList->pTex,dwColor,&rc);
	}
}


////////绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色,可以拉伸，旋转
void CFontD3D::DrawText_Trans(int x,int y,LPCTSTR str,DWORD dwColor,int iFontType,int iFontSize,DWORD dwFlag,float fScaleX,float fScaleY,float fYTrans,DWORD dwBackColor,DWORD dwFrameColor)
{
	if(g_pEGfx->IsNoDraw() || !str || iFontSize > 64 || iFontSize % 2)
		return;

	if(dwFlag & DTF_Center)
	{
		x -= GetLen(str,NULL,iFontSize) / 2;
	}

	if(!SetFont(iFontType))
	{
		if (iFontSize > FONTSIZE_BIG)//设置失败默认用宋体点阵,最大16号
		{
			iFontSize = FONTSIZE_BIG;
		}
	}

	stTextID text;

	text.iFontCRC	= m_iCurFontType;//m_FreeType.GetFont();
	text.iTextCRC	= g_CRC32.Get_CRC((BYTE *)str,(ULONG)strlen(str));
	text.dwColor	= dwColor;
	text.dwFrameColor = dwFrameColor;
	text.iFontSize	= iFontSize;
	text.dwFlag		= dwFlag;
	text.dwBkColor	= dwBackColor;

	stList *pList = ListFind(text);
	if(pList)
		ListMoveHead(pList);
	else if(GetTextTex(text,str))
		pList = ListAdd(text);

	if(pList && pList->pTex)
	{
		dwColor |= 0x00FFFFFF;

		if(dwFlag & DTF_Shadow)//带阴影
			g_pEGfx->DrawTextureNL_Trans(x + 1,y + 1 - pList->iBaseLine,pList->pTex,fScaleX,fScaleY,fYTrans,0xFF000000);

		g_pEGfx->DrawTextureNL_Trans(x,y - pList->iBaseLine,pList->pTex,fScaleX,fScaleY,fYTrans);
	}
}
// 绘制滚动文字
// iOff：像素偏移
// iDisArea：区域间的像素间隔
// iDisLine：每行间的象素间隔
int CFontD3D::DrawTextScroll(int x,int y,LPCTSTR str,DWORD dwColor,int iw,int ih,UINT iOff,int iDisArea,int iDisLine,int iFontType,int iFontSize,DWORD dwFlag,DWORD dwBkColor,DWORD dwFrameColor)
{
	if(g_pEGfx->IsNoDraw() || !str || iw == 0 || ih <= 0)
		return 0;

	if(!SetFont(iFontType))
	{
		if (iFontSize > FONTSIZE_BIG)//设置失败默认用宋体点阵,最大16号
		{
			iFontSize = FONTSIZE_BIG;
		}
	}

	int iRet = 0; 
	vector<string>	vStr;

	// 分解出每行
	int		i,j;
	int		w = 0;
	const char*	p	= NULL;
	const char*	p0	= str;
	const char*	p1	= str;

	while(true)
	{
		p = CharNext(p1);
		i = (int)(p - p1);

		// 是否是字符串结束
		if(i > 2 || i <= 0)
			goto ADDTEXT;

		// 是否是硬换行
		if(i == 1 && (*p1 == 0x0D || *p1 == 0x0A))
		{
			if(*p1 == 0x0D && *(p1 + 1) == 0x0A)
				p++;
			goto ADDTEXT;
		}

		j = w + i * iFontSize / 2;

		// 是否需要软换行
		if(j > iw)
		{
			p		= p1;
			goto ADDTEXT;
		}
		else
			w = j; 

		p1	= p;
		continue;

ADDTEXT:
		if(p1 > p0)
			vStr.push_back(std::string(p0,p1 - p0));

		if(i > 2 || i <= 0)		// 字符串结束
			break;

		p1		= p;
		p0		= p;
		w		= 0;
	}

	// 绘制每一行
	int	iMaxH	= (iFontSize < 16) ? 16 : iFontSize;
	int iLineH	= iMaxH + iDisLine;
	int	h		= (int)vStr.size() * iLineH;			// 文字的实际高度

	if(h <= ih)				// 不需要滚动
	{
		for(UINT c = 0; c < vStr.size(); c++)
			DrawText(x,y + c * iLineH,vStr[c].c_str(),dwColor,iFontType,iFontSize,dwFlag,dwBkColor,dwFrameColor);
	}
	else					// 需要滚动
	{
		int	idx,oy,off = iOff % (h + iDisArea);		// 相对偏移

		if(off >= h)
		{
			idx = 0;
			oy	= h + iDisArea - off;
		}
		else
		{
			idx	= off / iLineH;
			oy  = -(off % iLineH);
		}

		while(true)
		{
			if((oy + iLineH) > ih)		// 底部的一行
			{
				if((oy + iMaxH) > ih)
                    DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,ih - oy - iMaxH,iFontType,iFontSize,dwFlag,dwBkColor,dwFrameColor);
				else
					DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,0,iFontType,iFontSize,dwFlag,dwBkColor,dwFrameColor);
				break;
			}
			else if(oy <= 0)					// 顶端的一行
			{
				if((off % iLineH) < iMaxH)
					DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,off % iLineH,iFontType,iFontSize,dwFlag,dwBkColor,dwFrameColor);
			}
			else								// 中间的一行
				DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,0,iFontType,iFontSize,dwFlag,dwBkColor,dwFrameColor);

			idx++;
			oy += iLineH;
			if(idx >= (int)vStr.size())
			{
				oy += iDisArea;
				idx = 0;
			}

			if(oy >= ih)
				break;
		}
		if(idx == 0)
			iRet = 0;
		else
			iRet = off;
	}

	return iRet;
}

void CFontD3D::SetFontConfig(float fWeight,float fContrast,float fGamma,int iMode)
{
	s_AlphaBlendTable.Init(fWeight,fContrast,fGamma,iMode);
}
