#pragma once

#include "Control.h"
#include "GameData/TagText.h"
#include "CtrlScroll.h"
#include "CtrlButton.h"

class CMarkViewer : public CControlContainer
{
	DTI_DECLARE(CMarkViewer, CControlContainer)
public:
	CMarkViewer(void){}
	CMarkViewer(float fLineHeight,int iLineCount,int iOffX = 0,int iOffY = 0,bool bNewVersion = false);
	~CMarkViewer(void);    
    virtual void Draw();
    void DrawEx();      //绘制豪华NPC对话框
    void SetDisLine(int iDisLine);
    void SetDefaultColor(DWORD dwColor);
    void SetTagText(CTagText* tag);   
    virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
    void DoCommand();
	const string& GetCommand(){return m_strCommand;}//返回点击链接的命令
    void SetDrawType(int iType) {m_iDrawType = iType;}
	virtual bool OnWheel(int iWheel);					// 滚轮消息
	virtual void OnCreate(void);

	void AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx);
	void AddScrollEx(int iX,int iY,int iW,int iH);
	void SetScrollPos(int i);
	void SetDrawOffXY(int iX,int iY){m_iDrawOffX = iX;m_iDrawOffY = iY;}
	void SetScrollStep(int iScrollLines);
	int  GetScrollStep();

	void SetScrollShow(bool show);


protected:
	CCtrlButton * m_pUpButton;//向上按钮
	CCtrlButton * m_pDownButton;//向下按钮
	CCtrlScroll * m_pScroll; //滚动条,m_pScroll->GetPos()开始显示的行


private:
    void DrawGoodLooks(int iPrintX,int iPrintY,LPTexture pTex);   //绘制物品图像

    CTagText* m_TagText;
    string m_strCommand;        //点击链接的命令
    int   m_iDisLine;           //当前显示页的第一行的真正行数
    float m_fLineHeight;              
    int   m_iLineCount;         //一页中可以显示的行数
    DWORD m_dwDefaultColor;     //普通文字的显示颜色    
    int   m_iDrawType;          //绘制文字的方式，0为普通绘制，1为绘制豪华NPC对话框
	int   m_iDrawOffX,m_iDrawOffY;//绘制的起始位置，因为加了按钮的原因，绘制Y不能从0开始
	bool  m_bNewVersion;//新版本自带滚动条控制
	int   m_iScrollStep;//一次滚动多少行

};
