#pragma once

#include "control.h"


class CCtrlScroll : public CControlContainer
{
DTI_DECLARE(CCtrlScroll, CControl)
private:
	int m_iBarW,m_iBarH;                  // 滚动块的大小
	float m_fBarX,m_fBarY;                  // 滚动块相对于滚动控件的相对坐标
	int m_iButtonHeight;                  // 滚动控件2个按钮的高度
	int m_iScrollRange;                   // 滚动块可移动的范围大小

	int m_iLastMousePosX, m_iLastMousePosY; //用于拖动滑块时记录拖动前鼠标位置

    int m_iRange;                         // 滚动最大值，完整内容大小
	int m_iDisplayRange;                  // 能显示的内容大小，与完整内容大小、滑块可移动范围一起用来计算滑块大小

	int m_iStep;                          // 每步滚动值
	int m_iPos;                           // 当前Pos值

	bool m_bMoving;                       // 鼠标移动时滚动块是否应该滚动
	bool m_bVertical;
	
	//LPTexture m_pScrollTex;
	WORD    m_wScrollTexID;
	int		m_iCurColor;
	int     m_iIndex;                     // 背景索引

	CCtrlButton * m_pUpButton;//向上按钮
	CCtrlButton * m_pDownButton;//向下按钮
	bool m_bNewVersion;//新版本的滚动条自带按钮,背景,自动缩放,为了兼容,老版也还可以使用
	static WORD m_wUpBtnTex[BTEX_NUMS],m_wDownBtnTex[BTEX_NUMS],m_wBackTex[BTEX_NUMS],m_wBarTex;//滚条用到的纹理

public:
	CCtrlScroll();
	~CCtrlScroll(void);

	void SetRange(int iRange, int iDisplayRange = -1);   //iDisplayRange为-1则不修改滑块大小，为0则使用默认大小（控件高度/14），为其它值将重新计算滑块高度
	int  GetRange()                {  return m_iRange;   }
	void SetStep(int iStep)        {  m_iStep = iStep;   }
	int  GetStep()                 {  return m_iStep;    }
	void SetPos(int iPos);
	int  GetPos();
	void SetVertical(bool b)		{ m_bVertical = b;}

	virtual bool Create(CControl * pParent,int iX,int iY,int iW,int iH,int iScrollIdx = 0);
	virtual bool CreateXML(CControl* pParent,CXmlScroll* scroll);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
 	virtual void Draw(void);
	virtual void OnCreate(void);
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool ReSize(int iW, int iH);
	
	//下面两个函数只对m_bNewVersion为true的有效
	//允许设置滚动条的纹理,使用static保存,包括在任务其它控件中使用的滚动条都会改变,使用完了以后请再调用一次复原.不想每个用到SetScrollTex的控件都要提供类似接口,所以这里用static
	virtual void SetScrollTex(WORD wUpTex = 1026,WORD wUpMTex = 1027,WORD wUpCTex = 1028,WORD wUpDTex = 1029,WORD wDownTex = 1030,WORD wDownMTex = 1031,WORD wDownCTex = 1032,WORD wDownDTex = 1033,WORD wBarTex = 1034,WORD wBarMTex = 1035,WORD wBarCTex = 1036,WORD wBarDTex = 1037,WORD wBackTex = 1025);
	virtual bool CreateEx(CControl * pParent,int iX,int iY,int iW,int iH);//iw,ih包含上下按钮
		 


	void GoUp(void);
	void GoDown(void);
	virtual bool OnWheel(int iWheel);
	void SetButtonHeight(int iHeight);
	void ReloadScrollTex(WORD wTex);
};
