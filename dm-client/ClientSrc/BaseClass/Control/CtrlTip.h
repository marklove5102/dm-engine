#pragma once

#include "Control.h"

enum E_TipsFlag
{
	ETF_DRAWPART = 0x00010000,  //绘制一部分
};


class CCtrlTip : public CControlContainer
{
	DTI_DECLARE(CCtrlTip, CControlContainer)
public:
	CCtrlTip(void);
	~CCtrlTip(void);

	virtual bool Create(CControl *pParent);
	virtual bool CreateXML(CControl* pParent,CXmlTip* tip);
	virtual void OnAutoSize();						// 文字被改变时被调用来计算窗口尺寸
	virtual void Draw();
	virtual void OnMove();
	virtual void Clear();
	int GetMaxHeight() const { return m_iMaxHeight; }
	void SetMaxHeight(int val) { m_iMaxHeight = val; }
	int GetMaxWidth() const { return m_iMaxWidth; }
	void SetMaxWidth(int val) { m_iMaxWidth = val; }

	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);	// 消息函数
	virtual void SetText(const char * sText,int iFont = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD color = 0xFFFFFFFF,DWORD dwFontFlag = 0,char cEndChar = '\n');//cEndChar换行符

	void AddText(const char * str,int iFont = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD color = 0xFFFFFFFF,DWORD dwFontFlag = 0);
	bool IsEmpty(){return (m_Tips.size() == 0);}

	void CalSize();
protected:


	struct TipElement
	{
		TipElement()
		{
			strText.assign("");
			dwPicID = 0;		
			iX = 0;
			iY = 0;
			iWidth = 0;
			iHeight = 0;
			iFont = FONT_DEFAULT;
			iFontSize = FONTSIZE_DEFAULT;
			dwFontFlag = 0;
			dwColor = -1;		
			dwBackColor = 0;
			dwFrameColor = 0xFF000000;
			iAlignType = XAL_TOPLEFT;
			bNewLineNext = true;
			iRenderMode = RM_ALPHA;
		}

		TipElement(string str,DWORD id,int x,int y,int width,int height,int font = FONT_DEFAULT,int fontsize = FONTSIZE_DEFAULT,DWORD fontflag = 0,DWORD color = -1,int ialigntype = XAL_TOPLEFT,bool bnewlinenext = true,DWORD backcolor = 0,DWORD framecolor = 0xFF000000,int irm = RM_ALPHA)
		{
			strText = str;
			dwPicID = id;		
			iX = x;
			iY = y;
			iWidth = width;
			iHeight = height;
			iFont = font;
			iFontSize = fontsize;
			dwFontFlag = fontflag;
			dwColor = color;		
			iAlignType = ialigntype;
			bNewLineNext = bnewlinenext;
			dwBackColor = backcolor;
			dwFrameColor = framecolor;
			iRenderMode = irm;
		}

		string strText;
		int iX,iY;//原点坐标
		int iWidth,iHeight;//宽度和高度
		DWORD dwPicID;//图片编号,如果为0表示是文字信息
		int iAlignType;//对齐方式,XALINEMENT之一
		int iFont;//字体
		int iFontSize;//字号
		DWORD dwFontFlag;//文字绘制标记,eDrawTextFlag之一
		DWORD dwColor;//颜色
		DWORD dwBackColor;//底纹的颜色
		DWORD dwFrameColor;//描边的颜色
		bool bNewLineNext;//下一个元素是否为新的一行
		int iRenderMode;//绘制模式
	};

	vector<TipElement> m_Tips;


	DWORD m_dwFontFlag;//文字绘制标记,eDrawTextFlag之一
	bool m_bAutoAdjust;        //自动调整
	int m_iMaxWidth,m_iMaxHeight;//目前控件的最大宽度和高度
	int m_iLastCurLineX,m_iLastCurLineY;//当前行位置
	int m_iCurLineMaxHeight;//当前行最大高度

};