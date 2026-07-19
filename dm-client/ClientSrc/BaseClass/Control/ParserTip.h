#pragma once

#include "CtrlTip.h"
#include "GameData/Good.h"
#include "GameData/BoothData.h"
#include "GameData/TipsCfg.h"


class CParserTip : public CCtrlTip
{
public:
	CParserTip();
	virtual ~CParserTip();
	CParserTip * GetSelfEquipTips() const { return m_pSelfEquipTips; }
	void SetSelfEquipTips(CParserTip * val) { m_pSelfEquipTips = val; }

	void Parse(CGood& FocusGood,bool bClearFirst = true);
	void ParseWithPrice(CGood& FocusGood,GoodPrice_t& price,bool bClearFirst = true);
	void ParseWithPrice(CGood& FocusGood,bool bClearFirst = true);
	void ParseGoodInfo(CGood& FocusGood,bool bClearFirst = true);
	void ParseAttackSkillInfo(CGood& FocusGood);
	void AdjustXY(int& x,int& y);
	void ParsePetInfo(CGood& FocusGood,bool bOtherPlay = false);
	void ParserSelfEquip(CGood& FocusGood);

	virtual void Draw();
	virtual void Clear();
	void AddText(const char * str,DWORD color = 0xFFFFFFFF,int iMaxWidth = 19 * FONTSIZE_DEFAULT / 2,int iX = 0,int iY = -1,int iFont = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFontFlag = 0,bool bAddNextLinePosY = true,int iAlignType = XAL_TOPLEFT,int iOffSetX = 1,int iOffSetY = 0,bool bSaveCurLineX = true,bool bSaveCurLineY = true,int iGapX = 0,int iGapY = 3,DWORD dwBackColor = 0,DWORD dwFrameColor = 0xFF000000,bool bAutoCutByLeftWidth = false);//iX,iY为-1表示自动计算位置,否则按设定位置绘制;bAddNextLinePosY:加了这一行后是否影响下一行的位置,iOffSetXY:这一行离上行的偏移,iGapY:行间距,bSaveCurLineXY:是否保存m_iLastCurLineX,Y;bAutoCutByLeftWidth:true表示自动换行时按该行还剩下的宽度计算,否则以最大宽度计算
	//tips中加图片,dwID表示图片编号,0x0000FFFF以下的表示特殊用处:1:表示绘制线条
	void AddPic(DWORD dwID,DWORD color = 0xFFFFFFFF,int iX = 0,int iY = -1,int iWidth = -1,int iHeight = -1,bool bAddNextLinePosY = true,int iAlignType = XAL_TOPLEFT,int iOffSetX = 1,int iOffSetY = 0,bool bSaveCurLineX = true,bool bSaveCurLineY = true,int iGapX = 0,int iGapY = 3,DWORD dwFlag = 0, int rendermode = RM_ALPHA);//iX,iY为-1表示自动计算位置,否则按设定位置绘制;bAddNextLinePosY:加了这一行后是否影响下一行的位置,iOffSetXY:这一行离上行的偏移,iGapY:行间距,bSaveCurLineXY:是否保存m_iLastCurLineX,Y;
	void AddEmptyLine(bool bAddNewLine = true);
	void AddCfgTips(sTipsCfg * pTipCfg);
	void AddSkillCfgTips(sTipsCfg * pTipCfg);

	virtual bool ReSize(int iW,int iH);

protected:
	void ParseOtherInfo(CGood &FocusGood);

	CParserTip * m_pSelfEquipTips;//对照自己身上的tips
};

