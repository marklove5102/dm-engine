#pragma once
#include "Global/Global.h"
#include "Global/StdHeaders.h"
#include "Global/Interface/FontInterface.h"


//tips的列
struct sTipsCfgCol
{
	sTipsCfgCol(string content = "",int font = FONT_DEFAULT,int fontsize = FONTSIZE_DEFAULT,DWORD color = TIPS_KHAKI,DWORD flag = 0,DWORD backcolor = 0,DWORD framecolor = 0xFF000000,int ix = -1,int iy = -1,int offx = 0,int offy = 0,int picpackage = 0,int picidx = 0,bool autocutbyleftwidth = false,int type = 0)
	{
		strContent = content;
		iFont = font;
		iFontSize = fontsize;
		dwColor = color;
		dwBackColor = backcolor;
		dwFrameColor = framecolor;
		dwFlag = flag;
		iX = ix; iY = iy;
		iOffX = offx;iOffY = offy;iPicPackage = picpackage;iPicIdx = picidx;
		bAutoCutByLeftWidth = autocutbyleftwidth;
		iType = type;
	}

	int iFont;//字体
	int iFontSize;//字号
	DWORD dwColor;//颜色
	DWORD dwBackColor;//背景颜色
	DWORD dwFrameColor;//描边颜色
	DWORD dwFlag;//文字绘制标记,eDrawTextFlag之一
	int iX,iY;
	int iOffX,iOffY;//偏移
	int iPicPackage,iPicIdx;//图片位置
	bool bAutoCutByLeftWidth;//true表示自动换行时按该行还剩下的宽度计算,否则以最大宽度计算
	int  iType;//类型,用于特殊处理,1:技能用,表示该技能要求的前置技能;
	string strContent;//内容
};
//tips的行
struct sTipsCfgLine
{
	vector<sTipsCfgCol> VCols;
};

//tips
struct sTipsCfg
{
	sTipsCfg()
	{
		iMaxWidth = 0;
	}

	int iMaxWidth;//最大宽度
	vector<sTipsCfgLine> VLines;
};


typedef map<string,sTipsCfg> MTipsCfg;

class CTipsCfg
{
public:
	CTipsCfg(void);
	~CTipsCfg(void);

public:
	sTipsCfg * GetCfgTips(const char * szName);
	sTipsCfg * GetSkillTips(const char * szName);
	sTipsCfg * GetOtherTips(const char * szName);
	bool 	ReLoadGoodsTipsCfg();
	bool 	ReLoadSkillTipsCfg();
	bool 	ReLoadOtherTipsCfg();




protected:
	MTipsCfg m_MGoodsTipsCfg;
	MTipsCfg m_MSkillTipsCfg;
	MTipsCfg m_MOtherTipsCfg;

	bool 	ReLoadTipsCfg(const char* szXmlPath,MTipsCfg &mtipsCfg);
};

extern CTipsCfg g_TipsCfg;
