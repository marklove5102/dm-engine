#pragma once

struct BitmapInfo
{
	int		x,y,w,h;
	BOOL	bMono;
	int		pitch;
	BYTE *	pBuf;
};

class CFreeTypeInterface
{
public:
	virtual ~CFreeTypeInterface(){}
	virtual int	GetFont(void) = 0;
	virtual bool SetFont(int iFont) = 0;
	virtual bool GetWord(BitmapInfo **pBI,WCHAR wChar) = 0;
	virtual bool AddFont(const char* path) = 0;
	virtual bool SetCurFontSize(int iFontSize = 12,int *pCharWidth = NULL,int *pCharHeight = NULL,int *pBaseLine = NULL) = 0;
};

CFreeTypeInterface * GetFreeTypeInstance();