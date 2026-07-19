#pragma once

#include <windows.h>
#include <vector>
#include "Global/Interface/FreeTypeInterface.h"

class CFreeType : public CFreeTypeInterface
{
public:
	CFreeType(void);
	~CFreeType(void);

	int  	GetFont(void);
	bool 	SetFont(int iFont);
	bool	GetWord(BitmapInfo **pBI,WCHAR wChar);
	bool    AddFont(const char* path);
	bool    SetCurFontSize(int iFontSize = 12,int *pCharWidth = NULL,int *pCharHeight = NULL,int *pBaseLine = NULL);

private:
	//变量定义
	std::vector<void*>  m_Fonts; //字体的数据
	int     m_iCurFont;
};