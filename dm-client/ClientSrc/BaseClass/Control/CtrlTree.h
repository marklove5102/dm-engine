#pragma once
#include "Control.h"
#include "CtrlButton.h"
#include "CtrlScroll.h"
#include "ParserTip.h"

//树结点
class CTreeNode;
typedef vector<CTreeNode*> ChildrenArray;
class CCtrlTree;
class CTreeNode
{
public:
	CTreeNode(CCtrlTree* pTree,CTreeNode* pParent,DWORD dwID,LPCTSTR szText,bool bLeaf = false,bool bShow = true,bool bFavorite = false,bool bOfficer = false);
	~CTreeNode();
public:
	void InsertChildNode(CTreeNode* pNode);
	bool DeleteNode(CTreeNode* pTreeNode);
	void DeleteAllNode();
	CTreeNode* GetParent(){ return m_pParent;}
	void SetID(DWORD dwID){ m_dwID = dwID;}
	void SetText(LPCTSTR szText){ m_strText = szText;}	
	CTreeNode* FindNode(DWORD dwID);
	CTreeNode* FindNode(CCtrlButton* pButton);
	CTreeNode* FindNode(LPCTSTR szText);
	ChildrenArray* GetSiblingArray();
	ChildrenArray& GetChildrenArray(){ return m_vChildren;}
	CCtrlButton*   GetButton(){ return m_pButton;}

	bool IsLeaf(){ return m_bLeaf;}
	bool IsUnBind(){return m_bUnBind;}
	void OnClickButton(bool bShow = true);
	void SetUnBind(bool bUnBind=true);
	void Draw(int iX,int iY,int iCurLevel,int& iClickX,int& iClickY);
	void DrawTipInfo(int iX,int iY,string& strTip);
	int  GetNodeRows();

	void SetColor(DWORD dwColorStile,DWORD dwContent);
	static void SetMaxRowAndCols(WORD wRows,WORD wClos);
	static void SetRowColRel(WORD wRowHeigh,WORD wColWidth,WORD wIndent);
	static void DrawIni(WORD wShowFirstRow);
	static void SetShowTips(bool bShow){m_sbShowTips = bShow;}
protected:
	bool IsInShowSpace();
protected:
	DWORD			m_dwID;
	string			m_strText;
	ChildrenArray	m_vChildren;
	CTreeNode*		m_pParent;
	bool			m_bUnBind;
	bool			m_bLeaf;
	bool			m_bFavorite;
	bool			m_bOfficer;
	CCtrlButton*	m_pButton;
	CCtrlTree*		m_pTree;
	
	DWORD	m_dwColorTile;
	DWORD	m_dwColorContent;

	static WORD		m_swContentCols;//每一行叶子结点分几列显示
	static WORD		m_swDrawRows;//Draw时用来计数当前绘了多少行
	static WORD		m_swDrawMaxRows;//最大可显示行数
	static WORD		m_swFirstRow;//显示的第一行
	static WORD		m_swRowHeigh;//行高
	static WORD		m_swIndent;//缩进
	static WORD		m_swColWidth;//列宽
	static DWORD	m_sdwRow;//Draw走过时当前处理的行//每次Draw开始时清0
	static bool		m_sbShowTips;	
};

class CCtrlTree :public CControlContainer
{
	DTI_DECLARE(CCtrlTree, CControlContainer)
public:

	CCtrlTree(void);
	~CCtrlTree(void);

	virtual bool Create(CControl * pParent,int l,int t,int r,int b,int lines,int icolom = 4,float h = 16.0f);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw(int& iClickX,int& iClickY);
	virtual bool OnMouseMove(int iX,int iY);			// 鼠标移动消息
	virtual bool OnWheel(int iWheel);					// 滚轮消息
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);

	void AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex);
	void AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx);
	void SetRootNode(CTreeNode* pRoot){m_pRoot = pRoot;}//构造一个树要手工制作
	CTreeNode* GetRootNode(){ return m_pRoot;}
	void RefreshTreeRows();

public:
	CParserTip*		m_sTip;
	bool			m_bShowTips;
protected:
	CTreeNode	*   m_pRoot;						//根结点	
	CCtrlButton *	m_pUpButton;					//向上按钮
	CCtrlButton *	m_pDownButton;					//向下按钮
	CCtrlScroll *	m_pScroll;						//滚动条,m_pScroll->GetPos()开始显示的行

	int				m_iLinesPerPage;				//每页最多显示多少行
	int				m_iColoumn;						//每行有几列
	float			m_fLineHeight;					//每行的高度
	int				m_iScrollWidth;					//滚动条宽度
	bool			m_bDrawRoot;					//是否绘制根结点
	int				m_iTreeRows;					//树的行数
};
