#pragma once

#include "Control.h"
#include "XmlControl.h"
#include "CtrlTip.h"
#include "GameData/ObjectLink/MultiLine.h"
#include "CtrlButton.h"
#include "CtrlScroll.h"

class CTalkViewer : public CControlContainer
{
	DTI_DECLARE(CTalkViewer, CControl)
public:
	CTalkViewer();
	~CTalkViewer(void);

	virtual void OnReSize();
	virtual void Draw();
	virtual bool OnWheel(int iWheel);
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);

	void SetDisLine(int i) { m_iDisLine = i; }
	int  GetDisLine()  {  return m_iDisLine; }
	void Wheel(int i);
	void WheelPage(int i);
	void WheelToEnd();
	void SetTalkType(TalkType* pTalk){ m_pTalk = pTalk; }
	void SetAutoWheel(bool b) { m_bAutoWheel = b; }
	bool IsAutoWheel()        { return m_bAutoWheel; }
	const char* GetLine(int x,int y);

	void AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex,int iAlignType = XAL_TOPLEFT);
	void AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex,int iAlignType = XAL_TOPLEFT);
	void AddSwitchButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex,int iAlignType = XAL_TOPLEFT);
	void AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx,int iAlignType = XAL_TOPLEFT);
	void AddScrollEx(int iX,int iY,int iW,int iH,bool bShow = true,int iStep = -1,int iAlignType = XAL_TOPLEFT);//iStep:-1表示用原来默认的
	bool getLineHeader(std::string& strLine);

	void SetBigTalkBack(int iIdx){ m_iBigTalkBack = iIdx; }
	void SwitchTalkViewer();
	void SetLineHeight(int iLineHeight){ m_iLineHeight = iLineHeight;}//在create之前调用
	void GetDrawOffXY(int &iX,int &iY){iX = m_iDrawOffX;iY = m_iDrawOffY;}
	void SetDrawOffXY(int iX,int iY){m_iDrawOffX = iX;m_iDrawOffY = iY;}
	void SetLineCount(int iLins){m_iLineCount = iLins;}
	int  GetLineCount(){return m_iLineCount;}

	const char* GetCommand(){return m_strCommand.c_str();}
	BOOL IsBigTalkVier(){return m_bBigTalkViewer;}

protected:
	void CopyToClipboard();

	void DrawStringLine(CStringLine& line);
	void DrawStringLine(CStringLine& line,int begin,int end);

	int getLineByXY(int x,int y,int & col);
	CGood* getGoodByXY(int x,int y);
	void getSelection(int& rowBegin,int & colBegin,int & rowEnd,int & colEnd);

	bool IsSelected();

	int  m_iDisLine;        //开始行号
	int  m_iLineCount;      //显示行数
	int  m_iLineHeight;
	int  m_iTotalCount;
	int  m_iSelBeginRow,m_iSelBeginCol;
	int  m_iSelEndRow,m_iSelEndCol;
	int  m_iDrawOffX,m_iDrawOffY;
	bool m_bSelecting;
	bool m_bAutoWheel;
	CGood* m_pFocusGood;

	TalkType* m_pTalk;
	CCtrlScroll* m_pScroll;
	CCtrlButton* m_pUpButton;
	CCtrlButton* m_pDownButton;
	CCtrlButton* m_pSwitchButton;

	BOOL         m_bBigTalkViewer;
	int          m_iBigTalkBack;
	string       m_strCommand;
};
