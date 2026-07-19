#pragma once

#include "Global/Interface/FreeTypeInterface.h"
#include "FixPool.h"
#include "TextureD3D.h"
#include "Global/Interface/FontInterface.h"

class CFontD3D : public CFontInterface
{
	friend class CTextureD3D;

	struct stExpression
	{
		int	iPos;
		int iIdex;
	};

	struct stTextID
	{
		int		iFontCRC;
		int		iTextCRC;
		DWORD	dwColor;
		DWORD	dwBkColor;
		DWORD   dwFrameColor;
		int		iFontSize;
		DWORD	dwFlag;

		DWORD			dwCount;
		int				iw;
		int				ih;
		int             iBaseLine;
		LPTexture		pTex;

		stTextID():iFontCRC(0),iTextCRC(0),dwColor(-1),dwBkColor(0),dwFrameColor(0),iFontSize(0),dwFlag(0),
			dwCount(0),iw(0),ih(0),iBaseLine(0),pTex(NULL)
		{
		}
	};

	struct stList:public stTextID
	{
		stList *Prev;
		stList *Next;

		stList():Prev(NULL),Next(NULL)
		{
		}
	};


	struct SFontLib 
	{
		int	width,height;  // 单位宽度和高度，这里的宽度指一个汉字的宽度
		LOGFONT	sLogFont;  // 字体属性结构
	};	

private:
	stList* ListAdd(stTextID& textID);
	stList* ListFind(stTextID& textID);
	void ListDel(stList *pList);
	void ListMoveHead(stList *pList);
	void ListWash(DWORD dwCount,DWORD dwDelta);
	void ListClear(void);

	BOOL GetTextTex(stTextID& textID,LPCTSTR str);
	BOOL GetTextTexFNT(stTextID& textID,LPCTSTR str);
	BOOL GetTextTexGDI(stTextID& textID,LPCTSTR str);
	BOOL GetTextTexFreeType(stTextID& textID,LPCTSTR str);
	BOOL GetWordPixel(stTextID& textID,WORD wTemp,int& iw,DWORD* pdwBuf,DWORD dwColor);
	// 设置字体类型,eFont_Type类型之一
	bool	SetFont(int iFont = FONT_DEFAULT);

protected:
	BYTE		m_vAsc12[FONT_SONGTI_12_ASC_SIZE];
	BYTE		m_vFnt12[FONT_SONGTI_12_FNT_SIZE];
	WORD		m_vAsc14[FONT_SONGTI_14_ASC_SIZE];
	BYTE		m_vFnt14[FONT_SONGTI_14_FNT_SIZE];
	WORD		m_vAsc16[FONT_SONGTI_16_ASC_SIZE];
	BYTE		m_vFnt16[FONT_SONGTI_16_FNT_SIZE];
	WORD*       m_vFace;
	int         m_iFace;
	int         m_iCurFontType;
	CFreeTypeInterface *	m_pFreeType;

	stList*		m_pListHead;
	stList*		m_pListEnd;

	CFixPool	m_FixPool16;
	CFixPool	m_FixPool32;
	CFixPool	m_FixPool64;
	CFixPool	m_FixPool256;

	// GDI 字体
	HDC         m_hDC;			// 窗口DC
	HFONT       m_hFont;		// 字体句柄
	BITMAPINFO  m_sBmi;			// 字体图块结构
	int			m_iLastGUIFont;	// 上一次使用的GDI字体
	int         m_iLastFontSize;// 上一次使用的GDI字体的大小
	SFontLib    m_sFontLib[FONT_NUM];

public:
	CFontD3D(void);
	~CFontD3D(void);

	void    SetFontConfig(float fWeight = 1.5f,float fContrast = 1.5f,float fGamma = 1.4f,int iMode = 0);
	void    ClearCache(){ListClear();}

	// 取得字符串像素长度
	int		GetLen(LPCTSTR p0,LPCTSTR p1 = NULL,int iFontSize = 12);

	// 绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色
	void	DrawText(int x,int y,LPCTSTR str,DWORD dwColor,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000);
	void	DrawTextEx(int x,int y,int iDLX,int iDRX,LPCTSTR str,DWORD dwColor,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000);
	// 绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色,可以拉伸，旋转
	void    DrawText_Trans(int x,int y,LPCTSTR str,DWORD dwColor,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,float fScaleX = 1.0,float fScaleY = 1.0,float fYTrans = 0,DWORD dwBackColor = 0,DWORD dwFrameColor = 0xFF000000);

	// 绘制部分文字
	void	DrawTextPart(int x,int y,LPCTSTR str,DWORD dwColor,int iOffH,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000);
	// 绘制滚动文字
	// iOff：像素偏移
	// iDisArea：区域间的像素间隔
	// iDisLine：每行间的象素间隔
	int		DrawTextScroll(int x,int y,LPCTSTR str,DWORD dwColor,int iw,int ih,UINT iOff = 0,int iDisArea = 16,int iDisLine = 0,int iFontType = FONT_DEFAULT,int iFontSize = 12,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000);
};

extern CFontD3D * g_pEFont;