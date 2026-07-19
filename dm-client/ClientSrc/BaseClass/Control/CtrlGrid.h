#pragma once

#include "Global/Interface/TextureInterface.h"
#include "Control.h"
#include "CtrlScroll.h"
#include "CtrlButton.h"
#include <vector>
#include <string>

class CCtrlGrid;

enum ECridLineNumType
{
	ECT_NONE = 0     ,//不显示行号
	ECT_NUMBER       ,//以数字方式显示
	ECT_PIC          ,//以图片方式显示
};

class CCtrlGrid : public CControlContainer
{
	DTI_DECLARE(CCtrlGrid, CControlContainer)
public:
	struct stColInfo
	{
		stColInfo(int x,int w)
		{
			iX = x, iWidth = w;
		}
		int iX;
		int iWidth;
	};

	CCtrlGrid(void);
	~CCtrlGrid(void);

	virtual bool Create(CControl * pParent,int l,int t,int r,int b,int lines,float h = 16.0f);
	virtual bool CreateEx(CControl * pParent,int l,int t,int r,int b,int lines,float h = 16.0f,WORD wSelectedTex = 0,int iDev = 0,float fRatio = 1.0);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw(void);
	virtual void OnCreate(void);
	virtual bool OnMouseMove(int iX,int iY);			// 鼠标移动消息
	virtual bool OnWheel(int iWheel);					// 滚轮消息
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnRightButtonDown(int iX,int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);

	void  DrawGridEx(int row,int col,const char* str,bool bCenter = false,WORD wBackTex = 0,WORD wBotTex = 0,DWORD dwColor = 0,DWORD dwFlag = 0,int iDevX = 0,int iDevY = 0);////dwColor:如果外面传了颜色过来以传过来的为准，因为有可能单格要用特殊的颜色,dwFlag取eDrawTextFlag组合
	void  DrawGrid(int row,int col,const char* str,bool bCenter = false,DWORD dwColor = 0,DWORD dwFlag = 0,int iDevX = 0,int iDevY = 0);////dwColor:如果外面传了颜色过来以传过来的为准，因为有可能单格要用特殊的颜色,dwFlag取eDrawTextFlag组合
	void  DrawGrid(int row,int col,int iPackage,WORD wTex,bool bCenter = false,int iOffX = 0,int iOffY = 0);
	bool  GetGridXY(int row,int col,int& x,int& y);

	int	  GetHotLine(){return m_iHotLine;}
	int   GetDisLine(){return m_pScroll?m_pScroll->GetPos():0;}
	int   GetTotalCount(){return m_iTotalCount;}
	int   GetSelLine(){return m_iSelLine;}
	int   GetSelCol(){return m_iSelCol;}
	int   GetLinesPerPage(){return m_iLinesPerPage;}
	bool  IsDrawHotLineEffect(){return m_bDrawHotLineEffect;}
	int   GetScrollPos(){return m_pScroll?m_pScroll->GetPos():0;}
	bool  GetSelAllCols(){return m_bSelAllCols;}

	void  SetTextColor(DWORD color,DWORD hotcolor){m_dwTextColor = color,m_dwHotColor = hotcolor;}
	void  SetBackColor(DWORD color){m_dwBackColor = color;}
	void  SetLineNumType(ECridLineNumType lineNumType){m_iLineNumType = lineNumType;}
	void  SetSelTextColor(DWORD color){m_dwSelTextColor = color;}
	void  SetSelBackColor(DWORD color){m_dwSelBackColor = color;}
	void  SetDrawHotLineEffect(bool b){m_bDrawHotLineEffect = b;}
	void  SetLinesPerPage(int i){m_iLinesPerPage = i;}
	void  SetDrawOffXY(int iX,int iY){m_iDrawOffX = iX;m_iDrawOffY = iY;}
	void  SetSelAllCols(bool b){m_bSelAllCols = b;}

	bool IsDrawSelectedFrame(){return m_bDrawSelectedFrame;}
	void SetDrawSelectedFrame(bool b){m_bDrawSelectedFrame = b;}

	void  SetSelLine(int iLine);
	void  SetTotalCount(int i);
	void  SetDisLine(int i);
	void  SetScrollPos(int i);

	void  SetScrollWidth(int iW){m_iScrollWidth = iW;}
	int   GetScrollWidth(){return m_iScrollWidth;}

	void AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx,bool bShow = true);
	void AddScrollEx(int iX,int iY,int iW,int iH,bool bShow = true,int iStep = -1);//iStep:-1表示用原来默认的

	void  ClearColumn();
	void  PushColumn(int w);

	void  MoveAsNeed(int iPos);
protected:

	std::vector<stColInfo> m_vColInfo;//列信息表
	CCtrlButton * m_pUpButton;//向上按钮
	CCtrlButton * m_pDownButton;//向下按钮
	CCtrlScroll * m_pScroll; //滚动条,m_pScroll->GetPos()开始显示的行

	int   m_iLinesPerPage;//每页最多显示多少行
	float m_fLineHeight;//每行的高度
	int   m_iTotalCount;//总共的行数
	int   m_iHotLine;//鼠标指向的行
	int   m_iSelLine,m_iSelCol;//选中的行，列
	int   m_iScrollWidth;//滚动条的宽度
	DWORD m_dwHotColor,m_dwTextColor,m_dwSelTextColor;//指向、普通，选中的行，列的文本颜色
	DWORD m_dwSelBackColor,m_dwHotBackColor,m_dwBackColor;//指向、普通，选中的行，列的文本的背景颜色
	bool  m_bDrawHotLineEffect;//是否画选中行效果
	bool  m_bDrawSelectedFrame;
	bool  m_bCenter;//是否居中对齐
	bool  m_bSelAllCols;//是否选中整行，否则可以选择一行的某列,如果选中的是第一列那么整行都有选中效果[如果要求第一列可以单独选择可以把第一列宽度置为0,把第二列当第一列]
	int   m_iDrawOffX,m_iDrawOffY;//绘制的起始位置，因为加了按钮的原因，绘制Y不能从0开始
	WORD  m_wSelectTex;
	int	  m_iDevSelectedTex;
	float m_fRatio;


	ECridLineNumType m_iLineNumType;//显示行号的类型
	void Click(int iX,int iY);//左键或右键在iX,iY处点击一下
	bool  m_bNewVersion;//新版本自带滚动条控制
};
