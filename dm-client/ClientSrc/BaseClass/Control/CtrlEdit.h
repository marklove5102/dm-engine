
#pragma once

#include "Control.h"
#include "XmlControl.h"

#define MAX_EDIT_WORDS 512

class CCtrlEdit : public CControl
{
	DTI_DECLARE(CCtrlEdit, CControl)
public:
	CCtrlEdit(void);
	~CCtrlEdit(void);
	virtual bool Create(CControl * pParent,int iFontSize,DWORD dwColor,int iX,int iY,int iW=-1,int iH=-1,UINT uStyle= 0);
	virtual bool CreateEx(CControl * pParent,int iX,int iY,int iW=-1,int iH=-1,UINT uStyle= 0,WORD wTex = 0,WORD wMTex = 0,WORD wCTex = 0,WORD wDTex = 0);
    virtual bool CreateXML(CControl* pParent,CXmlEdit* edit);
	virtual void Draw(void);
	virtual void SetWidth(int w);
	virtual void SetHeight(int h);

	void  SetPwd(bool b)  { m_bPwd = b; }                  // 设置是否为密码框
	bool  IsPwd()         { return m_bPwd;}                // 是否是密码框

	const char* GetPrompt(){ return m_sPrompt.c_str(); }
	void  SetPrompt(const char* str){ m_sPrompt.assign(str); }
	virtual void  Clear();
	virtual const char *GetText() { return m_sText.c_str();} 
	virtual void SetText(const char * sText,DWORD dwColor = 0xFFFFFFFF,DWORD dwMouseOnColor = 0xFFFFFFFF,DWORD dwClickColor = 0xFFFFFFFF,DWORD dwDisableColor = 0xFFFFFFFF,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,int iFont = FONT_DEFAULT,int iOffX = 2,int iOffY = 0,int iGap = 0);

	int   GetNum(){  return atoi(m_sText.c_str()); }

	void SetMaxLength(int iLength);                        // 设置最大显示字符(一般小于m_iVisualWords)
	void InsertText(char* sText);

	void	SetDigital(bool bDigital);
	void    SetVisualWords(int iWords);
	void    SetCursorColor(DWORD dwColor);

	// 消息响应函数
	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	virtual bool OnChar(UCHAR cChar);

	virtual void OnCopy(void);     // 拷贝
	virtual void OnEscape(void);     // 拷贝
	virtual void OnCut(void);      // 剪切
	virtual void OnPaste(void);    // 粘贴
	virtual void OnEntry(void);    // 回车响应
	virtual void OnLeft(void);     // <-键响应
	virtual void OnRight(void);    // ->键响应
	virtual void OnDelete(void);   // Del键响应
	virtual void OnBack(void);     // Back键响应
	virtual void OnHome(void);     // Home键响应
	virtual void OnEnd(void);      // End键响应
	virtual void OnSelectAll(void); // 全选
	virtual bool SetFocus(void);
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnUp(void);
	virtual bool OnDown(void);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual bool IsFocus(void);
	virtual void OnSetFocus(void);

protected:
	int    m_iDisplay;                                     // 当前开始显示位置的索引
	int    m_iSelBegin,m_iSelEnd;                          // 选中区域指针索引
	int    m_iMaxLen;                                      // 最大输出字符个数
	int    m_iVisualWords;                                 // 可见字个数
	int    m_iCursor;                                      // 光标位置
	bool   m_bPwd;  	                                   // 是否是密码框标示
	bool   m_bSelected;                                    // 是否在选中态
	bool   m_bDigital;									   // 是否是仅支持数字输入
	DWORD  m_dwCursorColor;
	string m_sPrompt;                                    // 提示信息
	WORD   m_vTexID[4];

	DWORD       m_dwMouseColor;
	DWORD       m_dwClickColor;
	DWORD       m_dwDisableColor;

	bool CanStay(int iPos);
	bool CanStayCursor();
	bool CanStayDisplay();
	void DelChar(int iPos, int num = 1);
	void InsertChar(int iPos, char cChar);
	void InsertStr(int iPos, char* sSrc);
	bool CopyToClipboard(int iBegin, int iEnd);
	void RefreshSelect();
};
