#pragma once

#include "CtrlButton.h"
#include "CtrlScroll.h"


class CCtrlMenuButton : public CControlContainer
{
public:
	CCtrlMenuButton();
	~CCtrlMenuButton(void);
public:
	virtual bool Create(CControl* pParent,int iX,int iY,int iW,int iH,WORD wMenuBackTex = 0,WORD wTex = 39,WORD wMTex = 39,WORD wCTex = 39,WORD wDTex = 39,WORD wBtnTex = 1030,WORD wBtnMTex = 1031,WORD wBtnCTex = 1032,WORD wBtnDTex = 1033,int iMenuOffX = 1,int iMenuOffY = 18,bool bColor = false,int iBtnOffX = 0,int iBtnOffY = 0);//iBtnOffXY:相对于右上角
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual void Draw();            //绘制按钮
	virtual void OnCreate();
	virtual bool OnWheel(int iWheel);
	virtual void DrawMenu();        //绘制菜单
	virtual bool KillFocus(CControl * pNewFocus);

	void AddString(const char* strStr,bool bEnable = true);
	void InsertString(const char* strStr);
	void ResetContent();            //清空菜单内容
	int  GetCurSel();
	void SetCurSel(int i);
	void SetCurSel(const string &strSelText);
	const char*  GetSelText();
	DWORD GetSelColor();
	void SetMenuTextColor(DWORD dwColor);
	void SetSelColor(DWORD dwSelTextColor,DWORD dwSelColor);
	void ChangeString(int i,const char* strStr);
	void ChangeEnable(int i,bool bEnable);

	//选择颜色的菜单
	void SetVAlignTop(bool b){  m_bVAlignTop = b;  }
	bool IsMenuDown();

	//settext请在create后再调用
	virtual void SetText(const char * sText,DWORD dwColor = COLOR_BTN_NORMAL,DWORD dwMouseOnColor = COLOR_BTN_NORMAL,DWORD dwClickColor = COLOR_BTN_NORMAL,DWORD dwDisableColor = COLOR_BTN_NORMAL,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,int iFont = FONT_YAHEI,bool bVertical = false,int iOffX = 0,int iOffY = 0,int iGap = 0);
	//SetFont请在create之前调用
	virtual void SetFont(int iFont = -1,int iFontSize = -1,DWORD dwFontFlag = 0);

private:
	bool    m_bMenu;                //菜单是否弹出
	std::vector<std::string> m_vecCNT;
	std::vector<bool> m_vEnable;
	int     m_iOffX,m_iOffY;        //菜单的偏移量
	int     m_iCurSel;              //当前选中菜单项
	int     m_iMouseOnLine;         //鼠标指向的行
	int     m_iItemHeight;          //每行文字的高度
	DWORD   m_dwMenuTextColor;      //菜单文字颜色
	DWORD   m_dwMenuSelTextColor;   //菜单选择文字颜色
	DWORD   m_dwMenuSelBackColor;   //选择的文字底色
	int     m_iBeginPos;            //菜单内容开始行
	int     m_iDisplayLines;        //菜单显示的行数
	bool    m_bVAlignTop;           //垂直方向往上面弹出菜单
	bool    m_bColor;               //颜色选择菜单
	int     m_iButtonWidth;         //右边按钮宽度
	int     m_iParentW,m_iParentH;  //父窗口的大小,弹出来要改变,不然接收不到事件,关闭再恢复
	UCHAR * m_pParentMask;          //父窗口的mask,弹出来要改变,不然接收不到事件,关闭再恢复
	int    m_iBtnOffX,m_iBtnOffY;

	WORD    m_vBackTexID[BTEX_NUMS];
	WORD    m_vBtnTexID[BTEX_NUMS];
	WORD    m_wMenuBackTex;

	CCtrlButton *m_pButton;
	//滚动条
	CCtrlScroll*  m_pScroll;

	bool    IsInMenuArea(int iX,int iY);   //是否在整个菜单区域
	int     GetMouseLine(int iX,int iY);   //得到鼠标所在的Item
	void    OpenOrCloseMenu(bool bOpen);

};
