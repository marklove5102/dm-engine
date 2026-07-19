#pragma once

#include "CtrlEdit.h"
#include "GameData/ObjectLink/StringLine.h"

#define MAX_HISTORY  50

class CGlyphEdit :	public  CCtrlEdit
{
	DTI_DECLARE(CGlyphEdit, CCtrlEdit)
public:
	CGlyphEdit(void);
	virtual ~CGlyphEdit(void);

	CStringLine* getLine();
	void DisableFocus(void);
	bool InsertObjectLink(CObjectLink * lpLink);
	void SetFakeFocus(bool b){m_bFakeFocus = b;}
	bool IsFakeFocus(){return m_bFakeFocus;}

	virtual void Draw(void);
	virtual void  Clear();
	virtual const char *GetText();
	virtual void SetText(const char * sText);

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
private:
	void insertGlyph(const char * str, UINT len);
	UINT getGlyphByteLength();
	bool insertGlyph(int pos,CGlyph * lpGlyph);
	void RefreshSelect();
	//void ClearHistory();
	void ReSetShowPos();

private:
	//int		m_iHistorys;		//历史纪录相关----当前历史纪录的条数
	int		m_iDisNow;			//历史纪录相关----当前的显示位置
	//std::deque<CStringLine*> m_HistoryLines; //记录保存地址

	DWORD	m_dwLastActionTime;			 // 最近的输入动作时间
	int		m_iShowPos;	// 开始显示的位置
	BYTE	mTemp;
	bool    m_bFakeFocus;//输入框的假焦点,在按回车时如果输入框获得焦点失败将获得一个假焦点,有光标,可以输入内容,但不能Ctrl+C ,Ctrl+V等
    CStringLine * m_line;
};