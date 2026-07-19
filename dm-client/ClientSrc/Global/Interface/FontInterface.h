#pragma once

#define SICON_BEGIN						0xFE				// 静态表情字体开头字节

#define FONT_SONGTI_12_ASC_SIZE	(12 * 6 * 96 / 8)
#define FONT_SONGTI_12_FNT_SIZE	(2*12 * 126*191)
#define FONT_SONGTI_14_ASC_SIZE	(2*7*14 * 95)
#define FONT_SONGTI_14_FNT_SIZE	(2*14 * 126*191)
#define FONT_SONGTI_16_ASC_SIZE	(2*8*16 * 95)
#define FONT_SONGTI_16_FNT_SIZE	(2*16 * 126*191)


//各种文字的颜色
#define COLOR_TEXT_NORMAL  		    0xFFB48C5A//正文
#define COLOR_TEXT_MAIN_TITLE	    0xFFF8C993//窗口主标题
#define COLOR_TEXT_SBU_TITLE	    0xFFB46428//二级标题
#define COLOR_TEXT_DISABLE	        0xFF646464//灰色
#define COLOR_TEXT_NUMBER	        0xFF5AA0A0//数字的颜色
//普通按钮各态的颜色
#define COLOR_BTN_NORMAL	        0xFFF0AF64
#define COLOR_BTN_MOUSEON	        0xFFFAC896
#define COLOR_BTN_PRESS	            0xFF5AA0A0
#define COLOR_BTN_DISABLE	        0xFF646464
//弹出框按钮各态的颜色
#define COLOR_POPUP_BTN_NORMAL	    0xFFF1AF65
#define COLOR_POPUP_BTN_MOUSEON	    0xFFFAC897
#define COLOR_POPUP_BTN_PRESS	    0xFF5AA0A0
#define COLOR_POPUP_BTN_DISABLE	    0xFF646464
#define COLOR_POPUP_TEXT	        0xFFB48C5A//弹出框正文
#define COLOR_ALERT_TEXT	        0xFFB46428//警告框正文
//页签按钮的颜色
#define COLOR_TABPAGE_NORMAL        0xFF9E6D34
#define COLOR_TABPAGE_MOUSEON       0xFFFAC896
#define COLOR_TABPAGE_PRESS	        0xFFF0AF64
#define COLOR_TABPAGE_DISABLE       0xFF646464

//tips颜色
enum ADDTEXTCOLOR
{
	TIPS_GOLDEN = 0xFFFFFF00,  //金黄色
	TIPS_RED = 0xFFFF0000,         
	TIPS_GREEN = 0xFF00FF00,
	TIPS_WHITE = 0xFFFFFFFF,
	TIPS_BLUE = 0xFF0000FF,
	TIPS_KHAKI = 0xFFFFCF63,        //土黄色
	TIPS_GOODNAME = 0xFFFDCA66,     
};


// 画字时的标识
enum eDrawTextFlag
{
	DTF_UnderLine	= 0x00000001,	// 下划线
	DTF_Bold		= 0x00000002,	// 粗体
	DTF_BlackFrame	= 0x00000004,	// 黑边框
	DTF_Center      = 0x00000008,   // 居中
	DTF_Shadow      = 0x00000010,   // 阴影
};

enum eFont_Type
{
	FONT_DEFAULT         =      0,//系统默认字体
	FONT_SONGTI          =      0,//宋体
	FONT_LISHU           =      1,//隶书
	FONT_YAHEI           =      2,//微软雅黑
	FONT_FREETYPE_NUM,            //freetype支持的字体总数

	FONT_GDI_XINSONG     =   	4,// 新宋体
	FONT_GDI_KAITI		 =		5,// 楷体
	FONT_GDI_HEITI		 =		6,// 黑体
	FONT_GDI_FANGSONG	 =		7,// 仿宋体
	FONT_GDI_SONG_XIE	 =		8,// 新宋斜体
	FONT_GDI_SONG_CU	 =		9,// 新宋粗体
	FONT_GDI_YOUYUAN	 =		10,// 幼圆
	FONT_GDI_LISHU		 =		11,// 隶书
	FONT_GDI_SHUTI		 =		12,// 舒体
	FONT_GDI_TAOTI		 =		13,// 姚体
	FONT_GDI_CAIYUN		 =		14,// 彩云
	FONT_GDI_XIHEI		 =		15,// 细黑
	FONT_GDI_XINWEI		 =		16,// 新魏
	FONT_GDI_HANGKAI	 =		17,// 行楷
	FONT_GDI_ZHONGSONG	 =		18,// 中宋

	FONT_NUM             =      19,//支持的字体总数
};

enum eFont_Size
{
	FONTSIZE_DEFAULT = 12,
	FONTSIZE_SMALL = 12,
	FONTSIZE_MIDDLE = 14,
	FONTSIZE_BIG   = 16
};

class CFixPool;

class CFontInterface
{
public:	
	virtual ~CFontInterface(){}

	virtual void  SetFontConfig(float fWeight = 1.5f,float fContrast = 1.5f,float fGamma = 1.4f,int iMode = 0) = 0;
	virtual void  ClearCache() = 0;//清除文字绘制纹理缓冲

	// 取得字符串像素长度
	virtual int		GetLen(LPCTSTR p0,LPCTSTR p1 = NULL,int iFontSize = FONTSIZE_DEFAULT) = 0;

	// 绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色
	virtual void	DrawText(int x,int y,LPCTSTR str,DWORD dwColor,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000) = 0;
	virtual void	DrawTextEx(int x,int y,int iDLX,int iDRX,LPCTSTR str,DWORD dwColor,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000) = 0;
	// 绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色,可以拉伸，旋转
	virtual void    DrawText_Trans(int x,int y,LPCTSTR str,DWORD dwColor,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,float fScaleX = 1.0,float fScaleY = 1.0,float fYTrans = 0,DWORD dwBackColor = 0,DWORD dwFrameColor = 0xFF000000) = 0;

	// 绘制部分文字
	virtual void	DrawTextPart(int x,int y,LPCTSTR str,DWORD dwColor,int iOffH,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000) = 0;

	// 绘制滚动文字
	// iOff：像素偏移
	// iDisArea：区域间的像素间隔
	// iDisLine：每行间的象素间隔
	virtual int		DrawTextScroll(int x,int y,LPCTSTR str,DWORD dwColor,int iw,int ih,UINT iOff = 0,int iDisArea = 16,int iDisLine = 0,int iFontType = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwBkColor = 0,DWORD dwFrameColor = 0xFF000000) = 0;

};

extern CFontInterface *      g_pFont;
