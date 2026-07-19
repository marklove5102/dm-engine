#pragma once

#include "control.h"
#include "CtrlScroll.h"
#include "CtrlButton.h"

#include <string>
#include <vector>

class CCtrlMultiEdit:public CControlContainer
{
DTI_DECLARE(CCtrlMultiEdit, CControl)
public:
	CCtrlMultiEdit(int iMaxSize = 12288,int iRowSize = 80, int iRows = 20, bool bCanEdit = true,int iRowHeigh = 12);
	~CCtrlMultiEdit(void);

	virtual bool Create(CControl * pParent, int iFontSize, int iX, int iY, int iW, int iH,bool bNewVersion = false);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnChar(UCHAR cChar);
	virtual bool OnWheel(int iWheel);
	virtual void Draw(void);
	virtual const char * GetText(void);							// 取得字符串
	virtual void OnCreate(void);

	void	SetDisLine(int iPos);
	int		GetDisRows();					//取得屏幕可显示的行数
	int		GetDisLine();
	void	SetEditable(bool b);
	bool	IsEditable();
	void	SetNoFocus(bool bNoFocus);

	int		GetLines(void);							// 取得行数			
	void	SetText(const char * sText,int iFont = FONT_DEFAULT,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,int iOffX = 0,int iOffY = 0,DWORD dwColor = 0xFFFFFFFF,DWORD dwSelectColor = 0xFFFFFFFF,DWORD dwSelectBackColor = 0xFF0000FF,DWORD dwDisableColor = 0xFFFFFFFF,int iGap = 0);			// 设置字符串
	void	SetDisplayRows(int iDisRows);			// 设置可显示的行数
	void	SetHighColor(DWORD dwHighColor);		// 设置高亮的颜色
	void	GetLineText(char * sText,int iLine);	// 取得某行的字符串
	bool	IsChanged(void);						// 是否修改过了
	void    InsertStr(int iPos, char * sText);		//在m_sText的某个位置添加字符串
	int     GetCursorPos(){return m_iCursor;}//取得当前光标所在位置
	void    SetCursorPos(int iPos){m_iCursor = iPos;}//设置当前光标所在位置
	void    SetSelRange(int iBegin,int iEnd);//设置选中的范围
	int     GetCursorLine(void);//取得当前光标所在的位置
	void    SetLineHeight(int i){m_iLineHeight = i;}
	void    SetScroll(CCtrlScroll* pScroll){m_pScroll = pScroll;}

	void AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx);
	void AddScrollEx(int iX,int iY,int iW,int iH,bool bShow = true,int iStep = -1);//iStep:-1表示用原来默认的

private:
	void OnUp(void);							//Up键
	void OnDown(void);							//Down键
	void OnLeft(void);							//<-键
	void OnRight(void);							//->键
	void OnBack(void);							//Back键
	void OnDel(void);							//Del键
	void OnHome(void);							//Home键
	void OnEnd(void);							//End键
	void OnCopy(void);							//拷贝
	void OnPaste(void);							//粘贴
	void OnCut(void);							//剪切
	void OnSelectAll(void);						//全部选中

	void DrawHighArea(int iLine,int ix,int iy);				//绘制高亮选中区
	bool InsertChar(UCHAR cChar);							//添加一个字符
	void UpdateLineData(void);								//刷新行数据
	void UpdateSelect(void);								//刷新选中
	bool CursorInLine(int iLine);							//判断光标是否在某行
	bool Copy(int iBegin, int iEnd);						//把选中区域字符串放入系统粘贴板
	void DelStr(int iPos, int iNum);						//在m_sText的某个位置删除字符串
	void ClearLineData(void);								//清空行信息
	void GetMousePos(int& x, int& y, int iX, int iY);		//取得鼠标所在行和列
private:
	struct stLine			// 行数据
	{
		int		iBPos;		// 行首位置
		int		iEPos;		// 行尾位置
		BYTE	cEndType;	// 行结尾类型:0--字符串结束 1--硬换行 2--软换行
	};

	CCtrlButton * m_pUpButton;//向上按钮
	CCtrlButton * m_pDownButton;//向下按钮
	CCtrlScroll * m_pScroll; //滚动条,m_pScroll->GetPos()开始显示的行

	std::string			m_sEditText;		// 编辑框字符串
	std::vector<BYTE>	m_ArrayAsc;			// 字符对照表
	std::vector<stLine> m_ArrayLine;		// 每行行数据

	// 需要初始化设置
	const int	m_iMaxSize;		// 最大可保存字符的字节数	
	const int	m_iRowSize;		// 每行的字节数
	bool		m_bCanEdit;		// 可否编辑
	int			m_iDisRows;		// 屏幕可显示的行数
	int         m_iLineHeight;      // 每行高度

	// 编辑框当前状态数据
	int		m_iCursor;			// 光标位置
	int		m_iLines;			// 当前行数
	int		m_iDisLine;			// 当前显示开始行计数
	DWORD	m_dwHighColor;		// 高亮颜色

	// 选中状态数据
	bool	m_bSelecting;		// 是否正在选中态
	int		m_iSelBegin;		// 选中开始位置
	int		m_iSelEnd;			// 选中结束位置

	// 内部使用的临时数据
	stLine	m_stLineTemp;
	bool	m_bAsc;
	bool	m_bChanged;
	bool	m_bNewVersion;
};
